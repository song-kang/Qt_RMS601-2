/**
 *
 * 文 件 名 : SBaseDC.h
 * 创建日期 : 2007-12-20 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
 *            $Log: $
 *
 **/

#ifndef __SKT_BASE_DC_INC_H__
#define __SKT_BASE_DC_INC_H__

#include "../SInclude.h"
#include "../SApi.h"

#pragma pack(1)

typedef struct SCOLOR
{
	BYTE a;//alpha
	BYTE r;
	BYTE g;
	BYTE b;
}*LPSCOLOR;

#pragma pack()

typedef class SRect
{
public:
	SRect()
	{
	}
	~SRect()
	{
	}
	void OffsetRect(int dx,int dy)
	{
		left		+= dx;
		right		+= dx;
		top			+= dy;
		bottom	+= dy;
	}
	int left;
	int right;
	int top;
	int bottom;
}*LPYSRECT,YSRECT;

typedef class SSize
{
public:
	int cx;
	int cy;
}*LPYSSIZE,YSSIZE;
typedef class SPoint
{
public:
	int x;
	int y;
	SPoint(){}
	SPoint(int _x,int _y)
	{
		x = _x;
		y = _y;
	}
}*LPYSPOINT,YSPOINT;

class SBrush
{
public:
	SBrush()
	{
		m_iStyle = -1;
	}
	SBrush(SCOLOR color)
	{
		m_Color = color;
		m_iStyle = -1;
	}
	~SBrush()
	{
	}
	void CreateStockObject(int style)
	{
		m_iStyle = style;//NULL_BRUSH表示清除BRUSH
	}
	SCOLOR m_Color;
	int m_iStyle;
};

class SPen
{
public:
	SPen(int style,int width,SCOLOR color)
	{
		m_iStyle = style;
		m_iWidth = width;
		m_Color  = color;
	}
	~SPen()
	{
	}

	int m_iStyle,m_iWidth;
	SCOLOR m_Color;
};

class SFont
{
public:
	int m_iHeight;
	int m_iWidth;
	int m_iEscapement;
	int m_iOrientation;
	int m_iWeight;
  DWORD fdwItalic;         // italic attribute flag
  DWORD fdwUnderline;      // underline attribute flag
  DWORD fdwStrikeOut;      // strikeout attribute flag
  DWORD fdwCharSet;        // character set identifier
  DWORD fdwOutputPrecision;  // output precision
  DWORD fdwClipPrecision;  // clipping precision
  DWORD fdwQuality;        // output quality
  DWORD fdwPitchAndFamily;  // pitch and family
  char szFace[128];         // pointer to typeface name string
	
	//mothed
	SFont()
	{
		memset(this,0,sizeof(SFont));
	}
	virtual ~SFont()
	{
	}

};
SCOLOR SRGB(BYTE r,BYTE g,BYTE b,BYTE a=0);
class SBaseDC
{
public:
	SBaseDC()
	{
		m_pFont  = NULL;
		m_pBrush = NULL;
		m_pPen   = NULL;
	}
	virtual ~SBaseDC()
	{
	}

	virtual SSize GetTextSize(char *pStr)=0;
	virtual SFont* SelectObject(SFont *p)=0;
	virtual SPen* SelectObject(SPen *p)=0;
	virtual SBrush* SelectObject(SBrush *p)=0;
	virtual void SetTextColor(SCOLOR color)=0;
	virtual void TextOut(int x,int y,const char * sText) = 0;
	virtual void MoveTo(int x,int y) = 0;
	virtual void LineTo(int x,int y) = 0;
	virtual void Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )=0;
	virtual void ArcTo( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )=0;
	virtual void AngleArc( int x, int y, int nRadius, float fStartAngle, float fSweepAngle )=0;
	virtual void FillRect(int x,int y,int w,int h,SCOLOR color)=0;
	virtual void FillRect(LPYSRECT lpRect,SCOLOR color)=0;
	virtual void Ellipse(int x1,int y1,int x2,int y2)=0;
	virtual void Polygon(const YSPOINT* pts,int size)=0;
// 	virtual COLORREF GetTextColor()=0;//qcj add
// 	virtual BOOL DrawFocusRect(RECT *rt)=0;//qcj
	virtual unsigned int GetHandle()=0;

	void FillRect(LPYSRECT lpRect,SBrush *pBrush)
	{
		FillRect(lpRect,pBrush->m_Color);
	}
	inline SFont* GetFont()		{return m_pFont;};
	inline SBrush* GetBrush()	{return m_pBrush;};
	inline SPen* GetPen()			{return m_pPen;};
	inline void SetFont(SFont* p)	{m_pFont=p;};
	inline void SetBrush(SBrush* p){m_pBrush=p;};
	inline void SetPen(SPen* p)		{m_pPen=p;};
	
private:
	SFont  *m_pFont;
	SBrush *m_pBrush;
	SPen   *m_pPen;
};
#endif//__SKT_BASE_DC_INC_H__
