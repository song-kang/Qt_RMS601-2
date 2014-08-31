/**
 *
 * 文 件 名 : EscPrintUsb.cpp
 * 创建日期 : 2012-10-17 13:41
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : ESC(EPSON Standard Code for Printer)指令集基于USB口的打印类
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2012-10-17	邵凯田　创建文件
 * 002  2014-02-21  邵凯田  增加串口打印支持
 *
 **/

#include "SEscPrintUsb.h"

int g_printer_id[]={
	//vid,pid//对应制造商ID，产品ID
	0x00,0x00,//由传入参数决定，用于指定打印机型号
	0x1bA0,0x1504,//映美380
	0x04B8,0x0005//EPSON LQ300K+II
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SEscPrintUsb
// 作    者:  邵凯田
// 创建时间:  2012-10-17 13:20
// 描    述:  ESE指令集打印基类
//////////////////////////////////////////////////////////////////////////
SEscPrintUsb::SEscPrintUsb()
{
	m_pUsbDev = NULL;
	m_iVendorId = 0x1BA0;
	m_iProductId = 0x1504;
	m_iPortOut = 0x01;
	m_iPortIn = 0x82;
}

SEscPrintUsb::~SEscPrintUsb()
{

}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印初始化
// 作    者:  邵凯田
// 创建时间:  2012-10-17 13:46
// 参数说明:  @sAttr为扩展属性，不同实现类提供不同的参数
// 返 回 值:  true表示初始化成功,false表示初始化失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintUsb::Init(SString sAttr)
{
    usb_init();
// 	usb_set_debug(1);
    usb_find_busses();
    usb_find_devices();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打开打印机
// 作    者:  邵凯田
// 创建时间:  2012-10-17 13:23
// 参数说明:  @sAttr为打印机参数;如：vid=1234;pid=5678;pout=1;pin=129;
// 返 回 值:  true打开成功，false打开失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintUsb::Open(SString sAttr)
{
    struct usb_bus     *bus;
    struct usb_device  *dev;
	LOGDEBUG("打开USB打印机:%s",sAttr.data());
	if(SString::IsAttribute(sAttr,"vid"))
		m_iVendorId = (WORD)SString::GetAttributeValueI(sAttr,"vid");
	if(SString::IsAttribute(sAttr,"pid"))
		m_iProductId = (WORD)SString::GetAttributeValueI(sAttr,"pid");
	if(SString::IsAttribute(sAttr,"pout"))
		m_iPortOut = (WORD)SString::GetAttributeValueI(sAttr,"pout");
	if(SString::IsAttribute(sAttr,"pin"))
		m_iPortIn = (WORD)SString::GetAttributeValueI(sAttr,"pin");
	if(m_pUsbDev != NULL)
	{
		LOGWARN("打印机已经打开!");
		return false;
	}
	m_pUsbDev = NULL;
	//由传入型号改为预置几种支持的设备型号,逐一尝试打开直到成功为止,将传入型号作为第一次匹配型号
	g_printer_id[0] = m_iVendorId;
	g_printer_id[1] = m_iProductId;
	int drvs = sizeof(g_printer_id)/(sizeof(int)*2);
	for(bus = usb_get_busses(); bus != NULL && m_pUsbDev==NULL; bus = bus->next)
	{
		for(dev = bus->devices; dev != NULL /*&& m_pUsbDev==NULL*/; dev = dev->next)
		{
			LOGDEBUG("VID=%X; PID=%X", dev->descriptor.idVendor, dev->descriptor.idProduct);
			for(int drv=0;drv<drvs && m_pUsbDev==NULL;drv++)
			{
				m_iVendorId = g_printer_id[drv*2];
				m_iProductId = g_printer_id[drv*2+1];
				if(dev->descriptor.idVendor == m_iVendorId && dev->descriptor.idProduct == m_iProductId)
				{
					m_pUsbDev = usb_open(dev);
					if(m_pUsbDev == NULL)
					{
						LOGERROR("打开USB打印机失败!%s",usb_strerror());
						return false;
					}
					break;
				}
			}
		}
	}
	if(m_pUsbDev == NULL)
	{
		//LOGWARN("未找到指定型号的USB打印机[VID:%04X,PID:%04X]!",m_iVendorId,m_iProductId);
		LOGWARN("未找到指定支持的USB打印机!将通过串口1打印");
#ifndef WIN32
		if(!m_Com.Open("dev=/dev/ttyS0"))
		{
			LOGWARN("打开串口1失败!");
			return false;
		}
#else
		if(!m_Com.Open("dev=com4"))
		{
			LOGWARN("打开串口1失败!");
			return false;
		}
#endif
		return true;
	}
#ifndef WIN32
	usb_detach_kernel_driver_np(m_pUsbDev, 0);	// for Linux Only
#endif
	if(usb_set_configuration(m_pUsbDev, 1) < 0)
    {
        LOGERROR("usb_set_configuration错误#%d: %s\n", 1, usb_strerror());
		Close();
        return false;
    }
    if(usb_claim_interface(m_pUsbDev, 0) < 0)
    {
        LOGERROR("usb_claim_interface错误#%d:\n%s\n", 0, usb_strerror());
        Close();
        return false;
    }
	if(usb_clear_halt(m_pUsbDev, m_iPortOut) < 0)
    {
        LOGERROR("usb_clear_halt错误#%d:\n%s\n", 0, usb_strerror());
        Close();
        return false;
    }


	if(SEscPrint::Open(sAttr))
	{
		LOGDEBUG("USB打印机打开成功!");
		return true;
	}
	else
	{
		LOGDEBUG("USB打印机打开失败!");
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  关闭打印机
// 作    者:  邵凯田
// 创建时间:  2012-10-17 13:26
// 参数说明:  void
// 返 回 值:  true关闭成功，false关闭失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintUsb::Close()
{
	m_Com.Close();
	if(m_pUsbDev == NULL)
		return false;
	LOGDEBUG("关闭USB打印机!");
    usb_release_interface(m_pUsbDev, 0);
	usb_close(m_pUsbDev);
	m_pUsbDev = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  向打印机写数据
// 作    者:  邵凯田
// 创建时间:  2012-10-17 13:31
// 参数说明:  @pBuffer待写入内容缓冲区指针
//            @iLen为缓冲区字节长度
// 返 回 值:  >0表示实际发送字节数，<=0表示失败
//////////////////////////////////////////////////////////////////////////
int SEscPrintUsb::Write(BYTE *pBuffer,int iLen)
{
	if(m_Com.IsOpened())
		return m_Com.Write(pBuffer,iLen);
	if(m_pUsbDev == NULL)
		return -1;
    int send_len;
    send_len = usb_bulk_write(m_pUsbDev, m_iPortOut, (char*)pBuffer, iLen, USB_TIMEOUT);
	//LOGDEBUG("WRITE(len:%d):%s",send_len,SString::HexToStr(pBuffer,iLen).data());
    if (send_len < 0)
    {
		LOGERROR("向USB打印写数据失败%d:%s",send_len,usb_strerror());
		return -2;
    }

	return send_len;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  从打印机读取数据
// 作    者:  邵凯田
// 创建时间:  2012-10-17 13:35
// 参数说明:  @pBuffer为待读入缓冲区指针
//            @iLen为期望读取字节长度
// 返 回 值:  >0表示实际读取字节数，<0表示读取失败，=0表示读取超时
//////////////////////////////////////////////////////////////////////////
int SEscPrintUsb::Read(BYTE *pBuffer,int iLen)
{
	if(m_Com.IsOpened())
		return m_Com.Read(pBuffer,iLen);
	int recv_len;
    recv_len = usb_bulk_read(m_pUsbDev, m_iPortIn, (char*)pBuffer, iLen, USB_TIMEOUT);
    if (recv_len < 0)
    {
		LOGERROR("向USB打印写数据失败%d:%s",recv_len,usb_strerror());
		return -2;
    }

	return recv_len;
}



//////////////////////////////////////////////////////////////////////////

SEscPrintCom::SEscPrintCom()
{

}

SEscPrintCom::~SEscPrintCom()
{
	m_Com.Close();
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打印初始化
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:46
// 参数说明:  @sAttr为扩展属性，不同实现类提供不同的参数
// 返 回 值:  true表示初始化成功,false表示初始化失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Init(SString sAttr)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  打开打印机
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:23
// 参数说明:  @sAttr为打印机参数
// 返 回 值:  true打开成功，false打开失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Open(SString sAttr)
{
	SString dev = SString::GetAttributeValue(sAttr,"dev");
	int baud = SString::GetAttributeValueI(sAttr,"baud");
	if(baud == 0)
		baud = 19200;
	SString chk = SString::GetAttributeValue(sAttr,"chk");
	if(chk.length() == 0)
		chk = "N";
	if(!m_Com.Open(dev.data(),baud,chk.at(0)))
	{
		LOGERROR("打开串口时失败!attr=%s",sAttr.data());
		return false;
	}

	if(SEscPrint::Open(sAttr))
	{
		LOGDEBUG("串口打印机打开成功!");
		return true;
	}
	else
	{
		LOGDEBUG("串口打印机打开失败!");
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  关闭打印机
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:26
// 参数说明:  void
// 返 回 值:  true关闭成功，false关闭失败
//////////////////////////////////////////////////////////////////////////
bool SEscPrintCom::Close()
{
	m_Com.Close();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  向打印机写数据
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:31
// 参数说明:  @pBuffer待写入内容缓冲区指针
//            @iLen为缓冲区字节长度
// 返 回 值:  >0表示实际发送字节数，<=0表示失败
//////////////////////////////////////////////////////////////////////////
int SEscPrintCom::Write(BYTE *pBuffer,int iLen)
{
	if(m_Com.GetHandle() == 0)
		return -1;
	int send_len = m_Com.Send(pBuffer,iLen);
	LOGDEBUG("SEND(len=%d): %s",iLen,SString::HexToStr(pBuffer,iLen).data());
	LOGDEBUG("SEND(len=%d): %s",iLen,SString::toString((char*)pBuffer,iLen).data());
	if (send_len < 0)
	{
		LOGERROR("向串口打印写数据失败%d:%s",send_len,usb_strerror());
		return -2;
	}
#ifdef WIN32
	//	Sleep(10);
#else
	if(iLen == 1)
		usleep(100000);
	else if(iLen > 2000)
		usleep(300000);
	else 
		usleep(200000);
#endif


	return send_len;
}

//////////////////////////////////////////////////////////////////////////
// 描    述:  从打印机读取数据
// 作    者:  邵凯田
// 创建时间:  2014:2:21 13:35
// 参数说明:  @pBuffer为待读入缓冲区指针
//            @iLen为期望读取字节长度
// 返 回 值:  >0表示实际读取字节数，<0表示读取失败，=0表示读取超时
//////////////////////////////////////////////////////////////////////////
int SEscPrintCom::Read(BYTE *pBuffer,int iLen)
{
	int recv_len;
	recv_len = m_Com.Receive(pBuffer, iLen);
	if (recv_len < 0)
	{
		LOGERROR("从串口打印读数据失败%d:%s",recv_len,usb_strerror());
		return -2;
	}

	return recv_len;
}





