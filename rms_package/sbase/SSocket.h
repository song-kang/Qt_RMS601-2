/**
 *
 * 文 件 名	: SSocket.h
 * 创建日期	: 2006-09-10
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: 2011/08/31 08:43:35 $
 * 当前版本	: $Revision: 1.1 $
 * 功能描述 : SOCKET封装类
 * 修改记录 : 
 *            $Log: SSocket.h,v $
 *            Revision 1.1  2011/08/31 08:43:35  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/01 11:07:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/01/17 01:24:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:44  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.7  2009/01/31 06:37:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2008/10/16 09:16:50  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/10/16 03:14:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/09/26 05:06:46  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/09/26 03:16:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/09/26 02:16:21  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:03  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2008/04/03 02:44:34  shaokt
 *            LINUX下将ysbase分离为.so，单独编译，设备程序在编译时连接该so，不将目标码生成到执行文件中；WIN32下为调试方便还直接编译基类源码
 *
 *            Revision 1.6  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.4  2007/10/17 10:02:54  shaokt
 *            增加取本地所有IP的函数
 *
 *            Revision 1.3  2007/09/11 08:17:53  shaokt
 *            基类更新
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
 
#ifndef SKTSOCKET_H_
#define SKTSOCKET_H_

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 
	#include <stdlib.h> 
	#include <winsock2.h> 
	#include <WS2TCPIP.H>
//#ifndef socklen_t
//	#define socklen_t int
//#endif
	#pragma comment(lib,"ws2_32.lib")
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <sys/select.h>
	#include <unistd.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <net/if.h>
	#include <netinet/in.h>
	#include <linux/sockios.h>
	#include <sys/ioctl.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
#endif
#include <iostream>
#include <stdio.h>

#include "SObject.h"

#ifndef NULL
	#define NULL 0
#endif

#ifndef SOCKET 
#define SOCKET int
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef TCP_NODELAY
#define TCP_NODELAY 0x0001
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif
//typedef unsigned char BYTE;
#ifndef MAX_UDPLENGTH
#define MAX_UDPLENGTH 2048
#endif


//#ifdef WIN32
//struct ip_mreq {
//        struct in_addr  imr_multiaddr;  /* IP multicast address of group */
//        struct in_addr  imr_interface;  /* local IP address of interface */
//};
//#endif
//
//#ifndef IP_MULTICAST_IF
//#define IP_MULTICAST_IF     2           /* set/get IP multicast interface   */
//#define IP_MULTICAST_TTL    3           /* set/get IP multicast timetolive  */
//#define IP_MULTICAST_LOOP   4           /* set/get IP multicast loopback    */
//#define IP_ADD_MEMBERSHIP   5           /* add  an IP group membership      */
//#define IP_DROP_MEMBERSHIP  6           /* drop an IP group membership      */
//#endif

#include <string>

using namespace std;


class SSocket
{
public:
	/*
	============================================================================
		PING功能实现
	============================================================================
	*/
	#pragma pack(push)  
	#pragma pack(1)

	#define ICMP_ECHOREPLY	0
	#define ICMP_ECHOREQ	8

	// IP Header -- RFC 791
	typedef struct tagIPHDR
	{
		u_char  VIHL;			// Version and IHL
		u_char	TOS;			// Type Of Service
		short	TotLen;			// Total Length
		short	ID;				// Identification
		short	FlagOff;		// Flags and Fragment Offset
		u_char	TTL;			// Time To Live
		u_char	Protocol;		// Protocol
		u_short	Checksum;		// Checksum
		struct	in_addr iaSrc;	// Internet Address - Source
		struct	in_addr iaDst;	// Internet Address - Destination
	}IPHDR, *PIPHDR;


	// ICMP Header - RFC 792
	typedef struct tagICMPHDR
	{
		u_char	Type;			// Type
		u_char	Code;			// Code
		u_short	Checksum;		// Checksum
		u_short	ID;				// Identification
		u_short	Seq;			// Sequence
		char	Data;			// Data
	}ICMPHDR, *PICMPHDR;


	#define REQ_DATASIZE 32		// Echo Request Data size

	// ICMP Echo Request
	typedef struct tagECHOREQUEST
	{
		ICMPHDR icmpHdr;
		unsigned int dwTime;
		char	cData[REQ_DATASIZE];
	}ECHOREQUEST, *PECHOREQUEST;


	// ICMP Echo Reply
	typedef struct tagECHOREPLY
	{
		IPHDR	ipHdr;
		ECHOREQUEST	echoRequest;
		char    cFiller[256];
	}ECHOREPLY, *PECHOREPLY;


	#pragma pack()
	#pragma pack(pop)  

	SSocket()
	{
		SKTOBJECT("SSocket");
		m_hSocket=0;
		m_bTcp = true;
		m_iRecvTimeout = 5000000;
		m_iSendTimeout = 5000000;
	}
	virtual ~SSocket()
	{
		Close();
	}
	
private:
	SOCKET m_hSocket;
	int m_iAF;
	bool m_bTcp;
	int m_iSocketType;
	int m_iRecvTimeout,m_iSendTimeout;//接收、发送超时单位微秒
	unsigned int m_data;
	int m_iLocalPort,m_iRemotePort,m_iLastError;
	string m_sLocalIp,m_sRemoteIp;
	
public:
	

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建SOCKET实例
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:22
	// 参数说明:  @af为AF_INET
	//            @type为SOCK_STREAM/SOCK_DGRAM
	//            @protocol为IPPROTO_TCP/IPPROTO_UDP
	// 返 回 值:  true表示创建成功，false表示创建失败
	//////////////////////////////////////////////////////////////////////////
	bool Create(int af=AF_INET, int type=SOCK_STREAM, int protocol=IPPROTO_TCP);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建TCP SOCKET
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:23
	// 参数说明:  void
	// 返 回 值:  true表示创建成功，false表示创建失败
	//////////////////////////////////////////////////////////////////////////
	bool CreateTcp();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建UDP SOCKET
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:24
	// 参数说明:  void
	// 返 回 值:  true表示创建成功，false表示创建失败
	//////////////////////////////////////////////////////////////////////////
	bool CreateUdp();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建一个广播发送SOCKET
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:24
	// 参数说明:  void
	// 返 回 值:  true表示创建成功，false表示创建失败
	//////////////////////////////////////////////////////////////////////////
	bool CreateBroadCast();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建多播SOCKET
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:24
	// 参数说明:  @sMultiIp为多播IP地址
	//            @iMultiPort为多播端口
	//            @sLocalIp为绑定的本机IP（用于选择发送网卡）
	//            @bRecv是否同时打开接收模式
	// 返 回 值:  true表示创建成功，false表示创建失败
	//////////////////////////////////////////////////////////////////////////
	bool CreateMultiCast(char* sMultiIp, int iMultiPort, char* sLocalIp=NULL,bool bRecv=true);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  连接TCP服务端
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:30
	// 参数说明:  @serverIp为目标TCP服务端的IP地址
	//            @port为TCP端口号
	// 返 回 值:  true表示连接成功，false表示连接失败
	//////////////////////////////////////////////////////////////////////////
	bool Connect(const char * serverIp, int port);
	 
	////////////////////////////////////////////////////////////////////////
	// 描    述:  绑定端口,供服务端使用
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:32
	// 参数说明:  @iPort为待绑定的本机端口号
	//            @sLocalIp为待 绑定的本机IP，可选
	// 返 回 值:  true表示绑定成功，false表示绑定失败
	//////////////////////////////////////////////////////////////////////////
	bool Bind(int iPort , char * sLocalIp = NULL);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  帧听TCP连接
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:33
	// 参数说明:  @iBacklog为同时等待连接队列的数量，<=5，缺省为5
	// 返 回 值:  true表示帧听成功，false表示帧听失败
	//////////////////////////////////////////////////////////////////////////
	bool Listen(int iBacklog=5);
	  
	////////////////////////////////////////////////////////////////////////
	// 描    述:  接受一个新的TCP连接请求，阻塞函数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:35
	// 参数说明:  @newSocket为新SOCKET实例
	// 返 回 值:  true表示接受到一个新的连接，false表示没有新连接或失败
	//////////////////////////////////////////////////////////////////////////
	bool Accept(SSocket &newSocket);
	  
	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置发送与接收的超时,0表示不限制超时，单位毫秒
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:36
	// 参数说明:  @iRecvTimeout表示接收超时单位，单位毫秒
	//            @iSendTimeout表示发送超时单位，单位毫秒
	// 返 回 值:  true表示设置成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool SetTimeout(int iRecvTimeout=0, int iSendTimeout=0);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收指定字节的数据
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:38
	// 参数说明:  @pBuffer为接收缓冲区
	//            @iReadLen为准备读取的字节数
	//            @flags,0/MSG_PEEK/MSG_OOB
	// 返 回 值:  返回接收的长度，=0为超时，<0为失败
	//////////////////////////////////////////////////////////////////////////
	int Receive(void *pBuffer, int iReadLen, int flags=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收一次，收到多少就多少，不循环接收
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:41
	// 参数说明:  @pBuffer为接收缓冲区
	//            @iReadLen为准备读取的字节数
	//            @flags,0/MSG_PEEK/MSG_OOB
	// 返 回 值:  返回接收的长度，=0为超时，<0为失败
	//////////////////////////////////////////////////////////////////////////
	int ReceiveOnce(void *pBuffer, int iReadLen, int flags=0);

	//以下三个函数不推荐使用
	int ReceiveTimeOut(void *pBuffer, int iReadLen, int nTime,int flags=0);
	int ReceiveNoTime(void *pBuffer, int iReadLen, int flags=0);
	int ReceiveNoTimeEx(void *pBuffer, int iReadLen, int flags=0);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送指定的字节
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:42
	// 参数说明:  @pBuffer为发送缓冲区
	//            @iReadLen为准备发送的字节数
	//            @flags,0/MSG_PEEK/MSG_OOB
	// 返 回 值:  返回发送字节数
	//////////////////////////////////////////////////////////////////////////
	int Send( void *pBuffer, int iLen, int flags=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一次，可能不能全部发送完成
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:42
	// 参数说明:  
	// 参数说明:  @pBuffer为发送缓冲区
	//            @iReadLen为准备发送的字节数
	//            @flags,0/MSG_PEEK/MSG_OOB
	// 返 回 值:  返回发送字节数
	//////////////////////////////////////////////////////////////////////////
	int SendOnce( void *pBuffer, int iLen, int flags=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  UDP发送
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:43
	// 参数说明:  @sIP为目标IP
	//            @iPort为目标端口
	//            @pBuffer为发送缓冲区
	//            @iLen为发送字节数
	// 返 回 值:  返回发送字节数，=0为超时，<0为失败
	//////////////////////////////////////////////////////////////////////////
	int SendTo(char *sIP, int iPort, void *pBuffer, int iLen, int flags=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  UDP发送
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:43
	// 参数说明:  @dest_sin为发送地址
	//            @pBuffer为发送缓冲区
	//            @iLen为发送字节数
	// 返 回 值:  返回发送字节数，=0为超时，<0为失败
	//////////////////////////////////////////////////////////////////////////
	int SendTo(sockaddr_in &dest_sin, void *pBuffer, int iLen, int flags=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收UDP报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:44
	// 参数说明:  @pBuffer为接收缓冲区
	//            @iLen为准备读取的字节最大长度，此长度必须给足，如果不足将返回失败
	//            @saUdpClient为接收到报文的对应地址
	//            @flag,0/MSG_PEEK/MSG_OOB
	// 返 回 值:  返回接收实际长度，=0为超时，<0为失败
	//////////////////////////////////////////////////////////////////////////
	int RecvFrom(void *pBuffer, int iLen, sockaddr_in &saUdpClient, int flags=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  检查是否可以发送数据
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:47
	// 参数说明:  void
	// 返 回 值:  1表示可以发送，0表示超时，<0表示异常
	//////////////////////////////////////////////////////////////////////////
	int CheckForSend();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  检查是否有需要接收的数据
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:47
	// 参数说明:  void
	// 返 回 值:  1表示有新数量，0表示超时，<0表示异常
	//////////////////////////////////////////////////////////////////////////
	int CheckForRecv();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  等同于Receive
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:50
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline int read(BYTE* dest,size_t maxLen)
	{
#ifdef SSOCKET_RECV_BUFFER
		if(m_pBuffered == NULL || m_pBufferedReadPos >= m_pBuffered  + m_iBufferLen)
			return NULL;
		int len = maxLen;
		if(m_pBufferedReadPos + len  > m_pBuffered  + m_iBufferLen )
		{
			len = (m_pBuffered  + m_iBufferLen) - m_pBufferedReadPos;
		}
		memcpy(dest,m_pBufferedReadPos,len);
		m_pBufferedReadPos += len;
		if(m_pBufferedReadPos >= m_pBuffered  + m_iBufferLen)
		{
			m_pBuffered = m_pBufferedReadPos = NULL;
			m_iBufferLen = 0;
		}
		return len;
#else
		return Receive(dest,maxLen);
#endif
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  等同于Send
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:50
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline int write(BYTE* dest,size_t maxLen)
	{
		return Send((void*)dest,maxLen);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  等同于Receive
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:50
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline int read(char* dest,size_t maxLen)
	{
		return read((BYTE*) dest,maxLen);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  等同于Send
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:50
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline int write(char* dest,size_t maxLen)
	{
		return write((BYTE*)dest,maxLen);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭套接字
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:18
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int Close();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前连接本地端口
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 8:56
	// 参数说明:  void
	// 返 回 值:  本地端口
	//////////////////////////////////////////////////////////////////////////
	inline int GetLocalPort(){return m_iLocalPort;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前连接对端端口
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 8:57
	// 参数说明:  void
	// 返 回 值:  对端端口
	//////////////////////////////////////////////////////////////////////////
	inline int GetRemotePort(){return m_iRemotePort;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前连接本地IP地址
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 9:01
	// 参数说明:  void
	// 返 回 值:  ip
	//////////////////////////////////////////////////////////////////////////
	inline string GetLocalIpAddr(){return m_sLocalIp;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前连接对端IP地址
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 9:02
	// 参数说明:  void
	// 返 回 值:  ip
	//////////////////////////////////////////////////////////////////////////
	inline string GetRemoteIpAddr(){return m_sRemoteIp;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取附加的数据
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 9:02
	// 参数说明:  void
	// 返 回 值:  uint
	//////////////////////////////////////////////////////////////////////////
	inline unsigned int GetData(){return m_data;}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置附加的数据
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 9:03
	// 参数说明:  @d新的附加数据内容
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetData(unsigned int d){m_data=d;}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置SOCKET句柄
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 9:03
	// 参数说明:  @sock为新的句柄
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetSocketHandle(SOCKET sock)
	{
		m_hSocket = sock;
	}
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前SOCKET句柄
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 9:05
	// 参数说明:  void
	// 返 回 值:  句柄
	//////////////////////////////////////////////////////////////////////////
	inline int GetSocketHandle()
	{
		return (int)m_hSocket;
	}
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否通讯实例已经建立
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 9:05
	// 参数说明:  void
	// 返 回 值:  true表示已经建立，false表示未建立
	//////////////////////////////////////////////////////////////////////////
	inline bool IsConnected()
	{
		if(m_hSocket != 0)
			return true;
		else
			return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取最后的错误号
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:15
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	inline int GetLastError()
	{
		return m_iLastError;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取对端名称(IP)
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:26
	// 参数说明:  void
	// 返 回 值:  string
	//////////////////////////////////////////////////////////////////////////
	string GetPeerName();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取对端名称(IP)
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:26
	// 参数说明:  void
	// 返 回 值:  string
	//////////////////////////////////////////////////////////////////////////
	inline string GetPeerIp(){return GetPeerName();};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取对端端口
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:26
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int GetPeerPort();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取本端名称(IP)
	// 作    者:  邵凯田
	// 创建时间:  2014-01-21 11:26
	// 参数说明:  void
	// 返 回 值:  string
	//////////////////////////////////////////////////////////////////////////
	string GetSockName();
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  取本端名称(IP)
	// 作    者:  邵凯田
	// 创建时间:  2014-01-21 11:26
	// 参数说明:  void
	// 返 回 值:  string
	//////////////////////////////////////////////////////////////////////////
	inline string GetSockIp(){return GetSockName();};
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  取本端端口
	// 作    者:  邵凯田
	// 创建时间:  2014-01-21 11:26
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int GetSockPort();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置SOCKET的接收缓冲区大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:27
	// 参数说明:  @iSize新大小，单位字节
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetRecvBufSize(int iSize);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置SOCKET的发送缓冲区的大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:27
	// 参数说明:  @iSize新大小，单位字节
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetSendBufSize(int iSize);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取接收缓冲区大小的大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:28
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int  GetRecvBufSize();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取发送缓冲区大小的大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:29
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	int  GetSendBufSize();


	////////////////////////////////////////////////////////////////////////
	// 描    述:  取本地IP，多IP用','分隔
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:48
	// 参数说明:  @bWithEth表示是否在IP后加上网卡名，+":eth?";
	// 返 回 值:  string
	//////////////////////////////////////////////////////////////////////////
	static string GetLocalIp(bool bWithEth=false);


	////////////////////////////////////////////////////////////////////////
	// 描    述:  Ping指定的IP地址
	// 作    者:  邵凯田
	// 创建时间:  2010-3-11 13:31
	// 参数说明:  @sIp表示指定的IP地址
	//            @ms为引用返回的时间，单位毫秒
	//            @ttl为引用返回的TTL数
	// 返 回 值:  0表示通，非0表示不通
	//////////////////////////////////////////////////////////////////////////
	static int Ping(char *sIp,int &ms,int &ttl);

	//以下用于统计本进程的SOCKET通迅流量

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取接收到数据的总数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:19
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	static int GetRecvCount();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取发送数据的总数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:19
	// 参数说明:  void
	// 返 回 值:  int
	//////////////////////////////////////////////////////////////////////////
	static int GetSendCount();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清零接收总数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:20
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void ResetRecvCount();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清零发送总数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:20
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void ResetSendCount();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  外部添加接收数，在未使用SSocket接收时使用
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:20
	// 参数说明:  @a为接收字节数
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void AddRecvCount(int a);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  外部添加发送数，在未使用SSocket接收时使用
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:21
	// 参数说明:  @a为发送字节数
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void AddSendCount(int a);

private:
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  PING命令使用
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 11:16
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static u_short in_cksum(u_short *addr, int len);

public:

#ifdef SSOCKET_RECV_BUFFER
	void SetReadBuffer(BYTE *pBuf,int iLen)
	{
		m_pBuffered = m_pBufferedReadPos = pBuf;
		m_iBufferLen = iLen;
	}
	//为南自程序使用
	BYTE *m_pBuffered;//已经缓存的报文总数
	BYTE *m_pBufferedReadPos;//当前将要读取的位置
	int m_iBufferLen;//缓存总长度
#endif
};


#endif /*SKTSOCKET_H_*/
