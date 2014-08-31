#ifndef DLGMAKEPLIST_H
#define DLGMAKEPLIST_H

#include <QtGui>
#include <QFtp>
#include <QtXml>
#include <QDialog>
#include "ui_dlgmakeplist.h"
#include "update_package.h"
#include "skStaticClass.h"
#include "common.h"

#define MAKE_WIDGET_NAME		0
#define MAKE_WIDGET_PATH		1
#define MAKE_WIDGET_MD5			2
#define MAKE_WIDGET_PROGRESS	3
#define MAKE_WIDGET_DEL			4

#define MAKE_DIR				CSkStaticClass::GetCurrentPath()+"makeup/"
#define MAKE_PLIST_FILE			"update_package.plist"

class dlgMakePlist : public QDialog
{
	Q_OBJECT

public:
	dlgMakePlist(QWidget *parent = 0);
	~dlgMakePlist();

private:
	Ui::dlgMakePlist ui;

	void init();
	void initTableWidget();
	void fileToTableWidget(QString filename);
	bool getUpdatePathNameByPathFile(QString pathFile,QString &name,QString &path);
	bool isExist(QString filename);
	bool makeUpdate(int &count);
	bool writePlist(QString content);
	bool makePackage(QString path, QString name);
	bool dealUpdatePackageList(QString filename);
	void initTableWidgetContentByPath(QString path);
	void connectOrDisconnect();
	void putFileToRemoteHost();

private:
	QFtp  * ftp;
	QFile * file;
	QList<update_package*> updatePackageList;
	QProgressBar *progressBarItem;

signals:
	void ftpPutFinished();

public slots:
	void slotOpen();
	void slotMake();
	void slotUpload();
	void slotCellClicked(int row, int column);
	void slotTableHeader(int column);
	void slotFtpCommandStarted(int);
	void slotFtpCommandFinished(int,bool);
	void slotFtpTransferProgress(qint64,qint64);
	void slotFtpPutFinished();

};

#endif // DLGMAKEPLIST_H
