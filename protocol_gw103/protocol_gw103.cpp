#include "protocol_gw103.h"

static protocol_gw103 gw103;
extern "C" PROTOCOL_GW103_EXPORT void getLibInfo(QString &ver,QString &author,QString &decs)
{
	ver = VERSION;
	author = AUTHOR;
	decs = DESCRIPTION;
}

extern "C" PROTOCOL_GW103_EXPORT bool loadParameters(QHash<quint64,QByteArray> *hash,quint64 key)
{
	gw103.setHash(hash);
	gw103.setKey(key);

	return true;
}

extern "C" PROTOCOL_GW103_EXPORT bool run(QString &content)
{
	return gw103.run(content);
}

protocol_gw103::protocol_gw103()
{

}

protocol_gw103::~protocol_gw103()
{

}

bool protocol_gw103::run(QString &content)
{
	bool ok;
	QByteArray fData;
	frame_info_t fInfo;

	ok = parseFrameByKey(m_key,fInfo);
	if (!ok)
		return ok;

	if (fInfo.ptl == TCP_PROTOCOL && (fInfo.srcPort == PORT_2404 || fInfo.dstPort == PORT_2404))
	{
		ok = getFullData(m_key,fData,&fInfo);
		if (ok)
		{
			ok = handleFullData(fData,content);
			if (!ok)
				return false;
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

bool protocol_gw103::getFullData(quint64 key,QByteArray &data,frame_info_t *fInfo)
{
	qint8 hit;
	quint64 upPos = 1;
	quint64 downPos = 1;
	QByteArray fullData = fInfo->appData;
	frame_info_t currentInfo;

	hit = isFullData(fullData);
	while (hit != FRAME_HIT)
	{
		if (hit == FRAME_UP)
		{
			if (upPos > FRAME_UP_LEVEL || (m_key - upPos) < 1)
				return false;
			if (parseFrameByKey(m_key-upPos,currentInfo))
				return false;
			if (currentInfo.ptl == fInfo->ptl &&
				currentInfo.srcIpAddr == fInfo->srcIpAddr && currentInfo.dstIpAddr == fInfo->dstIpAddr &&
				currentInfo.srcPort == fInfo->srcPort && currentInfo.dstPort == fInfo->dstPort)
			{
				fullData = currentInfo.appData + fullData;
				upPos++;
			}
		}
		else if (hit == FRAME_DOWN)
		{
			if (downPos > FRAME_DOWN_LEVEL || (m_key + downPos) > m_hash->size())
				return false;
			if (parseFrameByKey(m_key+downPos,currentInfo))
				return false;
			if (currentInfo.ptl == fInfo->ptl &&
				currentInfo.srcIpAddr == fInfo->srcIpAddr && currentInfo.dstIpAddr == fInfo->dstIpAddr &&
				currentInfo.srcPort == fInfo->srcPort && currentInfo.dstPort == fInfo->dstPort)
			{
				fullData = fullData + currentInfo.appData;
				downPos++;
			}
		}
		else
		{
			return false;
		}

		hit = isFullData(fullData);
	}

	data = fullData;

	return true;
}

qint8 protocol_gw103::isFullData(QByteArray fullData)
{
	qint8 hit = FRAME_NONE;
	char buffer[4096] = {'\0'};

	memcpy(buffer,fullData.data(),fullData.size());
	if (buffer[0] != 0x68)
	{
		hit = FRAME_UP;
	}
	else if (buffer[0] == 0x68 && fullData.size() < (MAKEWORD(buffer[1],buffer[2])+3))
	{
		hit = FRAME_DOWN;
	}
	else if (buffer[0] == 0x68 && fullData.size() == (MAKEWORD(buffer[1],buffer[2])+3))
	{
		hit = FRAME_HIT;
	}
	else
	{
		hit = FRAME_NONE; 
	}

	return hit;
}

bool protocol_gw103::handleFullData(QByteArray fData,QString &content)
{
	bool ok;
	char buffer[MBYTE1] = {'\0'};

	ok = Analyze_GW(buffer,fData.size(),fData.data());
	if (!ok)
		return false;

	content = QTextCodec::codecForName("GB18030")->toUnicode(buffer);
	return true;
}

bool protocol_gw103::parseFrameByKey(quint64 key,frame_info_t &fInfo)
{
	quint64 pos = 0;
	pcap_frame_header_t pcapFrameHeader;
	mac_header_t macHeader;
	ip_header_t ipHeader;
	tcp_header_t tcpHeader;
	udp_header_t udpHeader;

	QHash<quint64, QByteArray>::iterator ifm = m_hash->find(key);
	if (ifm == m_hash->end() || ifm.key() != key)
	{
		SWarning(QString("Cannot find frame by key = %1.").arg(m_key),__FILE__,__LINE__);
		return false;
	}

	QByteArray valBuf = ifm.value();
	memcpy(&pcapFrameHeader,valBuf.data(),sizeof(pcap_frame_header_t));
	pos += sizeof(pcap_frame_header_t);
	memcpy(&macHeader,valBuf.data()+pos,sizeof(mac_header_t));
	macHeader.Ntoh();
	pos += sizeof(mac_header_t);
	switch(macHeader.type)
	{
	case IP_TYPE:
		memcpy(&ipHeader,valBuf.data()+pos,sizeof(ip_header_t));
		ipHeader.Ntoh();
		pos += sizeof(ip_header_t);
		fInfo.srcIpAddr = ipHeader.src_ip;
		fInfo.dstIpAddr = ipHeader.dst_ip;
		switch(ipHeader.ptl)
		{
		case TCP_PROTOCOL:
			memcpy(&tcpHeader,valBuf.data()+pos,sizeof(tcp_header_t));
			tcpHeader.Ntoh();
			pos += sizeof(tcp_header_t);
			fInfo.ptl = TCP_PROTOCOL;
			fInfo.srcPort = tcpHeader.src_port;
			fInfo.dstPort = tcpHeader.dst_port;
			fInfo.appData = valBuf.mid(pos);
			break;
		case UDP_PROTOCOL:
			memcpy(&udpHeader,valBuf.data()+pos,sizeof(udp_header_t));
			udpHeader.Ntoh();
			pos += sizeof(udp_header_t);
			fInfo.ptl = UDP_PROTOCOL;
			fInfo.srcPort = udpHeader.src_port;
			fInfo.dstPort = udpHeader.dst_port;
			fInfo.appData = valBuf.mid(pos);
			break;
		}
		break;
	default:
		SWarning("Is not the TCP protocol or UDP protocol.",__FILE__,__LINE__);
		return false;
	}

	return true;
}
