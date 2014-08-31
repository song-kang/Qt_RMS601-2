#ifndef RMS_UPDATE_H
#define RMS_UPDATE_H

#include <QtGui>
#include <QFtp>
#include <QtXml>
#include <QtGui/QMainWindow>
#include "ui_rms_update.h"
#include "skStaticClass.h"
#include "update_package.h"
#include "dlgmakeplist.h"
#include "dlgpasswd.h"
#include "skprocess.h"
#include "dlgsetting.h"
#include "dlgabout.h"
#include "common.h"

#define UPDATE_DIR			CSkStaticClass::GetCurrentPath()+"update/"
#define UPDATE_PLIST_NAME	"update_package.plist"
#define UPDATE_NAME_ID		0
#define UPDATE_MD5_ID		1
#define UPDATE_PATH_ID		2

#define T_WIDGET_CHECK		0
#define T_WIDGET_NAME		1
#define T_WIDGET_MD5		2
#define T_WIDGET_PROGRESS	3

#define T_WIDGET_CHECK_SIGN	"O"

class rms_update : public QMainWindow
{
	Q_OBJECT

public:
	rms_update(QWidget *parent = 0, Qt::WFlags flags = 0);
	~rms_update();

private:
	Ui::rms_updateClass ui;

	void init();
	void initTableWidget();
	void connectOrDisconnect();

public:
	void setTranslator(QTranslator *trans) { translator = trans; };
	void setLanguage(LANGUAGE langu) { current_language = langu; };

private:
	QFtp  * ftp;
	QFile * file;
	QString fileName;
	QList<update_package*> updatePackageList;

	QLabel		 *statusBarLabel;
	QProgressBar *progressBar;
	QProgressBar *progressBarItem;
	QTranslator  *translator;

	LANGUAGE	  current_language;

signals:
	void ftpGetFinished();
	void ftpCloseFinished();
	void emitReplayFile();

private:
	bool dealUpdatePackageList(QString filename);
	void getFileFromRemoteHost();
	bool createFolderByPath(QString path);
	bool replaceFile(QString filename);
	bool killProcessByFilename(QString filename);

public slots:
	void slotCheck();
	void slotUpdate();
	void slotMake();
	void slotSettng();
	void slotAbout();
	void slotCellClicked(int row, int column);
	void slotTableHeader(int column);
	void slotFtpCommandStarted(int);
	void slotFtpCommandFinished(int,bool);
	void slotFtpTransferProgress(qint64,qint64);
	void slotFtpGetFinished();
	void slotFtpCloseFinished();
	void slotReplaceFile();
};

#endif // RMS_UPDATE_H
