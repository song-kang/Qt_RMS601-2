/**
 *
 * 文 件 名 : SService.cpp
 * 创建日期 : 2011-11-4 13:48
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/

#include "SService.h"

SService::SService()
{
	m_iRunThreads = 0;
	m_bQuit = false;
	m_bRuning = false;
}

SService::~SService()
{
	if(m_bRuning || m_iRunThreads > 0)
	{
		StopAndWait();
	}
}


////////////////////////////////////////////////////////////////////////
// 描    述:  启动服务,虚函数，派生函数中必须先调用此函数
// 作    者:  邵凯田
// 创建时间:  2011-11-4 13:51
// 参数说明:  void
// 返 回 值:  true表示服务启动成功,false表示服务启动失败
//////////////////////////////////////////////////////////////////////////
bool SService::Start()
{
	IsStoped();
	if(m_bRuning)
	{
		//LOGERROR("服务已经处于运行状态，不能再次启动!");
		return false;
	}
	m_bQuit = false;
	m_iRunThreads = 0;
	m_bRuning = true;
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  停止服务，派生函数中必须先调用此函数
// 作    者:  邵凯田
// 创建时间:  2011-11-4 13:52
// 参数说明:  void
// 返 回 值:  true表示服务停止成功,false表示服务停止失败
//////////////////////////////////////////////////////////////////////////
bool SService::Stop()
{
	if(!m_bRuning)
	{
		m_bQuit = true;
		//LOGWARN("服务尚未运行，不必停止!");
		return false;
	}
	m_bQuit = true;
	return true;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  停止服务，同时在给定的超时时间内等待服务完全结束
// 作    者:  邵凯田
// 创建时间:  2011-11-4 14:02
// 参数说明:  @iTimeout表示超时秒数，0表示不超时(一直阻塞下去)
// 返 回 值:  true表示停止成功,false表示停止失败
//////////////////////////////////////////////////////////////////////////
bool SService::StopAndWait(int iTimeout/*=0*/)
{
	Stop();
	iTimeout *= 20;
	int times = 0;
	while(!IsStoped())
	{
		times++;
		if(iTimeout > 0 && times > iTimeout)
		{
			m_bRuning = false;
			LOGERROR("停止服务超时!");
			return false;
		}
		SApi::UsSleep(10000);
	}
	m_bRuning = false;
	return true;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  开始一个运行线程
// 作    者:  邵凯田
// 创建时间:  2011-11-7 16:15
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SService::BeginThread()
{
	#ifndef WIN32
		pthread_detach(pthread_self());
	#endif
	m_iRunThreads++;
};

////////////////////////////////////////////////////////////////////////
// 描    述:  结束一个运行线程
// 作    者:  邵凯田
// 创建时间:  2011-11-7 16:15
// 参数说明:  void
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SService::EndThread()
{
	m_iRunThreads--;
	if(m_iRunThreads < 0)
		m_iRunThreads = 0;
};
