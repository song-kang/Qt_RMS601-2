#include "dlgabout.h"

DlgAbout::DlgAbout(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	initTableWidget();
	readme();
	ui.versionLabel->setText(APP_VERSION);
	
	connect(ui.okButton,SIGNAL(clicked()),this,SLOT(slotButtonOk()));
	connect(ui.tabWidget,SIGNAL(currentChanged(int)),this,SLOT(slotTabCurrentChanged(int)));
}

DlgAbout::~DlgAbout()
{

}

void DlgAbout::initTableWidget()
{
	ui.tableWidget->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//不可编辑
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //单选模式
	ui.tableWidget->setFocusPolicy(Qt::NoFocus); //去除焦点，无虚框
}

void DlgAbout::initTextBrower()
{
	ui.textBrowser->setLineWrapMode(QTextEdit::NoWrap); //出现水平滚动条
}

void DlgAbout::slotTabCurrentChanged(int page)
{
	QString version;
	QString author;
	QString description;

	switch (page)
	{
	case PAGE_NETSHARK:
		break;
	case PAGE_PLUGINS:
		ui.tableWidget->setRowCount(loadLibNum);
		for (int i = 0; i < loadLibNum; i++)
		{
			GetLibInfo libInfo = (GetLibInfo)protocolib[i].lib->resolve("getLibInfo");
			if (!libInfo)
			{
				CSkStaticClass::SWarning(QString("Load library function:(getLibInfo) error %1!")
					.arg(protocolib[i].lib->errorString()),__FILE__,__LINE__);
				continue;
			}

			libInfo(version,author,description);

			QTableWidgetItem * col_name = new QTableWidgetItem(protocolib[i].name);
			col_name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.tableWidget->setItem(i,0,col_name);

			QTableWidgetItem * col_ver = new QTableWidgetItem(version);
			col_ver->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			col_ver->setBackground(QBrush(Qt::cyan));
			ui.tableWidget->setItem(i,1,col_ver);

			QTableWidgetItem * col_author = new QTableWidgetItem(author);
			col_author->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.tableWidget->setItem(i,2,col_author);

			QTableWidgetItem * col_desc = new QTableWidgetItem(description);
			ui.tableWidget->setItem(i,3,col_desc);

			ui.tableWidget->resizeColumnsToContents();
		}
		break;
	default:
		break;
	}
}

void DlgAbout::slotButtonOk()
{
	hide();
}

void DlgAbout::readme()
{
	strReadme = QString(tr("Version Description"));
	strReadme += "\n\n";
	strReadme += tr("> PCAP file packet parsing.");
	strReadme += "\n";
	strReadme += tr("> Network IP packets real-time capture and analysis.");
	strReadme += "\n";
	strReadme += tr("> Plug-in message parsing.");
	strReadme += "\n";
	strReadme += tr("> Local real-time message store.");
	strReadme += "\n";
	strReadme += tr("> Chinese and English switch.");

	ui.textBrowser->append(strReadme);
}
