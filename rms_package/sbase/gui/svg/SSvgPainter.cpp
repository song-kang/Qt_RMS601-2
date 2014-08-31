/**
 *
 * 文 件 名 : SSvgPainter.h
 * 创建日期 : 2007-04-08 15:46
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG各种对象的画法,封装对SVG各种基本元素的绘制方法
 * 修改记录 : 
 *            $Log: $
 *
 **/

#include "SSvgPainter.h"
#include "SSvgObject.h"
#include "../SPicture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SSvgPainter::SSvgPainter()
{
	m_dScale = 1;
	m_ViewRect.left = 0;
	m_ViewRect.right = 0;
	m_ViewRect.top = 0;
	m_ViewRect.bottom = 0;
	m_bEditAllow = false;
}

SSvgPainter::~SSvgPainter()
{
}

//绘制当前对象及其下的全部对象
void SSvgPainter::DrawObject(SBaseDC *pDC, SSvgObject *pObj, int x, int y,bool bOnlyOne)
{
	SSvgObject *p=pObj;
	while(p)
	{
		if(p->GetParent() == m_Document.GetRootObject())
		{
			//对象级
			if(p->IsFlash())
			{
				//跳过动画图元
				p = p->GetNext();
				continue;
			}
		}
		//当前对象是否在选中列表中，若在则不用在这里画,动态画时不判断
		//不使用这种方式，而改成只要选中框
		if(!bOnlyOne && m_SelectedObjList.exist(p))
		{
			p = p->GetNext();
			continue;
		}
		//if(0)//TODO !IsInViewRect(p))
		if(!IsInViewRect(p))
		{
			DrawObject(pDC,p->GetChild(),x,y);
			if(bOnlyOne)
				break;
			p = p->GetNext();
			continue;
		}
		//画布元素不画
//		if(m_bEditAllow == false && p == m_Document.GetRootObject()->GetChild())
//		{
//			p = p->GetNext();
//			continue;
//		}

		switch(p->GetType())
		{
			case SSvgObject::TYPE_RECT:
				DrawRect(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_TEXT:
				DrawSvgText(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_ELLIPSE:
				DrawEllipse(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_LINE:
				DrawLine(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_POLYLINE:
				DrawPolyLine(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_PATH:
				DrawPath(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_PIC:
				DrawPic(pDC,p,x,y);
				break;
			default:
				break;
		}

		DrawObject(pDC,p->GetChild(),x,y);
		if(bOnlyOne)
			break;
		p = p->GetNext();
	}
}

//绘制当前对象及其下的全部对象
void SSvgPainter::DrawFlashObject(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SSvgObject *p=pObj;
	while(p)
	{
		if(p->GetParent() == m_Document.GetRootObject())
		{
			//对象级
			if(!p->IsFlash())
			{
				//跳过非动画图元
				p = p->GetNext();
				continue;
			}
		}
		if(!IsInViewRect(p))
		{
			DrawFlashObject(pDC,p->GetChild(),x,y);
			p = p->GetNext();
			continue;
		}
		switch(p->GetType())
		{
			case SSvgObject::TYPE_RECT:
				DrawRect(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_TEXT:
				DrawSvgText(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_ELLIPSE:
				DrawEllipse(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_LINE:
				DrawLine(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_POLYLINE:
				DrawPolyLine(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_PATH:
				DrawPath(pDC,p,x,y);
				break;
			case SSvgObject::TYPE_PIC:
				DrawPic(pDC,p,x,y);
				break;
			default:
				break;
		}
		DrawFlashObject(pDC,p->GetChild(),x,y);
		p = p->GetNext();
	}
}

//根据根对象绘制对齐点
void SSvgPainter::DrawAlignPoint(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
//	m_ViewRect.left
	float x0,y0,sep=10,x2,y2;
	x0 = GetCoord(m_ViewRect.left);
	y0 = GetCoord(m_ViewRect.top);
	x2 = GetCoord(m_ViewRect.right);
	y2 = GetCoord(m_ViewRect.bottom);
	int px,py;
	for(px=sep+x;px<x2;px+=sep)
	{
		if(px<x0 || px>x2)
			continue;
		for(py=sep+y;py<y2;py+=sep)
		{
			if(py<y0 || py>y2)
				continue;
			//pDC->SetPixel(px,py,SRGB(255,0,0));
		}
	}
/*
	x0 = pObj->GetRect()->left;
	x2 = pObj->GetRect()->right;
	y2 = pObj->GetRect()->bottom;
	for(;x0<x2;x0+=sep)
	{
		y0 = pObj->GetRect()->top;
		for(;y0<y2;y0+=sep)
		{
			px = GetCoord(x0)+x;
			py = GetCoord(y0)+y;
			pDC->SetPixel(px,py,SRGB(255,0,0));
		}
	}*/

}

void SSvgPainter::DrawObjectSelected(SBaseDC *pDC, SSvgObject *pObj, int x, int y, 
																			 SCOLOR colTag,bool bEditable)
{
	SRect rect,r2;
	rect.left = GetCoord(pObj->GetRect()->left);
	rect.top = GetCoord(pObj->GetRect()->top);
	rect.right = GetCoord(pObj->GetRect()->right);
	rect.bottom = GetCoord(pObj->GetRect()->bottom);
	rect.OffsetRect(x,y);
	SBrush b(colTag);
	SPen p(PS_DOT,1,SRGB(0,0,0));
	SPen* pOld = pDC->SelectObject(&p);
	switch(pObj->GetType())
	{
		case SSvgObject::TYPE_GROUP:
		case SSvgObject::TYPE_RECT:
		case SSvgObject::TYPE_TEXT:
		case SSvgObject::TYPE_ELLIPSE:
		case SSvgObject::TYPE_PATH:
			if(bEditable== false || pObj->GetType() != SSvgObject::TYPE_RECT &&
				 pObj->GetType() != SSvgObject::TYPE_GROUP &&
				 pObj->GetType() != SSvgObject::TYPE_PIC )
			{
				pDC->MoveTo(rect.left,rect.top);
				pDC->LineTo(rect.right,rect.top);
				pDC->LineTo(rect.right,rect.bottom);
				pDC->LineTo(rect.left,rect.bottom);
				pDC->LineTo(rect.left,rect.top);
			}
			if(bEditable)
			{
				//左上角
				r2.left = rect.left - C_TAG_RECT_RADIUS;
				r2.top = rect.top - C_TAG_RECT_RADIUS;
				r2.right = rect.left + C_TAG_RECT_RADIUS;
				r2.bottom = rect.top + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				//左下角
				r2.left = rect.left - C_TAG_RECT_RADIUS;
				r2.top = rect.bottom - C_TAG_RECT_RADIUS;
				r2.right = rect.left + C_TAG_RECT_RADIUS;
				r2.bottom = rect.bottom + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				//右上角
				r2.left = rect.right - C_TAG_RECT_RADIUS;
				r2.top = rect.top - C_TAG_RECT_RADIUS;
				r2.right = rect.right + C_TAG_RECT_RADIUS;
				r2.bottom = rect.top + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				//右下角
				r2.left = rect.right - C_TAG_RECT_RADIUS;
				r2.top = rect.bottom - C_TAG_RECT_RADIUS;
				r2.right = rect.right + C_TAG_RECT_RADIUS;
				r2.bottom = rect.bottom + C_TAG_RECT_RADIUS;
				pDC->FillRect(&r2,&b);
				if(pObj->GetType() != SSvgObject::TYPE_TEXT)//文字不需要中操作块
				{
					//左中
					r2.left = rect.left - C_TAG_RECT_RADIUS;
					r2.top = rect.top+(rect.bottom-rect.top)/2 - C_TAG_RECT_RADIUS;
					r2.right = rect.left + C_TAG_RECT_RADIUS;
					r2.bottom = rect.top+(rect.bottom-rect.top)/2 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//右中
					r2.left = rect.right - C_TAG_RECT_RADIUS;
					r2.top = rect.top+(rect.bottom-rect.top)/2 - C_TAG_RECT_RADIUS;
					r2.right = rect.right + C_TAG_RECT_RADIUS;
					r2.bottom = rect.top+(rect.bottom-rect.top)/2 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//上中
					r2.left = rect.left+(rect.right-rect.left)/2 - C_TAG_RECT_RADIUS;
					r2.top = rect.top - C_TAG_RECT_RADIUS;
					r2.right = rect.left+(rect.right-rect.left)/2 + C_TAG_RECT_RADIUS;
					r2.bottom = rect.top + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//下中
					r2.left = rect.left+(rect.right-rect.left)/2 - C_TAG_RECT_RADIUS;
					r2.top = rect.bottom - C_TAG_RECT_RADIUS;
					r2.right = rect.left+(rect.right-rect.left)/2 + C_TAG_RECT_RADIUS;
					r2.bottom = rect.bottom + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
				}
			}
			break;
		case SSvgObject::TYPE_LINE:
			{
				float x1=GetCoord(pObj->GetAttributeF("x1"))+x;
				float y1=GetCoord(pObj->GetAttributeF("y1"))+y;
				float x2=GetCoord(pObj->GetAttributeF("x2"))+x;
				float y2=GetCoord(pObj->GetAttributeF("y2"))+y;
//				pDC->MoveTo(x1,y1);
//				pDC->LineTo(x2,y2);
				if(bEditable)
				{
					//P1
					r2.left = x1+ - C_TAG_RECT_RADIUS;
					r2.top = y1 - C_TAG_RECT_RADIUS;
					r2.right = x1+ C_TAG_RECT_RADIUS;
					r2.bottom = y1 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
					//P2
					r2.left = x2+ - C_TAG_RECT_RADIUS;
					r2.top = y2 - C_TAG_RECT_RADIUS;
					r2.right = x2+ C_TAG_RECT_RADIUS;
					r2.bottom = y2 + C_TAG_RECT_RADIUS;
					pDC->FillRect(&r2,&b);
				}
			}
			break;
		case SSvgObject::TYPE_POLYLINE:
			{
				SString points=pObj->GetAttribute("points");
				int p1=0,p2,p3;
				int x0,y0;
				int index=0;
				while(1)
				{
					p2=points.Find(",",p1);
					if(p2 < 0)
						break;
					x0 = GetCoord(SString::toInt(points.Mid(p1,p2-p1)))+x;
					p2 ++;
					p3=points.Find(" ",p2);
					if(p3 < 0)
						p3 = points.GetLength();
					y0 = GetCoord(SString::toInt(points.Mid(p2,p3-p2)))+y;
					p1 = p3+1;
//					if(index==0)
//						pDC->MoveTo(x0, y0);
//					else
//						pDC->LineTo(x0, y0);
					if(bEditable)
					{
						r2.left = x0+ - C_TAG_RECT_RADIUS;
						r2.top = y0 - C_TAG_RECT_RADIUS;
						r2.right = x0+ C_TAG_RECT_RADIUS;
						r2.bottom = y0 + C_TAG_RECT_RADIUS;
						pDC->FillRect(&r2,&b);
					}
					index++;
				}

			}
			break;
		default:
			break;
	}
	pDC->SelectObject(pOld);
}

//转换SVG坐标到当前窗口
int SSvgPainter::GetCoord(float x)
{
	return (int)((double)x*m_dScale);
}

//转换当前窗口到SVG坐标
float SSvgPainter::GetSvgCoord(float x)
{
	return (float)((double)x/m_dScale);
}

int GetHexByStr(char *str)
{
	int b=0;
	char ch;
	ch=str[0];
	if(ch>='A' && ch <='F')
		ch = 10 + (ch -'A');
	else if(ch >='a' && ch <='f')
		ch = 10 + (ch - 'a');
	else
		ch = ch - '0';
	b=ch*16;
	ch=str[1];
	if(ch>='A' && ch <='F')
		ch = 10 + (ch -'A');
	else if(ch >='a' && ch <='f')
		ch = 10 + (ch - 'a');
	else
		ch = ch - '0';
	b += ch;
	return b;
}

//转换颜色
SCOLOR SSvgPainter::GetColorByString(SString sColor)
{
	BYTE r,g,b;
	if(sColor.Left(1) == "#" && sColor.GetLength()==7)
	{
		char *pStr=(char*)sColor.GetBuffer(0);
		r = GetHexByStr(pStr+1);
		g = GetHexByStr(pStr+3);
		b = GetHexByStr(pStr+5);
	}
	else if(sColor.Left(3).CompareNoCase("rgb") == 0)
	{
		sColor = sColor.Mid(4);
		sColor = sColor.Left(sColor.GetLength()-1);
		r = SString::GetIdAttributeI(1,sColor,",");
		g = SString::GetIdAttributeI(2,sColor,",");
		b = SString::GetIdAttributeI(3,sColor,",");
	}
	else if(sColor == "black")
		return SRGB(0,0,0);
	else if(sColor == "white")
		return SRGB(255,255,255);
	else if(sColor == "red")
		return SRGB(255,0,0);
	else if(sColor == "green")
		return SRGB(0,255,0);
	else if(sColor == "blue")
		return SRGB(0,0,255);
	return SRGB(r,g,b);
}

SString SSvgPainter::GetStringByColor(SCOLOR color)
{
	SString val;
	val =	"#"+SString::toString((int)color.r,16)+
				SString::toString((int)color.g,16)+
				SString::toString((int)color.b,16);

	return val;
}

SString SSvgPainter::GetRGBStringByColor(SCOLOR color)
{
	SString val;
	val =	SString::toString((int)color.r)+","+
				SString::toString((int)color.g)+"," +
				SString::toString((int)color.b);

	return val;
}

/************************************************************************/
/* rect
 	x
	y
	z
	width
	height
	stroke		边框颜色
	stroke-width 边框宽度
	fill		填充颜色，none表示不填充,写法如#55aa00，也可以写成rgb(255,0,0)，
					但这种方式南思不认识
*/
/************************************************************************/
void SSvgPainter::DrawRect(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SRect rect;
	rect.left = GetCoord(pObj->GetAttributeF("x"));
	rect.top = GetCoord(pObj->GetAttributeF("y"));
	rect.right = rect.left + GetCoord(pObj->GetAttributeF("width"));
	rect.bottom = rect.top + GetCoord(pObj->GetAttributeF("height"));
	rect.OffsetRect(x,y);
	SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
	if(sFill != "" && sFill.CompareNoCase("none") != 0)
	{
		//填充区域
		SBrush b(GetColorByString(pObj->GetAttribute("fill")));
		pDC->FillRect(&rect,&b);
	}

	SPen p(pObj->GetAttributeI("stroke-dasharray"),pObj->GetAttributeI("stroke-width"),
		GetColorByString(GetAttributeByLevel(pObj,"stroke")));//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.right,rect.top);
	pDC->LineTo(rect.right,rect.bottom);
	pDC->LineTo(rect.left,rect.bottom);
	pDC->LineTo(rect.left,rect.top);
	pDC->SelectObject(pOld);
}

/************************************************************************/
/* text
	x
	y			表示文字底部的位置
	z			
	font-family	字体名
	font-size	字体大小
	font-weight	字重
	fill		颜色
*/
/************************************************************************/
#ifndef DEFAULT_CHARSET
#define DEFAULT_CHARSET 1
#endif

void SSvgPainter::DrawSvgText(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SString weight = pObj->GetAttribute("font-weight");
	SString family = pObj->GetAttribute("font-family");
	SFont font;
	memset(&font,0,sizeof(font));
	font.m_iHeight = GetCoord(pObj->GetAttributeF("font-size"));
	font.m_iWidth = 0;
#ifdef QT_GUI_LIB
	if(weight=="bold")
		font.m_iWeight = 75;
	else
		font.m_iWeight = 50;
#else
	if(weight=="bold")
		font.m_iWeight = 700;
	else
		font.m_iWeight = 500;
#endif
	font.fdwCharSet  = DEFAULT_CHARSET;
	strcpy(font.szFace,family.GetBuffer());
	pDC->SelectObject(&font);
	SSize textsize=GetTextSize(pDC,pObj);
	SSvgObject::SVG_RECT *pr = pObj->GetRect();
	pr->right = pr->left+textsize.cx;
	pr->bottom = pr->top+textsize.cy;
	int x0 = GetCoord(pObj->GetAttributeF("x"));
#ifdef QT_GUI_LIB
	int y0 = GetCoord((pObj->GetAttributeF("y")));
#else
	int y0 = GetCoord((pObj->GetAttributeF("y")-textsize.cy));
#endif
	int size = GetCoord(pObj->GetAttributeF("font-size"));
	pDC->SetTextColor(GetColorByString(GetAttributeByLevel(pObj,"fill")));//pObj->GetAttribute("fill")
	pDC->TextOut(x0+x,y0+y,pObj->GetAttribute("text").data());

	/*
	SSize textsize=GetTextSize(pDC,pObj);
	int x0 = GetCoord(pObj->GetAttributeF("x"));
	int y0 = GetCoord((pObj->GetAttributeF("y")-textsize.cy));
	int size = GetCoord(pObj->GetAttributeF("font-size"));
	SString weight = pObj->GetAttribute("font-weight");
	SString family = pObj->GetAttribute("font-family");
	CFont *pOldFont;
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	logfont.lfHeight=size;
	memcpy(logfont.lfFaceName ,family.GetBuffer(0),family.GetLength()); 
	logfont.lfWidth=0;
	if(weight=="bold")
		logfont.lfWeight=700;
	else
		logfont.lfWeight=500;
	logfont.lfCharSet=DEFAULT_CHARSET;
	CFont font;
	font.CreateFontIndirect(&logfont);
	pOldFont=pDC->SelectObject(&font);

	int oldMode = pDC->SetBkMode(TRANSPARENT);
	SCOLOR col=pDC->GetTextColor();
	pDC->SetTextColor(GetColorByString(GetAttributeByLevel(pObj,"fill")));//pObj->GetAttribute("fill")
	pDC->TextOut(x0+x,y0+y,pObj->GetAttribute("text"));

	pDC->SetTextColor(col);
	pDC->SetBkMode(oldMode);
	pDC->SelectObject(pOldFont);

	font.DeleteObject();
	*/
}

SBaseWnd *m_pBaseWnd = NULL;
void SSvgPainter::SetBaseWnd(SBaseWnd *p)
{
	m_pBaseWnd = p;
};

//取一个文本的大小
YSSIZE SSvgPainter::GetTextSize(SBaseDC *pDC, SSvgObject *pObj)
{
	bool bNew = false;
	if(pDC == NULL)
	{
		pDC = m_pBaseWnd->CreateDC();
		bNew = true;
	}
	YSSIZE tsize = pDC->GetTextSize(pObj->GetAttribute("text").data());
	if(bNew)
		m_pBaseWnd->ReleaseDC(pDC);

	return tsize;
	int size = pObj->GetAttributeI("font-size");
	tsize.cx = size * pObj->GetAttribute("text").length();
	tsize.cy = size;
	return tsize;
	/*TODO
	int size = pObj->GetAttributeI("font-size");
	SString weight = pObj->GetAttribute("font-weight");
	SString family = pObj->GetAttribute("font-family");

	CFont *pOldFont;
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	logfont.lfHeight=(int)((double)size);
	
	memcpy(logfont.lfFaceName ,family.GetBuffer(0),family.GetLength()); 
	logfont.lfWidth=0;
	if(weight=="bold")
		logfont.lfWeight=700;
	else
		logfont.lfWeight=500;
	logfont.lfCharSet=DEFAULT_CHARSET;
	CFont font;
	font.CreateFontIndirect(&logfont);
	pOldFont=pDC->SelectObject(&font);
	
	YSSIZE tsize = pDC->GetTextExtent(pObj->GetAttribute("text"));

	pDC->SelectObject(pOldFont);
	return tsize;
	*/
}

/************************************************************************/
/* ellipse
	z
	cx			X方向中心点
	cy			Y方向中心点
	rx			X方向半径
	ry			Y方向半径
	stroke		边框颜色
	stroke-width 边框宽度
	stroke-dasharray 线条样式(可选)
	fill		填充颜色，none表示不填充

	公式：xx + yy = a*b (ab为rx,ry)
*/
/************************************************************************/
void SSvgPainter::DrawEllipse(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	int x1,y1,x2,y2;
	x1 = GetCoord(pObj->GetAttributeF("cx")-pObj->GetAttributeF("rx"));
	y1 = GetCoord(pObj->GetAttributeF("cy")-pObj->GetAttributeF("ry"));
	x2 = GetCoord(pObj->GetAttributeF("cx")+pObj->GetAttributeF("rx"));
	y2 = GetCoord(pObj->GetAttributeF("cy")+pObj->GetAttributeF("ry"));
	SPen p(pObj->GetAttributeI("stroke-dasharray"),pObj->GetAttributeI("stroke-width"),
		GetColorByString(GetAttributeByLevel(pObj,"stroke")));//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	//填充区域
	x1+=x;
	y1+=y;
	x2+=x;
	y2+=y;
	SString fill=GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
	if(fill != "" && fill!="none")
	{
		SBrush b(GetColorByString(fill));
		SBrush *pOldBrush=pDC->SelectObject(&b);
		pDC->Ellipse(x1,y1,x2,y2);
		pDC->SelectObject(pOldBrush);
	}
	else
	{
		SBrush b;
		b.CreateStockObject(NULL_BRUSH);//清除画刷
		SBrush *pOldBrush=pDC->SelectObject(&b);
		pDC->Ellipse(x1,y1,x2,y2);
		pDC->SelectObject(pOldBrush);
	}

		pDC->SelectObject(pOld);
}

/************************************************************************/
/* line
	x1
	y1
	x2
	y2
	z
	stroke-width	线粗
	stroke			线条颜色
	stroke-dasharray 线条样式(可选)
*/
/************************************************************************/
void SSvgPainter::DrawLine(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	float x1=GetCoord(pObj->GetAttributeF("x1"));
	float y1=GetCoord(pObj->GetAttributeF("y1"));
	float x2=GetCoord(pObj->GetAttributeF("x2"));
	float y2=GetCoord(pObj->GetAttributeF("y2"));
	SPen p(pObj->GetAttributeI("stroke-dasharray"),pObj->GetAttributeI("stroke-width"),
		GetColorByString(GetAttributeByLevel(pObj,"stroke")));//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	pDC->MoveTo(x1+x,y1+y);
	pDC->LineTo(x2+x,y2+y);
	pDC->SelectObject(pOld);
}

/************************************************************************/
/* polyline
	points			多边形点轨迹"270,208 270,193"
	stroke			颜色
	stroke-width	线宽
*/
/************************************************************************/
void SSvgPainter::DrawPolyLine(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SString points=pObj->GetAttribute("points");
	SPen p(pObj->GetAttributeI("stroke-dasharray"),pObj->GetAttributeI("stroke-width"),
		GetColorByString(GetAttributeByLevel(pObj,"stroke")));//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	int p1=0,p2,p3;
	int x0,y0;
	int index=0;
	SArray<YSPOINT> polygon;
	YSPOINT pt;	
	while(1)
	{
		p2=points.Find(",",p1);
		if(p2 < 0)
			break;
		x0 = GetCoord(SString::toInt(points.Mid(p1,p2-p1)));
		p2 ++;
		p3=points.Find(" ",p2);
		if(p3 < 0)
			p3 = points.GetLength();
		y0 = GetCoord(SString::toInt(points.Mid(p2,p3-p2)));
		p1 = p3+1;
		if(index==0)
			pDC->MoveTo(x0+x, y0+y);
		else
			pDC->LineTo(x0+x, y0+y);
		pt.x = x0+x;
		pt.y = y0+y;
		polygon.append(pt);

		index++;
	}
	//是否填充
	SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
	if(sFill != "" && sFill.CompareNoCase("none") != 0)
	{
		if(polygon.count()>=3)
		{
			//填充区域
			SBrush b(GetColorByString(pObj->GetAttribute("fill")));
			SBrush *pOldB=pDC->SelectObject(&b);
			memcpy(&pt, polygon.getAt(0),sizeof(pt));
			pDC->Polygon(polygon.getData(),polygon.count());
			pDC->SelectObject(pOldB);
		}
	}
	pDC->SelectObject(pOld);
}
/************************************************************************/
/* 
mode="Arc"
d="M79,443.5 A11,11 0 1 0 90,432.5"
*/
/************************************************************************/
void SSvgPainter::DrawPath(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	/*TODO
	//由于根据两点计算外接矩形太复杂，所以临时采用南思的方式，增加外接矩形属性x/y/width/height
	int x1,y1,x2,y2;
	x1 = GetCoord(pObj->GetAttributeF("x"))+x;
	y1 = GetCoord(pObj->GetAttributeF("y"))+y;
	x2 = GetCoord(pObj->GetAttributeF("x")+pObj->GetAttributeF("width"))+x;
	y2 = GetCoord(pObj->GetAttributeF("y")+pObj->GetAttributeF("height"))+y;
	SString points=pObj->GetAttribute("d");
	SPen p(pObj->GetAttributeI("stroke-dasharray"),pObj->GetAttributeI("stroke-width"),
		GetColorByString(GetAttributeByLevel(pObj,"stroke")));//pObj->GetAttribute("stroke")
	SPen* pOld = pDC->SelectObject(&p);
	int p1=0,p2;
	float x0,y0,x00,y00;
	char oper,ch;
	SString str;
	CArray<POINT,POINT> polygon;
	POINT pt;	
	while(p1<points.GetLength())
	{
		oper = points.GetAt(p1++);//操作类型，目前支持M:MoveTo, L:LineTo, A:Arc
		switch(oper)
		{
		case 'M':
		case 'm':
		case 'L':
		case 'l':
			{
nextLine:
				p2 = points.Find(",",p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				x0 = SString::toDouble(str);
				p1 = p2+1;
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					p2 = points.GetLength();
				str = points.Mid(p1,p2-p1);
				y0 = SString::toDouble(str);
				p1 = p2+1;
				if(oper == 'M' || oper == 'm')
				{
					//if(y0<0)
					//	y0 = -y0;
					pDC->MoveTo(GetCoord(x0)+x, GetCoord(y0)+y);
					x00 = x0;
					y00 = y0;
					
					pt.x = GetCoord(x0)+x;
					pt.y = GetCoord(y0)+y;
					polygon.Add(pt);
				}
				else
				{
					pDC->LineTo(GetCoord(x00+x0)+x, GetCoord(y00+y0)+y);
					x00 += x0;
					y00 += y0;
					pt.x = GetCoord(x00+x0)+x;
					pt.y = GetCoord(y00+y0)+y;
					polygon.Add(pt);
				}
				if(p1 < points.GetLength())
				{
					ch = points.GetAt(p1);
					if(ch >='0' && ch <= '9' || ch == '-')
						goto nextLine;//重复
				}
			}
			break;
		case 'A':
		case 'a':
			{
				int rx,ry;
				float endx,endy;
				int xrotation;
				int large_arc_flag;
				int sweep_flag;
nextArc:
				//A11,11 0 1 0 90,432.5
				//rx
				p2 = points.Find(",",p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				rx = SString::toInt(str);
				p1 = p2+1;

				//ry
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				ry = SString::toInt(str);
				p1 = p2+1;
				
				//xrotation
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				xrotation = SString::toInt(str);
				p1 = p2+1;

				//large-arc-flag
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				large_arc_flag = SString::toInt(str);
				p1 = p2+1;

				//sweep-flag
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				sweep_flag = SString::toInt(str);
				p1 = p2+1;

				//x
				p2 = points.Find(',',p1);
				if(p2 < 0)
					break;
				str = points.Mid(p1,p2-p1);
				endx = SString::toDouble(str);
				p1 = p2+1;

				//y
				p2 = points.Find(' ',p1);
				if(p2 < 0)
					p2 = points.GetLength();
				str = points.Mid(p1,p2-p1);
				endy = SString::toDouble(str);
				p1 = p2+1;
				
				//DarwArc
				pDC->Arc(x1,y1,x2,y2,
								 GetCoord(x0)+x,GetCoord(y0)+y,GetCoord(endx)+x,GetCoord(endy)+y);
				if(p1 < points.GetLength())
				{
					ch = points.GetAt(p1);
					if(ch >='0' && ch <= '9' || ch == '-')
						goto nextArc;//重复
				}
			}
			break;
		}
	}

	//是否填充
	SString sFill = GetAttributeByLevel(pObj,"fill");//pObj->GetAttribute("fill");
	if(sFill != "" && sFill.CompareNoCase("none") != 0)
	{
		if(polygon.GetSize()>=3)
		{
			//填充区域
			SBrush b(GetColorByString(pObj->GetAttribute("fill")));
			SBrush *pOldB=pDC->SelectObject(&b);
			memcpy(&pt, &polygon.GetAt(0),sizeof(pt));
			pDC->Polygon(polygon.GetData(),polygon.GetSize());
			pDC->SelectObject(pOldB);
		}
	}
	pDC->SelectObject(pOld);
*/
}

void SSvgPainter::DrawPic(SBaseDC *pDC, SSvgObject *pObj, int x, int y)
{
	SRect rect;
	rect.left = GetCoord(pObj->GetAttributeF("x"));
	rect.top = GetCoord(pObj->GetAttributeF("y"));
	rect.right = rect.left + GetCoord(pObj->GetAttributeF("width"));
	rect.bottom = rect.top + GetCoord(pObj->GetAttributeF("height"));
	rect.OffsetRect(x,y);
	SPicture *pPic = (SPicture*)pObj->GetAttributeI("picobj");
	if(pPic == NULL)
	{
		pPic = new SPicture();
		SString sPicFile = this->m_Document.GetSvgPath() + pObj->GetAttribute("href");
		if(!pPic->Load(sPicFile.data()))
		{
			LOGWARN("加载图片[%s]失败!",sPicFile.data());
			delete pPic;
			return;
		}
		pObj->SetAttributeValue("picobj",SString::toString((int)pPic));
	}
	if(pPic != NULL)
		pPic->Render(pDC->GetHandle(),rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
}

//测试一个对象是否可显示
bool SSvgPainter::IsInViewRect(SSvgObject *pObj)
{
	SSvgObject::SVG_RECT rect;
	memcpy(&rect,pObj->GetRect(),sizeof(rect));
	if(rect.left==0 && rect.right == 0 && rect.top==0 && rect.bottom==0)
		return true;
	if(rect.left < m_ViewRect.left && rect.right < m_ViewRect.left ||
		 rect.left > m_ViewRect.right && rect.right > m_ViewRect.right ||
		 rect.top < m_ViewRect.top && rect.bottom < m_ViewRect.top ||
		 rect.top > m_ViewRect.bottom && rect.bottom > m_ViewRect.bottom)
		return false;

	return true;
}
//测试当前点是否经过当前对象及其下的兄弟节点，x,y为SVG坐标系
SSvgObject* SSvgPainter::IsPointInObjectRect(SSvgObject *pObj, float x,float y)
{
	if(pObj == NULL)
		return NULL;
	SSvgObject::SVG_RECT *pRect = pObj->GetRect();
	if(x>=pRect->left-3 && x <=pRect->right+3 && 
		 y>=pRect->top-3 && y <= pRect->bottom+3)
	{
		return pObj;
	}

	return IsPointInObjectRect(pObj->GetNext(),x,y);
}

//测试当前点是否经过当前对象及其下的兄弟节点，x,y为SVG坐标系
SSvgObject* SSvgPainter::IsObjectInPoint(SSvgObject *pObj, float x,float y)
{
	if(pObj == NULL)
		return NULL;
	SSvgObject::SVG_RECT *pRect = pObj->GetRect();
	if(x>=pRect->left-3 && x <=pRect->right+3 && 
		 y>=pRect->top-3 && y <= pRect->bottom+3)
	{
		//确定在其范围内，再根据节点类型分不同情况检查是否被抓取到
		switch(pObj->GetType())
		{
			case SSvgObject::TYPE_GROUP:
				{
					//组的选择根据组内是否选中，如有一个选中则组被选中
					SSvgObject *pChild=pObj->GetChild();
					while(pChild)
					{
						if(IsObjectInPoint(pChild,x,y) != NULL)
						{
							//先看看后面还有没有满足的，有则优先后面的
							SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
							if(pNext != NULL)
								return pNext;
							return pObj;
						}
						pChild = pChild->GetNext();
					}
				}
				break;
			case SSvgObject::TYPE_RECT:
				{
					//当前区域是否有填充颜色，有则可以根据填充区域抓取，否则要根据边线抓取
					SString sFill = pObj->GetAttribute("fill");
					if(sFill != "" && sFill.CompareNoCase("none") != 0)
					{
						//填充区域
						//先看看后面还有没有满足的，有则优先后面的
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
						if(pNext != NULL)
							return pNext;
						return pObj;
					}
					//不填充区域
					if(IsNear(x,pRect->left) ||
						 IsNear(x,pRect->right) ||
						 IsNear(y,pRect->top) ||
						 IsNear(y,pRect->bottom))
					{
						//先看看后面还有没有满足的，有则优先后面的
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
						if(pNext != NULL)
							return pNext;
						return pObj;
					}
				}
				break;
			case SSvgObject::TYPE_PIC:
				{
					//先看看后面还有没有满足的，有则优先后面的
					SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
					if(pNext != NULL)
						return pNext;
					return pObj;
				}
				break;
			case SSvgObject::TYPE_TEXT:
				{
					//先看看后面还有没有满足的，有则优先后面的
					SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
					if(pNext != NULL)
						return pNext;
					return pObj;
				}
			case SSvgObject::TYPE_ELLIPSE:
				{
					SString sFill = pObj->GetAttribute("fill");
					if(sFill != "" && sFill.CompareNoCase("none") != 0)
					{
						//填充区域
						//先看看后面还有没有满足的，有则优先后面的
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
						if(pNext != NULL)
							return pNext;
						return pObj;
					}
					//椭圆区域选取
					float cx=pObj->GetAttributeF("cx");
					float cy=pObj->GetAttributeF("cy");
					float rx=pObj->GetAttributeF("rx");
					float ry=pObj->GetAttributeF("ry");
					//cx -= x;
					//cy -= y;
					cx = x - cx;
					cy = y - cy;
					//if(IsNear(sqrt(cx*cx+cy*cy) , sqrt(rx*ry)))
					if(fabs(cx*cx/(rx*rx)+cy*cy/(ry*ry) - 1) < 0.1)
					{
						//先看看后面还有没有满足的，有则优先后面的
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
						if(pNext != NULL)
							return pNext;
						return pObj;
					}
				}
				break;
			case SSvgObject::TYPE_PATH:
				{
					//先看看后面还有没有满足的，有则优先后面的
					SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
					if(pNext != NULL)
						return pNext;
					return pObj;
					
					SString sFill = pObj->GetAttribute("fill");
					if(sFill != "" && sFill.CompareNoCase("none") != 0)
					{
						//填充区域
						//先看看后面还有没有满足的，有则优先后面的
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
						if(pNext != NULL)
							return pNext;
						return pObj;
					}
					//椭圆区域选取
					float cx=pObj->GetAttributeF("x")+pObj->GetAttributeF("width")/2;
					float cy=pObj->GetAttributeF("y")+pObj->GetAttributeF("height")/2;
					float rx=pObj->GetAttributeF("width")/2;
					float ry=pObj->GetAttributeF("height")/2;
					//cx -= x;
					//cy -= y;
					cx = x - cx;
					cy = y - cy;
					//if(IsNear(sqrt(cx*cx+cy*cy) , sqrt(rx*ry)))
					if(fabs(cx*cx/(rx*rx)+cy*cy/(ry*ry) - 1) < 0.1)
					{
						//先看看后面还有没有满足的，有则优先后面的
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
						if(pNext != NULL)
							return pNext;
						return pObj;
					}
				}
				break;
			case SSvgObject::TYPE_LINE:
				{
					float x1=pObj->GetAttributeF("x1");
					float y1=pObj->GetAttributeF("y1");
					float x2=pObj->GetAttributeF("x2");
					float y2=pObj->GetAttributeF("y2");
					if(IsInLine(x1,y1,x2,y2,x,y))
					{
						//先看看后面还有没有满足的，有则优先后面的
						SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
						if(pNext != NULL)
							return pNext;
						return pObj;
					}
				}
				break;
			case SSvgObject::TYPE_POLYLINE:
				{
					SString points=pObj->GetAttribute("points");
					int p1=0,p2,p3;
					float x0,y0,x1,y1;
					int index=0;
					while(1)
					{
						p2=points.Find(",",p1);
						if(p2 < 0)
							break;
						x1 = SString::toDouble(points.Mid(p1,p2-p1));
						p2 ++;
						p3=points.Find(" ",p2);
						if(p3 < 0)
							p3 = points.GetLength();
						y1 = SString::toDouble(points.Mid(p2,p3-p2));
						p1 = p3+1;
						if(index>0)
						{
							if(IsInLine(x0,y0,x1,y1,x,y))
							{
								//先看看后面还有没有满足的，有则优先后面的
								SSvgObject *pNext = IsObjectInPoint(pObj->GetNext(),x,y);
								if(pNext != NULL)
									return pNext;
								return pObj;
							}
						}
						x0=x1;
						y0=y1;
						index++;
					}
				}
				break;
			default:
				break;
		}

	}

	return IsObjectInPoint(pObj->GetNext(),x,y);
}

//测试当前区域是否包含经过当前对象及其下的兄弟节点，x,y为SVG坐标系
void SSvgPainter::SelectObjectInRect(SSvgObject *pObj, SSvgObject::SVG_RECT &selRect)
{
	if(pObj == NULL)
		return ;
	SSvgObject::SVG_RECT *pRect = pObj->GetRect();
	if(pRect->left >= selRect.left && pRect->left <= selRect.right  &&
		 pRect->right >= selRect.left && pRect->right <= selRect.right &&
		 pRect->top  >= selRect.top  && pRect->top <=selRect.bottom   &&
		 pRect->bottom  >= selRect.top  && pRect->bottom<=selRect.bottom)
	{
		//在其中，加入到选中列表
		if(!m_SelectedObjList.exist(pObj))
			m_SelectedObjList.append(pObj);
	}

	SelectObjectInRect(pObj->GetNext(),selRect);
}

//用于判断点是否靠近
bool SSvgPainter::IsNear(float v1, float v2)
{
	if(fabs(v1 - v2) < 3)
		return true;
	return false;
}

//用于判断一个点是否在指定的线段上
bool SSvgPainter::IsInLine(float x1, float y1, float x2, float y2, float px, float py)
{
	//计算p到两个顶点的距离之和是否等于两顶点距离
	float s1=sqrt((x1-px)*(x1-px)+(y1-py)*(y1-py));
	float s2=sqrt((x2-px)*(x2-px)+(y2-py)*(y2-py));
	float s=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	if(fabs((s1+s2)-s)<0.05)
		return true;
	return false;
}

//根据一个椭圆及一个光标点，定位光标点对应在椭圆上的点
void SSvgPainter::GetEllipseByPoint(float x,float y, 
						float width,float height,float px, float py,float &ex,float &ey)
{
	float cx=x + width/2;
	float cy=y + height/2;
	float rx=width/2;
	float ry=height/2;
	
	if(px < x)
		ex = x;
	else if(px > x+width)
		ex = x + width;
	else
		ex = px;
	ey = sqrt((1-(ex-cx)*(ex-cx)/(rx*rx))*(ry*ry));
	if(py - cy < 0)
	{
		ey = -ey;
	}
	ey += cy;
}

/************************************************************************/
/* 
	根据级别取对象属性，如果上层有属性则使用上层的属性，否则使用下层的属性
*/
/************************************************************************/
SString SSvgPainter::GetAttributeByLevel(SSvgObject *pObj, SString sAttr)
{
	SPtrList<SSvgObject> objList;
	
	SString str;
	SSvgObject *pParent = pObj;
	while(pParent != m_Document.GetRootObject())
	{
		if(pParent == NULL)
		{
			pParent = pObj;
			break;
		}
		objList.append(pParent);
		pParent = pParent->GetParent();
	}
	int i,cnt=objList.count();
	for(i=cnt-1;i>=0;i--)
	{
		pParent = objList.at(i);
		str = pParent->GetAttribute(sAttr);
		if(str.GetLength()>0)
			return str;
	}
	return pObj->GetAttribute(sAttr);
}

void SSvgPainter::SetScale(double scale)
{
	m_dScale = scale;
}
