/**
 *
 * 文 件 名 : STcpClientBase.cpp
 * 创建日期 : 2011-11-8 17:54
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/


#include "STcpClientBase.h"


//////////////////////////////////////////////////////////////////////////
// 名    称:  STcpClientBase
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 描    述:  TCP通讯客户端基类
//////////////////////////////////////////////////////////////////////////
STcpClientBase::STcpClientBase()
{
	m_bConnected = false;
	m_SendPackage.setAutoDelete(true);
	m_RecvPackage.setAutoDelete(false);
	m_SendPackage.setShared(true);
	m_RecvPackage.setShared(true);
	m_bConnectAtStart = false;

}

STcpClientBase::~STcpClientBase()
{
	Close();
	StopAndWait();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  启动服务
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 参数说明:  @ip表示服务端IP地址，@port表示端口
// 返 回 值:  true表示服务启动成功,false表示服务启动失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Start(SString ip,int port)
{
	m_sServerIp = ip;
	m_iServerPort = port;
	m_bConnected = false;
	if(!SService::Start())
	{
		return false;
	}
	if(m_bConnectAtStart)
	{
		if(!Login())
		{
			Stop();
			return false;
		}
	}
	SKT_CREATE_THREAD(ThreadClient,this);
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止服务
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 参数说明:  void
// 返 回 值:  true表示服务停止成功,false表示服务停止失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Stop()
{
	if(!SService::Stop())
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  登录服务器
// 作    者:  邵凯田
// 创建时间:  2011-11-17 10:41
// 参数说明:  void
// 返 回 值:  true登录成功,false登录失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Login()
{
	//连接服务端并登录
	SString sHead;
	SString sLoginHead;
	sLoginHead.sprintf("act=login;%s",m_sLoginInfo.data());
	int ret;

	m_bConnected = false;
	SSocket *pS = &m_Socket;
	pS->Close();
	pS->CreateTcp();
	LOGDEBUG("准备登录服务端:%s:%d",m_sServerIp.data(),m_iServerPort);
	if(!pS->Connect(m_sServerIp.data(),m_iServerPort))
	{
		OnClose();
		LOGERROR("连接服务端失败(%s:%d)!",m_sServerIp.data(),m_iServerPort);
		return false;
	}
	pS->SetTimeout(1000,5000);
	//登录
	sHead = "";
	if(!SendFrame(sLoginHead))
	{
		m_Socket.Close();
		OnClose();
		LOGERROR("发送登录报文失败!");
		return false;
	}
	SApi::UsSleep(100000);
	stuSTcpPackage *pPackage = NULL;
	ret = RecvFrame(pPackage);
	if(ret <= 0 || pPackage == NULL || SString::GetAttributeValue(pPackage->m_sHead,"act") != "login" ||
		SString::GetAttributeValueI(pPackage->m_sHead,"result") != 1)
	{
		m_Socket.Close();
		OnClose();
		if(pPackage != NULL)
			delete pPackage;
		LOGERROR("登录服务端失败(%s:%d)!",m_sServerIp.data(),m_iServerPort);
		return false;
	}
	delete pPackage;
	m_bConnected = true;
	OnLogin();
	LOGDEBUG("登录服务端:%s:%d成功!",m_sServerIp.data(),m_iServerPort);
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  以非服务形式连接客户端，不能与Start函数同时使用
// 作    者:  邵凯田
// 创建时间:  2011-11-17 10:08
// 参数说明:  @ip表示服务端IP地址，@port表示端口
// 返 回 值:  true表示连接成功,false表示连接失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::Connect(SString ip,int port)
{
	this->m_sServerIp = ip;
	this->m_iServerPort = port;
	return Login();
}

////////////////////////////////////////////////////////////////////////
// 描    述:  关闭连接,对应Connect函数
// 作    者:  邵凯田
// 创建时间:  2011-11-17 10:11
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void STcpClientBase::Close()
{
	m_Socket.Close();
	m_bConnected = false;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  添加数据到发送队列，在发送完成后释放pPackage对象
// 作    者:  邵凯田
// 创建时间:  2011-11-10 12:40
// 参数说明:  @pPackage待发送报文
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void STcpClientBase::AddToSend(stuSTcpPackage *pPackage)
{
	m_SendPackage.append(pPackage);
	while(m_SendPackage.count() > 10000)
		m_SendPackage.remove(0);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  添加数据包到接收队列中
// 作    者:  邵凯田
// 创建时间:  2012-2-3 14:01
// 参数说明:  @pPackage为新接收到的数据包
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void STcpClientBase::AddToRecv(stuSTcpPackage *pPackage)
{
	if(!pPackage->m_bAutoDelete)
	{
		//不允许自动删除的对象，需要复制一份
		stuSTcpPackage *pNew = new stuSTcpPackage();
		pNew->m_sHead = pPackage->m_sHead;
		pNew->m_iAsduLen = pNew->m_iAsduBufferLen = pPackage->m_iAsduLen;
		if(pPackage->m_pAsduBuffer != NULL && pPackage->m_iAsduLen > 0)
		{
			pNew->m_pAsduBuffer = new BYTE[pNew->m_iAsduLen];
			memcpy(pNew->m_pAsduBuffer,pPackage->m_pAsduBuffer,pNew->m_iAsduLen);
		}
		pPackage = pNew;
	}
	if( pPackage->m_bFixBuffer && !pPackage->m_bOverflow)
	{
		//固定内存时且没有溢出，不能将缓冲区加入队列，应说新建缓冲
		if(pPackage->m_pAsduBuffer != NULL && pPackage->m_iAsduLen > 0)
		{
			BYTE *pBuf = new BYTE[pPackage->m_iAsduLen];
			memcpy(pBuf,pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
			pPackage->m_bFixBuffer = pPackage->m_bOverflow = false;
			pPackage->m_pAsduBuffer = pBuf;
		}

	}
	m_RecvPackage.append(pPackage);
	while(m_RecvPackage.count() > 10000)
		m_RecvPackage.remove(0);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  通过报文头字符串中的act属性检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  @act表示待查的报文
// 返 回 值:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpClientBase::SearchRecvPackage(SString act)
{
	unsigned long pos;
	m_RecvPackage.lock();
	stuSTcpPackage *pPackage = m_RecvPackage.FetchFirst(pos);
	while(pPackage != NULL)
	{
		if(SString::GetAttributeValue(pPackage->m_sHead,"act") == act)
		{
			m_RecvPackage.unlock();
			m_RecvPackage.remove(pPackage);
			return pPackage;
		}
		pPackage = m_RecvPackage.FetchNext(pos);
	}
	m_RecvPackage.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  通过报文类型检索接收队列中的报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 21:32
// 参数说明:  @wFrameType表示
// 返 回 值:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpClientBase::SearchRecvPackage(WORD wFrameType)
{
	unsigned long pos;
	m_RecvPackage.lock();
	stuSTcpPackage *pPackage = m_RecvPackage.FetchFirst(pos);
	while(pPackage != NULL)
	{
		if(pPackage->m_wFrameType == wFrameType)
		{
			m_RecvPackage.unlock();
			m_RecvPackage.remove(pPackage);
			return pPackage;
		}
		pPackage = m_RecvPackage.FetchNext(pos);
	}
	m_RecvPackage.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  客户端通讯线程
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:41
// 参数说明:  this
// 返 回 值:  NULL
//////////////////////////////////////////////////////////////////////////
void* STcpClientBase::ThreadClient(void* lp)
{
	STcpClientBase *pThis = (STcpClientBase*)lp;
	pThis->BeginThread();
	int ret,cnt;
	SSocket *pS = &pThis->m_Socket;
	SString sLoginHead;
	sLoginHead.sprintf("act=login;%s",pThis->m_sLoginInfo.data());
	SString sHead;
	stuSTcpPackage *pPackage;
	int last_recv_time = (int)::time(NULL);//最后接收到数据的时间
	int last_send_time = (int)::time(NULL);//最后发送数据的时间
	int no_recv=0;//没有收到数据的次数
	int no_send=0;//没有发送数据的次数
	bool more;
	bool first = true;

	while(!pThis->IsQuit())
	{
		if(first)
		{
			first = false;
			if(!pThis->m_bConnected)
			{
				if(!pThis->Login())
				{
					LOGERROR("登录服务端失败!");
					for(int i=0;!pThis->IsQuit() && i<100;i++)
						SApi::UsSleep(50000);
					continue;
				}
				pThis->m_bConnected = true;
			}
		}
		else
		{
			pThis->OnClose();
			//连接服务端并登录
			if(!pThis->Login())
			{
				LOGERROR("登录服务端失败!");
				for(int i=0;!pThis->IsQuit() && i<100;i++)
					SApi::UsSleep(50000);
				continue;
			}
			pThis->m_bConnected = true;
		}
		
		while(!pThis->IsQuit())
		{
			//是否有需要发送的数据
			more = false;
			cnt = 0;
			pPackage = pThis->m_SendPackage[0];
			if(pPackage == NULL)
			{
				if(++no_send > 10)
				{
					no_send = 0;
					if(abs((int)::time(NULL)-last_send_time) >= 10)
					{
						//10秒没有内容发送，发一次心跳帧
						sHead = "act=echo;";
						pThis->lock();
						if(!pThis->SendFrame(sHead))
						{
							pThis->m_Socket.Close();
							pThis->unlock();
							pThis->OnClose();
							pThis->m_bConnected = false;
							LOGERROR("发送心跳报文失败!");
							break;
						}
						pThis->unlock();
						last_send_time = (int)::time(NULL);
					}
				}
			}
			else
			{
				no_send = 0;
			}
			while(pPackage)
			{
				pThis->lock();
				if(!pThis->SendFrame(pPackage))
				{
					LOGERROR("发送报文失败!");
					pThis->m_Socket.Close();
					pThis->OnClose();
					pThis->m_bConnected = false;
					pThis->unlock();
					break;
				}
				pThis->unlock();
				pThis->m_SendPackage.remove(pPackage);
				pPackage = pThis->m_SendPackage[0];
				if(cnt ++ > 100)
				{
					if(pPackage != NULL)
						more = true;
					last_send_time = (int)::time(NULL);//更新发送时间
					break;
				}
				if(pPackage == NULL)
					last_send_time = (int)::time(NULL);//更新发送时间
			}
			if(!pThis->m_bConnected)
			{
				LOGWARN("连接断开!");
				pThis->m_Socket.Close();
				pThis->OnClose();
				break;
			}

			//接收报文
			pThis->lock();
			pS->SetTimeout(1,5000);
			ret = pS->CheckForRecv();
			pS->SetTimeout(1000,5000);
			if(ret < 0)
			{
				LOGWARN("接收报文时失败!");
				pThis->m_Socket.Close();
				pThis->OnClose();
				pThis->m_bConnected = false;
				pThis->unlock();
				break;
			}
			else if(ret == 0)
			{
				//没有数据到达
				if(++no_recv > 10)
				{
					no_recv = 0;
					if(abs((int)::time(NULL)-last_recv_time) >= 30)
					{
						last_recv_time = (int)::time(NULL);
						pThis->m_Socket.Close();
						pThis->OnClose();
						pThis->m_bConnected = false;
						pThis->unlock();
						LOGWARN("30秒钟没有数据到达时，认为连接异常，断开连接!");
						break;//30秒钟没有数据到达时，认为连接异常，断开连接
					}
				}
			}
			else if(ret == 1)
			{
				//有新的数据到达
				no_recv = 0;
				pPackage = NULL;
				ret = pThis->RecvFrame(pPackage);
				if(ret == 0)
				{
					//没有数据
				}
				else if(ret < 0)
				{
					//异常
					LOGWARN("接收报文时失败!");
					pThis->m_Socket.Close();
					pThis->OnClose();
					pThis->m_bConnected = false;
					pThis->unlock();
					break;
				}
				else
				{
					if(pPackage != NULL)
					{
						if(pPackage->m_wFrameType == 0 && SString::GetAttributeValue(pPackage->m_sHead,"act")=="echo")
							delete pPackage;//0类型认为是心跳报文
						else if(!pThis->OnRecvFrame(pPackage))
						{
							pThis->m_RecvPackage.append(pPackage);
							if(pThis->m_RecvPackage.count()>10000)
								pThis->m_RecvPackage.remove(0);
						}
						else
						{
							if(pPackage->m_bAutoDelete)
							{
								delete pPackage;
							}
						}
						last_recv_time = (int)::time(NULL);//更新接收时间
						pPackage = NULL;
					}
				}
				if(pPackage != NULL)
					delete pPackage;
				more = true;
			}
			pThis->unlock();
			if(!more)
				SApi::UsSleep(100000);
		}
		SApi::UsSleep(1000000);
	}
	pThis->OnClose();

	pThis->EndThread();
	return NULL;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  发送一帧报文
// 作    者:  邵凯田
// 创建时间:  2011-11-10 12:31
// 参数说明:  @pPackage表示待发送的数据包
// 返 回 值:  true表示发送成功,false表示发送失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendFrame(stuSTcpPackage *pPackage)
{
	return STcpCommBase::SendFrame(&m_Socket,pPackage);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  发送一帧报文
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:17
// 参数说明:  @sHead表示报文头字符串
//            @wFrameType表报文类型
//            @pAsduBuffer表示ASDU内容缓冲区，当pAsduBuffer为NULL且iAsduLen不为0时，表示使用预设的发送缓冲区
//            @iAsduLen表示ASDU数据长度
// 返 回 值:  true表示发送成功,false表示发送失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendFrame(SString &sHead,WORD wFrameType,BYTE *pAsduBuffer,int iAsduLen)
{
	return STcpCommBase::SendFrame(&m_Socket,sHead,wFrameType,pAsduBuffer,iAsduLen);
}


////////////////////////////////////////////////////////////////////////
// 描    述:  接收一个数据包到stuSTcpPackage指针中并引用返回新实例
// 作    者:  邵凯田
// 创建时间:  2011-11-10 12:33
// 参数说明:  @pPackage表示数据包引用指针，内部申请stuSTcpPackage对象，外部负责释放
// 返 回 值:  <0表示接收失败，=0表示接收超时，>0表示接收成功且等于报文总长度
//////////////////////////////////////////////////////////////////////////
int STcpClientBase::RecvFrame(stuSTcpPackage* &pPackage)
{
	return STcpCommBase::RecvFrame(&m_Socket,pPackage);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  接收一个数据包到新缓冲区
// 作    者:  邵凯田
// 创建时间:  2011-11-8 19:18
// 参数说明:  @sHead表示报文头字符串
//            @wFrameType表示报文类型，引用返回
//            @pAsduBuffer表示ASDU内容缓冲区，内部申请内容并引用返回
//            @iAsduLen表示ASDU数据长度，引用返回
// 返 回 值:  <0表示接收失败，=0表示接收超时，>0表示接收成功且等于报文总长度
//////////////////////////////////////////////////////////////////////////
#if 0
int STcpClientBase::RecvFrame(SString &sHead,WORD &wFrameType,BYTE *&pAsduBuffer,int &iAsduLen)
{
	return STcpCommBase::RecvFrame(&m_Socket,sHead,wFrameType,pAsduBuffer,iAsduLen);
}
#endif

////////////////////////////////////////////////////////////////////////
// 描    述:  
// 作    者:  邵凯田
// 创建时间:  2011-11-17 13:37
// 参数说明:  @pPackage表示待发送的数据包
//            @pRecvPackage表示接收到的新包
//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
// 返 回 值:  true表示发送成功,false表示发送失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendAndRecv(stuSTcpPackage *pPackage,stuSTcpPackage* &pRecvPackage,int iTimeout_s)
{
	return STcpCommBase::SendAndRecv(&m_Socket,pPackage,pRecvPackage,iTimeout_s);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  
// 作    者:  邵凯田
// 创建时间:  2011-11-17 14:04
// 参数说明:  @pRecvPackage表示接收到的新包
//            @sHead表示报文头字符串
//            @wFrameType表示报文类型，引用返回
//            @pAsduBuffer表示ASDU内容缓冲区，内部申请内容并引用返回
//            @iAsduLen表示ASDU数据长度，引用返回
//            @iTimeout_s表示超时时间（单位为秒），0表示不超时未收到数据时一直等待
// 返 回 值:  true表示发收成功，false表示失败
//////////////////////////////////////////////////////////////////////////
bool STcpClientBase::SendAndRecv(stuSTcpPackage* &pRecvPackage,
				 SString &sHead,WORD wFrameType,BYTE *pAsduBuffer,int iAsduLen,int iTimeout_s)
{
	return STcpCommBase::SendAndRecv(&m_Socket,pRecvPackage,sHead,wFrameType,pAsduBuffer,iAsduLen,iTimeout_s);
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  接收指定类型的报文
// 作    者:  邵凯田
// 创建时间:  2012-2-15 11:09
// 参数说明:  @sFrameType表示待接收的报文类型
//            @pRecvPackage表示接收报文指针，引用返回
//            @iTimeout_s表示超时时间
// 返 回 值:  >0表示收到目标报文，=0表示接收超时，<0表示失败
//////////////////////////////////////////////////////////////////////////
int STcpClientBase::RecvPackage(WORD sFrameType,stuSTcpPackage* &pRecvPackage,int iTimeout_s/*=5*/)
{
	time_t now = ::time(NULL);
	int ret;
	while(this->IsConnected())
	{
		ret = RecvFrame(pRecvPackage);
		if(ret == 0)
		{
			if(abs((int)(::time(NULL)-now)) > iTimeout_s)
			{
				LOGWARN("RECV type=%d timeout(%d seconds)!",sFrameType,iTimeout_s);
				break;
			}
			SApi::UsSleep(100000);
			continue;
		}
		if(ret < 0)
		{
			return ret;
		}
		if(pRecvPackage == NULL)
		{
			SApi::UsSleep(100000);
			continue;
		}
		if(pRecvPackage->m_wFrameType != sFrameType)
		{
			if(OnRecvFrame(pRecvPackage))
			{
				if(pRecvPackage->m_bAutoDelete)
				{
					delete pRecvPackage;
					pRecvPackage = NULL;
				}
			}
			else
				AddToRecv(pRecvPackage);
			continue;
		}
		return 1;
	}
	return 0;

}

