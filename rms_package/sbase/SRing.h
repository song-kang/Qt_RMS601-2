/**
 *
 * 文 件 名 : SRing.h
 * 创建日期 : 2008-05-23
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:34 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 内存环模板
 * 修改记录 :
 *            $Log: SRing.h,v $
 *            Revision 1.1  2011/08/31 08:43:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/01/17 01:24:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.7  2009/03/10 07:44:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.6  2009/01/31 06:37:34  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2009/01/06 09:58:18  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2008/09/11 05:17:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2008/09/05 06:32:31  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/09/02 10:59:10  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:03  shaokt
 *            *** empty log message ***
 *
 *
 **/

#if !defined(AFX_SKTRING_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
#define AFX_SKTRING_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_

#include "SApi.h"

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef NULL
#define NULL 0
#endif

//最多多少个只读用户
#define C_READER_COUNT 10

//对内存环执行读操作的用户
enum RING_READER_LIST
{
	RING_READER1 = 0,
	RING_READER2 = 1,
	RING_READER3 = 2,
	RING_READER4 = 3,
	RING_READER5 = 4,
	RING_READER6 = 5,
	RING_READER7 = 6,
	RING_READER8 = 7,
	RING_READER9 = 8,
	RING_READER10 = 9,

	RING_READER_NONE = 199
};

template <class type>
class SRing
{
public:
	SRing(int iSize=0)
	{
		m_iWritePos = -1;
		m_pObjects = NULL;
		m_ReaderFlag = 0;
		init(iSize);
		m_Flag = 0;
	};
    
	virtual ~SRing()
	{
		if(m_pObjects != NULL)
			delete[] m_pObjects;
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  初始化内存环大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:44
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	void init(int iSize)
	{
		if(m_pObjects != NULL && m_iSize != iSize)
		{
			delete[] m_pObjects;
			m_pObjects = NULL;
		}
		m_iSize		= iSize;
		if(iSize == 0)
		{
			if(m_pObjects != NULL)
				delete[] m_pObjects;
			m_pObjects = NULL;
		}
		else if(m_pObjects == NULL)
		{
			m_pObjects	= new type[iSize];
			memset(m_pObjects,0,sizeof(type)*iSize);
		}
		m_iWritePos = -1;//当前最后一个数据元素的位置
		for(int i=0;i<C_READER_COUNT;i++)
		{
			m_iaReaderPos[i] = -1;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取空闲的只读用户，没有空闲返回RING_READER_NONE
	// 作    者:  邵凯田
	// 创建时间:  2008-9-2 17:26
	// 参数说明:  void
	// 返 回 值:  RING_READER_LIST
	//////////////////////////////////////////////////////////////////////////
	RING_READER_LIST getIdleReader()
	{
		DWORD d;
		static bool bDoing = false;//用于简单互拆
		while(bDoing);
		bDoing = true;
		for(int i=0;i<C_READER_COUNT;i++)
		{
			d = m_ReaderFlag;
			d = d >> i;
			if((d&1) == 0)
			{
				//写标识
				DWORD d0=(1 << i);
				m_ReaderFlag |= d0;
				bDoing = false;
				return (RING_READER_LIST)i;
			}
		}
		bDoing = false;
		return RING_READER_NONE;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  释放指定的读用户
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:44
	// 参数说明:  @reader表示读用户
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void releaseIdleReader(RING_READER_LIST reader)
	{
		DWORD d;
		d = ~(1 << reader);
		m_ReaderFlag &= d;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  移动到最后的位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:44
	// 参数说明:  @reader读用户
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void gotoLastPos(RING_READER_LIST reader)
	{
		m_iaReaderPos[reader] = m_iWritePos;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  提取缓冲区信息
	// 作    者:  邵凯田
	// 创建时间:  2012-2-24 10:45
	// 参数说明:  @pBuffer为缓冲区地址
	//            @iItemBytes为一个元素所占的字节数
	//            @iAllItems为元素总个数
	//            @piLastItemPos为最后一个元素的位置
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void getBufferInfo(const BYTE* &pBuffer,int &iItemBytes,int &iAllItems,const int* &piLastItemPos)
	{
		pBuffer = (BYTE*)m_pObjects;
		iItemBytes = sizeof(type);
		iAllItems = m_iSize;
		piLastItemPos = &m_iWritePos;
	}

	void P()
	{return;
		while(m_Flag != 0)
		{
#ifdef WIN32
			Sleep(1);
#else
			usleep(1);
#endif
		}
		m_Flag ++;
	}
	void V()
	{return;
		m_Flag--;
	}
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取下一个准备写入位置的对象指定
	// 作    者:  邵凯田
	// 创建时间:  2008-5-23
	// 参数说明:  void
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNextObjForWrite()
	{
		P();
		int pos;
		if(m_iWritePos == m_iSize-1)
			pos = 0;//折回
		else
			pos = m_iWritePos + 1;
		V();
		return &(m_pObjects[pos]);
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取最新的一个对象
	// 作    者:  邵凯田
	// 创建时间:  2008-7-30 9:37
	// 参数说明:  void
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getLastObj()
	{
		if(m_iWritePos < 0)
			return NULL;
		else
			return &(m_pObjects[m_iWritePos]);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  取前给定对象的前一个对象
	// 作    者:  邵凯田
	// 创建时间:  2008-7-30 12:22
	// 参数说明:  @pCurrObj参考对象
	//            @iTimes表示步长
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getPrevObj(type* pCurrObj,int iTimes=1)
	{
		pCurrObj  -= iTimes;
		
		if(pCurrObj<m_pObjects)
			pCurrObj = (&(m_pObjects[m_iSize-1]))-(m_pObjects-pCurrObj);
		return pCurrObj;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取给定的对象的后一个对象
	// 作    者:  邵凯田
	// 创建时间:  2009-3-4 21:27
	// 参数说明:  @pCurrObj参考对象
	//            @iTimes表示步长
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNextObj(type* pCurrObj,int iTimes=1)
	{
		pCurrObj += iTimes;
		if(pCurrObj < m_pObjects)
			pCurrObj = m_pObjects;
		else if(pCurrObj > &(m_pObjects[m_iSize-1]))
			pCurrObj = m_pObjects+(pCurrObj - &(m_pObjects[m_iSize-1]));
		return pCurrObj;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取给定的对象的后一个对象,直至当前写位置
	// 作    者:  邵凯田
	// 创建时间:  2009-3-4 21:27
	// 参数说明:  @iCurrPos指定位置,从0开始
	// 返 回 值:  返回新位置
	//////////////////////////////////////////////////////////////////////////
	int getNextPosUntilWrite(int iCurrPos)
	{
		iCurrPos ++;
		if(iCurrPos >= m_iSize)
			iCurrPos = 0;
		if((m_iWritePos == 0 && iCurrPos == m_iSize) || iCurrPos == m_iWritePos+1)
			return -1;//没有新的记录
		return iCurrPos;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将写指针移动到下一个对象位置
	// 作    者:  邵凯田
	// 创建时间:  2008-5-23
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void moveNext()
	{
		P();
		if(m_iWritePos == m_iSize-1)
			m_iWritePos = 0;//折回
		else
			m_iWritePos ++;
		V();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将写指针移动到下一个对象位置
	// 作    者:  邵凯田
	// 创建时间:  2008-5-23
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void operator++()
	{
		moveNext();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定用户的读位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:47
	// 参数说明:  @reader读用户
	// 返 回 值:  位置
	//////////////////////////////////////////////////////////////////////////
	int GetReaderPos(RING_READER_LIST reader)
	{
		return m_iaReaderPos[reader];
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取最新的对应
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:48
	// 参数说明:  void
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNewestObj()
	{
		if(m_iWritePos < 0)
			return NULL;
		if(m_iWritePos == 0)
			return &m_pObjects[m_iSize-1];
		else
			return &m_pObjects[m_iWritePos-1];
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取最新的对应的位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:48
	// 参数说明:  void
	// 返 回 值:  位置
	//////////////////////////////////////////////////////////////////////////
	int getNewestObjPos()
	{
		if(m_iWritePos < 0)
			return -1;
		if(m_iWritePos == 0)
			return m_iSize-1;
		else
			return m_iWritePos-1;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取下一个可读的元素
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:48
	// 参数说明:  @reader读用户
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* getNextObjForRead(RING_READER_LIST reader)
	{
		P();
		type *p = NULL;
		
		if(m_iaReaderPos[reader] != m_iWritePos)
		{
			m_iaReaderPos[reader] ++;
			if(m_iaReaderPos[reader] == m_iSize)
				m_iaReaderPos[reader] = 0;
			p = &m_pObjects[m_iaReaderPos[reader]];
		}
		V();
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取当前读用户的位置
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:49
	// 参数说明:  @reader读用户
	// 返 回 值:  位置
	//////////////////////////////////////////////////////////////////////////
	int getReaderPos(RING_READER_LIST reader)
	{
		return m_iaReaderPos[reader];
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  读取当前读用户的位置
	// 作    者:  邵凯田
	// 创建时间:  2009-1-14 18:07
	// 参数说明:  @reader读用户
	// 返 回 值:  位置
	//////////////////////////////////////////////////////////////////////////
	int getCurrentReaderPos(RING_READER_LIST reader)
	{
		return m_iaReaderPos[reader];
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置当前读用户的位置
	// 作    者:  邵凯田
	// 创建时间:  2009-1-14 18:07
	// 参数说明:  @reader读用户
	//            @pos新位置
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setCurrentReaderPos(RING_READER_LIST reader, int pos)
	{
		m_iaReaderPos[reader] = pos;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定读用户的下一个元素
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:50
	// 参数说明:  @reader读用户
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type * operator[](RING_READER_LIST reader)
	{
		return getNextObjForRead(reader);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取总数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:50
	// 参数说明:  void
	// 返 回 值:  数量
	//////////////////////////////////////////////////////////////////////////
	int GetSize()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第id个元素的指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 22:51
	// 参数说明:  id为序号，从0开始编号
	// 返 回 值:  type*
	//////////////////////////////////////////////////////////////////////////
	type* GetAt(int id)
	{
		if(id < 0 || id >= m_iSize)
			return NULL;
		return &(m_pObjects[id]);
	}

private:
	type *m_pObjects;
	int m_iSize;//元素数目
	int m_iWritePos;//下一个写元素的位置
	int m_iaReaderPos[C_READER_COUNT];//10个只读用户的状态
	DWORD m_ReaderFlag;//只读用户的注册状态，按位表示，0表示未使用，1表示已使用
	int m_Flag;//0表示空闲
};


#endif // !defined(AFX_SKTRING_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
