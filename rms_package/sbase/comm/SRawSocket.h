/**
 *
 * 文 件 名 : SRawSocketRecv.h
 * 创建日期 : 2011-12-1 10:08
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 原始套接字基础类
 * 修改记录 : 
 *            $Log: $
 *
 **/

#ifndef __SKT_RAW_SOCKET_H__
#define __SKT_RAW_SOCKET_H__

#ifdef WIN32
	#include <pcap.h>
	#pragma comment(lib,"wpcap.lib")
#else
	#include <stdio.h> 
	#include <stdlib.h> 
	#include <sys/socket.h> 
	#include <netinet/in.h> 
	#include <arpa/inet.h> 
	#include <netinet/ip.h> 
	#include <string.h> 
	#include <netdb.h> 
	#include <netinet/tcp.h> 
	#include <netinet/udp.h> 
	#include <stdlib.h> 
	#include <unistd.h> 
	#include <signal.h> 
	#include <net/if.h> 
	#include <sys/ioctl.h> 
	#include <sys/stat.h> 
	#include <fcntl.h> 
	#include <linux/if_ether.h> 
	#include <net/ethernet.h> 

	/* ssl_ifindex表示网卡编号*/
	struct sockaddr_ll 
	{ 
		u_short sll_family; 
		u_short sll_protocol; 
		int sll_ifindex; //网络字节序，（3为eth1,5为eth3）
		u_short sll_hatype; 
		u_char sll_pkttype; 
		u_char sll_halen; 
		u_char sll_addr[8]; 
	};

#endif
#ifndef SOCKET
#define SOCKET int
#endif

#include "../SApi.h"
#include "../SService.h"

//接收到原始包的回调函数定义
typedef void (*OnReceivePcapData)(void* cbparam,int soc,int usec,BYTE *pBuffer,int iLength);
typedef BYTE* (*OnGetWriteBuffer)(void* cbparam);

//////////////////////////////////////////////////////////////////////////
// 名    称:  SRawSocketRecv
// 作    者:  邵凯田
// 创建时间:  2011-12-1 10:09
// 描    述:  原始套接字接收类
//////////////////////////////////////////////////////////////////////////
class SRawSocketRecv : public SService
{
public:
	SRawSocketRecv();
	~SRawSocketRecv();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动服务
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 10:37
	// 参数说明:  void
	// 返 回 值:  true表示服务启动成功,false表示服务启动失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止服务
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 10:37
	// 参数说明:  void
	// 返 回 值:  true表示服务停止成功,false表示服务停止失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置待侦听的网络设备的名称
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 10:37
	// 参数说明:  @sDevName在WINDOWS平台下的WINPCAP的设备名，LINUX下为网卡名
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetDevName(SString sDevName){m_sDevName = sDevName;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  接收到原始报文的处理虚函数
	// 作    者:  邵凯田
	// 创建时间:  2011-12-24 20:53
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	virtual void OnReceive(int soc,int usec,BYTE *pBuffer,int iLength){};


	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置回调函数及用户指针
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 10:47
	// 参数说明:  @p为新指针
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetCallback(OnReceivePcapData pFun,void *p)
	{
		m_pCallbackOnReceive = pFun;
		m_pCallbackUserPtr = p;
	};

	inline void SetCallbackGetWriteBuffer(OnGetWriteBuffer pFun,int iMaxLen)
	{
		m_pGetWriteBuffer = pFun;
		m_iMaxWriteBufferLen = iMaxLen;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置接收缓冲区的大小
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 10:57
	// 参数说明:  @bytes新大小，单位字节
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetRecvBufferSize(int bytes){m_iRecvBufferSize = bytes;};

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
	void SetFilterBytes(BYTE *pFilterBytes,int len,int iFilterOffsetPos,bool bFilterExcept);

private:
	////////////////////////////////////////////////////////////////////////
	// 描    述:  PCAP抓包线程
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 9:35
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadPcap(void *lp);

	SString m_sDevName;//PCAP的网络接口名
	OnReceivePcapData m_pCallbackOnReceive;//接收回调函数的地址
	OnGetWriteBuffer m_pGetWriteBuffer;//取接收缓冲区地址的回调函数
	int m_iMaxWriteBufferLen;//单帧报文的最大长度
	void *m_pCallbackUserPtr;//回调时的用户指针
	int m_iRecvBufferSize;//接收缓冲区的大小，单位字节，缺省20M

	BYTE *m_pFilterBytes;//过滤字节串,NULL表示不过滤
	int m_iFilterOffsetPos;//过滤字节串在原始报文中的偏移位置
	int m_iFilterByteLen;//过滤字节串长度
	bool m_bFilterExcept;//true表示例外过滤，指过滤掉含有指定内容的报文，false表示过滤掉不含指定内容的报文

#ifdef WIN32
	pcap_t *m_pCap;
	char m_ErrBuf[PCAP_ERRBUF_SIZE];
	static void ys_packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

#else
	int *m_pRawsock;//原始接收SOCKET
	static void raw_die(char *why, int n);
	static bool raw_do_promisc(char *nif, int sock );
	static void raw_set_recvbuffer(int sock,int iBufferSize);
#endif
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SRawSocketSend
// 作    者:  邵凯田
// 创建时间:  2011-12-1 14:15
// 描    述:  原始套接字的发送类
//////////////////////////////////////////////////////////////////////////
class SRawSocketSend
{
public:
	SRawSocketSend();
	~SRawSocketSend();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建一个用于发送的原始套接字
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 14:16
	// 参数说明:  @sDevName为选择用于发送的目标网卡名
	// 返 回 值:  true表示创建成功，false表示失败 
	//////////////////////////////////////////////////////////////////////////
	bool Create(SString sDevName="eth0");

	////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭原始套接字
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 15:37
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void Close();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送指定大小的原始报文
	// 作    者:  邵凯田
	// 创建时间:  2011-12-1 14:18
	// 参数说明:  @pBuffer为原始报文全部内容，含目标、源MAC地址
	//            @iLen为原始报文总长度，单位字节
	// 返 回 值:  >0表示发送成功的字节数，<0表示失败，=0表示发送超时
	//////////////////////////////////////////////////////////////////////////
	int SendRaw(BYTE *pBuffer,int iLen);

private:

#ifdef WIN32
	pcap_t *m_pCap;
	char m_ErrBuf[PCAP_ERRBUF_SIZE];
#else
	int m_sock;//Raw socket HANDLE
    struct sockaddr_ll m_sll; 
	
#endif
};

#endif//__SKT_RAW_SOCKET_H__
