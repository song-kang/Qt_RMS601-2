#include "dlgabout.h"

dlgAbout::dlgAbout(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	init();

	connect(ui.okButton,SIGNAL(clicked()),this,SLOT(slotOk()));
}

dlgAbout::~dlgAbout()
{

}

void dlgAbout::init()
{
	ui.versionLabel->setText(QCoreApplication::applicationVersion());
	ui.textBrowser->setText(getVersionInformation());
}

void dlgAbout::slotOk()
{
	accept();
}

QString dlgAbout::getVersionInformation()
{
	QString VERSION_INFORMATION = 
		QString(tr(" Version Description"))+QString("\n\n")+
		QString(tr(" > Update application and other software package."))+QString("\n")+
		QString(tr(" > Remote update package and package list manange."))+QString("\n")+
		QString(tr(" > Chinese and English switch."));

	return VERSION_INFORMATION;
}