/**
 *
 * 文 件 名 : SIniFile.cpp
 * 创建日期 : 2002-05-14
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2008/09/02 01:50:02 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : INI文件操作类，可读写INI文件
 * 修改记录 : 
 *            $Log: IniFile.cpp,v $
 *            Revision 1.1  2008/09/02 01:50:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifdef WIN32
#pragma warning(disable: 4786)
#pragma warning(disable: 4503)
#endif

#include "SIniFile.h"

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
SIniFile::SIniFile()
{
	SKTOBJECT("SIniFile");
	m_pFirstGroup = NULL;
	m_bOpened = false;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  释放所有配置对象，释放内容
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:24
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SIniFile::Release()
{
	SIniFile::stuIniFileGroup *pGroup = m_pFirstGroup,*pG;
	SIniFile::stuIniFileField *pField,*pF;
	while(pGroup != NULL)
	{
		pField = pGroup->pFirstField;
		while(pField != NULL)
		{
			pF = pField->pNext;
			delete pField;
			pField = pF;
		}
		pG = pGroup->pNext;
		delete pGroup;
		pGroup = pG;
	}
	m_pFirstGroup = NULL;
}

SIniFile::~SIniFile()
{
	Release();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置INI文件全路径
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:09
// 参数说明:  @sPathFile表示文件名全路径
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SIniFile::SetPath(SString sPathFile)
{
	m_sPathFile = sPathFile;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  读取INI文件内容
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:10
// 参数说明:  @sPathFile表示待打开的文件名，为空时使用SetPath保存的文件名
// 返 回 值:  true表示读取成功,false表示读取失败
//////////////////////////////////////////////////////////////////////////
bool SIniFile::ReadIniFile(SString sPathFile)
{
	SString sLine,sField,sVal;
	FILE *fp;
	char ch;
	SString sGroup = "";
	int pos;
	SIniFile::stuIniFileField *pField=NULL;

	if(sPathFile.size() > 0)
		SetPath(sPathFile);
	
	fp = fopen(sPathFile.data(),"r");
	if(!fp)
	{
	    fp = fopen(sPathFile.data(),"a+");
	    if(!fp)
	    {
			return false;
	    }
	    fclose(fp);
	    fp = fopen(sPathFile.data(),"r");
	    if(!fp)
		return false;
	}
	bool bExit=false;
	while(1)
	{
		sLine = "";
		while(1)
		{
			if(fread(&ch,sizeof(char),1,fp) <=0)
			{
				bExit = true;
				break;
			}
			if(ch == '\n')
				break;
			if(ch != '\r')
				sLine += ch;
		}
		if(bExit)
			break;

		pos = sLine.find('=');
		
		if(sLine.size() > 0 && sLine.at(0) == '[' && sLine.find(']') >0 )
		{
			sGroup = sLine.substr(1,sLine.find(']') - 1);
			continue;
		}
		else if(pos > 0 && sLine.at(0) != '#')
		{
			sField = sLine.substr(0,pos);
			sVal = sLine.substr(pos + 1);
			if(sVal.at(sVal.size()-1) == '\r' || sVal.at(sVal.size()-1) == '\n')
				sVal = sVal.substr(0,sVal.size()-1);
			if(sVal.at(sVal.size()-1) == '\r' || sVal.at(sVal.size()-1) == '\n')
				sVal = sVal.substr(0,sVal.size()-1);
			pField = GetField(sGroup,sField);
			pField->sValue = sVal;
		}
		else //if(sLine.GetLength() == 0 || sLine.GetAt(0) == '#')		
		{
			if(pField == NULL)
			{
				pField=GetField("","");
				pField->sValue=sLine;
			}
			else
			{
				SIniFile::stuIniFileField *pNew=new SIniFile::stuIniFileField;
				pNew->pNext=pField->pNext;
				pField->pNext=pNew;
				pNew->sFieldName="";
				pNew->sValue=sLine;
			}
		}

	}

	fclose(fp);
	m_bOpened = true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  测试指定的配置项是否存在
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:11
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示组内的配置项名称
// 返 回 值:  true表示指定的配置项存在，false表示指定的配置项不存在
//////////////////////////////////////////////////////////////////////////
bool SIniFile::IsKey(SString sGroup, SString sField)
{
	SString sVal;
	if(m_bOpened == false)
		return false;

	SIniFile::stuIniFileField *pField;
	pField = GetField(sGroup,sField,false);
	if(pField == NULL)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  读取指定配置项的值内容
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:13
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示组内的配置项名称
// 返 回 值:  返回配置项内容
//////////////////////////////////////////////////////////////////////////
SString SIniFile::GetValue(SString sGroup,SString sField)
{
	SString sVal;
	if(m_bOpened == false)
		return "";

	SIniFile::stuIniFileField *pField;
	pField = GetField(sGroup,sField,false);
	if(pField == NULL)
		return "";
	sVal = pField->sValue;

	return sVal;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  读取指定的配置项的整型值，并允许指定未配置时的缺省值
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:14
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示组内的配置项名称
//            @idefault表示读取失败(未配置的情况下)时的缺省值
// 返 回 值:  配置值
//////////////////////////////////////////////////////////////////////////
int SIniFile::GetKeyIntValue(SString sGroup, SString sField, int idefault /*= 0*/)
{
	if(!IsKey(sGroup,sField))
		return idefault;

	SString val=GetValue(sGroup,sField);
	if(val.size() == 0)
		return idefault;

	return atoi(val.data());
}

////////////////////////////////////////////////////////////////////////
// 描    述:  读取指定的配置项的字符串值，并允许指定未配置时的缺省值
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:20
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示组内的配置项名称
//            @szdefault表示读取失败(未配置的情况下)时的缺省值
// 返 回 值:  配置值
//////////////////////////////////////////////////////////////////////////
SString SIniFile::GetKeyStringValue(SString sGroup, SString sField, SString szdefault /*= ""*/)
{
	if(!IsKey(sGroup,sField))
		return szdefault;
	return GetValue(sGroup,sField);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置指定配置项的值内容
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:21
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示组内的配置项名称
//            @sVal表示配置值
// 返 回 值:  true表示配置成功，false表示配置失败
//////////////////////////////////////////////////////////////////////////
bool SIniFile::SetValue(SString sGroup,SString sField,SString sVal)
{
	if(m_bOpened == false)
		return false;
	SIniFile::stuIniFileField *pField;
	pField = GetField(sGroup,sField);
	if(pField == NULL)
		return false;
	pField->sValue = sVal;

	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置指定配置项的整数型值，并允许自动配置文件
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:22
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示组内的配置项名称
//            @ivalue表示配置值
//            @autoSave表示是否自动保存，true自动保存，false不保存
// 返 回 值:  true表示配置成功，false表示配置失败
//////////////////////////////////////////////////////////////////////////
bool SIniFile::SetKeyIntValue(SString sGroup, SString sField, int ivalue, bool autoSave/*=true*/)
{
	char buf[32];
	memset(buf,0,32);
	sprintf(buf,"%d",ivalue);
	SetValue(sGroup,sField,buf);
	if(autoSave && !this->WriteFile())
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置指定配置项的字符串型值，并允许自动配置文件
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:23
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示组内的配置项名称
//            @szvalue表示配置值
//            @autoSave表示是否自动保存，true自动保存，false不保存
// 返 回 值:  true表示配置成功，false表示配置失败
//////////////////////////////////////////////////////////////////////////
bool SIniFile::SetKeyStringValue(SString sGroup, SString sField, SString szvalue, bool autoSave/*=true*/)
{
	SetValue(sGroup,sField,szvalue);
	if(autoSave && !this->WriteFile())
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  按打开时的文件名，保存INI文件
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:24
// 参数说明:  void
// 返 回 值:  true表示保存成功，false表示保存失败
//////////////////////////////////////////////////////////////////////////
bool SIniFile::WriteFile()
{
	if(m_bOpened == false)
		return false;
	SIniFile::stuIniFileGroup *pGroup;
	SIniFile::stuIniFileField *pField;
	pGroup = m_pFirstGroup;
	FILE *fp;;

	fp = fopen(m_sPathFile.data(),"w");
	if(!fp)
		return false;
	SString sLine;

	while(pGroup != NULL)
	{
		if(pGroup->sGroupName.size() > 0)
		{
			sLine = "[" + pGroup->sGroupName + "]\r\n";
			fwrite(sLine.data(),sizeof(char),sLine.size(),fp);
		}
		pField = pGroup->pFirstField;
		while(pField != NULL)
		{
			if(pField->sFieldName.size() > 0)
			{
				sLine = pField->sFieldName + "=" + pField->sValue + "\r\n";
				fwrite(sLine.data(),sizeof(char),sLine.size(),fp);
			}
			else
			{
				sLine = pField->sValue + "\r\n";
				fwrite(sLine.data(), sizeof(char), sLine.size(), fp);
			}
			pField = pField->pNext;
		}

		pGroup = pGroup->pNext;
	}
	fclose(fp);

	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  搜索指定的组,如果不存在允许自动创建
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:26
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @bCreate是否在组不存在的时候创建新的组,true允许创建，false不创建
// 返 回 值:  stuIniFileGroup*
//////////////////////////////////////////////////////////////////////////
SIniFile::stuIniFileGroup * SIniFile::GetGroup(SString sGroup,bool bCreate)
{
	SIniFile::stuIniFileGroup *pLast = m_pFirstGroup,*p = NULL;
	while(pLast != NULL)
	{
		if(pLast->sGroupName.compare(sGroup) == 0)
			break;
		p = pLast;
		pLast = pLast->pNext;
	}
	if(pLast == NULL && bCreate)
	{
		pLast = new SIniFile::stuIniFileGroup;
		pLast->sGroupName = sGroup;
		pLast->pNext = NULL;
		pLast->pFirstField = NULL;
		if(p == NULL)
			m_pFirstGroup = pLast;
		else
			p->pNext = pLast;
	}

	return pLast;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  投过指定配置项指定，如果不存在允许自动创建
// 作    者:  邵凯田
// 创建时间:  2011-11-13 16:28
// 参数说明:  @sGroup表示组名称(中括号的标题)
//            @sField表示配置项名称
//            @bCreate是否在组不存在的时候创建新的组或配置项,true允许创建，false不创建
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
SIniFile::stuIniFileField * SIniFile::GetField(SString sGroup,SString sField,bool bCreate)
{
	SIniFile::stuIniFileGroup *pGroup;
	SIniFile::stuIniFileField *pField,*p = NULL;
	pGroup = GetGroup(sGroup,bCreate);
	if(pGroup == NULL)
		return NULL;
	pField = pGroup->pFirstField;
	while(pField != NULL)
	{
		if(pField->sFieldName.compare(sField) == 0)
			break;
		p = pField;
		pField = pField->pNext;
	}
	if(pField == NULL && bCreate)
	{
		pField = new SIniFile::stuIniFileField;
		pField->pNext = NULL;
		pField->sFieldName = sField;
		pField->sValue = "";
		if(p == NULL)
			pGroup->pFirstField = pField;
		else
			p->pNext = pField;
	}
	return pField;
}
