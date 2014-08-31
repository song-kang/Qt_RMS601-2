/**
 *
 * 文 件 名 : SWnd.h
 * 创建日期 : 2007-12-20 15:22
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/

#ifndef __SKT_QT_WND_H__
#define __SKT_QT_WND_H__

#ifndef QT_GUI_LIB
#error "You cann't use qt svg code under non-qt plateform!!!"
#endif

#include "../SBaseWnd.h"
#include "SDC.h"

#pragma once
#include <QWidget>

class SWnd : public QWidget,public SBaseWnd
{
	Q_OBJECT
public:
#ifdef WIN32
	struct stuWndAttach
	{
		HWND hWnd;
		SWnd *pWnd;
	};
	HWND m_hWnd;
	HDC	 m_hMemDC;
#endif
public:
	SWnd()
	{
#ifdef WIN32
		m_hWnd = NULL;
		m_hMemDC = NULL;
#endif
		m_pParentWnd = NULL;
		m_bUserDraw = true;
		//m_pTimer = new QTimer(this);
		//connect(m_pTimer,SIGNAL(timeout()),this,SLOT(mouseClick()));
	}
	SWnd(QWidget *parent) :QWidget(parent)
	{
#ifdef WIN32
		m_hWnd = NULL;
		m_hMemDC = NULL;
#endif
		m_pParentWnd = (SWnd*)parent;
		m_bUserDraw = true;
	}
	virtual ~SWnd()
	{
	}
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void  mouseReleaseEvent ( QMouseEvent * event );

	//public slots:
	//void mouseClick();

public:

	void  mouseMoveEvent ( QMouseEvent * event );
	void wheelEvent(QWheelEvent *event);
	void paintEvent(QPaintEvent *event);
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  
	// 作    者:  邵凯田
	// 创建时间:  2007-12-20
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool RedrawWindow();

#ifdef WIN32
	static SWnd* GetAttachWnd(HWND hWnd);
	static void AddAttachWnd(HWND hWnd,SWnd* pWnd);
	static void RemoveAttachWnd(HWND hWnd);
	static void RemoveAttachWnd(SWnd* pWnd);
	bool IsWindow()
	{
		return m_hWnd == NULL;
	}
	inline SWnd* GetParentWindow()
	{
		return m_pParentWnd;
	}
	inline void SetUserDraw(bool b)
	{
		m_bUserDraw = b;
	}
	virtual bool IsWindowVisible()
	{
		return ((bool)::IsWindowVisible(m_hWnd));
	}
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  创建窗口
	// 作    者:  邵凯田
	// 创建时间:  2007-12-20
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool Create(char *sClassName,char *sWinName,unsigned long style,
		int x,int y,int w,int h,
		SWnd *pParentWnd=NULL,unsigned int nID=0,unsigned long dwAddData=0);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  显示窗口
	// 作    者:  邵凯田
	// 创建时间:  2007-12-20
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool ShowWindow( int nCmdShow );

#endif

	virtual SBaseDC* CreateDC();
	virtual void ReleaseDC(SBaseDC *pDC);
	virtual bool OnPaint(SBaseDC *pDC){return true;};

#ifdef WIN32
	virtual bool OnCommand(DWORD wParam, DWORD lParam){return true;};
	virtual bool OnMessage(int message,DWORD wParam, DWORD lParam){return true;};
	virtual int OnCreate();
	virtual int OnClose(){return 1;};
	virtual int OnSize(int type,int cx,int cy){return 1;};
	virtual int OnKeyDown(int nVirtKey,int iKeyData){return 1;};
	virtual int OnKeyUp(int nVirtKey,int iKeyData){return 1;};
	virtual int OnLButtonDblClk(int nFlag,SPoint point){return 1;};
	virtual int OnLButtonDown(int nFlag,SPoint point){return 1;};
	virtual int OnLButtonUp(int nFlag,SPoint point){return 1;};
	virtual int OnMouseMove(int nFlag,SPoint point){return 1;};
	virtual int OnMouseWheel(int nFlag,int delta){return 1;}
	virtual int OnRButtonDblClk(int nFlag,SPoint point){return 1;};
	virtual int OnRButtonDown(int nFlag,SPoint point){return 1;};
	virtual int OnRButtonUp(int nFlag,SPoint point){return 1;};
	virtual int OnNmclick(int nFlag,SPoint point){return 1;};//qcj	
	virtual int OnVscroll(DWORD wParam, DWORD lParam){return 1;};
    virtual int OnHscroll(DWORD wParam, DWORD lParam){return 1;};
	virtual int OnTimer(int nIDEvent){return 1;};
	virtual bool SetTimer(int nIDEvent,int nElapse);
	virtual bool KillTimer(int nIDEvent);
	virtual bool SendMessage(int message,DWORD wParam,DWORD lParam);
	int MessageBox(char* sMsg,char* sTitle,DWORD style);
#endif
	YSRECT GetClientRect();
private:
#ifdef WIN32
	static LRESULT CALLBACK SWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif
	SWnd *m_pParentWnd;
	bool m_bUserDraw;//是否用户自画窗口
//	QTimer *m_pTimer;
//	int m_iMouseAct;//1左按下，2右按下，3左起，4右起
//	int m_x,m_y;//最后的点击位置
};

#endif //__SKT_QT_WND_H__

