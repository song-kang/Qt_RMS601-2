/**
 *
 * 文 件 名 : SXmlConfig.h
 * 创建日期 : 2007-07-14
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:35 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 实现配置文件基类接口的XML配置类
 * 修改记录 : 
 *            $Log: SXmlConfig.h,v $
 *            Revision 1.1  2011/08/31 08:43:35  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:44  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/01/22 01:24:26  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:09  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.4  2009/05/19 02:50:38  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/12/22 08:54:13  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/10/06 08:48:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:04  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.2  2007/09/11 08:17:53  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#ifndef __SKTXMLCONFIG_H__
#define __SKTXMLCONFIG_H__

#include "SBaseConfig.h"
#include "SFile.h"

//////////////////////////////////////////////////////////////////////////
// 名    称:  SXmlConfig
// 作    者:  邵凯田
// 创建时间:  2011-11-24 15:10
// 描    述:  配置文件基类
//////////////////////////////////////////////////////////////////////////
class SXmlConfig : public SBaseConfig
{
	enum STR_ENCODING
	{
		STR_ENCODING_GB2312,
		STR_ENCODING_UTF8
	};
public:
	SXmlConfig();
	virtual ~SXmlConfig();
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  从文本字符串加载XML
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 15:10
	// 参数说明:  @sXmlText为文本内容
	// 返 回 值:  true表示读取成功，false表示读取失败
	//////////////////////////////////////////////////////////////////////////
	bool ReadText(char* sXmlText);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  用当前实例保存XML配置文件
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 15:11
	// 参数说明:  @sXmlFile为目标文件名
	//            @sEncoding字符集编码,缺省为GB
	// 返 回 值:  true表示保存成功，false为失败
	//////////////////////////////////////////////////////////////////////////
	bool SaveConfig(SString sXmlFile,SString sEncoding="GB2312");

	////////////////////////////////////////////////////////////////////////
	// 描    述:  从文件读取XML
	// 作    者:  邵凯田
	// 创建时间:  2010-1-7 14:12
	// 参数说明:  @sXmlFile文件名称
	// 返 回 值:  true读取文件
	//////////////////////////////////////////////////////////////////////////
	bool ReadConfig(SString sXmlFile);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  从文本字符串加载XML
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 15:10
	// 参数说明:  @szXmlText为文本内容
	// 返 回 值:  true表示读取成功，false表示读取失败
	//////////////////////////////////////////////////////////////////////////
	bool LoadXmlByText(char * szXmlText);

private:
	////////////////////////////////////////////////////////////////////////
	// 描    述:  保存指定的节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-24 15:12
	// 参数说明:  @file为文件实例
	//            @pNode为节点实例
	//            @level为当前级别
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool SaveNode(SFile &file,SBaseConfig *pNode,int &level);

	STR_ENCODING m_iEncodeing;
};

#endif

