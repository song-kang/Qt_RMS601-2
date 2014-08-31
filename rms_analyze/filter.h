#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QtGui>

#define STR_ADDR		"addr"
#define STR_SRC_ADDR	"srcaddr"
#define STR_DST_ADDR	"dstaddr"
#define STR_SRC_PORT	"srcport"
#define STR_DST_PORT	"dstport"
#define STR_LENGTH		"length"
#define STR_PROTOCOL	"protocol"

class filter : public QObject
{
	Q_OBJECT

public:
	filter(QObject *parent);
	~filter();

	void setCompleter(QLineEdit *lineEdit);
	bool parseFilterString(QString strFilter);

private:
	QCompleter *completer; 
	
};

#endif // FILTER_H
