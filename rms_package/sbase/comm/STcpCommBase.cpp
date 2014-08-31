/**
 *
 * 文 件 名 : STcpCommBase.cpp
 * 创建日期 : 2011-11-8 17:54
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/


#include "STcpCommBase.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  STcpCommBase
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 描    述:  TCP通讯基类
//////////////////////////////////////////////////////////////////////////

STcpCommBase::STcpCommBase()
{
	m_pSendBuffer = NULL;//发送缓冲区，用于发送报文，不释放，保留最大的缓冲区
	m_iSendBufferSize = 0;//发送缓冲区大小
	m_wStartWord = 0x51EB;
	SKT_SWAP_WORD(m_wStartWord);
	m_iSendLenLimit = 0;//一次发送的限制大小，超过则分次发，0表示不限制
	m_iSendSleep = 0;//一次发送后的休眠时间，单位微秒，0表示不休眠

}

STcpCommBase::~STcpCommBase()
{
	if(m_pSendBuffer != NULL)
		delete[] m_pSendBuffer;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置协议的启动字
// 作    者:  邵凯田
// 创建时间:  2011-11-8 17:55
// 参数说明:  @wStartWord表示启动字（两个字节）
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void STcpCommBase::SetStartWord(WORD wStartWord)
{
	m_wStartWord = wStartWord;
	SKT_SWAP_WORD(m_wStartWord);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  发送一帧报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 12:31
// 参数说明:  @pS表示SOCKET实例
//            @pPackage表示待发送的数据包
// 返 回 值:  true表示发送成功,false表示发送失败
//////////////////////////////////////////////////////////////////////////
bool STcpCommBase::SendFrame(SSocket *pS,stuSTcpPackage *pPackage)
{
	return SendFrame(pS,pPackage->m_sHead,pPackage->m_wFrameType,pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
}

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
stuSTcpPackage* STcpCommBase::NewPackage(SString &sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,
										   int iAsduLen/*=0*/,bool bCloneBuffer/*=true*/)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	p->m_sHead = sHead;
	p->m_wFrameType = wFrameType;
	if(pAsduBuffer != NULL && iAsduLen != 0)
	{
		if(bCloneBuffer)
		{
			p->m_pAsduBuffer = new BYTE[iAsduLen];
			memcpy(p->m_pAsduBuffer,pAsduBuffer,iAsduLen);
		}
		else
		{
			p->m_pAsduBuffer = pAsduBuffer;
		}
		p->m_iAsduLen = iAsduLen;
	}
	return p;
}
stuSTcpPackage* STcpCommBase::NewPackage(char* sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,
										   int iAsduLen/*=0*/,bool bCloneBuffer/*=true*/)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	if(sHead != NULL)
		p->m_sHead = sHead;
	p->m_wFrameType = wFrameType;
	if(pAsduBuffer != NULL && iAsduLen != 0)
	{
		if(bCloneBuffer)
		{
			p->m_pAsduBuffer = new BYTE[iAsduLen];
			memcpy(p->m_pAsduBuffer,pAsduBuffer,iAsduLen);
		}
		else
		{
			p->m_pAsduBuffer = pAsduBuffer;
		}
		p->m_iAsduLen = iAsduLen;
	}
	return p;
}
stuSTcpPackage* STcpCommBase::NewPackage(WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,
										   int iAsduLen/*=0*/,bool bCloneBuffer/*=true*/)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	p->m_wFrameType = wFrameType;
	if(pAsduBuffer != NULL && iAsduLen != 0)
	{
		if(bCloneBuffer)
		{
			p->m_pAsduBuffer = new BYTE[iAsduLen];
			memcpy(p->m_pAsduBuffer,pAsduBuffer,iAsduLen);
		}
		else
		{
			p->m_pAsduBuffer = pAsduBuffer;
		}
		p->m_iAsduLen = iAsduLen;
	}
	return p;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从原报文复制一个新报文
// 作    者:  邵凯田
// 创建时间:  2011-11-11 19:50
// 参数说明:  @pPackage原报文
// 返 回 值:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpCommBase::NewPackage(stuSTcpPackage *pPackage)
{
	stuSTcpPackage *p = new stuSTcpPackage();
	p->m_sHead = pPackage->m_sHead;
	p->m_wFrameType = pPackage->m_wFrameType;
	if(pPackage->m_iAsduLen != 0)
	{
		p->m_pAsduBuffer = new BYTE[pPackage->m_iAsduLen];
		memcpy(p->m_pAsduBuffer,pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
		p->m_iAsduLen = pPackage->m_iAsduLen;
	}
	return p;
}

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
bool STcpCommBase::SendFrame(SSocket *pS,SString &sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	if(iAsduLen < 0 )
		return false;
	int slen = sHead.length();
	int len = iAsduLen + slen + 10;//总长度
	if(len > m_iSendBufferSize)
	{
		if(m_iSendBufferSize > 0)
			delete[] m_pSendBuffer;
		m_pSendBuffer = new BYTE[len];
		m_iSendBufferSize = len;
		memcpy(m_pSendBuffer,&m_wStartWord,2);
	}
	WORD w;
	DWORD dw;
	w = sHead.length();
	SKT_SWAP_WORD(w);
	memcpy(m_pSendBuffer+2,&w,2);
	dw = iAsduLen;
	SKT_SWAP_DWORD(dw);
	memcpy(m_pSendBuffer+4,&dw,4);
	SKT_SWAP_WORD(wFrameType);
	memcpy(m_pSendBuffer+8,&wFrameType,2);
	if(slen > 0)
		memcpy(m_pSendBuffer+10,sHead.data(),slen);
	if(pAsduBuffer != NULL && iAsduLen > 0)
		memcpy(m_pSendBuffer+10+slen,pAsduBuffer,iAsduLen);

	int ret;
	if(m_iSendLenLimit == 0)
	{
		ret = pS->Send(m_pSendBuffer,len);
		if(ret != len)
		{
			LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
			return false;
		}
	}
	else
	{
		int pos = 0;
		while(pos < len)
		{
			slen = len - pos;
			if(slen > m_iSendLenLimit)
				slen = m_iSendLenLimit;
			ret = pS->Send(m_pSendBuffer+pos,slen);
			if(ret != slen)
			{
				LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
				return false;
			}
			pos += slen;
			if(m_iSendSleep != 0)
				SApi::UsSleep(m_iSendSleep);

		}
	}
	if(m_iSendSleep != 0)
		SApi::UsSleep(m_iSendSleep);

	return true;
}

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
bool STcpCommBase::SendFrame(SSocket *pS,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	if(iAsduLen < 0)
		return false;
	int len = iAsduLen + 10;//总长度
	if(len > m_iSendBufferSize)
	{
		if(m_iSendBufferSize > 0)
			delete[] m_pSendBuffer;
		m_pSendBuffer = new BYTE[len];
		m_iSendBufferSize = len;
		memcpy(m_pSendBuffer,&m_wStartWord,2);
	}

	DWORD dw;
	memset(m_pSendBuffer+2,0,2);
	dw = iAsduLen;
	SKT_SWAP_DWORD(dw);
	memcpy(m_pSendBuffer+4,&dw,4);
	SKT_SWAP_WORD(wFrameType);
	memcpy(m_pSendBuffer+8,&wFrameType,2);
	if(pAsduBuffer != NULL && iAsduLen > 0)
		memcpy(m_pSendBuffer+10,pAsduBuffer,iAsduLen);

	int ret;
	if(m_iSendLenLimit == 0)
	{
		ret = pS->Send(m_pSendBuffer,len);
		if(ret != len)
		{
			LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
			return false;
		}
	}
	else
	{
		int slen,pos = 0;
		while(pos < len)
		{
			slen = len - pos;
			if(slen > m_iSendLenLimit)
				slen = m_iSendLenLimit;
			ret = pS->Send(m_pSendBuffer+pos,slen);
			if(ret != slen)
			{
				LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
				return false;
			}
			pos += slen;
			if(m_iSendSleep != 0)
				SApi::UsSleep(m_iSendSleep);

		}
	}
	if(m_iSendSleep != 0)
		SApi::UsSleep(m_iSendSleep);
	/*
	int ret = pS->Send(m_pSendBuffer,len);
	if(ret != len)
	{
		LOGWARN("STcpCommBase::SendFrame error! len=%d,ret=%d",len,ret);
		return false;
	}
	*/
	return true;
}

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
int* STcpCommBase::FillFullFrameHead(BYTE *pBuffer,WORD wFrameType,int iAsduLen)
{
	memcpy(pBuffer,&m_wStartWord,2);
	
	memset(pBuffer+2,0,2);
	SKT_SWAP_DWORD(iAsduLen);
	memcpy(pBuffer+4,&iAsduLen,4);
	SKT_SWAP_WORD(wFrameType);
	memcpy(pBuffer+8,&wFrameType,2);

	return (int*)(pBuffer+4);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  预设缓冲区大小，当当前发送缓冲区大小大于给定大小时忽略
// 作    者:  邵凯田
// 创建时间:  2014:4:26 10:51
// 参数说明:  @iStrSize为头字符串长度
//         :  @iAsduSize为ASDU长度
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void STcpCommBase::SetSendBufferSize(int iStrSize,int iAsduSize)
{
	int size = GetFramePrefixLen()+iStrSize+iAsduSize;
	if(m_iSendBufferSize < size)
	{
		BYTE *p = m_pSendBuffer;
		m_pSendBuffer = new BYTE[size];
		if(p != NULL)
			delete[] p;
		m_iSendBufferSize = size;
	}
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取ASDU在整体发送缓冲区中的偏移位置
// 作    者:  邵凯田
// 创建时间:  2014:4:26 10:53
// 参数说明:  @iStrSize为关字符串长度
// 返 回 值:  BYTE*
//////////////////////////////////////////////////////////////////////////
BYTE* STcpCommBase::GetAsduOffsetInBuffer(int iStrSize)
{
	if(m_pSendBuffer == NULL)
		return NULL;
	return m_pSendBuffer + GetFramePrefixLen() + iStrSize;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  接收一个数据包到stuSTcpPackage指针中并引用返回新实例
// 作    者:  邵凯田
// 创建时间:  2011-11-10 12:33
// 参数说明:  @pS表示SOCKET实例
//            @pPackage表示数据包引用指针，内部申请stuSTcpPackage对象，外部负责释放
// 返 回 值:  <0表示接收失败，=0表示接收超时，>0表示接收成功且等于报文总长度
//////////////////////////////////////////////////////////////////////////
int STcpCommBase::RecvFrame(SSocket *pS,stuSTcpPackage* &pPackage)
{
	if(pPackage == NULL)
		pPackage = new stuSTcpPackage();
	else
	{
		if(pPackage->m_pAsduBuffer == NULL)
			pPackage->m_iAsduBufferLen = 0;
	}
	int ret;
	BYTE head[10];
	ret = pS->Receive(head,10);
	if(ret != 10)
	{
		LOGERROR("接收失败!期望返回10,实际返回=%d,sockerr=%d,HEX=%s",ret,pS->GetLastError(),ret<=0?"":SString::HexToStr((BYTE*)&head,ret).data());
		if(ret == 0)
			return 0;
		if(ret < 0)
			return -1;
		pS->Close();
		LOGDEBUG("RECV(len:%d):%s\n",ret,SString::HexToStr((BYTE*)&head,ret).data());
		return -1;
	}
	WORD w;
	DWORD dw;
	memcpy(&w,head,2);
	if(w != m_wStartWord)
	{
		LOGWARN("收到无效报文!错误的启动字符%02X%02X",head[0],head[1]);
		LOGDEBUG("RECV(len:%d):%s\n",ret,SString::HexToStr((BYTE*)&head,ret).data());
		BYTE *pBuffer=new BYTE[1024];
		while(1)
		{
			ret = pS->Receive(pBuffer,1024);
			if(ret <= 0)
				break;
			LOGDEBUG("RECV(len:%d):%s\n",ret,SString::HexToStr(pBuffer,ret).data());
		}
		delete[] pBuffer;
		return -2;
	}
	memcpy(&w,&head[2],2);
	SKT_SWAP_WORD(w);
	int slen = w;
	memcpy(&dw,&head[4],4);
	SKT_SWAP_DWORD(dw);
	pPackage->m_iAsduLen = dw;
	if(slen < 0 )
	{
		LOGWARN("收到无效报文!错误的长度:%d:%s",slen,pPackage->m_iAsduLen);
		return -3;
	}
	memcpy(&pPackage->m_wFrameType,&head[8],2);
	SKT_SWAP_WORD(pPackage->m_wFrameType);
	if(slen != 0)
	{
		pPackage->m_sHead.SetBuffer(slen+1);
		ret = pS->Receive(pPackage->m_sHead.data(),slen);
		pPackage->m_sHead.PrepareStrLen();
		if(ret != slen)
		{
			LOGWARN("接收报文头字符串时失败!slen=%d,ret=%d",slen,ret);
			return -4;
		}
	}
	if(pPackage->m_iAsduLen != 0)
	{
		if(pPackage->m_iAsduLen > pPackage->m_iAsduBufferLen)
		{
			if((!pPackage->m_bFixBuffer || pPackage->m_bOverflow) && pPackage->m_pAsduBuffer != NULL)
				delete[] pPackage->m_pAsduBuffer;
			if(pPackage->m_bFixBuffer)
				pPackage->m_bOverflow = true;//固定缓冲区溢出
			pPackage->m_iAsduBufferLen = pPackage->m_iAsduLen;
			pPackage->m_pAsduBuffer = new BYTE[pPackage->m_iAsduLen];
		}
		int len = 0;
		int timeout_times=0;
		while(len < pPackage->m_iAsduLen)
		{
			ret = pS->Receive(pPackage->m_pAsduBuffer+len,pPackage->m_iAsduLen-len);
			if(ret < 0)
			{
				pS->Close();
				LOGWARN("接收报文体时失败!len=%d,ret=%d",pPackage->m_iAsduLen,ret);
				return -5;
			}
			else if(ret == 0)
			{
				timeout_times ++;
				if(timeout_times > 100)
				{
					pS->Close();
					LOGWARN("接收报文体时失败!len=%d,ret=%d",pPackage->m_iAsduLen,ret);
					return -6;
				}
				SApi::UsSleep(100000);
			}
			len += ret;
		}
	}
	return 10+slen+pPackage->m_iAsduLen;
}

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
#if 0
int STcpCommBase::RecvFrame(SSocket *pS,SString &sHead,WORD &wFrameType,BYTE *&pAsduBuffer,int &iAsduLen,stuYsTcpPackageFixRef *pFixRef/*=NULL*/)
{
	pAsduBuffer = NULL;
	int ret;
	BYTE head[10];
	ret = pS->Receive(head,10);
	if(ret != 10)
	{
		LOGERROR("接收失败!期望返回10,实际返回=%d,sockerr=%d,HEX=%s",ret,pS->GetLastError(),ret<=0?"":SString::HexToStr((BYTE*)&head,ret).data());
		if(ret == 0)
			return 0;
		return -1;
	}
	WORD w;
	DWORD dw;
	memcpy(&w,head,2);
	if(w != m_wStartWord)
	{
		LOGWARN("收到无效报文!错误的启动字符%02X%02X",head[0],head[1]);
		return -2;
	}
	memcpy(&w,&head[2],2);
	SKT_SWAP_WORD(w);
	int slen = w;
	memcpy(&dw,&head[4],4);
	SKT_SWAP_DWORD(dw);
	iAsduLen = dw;
	if(slen < 0 )
	{
		LOGWARN("收到无效报文!错误的长度:%d:%s",slen,iAsduLen);
		return -3;
	}
	memcpy(&wFrameType,&head[8],2);
	SKT_SWAP_WORD(wFrameType);
	if(slen != 0)
	{
		sHead.SetBuffer(slen+1);
		ret = pS->Receive(sHead.data(),slen);
		pPackage->m_sHead.PrepareStrLen();
		if(ret != slen)
		{
			LOGWARN("接收报文头字符串时失败!slen=%d,ret=%d",slen,ret);
			return -4;
		}
	}
	if(iAsduLen != 0)
	{
		if(iAsduLen > m_iSendBufferSize)
		pAsduBuffer = new BYTE[iAsduLen];
		int len = 0;
		int timeout_times=0;
		while(len < iAsduLen)
		{
			ret = pS->Receive(pAsduBuffer+len,iAsduLen-len);
			if(ret < 0)
			{
				delete[] pAsduBuffer;
				pAsduBuffer = NULL;
				pS->Close();
				LOGWARN("接收报文体时失败!len=%d,ret=%d",iAsduLen,ret);
				return -5;
			}
			else if(ret == 0)
			{
				timeout_times ++;
				if(timeout_times > 100)
				{
					delete[] pAsduBuffer;
					pAsduBuffer = NULL;
					pS->Close();
					LOGWARN("接收报文体时失败!len=%d,ret=%d",iAsduLen,ret);
					return -6;
				}
				SApi::UsSleep(100000);
			}
			len += ret;
		}
	}
	return 10+slen+iAsduLen;
}
#endif

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
bool STcpCommBase::SendAndRecv(SSocket *pS,stuSTcpPackage *pPackage,stuSTcpPackage* &pRecvPackage,int iTimeout_s/*=0*/)
{
	if(!SendFrame(pS,pPackage))
		return false;
	time_t now = ::time(NULL);
	while(pS->IsConnected())
	{
		SApi::UsSleep(100000);
		if(RecvFrame(pS,pRecvPackage)>0)
		{
			return true;
		}
		if(iTimeout_s > 0 && abs((int)(::time(NULL)-now)) > iTimeout_s)
			break;
	}
	return false;
}

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
bool STcpCommBase::SendAndRecv(SSocket *pS,stuSTcpPackage* &pRecvPackage,
				 SString &sHead,WORD wFrameType,BYTE *pAsduBuffer,int iAsduLen,int iTimeout_s/*=0*/)
{
	if(!SendFrame(pS,sHead,wFrameType,pAsduBuffer,iAsduLen))
		return false;
	time_t now = ::time(NULL);
	while(pS->IsConnected())
	{
		SApi::UsSleep(100000);
		if(RecvFrame(pS,pRecvPackage)>0)
		{
			return true;
		}
		if(iTimeout_s > 0 && abs((int)(::time(NULL)-now)) > iTimeout_s)
			break;
	}
	return false;
}

