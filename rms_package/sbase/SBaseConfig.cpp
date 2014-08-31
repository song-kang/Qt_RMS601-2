/**
 *
 * 文 件 名 : SBaseConfig.cpp
 * 创建日期 : 2008-03-24
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 从SBaseConfig移入
 * 修改记录 : 
 *            $Log: SBaseConfig.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/21 05:14:56  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *
 **/


#include "SBaseConfig.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SBaseConfig
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:00
// 描    述:  树形结构的配置类
//////////////////////////////////////////////////////////////////////////
SBaseConfig::SBaseConfig()
{
	SKTOBJECT("SBaseConfig");
	m_Attribute.setAutoDelete(true);
	m_ChildConfig.setAutoDelete(true);
	m_sNodeName = "";
	m_pParent = NULL;
}
SBaseConfig::~SBaseConfig()
{
	clear();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  清除当前节点的所有子节点及属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:02
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::clear()
{
	m_Attribute.clear();
	m_ChildConfig.clear();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点的路径，一直追朔到顶级节点
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:03
// 参数说明:  void
// 返 回 值:  返回使用"."分隔的路径名，各节点名称为m_sNodeName对应内容
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetPath()
{
	SString sPath=m_sNodeName;
	SBaseConfig *p = this->m_pParent;
	while(p)
	{
		sPath = p->m_sNodeName+"."+sPath;
		p = p->m_pParent;
	}
	return sPath;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  增加下级节点
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:04
// 参数说明:  @sNodeName子节点的名称
// 返 回 值:  返回新增加的子节点实例指针
//////////////////////////////////////////////////////////////////////////
SBaseConfig * SBaseConfig::AddChildNode(SString sNodeName/*=""*/)
{
	SBaseConfig *pNew = new SBaseConfig();
	pNew->m_sNodeName = sNodeName;
	m_ChildConfig.append(pNew);
	pNew->m_pParent = this;
	return pNew;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  插入新的节点，新节点必须是new出来的，插入后由当前实例负责释放
// 作    者:  邵凯田
// 创建时间:  2013:12:11 10:28
// 参数说明:  @pNew
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::AddChildNode(SBaseConfig *pNew)
{
	m_ChildConfig.append(pNew);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从当前实例移除指定的节点，但并不释放被移除的节点，调用者应负责释放
// 作    者:  邵凯田
// 创建时间:  2013:12:11 10:36
// 参数说明:  @pSub
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::RemoveChildNode(SBaseConfig *pSub)
{
	m_ChildConfig.setAutoDelete(false);
	bool b = m_ChildConfig.remove(pSub);
	m_ChildConfig.setAutoDelete(true);
	return b;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  判断当前节点及其子节点是否完全相同
// 作    者:  邵凯田
// 创建时间:  2013:12:12 16:13
// 参数说明:  @pSub为另一个节点
// 返 回 值:  true表示相同，false表示不同
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::IsNodeSame(SBaseConfig *pNode)
{
	if(this->GetChildCount() != pNode->GetChildCount())
		return false;
	if(this->GetAttributeCount() != pNode->GetAttributeCount())
		return false;
	if(this->GetNodeName() != pNode->GetNodeName())
		return false;
	if(this->GetNodeValue().trim() != pNode->GetNodeValue().trim())
		return false;
	SString sAttrName;
	int i,cnt = GetAttributeCount();
	//判断各个属性
	for(i=0;i<cnt;i++)
	{
		sAttrName = this->GetAttributeName(i);
		if(GetAttribute(sAttrName) != pNode->GetAttribute(sAttrName))
			return false;
	}
	for(i=0;i<cnt;i++)
	{
		sAttrName = pNode->GetAttributeName(i);
		if(GetAttribute(sAttrName) != pNode->GetAttribute(sAttrName))
			return false;
	}
	//判断各子节点
	cnt = GetChildCount();
	for(i=0;i<cnt;i++)
	{
		if(!GetChildPtr()->at(i)->IsNodeSame(pNode->GetChildPtr()->at(i)))
			return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  替换当前节点及其子节点的属性值，如果等于oldVal则替换为newVal
// 作    者:  邵凯田
// 创建时间:  2013:12:12 16:18
// 参数说明:  
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::ReplaceNodeAttrVal(SString &oldVal,SString  &newVal)
{
	int i,cnt = GetAttributeCount();
	SString sAttrName;
	//判断各个属性
	for(i=0;i<cnt;i++)
	{
		sAttrName = this->GetAttributeName(i);
		if(GetAttribute(sAttrName) == oldVal)
			SetAttribute(sAttrName,newVal);
	}
	cnt = GetChildCount();
	for(i=0;i<cnt;i++)
	{
		GetChildPtr()->at(i)->ReplaceNodeAttrVal(oldVal,newVal);
	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  增加下级节点
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:05
// 参数说明:  @sNodeName子节点的名称
// 返 回 值:  返回新增加的子节点实例指针
//////////////////////////////////////////////////////////////////////////
SBaseConfig * SBaseConfig::AddChildNodeEx(char* sNodeName/*=""*/)
{
	SBaseConfig *pNew = new SBaseConfig();
	pNew->m_sNodeName = sNodeName;
	m_ChildConfig.append(pNew);
	pNew->m_pParent = this;
	return pNew;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点属性,字符型
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:05
// 参数说明:  @sAttrName属性名称
//            @sAttrValue属性值
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttribute(SString sAttrName,SString sAttrValue)
{
	//先查找是否存在
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
		{
			m_Attribute[i]->m_sValue = sAttrValue;
			return;
		}
	}
	SBaseConfig::SConfigAttribute *pNew = new SConfigAttribute();
	pNew->m_sName = sAttrName;
	pNew->m_sValue = sAttrValue;
	m_Attribute.append(pNew);	
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点属性，整型
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:05
// 参数说明:  @sAttrName属性名称
//            @iAttrValue属性值
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeI(SString sAttrName,int iAttrValue)
{
	SetAttribute(sAttrName,SString::toString(iAttrValue));
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点属性，浮点型
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:05
// 参数说明:  @sAttrName属性名称
//            @fAttrValue属性值
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeF(SString sAttrName,float fAttrValue)
{
	SetAttribute(sAttrName,SString::toString(fAttrValue));
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点属性，void指针型
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:05
// 参数说明:  @sAttrName属性名称
//            @sAttrValue属性值
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeP(SString sAttrName,void* fAttrValue)
{
	SetAttribute(sAttrName,SString::toString((int)fAttrValue));
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点属性，字符型，高效函数
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:05
// 参数说明:  @sAttrName属性名称
//            @sAttrValue属性值
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetAttributeEx(char* sAttrName,char* sAttrValue)
{
	//先查找是否存在
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(strcmp(m_Attribute[i]->m_sName.data(), sAttrName) == 0)
		{
			m_Attribute[i]->m_sValue = sAttrValue;
			return;
		}
	}
	SBaseConfig::SConfigAttribute *pNew = new SConfigAttribute();
	pNew->m_sName = sAttrName;
	pNew->m_sValue = sAttrValue;
	m_Attribute.append(pNew);	
}


////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点名称
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:09
// 参数说明:  @s表示节点名称
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeName(SString s)
{
	m_sNodeName = s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点名称
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:09
// 参数说明:  @s表示节点名称
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeNameEx(char* s)
{
	m_sNodeName = s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点值
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:10
// 参数说明:  @s表示节点值
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeValue(SString s)
{
	m_sNodeValue = s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置节点值
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:11
// 参数说明:  @s表示节点值
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfig::SetNodeValueEx(char* s)
{
	m_sNodeValue = s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  测试是否当前节点存在指定的属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:13
// 参数说明:  @sAttrName表示属性名
// 返 回 值:  true表示属性存在，false表示属性不存在
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::IsAttribute(SString sAttrName)
{
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点下的指定的属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:14
// 参数说明:  @sAttrname表示属性名
// 返 回 值:  属性值，缺少此属性时返回空字符串
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetAttribute(SString sAttrName)
{
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
			return m_Attribute[i]->m_sValue;
	}
	return "";
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点下的指定的整型属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:15
// 参数说明:  @sAttrName表示属性名
// 返 回 值:  属性值，缺少时返回0
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::GetAttributeI(SString sAttrName)
{
	return GetAttribute(sAttrName).toInt();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点下的指定的浮点型属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:15
// 参数说明:  @sAttrName表示属性名
// 返 回 值:  属性值，缺少时返回0
//////////////////////////////////////////////////////////////////////////
float SBaseConfig::GetAttributeF(SString sAttrName)
{
	return GetAttribute(sAttrName).toFloat();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  引用方式取属性，字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:16
// 参数说明:  @sAttrName表示属性名
//            @sAttrValue表示属性值
// 返 回 值:  true表示属性存在，且值已经存入sAttrValue，false表示属性不存在
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::GetAttribute(SString sAttrName,SString &sAttrValue)
{
	int i,cnt=m_Attribute.count();
	for(i=0;i<cnt;i++)
	{
		if(m_Attribute[i]->m_sName == sAttrName)
		{
			sAttrValue = m_Attribute[i]->m_sValue;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  引用方式取属性，整型
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:16
// 参数说明:  @sAttrName表示属性名
//            @iAttrValue表示属性值
// 返 回 值:  true表示属性存在，且值已经存入iAttrValue，false表示属性不存在
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::GetAttribute(SString sAttrName,int &iAttrValue)
{
	SString val;
	if(!GetAttribute(sAttrName,val))
		return false;
	iAttrValue = val.toInt();
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  引用方式取属性，浮点型
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:16
// 参数说明:  @sAttrName表示属性名
//            @fAttrValue表示属性值
// 返 回 值:  true表示属性存在，且值已经存入fAttrValue，false表示属性不存在
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::GetAttribute(SString sAttrName,float &fAttrValue)
{
	SString val;
	if(!GetAttribute(sAttrName,val))
		return false;
	fAttrValue = val.toFloat();
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点下指定路径下的某个属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:18
// 参数说明:  @sNodePath表示节点路径，如：body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttrName表示属性名
// 返 回 值:  属性值
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetAttribute(SString sNodePath,char* sAttrName)
{
	SBaseConfig *pConfig = this->SearchChild(sNodePath);
	if(pConfig == NULL)
		return "";
	return pConfig->GetAttribute(sAttrName);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点下指定路径下的节点内容
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:19
// 参数说明:  @sNodePath表示节点路径，如：body.GROUPTITLE(0).GROUP<character='PARAM'>
// 返 回 值:  节点内容
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetNodeValue(SString sNodePath)
{
	SBaseConfig *pConfig = this->SearchChild(sNodePath);
	if(pConfig == NULL)
		return "";
	return pConfig->GetNodeValue();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点的属性数量
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:23
// 参数说明:  void
// 返 回 值:  属性数量
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::GetAttributeCount()
{
	return m_Attribute.count();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前节点的指定第index个属性的属性名
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:23
// 参数说明:  @index为属性序号，从0开始编号
// 返 回 值:  属性名
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::GetAttributeName(int index)
{
	if(index >= m_Attribute.count())
	{
		return "";
	}
	return m_Attribute[index]->m_sName;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取下级节点数量
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:25
// 参数说明:  @sNodeName表示待统计下级节点的名称，空表示全部节点
//            @sCondition表示统计条件，空格分隔多个条件，如：name1='val1' name2='val2'
// 返 回 值:  满足条件的数量
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::GetChildCount(SString sNodeName/*=""*/,SString sCondition/*=""*/)
{
	if(sNodeName == "" && sCondition == "")
		return m_ChildConfig.count();
	
	int i,cnt,size=0;
	cnt = m_ChildConfig.count();
	for(i=0;i<cnt;i++)
	{
		if(sNodeName.length()>0)
		{
			if(sNodeName != m_ChildConfig[i]->GetNodeName())
				continue;
			if(sCondition.length()>0 && !m_ChildConfig[i]->CheckNodeAttribute(sCondition))
				continue;
		}
		size ++;
	}
	return size;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取第index个下级节点,index从0开始
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:30
// 参数说明:  @index表示序号,index从0开始
//            @sNodeName表示待统计下级节点的名称，空表示全部节点
//            @sCondition表示统计条件，空格分隔多个条件，如：name1='val1' name2='val2'
// 返 回 值:  指定的子节点实例指针，无满足时返回NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfig::GetChildNode(int index,SString sNodeName/*=""*/,SString sCondition/*=""*/)
{
	if(sNodeName == "" && sCondition == "")
	{
		if(index >= m_ChildConfig.count())
			return NULL;
		return m_ChildConfig[index];
	}

	int i,size=0,cnt=m_ChildConfig.count();
	for(i=0;i<cnt;i++)
	{
		if(sNodeName != m_ChildConfig[i]->GetNodeName())
			continue;
		if(!m_ChildConfig[i]->CheckNodeAttribute(sCondition))
			continue;
		if(size >= index)
			return m_ChildConfig[i];
		size ++;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  根据属性条件取下级节点
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:31
// 参数说明:  @sNodeName表示节点名称
//            @sAttrName1表示过滤属性名1，空表示不使用此条件
//            @sAttrVal1表示过滤属性值1
//            其它字符同上
// 返 回 值:  指定的子节点实例指针，无满足时返回NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfig::GetChildNode(SString sNodeName,
				SString sAttrName1,SString sAttrVal1,
				SString sAttrName2,SString sAttrVal2,
				SString sAttrName3,SString sAttrVal3,
				SString sAttrName4,SString sAttrVal4,
				SString sAttrName5,SString sAttrVal5,
				SString sAttrName6,SString sAttrVal6)
{
	int i,cnt=m_ChildConfig.count();
	for(i=0;i<cnt;i++)
	{
		SBaseConfig* p = m_ChildConfig[i];
		if(p->m_sNodeName != sNodeName)
			continue;
		if(sAttrName1!="" && p->GetAttribute(sAttrName1) !=sAttrVal1 ||
			 sAttrName2!="" && p->GetAttribute(sAttrName2) !=sAttrVal2 ||
			 sAttrName3!="" && p->GetAttribute(sAttrName3) !=sAttrVal3 ||
			 sAttrName4!="" && p->GetAttribute(sAttrName4) !=sAttrVal4 ||
			 sAttrName5!="" && p->GetAttribute(sAttrName5) !=sAttrVal5 ||
			 sAttrName6!="" && p->GetAttribute(sAttrName6) !=sAttrVal6 )
			continue;
		return p;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  检查当前节点是否符合指定的条件
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:33
// 参数说明:  @sAttr表示节点属性的过滤字符串，空格分隔多个条件，如：name1='val1' name2='val2'
// 返 回 值:  true表示节点满足指定条件
//////////////////////////////////////////////////////////////////////////
bool SBaseConfig::CheckNodeAttribute(SString sAttr)
{
	int iCnt = sAttr.length();//GetAttributeCount();
	SString sAttrName="",sAttrVal,sTemp;
	int flag = 0;
	char sep='\'',ch;
	for(int i = 0; i < iCnt; i++)
	{
		ch = sAttr.at(i);
		if(flag == 0)//拼名称段
		{
			if(ch == '=')//结束拼名称
			{
				sAttrVal = "";
				flag = 1;
			}
			else if(ch == ' ')
				continue;
			else
				sAttrName += ch;
		}
		else if(flag == 1)//进入内容段的第一个字符
		{
			if(ch == '\'' || ch == '\"')
				sep = ch;//有分隔符
			else
				sep = ' ';//无分隔符
			flag = 2;
		}
		else
		{
			if(ch == sep)//结束
			{
				//begin 
				if(sAttrVal.length() > 0)//如果没有匹配值，则不使用此条件
				{
					sAttrVal = sAttrVal.lower();
					SString sTemp;
					sTemp = GetAttribute(sAttrName);
					sTemp = sTemp.lower();
					if(sTemp != sAttrVal)
						return false;
				}
				//end 
				sAttrName="";
				flag = 0;
			}
			else
				sAttrVal += ch;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  根据特定的层次字符串取节点指针
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:34
// 参数说明:  @sGroup表示节点路径，如：body.GROUPTITLE(0).GROUP<character='PARAM'>
// 返 回 值:  指定的子节点实例指针，无满足时返回NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfig::SearchChild(SString sGroup)
{
	int i,id,iCnt,iIndex,iPos1,iPos2;
	SString sNode,sName,sAttr,sTemp;
	SBaseConfig* pParent=NULL, *pNode = NULL;
	int pi=0;
	unsigned long pos;

	pParent = this;
	pNode = pParent->m_ChildConfig.at(0);

	iCnt = SString::GetAttributeCount(sGroup,".");	
	
	for(i = 1; i <= iCnt; i ++)
	{
		if(pNode == NULL)
			return NULL;

		sNode = SString::GetIdAttribute(i,sGroup,".");
		sNode = sNode.toLower();
		//check node name, whether it have index number,'(X)'
		if(sNode.find("(") > 0 && sNode.find(")",sNode.find("(")+1) > 0)
		{
			iPos1 = sNode.find("(");
			iPos2 = sNode.find(")",iPos1 + 1);
			sTemp = sNode.mid(iPos1+1, iPos2 - iPos1 - 1);
			iIndex = sTemp.toInt();
			sNode = sNode.left(iPos1);
		}
		else
			iIndex = 1;
		if(sNode.find("<") > 0 && sNode.find(">",sNode.find("<")+1) > 0)
		{
			iPos1 = sNode.find("<");
			iPos2 = sNode.find(">",iPos1 + 1);
			sAttr = sNode.mid(iPos1+1, iPos2 - iPos1 - 1);
			sNode = sNode.left(iPos1);
		}
		else
			sAttr = "";
		id = 0;
		pi=0;
#if 0 //老算法，效率不高
		while(1)
		{
			if(pNode == NULL)
			{
				return NULL;
			}
			sName = "";sName += pNode->GetNodeName();
			sName = sName.toLower();
			if(sNode == sName)
			{
				id ++;
				if(sAttr.length() == 0)
				{
					if(iIndex == id)
						//找到了
						break;
				}
				else//有属性设置
				{
					if(pNode->CheckNodeAttribute(sAttr))
						break;//找到了
				}
			}

			//goto next
			pNode = pParent->m_ChildConfig.at(++pi);
		}

#else	//MEMO: 使用Fetch方式 [2013:9:18 10:51 邵凯田]

		pNode = pParent->m_ChildConfig.FetchFirst(pos);
		while(1)
		{
			if(pNode == NULL)
			{
				return NULL;
			}
			sName = "";sName += pNode->GetNodeName();
			sName = sName.toLower();
			if(sNode == sName)
			{
				id ++;
				if(sAttr.length() == 0)
				{
					if(iIndex == id)
						//找到了
						break;
				}
				else//有属性设置
				{
					if(pNode->CheckNodeAttribute(sAttr))
						break;//找到了
				}
			}

			//goto next
			pNode = pParent->m_ChildConfig.FetchNext(pos);
		}

#endif
		//已经找到当前的节点pNode
		if(i == iCnt)
			return pNode;

		//goto son level
		pParent = pNode;
		pNode = pNode->m_ChildConfig.at(0);
	}

	return pNode;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  搜索指定节点的字符串属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:35
// 参数说明:  @sGroup表示节点路径，如：body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttr表示属性名称
// 返 回 值:  属性值
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::SearchNodeAttribute(SString sGroup,SString sAttr)
{
	SBaseConfig *pCfg = SearchChild(sGroup);
	if(pCfg == NULL)
		return "";
	return pCfg->GetAttribute(sAttr);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  搜索指定节点的整数属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:35
// 参数说明:  @sGroup表示节点路径，如：body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttr表示属性名称
// 返 回 值:  属性值
//////////////////////////////////////////////////////////////////////////
int SBaseConfig::SearchNodeAttributeI(SString sGroup,SString sAttr)
{
	return SearchNodeAttribute(sGroup,sAttr).toInt();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  搜索指定节点的浮点属性
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:35
// 参数说明:  @sGroup表示节点路径，如：body.GROUPTITLE(0).GROUP<character='PARAM'>
//            @sAttr表示属性名称
// 返 回 值:  属性值
//////////////////////////////////////////////////////////////////////////
float SBaseConfig::SearchNodeAttributeF(SString sGroup,SString sAttr)
{
	return SearchNodeAttribute(sGroup,sAttr).toFloat();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  搜索指定节点的值
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:36
// 参数说明:  @sGroup表示节点路径，如：body.GROUPTITLE(0).GROUP<character='PARAM'>
// 返 回 值:  匹配节点的节点值，无匹配节点时返回空字符串
//////////////////////////////////////////////////////////////////////////
SString SBaseConfig::SearchNodeValue(SString sGroup)
{
	SBaseConfig *pCfg = SearchChild(sGroup);
	if(pCfg == NULL)
		return "";
	return pCfg->GetNodeValue();
}



//////////////////////////////////////////////////////////////////////////
// 名    称:  SBaseConfigSet
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:40
// 描    述:  配置文件集合类
//////////////////////////////////////////////////////////////////////////

SBaseConfigSet::SBaseConfigSet()
{
	SKTOBJECT("SBaseConfigSet");
	m_ConfigSet.setShared(true);
	m_ConfigSet.setAutoDelete(true);
	m_ConfigName.setShared(true);
}
SBaseConfigSet::~SBaseConfigSet()
{
}

////////////////////////////////////////////////////////////////////////
// 描    述:  添加新的配置实例对象
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:41
// 参数说明:  @pCfg表示新配置实例的首节点指针
//            @sName表示此配置的名称
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SBaseConfigSet::AddConfig(SBaseConfig* pCfg,SString sName)
{
	m_ConfigSet.append(pCfg);
	m_ConfigName.append(sName);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取指定的配置实例对象指针
// 作    者:  邵凯田
// 创建时间:  2011-11-14 14:42
// 参数说明:  @sName等提取配置实例的名称
// 返 回 值:  目标配置实例的首节点指针，无匹配时返回NULL
//////////////////////////////////////////////////////////////////////////
SBaseConfig* SBaseConfigSet::GetConfig(SString sName)
{
	int i,cnt=m_ConfigName.count();
	for(i=0;i<cnt;i++)
	{
		if(*m_ConfigName[i] == sName)
		{
			return m_ConfigSet[i];
		}
	}
	return NULL;
}



