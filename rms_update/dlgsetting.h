#ifndef DLGSETTING_H
#define DLGSETTING_H

#include <QtGui>
#include <QDialog>
#include "ui_dlgsetting.h"

typedef enum{
	UI_ZH,
	UI_EN
}LANGUAGE;

class dlgSetting : public QDialog
{
	Q_OBJECT

public:
	dlgSetting(QWidget *parent = 0);
	~dlgSetting();

	void setTranslator(QTranslator *trans) { translator = trans; };
	void setLanguage(LANGUAGE langu) { current_language = langu; 
									   ui.LanguageComboBox->setCurrentIndex(current_language); };
	int  getLanguage() { return current_language; };

private:
	void init();

private:
	Ui::dlgSetting ui;

	QTranslator  *translator;
	LANGUAGE	  current_language;

public slots:
	void slotOk();
};

#endif // DLGSETTING_H
