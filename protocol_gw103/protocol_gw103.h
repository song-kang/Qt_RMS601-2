#ifndef PROTOCOL_GW103_H
#define PROTOCOL_GW103_H

#include "protocol_gw103_global.h"
#include "pcap.h"
#include "AnalyzeGW103.h"

#define VERSION				"1.0.0"	//只可以写三位
#define AUTHOR				QObject::tr("SongKang")
#define DESCRIPTION			QObject::tr("Relay protection and fault information protocol for State Grid 103.")

#define PORT_2404			2404

#define FRAME_NONE			0
#define FRAME_HIT			1
#define FRAME_UP			2
#define FRAME_DOWN			3

#define FRAME_UP_LEVEL		5
#define FRAME_DOWN_LEVEL	5

#define	MBYTE1				(1024*1024)

static void SDebug(QString info,QString file,int line)
{
	qDebug("File:(%s) Line:(%d): %s",file.toLatin1().data(),line,info.toLatin1().data());
}

static void SWarning(QString info,QString file,int line)
{
	qWarning("File:(%s) Line:(%d): %s",file.toLatin1().data(),line,info.toLatin1().data());
}

static void SCritical(QString info,QString file,int line)
{
	qCritical("File:(%s) Line:(%d): %s",file.toLatin1().data(),line,info.toLatin1().data());
}

static void SFatal(QString info,QString file,int line)
{
	qFatal("File:(%s) Line:(%d): %s",file.toLatin1().data(),line,info.toLatin1().data());
}

typedef struct frame_info_t frame_info_t;
struct frame_info_t
{
	quint8  ptl;
	quint32 srcIpAddr;
	quint32 dstIpAddr;
	quint16 srcPort;
	quint16 dstPort;
	QByteArray appData;
};

class PROTOCOL_GW103_EXPORT protocol_gw103
{
public:
	protocol_gw103();
	~protocol_gw103();

	void setHash(QHash<quint64,QByteArray> *hash) { m_hash = hash; };
	void setKey(quint64 key) { m_key = key; };
	bool run(QString &content);

private:
	bool parseFrameByKey(quint64 key,frame_info_t &fInfo);
	bool getFullData(quint64 key,QByteArray &data,frame_info_t *fInfo);
	bool handleFullData(QByteArray fData,QString &content);
	qint8 isFullData(QByteArray fullData);

private:
	QHash<quint64,QByteArray> *m_hash;
	quint64	m_key;

};

#endif // PROTOCOL_GW103_H
