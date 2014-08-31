/**
 * 文 件 名 : PackageApplication.cpp
 * 创建日期 : 2014-4-22 11:32
 * 作    者 : 宋康
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 报文监视的应用程序框架
 **/

#include "PackageApplication.h"

char pcapFrameCache[MAXFRAMECACHE+sizeof(commframe_header_t)];
unsigned int pcapFrameLen;
SLock cacheLock;

void EthFlow(pcap_eth_t *pcpe)
{
	float	v = 0;

	if (pcpe->flow > MBYTE1 * 1024)
	{
		v = (float)pcpe->flow / (MBYTE1 * 1024);
		LOGDEBUG("Ethernet Name: %s, Flow: %5.5f Gbyte.",pcpe->name.c_str(),v);
	}
	else if (pcpe->flow > MBYTE1)
	{
		v = (float)pcpe->flow / (MBYTE1);
		LOGDEBUG("Ethernet Name: %s, Flow: %5.4f Mbyte.",pcpe->name.c_str(),v);
	}
	else if (pcpe->flow > 1024)
	{
		v = (float)pcpe->flow / 1024;
		LOGDEBUG("Ethernet Name: %s, Flow: %5.3f Kbyte.",pcpe->name.c_str(),v);
	}
	else 
	{
		LOGDEBUG("Ethernet Name: %s, Flow: %5.2f byte.",pcpe->name.c_str(),v);
	}
}

CPackageApplication::CPackageApplication(void)
{	
	char t[8] = {'\0'};

	bInitOk = false;
	bCompress = true;
	ethNumber = 8;
#ifndef WIN32
	storePath = "/usr/local/rms_package/";

	m_ethName[0] = "lo";
	for (int i = 1; i < MAXETH; i++)
	{
		memset(t,0,sizeof(t));
		sprintf(t,"eth%d",i-1);
		m_ethName[i] = t; 
	}
#endif
	storeFileSize = MAXZIPLEN;
	storeFileNum = MAXSAVEFILENUM;

	memset(pcapFrameCache,0,sizeof(pcapFrameCache));
	pcapFrameLen = 0;
}

CPackageApplication::~CPackageApplication(void)
{
	//for (int i = 0; i < ethNumber; i++)
	//{
	//	delete pcapEth[i].pcapFile;
	//	delete pcapEth[i].rawSock;
	//	delete [] pcapEth[i].recvBuffer;
	//}
	delete [] pcapEth;
}

void m_OnReceivePcapData(void* cbparam,int soc,int usec,BYTE *pBuffer,int iLength)
{
	char		tempBuffer[32] = {'\0'};
	char		cacheBuffer[32*1024] = {'\0'};
	int			tempLen = 0;
	int			cacheLen = 0;
	SDateTime	dTime;
	int			dSoc;
	int			dUsec;

	pcap_eth_t *pcpe = (pcap_eth_t *)cbparam;

	memcpy(&tempBuffer[tempLen],&soc,sizeof(int));
	tempLen += sizeof(int);
	memcpy(&tempBuffer[tempLen],&usec,sizeof(int));
	tempLen += sizeof(int);
	memcpy(&tempBuffer[tempLen],&iLength,sizeof(int));
	tempLen += sizeof(int);
	memcpy(&tempBuffer[tempLen],&iLength,sizeof(int));
	tempLen += sizeof(int);
	memcpy(&pcpe->recvBuffer[pcpe->bufferLen],tempBuffer,tempLen);
	memcpy(&cacheBuffer[cacheLen],tempBuffer,tempLen);
	pcpe->bufferLen += tempLen;
	cacheLen += tempLen;
	memcpy(&pcpe->recvBuffer[pcpe->bufferLen],pBuffer,iLength);
	memcpy(&cacheBuffer[cacheLen],pBuffer,iLength);
	pcpe->bufferLen += iLength;
	cacheLen += iLength;

	pcpe->pcapFile->CachePcapFrame(pcpe->name,cacheBuffer,cacheLen);
	dTime.getSystemTime(dSoc,dUsec);
	int	timeDiff = dSoc - pcpe->soc;
	if ((timeDiff >= MINCACHETIME && pcpe->bufferLen) || pcpe->bufferLen >= WRITEBUFFER)
	{
		pcpe->flow += pcpe->bufferLen;
		EthFlow(pcpe);
		pcpe->pcapFile->SavePcapFile(pcpe->name,pcpe->recvBuffer,pcpe->bufferLen);
		memset(pcpe->recvBuffer,0,MAXBUFFER);
		pcpe->bufferLen = 0;
		pcpe->soc = dSoc;
		pcpe->usec = dUsec;
	}
}

bool CPackageApplication::Init()
{
	int			i;
#ifdef WIN32
	pcap_if_t	*alldevs;
	pcap_if_t	*d;
	char		errbuf[PCAP_ERRBUF_SIZE+1];
#endif
	int			cnt;
	SDateTime	time;

	if (!mSocket.Create() || !mSocket.Bind(SERVERPORT) || !mSocket.Listen())
		return false;
	SKT_CREATE_THREAD_S(ThreadCommunicationRecv,this,512000);
	SKT_CREATE_THREAD_S(ThreadCommunicationSend,this,512000);

	pcapEth = new pcap_eth_t[ethNumber];
	for (i = 0; i < ethNumber; i++)
	{
		pcapEth[i].pcapFile = new CPcapFile;
		pcapEth[i].rawSock = new SRawSocketRecv;
		pcapEth[i].recvBuffer = new char[MAXBUFFER];
		pcapEth[i].bufferLen = 0;
		pcapEth[i].flow = 0;
		time.getSystemTime(pcapEth[i].soc,pcapEth[i].usec);
#ifdef WIN32
		pcapEth[i].pcapFile->SetPathName(SDir::currentDirPath().data());
#else
		pcapEth[i].pcapFile->SetPathName(storePath.data());
#endif
		pcapEth[i].pcapFile->SetFormat7z(bCompress,bCompress);
		pcapEth[i].pcapFile->SetStoreFileSize(storeFileSize);
		pcapEth[i].pcapFile->SetStoreFileNum(storeFileNum);
	}	

#ifdef WIN32
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		LOGERROR("Error in pcap_findalldevs: %s.", errbuf);
		return false;
	}

	for(d=alldevs,cnt=0 ;d ;d=d->next)
	{
		Ifconfig(d,cnt);
		cnt++;
		if (cnt >= ethNumber)
			break;
	}
	ethNumber = cnt;

	pcap_freealldevs(alldevs);
#else
	for (i = 0; i < ethNumber; i++)
		Ifconfig_linux(i);

#endif

	return true;
}

bool CPackageApplication::Start()
{
	bool bResult;

	ApplicationConfig();

	if (!bInitOk)
	{
		bResult = Init();
		if (!bResult)
			return false;
	}

	return true;
}

bool CPackageApplication::Stop()
{
	for (int i = 0; i < ethNumber; i++)
	{
		pcapEth[i].rawSock->Stop();
	}
	
	return true;
}

bool CPackageApplication::ProcessAgentMsg(int from_handle,SString &sMsg,BYTE* pBuffer,int iLength)
{
	return true;
}

#ifdef WIN32
void CPackageApplication::Ifconfig(pcap_if_t *d,int cnt)
{
	LOGDEBUG("Ethernet%d: %s[%s], Loopback: %s.",
		cnt,d->description,d->name,(d->flags & PCAP_IF_LOOPBACK)?"yes":"no");

	pcapEth[cnt].name = d->description;
	pcapEth[cnt].eth = d->name;
	pcapEth[cnt].rawSock->SetDevName(pcapEth[cnt].eth);
	pcapEth[cnt].rawSock->SetCallback(m_OnReceivePcapData,&pcapEth[cnt]);
	pcapEth[cnt].rawSock->Start();
}
#else
void CPackageApplication::Ifconfig_linux(int cnt)
{
	LOGDEBUG("Ethernet%d: %s.",cnt,m_ethName[cnt].c_str());

	pcapEth[cnt].name = m_ethName[cnt];
	pcapEth[cnt].eth = m_ethName[cnt];
	pcapEth[cnt].rawSock->SetDevName(pcapEth[cnt].eth);
	pcapEth[cnt].rawSock->SetCallback(m_OnReceivePcapData,&pcapEth[cnt]);
	pcapEth[cnt].rawSock->Start();
}
#endif

void CPackageApplication::ApplicationConfig()
{
	SXmlConfig xml;
	SString sFileName;
	SString m_sExePath = SDir::currentDirPath();
	if(m_sExePath.right(1) != "/" && m_sExePath.right(1) != "\\")
		m_sExePath += "/";

	sFileName=m_sExePath + "rms_package.xml";
	if(!xml.ReadConfig(sFileName))
	{
		LOGWARN("打开<rms_package.xml>文件失败,将使用默认配置.file=%s",sFileName.data());
		LOGDEBUG("Use default value, ethNumber = 8.");
		LOGDEBUG("Use default value, storePath = /usr/local/rms_package/.");
		LOGDEBUG("Use default value, compress = 1.");
		LOGDEBUG("Use default value, storeFileSize = 50.");
		LOGDEBUG("Use default value, storeFileNum = 200.");
		return;
	}

	ethNumber = xml.SearchNodeAttributeI("ethNumber","value");
	if (ethNumber == 0)
		ethNumber = MAXETH;
	ethNumber = ethNumber > MAXETH ? MAXETH : ethNumber;
	LOGDEBUG("<rms_package.xml> file, ethNumber = %d.",ethNumber);

	int c = xml.SearchNodeAttributeI("compress","value");
	if (c)
		bCompress = true;
	else
		bCompress = false;
	LOGDEBUG("<rms_package.xml> file, compress = %d.",bCompress);

	storePath = xml.SearchNodeAttribute("storePath","value");
	if (storePath == NULL)
		storePath = "/usr/local/rms_package/";
	LOGDEBUG("<rms_package.xml> file, storePath = %s.",storePath.data());
#ifndef WIN32
	SDir dir;
	if (!dir.dirExists(storePath.data()))
		dir.createDir(storePath.data());
#endif

	storeFileSize = xml.SearchNodeAttributeI("storeFileSize","value");
	if (storeFileSize == 0)
		storeFileSize = MAXZIPLEN;
	else
		storeFileSize = storeFileSize * MBYTE1;
	storeFileSize = storeFileSize > MAXZIPLEN ? MAXZIPLEN : storeFileSize;
	storeFileSize = storeFileSize < MINZIPLEN ? MINZIPLEN : storeFileSize;
	LOGDEBUG("<rms_package.xml> file, storeFileSize = %d.",storeFileSize / MBYTE1);

	storeFileNum = xml.SearchNodeAttributeI("storeFileNum","value");
	if (storeFileNum == 0)
		storeFileNum = MAXSAVEFILENUM;
	storeFileNum = storeFileNum > MAXSAVEFILENUM ? MAXSAVEFILENUM : storeFileNum;
	storeFileNum = storeFileNum < MINSAVEFILENUM ? MINSAVEFILENUM : storeFileNum;
	LOGDEBUG("<rms_package.xml> file, storeFileNum = %d.",storeFileNum);
}

void* CPackageApplication::ThreadCommunicationRecv(void *lp)
{
	int ret = 0;
	unsigned char recvbuf[1024];
	CPackageApplication *p = (CPackageApplication*)lp;

	while(1)
	{
		SSocket *pSocket = new SSocket();
		if (!p->mSocket.Accept(*pSocket))
		{
			delete pSocket;
			SApi::UsSleep(SECOND);
			continue;
		}

		SKT_CREATE_THREAD_S(ThreadCommunicationRecv,p,512000);

		pSocket->SetTimeout(MILLISECOND*5,MILLISECOND*5);
		pSocket->SetRecvBufSize(WRITEBUFFER*5);
		pSocket->SetSendBufSize(MAXFRAMECACHE);
		p->socketList.append(pSocket);
		while(ret >= 0)
		{
			if (!pSocket->IsConnected())
				break;
			ret = pSocket->ReceiveOnce(recvbuf,1024);
			if (ret > 0)
			{
				LOGDEBUG("Recv some data from %s:%d.",pSocket->GetRemoteIpAddr().c_str(),pSocket->GetRemotePort());
			}
		}
		pSocket->Close();
		p->socketList.remove(pSocket);
		delete pSocket;
		break;
	}

#ifdef WIN32
	LOGDEBUG("CommunicationRecv Thread exit. handle=%p.",GetCurrentThreadId());
#else
	LOGDEBUG("CommunicationRecv Thread exit. handle=%p.",pthread_self());
#endif
	SKT_INTO_THREAD;

	return NULL;
}

void* CPackageApplication::ThreadCommunicationSend(void *lp)
{
	int i;
	SSocket * pSocket;
	commframe_header_t commframeHeader;
	CPackageApplication *p = (CPackageApplication*)lp;

	while(1)
	{
		cacheLock.lock();
		if (!pcapFrameLen)
		{
			cacheLock.unlock();
			SApi::UsSleep(SECOND);
			continue;
		}
		commframeHeader.syn = COMMFRAMESYN;
		commframeHeader.len = pcapFrameLen;
		memmove(&pcapFrameCache[sizeof(commframe_header_t)],pcapFrameCache,pcapFrameLen);
		memcpy(pcapFrameCache,&commframeHeader,sizeof(commframe_header_t));
		pcapFrameLen += sizeof(commframe_header_t);
		for (i = 0; i < p->socketList.count(); i++)
		{
			pSocket = p->socketList.at(i);
			if (pSocket->Send(pcapFrameCache,pcapFrameLen) == SOCKET_ERROR)
			{
				pSocket->Close();
				p->socketList.remove(pSocket);
				//delete pSocket;
			}
		}
		memset(pcapFrameCache,0,sizeof(pcapFrameCache));
		pcapFrameLen = 0;
		cacheLock.unlock();
		
		SApi::UsSleep(SECOND);
	}

	return NULL;
}
