/**
 * 文 件 名 : main.cpp
 * 创建日期 : 2014-4-26 11:24
 * 作    者 : 宋康
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 报文监视入口
 **/

#define MOD_VERSION "2.0.0"

#ifndef MOD_DATE
#define MOD_DATE "2014-06-20"
#endif
#ifndef MOD_TIME
#define MOD_TIME "16:17:00"
#endif

#include "PackageApplication.h"
CPackageApplication g_app;

int main(int argc, char* argv[])
{
	g_app.SetModuleDesc("RMS601+报文监听");
	g_app.SetVersion(MOD_VERSION);
	g_app.Run(argc,argv,MOD_DATE,MOD_TIME);

	return 0;
}
