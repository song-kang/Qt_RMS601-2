#include "passwdlineedit.h"

passwdLineEdit::passwdLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	//设置无右键菜单
	this->setContextMenuPolicy(Qt::NoContextMenu);
	//设置密码提示
	this->setPlaceholderText(QString(tr("please input password...")));
	//设置密码隐藏
	this->setEchoMode(QLineEdit::Password);
	//设置最大长度32位
	this->setMaxLength(32);
}

passwdLineEdit::~passwdLineEdit()
{

}

//屏蔽输入框全选、复制、粘贴功能
void passwdLineEdit::keyPressEvent(QKeyEvent *event)
{
	if(event->matches(QKeySequence::SelectAll))
	{   
		return ;   
	}
	else if(event->matches(QKeySequence::Copy))
	{   
		return ;   
	}
	else if(event->matches(QKeySequence::Paste))
	{   
		return ;   
	}
	else
	{
		QLineEdit::keyPressEvent(event);
	}
} 

//设置鼠标位于密码框中时不可移动
void passwdLineEdit::mouseMoveEvent(QMouseEvent *)
{
	return ;
}

//设置鼠标双击无效
void passwdLineEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
	return ;
}
