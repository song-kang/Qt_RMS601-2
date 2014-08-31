#include "dlgoption.h"

DlgOption::DlgOption(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	int value;
	if (appDb->getOptionLanguage(value))
	{
		if (value == LANGUAGE_VAL_CH)
			ui.comboBox->setCurrentIndex(0);
		else if (value == LANGUAGE_VAL_EN)
			ui.comboBox->setCurrentIndex(1);
	}
	else
		ui.comboBox->setCurrentIndex(0);

	if (appDb->getOptionZipMode(value))
	{
		if (value == ZIPMODE_VAL_ZERO)
		{
			ui.zipCheckBox->setCheckState(Qt::Unchecked);
			bZipFile = false;
		}
		else if (value == ZIPMODE_VAL_VAILD)
		{
			ui.zipCheckBox->setCheckState(Qt::Checked);
			bZipFile = true;
		}
	}
	else
	{
		ui.zipCheckBox->setCheckState(Qt::Unchecked);
		bZipFile = false;
	}

	connect(ui.okButton,SIGNAL(clicked()),this,SLOT(slotButtonOk()));
	connect(ui.cancelButton,SIGNAL(clicked()),this,SLOT(close()));
}

DlgOption::~DlgOption()
{

}

void DlgOption::slotButtonOk()
{
	bool ok = true;

	int ret = ui.zipCheckBox->checkState();
	if (ret == Qt::Unchecked)
	{
		ok = appDb->saveOptionZipMode(ZIPMODE_VAL_ZERO);
		if (ok)
			bZipFile = false;
	}
	else if (ret == Qt::Checked)
	{
		ok = appDb->saveOptionZipMode(ZIPMODE_VAL_VAILD);
		if (ok)
			bZipFile = true;
	}
	if (!ok)
		QMessageBox::warning(this,tr("Message"),tr("Save zipmode option failed! "));

	int value;
	bool bMessage = false;
	appDb->getOptionLanguage(value);
	QString str = ui.comboBox->currentText();
	if (str == tr("Chinese"))
	{
		ok = appDb->saveOptionLanguage(LANGUAGE_VAL_CH);
		if (value != LANGUAGE_VAL_CH)
		{
			value = LANGUAGE_VAL_CH;
			bMessage = true;
		}
	}
	else if (str == tr("English"))
	{
		ok = appDb->saveOptionLanguage(LANGUAGE_VAL_EN);
		if (value != LANGUAGE_VAL_EN)
		{
			value = LANGUAGE_VAL_EN;
			bMessage = true;
		}
	}
	if (!ok)
		QMessageBox::warning(this,tr("Message"),tr("Save language option failed! "));
	else if (bMessage)
	{
		switch (value)
		{
		case LANGUAGE_VAL_EN:
			current_language = LANGUAGE_VAL_EN;
			translator->load(":/i18n/rms_analyze_en.qm");
			break;
		case LANGUAGE_VAL_CH:
			current_language = LANGUAGE_VAL_CH;
			translator->load(":/i18n/rms_analyze_zh.qm");
			break;
		default:
			current_language = LANGUAGE_VAL_CH;
			translator->load(":/i18n/rms_analyze_zh.qm");
			break;
		}
		ui.retranslateUi(this);
	}
		
	accept();
}

void DlgOption::closeEvent(QCloseEvent * event)
{
	QDialog::closeEvent(event);
	reject();
	this->close();
}
