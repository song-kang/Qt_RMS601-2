/**
 *
 * 文 件 名 : SSvgObject.h
 * 创建日期 : 2007-04-09 08:55
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG对象基类,封装SVG的基本元素
 * 修改记录 : 
 *            $Log: $
 *
 **/

#if !defined(AFX_SKTSVGOBJECT_H__486DC941_8FDD_4B7B_8C09_947ECDB2F642__INCLUDED_)
#define AFX_SKTSVGOBJECT_H__486DC941_8FDD_4B7B_8C09_947ECDB2F642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../SString.h"
#include "../../SFile.h"
#include "../../SXmlConfig.h"
#include "../SBaseDC.h"
#include "../SBaseWnd.h"

class SSvgAttribute
{
public:
	struct stuAttribute
	{
		SString name;
		SString value;
	};
	SSvgAttribute()
	{
		m_Attribute.setAutoDelete(true);
	}
	~SSvgAttribute()
	{
	}
	stuAttribute* Find(SString sName)
	{
		int i,cnt = m_Attribute.count();
		for(i=0;i<cnt;i++)
		{
			stuAttribute *pAttr = m_Attribute[i];
			if(pAttr->name == sName)
			{
				return pAttr;
			}
		}
		return NULL;
	}
	bool Lookup(SString sName,SString &sVal)
	{
		stuAttribute *pAttr = Find(sName);
		if(pAttr == NULL)
			return false;
		sVal = pAttr->value;
		return true;
	}
	void SetAt(SString sName,SString sValue)
	{
		stuAttribute *pAttr = Find(sName);
		if(pAttr != NULL)
		{
			pAttr->value = sValue;
		}
		else
		{
			pAttr = new stuAttribute();
			pAttr->name = sName;
			pAttr->value = sValue;
			m_Attribute.append(pAttr);
		}		
	}
	int GetCount()
	{
		return m_Attribute.count();
	}
	bool GetItem(int index,SString &sName,SString &sValue)
	{
		if(index<0 && index>=m_Attribute.count())
			return false;
		stuAttribute *pAttr = m_Attribute[index];
		if(pAttr == NULL)
			return false;
		sName = pAttr->name;
		sValue= pAttr->value;
		return true;
	}

private:
	SPtrList<stuAttribute> m_Attribute;
};
class SSvgObject : public SString
{
public:
	//定义SVG元素类型
	enum SVG_TYPE
	{
		TYPE_SVG,				//根对象
		TYPE_GROUP,			//SVG组
		TYPE_RECT,			//区域
		TYPE_TEXT,			//文字
		TYPE_ELLIPSE,		//椭圆
		TYPE_LINE,			//直线
		TYPE_POLYLINE,	//折线
		TYPE_PATH,			//路径
		TYPE_PIC,				//图片
		TYPE_UNKNOWN		//未知
	};
	//定义SVG显示范围
	struct SVG_RECT
	{
		float left,top,right,bottom;
	};
	

public:
	SSvgObject();
	virtual ~SSvgObject();
	
public:
	bool IsExistObj(SSvgObject *pObj);
	//设置属性值
	void SetAttributeValue(char* sAttribute, char* sValue, bool bSetNext=false,bool bSetChild=true);
	//设置属性值
	void SetAttributeValue(char* sAttribute, SString sValue, bool bSetNext=false,bool bSetChild=true);
	//增加下一个对象
	void SetNextObject(SSvgObject* pNewObj);
	//取下一个对象
	inline SSvgObject* GetNext(){return m_pNext;};
	inline void SetNext(SSvgObject* pNext){m_pNext = pNext;};
	//取下一层对象的首节点
	inline SSvgObject* GetChild(){return m_pChild;};
	//设置下一层对象的首节点
	inline void SetChild(SSvgObject* pChild){m_pChild = pChild;};
	//取父节点
	inline SSvgObject* GetParent(){return m_pParent;};
	//设置父节点
	inline void SetParent(SSvgObject* pParent){m_pParent = pParent;};
	//插入下级节点，同时根据下级的Z坐标值排序
	void AddChild(SSvgObject* pChild);
	//设置对象类型名称
	inline void SetName(SString sName){m_sName = sName;};
	inline SString GetName(){return m_sName;};
	//删除当前对象及其后续节点及下级节点
	void Detete();
	//删除指定的子节点
	void DeleteChild(SSvgObject* pChild, bool bDestroy=false);
	//加载当前对象的有效属性,如对象类型、显示范围等
	bool BuildObject(SSvgObject *pObj);//
	//根据类型名称取对象类型
	int GetObjectTypeByName(SString sName);
	//依据当前对象类型，计算当前对象的显示范围
	bool CalcObjectRect();
	//取指定名称的属性值
	SString GetAttribute(SString sName);
	float GetAttributeF(SString sName);
	int GetAttributeI(SString sName);
	//取对象类型
	inline int GetType(){return m_SvgType;};
	inline SVG_RECT* GetRect(){return &m_Rect;};

	static bool IsRectInRect(SVG_RECT *pRect1, SVG_RECT *pRect2);
	//将当前对象及其下级偏移指定大小
	bool Offset(float dx, float dy);
	//将当前对象基于指定位置成比例缩放
	bool Scale(float scale,float x0=0,float y0=0);

	//复制自身对象及其下级
	SSvgObject* Clone(SSvgObject *pParentObj=NULL, bool bWithNext=false);
	inline bool IsFlash(){return m_bFlash;};
	inline void SetFlash(bool b){m_bFlash = b;};
	SSvgObject *m_pChild;//指向下级节点的首节点
	SSvgObject *m_pLastChild;//指向下级节点的首节点
private:
	SSvgAttribute m_mapAttribute;//对象属性集合
	SSvgObject *m_pNext;//指向下一节点
	//SSvgObject *m_pChild;//指向下级节点的首节点
	SSvgObject *m_pParent;//指向父节点
	bool m_bFlash;//是否是动画图层
	int m_SvgType;//对象类型
	SVG_RECT m_Rect;//当前元素的显示范围
	SString m_sName;//对象类型名称：line/text/path/rect
};

#endif // !defined(AFX_SKTSVGOBJECT_H__486DC941_8FDD_4B7B_8C09_947ECDB2F642__INCLUDED_)
