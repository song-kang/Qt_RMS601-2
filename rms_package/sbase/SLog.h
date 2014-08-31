/**
 *
 * 文 件 名 : SLog.h
 * 创建日期 : 2007-01-01
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 日志记录的信息元素有：
 *						1 模块名
 *						2 日志类型：1:debug/2:warn/3:error/4:fault
 *						3 发生时间
 *						4 源文件名
 *						5 源文件行号
 *						6 日志内容描述
 * 修改记录 : 
 *            $Log: SLog.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/29 08:53:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/20 14:00:54  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2009/06/19 10:35:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/02 01:50:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/19 06:18:33  shaokt
 *            限制日志文件大小
 *
 *            Revision 1.3  2007/09/11 08:17:53  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/09/04 01:43:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef SKTLOGGER_H_
#define SKTLOGGER_H_

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 
	#include <stdio.h> 
	#include <stdlib.h> 
	#include <winsock2.h> 
	#pragma comment(lib,"ws2_32.lib")
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <sys/select.h>
	#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifdef _POWERPC
#ifndef DWORD
#define DWORD unsigned long
#endif
#define LOG_SWAP_DWORD(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#else
#define LOG_SWAP_DWORD(dw)
#endif

using namespace std;

#ifdef WIN32
#ifndef __FUNCTION__
#define __FUNCTION__ ""
#endif
#endif

#ifdef _SKT_WITHOUT_BASEDEBUG
	#define LOGBASEDEBUG 
#else
	#define LOGBASEDEBUG SLog::getLog()->logHead(0,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
#endif
#ifdef _SKT_WITHOUT_DEBUG
	#define LOGDEBUG 
#else
	#define LOGDEBUG SLog::getLog()->logHead(1,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
#endif
#ifdef _SKT_WITHOUT_WARN
	#define LOGWARN  
#else
	#define LOGWARN  SLog::getLog()->logHead(2,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
#endif
#ifdef _SKT_WITHOUT_ERROR
	#define LOGERROR 
#else
	#define LOGERROR SLog::getLog()->logHead(3,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
#endif
#ifdef _SKT_WITHOUT_FAULT
	#define LOGFAULT 
#else
	#define LOGFAULT SLog::getLog()->logHead(4,__FILE__,__LINE__,__FUNCTION__);SLog::getLog()->logBody
#endif

#include "SLock.h"

#ifndef SOCKET 
#define SOCKET int
#endif

#ifndef NULL 
#define NULL 0
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#define MAX_PACKAGE_SIZE 1020


#pragma pack(push)
#pragma pack(1)
class SLog  
{
public:
	enum LOG_LEVEL
	{
		LOG_BASEDEBUG=0,
		LOG_DEBUG=1,
		LOG_WARN=2,
		LOG_ERROR=3,
		LOG_FAULT=4,
		LOG_NONE=5
	};
	struct stuDateTime
	{
		char year;
		char month;
		char day;
		char hour;
		char minute;
		char second;
	};
	struct stuLog
	{
		int	size;
		long	threadid;
		char	logtype;
		stuDateTime		datetime;
		char	modulename[20];
		char	filename[40];
		int		line;
		char	msg[10240];//attr:m=xxx;f=xxx;t=xxx;
	};
	struct stuLogDetail
	{
	public:
		stuLogDetail()
		{
			pNext = NULL;
		}
		string	clientip;
		string	modulename;
		long	threadid;
		string	datetime;
		short	logtype;
		string	filename;
		string	field1;
		string	field2;
		string	field3;
		string	field4;
		string	field5;
		int		line;
		string	msg;
		stuLogDetail *pNext;
	};

	SLog();
	virtual ~SLog();


	static std::string bufToHexString(BYTE *pBuffer, int iLen);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取静态日志实例
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:31
	// 参数说明:  void
	// 返 回 值:  SLog*
	//////////////////////////////////////////////////////////////////////////
	static SLog* getLog();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  输出日志头
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:32
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void logHead(short logtype,const char* file,int line,const char* func);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  输出日志体
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:32
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void logBody(const char * cformat, ...);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  记录到文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:32
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	inline void logFile(char *buff);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  写文件行
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:33
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static void writeFileLine(char * filename,char *buff);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置日志级别
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:17
	// 参数说明:  @l为新的日志级别
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogLevel(LOG_LEVEL l);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置日志文件最大大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:17
	// 参数说明:  @size文件大小，单位字节
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogFileMaxSize(int size);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置备份文件的最多数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:18
	// 参数说明:  @files表示备份文件最多数量
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setLogFileMaxBackFiles(int files);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置当前模块名称
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:19
	// 参数说明:  @name新的模块名称
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void setModuleName(char* name);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前模块名称
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:19
	// 参数说明:  void
	// 返 回 值:  模块名称
	//////////////////////////////////////////////////////////////////////////
	static char* getModuleName();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到UDP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:20
	// 参数说明:  @ip为服务器IP地址
	//            @port表示端口
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToUDPSvr(char* ip,int port=7001);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到TCP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:24
	// 参数说明:  @ip为服务器IP地址
	//            @port表示端口
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToTCPSvr(char* ip,int port=7001);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到指定的文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:25
	// 参数说明:  @file表示日志文件名称
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToFile(char* file);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  开始日志输出到控制台
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:25
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool startLogToConsole();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到UDP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:25
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToUDPSvr();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到TCP服务器
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:26
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToTCPSvr();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:27
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToFile();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止日志输出到控制台
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:27
	// 参数说明:  void
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool stopLogToConsole();
//	SLock lock;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  初始化
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:28
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void init();
private:
	SOCKET m_hTcpSocket;
	SOCKET m_hUdpSocket;
	sockaddr_in m_dest_sin;//LOG服务器的地址

	string m_sModuleName;//模块名

	string m_sLogFile;//LOG文件名

	bool m_bToConsole;//是否输出到控制台
	bool m_bToFile;//是否输出到文件
	bool m_bToLogUDPSvr;//是否输出到UDP服务器
	bool m_bToLogTCPSvr;//是否输出到TCP服务器

	sockaddr_in m_udp_dest_sin;

	int m_iLogLevel;//日志级别
	long m_iMaxFileSize;//最大的日志文件大小
	int m_iMaxBackFiles;//最多的备份日志文件数，缺省5个
	stuLog m_log;
};
#pragma pack()
#pragma pack(pop)


#endif 
