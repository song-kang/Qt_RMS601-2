#ifndef PASSWDLINEEDIT_H
#define PASSWDLINEEDIT_H

#include <QtGui>
#include <QLineEdit>

class passwdLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	passwdLineEdit(QWidget *parent);
	~passwdLineEdit();

private:
	void keyPressEvent(QKeyEvent *event);
	void mouseMoveEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // PASSWDLINEEDIT_H
