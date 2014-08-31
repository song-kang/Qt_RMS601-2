/**
 *
 * 文 件 名 : SFile.cpp
 * 创建日期 : 2007-01-01
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 类QT的文件类
 * 修改记录 : 
 *            $Log: SFile.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/19 09:10:58  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/01/17 01:24:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:07  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2008/09/11 05:17:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:01  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.5  2007/10/19 11:57:26  shaokt
 *            size函数增加可选的文件名参数
 *
 *            Revision 1.4  2007/09/11 08:17:52  shaokt
 *            基类更新
 *
 *            Revision 1.3  2007/09/04 01:43:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/08/16 08:10:25  shaokt
 *            修正：打开文件时加上'b'标志，否则会0x0D在行尾
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SFile.h"

static SString g_currDir;

//////////////////////////////////////////////////////////////////////////
// 名    称:  SFile
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:08
// 描    述:  文件类
//////////////////////////////////////////////////////////////////////////

SFile::SFile()
{
	SKTOBJECT("SFile");
	m_fp = 0;
}

SFile::SFile(SString fileName)  
{
	SKTOBJECT("SFile");
	m_fileName = fileName;m_fp=0;
}

SFile::~SFile()
{
	close();
}

void SFile::setName(SString name)
{
	m_fileName = name;
#ifdef WIN32
	if(name.left(2).right(1) != ":")
#else
	if(name.left(1) != "/")
#endif
	{
		if(g_currDir.right(1) != "/")
			g_currDir += "/";
		m_fileName = g_currDir+m_fileName;
	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  关闭文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:19
// 参数说明:  void
// 返 回 值:  true表示关闭成功,flase表示关闭失败
//////////////////////////////////////////////////////////////////////////
bool SFile::close()
{
	if(m_fp)
	{
		fclose(m_fp);
		m_fp = 0;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  打开文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:20
// 参数说明:  @mode表示打开方式
// 返 回 值:  true表示打开成功，false表示打开失败
//////////////////////////////////////////////////////////////////////////
bool SFile::open(OpenMode mode)
{
	if(m_fp)
	{
		return false;
	}
	
	SString m="";
	if(mode == IO_Append)
		m="rb+";//"wb+";
	else if(mode == IO_ReadOnly)
		m="rb";
	else if(mode == IO_ReadWrite)
		m= "arb+";
	else if(mode == IO_Translate)
		m= "rtb";
	else if(mode == IO_Truncate)
		m="wb";
	else if(mode == IO_WriteOnly)
		m="wb";
	m_fp = fopen(m_fileName.data(), m.data());
	if(m_fp == 0)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  以指定的模式字符串打开文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:20
// 参数说明:  @mode为模式，如"wb"
// 返 回 值:  true表示打开成功，false表示打开失败
//////////////////////////////////////////////////////////////////////////
bool SFile::openByMode(char* mode)
{
	if(m_fp)
	{
		return false;
	}
	m_fp = fopen(m_fileName.data(), mode);
	if(m_fp == 0)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  删除当前文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:21
// 参数说明:  void
// 返 回 值:  true删除成功，false删除失败
//////////////////////////////////////////////////////////////////////////
bool SFile::remove()
{
	return remove(m_fileName.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  重命名当前的文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:21
// 参数说明:  @newname表示新的文件名
// 返 回 值:  true表示命名成功，false表示命名失败
//////////////////////////////////////////////////////////////////////////
bool SFile::rename(SString newName)
{
	return rename(m_fileName, newName) == 0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  当前文件是否存在
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:22
// 参数说明:  void
// 返 回 值:  true表示文件存在,false表示文件不存在
//////////////////////////////////////////////////////////////////////////
bool SFile::exists()
{
	return exists(m_fileName.data());
}

////////////////////////////////////////////////////////////////////////
// 描    述:  复制当前文件到另外一个文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:22
// 参数说明:  @newFile新文件的文件名
// 返 回 值:  true表示复制成功，false表示复制失败
//////////////////////////////////////////////////////////////////////////
bool SFile::copy(SString newFile)
{
	newFile=newFile;return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  读取一个数据块
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:24
// 参数说明:  @pBuffer为待写入的缓冲区
//            @maxSize为缓冲区的总大小,也是当前期望读取的最大字节数
// 返 回 值:  实际读取到的字节数量,<0表示失败
//////////////////////////////////////////////////////////////////////////
long SFile::readBlock(BYTE *pBuff, int maxSize)
{
	if(m_fp == 0)
		return -1;
	return fread(pBuff,1,maxSize, m_fp);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  写入指定数量的0字节
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:27
// 参数说明:  @bytes表示要定稿的字节数量
// 返 回 值:  返回1表示成功，<0表示异常
//////////////////////////////////////////////////////////////////////////
long SFile::writeZero(int bytes)//写入指定数量的0字节
{
	if(m_fp == 0)
		return -1;
	int size = 0;
	BYTE *zero = new BYTE[4096];
	memset(zero,0,4096);
	while(bytes > 0)
	{
		size = bytes;
		if(size > 4096)
			size = 4096;
		fwrite(zero,1,size,m_fp);
		bytes -= size;
	}
	delete[] zero;
	return 1;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  写入数据块
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:27
// 参数说明:  @pBuff表示缓冲区地址
//            @size表示待写入长度
// 返 回 值:  返回实际写入长度，<0表示失败
//////////////////////////////////////////////////////////////////////////
long SFile::writeBlock(BYTE *pBuff, int size)
{
	if(m_fp == 0)
		return -1;
	return fwrite(pBuff,1,size, m_fp);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  读取一行到指定的缓冲区，碰到\n、文件结束或缓冲区满时退出
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:30
// 参数说明:  @pBuff表示缓冲区地址
//            @maxSize表示缓冲区大小（字节数）
// 返 回 值:  返回实际读取的字节数
//////////////////////////////////////////////////////////////////////////
long SFile::readLine(char *pBuff, int maxSize)
{
	if(m_fp == 0)
		return -1;
	for(int i=0; i<maxSize-1; i++)
	{
		fread(pBuff+i,1,1,m_fp);
		if(pBuff[i] == '\n' || feof(m_fp))
		{
			pBuff[i] = '\0';
			if(pBuff[i-1] == '\r')
			{
				pBuff[i-1] = '\0';
				return i-1;
			}
			return i;
		}
	}return -1;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  读取一行到返回字符串中
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:32
// 参数说明:  @maxSize表示字符串的最大长度
// 返 回 值:  返回字符串
//////////////////////////////////////////////////////////////////////////
SString SFile::readLine(int maxSize/*=10240*/)
{
	char *buf = new char[maxSize+1];
	memset(buf,0,maxSize+1);
	readLine(buf,maxSize);
	SString str=buf;
	delete[] buf;
	return str;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定文件文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:41
// 参数说明:  @sFileName为指定的文件名，未指定时为当前文件
// 返 回 值:  返回文件大小字节数
//////////////////////////////////////////////////////////////////////////
long SFile::size(SString sFileName)
{
	SFile file(sFileName==""?m_fileName:sFileName);
	if(!file.open(IO_ReadOnly))
		return -1;
	fseek(file.getHandle(),0,SEEK_END);
	long len=ftell(file.getHandle());
	file.close();
	return len;
	
}


////////////////////////////////////////////////////////////////////////
// 描    述:  向文件中打印格式化字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:46
// 参数说明:  同printf
// 返 回 值:  返回实际写入长度，<0表示失败
//////////////////////////////////////////////////////////////////////////
long SFile::fprintf(const char * cformat, ...)
{
	char buf[10240];
	memset(buf,0,10240);
	va_list ap;
	va_start(ap, cformat);
	vsprintf(buf,cformat,ap);
	va_end(ap);
	return writeBlock((BYTE*)buf,(int)strlen(buf));
}

////////////////////////////////////////////////////////////////////////
// 描    述:  定位文件指针到指定的绝对位置
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:48
// 参数说明:  @pos为文件偏移的绝对位置
// 返 回 值:  0表示成功，其它均为失败
//////////////////////////////////////////////////////////////////////////
int SFile::at(int pos)
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, pos, SEEK_SET);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  定位文件指针到指定的绝对位置
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:48
// 参数说明:  @pos为文件偏移的绝对位置
// 返 回 值:  0表示成功，其它均为失败
//////////////////////////////////////////////////////////////////////////
int SFile::seekTo(int pos)
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, pos, SEEK_SET);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  定位文件指针到指定的相对位置
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:48
// 参数说明:  @pos为文件偏移的相对位置
// 返 回 值:  0表示成功，其它均为失败
//////////////////////////////////////////////////////////////////////////
int SFile::seekCur(int pos)
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, pos, SEEK_CUR);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  定位文件指针到文件开始位置
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:48
// 参数说明:  void
// 返 回 值:  0表示成功，其它均为失败
//////////////////////////////////////////////////////////////////////////
int SFile::seekBegin()
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, 0, SEEK_SET);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  定位文件指针到文件结束位置
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:48
// 参数说明:  void
// 返 回 值:  0表示成功，其它均为失败
//////////////////////////////////////////////////////////////////////////
int SFile::seekEnd()
{
	if(m_fp == 0)
		return -1;
	return fseek(m_fp, 0, SEEK_END);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  清除文件缓冲区，一般用于将写入文件立即生效
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:00
// 参数说明:  void
// 返 回 值:  0表示成功，其它均为失败
//////////////////////////////////////////////////////////////////////////
int SFile::flush()
{
	if(m_fp == 0)
		return -1;
	int ret = fflush(m_fp);
#ifndef WIN32
	sync();
#endif
	return ret;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前文件指针的位置
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:01
// 参数说明:  void
// 返 回 值:  返回文件指针的当前位置
//////////////////////////////////////////////////////////////////////////
int SFile::position()
{
	if(m_fp == 0)
		return -1;
	return ftell(m_fp);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  判断文件指针是否在文件末尾
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:05
// 参数说明:  void
// 返 回 值:  true表示在文件末尾，false表示不在末尾
//////////////////////////////////////////////////////////////////////////
bool SFile::atEnd()
{
	if(feof(m_fp) == 0)
		return false;
	else
		return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前文件名全路径中的文件名部分
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:05
// 参数说明:  void
// 返 回 值:  文件名
//////////////////////////////////////////////////////////////////////////
SString SFile::name()
{
	int pos = m_fileName.findRev("/");
	if(m_fileName.findRev("\\") > pos)
		pos = m_fileName.findRev("\\");
	if(pos < 0)
		return m_fileName;
	pos ++;
	return m_fileName.right(m_fileName.length() - pos);
}

///static

////////////////////////////////////////////////////////////////////////
// 描    述:  创建一个空的新文件，并允许指定文件大小
// 作    者:  邵凯田
// 创建时间:  2011-11-18 10:25
// 参数说明:  @sPathName表示文件全路径名
//            @iFileSize表示文件大小
// 返 回 值:  true表示成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SFile::touchFile(SString sPathName,int iFileSize/*=0*/)//创建指定大小的空文件
{
	unsigned char b = 0;
	SFile fl;
	fl.setFileName(sPathName);
	time_t t1 = ::time(NULL);
	if(!fl.open(IO_WriteOnly))
	{
		LOGERROR("SFile::touchFile :%s failed........",sPathName.data());
		return false;
	}
#ifdef WIN32
	fl.seekTo(iFileSize-1);
	fl.writeBlock(&b,1);
#else
	fl.seekBegin();
	int s,slen = 65536;
	if(iFileSize > 10485760)
		slen = 1048576;
	BYTE *buf = new BYTE[slen];
	memset(buf,0,slen);
	while(iFileSize>0)
	{
		s = iFileSize;
		if(s > slen)
			s = slen;
		fl.write(buf,s);
		iFileSize -= s;
#ifdef _POWERPC //PPC平台没有DMA，不延时会导致CPU空闲为0
		usleep(10000);//每写1M延时10毫秒，1G文件多花10秒
#endif
	}
	delete[] buf;
#endif
	fl.close();
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  复制文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:09
// 参数说明:  @fileName表示源文件名
//            @newFile表示目标文件
// 返 回 值:  true表示复制成功，false表示复制失败
//////////////////////////////////////////////////////////////////////////
bool SFile::copy(SString fileName, SString newFile)
{
	SFile f1(fileName);
	SFile f2(newFile);
	if(!f1.open(IO_ReadOnly) || !f2.open(IO_WriteOnly))
	{
		f1.close();
		f2.close();
		return false;
	}
	BYTE buf[1025];
	while(1)
	{
		int ret = f1.readBlock(buf,1025);
		if(ret <= 0)
			break;
		f2.writeBlock(buf,ret);
	}
	f1.close();
	f2.close();
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  合并文件,将源文件的内容复制到目标文件的后面
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:11
// 参数说明:  @fileFrom表示源文件名
//            @fileTo表示目标文件名
// 返 回 值:  true表示合并成功，false表示合并失败
//////////////////////////////////////////////////////////////////////////
bool SFile::merge(SString fileFrom,SString fileTo)
{
	SFile f1(fileFrom);
	SFile f2(fileTo);
	if(!f1.open(IO_ReadOnly) || !f2.open(IO_Append))
	{
		f1.close();
		f2.close();
		return false;
	}
	f2.seekEnd();
	BYTE buf[1025];
	while(1)
	{
		int ret = f1.readBlock(buf,1025);
		if(ret <= 0)
			break;
		f2.writeBlock(buf,ret);
	}
	f1.close();
	f2.close();
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  指定文件是否存在
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:12
// 参数说明:  @fileName表示文件名
// 返 回 值:  true表示文件存在，false表示文件不存在
//////////////////////////////////////////////////////////////////////////
bool SFile::exists(SString fileName)
{
	SFile f(fileName);
	bool b=f.open(IO_ReadOnly);
	f.close();
	return b;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  删除指定的文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:15
// 参数说明:  @fileName待删除的文件
// 返 回 值:  true表示删除成功，false表示删除失败
//////////////////////////////////////////////////////////////////////////
bool SFile::remove(SString fileName)
{
	LOGDEBUG("删除文件:%s",fileName.data());
	return ::remove(fileName.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  重命名指定的文件
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:15
// 参数说明:  @fileName为原文件名
//            @newFile为新文件
// 返 回 值:  true表示重命名成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SFile::rename(SString fileName,SString newFile)
{
	return ::rename(fileName.data(),newFile.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定文件名的文件修改时间
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:16
// 参数说明:  @fileName为文件名
// 返 回 值:  SDateTime修改时间
//////////////////////////////////////////////////////////////////////////
SDateTime SFile::filetime(SString fileName)
{
	struct stat statbuf;
	stat(fileName.data(),&statbuf);
	return SDateTime::makeDateTime(statbuf.st_mtime);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  表示此文件是否可执行
// 作    者:  邵凯田
// 创建时间:  2009-2-11 17:12
// 参数说明:  fileName表示文件名
// 返 回 值:  true:表示有运行权限,false:表示没有运行权限
//////////////////////////////////////////////////////////////////////////
bool SFile::is_exec_able(SString fileName)
{
#ifdef WIN32
	if(fileName.right(4) == ".exe")
		return true;
	else
		return false;
#else
	struct stat statbuf;
	stat(fileName.data(),&statbuf);
	unsigned int mod = statbuf.st_mode;
//8进制       9876543210
//644	1000000110100100	无X
//744	1000000111100100	1X	 bit6  拥有者
//754	1000000111101100	2X	 bit3  组
//755	1000000111101101	3X	 bit0  其它
//bit:6  3  0		
	if((mod & (1<<6)) != 0)
	{
		//说明有拥有者权限
		return true;
	}
	else
		return false;
#endif	
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//STextStream

STextStream::STextStream(SFile *pFile)
{
	SKTOBJECT("STextStream");
	m_pFile = pFile;
}
STextStream::~STextStream()
{
}

//operator
STextStream& STextStream::operator<<(const SString &str)
{
	m_pFile->write(str.data(),str.length());
	return *this;
}

STextStream& STextStream::operator<<(const char* pStr)
{
	SString str=pStr;
	m_pFile->write(str.data(),str.length());
	return *this;
}

SString STextStream::readLine()
{
	return m_pFile->readLine();
}

bool STextStream::atEnd()
{
	return m_pFile->atEnd();
}


//////////////////////////////////////////////////////////////////////////
// 名    称:  SDir
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:20
// 描    述:  目录类
//////////////////////////////////////////////////////////////////////////

SDir::SDir()
{
	SKTOBJECT("SDir");
	m_fileList.setAutoDelete(true);
	m_fileAttrList.setAutoDelete(true);
}
SDir::SDir(SString dirName, SString nameFilter/*="*"*/)
{
	SKTOBJECT("SDir");
	m_sDirName = dirName;
	m_sFilter = nameFilter;
	m_fileList.setAutoDelete(true);
	m_fileAttrList.setAutoDelete(true);
}
SDir::~SDir()
{
	m_fileList.clear();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  将当前目录下的一个文件重命名
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:30
// 参数说明:  @oldName表示原文件名
//            @newName表示新的文件名
// 返 回 值:  true表示改名成功，false表示改名失败
//////////////////////////////////////////////////////////////////////////
bool SDir::rename(SString oldName, SString newName)
{
	if(m_sDirName.length() > 0 && oldName.left(1) != "/" && oldName.left(2).right(1) != ":")
	{
		oldName = m_sDirName+"/"+oldName;
		newName = m_sDirName+"/"+newName;
	}
	return ::rename(oldName.data(),newName.data()) == 0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  删除当前目录下指定的文件名
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:31
// 参数说明:  @name不含路径的文件名
// 返 回 值:  true删除成功，false删除失败
//////////////////////////////////////////////////////////////////////////
bool SDir::remove(SString name)
{
	return SFile::remove(m_sDirName+"/"+name);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  当前目录下指定的文件名是否存在
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:31
// 参数说明:  @name不含路径的文件名
// 返 回 值:  true文件存在，false不
//////////////////////////////////////////////////////////////////////////
bool SDir::exists(SString name)
{
	return SFile::exists(m_sDirName+"/"+name);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  统计当前目录下的文件和子目录，并返回数量，需要指定条件m_sFilter
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:32
// 参数说明:  void
// 返 回 值:  文件和子目录的数量
//////////////////////////////////////////////////////////////////////////
int SDir::count()
{
	m_fileList.clear();
	m_fileAttrList.clear();
	
#ifdef WIN32
	if(m_sFilter == "")
		m_sFilter = "*";
	SString str = m_sDirName+"/"+m_sFilter;
	void *m_pNextInfo = new WIN32_FIND_DATA;
	HANDLE h = ::FindFirstFile(str.data(), (WIN32_FIND_DATA*) m_pNextInfo);
	while(h != INVALID_HANDLE_VALUE)
	{
		SString *pStr=new SString(((LPWIN32_FIND_DATA) m_pNextInfo)->cFileName);			
		m_fileList.append(pStr);
		SString *pAttr=new SString();
		DWORD attr = ((LPWIN32_FIND_DATA) m_pNextInfo)->dwFileAttributes;
		if((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
			*pAttr = "isdir=1;";//是目录
		else
			*pAttr = "isdir=0;";//不是目录
		m_fileAttrList.append(pAttr);
		BOOL bFile = ::FindNextFile(h, (LPWIN32_FIND_DATA) m_pNextInfo);
		if(bFile == FALSE)
			break;
	}
	if(h != INVALID_HANDLE_VALUE)
		FindClose(h);
	delete m_pNextInfo;
#else
	DIR *dir;
	struct dirent *ptr;
	SString sTemp;
	dir=opendir(m_sDirName.data());
	if(dir == NULL)
		return 0;//可能目录不存在，不能readdir
	while((ptr = readdir(dir)) != 0)
	{
		SString str=m_sFilter;
		str.replace("*","");
		SString *pStr=new SString(ptr->d_name);			
		if(str.length() > 0 && pStr->find(str) < 0)
		{
			delete pStr;
			continue;
		}

		SString *pAttr=new SString();
		struct   stat   st;
		sTemp.sprintf("%s/%s",m_sDirName.data(),pStr->data());
		stat(sTemp.data(),&st);
		if(S_ISDIR(st.st_mode))
			*pAttr = "isdir=1;";//是目录
		else
			*pAttr = "isdir=0;";//不是目录
		m_fileAttrList.append(pAttr);

		m_fileList.append(pStr);
	}
	closedir(dir);
#endif
	return m_fileList.count();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定的第index个文件/目录的名称
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:34
// 参数说明:  @index为序号，从0开始编号
// 返 回 值:  文件名或子目录名
//////////////////////////////////////////////////////////////////////////
SString SDir::operator[](int index)
{
	if(index >= (int)m_fileList.count())
		return "";
	return *(m_fileList[index]);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定的第index个文件/目录的属性字符串
//                    (isdir=1;表示是子目录  isdir=0;表示是文件)
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:36
// 参数说明:  @index为序号，从0开始编号
// 返 回 值:  属性字符串
//////////////////////////////////////////////////////////////////////////
SString SDir::attribute(int index)
{
	if(index >= (int)m_fileAttrList.count())
		return "";
	return *(m_fileAttrList[index]);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  第index个文件/目录是否是目录
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:38
// 参数说明:  @index为序号，从0开始编号
// 返 回 值:  true表示是目录，false表示不是目录而是文件
//////////////////////////////////////////////////////////////////////////
bool SDir::IsDir(int index)
{
	if(index >= (int)m_fileAttrList.count())
		return false;
	return SString::GetAttributeValueI(*(m_fileAttrList[index]),"isdir")==1;
}

//static member静态函数
////////////////////////////////////////////////////////////////////////
// 描    述:  设置当前路径到一个全局变量中
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:39
// 参数说明:  @dir为待设置的新路径
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDir::setCurrent(SString dir)
{
	g_currDir = dir;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前路径
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:44
// 参数说明:  void
// 返 回 值:  当前路径，含路径分隔符
//////////////////////////////////////////////////////////////////////////
SString SDir::currentPath()
{
	SString str;
	char currentName[SKT_PATH_MAX+1];
	memset(currentName,0,SKT_PATH_MAX+1);
#ifdef WIN32
	GetModuleFileName(NULL,currentName,SKT_PATH_MAX);
	str = currentName;
	int pos=str.findRev("/");
	if(pos<0)
		pos = str.findRev("\\");
	if(pos>0)
		str = str.left(pos+1);
#else
	if(getcwd(currentName,SKT_PATH_MAX))
		str = currentName;
	else
		str = "";
	if(str.right(1) != "/")
		str += "/";
#endif
	return str;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  创建一个新的目录
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:40
// 参数说明:  @dir为待创建的目录全路径
// 返 回 值:  true表示创建的成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool SDir::createDir(SString dir)
{
#ifdef WIN32
	return _mkdir(dir.data()) == 0;
#else
	return mkdir(dir.data(),10705) == 0;
#endif
}


////////////////////////////////////////////////////////////////////////
// 描    述:  清除指定目录下的全部内容，同时递归清除全部子目录，不清除指定的目录自身
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:40
// 参数说明:  @dir为指定的目录
// 返 回 值:  true清除成功，false表示清除失败
//////////////////////////////////////////////////////////////////////////
bool SDir::clearDir(SString sPath)//清除指定目录下的全部内容
{
	if(sPath.Right(1) != "\\" && sPath.Right(1) != "/")
	{
#ifdef WIN32
		sPath += "\\";
#else
		sPath += "/";
#endif
	}
	SDir dir(sPath,"*");
	bool err = false;
	int i,cnt = dir.count();
	for(i=0;i<cnt;i++)
	{
		SString sFile = dir[i];
		if(sFile == "." || sFile == "..")
			continue;
		SString attr = dir.attribute(i);
		if(SString::GetAttributeValueI(attr,"isdir") == 1)
		{
			if(!clearDir(sPath+sFile))
				err = true;
			if(!SDir::removeDir(sPath+sFile))
				err = true;
		}
		else
		{
			if(!SFile::remove(sPath+sFile))
				err = true;
		}
	}
	if(err)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  删除一个指定的目录，只有当目录为空才能删除成功
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:42
// 参数说明:  @dir待删除的目录
// 返 回 值:  true删除目录成功，false失败
//////////////////////////////////////////////////////////////////////////
bool SDir::removeDir(SString dir)
{
#ifdef WIN32
	return _rmdir(dir.data()) == 0;
#else
	return rmdir(dir.data()) == 0;
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  判断指定的文件目录是否存在
// 作    者:  邵凯田
// 创建时间:  2011-11-18 11:43
// 参数说明:  @sDir为目录全路径
// 返 回 值:  true表示目录存在，false表示不存在
//////////////////////////////////////////////////////////////////////////
bool SDir::dirExists(SString sDir)
{
#ifdef WIN32
	sDir.replace("/","\\");
	while(sDir.right(1) == "\\")
		sDir = sDir.left(sDir.length()-1);
	//void *m_pNextInfo = new WIN32_FIND_DATA;
	WIN32_FIND_DATA data;
	HANDLE h = ::FindFirstFile(sDir.data(), (WIN32_FIND_DATA*) &data/*m_pNextInfo*/);
	if(h == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	FindClose(h);
#else
	DIR *dir;
	//struct dirent *ptr;
	dir=opendir(sDir.data());
	if(dir == NULL)
		return false;//可能目录不存在，不能readdir
	closedir(dir);
#endif
	return true;
}
