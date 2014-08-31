#include "database.h"

CDatabase::CDatabase(QObject *parent)
	: QObject(parent)
{

}

CDatabase::~CDatabase()
{

}

bool CDatabase::createDatabase(QString name)
{
	if(QSqlDatabase::contains("qt_sql_default_connection"))
		m_db = QSqlDatabase::database("qt_sql_default_connection");
	else
		m_db = QSqlDatabase::addDatabase("QSQLITE");
	
	m_db.setDatabaseName("config.db");
	if (!m_db.open())
	{
		QMessageBox::warning(NULL,tr("Message"),tr("Open database failed! database name: ")+name);
		return false;
	}

	QString sql = "CREATE TABLE IF NOT EXISTS connect_addr (addr VARCHAR(16),port INTEGER)";
	QSqlQuery sqlQuery;
	if (!sqlQuery.exec(sql))
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}
	sql = "CREATE TABLE IF NOT EXISTS option (name VARCHAR(16),value INTEGER)";
	if (!sqlQuery.exec(sql))
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}
	
	return true;
}

bool CDatabase::saveConnectData(QString addr, int port)
{
	QSqlQuery sqlQuery;

	int count = getConnectDataNum(addr,port);
	if (count == -1 || count > 0)
		return false;
		
	QString sql = "INSERT INTO connect_addr VALUES(:addr, :port)";
	sqlQuery.prepare(sql);
	sqlQuery.bindValue(":addr",addr);
	sqlQuery.bindValue(":port",port);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}

	return true;
}

int CDatabase::getConnectDataNum()
{
	int count = 0;
	QSqlQuery sqlQuery;

	QString sql = "SELECT * FROM connect_addr";
	sqlQuery.prepare(sql);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+". \n[ "+sql+" ]");
		return -1;
	}
	
	while(sqlQuery.next())
		count++;
	
	return count;
}

int CDatabase::getConnectDataNum(QString addr, int port)
{
	int count = 0;

	QString sql = "SELECT * FROM connect_addr WHERE(addr=:addr and port=:port)";
	QSqlQuery sqlQuery;
	sqlQuery.prepare(sql);
	sqlQuery.bindValue(":addr",addr);
	sqlQuery.bindValue(":port",port);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return -1;
	}

	while(sqlQuery.next())
		count++;

	return count;
}

bool CDatabase::getConnectData(QList<connect_data_t> *cData)
{
	QSqlQuery sqlQuery;
	connect_data_t cdata;

	QString sql = "SELECT * FROM connect_addr";
	sqlQuery.prepare(sql);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}

	while(sqlQuery.next())
	{
		cdata.addr = sqlQuery.value(0).toString();
		cdata.port = sqlQuery.value(1).toInt();
		cData->append(cdata);
	}

	if (!cData->size())
		return false;

	return true;
}

bool CDatabase::saveOptionLanguage(int val)
{
	bool ok;
	QSqlQuery sqlQuery;

	ok = existOptionLanguage();
	if (ok)
	{
		QString sql = "UPDATE option SET value=:value WHERE name=:name";
		sqlQuery.prepare(sql);
		sqlQuery.bindValue(":value",val);
		sqlQuery.bindValue(":name",LANGUAGE_NAME);
		if (!sqlQuery.exec())
		{
			QMessageBox::warning(NULL,tr("Message"),
				tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
			return false;
		}
	}
	else
	{
		QString sql = "INSERT INTO option VALUES(:name, :value)";
		sqlQuery.prepare(sql);
		sqlQuery.bindValue(":name",LANGUAGE_NAME);
		sqlQuery.bindValue(":value",val);
		if (!sqlQuery.exec())
		{
			QMessageBox::warning(NULL,tr("Message"),
				tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
			return false;
		}
	}

	return true;
}

bool CDatabase::saveOptionZipMode(int val)
{
	bool ok;
	QSqlQuery sqlQuery;

	ok = existOptionZipMode();
	if (ok)
	{
		QString sql = "UPDATE option SET value=:value WHERE name=:name";
		sqlQuery.prepare(sql);
		sqlQuery.bindValue(":value",val);
		sqlQuery.bindValue(":name",ZIPMODE_NAME);
		if (!sqlQuery.exec())
		{
			QMessageBox::warning(NULL,tr("Message"),
				tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
			return false;
		}
	}
	else
	{
		QString sql = "INSERT INTO option VALUES(:name, :value)";
		sqlQuery.prepare(sql);
		sqlQuery.bindValue(":name",ZIPMODE_NAME);
		sqlQuery.bindValue(":value",val);
		if (!sqlQuery.exec())
		{
			QMessageBox::warning(NULL,tr("Message"),
				tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
			return false;
		}
	}

	return true;
}

bool CDatabase::getOptionLanguage(int &val)
{
	QSqlQuery sqlQuery;
	val = LANGUAGE_VAL_ZERO;

	if (!existOptionLanguage())
		return false;

	QString sql = "SELECT * FROM option WHERE name=:name";
	sqlQuery.prepare(sql);
	sqlQuery.bindValue(":name",LANGUAGE_NAME);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}

	while(sqlQuery.next())
		val = sqlQuery.value(1).toInt();

	return true;
}

bool CDatabase::getOptionZipMode(int &val)
{
	QSqlQuery sqlQuery;
	val = ZIPMODE_VAL_ZERO;

	if (!existOptionZipMode())
		return false;

	QString sql = "SELECT * FROM option WHERE name=:name";
	sqlQuery.prepare(sql);
	sqlQuery.bindValue(":name",ZIPMODE_NAME);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}

	while(sqlQuery.next())
		val = sqlQuery.value(1).toInt();

	return true;
}

bool CDatabase::existOptionLanguage()
{
	QSqlQuery sqlQuery;

	QString sql = "SELECT * FROM option WHERE name=:name";
	sqlQuery.prepare(sql);
	sqlQuery.bindValue(":name",LANGUAGE_NAME);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}

	while(sqlQuery.next())
		return true;

	return false;
}

bool CDatabase::existOptionZipMode()
{
	QSqlQuery sqlQuery;

	QString sql = "SELECT * FROM option WHERE name=:name";
	sqlQuery.prepare(sql);
	sqlQuery.bindValue(":name",ZIPMODE_NAME);
	if (!sqlQuery.exec())
	{
		QMessageBox::warning(NULL,tr("Message"),
			tr("Sql statment execute failed! ")+"\n"+tr("Error: ")+sqlQuery.lastError().text()+(". \n[ ")+sql+" ]");
		return false;
	}

	while(sqlQuery.next())
		return true;

	return false;
}
