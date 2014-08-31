#ifndef DLGEMAIL_H
#define DLGEMAIL_H

#include <QtGui>
#include <QDialog>
#include "ui_dlgemail.h"
#include "../email/SmtpMime"

#define	EMAIL_HOST		"smtp.163.com"
#define EMAIL_PORT		25
#define EMAIL_USER		"rms601@163.com"
#define EMAIL_SENDER	"rms601@163.com"
#define EMAIL_RECVER	"rms601@163.com"
#define EMAIL_PASSWORD	"dlkzrms601"

class DlgEmail : public QDialog
{
	Q_OBJECT

public:
	DlgEmail(QWidget *parent = 0);
	~DlgEmail();

private:
	Ui::DlgEmail ui;

private:
	EmailAddress* stringToEmail(const QString &str);

public slots:
	void slotSendEmail();
};

#endif // DLGEMAIL_H
