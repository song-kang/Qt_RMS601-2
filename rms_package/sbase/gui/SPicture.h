/**
 *
 * 文 件 名 : SPicture.h
 * 创建日期 : 2008-02-15 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
 *            $Log: $
 *
 **/

#ifndef __SKT_PICTURE_INC_H__
#define __SKT_PICTURE_INC_H__

#include "SBaseDC.h"

#ifdef QT_GUI_LIB
#include "qt/SDC.h"
#include <QPixmap>
#else
#ifndef WIN32 
#include "linux/SDC.h"
#endif

#endif


class SPicture
{
public:
	SPicture();
	~SPicture();
	bool Load(char* sFileName);
	void Render(unsigned int uDC,int x,int y,int width,int height);

	inline int GetWidth()		{return m_iWidth;};
	inline int GetHeight()	{return m_iHeight;};
private:
#ifdef QT_GUI_LIB
	QPixmap m_pixmap;
#else
#ifdef WIN32
	HANDLE m_hBitmap;	//位图句柄
	HDC m_hDCMem;			//内存DC
#else
	BITMAP *m_pBitmap;
#endif
#endif
	int m_iWidth,m_iHeight;//位图原始大小
};


#endif//__YS_PICTURE_INC_H__
