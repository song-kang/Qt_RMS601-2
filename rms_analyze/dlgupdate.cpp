#include "dlgupdate.h"

DlgUpdate::DlgUpdate(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	downOk = false;

	this->setFixedSize(this->width(), this->height());
	ui.checkButton->setEnabled(true);
	ui.updateButton->setEnabled(false);
	ui.manualButton->setEnabled(true);

	progressDialog = new QProgressDialog(this);
	progressDialog->setModal(true);
	progressDialog->setCancelButton(NULL);

	connect(ui.checkButton,SIGNAL(clicked(bool)),this,SLOT(slotCheck()));
	connect(ui.updateButton,SIGNAL(clicked(bool)),this,SLOT(slotUpdate()));
	connect(ui.manualButton,SIGNAL(clicked(bool)),this,SLOT(slotManual()));
}

DlgUpdate::~DlgUpdate()
{

}

void DlgUpdate::slotCheck()
{
	url = "http://actjlp.free.fr/download/NetShark/version";

	downType = DOWNLOAD_VERSION;
	downloadFile(url);
}

void DlgUpdate::slotUpdate()
{
	url = "http://actjlp.free.fr/download/NetShark/NetShark.new";

	downType = DOWNLOAD_NETSHARK;
	downloadFile(url);
}

void DlgUpdate::slotManual()
{
	url = "http://actjlp.free.fr/download/NetShark/manual.pdf";

	downType = DOWNLOAD_MANUAL;
	downloadFile(url);
}

void DlgUpdate::downloadFile(QUrl url)
{
	QFileInfo fileInfo(url.path());
	QString fileName = fileInfo.fileName();

	if (QFile::exists(fileName))
	{
		QFile::remove(fileName);
	}

	file = new QFile(fileName);
	if (!file->open(QIODevice::WriteOnly))
	{
		QMessageBox::information(this, tr("Download"),tr("Unable to save the file, error: %1.").arg(file->errorString()));
		delete file;
		file = 0;
		return;
	}

	progressDialog->setWindowTitle(tr("Download"));
	progressDialog->setLabelText(tr("Downloading ......"));
	progressDialog->show();

	startRequest(url);
}

//============= QNetworkReply =============
void DlgUpdate::startRequest(QUrl url)
{
	reply = qnam.get(QNetworkRequest(url));

	connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
	connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void DlgUpdate::httpFinished()
{
	bool	ok;
	int		ver;

	progressDialog->close();
	file->flush();
	file->close();

	QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (reply->error())
	{
		file->remove();
		QMessageBox::critical(this, tr("Download"), tr("Download failed."));
	}
	else if (!redirectionTarget.isNull())
	{
		QUrl newUrl = url.resolved(redirectionTarget.toUrl());
		if (QMessageBox::question(this, tr("Download"), tr("Redirect to %1 ?").arg(newUrl.toString()),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			url = newUrl;
			reply->deleteLater();
			file->open(QIODevice::WriteOnly);
			file->resize(0);
			startRequest(url);
			return;
		}
	}
	else
	{
		if (downType == DOWNLOAD_NETSHARK)
		{
			downOk = true;
			ui.checkButton->setEnabled(false);
			ui.updateButton->setEnabled(false);
			QMessageBox::information(this, tr("Download"),
				tr("New version download success. Please restart, finish version update."));
		}
		else if (downType == DOWNLOAD_MANUAL)
		{
			QMessageBox::information(this, tr("Download"), tr("Manual Download success."));
		}
		else if (downType == DOWNLOAD_VERSION)
		{
			ver = tmpVersion.toInt(&ok,10);
			if (APP_VERSION >= ver)
			{
				QMessageBox::information(this, tr("Version"), tr("Already the latest version."));
			}
			else
			{
				ui.checkButton->setEnabled(false);
				ui.updateButton->setEnabled(true);
				QMessageBox::information(this, tr("Version"),tr("New version update, version %1.%2 ").arg(ver/100).arg(ver%100));
			}
		}
	}

	reply->deleteLater();
	reply = 0;
	delete file;
	file = 0;
}

void DlgUpdate::httpReadyRead()
{
	if (downType == DOWNLOAD_NETSHARK || downType == DOWNLOAD_MANUAL)
	{
		if (file)
			file->write(reply->readAll());
	}
	else if (downType == DOWNLOAD_VERSION)
	{
		tmpVersion.clear();
		tmpVersion = reply->readAll();
	}
}

void DlgUpdate::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
	progressDialog->setMaximum(totalBytes);
	progressDialog->setValue(bytesRead);
}
