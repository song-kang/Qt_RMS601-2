/**
 *
 * 文 件 名 : SBuffer.cpp
 * 创建日期 : 2009-8-4 10:56
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: 2011/08/31 08:43:33 $
 * 当前版本 : $Revision: 1.1 $
 * 功能描述 : 可多用户读取的不定长内存缓冲区类
 * 修改记录 : 
 *            $Log: SBuffer.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:05  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/01/17 01:24:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2010/12/23 01:38:17  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/12/20 08:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/10/29 08:48:41  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:06  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *
 **/


#include "SBuffer.h"


SBuffer::SBuffer()
{
	m_pBuffer = NULL;
	m_ReaderFlag = 0;
	memset(m_iaReaderPos,0,sizeof(m_iaReaderPos));
	m_bShared = true;
}

SBuffer::SBuffer(int iBufferSize)
{
	m_pBuffer = NULL;
	m_ReaderFlag = 0;
	memset(m_iaReaderPos,0,sizeof(m_iaReaderPos));
	init(iBufferSize);
}

SBuffer::~SBuffer()
{
	if(m_pBuffer != NULL)
		delete[] m_pBuffer;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  初始化缓冲区
// 作    者:  邵凯田
// 创建时间:  2009-8-4 11:00
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SBuffer::init(int iBufferSize)
{
	if(m_pBuffer != NULL)
		delete[] m_pBuffer;
	m_pBuffer = new BYTE[iBufferSize];
	if(m_pBuffer == NULL)
	{
		printf("@@@ ERROR @@@ Memory out! SBuffer::Init(%d)",iBufferSize);
		return false;
	}
	m_iBufferSize = iBufferSize;
	m_iWritePoint = 0;
	//数据区为空，所有空间作为空闲区
	stuBufferHeader *pHead = (stuBufferHeader*)m_pBuffer;
#ifdef YS_BUFFER_DEBUG
	pHead->flag = 0xeb905372;
#endif
	pHead->iLength = 0;
	pHead->iFreeLength = m_iBufferSize-sizeof(stuBufferHeader);
	return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取空闲的只读用户，没有空闲返回BUFFER_READER_NONE
// 作    者:  邵凯田
// 创建时间:  2008-9-2 17:26
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
BUFFER_READER_LIST SBuffer::getIdleReader()
{
	DWORD d;
	static bool bDoing = false;//用于简单互拆
	while(bDoing);
	bDoing = true;
	for(int i=0;i<C_BUFFER_READER_COUNT;i++)
	{
		d = m_ReaderFlag;
		d = d >> i;
		if((d&1) == 0)
		{
			//写标识
			DWORD d0=(1 << i);
			m_ReaderFlag |= d0;
			m_iaReaderPos[i] = m_iWritePoint;//向指定用户写入当前数据的位置
			bDoing = false;
			return (BUFFER_READER_LIST)i;
		}
	}
	bDoing = false;
	return BUFFER_READER_NONE;
}

void SBuffer::releaseIdleReader(BUFFER_READER_LIST reader)
{
	DWORD d;
	d = ~(1 << reader);
	m_ReaderFlag &= d;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  插入一块新的数据块
// 作    者:  邵凯田
// 创建时间:  2009-8-4 11:12
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SBuffer::append(BYTE *pBuf,int iLen,BYTE *pBuf2,int iLen2)
{
	if(m_bShared)
		m_Lock.lock();
	stuBufferHeader *pNext,*pHead = (stuBufferHeader*)(&m_pBuffer[m_iWritePoint]);
	int iLenFull = iLen + iLen2 + sizeof(stuBufferHeader);
	int lastWritePoint = m_iWritePoint;//保存最初的写位置
	int iNextPoint;
	bool bWriteAll = false;
//printf("+(%d)",m_iWritePoint);
//printf("\n%d->",m_iWritePoint);
	while(1)
	{
		//当前位置的空闲区，是否足够存放当前数据块
		if(/*pHead->iLength == 0 && */pHead->iFreeLength >= iLenFull)
		{
			memcpy((((BYTE*)pHead)+sizeof(stuBufferHeader)), pBuf,iLen);
			if(iLen2 > 0)
				memcpy((((BYTE*)pHead)+sizeof(stuBufferHeader)+iLen), pBuf2,iLen2);
			#ifdef YS_BUFFER_DEBUG
			if(pHead->iLength != 0)
				pHead = pHead;
			#endif
			pHead->iLength = iLen+iLen2;
			pNext = (stuBufferHeader*)(((BYTE*)pHead) + pHead->iLength+sizeof(stuBufferHeader));
			#ifdef YS_BUFFER_DEBUG
			pNext->flag = 0xeb905372;
			#endif
			pNext->iLength = 0;//下一个写位置
			pNext->iFreeLength = pHead->iFreeLength - iLenFull;
			#ifdef YS_BUFFER_DEBUG
			if(pNext->iFreeLength == 0)
				pHead = pHead;
			#endif
			pHead->iFreeLength = 0;
			m_iWritePoint += pHead->iLength+sizeof(stuBufferHeader);
			if(m_iWritePoint >= m_iBufferSize)
				m_iWritePoint = 0;
		}
		else
		{
			//空间不足，需要合并下一个记录
			while(pHead->iFreeLength < iLenFull)
			{
				iNextPoint = m_iWritePoint + pHead->iLength + pHead->iFreeLength + sizeof(stuBufferHeader);
				if(iNextPoint == lastWritePoint)
				{
					//覆盖了全部的缓冲区，所有读位置均已经失效
					bWriteAll = true;
				}
				if(iNextPoint >= m_iBufferSize)
					break;//到达末尾，已经没有后续数据块了
				pNext = (stuBufferHeader*)(&m_pBuffer[iNextPoint]);
				#ifdef YS_BUFFER_DEBUG
				if(pNext->flag != 0xeb905372)
				{
					pNext = pNext;
				}
				#endif
				pHead->iFreeLength += pNext->iLength + pNext->iFreeLength + sizeof(stuBufferHeader);
			}
			if(pHead->iFreeLength < iLenFull)
			{
				//尾部空间不足，需要从头开始存放,先吃掉头部的数据区空间
				printf("#");
				m_iWritePoint = 0;
				pHead = (stuBufferHeader*)(&m_pBuffer[m_iWritePoint]);
				#ifdef YS_BUFFER_DEBUG
				if(pHead->flag != 0xeb905372)
				{
					pNext = pNext;
				}
				#endif
				pHead->iFreeLength += pHead->iLength;
				pHead->iLength = 0;
			}
			

			continue;
		}
		break;
	}



	//当前接点的下一个位置
	iNextPoint = m_iWritePoint + pNext->iLength + pNext->iFreeLength + sizeof(stuBufferHeader);
	if(iNextPoint >= m_iBufferSize)
		iNextPoint = 0;
	#ifdef YS_BUFFER_DEBUG
	pNext = (stuBufferHeader*)(&m_pBuffer[iNextPoint]);
	if(pNext->flag != 0xeb905372 || iNextPoint < 0 || iNextPoint >= m_iBufferSize || pNext->iLength<0 || pNext->iFreeLength < 0)
	{
		pNext = pNext;
	}
	#endif
	if(bWriteAll)
	{
		//是否有读位置处于被覆盖区内
		for(int i=0;i<C_BUFFER_READER_COUNT;i++)
		{
			//准备读位置需要后移
			m_iaReaderPos[i] = iNextPoint;
			#ifdef YS_BUFFER_DEBUG
			stuBufferHeader *p = (stuBufferHeader*)(&m_pBuffer[m_iaReaderPos[i]]);
			if(p->flag != 0xeb905372 || p->iLength<0 || p->iFreeLength < 0)
			{
				p = p;
			}
			#endif
		}
	}
	else if(iNextPoint > lastWritePoint)
	{
		//是否有读位置处于被覆盖区内
		for(int i=0;i<C_BUFFER_READER_COUNT;i++)
		{
			//如果有某个读位置位于当前准备写入位置的，必须将其向下移动
			if(m_iaReaderPos[i] > lastWritePoint && m_iaReaderPos[i] < iNextPoint)
			{
				//准备读位置需要后移
				m_iaReaderPos[i] = iNextPoint;
			}
			#ifdef YS_BUFFER_DEBUG
			stuBufferHeader *p = (stuBufferHeader*)(&m_pBuffer[m_iaReaderPos[i]]);
			if(p->flag != 0xeb905372 || p->iLength<0 || p->iFreeLength < 0)
			{
				p = p;
			}
			#endif
		}
	}
	else
	{
		//是否有读位置处于被覆盖区内
		for(int i=0;i<C_BUFFER_READER_COUNT;i++)
		{
			//如果有某个读位置位于当前准备写入位置的，必须将其向下移动
			if(m_iaReaderPos[i] > lastWritePoint || m_iaReaderPos[i] < iNextPoint)
			{
				//准备读位置需要后移
				m_iaReaderPos[i] = iNextPoint;
			}
			#ifdef YS_BUFFER_DEBUG
			stuBufferHeader *p = (stuBufferHeader*)(&m_pBuffer[m_iaReaderPos[i]]);
			if(p->flag != 0xeb905372 || p->iLength<0 || p->iFreeLength < 0)
			{
				p = p;
			}
			#endif
		}
	}

	if(m_bShared)
		m_Lock.unlock();

	return true;
}

bool SBuffer::getNextData(BUFFER_READER_LIST reader,BYTE* pBuffer,int &iLen)
{
	//######添加异常判断(后添加)
	if (reader==BUFFER_READER_NONE)
	{
		return false;
	}
	if(m_bShared)
		m_Lock.lock();
	int pos = m_iaReaderPos[reader];
	stuBufferHeader *p = (stuBufferHeader*)(&m_pBuffer[pos]);
	#ifdef YS_BUFFER_DEBUG
	if(p->flag != 0xeb905372 || p->iLength<0 || p->iFreeLength < 0)
	{
		p = p;
	}
	#endif
	if(pos == m_iWritePoint)
	{
		//缺少数据
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}
	while(p->iLength == 0)
	{
		if(pos == m_iWritePoint)
		{
			//缺少数据
			if(m_bShared)
				m_Lock.unlock();
			return false;
		}
		//直接跳过无数据，且不等于写位置的数据块
		pos += (p->iFreeLength+p->iLength+sizeof(stuBufferHeader));
		if(pos >= m_iBufferSize)
		{
			p = (stuBufferHeader*)m_pBuffer;
			#ifdef YS_BUFFER_DEBUG
			if(p->flag != 0xeb905372 || p->iLength<0 || p->iFreeLength < 0)
			{
				p = p;
			}
			#endif
			pos = 0;
		}
		else
		{
			p = (stuBufferHeader*)(&m_pBuffer[pos]);
			#ifdef YS_BUFFER_DEBUG
			if(p->flag != 0xeb905372 || p->iLength<0 || p->iFreeLength < 0)
			{
				p = p;
			}
			#endif
		}
	}
	if(p->iLength > iLen)
	{
		//当前缓冲区放不下
		iLen = p->iLength;
		pos += p->iLength + p->iFreeLength + sizeof(stuBufferHeader);
		if(pos >= m_iBufferSize)
			pos = 0;
		m_iaReaderPos[reader] = pos;
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}
	iLen = p->iLength;
	memcpy(pBuffer,&m_pBuffer[pos+sizeof(stuBufferHeader)],iLen);
	pos += p->iLength + p->iFreeLength + sizeof(stuBufferHeader);
	if(pos >= m_iBufferSize)
		pos = 0;
	m_iaReaderPos[reader] = pos;
	#ifdef YS_BUFFER_DEBUG
	p = (stuBufferHeader*)(&m_pBuffer[pos]);
	if(p->flag != 0xeb905372 || p->iLength<0 || p->iFreeLength < 0)
	{
		p = p;
	}
	#endif
	if(m_bShared)
		m_Lock.unlock();
	return true;

}

////////////////////////////////////////////////////////////////////////
// 描    述:  清除所有的数据
// 作    者:  邵凯田
// 创建时间:  2010-10-29 16:04
// 参数说明:  
// 返 回 值:  
//////////////////////////////////////////////////////////////////////////
bool SBuffer::clear()
{
	if(m_bShared)
		m_Lock.lock();
	if(m_pBuffer == NULL)
	{
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}
	//清除注册用户

	m_ReaderFlag = 0;
	memset(m_iaReaderPos,0,sizeof(m_iaReaderPos));
	m_iWritePoint = 0;
	//数据区为空，所有空间作为空闲区
	stuBufferHeader *pHead = (stuBufferHeader*)m_pBuffer;
	#ifdef YS_BUFFER_DEBUG
		pHead->flag = 0xeb905372;
	#endif
	pHead->iLength = 0;
	pHead->iFreeLength = m_iBufferSize-sizeof(stuBufferHeader);

	if(m_bShared)
		m_Lock.unlock();
	return true;
}
