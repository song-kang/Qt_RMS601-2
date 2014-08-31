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

#include "SSvgObject.h"
#include "../SPicture.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "SSvgPainter.h"

SSvgObject::SSvgObject()
{
	m_pNext = NULL;
	m_pChild = NULL;
	m_pParent = NULL;
	m_SvgType = TYPE_GROUP;
	m_Rect.left = 0.0f;
	m_Rect.top = 0.0f;
	m_Rect.right = 0.0f;
	m_Rect.bottom = 0.0f;
	m_bFlash = false;
}

SSvgObject::~SSvgObject()
{

}

void SSvgObject::Detete()
{
	SSvgObject *p=this->GetChild();
	if(p)
		p->Detete();
	p = this->GetNext();
	if(p)
	{
		p->Detete();
	}
	SPicture *pPic = (SPicture*)GetAttributeI("picobj");
	if(pPic != NULL)
		delete pPic;

	delete this;
}

//删除指定的子节点
void SSvgObject::DeleteChild(SSvgObject* pChild, bool bDestroy)
{
	SSvgObject *p0=NULL,*p=this->GetChild();
	while(p && p != pChild)
	{
		p0 = p;
		p = p->GetNext();
	}
	if(p0 == NULL)
	{
		//首节点
		p->SetChild(pChild->GetNext());
	}
	else
	{
		p0->SetNext(pChild->GetNext());
	}
	if(bDestroy)
		delete pChild;
}

//设置属性值
void SSvgObject::SetAttributeValue(char* sAttribute, char* sValue, bool bSetNext, bool bSetChild)
{
	SSvgObject *p = this;
	while(p)
	{
		p->m_mapAttribute.SetAt(sAttribute,sValue);

		//修改图片
		if(p->GetType() ==  TYPE_PIC && strcmp(sAttribute,"href") == 0)
		{
			SPicture *pPic = (SPicture*)p->GetAttributeI("picobj");
			if(pPic != NULL)
			{
				p->SetAttributeValue("picobj","",false,false);
				delete pPic;
			}
		}

		if(bSetChild && p->GetChild())
			p->GetChild()->SetAttributeValue(sAttribute,sValue,true);
		if(!bSetNext)
			break;
		p = p->GetNext();
	}
}

//设置属性值
void SSvgObject::SetAttributeValue(char* sAttribute, SString sValue, bool bSetNext, bool bSetChild)
{
	SetAttributeValue(sAttribute,sValue.GetBuffer(0));
}

//增加下级对象
void SSvgObject::SetNextObject(SSvgObject* pNewObj)
{
	SSvgObject *p=this;
	while(p->m_pNext)
	{
		p = p->m_pNext;
	}
	p->m_pNext = pNewObj;
}

//加载当前对象的有效属性,如对象类型、显示范围等
bool SSvgObject::BuildObject(SSvgObject *pObj)
{
	SSvgObject *p=pObj;
	while(p)
	{
		p->m_SvgType = p->GetObjectTypeByName(p->m_sName);
		BuildObject(p->GetChild());
		p = p->GetNext();
	}	
	return true;
}

//根据类型名称取对象类型
int SSvgObject::GetObjectTypeByName(SString sName)
{
	int SvgType;
	if(sName == "svg")
		SvgType = TYPE_SVG;
	else if(sName == "g")
		SvgType = TYPE_GROUP;
	else if(sName == "rect")
		SvgType = TYPE_RECT;
	else if(sName == "text")
		SvgType = TYPE_TEXT;
	else if(sName == "ellipse")
		SvgType = TYPE_ELLIPSE;
	else if(sName == "line")
		SvgType = TYPE_LINE;
	else if(sName == "polyline")
		SvgType = TYPE_POLYLINE;
	else if(sName == "path")
		SvgType = TYPE_PATH;
	else if(sName == "image")
		SvgType = TYPE_PIC;
	else
		SvgType = TYPE_UNKNOWN;
	return SvgType;
}

//依据当前对象类型，计算当前对象的显示范围
bool SSvgObject::CalcObjectRect()
{
	switch(m_SvgType)
	{
		case TYPE_SVG:
		case TYPE_GROUP://组的范围为组内各元素交集的最大范围
			{
				SSvgObject *pObj=GetChild();
				m_Rect.left  =  999999;
				m_Rect.right = -999999;
				m_Rect.top   =  999999;
				m_Rect.bottom= -999999;
				while(pObj)
				{
					pObj->CalcObjectRect();//先计算出下级的范围
					if(!(pObj->GetType() == TYPE_GROUP ||
						 pObj->GetType() == TYPE_RECT ||
						 pObj->GetType() == TYPE_PIC  ||
						 pObj->GetType() == TYPE_TEXT ||
						 pObj->GetType() == TYPE_ELLIPSE ||
						 pObj->GetType() == TYPE_LINE ||
						 pObj->GetType() == TYPE_POLYLINE ||
						 pObj->GetType() == TYPE_PATH
						 ))
					{
						pObj = pObj->GetNext();
						continue;
					}
					SVG_RECT *pRect=pObj->GetRect();
					if(pRect->left < m_Rect.left)
						m_Rect.left = pRect->left;
					if(pRect->top < m_Rect.top)
						m_Rect.top = pRect->top;
					if(pRect->right > m_Rect.right)
						m_Rect.right = pRect->right;
					if(pRect->bottom > m_Rect.bottom)
						m_Rect.bottom = pRect->bottom;
					pObj = pObj->GetNext();
				}
			}
			break;
		case TYPE_RECT:
		case TYPE_PATH:
		case TYPE_PIC:
			m_Rect.left = GetAttributeF("x");
			m_Rect.top = GetAttributeF("y");
			m_Rect.right = m_Rect.left + GetAttributeF("width");
			m_Rect.bottom = m_Rect.top + GetAttributeF("height");
			if(m_Rect.right<m_Rect.left)
			{
				float f;
				f=m_Rect.right;
				m_Rect.right=m_Rect.left;
				m_Rect.left=f;
			}
			if(m_Rect.bottom<m_Rect.top)
			{
				float f;
				f=m_Rect.top;
				m_Rect.top=m_Rect.bottom;
				m_Rect.bottom=f;
			}
			break;
		case TYPE_TEXT:
			{
				m_Rect.left = GetAttributeF("x");
				m_Rect.bottom = GetAttributeF("y");
				//CWnd *pWnd=::AfxGetMainWnd();
				/*TODO
				HWND h=GetDesktopWindow();
				if(h == NULL)
					break;
				*/
				
				//SBaseDC dc;
				//dc.Attach(::GetDC(h));// ();
				YSSIZE size=SSvgPainter::GetTextSize(NULL,this);//TODO
				m_Rect.top = m_Rect.bottom - size.cy;
				m_Rect.right = m_Rect.left + size.cx;
			}
			break;
		case TYPE_ELLIPSE:
			m_Rect.left = GetAttributeF("cx")-GetAttributeF("rx");
			m_Rect.top = GetAttributeF("cy")-GetAttributeF("ry");
			m_Rect.right = GetAttributeF("cx")+GetAttributeF("rx");
			m_Rect.bottom = GetAttributeF("cy")+GetAttributeF("ry");			
			break;
		case TYPE_LINE:
			{
				float x1=GetAttributeF("x1");
				float y1=GetAttributeF("y1");
				float x2=GetAttributeF("x2");
				float y2=GetAttributeF("y2");
				m_Rect.left = x1>=x2?x2:x1;
				m_Rect.right = x1>=x2?x1:x2;
				m_Rect.top = y1>=y2?y2:y1;
				m_Rect.bottom = y1>=y2?y1:y2;
			}
			break;
		case TYPE_POLYLINE:
			{
				SString points=GetAttribute("points");
				int p1=0,p2,p3;
				float x0,y0;
				float x1=-99999,y1=-99999,x2=99999,y2=99999;
				while(1)
				{
					p2=points.Find(",",p1);
					if(p2 < 0)
						break;
					x0 = atof(points.Mid(p1,p2-p1));
					p2 ++;
					p3=points.Find(" ",p2);
					if(p3 < 0)
						p3 = points.GetLength();
					y0 = atof(points.Mid(p2,p3-p2));
					p1 = p3+1;
					
					if(x0>x1)
						x1 = x0;
					if(x0<x2)
						x2 = x0;
					if(y0>y1)
						y1 = y0;
					if(y0<y2)
						y2 = y0;
				}
				m_Rect.left = x2;
				m_Rect.right = x1;
				m_Rect.top = y2;
				m_Rect.bottom = y1;				
			}
			break;
	}
	return true;
}

//取指定名称的属性值
SString SSvgObject::GetAttribute(SString sName)
{
	if(sName == "name")
	{
		return m_sName;
	}
	if(this->m_SvgType == TYPE_GROUP)
	{
		if(sName == "x")
			return SString::toString((long)m_Rect.left);
		if(sName == "y")
			return SString::toString((long)m_Rect.top);
		if(sName == "width")
			return SString::toString((long)(m_Rect.right-m_Rect.left));
		if(sName == "height")
			return SString::toString((long)(m_Rect.bottom-m_Rect.top));
	}
	SString val;
	m_mapAttribute.Lookup(sName,val);
	return val;
}

float SSvgObject::GetAttributeF(SString sName)
{
	return GetAttribute(sName).toFloat();
}

int SSvgObject::GetAttributeI(SString sName)
{
	return GetAttribute(sName).toInt();
}

bool SSvgObject::IsRectInRect(SVG_RECT *pRect1, SVG_RECT *pRect2)
{
	//左上角
	if(pRect1->left >= pRect2->left && pRect1->left <= pRect2->right &&
		 pRect1->top >= pRect2->top && pRect1->top <= pRect2->bottom)
		 return true;
	//左下角
	if(pRect1->left >= pRect2->left && pRect1->left <= pRect2->right &&
		 pRect1->bottom >= pRect2->top && pRect1->bottom <= pRect2->bottom)
		 return true;
	//右上角
	if(pRect1->right >= pRect2->left && pRect1->right <= pRect2->right &&
		 pRect1->top >= pRect2->top && pRect1->top <= pRect2->bottom)
		 return true;
	//右下角
	if(pRect1->right >= pRect2->left && pRect1->right <= pRect2->right &&
		 pRect1->bottom >= pRect2->top && pRect1->bottom <= pRect2->bottom)
		 return true;
	//中心点
	float x,y;
	x=pRect1->left+(pRect1->right-pRect1->left)/2;
	y=pRect1->top+(pRect1->bottom-pRect1->top)/2;
	if(x >= pRect2->left && x <= pRect2->right &&
		 y >= pRect2->top && y <= pRect2->bottom)
		 return true;
	return false;
}

//插入下级节点，同时根据下级的Z坐标值排序
void SSvgObject::AddChild(SSvgObject* pChild)
{
#if 1
	if(m_pChild == NULL)
	{
		m_pLastChild = m_pChild = pChild;
	}
	else
	{
		m_pLastChild->m_pNext = pChild;
		m_pLastChild = pChild;
	}
#else
	pChild->m_pParent = this;
	if(m_pChild == NULL)
	{
		m_pChild = pChild;
		return;
	}
	SSvgObject *prev=NULL,*p=m_pChild;
	while(p)
	{
		if(pChild->GetAttributeI("z")<p->GetAttributeI("z"))
		{
			//就插在当前节点之前
			if(prev == NULL)//表示当前节点是第一个节点
			{
				m_pChild = pChild;
				pChild->m_pNext = p;
			}
			else
			{
				pChild->m_pNext = p;
				prev->m_pNext = pChild;
			}
			return;
		}
		prev = p;
		p = p->GetNext();
	}
	if(prev)
	{
		pChild->m_pNext = p;
		prev->m_pNext = pChild;
	}
#endif
}

//将当前对象及其下级偏移指定大小
bool SSvgObject::Offset(float dx, float dy)
{
	float x,y;
	SString sTemp;
	switch(GetType())
	{
		case TYPE_GROUP:
			{
				SSvgObject *p = GetChild();
				while(p)
				{
					p->Offset(dx,dy);
					p = p->GetNext();
				}
			}
			break;
		case TYPE_RECT:
		case TYPE_TEXT:	
		case TYPE_PIC:
			x = GetAttributeF("x") + dx;
			y = GetAttributeF("y") + dy;
			SetAttributeValue("x",SString::toString(x).GetBuffer(0));
			SetAttributeValue("y",SString::toString(y).GetBuffer(0));
			break;
		case TYPE_PATH:
			{
				//break;
				x = GetAttributeF("x") + dx;
				y = GetAttributeF("y") + dy;
				SetAttributeValue("x",SString::toString(x).GetBuffer(0));
				SetAttributeValue("y",SString::toString(y).GetBuffer(0));
				//修改d属性
				SString points = GetAttribute("d");
				int index=0,p1=0,p2;
				char oper,ch;
				SString newpnt="",str;
				float x0,y0;
				//M179.2,96.9 l 4.2,-11.1
				p1 = points.Find(" ");
				if(p1<=0)
					break;
				str = points.Left(p1);
				str = str.Right(str.GetLength()-1);
				newpnt = "M"+SString::toString(SString::GetIdAttributeF(1,str,",")+dx)+","+
						SString::toString(SString::GetIdAttributeF(2,str,",")+dy)+points.Mid(p1);
				SetAttributeValue("d",newpnt);
				newpnt = "";
				bool bArc=false;
				p1 = 0;
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
							newpnt += oper;
			nextLine:
							p2 = points.Find(",",p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							x0 = str.toFloat();
							p1 = p2+1;
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								p2 = points.GetLength();
							str = points.Mid(p1,p2-p1);
							y0 = str.toFloat();
							p1 = p2+1;
							newpnt += SString::toString(x0+dx)+","+SString::toString(y0+dy);
							if(p1 < points.GetLength())
							{
								ch = points.GetAt(p1);
								if(ch >='0' && ch <= '9' || ch == '-')
									goto nextLine;//重复
								if(points.GetAt(p1-1) == ' ')
									newpnt+=" ";
							}
						}
						break;
					case 'A':
					case 'a':
						{
							float rx,ry;
							float endx,endy;
							int xrotation;
							int large_arc_flag;
							int sweep_flag;
							newpnt+=oper;
							bArc = true;
			nextArc:
							//A11,11 0 1 0 90,432.5
							//rx
							p2 = points.Find(",",p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							rx = str.toInt();
							p1 = p2+1;

							//ry
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							ry = str.toInt();
							p1 = p2+1;
							
							//xrotation
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							xrotation = str.toInt();
							p1 = p2+1;

							//large-arc-flag
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							large_arc_flag = str.toInt();
							p1 = p2+1;

							//sweep-flag
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							sweep_flag = str.toInt();
							p1 = p2+1;

							//x
							p2 = points.Find(',',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							endx = str.toFloat();
							p1 = p2+1;

							//y
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								p2 = points.GetLength();
							str = points.Mid(p1,p2-p1);
							endy = str.toFloat();
							p1 = p2+1;
							
							newpnt += SString::toString(rx+dx)+","+SString::toString(ry+dy)+" "+
								SString::toString(xrotation)+" "+SString::toString(large_arc_flag)+" "+
								SString::toString(sweep_flag)+" "+SString::toString(endx+dx)+","+SString::toString(endy+dy);
							if(p1 < points.GetLength())
							{
								ch = points.GetAt(p1);
								if(ch >='0' && ch <= '9' || ch == '-')
									goto nextArc;//重复
								if(points.GetAt(p1-1) == ' ')
									newpnt+=" ";
							}
						}
					break;
					}
				}
				if(bArc)
					SetAttributeValue("d",newpnt);
				break;
			}
			break;
		case TYPE_ELLIPSE:	
			x = GetAttributeF("cx") + dx;
			y = GetAttributeF("cy") + dy;
			SetAttributeValue("cx",SString::toString(x).GetBuffer(0));
			SetAttributeValue("cy",SString::toString(y).GetBuffer(0));
			break;
		case TYPE_LINE:	
			x = GetAttributeF("x1") + dx;
			y = GetAttributeF("y1") + dy;
			SetAttributeValue("x1",SString::toString(x).GetBuffer(0));
			SetAttributeValue("y1",SString::toString(y).GetBuffer(0));
			x = GetAttributeF("x2") + dx;
			y = GetAttributeF("y2") + dy;
			SetAttributeValue("x2",SString::toString(x).GetBuffer(0));
			SetAttributeValue("y2",SString::toString(y).GetBuffer(0));
			break;
		case TYPE_POLYLINE:
			{
				SString points=GetAttribute("points");
				SString newpoints="";
				int p1=0,p2,p3;
				while(1)
				{
					p2=points.Find(",",p1);
					if(p2 < 0)
						break;
					x = atof(points.Mid(p1,p2-p1));
					p2 ++;
					p3=points.Find(" ",p2);
					if(p3 < 0)
						p3 = points.GetLength();
					y = atof(points.Mid(p2,p3-p2));
					p1 = p3+1;
					
					x += dx;
					y += dy;
					newpoints += SString::toString(x)+","+SString::toString(y)+" ";
				}
				if(newpoints.GetLength()>0)
					newpoints = newpoints.Left(newpoints.GetLength()-1);
				SetAttributeValue("points",newpoints.GetBuffer(0));
			}

			break;
	}
	CalcObjectRect();
	return true;
}

//将当前对象基于指定位置成比例缩放
bool SSvgObject::Scale(float scale,float px0,float py0)
{
	float x,y,width,height;
	SString sTemp;
	switch(GetType())
	{
		case TYPE_GROUP:
			{
				SSvgObject *p = GetChild();
				while(p)
				{
					p->Scale(scale,px0,py0);
					p = p->GetNext();
				}
			}
			break;
		case TYPE_RECT:
		case TYPE_PIC:
			x = GetAttributeF("x") ;
			y = GetAttributeF("y") ;
			width = GetAttributeF("width") ;
			height = GetAttributeF("height") ;
			x = (x-px0)*scale;
			y = (y-py0)*scale;
			width *= scale;
			height *= scale;
			SetAttributeValue("x",SString::toString(x).GetBuffer(0));
			SetAttributeValue("y",SString::toString(y).GetBuffer(0));
			SetAttributeValue("width",SString::toString(width).GetBuffer(0));
			SetAttributeValue("height",SString::toString(height).GetBuffer(0));
			break;
		case TYPE_TEXT:	
			x = GetAttributeF("x") ;
			y = GetAttributeF("y") ;
			height = GetAttributeF("font-size");
			x = (x-px0)*scale;
			y = (y-py0)*scale;
			height *= scale;
			SetAttributeValue("x",SString::toString(x).GetBuffer(0));
			SetAttributeValue("y",SString::toString(y).GetBuffer(0));
			SetAttributeValue("font-size",SString::toString(height).GetBuffer(0));
			break;
		case TYPE_PATH:
			{
				//break;
				x = GetAttributeF("x") ;
				y = GetAttributeF("y") ;
				SetAttributeValue("x",SString::toString(x).GetBuffer(0));
				SetAttributeValue("y",SString::toString(y).GetBuffer(0));
				//修改d属性
				SString points = GetAttribute("d");
				int index=0,p1=0,p2;
				char oper,ch;
				SString newpnt="",str;
				float x0,y0;
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
							newpnt += oper;
			nextLine:
							p2 = points.Find(",",p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							x0 = str.toFloat();
							p1 = p2+1;
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								p2 = points.GetLength();
							str = points.Mid(p1,p2-p1);
							y0 = str.toFloat();
							p1 = p2+1;
							if(oper == 'M' || oper == 'm')
								newpnt += SString::toString((x0-px0)*scale)+","+SString::toString((y0-py0)*scale);
							else
								newpnt += SString::toString(x0*scale)+","+SString::toString(y0*scale);
							newpnt += " ";
							if(p1 < points.GetLength())
							{
								ch = points.GetAt(p1);
								if(ch >='0' && ch <= '9' || ch == '-')
									goto nextLine;//重复
								if(points.GetAt(p1-1) == ' ')
									newpnt+=" ";
							}
						}
						break;
					case 'A':
					case 'a':
						{
							float rx,ry;
							float endx,endy;
							int xrotation;
							int large_arc_flag;
							int sweep_flag;
							newpnt+=oper;
			nextArc:
							//A11,11 0 1 0 90,432.5
							//rx
							p2 = points.Find(",",p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							rx = str.toInt();
							p1 = p2+1;

							//ry
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							ry = str.toInt();
							p1 = p2+1;
							
							//xrotation
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							xrotation = str.toInt();
							p1 = p2+1;

							//large-arc-flag
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							large_arc_flag = str.toInt();
							p1 = p2+1;

							//sweep-flag
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							sweep_flag = str.toInt();
							p1 = p2+1;

							//x
							p2 = points.Find(',',p1);
							if(p2 < 0)
								break;
							str = points.Mid(p1,p2-p1);
							endx = str.toFloat();
							p1 = p2+1;

							//y
							p2 = points.Find(' ',p1);
							if(p2 < 0)
								p2 = points.GetLength();
							str = points.Mid(p1,p2-p1);
							endy = str.toFloat();
							p1 = p2+1;
							
							newpnt += SString::toString((rx-px0)*scale)+","+SString::toString((ry-px0)*scale)+" "+
								SString::toString(xrotation)+" "+SString::toString(large_arc_flag)+" "+
								SString::toString(sweep_flag)+" "+SString::toString((endx-px0)*scale)+","+SString::toString((endy-px0)*scale);
							if(p1 < points.GetLength())
							{
								ch = points.GetAt(p1);
								if(ch >='0' && ch <= '9' || ch == '-')
									goto nextArc;//重复
								if(points.GetAt(p1-1) == ' ')
									newpnt+=" ";
							}
						}
					break;
					}
				}
				SetAttributeValue("d",newpnt);
				break;
			}
			break;
		case TYPE_ELLIPSE:	
			x = GetAttributeF("cx") ;
			y = GetAttributeF("cy") ;
			width = GetAttributeF("rx") ;
			height = GetAttributeF("ry") ;
			x = (x-px0)*scale;
			y = (y-py0)*scale;
			width *= scale;
			height *= scale;
			SetAttributeValue("cx",SString::toString(x).GetBuffer(0));
			SetAttributeValue("cy",SString::toString(y).GetBuffer(0));
			SetAttributeValue("rx",SString::toString(width).GetBuffer(0));
			SetAttributeValue("ry",SString::toString(height).GetBuffer(0));
			break;
		case TYPE_LINE:	
			x = GetAttributeF("x1") ;
			y = GetAttributeF("y1") ;
			x = (x-px0)*scale;
			y = (y-py0)*scale;
			SetAttributeValue("x1",SString::toString(x).GetBuffer(0));
			SetAttributeValue("y1",SString::toString(y).GetBuffer(0));
			x = GetAttributeF("x2") ;
			y = GetAttributeF("y2") ;
			x = (x-px0)*scale;
			y = (y-py0)*scale;
			SetAttributeValue("x2",SString::toString(x).GetBuffer(0));
			SetAttributeValue("y2",SString::toString(y).GetBuffer(0));
			break;
		case TYPE_POLYLINE:
			{
				SString points=GetAttribute("points");
				SString newpoints="";
				int p1=0,p2,p3;
				while(1)
				{
					p2=points.Find(",",p1);
					if(p2 < 0)
						break;
					x = atof(points.Mid(p1,p2-p1));
					p2 ++;
					p3=points.Find(" ",p2);
					if(p3 < 0)
						p3 = points.GetLength();
					y = atof(points.Mid(p2,p3-p2));
					p1 = p3+1;
					
					x = (x-px0)*scale;
					y = (y-py0)*scale;
					newpoints += SString::toString(x)+","+SString::toString(y)+" ";
				}
				if(newpoints.GetLength()>0)
					newpoints = newpoints.Left(newpoints.GetLength()-1);
				SetAttributeValue("points",newpoints.GetBuffer(0));
			}

			break;
	}
	CalcObjectRect();	
	return true;
}


SSvgObject* SSvgObject::Clone(SSvgObject *pParentObj, bool bWithNext)
{
	SSvgObject *pNewObj=NULL;
	SSvgObject *p = this;
	while(p)
	{
		pNewObj = new SSvgObject();
		//copy attribute
		SString key,val;
		int i,cnt = p->m_mapAttribute.GetCount();
		for(i=0;i<cnt;i++)
		{
			p->m_mapAttribute.GetItem(i,key,val);
			pNewObj->m_mapAttribute.SetAt(key,val);
		}
		pNewObj->m_SvgType = p->m_SvgType;
		pNewObj->m_Rect = p->m_Rect;
		pNewObj->m_sName = p->m_sName;
		LOGDEBUG("%s\n",pNewObj->m_sName.data());
		pNewObj->m_pParent = pParentObj;
		if(bWithNext == false)
		{
			pNewObj->m_pNext = NULL;
		}
		if(p->m_pChild)
		{
			p->m_pChild->Clone(pNewObj,true);//执行下级
		}
		if(pParentObj)
		{
			pParentObj->AddChild(pNewObj);
		}
		if(bWithNext == false)
			break;
		p = p->GetNext();
	}
	return pNewObj;
};

bool SSvgObject::IsExistObj(SSvgObject *pObj)
{
	SSvgObject *p=this;
	while(p)
	{
		if(p == pObj)
			return true;
		if(p->GetChild()->IsExistObj(pObj))
			return true;
		p = p->GetNext();
	}
	
	return false;
}
