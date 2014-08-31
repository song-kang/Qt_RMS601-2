#include "rms_analyze.h"

rms_analyze::rms_analyze(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	initDatabase();
//	createFilterTools();
	initTreeView();
	initTreeWidget();
	initTableWidget();
	initTextBrower();
	initProtocolLib();

	numKey = 1;
	tableNo = 0;
	filterFlags = NO_FILTER;
	bStop = false;
	setWindowIcon(QIcon(":/images/logo.png"));
	memset(&treeExpandFlag,0,sizeof(tree_expand_flag_t));
	progressDialog.setModal(true);
	ui.statusBar->showMessage(tr("Welcome to use NetShark software."));
	ui.actionDisconnect->setEnabled(false);
	ui.actionStart->setEnabled(false);
	ui.actionStop->setEnabled(false);
	QFile::remove("NetShark.tmp");
	QFile::remove("version");

	connect(ui.actionOpen,SIGNAL(triggered(bool)),this,SLOT(slotOpen()));
	connect(ui.actionOpenEx,SIGNAL(triggered(bool)),this,SLOT(slotOpenEx()));
	connect(ui.actionClose,SIGNAL(triggered(bool)),this,SLOT(slotClose()));
	connect(ui.actionConnect,SIGNAL(triggered(bool)),this,SLOT(slotConnect()));
	connect(ui.actionDisconnect,SIGNAL(triggered(bool)),this,SLOT(slotDisconnect()));
	connect(ui.actionStart,SIGNAL(triggered(bool)),this,SLOT(slotStart()));
	connect(ui.actionStop,SIGNAL(triggered(bool)),this,SLOT(slotStop()));
	connect(ui.actionOption,SIGNAL(triggered(bool)),this,SLOT(slotOption()));
	connect(ui.actionEmail,SIGNAL(triggered(bool)),this,SLOT(slotEmail()));
	connect(ui.actionAbout,SIGNAL(triggered(bool)),this,SLOT(slotAbout()));
	connect(ui.actionQuit,SIGNAL(triggered(bool)),this,SLOT(close()));
	connect(ui.tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slotTableClicked(int,int)));
	connect(ui.treeView,SIGNAL(expanded(const QModelIndex &)),this,SLOT(slotTreeViewExpanded(const QModelIndex &)));
	connect(ui.treeView,SIGNAL(collapsed(const QModelIndex &)),this,SLOT(slotTreeViewCollapsed(const QModelIndex &)));
	connect(ui.treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(slotTreeWigetItemClicked(QTreeWidgetItem *,int)));
//	connect(filterComboBox,SIGNAL(editTextChanged(const QString &)),this,SLOT(slotComboBoxEditTextChanged(const QString &)));
}

rms_analyze::~rms_analyze()
{
	int i;
	frameHash.clear();
	treeHash.clear();
	delete model;
	appDb->getDatabase().close();
	delete appDb;
	for (i = 0; i < loadLibNum; i++)
        protocolib[i].lib->unload();
	delete []protocolib;
}

void rms_analyze::createFilterTools()
{
	m_Filter = new filter(this);
	QFont font("Courier New");
	filterLabel = new QLabel;
	filterLabel->setText(tr("Filter: "));
	filterLabel->setFont(font);

	filterComboBox = new QComboBox;
	filterComboBox->setFixedWidth(400);
	filterComboBox->setFixedHeight(28);
	filterComboBox->view()->setFixedWidth(600);
	filterComboBox->setFont(font);
	filterComboBox->setEditable(true);
	m_Filter->setCompleter(filterComboBox->lineEdit());

	filterClearBtn = new QPushButton;
	filterClearBtn->setIcon(QIcon(":/images/trash.png"));
	filterClearBtn->setToolTip(tr("clear"));
	filterClearBtn->setFont(font);

	filterApplyBtn = new QPushButton;
	filterApplyBtn->setIcon(QIcon(":/images/ok.png"));
	filterApplyBtn->setToolTip(tr("apply"));
	filterApplyBtn->setFont(font);
	filterApplyBtn->setEnabled(false);
	
	filterToolBar = addToolBar(tr("Filter Tool"));
	filterToolBar->addWidget(filterLabel);
	filterToolBar->addWidget(filterComboBox);
	filterToolBar->addWidget(filterApplyBtn);
	filterToolBar->addWidget(filterClearBtn);
}

void rms_analyze::initTreeView()
{
	QFont font("Courier New",9);
	ui.treeView->setFont(font);
	ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.treeView->setHeaderHidden(true); //隐藏表头
	ui.treeView->header()->setResizeMode(QHeaderView::ResizeToContents); //出现水平滚动条
	ui.treeView->header()->setStretchLastSection(false); //出现水平滚动条
	model = new QStandardItemModel(ui.treeView);
}

void rms_analyze::initTreeWidget()
{
	QIcon rootIcon;
	QFont font("Courier New",9);
	ui.treeWidget->setFont(font);
	ui.treeWidget->setMaximumWidth(380);
	ui.treeWidget->clear();
	rootItem = new QTreeWidgetItem(ui.treeWidget,NULL);
	rootItem->setText(0,tr("Machine"));
	rootIcon = QIcon(":/images/monitor.png");
	rootItem->setIcon(0,rootIcon);
	ui.treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents); //出现水平滚动条
	ui.treeWidget->header()->setStretchLastSection(false); //出现水平滚动条
}

void rms_analyze::initTableWidget()
{
	QFont font("Courier New",9);
	ui.tableWidget->setFont(font);
	ui.tableWidget->resize(0,330);
	ui.tableWidget->setColumnWidth(0,60);
	ui.tableWidget->setColumnWidth(1,200);
	ui.tableWidget->setColumnWidth(2,160);
	ui.tableWidget->setColumnWidth(3,160);
	ui.tableWidget->setColumnWidth(4,100);
	ui.tableWidget->setColumnWidth(5,50);
	ui.tableWidget->setColumnWidth(6,550);
	//ui.tableWidget->horizontalHeader()->setStretchLastSection(true); //最后一列自动拉伸
	ui.tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch); //自动调整大小
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择模式
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//不可编辑
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单选模式
	ui.tableWidget->setFocusPolicy(Qt::NoFocus); //去除焦点，无虚框
	ui.tableWidget->verticalHeader()->setVisible(false); //去除最前列
}

void rms_analyze::initTextBrower()
{
	QFont font("Courier New",9);
	ui.textBrowser->setFont(font);
	ui.textBrowser->resize(320,0);
	ui.textBrowser->setLineWrapMode(QTextEdit::NoWrap); //出现水平滚动条
}

void rms_analyze::initDatabase()
{
	int value;
	appDb = new CDatabase(this);
	if (!appDb->createDatabase("config.db"))
		exit(0);
	else
	{
		if (!appDb->existOptionLanguage())
			appDb->saveOptionLanguage(LANGUAGE_VAL_CH);
		if (!appDb->existOptionZipMode())
			appDb->saveOptionZipMode(ZIPMODE_VAL_ZERO);
		else
		{
			appDb->getOptionZipMode(value);
			switch (value)
			{
			case ZIPMODE_VAL_ZERO:
				bZipFile = false;
				break;
			case ZIPMODE_VAL_VAILD:
				bZipFile = true;
				break;
			default:
				bZipFile = false;
				break;
			}
		}
	}
}

void rms_analyze::initProtocolLib()
{
	int i;
	QString name;
	QFileInfo fileInfo;

	loadLibNum = 0;
	QDir dllDir(CSkStaticClass::GetCurrentPath()+"protocol");
	QStringList fList = dllDir.entryList();
	for (i = 0; i < fList.size();)
	{
		name = fList.at(i);
		fileInfo.setFile(name);
		if (fileInfo.suffix() != "dll" && fileInfo.suffix() != "DLL")
			fList.removeAt(i);
		else if (name.left(9) != "protocol_" && name.left(9) != "PROTOCOL_")
			fList.removeAt(i);
		else
			i++;
	}

	if (fList.isEmpty())
	{
		CSkStaticClass::SWarning(QString("Cannot find parse protocol library."),__FILE__,__LINE__);
		return;
	}

	loadLibNum = fList.size();
	protocolib = new lib_plugin_t[loadLibNum];
	for (i = 0; i < loadLibNum; i++)
	{
		protocolib[i].lib = new QLibrary(CSkStaticClass::GetCurrentPath()+"protocol/"+fList.at(i));
		if (!protocolib[i].lib->load())
		{
			CSkStaticClass::SWarning(QString("Load %1 parse protocol library error.").arg(fList.at(i)),__FILE__,__LINE__);
			continue;
		}
		else
		{
			protocolib[i].name = fList.at(i);
		}
	}
}

void rms_analyze::subFrame(QByteArray fileBuffer,quint8 openFlag,bool isProgress)
{
	QByteArray	tmp;
	quint64 pos = 0;
	pcap_frame_header_t hFrame;
	QHash<quint64,QByteArray> *m_hash;

	if (openFlag == OPEN)
	{
		numKey = 1;
		frameHash.clear();
		frameHashTemp.clear();
		treeHash.clear();
		initTreeWidget();
		m_hash = &frameHash;
	}
	else if (openFlag == OPEN_ADD)
	{
		frameHashTemp.clear();
		m_hash = &frameHashTemp;
	}

	tmp = fileBuffer.mid(pos,sizeof(pcap_file_header_t));
	pos += sizeof(pcap_file_header_t);

	while(pos < fileBuffer.size())
    {
		tmp = fileBuffer.mid(pos,sizeof(pcap_frame_header_t));
		memcpy(&hFrame,tmp.data(),sizeof(pcap_frame_header_t));
		tmp = fileBuffer.mid(pos,sizeof(pcap_frame_header_t) + hFrame.len);
		m_hash->insert(numKey,tmp);
		numKey++;
		pos += sizeof(pcap_frame_header_t) + hFrame.len;
	}
	fileBuffer.clear();
	processFrame(filterFlags,openFlag,isProgress);
}

void rms_analyze::processFrame(quint8 sFilter,quint8 openFlag,bool isProgress)
{
	quint64	sNo = 0;
	QByteArray sFrame;
	QByteArray sData;
	quint64 pos = 0;
	pcap_frame_header_t pcapFrameHeader;
	mac_header_t macHeader;
	ip_header_t ipHeader;
	tcp_header_t tcpHeader;
	udp_header_t udpHeader;
	QHash<quint64,QByteArray> *m_hash;
	QHash<quint64,QByteArray>::const_iterator at;
    int hashSize;

	if (openFlag == OPEN)
	{
		if (!frameHash.size())
			return;
		tableNo = 0;
		ui.tableWidget->setRowCount(0);
		ui.tableWidget->clearContents();
		m_hash = &frameHash;
		hashSize = frameHash.size();
	}
	else if (openFlag == OPEN_ADD)
	{
		if (!frameHashTemp.size())
			return;
		m_hash = &frameHashTemp;
		hashSize = frameHash.size()+frameHashTemp.size();
	}

	if (sFilter == NO_FILTER )
		ui.tableWidget->setRowCount(hashSize);
	
	if (isProgress)
	{
		progressDialog.setLabelText(tr("process frame, please wait..."));
		progressDialog.setRange(0,hashSize);
		progressDialog.setValue(1); //避免进度值小于1000时，进度栏不刷新问题
		progressDialog.show();
	}
	
	int number = m_hash->size();
	while(number)
	{
		at = m_hash->find(numKey - number);
		number--;
		sNo = at.key();
		sFrame = at.value();
		if (openFlag == OPEN_ADD)
			frameHash.insert(sNo,sFrame); //如果是增加属性，临时哈希数据放入完整帧哈希数据中
		pos = 0;
		memcpy(&pcapFrameHeader,sFrame.data(),sizeof(pcap_frame_header_t));
		pos += sizeof(pcap_frame_header_t);
		memcpy(&macHeader,sFrame.data()+pos,sizeof(mac_header_t));
		macHeader.Ntoh();
		pos += sizeof(mac_header_t);
		switch(macHeader.type)
		{
		case IP_TYPE:
			memcpy(&ipHeader,sFrame.data()+pos,sizeof(ip_header_t));
			ipHeader.Ntoh();
			pos += sizeof(ip_header_t);
			switch(ipHeader.ptl)
			{
			case TCP_PROTOCOL:
				memcpy(&tcpHeader,sFrame.data()+pos,sizeof(tcp_header_t));
				tcpHeader.Ntoh();
				pos += sizeof(tcp_header_t);
				sData = sFrame.mid(pos);
				processFrameIpTcp(sFilter,sNo,pcapFrameHeader,ipHeader,tcpHeader,sData);
				break;
			case UDP_PROTOCOL:
				memcpy(&udpHeader,sFrame.data()+pos,sizeof(udp_header_t));
				udpHeader.Ntoh();
				pos += sizeof(udp_header_t);
				sData = sFrame.mid(pos);
				processFrameIpUdp(sFilter,sNo,pcapFrameHeader,ipHeader,udpHeader,sData);
				break;
			default:
				processFrameUnkown(sFilter,sNo,pcapFrameHeader);
				break;
			}
			break;
		case ARP_TYPE:
			sData = sFrame.mid(pos);
			processFrameArp(sFilter,sNo,pcapFrameHeader,macHeader,sData);
			break;
		default:
			processFrameUnkown(sFilter,sNo,pcapFrameHeader);
			break;
		}

		if (isProgress && (!(sNo % 1000) || sNo == hashSize))
			progressDialog.setValue(sNo);
	}
	ui.tableWidget->scrollToBottom();
}

void rms_analyze::processFrameIpTcp(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader,
									ip_header_t &ipHeader,tcp_header_t &tcpHeader,QByteArray sData)
{
	if (!_SatisfyFilter(sFilter,ipHeader.src_ip,tcpHeader.src_port,ipHeader.dst_ip,tcpHeader.dst_port))
		return;

	if (sFilter != NO_FILTER)
		ui.tableWidget->insertRow(tableNo);

	QString strSrcPort = QString::number(tcpHeader.src_port,10);
	QString strDstPort = QString::number(tcpHeader.dst_port,10);
	
	QString strProtocol = _JudgeProtocolByPort(tcpHeader.src_port,tcpHeader.dst_port);
	QTableWidgetItem * col_Protocol = new QTableWidgetItem(strProtocol);
	ui.tableWidget->setItem(tableNo,4,col_Protocol);

	processPcapFrameHeader(order,pcapFrameHeader);
	processIpHeader(order,ipHeader,strSrcPort,strDstPort);
	processFrameData(order,sData);
	processFrameTree(ipHeader,strSrcPort,strDstPort);
	tableNo++;
}

void rms_analyze::processFrameIpUdp(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader,
									ip_header_t &ipHeader,udp_header_t &udpHeader,QByteArray sData)
{
	if (!_SatisfyFilter(sFilter,ipHeader.src_ip,udpHeader.src_port,ipHeader.dst_ip,udpHeader.dst_port))
		return;

	if (sFilter != NO_FILTER)
		ui.tableWidget->insertRow(tableNo);

	QString strSrcPort = QString::number(udpHeader.src_port,10);
	QString strDstPort = QString::number(udpHeader.dst_port,10);

	QTableWidgetItem * col_Protocol = new QTableWidgetItem("UDP");
	ui.tableWidget->setItem(tableNo,4,col_Protocol);

	processPcapFrameHeader(order,pcapFrameHeader);
	processIpHeader(order,ipHeader,strSrcPort,strDstPort);
	processFrameData(order,sData);
	processFrameTree(ipHeader,strSrcPort,strDstPort);
	tableNo++;
}

void rms_analyze::processFrameArp(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader,
								  mac_header_t &macHeader,QByteArray sData)
{
	if (sFilter)
		return;

	if (sFilter != NO_FILTER)
		ui.tableWidget->insertRow(tableNo);

	QString strSrcMacAddr = _MacAddrToString(macHeader.src_mac_addr).toUpper();
	QString strDstMacAddr = _MacAddrToString(macHeader.dst_mac_addr).toUpper();

	processPcapFrameHeader(order,pcapFrameHeader);

	QTableWidgetItem * col_SrcIp = new QTableWidgetItem(strSrcMacAddr);
	ui.tableWidget->setItem(tableNo,2,col_SrcIp);
	QTableWidgetItem * col_DstIP = new QTableWidgetItem(strDstMacAddr);
	ui.tableWidget->setItem(tableNo,3,col_DstIP);
	QTableWidgetItem * col_Protocol = new QTableWidgetItem("ARP");
	ui.tableWidget->setItem(tableNo,4,col_Protocol);
	
	processFrameData(order,sData);
	tableNo++;
}

void rms_analyze::processFrameUnkown(quint8 sFilter,quint64 order,pcap_frame_header_t &pcapFrameHeader)
{
	if (sFilter)
		return;

	if (sFilter != NO_FILTER)
		ui.tableWidget->insertRow(tableNo);

	processPcapFrameHeader(order,pcapFrameHeader);
	tableNo++;
}

void rms_analyze::processPcapFrameHeader(quint64 order,pcap_frame_header_t &pcapFrameHeader)
{
	QTableWidgetItem * col_No = new QTableWidgetItem(QString::number(order,10));
	ui.tableWidget->setItem(tableNo,0,col_No);
	QDateTime dTime = QDateTime::fromTime_t(pcapFrameHeader.tv_sec);
	QString strTime = dTime.toString("yyyy-MM-dd hh:mm:ss ");
	strTime += QString("%1").arg(pcapFrameHeader.tv_usec, 6, 10, QChar('0'));
	QTableWidgetItem * col_Time = new QTableWidgetItem(strTime);
	ui.tableWidget->setItem(tableNo,1,col_Time);
}

void rms_analyze::processIpHeader(quint64 order,ip_header_t &ipHeader,QString strSrcPort,QString strDstPort)
{
	QHostAddress srcAddr(ipHeader.src_ip);
	QTableWidgetItem * col_SrcIp = new QTableWidgetItem(srcAddr.toString()+":"+strSrcPort);
	ui.tableWidget->setItem(tableNo,2,col_SrcIp);
	QHostAddress dstAddr(ipHeader.dst_ip);
	QTableWidgetItem * col_DstIP = new QTableWidgetItem(dstAddr.toString()+":"+strDstPort);
	ui.tableWidget->setItem(tableNo,3,col_DstIP);
}

void rms_analyze::processFrameData(quint64 order,QByteArray sData)
{
	QTableWidgetItem * col_Length = new QTableWidgetItem(QString::number(sData.size(),10));
	ui.tableWidget->setItem(tableNo,5,col_Length);
	QString strData = _ByteToHexString(sData);
	QTableWidgetItem * col_Info = new QTableWidgetItem(strData);
	ui.tableWidget->setItem(tableNo,6,col_Info);
}

void rms_analyze::processFrameTree(ip_header_t &ipHeader,QString strSrcPort,QString strDstPort)
{
	QTreeWidgetItem *ipLevel1;
	QTreeWidgetItem *ipLevel2;
	QString	strLevel1;
	QString	strLevel2;

	QHostAddress srcAddr(ipHeader.src_ip);
	QHostAddress dstAddr(ipHeader.dst_ip);
	strLevel1 = srcAddr.toString()+":"+strSrcPort+" - "+dstAddr.toString()+":"+strDstPort;
	strLevel2 = dstAddr.toString()+":"+strDstPort+" - "+srcAddr.toString()+":"+strSrcPort;

	QHash<QString, QString>::iterator itH1,itH2;
	itH1 = treeHash.find(strLevel1);
	if (itH1 != treeHash.end())
		return;
	else 
	{
		itH2 = treeHash.find(strLevel2);
		if (itH2 != treeHash.end())
			return;
	}

	ipLevel1 = new QTreeWidgetItem(rootItem);
	ipLevel1->setIcon(0,QIcon(":/images/ptop.png"));
	ipLevel1->setText(0,strLevel1);
	ipLevel1->setData(0,Qt::UserRole,QVariant(strLevel1));
	ipLevel2 = new QTreeWidgetItem(ipLevel1);
	strLevel2 = srcAddr.toString()+":"+strSrcPort;
	ipLevel2->setIcon(0,QIcon(":/images/local.png"));
	ipLevel2->setText(0,strLevel2);
	ipLevel2->setData(0,Qt::UserRole,QVariant(strLevel2));
	ipLevel2 = new QTreeWidgetItem(ipLevel1);
	strLevel2 = dstAddr.toString()+":"+strDstPort;
	ipLevel2->setIcon(0,QIcon(":/images/local.png"));
	ipLevel2->setText(0,strLevel2);
	ipLevel2->setData(0,Qt::UserRole,QVariant(strLevel2));
	ui.treeWidget->setItemExpanded(rootItem,true);

	treeHash.insert(strLevel1,strLevel1);
	treeHash.insert(strLevel2,strLevel2);
}

void rms_analyze::showFrame(quint64 sNo,QByteArray sFrame)
{
	QByteArray sData;
	quint64 pos = 0;
	pcap_frame_header_t pcapFrameHeader;
	mac_header_t macHeader;
	ip_header_t ipHeader;
	tcp_header_t tcpHeader;
	udp_header_t udpHeader;
	arp_data_t arpData;

	memcpy(&pcapFrameHeader,sFrame.data(),sizeof(pcap_frame_header_t));
	pos += sizeof(pcap_frame_header_t);
	memcpy(&macHeader,sFrame.data()+pos,sizeof(mac_header_t));
	macHeader.Ntoh();
	pos += sizeof(mac_header_t);
	anaylzePcapFrameHeader(sNo,pcapFrameHeader);
	anaylzeMacHeader(macHeader);
	switch(macHeader.type)
	{
	case IP_TYPE:
		memcpy(&ipHeader,sFrame.data()+pos,sizeof(ip_header_t));
		ipHeader.Ntoh();
		pos += sizeof(ip_header_t);
		switch(ipHeader.ptl)
		{
		case TCP_PROTOCOL:
			memcpy(&tcpHeader,sFrame.data()+pos,sizeof(tcp_header_t));
			tcpHeader.Ntoh();
			pos += sizeof(tcp_header_t);
			sData = sFrame.mid(pos);
			showFrameIpTcp(ipHeader,tcpHeader,sData);
			break;
		case UDP_PROTOCOL:
			memcpy(&udpHeader,sFrame.data()+pos,sizeof(udp_header_t));
			udpHeader.Ntoh();
			pos += sizeof(udp_header_t);
			sData = sFrame.mid(pos);
			showFrameIpUdp(ipHeader,udpHeader,sData);
			break;
		}
		break;
	case ARP_TYPE:
		memcpy(&arpData,sFrame.data()+pos,sizeof(arp_data_t));
		arpData.Ntoh();
		pos += sizeof(arp_data_t);
		sData = sFrame.mid(pos);
		showFrameArp(arpData,sData);
		break;
	}
}

void rms_analyze::showFrameIpTcp(ip_header_t &ipHeader,tcp_header_t &tcpHeader,QByteArray sData)
{
	anaylzeIpHeader(ipHeader);
	anaylzeTcpHeader(tcpHeader);
	showFrameAppData(sData);
}

void rms_analyze::showFrameIpUdp(ip_header_t &ipHeader,udp_header_t &udpHeader,QByteArray sData)
{
	anaylzeIpHeader(ipHeader);
	anaylzeUdpHeader(udpHeader);
	showFrameAppData(sData);
}

void rms_analyze::showFrameArp(arp_data_t &arpData,QByteArray sData)
{
	QString str;
	QStandardItem* itemRoot;
	QStandardItem* itemLevel1;
	QHostAddress senderAddr(arpData.sender_ip);
	QHostAddress targetAddr(arpData.target_ip);

	str = "Address Resolution protocol ";
	if (arpData.oper_code == 1)
		str += "(request)";
	else if (arpData.oper_code == 2)
		str += "(reply)"; 
	else if (arpData.oper_code == 3)
		str += "(RARP request)"; 
	else if (arpData.oper_code == 4)
		str += "(RARP reply)"; 
	itemRoot = new QStandardItem(str);
	model->appendRow(itemRoot);
	str.clear();
	str = "Hareware type: ";
	if (arpData.hardware_type == 1)
		str += "Ethernet ("+QString::number(arpData.hardware_type,10)+")";
	else
		str += "Unknow ("+QString::number(arpData.hardware_type,10)+")";
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Protocol type: ";
	if (arpData.protocol_type == IP_TYPE)
		str += "IP (0x"+QString("%1").arg(arpData.protocol_type, 4, 16, QChar('0'))+")";
	else
		str += "Unknow ("+QString("%1").arg(arpData.protocol_type, 4, 16, QChar('0'))+")";
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Hareware size: "+QString::number(arpData.hardware_size,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Protocol size: "+QString::number(arpData.protocol_size,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "opcode: ";
	if (arpData.oper_code == 1)
		str += "request ("+QString::number(arpData.oper_code,10)+")";
	else if (arpData.oper_code == 2)
		str += "reply ("+QString::number(arpData.oper_code,10)+")"; 
	else if (arpData.oper_code == 3)
		str += "RARP request ("+QString::number(arpData.oper_code,10)+")"; 
	else if (arpData.oper_code == 4)
		str += "RARP reply ("+QString::number(arpData.oper_code,10)+")"; 
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Sender MAC address: "+_MacAddrToString(arpData.sender_mac_addr);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Sender IP address: "+senderAddr.toString();
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Target MAC address: "+_MacAddrToString(arpData.target_mac_addr);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Target IP address: "+targetAddr.toString();
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);

	ui.treeView->setModel(model);
	if (treeExpandFlag.internetHeader)
		ui.treeView->expand(ui.treeView->model()->index(2,0));
}

void rms_analyze::showFrameAppData(QByteArray sData)
{
	QByteArray data;
	QString	strData;

	int num = sData.size() % 16 ? sData.size() / 16 + 1 : sData.size() / 16;
	for (int i = 0,pos = 0; i < num; i++,pos += 16)
	{
		data = (sData.mid(pos,16));
		strData += QString("%1").arg(pos, 4, 16, QChar('0'));
		strData += "  ";
		strData += _ByteToHexText(data);
		strData += _ByteToAscText(data);
		ui.textBrowser->append(strData);
		strData.clear();
	}
	ui.textBrowser->moveCursor(QTextCursor::Start);
}

void rms_analyze::showParseFrameByLib(quint64 sNo)
{
	int i;

	for (i = 0; i < loadLibNum; i++)
	{
		LoadParameters loadparam = (LoadParameters)protocolib[i].lib->resolve("loadParameters");
		if (!loadparam)
		{
			CSkStaticClass::SWarning(QString("Load library function:(loadParameters) error %1!")
				.arg(protocolib[i].lib->errorString()),__FILE__,__LINE__);
			continue;
		}
		Run run = (Run)protocolib[i].lib->resolve("run");
		if (!run)
		{
			CSkStaticClass::SWarning(QString("Load library function:(run) error!"),__FILE__,__LINE__);
			continue;
		}

		if (loadparam(&frameHash,sNo))
		{
			QString content;
			if(run(content))
			{
				ui.textEdit->setText(content);
				ui.textEdit->moveCursor(QTextCursor::Start);
				break;
			}
		}	
	}
	if (i >= loadLibNum)
		ui.textEdit->setText(tr("Cannot resolve the message.")+("\n")+tr("Reson: Did not find a matching parsing module"));
}

void rms_analyze::anaylzePcapFrameHeader(quint64 sNo,pcap_frame_header_t &pcapFrameHeader)
{
	QString str;
	QStandardItem* itemRoot;
	QStandardItem* itemLevel1;

	str = "Frame "+QString::number(sNo,10)+": ";
	str += QString::number(pcapFrameHeader.len,10)+" bytes on wire, ";
	str += QString::number(pcapFrameHeader.caplen,10)+" bytes on captured";
	itemRoot = new QStandardItem(str);
	model->appendRow(itemRoot);
	str.clear();
	str = "Arrival Time: ";
	QDateTime dTime = QDateTime::fromTime_t(pcapFrameHeader.tv_sec);
	str += dTime.toString("yyyy-MM-dd hh:mm:ss ");
	str += QString("%1").arg(pcapFrameHeader.tv_usec, 6, 10, QChar('0'));
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Frame number: "+QString::number(sNo,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Frame Length: "+QString::number(pcapFrameHeader.len,10)+" bytes ("+
		QString::number(pcapFrameHeader.len*8,10)+" bits)";
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Capture Length: "+QString::number(pcapFrameHeader.caplen,10)+" bytes ("+
		QString::number(pcapFrameHeader.caplen*8,10)+" bits)";
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);

	ui.treeView->setModel(model);
	if (treeExpandFlag.allFrame)
		ui.treeView->expand(ui.treeView->model()->index(0,0));
}

void rms_analyze::anaylzeMacHeader(mac_header_t &macHeader)
{
	QString str;
	QStandardItem* itemRoot;
	QStandardItem* itemLevel1;

	str = "Ethernet II, ";
	str += "Src: "+_MacAddrToString(macHeader.src_mac_addr)+", ";
	str += "Dst: "+_MacAddrToString(macHeader.dst_mac_addr);
	itemRoot = new QStandardItem(str);
	model->appendRow(itemRoot);
	str.clear();
	str = "Source: "+_MacAddrToString(macHeader.src_mac_addr);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Destination: "+_MacAddrToString(macHeader.dst_mac_addr);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Type: ";
	switch(macHeader.type)
	{
	case IP_TYPE:
		str += "IP ";
		break;
	case ARP_TYPE:
		str += "ARP ";
		break;
	}
	str += "(0x"+QString("%1").arg(macHeader.type,4,16,QChar('0'))+")";
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);

	ui.treeView->setModel(model);
	if (treeExpandFlag.macHeader)
		ui.treeView->expand(ui.treeView->model()->index(1,0));
}

void rms_analyze::anaylzeIpHeader(ip_header_t &ipHeader)
{
	QString str;
	QStandardItem* itemRoot;
	QStandardItem* itemLevel1;
	QHostAddress srcAddr(ipHeader.src_ip);
	QHostAddress dstAddr(ipHeader.dst_ip);

	str = "Internet Protocol Version "+QString::number(ipHeader.ver_len.version,10)+", ";
	str += "Src: "+srcAddr.toString()+", ";
	str += "Dst: "+dstAddr.toString();
	itemRoot = new QStandardItem(str);
	model->appendRow(itemRoot);
	str.clear();
	str = "version: "+QString::number(ipHeader.ver_len.version,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Header length: "+QString::number(ipHeader.ver_len.length*4,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Total Length: "+QString::number(ipHeader.len,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Identification: 0x"+QString("%1").arg(ipHeader.id,4,16,QChar('0'))+" ("+QString::number(ipHeader.id,10)+")";
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	anaylzeIpHeaderFlagOffset(itemRoot,ipHeader.flag_off,3);
	str.clear();
	str = "Time to live: "+QString::number(ipHeader.ttl,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Protcol: ";
	switch(ipHeader.ptl)
	{
	case TCP_PROTOCOL:
		str += "TCP ";
		break;
	case UDP_PROTOCOL:
		str += "UDP ";
		break;
	}
	str += "("+QString::number(ipHeader.ptl,10)+")";
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Header checksum: 0x"+QString("%1").arg(ipHeader.sum,4,16,QChar('0'));
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Source: "+srcAddr.toString();
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Destination: "+dstAddr.toString();
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);

	ui.treeView->setModel(model);
	if (treeExpandFlag.internetHeader)
	{
		ui.treeView->expand(ui.treeView->model()->index(2,0));
		ui.treeView->expand(ui.treeView->model()->index(2,0).child(4,0));
	}
}

void rms_analyze::anaylzeIpHeaderFlagOffset(QStandardItem *current,quint16 val,quint16 len)
{
	int i;
	QString str;
	QString strFlag;
	QStandardItem* itemLevel1;
	QStandardItem* itemLevel2;
	QList<QString> bitDesc;

	flag_off_t tFlagOff;
	memcpy(&tFlagOff,&val,sizeof(flag_off_t));
	for(i = 0; i < len; i++)
	{
		switch(i)
		{
		case 0:
			if ((tFlagOff.flag >> i) & 1)
			{
				str = "1... .... = Reserved bit: Set";
				strFlag += "Reserved bit";
			}
			else
				str = "0... .... = Reserved bit: Not Set";
			bitDesc.append(str);
			break;
		case 1:
			str.clear();
			if ((tFlagOff.flag >> i) & 1)
			{
				str = ".1.. .... = Don't fragment: Set";
				strFlag += "Don't fragment";
			}
			else
				str = ".0.. .... = Don't fragment: Not Set";
			bitDesc.append(str);
			break;
		case 2:
			str.clear();
			if ((tFlagOff.flag >> i) & 1)
			{
				str = "..1. .... = More fragment: Set";
				strFlag += "More fragment";
			}
			else
				str = "..0. .... = More fragment: Not Set";
			bitDesc.append(str);
			break;
		}
	}
	str.clear();
	str = "Flags: 0x"+QString("%1").arg(tFlagOff.flag,2,16,QChar('0'));
	if (!strFlag.isEmpty())
		str += " ("+strFlag+")";
	itemLevel1 = new QStandardItem(str);
	current->appendRow(itemLevel1);
	str.clear();
	for (i = 0; i < bitDesc.size(); i++)
	{
		itemLevel2 = new QStandardItem(bitDesc.at(i));
		itemLevel1->appendRow(itemLevel2);
	}
	str.clear();
	str = "Fragment offset: "+QString::number(tFlagOff.offset,10);
	itemLevel1 = new QStandardItem(str);
	current->appendRow(itemLevel1);
}

void rms_analyze::anaylzeTcpHeader(tcp_header_t &tcpHeader)
{
	QString str;
	QStandardItem* itemRoot;
	QStandardItem* itemLevel1;

	str = "Transmission Control Protocol, ";
	str += "Src Port: "+QString::number(tcpHeader.src_port,10)+", ";
	str += "Dst Port: "+QString::number(tcpHeader.dst_port,10);
	itemRoot = new QStandardItem(str);
	model->appendRow(itemRoot);
	str.clear();
	str = "Source port: "+QString::number(tcpHeader.src_port,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Destination port: "+QString::number(tcpHeader.dst_port,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Sequence number: "+QString::number(tcpHeader.seq_num,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Acknowledgement number: "+QString::number(tcpHeader.ack_num,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	anaylzeTcpHeaderOffsetFlags(itemRoot,tcpHeader.off_flags,12);
	str.clear();
	str = "Window size value: "+QString::number(tcpHeader.window,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Checksum: 0x"+QString("%1").arg(tcpHeader.chk_sum,4,16,QChar('0'));
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Urgent Pointer: "+QString::number(tcpHeader.urg_ptr,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);

	ui.treeView->setModel(model);
	if (treeExpandFlag.transportHeader)
	{
		ui.treeView->expand(ui.treeView->model()->index(3,0));
		ui.treeView->expand(ui.treeView->model()->index(3,0).child(5,0));
	}
}

void rms_analyze::anaylzeTcpHeaderOffsetFlags(QStandardItem *current,quint16 val,quint16 len)
{
	QString str;
	QString strFlag;
	QStandardItem* itemLevel1;
	QStandardItem* itemLevel2;
	QList<QString> bitDesc;

	off_flags_t tOffFlags;
	offset_flags_t tOffsetFlags;
	memcpy(&tOffFlags,&val,sizeof(off_flags_t));
	memcpy(&tOffsetFlags,&val,sizeof(offset_flags_t));
	str = "0000 00.. .... = Reserved: Not Set";
	bitDesc.append(str);
	str.clear();
	if (tOffFlags.URG)
	{
		str = ".... ..1. .... = Urgent: Set";
		strFlag += "USG";
	}
	else
		str = ".... ..0. .... = Urgent: Not Set";
	bitDesc.append(str);
	str.clear();
	if (tOffFlags.ACK)
	{
		str = ".... ...1 .... = Acknowledgement: Set";
		strFlag.isEmpty() ? strFlag += "ACK" : strFlag += ", ACK";
	}
	else
		str = ".... ...0 .... = Acknowledgement: Not Set";
	bitDesc.append(str);
	str.clear();
	if (tOffFlags.PSH)
	{
		str = ".... .... 1... = Push: Set";
		strFlag.isEmpty() ? strFlag += "PSH" : strFlag += ", PSH";
	}
	else
		str = ".... .... 0... = Push: Not Set";
	bitDesc.append(str);
	str.clear();
	if (tOffFlags.RST)
	{
		str = ".... .... .1.. = Reset: Set";
		strFlag.isEmpty() ? strFlag += "RST" : strFlag += ", RST";
	}
	else
		str = ".... .... .0.. = Reset: Not Set";
	bitDesc.append(str);
	str.clear();
	if (tOffFlags.SYN)
	{
		str = ".... .... ..1. = Syn: Set";
		strFlag.isEmpty() ? strFlag += "SYN" : strFlag += ", SYN";
	}
	else
		str = ".... .... ..0. = Syn: Not Set";
	bitDesc.append(str);
	str.clear();
	if (tOffFlags.FIN)
	{
		str = ".... .... ...1 = Fin: Set";
		strFlag.isEmpty() ? strFlag += "FIN" : strFlag += ", FIN";
	}
	else
		str = ".... .... ...0 = Fin: Not Set";
	bitDesc.append(str);

	str.clear();
	str = "Header length: "+QString::number(tOffFlags.offset*4,10);
	itemLevel1 = new QStandardItem(str);
	current->appendRow(itemLevel1);
	str.clear();
	str = "Flags: 0x"+QString("%1").arg(tOffsetFlags.flags,2,16,QChar('0'));
	if (!strFlag.isEmpty())
		str += " ("+strFlag+")";
	itemLevel1 = new QStandardItem(str);
	current->appendRow(itemLevel1);
	str.clear();
	for (int i = 0; i < bitDesc.size(); i++)
	{
		itemLevel2 = new QStandardItem(bitDesc.at(i));
		itemLevel1->appendRow(itemLevel2);
	}
}

void rms_analyze::anaylzeUdpHeader(udp_header_t &udpHeader)
{
	QString str;
	QStandardItem* itemRoot;
	QStandardItem* itemLevel1;

	str = "User Datagram Protocol, ";
	str += "Src Port: "+QString::number(udpHeader.src_port,10)+", ";
	str += "Dst Port: "+QString::number(udpHeader.dst_port,10);
	itemRoot = new QStandardItem(str);
	model->appendRow(itemRoot);
	str.clear();
	str = "Source port: "+QString::number(udpHeader.src_port,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Destination port: "+QString::number(udpHeader.dst_port,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Length: "+QString::number(udpHeader.len,10);
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);
	str.clear();
	str = "Checksum: 0x"+QString("%1").arg(udpHeader.chk_sum,4,16,QChar('0'));
	itemLevel1 = new QStandardItem(str);
	itemRoot->appendRow(itemLevel1);

	ui.treeView->setModel(model);
	if (treeExpandFlag.transportHeader)
	{
		ui.treeView->expand(ui.treeView->model()->index(3,0));
		ui.treeView->expand(ui.treeView->model()->index(3,0).child(5,0));
	}
}

////////////////// slot interface /////////////////////////
void rms_analyze::slotOpen()
{
	bStop = false;
	filterFlags = NO_FILTER;
	fileNameList.clear();
	
	bool bOpen = _OpenFile();
	if (bOpen)
		subFrame(fileBuffer,OPEN,true);
}

void rms_analyze::slotOpenEx()
{
	bStop = false;
	filterFlags = NO_FILTER;
	bool bOpen = _OpenFile();
	
	if (fileNameList.size() >= MAX_OPEN_FILE)
	{
		QMessageBox::warning(this,tr("Message"),tr("Accumulative open a maximum of 5 files!"));
		return;
	}

	if (frameHash.size() >= MAX_FRAME_NUM)
	{
		QMessageBox::warning(this,tr("Message"),tr("The maximum number of lines is more than can be open, unable to open the file!"));
		return;
	}

	if (bOpen)
		subFrame(fileBuffer,OPEN_ADD,true);
}

void rms_analyze::slotClose()
{
	tableNo = 0;
	filterFlags = NO_FILTER;
	numKey = 1;
	fileBuffer.clear();
	fileNameList.clear();
	frameHash.clear();
	treeHash.clear();
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->clearContents();
	model->clear();
	ui.textBrowser->clear();
	initTreeWidget();
	ui.statusBar->clearMessage();
}

void rms_analyze::slotConnect()
{
	DlgConnect dlg(this);

	dlg.setDatabase(appDb);
	if (dlg.exec() == QDialog::Accepted)
	{
		tcpSocket = new QTcpSocket();
		connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
		connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
		connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(slotReadMessage()));
		connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,
			SLOT(slotDisplayError(QAbstractSocket::SocketError)));

		hostName = dlg.getHostName();
		hostPort = dlg.getHostPort();
		tcpSocket->connectToHost(hostName,hostPort);
		ui.statusBar->showMessage(tr("Connect host name ")+hostName+tr(", please wait..."));
		if (!tcpSocket->waitForConnected(SECOND*5))
		{
			QMessageBox::warning(this,tr("Message"),tr("Connect host name failed! Host name = ")+hostName);
			ui.statusBar->showMessage(tr("Connect host name ")+hostName+tr(" failed!"));
			delete tcpSocket;
			return;
		}
		appDb->saveConnectData(hostName,hostPort);
	}
}

void rms_analyze::slotDisconnect()
{
	tcpSocket->disconnectFromHost();
	if (!(tcpSocket->state() == QAbstractSocket::UnconnectedState || tcpSocket->waitForDisconnected(SECOND*2)))
	{
		QMessageBox::warning(this,tr("Message"),tr("Disconnect host name failed! Host name = ")+hostName);
		ui.statusBar->showMessage(tr("Disconnect host name ")+hostName+tr(" failed!"));
	}
	slotClose();
	delete tcpSocket;
}

void rms_analyze::slotStart()
{
	bStop = false;
	ui.actionStart->setEnabled(false);
	ui.actionStop->setEnabled(true);
}

void rms_analyze::slotStop()
{
	bStop = true;
	ui.actionStart->setEnabled(true);
	ui.actionStop->setEnabled(false);
}

void rms_analyze::slotOption()
{
	DlgOption dlg(this);

	dlg.setDatabase(appDb);
	dlg.setTranslator(translator);
	dlg.setLanguage(current_language);
	if (dlg.exec() == QDialog::Accepted)
	{
		bZipFile = dlg.getZipFileFlag();
		current_language = dlg.getLanguage();
		ui.retranslateUi(this);
		rootItem->setText(0,tr("Machine"));
	}
}

void rms_analyze::slotEmail()
{
	DlgEmail dlg(this);

	dlg.exec();
}

void rms_analyze::slotAbout()
{
	DlgAbout dlg(this);

	dlg.setLibNum(loadLibNum);
	dlg.setLibProtocol(protocolib);
	dlg.exec();
}

void rms_analyze::slotTableClicked(int row,int column)
{
	bool ok;

	model->clear();
	ui.textBrowser->clear();
	QTableWidgetItem *pItem = ui.tableWidget->item(row,0);
	if (!pItem) return;
	QString str = pItem->text();
	qulonglong sNo = str.toLongLong(&ok,10);
	if (!ok)
		return;

	QHash<quint64, QByteArray>::iterator ifm = frameHash.find(sNo);
	if (ifm != frameHash.end() && ifm.key() == sNo)
	{
		QByteArray sFrame = ifm.value();
		showFrame(sNo,sFrame);
		showParseFrameByLib(sNo);
	}
}

void rms_analyze::slotTreeViewExpanded(const QModelIndex &index)
{
	QModelIndex parent = index.parent();
	if (parent.isValid())
		return;
	
	switch(index.row())
	{
	case 0:
		treeExpandFlag.allFrame = 1;
		break;
	case 1:
		treeExpandFlag.macHeader = 1;
		break;
	case 2:
		treeExpandFlag.internetHeader = 1;
		break;
	case 3:
		treeExpandFlag.transportHeader = 1;
		break;
	}
}

void rms_analyze::slotTreeViewCollapsed(const QModelIndex &index)
{
	QModelIndex parent = index.parent();
	if (parent.isValid())
		return;
	
	switch(index.row())
	{
	case 0:
		treeExpandFlag.allFrame = 0;
		break;
	case 1:
		treeExpandFlag.macHeader = 0;
		break;
	case 2:
		treeExpandFlag.internetHeader = 0;
		break;
	case 3:
		treeExpandFlag.transportHeader = 0;
		break;
	}
}

void rms_analyze::slotTreeWigetItemClicked(QTreeWidgetItem *item,int column)
{
	int i;
	bool ok;
	QString	strItem;
	QStringList sdList;
	QString ipport;
	QStringList ipList;
	QHostAddress hostAddr;
	QString strComboBox;

	if (treeWidgetItemBak == item)
		return;

	treeWidgetItemBak = item;
	if (!item->parent()) 
	{
		slotStop();
		processFrame(filterFlags = NO_FILTER,OPEN,true);
		slotStart();
		return;
	}
	strItem = item->data(0,Qt::UserRole).toString();
	if (strItem.contains(" - ",Qt::CaseInsensitive))
	{
		QStringList sdList = strItem.split(" - ");
		for (i = 0; i < sdList.size(); i++)
		{
			ipport = sdList.at(i);
			ipList = ipport.split(":");
			hostAddr.setAddress(ipList.at(0));
			if (i == 0)
			{
				strComboBox += "addr == "+ipList.at(0)+" && port == "+ipList.at(1);
				srcAddr = hostAddr.toIPv4Address();
				srcPort = ipList.at(1).toInt(&ok,10);
			}
			else
			{
				strComboBox += " && addr == "+ipList.at(0)+" && port == "+ipList.at(1);
				dstAddr = hostAddr.toIPv4Address();
				dstPort = ipList.at(1).toInt(&ok,10);
			}
		}
		filterFlags = PEER_SRC_DST;
	}
	else
	{
		ipList = strItem.split(":");
		hostAddr.setAddress(ipList.at(0));
		strComboBox += "srcaddr == "+ipList.at(0)+" && srcport == "+ipList.at(1);
		srcAddr = hostAddr.toIPv4Address();
		srcPort = ipList.at(1).toInt(&ok,10);

		QTreeWidgetItem *parentItem = item->parent();
		strItem = parentItem->data(0,Qt::UserRole).toString();
		sdList = strItem.split(" - ");
		for (i = 0; i < sdList.size(); i++)
		{
			ipport = sdList.at(i);
			ipList = ipport.split(":");
			hostAddr.setAddress(ipList.at(0));
			if (srcAddr == hostAddr.toIPv4Address()&& srcPort == ipList.at(1).toInt(&ok,10))
				continue;
			else
			{
				strComboBox += " && dstaddr == "+ipList.at(0)+" && dstport == "+ipList.at(1);
				dstAddr = hostAddr.toIPv4Address();
				dstPort = ipList.at(1).toInt(&ok,10);
			}
		}
		filterFlags = FIX_SRC;
	}
	
	//filterComboBox->addItem(strComboBox);
	slotStop();
	processFrame(filterFlags,OPEN,true);
	slotStart();
}

void rms_analyze::slotComboBoxEditTextChanged(const QString &strEdit)
{
	if (strEdit.isEmpty())
		filterComboBox->lineEdit()->setStyleSheet("background-color:white");
	else 
	{
		if (m_Filter->parseFilterString(strEdit))
			filterComboBox->lineEdit()->setStyleSheet("background-color:rgb(170, 255, 127)");
		else
			filterComboBox->lineEdit()->setStyleSheet("background-color:rgb(255, 135, 126)");
	}
}

void rms_analyze::slotConnected()
{
	slotClose();
	bStop = false;
	ui.actionOpen->setEnabled(false);
	ui.actionOpenEx->setEnabled(false);
	ui.actionConnect->setEnabled(false);
	ui.actionStart->setEnabled(false);
	ui.actionDisconnect->setEnabled(true);
	ui.actionStart->setEnabled(false);
	ui.actionStop->setEnabled(true);
	ui.statusBar->showMessage(tr("Connected to ")+hostName);
}

void rms_analyze::slotDisconnected()
{
	ui.actionOpen->setEnabled(true);
	ui.actionOpenEx->setEnabled(true);
	ui.actionConnect->setEnabled(true);
	ui.actionStart->setEnabled(true);
	ui.actionDisconnect->setEnabled(false);
	ui.actionStart->setEnabled(false);
	ui.actionStop->setEnabled(false);
	ui.statusBar->showMessage(tr("Not connected - press connect button to connect.")+hostName);
}

void rms_analyze::slotDisplayError(QAbstractSocket::SocketError)
{
	QMessageBox::warning(this,tr("Message"),tcpSocket->errorString());
}

void rms_analyze::slotReadMessage()
{
	qint64 ret;
	QByteArray mArray;
	QByteArray dealArray;
	commframe_header_t commFrameHeader;

	while(tcpSocket->bytesAvailable())
	{
		mArray.resize(tcpSocket->bytesAvailable());
		ret = tcpSocket->read(mArray.data(),mArray.size());
		commArray.append(mArray);
	}
	
	while (commArray.length() >= sizeof(commframe_header_t))
	{
		memcpy(&commFrameHeader,commArray.data(),sizeof(commframe_header_t));
		if (commFrameHeader.syn == COMMFRAMESYN)
		{
			if (commArray.length() >= commFrameHeader.len + sizeof(commframe_header_t))
			{
				dealArray = commArray.mid(sizeof(commframe_header_t),commFrameHeader.len);
				commArray = commArray.right(mArray.length()-commFrameHeader.len-sizeof(commframe_header_t));
				saveFrame.processSave(dealArray,bZipFile);
				if (!bStop)
				{
					if (frameHash.size() > MAX_NET_FRAME_NUM)
						slotClose();
					subFrame(dealArray,OPEN_ADD,false);
				}
			}
			else
				break;
		}
		else
		{
			commArray.clear();
			break;
		}
	}
}

////////////////// Auxiliary interface /////////////////////////
QString rms_analyze::_ByteToHexString(QByteArray sData)
{
	QString str;
	int showLen = 24;
	int len = sData.size() > showLen ? showLen : sData.size();

	for (int i = 0; i < len; i++)
	{
		str += sData.mid(i,1).toHex();
		if (i == (len - 1) && len == showLen && sData.size() > showLen)
			str += "...";
		else
			str += " ";
	}

	return str;
}

QString rms_analyze::_ByteToHexText(QByteArray sData)
{
	QString str;
	int i;

	for (i = 0; i < sData.size(); i++)
	{
		str += sData.mid(i,1).toHex();
		if (i == 7 || i == 15)
			str += "  ";
		else
			str += " ";
	}

	for (; i < 16; i++)
	{
		if (i == 7 || i == 15)
			str += "    ";
		else
			str += "   ";
	}

	return str;
}

QString rms_analyze::_ByteToAscText(QByteArray sData)
{
	bool ok;
	int	n;
	QString str;

	for (int i = 0; i < sData.size(); i++)
	{
		n = sData.mid(i,1).toHex().toInt(&ok,16);
		if (ok)
		{
			if (n >=32 && n <= 126)
				str += sData.mid(i,1);
			else
				str += ".";
		}
		else
			str += ".";

		if (i == 7)
			str += " ";
	}

	return str;
}

QString rms_analyze::_MacAddrToString(qint8 *addr)
{
	QByteArray tmp;
	QString str;

	for (int i = 0; i < 6; i++)
	{
		tmp.clear();
		tmp.append(addr[i]);
		str += tmp.toHex();
		if (i != 5) 
			str += ":";
	}

	return str;
}

bool rms_analyze::_SatisfyFilter(quint8 sFilter,quint32 sIpaddr,quint16 sPort,quint32 dIpaddr,quint16 dPort)
{
	if (sFilter == PEER_SRC_DST)
	{
		if ((srcAddr == sIpaddr && srcPort == sPort && dstAddr == dIpaddr && dstPort == dPort) ||
			(srcAddr == dIpaddr && srcPort == dPort && dstAddr == sIpaddr && dstPort == sPort))
			return true;
	}
	else if (sFilter == FIX_SRC)
	{
		if (srcAddr == sIpaddr && srcPort == sPort && dstAddr == dIpaddr && dstPort == dPort)
			return true;
	}
	else if (sFilter == NO_FILTER)
		return true;
	
	return false;
}

bool rms_analyze::_OpenFile()
{
	int i = 0;

	QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("pcap file (*.cap *.pcap);;compress file (*.7z *.tar)"));
	if(fileName.length() == 0)
		return false;

	while(i < fileNameList.size())
	{
		if (fileName == fileNameList.at(i))
		{
			QMessageBox::warning(this,tr("Message"),tr("The file already opened!"));
			return false;
		}
		i++;
	}
	fileNameList.append(fileName);

	QFile sFile(fileName);
	if (!sFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this,tr("Message"),tr("Open file failed!"));
		return false;
	}
	if (sFile.size() > MBYTE1*55)
	{
		QMessageBox::warning(this,tr("Message"),tr("Open file failed! File size > 55Mbyte."));
		return false;
	}
	
	fileBuffer.clear();
	fileBuffer = sFile.read(sFile.size());
	sFile.close();

	ui.statusBar->showMessage(tr("Opened file ")+fileName);

	return true;
}

QString rms_analyze::_JudgeProtocolByPort(quint16 srcPort,quint16 dstPort)
{
	if (srcPort == FTP_PORT || dstPort == FTP_PORT)
		return tr("FTP");
	else if (srcPort == SSH_PORT || dstPort == SSH_PORT)
		return tr("SSH");
	else if (srcPort == TELNET_PORT || dstPort == TELNET_PORT)
		return tr("TELNET");
	else if (srcPort == HTTP_PORT || dstPort == HTTP_PORT)
		return tr("HTTP");
	else if (srcPort == HTTPS_PORT || dstPort == HTTPS_PORT)
		return tr("HTTPS");
	else if (srcPort == REMOTE_PORT_2404 || dstPort == REMOTE_PORT_2404 ||
			 srcPort == REMOTE_PORT_2405 || dstPort == REMOTE_PORT_2405)
		return tr("EP-REMOTE");
	else
		return tr("TCP");
}
