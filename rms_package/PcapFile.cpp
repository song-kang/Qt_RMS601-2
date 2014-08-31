/**
 * 文 件 名 : PcapFile.cpp
 * 创建日期 : 2014-4-22 11:32
 * 作    者 : 宋康
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 报文监视的文件处理
 **/

#include "PcapFile.h"

extern char pcapFrameCache[];
extern unsigned int pcapFrameLen;
extern SLock cacheLock;

CPcapFile::CPcapFile(void)
{
#ifdef WIN32
	lzma7z = new CLzma7z;
#endif
	m_fpFile = 0;
	m_pLock = new SLock;
}

CPcapFile::~CPcapFile(void)
{
#ifdef WIN32
	delete lzma7z;
#endif
	delete m_pLock;
}

bool CPcapFile::SetPathName(string path)
{
	storePath = path;
#ifdef WIN32
	lzma7z->SetPathName(storePath);
#endif

	return true;
}

void CPcapFile::SetFormat7z(bool bZip,bool b7z)
{
	bCompress = bZip; 
	bFormat7z = b7z;
#ifdef WIN32
	string exe7z = storePath + "7za.exe";
	FILE *fp = fopen(exe7z.c_str(),"rb");
	if(fp == 0)
	{
		LOGWARN("[%s] no exist,don't compress.",exe7z.c_str());
		bCompress = false;
		bFormat7z = false;
	}
	lzma7z->SetFormat7z(bFormat7z);
#endif
	
}

void CPcapFile::ClosePcapFile()
{
	if(m_fpFile)
	{
		fclose(m_fpFile);
		m_fpFile = NULL;
	}
}

bool CPcapFile::OpenPcapFile(string fileName)
{
	m_sFile = "";
	if (m_fpFile!=NULL)
	{
		ClosePcapFile();
		m_fpFile = NULL;
	}

	m_sFile = fileName;	
	m_fpFile = fopen(m_sFile.c_str(),"ab+");
	if (!m_fpFile)
	{
		LOGERROR("Open file %s failed.", m_sFile.c_str());
		return false;
	}

	return true;
}

bool CPcapFile::SavePcapFile(string ethName,char *buffer,int length)
{
	int		len;
	bool	bResult;
	string  fullPathFile = storePath + ethName + ".pcap";

	len = GetFileLen(fullPathFile);
	if (len == -1 || len < sizeof(pcap_file_header_t))
	{
		bResult = WritePcapFileHeader(fullPathFile);
		if (bResult)
		{
			bResult = WritePcapFileBuffer(fullPathFile,buffer,length);
		}
		else
		{
			return false;
		}
	}
	else 
	{
		bResult = WritePcapFileBuffer(fullPathFile,buffer,length);
	}

	len = GetFileLen(fullPathFile);
	if (len > storeFileSize)
	{
		BackupFile(ethName);
	}

	return true;
}

bool CPcapFile::CachePcapFrame(string ethName,char *buffer,int length)
{
	int pos = sizeof(pcap_frame_header_t);
	pcap_file_header_t	pfh;
	mac_header_t		macHeader;
	ip_header_t			ipHeader;
	tcp_header_t		tcpHeader;

	memcpy(&macHeader,&buffer[pos],sizeof(mac_header_t));
	macHeader.Ntoh();
	pos += sizeof(mac_header_t);
	if (macHeader.type != IP_TYPE)
		return false;
	memcpy(&ipHeader,&buffer[pos],sizeof(ip_header_t));
	ipHeader.Ntoh();
	pos += sizeof(ip_header_t);
	if (ipHeader.ptl != TCP_PROTOCOL && ipHeader.ptl != UDP_PROTOCOL)
		return false;
	if (ipHeader.ptl == TCP_PROTOCOL)
	{
		memcpy(&tcpHeader,&buffer[pos],sizeof(tcpHeader));
		tcpHeader.Ntoh();
		pos += sizeof(tcp_header_t);
		if (tcpHeader.src_port == SERVERPORT || tcpHeader.dst_port == SERVERPORT)
			return false;
	}

	cacheLock.lock();
	if (pcapFrameLen + length > MAXFRAMECACHE)
	{
		LOGWARN("在网口[%s]缓存抓包帧失败！网络分析工具将丢包。",ethName.c_str());
		cacheLock.unlock();
		return false;
	}
	if (!pcapFrameLen)
	{
		MakePcapFileHeader(pfh);
		memcpy(pcapFrameCache,&pfh,sizeof(pcap_file_header_t));
		pcapFrameLen += sizeof(pcap_file_header_t);
	}
	memcpy(&pcapFrameCache[pcapFrameLen],buffer,length);
	pcapFrameLen += length;
	cacheLock.unlock();

	return true;
}

int CPcapFile::GetFileLen(string sFile)
{
	FILE* fp = fopen(sFile.c_str(),"r");

	if (!fp)
	{
		LOGERROR("Not find file %s ,because open this file failed.", sFile.c_str());
		return -1;
	}

	fseek(fp,0,SEEK_END);
	int len  = ftell(fp);

	fclose(fp);

	return len;
}

void CPcapFile::MakePcapFileHeader(pcap_file_header_t &pfh)
{
	pfh.magic			= 0xa1b2c3d4;
	pfh.version_major	= 0x02;
	pfh.version_minor	= 0x04;
	pfh.thiszone		= 0;
	pfh.sigfigs			= 0;
	pfh.snaplen			= 0xffff;
	pfh.linktype		= 1; 
}

bool CPcapFile::WritePcapFileHeader(string path)
{
	bool	bResult;
	pcap_file_header_t	pfh;

	bResult = OpenPcapFile(path);
	if (bResult)
	{
		MakePcapFileHeader(pfh);
		fwrite(&pfh,1,sizeof(pfh),m_fpFile);
		ClosePcapFile();
	}
	else
	{
		return false;
	}

	return true;
}

bool CPcapFile::WritePcapFileBuffer(string path,char *buffer,int length)
{
	bool	bResult;

	bResult = OpenPcapFile(path);
	if (bResult)
	{
		size_t size = fwrite(buffer,length,1,m_fpFile);
		ClosePcapFile();
	}
	else
	{
		return false;
	}

	return true;
}

void CPcapFile::BackupFile(string ethName)
{
	char	tmpfile0[512] = {'\0'};
	char	tmpfile1[512] = {'\0'};
	char	cmd[1024] = {'\0'};

	string  path = storePath + ethName + ".pcap";

#ifndef WIN32
	for(int t=2;t<=MAXSAVEFILENUM;t++)	//删除最旧的文件
	{
		memset(tmpfile0,0,sizeof(tmpfile0));
		memset(tmpfile1,0,sizeof(tmpfile1));
		if (bCompress)
		{
			sprintf(tmpfile0,"%s.back%03d.tgz",path.c_str(),t-1);
			sprintf(tmpfile1,"%s.back%03d.tgz",path.c_str(),t);
		}
		else
		{
			sprintf(tmpfile0,"%s.back%03d.pcap",path.c_str(),t-1);
			sprintf(tmpfile1,"%s.back%03d.pcap",path.c_str(),t);
		}
		::remove(tmpfile0);
		::rename(tmpfile1,tmpfile0);
	}

	if (bCompress)
	{
		m_pLock->lock();	//加锁,防止采集速度大于压缩速度
		string toFile = SDir::currentDirPath().data() + ethName + ".pcap";
		::rename(path.c_str(),toFile.c_str());
		srcCompressFile = ethName + ".pcap";
		dstCompressFile = tmpfile1; 
		m_pLock->unlock();

		SKT_CREATE_THREAD_S(ThreadCompress,this,512000);
	}
	else
		::rename(path.c_str(),tmpfile1);
#else
	for(int t=2;t<=MAXSAVEFILENUM;t++)	//删除最旧的文件
	{
		memset(tmpfile0,0,sizeof(tmpfile0));
		memset(tmpfile1,0,sizeof(tmpfile1));
		if (bCompress)
		{
			if (bFormat7z)
			{
				sprintf(tmpfile0,"%s.back%03d.7z",path.c_str(),t-1);
				sprintf(tmpfile1,"%s.back%03d.7z",path.c_str(),t);
			}
			else
			{
				sprintf(tmpfile0,"%s.back%03d.lzma",path.c_str(),t-1);
				sprintf(tmpfile1,"%s.back%03d.lzma",path.c_str(),t);
			}
		}
		else
		{
			sprintf(tmpfile0,"%s.back%03d.pcap",path.c_str(),t-1);
			sprintf(tmpfile1,"%s.back%03d.pcap",path.c_str(),t);
		}
		::remove(tmpfile0);
		::rename(tmpfile1,tmpfile0);
	}

	if (bCompress)
	{
		exe7z = storePath + "7za.exe";
		FILE *fp = fopen(exe7z.c_str(),"rb");
		if(fp == 0)
		{
			LOGWARN("[%s] no exist,don't compress.",exe7z.c_str());
			bCompress = false;
			bFormat7z = false;
			lzma7z->SetFormat7z(bFormat7z);
		}
		else
		{
			fclose(fp);
			m_pLock->lock();	//加锁,防止采集速度大于压缩速度
			srcCompressFile = path + ".pcap";
			::rename(path.c_str(),srcCompressFile.c_str());
			dstCompressFile = tmpfile1;
			m_pLock->unlock();

			SKT_CREATE_THREAD_S(ThreadCompress,this,512000);
		}
	}
	else
		::rename(path.c_str(),tmpfile1);
#endif
}

void* CPcapFile::ThreadCompress(void *lp)
{
	char cmd[1024] = {'\0'};
	CPcapFile *p = (CPcapFile*)lp;

	p->m_pLock->lock();

#ifdef WIN32
	sprintf(cmd," \"%s a \"%s\" \"%s\" ",p->exe7z.c_str(),p->dstCompressFile.c_str(),p->srcCompressFile.c_str());
#else
	sprintf(cmd,"tar zcvf \"%s\" \"%s\" ",p->dstCompressFile.c_str(),p->srcCompressFile.c_str());
#endif
	::system(cmd);
	::remove(p->srcCompressFile.c_str());

	p->m_pLock->unlock();

	return NULL;
}
