/**
 *
 * 文 件 名 : SOracle.h
 * 创建日期 : 2007-11-24
 * 作    者 : 邵凯田
 * 修改日期 : $Date: 2007/11/24 13:29:08 $
 * 当前版本 : $Revision: 1.3 $
 * 功能描述 : Oracle数据库对SDatabase接口类的实现
 * 修改记录 : 
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef _SKT_ORACLE_
#define _SKT_ORACLE_

#include "../SDatabase.h"
#include "../../SLog.h"

#ifdef WIN32
#pragma warning(disable:4786)
#endif

/*
  1.执行一个Sql语句时,如果是Update,Insert,Delete等不需要绑定参数的语句,可以采用下列步骤:
     string Sql="update table1 set table1_a='123' where table1_b=1985 "
     SqlExecute(Sql语句);
     Commit;
  2.执行一个Sql的Select语句,但不需要绑定参数的,可以采用下列步骤:
     string sql="select a,b,c from table2 where recordindex='111111' ";
     OdbRecordSet rs; long hStmt=0;
     sword status=Select(Sql,rs,hStmt);
     while(status==0)
     {
         status=FetchNext(hStmt,rs);	 a_value=rs.GetFieldValue(0);	 b_value=rs.GetFieldValue(1);	 c_Value=rs/GetFieldValue("c");
     }
     ClearRecordSets(hStmt,rs);
   3.执行一个Sql的Select语句,需要绑定参数的,可以采用下列步骤:  
     定义参数->准备Sql(SqlPrepare)->绑定参数->执行Sql->定义输出->取集录
     例如:
     string sql="select a,b,c from table2 where recordindex=:1"; //参数:1可以在执行时赋值
     OdbRecordSet rs; long hStmt=0;
     //定义参数
     OdbParamArrary oa;
     OdbParam* pParam=new OdbParam;
     pParam->wType=SQLT_STR; pParam->Set("111111");  oa.paParams.push_back(pParam);
     //
     db.SqlPrepare(sele,hStmt);     db.BindParam(hStmt,oa);     db.SqlExecuteWithParam(hStmt);     status=db.DefineColumn(hStmt,rs);
     while(status==0)
     {
         status=FetchNext(hStmt,rs);	 a_value=rs.GetFieldValue(0);	 b_value=rs.GetFieldValue(1);	 c_Value=rs/GetFieldValue("c");
     }
     ClearRecordSets(hStmt,rs);  db.FreeParams(hStmt,oa);    
   4.执行一个带有CLOB/BLOB查询结果的语句
     与上面的方法一样,但在取集录时不同,例如:
    string sele="select note from unionnote_rpt";
    long Stmt;  OdbRecordSet rs; db.Select(sele,rs,Stmt);
    while(db.FetchNext(Stmt,rs)==0)
    {
	for(int i=0;i<rs.paFields.size();i++)
	{
	    OdbField* pField=rs.paFields[i];
	    if(pField->wType==SQLT_CLOB||pField->wType==SQLT_BLOB)
	       db.ReadLobToFile(pField,"/home/temp.txt");   //将内容保存到文件.此处也可以:StringArray oa;db.ReadCLob(pField,oa);读到字符数组中
        }
    }
    db.ClearRecordSets(Stmt,rs);
    return 1;
   5.执行一个带有CLOB/BLOB的Update语句,直接写入内容
     与上面的方法一样,但在取集录时不同,例如:
    string sele="Update unionnote_rpt set Note=:1";
    long gg;   OdbParamArray oa;
    OdbParam* pParam=new OdbParam;    pParam->wType=SQLT_CLOB;  pParam->BindPos=1; oa.paParams.push_back(pParam);
    db.SqlPrepare(sele,gg);   db.BindParam(gg,oa);
    StringArrary sac;   sac.push_back("we are mark1");  sac.push_back("we are mark2"); sac.push_back("we are mark3"); sac.push_back("we are mark4");
    db.WriteCLob(pParam,sac);
    db.SqlExecuteWithParam(gg); db.Commit(); db.FreeParams(gg,oa);
   6.执行一个带有CLOB/BLOB的Update语句,将文件内容保存到CLOB/BLOB字段中
    string sele="Update unionnote_rpt set Note=:1";string filename='/home/temp.txt';
    long gg;   OdbParamArray oa;
    OdbParam* pParam=new OdbParam;    pParam->wType=SQLT_CLOB;  pParam->BindPos=1; oa.paParams.push_back(pParam);
    db.SqlPrepare(sele,gg);   db.BindParam(gg,oa);
    WriteLobFromFile(pParam,filename);
    db.SqlExecuteWithParam(gg); db.Commit(); db.FreeParams(gg,oa);
   7.执行一个存储过程的用法,存储过程testp(a1 in varchar2,a2 in/out varchar2)
    string sele="begin testp(:1,:2); end;";
    long gg;   OdbParamArray oa;
    OdbParam* pParam=new OdbParam;    pParam->wType=SQLT_STR;  pParam->BindPos=1; oa.paParams.push_back(pParam);
              pParam=new OdbParam;    pParam->wType=SQLT_STR;  pParam->BindPos=2; oa.paParams.push_back(pParam);
    db.SqlPrepare(sele,gg);   db.BindParam(gg,oa);
    db.SqlExecuteWithParam(gg); db.FreeParams(gg,oa);
    //a2是输出变量
*/


#include <stdio.h>
#include <stdlib.h>
#include <oci.h>
#include <string>
#include <vector>

#include <sys/timeb.h>
#include <time.h>

#ifndef lint 
typedef         ub4      duword;                   
typedef         sb4      dsword;                 
typedef         dsword   dword;

#else 
#define duword ub4 
#define dsword sb4 
#define dword  dsword
#endif  

#define READ_FILE_LENGTH 1024
typedef std::vector<std::string> StringArrary;
	struct OdbField
{
    std::string 	Field_Name;					//字段名称
    dvoid* 	pBuffer;						//内容
    ub2   	wType;						//字段类型
    ub2		wSize;						//字段长度
    ub2		wLen;						//内容长度
    sb2		nInd;						//顺序
    OCILobLocator* oLocator;					//LOB描述器
    std::string   FileName;
    bool	bQuotedOnUpdate;				//不使用
    dword   status;
    OdbField();							//构造
    ~OdbField();							//析构
    void SetSize(ub2 w);						//产生pBuffer的内存,大小 w
    void Set(std::string cs);					//将cs的值付给pBuffer
    std::string Get() const;						//得到pBuffer的值,返回string
    void Free(int free);						//释放pBuffer及oLocator
};

struct OdbRecordSet
{
    std::vector<OdbField*> paFields;				//被选择集录的结果字段集合
    int             m_nRows;					//行数
    std::string           m_csStatement;				//
    
    OdbRecordSet();
    ~OdbRecordSet();
	int GetFieldCount();
    int Find(const std::string &csName);				//查找一个字段在集合中的位置
    std::string GetFieldValue(const std::string csName);		//得到一个字段的值
    std::string GetFieldValue(int iField);				//得到第几个字段的值
    void RemoveAll();						//清除字段的结果集
};

struct OdbParam
{
    dvoid* 	pVar;						//绑定变量
    ub2   	wType;						//变量类型
    ub2		wSize;						//变量长度
    ub2		wLen;						//内容长度
    ub2		BindPos;						//绑定位置
    std::string   BindName;					//绑定的名称
    ub2       By;                                       //通过什么方式绑定:0=位置;1=名称
    ub2       InOut;						//输出输入0=输入,1=输出;2=输入输出
    OCIBind*  bindhp;						//绑定句柄
    OCILobLocator *oLocator;					//CLOB.BLOB指示器
    std::string   FileName;
    OdbParam();
    ~OdbParam();
    void SetSize(ub2 w);
    void Set(std::string cs);
    void Set(dvoid* mem,int len);
    std::string Get() const;
    void Free(int free);
};

struct OdbParamArray
{
    std::vector<OdbParam*> paParams;				//绑定变量的集合
    OdbParamArray();						
    ~OdbParamArray();
    void SetFieldValue(int iParam,const std::string cs);		//设定第几个变量的值
    std::string GetFieldValue(int iParam);				//得到第几个变量的值
    void RemoveAll();
};

class CDataBase
{
    public:
    CDataBase();
    CDataBase(char* UserName,char* PassWord,char* DbName);
    ~CDataBase();
    public:
    bool ConnectDataBase();
    bool ConnectDataBase(const char* UserName,const char* PassWord,const char* DbName);
    bool ConnectDataBase(const char* UserName,const char* PassWord,const char* DbName,long *pHenv,long *pHdbc);   //联接数据库
    bool DisConnectDataBase();
    //查询数据库内容
    long Select(const std::string Stmt,OdbRecordSet &rs,long &hpSelect/*Stmt handle*/);			 //查询
    sword FetchNext(long hpSelect,OdbRecordSet &rs);    						 //取结果
    void ClearRecordSets(long hpSelect,OdbRecordSet &rs);						 //清除结果
    //执行Sql语句
    long SqlExecute(const std::string Stmt);								 //执行SQL语句
    long Commit();                       							 //如果Insert,Update,Delete提交
    long Rollback();										 //回滚
    //执行绑定参数的Sql语句
    long SqlPrepare(const std::string Stmt,long &hpStmt);						 //准备Sql语句
    long DefineColumn(long hpSelect,OdbRecordSet &rs);						 //定义输出变量
    long BindParam(long hpStmt,OdbParamArray &params);						 //绑定参数
    long SqlExecuteWithParam(long hpStmt); 							 //执行Sql语句
    void FreeParams(long hpStmt,OdbParamArray &rs);						 //释放句柄及参数
    //得到满足条件结果的记录数
    long SqlGetRecordCount(const std::string Stmt);
    long SqlGetRecordCount(const std::string Table,const std::string Where);
    //LOB字段的处理函数
    long SqlGetLobLength(OdbField* pField);							//得到LOB字段内容长度
    long SqlReadLob(OdbField* pParam,long Offset,long iRead,long &ReadCount);			//读LOB字段
    long SqlReadLob(OdbRecordSet &rs,long Offset,long iRead,long &ReadCount);			//读LOB字段
    long SqlWriteLob(OdbParam* pParam,long Offset,long iWrite,long &WriteCount);			//写LOB字段
    long SqlWriteLob(OdbParamArray &rs,long Offset,long iWrite,long &WriteCount);			//写LOB字段
    //
    long ReadCLob(OdbField* pField,StringArrary &sa);
    long ReadLobToFile(OdbField* pField,std::string FileName);
    long WriteCLob(OdbParam* pParam,StringArrary &sa);
    long WriteLobFromFile(OdbParam* pParam,std::string FileName);
    //
    long GetFileLength(std::string FileName);
    long BindLongRaw(long hpStmt,OdbParamArray &params);
    long ExecLongRawFile(long hpStmt,OdbParam* pParam);
    long ReadLongRawToFile(long hpStmt,std::string Filename);
public:
    std::string GetErrorText();
    std::string MakeRecordIndex(int nSubNo, int nDevNo, int nCpuNo);								//得到错误信息
    std::string Format(const char* fmt,...);
    public:
    std::string m_UserName;
    std::string m_PassWord;
    std::string m_DbName;
    long  m_pHenv;
    long  m_pHdbc;
    private:
    std::string checkerr(OCIError* pErr, sword status,bool bPrint=true);
    private:
    OCIEnv *envhp;
    OCIError *errhp;
    OCIServer *srvhp;
    OCISvcCtx *svchp;
    OCISession *authp;
    std::string errText;
};


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#pragma comment(lib,"oci.lib")
//#pragma comment(lib,"ociw32.lib")
#endif

#ifndef NULL
#define NULL 0
#endif

class SOracle  : public SDatabase
{
private:
	CDataBase* m_pConn;
	int m_iTestTimes;
public:
	SOracle();
	virtual ~SOracle();

	virtual bool Connect();
	virtual bool DisConnect();
	virtual bool TestConnect();
	virtual int Retrieve(SString sql,SRecordset &rs);
	virtual bool Execute(SString sql);

	virtual bool UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);
	virtual bool ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	virtual bool ReadLongRawToFile(SString sTable,SString sLobField,SString sWhere,SString ysFile);
	virtual bool UpdateLongRawFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile);

	bool readLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value);
	bool updateLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value);

	SString getColumnType(SString tableName,SString fieldName);

};

#endif// _SKT_ORACLE_





