#include "dlgconnect.h"

DlgConnect::DlgConnect(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());
	ui.HostComboBox->setEditable(true);
	ui.PortLineEdit->setText(DEFAULT_PORT);

	connect(ui.okButton,SIGNAL(clicked()),this,SLOT(slotButtonOk()));
	connect(ui.cancelButton,SIGNAL(clicked()),this,SLOT(close()));

	QList<connect_data_t> data;
	if (appDb->getConnectData(&data))
	{
		for (int i = 0; i < data.size(); i++)
			ui.HostComboBox->addItem(data.at(i).addr);
	}
	else
		ui.HostComboBox->addItem("172.20.88.1");
}

DlgConnect::~DlgConnect()
{

}

void DlgConnect::slotButtonOk()
{
	bool ok;

	hostName = ui.HostComboBox->currentText();
	if (hostName.isEmpty())
	{
		QMessageBox::warning(this,tr("Message"),tr("Host Name Empty! Please input again."));
		return;
	}
	hostPort = ui.PortLineEdit->text().toInt(&ok,10);
	if (!ok)
	{
		QMessageBox::warning(this,tr("Message"),tr("Port Error! Please input again."));
		return;
	}

	accept();
}

void DlgConnect::closeEvent(QCloseEvent * event)
{
	QDialog::closeEvent(event);
	reject();
	this->close();
}
