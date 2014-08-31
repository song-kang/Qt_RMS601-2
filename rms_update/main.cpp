#include "rms_update.h"
#include <QtGui/QApplication>
#include "util.h"

#define G_VERSION	"1.0.0"

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

	QString language_value;
	LANGUAGE lang = UI_ZH;
	QTranslator translator;
	bool is_read = Util::readInit(QString("./config.ini"), QString("language"), language_value);
	if(is_read)
	{
		bool ok;
		lang = (LANGUAGE)language_value.toInt(&ok);
		if (ok)
		{
			switch (lang)
			{
			case UI_ZH:
				translator.load(":/i18n/rms_update_zh.qm");
				break;
			case UI_EN:
				translator.load(":/i18n/rms_update_en.qm");
				break;
			default:
				translator.load(":/i18n/rms_update_zh.qm");
				break;
			}
		}
		else
		{
			lang = UI_ZH;
			translator.load(":/i18n/rms_update_zh.qm");
		}
	}
	else
	{
		lang = UI_ZH;
		translator.load(":/i18n/rms_update_zh.qm");
	}
	a.installTranslator(&translator);

	QCoreApplication::setApplicationVersion(G_VERSION);
	QCoreApplication::setApplicationName(QCoreApplication::applicationFilePath().section('/',-1));

	rms_update w;
	w.setTranslator(&translator);
	w.setLanguage(lang);
	w.move((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);
	w.show();
	return a.exec();
}
