#ifndef SKPROCESS_H
#define SKPROCESS_H

#include <QtGui>
#include <QObject>

class skProcess : public QObject
{
	Q_OBJECT

public:
	skProcess(QObject *parent = NULL);
	~skProcess();

private:
	
public:
	int  getSystemProcess(QStringList &plist);
	bool getProcessByName(int &pid,int &ppid,QString name);
	bool closeProcess(int pid);
};

#endif // SKPROCESS_H
