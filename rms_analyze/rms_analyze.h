#ifndef RMS_ANALYZE_H
#define RMS_ANALYZE_H

#include <QWidget>
#include <QtGui>
#include <QtGui/QMainWindow>
#include <QHostAddress>
#include <QtNetwork>
#include "pcap.h"
#include "ui_rms_analyze.h"
#include "filter.h"
#include "dlgconnect.h"
#include "database.h"
#include "dlgoption.h"
#include "dlgabout.h"
#include "dlgemail.h"
#include "saveframe.h"

#define	OPEN			0
#define	OPEN_ADD		1

#define	NO_FILTER		0
#define	PEER_SRC_DST	1
#define FIX_SRC			2

#define	MAX_OPEN_FILE		5
#define	MAX_FRAME_NUM		500000
#define	MAX_NET_FRAME_NUM	100000

#define COMMFRAMESYN	0x09D7

#define FTP_PORT			21
#define SSH_PORT			22
#define TELNET_PORT			23
#define HTTP_PORT			80
#define HTTPS_PORT			443
#define REMOTE_PORT_2404	2404
#define REMOTE_PORT_2405	2405

#pragma pack(1)
typedef struct tree_expand_flag_t tree_expand_flag_t;
struct tree_expand_flag_t
{
	quint8 allFrame:1;
	quint8 macHeader:1;
	quint8 internetHeader:1;
	quint8 transportHeader:1;
	quint8 resever:4;
};

typedef struct commframe_header_t commframe_header_t;
struct commframe_header_t {
	unsigned short	syn;
	unsigned int	len;
};
#pragma pack()

class rms_analyze : public QMainWindow
{
	Q_OBJECT

public:
	rms_analyze(QWidget *parent = 0, Qt::WFlags flags = 0);
	~rms_analyze();

	void createFilterTools();
	void initTreeView();
	void initTreeWidget();
	void initTableWidget();
	void initTextBrower();
	void initDatabase();
	void initProtocolLib();
	void subFrame(QByteArray fileBuffer,quint8 openFlag,bool isProgress);
	void processFrame(quint8 sFilter,quint8 openFlag,bool isProgress);
	void processPcapFrameHeader(quint64 order,pcap_frame_header_t &pcapFrameHeader);
	void processIpHeader(quint64 order,ip_header_t &ipHeader,QString strSrcPort,QString strDstPort);
	void processFrameData(quint64 order,QByteArray sData);
	void processFrameTree(ip_header_t &ipHeader,QString strSrcPort,QString strDstPort);
	void processFrameIpTcp(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader,
		ip_header_t &ipHeader,tcp_header_t &tcpHeader,QByteArray sData);
	void processFrameIpUdp(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader,
		ip_header_t &ipHeader,udp_header_t &udpHeader,QByteArray sData);
	void processFrameArp(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader,
		mac_header_t &macHeader,QByteArray sData);
	void processFrameUnkown(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader);
	void showFrame(quint64 sNo,QByteArray sFrame);
	void showFrameIpTcp(ip_header_t &ipHeader,tcp_header_t &tcpHeader,QByteArray sData);
	void showFrameIpUdp(ip_header_t &ipHeader,udp_header_t &udpHeader,QByteArray sData);
	void showFrameArp(arp_data_t &aprData,QByteArray sData);
	void showFrameAppData(QByteArray sData);
	void showParseFrameByLib(quint64 sNo);
	void anaylzePcapFrameHeader(quint64 sNo,pcap_frame_header_t &pcapFrameHeader);
	void anaylzeMacHeader(mac_header_t &macHeader);
	void anaylzeIpHeader(ip_header_t &ipHeader);
	void anaylzeIpHeaderFlagOffset(QStandardItem *current,quint16 val,quint16 len);
	void anaylzeTcpHeader(tcp_header_t &tcpHeader);
	void anaylzeTcpHeaderOffsetFlags(QStandardItem *current,quint16 val,quint16 len);
	void anaylzeUdpHeader(udp_header_t &udpHeader);

////////////////// Auxiliary interface /////////////////////////
	QString _ByteToHexString(QByteArray sData);
	QString _ByteToHexText(QByteArray sData);
	QString _ByteToAscText(QByteArray sData);
	QString _MacAddrToString(qint8 *addr);
	bool    _SatisfyFilter(quint8 sFilter,quint32 sIpaddr,quint16 sPort,quint32 dIpaddr,quint16 dPort);
	bool	_OpenFile();
	QString _JudgeProtocolByPort(quint16 srcPort,quint16 dstPort);

public:
	void setTranslator(QTranslator *trans) { translator = trans; };
	void setLanguage(int langu) { current_language = langu; };

private:
	QToolBar		*filterToolBar;
	QLabel			*filterLabel;
	QComboBox		*filterComboBox;
	QPushButton     *filterApplyBtn;
	QPushButton     *filterClearBtn;

private:
	int				loadLibNum;
	lib_plugin_t	*protocolib;
	QTranslator		*translator;
	int				current_language;

private:
	QTreeWidgetItem *rootItem;
	QHash<quint64,QByteArray> frameHash;
	QHash<quint64,QByteArray> frameHashTemp;
	QHash<QString,QString> treeHash;
	QProgressDialog progressDialog;
	QStandardItemModel *model;
	QList<QString> fileNameList;
	QByteArray fileBuffer;
	quint32 tableNo;
	quint64 numKey;
	tree_expand_flag_t treeExpandFlag;
	QTreeWidgetItem *treeWidgetItemBak;

	QString hostName;
	int hostPort;
	QTcpSocket *tcpSocket;
	QByteArray commArray;
	bool bStop;

	CDatabase *appDb;
	CSaveFrame saveFrame;
	bool bZipFile;

	quint8 filterFlags;
	quint32 srcAddr;
	quint32 dstAddr;
	quint16 srcPort;
	quint16 dstPort;
	filter *m_Filter;

private:
	Ui::rms_analyzeClass ui;

public:
	CDatabase *getDatabase() { return appDb; };

public slots:
	void slotOpen();
	void slotOpenEx();
	void slotClose();
	void slotConnect();
	void slotDisconnect();
	void slotStart();
	void slotStop();
	void slotOption();
	void slotEmail();
	void slotAbout();
	void slotTableClicked(int row,int column);
	void slotTreeViewExpanded(const QModelIndex &index);
	void slotTreeViewCollapsed(const QModelIndex &index);
	void slotTreeWigetItemClicked(QTreeWidgetItem *item,int column);
	void slotComboBoxEditTextChanged(const QString &strEdit);
	void slotConnected();
	void slotDisconnected();
	void slotReadMessage();
	void slotDisplayError(QAbstractSocket::SocketError);

private:
	typedef bool (*LoadParameters)(QHash<quint64,QByteArray> *,quint64);
	typedef bool (*Run)(QString &content);
};

#endif // RMS_ANALYZE_H
