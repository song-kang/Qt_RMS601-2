#include "dlgemail.h"
#include "dlgabout.h"

DlgEmail::DlgEmail(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	connect(ui.sendPushButton,SIGNAL(clicked()),this,SLOT(slotSendEmail()));
}

DlgEmail::~DlgEmail()
{

}

EmailAddress* DlgEmail::stringToEmail(const QString &str)
{
	int p1 = str.indexOf("<");
	int p2 = str.indexOf(">");

	if (p1 == -1)
	{
		// no name, only email address
		return new EmailAddress(str);
	}
	else
	{
		return new EmailAddress(str.mid(p1 + 1, p2 - p1 - 1), str.left(p1));
	}

}

void DlgEmail::slotSendEmail()
{
	if (ui.contentTextEdit->toPlainText().isEmpty())
	{
		QMessageBox::warning(this,tr("Message"),tr("Please enter questions or suggestions the content."));
		return;
	}

	if (ui.nameLineEdit->text().trimmed().isEmpty())
	{
		QMessageBox::warning(this,tr("Message"),tr("Please enter asker name."));
		return;
	}

	EmailAddress *sender = stringToEmail(EMAIL_SENDER);
	QString subject = QString("NetShark %1 [%2]").arg(APP_VERSION).arg(ui.nameLineEdit->text().trimmed());
	QString html = ui.contentTextEdit->toHtml();

	SmtpClient smtp(EMAIL_HOST,EMAIL_PORT,SmtpClient::TcpConnection);

	MimeMessage message;
	message.setSender(sender);
	message.setSubject(subject);
	message.addRecipient(stringToEmail(EMAIL_RECVER));

	MimeHtml content;
	content.setHtml(html);
	message.addPart(&content);

	if (!smtp.connectToHost())
	{
		QMessageBox::warning(this,tr("Message"),tr("SMTP connect failed."));
		return;
	}

	if (!smtp.login(EMAIL_USER, EMAIL_PASSWORD))
	{
		QMessageBox::warning(this,tr("Message"),tr("Login failed."));
		return;
	}

	if (!smtp.sendMail(message))
	{
		QMessageBox::warning(this,tr("Message"),tr("Send email failed, Please check the network is normal."));
		return;
	}
	else
	{
		QMessageBox::warning(this,tr("Message"),tr("Send email succeed, Thank you for your support."));
	}

	smtp.quit();

	this->close();
}
