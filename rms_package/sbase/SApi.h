/**
 *
 * 文 件 名	: SApi.h
 * 创建日期	: 2007-06-25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: 2011/08/31 08:43:33 $
 * 当前版本	: $Revision: 1.1 $
 * 功能描述 : 平台无关的公用函数、适用于Linux/Windows
 * 修改记录 : 
 *            $Log: SApi.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/21 12:32:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/16 08:35:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/02/28 11:21:06  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/11/19 02:03:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/10/29 08:48:41  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.6  2009/05/19 02:50:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/10/17 03:33:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/10/16 09:16:50  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/10/16 03:14:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/09/05 12:39:39  shaokt
 *            添加取当前模块EXE文件所有目录的函数
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:00  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.10  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.9  2007/12/10 08:18:57  shaokt
 *            修改取记录号的程序，将标识编号（设备号）放到后面，保证记录号最前面为yyyyMMddhhmmsszzz
 *
 *            Revision 1.8  2007/11/19 11:10:47  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.7  2007/11/14 11:31:15  shaokt
 *            DWORD类型改为unsigned long
 *
 *            Revision 1.6  2007/10/30 10:46:20  shaokt
 *            启动程序时添加一个附加参数传递临时工作路径
 *
 *            Revision 1.5  2007/10/25 10:40:31  shaokt
 *            LINUX下取系统进程信息时安符串未清除的BUG
 *
 *            Revision 1.4  2007/10/17 10:00:33  shaokt
 *            扩展进程函数
 *
 *            Revision 1.3  2007/09/11 08:17:52  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/08/15 03:23:21  shaokt
 *            增加字节序注释
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#if !defined(AFX_SKTAPI_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_)
#define AFX_SKTAPI_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_

#pragma once

#include "SInclude.h"
#include "SDateTime.h"

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef INT64
#ifdef WIN32
#define INT64 __int64
#define UINT64 unsigned __int64
#else
#define INT64 long long
#define UINT64 unsigned long long
#endif
#endif

#ifndef U_LONG
//无符号长整型，32位程序中为4字节，64位程序中为8字节
#define U_LONG unsigned long
#endif

#ifdef WIN32
#include <tlhelp32.h>
#else
#include <signal.h>   
#endif

enum SKT_BYTE_ORDER
{
	SKT_LITTLE_ENDIAN=1,//低位在前，INTEL字节序
	SKT_BIG_ENDIAN=2,//高位在前，网络/POWERPC/SPARC字节序
	SKT_UNKNOWN_ENDIAN=3//未知
};

//以下定义用于实现高效的交换字节序，
//当定义了_POWERPC符号时交换字节，否则不进行字节序交换
//POWERPC平台的float与X86及ARM的float字节顺序也是相反的，以X86的float为标准
#ifdef _POWERPC
#define _SKT_NUMBER_SWAP //是否需要字节顺序交换
#endif

#ifdef _SKT_NUMBER_SWAP

#define SKT_SWAP_WORD(w) w = (((WORD)w) << 8) | (((WORD)w) >> 8);
#define SKT_SWAP_DWORD(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#define SKT_SWAP_FLOAT(f) {DWORD dw;memcpy(&dw,&f,4);dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);memcpy(&f,&dw,4);}

#define SKT_SWAP_WORD_REV(w)
#define SKT_SWAP_DWORD_REV(w)
#define SKT_SWAP_FLOAT_REV(f)

#else

#define SKT_SWAP_WORD(w)
#define SKT_SWAP_DWORD(w)
#define SKT_SWAP_FLOAT(f)

#define SKT_SWAP_WORD_REV(w) w = (((WORD)w) << 8) | (((WORD)w) >> 8);
#define SKT_SWAP_DWORD_REV(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#define SKT_SWAP_FLOAT_REV(f) {DWORD dw;memcpy(&dw,&f,4);dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);memcpy(&f,&dw,4);}

#endif

#define SKT_SWAP_WORD_ALWAYS(w) w = (((WORD)w) << 8) | (((WORD)w) >> 8);
#define SKT_SWAP_DWORD_ALWAYS(dw) dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);
#define SKT_SWAP_FLOAT_ALWAYS(f) {DWORD dw;memcpy(&dw,&f,4);dw = (((DWORD)dw) << 24 | ((DWORD)dw) >> 24) | ((((DWORD)dw)&0x0000ff00) << 8 | (((DWORD)dw)&0x00ff0000) >> 8);memcpy(&f,&dw,4);}


//单字节的校验和
#define SKT_BYTECHECKSUM_WORD(w) ((((WORD)w) >> 8) + (((WORD)w)  & 0xFF))
#define SKT_BYTECHECKSUM_DWORD(dw) ((((DWORD)dw>>24)&0xFF) + (((DWORD)dw>>16)&0xFF) + (((DWORD)dw>>8)&0xFF) + (((DWORD)dw)&0xFF))

//取最大值
#define SKT_MAX(a,b) ((a)>(b)?(a):(b))

//取最小值
#define SKT_MIN(a,b) ((a)<(b)?(a):(b))

#ifdef WIN32
	//创建线程
	#define SKT_CREATE_THREAD(fun,p) \
		{ \
			HANDLE h; \
			h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(int)fun, (LPVOID)p, 0, 0); \
			CloseHandle(h); \
		} 
	//创建线程并指定栈大小，WINDOW上无效
	#define SKT_CREATE_THREAD_S(fun,p,stack_size) \
		{ \
			HANDLE h; \
			h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(int)fun, (LPVOID)p, 0, 0); \
			CloseHandle(h); \
		} 

	#define SKT_INTO_THREAD

#else
	//创建线程
	#define SKT_CREATE_THREAD(fun,p) \
		{ \
			pthread_t t=0; \
			int stacksize = 204800;/*20480为20K*/; \
			pthread_attr_t attr,*pattr=NULL; \
			if(pthread_attr_init(&attr) == 0 && pthread_attr_setstacksize(&attr, stacksize) == 0) \
				pattr = &attr; \
			else \
				printf("@@@@ pthread_attr_setstacksize to %d error!\r\n",stacksize); \
			pthread_create(&t, pattr, fun,(void*)p); \
			if(t == 0) \
				printf("@@@ pthread_create error!"); \
		} 
	//创建线程并指定栈大小
	#define SKT_CREATE_THREAD_S(fun,p,stack_size) \
		{ \
			pthread_t t=0; \
			int stacksize = stack_size;/*20480为20K*/; \
			pthread_attr_t attr,*pattr=NULL; \
			if(pthread_attr_init(&attr) == 0 && pthread_attr_setstacksize(&attr, stacksize) == 0) \
				pattr = &attr; \
			else \
				printf("@@@@ pthread_attr_setstacksize to %d error!\r\n",stacksize); \
			pthread_create(&t, pattr, fun,(void*)p); \
			if(t == 0) \
				printf("@@@ pthread_create error!"); \
		} 
	#define SKT_INTO_THREAD  pthread_detach(pthread_self());
#endif

//////////////////////////////////////////////////////////////////////////
// 名    称:  SApi
// 作    者:  邵凯田
// 创建时间:  2011-11-14 12:58
// 描    述:  基本API接口函数，多以静态函数形式
//////////////////////////////////////////////////////////////////////////
class SApi
{
public:
	////////////////////////////////////////////////////////////////////////
	// 描    述:  休眠，单位微秒，WIN平台下只能精确休眠到毫秒级
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 12:59
	// 参数说明:  @mms表示休眠的微秒数
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void UsSleep(long mms);

	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前运行CPU的字节顺序序
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:02
	// 参数说明:  void
	// 返 回 值:  SKT_BYTE_ORDER(SKT_LITTLE_ENDIAN / SKT_BIG_ENDIAN)
	//////////////////////////////////////////////////////////////////////////
	static SKT_BYTE_ORDER YsGetSysByteOrder();
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  转换2字节整数的字节顺序为指定的字节序
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:03
	// 参数说明:  @byteOrder要求转换后的字节顺序
	//            @w待转换值
	// 返 回 值:  转换后的值
	//////////////////////////////////////////////////////////////////////////
	static WORD Yshton(SKT_BYTE_ORDER byteOrder,WORD w);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  转换4字节整数的字节顺序为指定的字节序
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:03
	// 参数说明:  @byteOrder要求转换后的字节顺序
	//            @w待转换值
	// 返 回 值:  转换后的值
	//////////////////////////////////////////////////////////////////////////
	static DWORD Yshton(SKT_BYTE_ORDER byteOrder,DWORD w);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  转换2字节整数的字节顺序为指定的字节序
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:03
	// 参数说明:  @byteOrder要求转换后的字节顺序
	//            @w待转换值
	// 返 回 值:  转换后的值
	//////////////////////////////////////////////////////////////////////////
	static WORD Ysntoh(SKT_BYTE_ORDER byteOrder,WORD w);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  转换4字节整数的字节顺序为指定的字节序
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:03
	// 参数说明:  @byteOrder要求转换后的字节顺序
	//            @w待转换值
	// 返 回 值:  转换后的值
	//////////////////////////////////////////////////////////////////////////
	static DWORD Ysntoh(SKT_BYTE_ORDER byteOrder,DWORD w);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  生成字符型记录号
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:05
	// 参数说明:  id为附加标识号
	// 返 回 值:  23个字符组成的记录号
	//////////////////////////////////////////////////////////////////////////
	static SString GetRecordIndex(int id=0);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取系统当前运行进程信息到一个字符串列表中
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:07
	// 参数说明:  @plist为存放进程信息的字符串列表,每个进程信息的表示方式为(pid,ppid,name;....)
	// 返 回 值:  =0表示提取失败，>0表示成功且返回的是进程数量
	//////////////////////////////////////////////////////////////////////////
	static int GetSystemProcess(SStringList &plist);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前运行进程的执行文件所在路径
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:10
	// 参数说明:  void
	// 返 回 值:  执行文件所有路径
	//////////////////////////////////////////////////////////////////////////
	static SString GetExeModulePath();
		
	////////////////////////////////////////////////////////////////////////
	// 描    述:  执行一个外部命令行，且不影响本进程的运行(即不阻塞当前进程,不必等待外部命令退出后才返回)
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:13
	// 参数说明:  @sCmdLine表示运行时命令行，可以为路径加执行文件名
	//            @sModule表示运行模块的可执行程序文件名（不含路径）
	//            @sArg表示运行参数，多个参数可以用空格分隔，最多可以放8个参数，同个参数中不允许出现空格
	//            @sPath表示执行文件所有的路径
	// 返 回 值:  >0表示执行成功，否则为执行失败
	//////////////////////////////////////////////////////////////////////////
	static int SystemExec(SString sCmdLine,SString sModule,SString sArg,SString sPath="");

	////////////////////////////////////////////////////////////////////////
	// 描    述:  强行终止指定进程号的进程
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:21
	// 参数说明:  @pid表示要终止的进程的进程号
	// 返 回 值:  true表示终止成功,false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool SystemKillProcess(int pid);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  重新启动操作系统
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:22
	// 参数说明:  void
	// 返 回 值:  true表示执行成功,false表示执行失败
	//////////////////////////////////////////////////////////////////////////
	static bool SystemReboot();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  关闭操作系统
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:23
	// 参数说明:  void
	// 返 回 值:  true表示执行成功,false表示执行失败
	//////////////////////////////////////////////////////////////////////////
	static bool SystemHalt();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  获取timeval格式的当前系统时间
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:24
	// 参数说明:  @tmVal为引用返回的时间结构体
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void GetLocalDayTime(timeval& tmVal);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  使用timeval格式设置当前系统时间
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:24
	// 参数说明:  @tmVal表示需要设置的新时间
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void SetLocalDayTime(timeval& tmVal);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取CPU使用率百分比单位%
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:25
	// 参数说明:  void
	// 返 回 值:  当前CPU百分比
	//////////////////////////////////////////////////////////////////////////
	static int GetCpuUsedPercent();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前内存的使用状态
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:25
	// 参数说明:  @iMemAll表示内存总大小，单位K字节
	//            @iMemUsed表示内存已使用的大小，单位K字节
	//            @iMemFree表示内存空闲大小，单位K字节
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void GetMemStatus(int &iMemAll,int &iMemUsed,int &iMemFree);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定硬盘分区的使用情况
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:27
	// 参数说明:  @sDiskDev表示硬盘设备名称
	//            @iDiskAll表示硬盘总大小，单位K字节
	//            @iDiskUsed表示硬盘已使用大小，单位K字节
	//            @iDiskFree表示硬盘剩余空间大小，单位K字节
	//            @iDiskUsePer表示硬盘已使用的百分比
	// 返 回 值:  true表示提取成功，false表示提取失败
	//////////////////////////////////////////////////////////////////////////
	static bool GetDiskSpace(SString sDiskDev,int &iDiskAll,int &iDiskUsed,int &iDiskFree,int &iDiskUsePer);
	
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定硬盘的总大小，仅针对LINUX系统
	// 作    者:  邵凯田
	// 创建时间:  2012-7-25 11:40
	// 参数说明:  @sDiskDev为硬盘设备名
	// 返 回 值:  大小加单位，如：320.0 GB / 8109 MB
	//////////////////////////////////////////////////////////////////////////
	static SString GetDiskSize(SString sDiskDev="/dev/sda");

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定目录的总大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:31
	// 参数说明:  @sDir表示目录的全路径名
	// 返 回 值:  目录及子目录的全部文件总大小，单位K字节
	//////////////////////////////////////////////////////////////////////////
	static float GetDirSpace(SString sDir);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  用指定长度的缓冲区内容生成32位的CRC校验码
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:32
	// 参数说明:  @DataBuf表示缓冲区指针
	//            @len表示缓冲区长度
	// 返 回 值:  DWORD型的CRC校验码
	//////////////////////////////////////////////////////////////////////////
	static DWORD GenerateCRC32(unsigned char * DataBuf,int  len);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  用指定的文件生成32位的CRC校验码，并返回为按16进制格式化后的字符串
	// 作    者:  邵凯田
	// 创建时间:  2011-11-14 13:34
	// 参数说明:  @filename表示文件名
	//            @err表示错误时的返回错误内容
	// 返 回 值:  空字符串表示生成错误(错误内容在err中)，否则返回按16进制格式化后的字符串共8个字节
	//////////////////////////////////////////////////////////////////////////
	static SString GenerateCRCByFile(SString filename,SString &err);
	
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  挂载USB盘，只支持LINUX系统
	// 作    者:  邵凯田
	// 创建时间:  2012-10-12 10:08
	// 参数说明:  void
	// 返 回 值:  返回挂载点，空串表示无U盘
	//////////////////////////////////////////////////////////////////////////
	static SString MountUsbDisk();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  卸载U盘，只支持LINUX系统，与MountUsbDisk配合使用
	// 作    者:  邵凯田
	// 创建时间:  2012-10-12 10:09
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	static void UmountUsbDisk();

	static void Create_CRC_table();
		//生成CRC32校验码
	static unsigned long Cal_CRC(unsigned char *p_begin, unsigned char *p_end);
		//验证CRC32校验码
	static unsigned int Check_CRC(unsigned char *p_begin, unsigned char *p_end);



	//////////////////////////////////////////////////////////////////////////
	//MEMO: DES/3DES 加密算法 [2013:10:15 14:23 邵凯田]

	////////////////////////////////////////////////////////////////////////
	// 描    述:  DES加密
	// 作    者:  邵凯田
	// 创建时间:  20xx
	// 参数说明:  @bEnspot：true表示加密，false表示解密
	//         :  @pbyKey：指向进行加密的密钥，8字节
	//         :  @nLength：需要进行加密的数据长度，以字节为单位，需要为8的倍数
	//         :  @pbySource：指向需要进行加密的数据首指针
	//         :  @pbyTarget：指向返回的加密后的数据首指针
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool Encrypt_DES(bool bEnspot, unsigned char* pbyKey,int nLength, unsigned char* pbySource, unsigned char* pbyTarget);


	////////////////////////////////////////////////////////////////////////
	// 描    述:  3DES加密
	// 作    者:  邵凯田
	// 创建时间:  20xx
	// 参数说明:  @bEnspot：true表示加密，false表示解密
	//         :  @pbyKey：指向进行加密的密钥，8字节
	//         :  @nLength：需要进行加密的数据长度，以字节为单位，需要为8的倍数
	//         :  @pbySource：指向需要进行加密的数据首指针
	//         :  @pbyTarget：指向返回的加密后的数据首指针
	// 返 回 值:  true表示成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	static bool Encrypt_3DES(bool bEnspot, unsigned char* pbyKey,int nLength, unsigned char* pbySource,unsigned char* pbyTarget);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  字符串加密
	// 作    者:  邵凯田
	// 创建时间:  2013:10:15 15:02
	//         :  @sSrc为源字符串明文
	//         :  @sKey为密钥，16字节
	// 返 回 值:  密文
	//////////////////////////////////////////////////////////////////////////
	static SString Encrypt_String(SString sSrc,SString sKey);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  字符串解密
	// 作    者:  邵凯田
	// 创建时间:  2013:10:15 15:02
	//         :  @sSrc为加密了的字符串密文
	//         :  @sKey为密钥，16字节
	// 返 回 值:  明文
	//////////////////////////////////////////////////////////////////////////
	static SString Decrypt_String(SString sSrc,SString sKey);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前进程号
	// 作    者:  邵凯田
	// 创建时间:  2014:4:22 11:41
	// 参数说明:  void
	// 返 回 值:  进程号
	//////////////////////////////////////////////////////////////////////////
	static int GetPid(); 

private:
	static void deskey(unsigned char *key,short edf);
	static void cookey(register unsigned long *raw1);
	static void cpkey(register unsigned long *into);
	static void usekey(register unsigned long *from);
	static void des(unsigned char *inblock, unsigned char *outblock);
	static void scrunch(register unsigned char *outof, register unsigned long *into);
	static void unscrun(register unsigned long *outof, register unsigned char *into);
	static void desfunc(register unsigned long *block, register unsigned long *keys);
	static void des2key(unsigned char *hexkey, short mode);
	static void Ddes(unsigned char *from, unsigned char *into);

	//end des/3des


public:

};

#endif // !defined(AFX_SKTAPI_H__3082C864_723C_4BE6_AB17_2CFE75460894__INCLUDED_)
