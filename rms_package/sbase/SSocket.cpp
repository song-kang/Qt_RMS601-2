/**
 *
 * 文 件 名	: SSocket.cpp
 * 创建日期	: 2006-09-10
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: 2011/08/31 08:43:35 $
 * 当前版本	: $Revision: 1.1 $
 * 功能描述 : SOCKET封装类
 * 修改记录 : 
 *            $Log: SSocket.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:35  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/11 12:08:36  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/01 11:07:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/01/17 01:24:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.6  2009/01/31 06:37:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/10/16 09:16:50  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/10/16 03:14:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/09/26 02:16:21  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/09/16 05:32:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:03  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.11  2008/04/03 02:44:34  shaokt
 *            LINUX下将ysbase分离为.so，单独编译，设备程序在编译时连接该so，不将目标码生成到执行文件中；WIN32下为调试方便还直接编译基类源码
 *
 *            Revision 1.10  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.9  2007/12/21 06:25:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2007/10/30 10:48:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.6  2007/10/17 10:00:16  shaokt
 *            末尾加空行
 *
 *            Revision 1.5  2007/10/09 06:50:41  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/09/30 05:17:31  shaokt
 *            修改SOCKET对网络异常时的处理
 *
 *            Revision 1.3  2007/09/18 02:45:59  shaokt
 *            修改接收40次退出的BUG
 *
 *            Revision 1.2  2007/08/16 05:58:42  shaokt
 *            向SSocket类中增加对多播的支持
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/05/28 07:33:59  shaokt
 *            *** empty log message ***
 *
 **/
 
#include "SSocket.h"
#include "SLog.h"

//用于统计本进程的SOCKET通迅流量
static int g_iRecvCount=0;
static int g_iSendCount=0;

int SSocket::GetRecvCount()//取接收到数据的总数
{
	return g_iRecvCount;
}
int SSocket::GetSendCount()//取发送数据的总数
{
	return g_iSendCount;
}
void SSocket::ResetRecvCount()//清零接收总数
{
	g_iRecvCount = 0;
}
void SSocket::ResetSendCount()//清零发送总数
{
	g_iSendCount = 0;
}
void SSocket::AddRecvCount(int a)//外部添加接收数
{
	g_iRecvCount += a;
}
void SSocket::AddSendCount(int a)//外部添加发送数
{
	g_iSendCount += a;
}

int SSocket::Close()
{
	LOGBASEDEBUG("into SSocket's Close(),this=%p,handle=%X",this,m_hSocket);
	
	int ret=0;
	if(m_hSocket != 0)
	{
		ret=shutdown(m_hSocket, 2);
		#ifdef WIN32
			closesocket(m_hSocket);
		#else
			close(m_hSocket);
		#endif
		m_hSocket = 0;
	}
	return ret;
}

/**
 * 创建SOCKET实例
 */
#ifdef _WIN32
static bool g_bWinSockInit=false;
#endif
bool SSocket::Create(int af, int type, int protocol)
{
	LOGBASEDEBUG("into SSocket's Create(), this=%p",this);
	m_iLastError = 0;
	Close();
	if(type != SOCK_STREAM)
		m_bTcp = false;
	#ifdef _WIN32
		if(!g_bWinSockInit)
		{
			WSADATA WSAData;
			WSAStartup( MAKEWORD( 1, 1 ), &WSAData );
			g_bWinSockInit = true;
		}
	#endif
	m_iAF = af;
	m_iSocketType = type;
	m_hSocket = ::socket(af,type,protocol);

	if(m_hSocket == INVALID_SOCKET)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
	  LOGWARN("error in SSocket's Create() when socket, this=%p, errno=%d",this,m_iLastError);
		return false;
	}
	if(type != SOCK_STREAM)
		return true;
  int bNodelay = 1;  
	int err = setsockopt(m_hSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay));//不采用延时算法 
	if ( err == SOCKET_ERROR ) 
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
	  LOGWARN("error in SSocket's Create() when setsockopt, this=%p, handle=%X,errno=%d",
			this, m_hSocket, m_iLastError);
		Close();
		return false;       
	}

	int   keepAlive   =   1;//设定KeepAlive   
//	int   keepIdle   =   5;//开始首次KeepAlive探测前的TCP空闭时间   
//	int   keepInterval   =   5;//两次KeepAlive探测间的时间间隔   
//	int   keepCount   =   3;//判定断开前的KeepAlive探测次数   
	err = setsockopt(m_hSocket,SOL_SOCKET,SO_KEEPALIVE,(char*)&keepAlive,sizeof(keepAlive));
	if(err == SOCKET_ERROR)
	{   
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's Create() when SO_KEEPALIVE, this=%p, handle=%X,errno=%d",
			this, m_hSocket, m_iLastError);
		Close();
	}   

	return true;
}

bool SSocket::CreateTcp()//创建TCP SOCKET
{
	return Create(AF_INET,SOCK_STREAM,IPPROTO_TCP);
}

bool SSocket::CreateUdp()//创建UDP SOCKET
{
	return Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
}

bool SSocket::CreateBroadCast()//创建一个广播发送SOCKET
{
	if(CreateUdp() == false)
		return false;
	int broadcast = 1; 
//	int num = 0; 
	m_iLastError = 0;

	int ret_val = setsockopt(m_hSocket,SOL_SOCKET,SO_BROADCAST,(const char*)&broadcast,sizeof(broadcast)); 
	if(ret_val == -1) 
	{ 
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's CreateBroadCast() when SO_BROADCAST, this=%p, handle=%X,errno=%d",
			this, m_hSocket, m_iLastError);
		return false;
	} 
	return true;
}

//创建多播 SOCKET
bool SSocket::CreateMultiCast(char* sMultiIp, int iMultiPort, char* sLocalIp,bool bRecv/*=true*/)
{
	m_iLastError = 0;
	LOGDEBUG("Into CreateMultiCase(%s,%d,%s)",sMultiIp,iMultiPort,sLocalIp==NULL?"":sLocalIp);
	if(!Create(AF_INET,SOCK_DGRAM,0/*IPPROTO_IP*/))
	{
		LOGWARN("Error in CreateMultiCase(%s,%d,%s)",sMultiIp,iMultiPort,sLocalIp==NULL?"":sLocalIp);
		return false;
	}
	int ret;
	int iReUse=1; //设置套接字选项，使套接字为可重用端口地址 
	ret = setsockopt(m_hSocket,SOL_SOCKET,SO_REUSEADDR,(char*)&iReUse,sizeof(iReUse)); 
	if (ret < 0)
	{
		LOGWARN("Error in CreateMultiCase(%s,%d,%s)",sMultiIp,iMultiPort,sLocalIp==NULL?"":sLocalIp);
		Close();
		return false;
	}

	int iRouteNum = 1;
	ret = setsockopt(m_hSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&iRouteNum, sizeof(iRouteNum));
	if (ret < 0)
	{
		LOGWARN("Error in CreateMultiCase(%s,%d,%s)",sMultiIp,iMultiPort,sLocalIp==NULL?"":sLocalIp);
		Close();
		return false;
	}
	
	int iLoopBack=1;
	ret = setsockopt(m_hSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&iLoopBack, sizeof(iLoopBack));
	if (ret < 0)
	{
		LOGWARN("Error in CreateMultiCase(%s,%d,%s)",sMultiIp,iMultiPort,sLocalIp==NULL?"":sLocalIp);
		Close();
		return false;
	}
	
	if(!Bind(iMultiPort,bRecv?NULL:sLocalIp))
	{
		LOGWARN("Error in CreateMultiCase(%s,%d,%s)",sMultiIp,iMultiPort,sLocalIp==NULL?"":sLocalIp);
		Close();
		return false;
	}
	
	struct ip_mreq mcast;
	mcast.imr_interface.s_addr = sLocalIp==NULL?INADDR_ANY:inet_addr(sLocalIp);
	mcast.imr_multiaddr.s_addr = inet_addr(sMultiIp);
	ret = setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
										(char*)&(mcast), sizeof(mcast));
	if (ret < 0)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("Error in CreateMultiCase(%s,%d,%s), err=%d",
			sMultiIp,iMultiPort,sLocalIp==NULL?"":sLocalIp,m_iLastError);
		Close();
		return false;
	}

	return true;
}

u_short SSocket::in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if( nleft == 1 ) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}

#include "SDateTime.h"
////////////////////////////////////////////////////////////////////////
// 描    述:  Ping指定的IP地址
// 作    者:  邵凯田
// 创建时间:  2010-3-11 13:31
// 参数说明:  
// 返 回 值:  0表示通，非0表示不通
//////////////////////////////////////////////////////////////////////////
int SSocket::Ping(char *sIp,int &ms,int &ttl)
{
	SSocket skt;
	if(!skt.Create(AF_INET, SOCK_RAW, IPPROTO_ICMP))
		return 1001;
	skt.SetTimeout(1000,1000);
	ECHOREQUEST echoReq;
	static int nId = 1;
	static int nSeq = 1;
	int nRet;

	// Fill in echo request
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ;
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= nId++;
	echoReq.icmpHdr.Seq			= nSeq++;

	// Fill in some data to send
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' '+nRet;

	// Save tick count when sent
	int soc1,usec1,soc2,usec2;
	SDateTime::getSystemTime(soc1,usec1);
	echoReq.dwTime				= usec1;

	// Put data in packet and compute checksum
	echoReq.icmpHdr.Checksum = in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));

	// Send the echo request  				
	struct    sockaddr_in saDest;
	saDest.sin_addr.s_addr = inet_addr(sIp);
	saDest.sin_family = AF_INET;
	saDest.sin_port = 0;
	nRet = skt.SendTo(saDest,&echoReq,sizeof(ECHOREQUEST));
	if (nRet != sizeof(ECHOREQUEST)) 
	{
		return 1002;
	}
	if(skt.CheckForRecv() < 1)
	{
		return 1003;
	}
	ECHOREPLY echoReply;
	sockaddr_in in;
	nRet = skt.RecvFrom(&echoReply,sizeof(ECHOREPLY),in);
	if(nRet <= 0)
	{
		return 1004;
	}
	SDateTime::getSystemTime(soc2,usec2);
	ttl = echoReply.ipHdr.TTL;
	ms = (soc2-soc1)*1000+(usec2-usec1)/1000;	
	return 0;
}


string SSocket::GetPeerName()									
{
	sockaddr_in host;
#ifdef WIN32
	int iRet=sizeof(host);
#else
	socklen_t iRet=sizeof(host);
#endif
	getpeername(m_hSocket,(sockaddr *)&host,&iRet);
	return inet_ntoa(host.sin_addr);
}

int SSocket::GetPeerPort()									
{
	sockaddr_in host;	
#ifdef WIN32
	int iRet=sizeof(host);
#else
	socklen_t iRet=sizeof(host);
#endif
	getpeername(m_hSocket,(sockaddr *)&host,&iRet);
	return host.sin_port;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取本端名称(IP)
// 作    者:  邵凯田
// 创建时间:  2014-01-21 11:26
// 参数说明:  void
// 返 回 值:  string
//////////////////////////////////////////////////////////////////////////
string SSocket::GetSockName()
{
	sockaddr_in host;
#ifdef WIN32
	int iRet=sizeof(host);
#else
	socklen_t iRet=sizeof(host);
#endif
	getsockname(m_hSocket,(sockaddr *)&host,&iRet);
	return inet_ntoa(host.sin_addr);
}


////////////////////////////////////////////////////////////////////////
// 描    述:  取本端端口
// 作    者:  邵凯田
// 创建时间:  2014-01-21 11:26
// 参数说明:  void
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int SSocket::GetSockPort()
{
	sockaddr_in host;	
#ifdef WIN32
	int iRet=sizeof(host);
#else
	socklen_t iRet=sizeof(host);
#endif
	getsockname(m_hSocket,(sockaddr *)&host,&iRet);
	return host.sin_port;
}

void SSocket::SetRecvBufSize(int iSize)
{
	m_iLastError = 0;
#ifdef WIN32
	int sizelen = 4;
#else
	socklen_t sizelen = 4;
#endif
	int ret;
	ret = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*)&iSize,sizelen);
	if (ret != 0)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("Error in SetRecvBufSize(%d), err=%d",iSize,m_iLastError);
	}
}

void SSocket::SetSendBufSize(int iSize)
{
	m_iLastError = 0;
#ifdef WIN32
	int sizelen = 4;
#else
	socklen_t sizelen = 4;
#endif
	int ret;
	ret = setsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&iSize,sizelen);
	if (ret != 0)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("Error in SetSendBufSize(%d), err=%d",iSize,m_iLastError);
	}
}

int  SSocket::GetRecvBufSize()
{
	m_iLastError = 0;
	int size=0;
#ifdef WIN32
	int sizelen = 4;
#else
	socklen_t sizelen = 4;
#endif	
	int ret;
	ret = getsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,(char*)&size,&sizelen);
	if (ret != 0)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("Error in SetSendBufSize(), err=%d",m_iLastError);
	}
	return size;
}

int  SSocket::GetSendBufSize()
{
	m_iLastError = 0;
	int size=0;
#ifdef WIN32
	int sizelen = 4;
#else
	socklen_t sizelen = 4;
#endif
	int ret;
	ret = getsockopt(m_hSocket,SOL_SOCKET,SO_SNDBUF,(char*)&size,&sizelen);
	if (ret != 0)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("Error in GetSendBufSize(), err=%d",m_iLastError);
	}
	return size;
}


/**
 * 连接服务端
 */
bool SSocket::Connect(const char * serverIp, int port)
{
	m_iLastError = 0;
	LOGBASEDEBUG("into SSocket's Connect(%s:%d),handle=%X",serverIp,port,m_hSocket);
	int ret;
	sockaddr_in dest_sin;
	memset(&dest_sin, 0 ,sizeof(dest_sin));
	dest_sin.sin_family 		= m_iAF;
	dest_sin.sin_addr.s_addr = inet_addr(serverIp);
	dest_sin.sin_port 			= htons(port); 
	ret = ::connect(m_hSocket, (sockaddr *)&dest_sin, sizeof(dest_sin));
	if( ret < 0)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's Connect(%s:%d), this=%p,err=%d,handle=%X",
			serverIp,port,this,m_iLastError,m_hSocket);
		Close();
		return false;
	}
	LOGBASEDEBUG("connect successful!"); 
	return true;
}
 
 /**
	* 绑定端口,供服务端使用
	*/
bool SSocket::Bind(int iPort , char * sLocalIp)
{
	LOGBASEDEBUG("into SSocket's Bind(%d),this=%p",iPort,this);
	int ret;
	sockaddr_in local_sin;
	m_iLastError = 0;
#ifndef WIN32
	//设置SOCKET为可重用端口,设为1时,TIME_WAIT状态不会影响下一次BIND操作
	int on=1;
	ret = ::setsockopt( m_hSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
#endif
	local_sin.sin_family = m_iAF;
	local_sin.sin_addr.s_addr = (sLocalIp==NULL || strlen(sLocalIp) == 0)?
																		INADDR_ANY:inet_addr(sLocalIp);
	local_sin.sin_port = htons(iPort);
	ret = ::bind(m_hSocket, (sockaddr *)&local_sin, sizeof(local_sin));
	if(ret == SOCKET_ERROR)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's Bind(%s,%d),errno=%d,this=%p",sLocalIp,iPort,m_iLastError,this);
		return false;
	}
	return true;
}
	
bool SSocket::Listen(int iBacklog)
{
	LOGBASEDEBUG("into SSocket's Listen(%d),this=%p",iBacklog,this);
	m_iLastError = 0;
	int ret;
	ret = ::listen(m_hSocket, iBacklog);
	if(ret == SOCKET_ERROR)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's Listen(),errno=%d,this=%p",m_iLastError,this);
		return false;
	}
	return true;
}
	
bool SSocket::Accept(SSocket &newSocket)
{
	LOGBASEDEBUG("into SSocket's Accept(),this=%p",this);
	m_iLastError = 0;
	sockaddr_in acc_sin;
	int acc_sin_len=sizeof(acc_sin);
#ifdef WIN32
	SOCKET sock = ::accept(m_hSocket, (sockaddr*)&acc_sin, (int *)&acc_sin_len);
#else
	SOCKET sock = ::accept(m_hSocket, (sockaddr*)&acc_sin, (socklen_t *)&acc_sin_len);
#endif
	if(sock == INVALID_SOCKET)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's Accept(),errno=%d,this=%p",m_iLastError,this);
		return false;
	}
	int bNodelay = 1;  
	int err = setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay));//不采用延时算法 

	newSocket.SetSocketHandle(sock);
	newSocket.m_iRemotePort = ntohs(acc_sin.sin_port);
	newSocket.m_sRemoteIp = inet_ntoa(acc_sin.sin_addr);
	newSocket.m_iLocalPort = GetPeerPort();
	newSocket.m_sLocalIp = GetPeerIp();
	return true;
}
	
 /**
	* 设置发送与接收的超时,0表示不限制超时，单位毫秒
	*/
bool SSocket::SetTimeout(int iRecvTimeout, int iSendTimeout)
{
	//LOGBASEDEBUG("into SetTimeOut(%d,%d),this=%p",iRecvTimeout,iSendTimeout,this);
	m_iRecvTimeout = iRecvTimeout*1000;
	m_iSendTimeout = iSendTimeout*1000;
	/*
	int ret=0;
	if(iRecvTimeout>0)
	{
	 	ret = ::setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO,(char*)&iRecvTimeout, sizeof(int));
	}
	if(ret == SOCKET_ERROR)
	{
		printf("error to set SO_RCVTIMEO=%d,errno=%d, socket=%u",ret, errno, m_hSocket);
		return false;
	}
	if(iSendTimeout>0)
	{
	 	ret = ::setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO,(char*)&iSendTimeout, sizeof(int));
	}
	if(ret == SOCKET_ERROR)
	{
		printf("error to set SO_SNDTIMEO=%d",ret);
		return false;
	}
	*/
	return true;
}

/**
 * 接收指定字节的数据
 * pBuffer		接收缓冲区
 * iReadLen	准备读取的字节数
 * flags			MSG_PEEK/MSG_OOB
 */ 
int SSocket::Receive(void *pBuffer, int iReadLen, int flags)
{
	LOGBASEDEBUG("into SSocket's Receive(len=%d),this=%p",iReadLen,this);
	int iReaded=0;
	int ret0,ret;
	m_iLastError = 0;
	while(iReaded < iReadLen)
	{
		ret0 = CheckForRecv();
		if(ret0 < 0)
		{
			return -1;
		}
		else if(ret0 < 1)
		{
			ret0 = CheckForRecv();
			if(ret0 < 0)
				return -2;
			else if(ret0 < 1)
				return iReaded;
		}
		ret = ::recv(m_hSocket, (char *)pBuffer+iReaded, iReadLen-iReaded,flags);
		
		if(ret < 0)
		{
			#ifdef WIN32
				m_iLastError = WSAGetLastError();
			#else
				m_iLastError = errno;
			#endif
		  LOGWARN("error in SSocket's Receive() =%d, errno=%d, this=%p",ret,m_iLastError,this);
			return -1;//接收失败
		}
		if(ret == 0)
		{
			if(iReaded > 0)
				return iReaded;
			if(ret0 == 1)
			{
				LOGWARN("select返回1但recv返回0，认为是等待关闭连接!将关闭连接!");
				Close();
				return -1;
			}
			#ifdef WIN32
				m_iLastError = WSAGetLastError();
			#else
				m_iLastError = errno;
			#endif
		  LOGWARN("timeout in SSocket's Receive() =%d, errno=%d, this=%p",ret,m_iLastError,this);
			return iReaded;
		}
		iReaded += ret;
		g_iRecvCount += ret;
//		#ifdef WIN32
//			Sleep(10);
//		#else
//			usleep(10*1000);
//		#endif
	}
	LOGBASEDEBUG("Recv return=%d, this=%p",iReaded,this);
	return iReaded;
}

int SSocket::ReceiveOnce(void *pBuffer, int iReadLen, int flags)
{
	LOGBASEDEBUG("into SSocket's Receive(len=%d),this=%p",iReadLen,this);
	int iReaded=0;
	int ret0,ret;
	
	ret0 = CheckForRecv();
	if(ret0 < 0)
		return -1;
	else if(ret0 < 1)
		return 0;
	ret = ::recv(m_hSocket, (char *)pBuffer+iReaded, iReadLen-iReaded,flags);
	if(ret < 0)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's Receive() =%d, errno=%d, this=%p",ret,m_iLastError,this);
		return -1;//接收失败
	}
	g_iRecvCount += ret;
	return ret;
}



int SSocket::ReceiveTimeOut(void *pBuffer, int iReadLen,int nTime, int flags)
{
	fd_set writefds;
	int r;
	FD_ZERO(&writefds);
	FD_SET(m_hSocket, &writefds);
	timeval t;
	t.tv_sec = nTime;
	t.tv_usec = 0;
	r =  select(m_hSocket+1, &writefds,NULL,  NULL, &t);
	if( r != 1)
	{
		return false;
	}
	return ::recv(m_hSocket, (char *)pBuffer, iReadLen,flags);
}

int SSocket::ReceiveNoTime(void *pBuffer, int iReadLen, int flags)
{
	return ::recv(m_hSocket, (char *)pBuffer, iReadLen,flags);
}

int SSocket::ReceiveNoTimeEx(void *pBuffer, int iReadLen, int flags)
{
	int nHaveRecv=0;
	int n;
	while(nHaveRecv<iReadLen)
	{
		n=::recv(m_hSocket, ((char *)pBuffer)+nHaveRecv, iReadLen-nHaveRecv,flags);
		if(n<1) return 0;
		nHaveRecv+=n;
	}
	return nHaveRecv;
}
/**
 * 发送指定的字节
 */

int SSocket::Send( void *pBuffer, int iLen, int flags)
{
	//LOGBASEDEBUG("into SSocket's Send(this=%p,len=%d,char=%s)",this,iLen,bufToHexString((BYTE *)pBuffer,iLen).c_str());
	int ret0,ret;
	int cnt = 0;
	int len;
	while(cnt < iLen)
	{
		ret0 = CheckForSend();
		if(ret0 < 0)
		{
			int err;
			#ifdef WIN32
				err = WSAGetLastError();
			#else
				err = errno;
			#endif
			LOGWARN("CheckForSend error=%d,errno=%d, this=%p",ret0,err,this);
			return -1;
		}
		//else if(ret0 < 1)
		//	return 0;
#if 1
		len = iLen-cnt;
		if(len > 1400)
			len = 1400;
		ret=::send(m_hSocket, (char *)pBuffer+cnt, len, flags);
#else
		ret=::send(m_hSocket, (char *)pBuffer+cnt, iLen-cnt, flags);
#endif
		if(ret == 0)
		{
			if(ret0 == 1 && iLen-cnt != 0)
			{
				LOGBASEDEBUG("select返回1但send返回0，认为是等待关闭连接!将关闭连接!");
				Close();
				return -1;
			}
		}
		else if(ret == SOCKET_ERROR)
		{
			int err;
			#ifdef WIN32
				err = WSAGetLastError();
			#else
				err = errno;
			#endif
			LOGWARN("error in SSocket's Send() =%d,errno=%d, this=%p",ret,err,this);
			return -1;
		}
		else
		{
			cnt += ret;
		}
	}
	g_iSendCount += cnt;
	LOGBASEDEBUG("Send return=%d,this=%p",cnt,this);
	return cnt;
}

int SSocket::SendOnce( void *pBuffer, int iLen, int flags)
{
	m_iLastError = 0;
	//LOGBASEDEBUG("into SSocket's Send(this=%p,len=%d,char=%s)",this,iLen,bufToHexString((BYTE *)pBuffer,iLen).c_str());
	int ret0,ret;
	ret0 = CheckForSend();
	if(ret0 < 0)
		return -1;
	//else if(ret0 < 1)
	//	return 0;
	ret=::send(m_hSocket, (char *)pBuffer, iLen, flags);
	if(ret == 0)
	{
		if(ret0 == 1 && iLen != 0)
		{
			LOGBASEDEBUG("select返回1但send返回0，认为是等待关闭连接!将关闭连接!");
			Close();
			return -1;
		}
	}
	else if(ret == SOCKET_ERROR)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's Send() =%d,errno=%d, this=%p",ret,m_iLastError,this);
		return -1;
	}
	g_iSendCount += ret;
	LOGBASEDEBUG("Send return=%d,this=%p",ret,this);
	return ret;
}

/**
 * UDP发送指定的字节
 */
int SSocket::SendTo(char *sIP, int iPort, void *pBuffer, int iLen, int flags)
{
	LOGBASEDEBUG("into SSocket's SendTo(this=%p, addr=%s, port=%d, len=%d)",this, sIP, iPort, iLen);
	m_iLastError = 0;
	sockaddr_in dest_sin;
	memset(&dest_sin, 0 ,sizeof(dest_sin));
	dest_sin.sin_family 	   = m_iAF;
	dest_sin.sin_addr.s_addr = inet_addr(sIP);
	dest_sin.sin_port 	   = htons(iPort); 
	int ret = CheckForSend();
	if(ret < 0)
		return -1;
	else if(ret < 1)
		return 0;
	
	ret = sendto(m_hSocket, (char *)pBuffer, iLen, flags, (sockaddr*)&dest_sin, sizeof(dest_sin));
	if(ret == SOCKET_ERROR)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's SendTo(%s:%d) =%d, this=%p,errno=%d",sIP,iPort,ret,this,m_iLastError);
	 	return -1;
	}
	g_iSendCount += ret;
	return ret;
}

int SSocket::SendTo(sockaddr_in &dest_sin, void *pBuffer, int iLen, int flags)
{
	LOGBASEDEBUG("into SSocket's SendTo with sockaddr_in,this=%p,iLen=%d",this,iLen);
	m_iLastError = 0;

	int ret = CheckForSend();
	if(ret < 0)
		return -1;
	else if(ret < 1)
		return 0;
	
	ret = sendto(m_hSocket, (char *)pBuffer, iLen, flags, (sockaddr*)&dest_sin, sizeof(dest_sin));
	if(ret == SOCKET_ERROR)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's SendTo() =%d, this=%p,errno=%d",ret,this,m_iLastError);
	 	return -1;
	}
	g_iSendCount += ret;
	return ret;
}

 
int SSocket::RecvFrom(void *pBuffer, int iLen, sockaddr_in &saUdpClient, int flags)
{
	m_iLastError = 0;
#ifdef WIN32
	int nSize = sizeof( sockaddr_in );
#else
	socklen_t nSize = sizeof( sockaddr_in );
#endif
	int ret = CheckForRecv();
	if(ret < 0)
		return -1;
	else if(ret < 1)
		return 0;
	ret = recvfrom(m_hSocket,(char*)pBuffer,iLen<MAX_UDPLENGTH?iLen:MAX_UDPLENGTH,
						flags,(sockaddr *) &saUdpClient,&nSize); 
	if(ret == SOCKET_ERROR) 
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		LOGWARN("error in SSocket's RecvFrom() =%d, this=%p,errno=%d",ret,this,m_iLastError);
	 	return -1;
	}
	g_iRecvCount += ret;
	return ret;
}

int SSocket::CheckForSend()
{
	m_iLastError = 0;
	fd_set writefds;
	int r;
	LOGBASEDEBUG("Into CheckForSend() this=%p",this); 
	FD_ZERO(&writefds);
	FD_SET(this->m_hSocket, &writefds);
	//
	// indefinite wait select    
	//
	timeval t;
	t.tv_sec = m_iSendTimeout/1000000;
	t.tv_usec = m_iSendTimeout%1000000;
	r =  select(this->m_hSocket+1, NULL, &writefds, NULL, &t);
	if( r != 1)
	{
		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		if(r == SOCKET_ERROR)
		{
			LOGWARN("send select:error=%d, this=%p",m_iLastError,this);
			Close();
		}
		else
		{
			LOGWARN("send select timeout, error=%d, this=%p",m_iLastError,this);
			Close();
		}
	}
	return r;
} 

int SSocket::CheckForRecv()
{
	m_iLastError = 0;
	fd_set readfds;
	int r;
	FD_ZERO(&readfds);
	FD_SET(m_hSocket, &readfds);
	LOGBASEDEBUG("Into CheckForRecv(),this=%p",this); 
	timeval t;
	t.tv_sec = m_iRecvTimeout/1000000;
	t.tv_usec = m_iRecvTimeout%1000000;
	r =  select(this->m_hSocket+1, &readfds, NULL, NULL, &t);
	if( r != 1)
	{
		/* CSC MCAST
		if (r == -1)
		{
			if (errno!=EINVAL && errno!=EAGAIN && errno!=EINTR) return -1;
		}
		else if (r != 0)
		{
			if (FD_ISSET(this->m_hSocket, &readfds))
				return 1;
		}*/

		#ifdef WIN32
			m_iLastError = WSAGetLastError();
		#else
			m_iLastError = errno;
		#endif
		if(r == SOCKET_ERROR)
		{
			LOGWARN("recv select:error=%d,this=%p",m_iLastError,this);
			Close();
		}
		else
		{
			LOGBASEDEBUG("recv select timeout, error=%d,this=%p",m_iLastError,this);
		}
	}
	return r;
} 

string SSocket::GetLocalIp(bool bWithEth/*=false*/)
{
	string str="";
#ifdef WIN32
	if(!g_bWinSockInit)
	{
		WSADATA WSAData;
		WSAStartup( MAKEWORD( 1, 1 ), &WSAData );
		g_bWinSockInit = true;
	}
 	char  szHostName[512];
	memset(szHostName,0,sizeof(szHostName));
 	gethostname(szHostName, sizeof(szHostName));
	hostent *hp = gethostbyname(szHostName);
	if(hp == 0)
		return str;
	for(int p=0;hp->h_addr_list[p];++p)   
	{
		str += inet_ntoa(*(struct   in_addr*)hp->h_addr_list[p]);
		if(bWithEth)
		{
			str += ":eth";
		}
		str += ",";
	}
#else
	int s;
	struct ifconf conf;
	struct ifreq *ifr;
	char buff[BUFSIZ];
	int num;
	int i;

	s = socket(PF_INET, SOCK_DGRAM, 0);
	conf.ifc_len = BUFSIZ;
	conf.ifc_buf = buff;

	ioctl(s, SIOCGIFCONF, &conf);
	num = conf.ifc_len / sizeof(struct ifreq);
	ifr = conf.ifc_req;

	for(i=0;i < num;i++)
	{
		struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
		ioctl(s, SIOCGIFFLAGS, ifr);
		if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
		{
			str += inet_ntoa(sin->sin_addr);
			if(bWithEth)
			{
				str += ":";
				str += ifr->ifr_name;
			}
			str += ",";
		}
		ifr++;
	}

#endif
	if(str.length()>0)
		str = str.substr(0,str.length()-1);
	return str;
}

