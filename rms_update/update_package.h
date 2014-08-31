#ifndef UPDATE_PACKAGE_H
#define UPDATE_PACKAGE_H

#include <QObject>
#include "skStaticClass.h"

class update_package : public QObject
{
	Q_OBJECT

public:
	update_package(QObject *parent);
	~update_package();

public:
	void setName(QString m_name) { name = m_name; };
	void setMd5(QString m_md5)   { md5 = m_md5; };
	void setPath(QString m_path) { path = m_path; };

	QString getName() { return name; };
	QString getMd5()  { return md5; };
	QString getPath() { return path; };

	bool md5Compare();

private:
	QString name;
	QString md5;
	QString path;
	
};

#endif // UPDATE_PACKAGE_H
