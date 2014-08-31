#include "rms_update.h"
#include <windows.h>

rms_update::rms_update(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	this->setWindowFlags(Qt::WindowCloseButtonHint);
	this->setFixedSize(this->width(), this->height());

	init();
	initTableWidget();
	connectOrDisconnect();

	connect(ui.actionCheck,SIGNAL(triggered(bool)),this,SLOT(slotCheck()));
	connect(ui.actionUpdate,SIGNAL(triggered(bool)),this,SLOT(slotUpdate()));
	connect(ui.actionMake,SIGNAL(triggered(bool)),this,SLOT(slotMake()));
	connect(ui.actionQuit,SIGNAL(triggered(bool)),this,SLOT(close()));
	connect(ui.actionSetting,SIGNAL(triggered(bool)),this,SLOT(slotSettng()));
	connect(ui.actionAbout,SIGNAL(triggered(bool)),this,SLOT(slotAbout()));
	connect(ui.tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slotCellClicked(int,int)));
	connect(ui.tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotTableHeader(int)));
	connect(this,SIGNAL(ftpGetFinished()),this,SLOT(slotFtpGetFinished()));
	connect(this,SIGNAL(ftpCloseFinished()),this,SLOT(slotFtpCloseFinished()));
	connect(this,SIGNAL(emitReplayFile()),this,SLOT(slotReplaceFile()));
}

rms_update::~rms_update()
{
	QString sourceFile = UPDATE_DIR+QCoreApplication::applicationName();
	QString targetFile = CSkStaticClass::GetCurrentPath()+QCoreApplication::applicationName();
	QString bridgeFile = CSkStaticClass::GetCurrentPath()+QCoreApplication::applicationName()+".tmp";
	
	if (CSkStaticClass::FileExists(sourceFile))
	{
		if (CSkStaticClass::FileExists(targetFile))
		{
			if (!QFile::remove(bridgeFile))
				CSkStaticClass::SWarning(QString(tr("Remove file [%1] failed.")).arg(bridgeFile),__FILE__,__LINE__);
		}
		if (!QFile::rename(targetFile,bridgeFile))
		{
			CSkStaticClass::SWarning(QString(tr("Source file [%1] replace Bridge file [%2] failed."))
				.arg(sourceFile).arg(bridgeFile),__FILE__,__LINE__);
		}

		if (!QFile::rename(sourceFile,targetFile))
		{
			CSkStaticClass::SWarning(QString(tr("Bridge file [%1] replace Target file [%2] failed."))
				.arg(bridgeFile).arg(targetFile),__FILE__,__LINE__);
		}
	}
}

void rms_update::init()
{
	ftp = 0;
	file = 0;

	if (CSkStaticClass::FileExists(CSkStaticClass::GetCurrentPath()+QCoreApplication::applicationName()+".tmp"))
		QFile::remove(CSkStaticClass::GetCurrentPath()+QCoreApplication::applicationName()+".tmp");

	if (!CSkStaticClass::FolderExists(UPDATE_DIR))
		CSkStaticClass::CreateFolder(UPDATE_DIR);

	ui.actionCheck->setEnabled(true);
	ui.actionUpdate->setEnabled(false);
	ui.actionMake->setEnabled(false);

	progressBar = new QProgressBar;
	progressBar->setAlignment(Qt::AlignHCenter);
	progressBar->setFixedWidth(150);
	statusBarLabel = new QLabel;
	statusBarLabel->setFixedWidth(this->width()-150);
	ui.statusBar->addWidget(progressBar);
	ui.statusBar->addWidget(statusBarLabel);
	ui.statusBar->setFixedHeight(20);
}

void rms_update::initTableWidget()
{
	ui.tableWidget->setColumnWidth(T_WIDGET_CHECK,30);
	ui.tableWidget->setColumnWidth(T_WIDGET_NAME,150);
	ui.tableWidget->setColumnWidth(T_WIDGET_MD5,300);
	ui.tableWidget->setColumnWidth(T_WIDGET_PROGRESS,100);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);	//最后一列自动拉伸
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//不可编辑
	ui.tableWidget->setSelectionMode(QAbstractItemView::NoSelection);	//不能选择模式
	ui.tableWidget->setFocusPolicy(Qt::NoFocus);						//去除焦点，无虚框
	ui.tableWidget->verticalHeader()->setVisible(false);				//去除最前列
}

void rms_update::connectOrDisconnect()
{
	if (ftp) 
	{
		ftp->abort();
		ftp->deleteLater();
		ftp = 0;
	}

	ui.actionCheck->setEnabled(true);

	ftp = new QFtp(this);
	connect(ftp,SIGNAL(commandStarted(int)),this,SLOT(slotFtpCommandStarted(int)));
	connect(ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(slotFtpCommandFinished(int,bool)));
	connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT(slotFtpTransferProgress(qint64,qint64)));
}

void rms_update::slotCheck()
{
	fileName = UPDATE_PLIST_NAME;
	file = new QFile(UPDATE_DIR + fileName);
	if (!file->open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("Message"),
			tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString()));
		delete file;
		return;
	}

	ui.actionCheck->setEnabled(false);
	ui.actionUpdate->setEnabled(false);
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->clearContents();
	updatePackageList.clear();

	ftp->connectToHost(FTP_HOST,FTP_PORT);
	ftp->login(FTP_USER,FTP_PASSWORD);
	ftp->cd(FTP_DOWNLOAD_DIR);
	ftp->get(fileName,file);
	ftp->close();
}

void rms_update::slotUpdate()
{
	ui.actionCheck->setEnabled(false);
	ui.actionUpdate->setEnabled(false);

	ftp->connectToHost(FTP_HOST,FTP_PORT);
	ftp->login(FTP_USER,FTP_PASSWORD);
	ftp->cd(FTP_DOWNLOAD_DIR);

	getFileFromRemoteHost();
}

void rms_update::slotMake()
{
	dlgPasswd dlgPD(this);
	if (dlgPD.exec() == QDialog::Accepted)
	{
		dlgMakePlist dlgMP(this);
		dlgMP.exec();
	}
}

void rms_update::slotSettng()
{
	dlgSetting dlg(this);

	dlg.setTranslator(translator);
	dlg.setLanguage(current_language);
	if (dlg.exec() == QDialog::Accepted)
	{
		current_language = (LANGUAGE)dlg.getLanguage();
		ui.retranslateUi(this);
	}
}

void rms_update::slotAbout()
{
	dlgAbout dlg(this);

	dlg.exec();
}

void rms_update::slotCellClicked(int row, int column)
{
	if (column == T_WIDGET_CHECK)
	{
		QTableWidgetItem *item = ui.tableWidget->item(row,T_WIDGET_CHECK);
		QString itemText = item->text().trimmed();
		if (itemText != T_WIDGET_CHECK_SIGN)
			item->setText(T_WIDGET_CHECK_SIGN);
		else
			item->setText("");
	}
}

void rms_update::slotTableHeader(int column)
{
	int	 i = 0;
	bool bCheck = false;

	int rowCount = ui.tableWidget->rowCount();
	if (!rowCount)
		return;

	if (column == T_WIDGET_CHECK)
	{
		for (i = 0; i < rowCount; i++)
		{
			QTableWidgetItem *item = ui.tableWidget->item(i,T_WIDGET_CHECK);
			QString itemText = item->text().trimmed();
			if (itemText != T_WIDGET_CHECK_SIGN)
			{
				item->setText(T_WIDGET_CHECK_SIGN);
				bCheck = true;
			}
		}

		if (bCheck == false)
		{
			for (i = 0; i < rowCount; i++)
			{
				QTableWidgetItem *item = ui.tableWidget->item(i,T_WIDGET_CHECK);
				item->setText("");
			}
		}
	}
}

void rms_update::slotFtpCommandStarted(int)
{
	if(ftp->currentCommand() == QFtp::ConnectToHost)
	{
		statusBarLabel->setText(tr("connecting..."));
	}
	else if (ftp->currentCommand() == QFtp::Login)
	{
		statusBarLabel->setText(tr("login..."));
	}
	else if (ftp->currentCommand() == QFtp::Get)
	{
		statusBarLabel->setText(tr("downloading..."));		
	}
	else if (ftp->currentCommand() == QFtp::Close)
	{
		statusBarLabel->setText(tr("closing..."));
	}
}

void rms_update::slotFtpCommandFinished(int,bool error)
{
	if(ftp->currentCommand() == QFtp::ConnectToHost)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("Connect error."));
			statusBarLabel->setText(tr("Connect error."));
			connectOrDisconnect();
		}
		else
			statusBarLabel->setText(tr("Connect succeed."));
	}
	else if (ftp->currentCommand() == QFtp::Login)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("%1.").arg(ftp->errorString()));
			statusBarLabel->setText(tr("%1.").arg(ftp->errorString()));
			connectOrDisconnect();
		}
		else 
			statusBarLabel->setText(tr("Login succeed."));
	}
	else if (ftp->currentCommand() == QFtp::Get)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("%1.").arg(ftp->errorString()));
			statusBarLabel->setText(tr("%1.").arg(ftp->errorString()));
			connectOrDisconnect();
		}
		else 
		{
			statusBarLabel->setText(tr("Download succeed."));
		}
		file->close();
		delete file;
		ftpGetFinished();
	}
	else if (ftp->currentCommand() == QFtp::Close)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("Close error: %1.").arg(ftp->errorString()));
			statusBarLabel->setText(tr("Close error: %1.").arg(ftp->errorString()));
			connectOrDisconnect();
		}
		else 
		{
			statusBarLabel->setText(tr("Close succeed."));
			ftpCloseFinished();
		}
	}
}

void rms_update::slotFtpTransferProgress(qint64 readBytes,qint64 totalBytes)
{
	if (fileName == UPDATE_PLIST_NAME)
	{
		progressBar->setMaximum(totalBytes);
		progressBar->setValue(readBytes);
	}
	else
	{		
		progressBarItem->setMaximum(totalBytes);
		progressBarItem->setValue(readBytes);
	}
}

void rms_update::slotFtpGetFinished()
{
	int i = 0;

	if (fileName == UPDATE_PLIST_NAME)
		return;

	getFileFromRemoteHost();
}

void rms_update::slotFtpCloseFinished()
{
	bool bUp = false;
	int  rowCount = 0;

	if (fileName == UPDATE_PLIST_NAME)
	{
		if (dealUpdatePackageList(UPDATE_PLIST_NAME))
		{
			for (int i = 0; i < updatePackageList.size(); i++)
			{
				update_package *upPackage = updatePackageList.at(i);
				if (upPackage->md5Compare()) continue;

				ui.tableWidget->setRowCount(++rowCount);
				//check
				QString check = T_WIDGET_CHECK_SIGN;
				QTableWidgetItem * col_check = new QTableWidgetItem(check);
				col_check->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(rowCount-1,T_WIDGET_CHECK,col_check);
				//name
				QTableWidgetItem * col_name = new QTableWidgetItem(upPackage->getName());
				col_name->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(rowCount-1,T_WIDGET_NAME,col_name);
				//md5
				QTableWidgetItem * col_md5 = new QTableWidgetItem(upPackage->getMd5());
				col_md5->setTextAlignment(Qt::AlignCenter);
				ui.tableWidget->setItem(rowCount-1,T_WIDGET_MD5,col_md5);
				//progress
				QProgressBar * col_progress_bar = new QProgressBar;
				col_progress_bar->setAlignment(Qt::AlignHCenter);
				ui.tableWidget->setCellWidget(rowCount-1,T_WIDGET_PROGRESS,col_progress_bar);

				bUp = true;
			}
			if (bUp)
				ui.actionUpdate->setEnabled(true);
			else
				QMessageBox::warning(NULL, QString(tr("Message")), QString(tr("Module already new version.")));

			ui.actionMake->setEnabled(true);
		}
	}
	else
		ui.actionUpdate->setEnabled(true);	//所有文件下载完毕

	ui.actionCheck->setEnabled(true);
	fileName.clear();
}

void rms_update::slotReplaceFile()
{
	int i;
	int rowCount = ui.tableWidget->rowCount();
	for (i = 0; i < rowCount; i++)
	{
		QWidget *widgetItem = ui.tableWidget->cellWidget(i,T_WIDGET_PROGRESS);
		progressBarItem = (QProgressBar*)widgetItem;
		if (progressBarItem->value() != -1 && progressBarItem->value() == progressBarItem->maximum())
		{
			QTableWidgetItem *nameItem = ui.tableWidget->item(i,T_WIDGET_NAME);
			QString name = nameItem->text().trimmed();
			if (name == QCoreApplication::applicationName())
				continue;	//自身程序暂不在此更新，退出时更新
			if (!CSkStaticClass::FileExists(UPDATE_DIR+name))
				continue;	//已经被升级或被人为删除，忽略之。
			if (!replaceFile(name))
			{
				QMessageBox::warning(NULL, QString("Message"),
					QString(tr("File [%1] update error! Update failed.")).arg(name));
				progressBarItem->setValue(-1);
				progressBarItem->setMaximum(100);
				break;
			}
		}	
	}

	if (i >= rowCount)
	{
		ui.actionUpdate->setEnabled(false);
		QMessageBox::information(NULL, QString("Message"),
			QString(tr("All file Update succeed, please affirm update by enter check again.")));
	}
}

bool rms_update::replaceFile(QString filename)
{
	QString targetFile;
	QString sourceFile;
	QString bridgeFile;

	targetFile.clear();
	update_package *uPackage;
	for (int i = 0; i < updatePackageList.size(); i++)
	{
		uPackage = updatePackageList.at(i);
		if (uPackage->getName() == filename)
		{
			if (uPackage->getPath() == "./")
				targetFile = CSkStaticClass::GetCurrentPath()+uPackage->getName();
			else
			{
				if (!createFolderByPath(uPackage->getPath()))
					return false;
				targetFile = CSkStaticClass::GetCurrentPath()+uPackage->getPath()+uPackage->getName();
			}
			break;
		}
	}

	if (targetFile.isEmpty())
		return false;

	if (!killProcessByFilename(uPackage->getName()))
		return false;

	sourceFile = UPDATE_DIR+filename;
	bridgeFile = UPDATE_DIR+filename+".tmp";	//rename无法同名，用了中间文件
	//转为中间过渡文件
	if (CSkStaticClass::FileExists(bridgeFile))
	{
		if (!QFile::remove(bridgeFile))
		{
			CSkStaticClass::SWarning(QString(tr("Remove file [%1] failed.")).arg(bridgeFile),__FILE__,__LINE__);
			return false;
		}
	}
	if (!QFile::rename(sourceFile,bridgeFile))
	{
		CSkStaticClass::SWarning(QString(tr("Source file [%1] replace Bridge file [%2] failed."))
			.arg(sourceFile).arg(bridgeFile),__FILE__,__LINE__);
		return false;
	}
	//生成最终文件
	if (CSkStaticClass::FileExists(targetFile))
	{
		if (!QFile::remove(targetFile))
		{
			CSkStaticClass::SWarning(QString(tr("Remove file [%1] failed.")).arg(targetFile),__FILE__,__LINE__);
			return false;
		}
	}
	if (!QFile::rename(bridgeFile,targetFile))
	{
		CSkStaticClass::SWarning(QString(tr("Bridge file [%1] replace Target file [%2] failed."))
			.arg(bridgeFile).arg(targetFile),__FILE__,__LINE__);
		return false;
	}
	
	return true;
}

bool rms_update::createFolderByPath(QString path)
{
	QString m_path;
	QStringList pathList = path.split("/");

	for (int i = 1; i <= pathList.size(); i++)
	{
		if (pathList.at(i-1).isEmpty())
			continue;	//无文件路径，省略之。

		m_path = CSkStaticClass::GetCurrentPath();
		for (int j = 0; j < i; j++)
		{
			m_path = m_path + pathList.at(j) + "/";
		}
		
		if (!CSkStaticClass::FolderExists(m_path))
		{
			if (!CSkStaticClass::CreateFolder(m_path))
			{
				CSkStaticClass::SWarning(QString(tr("Create folder [%1] failed.")).arg(m_path),__FILE__,__LINE__);
				return false;
			}
		}
	}

	return true;
}

bool rms_update::killProcessByFilename(QString filename)
{
	int pid;
	int ppid;
	skProcess process;

	QFileInfo fi(filename);
	if (fi.suffix() == "exe")
	{
		if (process.getProcessByName(pid,ppid,filename))
		{
			process.closeProcess(pid);
			Sleep(1000);	//睡1S等待关闭进程完成
		}
	}

	return true;
}

bool rms_update::dealUpdatePackageList(QString filename)
{
	QString file_name = UPDATE_DIR +filename;

	if(file_name.isEmpty())  
		return false;  

	QFile file(file_name);  
	if(!file.open(QFile::ReadOnly | QFile::Text))
	{  
		QMessageBox::warning(NULL, QString("Message"), QString(tr("open file %1 error!")).arg(file_name));
		return false;  
	}

	QDomDocument document;  
	QString error;  
	int row = 0, column = 0;  
	if(!document.setContent(&file, false, &error, &row, &column))
	{  
		QMessageBox::warning(NULL, QString(tr("Message")), 
			QString(tr("parse file failed at line row and column")) + QString::number(row, 10) + QString(",") + QString::number(column, 10));
		return false;  
	}  

	if(document.isNull())
	{  
		QMessageBox::warning(NULL, QString(tr("Message")), QString(tr("document is null!")));
		return false;  
	}

	QDomElement root = document.documentElement();
	QDomNodeList update_store_list = root.childNodes();
	int count = update_store_list.count();
	for(int i = 0; i < count; i++)
	{
		QDomNode dom_node = update_store_list.item(i);
		QDomElement element = dom_node.toElement();
		QDomNodeList update_list = element.childNodes();
		QDomElement dom_name = update_list.at(UPDATE_NAME_ID).toElement();
		QString up_name = dom_name.text().trimmed();
		QDomElement dom_md5 = update_list.at(UPDATE_MD5_ID).toElement();
		QString up_md5 = dom_md5.text().trimmed();
		QDomElement dom_path = update_list.at(UPDATE_PATH_ID).toElement();
		QString up_path = dom_path.text().trimmed();

		update_package *upPackage = new update_package(this);
		upPackage->setName(up_name);
		upPackage->setMd5(up_md5);
		upPackage->setPath(up_path);

		updatePackageList.append(upPackage);
	}

	return true;
}

void rms_update::getFileFromRemoteHost()
{
	int		i;

	int rowCount = ui.tableWidget->rowCount();
	for (i = 0; i < rowCount; i++)
	{
		QTableWidgetItem *checkItem = ui.tableWidget->item(i,T_WIDGET_CHECK);
		QString itemText = checkItem->text().trimmed();
		if (itemText != T_WIDGET_CHECK_SIGN)
			continue;

		QWidget *widgetItem = ui.tableWidget->cellWidget(i,T_WIDGET_PROGRESS);
		progressBarItem = (QProgressBar*)widgetItem;
		if (progressBarItem->value() == -1 || progressBarItem->value() != progressBarItem->maximum())
		{
			QTableWidgetItem *nameItem = ui.tableWidget->item(i,T_WIDGET_NAME);
			fileName = nameItem->text().trimmed();
			file = new QFile(UPDATE_DIR + fileName);
			if (!file->open(QIODevice::WriteOnly))
			{
				QMessageBox::warning(this, tr("Message"),
					tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString()));
				delete file;
				return;
			}
			ftp->get(fileName,file);
			break;
		}	
	}

	if (i >= rowCount)
	{
		ftp->close();
		slotReplaceFile();
	}
}
