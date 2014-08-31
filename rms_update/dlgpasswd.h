#ifndef DLGPASSWD_H
#define DLGPASSWD_H

#include <QtGui>
#include <QDialog>
#include "ui_dlgpasswd.h"

#define PASSWORD_VAL	"=="

class dlgPasswd : public QDialog
{
	Q_OBJECT

public:
	dlgPasswd(QWidget *parent = 0);
	~dlgPasswd();

private:
	Ui::dlgPasswd ui;

	void init();

public slots:
	void slotOk();
};

#endif // DLGPASSWD_H
