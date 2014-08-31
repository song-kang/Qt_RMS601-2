/**
 *
 * 文 件 名	: SDatabaseOper.h
 * 创建日期	: 2008-06-16
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: $
 * 当前版本	: $Revision: $
 * 功能描述 : 数据库操作助手类
 * 修改记录 : 
 *            $Log: $
 *
 **/

#if !defined(AFX_SKTDATABASE_OPER_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
#define AFX_SKTDATABASE_OPER_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_

#include "SDatabase.h"

class CSDatabaseOper
{
public:

	CSDatabaseOper();
	~CSDatabaseOper();

	void SetDatabasePool(SDatabasePool<SDatabase> *pPool);

	//取数据库连接实例，bTestConnect=true时表示得到空闲连接时先检测可用性，若失败返回NULL
	//bTestConnect=false时不检测，直接使用，由上层程序再次调用GetDatabase(true)取从库连接
	SDatabase* GetDatabase(bool bTestConnect=false);

	//释放连接
	void ReleaseDatabase(SDatabase* pDB);

	//取记录集，失败回返<0
	int RetrieveRecordset(SString sql,SRecordset &rs);

	int SelectIntoI(SString sql,int col=0);
	float SelectIntoF(SString sql,int col=0);
	SString SelectInto(SString sql,int col=0);

	//执行SQL，成功返回true,失败返回false
	bool ExecuteSQL(SString sql);

private:
	SDatabasePool<SDatabase> *m_pDatabasePool;

};


#endif // !defined(AFX_SKTDATABASE_OPER_H__D6BE0D97_13A8_11D4_A5D2_002078B03530__INCLUDED_)
