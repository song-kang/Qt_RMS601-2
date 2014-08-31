/**
 *
 * 文 件 名 : SObject.h
 * 创建日期 : 2007-10-26 19:09
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:34 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 定义SObject基类，所有类均必须从其派生，用于检查对象地址是否4字节对齐
 * 修改记录 : 
 *            $Log: SObject.h,v $
 *            Revision 1.1  2011/08/31 08:43:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2008/09/01 11:48:22  taol
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/10/30 10:48:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *
 **/

#ifndef __SKT_OBJECT_H__

/**
 *
 * 文 件 名 : SObject.h
 * 创建日期 : 2007-10-26 19:12
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:34 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 定义SObject基类，用于检查对象地址是否4字节对齐
 * 修改记录 : 
 *            $Log: SObject.h,v $
 *            Revision 1.1  2011/08/31 08:43:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.2  2008/09/01 11:48:22  taol
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:02  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2007/10/30 10:48:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *
 **/

#define __SKT_OBJECT_H__

#include "SLog.h"

void SObjectWarn();


#ifndef _CHECK_OBJECT_
#define _SKTOBJECT_
#define SKTOBJECT(cls) 
#else
#define _SKTOBJECT_ \
					if(((int)this) % 4 != 0) \
					{ \
						LOGWARN("对象地址[%p]未按4字节对齐!这将造成ARM平台上的运行不稳定!!!",this); \
						SObjectWarn(); \
					} \
						
#define SKTOBJECT(cls) \
					if(((int)this) % 4 != 0) \
					{ \
						LOGWARN("%s对象地址[%p]未按4字节对齐!这将造成ARM平台上的运行不稳定!!!",cls,this); \
						SObjectWarn(); \
					} \

#endif


#endif//__SKT_OBJECT_H__


