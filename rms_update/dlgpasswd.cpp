#include "dlgpasswd.h"

dlgPasswd::dlgPasswd(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	init();

	connect(ui.okBtn, SIGNAL(clicked()), this, SLOT(slotOk()));
}

dlgPasswd::~dlgPasswd()
{

}

void dlgPasswd::init()
{
	ui.okBtn->setFocus();
}

void dlgPasswd::slotOk()
{
	QString strPasswd = ui.lineEdit->text().trimmed();
	if (strPasswd == PASSWORD_VAL)
	{
		accept();
	}
	else
	{
		QMessageBox::warning(this,tr("Message"),tr("Password error! Please input again."));
		ui.lineEdit->clear();
		ui.lineEdit->setFocus();
	}
}
