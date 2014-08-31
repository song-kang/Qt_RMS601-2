/**
 *
 * 文 件 名 : SCom.h
 * 创建日期 : 2007-11-06
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 串口通迅类
 * 修改记录 : 
 *            $Log: SCom.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:06  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:07  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2008/09/11 09:47:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/11/19 03:48:18  shaokt
 *            串口通迅类
 *
 *
 **/

#ifndef __SKTCOM_H_
#define __SKTCOM_H_

#ifndef WIN32
	#include     <stdio.h>      /*标准输入输出定义*/
	#include     <stdlib.h>     /*标准函数库定义*/
	#include     <unistd.h>     /*Unix标准函数定义*/
	#include     <sys/types.h>  /**/
	#include     <sys/stat.h>   /**/
	#include     <fcntl.h>      /*文件控制定义*/
	#include     <termios.h>    /*PPSIX终端控制定义*/
	#include     <errno.h>      /*错误号定义*/
#else
	#define FC_DTRDSR       0x01
	#define FC_RTSCTS       0x02
	#define FC_XONXOFF      0x04
	#define ASCII_BEL       0x07
	#define ASCII_BS        0x08
	#define ASCII_LF        0x0A
	#define ASCII_CR        0x0D
	#define ASCII_XON       0x11
	#define ASCII_XOFF      0x13
#endif

#include "SApi.h"
#include "SLog.h"
#include "SDateTime.h"
class SCom
{
public:
	SCom();
	virtual ~SCom();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  打开串口设备
	// 作    者:  邵凯田
	// 创建时间:  2007-11-6
	// 参数说明:  szComDev串口设备名:win:com1/com2... linux:/dev/ttyS0;/dev/ttyS1...
	//						iBaudrate特殊率,default=9600
	//						cParity校验方式：N无校验,E偶校验,O奇校验,S停止校验
	//						iDataBits数据位长度，default=8
	//						iStopBits停止位长度,default=1
	//						iFlowControl流量控制,default=0,0:无;1:硬件;2:XONXOFF;3:RTS and XonXoff
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Open(char *szComDev,int iBaudrate=9600,char cParity='N',
		int iDataBits=8,float fStopBits=1,int iFlowControl=0);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭串口设备
	// 作    者:  邵凯田
	// 创建时间:  2007-11-7
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool Close();
	
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  接收指定长度的数据
	// 作    者:  邵凯田
	// 创建时间:  2007-11-7
	// 参数说明:  缓冲区及最大接收长度
	// 返 回 值:  实际接收到的数据<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int Receive(BYTE *pBuffer,int iNeedBytes);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  发送指定缓冲区指定大小的数据
	// 作    者:  邵凯田
	// 创建时间:  2007-11-7
	// 参数说明:  缓冲区及发送长度
	// 返 回 值:  实际发出的长度
	//////////////////////////////////////////////////////////////////////////
	int Send(BYTE *pBufer,int iSendBytes);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置超时时间
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 15:04
	// 参数说明:  @iRecvTimeout表示接收超时时间，单位毫秒
	//            @iSendTimeout表示发送超时时间，单位毫秒
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline void SetTimeOut(int iRecvTimeout,int iSendTimeout)
	{
		m_iSendTimeout = iSendTimeout;
		m_iRecvTimeout = iRecvTimeout;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取发送超时时间
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 15:05
	// 参数说明:  void
	// 返 回 值:  发送超时时间，单位毫秒
	//////////////////////////////////////////////////////////////////////////
	inline int GetSendTimeout(){return m_iSendTimeout;}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取接收超时时间
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 15:05
	// 参数说明:  void
	// 返 回 值:  接收超时时间，单位毫秒
	//////////////////////////////////////////////////////////////////////////
	inline int GetRecvTimeout(){return m_iRecvTimeout;}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前串口操作句柄
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 15:05
	// 参数说明:  void
	// 返 回 值:  句柄号
	//////////////////////////////////////////////////////////////////////////
	inline DWORD GetHandle()   {return (DWORD)m_hCom;}

private:
#ifdef WIN32
	HANDLE m_hCom;
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
#else
	int m_hCom;
#endif
	int m_iSendTimeout,m_iRecvTimeout;//发送和接收的超时，单位毫秒
	bool m_bOpened;
};

#endif /*__SKTCOM_H_*/
