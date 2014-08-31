#ifndef DLGOPTION_H
#define DLGOPTION_H

#include <QDialog>
#include "ui_dlgoption.h"
#include "database.h"

class DlgOption : public QDialog
{
	Q_OBJECT

public:
	DlgOption(QWidget *parent = 0);
	~DlgOption();

	void setDatabase(CDatabase *db) { appDb = db; };
	CDatabase *getDatabase() { return appDb; };
	bool getZipFileFlag() { return bZipFile; };

	void setTranslator(QTranslator *trans) { translator = trans; };
	void setLanguage(int langu) { current_language = langu; };
	int  getLanguage() { return current_language; };

private:
	Ui::DlgOption ui;

	CDatabase	 *appDb;
	bool		 bZipFile;
	QTranslator  *translator;
	int			 current_language;

protected:
	void closeEvent(QCloseEvent * event);

public slots:
	void slotButtonOk();
};

#endif // DLGOPTION_H
