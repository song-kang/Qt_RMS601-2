#ifndef DLGCONNECT_H
#define DLGCONNECT_H

#include <QtGui>
#include <QDialog>
#include <QtNetwork>
#include <QHostAddress>
#include <QAbstractSocket>
#include "ui_dlgconnect.h"
#include "database.h"

#define DEFAULT_PORT	"8791"
#define SECOND			1000

class DlgConnect : public QDialog
{
	Q_OBJECT

public:
	DlgConnect(QWidget *parent = 0);
	~DlgConnect();

	void setHostName(QString hName) { hostName = hName; };
	void setHostPort(int hPort) { hostPort = hPort; };
	void setDatabase(CDatabase *db) { appDb = db; };

	QString   getHostName() { return hostName; };
	int		  getHostPort() { return hostPort; };
	CDatabase *getDatabase() { return appDb; };

private:
	Ui::DlgConnect ui;

	QString hostName;
	int		hostPort;

	CDatabase *appDb;

protected:
	void closeEvent(QCloseEvent * event);

public slots:
	void slotButtonOk();

};

#endif // DLGCONNECT_H
