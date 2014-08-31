/**
 *
 * 文 件 名 : SPostgres.cpp
 * 创建日期 : 2007-07-24
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2009/01/16 06:16:02 $
 * 当前版本 : $Revision: 1.2 $
 * 功能描述 : Postgres数据库对SDatabase接口类的实现
 * 修改记录 : 
 *            $Log: SPostgres.cpp,v $
 *            Revision 1.2  2009/01/16 06:16:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/02 01:50:09  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.9  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.7  2007/10/19 06:22:25  shaokt
 *            读取大字段BUG修正
 *
 *            Revision 1.6  2007/10/10 13:29:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2007/10/09 12:00:00  shaokt
 *            语句优化
 *
 *            Revision 1.4  2007/09/21 07:53:41  shaokt
 *            基类更新
 *
 *            Revision 1.3  2007/09/14 01:26:13  shaokt
 *            修改存大字段时两个RESULT未释放的BUG
 *
 *            Revision 1.2  2007/09/11 08:17:53  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SPostgres.h"

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
SPostgres::SPostgres()
{
	SKTOBJECT("SPostgres");
	m_pConn = NULL;
	m_iTestTimes = 0;
}

SPostgres::~SPostgres()
{
	DisConnect();
}
//#include <server/mb/pg_wchar.h>
//#pragma comment(lib,")
bool SPostgres::Connect()
{
	if(m_pConn != NULL)
		DisConnect();

	SString sParam = GetParams();//hostaddr=127.0.0.1;port=5432;dbname=znl4;user=postgres;password=123456;
	SString sTemp,sTempNoPwd;
	if(SString::GetAttributeValue(sParam,"hostaddr").length() == 0)
		return false;
	sTemp.sprintf("hostaddr=%s port=%s dbname=%s user=%s password=%s connect_timeout=0",
		SString::GetAttributeValue(sParam,"hostaddr").data(),
		SString::GetAttributeValue(sParam,"port").data(),
		SString::GetAttributeValue(sParam,"dbname").data(),
		SString::GetAttributeValue(sParam,"user").data(),
		SString::GetAttributeValue(sParam,"password").data());
	sTempNoPwd.sprintf("hostaddr=%s port=%s dbname=%s user=%s password=%s connect_timeout=0",
		SString::GetAttributeValue(sParam,"hostaddr").data(),
		SString::GetAttributeValue(sParam,"port").data(),
		SString::GetAttributeValue(sParam,"dbname").data(),
		SString::GetAttributeValue(sParam,"user").data(),
		"******");

	LOGDEBUG("Begin to Connect Postgres:%s",sTempNoPwd.data());
	m_pConn = PQconnectdb(sTemp.data());		

	if (PQstatus(m_pConn) != CONNECTION_OK)
	{
		SetStatus(DBERROR);
		SString err;

		err.sprintf("Connect to postgres ERROR:%s params:%s",PQerrorMessage(m_pConn),sTempNoPwd.data());
		LOGERROR("%s",err.data());
		DisConnect();
		return false;
	}
	LOGDEBUG("Connect to postgres OK params:%s",sTempNoPwd.data());
	int ret = PQsetClientEncoding(m_pConn,"GBK");
	if(ret < 0)
	{
		LOGERROR("设置字符集为GBK时失败!");
	}
//	encoding = PQclientEncoding(m_pConn);
//	ret = PQsetClientEncoding(m_pConn,"UNICODE");
//	encoding = PQclientEncoding(m_pConn);
//	ret = PQsetClientEncoding(m_pConn,"SQL_ASCII");
//	encoding = PQclientEncoding(m_pConn);
//	ret = PQsetClientEncoding(m_pConn,"GB18030");
//	encoding = PQclientEncoding(m_pConn);
//	ret = PQsetClientEncoding(m_pConn,"UTF8");
//	encoding = PQclientEncoding(m_pConn);
	
	SetStatus(IDLE);

//	Execute("RESET CLIENT_ENCODING;");

//	SString sql = "insert into substation values('222',1,'新站','亲站','aa','aa','',1,0,0,'')";
//	Execute(sql);
	return true;
}

bool SPostgres::DisConnect()
{
	if(m_pConn != NULL)
	{
		PQfinish(m_pConn);
		m_pConn = NULL;
	}
	return true;
}

bool SPostgres::TestConnect()
{
	if(m_iTestTimes>0)
		return false;
	m_iTestTimes ++;
	if(SelectInto("select 'skt'") != "skt")
	{
		SetStatus(DBERROR);
		m_iTestTimes  = 0;
		return false;
	}
	SetStatus(IDLE);
	m_iTestTimes  = 0;
	return true;
}

int SPostgres::Retrieve(SString sql,SRecordset &rs)
{
	LOGBASEDEBUG("into SPostgres::Retrieve(%s)",sql.data());
	sql = sql.replace("\\","\\\\");//postgres数据库SQL中\为转义符
	if(m_pConn == NULL)
	{
		LOGWARN("m_pConn is NULL is SPostgres::Retrieve, Connect it at first!");
		Connect();
		if(m_pConn == NULL)
		{
			LOGWARN("m_pConn is NULL is SPostgres::Retrieve, Connect error!");
			return -1;
		}
	}
	PGresult *pRes = PQexec(m_pConn,sql.data());
	if(PQresultStatus(pRes) != PGRES_TUPLES_OK)
	{
		if(pRes != NULL)
			PQclear(pRes);
		if(TestConnect() == true)//连接可用
			return -1;
		//连接不可用自动重连一次数据库
		if(!Reconnect())
			return -2;//连接失败
		pRes = PQexec(m_pConn,sql.data());
		if(PQresultStatus(pRes) != PGRES_TUPLES_OK) 
		{
			if(pRes != NULL)
				PQclear(pRes);
			return -1;
		}
	}

	int cols = PQnfields(pRes);
	int rows = PQntuples(pRes);
	int i,j;
	SRecord *pRecord;
	//写入记录集
	rs.clear();
	rs.SetSize(rows,cols);
	for(i=0;i<cols;i++)
	{
		rs.SetColumnName(i,PQfname(pRes,i));
	}
	for(i=0;i<rows;i++)
	{
		pRecord = rs.GetRecord(i);
		for(j=0;j<cols;j++)
		{
			pRecord->SetValue(j,PQgetvalue(pRes,i,j));
		}
	}
	PQclear(pRes);
	
	return rows;
}

bool SPostgres::Execute(SString sql)
{
	LOGBASEDEBUG("into SPostgres::Execute(%s)",sql.data());
	sql = sql.replace("\\","\\\\");//postgres数据库SQL中\为转义符
	if(m_pConn == NULL)
	{
		LOGWARN("m_pConn is NULL is SPostgres::Execute, Connect it at first!");
		Connect();
		if(m_pConn == NULL)
		{
			LOGWARN("m_pConn is NULL is SPostgres::Execute, Connect error!");
			return false;
		}
	}
	 
	PGresult *pRes = PQexec(m_pConn,sql.data());
	if(PQresultStatus(pRes) != PGRES_COMMAND_OK )
	{
		SString err;
		err.sprintf("Error in SPostgres::Execute(%s), err=%s",sql.data(),PQresultErrorMessage(pRes));
		LOGERROR("%s",err.data());
		if(pRes != NULL)
			PQclear(pRes);
		if(TestConnect() == true)//连接可用
			return false;

		//失败自动重连一次数据库
		if(!Reconnect())
			return false;//连接失败
		pRes = PQexec(m_pConn,sql.data());
		if(PQresultStatus(pRes)!=PGRES_COMMAND_OK) 
		{
			LOGERROR("Error in SPostgres::Execute(%s), err=%s",sql.data(),PQresultErrorMessage(pRes));
			if(pRes != NULL)
				PQclear(pRes);
			return false;
		}
	}
	PQclear(pRes);
	return true;
}

//update wave_detail_rpt set efile=%d where 
bool SPostgres::UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	LOGBASEDEBUG("Into SPostgres::UpdateLobFromFile(%s,%s,%s,%s)",
						sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
	SString sql;
	Oid oid;
	SString param;
	PGresult *pRes = PQexec(m_pConn, "begin");
	if(pRes == NULL)
	{
		if(TestConnect() == true)//连接可用
		{
			LOGERROR("Error in SPostgres::UpdateLobFromFile, exec begin error, err=%s", PQerrorMessage(m_pConn));
			return false;
		}
		//失败自动重连一次数据库
		if(!Reconnect())
		{
			LOGERROR("Error in SPostgres::UpdateLobFromFile, exec begin error, err=%s", PQerrorMessage(m_pConn));
			return false;
		}
		pRes = PQexec(m_pConn, "begin");
		if(pRes == NULL)
		{
			LOGERROR("Error in SPostgres::UpdateLobFromFile, exec begin error, err=%s", PQerrorMessage(m_pConn));
			return false;
		}
	}
	PQclear(pRes);
	oid = lo_import(m_pConn, sFile.data());
	if(oid == 0)
	{
		LOGERROR("Error in SPostgres::UpdateLobFromFile(%s,%s,%s,%s), err=%s",
			sTable.data(),sLobField.data(),sWhere.data(),sFile.data(),PQerrorMessage(m_pConn));
		pRes = PQexec(m_pConn, "end");
		if(pRes != NULL)
			PQclear(pRes);
		return false;
	}
	sql.sprintf("update %s set %s=%d where %s",sTable.data(),sLobField.data(),oid,sWhere.data());
	pRes = PQexec(m_pConn, sql.data());
	LOGBASEDEBUG("%s",sql.data());
	if(pRes == NULL || PQresultStatus(pRes) != PGRES_COMMAND_OK )
	{
		LOGERROR("Error in SPostgres::UpdateLobFromFile(%s,%s,%s,%s), sql=%s, err=%s",
			sTable.data(),sLobField.data(),sWhere.data(),sFile.data(),sql.data(),PQerrorMessage(m_pConn));
		lo_unlink(m_pConn,oid);
		if(pRes != NULL)
			PQclear(pRes);
		pRes = PQexec(m_pConn, "end");
		if(pRes != NULL)
			PQclear(pRes);
		return false;
	}
	PQclear(pRes);//add by skt at 2007-9-13不清除将导致内存缓慢增长
	pRes = PQexec(m_pConn, "end");
	if(pRes != NULL)
		PQclear(pRes);	
	return pRes != NULL;
}

bool SPostgres::ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile)
{
	LOGBASEDEBUG("Into SPostgres::ReadLobToFile(%s,%s,%s,%s)",
						sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
	SString sql;
	sql.sprintf("select %s from %s where %s",sLobField.data(),sTable.data(),sWhere.data());
	Oid oid = SelectIntoI(sql);
	if(oid == 0)
	{
		LOGERROR("Into SPostgres::ReadLobToFile(%s,%s,%s,%s) get oid error, oid=0",
							sTable.data(),sLobField.data(),sWhere.data(),sFile.data());
		return false;
	}
	PGresult *pRes = PQexec(m_pConn, "begin");
	if(pRes == NULL)
	{
		if(TestConnect() == true)//连接可用
		{
			LOGERROR("Error in SPostgres::ReadLobToFile, exec begin error, err=%s", PQerrorMessage(m_pConn));
			return false;
		}
		//失败自动重连一次数据库
		if(!Reconnect())
		{
			LOGERROR("Error in SPostgres::ReadLobToFile, exec begin error, err=%s", PQerrorMessage(m_pConn));
			return false;
		}
		pRes = PQexec(m_pConn, "begin");
		if(pRes == NULL)
		{
			LOGERROR("Error in SPostgres::ReadLobToFile, exec begin error, err=%s", PQerrorMessage(m_pConn));
			return false;
		}
	}
	PQclear(pRes);
	pRes = NULL;
	if(lo_export(m_pConn,oid,sFile.data()) == 0)
	{
		LOGERROR("Error in SPostgres::ReadLobToFile(%s,%s,%s,%s), lo_export error, err:%s",
							sTable.data(),sLobField.data(),sWhere.data(),sFile.data(),PQerrorMessage(m_pConn));
		pRes = PQexec(m_pConn, "end");
		if(pRes != NULL)
			PQclear(pRes);
		return false;
	}
	if(pRes != NULL)
		PQclear(pRes);

	pRes = PQexec(m_pConn, "end");
	if(pRes != NULL)
		PQclear(pRes);	
	return pRes != NULL;
}


