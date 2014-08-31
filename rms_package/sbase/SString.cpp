 /**
 *
 * 文 件 名 : SString.cpp
 * 创建日期 : 2006-09-27
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:35 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : a string class just like QString
 * 修改记录 : 
 *            $Log: SString.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:35  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/04/29 08:53:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/23 10:53:52  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/12 01:35:30  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/11 12:08:36  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/02/28 01:28:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/12/20 08:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:44  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.5  2009/05/19 02:50:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2009/01/31 08:06:42  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2009/01/31 06:37:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/12/22 08:54:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:03  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2007/12/10 06:40:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.6  2007/10/19 11:57:58  shaokt
 *            增加>/>=/</<=的字符串比较函数
 *
 *            Revision 1.5  2007/09/29 07:27:05  shaokt
 *            警告修正
 *
 *            Revision 1.4  2007/09/29 05:13:35  shaokt
 *            属性操作使用引用方式
 *
 *            Revision 1.3  2007/09/21 07:53:01  shaokt
 *            使用缓冲区重写SString,放弃原来使用的std::string类
 *
 *            Revision 1.2  2007/08/27 09:51:07  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/05/28 07:33:59  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2006/10/19 02:06:41  tester
 *            修改communicate,xml,ini
 *
 **/

#include "SString.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SString::SString()
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString("");
}

SString::SString(int size)
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetBuffer(size+1);
}

SString::SString(const char *pStr)
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString(pStr);
}

SString::SString(const SString &str)
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString(str.m_pStr);
}
SString::SString(const std::string &str)
{
	SKTOBJECT("SString");
	m_pStr = NULL;
	m_iSize = 0;
	m_iStrLen = 0;
	SetString(str.c_str());
}
SString::~SString()
{
	if(m_pStr != NULL)
		delete[] m_pStr;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置缓冲区大小
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:06
// 参数说明:  @size为新的大小
// 返 回 值:  true表示设置成功，false表示新大小小于目前的字符串长度+1时返回错误
//////////////////////////////////////////////////////////////////////////
bool SString::SetBuffer(int size)
{
	if(m_iSize < size)
	{
		char *p = new char[size];
		memset(p,0,size);
		if(m_iSize > 0)
			memcpy(p,m_pStr,m_iSize);
		p[m_iSize]='\0';
		char *p0 = m_pStr;
		m_pStr = p;
		m_iSize = size;
		delete[] p0;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  根据字符串缓冲区长度重新计算字符串大小，在外部修改字符缓冲内容后使用
// 作    者:  邵凯田
// 创建时间:  2013-3-6 9:16
// 参数说明:  void
// 返 回 值:  返回当前字符串真实长度，并修改m_iStrLen
//////////////////////////////////////////////////////////////////////////
int SString::PrepareStrLen()
{
	if(m_pStr == NULL)
		m_iStrLen = 0;
	else
		m_iStrLen = strlen(m_pStr);
	return m_iStrLen;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  收缩字符串，释放多余的缓冲区
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:36
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SString::FreeExtra()
{
	if(m_pStr != NULL)
	{
		int size = m_iStrLen;
		if(m_iSize > size)
		{
			//收缩
			if(size == 0)
			{
				delete[] m_pStr;
				m_pStr = NULL;
				m_iSize= 0;
			}
			else
			{
				char *pStr = new char[size+1];
				pStr[size] = 0;
				memcpy(pStr,m_pStr,size);
				delete[] m_pStr;
				m_pStr = pStr;
			}
		}
	}
}

bool SString::isEmpty() const
{
	return 0 == m_iStrLen ;
}

char* SString::data() const
{
	return m_pStr;
}

char SString::at(int index) const
{
	if(index >= m_iStrLen)
		return '\0';
	return m_pStr[index];	
}

SString SString::substr(int beg, int len)
{
	SString str;
	if(beg<0 || beg>m_iStrLen)
		beg = m_iStrLen;
	if(len == -1 || len > m_iStrLen-beg)
		len = m_iStrLen-beg;

	delete[] str.m_pStr;
	str.m_pStr = new char[len+1];
	str.m_iSize= len+1;
	str.m_iStrLen = len;
	str.m_pStr[len] = '\0';
	if(len > 0)
		memcpy(str.m_pStr,m_pStr+beg,len);
	return str;
}

int SString::length() const
{
	return m_iStrLen;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  填充一个整数到指定长度的字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:24
// 参数说明:  @a为给定的整数值
//            @fieldWidth为期望的字符串长度，>0表示不足是在左边补齐，<0则是在右边补齐
//            @base为进制数，固定为10
//            @fillChar为填充的字符
// 返 回 值:  字符串
//////////////////////////////////////////////////////////////////////////
SString SString::arg(int a, int fieldWidth /*= 0*/, int base /*= 10*/, char fillChar /*= ' '*/)
{
	base=base;	char buf[32];
	memset(buf,0,32);
	sprintf(buf,"%d",a);
	//itoa(a,buf,base);
	SString str(buf);
	if(fieldWidth>0)
	{
		while(m_iStrLen < fieldWidth)
		{
			//*this = fillChar + *this;
			SString s2= fillChar;
			s2 += *this;
			*this = s2;
		}
	}
	else if(fieldWidth<0)
	{
		fieldWidth = -fieldWidth;
		while(m_iStrLen < fieldWidth)
		{
			*this = *this + fillChar;
		}
	}
	return str;
}

void SString::clear()
{
	SetString("");
}

void SString::SetString(const char *pStr,int len/*=-1*/)
{
	if(pStr == NULL)
	{
		SetString("");
		return;
	}
	int size = len==-1?(strlen(pStr)+1):(len+1);
	if(size > m_iSize)
	{
		if(m_pStr != NULL)
			delete[] m_pStr;
		m_pStr = new char[size];
		if(m_pStr == NULL)
		{
			m_pStr = new char[1];
			m_iSize = 1;
			m_iStrLen = 0;
			m_pStr[0] = '\0';
			printf("SString ERROR:alloc bytes=%d return null!",size);
			return ;
		}
		m_iSize = size;
	}
	m_iStrLen = size-1;
	m_pStr[size-1] = '\0';
	if(size > 1)
		memcpy(m_pStr,pStr,size-1);
}

SString &SString::operator=(char c)
{
	char buf[2];
	buf[0] = c;
	buf[1] = '\0';
	SetString(buf);
	return *this;
}

SString &SString::operator=(const SString &s)
{
	SetString(s.m_pStr);
	return *this;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  字符串比较
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:29
// 参数说明:  @被比较的字符串
// 返 回 值:  <0表示当前串小于str,=0表示当前串等于str,>0表示当前串大于str
//////////////////////////////////////////////////////////////////////////
int SString::compare(const SString &str)
{
	return strcmp(m_pStr,str.m_pStr);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  字符串大小写不敏感比较
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:29
// 参数说明:  @被比较的字符串
// 返 回 值:  <0表示当前串小于str,=0表示当前串等于str,>0表示当前串大于str
//////////////////////////////////////////////////////////////////////////
int SString::compareNoCase(const SString &str)
{
#ifdef WIN32
	SString s1=*this;
	SString s2=str;
	_strupr(s1.m_pStr);
	_strupr(s2.m_pStr);
	return strcmp(s1.m_pStr,s2.m_pStr);
#else
	return strcasecmp(m_pStr,str.m_pStr);
#endif
}

int SString::find(char c, int i /*= 0*/, bool cs /*= true*/)//查找字符,cs是否大小写敏感
{
	SString cmp;
	cmp=c;
	return find(cmp,i,cs);
}

int SString::find(SString s, int i /*= -1*/, bool cs /*= true*/)//查找字符串,cs是否大小写敏感
{
	SString s1,s2;
	if(i>m_iStrLen)
		i = m_iStrLen;
	if(cs == false)
	{
		s1=toUpper();
		s2 = s.toUpper();
	}
	else
	{
		s1 = *this;
		s2 = s;
	}
	char *pHexesult = strstr(s1.m_pStr+i,s2.m_pStr);
	if(pHexesult == NULL)
		return -1;
	return pHexesult - s1.m_pStr;
}

int SString::findRev(char c, int i /*= 0*/, bool cs /*= true*/)//反向查找字符,cs是否大小写敏感
{
	SString cmp;
	cmp=c;
	return findRev(cmp,i,cs);
}

int SString::findRev(SString s, int i /*= -1*/, bool cs /*= true*/)//反向查找字符串,cs是否大小写敏感
{
	if(i == -1 || i>m_iStrLen-1)
		i = m_iStrLen-1;
	if(i < 0)
		return -1;

	SString s1,s2;
	if(cs == false)
	{
		s1=toUpper();
		s2 = s1.toUpper();
	}
	else
	{
		s1 = *this;
		s2 = s;
	}
	char *pStrTmp=s1.m_pStr+i;
	for(;pStrTmp!=s1.m_pStr;pStrTmp--)
	{
		char *pHexesult = strstr(pStrTmp,s2.m_pStr);
		if(pHexesult != NULL)
		{
			return (pHexesult - s1.m_pStr);
		}
	}
	return -1;
}

bool SString::operator==(const SString &s)
{
	return (m_iStrLen==s.m_iStrLen) && (compare(s) == 0);
}

bool SString::operator==(const char *s)
{
	return (m_iStrLen==(s==NULL?0:(int)strlen(s))) && (compare(s==NULL?"":s) == 0);
}
//compare
bool SString::operator>(const char *s)
{
	return compare(s==NULL?"":s) > 0;
}
bool SString::operator>=(const char *s)
{
	if(s == NULL)
		return false;
	return compare(s==NULL?"":s) >= 0;
}
bool SString::operator<(const char *s)
{
	if(s == NULL)
		return false;
	return compare(s==NULL?"":s) < 0;
}
bool SString::operator<=(const char *s)
{
	if(s == NULL)
		return false;
	return compare(s==NULL?"":s) <= 0;
}
bool SString::operator>(const SString &s)
{
	return compare(s) > 0;
}
bool SString::operator>=(const SString &s)
{
	return compare(s) >= 0;
}
bool SString::operator<(const SString &s)
{
	return compare(s) < 0;
}
bool SString::operator<=(const SString &s)
{
	return compare(s) <= 0;
}

bool SString::operator!=(const SString &s)
{
	return (m_iStrLen!=s.m_iStrLen) || (compare(s) != 0);
}

bool SString::operator!=(const char *s)
{
	return (m_iStrLen!=(int)strlen(s)) || (compare(s==NULL?"":s) != 0);
}

SString & SString::operator+= ( const SString & other )
{
	int size  = m_iStrLen + other.m_iStrLen+1;
	if(m_iSize < size)
	{
		//空间不足，放大一倍
		SetBuffer(size<<1);
	}
	memcpy(m_pStr+m_iStrLen,other.m_pStr,other.m_iStrLen);
	m_iStrLen += other.m_iStrLen;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString & SString::operator+=( char c )
{
	int size  = m_iStrLen + 2;
	if(m_iSize < size)
	{
		//空间不足，放大一倍
		SetBuffer(size<<1);
	}
	m_pStr[m_iStrLen]=c;
	m_iStrLen ++;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString & SString::operator+=( char *str )
{
	int len = strlen(str);
	int size  = m_iStrLen + len +1;
	if(m_iSize < size)
	{
		//空间不足，放大一倍
		SetBuffer(size<<1);
	}
	memcpy(m_pStr+m_iStrLen,str,len);
	m_iStrLen += len;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString & SString::operator+=(const  char *str )
{
	int len = strlen(str);
	int size  = m_iStrLen + len +1;
	if(m_iSize < size)
	{
		//空间不足，放大一倍
		SetBuffer(size<<1);
	}
	memcpy(m_pStr+m_iStrLen,str,len);
	m_iStrLen += len;
	m_pStr[m_iStrLen] = '\0';
	return *this;
}

SString SString::operator+(const SString &s)
{
	int size  = m_iStrLen + s.m_iStrLen;
	SString str(size);
	if(m_iStrLen>0)
		memcpy(str.m_pStr,m_pStr,m_iStrLen);
	if(s.m_iStrLen>0)
		memcpy(str.m_pStr+m_iStrLen,s.m_pStr,s.m_iStrLen);
	
	str.m_pStr[size] ='\0';
	str.m_iStrLen = size;
	return str;

}

SString SString::operator+(char * s) 
{
	int len = strlen(s);
	int size  = m_iStrLen + len;
	SString str(size);
	if(m_iStrLen>0)
		memcpy(str.m_pStr,m_pStr,m_iStrLen);
	if(len>0)
		memcpy(str.m_pStr+m_iStrLen,s,len);
	
	str.m_pStr[size] ='\0';
	str.m_iStrLen = size;
	return str;
}

SString SString::operator+(char c)
{
	int size  = m_iStrLen + 1;
	SString str(size);
	if(m_iStrLen>0)
		memcpy(str.m_pStr,m_pStr,m_iStrLen);
	str.m_pStr[m_iStrLen] = c;
	str.m_pStr[m_iStrLen+1] = '\0';
	str.m_iStrLen = m_iStrLen+1;
	return str;
}

SString SString::left(int len)
{
	return mid(0,len);
}

SString SString::right(int len)
{
	int i=m_iStrLen-len;
	if(i<0)
		i=0;
	return mid(i,m_iStrLen-i);
}

SString SString::mid(int i, int len /*= -1*/)
{
	return substr(i,len);
}

SString SString::replace ( int pos, int len, const SString & sDesc )
{
	*this = left(pos) + sDesc + right(m_iStrLen - pos - len);
	return *this;
}

SString SString::replace ( int pos, int len, const char * str)
{
	*this = left(pos) + (str==NULL?"":str) + right(m_iStrLen - pos - len);
	return *this;
}

SString SString::replace ( int pos, int len, char c )
{
	*this = left(pos) + c + right(m_iStrLen - pos - len);
	return *this;
}

SString SString::replace ( const SString &before, const SString &after, bool cs/*=true*/)//cs: case sensitive
{
	int pos=0;
	while(1)
	{
		pos=find(before,pos,cs);
		if(pos < 0)
			return *this;
		replace(pos,before.m_iStrLen,after);
		pos += after.m_iStrLen;
	}
	
	return *this;
}

SString SString::replace ( char* before, char *after, bool cs/*=true*/)//cs: case sensitive
{
	SString sb,sa;
	sb = before;
	sa = after;
	return replace(sb,sa,cs);
}
SString& SString::sprintf(const char * cformat, ...)
{
	char buf[10240];
	memset(buf,0,10240);
	va_list ap;
	va_start(ap, cformat);
	vsprintf(buf,cformat,ap);
	va_end(ap);
	*this=buf;
	return *this;
}

SString SString::toUpper()
{
	SString s=*this;
	char *p = (char*)s.m_pStr;
	register int i,cnt=m_iStrLen;
	for(i=0;i<cnt;i++)
	{
		if(p[i] >= 'a' && p[i] <= 'z')
		{
			p[i] = p[i] + ('A' - 'a');
		}
	}
	return s;
}

SString SString::toLower()
{
	SString s=*this;
	char *p = (char*)s.m_pStr;
	register int i,cnt=m_iStrLen;
	for(i=0;i<cnt;i++)
	{
		if(p[i] >= 'A' && p[i] <= 'Z')
		{
			p[i] = p[i] + ('a' - 'A');
		}
	}
	return s;	
}

short SString::toShort(bool * ok)
{
	int v=atoi(m_pStr);
	return (short)v;
}

int SString::toInt(bool * ok)
{
	SString sTemp = *this;
	sTemp = sTemp.toUpper();
	if(sTemp.left(2) == "0X")
	{
		//16进制
		sTemp = sTemp.mid(2);
		int v = 0;
		while(sTemp.m_iStrLen > 0)
		{
			v *= 16;
			char ch = sTemp.at(0);
			if(ch >= 'A' && ch <= 'F')
				v += 10 + ch - 'A';
			else
				v += ch - '0';
			sTemp = sTemp.mid(1);
		}
		return v;
	}
	else
	{
		int v=atoi(m_pStr);
		return v;
	}
}

long SString::toLong(bool * ok)
{
	SString sTemp = *this;
	sTemp = sTemp.toUpper();
	if(sTemp.left(2) == "0X")
	{
		//16进制
		sTemp = sTemp.mid(2);
		int v = 0;
		while(sTemp.m_iStrLen > 0)
		{
			v *= 16;
			char ch = sTemp.at(0);
			if(ch >= 'A' && ch <= 'F')
				v += 10 + ch - 'A';
			else
				v += ch - '0';
			sTemp = sTemp.mid(1);
		}
		return v;
	}
	else
	{
		int v=atol(m_pStr);
		return v;
	}
}

unsigned short SString::toUShort(bool * ok)
{
	return (unsigned short)toShort();
}

unsigned int SString::toUInt(bool * ok)
{
	return (unsigned int)toInt(ok);
}

unsigned long SString::toULong(bool * ok)
{
	return (unsigned long)toLong(ok);
}

float SString::toFloat(bool *ok)
{
	float v=(float)atof(m_pStr);
	return v;
}

double SString::toDouble(bool *ok)
{
	return (double)toFloat(ok);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  将指定的MAC地址字符串转换为6个字节缓冲区(00:00:00:00:00:00或00-00-00-00-00-00)
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:07
// 参数说明:  @str为源字符串形式MAC地址
//            @pHex为MAC地址的存放缓冲区，6字节
// 返 回 值:  true表示转换成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SString::MacStrToHex(SString str,unsigned char *pHex)
{
	if(str.length() != 17)
		return false;
	if(str.find(":") >= 0)
		str = str.replace(":","-");
	str = str.toUpper();
	for(int i2=0;i2<6;i2++)
	{
		SString s2 = SString::GetIdAttribute(i2+1,str,"-");
		if(s2.m_iStrLen > 2)
			s2 = s2.Left(2);
		while(s2.m_iStrLen < 2)
			s2 = "0"+s2;
		SString::StrToHex(s2,&pHex[i2]);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将16进制的MAC地址转换成字符串
// 作    者:  邵凯田
// 创建时间:  2013-2-27 17:38
// 参数说明:  @pMacByte为MAC地址
//            @pHexStr为字符串缓冲区，必须空间大于等于18个字符
//            @ch为间隔符，缺省为':'
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SString::MacHexToStr(BYTE *pMacByte,char *pHexStr,char ch/*=':'*/)
{
	BYTE bHexChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	register int i,pos=0;
	for (i=0;i<6;i++)
	{
		pHexStr[pos] = bHexChar[pMacByte[i]>>4];
		pHexStr[pos+1] = bHexChar[pMacByte[i]&0x0F];
		pHexStr[pos+2] = ch;
		pos +=3;
	}
	pHexStr[17] = '\0';
}


////////////////////////////////////////////////////////////////////////
// 描    述:  将给定的字符串形式的16进制数转换成BYTE缓冲区
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:03
// 参数说明:  @str为16进制表示的数，各字节中间无分隔符，每个字节占两个字符
//            @pHex为目标缓冲区
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SString::StrToHex(char* str,int str_len,unsigned char *pHex)
{
	register int i,times=str_len>>1;
	register BYTE b;
	register char ch;
	for(i=0;i<times;i++)
	{
		ch = *str;
		if(ch>='0' && ch<='9')
			ch -= '0';
		else if(ch >='a' && ch <= 'f')
			ch = ch + 10 - 'a';
		else if(ch >= 'A' && ch <= 'F')
			ch = ch + 10 - 'A';
		else
			ch=0;
		b=(ch<<4);
		str++;

		ch = *str;
		if(ch>='0' && ch<='9')
			ch -= '0';
		else if(ch >='a' && ch <= 'f')
			ch = ch + 10 - 'a';
		else if(ch >= 'A' && ch <= 'F')
			ch = ch + 10 - 'A';
		else
			ch=0;
		b|=ch;
		str++;

		*pHex = b;
		pHex++;
	}
}
void SString::StrToHex(SString str,unsigned char *pHex)
{
	StrToHex(str.data(),str.length(),pHex);
// 	SString s;
// 	int index=0;
// 	while(str.m_iStrLen >= 2)
// 	{
// 		s = "0x"+str.left(2);
// 		str = str.mid(2);
// 		pHex[index++] = s.toInt();
// 	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  将指定的16进制缓冲区内容转换成字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:05
// 参数说明:  @pHex为缓冲区内容
//            @iBytes为字节数
//            @bSpace为各字节间是否带空格
// 返 回 值:  新的字符串，每个字节占两个字符，如bSpace为true再加一个空格符
//////////////////////////////////////////////////////////////////////////
SString SString::HexToStr(unsigned char *pHex,int iBytes,bool bSpace)
{
	static BYTE bHexChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    char *pr=NULL;
	if(bSpace)
	{
		pr=new char[3*iBytes +1];
		pr[3*iBytes]=0;
		int  i;
		int pos=0;
		for (i=0;i<iBytes;i++)
		{
			pr[pos] = bHexChar[pHex[i]>>4];
			pr[pos+1] = bHexChar[pHex[i] & 0x0F];
			pr[pos+2]=' ';
			pos +=3;
		}
	}
	else
	{
		pr=new char[2*iBytes +1];
		pr[2*iBytes]=0;
		int  i;
		int pos=0;
		for (i=0;i<iBytes;i++)
		{
			pr[pos] = bHexChar[pHex[i]>>4];
			pr[pos+1] = bHexChar[pHex[i] & 0x0F];
			pos +=2;
		}
	}
	SString str=pr;
	delete[] pr;
	return str;
}

SString SString::upper()
{
	return toUpper();
}
SString SString::lower()
{
	return toLower();
}
SString SString::stripWhiteSpace()
{
	while(left(1) == " " || left(1) == "\r" || left(1) == "\n" || left(1) == "\t")
		*this = right(m_iStrLen-1);
	while(right(1) == " " || right(1) == "\r" || right(1) == "\n" || right(1) == "\t")
		*this = left(m_iStrLen-1);
	return *this;
}

SString SString::fill(char ch, int size/*=-1*/)
{
	int i,cnt;
	char *p;
	if(size == -1)
	{
		cnt = m_iStrLen;
		p = (char*)m_pStr;
		for(i=0;i<cnt;i++)
		{
			p[i] = ch;
		}
	}
	else
	{
		*this="";
		for(i=0;i<size;i++)
			*this += ch;
	}
	return *this;
}
SString SString::limit(int size)
{
	SString str = *this;
	if(str.m_iStrLen > size)
		str = str.left(size);
	return str;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  格式化字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:45
// 参数说明:  ...
// 返 回 值:  SString
//////////////////////////////////////////////////////////////////////////
SString SString::toFormat(const char * cformat, ...)
{
	char buf[10240];
	memset(buf,0,10240);
	va_list ap;
	va_start(ap, cformat);
	vsprintf(buf,cformat,ap);
	va_end(ap);
	SString str(buf);
	return str;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  转换long为字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @v为数值
//            @sen为进制数，支持10进制和16进制
// 返 回 值:  新的字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toString(long v,int sen/*=10*/)
{
	return toString((int)v,sen);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  将当前指定的字符串设为指定的长度
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:53
// 参数说明:  @str源字符串
//            @len为新串长度
//            @chFill为填充字符
//            @bAddAtleft为true表示在左边填充，false表示右边填充
// 返 回 值:  新字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toFillString(SString str,int len,char chFill/*=' '*/,bool bAddAtLeft/*=true*/)
{
	SString s = str;
	while(s.m_iStrLen < len)
	{
		if(bAddAtLeft)
			s = SString::toString(&chFill,1) + s;
		else
			s += chFill;
	}
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换int为字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @v为数值
//            @sen为进制数，支持10进制和16进制
// 返 回 值:  新的字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toString(int v,int sen/*=10*/)
{
	SString s;
	if(sen == 16)
		s.sprintf("%X",v);
	else
		s.sprintf("%d",v);
	return s;
}
////////////////////////////////////////////////////////////////////////
// 描    述:  从缓冲区创建字符串，当不确认字符缓冲区内是否有结束符时使用该函数指定长度
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:54
// 参数说明:  @pStr为字符缓冲区
//            @iLength为长度
// 返 回 值:  新的字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toString(const char* pStr,int iLength)
{
	char *pBuf = new char[iLength+1];
	memset(pBuf,0,iLength+1);
	memcpy(pBuf,pStr,iLength);
	SString s = pBuf;
	delete[] pBuf;
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换uchar为字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @v为数值
//            @sen为进制数，支持10进制和16进制
// 返 回 值:  新的字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toString(unsigned char v)
{
	SString s;
	s.sprintf("%d",v);
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换short为字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @v为数值
//            @sen为进制数，支持10进制和16进制
// 返 回 值:  新的字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toString(short v)
{
	SString s;
	s.sprintf("%d",v);
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换float为字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @v为数值
//            @sen为进制数，支持10进制和16进制
// 返 回 值:  新的字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toString(float v,int d/*=-1*/)
{
	SString s;
	if(d == -1)
		s.sprintf("%f",v);
	else if(d == 1)
		s.sprintf("%.1f",v);
	else if(d == 2)
		s.sprintf("%.2f",v);
	else if(d == 3)
		s.sprintf("%.3f",v);
	else if(d == 4)
		s.sprintf("%.4f",v);
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换double为字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @v为数值
//            @sen为进制数，支持10进制和16进制
// 返 回 值:  新的字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toString(double v,int d/*=-1*/)
{
	SString s;
	if(d == -1)
		s.sprintf("%f",v);
	else
	{
		SString mask;
		mask.sprintf("%%.%df",d);
		s.sprintf(mask.m_pStr,v);
	}
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换指定字符串为int
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @str为指定的字符串
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int SString::toInt(SString str)
{
	return str.toInt();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换指定字符串为float
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @str为指定的字符串
// 返 回 值:  float
//////////////////////////////////////////////////////////////////////////
float SString::toFloat(SString str)
{
	return str.toFloat();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换指定字符串为double
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:52
// 参数说明:  @str为指定的字符串
// 返 回 值:  double
//////////////////////////////////////////////////////////////////////////
double SString::toDouble(SString str)
{
	return str.toDouble();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  将字节数（含G字节）转换为字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 13:57
// 参数说明:  @bytes为字节数
//            @gbytes为G字节数(1024*1024*1024)
// 返 回 值:  新字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toBytesString(int bytes,int gbytes/*=0*/)
{
	SString str;
	float gb = (float)gbytes;
	float b = (float)bytes;
	if(gb > 0)
	{
		if(gb >= 1024)
			str.sprintf("%0.2fT",gb/1024);
		else
			str.sprintf("%0.2fG",gb+b/1073741824);
	}
	else
	{
		if(b >= 1048576)
			str.sprintf("%0.2fM",b/1048576);
		else if(b >= 1024)
			str.sprintf("%0.2fK",b/1024);
		else
			str.sprintf("%d",bytes);
	}
	return str;
}


/************************************************************************/
/* 属性串                                                               */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
// 描    述:  读取字符串到指定的字符
// 作    者:  邵凯田
// 创建时间:  2012-3-14 12:54
// 参数说明:  @pSrcStr为源字符串，读取一次后将自动向后移动到停止字符串之后
//            @sStopChar为停止字符串
//            @sResult为结果字符串
// 返 回 值:  true表示找到，false表示找不到
//////////////////////////////////////////////////////////////////////////
bool SString::ReadToChar(char* &pSrcStr,char *sStopChar,SString &sResult)
{
	char* str = strstr(pSrcStr,sStopChar);
	if(str == NULL)
	{
		if(*pSrcStr == '\0')
			return false;
		sResult.SetString(pSrcStr);
		pSrcStr += strlen(pSrcStr);
		return true;
	}
	sResult.SetString(pSrcStr,str-pSrcStr);
	pSrcStr = str + strlen(sStopChar);
	return true;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  取指定分隔字符串中的第index个子串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:40
// 参数说明:  @index为子串序号，从1开始编号
//            @sAttr为字符串
//            @chSep为分隔字符
// 返 回 值:  子串内容
//////////////////////////////////////////////////////////////////////////
SString SString::GetIdAttribute(int index,SString &sAttr,SString chSep/*=";"*/)
{
	int iPos1=0,iPos2;
	
	for(int i=1;i<index;i++)
	{
		iPos1 = sAttr.find(chSep,iPos1);
		if(iPos1 < 0)
		{
			if(i==1)
				return sAttr;
			return "";
		}
		iPos1 += chSep.m_iStrLen;
	}
	iPos2 = sAttr.find(chSep,iPos1);
	if(iPos2 < 0)
		return sAttr.mid(iPos1);
	else
		return sAttr.mid(iPos1,iPos2 - iPos1);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定分隔字符串中的第index个子串,并转换为int返回
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:40
// 参数说明:  @index为子串序号，从1开始编号
//            @sAttr为字符串
//            @chSep为分隔字符
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int SString::GetIdAttributeI(int index,SString &sAttr,SString chSep/*=";"*/)
{
	return GetIdAttribute(index,sAttr,chSep).toInt();
}

#ifdef WIN32
__int64 SString::GetIdAttributeI64(int index,SString &sAttr,SString chSep)
{
	SString str = GetIdAttribute(index,sAttr,chSep);
	return _atoi64(str.data());
}
#else
long long SString::GetIdAttributeI64(int index,SString &sAttr,SString chSep)
{
	SString str = GetIdAttribute(index,sAttr,chSep);
	return atoll(str.data());
}
#endif

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定分隔字符串中的第index个子串,并转换为float返回
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:40
// 参数说明:  @index为子串序号，从1开始编号
//            @sAttr为字符串
//            @chSep为分隔字符
// 返 回 值:  float
//////////////////////////////////////////////////////////////////////////
float SString::GetIdAttributeF(int index,SString &sAttr,SString chSep/*=";"*/)
{
	return GetIdAttribute(index,sAttr,chSep).toFloat();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取子字符串的个数
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:50
// 参数说明:  @sAttr为字符串
//            @chSep为分隔字符
// 返 回 值:  数量
//////////////////////////////////////////////////////////////////////////
int SString::GetAttributeCount(SString &sAttr,SString chSep/*=";"*/)
{
	int p=0,cnt=1,p1;
	while(1)
	{
		p1 = sAttr.find(chSep,p);
		if(p1 < 0)
			break;
		p = p1+chSep.m_iStrLen;
		cnt++;
	}
	return cnt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取属性值name1=value1;name2=value2;
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:50
// 参数说明:  @sAttr为属性字符串，如：name1=value1;name2=value2;
//            @sName为属性名称
//            @chEqual为等于号字符，缺省为"=",也可以是多个字符
//            @chSep分隔符内容，缺省为";",也可以是多个字符
// 返 回 值:  属性值
//////////////////////////////////////////////////////////////////////////
SString SString::GetAttributeValue(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	int iPos1,iPos2;
	iPos1 = sAttr.find(sName+chEqual);
	if(iPos1==0)
	{
		iPos1 += strlen(sName)+strlen(chEqual);
		iPos2 = sAttr.find(chSep,iPos1);
		if(iPos2 < 0)
		{
			return sAttr.mid(iPos1);
		}
		else
		{
			return sAttr.mid(iPos1,iPos2-iPos1);
		}
	}
	else
	{
		iPos1 = sAttr.find(chSep+sName+chEqual);
		if(iPos1 < 0)
			return "";
		iPos1 += strlen(sName)+1+chSep.m_iStrLen;
		iPos2 = sAttr.find(chSep,iPos1);
		if(iPos2 < 0)
			return sAttr.mid(iPos1);
		else
			return sAttr.mid(iPos1,iPos2-iPos1);
	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  当前属性名是否存在
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:50
// 参数说明:  @sAttr为属性字符串，如：name1=value1;name2=value2;
//            @sName为属性名称
//            @chEqual为等于号字符，缺省为"=",也可以是多个字符
//            @chSep分隔符内容，缺省为";",也可以是多个字符
// 返 回 值:  属性值
//////////////////////////////////////////////////////////////////////////
bool SString::IsAttribute(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	int iPos1;
	iPos1 = sAttr.find(sName+chEqual);
	if(iPos1==0)
		return true;
	else
	{
		iPos1 = sAttr.find(chSep+sName+chEqual);
		if(iPos1 < 0)
			return false;
		return true;
	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取属性值并转换为int
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:50
// 参数说明:  @sAttr为属性字符串，如：name1=value1;name2=value2;
//            @sName为属性名称
//            @chEqual为等于号字符，缺省为"=",也可以是多个字符
//            @chSep分隔符内容，缺省为";",也可以是多个字符
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int SString::GetAttributeValueI(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	return GetAttributeValue(sAttr,sName,chEqual,chSep).toInt();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取属性值并转换成float
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:50
// 参数说明:  @sAttr为属性字符串，如：name1=value1;name2=value2;
//            @sName为属性名称
//            @chEqual为等于号字符，缺省为"=",也可以是多个字符
//            @chSep分隔符内容，缺省为";",也可以是多个字符
// 返 回 值:  float
//////////////////////////////////////////////////////////////////////////
float SString::GetAttributeValueF(SString &sAttr,SString sName,SString chEqual/*="="*/,SString chSep/*=";"*/)
{
	return GetAttributeValue(sAttr,sName,chEqual,chSep).toFloat();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  将当前字符缓冲区中GB字符串转换为UTF-8编码
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:10
// 参数说明:  @pIn为原字符
// 返 回 值:  新字符串
//////////////////////////////////////////////////////////////////////////
SString SString::Gb2Utf8(char *pIn)
{
	SString sRet;

#ifdef WIN32
     WCHAR *strSrc;
     TCHAR *szRes;

     //获得临时变量的大小
     int i = MultiByteToWideChar(CP_ACP, 0, pIn, -1, NULL, 0);
     strSrc = new WCHAR[i+1];
     MultiByteToWideChar(CP_ACP, 0, pIn, -1, strSrc, i);

     //获得临时变量的大小
     i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
     szRes = new TCHAR[i+1];
     WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

     sRet = (char*)szRes;
     delete []strSrc;
     delete []szRes;
#else
	int iInLen = strlen(pIn);
	int iOutLen = strlen(pIn)*2+1;
	char *pOut = new char[iOutLen];
	pOut[0] = '\0';
	//CodeConverter cc("gb2312//IGNORE","utf-8//IGNORE");
	CodeConverter cc("gb18030//IGNORE","utf-8//IGNORE");
	cc.convert(pIn,iInLen,pOut,iOutLen);
	/*
	printf("pIn=%s, iInlen=%d, iOutLen=%d\n",pIn,iInLen,iOutLen);
	iconv_t cd;
	cd = iconv_open("utf-8","gb2312");
	printf("cd=%X\n",(int)cd);
	char **pin = &pIn;
	char **pout = &pOut;	
	memset(pOut,0,iOutLen);
	iconv(cd,pin,(size_t *)&iInLen,pout,(size_t *)&iOutLen);
	printf("pOut=%s\n",pOut);
	iconv_close(cd);
	*/
	sRet = pOut;
//	printf("sRet=%s\n",sRet.data());
//	printf("&pOut=%X\n",pOut);
	delete[] pOut;
#endif

	return sRet;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  在指定的字符缓冲区内转换UTF-8编码为GB编码
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:34
// 参数说明:  @pIn为原字符
//            @iSize为长度
// 返 回 值:  true表示转换成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SString::Utf82Gb(char *pIn,int iSize)
{
#ifdef WIN32
     WCHAR *strSrc;// = (TCHAR*)pOut;
     TCHAR *szRes = (TCHAR*)pIn;

     //获得临时变量的大小
     int i = MultiByteToWideChar(CP_UTF8, 0, pIn, -1, NULL, 0);
     strSrc = new WCHAR[i+1];
	 memset(strSrc,0,sizeof(WCHAR)*(i+1));
     MultiByteToWideChar(CP_UTF8, 0, pIn, -1, strSrc, i);

     //获得临时变量的大小
     i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
     //szRes = new TCHAR[i+1];
	 //memset(szRes,0,sizeof(TCHAR)*(i+1));
	 memset(pIn,0,iSize);
     WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

     delete []strSrc;
#else
	int iInLen = strlen(pIn);
	int iOutLen = strlen(pIn)/**2*/+1;
	char *pOut = new char[iOutLen];
	memset(pOut,0,iOutLen);
	pOut[0] = '\0';
	CodeConverter cc("utf-8//IGNORE","gb18030//IGNORE");
	cc.convert(pIn,iInLen,pOut,iOutLen);
	memset(pIn,0,iSize);
	if(iOutLen > iSize - 1)
		iOutLen = iSize - 1;
	memcpy(pIn,pOut,iOutLen);
	delete[] pOut;
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  将UTF16字符转换为UTF8
// 作    者:  邵凯田
// 创建时间:  2012-12-19 13:41
// 参数说明:  @pUTF16Start源UTF16开始位置,
//            @pUTF16End源UTF16结束位置,
//            @pUTF8Start目标UTF8缓冲区开始位置，
//            @pUTF8End目标UTF8缓冲区结束位置，UTF缓冲区应为UTF16缓冲区大小的两倍
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SString::Utf16_to_Utf8(UTF16* pUTF16Start,UTF16* pUTF16End,UTF8* pUTF8Start,UTF8* pUTF8End)
{
	UTF16* pTempUTF16 = pUTF16Start;
	UTF8* pTempUTF8 = pUTF8Start;
	while(pTempUTF16 <= pUTF16End)
	{
		if( *pTempUTF16 <= UTF8_ONE_END && pTempUTF8+1 < pUTF8End)
		{
			*pTempUTF8++=(UTF8)*pTempUTF16;
		}
		else if(*pTempUTF16 >= UTF8_TWO_START && *pTempUTF16 <= UTF8_TWO_END && pTempUTF8+2 < pUTF8End)
		{
			*pTempUTF8 ++=(*pTempUTF16>>6)|0xC0;
			*pTempUTF8 ++=(*pTempUTF16 & 0x3f) | 0x80;
		}
		else if(*pTempUTF16 >= UTF8_THERE_START && *pTempUTF16 <= UTF8_THERE_END && pTempUTF8+3 < pUTF8End)
		{
			*pTempUTF8++=(*pTempUTF16>>12)|0xE0;
			*pTempUTF8++=((*pTempUTF16>>6)&0x3f)|0x80;
			*pTempUTF8++=(*pTempUTF16&0x3f)|0x80;
		}
		else
			break;
		pTempUTF16++;
	}
	*pTempUTF8=0;
}

SString SString::Utf82Gb(char *pIn)
{
	SString sRet;

#ifdef WIN32
     WCHAR *strSrc;
     TCHAR *szRes;

     //获得临时变量的大小
     int i = MultiByteToWideChar(CP_UTF8, 0, pIn, -1, NULL, 0);
     strSrc = new WCHAR[i+1];
	 memset(strSrc,0,sizeof(WCHAR)*(i+1));
     MultiByteToWideChar(CP_UTF8, 0, pIn, -1, strSrc, i);

     //获得临时变量的大小
     i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
     szRes = new char[i+1];
	 memset(szRes,0,sizeof(char)*(i+1));
     WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

     sRet = (char*)szRes;
     delete []strSrc;
     delete []szRes;
#else
	int iInLen = strlen(pIn);
	int iOutLen = strlen(pIn)*2+1;
//	printf("pIn=%s, iInlen=%d, iOutLen=%d\n",pIn,iInLen,iOutLen);

	char *pOut = new char[iOutLen];
	memset(pOut,0,iOutLen);
	pOut[0] = '\0';

	
	//CodeConverter cc("utf-8//IGNORE","gb2312");
	//CodeConverter cc("utf-8//IGNORE","gb2312//IGNORE");
	CodeConverter cc("utf-8//IGNORE","gb18030//IGNORE");
	cc.convert(pIn,iInLen,pOut,iOutLen);

	/*
	iconv_t cd;
	cd = iconv_open("utf-8","gb2312");
	char **pin = &pIn;
	char **pout = &pOut;	
	memset(pOut,0,iOutLen);
	iconv(cd,pin,(size_t *)&iInLen,pout,(size_t *)&iOutLen);
	iconv_close(cd);
	printf("pOut=%s\n",pOut);
	*/
	sRet = pOut;
//	printf("Utf82Gb::sRet=%s\n",sRet.data());
//	printf("Utf82Gb::sRet.right(100)=%s,length=%d\n",sRet.right(100).data(),sRet.length());
//	printf("&pOut=%X\n",pOut);

	delete[] pOut;
#endif

	return sRet;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  当前字符串转换为新的UTF-8编码的字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:35
// 参数说明:  void
// 返 回 值:  新字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toUtf8()
{
	return Gb2Utf8(m_pStr);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  当前字符串转换为新的GB编码的字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-24 14:35
// 参数说明:  void
// 返 回 值:  新字符串
//////////////////////////////////////////////////////////////////////////
SString SString::toGb2312()
{
	return Utf82Gb(m_pStr);
}
