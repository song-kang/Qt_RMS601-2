/**
 *
 * 文 件 名 : SApplication.cpp
 * 创建日期 : 2009-8-20 13:36
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:32 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: SApplication.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:32  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.11  2011/05/09 11:08:27  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.10  2011/04/27 02:58:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.9  2011/04/23 10:53:52  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2011/04/22 06:32:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2011/04/21 02:14:16  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/04/11 12:08:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/04 08:20:57  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/03 07:33:12  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/03/29 07:07:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/03 12:07:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/01/17 01:20:49  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2010/12/20 08:37:09  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2010/11/19 02:03:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/11/17 02:19:15  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/11/16 08:08:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *
 **/


#include "SApplication.h"

//////////////////////////////////////////////////////////////////////////
// History
// 1.0.0	2011-xx-xx  邵凯田　应用程序框架类
// 2.0.0	2013-08-xx  邵凯田　应用程序框架类重新整理
//////////////////////////////////////////////////////////////////////////

#define SKTBASE_VERSION "2.0.0"

#ifndef _WITHOUT_AGENT_CHANNEL_

CSAgentClient::CSAgentClient()
{
	m_sAgentIp = "127.0.0.1";
	m_iAgentPort = 6666;
	m_SendList.setShared(true);
	m_SendList.setAutoDelete(true);
	m_pApp = NULL;
	m_iThreads = 0;
}

CSAgentClient::~CSAgentClient()
{
	if(m_pApp != NULL)
	{
		m_pApp->m_bHalt = true;
		while(m_iThreads > 0)
			SApi::UsSleep(1000);
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  启动代理服务
// 作    者:  邵凯田
// 创建时间:  2012-5-31 10:13
// 参数说明:  @bBlock为true表示阻塞运行，直到程序退出，false表示不阻塞启动线程运行代理服务
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool CSAgentClient::StartAgent(bool bBlock)
{
	if(bBlock)
	{
		ThreadAgent(this);
	}
	else
	{
		SKT_CREATE_THREAD(ThreadAgent,this);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  代理线程，执行代理任务，线程运行时不占用Run函数，也可直接运行
// 作    者:  邵凯田
// 创建时间:  2012-5-30 11:00
// 参数说明:  @lp：this
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void* CSAgentClient::ThreadAgent(void* lp)
{
	CSAgentClient *pThis = (CSAgentClient*)lp;
	if(pThis->m_pApp == NULL)
	{
		LOGERROR("m_pApp未设置!");
		return NULL;
	}
	pThis->m_iThreads ++;
	//代理通道
	SSocket skt;
	SString sMsg,sRet;
	BYTE *pBuffer=NULL;
	int ret,iLength,i,cnt,from_handle;
	while(!pThis->m_pApp->m_bHalt)
	{
		pThis->m_bLogined = false;
		pThis->m_iCpuPercent = SApi::GetCpuUsedPercent();
		if(pThis->m_pApp != NULL && pThis->m_pApp->IsClosedExist())
		{
			LOGWARN("收到退出信号! 准备关闭当前进程!");
			pThis->m_pApp->Stop();
			//准备关闭当前进程
			pThis->m_pApp->RemoveClose();
			goto halt;
		}
		if(!skt.Create() || !skt.Connect(pThis->m_sAgentIp.data(),pThis->m_iAgentPort))
		{
			for(i=0;i<200 && !pThis->m_pApp->m_bHalt;i++)
				SApi::UsSleep(10000);
			continue;
		}
		//发送登录信息
		sMsg.sprintf("act=login;pid=%d;modname=%s;modver=%s;moddesc=%s;modcrc=%s;modtime=%s;",
			SApi::GetPid(),pThis->m_pApp->m_sModule.data(),pThis->m_pApp->m_sModuleVer.data(),pThis->m_pApp->m_sModuleDesc.data(),
			pThis->m_pApp->m_sModuleCrc.data(),pThis->m_pApp->m_sModuleTime.data());
		sRet = "";
		skt.SetTimeout(10,2000);
		if( !pThis->SendMsg(&skt,sMsg) || 
			!pThis->ReceiveMsg(&skt,sRet,pBuffer,iLength) || 
			SString::GetAttributeValueI(sRet,"result") != 1)
		{
			skt.Close();
			if(pBuffer != NULL)
			{
				delete[] pBuffer;
				pBuffer = NULL;
			}
			for(i=0;i<100 && !pThis->m_pApp->m_bHalt;i++)
				SApi::UsSleep(10000);
			continue;
		}
		if(pBuffer != NULL)
		{
			delete[] pBuffer;
			pBuffer = NULL;
		}
		time_t lastsoc = ::time(NULL);
		pThis->m_bLogined = true;
		while(!pThis->m_pApp->m_bHalt)
		{
			if(abs((long)(::time(NULL)-lastsoc)) > 2)
			{
				lastsoc = ::time(NULL);
				pThis->m_iCpuPercent = SApi::GetCpuUsedPercent();
			}
			//循环，代理消息处理
			ret = pThis->ReceiveMsg(&skt,sMsg,pBuffer,iLength);
			if(ret < 0)
				break;
			if(ret > 0)
			{
				LOGDEBUG("收到代理消息:%s",sMsg.data());
				from_handle = SString::GetAttributeValueI(sMsg,"from_handle");
				if(!pThis->ProcessAgentMsgBase(sMsg,pBuffer,iLength))
					pThis->ProcessAgentMsg(from_handle,sMsg,pBuffer,iLength);
				if(pBuffer != NULL)
				{
					delete[] pBuffer;
					pBuffer = NULL;
				}
			}
			//是否有需要发送的报文
			cnt = pThis->m_SendList.count();
			if(cnt == 0)
			{
				for(i=0;i<100;i++)
				{
					SApi::UsSleep(2000);
					if(skt.CheckForRecv() > 0 || pThis->m_SendList.count() > 0 || pThis->m_pApp->m_bHalt)
						break;
				}
			}
			else
			{
				for(i=0;i<cnt&&!pThis->m_pApp->m_bHalt;i++)
				{
					CAgentPackage *p = pThis->m_SendList[0];
					if(p == NULL)
						continue;
					if(pThis->SendMsg(&skt,p->sMsg,p->pBuffer,p->iLength) < 0)
						break;
					pThis->m_SendList.remove(0);
				}
			}
		}
	}
halt:
	pThis->m_iThreads --;
	return NULL;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  提取当前已经登录到代理的进程模块列表信息
// 作    者:  邵凯田
// 创建时间:  2011-3-29 9:55
// 参数说明:  引用返回的列表
// 返 回 值:  >=0表示进程数量,<0表示失败
//////////////////////////////////////////////////////////////////////////
int CSAgentClient::GetModuleList(SPtrList<stuModuleInfo> &module_list)
{
	SString sMsg = "act=ps;";
	m_sWaitMsgAct = "ps";
	m_sWaitMsg = "";
	this->AddMsgToAgent(sMsg);
	//int times = 0;
	int i;
	for(i=0;i<200;i++)
	{
		if(m_sWaitMsg.length() > 0)
			break;
		SApi::UsSleep(10000);
	}
	if(m_sWaitMsg.length() > 0)
	{
		SString key,sLine;
		//收到回复
		int size = SString::GetAttributeValueI(m_sWaitMsg,"size");
		for(i=0;i<size;i++)
		{
			key.sprintf("item%d",i+1);
			sLine = SString::GetAttributeValue(m_sWaitMsg,key);
			if(sLine.length() == 0)
				continue;
			stuModuleInfo *pNew = new stuModuleInfo();
			pNew->handle = SString::GetIdAttributeI(1,sLine,",");
			pNew->pid = SString::GetIdAttributeI(2,sLine,",");
			pNew->name = SString::GetIdAttribute(3,sLine,",");
			pNew->version = SString::GetIdAttribute(4,sLine,",");
			pNew->crc = SString::GetIdAttribute(5,sLine,",");
			pNew->compile_time = SString::GetIdAttribute(6,sLine,",");
			pNew->desc = SString::GetIdAttribute(7,sLine,",");
			pNew->login_ip = SString::GetIdAttribute(8,sLine,",");
			pNew->login_port = SString::GetIdAttributeI(9,sLine,",");
			key = SString::GetIdAttribute(10,sLine,",");
			pNew->login_time = SDateTime::makeDateTime("yyyy-MM-dd hh:mm:ss.zzz",key);
			module_list.append(pNew);
		}
	}
	else
	{
		return -1;
	}
	m_sWaitMsgAct = "";
	return module_list.count();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取代理服务端的运行信息
// 作    者:  邵凯田
// 创建时间:  2012-9-26 11:44
// 参数说明:  @sDisk 表示需要读取的硬盘信息，disks=3;disk1=/dev/sda1;disk2=/dev/sda2;disk3=/dev/sda3;
//            @sRunInfo引用返回的以字符串描述的运行信息,act=runinfo;cpu_used=%d;mem_all=%d;mem_used=%d;mem_free=%d;disk1_all=%d;disk1_used=%d;disk1_free=%d;disk1_per=%d;
// 返 回 值:  1表示成功，0表示失败
//////////////////////////////////////////////////////////////////////////
bool CSAgentClient::GetRunInfo(SString sDisk,SString &sRunInfo)
{
	SString sMsg = "act=runinfo;"+sDisk;
	m_sWaitMsgAct = "runinfo";
	m_sWaitMsg = "";
	this->AddMsgToAgent(sMsg);
	int i;
	for(i=0;i<200;i++)
	{
		if(m_sWaitMsg.length() > 0)
			break;
		SApi::UsSleep(10000);
	}
	if(m_sWaitMsg.length() > 0)
	{
		sRunInfo = m_sWaitMsg;
	}
	else
	{
		return false;
	}
	m_sWaitMsgAct = "";
	return true;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  取指定模块的在代理进程上的句柄
// 作    者:  邵凯田
// 创建时间:  2011-3-29 14:42
// 参数说明:  模块名称，即进程名称，WIN32平台上不需要带.exe
// 返 回 值:  =0表示模块未找到，>0表示进程句柄
//////////////////////////////////////////////////////////////////////////
int CSAgentClient::GetModuleHandle(SString sModName)
{
	SPtrList<stuModuleInfo> module_list;
	module_list.setAutoDelete(true);
	if(GetModuleList(module_list) < 0)
		return 0;
	SString name2 = sModName+".exe";
	unsigned long pos;
	stuModuleInfo *pInfo = module_list.FetchFirst(pos);
	while(pInfo)
	{
		if(pInfo->name == sModName || pInfo->name == name2)
		{
			return pInfo->handle;
		}
		pInfo = module_list.FetchNext(pos);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  向指定的模块发送一个消息
// 作    者:  邵凯田
// 创建时间:  2011-3-29 11:06
// 参数说明:  to_handle为目标模块的句柄
// 返 回 值:  发送成功返回ture，失败返回false
//////////////////////////////////////////////////////////////////////////
bool CSAgentClient::AddMsgToModule(int to_handle,SString &sMsg,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	sMsg.replace("act=","act2=");
	if(sMsg.right(1) != ";")
		sMsg += ";";
	sMsg += "act=prox;to_handle="+SString::toString(to_handle)+";";
	AddMsgToAgent(sMsg,pBuffer,iLength);
	return true;
}

bool CSAgentClient::ProcessAgentMsgBase(SString &sMsg,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	if(m_pApp == NULL)
		return false;
	SString sRet = "";
	//LOGDEBUG("收到代理消息:%s",sMsg.data());
	SString act = SString::GetAttributeValue(sMsg,"act");
	if(m_sWaitMsgAct == act)
	{
		m_sWaitMsg = sMsg;
		return true;
	}
	if(act == "start")
	{
		LOGWARN("代理消息要求启动程序!");
		if(m_pApp->IsStarted() == true)
		{
			LOGERROR("应用已经启动！忽略启动命令！");
		}
		else
		{
			m_pApp->m_bHalt = false;
			m_pApp->SetStarted(m_pApp->Start());
		}
	}
	else if(act == "stop")
	{
		LOGWARN("代理消息要求关闭程序!");
		if(m_pApp->IsStarted() == false)
		{
			LOGERROR("应用尚未启动！忽略关闭命令！");
		}
		else
		{
			m_pApp->m_bHalt = true;
			m_pApp->Stop();
			m_pApp->SetStarted(false);
		}
	}
	else if(act == "restart")
	{
		LOGWARN("收到代理消息要求重启应用!");
		m_pApp->Stop();
		if(!m_pApp->Start())
		{
			m_pApp->SetStarted(false);
			LOGFAULT("启动应用失败!");
			return false;
		}
		else
		{
			m_pApp->SetStarted(true);
		}
	}
	else if(act == "log")
	{
		SString cmd = SString::GetAttributeValue(sMsg,"cmd");
		m_pApp->ParseCommandLine(cmd);
	}
	else if(act == "cpupercent")
	{
		sRet.sprintf("cpu=%d;%s",m_iCpuPercent,sMsg.data());
		AddMsgToAgent(sRet);
	}
	else if(act == "info")
	{
		//先更新模块信息
		ProcessAgentMsg(0,sMsg,pBuffer,iLength);

		sRet = "info="+m_pApp->m_sModuleInfo+";"+sMsg;
		AddMsgToAgent(sRet);
	}
	else if(act == "putfile")
	{
		//向指定目录写入文件
		SString pathfile = SString::GetAttributeValue(sMsg,"pathfile");
		SFile file(pathfile);
		if(!file.open(IO_Truncate))
		{
			sRet += ";result=0;";
		}
		else
		{
			if(pBuffer != NULL && iLength > 0)
				file.writeBlock(pBuffer,iLength);
			file.close();
			sRet += ";result=1;";
		}
		AddMsgToAgent(sRet);
	}
	else if(act == "getfile")
	{
		//读取指定的文件
		SString pathfile = SString::GetAttributeValue(sMsg,"pathfile");
		SFile file(pathfile);
		if(!file.open(IO_ReadOnly))
		{
			sRet += ";result=0;";
			AddMsgToAgent(sRet);
		}
		else
		{
			int size = file.size();
			if(size == 0)
			{
				sRet += ";result=1;";
				AddMsgToAgent(sRet);
			}
			else
			{
				BYTE *pFileBuf = new BYTE[size];
				if(file.readBlock(pFileBuf,size) != size)
				{
					sRet += ";result=0;";
					AddMsgToAgent(sRet);
					delete[] pFileBuf;
				}
				else
				{
					sRet += ";result=1;";
					AddMsgToAgent(sRet,pFileBuf,size);

				}
				file.close();
			}
		}
	}
	else
		return false;
	return true;
}

int CSAgentClient::ReceiveMsg(SSocket *pSocket,SString &sMsg,BYTE* &pBuffer,int &iLength)
{
	BYTE buf[16];
	int ret,len1,len2;
	ret = pSocket->Receive(buf,10);
	if(ret == 0)
		return 0;
	if(ret != 10)
	{
		return -1;
	}
	if(buf[0] != 0xEC || buf[1] != 0x57)
	{
		return -2;
	}
	memcpy(&len1,buf+2,4);
	memcpy(&len2,buf+6,4);

	//大端字节序
	SKT_SWAP_DWORD(len1);
	SKT_SWAP_DWORD(len2);
	//SApi::Ysntoh(SKT_LITTLE_ENDIAN,(DWORD)len1);
	//SApi::Ysntoh(SKT_LITTLE_ENDIAN,(DWORD)len2);

	sMsg.SetBuffer(len1+1);
	char *pStr = sMsg.data();
	ret = pSocket->Receive(pStr,len1);
	sMsg.PrepareStrLen();
	if(ret != len1)
	{
		int times = 0;
		while(ret == 0 && times < 1000)
		{
			times++;
			SApi::UsSleep(10000);
			ret = pSocket->Receive(pStr,len1);
		}
		if(ret != len1)
			return -3;
	}
	pStr[len1] = '\0';
	if(len2 > 0)
	{
		if(len2 > iLength)
		{
			if(pBuffer != NULL)
				delete[] pBuffer;
			pBuffer = new BYTE[len2];
		}
		iLength = len2;
		ret = pSocket->Receive(pBuffer,len2);
		iLength = len2;
	}
	else
	{
		pBuffer = NULL;
		iLength = 0;
	}
	//LOGDEBUG("RECV:%s",sMsg.data());
	return len1;
}

int CSAgentClient::SendMsg(SSocket *pSocket,SString &sMsg,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	BYTE buf[16];
	int ret,len1,len2;
	buf[0] = 0xEC;
	buf[1] = 0x57;
	len1 = sMsg.length();
	len2 = iLength;

	//大端字节序
	SKT_SWAP_DWORD(len1);
	SKT_SWAP_DWORD(len2);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len1);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len2);

	memcpy(buf+2,&len1,4);
	memcpy(buf+6,&len2,4);

	//大端字节序
	SKT_SWAP_DWORD(len1);
	SKT_SWAP_DWORD(len2);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len1);
	//SApi::Yshton(SKT_LITTLE_ENDIAN,(DWORD)len2);

	ret = pSocket->Send(buf,10);
	if(ret != 10)
		return -1;

	ret = pSocket->Send(sMsg.data(),len1);
	if(ret != len1)
		return -2;
	if(len2 != 0 && pBuffer != NULL)
	{
		ret = pSocket->Send(pBuffer,len2);
		if(ret != len2)
			return -3;
	}
	//LOGDEBUG("SEND:%s",sMsg.data());
	return len1;
}
#endif	

//////////////////////////////////////////////////////////////////////////
// 名    称:  SApplication
// 作    者:  邵凯田
// 创建时间:  2008-10-10 14:16
// 描    述:  应用程序管理类
//////////////////////////////////////////////////////////////////////////

SApplication::SApplication()
{
	m_sAppbaseVer = SKTBASE_VERSION;
	m_iArgc = 0;
	m_ppArgv = NULL;
	m_bHalt = false;
	m_bDebugMode = false;
	m_bLogToConsole = true;
	m_bFork = true;
	m_bStarted = false;
#ifndef _WITHOUT_AGENT_CHANNEL_
	m_pApp = this;
#endif
}

SApplication::~SApplication()
{
	//SLog::WaitForLogEmpty();
}

bool SApplication::ParseCommandLine(SString str)
{
	if(str == "log level none")
	{
		SLog::setLogLevel(SLog::LOG_NONE);
	}
	else if(str == "log level fault")
	{
		SLog::setLogLevel(SLog::LOG_FAULT);
	}
	else if(str == "log level error")
	{
		SLog::setLogLevel(SLog::LOG_ERROR);
	}
	else if(str == "log level warn")
	{
		SLog::setLogLevel(SLog::LOG_WARN);
	}
	else if(str == "log level debug")
	{
		SLog::setLogLevel(SLog::LOG_DEBUG);
	}
	else if(str == "log level basedebug")
	{
		SLog::setLogLevel(SLog::LOG_BASEDEBUG);
	}
	else if(str == "log to console on")
	{
		SLog::startLogToConsole();
	}
	else if(str == "log to console off")
	{
		SLog::stopLogToConsole();
	}
	else if(str.left(12) == "log to file ")
	{
		SLog::startLogToFile(str.mid(12).data());
	}
	else if(str.left(14) == "log file size ")
	{
		SLog::setLogFileMaxSize(str.mid(14).toInt());
	}
	else if(str.left(19) == "log file back size ")
	{
		SLog::setLogFileMaxBackFiles(str.mid(19).toInt());
	}
	else if(str == "log to file off")
	{
		SLog::stopLogToFile();
	}
	else if(str == "log to tcp off" || str == "log to TCP off")
	{
		SLog::stopLogToTCPSvr();
	}
	else if(str.left(11) == "log to tcp " || str.left(11) == "log to TCP ")
	{
		SString ip = str.mid(11);
		int port=ip.find(":");
		if(port > 0)
		{
			port = ip.mid(ip.find(":")+1).toInt();
			ip = ip.left(ip.find(":"));
		}
		SLog::startLogToTCPSvr(ip.data(),port);
	}
	else if(str == "log to udp off" || str == "log to UDP off")
	{
		SLog::stopLogToUDPSvr();
	}
	else if(str.left(11) == "log to udp " || str.left(11) == "log to UDP ")
	{
		SString ip = str.mid(11);
		int port=ip.find(":");
		if(port > 0)
		{
			port = ip.mid(ip.find(":")+1).toInt();
			ip = ip.left(ip.find(":"));
		}
		SLog::startLogToUDPSvr(ip.data(),port);
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置模块版本号
// 作    者:  邵凯田
// 创建时间:  2009-5-19 10:56
// 参数说明:  sVer(采用:<产品内部型号>.<阶段版本号>.<模块版本号>)
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void SApplication::SetVersion(SString sVer)
{
	m_sModuleVer = sVer;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置模块描述
// 作    者:  邵凯田
// 创建时间:  2009-5-19 12:29
// 参数说明:  sDesc描述
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void SApplication::SetModuleDesc(SString sDesc)
{
	m_sModuleDesc = sDesc;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置模块信息
// 作    者:  邵凯田
// 创建时间:  2009-5-19 12:29
// 参数说明:   sInfo = name=;ver=;crc=;date=
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void SApplication::SetModuleInfo(SString sInfo)
{
	m_sModuleInfo	= sInfo;
	m_sModule		= SString::GetAttributeValue(sInfo,"name");
	m_sModuleVer	= SString::GetAttributeValue(sInfo,"ver");
	m_sModuleCrc	= SString::GetAttributeValue(sInfo,"crc");
	m_sModuleTime	=SString::GetAttributeValue(sInfo,"date");
	if (m_sModuleVer.left(1).toLower() == "v")
		m_sModuleVer= m_sModuleVer.mid(1);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  启动应用程序
// 作    者:  邵凯田
// 创建时间:  2008-10-10 14:17
// 参数说明:  @argc/argv为main输入参数
//            @szDate/szTime为编译日期时间
//            @bBlock为是否阻塞运行，true为阻塞，false为不阻塞
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SApplication::Run(int argc, char* argv[],const char* szDate,const char* szTime,bool bBlock/*=true*/)
{
	m_iArgc = argc;
	m_ppArgv = (char**)argv;
	m_sModule = argv[0];
	#ifndef _WITHOUT_AGENT_CHANNEL_
	m_sAgentIp = "127.0.0.1";
	#endif
	SString err;
	SString crc = SApi::GenerateCRCByFile(m_sModule,err);
	m_sModuleCrc = crc;
	m_sModuleTime.sprintf("%s %s",szDate,szTime);
	
	if(m_sModule.findRev('/') >= 0)
		m_sModule = m_sModule.mid(m_sModule.findRev('/')+1);
	if(m_sModule.findRev('\\') >= 0)
		m_sModule = m_sModule.mid(m_sModule.findRev('\\')+1);

	printf("#########################################################\n");
	printf("## SKTBASE Application frame\n");
	printf("## Module  Name: %s \n",m_sModule.data());
	printf("## Description : %s \n",m_sModuleDesc.data());
	printf("## Compile Time: %s %s\n",szDate,szTime);
	printf("## Appbase Ver : V%s \n",m_sAppbaseVer.data());
	printf("## Module  Ver : V%s \n",m_sModuleVer.data());
	printf("## CRC    Code : %s \n",crc.data());
	printf("#########################################################\n");

	SString sTemp="";
	if(IsParam("version",sTemp))
	{
		bool bWrite = false;
		SString sLine;
		SFile file;
		if(sTemp.length() > 0)
		{
			file.setFileName(sTemp);
			if(file.open(IO_Truncate))
				bWrite = true;
		}
		if(bWrite)
		{
			sLine.sprintf("#########################################################\n");
			file.writeString(sLine);
			sLine.sprintf("## SKTBASE Application frame\n");
			file.writeString(sLine);
			sLine.sprintf("## Module  Name: %s \n",m_sModule.data());
			file.writeString(sLine);
			sLine.sprintf("## Description : %s \n",m_sModuleDesc.data());
			file.writeString(sLine);
			sLine.sprintf("## Compile Time: %s %s\n",szDate,szTime);
			file.writeString(sLine);
			sLine.sprintf("## Appbase Ver : V%s \n",m_sAppbaseVer.data());
			file.writeString(sLine);
			sLine.sprintf("## Module  Ver : V%s \n",m_sModuleVer.data());
			file.writeString(sLine);
			sLine.sprintf("## CRC    Code : %s \n",crc.data());
			file.writeString(sLine);
			sLine.sprintf("#########################################################\n");
			file.writeString(sLine);
			file.close();
		}
		exit(0);
		abort();
	}


	if(m_sModuleDesc.length() == 0)
	{
		printf("ERROR!!! 缺少模块描述信息!请调用基类CSRecApplication::SetModuleDesc方法!\n\n\n");
	}
	if(m_sModuleVer.length() == 0)
	{
		printf("ERROR!!! 缺少模块版本信息!请调用基类CSRecApplication::SetVersion方法!\n\n\n");
	}
#ifndef WIN32
	signal(SIGPIPE,SIG_IGN);//忽略管道断开信号
	signal(SIGCHLD,SIG_IGN);
	signal(SIGCLD, SIG_IGN);
#endif

#ifndef WIN32
	if(m_bFork)
	{
		//LINUX下将进程设置为守护进程
		//printf("@@@@@@@@@@@@begin fork()\n");
		if(fork())
		{
			//printf("@@@@@@@@@@@@fork()\n");
			exit(0);
		}
	}
	SApi::UsSleep(100000);
	signal(SIGCHLD,SIG_IGN);
#endif

	SLog::setLogLevel(SLog::LOG_DEBUG);
	SLog::setModuleName(m_sModule.data());

	//预处理命令
	SXmlConfig xml;
	SString sFileName;
	m_sExePath = SDir::currentDirPath();
	if(m_sExePath.right(1) != "/" && m_sExePath.right(1) != "\\")
		m_sExePath += "/";
	sFileName=m_sExePath + "logconfig.xml";
	if(!xml.ReadConfig(sFileName))
	{
		LOGWARN("打开<logconfig.xml>文件失败!将无法使用日志记录功能! file=%s",sFileName.data());
	}
	else
	{
		sFileName = m_sModule;
		SBaseConfig *pPrecommand = xml.SearchChild("log<module='"+sFileName+"'>");
		if(pPrecommand == NULL)
		{
			if(sFileName.right(4).toLower() == ".exe")
				sFileName = sFileName.left(sFileName.length()-4);
			pPrecommand = xml.SearchChild("log<module='"+sFileName+"'>");
		}

		if(pPrecommand == NULL)
		{
			//没有专用的，找缺省的
			pPrecommand = xml.SearchChild("log<module=''>");
		}
		if(pPrecommand != NULL)
		{
			//预处理命令
			int i,cnt = pPrecommand==NULL?0:pPrecommand->GetChildCount("command");
			for(i=0;i<cnt;i++)
			{
				SBaseConfig *pCommand = pPrecommand->GetChildNode(i,"command");
				if(pCommand == NULL )
					continue;
				SString cmd = pCommand->GetNodeValue();
				if(cmd.find("[@modulename]") >= 0)
				{
					SString sModule = SLog::getModuleName();
					cmd = cmd.replace("[@modulename]",sModule);
				}
				ParseCommandLine(cmd);
			}
		}
	}

	//是否处于调试模式
	if(IsParam("debug"))
		m_bDebugMode = true;

	if(IsParam("console",sTemp))
	{
		sTemp = sTemp.toLower();
		if(sTemp == "off")
		{
			//强制关闭控制台的日志输出
			m_bLogToConsole = false;
			ParseCommandLine("log to console off");
		}
	}
#ifndef _WITHOUT_AGENT_CHANNEL_
	if(IsParam("agentport",sTemp))
	{
		m_iAgentPort = sTemp.toInt();
		if(m_iAgentPort == 0)
			m_iAgentPort = 6666;
	}
#endif
	if(IsParam("stop"))
	{
		if(!_Stop())
		{
			LOGFAULT("停止应用失败!");
			//SLog::WaitForLogEmpty();
			return false;
		}
		return true;
	}
	else if(IsParam("restart"))
	{
		_Stop();
		if(!_Start())
		{
			LOGFAULT("启动应用失败!");
			//SLog::WaitForLogEmpty();
			return false;
		}
	}
	else
	{
		if(!_Start())
		{
			LOGFAULT("启动应用失败!");
			//SLog::WaitForLogEmpty();
			return false;
		}
	}
	LOGDEBUG("应用服务启动完成!");

	if(bBlock)
	{
		//阻塞方式运行
#ifndef _WITHOUT_AGENT_CHANNEL_
		StartAgent(bBlock);
#else
		while(!m_bHalt)
		{
			//等待有人发出退出信号
			if(IsClosedExist())
			{
				LOGWARN("收到退出信号! 准备关闭当前进程!");
				Stop();
				//准备关闭当前进程
				this->RemoveClose();
				break;
			}
			SApi::UsSleep(2000000);
		}
#endif
	}
	else
	{
		//非阻塞方式运行
#ifndef _WITHOUT_AGENT_CHANNEL_
		StartAgent(bBlock);
#endif
	}

	LOGDEBUG("应用服务已退出!");
	//SLog::WaitForLogEmpty();
	return true;
}


bool SApplication::IsParam(SString sParamName)
{
	SString sLine;
	for(int i=1;i<m_iArgc;i++)
	{
		sLine = m_ppArgv[i];
		if(sLine.left(1) == "-")
			sLine = sLine.mid(1);
		if(SString::GetIdAttribute(1,sLine,"=") == sParamName)
			return true;
	}
	return false;
}
bool SApplication::IsParam(SString sParamName,SString &sParamValue)
{
	SString sLine;
	for(int i=1;i<m_iArgc;i++)
	{
		sLine = m_ppArgv[i];
		if(sLine.left(1) == "-")
			sLine = sLine.mid(1);
		if(SString::GetIdAttribute(1,sLine,"=") == sParamName)
		{
			sParamValue = SString::GetIdAttribute(2,sLine,"=");
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取命令行参数内容
// 作    者:  邵凯田
// 创建时间:  2014-5-19 10:13
// 参数说明:  @sParamName为参数名
// 返 回 值:  参数值
//////////////////////////////////////////////////////////////////////////
SString SApplication::GetParam(SString sParamName)
{
	SString sLine;
	for(int i=1;i<m_iArgc;i++)
	{
		sLine = m_ppArgv[i];
		if(sLine.left(1) == "-")
			sLine = sLine.mid(1);
		if(SString::GetIdAttribute(1,sLine,"=") == sParamName)
		{
			return SString::GetIdAttribute(2,sLine,"=");
		}
	}
	return "";
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取命令行整型参数内容
// 作    者:  邵凯田
// 创建时间:  2014-5-19 10:13
// 参数说明:  @sParamName为参数名
// 返 回 值:  参数值
//////////////////////////////////////////////////////////////////////////
int SApplication::GetParamI(SString sParamName)
{
	return GetParam(sParamName).toInt();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取命令行浮点型参数内容
// 作    者:  邵凯田
// 创建时间:  2014-5-19 10:13
// 参数说明:  @sParamName为参数名
// 返 回 值:  参数值
//////////////////////////////////////////////////////////////////////////
float SApplication::GetParamF(SString sParamName)
{
	return GetParam(sParamName).toFloat();
}

bool SApplication::TouchClose()
{
#ifdef WIN32
	SFile file(m_sExePath+"_skt_close_"+m_sModule);
#else
	SFile file("/proc/.skt_close_"+m_sModule);
#endif
	if(!file.open(IO_Truncate))
		return false;
	file.close();
	return true;
}

bool SApplication::RemoveClose()
{
#ifdef WIN32
	return SFile::remove(m_sExePath+"_skt_close_"+m_sModule);
#else
	return SFile::remove("/proc/.skt_close_"+m_sModule);
#endif
}

bool SApplication::IsClosedExist()
{
#ifdef WIN32
	SFile file(m_sExePath+"_skt_close_"+m_sModule);
#else
	SFile file("/proc/.skt_close_"+m_sModule);
#endif
	if(!file.open(IO_ReadOnly))
		return false;
	file.close();
	return true;
}

bool SApplication::SearchOldProcess(int &pid,int &ppid,SString &name)
{
	//进程是否存在
	SStringList plist;//pid,ppid,name;....
	SString sLine;
	int p,i,cnt = SApi::GetSystemProcess(plist);
	for(i=0;i<cnt;i++)
	{
		sLine = *plist[i];
		//LOGDEBUG("MOUDLE:%s",sLine.data());
		pid = SString::GetIdAttributeI(1,sLine,",");
		ppid= SString::GetIdAttributeI(2,sLine,",");
		name= SString::GetIdAttribute( 3,sLine,",");
#ifdef WIN32
		name += ".exe";
#endif

		if(pid == SApi::GetPid())
			continue;//忽略自身的进程
		p = name.find(m_sModule);
		if(p < 0)
			continue;
		if(name.length() == p+m_sModule.length() ||
			 name.at(p+m_sModule.length()) == ' ')
		{
			LOGDEBUG("得到当前进程对应的进程!");
			return true;
		}
	}
	return false;
}

bool SApplication::_Start()
{
	if(m_bStarted)
		return false;

	//进程是否存在
	int pid,ppid;
	SString name;
	if(SearchOldProcess(pid,ppid,name) && m_sModule.Find("watch") < 0)
	{
		LOGWARN("当前进程的另一个实例已运行(pid:%d,ppid%d,name:%s),当前进程将中止运行!!!",pid,ppid,name.data());
		return false;
	}
	if(Start())
	{
		m_bStarted = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool SApplication::_Stop()
{
	if(!m_bStarted)
		return false;

	//进程是否存在
	int pid,ppid;
	SString name;
	if(!SearchOldProcess(pid,ppid,name) && m_sModule.Find("watch") < 0)
		return false;

	//通知pid进程关闭自身进程
	TouchClose();
	//等待结果
	time_t begin = ::time(NULL);
	while(1)
	{
		SApi::UsSleep(100000);
		if(!IsClosedExist())
			break;
		if(::time(NULL) - begin > 10)
		{
			LOGWARN("等待进程关闭超时，将直接杀死进程(pid=%d,name=%s)",pid,name.data());
			SApi::SystemKillProcess(pid);
			RemoveClose();
			break;
		}
	}
	if(!IsClosedExist())
	{
		m_bStarted = false;
		return true;
	}
	else
	{
		RemoveClose();
		return false;
	}
}
