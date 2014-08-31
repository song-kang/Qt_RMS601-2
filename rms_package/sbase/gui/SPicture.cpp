/**
 *
 * 文 件 名 : SPicture.cpp
 * 创建日期 : 2008-02-15 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
 *            $Log: $
 *
 **/
#include "SPicture.h"

#ifdef QT_GUI_LIB
#include <QPainter>
#else
#endif
SPicture::SPicture()
{
	m_iWidth = m_iHeight = 0;
#ifdef QT_GUI_LIB
#else
#ifdef WIN32
	m_hBitmap = NULL;
	m_hDCMem = NULL;
#else
	m_pBitmap = NULL;
#endif
#endif

}

SPicture::~SPicture()
{
#ifdef QT_GUI_LIB
#else
#ifdef WIN32
	if(m_hDCMem != NULL)
	{	
		DeleteDC(m_hDCMem);
		m_hDCMem = NULL;
	}
#else
	if(m_pBitmap != NULL)
	{
		UnloadBitmap(m_pBitmap);
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
#endif
#endif
}

bool SPicture::Load(char* sFileName)
{
#ifdef QT_GUI_LIB
	return m_pixmap.load(QString(sFileName));
#else
#ifdef WIN32
  m_hBitmap = (HBITMAP) LoadImage(NULL,			// Want to load bitmap from file
												sFileName,		// Filename where bitmap is stored
												IMAGE_BITMAP,				// Type of image to be loaded
												0, 0,								// The desired width and height
												LR_LOADFROMFILE); 
	if(m_hBitmap == NULL)
		return false;

	BITMAP bmp;
	GetObject(m_hBitmap, sizeof(BITMAP),(LPVOID) &bmp);
	m_iWidth = bmp.bmWidth;
	m_iHeight= bmp.bmHeight;
#else
	if(m_pBitmap != NULL)
	{
		UnloadBitmap(m_pBitmap);
		delete m_pBitmap;
	}
	m_pBitmap = new BITMAP;
	if (LoadBitmap (HDC_SCREEN, m_pBitmap, sFileName))
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
		return false;
	}
#endif
	return true;
#endif
}

void SPicture::Render(unsigned int uDC,int x,int y,int width,int height)
{
#ifdef QT_GUI_LIB
	HDC hDC = (HDC)uDC;
	QPainter *p = (QPainter*)hDC;
	if(p)
	{
		p->drawPixmap(x,y,width,height,m_pixmap);
	}
#else
	HDC hDC = (HDC)uDC;
#ifdef WIN32
	if(m_hDCMem == NULL)
	{
		m_hDCMem = CreateCompatibleDC(hDC);
		SelectObject (m_hDCMem, m_hBitmap);
		SetMapMode (m_hDCMem, GetMapMode(hDC));// Draw the bitmap using the BitBlt function
	}
//	BitBlt(hDC,				// The destination device context             
//		x, y,               // Coordinates of the destination rectangle             
//		width,							// Width of the dest. and source rectangle             
//		height,							// Height of the dest. and source rectangle             
//		m_hDCMem,             
//		0, 0,             
//		SRCCOPY);	
	StretchBlt(hDC,				// The destination device context             
		x, y,               // Coordinates of the destination rectangle             
		width,							// Width of the dest. and source rectangle             
		height,							// Height of the dest. and source rectangle             
		m_hDCMem,             
		0, 0, m_iWidth, m_iHeight,             
		SRCCOPY);	
#else
	if(m_pBitmap != NULL)
		FillBoxWithBitmap (hDC, x, y, width, height, m_pBitmap);
#endif
#endif
}
