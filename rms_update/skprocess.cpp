#include "skprocess.h"
#include <windows.h>
#include <tlhelp32.h>
#include <process.h>

skProcess::skProcess(QObject *parent)
	: QObject(parent)
{

}

skProcess::~skProcess()
{

}

int skProcess::getSystemProcess(QStringList &plist)
{
	int index=0;
	QString sRet = "";
#ifdef WIN32
	HANDLE   SS;   
	PROCESSENTRY32   processinfo;   
	processinfo.dwSize=sizeof(processinfo);   
	SS=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);   
	if(SS==NULL)   
	{
		return 0;   
	}   

	bool status=Process32First(SS,&processinfo);   
	while(status)   
	{
		sRet = "";
		sRet += QString::number((int)processinfo.th32ProcessID)+","+
			QString::number((int)processinfo.th32ParentProcessID)+",";
		sRet += QString::fromWCharArray(processinfo.szExeFile);

		plist.append(sRet);
		index++;
		status=Process32Next(SS,&processinfo);   
	}   
	::CloseHandle(SS);   
#else
	////为不访问flash或硬盘，从/proc下查找进程信息
	//SDir dir("/proc");
	//SString sDir;
	//int i,cnt = dir.count();
	//int j,cnt2;
	//bool bProcess;
	//int pid,ppid;
	//SString sCmdLine;
	//for(i=0;i<cnt;i++)
	//{
	//	sDir = dir[i];
	//	//是否是进程目录?全数字目录为进程目录
	//	bProcess = true;
	//	cnt2 = sDir.length();
	//	for(j=0;j<cnt2;j++)
	//	{
	//		if(!(sDir.at(j) >= '0' && sDir.at(j) <= '9'))
	//		{
	//			bProcess = false;
	//			break;
	//		}
	//	}
	//	if(!bProcess)
	//		continue;
	//	pid = sDir.toInt();
	//	ppid = 0;//为少访问，暂不实现ppid读取
	//	SFile file("/proc/"+sDir+"/cmdline");
	//	if(!file.open(IO_ReadOnly))
	//		continue;
	//	sCmdLine = file.readLine();
	//	file.close();
	//	sRet = SString::toString((int)pid)+","+
	//		SString::toString((int)ppid)+","+sCmdLine;
	//	plist.append(sRet);
	//	index++;
	//}
#endif
	return index;
}

bool skProcess::getProcessByName(int &pid,int &ppid,QString name)
{
	bool ok;
	QStringList plist;
	QString sLine;
	QString pName;

	int p,i,cnt = getSystemProcess(plist);
	for(i=0;i<cnt;i++)
	{
		sLine = plist.at(i);
		QStringList list = sLine.split(",");
		pid   = list.at(0).toInt(&ok);
		ppid  = list.at(1).toInt(&ok);
		pName = list.at(2);

		if(pid == _getpid())
			continue;	//忽略自身的进程

		if(pName == name || name.contains(pName,Qt::CaseSensitive))	//winxp不带exe与win7带exe
			return true;
	}

	return false;
}

bool skProcess::closeProcess(int pid)
{
	bool ret=true;
#ifdef WIN32
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,(DWORD)pid);   
	if(TerminateProcess(hProcess,0) == FALSE)
		ret = false;
#else
	kill(pid,SIGUSR1);   
	waitpid(pid,NULL,0);   
#endif
	return ret;
}
