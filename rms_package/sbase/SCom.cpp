/**
 *
 * 文 件 名 : SCom.cpp
 * 创建日期 : 2007-01-01
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 串口通迅类
 * 修改记录 : 
 *            $Log: SCom.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/27 02:58:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/21 06:42:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/08 12:07:26  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/06 00:35:32  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:06  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.3  2008/12/03 15:00:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/12/03 14:55:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/01/24 07:01:19  shaokt
 *            LINUX下串口接收参数XON、XOFF的过滤
 *
 *            Revision 1.2  2007/12/10 06:40:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/11/19 03:48:18  shaokt
 *            串口通迅类
 *
 *
 **/

#include "SCom.h"

////////////////////////////////////////////////////////////////////////////
/// SCom

SCom::SCom()
{
	m_bOpened = false;
	m_hCom = 0;
	SetTimeOut(5000,5000);//默认5秒超时
#ifdef WIN32
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
#endif
}

SCom::~SCom()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打开串口设备
// 作    者:  邵凯田
// 创建时间:  2007-11-6
// 参数说明:  szComDev串口设备名:win:com1/com2... linux:/dev/ttyS0;/dev/ttyS1...
//						iBaudrate特殊率,default=9600
//						cParity校验方式：N无校验,E偶校验,O奇校验,S停止校验
//						iDataBits数据位长度，default=8
//						fStopBits停止位长度,default=1
// 返 回 值:  true/false
//////////////////////////////////////////////////////////////////////////
bool SCom::Open(char *szComDev,int iBaudrate/*=9600*/,char cParity/*='N'*/,
								 int iDataBits/*=8*/,float fStopBits/*=1*/,int iFlowControl/*=0*/)
{
	LOGBASEDEBUG("打开串口(%s,%d,%C,%d,%f,%d)",szComDev,iBaudrate,cParity,iDataBits,fStopBits,iFlowControl);
	if(m_bOpened)
	{
		LOGWARN("串口'%s'已经打开!",szComDev);
		return false;
	}
#ifdef WIN32
	DCB dcb;
//  if(nPort < 10)
//		wsprintf( szPort, "COM%d", nPort );
//	else
//		wsprintf( szPort, "\\\\.\\COM%d", nPort );
	DWORD DD=OPEN_EXISTING;
	m_hCom = CreateFile( szComDev, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if( m_hCom == NULL ) return( FALSE );

	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( m_hCom, &CommTimeOuts );

	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	dcb.DCBlength = sizeof( DCB );
	GetCommState( m_hCom, &dcb );
	//9600,n,8,1
	//波特率
	dcb.fBinary=TRUE;
	dcb.BaudRate = iBaudrate;
	//parity=奇偶效验;//0-4=no,odd,even,mark,space
	dcb.fParity=TRUE;
	switch(cParity)
	{
		case 'N':
		case 'n':
			dcb.Parity=NOPARITY;
			dcb.fParity=FALSE;
			break;
		case 'O':
		case 'o':
			dcb.Parity=ODDPARITY;
			break;
		case 'E':
		case 'e':
			dcb.Parity=EVENPARITY;
			break;
		case 'M':
		case 'm':
			dcb.Parity=MARKPARITY;
			break;
		case 'S':
		case 's':
			dcb.Parity=SPACEPARITY;
			break;
	}
	//数据位
	dcb.ByteSize = iDataBits;
	//停止位
	if(fStopBits == 1)
		dcb.StopBits=ONESTOPBIT;
	if(fStopBits == 1.5)
		dcb.StopBits=ONE5STOPBITS;
	if(fStopBits == 2)
		dcb.StopBits=TWOSTOPBITS;
	//流量控制flow  0无　1硬件　2XONXOFF 3RTS and XonXoff
//	dcb.fOutxCtsFlow=TRUE;
//	dcb.fOutxDsrFlow=TRUE;
//	dcb.fTXContinueOnXoff=FALSE;
//	dcb.fDtrControl=DTR_CONTROL_ENABLE;
	switch(iFlowControl)
	{
		case 0:
			dcb.fRtsControl=RTS_CONTROL_DISABLE;
			dcb.fOutX=FALSE;
			dcb.fInX=FALSE;
			break;
		case 1:
			dcb.fRtsControl=RTS_CONTROL_ENABLE;
			dcb.fOutX=FALSE;
			dcb.fInX=FALSE;
			break;
		case 2:
			dcb.fRtsControl=RTS_CONTROL_DISABLE;
			dcb.fOutX=TRUE;
			dcb.fInX=TRUE;
			break;
		case 3:
			dcb.fRtsControl=RTS_CONTROL_ENABLE;
			dcb.fOutX=TRUE;
			dcb.fInX=TRUE;
			break;
	}

	unsigned char ucSet;
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );
	if( !SetCommState( m_hCom, &dcb ) ||
		!SetupComm( m_hCom, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL )
	{
		DWORD dwError = GetLastError();
		if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
		if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
		CloseHandle( m_hCom );
		m_hCom = 0;
		return( FALSE );
	}

#else
	//linux/unix
	//m_hCom = open(szComDev,O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);//
	m_hCom = open(szComDev,O_RDWR | O_NONBLOCK | O_NOCTTY);
	if(m_hCom == -1)
	{
		LOGERROR("打开串口设备'%s'失败!",szComDev);
		return false;
	}
	LOGDEBUG("SCom(%s) open=%d",szComDev,m_hCom);

	//设置波特率
	int speed_arr[] = {B115200,B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300,
				B38400, B19200, B9600, B4800, B2400, B1200, B300, };
	int name_arr[] = {115200,57600,38400,  19200,  9600,  4800,  2400,  1200,  300,
				38400,  19200,  9600, 4800, 2400, 1200,  300, };
  int i;
  int status;
  struct termios Opt;
  tcgetattr(m_hCom, &Opt);
	for( i= 0; i < (int)(sizeof(speed_arr) / sizeof(int));  i++)
	{
		if(iBaudrate == name_arr[i])
		{
			tcflush(m_hCom, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(m_hCom, TCSANOW, &Opt);
			if(status != 0)
			{
				LOGERROR("m_hCom=%d,串口设置错误!串口打开操作失败!",m_hCom);
				Close();
				return false;
				//perror("tcsetattr fd1");
			}
			break;
		}
		tcflush(m_hCom,TCIOFLUSH);
	}

	//设置校验、数据位、停止位
	struct termios options;
	if( tcgetattr( m_hCom,&options)  !=  0)
	{
		LOGERROR("tcgetattr return != 0");
		//perror("SetupSerial 1");
		Close();
		return false;
	}
	options.c_cflag &= ~CSIZE;
	//设置输入、输出的初始属性
	options.c_iflag = 0;
	options.c_oflag = 0;

	switch (iDataBits) /*设置数据位数*/
	{
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		LOGERROR("不支持的数据位大小:%d",iDataBits);
		Close();
		return false;
	}
	switch (cParity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);  /* 设置为奇效验*/ 
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* 转换为偶效验*/  
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		LOGERROR("不支持的校验方式:%C",cParity);
		Close();
		return false;
	}
	/* 设置停止位*/   
	if(fStopBits == 1)
		options.c_cflag &= ~CSTOPB;
	else if(fStopBits == 2)
		options.c_cflag |= CSTOPB;
	else
	{
		LOGERROR("不支持的停止位:%f",fStopBits);
		Close();
		return false;
	}

	/* Set input parity option */
	if (cParity != 'n')
		options.c_iflag |= INPCK;
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;
	
	//如果不是开发终端之类的，只是串口传输数据，而不需要串口来处理，那么使用原始模式(Raw Mode)方式来通讯，设置方式如下： 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/

	//Raw 模式输出. 
	//options.c_oflag = 0; 
	//ICANON  : 致能标准输入, 使所有回应机能停用, 并不送出信号以叫用程式 
	//options.c_lflag = ICANON;
	tcflush(m_hCom,TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(m_hCom,TCSANOW,&options) != 0)
	{
		LOGERROR("tcsetattr return != 0");
		Close();
		return false;
	}

#endif

	m_bOpened = true;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  关闭串口设备
// 作    者:  邵凯田
// 创建时间:  2007-11-7
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SCom::Close()
{
	LOGBASEDEBUG("关闭串口,handle=%d",m_hCom);
	if(!m_bOpened && m_hCom <= 0)
	{
		//LOGWARN("串口设备尚未打开，忽略关闭!");
		return false;
	}
#ifdef WIN32
	if( m_OverlappedRead.hEvent != NULL ) 
		CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL ) 
		CloseHandle( m_OverlappedWrite.hEvent );
	CloseHandle(m_hCom);
#else
	//linux/unix
	close(m_hCom);
#endif
	m_hCom = 0;
	m_bOpened = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  接收指定长度的数据
// 作    者:  邵凯田
// 创建时间:  2007-11-7
// 参数说明:  缓冲区及最大接收长度
// 返 回 值:  实际接收到的数据<0表示失败
//////////////////////////////////////////////////////////////////////////
int SCom::Receive(BYTE *pBuffer,int iNeedBytes)
{
	LOGBASEDEBUG("Into SCom::Receive(%p,%d)",pBuffer,iNeedBytes);
	int iRet = 0;
	int iTotal = 0,iLeftBytes = iNeedBytes;
	if(!m_bOpened || m_hCom == 0)
	{
		LOGWARN("串口设备未打开!接收数据失败!");
		return -1;
	}
	SDateTime first = SDateTime::currentDateTime();
begin:
	iRet = 0;
#ifdef WIN32
	BOOL bReadStatus;
	unsigned long dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;
	ClearCommError( m_hCom, &dwErrorFlags, &ComStat );
	if(ComStat.cbInQue)
	{
		dwBytesRead = (DWORD) ComStat.cbInQue;
		if( iLeftBytes < (int) dwBytesRead )
			dwBytesRead = (DWORD) (iLeftBytes);
		bReadStatus = ReadFile( m_hCom, pBuffer+iTotal, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
		if( !bReadStatus )
		{
			if( GetLastError() == ERROR_IO_PENDING )
			{
				WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
				iRet = (int) dwBytesRead;
			}
			return( 0 );
		}
		else
			iRet = (int) dwBytesRead;
	}
#else
	//linux/unix
	iRet = read(m_hCom,pBuffer+iTotal,iLeftBytes);
	if(iRet < 0)
	{
		iRet = 0;
	}
#endif
	if(iRet < 0)
		return -1;
	//if(iRet > 0)
	//	return iRet;
	iTotal += iRet;
	SDateTime now = SDateTime::currentDateTime();
	//考虑到时间可能被向后修改
	if(abs(first.secsTo(now)) > (m_iRecvTimeout<1000?1:(m_iRecvTimeout/1000)) * 2)
		first = now;
	if(first.secsTo(now)*1000+(now.msec()-first.msec()) > m_iRecvTimeout)
		goto end;//超时退出
	if(iTotal < iNeedBytes)
	{
		iLeftBytes = iNeedBytes - iTotal;
		SApi::UsSleep(1000);
		goto begin;
	}
end:
	LOGBASEDEBUG("SCom::Receive return=%d",iTotal);
	return iTotal;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  发送指定缓冲区指定大小的数据
// 作    者:  邵凯田
// 创建时间:  2007-11-7
// 参数说明:  缓冲区及发送长度
// 返 回 值:  实际发出的长度
//////////////////////////////////////////////////////////////////////////
int SCom::Send(BYTE *pBuffer,int iSendBytes)
{
	LOGBASEDEBUG("Into SCom::Send(%p,%d)",pBuffer,iSendBytes);
	int iRet = 0;
#ifdef WIN32
	BOOL bWriteStat;
	unsigned long dwBytesWritten;

	bWriteStat = WriteFile( m_hCom, pBuffer, iSendBytes, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) )
	{
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) 
			dwBytesWritten = 0;
		else
		{
			GetOverlappedResult( m_hCom, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
		}
	}

	iRet = dwBytesWritten;
#else
	//linux/unix
	iRet = write(m_hCom,pBuffer,iSendBytes);
#endif

	LOGBASEDEBUG("SCom::Receive return=%d",iRet);
	return iRet;
}
