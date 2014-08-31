/**
 *
 * 文 件 名	: SDatabaseOper.cpp
 * 创建日期	: 2008-06-16
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: $
 * 当前版本	: $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/

#include "SDatabaseOper.h"

CSDatabaseOper::CSDatabaseOper()
{
}

CSDatabaseOper::~CSDatabaseOper()
{
}

void CSDatabaseOper::SetDatabasePool(SDatabasePool<SDatabase> *pPool)
{
	m_pDatabasePool = pPool;
}

SDatabase* CSDatabaseOper::GetDatabase(bool bTestConnect)
{
	SDatabase* p = m_pDatabasePool->GetDatabase(bTestConnect);
	return p;
}

//释放连接
void CSDatabaseOper::ReleaseDatabase(SDatabase* pDB)
{
	m_pDatabasePool->Release(pDB);//只要任意调用一个池的Release即可，因为内部只是对pDB下的连接unlock，跟池无关
}

//取记录集，失败回返<0
int CSDatabaseOper::RetrieveRecordset(SString sql,SRecordset &rs)
{
	SDatabase *pDB = GetDatabase();
	if(pDB == NULL)
	{
		LOGWARN("无法获取可用的数据库连接!取记录集失败!sql=%s",sql.data());
		return -1;
	}
	int cnt = pDB->Retrieve(sql,rs);
	ReleaseDatabase(pDB);
	return cnt;
}

int CSDatabaseOper::SelectIntoI(SString sql,int col/*=0*/)
{
	return SelectInto(sql,col).toInt();
}
float CSDatabaseOper::SelectIntoF(SString sql,int col/*=0*/)
{
	return SelectInto(sql,col).toFloat();
}
//取记录集，失败回返<0
SString CSDatabaseOper::SelectInto(SString sql,int col/*=0*/)
{
	SDatabase *pDB = GetDatabase();
	if(pDB == NULL)
	{
		LOGWARN("无法获取可用的数据库连接!取记录集失败!sql=%s",sql.data());
		return "";
	}
	SString ret = pDB->SelectInto(sql,col);
	ReleaseDatabase(pDB);
	return ret;
}

//执行SQL，成功返回true,失败返回false
bool CSDatabaseOper::ExecuteSQL(SString sql)
{
	SDatabase *pDB = GetDatabase();
	if(pDB == NULL)
	{
		LOGWARN("无法获取可用的数据库连接!执行SQL失败!sql=%s",sql.data());
		return false;
	}
	bool cnt = pDB->Execute(sql);
	ReleaseDatabase(pDB);
	return cnt;
}


