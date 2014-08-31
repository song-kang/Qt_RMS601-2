/**
 *
 * �� �� �� : STcpServerBase.cpp
 * �������� : 2011-11-8 17:54
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/


#include "STcpServerBase.h"


//////////////////////////////////////////////////////////////////////////
// ��    ��:  STcpServerSession
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-11 8:46
// ��    ��:  ����Ự��
//////////////////////////////////////////////////////////////////////////

STcpServerSession::STcpServerSession()
{
	m_SendPackage.setAutoDelete(true);
	m_RecvPackage.setAutoDelete(false);
	m_SendPackage.setShared(true);
	m_RecvPackage.setShared(true);
	m_pSocket = NULL;
	m_ValidWord = 0xeb903469;
}

STcpServerSession::~STcpServerSession()
{
	m_ValidWord = 0;
	if(m_pSocket != NULL && m_pSocket->IsConnected())
		m_pSocket->Close();

	lock();
	if(m_pSocket != NULL)
		delete m_pSocket;
	m_SendPackage.setAutoDelete(true);
	m_RecvPackage.setAutoDelete(true);
	m_SendPackage.clear();
	m_RecvPackage.clear();
	unlock();
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ݵ����Ͷ��У��ڷ�����ɺ��ͷ�pPackage����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:40
// ����˵��:  @pPackage�����ͱ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpServerSession::AddToSend(stuSTcpPackage *pPackage)
{
	m_SendPackage.append(pPackage);
	while(m_SendPackage.count() > 10000)
		m_SendPackage.remove(0);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�Ự�Զ˷���һ֡���ģ�ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-01-17 13:46
// ����˵��:  @pPackage��ʾ�����͵����ݰ�
//            @bAutoDelPackage��ʾ�Ƿ���ʹ����Ϻ��Զ�ɾ��pPackage
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerSession::SendFrame(stuSTcpPackage *pPackage,bool bAutoDelPackage/*=false*/)
{
	bool ret = STcpCommBase::SendFrame(m_pSocket,pPackage);
	if(bAutoDelPackage)
		delete pPackage;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�Ự�Զ���������һ֡���ģ���ͨ�����ͻ���������ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-01-17 13:57
// ����˵��:  @sHead��ʾ����ͷ�ַ���
//            @wFrameType��ʾ��������
//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
//            @iAsduLen��ʾASDU���ݳ���
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerSession::SendFrame(SString &sHead,WORD wFrameType/*=0*/,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	return STcpCommBase::SendFrame(m_pSocket,sHead,wFrameType,pAsduBuffer,iAsduLen);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ�Ự�Զ���������һ֡���ģ���ͨ�����ͻ���������ֻ����OnRecvFrame������ʹ�ã������̲߳���ȫ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-01-17 13:57
// ����˵��:  @wFrameType��ʾ��������
//            @pAsduBuffer��ʾASDU���ݻ���������pAsduBufferΪNULL��iAsduLen��Ϊ0ʱ����ʾʹ��Ԥ��ķ��ͻ�����
//            @iAsduLen��ʾASDU���ݳ���
// �� �� ֵ:  true��ʾ���ͳɹ�,false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerSession::SendFrame(WORD wFrameType,BYTE *pAsduBuffer/*=NULL*/,int iAsduLen/*=0*/)
{
	return STcpCommBase::SendFrame(m_pSocket,wFrameType,pAsduBuffer,iAsduLen);
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerSession::SearchRecvPackage()
{
	stuSTcpPackage *pPackage = m_RecvPackage[0];
	if(pPackage != NULL)
	{
		m_RecvPackage.remove(pPackage);
	}
	return pPackage;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ������ͷ�ַ����е�act���Լ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @act��ʾ����ı���
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerSession::SearchRecvPackage(SString act)
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
// ��    ��:  ͨ���������ͼ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @wFrameType��ʾ
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerSession::SearchRecvPackage(WORD wFrameType)
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

//////////////////////////////////////////////////////////////////////////
// ��    ��:  STcpServerBase
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ��    ��:  TCPͨѶ�ͻ��˻���
//////////////////////////////////////////////////////////////////////////
STcpServerBase::STcpServerBase()
{
	m_Sessions.setShared(true);
	m_Sessions.setAutoDelete(true);
}

STcpServerBase::~STcpServerBase()
{
	
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  @port��ʾ�˿�,@ip��ʾ�����IP��ַ
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerBase::Start(int port,SString ip/*=""*/)
{
	m_sServerIp = ip;
	m_iServerPort = port;
	if(!SService::Start())
	{
		return false;
	}
	SKT_CREATE_THREAD(ThreadListen,this);
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool STcpServerBase::Stop()
{
	if(!SService::Stop())
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ݵ����Ͷ��У��ڷ�����ɺ��ͷ�pPackage����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 12:40
// ����˵��:  @pPackage�����ͱ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void STcpServerBase::AddToSend(stuSTcpPackage *pPackage)
{
	unsigned long pos;
	m_Sessions.lock();
	STcpServerSession *p2,*pSession = m_Sessions.FetchFirst(pos);
	while(pSession)
	{
		p2 = m_Sessions.FetchNext(pos);
		if(p2 == NULL)
		{
			pSession->AddToSend(pPackage);
			pPackage = NULL;
		}
		else
		{
			pSession->AddToSend(NewPackage(pPackage));
		}
		pSession = p2;
	}
	m_Sessions.unlock();
	if(pPackage != NULL)
		delete pPackage;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @pSessionΪ���õĻỰָ��
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerBase::SearchRecvPackage(STcpServerSession* &pSession)
{
	unsigned long pos;
	stuSTcpPackage *pPackage;
	m_Sessions.lock();
	STcpServerSession *pSession0 = m_Sessions.FetchFirst(pos);
	while(pSession0)
	{
		pPackage = pSession0->SearchRecvPackage();
		if(pPackage != NULL)
		{
			pSession = pSession0;
			pSession->m_RecvPackage.remove(pPackage);
			m_Sessions.unlock();
			return pPackage;
		}
		pSession0 = m_Sessions.FetchNext(pos);
	}
	m_Sessions.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ������ͷ�ַ����е�act���Լ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @act��ʾ����ı���
//            @pSessionΪ���õĻỰָ��
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerBase::SearchRecvPackage(SString act,STcpServerSession* &pSession)
{
	unsigned long pos;
	stuSTcpPackage *pPackage;
	m_Sessions.lock();
	STcpServerSession *pSession0 = m_Sessions.FetchFirst(pos);
	while(pSession0)
	{
		pPackage = pSession0->SearchRecvPackage(act);
		if(pPackage != NULL)
		{
			pSession = pSession0;
			pSession->m_RecvPackage.remove(pPackage);
			m_Sessions.unlock();
			return pPackage;
		}
		pSession0 = m_Sessions.FetchNext(pos);
	}
	m_Sessions.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ���������ͼ������ն����еı���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 21:32
// ����˵��:  @wFrameType��ʾ
//            @pSessionΪ���õĻỰָ��
// �� �� ֵ:  stuSTcpPackage*
//////////////////////////////////////////////////////////////////////////
stuSTcpPackage* STcpServerBase::SearchRecvPackage(WORD wFrameType,STcpServerSession* &pSession)
{
	unsigned long pos;
	stuSTcpPackage *pPackage;
	m_Sessions.lock();
	STcpServerSession *pSession0 = m_Sessions.FetchFirst(pos);
	while(pSession0)
	{
		pPackage = pSession0->SearchRecvPackage(wFrameType);
		if(pPackage != NULL)
		{
			pSession = pSession0;
			pSession->m_RecvPackage.remove(pPackage);
			m_Sessions.unlock();
			return pPackage;
		}
		pSession0 = m_Sessions.FetchNext(pos);
	}
	m_Sessions.unlock();
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �����֡���߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-8 19:41
// ����˵��:  this
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* STcpServerBase::ThreadListen(void* lp)
{
	STcpServerBase *pThis = (STcpServerBase*)lp;
	pThis->BeginThread();
	SSocket *pS = &pThis->m_Socket;
	SSocket *pNewS = NULL;
	stuSTcpPackage *pPackage;
	int ret;
	SString sHead;

	while(!pThis->IsQuit())
	{
		pS->CreateTcp();
		if( !pS->Bind(pThis->m_iServerPort,pThis->m_sServerIp.length()==0?NULL:pThis->m_sServerIp.data()) || 
			!pS->Listen())
		{
			LOGERROR("�󶨷���˿�(%s:%d)ʱʧ��!",pThis->m_sServerIp.data(),pThis->m_iServerPort);
			for(int i=0;!pThis->IsQuit() && i<100;i++)
				SApi::UsSleep(50000);
			continue;
		}

		while(!pThis->IsQuit())
		{
			SApi::UsSleep(100000);
			if(pNewS != NULL)
			{
				pNewS->Close();
				delete pNewS;
			}
			pNewS = new SSocket();
			if(!pS->Accept(*pNewS))
			{
				LOGERROR("�ڶ˿�(%d)��������ʱʧ��!",pThis->m_iServerPort);
				break;
			}
			LOGWARN("�õ��µĻỰ����!(%s:%d --> %s:%d)",
					pNewS->GetPeerIp().data(),pNewS->GetPeerPort(),
					pThis->m_sServerIp.data(),pThis->m_iServerPort);
			//��֤��¼
			pPackage = NULL;
			pNewS->SetTimeout(1000,5000);
			ret = pThis->RecvFrame(pNewS,pPackage);
			if(ret <= 0 || pPackage == NULL || SString::GetAttributeValue(pPackage->m_sHead,"act") != "login")
			{
				if(pPackage != NULL)
					delete pPackage;
				LOGDEBUG("��֤��¼ʧ�ܣ����ܾ�����!(%s:%d --> %s:%d)",
					pNewS->GetPeerIp().data(),pNewS->GetPeerPort(),
					pThis->m_sServerIp.data(),pThis->m_iServerPort);
				SApi::UsSleep(500000);
				continue;
			}
			sHead = "act=login;result=1;";
			if(!pThis->SendFrame(pNewS,sHead))
			{
				delete pPackage;
				LOGDEBUG("���Ӹյ�¼���Ͽ�!");
				SApi::UsSleep(500000);
				continue;
			}
			if(!pThis->OnLogin(pThis->m_sServerIp.data(),pThis->m_iServerPort,pPackage->m_sHead))
			{
				delete pPackage;
				LOGDEBUG("��¼�����ϲ�ܾ�!");
				SApi::UsSleep(500000);
				continue;
			}
			STcpServerSession *pNewSession = new STcpServerSession();
			WORD startword = pThis->GetStartWord();
			SKT_SWAP_WORD(startword);
			pNewSession->SetStartWord(startword);
			pNewSession->m_pServer = pThis;
			pNewSession->m_pSocket = pNewS;
			pNewSession->m_sLoginAttr = pPackage->m_sHead;
			pThis->m_Sessions.append(pNewSession);
			//�»Ự�ص�
			pThis->OnNewSession(pNewSession);
			SKT_CREATE_THREAD(ThreadService,pNewSession);
			delete pPackage;
			pNewS = NULL;
		}
	}
	pThis->EndThread();
	return NULL;
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ����˻Ự�߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2011-11-10 22:20
// ����˵��:  this
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* STcpServerBase::ThreadService(void* lp)
{
	STcpServerSession *pSession = (STcpServerSession*)lp;
	STcpServerBase *pThis = pSession->m_pServer;

	stuSTcpPackage *pPackage;
	int cnt,ret;
	bool bError = false;
	int more = false;
	SString sHead;
	SSocket *pS = pSession->m_pSocket;
	pThis->BeginThread();
	pS->SetTimeout(1000,5000);
	int last_recv_time = (int)::time(NULL);//�����յ����ݵ�ʱ��
	int last_send_time = (int)::time(NULL);//��������ݵ�ʱ��
	int no_recv=0;//û���յ����ݵĴ���
	int no_send=0;//û�з������ݵĴ���
	while(!pThis->IsQuit())
	{
		more = false;
		cnt = 0;
		//�Ƿ�����Ҫ���͵�����
		pPackage = pSession->m_SendPackage[0];
		if(pPackage == NULL)
		{
			if(++no_send > 100)
			{
				no_send = 0;
				if(abs((int)::time(NULL)-last_send_time) >= 10)
				{
					//10��û�����ݷ��ͣ���һ������֡
					sHead = "act=echo;";
					pSession->lock();
					if(!((STcpCommBase*)pSession)->SendFrame(pS,sHead))
					{
						pSession->unlock();
						LOGERROR("������������ʧ��!");
						bError = true;
						break;
					}
					pSession->unlock();
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
			pSession->lock();
			if(!((STcpCommBase*)pSession)->SendFrame(pS,pPackage))
			{
				pSession->unlock();
				LOGERROR("���ͱ���ʧ��!");
				bError = true;
				break;
			}
			pSession->unlock();
			pSession->m_SendPackage.remove(pPackage);
			pPackage = pSession->m_SendPackage[0];
			if(cnt ++ > 100)
			{
				if(pPackage != NULL)
					more = true;
				last_send_time = (int)::time(NULL);//���·���ʱ��
				break;
			}
			if(pPackage == NULL)
				last_send_time = (int)::time(NULL);//���·���ʱ��
		}
		if(bError)
			break;

		//���ձ���
		pS->SetTimeout(10,5000);
		pSession->lock();
		ret = pS->CheckForRecv();
		pSession->unlock();
		pS->SetTimeout(1000,5000);
		if(ret < 0)
		{
			//�쳣
			break;
		}
		else if(ret == 0)
		{
			//û�����ݵ���
			if(++no_recv > 100)
			{
				no_recv = 0;
				if(abs((int)::time(NULL)-last_recv_time) > 30)
				{
					LOGWARN("30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����!");
					break;//30����û�����ݵ���ʱ����Ϊ�����쳣���Ͽ�����
				}
			}
		}
		else if(ret == 1)
		{
			//���µ����ݵ���
			no_recv = 0;
			pPackage = NULL;
			pSession->lock();
			ret = pThis->RecvFrame(pS,pPackage);
			if(ret == 0)
			{
				//û������
			}
			else if(ret < 0)
			{
				//�쳣
				pSession->unlock();
				LOGERROR("���ձ���ʱʧ��! ret=%d",ret);
				break;
			}
			else
			{
				if(pPackage != NULL)
				{
					//pSession->lock();
					if(pPackage->m_wFrameType == 0 && SString::GetAttributeValue(pPackage->m_sHead,"act")=="echo")
					{
						delete pPackage;//0������Ϊ����������
						pPackage = NULL;
					}
					else
					{
						if(!pThis->OnRecvFrame(pSession,pPackage))
						{
							pSession->m_RecvPackage.append(pPackage);
							if(pSession->m_RecvPackage.count()>10000)
								pSession->m_RecvPackage.remove(0);
							pPackage = NULL;
						}
						else
						{
							if(pPackage->m_bAutoDelete)
							{
								delete pPackage;
								pPackage = NULL;
							}
						}
					}
					//pSession->unlock();
					last_recv_time = (int)::time(NULL);//���½���ʱ��
				}
			}
			pSession->unlock();
			if(pPackage != NULL)
			{
				delete pPackage;
				pPackage = NULL;
			}
			more = true;
		}
		if(!more)
			SApi::UsSleep(10000);
	}
	pS->Close();
	pThis->OnDeleteSession(pSession);
	SApi::UsSleep(1000000);//��ʱһ�����ɾ��
	pThis->EndThread();
	pThis->m_Sessions.remove(pSession);
	return NULL;
}
