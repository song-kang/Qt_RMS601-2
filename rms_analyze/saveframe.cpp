#include "saveframe.h"    

CZipThread::CZipThread(QObject *parent)
	: QThread(parent)
{
	
}

CZipThread::~CZipThread()
{

}

void CZipThread::run()
{
	zipMutex->lock();

	QString cmd = zip7zExe+" a "+zipDstFile+" "+zipSrcFile;
	QByteArray ba = cmd.toLatin1();
	char *ch = ba.data();
	::system(ch);

	QFile::remove(zipSrcFile);

	zipMutex->unlock();
}

//////////////// CSaveFrame ///////////////////////////
CSaveFrame::CSaveFrame(QObject *parent)
	: QObject(parent)
{
	zip7zExe = CSkStaticClass::GetCurrentPath() + "7za.exe";
}

CSaveFrame::~CSaveFrame()
{

}

void CSaveFrame::processSave(QByteArray data,bool bZipFile)
{
	QByteArray tmp;

	QString	savePath = CSkStaticClass::GetCurrentPath()+SAVE_FOLDER;
	QString saveFile = CSkStaticClass::GetCurrentPath()+SAVE_FOLDER+"/"+SAVE_FILE;

	if (!CSkStaticClass::FolderExists(savePath))
	{
		if (!CSkStaticClass::CreateFolder(savePath))
			return;
	}
	
	if (!CSkStaticClass::FileExists(saveFile))
	{
		if (!CSkStaticClass::CreateFile(saveFile))
			return;
		CSkStaticClass::writeFileContent(saveFile,data);
	}

	tmp = data.mid(sizeof(pcap_file_header_t),data.size()-sizeof(pcap_file_header_t));
	CSkStaticClass::writeFileContent(saveFile,tmp);

	QFile file(saveFile);
	if (file.size() > MBYTE1*50)
		backupFile(bZipFile);
}

void CSaveFrame::backupFile(bool bZipFile)
{
	bool	bCompress;
	QString	tmpfile0;
	QString	tmpfile1;

	QString zipFile = CSkStaticClass::GetCurrentPath()+SAVE_FOLDER+"/"+SAVE_FILE;
	if (CSkStaticClass::FileExists(zip7zExe)) //目前只支持windows
		bCompress = bZipFile;
	else
		bCompress = false;

	for(int t = 2; t <= MAXSAVEFILENUM; t++)	//删除最旧的文件
	{
		if (bCompress)
		{
			tmpfile0 = QString("%1.back%2.7z").arg(zipFile).arg(t-1, 3, 10, QChar('0'));
			tmpfile1 = QString("%1.back%2.7z").arg(zipFile).arg(t, 3, 10, QChar('0'));
		}
		else
		{
			tmpfile0 = QString("%1.back%2.pcap").arg(zipFile).arg(t-1, 3, 10, QChar('0'));
			tmpfile1 = QString("%1.back%2.pcap").arg(zipFile).arg(t, 3, 10, QChar('0'));
		}

		QFile::remove(tmpfile0);
		QFile::rename(tmpfile1,tmpfile0);
	}

	if (bCompress)
	{
		zipMutex.lock();	//加锁,防止采集速度大于压缩速度
		zipSrcFile = zipFile+".pcap";
		QFile::rename(zipFile,zipSrcFile);
		zipDstFile = tmpfile1;
		zipMutex.unlock();

		zipThread = new CZipThread();
		zipThread->setZipMutex(&zipMutex);
		zipThread->setZip7zExe(zip7zExe);
		zipThread->setZipSrcFile(zipSrcFile);
		zipThread->setZipDstFile(zipDstFile);
		zipThread->start();
	}
	else
		QFile::rename(zipFile,tmpfile1);
}
