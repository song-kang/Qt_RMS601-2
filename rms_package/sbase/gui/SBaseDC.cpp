/**
 *
 * 文 件 名 : SBaseDC.cpp
 * 创建日期 : 2008-01-17 14:25
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 通用的窗口类接口，仅定义标准的窗口操作，不具体实现
 * 修改记录 : 
 *            $Log: $
 *
 **/
#include "SBaseDC.h"

SCOLOR SRGB(BYTE r,BYTE g,BYTE b,BYTE a/*=0*/)
{
	SCOLOR color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
};
