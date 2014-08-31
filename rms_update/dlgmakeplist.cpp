#include "dlgmakeplist.h"

dlgMakePlist::dlgMakePlist(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->width(), this->height());

	init();
	initTableWidget();
	connectOrDisconnect();

	connect(ui.openBtn,SIGNAL(clicked()),this,SLOT(slotOpen()));
	connect(ui.makeBtn,SIGNAL(clicked()),this,SLOT(slotMake()));
	connect(ui.uploadBtn,SIGNAL(clicked()),this,SLOT(slotUpload()));
	connect(ui.quitBtn,SIGNAL(clicked()),this,SLOT(close()));
	connect(this,SIGNAL(ftpPutFinished()),this,SLOT(slotFtpPutFinished()));
	connect(ui.tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(slotCellClicked(int,int)));
	connect(ui.tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotTableHeader(int)));
}

dlgMakePlist::~dlgMakePlist()
{

}

void dlgMakePlist::init()
{
	ftp = 0;
	file = 0;
	ui.uploadBtn->setEnabled(false);

	if (!CSkStaticClass::FolderExists(MAKE_DIR))
		CSkStaticClass::CreateFolder(MAKE_DIR);
}

void dlgMakePlist::initTableWidget()
{
	ui.tableWidget->setColumnWidth(MAKE_WIDGET_NAME,140);
	ui.tableWidget->setColumnWidth(MAKE_WIDGET_PATH,140);
	ui.tableWidget->setColumnWidth(MAKE_WIDGET_MD5,250);
	ui.tableWidget->setColumnWidth(MAKE_WIDGET_PROGRESS,100);
	ui.tableWidget->setColumnWidth(MAKE_WIDGET_DEL,20);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);	//最后一列自动拉伸
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//不可编辑
	ui.tableWidget->setSelectionMode(QAbstractItemView::NoSelection);	//不能选择模式
	ui.tableWidget->setFocusPolicy(Qt::NoFocus);						//去除焦点，无虚框
	ui.tableWidget->verticalHeader()->setVisible(false);				//去除最前列
	//初始化路径下的文件
	initTableWidgetContentByPath(CSkStaticClass::GetCurrentPath());
}

void dlgMakePlist::slotOpen()
{
	QString fileName;
	QStringList fileNameList = QFileDialog::getOpenFileNames(this,tr("Select File"), "", tr("all file (*.*)"));

	for (int i = 0; i < fileNameList.size(); i++)
	{
		fileName = fileNameList.at(i);
		fileToTableWidget(fileName);
	}
}

void dlgMakePlist::slotMake()
{
	int uploadCount;
	int rowCount = ui.tableWidget->rowCount();
	if (!rowCount)
	{
		QMessageBox::information(NULL, QString(tr("Message")), QString(tr("No files, cann't make.")));
		return;
	}

	if (!CSkStaticClass::FolderExists(MAKE_DIR))
	{
		if (!CSkStaticClass::CreateFolder(MAKE_DIR))
		{
			QMessageBox::warning(NULL, QString(tr("Message")), QString(tr("Folder %1 create failed.")).arg(MAKE_DIR));
			return;
		}
	}

	if (CSkStaticClass::FileExists(MAKE_DIR + MAKE_PLIST_FILE))
	{
		if (!QFile::remove(MAKE_DIR + MAKE_PLIST_FILE))
		{
			QMessageBox::warning(NULL, QString(tr("Message")), QString(tr("Remove %1 failed.")).arg(MAKE_PLIST_FILE));
			return;
		}
	}

	if (!CSkStaticClass::CreateFile(MAKE_DIR + MAKE_PLIST_FILE))
	{
		QMessageBox::warning(NULL, QString(tr("Message")), QString(tr("Create %1 failed.")).arg(MAKE_PLIST_FILE));
		return;
	}

	if (!makeUpdate(uploadCount))
	{
		QMessageBox::warning(NULL, QString(tr("Message")), QString(tr("Update package contents make failed.")));
		return;
	}
	else if (uploadCount <= 0)
	{
		QMessageBox::warning(NULL, QString(tr("Message")), QString(tr("Have not need to be update package.")));
		return;
	}

	QMessageBox::information(NULL, QString(tr("Message")), QString(tr("Update package make succeed, can upload.")));
	ui.uploadBtn->setEnabled(true);
}

void dlgMakePlist::slotUpload()
{
	ui.uploadBtn->setEnabled(false);
	ui.makeBtn->setEnabled(false);

	ftp->connectToHost(FTP_HOST,FTP_PORT);
	ftp->login(FTP_USER,FTP_PASSWORD);
	ftp->cd(FTP_DOWNLOAD_DIR);

	putFileToRemoteHost();
}

void dlgMakePlist::slotCellClicked(int row, int column)
{
	if (column == MAKE_WIDGET_DEL)
	{
		int ret = QMessageBox::question(NULL,tr("Message"),tr("Whether to delete the line?"),
			QMessageBox::Yes | QMessageBox::No);
		if (ret == QMessageBox::Yes)
		{
			ui.tableWidget->removeRow(row);
		}
	}
}

void dlgMakePlist::slotTableHeader(int column)
{
	if (column == MAKE_WIDGET_DEL)
	{
		int ret = QMessageBox::question(NULL,tr("Message"),tr("Whether to delete all line?"),
			QMessageBox::Yes | QMessageBox::No);
		if (ret == QMessageBox::Yes)
		{
			ui.tableWidget->setRowCount(0);
			ui.tableWidget->clearContents();
		}
	}
}

void dlgMakePlist::fileToTableWidget(QString filename)
{
	QString name;
	QString path;

	if (!getUpdatePathNameByPathFile(filename,name,path))
		return;

	if (isExist(name))
		return;

	int rowCount = ui.tableWidget->rowCount();
	ui.tableWidget->setRowCount(rowCount+1);
	//name
	QTableWidgetItem * col_name = new QTableWidgetItem(name);
	col_name->setTextAlignment(Qt::AlignCenter);
	ui.tableWidget->setItem(rowCount,MAKE_WIDGET_NAME,col_name);
	//path
	QTableWidgetItem * col_path = new QTableWidgetItem(path);
	col_path->setTextAlignment(Qt::AlignCenter);
	ui.tableWidget->setItem(rowCount,MAKE_WIDGET_PATH,col_path);
	//md5
	QString md5 = CSkStaticClass::getFileMd5(filename);
	QTableWidgetItem * col_md5 = new QTableWidgetItem(md5);
	col_md5->setTextAlignment(Qt::AlignCenter);
	ui.tableWidget->setItem(rowCount,MAKE_WIDGET_MD5,col_md5);
	//progress
	QProgressBar * col_progress_bar = new QProgressBar;
	col_progress_bar->setAlignment(Qt::AlignHCenter);
	ui.tableWidget->setCellWidget(rowCount,MAKE_WIDGET_PROGRESS,col_progress_bar);
	//del
	QString del = "X";
	QTableWidgetItem * col_del = new QTableWidgetItem(del);
	col_del->setTextAlignment(Qt::AlignCenter);
	ui.tableWidget->setItem(rowCount,MAKE_WIDGET_DEL,col_del);
}

bool dlgMakePlist::getUpdatePathNameByPathFile(QString pathFile,QString &name,QString &path)
{
	int	pos = pathFile.lastIndexOf("\\");

	name = pathFile.right(pathFile.length() - pos - 1);
	QString m_path = pathFile.left(pos + 1);

	QString currentPath = CSkStaticClass::GetCurrentPath();
	currentPath.replace("/","\\");
	if (!m_path.contains(currentPath,Qt::CaseInsensitive))
	{
		return false;
	}

	if (m_path.length() == currentPath.length())
	{
		path = "./";
	}
	else
	{
		path = m_path.mid(currentPath.length(),m_path.length());
		path.replace("\\","/");
	}

	return true;
}

bool dlgMakePlist::isExist(QString filename)
{
	int rowCount = ui.tableWidget->rowCount();

	for (int i = 0; i < rowCount; i++)
	{
		QTableWidgetItem *item = ui.tableWidget->item(i,MAKE_WIDGET_NAME);
		QString itemText = item->text();
		if (itemText == filename)
		{
			return true;
		}
	}

	return false;
}

bool dlgMakePlist::makeUpdate(int &count)
{
	int i = 0;
	QString remotePlist = CSkStaticClass::GetCurrentPath()+"update/"+"update_package.plist";
	if (!CSkStaticClass::FileExists(remotePlist))
	{
		CSkStaticClass::SWarning(QString(tr("Not found RemotePlist [%1].")).arg(remotePlist),__FILE__,__LINE__);
		return false;
	}
	if (!dealUpdatePackageList(remotePlist))
		return false;
	
	//生成update_package.plist
	if (!writePlist("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"))
		return false;
	
	if (!writePlist("<update_store>\n"))
		return false;

	int rowCount = ui.tableWidget->rowCount();
	for (i = 0; i < rowCount; i++)
	{
		if (!writePlist("	<update>\n"))
			return false;
		//name
		QTableWidgetItem *nameItem = ui.tableWidget->item(i,MAKE_WIDGET_NAME);
		QString name = nameItem->text();
		//path
		QTableWidgetItem *pathItem = ui.tableWidget->item(i,MAKE_WIDGET_PATH);
		QString path = pathItem->text();
		//md5
		QTableWidgetItem *md5Item = ui.tableWidget->item(i,MAKE_WIDGET_MD5);
		QString md5 = md5Item->text();

		if (!writePlist(QString("		<name>%1</name>\n").arg(name)))
			return false;
		if (!writePlist(QString("		<md5>%1</md5>\n").arg(md5)))
			return false;
		if (!writePlist(QString("		<path>%1</path>\n").arg(path)))
			return false;
		if (!writePlist("	</update>\n"))
			return false;

		if (!makePackage(path,name))	//非更新数据包，从列表中删除
		{
			ui.tableWidget->removeRow(i);
			i--;
			rowCount--;
		}
		else
			count++;
	}

	if (!writePlist("</update_store>\n"))
		return false;

	if (i > 0)	//列表中没有可以更新的数据包
	{
		QString localPlist = MAKE_DIR + MAKE_PLIST_FILE;
		localPlist.replace("/","\\");
		fileToTableWidget(localPlist);	//将准备上传的plist放入列表内
	}
	
	return true;
}

bool dlgMakePlist::writePlist(QString content)
{
	QString pathFile = MAKE_DIR + MAKE_PLIST_FILE;
	QByteArray array = content.toLocal8Bit();

	if (!CSkStaticClass::writeFileContent(pathFile,array))
	{
		CSkStaticClass::SWarning(QString(tr("Write %1, %2 failed.")).arg(MAKE_PLIST_FILE).arg(content),__FILE__,__LINE__);
		return false;
	}

	return true;
}

bool dlgMakePlist::makePackage(QString path, QString name)
{
	int  i = 0;
	bool retFind = false;
	bool retMd5 = false;
	QString md5 = CSkStaticClass::getFileMd5(CSkStaticClass::GetCurrentPath()+(path == "./" ? "" : path)+name);

	for (i = 0; i < updatePackageList.size(); i++)
	{
		if (name == updatePackageList.at(i)->getName())
		{
			retFind = true;		
			if (md5 != updatePackageList.at(i)->getMd5())
			{
				retMd5 = true;
			}
			break;
		}
	}

	if (retMd5 || !retFind)	//本地包与远程包MD5不一致，或本地包在远程plist中未找到
	{
		return true;
	}
	
	return false;
}

bool dlgMakePlist::dealUpdatePackageList(QString filename)
{
	QString file_name = filename;

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
		QDomElement dom_name = update_list.at(0).toElement();
		QString up_name = dom_name.text().trimmed();
		QDomElement dom_md5 = update_list.at(1).toElement();
		QString up_md5 = dom_md5.text().trimmed();
		QDomElement dom_path = update_list.at(2).toElement();
		QString up_path = dom_path.text().trimmed();

		update_package *upPackage = new update_package(this);
		upPackage->setName(up_name);
		upPackage->setMd5(up_md5);
		upPackage->setPath(up_path);

		updatePackageList.append(upPackage);
	}

	return true;
}

void dlgMakePlist::initTableWidgetContentByPath(QString path)
{
	if (!CSkStaticClass::FolderExists(path))
		return;

	QStringList filters;	//获取所选文件类型过滤器
	filters	<<QString("*.exe")
			<<QString("*.EXE")
			<<QString("*.dll")
			<<QString("*.DLL")
			<<QString("*.xml")
			<<QString("*.XML");

	//定义迭代器并设置过滤器
	QDirIterator dir_iterator(path,filters,QDir::Files | QDir::NoSymLinks,QDirIterator::Subdirectories);

	while(dir_iterator.hasNext())
	{
		dir_iterator.next();
		QFileInfo file_info = dir_iterator.fileInfo();
		QString absolute_file_path = file_info.absoluteFilePath();
		absolute_file_path.replace("/","\\");
		fileToTableWidget(absolute_file_path);
	}
}

void dlgMakePlist::connectOrDisconnect()
{
	if (ftp) 
	{
		ftp->abort();
		ftp->deleteLater();
		ftp = 0;
	}

	ui.makeBtn->setEnabled(true);

	ftp = new QFtp(this);
	connect(ftp,SIGNAL(commandStarted(int)),this,SLOT(slotFtpCommandStarted(int)));
	connect(ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(slotFtpCommandFinished(int,bool)));
	connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT(slotFtpTransferProgress(qint64,qint64)));
}

void dlgMakePlist::putFileToRemoteHost()
{
	int		i;
	QString	name;
	QString path;
	QString uploadPath;

	int rowCount = ui.tableWidget->rowCount();
	for (i = 0; i < rowCount; i++)
	{
		QWidget *widgetItem = ui.tableWidget->cellWidget(i,MAKE_WIDGET_PROGRESS);
		progressBarItem = (QProgressBar*)widgetItem;
		if (progressBarItem->value() == -1 || progressBarItem->value() != progressBarItem->maximum())
		{
			QTableWidgetItem *nameItem = ui.tableWidget->item(i,MAKE_WIDGET_NAME);
			name = nameItem->text().trimmed();
			QTableWidgetItem *pathItem = ui.tableWidget->item(i,MAKE_WIDGET_PATH);
			path = pathItem->text().trimmed();
			uploadPath = CSkStaticClass::GetCurrentPath()+(path == "./" ? "" : path)+name;
			file = new QFile(uploadPath);
			if (!file->open(QIODevice::ReadOnly))
			{
				QMessageBox::warning(this, tr("Message"),
					tr("Can't readonly open file %1: %2.").arg(uploadPath).arg(file->errorString()));
				delete file;
				return;
			}
			ftp->put(file,name);
			break;
		}	
	}

	if (i >= rowCount)
	{
		ftp->close();
	}
}

void dlgMakePlist::slotFtpCommandStarted(int)
{
	if(ftp->currentCommand() == QFtp::ConnectToHost)
	{
		ui.uploadLabel->setText(tr("connecting..."));
	}
	else if (ftp->currentCommand() == QFtp::Login)
	{
		ui.uploadLabel->setText(tr("login..."));
	}
	else if (ftp->currentCommand() == QFtp::Put)
	{
		ui.uploadLabel->setText(tr("uploading..."));		
	}
	else if (ftp->currentCommand() == QFtp::Close)
	{
		ui.uploadLabel->setText(tr("closing..."));
	}
}

void dlgMakePlist::slotFtpCommandFinished(int,bool error)
{
	if(ftp->currentCommand() == QFtp::ConnectToHost)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("Connect error."));
			ui.uploadLabel->setText(tr("Connect error."));
			connectOrDisconnect();
		}
		else
			ui.uploadLabel->setText(tr("Connect succeed."));
	}
	else if (ftp->currentCommand() == QFtp::Login)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("%1.").arg(ftp->errorString()));
			ui.uploadLabel->setText(tr("%1.").arg(ftp->errorString()));
			connectOrDisconnect();
		}
		else 
			ui.uploadLabel->setText(tr("Login succeed."));
	}
	else if (ftp->currentCommand() == QFtp::Put)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("%1.").arg(ftp->errorString()));
			ui.uploadLabel->setText(tr("%1.").arg(ftp->errorString()));
			connectOrDisconnect();
		}
		else 
		{
			ui.uploadLabel->setText(tr("Upload succeed."));
		}
		file->close();
		delete file;
		ftpPutFinished();
	}
	else if (ftp->currentCommand() == QFtp::Close)
	{
		if(error) 
		{
			QMessageBox::critical(this,tr("Message"),tr("Close error: %1.").arg(ftp->errorString()));
			ui.uploadLabel->setText(tr("Close error: %1.").arg(ftp->errorString()));
			connectOrDisconnect();
		}
		else 
			ui.uploadLabel->setText(tr("Close succeed."));
	}
}

void dlgMakePlist::slotFtpTransferProgress(qint64 readBytes,qint64 totalBytes)
{
	progressBarItem->setMaximum(totalBytes);
	progressBarItem->setValue(readBytes);
}

void dlgMakePlist::slotFtpPutFinished()
{
	putFileToRemoteHost();
}
