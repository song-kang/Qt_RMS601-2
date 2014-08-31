/**
 *
 * 文 件 名 : SSvgDocument.h
 * 创建日期 : 2007-04-08 18:55
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : SVG文档类
 * 修改记录 : 
 *            $Log: $
 *
 **/

#if !defined(AFX_SKTSVGDOCUMENT_H__F7C1AB6F_CEDE_4835_8D08_4D5EEA9BA80F__INCLUDED_)
#define AFX_SKTSVGDOCUMENT_H__F7C1AB6F_CEDE_4835_8D08_4D5EEA9BA80F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../SString.h"
#include "../../SFile.h"
#include "../../SXmlConfig.h"

#include "SSvgObject.h"

class SSvgDocument  
{
public:
	void ParseStyle(SSvgObject *pObj, SString style);
	SSvgObject* AddObject(SString sName, SString sAttr);
	SString GetAllObjects();
	SSvgObject* SearchByURI(SString uri);
	SSvgObject* SearchByAttribute(char* attr,char* val);
	void SortObject(SSvgObject *pObj);
	bool NewFile();
	SSvgDocument();
	virtual ~SSvgDocument();

	//加载SVG文件
	bool LoadSvgFile(SString sSvgFile);
	//保存SVG文件
	bool SaveSvgFile(SString sSvgFile);
	bool SaveSvgFileAsUtf8(SString sSvgFile);
	//递归写SVG节点
	bool WriteSvgNode(SSvgObject *pSvgObj, SFile *pFile, int iLevel=0);
	//写属性到文件
	bool WriteAttribute(SSvgObject *pSvgObj, SFile *pFile, SString sAttrName, SString sAttrValue="'{<NONE>}'");
	//递归加载SVG元素到根对象下
	bool ReadSvgNode(SSvgObject *pParentSvgObj, SBaseConfig *pXmlNode);
	//清除所有对象
	bool RemoveObjects();
	//取根对象
	SSvgObject* GetRootObject(){return m_pSvgRoot;};
	SSvgObject* GetFirstObject()
	{
		SSvgObject *pObj = GetRootObject();
		if(pObj == NULL)
			return NULL;
		pObj = pObj->GetChild();
		return pObj;
	};
	inline SString GetSvgPath(){return m_sSvgPath;};

private:
	SString m_sSvgFile;//SVG文件名
	SString m_sSvgPath;//SVG路径
	SSvgObject *m_pSvgRoot;//SVG根对象
	bool m_bUtf8;
};

#endif // !defined(AFX_YSSVGDOCUMENT_H__F7C1AB6F_CEDE_4835_8D08_4D5EEA9BA80F__INCLUDED_)
