/**
 *
 * 文 件 名	: SDatabase.cpp
 * 创建日期	: 2007-07-23
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: 2008/09/02 01:50:08 $
 * 当前版本	: $Revision: 1.1 $
 * 功能描述 : 通用数据库接口类及底层类
 * 修改记录 : 
 *            $Log: SDatabase.cpp,v $
 *            Revision 1.1  2008/09/02 01:50:08  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/04/03 02:44:34  shaokt
 *            LINUX下将ysbase分离为.so，单独编译，设备程序在编译时连接该so，不将目标码生成到执行文件中；WIN32下为调试方便还直接编译基类源码
 *
 *            Revision 1.2  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 **/

#include "SDatabase.h"


/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
SRecord::SRecord()
{
	SKTOBJECT("SRecord");
	m_pFields = NULL;
	m_iCols = 0;
}

SRecord::SRecord(int iCols)
{
	SKTOBJECT("SRecord");
	m_pFields = NULL;
	SetColumns(iCols);
}

SRecord::~SRecord()
{
	clear();
}

//设置列数
void SRecord::SetColumns(int iCols)
{
	m_iCols = iCols;
	if(iCols>0)
	{
		m_pFields = new SString[iCols];
	}
}

void SRecord::clear()
{
	if(m_pFields != NULL)
	{
		delete[] m_pFields;
		m_pFields = NULL;
		m_iCols = 0;
	}
}

//取第几个字段的值，iCol从0开始
SString SRecord::GetValue(int iCol )
{
	if(m_pFields == NULL || iCol>=m_iCols || iCol < 0)
		return "";
	return m_pFields[iCol];
}

//设置第几个字段的值，iCol从0开始
bool SRecord::SetValue(int iCol,SString val)
{
	if(m_pFields == NULL || iCol>=m_iCols || iCol < 0)
		return false;
	m_pFields[iCol] = val;
	return true;
}

SRecordset::SRecordset()
{
	SKTOBJECT("SRecordset");
	m_RecordArray.setAutoDelete(true);
}

SRecordset::SRecordset(int iCols)
{
	SKTOBJECT("SRecordset");
	m_RecordArray.setAutoDelete(true);
	if(iCols>0)
	{
		m_ColNames.SetColumns(iCols);
	}
}

SRecordset::~SRecordset()
{
	clear();
}

void SRecordset::clear()
{
	m_RecordArray.clear();
	m_ColNames.clear();
}

//插入记录
void SRecordset::SetSize(int iRows,int iCols)
{
	m_ColNames.SetColumns(iCols);
	m_RecordArray.setSize(iRows);
	for(int i=0;i<iRows;i++)
	{
		m_RecordArray.setAt(i,new SRecord(iCols));
	}
}

//取记录
SRecord* SRecordset::GetRecord(int iRow)
{
	return m_RecordArray.getAt(iRow);
}

SString SRecordset::GetValue(int iRow,int iCol)
{
	SRecord *p = GetRecord(iRow);
	if(p == NULL)
		return "";
	return p->GetValue(iCol);
}

bool SRecordset::SetValue(int iRow,int iCol,SString val)
{
	SRecord *p = GetRecord(iRow);
	if(p == NULL)
		return false;
	return p->SetValue(iCol,val);
}

SDatabase::SDatabase()
{
	SKTOBJECT("SDatabase");
	m_Status = UNCONNECT;
}

SDatabase::~SDatabase()
{
	DisConnect();
}

SString SDatabase::SelectInto(SString sql,int col/*=0*/)	
{
	SRecordset rs;
	if(Retrieve(sql,rs) > 0)
	{
		return rs.GetRecord(0)->GetValue(col);
	}
	return "";
}

bool SDatabase::Reconnect()
{
	DisConnect();
	if(Connect())
	{
		SetStatus(SDatabase::IDLE);
		return true;
	}
	else
	{
		SetStatus(SDatabase::DBERROR);
		return false;
	}		
}

int SDatabase::SelectIntoI(SString sql,int col/*=0*/)
{
	SString s = SelectInto(sql,col);
	return s.toInt();
}

float SDatabase::SelectIntoF(SString sql,int col/*=0*/)
{
	SString s = SelectInto(sql,col);
	return s.toFloat();
}

