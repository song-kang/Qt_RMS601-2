#ifndef DATABASE_H
#define DATABASE_H

#include <QtGui>
#include <QMessageBox>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#define LANGUAGE_NAME		"language"
#define LANGUAGE_VAL_ZERO	0
#define LANGUAGE_VAL_EN		1
#define LANGUAGE_VAL_CH		2
#define ZIPMODE_NAME		"zipMode"
#define ZIPMODE_VAL_ZERO	0
#define ZIPMODE_VAL_VAILD	1

typedef struct connect_data_t connect_data_t;
struct connect_data_t {
	QString	addr;
	int		port;
};

class CDatabase : public QObject
{
	Q_OBJECT

public:
	CDatabase(QObject *parent);
	~CDatabase();

	QSqlDatabase getDatabase() { return m_db; };

public:
	bool createDatabase(QString name);
	bool saveConnectData(QString addr, int port);
	bool saveOptionLanguage(int val);
	bool saveOptionZipMode(int val);
	int  getConnectDataNum();
	int  getConnectDataNum(QString addr, int port);
	bool getConnectData(QList<connect_data_t> *cData);
	bool getOptionLanguage(int &val);
	bool getOptionZipMode(int &val);
	bool existOptionLanguage();
	bool existOptionZipMode();
private:
	QSqlDatabase m_db;
	
};

#endif // DATABASE_H
