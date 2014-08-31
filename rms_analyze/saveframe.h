#ifndef SAVEFRAME_H
#define SAVEFRAME_H

#include <QtGui>
#include <QObject>
#include "pcap.h"
#include "skStaticClass.h"
#include <QThread>

#define SAVE_FOLDER			"save"
#define SAVE_FILE			"save.pcap"

#define	MBYTE1				(1024*1024)
#define	MAXZIPLEN			(MBYTE1*50)	//最大文件压缩大小
#define MAXSAVEFILENUM		200			//最大备份文件数

class CZipThread : public QThread
{
	Q_OBJECT

public:
	CZipThread(QObject *parent = 0);
	~CZipThread();

protected:
	void run();

public:
	void setZipMutex(QMutex *mutex) { zipMutex = mutex; };
	void setZip7zExe(QString str) { zip7zExe = str; };
	void setZipSrcFile(QString str) { zipSrcFile = str; };
	void setZipDstFile(QString str) { zipDstFile = str; };

private:
	QMutex *zipMutex;
	QString zip7zExe;
	QString zipSrcFile;
	QString zipDstFile;
};

class CSaveFrame : public QObject
{
	Q_OBJECT

public:
	CSaveFrame(QObject *parent = 0);
	~CSaveFrame();

public:
	void processSave(QByteArray data,bool bZipFile);

public:
	QMutex zipMutex;
	QString zip7zExe;
	QString zipSrcFile;
	QString zipDstFile;

	CZipThread *zipThread;

private:
	void backupFile(bool bZipFile);

};

#endif // SAVEFRAME_H
