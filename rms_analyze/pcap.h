#ifndef _RMS_ANALYZE_PCAP_
#define _RMS_ANALYZE_PCAP_

#define	IP_TYPE		0x0800
#define ARP_TYPE	0x0806

#define	TCP_PROTOCOL	6
#define	UDP_PROTOCOL	17

#pragma pack(1)
typedef struct pcap_file_header_t pcap_file_header_t;
struct pcap_file_header_t {
	quint32	magic;
	quint16	version_major;
	quint16 version_minor;
	qint32	thiszone;	/* gmt to local correction */
	quint32	sigfigs;	/* accuracy of timestamps */
	quint32	snaplen;	/* max length saved portion of each pkt */
	quint32	linktype;	/* data link type (LINKTYPE_*) */
};

typedef struct pcap_frame_header_t pcap_frame_header_t;
struct pcap_frame_header_t {
	long	tv_sec;     /* seconds */
	long	tv_usec;    /* microseconds */
	quint32	caplen;		/* length of portion present */
	quint32	len;		/* length this packet (off wire) */
};

typedef struct mac_header_t mac_header_t;
struct mac_header_t
{
	qint8	dst_mac_addr[6];
	qint8	src_mac_addr[6];
	quint16	type;
	void Ntoh()
	{
		type = qToBigEndian(type);
	}
};

typedef struct ver_len_t ver_len_t;
struct ver_len_t
{
	quint8 length:4;
	quint8 version:4;
};

typedef struct flag_off_t flag_off_t;
struct flag_off_t
{
	quint16 offset:13;
	quint16 flag:3;
};

typedef struct ip_header_t ip_header_t;
struct ip_header_t
{
	ver_len_t   ver_len;        // 版本 + 首部长度
	quint8		tos;            // 服务类型
	quint16		len;            // 总长度
	quint16		id;             // 标识
	quint16 	flag_off;       // 3为偏移和13位片偏移
	quint8		ttl;            // 生存时间
	quint8		ptl;            // 协议
	quint16		sum;            // 首部校验和
	quint32		src_ip;         // 源IP
	quint32		dst_ip;         // 目标IP
	void Ntoh()
	{
		len = qToBigEndian(len);
		id = qToBigEndian(id);
		flag_off = qToBigEndian(flag_off);
		sum = qToBigEndian(sum);
		src_ip = qToBigEndian(src_ip);
		dst_ip = qToBigEndian(dst_ip);
	}
};

typedef struct off_flags_t off_flags_t;
struct off_flags_t
{
	quint16 FIN:1;
	quint16 SYN:1;
	quint16 RST:1;
	quint16 PSH:1;
	quint16 ACK:1;
	quint16 URG:1;
	quint16 reserve:6;
	quint16 offset:4;
};

typedef struct offset_flags_t offset_flags_t;
struct offset_flags_t
{
	quint16 flags:12;
	quint16 offset:4;
};

typedef struct tcp_header_t tcp_header_t;
struct tcp_header_t
{
	quint16 src_port;    // 源端口
	quint16 dst_port;    // 目标端口
	quint32 seq_num;     // 报文序号
	quint32 ack_num;     // 确认序号
	quint16 off_flags;   // 数据偏移 + 标识位('s)
	quint16 window;      // 滑动窗口大小
	quint16 chk_sum;     // 校验和
	quint16 urg_ptr;     // 紧急指针
	void Ntoh()
	{
		src_port = qToBigEndian(src_port);
		dst_port = qToBigEndian(dst_port);
		seq_num = qToBigEndian(seq_num);
		ack_num = qToBigEndian(ack_num);
		off_flags = qToBigEndian(off_flags);
		window = qToBigEndian(window);
		chk_sum = qToBigEndian(chk_sum);
		urg_ptr = qToBigEndian(urg_ptr);
	}
};

typedef struct udp_header_t udp_header_t;
struct udp_header_t
{
	quint16 src_port;    // 源端口
	quint16 dst_port;    // 目的端口
	quint16 len;         // UDP长度
	quint16 chk_sum;     // 校验和
	void Ntoh()
	{
		src_port = qToBigEndian(src_port);
		dst_port = qToBigEndian(dst_port);
		len = qToBigEndian(len);
		chk_sum = qToBigEndian(chk_sum);
	}
};

typedef struct arp_data_t arp_data_t;
struct arp_data_t
{
	quint16 hardware_type;		// 硬件类型
	quint16 protocol_type;		// 协议类型
	quint8  hardware_size;		// 硬件长度
	quint8  protocol_size;		// 协议长度
	quint16 oper_code;			// 操作码
	qint8	sender_mac_addr[6];	// 发送者硬件地址
	quint32	sender_ip;			// 发送者IP地址
	qint8	target_mac_addr[6]; // 目的硬件地址
	quint32	target_ip;          // 目的IP地址
	void Ntoh()
	{
		hardware_type = qToBigEndian(hardware_type);
		protocol_type = qToBigEndian(protocol_type);
		oper_code = qToBigEndian(oper_code);
		sender_ip = qToBigEndian(sender_ip);
		target_ip = qToBigEndian(target_ip);
	}
};
#pragma pack()

#endif // _RMS_ANALYZE_PCAP_
