#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>
#include <QLibrary>
#include "ui_dlgabout.h"
#include "skStaticClass.h"

#define PAGE_NETSHARK		0
#define PAGE_PLUGINS		1

#define	APP_VERSION			"1.0.0"

typedef struct lib_plugin_t lib_plugin_t;
struct lib_plugin_t {
	QLibrary	*lib;
	QString		name;
};

class DlgAbout : public QDialog
{
	Q_OBJECT

public:
	DlgAbout(QWidget *parent = 0);
	~DlgAbout();

	void setLibNum(int num) { loadLibNum = num; };
	void setLibProtocol(lib_plugin_t *lib) { protocolib = lib; };

private:
	Ui::DlgAbout ui;

	int				loadLibNum;
	lib_plugin_t	*protocolib;

private:
	QString	strReadme;

private:
	void initTableWidget();
	void initTextBrower();
	typedef void (*GetLibInfo)(QString &version,QString &author,QString &decsription);
	void readme();

public slots:
	void slotButtonOk();
	void slotTabCurrentChanged(int page);
};

#endif // DLGABOUT_H
