#include "dlgsetting.h"
#include "util.h"

dlgSetting::dlgSetting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	init();

	connect(ui.okBtn,SIGNAL(clicked()),this,SLOT(slotOk()));
	connect(ui.cancelBtn,SIGNAL(clicked()),this,SLOT(close()));
}

dlgSetting::~dlgSetting()
{

}

void dlgSetting::init()
{
	ui.LanguageComboBox->setFixedWidth(190);
}

void dlgSetting::slotOk()
{
	current_language = (LANGUAGE)ui.LanguageComboBox->currentIndex();

	switch (current_language)
	{
	case UI_ZH:
		translator->load(":/i18n/rms_update_zh.qm");
		break;
	case UI_EN:
		translator->load(":/i18n/rms_update_en.qm");
		break;
	default:
		translator->load(":/i18n/rms_update_zh.qm");
		break;
	}
	
	Util::writeInit(QString("./config.ini"), QString("language"), QString::number(current_language, 10));

	ui.retranslateUi(this);

	accept();
}


