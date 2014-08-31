/**
 *
 * 文 件 名 : SSvgWnd.cpp
 * 创建日期 : 2008-02-15 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
 *            $Log: $
 *
 **/
#include "SSvgWnd.h"

SSvgWnd::SSvgWnd()
{
	m_bMovable = false;
	m_iMoving = 0;
	m_iOffsetX = m_iOffsetY = 0;
	setMouseTracking(true);
}

SSvgWnd::~SSvgWnd()
{
}

bool SSvgWnd::Load(SString sSvgFile)
{
	LOGDEBUG("Load svg:%s",sSvgFile.data());
	m_sSvgPath = sSvgFile;
	SetBaseWnd((SBaseWnd *)this);
	// sSvgFile="d:/znl4monitor/svg/null.svg";
	if(!m_Document.LoadSvgFile(sSvgFile))
		return false;
	ToCenter();
	return true;
}

bool SSvgWnd::OnPaint(SBaseDC *pDC)
{
//	pDC->TextOutA(10,10,"My QT SVG!");
	SSvgObject *pFirstObj = m_Document.GetRootObject();
	if(pFirstObj != NULL)
		pFirstObj = pFirstObj->GetChild();
//	if(pFirstObj != NULL)
//		pFirstObj = pFirstObj->GetNext();
	if( pFirstObj != NULL)
	{
		YSRECT r = GetClientRect();

		m_ViewRect.left =GetSvgCoord((double)-m_iOffsetX);
		m_ViewRect.top = GetSvgCoord((double)-m_iOffsetY );
		m_ViewRect.right = m_ViewRect.left + GetSvgCoord((double)(r.right-r.left/*-m_iOffsetX*/));
		m_ViewRect.bottom = m_ViewRect.top + GetSvgCoord((double)(r.bottom-r.top/*-m_iOffsetY*/));

		pDC->FillRect(0,0,r.right-r.left,r.bottom-r.top,
			GetColorByString(pFirstObj->GetAttribute("fill")));
		DrawObject(pDC,pFirstObj,m_iOffsetX,m_iOffsetY);
	}
	return true;
}
int SSvgWnd::OnLButtonDown(int nFlag,SPoint point)
{
//	LOGDEBUG("SSvgWnd::OnLButtonDown");
	//根据当前位置查找被捕获的元素
//	float x = GetSvgCoord(point.x + /*GetScrollPosH()*/0);
//	float y = GetSvgCoord(point.y + /*GetScrollPosV()*/0);
	//float x = GetSvgCoord(point.x - m_iOffsetX);
	//float y = GetSvgCoord(point.y - m_iOffsetY);
	//SSvgObject *pObj = m_Document.GetRootObject();
	//if(pObj)
	//	pObj = pObj->GetChild();
	//if(pObj)
	//	pObj = pObj->GetNext();
	////if(m_bSelectWithRect)
	////	pObj = IsPointInObjectRect(pObj,x,y);
	////else
	//	pObj = IsObjectInPoint(pObj,x,y);
	//if(pObj != NULL)
	//{
	//	LOGDEBUG("SSvgWnd::OnClickObject");
	//	OnClickObject(pObj);
	//}
	//else 
	if(m_bMovable)
	{
		//可移动
		m_ptLast = point;
		m_iMoving = 1;
	}

	return 1;
}

void SSvgWnd::ToCenter()
{
	YSRECT rWnd = GetClientRect();
	SSvgObject *pFirstObj = m_Document.GetRootObject();
	if(pFirstObj == NULL)
		return ;
	int svgw = GetCoord(pFirstObj->GetRect()->right);
	int svgh = GetCoord(pFirstObj->GetRect()->bottom);

	int x,y;
	x = (rWnd.right - svgw)/2;
	y = (rWnd.bottom - svgh)/2;
	SetOffset(x,0);
}

int SSvgWnd::OnMouseMove(int nFlag,SPoint point)
{
	bool bTip=false;
	if(m_iMoving > 0)
	{
		int sx=point.x - m_ptLast.x;
		int sy=point.y - m_ptLast.y;
		m_ptLast = point;
		m_iOffsetX += sx;
		m_iOffsetY += sy;
		//判断是否超出画板范围
		YSRECT rWnd = GetClientRect();
		int wndw = rWnd.right - rWnd.left;
		int wndh = rWnd.bottom - rWnd.top;
		SSvgObject *pFirstObj = m_Document.GetRootObject();
		if(pFirstObj == NULL)
			return 1;
		int svgw = GetCoord(pFirstObj->GetRect()->right);
		int svgh = GetCoord(pFirstObj->GetRect()->bottom);
		if(wndw < svgw)
		{
			//窗口宽度小于SVG宽度时
			if(m_iOffsetX < wndw-svgw)
				m_iOffsetX = wndw-svgw;
			if(m_iOffsetX > 0)
				m_iOffsetX = 0;
		}
		else
		{
			//窗口宽度大于SVG宽度时
			if(m_iOffsetX < 0)
				m_iOffsetX = 0;
			if(m_iOffsetX > wndw - svgw)
				m_iOffsetX = wndw - svgw;
		}
		if(wndh < svgh)
		{
			//窗口高度小于SVG宽度时
			if(m_iOffsetY < wndh-svgh)
				m_iOffsetY = wndh-svgh;
			if(m_iOffsetY > 0)
				m_iOffsetY = 0;
		}
		else
		{
			//窗口高度大于SVG宽度时
			if(m_iOffsetY < 0)
				m_iOffsetY = 0;
			if(m_iOffsetY > wndh - svgh)
				m_iOffsetY = wndh - svgh;
		}
		RedrawWindow();
		m_iMoving = 2;
	}
	else
	{
		float x = GetSvgCoord(point.x - m_iOffsetX);
		float y = GetSvgCoord(point.y - m_iOffsetY);
		SSvgObject *pObj = m_Document.GetRootObject();
		if(pObj)
			pObj = pObj->GetChild();
		if(pObj)
			pObj = pObj->GetNext();
		pObj = IsObjectInPoint(pObj,x,y);
		if(pObj != NULL)
		{
			SString caption = pObj->GetAttribute("caption");
			if(caption.length() > 0)
			{
				QPoint p(point.x,point.y);
				p = this->mapToGlobal(p);
				QToolTip::showText(p,caption.data());
				bTip=true;
			}
		}
	}
	if(!bTip)
	{
		QToolTip::hideText();
	}
	return 1;
}

int SSvgWnd::OnMouseWheel(int nFlag,int delta)
{
	if(delta > 0)
	{
		if(m_dScale < 100)
			SSvgPainter::SetScale(m_dScale*1.1);
	}
	else if(delta < 0)
	{
		if(m_dScale > 0.1)
			SSvgPainter::SetScale(m_dScale/1.1);
	}
	this->RedrawWindow();
	return 0;
}


int SSvgWnd::OnLButtonUp(int nFlag,SPoint point)
{
	if(m_iMoving == 1)
	{
		float x = GetSvgCoord(point.x - m_iOffsetX);
		float y = GetSvgCoord(point.y - m_iOffsetY);
		SSvgObject *pObj = m_Document.GetRootObject();
		if(pObj)
			pObj = pObj->GetChild();
		if(pObj)
			pObj = pObj->GetNext();
		pObj = IsObjectInPoint(pObj,x,y);
		m_iMoving = 0;
		if(pObj != NULL )
		{
			//LOGDEBUG("SSvgWnd::OnClickObject");
			OnClickObject(pObj);
		}
	}
	else if(m_iMoving >= 2)
	{
		OnMouseMove(nFlag,point);
		m_iMoving = 0;
	}
	// 未移动点击响应
	else if (m_iMoving == 0)
	{
		float x = GetSvgCoord(point.x);
		float y = GetSvgCoord(point.y);
		SSvgObject *pObj = m_Document.GetRootObject();
		if(pObj)
			pObj = pObj->GetChild();
		if(pObj)
			pObj = pObj->GetNext();
		pObj = IsObjectInPoint(pObj,x,y);
		m_iMoving = 0;
		if(pObj != NULL)
		{
			//LOGDEBUG("SSvgWnd::OnClickObject");
			OnClickObject(pObj);
		}
	}
	return 1;
}

SSvgObject* SSvgWnd::GetSvgObjectByCaption(SString sCaption)
{
	return this->m_Document.SearchByAttribute("caption",sCaption.data());
}

SSvgObject* SSvgWnd::GetSvgObjectByUri(SString sUri)
{
	return this->m_Document.SearchByURI(sUri);
}

void SSvgWnd::SetMovable(bool b)
{
	m_bMovable = b;
}

bool SSvgWnd::SetSvgText(SString sCaption, SString sText)
{
	LOGDEBUG("SetSvgText(%s,%s)",sCaption.data(),sText.data());
	SSvgObject *pSvgObj = this->GetSvgObjectByCaption(sCaption);
	if(pSvgObj == NULL)
	{
		LOGWARN("[%s]未在SVG中发现!修改内容为[%s]失败!",sCaption.data(),sText.data());
		return false;
	}
	pSvgObj->SetAttributeValue("text",sText);
#ifdef WIN32
	RECT rect;
	rect.left = pSvgObj->GetRect()->left;
	rect.top = pSvgObj->GetRect()->top;
	rect.right = pSvgObj->GetRect()->right;
	rect.bottom = pSvgObj->GetRect()->bottom;
	::InvalidateRect(m_hWnd,&rect,TRUE);
#else
#endif
	return true;
}
SPoint  SSvgWnd::GetOffsetPoint()
{
	SPoint point;
	point.x=m_iOffsetX;
	point.y=m_iOffsetY;
	return point;
}

