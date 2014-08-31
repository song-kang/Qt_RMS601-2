/**
 *
 * 文 件 名 : SDC.h
 * 创建日期 : 2008-01-17 11:22
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 
 * 修改记录 : 
 *            $Log: $
 *
 **/

#ifndef __SKT_QT_DC_H__
#define __SKT_QT_DC_H__

#include <QtGui>

#ifndef QT_GUI_LIB
#error "You cann't use qt svg code under non-qt plateform!!!"
#endif

#include "../SBaseDC.h"
#ifndef HDC
#define HDC QPainter*
#endif

class SDC : public SBaseDC
{
public:

	QPainter *m_hDC;
#ifdef WIN32
	PAINTSTRUCT m_PS;
#endif


public:
	SDC()
	{
		m_hDC = NULL;
		m_x = m_y = 0;
	}
	SDC(HDC hDC)
	{
		m_x = m_y = 0;
		m_hDC = (QPainter*)hDC;
	}
	virtual ~SDC()
	{
		if(m_hDC != NULL)
		{
			delete m_hDC;
		}
	}

	void Attach(HDC hDC)
	{
		m_hDC = (QPainter*)hDC;
	}
	virtual unsigned int GetHandle()
	{
		return (unsigned int)m_hDC;
	}
	virtual SSize GetTextSize(char *pStr)
	{
		SSize s;
		QFontMetrics fm(m_hDC->font());
		s.cx = fm.width(pStr);
		s.cy = fm.height();
		return s;
	}
	virtual SPen* SelectObject(SPen *p)
	{
/*
Qt::NoPen 0 no line at all. For example, QPainter::drawRect() fills but does not draw any boundary line. 
Qt::SolidLine 1 A plain line. 
Qt::DashLine 2 Dashes separated by a few pixels. 
Qt::DotLine 3 Dots separated by a few pixels. 
Qt::DashDotLine 4 Alternate dots and dashes. 
Qt::DashDotDotLine 5 One dash, two dots, one dash, two dots. 
Qt::CustomDashLine 6 A custom pattern defined using QPainterPathStroker::setDashPattern(). 

*/
		m_hDC->setPen((Qt::PenStyle)p->m_iStyle);
		m_hDC->setPen(QColor(p->m_Color.r, p->m_Color.g, p->m_Color.b));
		QPen pen = m_hDC->pen();
		pen.setWidth(p->m_iWidth);//改变线宽
		m_hDC->setPen(pen);
		return p;
	}
	virtual SFont* SelectObject(SFont *p)
	{
		QFont font(p->szFace,(int)(((float)p->m_iHeight)/1.2),p->m_iWeight,p->fdwItalic);
		m_hDC->setFont(font);
		/*
		static HFONT hFont = NULL;
		if(hFont != NULL)
			::DeleteObject(hFont);
		hFont = CreateFont(	p->m_iHeight,
												p->m_iWidth,
												p->m_iEscapement,
												p->m_iOrientation,
												p->m_iWeight,
												p->fdwItalic,         // italic attribute flag
												p->fdwUnderline,      // underline attribute flag
												p->fdwStrikeOut,      // strikeout attribute flag
												p->fdwCharSet,        // character set identifier
												p->fdwOutputPrecision,  // output precision
												p->fdwClipPrecision,  // clipping precision
												p->fdwQuality,        // output quality
												p->fdwPitchAndFamily,  // pitch and family
												p->szFace);
		::SelectObject(m_hDC,hFont);
		*/
		return p;
	}
	virtual SBrush* SelectObject(SBrush *pb)
	{
		QBrush b(QColor(pb->m_Color.r, pb->m_Color.g, pb->m_Color.b),pb->m_iStyle != -1?Qt::NoBrush:(Qt::SolidPattern));
		m_hDC->setBrush(b);
		/*
		static HBRUSH hBrush = NULL;
		if(hBrush != NULL)
			::DeleteObject(hBrush);
		if(pb->m_iStyle == -1)
			hBrush = ::CreateSolidBrush(RGB(pb->m_Color.r, pb->m_Color.g, pb->m_Color.b));
		else
			hBrush = (HBRUSH)::GetStockObject(pb->m_iStyle);
		::SelectObject(m_hDC,hBrush);
		*/
		return pb;
	}

	virtual void SetTextColor(SCOLOR color)
	{
		m_hDC->setPen(QColor(color.r, color.g, color.b));
		/*
		::SetBkMode(m_hDC,TRANSPARENT);
		::SetTextColor (m_hDC,RGB(color.r, color.g, color.b));
		*/
	}
	virtual void TextOut(int x,int y,const char * sText)
	{
		//SSize s = GetTextSize((char*)sText);
		m_hDC->drawText(x,y,QString(sText));
		//::TextOut(m_hDC,x,y,sText,strlen(sText));
	}
	
	virtual void MoveTo(int x,int y)
	{
		m_x = x;
		m_y = y;
		//::MoveToEx(m_hDC,x,y,NULL);
	}
	virtual void LineTo(int x,int y)
	{
		m_hDC->drawLine(m_x,m_y,x,y);
		m_x = x;
		m_y = y;
		//::LineTo(m_hDC,x,y);
	}
	virtual void Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		m_hDC->drawArc(x1,y1,x4-x1,y4-y1,0,0);
		//::Arc(m_hDC,x1,y1,x2,y2,x3,y3,x4,y4);
	}
	virtual void ArcTo( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		m_hDC->drawArc(x1,y1,x4-x1,y4-y1,0,0);
		//::ArcTo(m_hDC,x1,y1,x2,y2,x3,y3,x4,y4);
	}
	virtual void AngleArc( int x, int y, int nRadius, float fStartAngle, float fSweepAngle )
	{
		//m_hDC->drawArc(x,y,x4-x1,y4-y1,fStartAngle,fSweepAngle);
		//::AngleArc(m_hDC,x,y,nRadius,fStartAngle,fSweepAngle);
	}
	virtual void FillRect(int x,int y,int w,int h,SCOLOR color)
	{
		m_hDC->fillRect(x,y,w,h,QColor(color.r,color.g,color.b));
		/*
		HBRUSH hbr;
		hbr = CreateSolidBrush(RGB(color.r,color.g,color.b));
		RECT rect;
		rect.left = x;
		rect.right = x+w;
		rect.top   = y;
		rect.bottom= y+h;
		::FillRect(m_hDC, (LPRECT)&rect, hbr); 
		DeleteObject(hbr);
		*/
	}
	virtual void FillRect(LPYSRECT lpRect,SCOLOR color)
	{
		m_hDC->fillRect(lpRect->left,lpRect->top,lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,QColor(color.r,color.g,color.b));
		/*
		FillRect(lpRect->left,lpRect->top,
			lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,color);
			*/
	}
	virtual void Ellipse(int x1,int y1,int x2,int y2)
	{
		m_hDC->drawEllipse(x1,y1,x2-x1,y2-y1);
		//::Ellipse(m_hDC,x1,y1,x2,y2);
	}
	virtual void Polygon(const YSPOINT* pts,int size)
	{
		QPainterPath path;
		QPolygon p(size);
		for(int i=0;i<size;i++)
		{
			p[i].setX(pts[i].x);
			p[i].setY(pts[i].y);
		}
		path.addPolygon(QPolygonF(p));
		m_hDC->drawPath( path);
		//::Polygon(m_hDC,(POINT*)pts,size);
	}
/*
	virtual COLORREF GetTextColor()
	{
		return::GetTextColor(m_hDC);
	}
	virtual BOOL DrawFocusRect(RECT *rt)
	{
        return::DrawFocusRect(m_hDC,rt);
	}*/

private:
	int m_x,m_y;
};

#endif //__SKT_WIN_DC_H__
