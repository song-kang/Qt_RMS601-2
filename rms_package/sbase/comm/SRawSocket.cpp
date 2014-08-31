/**
 *
 * 文 件 名 : SRawSocketRecv.cpp
 * 创建日期 : 2011-12-1 10:38
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/

#include "SRawSocket.h"

SRawSocketRecv::SRawSocketRecv()
{
	m_pCallbackOnReceive = NULL;
	m_pCallbackUserPtr = NULL;
	m_iRecvBufferSize = 20*1024*1024;
#ifdef WIN32
	m_pCap=NULL;
#else
	m_pRawsock = NULL;
#endif
	m_pGetWriteBuffer = NULL;
	m_iMaxWriteBufferLen = 0;
	m_pFilterBytes = NULL;
	m_iFilterByteLen = 0;
}

SRawSocketRecv::~SRawSocketRecv()
{
#ifdef WIN32
	if(m_pCap != NULL)
	{
		pcap_close(m_pCap);
		m_pCap = NULL;
	}
#endif
	if(m_pFilterBytes != NULL)
		delete[] m_pFilterBytes;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  启动服务
// 作    者:  邵凯田
// 创建时间:  2011-12-1 10:37
// 参数说明:  void
// 返 回 值:  true表示服务启动成功,false表示服务启动失败
//////////////////////////////////////////////////////////////////////////
bool SRawSocketRecv::Start()
{
	if(!SService::Start())
		return false;
#ifdef WIN32
	if ((m_pCap = pcap_open_live(m_sDevName.data(),		// name of the device
							 65536,			// portion of the packet to capture. It doesn't matter in this case 
							 1,				// promiscuous mode (nonzero means promiscuous)
							 1000,			// read timeout
							 m_ErrBuf			// error buffer
							 )) == NULL)
	{
		LOGERROR("无法打开网卡. '%s' 不是WinPcap支持的有效设备名!", m_sDevName.data());
		return false;
	}
#else
	int eth_index = if_nametoindex(m_sDevName.data());
	if(eth_index == 0)
	{
		LOGERROR("无效的网络设备名[%s]",m_sDevName.data());
		return false;
	}
#endif
	SKT_CREATE_THREAD_S(ThreadPcap,this,512000);
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止服务
// 作    者:  邵凯田
// 创建时间:  2011-12-1 10:37
// 参数说明:  void
// 返 回 值:  true表示服务停止成功,false表示服务停止失败
//////////////////////////////////////////////////////////////////////////
bool SRawSocketRecv::Stop()
{
	if(!SService::Stop())
		return false;

#ifdef WIN32
	if(m_pCap != NULL)
	{
		pcap_breakloop(m_pCap);
		while(!IsStoped())
			SApi::UsSleep(1000);
		pcap_close(m_pCap);
		m_pCap = NULL;
	}
#else
	if(m_pRawsock != NULL && *m_pRawsock > 0)
	{
		shutdown(*m_pRawsock, 2);
		close(*m_pRawsock);
		while(!IsStoped())
			SApi::UsSleep(1000);
		*m_pRawsock = 0;
	}

#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  设置过滤字节
// 作    者:  邵凯田
// 创建时间:  2012-2-1 8:47
// 参数说明:  @pFilterBytes过滤字节首地址
//            @len过滤字节长度
//            @iFilterOffsetPos过滤位置，批在原始报文中的偏移字节数
//            @bFilterExcept:true表示过滤掉含有指定内容的报文，false表示过滤掉不含指定内容的报文
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SRawSocketRecv::SetFilterBytes(BYTE *pFilterBytes,int len,int iFilterOffsetPos,bool bFilterExcept)
{
	if(m_pFilterBytes != NULL)
	{
		delete[] m_pFilterBytes;
	}
	m_pFilterBytes = new BYTE[len];
	memcpy(m_pFilterBytes,pFilterBytes,len);
	m_iFilterByteLen = len;
	m_iFilterOffsetPos = iFilterOffsetPos;
	m_bFilterExcept = bFilterExcept;
}

#ifdef WIN32
void SRawSocketRecv::ys_packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	SRawSocketRecv *pThis = (SRawSocketRecv*)param;
	if(pThis->m_pFilterBytes != NULL)
	{
		if(pThis->m_bFilterExcept)
		{
			if(memcmp(((BYTE*)pkt_data)+pThis->m_iFilterOffsetPos,pThis->m_pFilterBytes,pThis->m_iFilterByteLen) == 0)
				return;

		}
		else
		{
			if(memcmp(((BYTE*)pkt_data)+pThis->m_iFilterOffsetPos,pThis->m_pFilterBytes,pThis->m_iFilterByteLen) != 0)
				return;
		}
	}
	if(pThis->m_pGetWriteBuffer != NULL)
	{
		BYTE *pBuf = pThis->m_pGetWriteBuffer(pThis->m_pCallbackUserPtr);
		memcpy(pBuf,(BYTE*)pkt_data,header->len);
		if(pThis->m_pCallbackOnReceive == NULL)
		{
			pThis->OnReceive(header->ts.tv_sec,header->ts.tv_usec,(BYTE*)pkt_data,header->len);
			return;
		}
		pThis->m_pCallbackOnReceive(pThis->m_pCallbackUserPtr,header->ts.tv_sec,header->ts.tv_usec,pBuf,header->len);
		return;
	}
	if(pThis->m_pCallbackOnReceive == NULL)
	{
		pThis->OnReceive(header->ts.tv_sec,header->ts.tv_usec,(BYTE*)pkt_data,header->caplen);
		return;
	}
	pThis->m_pCallbackOnReceive(pThis->m_pCallbackUserPtr,header->ts.tv_sec,header->ts.tv_usec,(BYTE*)pkt_data,header->caplen);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  PCAP抓包线程
// 作    者:  邵凯田
// 创建时间:  2011-12-1 9:35
// 参数说明:  this
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void* SRawSocketRecv::ThreadPcap(void *lp)
{
	SRawSocketRecv *pThis = (SRawSocketRecv*)lp;
	if(pThis->m_pCap == NULL)
		return NULL;
	pThis->BeginThread();
	pcap_loop(pThis->m_pCap, 0, ys_packet_handler, (BYTE*)lp);
	pThis->EndThread();
	return NULL;
}

#else

void SRawSocketRecv::raw_die(char *why, int n) 
{ 
    perror(why); 
    exit(n); 
}

//修改网卡成PROMISC(混杂)模式 
bool SRawSocketRecv::raw_do_promisc(char *nif, int sock ) 
{
	LOGDEBUG("设置网卡(%s)为混杂模式!",nif);

    struct ifreq ifr; 
    strncpy(ifr.ifr_name, nif,strlen(nif)+1); 
    if ((ioctl(sock, SIOCGIFFLAGS, &ifr) == -1)) //获得flag 
    { 
		LOGERROR("Error in raw_do_promisc");
        return false;//raw_die("ioctl", 2); 
    } 
    ifr.ifr_flags |= IFF_PROMISC; //重置flag标志 
    if (ioctl(sock, SIOCSIFFLAGS, &ifr) == -1 ) //改变模式 
    { 
		LOGERROR("Error in raw_do_promisc");
        return false;//raw_die("ioctl", 3); 
    }
	return true;
}

void SRawSocketRecv::raw_set_recvbuffer(int sock,int iBufferSize)
{
	socklen_t sizelen = 4;
	int ret;
	ret = setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char*)&iBufferSize,sizelen);
	if (ret != 0)
	{
		int err;
		#ifdef WIN32
			err = WSAGetLastError();
		#else
			err = errno;
		#endif
		LOGWARN("Error in raw_set_recvbuffer(%d), err=%d",iBufferSize,err);
	}
	else
	{
		iBufferSize = 0;
		ret = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char*)&iBufferSize,&sizelen);
		LOGDEBUG("getsockopt=%d bytes!",iBufferSize);
	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  PCAP抓包线程
// 作    者:  邵凯田
// 创建时间:  2011-12-1 9:35
// 参数说明:  this
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void* SRawSocketRecv::ThreadPcap(void *lp)
{
	SRawSocketRecv *pThis = (SRawSocketRecv*)lp;
	pThis->BeginThread();
    struct sockaddr_ll addr; 

    char mac[16]; 
    int i,sock, r, len,soc_usec[2];
    char *data; 
    char *ptemp; 
    char ss[32],dd[32];
	soc_usec[0]=0;
	soc_usec[1]=0;
    if ((sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) //建立socket//man socket可以看到上面几个宏的意思以看 
    { 
        raw_die("socket", 1); 
    } 
	pThis->m_pRawsock = &sock;
	if(pThis->m_iRecvBufferSize > 0)
	{
		raw_set_recvbuffer(sock,pThis->m_iRecvBufferSize/4);
		raw_set_recvbuffer(sock,pThis->m_iRecvBufferSize/2);
		raw_set_recvbuffer(sock,pThis->m_iRecvBufferSize);
	}

    bool bethok = raw_do_promisc((char*)pThis->m_sDevName.data()/*"eth0"*/, sock); //eth0为网卡名 
	int eth_index = if_nametoindex(pThis->m_sDevName.data());
	if(!bethok || eth_index == 0)
	{
		LOGFAULT("无效的监听网卡(%s),启动原始抓包失败!!!",pThis->m_sDevName.data());
		pThis->EndThread();
		return 0;
	}
	LOGDEBUG("监听网卡(%s)在ifindex=%d上!",pThis->m_sDevName.data(),eth_index);
	//sll_ifindex; //网络字节序，
    len = sizeof(addr);

	if(pThis->m_pGetWriteBuffer == NULL)
	{
		//没有缓冲区回调
		BYTE buffer[2100];//缓冲区
		while(!pThis->IsQuit())
		{
			r = recvfrom(sock,(char *)buffer,sizeof(buffer), 0, (struct sockaddr *)&addr,(socklen_t *)&len); 

			if(r <= 0 || addr.sll_ifindex != eth_index)
				continue;
			SDateTime::getSystemTime(soc_usec[0],soc_usec[1]);
			if(pThis->m_pFilterBytes != NULL)
			{
				if(pThis->m_bFilterExcept)
				{
					if(memcmp(&buffer[pThis->m_iFilterOffsetPos],pThis->m_pFilterBytes,pThis->m_iFilterByteLen) == 0)
						continue;
				}
				else
				{
					if(memcmp(&buffer[pThis->m_iFilterOffsetPos],pThis->m_pFilterBytes,pThis->m_iFilterByteLen) != 0)
						continue;
				}
			}

			if(pThis->m_pCallbackOnReceive == NULL)
			{
				pThis->OnReceive(soc_usec[0],soc_usec[1],buffer,r);
				continue;
			}
			pThis->m_pCallbackOnReceive(pThis->m_pCallbackUserPtr,soc_usec[0],soc_usec[1],buffer,r);

		} 
	}
	else
	{
		BYTE *pRecvBuffer;
		while(!pThis->IsQuit())
		{
			pRecvBuffer = pThis->m_pGetWriteBuffer(pThis->m_pCallbackUserPtr);
			if(pRecvBuffer == NULL)
			{
				LOGFAULT("缺少有接收缓冲区!接收任务异常结束");
				exit(1);
				break;
			}
			r = recvfrom(sock,(char *)pRecvBuffer,pThis->m_iMaxWriteBufferLen, 0, (struct sockaddr *)&addr,(socklen_t *)&len); 
			if(r <= 0 || addr.sll_ifindex != eth_index)
				continue;
			SDateTime::getSystemTime(soc_usec[0],soc_usec[1]);
			if(pThis->m_pFilterBytes != NULL)
			{
				if(pThis->m_bFilterExcept)
				{
					if(memcmp(pRecvBuffer+pThis->m_iFilterOffsetPos,pThis->m_pFilterBytes,pThis->m_iFilterByteLen) == 0)
						continue;
				}
				else
				{
					if(memcmp(pRecvBuffer+pThis->m_iFilterOffsetPos,pThis->m_pFilterBytes,pThis->m_iFilterByteLen) != 0)
						continue;
				}
			}

			if(pThis->m_pCallbackOnReceive == NULL)
			{
				pThis->OnReceive(soc_usec[0],soc_usec[1],pRecvBuffer,r);
				continue;
			}
			pThis->m_pCallbackOnReceive(pThis->m_pCallbackUserPtr,soc_usec[0],soc_usec[1],pRecvBuffer,r);

		} 
	}
 	pThis->EndThread();
	if(sock > 0)
	{
		shutdown(sock, 2);
		close(sock);
		sock = 0;
	}
   return 0; 
}
#endif


//////////////////////////////////////////////////////////////////////////
// 名    称:  SRawSocketSend
// 作    者:  邵凯田
// 创建时间:  2011-12-1 14:15
// 描    述:  原始套接字的发送类
//////////////////////////////////////////////////////////////////////////

SRawSocketSend::SRawSocketSend()
{
#ifdef WIN32
	m_pCap = NULL;
#else
	m_sock = 0;
#endif
}

SRawSocketSend::~SRawSocketSend()
{
	Close();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  创建一个原始套接字
// 作    者:  邵凯田
// 创建时间:  2011-12-1 14:16
// 参数说明:  @sDevName为选择用于发送的目标网卡名
// 返 回 值:  true表示创建成功，false表示失败 
//////////////////////////////////////////////////////////////////////////
bool SRawSocketSend::Create(SString sDevName/*="eth0"*/)
{
#ifdef WIN32
	if ((m_pCap = pcap_open_live(sDevName.data(),		// name of the device
							 65536,			// portion of the packet to capture. It doesn't matter in this case 
							 1,				// promiscuous mode (nonzero means promiscuous)
							 1000,			// read timeout
							 m_ErrBuf			// error buffer
							 )) == NULL)
	{
		LOGERROR("无法打开网卡. '%s' 不是WinPcap支持的有效设备名!", sDevName.data());
		return false;
	}

#else
    if ((m_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) //建立socket//man socket可以看到上面几个宏的意思以看 
    { 
		LOGERROR("无法创建原始套接字!\n", sDevName.data());
		return false;
    } 

	int eth_index = if_nametoindex(sDevName.data());
	if(eth_index == 0)
	{
		LOGERROR("无效的网卡(%s),创建原始套接字失败!!!",sDevName.data());
		Close();
		return false;
	}
	LOGDEBUG("发送网卡(%s)在ifindex=%d上!",sDevName.data(),eth_index);
	memset(&m_sll,0,sizeof(m_sll));
	m_sll.sll_family = AF_PACKET;
	m_sll.sll_ifindex = eth_index;
	m_sll.sll_protocol = htons(ETH_P_ALL);
	if(bind(m_sock,(struct sockaddr*)&m_sll,sizeof(m_sll)) == -1)
	{
		LOGERROR("绑定网卡(%s,ifindex=%d)时失败!!",sDevName.data(),eth_index);
		Close();
		return false;
	}
	
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  关闭原始套接字
// 作    者:  邵凯田
// 创建时间:  2011-12-1 15:37
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SRawSocketSend::Close()
{
#ifdef WIN32
	if(m_pCap != NULL)
	{
		pcap_close(m_pCap);
		m_pCap = NULL;
	}
#else
	if(m_sock != 0)
	{
		close(m_sock);
		m_sock = 0;
	}
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  发送指定大小的原始报文
// 作    者:  邵凯田
// 创建时间:  2011-12-1 14:18
// 参数说明:  @pBuffer为原始报文全部内容，含目标、源MAC地址
//            @iLen为原始报文总长度，单位字节
// 返 回 值:  >0表示发送成功的字节数，<0表示失败，=0表示发送超时
//////////////////////////////////////////////////////////////////////////
int SRawSocketSend::SendRaw(BYTE *pBuffer,int iLen)
{
#ifdef WIN32
	if(m_pCap != NULL)
	{
		if(pcap_sendpacket(m_pCap,pBuffer,iLen) != -1)
			return iLen;
	}
#else
	if(m_sock != 0)
	{
		return ::sendto(m_sock,pBuffer,iLen,0,(struct sockaddr*)&m_sll,sizeof(m_sll));
	}
#endif
	return -1;
}
