/**
 *
 * 文 件 名 : SInclude.h
 * 创建日期 : 2007-07-01
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 公共头文件
 * 修改记录 : 
 *            $Log: SInclude.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:07  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2008/11/08 11:47:15  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:01  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.3  2007/09/28 11:33:13  shaokt
 *            增加对5.0消息队列的支持，信息定置与发送
 *
 *            Revision 1.2  2007/09/11 08:17:52  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef __SKTINCLUDE_H__

#define __SKTINCLUDE_H__


#ifdef WIN32
	//#include <afx.h>
	//#include <afxwin.h>         // MFC core and standard components
	//#include <afxext.h>         // MFC extensions
	//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
	//#pragma warning(disable: 4786)

	#define WIN32_LEAN_AND_MEAN
	//#include <windows.h>
	#include <locale.h>
	#include <string.h>
	#include <time.h>		/* timespec{} for pselect() */
	#include <stdio.h>
	#include <iostream>
	#include <fstream>
	#include <process.h>//for thread
	#include <fcntl.h>		/* for nonblocking */
#else
	#include <stdlib.h>
	#include <ctype.h>
	#include <fcntl.h>		/* for nonblocking */
	#include <signal.h>
	#include <signal.h>
	//#include <varargs.h>
	//#include <stdarg.h>
	#include <execinfo.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <langinfo.h>
	#include <pthread.h>
	#include <sys/ioctl.h>
	#include <sys/socket.h>	/* basic socket definitions */
	#include <sys/time.h>	/* timeval{} for select() */
	#include <sys/stat.h>	/* for S_xxx file mode constants */
	#include <sys/uio.h>		/* for iovec{} and readv/writev */
	#include <sys/wait.h>
	#include <sys/un.h>		/* for Unix domain sockets */
	#include <sys/sem.h>
	#include <semaphore.h>
	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/msg.h>
	#include <sys/shm.h>
	#include <sys/mman.h>
	#include <sys/wait.h>


	#include <linux/kernel.h>
//#ifndef _ARM
//	#include <linux/ext2_fs.h>
//#endif
	#include <netinet/in.h>	/* sockaddr_in{} and other Internet defns */
	#include <arpa/inet.h>	/* inet(3) functions */
#endif

#include "SLog.h"
#include "SSocket.h"
#include "SString.h"
#include "SDateTime.h"
#include "SList.h"
#include "SLock.h"
#include "SFile.h"
#include "SBaseConfig.h"
#include "SXmlConfig.h"
#include "SLock.h"
#include "SObject.h"
#include "SApi.h"
#include "SRing.h"
#include "SBuffer.h"
#include "SCom.h"


#endif//__SKTINCLUDE_H__

