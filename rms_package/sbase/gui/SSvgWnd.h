/**
 *
 * 文 件 名 : SSvgWnd.h
 * 创建日期 : 2008-02-15 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
 *            $Log: $
 *
 **/

#ifndef __SKT_SVGWND_INC_H__
#define __SKT_SVGWND_INC_H__

#include "SGui.h"
#include "svg/SSvgDefine.h"
#include "svg/SSvgDocument.h"
#include "svg/SSvgPainter.h"

class SSvgWnd : public SWnd, public SSvgPainter
{
public:

	bool SetSvgText(SString sCaption,SString sText);
	void SetMovable(bool b);
	SSvgWnd();
#ifdef QT_GUI_LIB
	SSvgWnd(QWidget *parent) : SWnd(parent)
	{
		m_bMovable = false;
		m_iMoving = 0;
		m_iOffsetX = m_iOffsetY = 0;
	}
#endif
	~SSvgWnd();

	inline void SetSvgPath(SString sPath){m_sSvgPath = sPath;};
	bool Load(SString sSvgFile);

	SSvgObject* GetSvgObjectByCaption(SString sCaption);
	SSvgObject* GetSvgObjectByUri(SString sUri);
	
	virtual bool OnPaint(SBaseDC *pDC);
	virtual int OnLButtonDown(int nFlag,SPoint point);
	virtual int OnLButtonUp(int nFlag,SPoint point);
	virtual int OnMouseMove(int nFlag,SPoint point);
	virtual int OnMouseWheel(int nFlag,int delta);

	virtual void OnClickObject(SSvgObject *pSvgObj){};
	virtual int OnTimer(int nIDEvent){/*printf("@@@@@@@@@@@@@@@@@@@@@SSvgWnd::OnTimer(),this=%p\n",this);*/return 1;};
	SPoint  GetOffsetPoint();
	void SetOffset(int x,int y)
	{
		m_iOffsetX = x;
		m_iOffsetY = y;
	}
	int GetOffsetX(){return m_iOffsetX;}
	int GetOffsetY(){return m_iOffsetY;}
	void ToCenter();

private:
	SString m_sSvgPath;//svg文件的绝对路径
	bool m_bMovable;//是否可移动
	int m_iMoving;//是否正在移动，0未移动，1准备移动，2已经移动过且仍准备移动
	int m_iOffsetX,m_iOffsetY;//X、Y方向的偏移
	SPoint m_ptLast;
};


#endif//__YS_SVGWND_INC_H__
