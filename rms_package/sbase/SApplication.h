/**
 *
 * 文 件 名 : SApplication.h
 * 创建日期 : 2009-8-20 13:36
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:32 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: SApplication.h,v $
 *            Revision 1.1  2011/08/31 08:43:32  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.8  2011/04/27 02:58:48  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2011/04/23 10:53:52  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2011/04/22 06:32:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/21 02:14:16  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/11 12:08:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/03/29 07:07:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/29 01:03:26  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/11/17 02:19:15  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *
 **/

#ifndef __SKT_APPLICATION_H__
#define __SKT_APPLICATION_H__

#include "SApi.h"
#include "SFile.h"
#include "SLog.h"
#include "SString.h"

#ifndef _WITHOUT_AGENT_CHANNEL_

//////////////////////////////////////////////////////////////////////////
// 名    称:  CAgentPackage
// 作    者:  邵凯田
// 创建时间:  2011-3-29 9:54
// 描    述:  代理通道
//////////////////////////////////////////////////////////////////////////
struct CAgentPackage
{
public:
	CAgentPackage()
	{
		pBuffer = NULL;
		iLength = 0;
	}
	virtual ~CAgentPackage()
	{
		if(pBuffer != NULL)
		{
			delete[] pBuffer;
			pBuffer = NULL;
			iLength = 0;
		}
	}
	SString sMsg;
	BYTE *pBuffer;
	int iLength;
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  stuModuleInfo
// 作    者:  邵凯田
// 创建时间:  2011-3-29 9:53
// 描    述:  描述信息结构
//////////////////////////////////////////////////////////////////////////
struct stuModuleInfo
{
	int handle;//句柄
	int pid;//进程号
	SString name;//模块名
	SString version;//版本号
	SString desc;//模块描述
	SString crc;//模块CRC
	SString compile_time;//编译时间
	SString login_ip;//登录运行IP
	int login_port;//登录运行端口
	SDateTime login_time;//登录时间
};

class SApplication;

class CSAgentClient
{
public:
	CSAgentClient();
	~CSAgentClient();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  接收代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:59
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static int ReceiveMsg(SSocket *pSocket,SString &sMsg,BYTE* &pBuffer,int &iLength);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  发送代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:59
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static int SendMsg(SSocket *pSocket,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向代理进程发送消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:56
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void AddMsgToAgent(SString &sMsg,BYTE* pBuffer=NULL,int iLength=0)
	{
		CAgentPackage *p = new CAgentPackage();
		p->sMsg = sMsg;
		p->pBuffer = pBuffer;
		p->iLength = iLength;
		m_SendList.append(p);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  提取当前已经登录到代理的进程模块列表信息
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 9:55
	// 参数说明:  引用返回的列表
	// 返 回 值:  >=0表示进程数量,<0表示失败
	//////////////////////////////////////////////////////////////////////////
	int GetModuleList(SPtrList<stuModuleInfo> &module_list);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取代理服务端的运行信息
	// 作    者:  邵凯田
	// 创建时间:  2012-9-26 11:44
	// 参数说明:  @sDisk 表示需要读取的硬盘信息，disks=3;disk1=/dev/sda1;disk2=/dev/sda2;disk3=/dev/sda3;
	//            @sRunInfo引用返回的以字符串描述的运行信息,act=runinfo;cpu_used=%d;mem_all=%d;mem_used=%d;mem_free=%d;disk1_all=%d;disk1_used=%d;disk1_free=%d;disk1_per=%d;
	// 返 回 值:  1表示成功，0表示失败
	//////////////////////////////////////////////////////////////////////////
	bool GetRunInfo(SString sDisk,SString &sRunInfo);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定模块的在代理进程上的句柄
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 14:42
	// 参数说明:  模块名称，即进程名称，WIN32平台上不需要带.exe
	// 返 回 值:  =0表示模块未找到，>0表示进程句柄
	//////////////////////////////////////////////////////////////////////////
	int GetModuleHandle(SString sModName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向指定的模块发送一个消息
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 11:06
	// 参数说明:  handle为目标模块的句柄
	// 返 回 值:  发送成功返回ture，失败返回false
	//////////////////////////////////////////////////////////////////////////
	bool AddMsgToModule(int handle,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  基类处理代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:58
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool ProcessAgentMsgBase(SString &sMsg,BYTE* pBuffer=NULL,int iLength=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  派生类处理代理消息
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 16:58
	// 参数说明:  @from_handle为消息来源模块的句柄(0表示消息来自代理进程)
	//         :  @sMsg为消息字符串
	//         :  @pBuffer为消息携带的二进制数据内容，NULL表示无二进制数据
	//         :  @iLength为二进制数据长度
	// 返 回 值:  true表示处理成功，false表示处理失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessAgentMsg(int from_handle,SString &sMsg,BYTE* pBuffer=NULL,int iLength=0)=0;

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  代理线程，执行代理任务，线程运行时不占用Run函数，也可直接运行
	// 作    者:  邵凯田
	// 创建时间:  2012-5-30 11:00
	// 参数说明:  @lp：this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadAgent(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  启动代理服务
	// 作    者:  邵凯田
	// 创建时间:  2012-5-31 10:13
	// 参数说明:  @bBlock为true表示阻塞运行，直到程序退出，false表示不阻塞启动线程运行代理服务
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool StartAgent(bool bBlock);

public:
	bool m_bLogined;//是否已经登录
	SString m_sAgentIp;//代理IP地址，缺省为127.0.0.1
	int m_iAgentPort;//代理端口
	SPtrList<CAgentPackage> m_SendList;
	SString m_sWaitMsgAct;//等待消息的act标志
	SString m_sWaitMsg;//等待消息的返回字符串
	int m_iCpuPercent;
	SApplication *m_pApp;
	int m_iThreads;//线程数量
};
#endif

//////////////////////////////////////////////////////////////////////////
// 名    称:  SApplication
// 作    者:  邵凯田
// 创建时间:  2008-10-10 14:16
// 描    述:  应用程序管理类
//////////////////////////////////////////////////////////////////////////
#ifndef _WITHOUT_AGENT_CHANNEL_
class SApplication : public CSAgentClient
{
public:
	friend class CSAgentClient;
#else
class SApplication
{
public:
#endif
	SApplication();
	virtual ~SApplication();
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动应用程序
	// 作    者:  邵凯田
	// 创建时间:  2008-10-10 14:17
	// 参数说明:  @argc/argv为main输入参数
	//            @szDate/szTime为编译日期时间
	//            @bBlock为是否阻塞运行，true为阻塞，false为不阻塞
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Run(int argc, char* argv[],const char* szDate,const char* szTime,bool bBlock=true);
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动服务，开启所有应用的服务，该函数必须由派生类实现，且启动服务完成后必须返回
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 9:15
	// 参数说明:  
	// 返 回 值:  null
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start()=0;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止服务，关闭所有应用的服务，
	// 作    者:  邵凯田
	// 创建时间:  2010-7-1 9:18
	// 参数说明:  
	// 返 回 值:  null
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop()=0;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置模块版本号
	// 作    者:  邵凯田
	// 创建时间:  2009-5-19 10:56
	// 参数说明:  sVer(采用:<产品内部型号>.<阶段版本号>.<模块版本号>)
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	void SetVersion(SString sVer);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置模块描述
	// 作    者:  邵凯田
	// 创建时间:  2009-5-19 12:29
	// 参数说明:  sDesc描述
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	void SetModuleDesc(SString sDesc);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置模块信息
	// 作    者:  邵凯田
	// 创建时间:  2009-5-19 12:29
	// 参数说明:  sInfo = name=;ver=;crc=;date=
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	void SetModuleInfo(SString sInfo);
	////////////////////////////////////////////////////////////////////////
	// 描    述:  退出模块
	// 作    者:  邵凯田
	// 创建时间:  2011-3-29 8:44
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void Quit()
	{
		m_bHalt = true;
	}
	inline bool IsDebugMode(){return m_bDebugMode;};
	inline bool IsLogToConsole(){return m_bLogToConsole;};
	bool IsParam(SString sParamName);
	bool IsParam(SString sParamName,SString &sParamValue);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取命令行参数内容
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 10:13
	// 参数说明:  @sParamName为参数名
	// 返 回 值:  参数值
	//////////////////////////////////////////////////////////////////////////
	SString GetParam(SString sParamName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取命令行整型参数内容
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 10:13
	// 参数说明:  @sParamName为参数名
	// 返 回 值:  参数值
	//////////////////////////////////////////////////////////////////////////
	int GetParamI(SString sParamName);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取命令行浮点型参数内容
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 10:13
	// 参数说明:  @sParamName为参数名
	// 返 回 值:  参数值
	//////////////////////////////////////////////////////////////////////////
	float GetParamF(SString sParamName);

	inline SString GetExePath(){return m_sExePath;};
	inline SString GetModule(){return m_sModule;};
	inline SString GetAppbaseVer(){return m_sAppbaseVer;};
	inline SString GetModuleVer(){return m_sModuleVer;};
	inline SString GetModuleCrc(){return m_sModuleCrc;};
	inline SString GetModuleTime(){return m_sModuleTime;};
	inline SString GetModuleDesc(){return m_sModuleDesc;};
	inline void SetFork(bool b){m_bFork = b;};
	inline SString GetInfo(){return m_sModuleInfo;};
	inline void SetInfo(SString info){m_sModuleInfo = info;};

	static bool ParseCommandLine(SString str);

	inline bool IsStarted(){return m_bStarted;};
	inline void SetStarted(bool b){m_bStarted=b;};
private:
	
	bool TouchClose();//创建一个本进程的close文件，以通知原进程关闭
	bool RemoveClose();//移除close文件
	bool IsClosedExist();//close文件是否存在？
	bool SearchOldProcess(int &pid,int &ppid,SString &name);

	bool _Start();
	bool _Stop();

	SString m_sExePath;//执行程序所在路径
	SString m_sModule;//模块名
	SString m_sAppbaseVer;//基类库版本
	SString m_sModuleVer;//模块版本
	SString m_sModuleCrc;//模块CRC
	SString m_sModuleTime;//模块生成时间
	SString m_sModuleDesc;//模块描述
	SString m_sModuleInfo;//模块信息
	bool m_bHalt;//是否准备退出?

	bool m_bDebugMode;//是否处于调试模式
	bool m_bLogToConsole;//是否允许日志信息输出到控制台
	bool m_bFork;//LINUX下是否执行fork
	bool m_bStarted;//是否已经启动应用
	
public:
	int m_iArgc;
	char** m_ppArgv;
};



#endif//__YS_REC_APPLICATION_H__
