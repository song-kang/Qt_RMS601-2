/**
 *
 * 文 件 名 : SOracle.cpp
 * 创建日期 : 2007-11-4
 * 作    者 : 邵凯田
 * 修改日期 : $Date: 2007/11/24 07:30:23 $
 * 当前版本 : $Revision: 1.8 $
 * 功能描述 : Postgres数据库对YsDatabase接口类的实现
 * 修改记录 : 
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SOracle.h"

#ifndef uint
#define uint unsigned int 
#endif

using std::vector;
using std::string;

OdbField::OdbField()
{
    pBuffer=NULL;
    wType=SQLT_STR;
    wSize=0;
    wLen=0;
    nInd=0;
    bQuotedOnUpdate=TRUE;
    oLocator=NULL;
}

OdbField::~OdbField()
{
    Free(0);
}

void OdbField::SetSize(ub2 w)
{
    if(w>wSize||!pBuffer)
    {
	Free(1);
	pBuffer = (dvoid*)new char[w+1];
	memset(pBuffer,0x00,w+1);
	wSize=w;
    }
}

void OdbField::Set(string cs)
{
    wLen=cs.size()+1;
    SetSize(wLen);
    pBuffer=(dvoid*)cs.c_str();
}

string OdbField::Get() const
{
    string s;
    if(wLen>0)
    {
	char* buff=new char[wLen+1];
	memset(buff,0x00,wLen);
	sprintf(buff,"%.*s",wLen,(char*)pBuffer);
	s=buff;
	delete []buff;
    }
    return s;
}

void OdbField::Free(int free)
{
    if(pBuffer&&(free==0||free==1))
    {
	delete [](char*)pBuffer;pBuffer=NULL;
    }
    if(oLocator&&(free==0||free==2))
    {
	OCIDescriptorFree((dvoid*) oLocator,(ub4) OCI_DTYPE_LOB);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OdbRecordSet::OdbRecordSet()
{
    
}

OdbRecordSet::~OdbRecordSet()
{
    RemoveAll();
}

void OdbRecordSet::RemoveAll()
{
    for(uint i=0;i<paFields.size();i++)
    {
		OdbField *p=paFields[i];
		delete p;
    }
    paFields.clear();
}

int OdbRecordSet::GetFieldCount()
{
    return paFields.size();
}

int OdbRecordSet::Find(const string &csName)
{
    for(uint i=0;i<paFields.size();i++)
    {
	if(paFields[i]->Field_Name==csName) return i;
    }
    return -1;
}

string OdbRecordSet::GetFieldValue(int iField)
{
    if(iField<0||iField>=paFields.size()) return "";
    return (string)paFields[iField]->Get();
}

string OdbRecordSet::GetFieldValue(const string csName)
{
    int iField=Find(csName);
    if(iField==-1) return "";
    return (string)paFields[iField]->Get();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
OdbParam::OdbParam()
{
    pVar=NULL;
    wType=SQLT_STR;
    wSize=0;
    InOut=0;
    BindPos=1;
    BindName="";
    By=0;
    bindhp=NULL;
    oLocator=NULL;
}

OdbParam::~OdbParam()
{
    Free(0);
}

void OdbParam::SetSize(ub2 w)
{
    if(w>wSize||!pVar)
    {
	Free(1);
	pVar = (dvoid*)new char[w+1];
	memset(pVar,0x00,w+1);
	wSize=w;
    }
}

void OdbParam::Set(string cs)
{
    if(wType==SQLT_INT)
    {
	wLen=sizeof(int);
	SetSize(wLen);
	int c=atol(cs.c_str());
	memcpy(pVar,(dvoid*)&c,sizeof(int));
	printf("code=;%d\n",c);
    }
    else
    {
	wLen=cs.size()+1;
	SetSize(wLen);
	memcpy(pVar, (dvoid*)cs.c_str(), cs.size());
    }
}

void OdbParam::Set(dvoid* mem,int len)
{
    SetSize(len);wLen=len;
    memcpy(pVar,mem,len);
}

string OdbParam::Get() const
{
    string s;
    if(wLen>0)
    {
	char* buff=new char[wLen+1];
	memset(buff,0x00,wLen);
	sprintf(buff,"%.*s",wLen,(char*)pVar);
	s=buff;
	delete []buff;
    }
    return s;
}

void OdbParam::Free(int free)
{
    if(pVar&&(free==0||free==1))
    {
	delete [](char*)pVar;pVar=NULL;
    }
    if(oLocator&&(free==0||free==2))
    {
	OCIDescriptorFree((dvoid*) oLocator,(ub4) OCI_DTYPE_LOB);
    }
}

OdbParamArray::OdbParamArray()
{
    
}

OdbParamArray::~OdbParamArray()
{
    RemoveAll();
}

void OdbParamArray::SetFieldValue(int iParam,const string cs)
{
    if(iParam < 0||iParam >= paParams.size()) return;
    paParams[iParam]->Set(cs);
}

std::string OdbParamArray::GetFieldValue(int iParam)
{
    if(iParam < 0||iParam >= paParams.size()) return "";
    return (string)paParams[iParam]->Get();

}

void OdbParamArray::RemoveAll()
{
    for(uint i=0;i<paParams.size();i++)
    {
	delete paParams[i];
    }
    paParams.clear();
}


CDataBase::CDataBase(char* UserName,char* PassWord,char* DbName)
{
    m_UserName=UserName;
    m_PassWord=PassWord;
    m_DbName=DbName;
    authp = (OCISession *) 0;svchp=NULL;srvhp=NULL;errhp=NULL;envhp=NULL;
}

CDataBase::CDataBase()
{
    authp = (OCISession *) 0;svchp=NULL;srvhp=NULL;errhp=NULL;envhp=NULL;
}

CDataBase::~CDataBase()
{
    OCIServerDetach(srvhp,errhp,OCI_DEFAULT);
    if (authp)
	(void) OCIHandleFree((dvoid *) authp, OCI_HTYPE_SESSION);
    if (svchp)
	(void) OCIHandleFree((dvoid *) svchp, OCI_HTYPE_SVCCTX);
    if (srvhp)
	(void) OCIHandleFree((dvoid *) srvhp, OCI_HTYPE_SERVER);
    if (errhp)
	(void) OCIHandleFree((dvoid *) errhp, OCI_HTYPE_ERROR);
    if (envhp)
	(void) OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);
}

string CDataBase::Format(const char* fmt,...)
{
    char buff[10240];string rets="";
    memset(buff,0x00,10240);
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buff,fmt,ap);
    va_end(ap);
    rets=buff;
    return rets;
}

string CDataBase::checkerr(OCIError* errhp, sword status,bool bPrint)
{
  if(!errhp) return "错误环境句柄非法";
  text errbuf[512]; sb4 errcode = 0;
  string smesg;
  switch (status)
  {
  case OCI_SUCCESS:
      smesg="执行成功-OCI_SUCCESS";
      if(bPrint) (void) printf("SUCCESS - OCI_SUCCESS\n");
      break;
  case OCI_SUCCESS_WITH_INFO:
      smesg="执行成功,但是有些问题-OCI_SUCCESS_WITH_INFO";
      if(bPrint) (void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
    break;
  case OCI_NEED_DATA:
      smesg="执行时需要数据-OCI_NEED_DATA";
      if(bPrint) (void) printf("Error - OCI_NEED_DATA\n");
      break;
  case OCI_NO_DATA:
      smesg="没有数据-OCI_NO_DATA";
      if(bPrint) (void) printf("Error - OCI_NODATA\n");
      break;
  case OCI_ERROR:
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    smesg=(char*)errbuf;
    if(bPrint) (void) printf("Error - %.*s\n", 512, errbuf);
    break;
  case OCI_INVALID_HANDLE:
    smesg="非法的句柄-OCI_INVALID_HANDLE";
    if(bPrint) (void) printf("Error - OCI_INVALID_HANDLE\n");
    break;
  case OCI_STILL_EXECUTING:
    smesg="正在执行-OCI_STILL_EXECUTE";
    if(bPrint) (void) printf("Error - OCI_STILL_EXECUTE\n");
    break;
  case OCI_CONTINUE:
    if(bPrint) (void) printf("Error - OCI_CONTINUE\n");
    break;
  default:
    break;
  }
  return smesg;
}

string CDataBase::GetErrorText()
{
    return errText;
}

bool CDataBase::ConnectDataBase()
{
    return ConnectDataBase((char*)m_UserName.c_str(),(char*)m_PassWord.c_str(),(char*)m_DbName.c_str());
}

bool CDataBase::ConnectDataBase(const char* UserName,const char* PassWord,const char* DbName)
{
    long pHenv=0,pHdbc=0;
    return ConnectDataBase(UserName,PassWord,DbName,&pHenv,&pHdbc);
}

bool CDataBase::ConnectDataBase(const char* UserName,const char* PassWord,const char* DbName,long *pHenv,long *pHdbc)
{
    //初始化OCI环境
    sword status=0;
    OCIInitialize((ub4) OCI_THREADED|OCI_DEFAULT, (dvoid *)0,
			 (dvoid * (*)(dvoid *, size_t)) 0,
			 (dvoid * (*)(dvoid *, dvoid *, size_t))0,
			 (void (*)(dvoid *, dvoid *)) 0 );
    (void) OCIEnvInit( (OCIEnv **) &envhp, OCI_DEFAULT, (size_t) 0,
		       (dvoid **) 0 );
    
    (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &errhp, OCI_HTYPE_ERROR,
			   (size_t) 0, (dvoid **) 0);
    /*分配数据服务句柄*/
    status=OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &srvhp, OCI_HTYPE_SERVER,
			   (size_t) 0, (dvoid **) 0);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    status=OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &svchp, OCI_HTYPE_SVCCTX,
			   (size_t) 0, (dvoid **) 0);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    //创建访问数据服务的路径
//	OCIServerAttach(srvhp, errhp, (text *)0, 0, OCI_DEFAULT);
    status=OCIServerAttach( srvhp, errhp, (text *)DbName, strlen(DbName), 0);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    /* set attribute server context in the service context */
    (void) OCIAttrSet( (dvoid *) svchp, OCI_HTYPE_SVCCTX, (dvoid *)srvhp,
		       (ub4) 0, OCI_ATTR_SERVER, (OCIError *) errhp);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    //分配数据库事务
    status=OCIHandleAlloc((dvoid *) envhp, (dvoid **)&authp,
			  (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    //m_UserName
    status= OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
		      (dvoid *) UserName, (ub4) strlen((char *)UserName),
		      (ub4) OCI_ATTR_USERNAME, errhp);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    
    status= OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
		      (dvoid *) PassWord, (ub4) strlen((char *)PassWord),
		      (ub4) OCI_ATTR_PASSWORD, errhp);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    
    status=OCISessionBegin ( svchp,  errhp, authp, OCI_CRED_RDBMS,(ub4) OCI_DEFAULT);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    
    status= OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
		      (dvoid *) authp, (ub4) 0,
		      (ub4) OCI_ATTR_SESSION, errhp);
    if(status!=0){	errText=checkerr(errhp,status);return false;}   
    *pHenv=(long)envhp;
    *pHdbc=(long)svchp;
    return true;
}

bool CDataBase::DisConnectDataBase()
{
	OCISessionEnd(svchp,errhp,authp,OCI_DEFAULT);
    OCIServerDetach(srvhp,errhp,OCI_DEFAULT);
    if (authp)
    {
	(void) OCIHandleFree((dvoid *) authp, OCI_HTYPE_SESSION);authp=NULL;
    }
    if (svchp)
    {
	(void) OCIHandleFree((dvoid *) svchp, OCI_HTYPE_SVCCTX);svchp=NULL;
    }
    if (srvhp)
    {
	(void) OCIHandleFree((dvoid *) srvhp, OCI_HTYPE_SERVER);srvhp=NULL;
    }
    if (errhp)
    {
	(void) OCIHandleFree((dvoid *) errhp, OCI_HTYPE_ERROR);errhp=NULL;
    }
    if (envhp)
    {
	(void) OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);envhp=NULL;
    }
    return true;
}

long CDataBase::Select(const std::string Smt,OdbRecordSet &rs,long &hpSelect/*Stmt handle*/)
{
    sword status;
    OCIStmt* hpStmt=NULL;
    const char* buff=Smt.c_str();
    status=OCIHandleAlloc(envhp,(void**)&hpStmt,OCI_HTYPE_STMT,0,NULL);
    if(status!=0){	errText=checkerr(errhp,status);return -1;}   
    status=OCIStmtPrepare(hpStmt,errhp,(text*) buff,strlen(buff),OCI_NTV_SYNTAX,OCI_DEFAULT);
    if(status!=0){	errText=checkerr(errhp,status);OCIHandleFree(hpStmt,OCI_HTYPE_STMT);return -1;}//wyj
    status=OCIStmtExecute(svchp,hpStmt,errhp,0,0,NULL,NULL,OCI_DEFAULT);
    if(status!=0){	errText=checkerr(errhp,status);OCIHandleFree(hpStmt,OCI_HTYPE_STMT);return -1;}//wyj
    int ColCount=0;int ret=0;
    while(status==OCI_SUCCESS)
    {
		void* pFieldAttr=NULL;
		sword hrGetNext=0;
		hrGetNext=OCIParamGet(hpStmt,OCI_HTYPE_STMT,errhp,&pFieldAttr,ColCount+1);
		if(hrGetNext!=OCI_SUCCESS)
		{
			if(ColCount<=0) ret=-1;
			errText=checkerr(errhp,hrGetNext,false);break;
		}
		++ColCount;
		OdbField* pField=new OdbField();
		rs.paFields.push_back(pField);
		status=OCIAttrGet(pFieldAttr,OCI_DTYPE_PARAM,&pField->wType,0,OCI_ATTR_DATA_TYPE,errhp);
		if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
		status=OCIAttrGet(pFieldAttr,OCI_DTYPE_PARAM,&pField->wSize,0,OCI_ATTR_DATA_SIZE,errhp);
		if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
		switch(pField->wType)
		{
		case SQLT_LNG:
			pField->wSize=32760;break;
		case SQLT_DAT:
			pField->wType=SQLT_STR;pField->wSize=50;break;
		case SQLT_NUM:
			pField->wType=SQLT_STR;pField->wSize+=1;break;
		case SQLT_CLOB:
			pField->wType=SQLT_CLOB;pField->wSize=0;break;
		case SQLT_BLOB:
			pField->wType=SQLT_BLOB;pField->wSize=0;break;
		case SQLT_LBI:
			pField->wType=SQLT_LBI;pField->wSize=10000;break;
		}
		text* pName=NULL; ub4 nNameLen;
		status=OCIAttrGet(pFieldAttr,OCI_DTYPE_PARAM,(void**)&pName,&nNameLen,OCI_ATTR_NAME,errhp);
		if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}
		if(pField->wSize>0) pField->SetSize(pField->wSize);
		char* tmpName=new char[nNameLen+1];
		memset(tmpName,0x00,nNameLen+1);
		sprintf(tmpName,"%.*s",nNameLen,(char*)pName);
		pField->Field_Name=tmpName;
	//	printf("%d\n",pField->wSize);
		delete []tmpName;
    }
    //绑定变量
    OCIDefine* pDefn=NULL;
    for(uint Col=0;Col<rs.paFields.size();Col++)
    {
		OdbField* pField=rs.paFields[Col];
		pDefn=NULL;
		if(pField->wType==SQLT_LBI||pField->wType==SQLT_BIN)
		{
			status=OCIDefineByPos(hpStmt,&pDefn,errhp,Col+1,(dvoid*)0,(ub4)pField->wSize,
				pField->wType,(dvoid*)0,(ub2*)0,(ub2*)0,OCI_DYNAMIC_FETCH);
			if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
			printf("al bind\n");
		}
		else if(pField->wType==SQLT_CLOB||pField->wType==SQLT_BLOB)
		{
			//分配指示器
			status=OCIDescriptorAlloc(envhp,(dvoid **)&pField->oLocator,(ub4) OCI_DTYPE_LOB,(size_t) 0,(dvoid **) 0);
			if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
			status=OCIDefineByPos(hpStmt,&pDefn,errhp,Col+1,(dvoid*)&pField->oLocator, -1,
				pField->wType,&pField->nInd,&pField->wLen,NULL,OCI_DEFAULT);
			if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
		}
		else
		{
			status=OCIDefineByPos(hpStmt,&pDefn,errhp,Col+1,pField->pBuffer,pField->wSize,
				pField->wType,&pField->nInd,&pField->wLen,NULL,OCI_DEFAULT);
			if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
		}
    }
    hpSelect=(long)hpStmt;
    return ret;
}

sword CDataBase::FetchNext(long hpSelect,OdbRecordSet &rs)
{
    sword status=0;int ret=0;
    OCIStmt* hpStmt=(OCIStmt*)hpSelect;
    if(!hpStmt) return -1;
    status=OCIStmtFetch(hpStmt,errhp,1,OCI_FETCH_NEXT,OCI_DEFAULT);
    rs.m_nRows=0;
    if(status==OCI_SUCCESS)
    {
	rs.m_nRows=1;
    }
    else
    {
	errText=checkerr(errhp,status,false);
    }
    return status;
}

void CDataBase::ClearRecordSets(long hpSelect,OdbRecordSet &rs)
{
    OCIStmt* hStmt=(OCIStmt*)hpSelect;
    if(!hStmt) return ;
    OCIHandleFree(hStmt,OCI_HTYPE_STMT);
    rs.RemoveAll();//wyj
    //rs.RemoveAll();//wyj
}

long CDataBase::SqlExecute(const std::string Smt)
{
    OCIStmt* tmpStmt=NULL;
    const char* buff=Smt.c_str();
    sword status=OCIHandleAlloc(envhp,(void**)&tmpStmt,OCI_HTYPE_STMT,0,NULL);
    if(status!=0) {errText=checkerr(errhp,status);return -1;}   
    status=OCIStmtPrepare(tmpStmt,errhp,(text*)buff,strlen(buff),OCI_NTV_SYNTAX,OCI_DEFAULT);
    if(status!=0) {errText=checkerr(errhp,status);OCIHandleFree(tmpStmt,OCI_HTYPE_STMT);return -1;}//wyj
    status=OCIStmtExecute(svchp,tmpStmt,errhp,1,0,NULL,NULL,OCI_DEFAULT);
    if(status!=0) {errText=checkerr(errhp,status);OCIHandleFree(tmpStmt,OCI_HTYPE_STMT);return -1;}//wyj
    if(tmpStmt)
		OCIHandleFree(tmpStmt,OCI_HTYPE_STMT);
    return 0;
}

long CDataBase::Commit()
{
    sword status=OCITransCommit(svchp,errhp,(ub4) 0);
    if(status!=0) {errText=checkerr(errhp,status);return -1;}  
    return 0;
}

long CDataBase::Rollback()
{
   sword status=OCITransRollback(svchp,errhp,(ub4) 0);
    if(status!=0) {errText=checkerr(errhp,status);return -1;}  
    return 0;
}

long CDataBase::SqlPrepare(const string Stmt,long &hprStmt)
{
    sword status;
    OCIStmt* hpStmt=NULL;
    const char* buff=Stmt.c_str();
    status=OCIHandleAlloc(envhp,(void**)&hpStmt,OCI_HTYPE_STMT,0,NULL);
    if(status!=0){	errText=checkerr(errhp,status);return -1;}   
    status=OCIStmtPrepare(hpStmt,errhp,(text*) buff,strlen(buff),OCI_NTV_SYNTAX,OCI_DEFAULT);
    if(status!=0){	errText=checkerr(errhp,status);OCIHandleFree(hpStmt,OCI_HTYPE_STMT);return -1;}//wyj
    hprStmt=(long)hpStmt;
    return 0;
}

long CDataBase::DefineColumn(long hpSelect,OdbRecordSet &rs)
{
    OCIStmt* hpStmt = (OCIStmt*)hpSelect;
    if(!hpStmt) return -1;
	
    sword status=0;int ColCount=0;int ret=0;
    while(status==OCI_SUCCESS)
    {
		void* pFieldAttr=NULL; sword hrGetNext=0;
		hrGetNext=OCIParamGet(hpStmt,OCI_HTYPE_STMT,errhp,&pFieldAttr,ColCount+1);
		if(hrGetNext!=OCI_SUCCESS)
		{
			if(ColCount<=0) ret=-1;errText=checkerr(errhp,hrGetNext);break;
		}
		++ColCount;
		OdbField* pField=new OdbField();
		status=OCIAttrGet(pFieldAttr,OCI_DTYPE_PARAM,&pField->wType,0,OCI_ATTR_DATA_TYPE,errhp);
		rs.paFields.push_back(pField);
		if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
		status=OCIAttrGet(pFieldAttr,OCI_DTYPE_PARAM,&pField->wSize,0,OCI_ATTR_DATA_SIZE,errhp);
		if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
		switch(pField->wType)
		{
		case SQLT_LNG:
			pField->wSize=32760;break;
		case SQLT_DAT:
			pField->wType=SQLT_STR;pField->wSize=50;break;
		case SQLT_NUM:
			pField->wType=SQLT_STR;pField->wSize+=1;break;
		case SQLT_CLOB:
			pField->wType=SQLT_CLOB;pField->wSize=0;break;
		case SQLT_BLOB:
			pField->wType=SQLT_BLOB;pField->wSize=0;break;
		case SQLT_LBI:
			pField->wType=SQLT_LBI;pField->wSize=0;break;
		}
		text* pName=NULL; ub4 nNameLen;
		status=OCIAttrGet(pFieldAttr,OCI_DTYPE_PARAM,(void**)&pName,&nNameLen,OCI_ATTR_NAME,errhp);
		if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}
		if(pField->wSize>0) pField->SetSize(pField->wSize);
		char* tmpName=new char[nNameLen+1];
		memset(tmpName,0x00,nNameLen+1);
		sprintf(tmpName,"%.*s",nNameLen,(char*)pName);
		pField->Field_Name=tmpName;
		delete tmpName;
    }
    //绑定变量
    OCIDefine* pDefn=NULL;
    for(int Col=0;Col<rs.paFields.size();Col++)
    {
		OdbField* pField=rs.paFields[Col];
		pDefn=NULL;
		if(pField->wType!=SQLT_CLOB&&pField->wType!=SQLT_BLOB&&pField->wType!=SQLT_LBI)
		{
			status=OCIDefineByPos(hpStmt,&pDefn,errhp,Col+1,pField->pBuffer,pField->wSize,
				pField->wType,&pField->nInd,(ub2*)&pField->wLen,NULL,OCI_DEFAULT);
			if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
			printf("%s\n",pField->Field_Name.c_str());
		}
		else if(pField->wType==SQLT_CLOB||pField->wType==SQLT_BLOB)
		{
			//分配指示器
			status=OCIDescriptorAlloc(envhp,(dvoid **)&pField->oLocator,(ub4) OCI_DTYPE_LOB,(size_t) 0,(dvoid **) 0);
			if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
			status=OCIDefineByPos(hpStmt,&pDefn,errhp,Col+1,(dvoid*)&pField->oLocator, -1,
				pField->wType,&pField->nInd,(ub2*)&pField->wLen,NULL,OCI_DEFAULT);
			if(status!=0) {errText=checkerr(errhp,status);ret=-1;break;}   
			printf("abc:%s\n",pField->Field_Name.c_str());
		}
		else
		{
			ub2 actsize;
			status=OCIDefineByPos(hpStmt, &pDefn, errhp, (ub4) 1, (dvoid *) pField->pBuffer, 
				(sb4) 2147483647, (ub2)SQLT_LBI, (dvoid *)0, 
				(ub2 *) &actsize, (ub2 *)0, (ub4)OCI_DYNAMIC_FETCH);
		}
    }
    return ret;
}


long CDataBase::BindParam(long hpStmt,OdbParamArray &params)
{
    OCIStmt* tmpStmt=(OCIStmt*)hpStmt;
    if(!tmpStmt) return -1;
    sword status=0;
    for(uint par=0;par<params.paParams.size();par++)
    {
	OdbParam* pParam=params.paParams[par];
	if(pParam->By==0)
	{
	    if(pParam->wType!=SQLT_CLOB&&pParam->wType!=SQLT_BLOB)
	    {
		status=OCIBindByPos(tmpStmt,&pParam->bindhp,errhp,(ub4)pParam->BindPos,
				    (dvoid *)pParam->pVar,pParam->wSize,pParam->wType,
				    (dvoid *)0,(ub2 *)0,(ub2 *)0,
				    (ub4) 0,(ub4 *) 0,(ub4) OCI_DEFAULT);
	    }
	    else
	    {
		//分配LOB指示器
		status=OCIDescriptorAlloc(envhp,(dvoid **)&pParam->oLocator,(ub4) OCI_DTYPE_LOB,(size_t) 0,(dvoid **) 0);
		if(status!=0) {errText=checkerr(errhp,status);return -1;}
		//分配;临时LOB内存
		status=OCILobCreateTemporary(svchp,errhp,pParam->oLocator,(ub2)SQLCS_IMPLICIT,
					     (ub1)OCI_DEFAULT,OCI_TEMP_CLOB,FALSE,OCI_DURATION_SESSION);
		if(status!=0) {errText=checkerr(errhp,status);return -1;}
		status=OCIBindByPos(tmpStmt,&pParam->bindhp,errhp,(ub4)pParam->BindPos,
				    (dvoid *)&pParam->oLocator,(sb4)-1,pParam->wType,
				    (dvoid *)0,(ub2 *)0,(ub2 *)0,
				    (ub4) 0,(ub4 *) 0,(ub4) OCI_DEFAULT);
		if(status!=0) {errText=checkerr(errhp,status);return -1;}
	    }
	}
	else
	{
	    const char* BindName=pParam->BindName.c_str();
	    if(pParam->wType!=SQLT_CLOB&&pParam->wType!=SQLT_BLOB)
	    {
		status=OCIBindByName(tmpStmt,&pParam->bindhp,errhp,(text *)BindName,(sb4) strlen(BindName),
				     (dvoid*)pParam->pVar,pParam->wSize+1,pParam->wType,
				     (dvoid*) 0,(ub2 *) 0,(ub2 *) 0,(ub4) 0,(ub4 *) 0,OCI_DEFAULT);
	    }
	    else
	    {
		//分配LOB指示器
		status=OCIDescriptorAlloc(envhp,(dvoid **)&pParam->oLocator,(ub4) OCI_DTYPE_LOB,(size_t) 0,(dvoid **) 0);
		if(status!=0) {errText=checkerr(errhp,status);return -1;}
		//分配;临时LOB内存
		status=OCILobCreateTemporary(svchp,errhp,pParam->oLocator,(ub2)SQLCS_IMPLICIT,
					     (ub1)OCI_DEFAULT,OCI_TEMP_CLOB,FALSE,OCI_DURATION_SESSION);
		if(status!=0) {errText=checkerr(errhp,status);return -1;}
		status=OCIBindByName(tmpStmt,&pParam->bindhp,errhp,(text *)BindName,(sb4) strlen(BindName),
				     (dvoid*)&pParam->oLocator,-1,pParam->wType,
				     (dvoid*) 0,(ub2 *) 0,(ub2 *) 0,(ub4) 0,(ub4 *) 0,OCI_DEFAULT);
		if(status!=0) {errText=checkerr(errhp,status);return -1;}
	    }
	}
	if(status!=0) {errText=checkerr(errhp,status);return -1;}   
    }
    return 0;
}

long CDataBase::SqlExecuteWithParam(long hpStmt)
{
    OCIStmt* tmpStmt=(OCIStmt*)hpStmt;
    ub2 f=0;
    sword status=OCIAttrGet(tmpStmt,OCI_HTYPE_STMT,(dvoid*)&f,0,OCI_ATTR_STMT_TYPE,errhp);
    if(f==OCI_STMT_SELECT)
    {
	status=OCIStmtExecute(svchp,tmpStmt,errhp,(ub4) 0,(ub4) 0,(OCISnapshot *) 0,
			      (OCISnapshot *) 0,(ub4) OCI_DEFAULT);
    }
    else
    {
	status=OCIStmtExecute(svchp,tmpStmt,errhp,(ub4) 1,(ub4) 0,(OCISnapshot *) 0,
			      (OCISnapshot *) 0,(ub4) OCI_DEFAULT);
    }
    if(status!=0) {errText=checkerr(errhp,status);return -1;}   
    return 0;
}


void CDataBase::FreeParams(long hpStmt,OdbParamArray &rs)
{
    OCIStmt* hStmt=(OCIStmt*)hpStmt;
    if(!hStmt) return ;
    for(uint i=0;i<rs.paParams.size();i++)
    {
	if(rs.paParams[i]->oLocator)
	    OCILobFreeTemporary(svchp,errhp,rs.paParams[i]->oLocator);
    }
    OCIHandleFree(hStmt,OCI_HTYPE_STMT);
    rs.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
long CDataBase::SqlGetLobLength(OdbField* pField)
{
    ub4 len;
    OCILobGetLength(svchp,errhp,pField->oLocator,&len);
    return (long)len;
}

long CDataBase::SqlReadLob(OdbField* pParam,long Offset,long iRead,long &ReadCount)
{
    if(pParam==NULL) return -1;
    sword status=-1;
    if(pParam->wType==SQLT_CLOB||pParam->wType==SQLT_BLOB)
    {
	pParam->SetSize(iRead);
	status=OCILobRead(svchp,errhp,pParam->oLocator,(ub4*)&ReadCount,(ub4)Offset,pParam->pBuffer,iRead,(dvoid *) 0,
			  (sb4 (*) (dvoid *,CONST dvoid *,ub4,ub1)) 0,
			  (ub2) 0,(ub1) SQLCS_IMPLICIT);
	if(status!=OCI_SUCCESS && status != OCI_NEED_DATA) {errText=checkerr(errhp,status);return -1;}   
	pParam->wLen=ReadCount;
    }
    return status;
}

long CDataBase::SqlReadLob(OdbRecordSet &rs,long Offset,long iRead,long &ReadCount)
{
    long ret=-1;
    for(uint i=0;i<rs.paFields.size();i++)
    {
	OdbField* pParam=(OdbField *)rs.paFields[i];
	ret=SqlReadLob(pParam,Offset,iRead,ReadCount);
    }
    return ret;
}

long CDataBase::ReadCLob(OdbField* pField,StringArrary &sa)
{
    if(!pField->oLocator) return -1;
    long len=SqlGetLobLength(pField);
    if(len<=0) return 0;
    int count=len/1024;long ReadCount=1024;
    if((double)count<(double)len/1024) count++;
    int ret=0;
    for(uint i=0;i<count;i++)
    {
	int Offset=i*ReadCount+1;
	ret=SqlReadLob(pField , Offset, 1024, ReadCount);
	if(ret!=0) break;
	sa.push_back(pField->Get());
    }
    return ret;
}

long CDataBase::ReadLobToFile(OdbField* pField,string FileName)
{
    if(!pField->oLocator) return -1;
    long len=SqlGetLobLength(pField);
    if(len<=0) return 0;
    //打开文件
    FILE* f=fopen(FileName.c_str(),"w");
    if(!f) return -1;
    int count=len/1024;long ReadCount=1024;
    if((double)count<(double)len/1024) count++;
    int ret=0;
    for(uint i=0;i<count;i++)
    {
	int Offset=i*ReadCount+1;
	ret=SqlReadLob(pField , Offset, 1024, ReadCount);
	if(ret!=0) break;
	fwrite(pField->pBuffer,1,ReadCount,f);
    }
    fclose(f);
    return ret;
}

long CDataBase::SqlWriteLob(OdbParam* pParam,long Offset,long iWrite,long &WriteCount)
{	
    if(pParam==NULL) return -1;
    sword status=-1;
    if(pParam->wType==SQLT_CLOB||pParam->wType==SQLT_BLOB)
    {
	status=OCILobWrite(svchp,errhp,pParam->oLocator,(ub4*)&WriteCount,(ub4)Offset,
			   pParam->pVar,(ub4)iWrite,OCI_ONE_PIECE,
			  (dvoid *) 0,
			  (sb4 (*) (dvoid *,dvoid *,ub4*,ub1*)) 0,
			  (ub2) 0,(ub1) SQLCS_IMPLICIT);
	if(status!=0) {errText=checkerr(errhp,status);return -1;}   
    }
    return status;
}

long CDataBase::SqlWriteLob(OdbParamArray &rs,long Offset,long iWrite,long &WriteCount)
{
    long ret=-1;
    for(uint i=0;i<rs.paParams.size();i++)
    {
	OdbParam* pParam=(OdbParam *)rs.paParams[i];
	ret=SqlWriteLob(pParam,Offset,iWrite,WriteCount);
    }
    return ret;
}

long CDataBase::WriteCLob(OdbParam* pParam,StringArrary &sa)
{
    if(!pParam->oLocator) return -1;
    long WriteCount=0;long Offset=0;long iWrite=0;int ret=0;
    for(uint i=0;i<sa.size();i++)
    {
	pParam->Set(sa[i]);
	Offset=Offset+WriteCount+1;
	iWrite=sa[i].size();WriteCount=sa[i].size();
	ret=SqlWriteLob(pParam,Offset,iWrite,WriteCount);
	if(ret!=0) break;
    }
    return ret;
}

long CDataBase::WriteLobFromFile(OdbParam* pParam,string FileName)
{
    if(!pParam->oLocator) return -1;
    FILE* f=fopen(FileName.c_str(),"r");
    if(!f) return -1;
    long WriteCount=0;long Offset=1;long iWrite=0;
    const int readcount=1024;
    char buff[readcount];memset(&buff,0x00,readcount);
    iWrite=fread(buff,sizeof(char),readcount,f);
    int ret=0;
    while(iWrite>0)
    {
	WriteCount=iWrite;
	pParam->Set(buff,WriteCount);
	ret=SqlWriteLob(pParam,Offset,iWrite,WriteCount);
	if(ret!=0) break;
	Offset+=WriteCount;
	iWrite=fread(buff,sizeof(char),readcount,f);
    }
    fclose(f);
    return ret;
}

long CDataBase::SqlGetRecordCount(const std::string Stmt)
{
    OdbRecordSet rs;long hStmt=0;
    long ret=Select(Stmt,rs,hStmt);
    if(ret!=0) return -1;
    ret=FetchNext(hStmt,rs);
    if(ret!=0) return -1;
    string s=rs.paFields[0]->Get();
    return atol(s.c_str());
}

long CDataBase::SqlGetRecordCount(const std::string Table,const std::string Where)
{
    string Sql="Select /*+all_rows*/count(*) from "+Table+" "+Where;
    OdbRecordSet rs;long hStmt=0;
    long ret=Select(Sql,rs,hStmt);
    if(ret!=0) return -1;
    ret=FetchNext(hStmt,rs);
    if(ret!=0) return -1;
    string s=rs.paFields[0]->Get();
    return atol(s.c_str());
}

long CDataBase :: GetFileLength(string FileName)
{
    FILE* f=fopen(FileName.c_str(),"r");
    if(!f) return -1;
    int ret=fseek(f,0,SEEK_END);
    fpos_t pos;fgetpos(f,&pos);
    char tmpbuff[20];memset(tmpbuff,0x00,20);sprintf(tmpbuff,"%d",pos);
    long filelen=atol(tmpbuff);
    fseek(f,0,SEEK_SET);
    fclose(f);
    return filelen;
}
//////////////////////////////////////////////对Long Raw字段的处理///////////////////////////
long CDataBase :: BindLongRaw(long hpStmt,OdbParamArray &params)
{
    OCIStmt* tmpStmt=(OCIStmt*)hpStmt;
    if(!tmpStmt) return -1;
    sword status=0;
    for(uint par=0;par<params.paParams.size();par++)
    {
	OdbParam* pParam=params.paParams[par];
	long len=GetFileLength(pParam->FileName);
	if(len==-1) continue;
	if(pParam->By==0)
	{
	    status=OCIBindByPos(tmpStmt,&pParam->bindhp,errhp,(ub4)pParam->BindPos,
				(dvoid *)pParam->pVar,(sb4)len,pParam->wType,
				(dvoid *)0,(ub2 *)0,(ub2 *)0,
				(ub4) 0,(ub4 *) 0,(ub4) OCI_DATA_AT_EXEC);
	    if(status!=0) {errText=checkerr(errhp,status);return -1;}   
	}
    }
    return 0;
}

long CDataBase :: ExecLongRawFile(long hpStmt,OdbParam* pParam)
{
    long filelen=GetFileLength(pParam->FileName);
    if(filelen==-1) return -1;
    int count=filelen/READ_FILE_LENGTH;
    double countd=filelen/READ_FILE_LENGTH;
    if((double)count<countd) count++;
    
    OCIStmt* tmpStmt=(OCIStmt*)hpStmt;
    if(!tmpStmt) return -1;
    FILE* f=fopen(pParam->FileName.c_str(),"rb");
    if(!f) return -1;
    sword status=0;char buff[READ_FILE_LENGTH];
    ub1 piece=OCI_FIRST_PIECE;ub2 rcode2;long iWrite=0,WriteCount=0;int ret=0;
    while(1)
    {
	status=OCIStmtExecute(svchp,tmpStmt,errhp,(ub4) 1,(ub4) 0,(OCISnapshot *) 0,
			      (OCISnapshot *) 0,(ub4) OCI_DEFAULT);
	switch(status)
	{
	case OCI_NEED_DATA:
	    {
		memset(buff,0x00,READ_FILE_LENGTH);
		iWrite=fread(buff,sizeof(char),READ_FILE_LENGTH,f);
		WriteCount=iWrite;
		pParam->Set(buff,WriteCount);
		ret=OCIStmtSetPieceInfo((dvoid*)pParam->bindhp,(ub4)OCI_HTYPE_BIND,errhp,(dvoid*)pParam->pVar,
				    (ub4*)&WriteCount,piece,(dvoid*)0,&rcode2);
		if(ret!=0) {errText=checkerr(errhp,ret);}   
		count--;
		if(count>=1) piece=OCI_NEXT_PIECE;
		else piece=OCI_LAST_PIECE;
		status=OCI_NEED_DATA;
		break;
	    }
	case OCI_SUCCESS:
	    break;
	default:
	    status=0;break;
	}
	if(!status) break;
    }
    fclose(f);
    if(status!=0||ret!=0) return -1;
    return 0;
}

long CDataBase::ReadLongRawToFile(long hpStmt,string Filename)
{
	OCIStmt* tmpStmt=(OCIStmt*)hpStmt;
    if(!tmpStmt) return -1;
	sword status=99;
	status = OCIStmtFetch(tmpStmt, errhp,(ub4) 1, (ub2) OCI_FETCH_NEXT, (ub4) OCI_DEFAULT);
	if(status!=0) {errText=checkerr(errhp,status);}   
    ub1   piece = OCI_NEXT_PIECE;
    dvoid *hdlptr = (dvoid *) 0;
    ub4 hdltype = OCI_HTYPE_DEFINE, iter = 0, idx = 0;
    ub1   in_out = 0;
	//ub1 buf2[READ_FILE_LENGTH];
	ub1  *buf2=(ub1 *)malloc( READ_FILE_LENGTH+10);
	if(buf2 == NULL)
		return -2;
	ub4 alen=READ_FILE_LENGTH;
    sb2   indptr = 0;
    ub2   rcode = 0;
    FILE* f=fopen(Filename.c_str(),"wb");
	if(f == NULL)
	{
		free(buf2);
		return -3;
	}
	long total=0;
	while (status == OCI_NEED_DATA)
	{
		memset(buf2,0x00,READ_FILE_LENGTH);
		status=OCIStmtGetPieceInfo(tmpStmt,errhp, &hdlptr, &hdltype,
			&in_out, &iter, &idx, &piece);
		
		status=OCIStmtSetPieceInfo((dvoid *)hdlptr, (ub4)hdltype,
			errhp, (dvoid *) buf2, &alen, piece,
			(dvoid *)0, 0);
		status = OCIStmtFetch(tmpStmt, errhp,(ub4) 1, (ub2) OCI_FETCH_NEXT, (ub4) OCI_DEFAULT);
		if(status<0) break;
		fwrite(buf2,sizeof(char),alen,f);
		total+=alen;
        status = OCI_NEED_DATA;                        
		if(alen<READ_FILE_LENGTH)
		{
			break;
		}
	}
	fclose(f);
	free(buf2);
    return 0;
}

string CDataBase::MakeRecordIndex(int nSubNo, int nDevNo, int nCpuNo)
{
	static unsigned short index = 0;
	index++;
	string szstr;
	char buf[60]; memset(buf, 0x00, sizeof(buf));
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep); /*取得当地时间*/
	if (p!=NULL)
	{
/*		sprintf(buf, "%05d%05d%02d%04d%02d%02d%02d%02d%02d%010d",
			nSubNo, nDevNo, nCpuNo, 1900+p->tm_year, p->tm_mon, p->tm_mday, 
			p->tm_hour, p->tm_min, p->tm_sec, index);
*/
		sprintf(buf, "%04d%02d%02d%02d%02d%02d_%03d_%03d_%02d_%05d",
			1900+p->tm_year, p->tm_mon+1, p->tm_mday, 
			p->tm_hour, p->tm_min, p->tm_sec, nSubNo,nDevNo,nCpuNo,index);
	}
	szstr = string(buf);

	return szstr; 
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOracle::SOracle()
{
	m_pConn = NULL;
	m_iTestTimes = 0;
}

SOracle::~SOracle()
{
	DisConnect();
}

bool SOracle::Connect()
{
	if(m_pConn != NULL)
		DisConnect();
	m_pConn = new CDataBase;

	SString sParam = GetParams();//hostaddr=127.0.0.1;port=5432;dbname=znl4;user=postgres;password=123456;
	SString sTemp;
	sTemp.sprintf("hostaddr=%s port=%s dbname=%s user=%s password=%s connect_timeout=0",
		SString::GetAttributeValue(sParam,"hostaddr").data(),
		SString::GetAttributeValue(sParam,"port").data(),
		SString::GetAttributeValue(sParam,"dbname").data(),
		SString::GetAttributeValue(sParam,"user").data(),
		SString::GetAttributeValue(sParam,"password").data());
	SString userName = SString::GetAttributeValue(sParam,"user");
	SString password = SString::GetAttributeValue(sParam,"password");
	SString dbname = SString::GetAttributeValue(sParam,"dbname");
    #ifndef WIN32
	    dbname.sprintf("%s:%s/%s",SString::GetAttributeValue(sParam,"hostaddr").data(),SString::GetAttributeValue(sParam,"port").data(),dbname.data());
	#endif
	
	LOGDEBUG("Begin to Connect Oracle:%s",sTemp.data());
	bool b = m_pConn->ConnectDataBase(userName.data(),password.data(),dbname.data());
	//m_pConn = PQconnectdb(sTemp.data());
	
// 	if (PQstatus(m_pConn) != CONNECTION_OK)
// 	{
// 		YSERROR("Connect to oracle ERROR:%s params:%s",PQerrorMessage(m_pConn),sTemp.data());
// 		DisConnect();
// 		return false;
// 	}
	LOGDEBUG("Connect to oracle OK params:%s",sTemp.data());
/*	PQsetClientEncoding(m_pConn,"gbk");*/
	
	return b;
}

bool SOracle::DisConnect()
{
	if(m_pConn == NULL)
		return false;
	m_pConn->DisConnectDataBase();
	delete m_pConn;
	m_pConn = NULL;
	return true;
}

bool SOracle::TestConnect() 
{
	//bConnect:true时，如果断了，自动重连
	OdbRecordSet rs; 
	long hStmt=0;
	string sSql="select 1 from SYS.user$";
    sword status = m_pConn->Select(sSql,rs,hStmt);
	if(status != 0) 
	{
		return Connect();
	}
	m_pConn->ClearRecordSets(hStmt,rs);
 	return true;
}



int SOracle::Retrieve(SString sql,SRecordset &rs)
{
	
	OdbRecordSet ors;
	long hStmt=0;
	string sSql(sql.data());
	string sLastError;

	LOGBASEDEBUG("Retrieve:%s",sql.data());
	//取结果集

    sword status = m_pConn->Select(sSql,ors,hStmt);
	if(status!=0) 
	{
		if(!TestConnect()) return 0;
		status = m_pConn->Select(sSql,ors,hStmt);
		if(status!=0)
		{
			sLastError=m_pConn->GetErrorText();
			LOGDEBUG("SQL语句错误:%s, err:%s\n",sSql.c_str(),sLastError.data());
			return 0;
		}
	}

	try
	{
		rs.clear();
		
		int nFields=ors.GetFieldCount();
		SPtrList<SPtrList<SString> > resultList;
		resultList.setAutoDelete(true);//add by skt
		while(m_pConn->FetchNext(hStmt,ors)==0) 
		{
			SPtrList<SString> *aReocrd = new SPtrList<SString>();
			aReocrd->setAutoDelete(true);//add by skt
			for(int index=0; index<nFields; index++)
			{
				SString *aValue = new SString();
				aValue->SetString(ors.GetFieldValue(index).c_str());
				aReocrd->append(aValue);
			}
			resultList.append(aReocrd);
		}

		int nRows = resultList.count();
		rs.SetSize(nRows, nFields);
		for(int rows=0; rows<nRows; rows++)
		{
			SPtrList<SString> *srcRecord = resultList.at(rows);	
			SRecord* oneRecord = rs.GetRecord(rows);
			for(int index=0; index<nFields; index++)
			{
				SString value = *(srcRecord->at(index));
				oneRecord->SetValue(index, value);
			}
		} //rows
		
		m_pConn->ClearRecordSets(hStmt,ors);
	}
	catch(...)
	{
		m_pConn->ClearRecordSets(hStmt,ors);
		sLastError=m_pConn->GetErrorText();
		LOGDEBUG("SQL语句错误:%s,err:%s\n",sql.data(),sLastError.data());
		return 0;
	}
	return rs.GetRows();
}

bool SOracle::Execute(SString sql)
{
	string sLastError;
	try
	{
	LOGBASEDEBUG("Execute:%s",sql.data());

		string c_Sql(sql.data());
		if (m_pConn->SqlExecute(c_Sql) == -1)
		{
			sLastError=m_pConn->GetErrorText();
			LOGDEBUG("SQL语句错误:%s,err:%s\n",sql.data(),sLastError.data());
			return false;
		}

        m_pConn->Commit();
	}
	catch(...)
	{
		sLastError=m_pConn->GetErrorText();
		LOGDEBUG("SQL语句错误:%s,err:%s\n", sql.data(),sLastError.data());

		return false;
	}

	return true;
}

bool SOracle::UpdateLobFromFile(SString sTable,SString sLobField,SString sWhere,SString sFile) 
{

	//6.执行一个带有CLOB/BLOB的Update语句,将文件内容保存到CLOB/BLOB字段中
	SString sql;
	sql.sprintf("update %s set %s=:1 where %s", sTable.data(),sLobField.data(),sWhere.data());
	LOGDEBUG("文件内容保存到CLOB/BLOB字段中:%s",sql.data());
	string sSql(sql.data());
	string filename(sFile.data());
	long gg;
	OdbParamArray oa;
	OdbParam* pParam=new OdbParam;
	pParam->wType=SQLT_CLOB;  
	pParam->BindPos=1; 
	oa.paParams.push_back(pParam);
	m_pConn->SqlPrepare(sSql,gg);   
	m_pConn->BindParam(gg,oa);
	m_pConn->WriteLobFromFile(pParam,filename);
	m_pConn->SqlExecuteWithParam(gg); 
	m_pConn->Commit(); 
	m_pConn->FreeParams(gg,oa);
	return true;
}

bool SOracle::ReadLobToFile(SString sTable,SString sLobField,SString sWhere,SString ysFile)
{ 
	//4.执行一个带有CLOB/BLOB查询结果的语句
		//与上面的方法一样,但在取集录时不同,例如:
	SString sql;
	sql.sprintf("select %s from %s where %s ", sLobField.data(),sTable.data(),sWhere.data());
	LOGDEBUG("将CLOB/BLOB字段内容保存到文件中:%s",sql.data());
	string sele(sql.data());
	string fileName(ysFile.data());

    long Stmt;  OdbRecordSet rs; m_pConn->Select(sele,rs,Stmt);
    while(m_pConn->FetchNext(Stmt,rs)==0)
    {
		for(int i=0;i<rs.paFields.size();i++)
		{
			OdbField* pField=rs.paFields[i];
			if(pField->wType==SQLT_CLOB||pField->wType==SQLT_BLOB)
				m_pConn->ReadLobToFile(pField,fileName);   //将内容保存到文件.此处也可以:StringArray oa;db.ReadCLob(pField,oa);读到字符数组中
        }
    }
    m_pConn->ClearRecordSets(Stmt,rs);
    return true;
}

bool SOracle::UpdateLongRawFromFile(SString sTable,SString sLobField,SString sWhere,SString ysFile) 
{
	//6.执行一个带有LongRaw的Update语句,将文件内容保存到LongRaw字段中
	SString sql;
	sql.sprintf("Update %s set %s=:1 where %s", sTable.data(),sLobField.data(),sWhere.data());
	LOGDEBUG("文件内容保存到LongRaw字段中:%s",sql.data());
	string sSql(sql.data());
	string sFile(ysFile.data());

	long gg = 0;   
	bool bReturn=false;
	OdbParam* pParam = new OdbParam;
	pParam->wType = SQLT_LBI;
	pParam->BindPos = 1;
	pParam->FileName = sFile;
	
	OdbParamArray oa;
	oa.paParams.push_back(pParam);
	
	if (m_pConn->SqlPrepare(sSql,gg)==0
		&& m_pConn->BindLongRaw(gg,oa)==0
		&& m_pConn->ExecLongRawFile(gg, pParam)==0)
	{
		m_pConn->Commit();
		bReturn=true;
	}
	m_pConn->FreeParams(gg,oa);
	return bReturn;
}

bool SOracle::ReadLongRawToFile(SString sTable,SString sLobField,SString sWhere,SString ysFile)
{ 
	//6.执行一个带有LongRaw的select语句,将LongRaw字段内容保存到文件中
	SString sql;
	sql.sprintf("select %s from %s where %s", sLobField.data(),sTable.data(),sWhere.data());
	LOGDEBUG("将LongRaw字段内容保存到文件中:%s",sql.data());
	string sSql(sql.data());
	string sFile(ysFile.data());

	long t_Stmt;  OdbRecordSet t_rs; 
	OdbField* pField=new OdbField;
	pField->Field_Name = sLobField.data();
	pField->FileName = sFile;
	t_rs.paFields.push_back(pField);
	m_pConn->SqlPrepare(sSql.c_str(),t_Stmt);
	m_pConn->SqlExecuteWithParam(t_Stmt);
	m_pConn->DefineColumn(t_Stmt,t_rs);
	m_pConn->ReadLongRawToFile(t_Stmt,pField->FileName);
	m_pConn->ClearRecordSets(t_Stmt,t_rs);
	return true;
}

bool SOracle::readLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value)
{
	try
	{
		SString sql="select "+fieldName+" from "+tableName+" where "+whereField+"='"+recordIndex+"'  ";
		long Stmt;
		OdbRecordSet rs;
		value = ""; 
		string sele(sql);
		if(m_pConn->Select(sele,rs,Stmt) != 0)
		{
			//m_db.ClearRecordSets(Stmt,rs);
			return false;
		}
		while(m_pConn->FetchNext(Stmt,rs)==0)
		{
			for(int i=0;i<rs.paFields.size();i++)
			{
				OdbField* pField=rs.paFields[i];
				if(pField->wType==SQLT_CLOB||pField->wType==SQLT_BLOB)
				{
					//将内容保存到文件.此处也可以:
					StringArrary sa;
					m_pConn->ReadCLob(pField,sa);//读到字符数组中
					//string fileName("E:\\temp.txt");
					//m_pConn->ReadLobToFile(pField,fileName);
					for(int j=0;j<sa.size();j++)
						value += sa[j].c_str();
				}
			}
		}
		m_pConn->ClearRecordSets(Stmt,rs);
		return true;
	}
	catch (...)
	{
		//MessageBox(NULL,"err","err",MB_OK);
	}
	return false;
}

bool SOracle::updateLob(SString tableName,SString fieldName,SString whereField,SString recordIndex,SString &value)
{
	try
	{
		for(int times=0;times<2;times++)
		{
			SString sql;
			sql = "update "+tableName+" set "+fieldName+"=:1 where "+whereField+"='"+recordIndex+"'";
			string update(sql);
			long gg;
			bool ok=true;
			OdbParamArray oa;
			OdbParam* pParam=new OdbParam;
			pParam->wType=SQLT_CLOB;
			pParam->BindPos=1;
			oa.paParams.push_back(pParam);
			if(m_pConn->SqlPrepare(update,gg) != 0)
			{
				if(!m_pConn->ConnectDataBase())
				{
					m_pConn->Rollback();
					return false;
				}
				continue;
			}
			
			m_pConn->BindParam(gg,oa);
			StringArrary sac;
			sac.push_back(update);
			if(m_pConn->WriteCLob(pParam,sac) != 0)
				ok=false;
			m_pConn->SqlExecuteWithParam(gg); 
			m_pConn->Commit(); 
			m_pConn->FreeParams(gg,oa);
			return ok;
		}
		m_pConn->Rollback();
	}
	catch (...)
	{
		//MessageBox(NULL,"err","err",MB_OK);		
	}
	return false;
}

SString SOracle::getColumnType(SString tableName,SString fieldName)
{

	SString sql; 
	sql.sprintf("select type# from sys.col$ t "
						"where obj# in(select obj# from sys.obj$ t where lower(name)='%s') "
						"and lower(name)='%s'",tableName.lower().data(),fieldName.lower().data() );
	//sql.sprintf("select data_type from cols where tableName=upper('%s') and column_name=upper('%s')",tableName,fieldName);
	SRecordset rs;
	int n = Retrieve(sql,rs);
	if(n)
	{
		n = rs.GetRecord(0)->GetValue(0).toInt();
		switch(n)
		{
		case 24:
			return "LONG RAW";
		default:
			return "";
		}
	}
	else 
	{
		return "";
	}

}

