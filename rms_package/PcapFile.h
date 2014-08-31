/**
 * 文 件 名 : PcapFile.h
 * 创建日期 : 2014-4-22 11:32
 * 作    者 : 宋康
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 报文监视的文件处理
 **/

#ifndef __PCAP_FILE_H__
#define __PCAP_FILE_H__

#include "sbase/SApi.h"
#ifdef WIN32
#include "Lzma7z.h"
#endif

#define	MBYTE1				(1024*1024)
#define	MAXZIPLEN			(MBYTE1*50)	//最大文件压缩大小
#define	MINZIPLEN			(MBYTE1*5)	//最小文件压缩大小
#define	MAXSAVEFILENUM		200			//最大文件压缩个数
#define	MINSAVEFILENUM		20			//最小文件压缩个数
#define MAXFRAMECACHE		(MBYTE1*20)	//最大缓存数据大小
#define SERVERPORT			8791
#define COMMFRAMESYN		0x09D7

#define	IP_TYPE			0x0800
#define	TCP_PROTOCOL	6
#define	UDP_PROTOCOL	17

#pragma pack(1)
typedef struct commframe_header_t commframe_header_t;
struct commframe_header_t {
	unsigned short	syn;
	unsigned int	len;
};

typedef struct pcap_file_header_t pcap_file_header_t;
struct pcap_file_header_t {
	unsigned int	magic;
	unsigned short	version_major;
	unsigned short  version_minor;
	int				thiszone;	/* gmt to local correction */
	unsigned int	sigfigs;	/* accuracy of timestamps */
	unsigned int	snaplen;	/* max length saved portion of each pkt */
	unsigned int	linktype;	/* data link type (LINKTYPE_*) */
};

typedef struct pcap_frame_header_t pcap_frame_header_t;
struct pcap_frame_header_t {
	long			tv_sec;     /* seconds */
	long			tv_usec;    /* microseconds */
	unsigned int	caplen;		/* length of portion present */
	unsigned int	len;		/* length this packet (off wire) */
};

typedef struct mac_header_t mac_header_t;
struct mac_header_t
{
	unsigned char	dst_mac_addr[6];
	unsigned char	src_mac_addr[6];
	unsigned short  type;
	void Ntoh()
	{
		type = ntohs(type);
	}
};

typedef struct ip_header_t ip_header_t;
struct ip_header_t
{
	unsigned char	ver_len;        // 版本 + 首部长度
	unsigned char	tos;            // 服务类型
	unsigned short	len;            // 总长度
	unsigned short	id;             // 标识
	unsigned short 	flag_off;       // 3为偏移和13位片偏移
	unsigned char	ttl;            // 生存时间
	unsigned char	ptl;            // 协议
	unsigned short	sum;            // 首部校验和
	unsigned int 	src_ip;         // 源IP
	unsigned int	dst_ip;         // 目标IP
	void Ntoh()
	{
		len = ntohs(len);
		id = ntohs(id);
		flag_off = ntohs(flag_off);
		sum = ntohs(sum);
		src_ip = ntohl(src_ip);
		dst_ip = ntohl(dst_ip);
	}
};

typedef struct tcp_header_t tcp_header_t;
struct tcp_header_t
{
	unsigned short	src_port;    // 源端口
	unsigned short	dst_port;    // 目标端口
	unsigned int	seq_num;     // 报文序号
	unsigned int	ack_num;     // 确认序号
	unsigned short	off_flags;   // 数据偏移 + 标识位('s)
	unsigned short	window;      // 滑动窗口大小
	unsigned short	chk_sum;     // 校验和
	unsigned short	urg_ptr;     // 紧急指针
	void Ntoh()
	{
		src_port = ntohs(src_port);
		dst_port = ntohs(dst_port);
		seq_num = ntohl(seq_num);
		ack_num = ntohl(ack_num);
		off_flags = ntohs(off_flags);
		window = ntohs(window);
		chk_sum = ntohs(chk_sum);
		urg_ptr = ntohs(urg_ptr);
	}
};
#pragma pack()

class CPcapFile
{
public:
	CPcapFile(void);
	~CPcapFile(void);

	bool	SetPathName(string path);
	void	SetStoreFileSize(int sSize) { storeFileSize = sSize; };
	void	SetStoreFileNum(int sNum) { storeFileNum = sNum; };
	void	SetFormat7z(bool bZip,bool b7z);
	bool	SavePcapFile(string ethName,char *buffer,int length);
	bool	CachePcapFrame(string ethName,char *buffer,int length);
	bool	OpenPcapFile(string fileName);
	void	ClosePcapFile();
	int		GetFileLen(string sFile);
	void	BackupFile(string ethName);

	void	MakePcapFileHeader(pcap_file_header_t & pfh);
	bool	WritePcapFileHeader(string path);
	bool	WritePcapFileBuffer(string path,char *buffer,int length);	

private:
#ifdef WIN32
	CLzma7z		*lzma7z;
#endif
	string		m_sFile;
	FILE*		m_fpFile;
	string		storePath;
	bool		bCompress;
	bool		bFormat7z;
	int			storeFileSize;
	int			storeFileNum;
	string		exe7z;
	string		srcCompressFile;
	string		dstCompressFile;

	SLock		*m_pLock;		//线程锁
	static void* ThreadCompress(void *lp);
};

#endif	//__PCAP_FILE_H__
