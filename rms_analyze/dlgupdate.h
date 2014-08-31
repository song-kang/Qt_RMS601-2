#ifndef DLGUPDATE_H
#define DLGUPDATE_H

#include <QtGui>
#include <QtNetwork>
#include <QMessageBox>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QDialog>
#include "ui_dlgupdate.h"

#define		DOWNLOAD_NETSHARK		1
#define		DOWNLOAD_MANUAL			2
#define		DOWNLOAD_VERSION		3

#define		APP_VERSION				100

class DlgUpdate : public QDialog
{
	Q_OBJECT

public:
	DlgUpdate(QWidget *parent = 0);
	~DlgUpdate();

private:
	Ui::DlgUpdate ui;

private:
	QProgressDialog *progressDialog;

	QFile           *file;
	QNetworkReply   *reply;
	QUrl            url;
	QNetworkAccessManager qnam;

	bool			downOk;
	uchar			downType;
	QByteArray		tmpVersion;

	QHttp			*httpClient;
	int				requestId;
	bool			httpRequestAborted;

public:
	void startRequest(QUrl url);
	bool getDownloadState() { return downOk; };
	void downloadFile(QUrl url);

public slots:
	void slotCheck();
	void slotUpdate();
	void slotManual();
	void httpFinished();
	void httpReadyRead();
	void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
};

#endif // DLGUPDATE_H
