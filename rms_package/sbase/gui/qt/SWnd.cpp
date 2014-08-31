
#include "SWnd.h"

#include <QtGui>

#include "../../SList.h"
static SWnd* g_pMainWnd = NULL;

#ifdef WIN32
static SPtrList<SWnd::stuWndAttach> g_WndAttach;
SWnd* SWnd::GetAttachWnd(HWND hWnd)
{
	int i,cnt = g_WndAttach.count();
	for(i=0;i<cnt;i++)
	{
		SWnd::stuWndAttach *pAttach = g_WndAttach[i];
		if(pAttach == NULL)
			continue;
		if(pAttach->hWnd == NULL)
		{
			pAttach->hWnd = hWnd;
			pAttach->pWnd->m_hWnd = hWnd;
		}
		if(pAttach->hWnd == hWnd)
			return pAttach->pWnd;
	}
	return NULL;
}

void SWnd::AddAttachWnd(HWND hWnd,SWnd* pWnd)
{
	SWnd::stuWndAttach *pAttach = new SWnd::stuWndAttach();
	pAttach->hWnd = hWnd;
	pAttach->pWnd = pWnd;
	g_WndAttach.append(pAttach);
}

void SWnd::RemoveAttachWnd(HWND hWnd)
{
	int i,cnt = g_WndAttach.count();
	for(i=0;i<cnt;i++)
	{
		SWnd::stuWndAttach *pAttach = g_WndAttach[i];
		if(pAttach == NULL)
			continue;
		if(pAttach->hWnd == hWnd)
		{
			g_WndAttach.remove(pAttach);
		}
	}
}

void SWnd::RemoveAttachWnd(SWnd* pWnd)
{
	int i,cnt = g_WndAttach.count();
	for(i=0;i<cnt;i++)
	{
		SWnd::stuWndAttach *pAttach = g_WndAttach[i];
		if(pAttach == NULL)
			continue;
		if(pAttach->pWnd == pWnd)
		{
			g_WndAttach.remove(pAttach);
		}
	}
}
#endif

void SWnd::mousePressEvent(QMouseEvent *event)
{
	/*

Qt::NoButton 0x00000000 The button state does not refer to any button (see QMouseEvent::button()). 
Qt::LeftButton 0x00000001 The left button is pressed, or an event refers to the left button. (The left button may be the right button on left-handed mice.) 
Qt::RightButton 0x00000002 The right button. 
Qt::MidButton 0x00000004 The middle button. 
Qt::MiddleButton MidButton The middle button. 
Qt::XButton1 0x00000008 The first X button. 
Qt::XButton2 0x00000010 The second X button. 

			*/
	//m_x = event->x();
	//m_y = event->y();
	switch(event->button())
	{
	case Qt::LeftButton:
		OnLButtonDown(0,SPoint(event->x(),event->y()));
		//m_iMouseAct = 1;
		break;
	case Qt::RightButton:
		OnRButtonDown(0,SPoint(event->x(),event->y()));
		//m_iMouseAct = 2;
		break;
	}
	//m_pTimer->start(200);
	
}

void  SWnd::mouseReleaseEvent ( QMouseEvent * event )
{
	if(event->button() == Qt::LeftButton)
		OnLButtonUp(0,SPoint(event->x(),event->y()));
	else if(event->button() == Qt::RightButton)
		OnRButtonUp(0,SPoint(event->x(),event->y()));
}
/*
void SWnd::mouseClick()
{
	m_pTimer->stop();
	if(m_iMouseAct == 1)
		OnLButtonDown(0,SPoint(m_x,m_y));
	else if(m_iMouseAct == 2)
		OnRButtonDown(0,SPoint(m_x,m_y));
	else  if(m_iMouseAct == 3)
		OnLButtonUp(0,SPoint(m_x,m_y));
	else  if(m_iMouseAct == 4)
		OnRButtonUp(0,SPoint(m_x,m_y));
}*/

void SWnd::mouseDoubleClickEvent(QMouseEvent *event)
{
	//m_pTimer->stop();
	switch(event->button())
	{
	case Qt::LeftButton:
		OnLButtonDblClk(0,SPoint(event->x(),event->y()));
		break;

	}

}

void  SWnd::mouseMoveEvent ( QMouseEvent * event )
{
	OnMouseMove(0,SPoint(event->x(),event->y()));
}

void SWnd::wheelEvent(QWheelEvent *event)
{
	OnMouseWheel(0,event->delta());
}
void SWnd::paintEvent(QPaintEvent *)
{
	SDC dc(new QPainter(this));
	OnPaint(&dc);
}



//////////////////////////////////////////////////////////////////////////
// 描    述:  
// 作    者:  邵凯田
// 创建时间:  2007-12-20
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SWnd::RedrawWindow()
{
	update();

	//::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	//::InvalidateRect(m_hWnd,NULL,FALSE);
	return true;
}


YSRECT SWnd::GetClientRect()
{
	YSRECT r;
	r.left = 0;
	r.right= width();
	r.top  = 0;
	r.bottom= height();
	return r;
}



//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

#ifdef WIN32
LRESULT CALLBACK SWnd::SWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{/*
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//TCHAR szHello[256];
	
	SWnd *pWnd = GetAttachWnd(hWnd);
	if(pWnd == NULL)
	{
		YSWARN("未找到指定窗口句柄(%d)的对应窗口类!",hWnd);
	}
	else
	{
		pWnd->OnMessage(message,wParam,lParam);
	}
	switch (message) 
	{
		case WM_COMMAND:
			if(pWnd)
				pWnd->OnCommand(wParam, lParam);
//			wmId    = LOWORD(wParam);
//			wmEvent = HIWORD(wParam); 
//			// Parse the menu selections:
//			switch (wmId)
//			{
//				//case IDM_ABOUT:
//				//   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
//				//   break;
//				//case IDM_EXIT:
//				//   DestroyWindow(hWnd);
//				//   break;
//				default:
//				   return DefWindowProc(hWnd, message, wParam, lParam);
//			}

			break;

		case WM_ERASEBKGND:
				break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			if(pWnd && pWnd->m_bUserDraw)
			{
				HDC memdc = ::CreateCompatibleDC(hdc);
				RECT rcClient;
				::GetClientRect(hWnd,&rcClient);
				HBITMAP hbmp = ::CreateCompatibleBitmap(hdc,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top);
				::SelectObject(memdc,hbmp);
				SDC dc(memdc);
				pWnd->OnPaint(&dc);
			    ::BitBlt(hdc, 0, 0, rcClient.right-rcClient.left,rcClient.bottom-rcClient.top, memdc, 0, 0,SRCCOPY);
				::DeleteObject(hbmp);
				::DeleteDC(memdc);
			}
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			if(pWnd)
				pWnd->OnClose();
			if(g_pMainWnd == pWnd)
				PostQuitMessage(0);
			break;
		case WM_CREATE:
			if(pWnd)
				pWnd->OnCreate();
			break;
		case WM_SIZE:
			if(pWnd)
				pWnd->OnSize(wParam, LOWORD(lParam),HIWORD(lParam));
			break;
		case WM_KEYDOWN:
			if(pWnd)
				pWnd->OnKeyDown(wParam,lParam);
			break;
		case WM_KEYUP:
			if(pWnd)
				pWnd->OnKeyUp(wParam,lParam);
			break;
		case WM_LBUTTONDBLCLK:
			if(pWnd)
				pWnd->OnLButtonDblClk(wParam,SPoint(LOWORD(lParam),HIWORD(lParam)));
			break;
		case WM_LBUTTONDOWN:
				if(pWnd)  
					pWnd->OnLButtonDown(wParam,SPoint(LOWORD(lParam),HIWORD(lParam)));
				break;
		case WM_LBUTTONUP:
			if(pWnd)
				pWnd->OnLButtonUp(wParam,SPoint(LOWORD(lParam),HIWORD(lParam)));
			break;

		case WM_MOUSEMOVE:
			if(pWnd)
				pWnd->OnMouseMove(wParam,SPoint(LOWORD(lParam),HIWORD(lParam)));
			break;
		case WM_RBUTTONDBLCLK:
			if(pWnd)
				pWnd->OnRButtonDblClk(wParam,SPoint(LOWORD(lParam),HIWORD(lParam)));
			break;
		case WM_RBUTTONDOWN:
			if(pWnd)
				pWnd->OnRButtonDown(wParam,SPoint(LOWORD(lParam),HIWORD(lParam)));
			break;
		case WM_RBUTTONUP:
			if(pWnd)
				pWnd->OnRButtonUp(wParam,SPoint(LOWORD(lParam),HIWORD(lParam)));
			break;
		case WM_TIMER:
			if(pWnd)
				pWnd->OnTimer(wParam);
			break;
		case WM_NOTIFY:
			{
				LPNMLISTVIEW lpnmlv=(LPNMLISTVIEW)lParam; 
				switch(lpnmlv-> hdr.code) 
				{ 
				case   NM_DBLCLK: 
					{
						int j=0;
						string str1;
						j=lpnmlv->iItem;
						pWnd->OnNmclick(j,SPoint(LOWORD(lParam),HIWORD(lParam)));
					}
					break;
					
				default:
					return  DefWindowProc(hWnd, message, wParam, lParam);
				} 	
			}
			break;

		case WM_VSCROLL:
			if(pWnd)
				pWnd->OnVscroll(wParam,lParam);
			break;
		case WM_HSCROLL:
			if(pWnd)
				pWnd->OnHscroll(wParam,lParam);
			break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }*/
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// 描    述:  显示窗口
// 作    者:  邵凯田
// 创建时间:  2007-12-20
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SWnd::ShowWindow( int nCmdShow )
{
	if(m_hWnd == NULL)
		return false;
	return ::ShowWindow(m_hWnd,nCmdShow)==TRUE;
}

#endif

SBaseDC* SWnd::CreateDC()
{
	SDC *pDC = new SDC();
	QPainter *p = new QPainter(this);
	pDC->Attach(p);
	//HDC hdc = BeginPaint(m_hWnd,&(pDC->m_PS));
	//pDC->Attach(hdc);
	return pDC;
}

void SWnd::ReleaseDC(SBaseDC *pDC)
{
	SDC *pWDC = (SDC*)pDC;
#ifdef WIN32
	EndPaint(m_hWnd, &(pWDC->m_PS));
#endif
	delete pWDC;
}

#ifdef WIN32

//////////////////////////////////////////////////////////////////////////
// 描    述:  创建窗口
// 作    者:  邵凯田
// 创建时间:  2007-12-20
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SWnd::Create(char *sClassName,char *sWinName,unsigned long style,
										 int x,int y,int w,int h,
										 SWnd *pParentWnd,unsigned int nID,unsigned long dwAddData)
{
	WNDCLASSEX wcex;
	HINSTANCE hInstance = NULL;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style				= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)SWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon				= NULL;
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= "";
	wcex.lpszClassName= sClassName;
	wcex.hIconSm			= NULL;
	ATOM a=RegisterClassEx(&wcex);
	//空的窗口
 	AddAttachWnd(NULL,this);

  m_hWnd = CreateWindow(sClassName, sWinName, style,// WS_OVERLAPPEDWINDOW,
		x, y, w, h, 
		pParentWnd==NULL?NULL:pParentWnd->m_hWnd, NULL, hInstance, NULL);

	if(m_hWnd == NULL)
		return false;
	g_WndAttach.setAutoDelete(true);
	m_pParentWnd = pParentWnd;
	//OnCreate();
	return true;
}

int SWnd::OnCreate()
{
	//添加当前窗口到映射列表
 	AddAttachWnd(m_hWnd,this);

	UpdateWindow(m_hWnd);
	static HFONT hFont = NULL;
	if(hFont == NULL)
	{
		hFont = CreateFont(	24,
												0,
												0,
												0,
												500,
												0,         // italic attribute flag
												0,      // underline attribute flag
												0,      // strikeout attribute flag
												DEFAULT_CHARSET,        // character set identifier
												0,  // output precision
												0,  // clipping precision
												0,        // output quality
												0,  // pitch and family
												"宋体");

    hFont = CreateFont (24, 0, 0, 0, 400,
			0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			TEXT("宋体")); 
	}
	::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, MAKELONG (TRUE, 0));
	::SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)SWndProc);
	return 1;
}

bool SWnd::SendMessage(int message,DWORD wParam,DWORD lParam)
{
	if(this == NULL)
		return false;
	::SendMessage(m_hWnd,message,wParam,lParam);
	return true;
}

int SWnd::MessageBox(char* sMsg,char* sTitle,DWORD style)
{
	return (int)::MessageBox(m_hWnd,sMsg,sTitle,style);
}


bool SWnd::SetTimer(int nIDEvent,int nElapse)
{
	return ::SetTimer(m_hWnd,nIDEvent,nElapse,NULL) != 0;
}

bool SWnd::KillTimer(int nIDEvent)
{
	return ::KillTimer(m_hWnd,nIDEvent) != 0;
}

#endif
