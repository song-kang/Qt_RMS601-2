#include "rms_analyze.h"
#include <QtGui/QApplication>
#include "skStaticClass.h"

int init()
{
	int value = LANGUAGE_VAL_ZERO;
	QSqlDatabase db;

	if(QSqlDatabase::contains("qt_sql_default_connection"))
		db = QSqlDatabase::database("qt_sql_default_connection");
	else
		db = QSqlDatabase::addDatabase("QSQLITE");

	db.setDatabaseName("config.db");
	if (!db.open())
		return LANGUAGE_VAL_ZERO;

	QString sql = "SELECT * FROM option WHERE name=:name";
	QSqlQuery sqlQuery;
	sqlQuery.prepare(sql);
	sqlQuery.bindValue(":name",LANGUAGE_NAME);
	if (!sqlQuery.exec())
		return LANGUAGE_VAL_ZERO;

	while(sqlQuery.next())
		value = sqlQuery.value(1).toInt();

	db.close();

	return value;
}

static void outputMessage(QtMsgType type, const char *msg)
{
	bool ok;
	static QMutex logMutex;
	logMutex.lock();

	QString text;
	switch(type)
	{
	case QtDebugMsg:
		text = QString("Debug:");
		break;

	case QtWarningMsg:
		text = QString("Warning:");
		break;

	case QtCriticalMsg:
		text = QString("Critical:");
		break;

	case QtFatalMsg:
		text = QString("Fatal:");
	}

	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");
	QString message = QString("%1|%2 %3").arg(current_date_time).arg(text).arg(msg);

	QFile file;
	ok = CSkStaticClass::FolderExists(CSkStaticClass::GetCurrentPath()+"log");
	if (ok)
	{
		file.setFileName(CSkStaticClass::GetCurrentPath()+"log"+"/log.txt");
		file.open(QIODevice::WriteOnly | QIODevice::Append);
	}
	else
	{
		if (CSkStaticClass::CreateFolder(CSkStaticClass::GetCurrentPath()+"log"))
		{
			file.setFileName(CSkStaticClass::GetCurrentPath()+"log"+"/log.txt");
			file.open(QIODevice::WriteOnly | QIODevice::Append);
		}
		else
			return;
	}

	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();

	logMutex.unlock();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qInstallMsgHandler(outputMessage);

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);

	int lang = LANGUAGE_VAL_CH;
	QTranslator translator;
	int value = init();
	if (value == LANGUAGE_VAL_CH || value == LANGUAGE_VAL_ZERO)
	{
		lang = LANGUAGE_VAL_CH;
		translator.load(":/i18n/rms_analyze_zh.qm");
	}
	else if (value == LANGUAGE_VAL_EN)
	{
		lang = LANGUAGE_VAL_EN;
		translator.load(":/i18n/rms_analyze_en.qm");
	}
	else
	{
		lang = LANGUAGE_VAL_CH;
		translator.load(":/i18n/rms_analyze_zh.qm");
	}
	a.installTranslator(&translator);

	QFile m_Qss(":/qss/rms_analyze.qss");
    m_Qss.open(QFile::ReadOnly);
    a.setStyleSheet(m_Qss.readAll());
    m_Qss.close();

	rms_analyze w;
	w.setTranslator(&translator);
	w.setLanguage(lang);
	w.showMaximized();
	return a.exec();
}
