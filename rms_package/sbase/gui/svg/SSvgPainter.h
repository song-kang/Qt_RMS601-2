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

#if !defined(AFX_SKTSVGPAINTER_H__52A21C11_3038_48F4_9DD4_01C0BF478E71__INCLUDED_)
#define AFX_SKTSVGPAINTER_H__52A21C11_3038_48F4_9DD4_01C0BF478E71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "../SBaseWnd.h"
#include "../SBaseDC.h"
#include "SSvgObject.h"
#include "SSvgDocument.h"

#ifndef PS_DOT
#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5
#define PS_INSIDEFRAME      6
#define PS_USERSTYLE        7
#define PS_ALTERNATE        8
#define PS_STYLE_MASK       0x0000000F
#endif

#ifndef NULL_BRUSH
#define WHITE_BRUSH         0
#define LTGRAY_BRUSH        1
#define GRAY_BRUSH          2
#define DKGRAY_BRUSH        3
#define BLACK_BRUSH         4
#define NULL_BRUSH          5
#define HOLLOW_BRUSH        NULL_BRUSH
#define WHITE_PEN           6
#define BLACK_PEN           7
#define NULL_PEN            8
#define OEM_FIXED_FONT      10
#define ANSI_FIXED_FONT     11
#define ANSI_VAR_FONT       12
#define SYSTEM_FONT         13
#define DEVICE_DEFAULT_FONT 14
#define DEFAULT_PALETTE     15
#define SYSTEM_FIXED_FONT   16
#endif

#define C_TAG_RECT_RADIUS 3 //选对象的几个角的操作区块半径

//去除MINIGUI的宏定义
//#ifdef DrawText
//#undef DrawText
//#endif

class SSvgPainter 
{
public:
	void SetScale(double scale);
	SString GetAttributeByLevel(SSvgObject* pObj, SString sAttr);
	SSvgPainter();
	virtual ~SSvgPainter();
	
	//绘制一个对象
	void DrawObject(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0, bool bOnlyOne=false);
	void DrawFlashObject(SBaseDC *pDC, SSvgObject *pObj, int x, int y);
	void DrawObjectSelected(SBaseDC *pDC, SSvgObject *pObj, int x, int y,SCOLOR colTag,bool bEditable);
	//根据根对象绘制对齐点
	void DrawAlignPoint(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);

	void DrawRect(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);
	void DrawSvgText(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);
	void DrawEllipse(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);
	void DrawLine(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);
	void DrawPolyLine(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);
	void DrawPath(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);
	void DrawPic(SBaseDC *pDC, SSvgObject *pObj, int x=0, int y=0);
	static YSSIZE GetTextSize(SBaseDC *pDC, SSvgObject *pObj);
	
	//测试一个对象是否可显示
	bool IsInViewRect(SSvgObject *pObj);

	SSvgObject* IsPointInObjectRect(SSvgObject *pObj, float x,float y);
	//测试当前点是否经过当前对象及其下的兄弟节点，x,y为SVG坐标系
	SSvgObject* IsObjectInPoint(SSvgObject *pObj, float x,float y);
	//测试当前区域是否包含经过当前对象及其下的兄弟节点，x,y为SVG坐标系
	void SelectObjectInRect(SSvgObject *pObj, SSvgObject::SVG_RECT &selRect);

	//转换SVG坐标到当前窗口
	int GetCoord(float x);
	//转换当前窗口到SVG坐标
	float GetSvgCoord(float x);

	//转换颜色
	static SCOLOR GetColorByString(SString sColor);
	static SString GetStringByColor(SCOLOR color);
	static SString GetRGBStringByColor(SCOLOR color);

	//用于判断点是否靠近
	bool IsNear(float v1, float v2);
	//用于判断一个点是否在指定的线段上
	bool IsInLine(float x1, float y1, float x2, float y2, float px, float py);
	//根据一个椭圆及一个光标点，定位光标点对应在椭圆上的点
	void GetEllipseByPoint(float x,float y, 
		float width,float height,float px, float py,float &ex,float &ey);

	static void SetBaseWnd(SBaseWnd *p);

	SSvgDocument m_Document;//SVG文档对象
	//缩放比例
	double m_dScale;
	//当前视口范围，此范围是映射到SVG坐标系的
	SSvgObject::SVG_RECT m_ViewRect;
	//当前选中的对象列表，选中对象不在正常绘制中，而是动态刷新时绘制与选择框一起显示
	SPtrList<SSvgObject> m_SelectedObjList;
	SPtrList<SSvgObject> m_MemObjList;
	bool m_bEditAllow;//是否允许编辑对象
	
};

#endif // !defined(AFX_SKTSVGPAINTER_H__52A21C11_3038_48F4_9DD4_01C0BF478E71__INCLUDED_)
