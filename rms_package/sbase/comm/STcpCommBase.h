/**
 *
 * 文 件 名 : TcpCommBase.h
 * 创建日期 : 2011-11-8 17:54
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通讯基础类
本通讯类必须使用又下通讯链路层封装
数据			长度(字节)		值		描述
-------------   ----------      ------  --------------------------------------------------------------------
启动字符		2					    启动字符可以由程序在实例时指定，又区分同一系统内的不同协议
头字符串长度	2				WORD	表示报文头字符串的大小M，每个报文都可以携带一个报文头字符串，其中可以放置一些参数性的数据，可以为空字符串
头字符串内容	M		
报文长度		2				WORD	不含启动字符和自身，即:(N)
报文类型		2						用于标识当前报文，报文长度由其决定
应用数据单元	N						由报文类型决定,可以为空

【注:所有字节序采用Little Endian(即小端字节序)。】

 * 修改记录 : 
 *            $Log: $
 *
 **/




#ifndef __SKT_TCPCOMM_BASE_H__
#define __SKT_TCPCOMM_BASE_H__

#include "../SSocket.h"
#include "../SApi.h"
#include "../SService.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuSTcpPackage
// 作    者:  邵凯田
// 创建时间:  2011-11-24 15:18
// 描    述:  TCP数据包结构
//////////////////////////////////////////////////////////////////////////
struct stuSTcpPackage
{
public:
	stuSTcpPackage()
	{
		m_pAsduBuffer = NULL;
		m_iAsduBufferLen = m_iAsduLen = 0;
		m_bFixBuffer = false;
		m_bOverflow = false;
		m_bAutoDelete = true;
	}
	~stuSTcpPackage()
	{
		free();
	}
	void free()
	{
		if(m_pAsduBuffer != NULL && (!m_bFixBuffer || m_bOverflow))
		{
			delete[] m_pAsduBuffer;
			m_pAsduBuffer = NULL;
		}
	}
	SString m_sHead;//报文头字符串
	WORD m_wFrameType;//报文类型
	BYTE *m_pAsduBuffer;//ASDU缓冲区
	int m_iAsduBufferLen;//ASDU缓冲区总长度
	int m_iAsduLen;//ASDU长度
	bool m_bFixBuffer;//是否是固定缓冲区，true为固定（不可释放，当空间不足时新建缓冲但不释放原缓冲,同时m_bOverflow置true），false为动态
	bool m_bOverflow;//固定缓冲区溢出
	bool m_bAutoDelete;//是否可以自动删除，如果是栈上的对象则不能删除
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  CTcpCommBase
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 描    述:  TCP通讯基类
//////////////////////////////////////////////////////////////////////////
class STcpCommBase
{
public:
	STcpCommBase();
	virtual ~STcpCommBase();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置协议的启动字
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 17:55
	// 参数说明:  @wStartWord表示启动字（两个字节）
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetStartWord(WORD wStartWord);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一帧报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 12:31
	// 参数说明:  @pS表示SOCKET实例
	//            @pPackage表示待发送的数据包
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(SSocket *pS,stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  创建一个新的报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 13:59
	// 参数说明:  @sHead表示报文头字符串
	//            @wFrameType表示报文类型
	//            @pAsduBuffer表示ASDU内容缓冲区
	//            @iAsduLen表示ASDU数据长度
	//            @bCloneBuffer表示是否克隆
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* NewPackage(SString &sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0,bool bCloneBuffer=true);
	stuSTcpPackage* NewPackage(char* sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0,bool bCloneBuffer=true);
	stuSTcpPackage* NewPackage(WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0,bool bCloneBuffer=true);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  从原报文复制一个新报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-11 19:50
	// 参数说明:  @pPackage原报文
	// 返 回 值:  stuSTcpPackage*
	//////////////////////////////////////////////////////////////////////////
	stuSTcpPackage* NewPackage(stuSTcpPackage *pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一帧报文
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:17
	// 参数说明:  @pS表示SOCKET实例
	//            @sHead表示报文头字符串
	//            @wFrameType表示报文类型
	//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
	//            @iAsduLen表示ASDU数据长度
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(SSocket *pS,SString &sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送一帧报文
	// 作    者:  邵凯田
	// 创建时间:  2012-01-17 14:01
	// 参数说明:  @pS表示SOCKET实例
	//            @wFrameType表示报文类型
	//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
	//            @iAsduLen表示ASDU数据长度
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendFrame(SSocket *pS,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收一个数据包到stuSTcpPackage指针中并引用返回新实例
	// 作    者:  邵凯田
	// 创建时间:  2011-11-10 12:33
	// 参数说明:  @pS表示SOCKET实例
	//            @pPackage表示数据包引用指针，内部申请stuSTcpPackage对象，外部负责释放
	// 返 回 值:  <0表示接收失败，=0表示接收超时，>0表示接收成功且等于报文总长度
	//////////////////////////////////////////////////////////////////////////
	int RecvFrame(SSocket *pS,stuSTcpPackage* &pPackage);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收一个数据包到新缓冲区
	// 作    者:  邵凯田
	// 创建时间:  2011-11-8 19:18
	// 参数说明:  @pS表示SOCKET实例
	//            @sHead表示报文头字符串
	//            @wFrameType表示报文类型，引用返回
	//            @pAsduBuffer表示ASDU内容缓冲区，内部申请内容并引用返回
	//            @iAsduLen表示ASDU数据长度，引用返回
	//            @pFixRef为固定缓冲区信息，NULL表示非固定缓冲
	// 返 回 值:  <0表示接收失败，=0表示接收超时，>0表示接收成功且等于报文总长度
	//////////////////////////////////////////////////////////////////////////
	//int RecvFrame(SSocket *pS,SString &sHead,WORD &wFrameType,BYTE *&pAsduBuffer,int &iAsduLen,stuYsTcpPackageFixRef *pFixRef=NULL);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  
	// 作    者:  邵凯田
	// 创建时间:  2011-11-17 13:37
	// 参数说明:  @pS表示SOCKET实例
	//            @pPackage表示待发送的数据包
	//            @pRecvPackage表示接收到的新包
	//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
	// 返 回 值:  true表示发送成功,false表示发送失败
	//////////////////////////////////////////////////////////////////////////
	bool SendAndRecv(SSocket *pS,stuSTcpPackage *pPackage,stuSTcpPackage* &pRecvPackage,int iTimeout_s=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  
	// 作    者:  邵凯田
	// 创建时间:  2011-11-17 14:04
	// 参数说明:  @pS表示SOCKET实例
	//            @pRecvPackage表示接收到的新包
	//            @sHead表示报文头字符串
	//            @wFrameType表示报文类型，引用返回
	//            @pAsduBuffer表示ASDU内容缓冲区，内部申请内容并引用返回
	//            @iAsduLen表示ASDU数据长度，引用返回
	//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
	// 返 回 值:  true表示发收成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool SendAndRecv(SSocket *pS,stuSTcpPackage* &pRecvPackage,
					 SString &sHead,WORD wFrameType=0,BYTE *pAsduBuffer=NULL,int iAsduLen=0,int iTimeout_s=0);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前通讯报文的启动字
	// 作    者:  邵凯田
	// 创建时间:  2012-4-20 8:56
	// 参数说明:  void
	// 返 回 值:  WORD
	//////////////////////////////////////////////////////////////////////////
	inline WORD GetStartWord(){return m_wStartWord;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取完整帧减去ASDU的长度，即报文帧头长度，没有字符串时为10
	// 作    者:  邵凯田
	// 创建时间:  2012-4-20 9:02
	// 参数说明:  void
	// 返 回 值:  int(固定为10)
	//////////////////////////////////////////////////////////////////////////
	inline int GetFramePrefixLen(){return 10;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  填充完整帧的帧头部分内容，完整帧为启动字开始的全部TCP以上的用户层数据
	//            相同类型报文第一次发送时调用此函数填充缓冲报文头，后续报文只有当长度改变时
	//            修改本函数返回的int*内容即可
	// 作    者:  邵凯田
	// 创建时间:  2012-4-20 9:00
	// 参数说明:  @pBuffer为完整帧的缓冲区指针
	//            @wFrameType为报文类型
	//            @iAsduLen为ASDU的长度
	// 返 回 值:  int*表示当前缓冲区中存放ASDU长度的指针，修改后需要调用SKT_SWAP_DWORD处理字节序
	//////////////////////////////////////////////////////////////////////////
	int* FillFullFrameHead(BYTE *pBuffer,WORD wFrameType,int iAsduLen);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置发送长度限制
	// 作    者:  邵凯田
	// 创建时间:  2012-11-17 12:18
	// 参数说明:  @len限制长度，0表示不限制
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetSendLenLimit(int len){m_iSendLenLimit = len;};

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  设置发送休眠时间
	// 作    者:  邵凯田
	// 创建时间:  2012-11-17 12:18
	// 参数说明:  @us微秒，0表示不休眠
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void SetSendSleep(int us){m_iSendSleep = us;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  预设缓冲区大小，当当前发送缓冲区大小大于给定大小时忽略
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 10:51
	// 参数说明:  @iStrSize为头字符串长度
	//         :  @iAsduSize为ASDU长度
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void SetSendBufferSize(int iStrSize,int iAsduSize);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取ASDU在整体发送缓冲区中的偏移位置
	// 作    者:  邵凯田
	// 创建时间:  2014:4:26 10:53
	// 参数说明:  @iStrSize为关字符串长度
	// 返 回 值:  BYTE*
	//////////////////////////////////////////////////////////////////////////
	BYTE* GetAsduOffsetInBuffer(int iStrSize);
private:

	WORD m_wStartWord;//两个字节的启动字符
	BYTE *m_pSendBuffer;//发送缓冲区，用于发送报文，不释放，保留最大的缓冲区
	int m_iSendBufferSize;//发送缓冲区大小
	int m_iSendLenLimit;//一次发送的限制大小，超过则分次发，0表示不限制
	int m_iSendSleep;//一次发送后的休眠时间，单位微秒，0表示不休眠
};


#endif//__SKT_TCPCOMM_BASE_H__
