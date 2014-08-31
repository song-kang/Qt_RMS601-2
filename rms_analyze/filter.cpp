#include "filter.h"

filter::filter(QObject *parent)
	: QObject(parent)
{

}

filter::~filter()
{
	
}

void filter::setCompleter(QLineEdit *lineEdit)
{
	QStringList word_list;
	word_list<<STR_ADDR<<STR_SRC_ADDR<<STR_DST_ADDR<<STR_SRC_PORT<<STR_DST_PORT<<STR_LENGTH<<STR_PROTOCOL;
	completer = new QCompleter(word_list);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	lineEdit->setCompleter(completer);
}

bool filter::parseFilterString(QString strFilter)
{
	QString str = strFilter.replace(" ",""); //È¥³ý¿Õ¸ñ
	return true;
}
