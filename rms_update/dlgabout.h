#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QtGui>
#include <QDialog>
#include "ui_dlgabout.h"

class dlgAbout : public QDialog
{
	Q_OBJECT

public:
	dlgAbout(QWidget *parent = 0);
	~dlgAbout();

private:
	Ui::dlgAbout ui;

	void init();

	QString getVersionInformation();

public slots:
	void slotOk();

};

#endif // DLGABOUT_H
