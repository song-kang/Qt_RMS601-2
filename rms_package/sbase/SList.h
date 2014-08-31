/**
 *
 * 文 件 名	: SList.h
 * 创建日期	: 2006-09
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期	: $Date: 2011/08/31 08:43:33 $
 * 当前版本	: $Revision: 1.1 $
 * 功能描述 : 类QT的链表模板类 SPtrList SValueList SClassList
 * 修改记录 :
 *            $Log: SList.h,v $
 *            Revision 1.1  2011/08/31 08:43:33  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2011/04/26 01:16:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2011/04/26 00:52:37  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2011/04/04 06:27:29  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/03/21 05:14:56  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/02/25 03:12:07  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/09/28 08:14:23  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2010/08/31 07:19:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2010/08/09 03:04:43  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2010/01/22 01:24:25  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2009/09/18 07:46:31  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2009/09/15 02:14:08  shaokt
 *            通迅记录分析装置项目首次提交
 *
 *            Revision 1.3  2009/02/21 15:55:39  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2008/10/06 08:48:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 11:38:28  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2008/09/01 08:58:01  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.5  2008/02/18 01:07:55  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.4  2007/10/29 07:30:23  shaokt
 *            添加对象地址是否为4的倍数的检测函数，为检测ARM下由于pack(1)造成的内存问题
 *
 *            Revision 1.3  2007/09/21 07:53:41  shaokt
 *            基类更新
 *
 *            Revision 1.2  2007/09/11 08:17:52  shaokt
 *            基类更新
 *
 *            Revision 1.1  2007/08/15 03:09:22  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.3  2007/05/28 07:33:59  shaokt
 *            *** empty log message ***
 *
 **/

#if !defined(AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
#define AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_
#include <list>

#include "SLock.h"
#include "SObject.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef ASSERT
#define ASSERT
#endif

//////////////////////////////////////////////////////////////////////////
// 名    称:  SPtrList
// 作    者:  邵凯田
// 创建时间:  2011-11-18 15:57
// 描    述:  动态指针队列模板类
//////////////////////////////////////////////////////////////////////////
template <class type>
class SPtrList
{
public:
	struct NODE
	{
		type *pObj;
		NODE* pNext;
	};
	struct FREENODE
	{
		NODE* p;
		bool bAutoDel;
	};
	SPtrList()
	{
		SKTOBJECT("SPtrList");
		m_pNode = NULL;
		m_iSize  = 0;
		m_bAutoDel = false;
		m_pNode = NULL;
		m_pTail = NULL;
		m_pLock = NULL;
	};
	virtual ~SPtrList()
	{
		clear();
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置自动释放模式（是否在删除节点时释放节点对应的对象）
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:27
	// 参数说明:  @b是否释放对象
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否为线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:29
	// 参数说明:  void
	// 返 回 值:  true表示是线程共享模式，false表示不是共享模式
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_pLock != NULL;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式，当多线程可能并发读写时需要设为true
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:29
	// 参数说明:  @b是否线程共享模式
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) 
	{
		if(b)
		{
			if(m_pLock == NULL)
				m_pLock = new SLock();
		}
		else
		{
			if(m_pLock != NULL)
			{
				delete m_pLock;
				m_pLock = NULL;
			}
		}
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:30
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_pLock != NULL)
			m_pLock->lock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:30
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  将当前队列中的内容复制到list表示的另一个队列中
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:31
	// 参数说明:  @list为目录队列
	// 返 回 值:  返回复制过去的数量
	//////////////////////////////////////////////////////////////////////////
	int copyto(SPtrList<type> &list)
	{
		lock();
		unsigned long  pos;
		type *p = FetchFirst(pos);
		while(p)
		{
			list.append(p);
			p = FetchNext(pos);
		}
		unlock();
		return count();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  启用后台线程删除全部节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:33
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void clearbk()
	{
		lock();
		NODE *p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		unlock();
		FREENODE *pNode = new FREENODE();
		pNode->p = p;
		pNode->bAutoDel = m_bAutoDel;
	#ifdef WIN32
		HANDLE h;
		h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFree, (LPVOID)pNode, 0, 0);
		CloseHandle(h);
	#else
		pthread_t t;
		pthread_create(&t, NULL, ThreadFree,(void*) pNode);
	#endif	
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除全部节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:34
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void clear()
	{
		lock();
		NODE *next,*p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		while(p)
		{
			next = p->pNext;
			if(m_bAutoDel)
				delete p->pObj;
			delete p;
			p = next;
		}
		unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定指针的对象
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:34
	// 参数说明:  @pObj为待删除的对象指针
	// 返 回 值:  true表示删除成功，false失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(type *pObj)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		while(p)
		{
			if(p->pObj == pObj)
			{
				if(prev == NULL)//first?
				{
					m_pNode = p->pNext;
					if(p == m_pTail)
						m_pTail = m_pNode;
				}
				else
				{
					prev->pNext = p->pNext;
					if(p == m_pTail)
						m_pTail = prev;
				}
				m_iSize --;
				if(m_bAutoDel)
					delete p->pObj;
				delete p;
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  指定的对象是否存在
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:37
	// 参数说明:  @pObj为对象的指针
	// 返 回 值:  true表示此对象存在，false表示不存在
	//////////////////////////////////////////////////////////////////////////
	bool exist(type *pObj)
	{
		return position(pObj) >= 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定对象在队列中的位置,从0开始编号
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:44
	// 参数说明:  @pObj为对象的指针
	// 返 回 值:  >0为对象位置，-1表示对象不存在
	//////////////////////////////////////////////////////////////////////////
	int position(type *pObj)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		int i=-1;
		while(p)
		{
			i++;
			if(p->pObj == pObj)
			{
				unlock();
				return i;
			}
			prev = p;
			p = p->pNext;
		}
		unlock();
		return -1;
	}
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  替换指定位置的对象为一个新的对象指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:48
	// 参数说明:  @i表示队列中的位置，从0开始编号
	//            @pObj表示新的对象指针
	//            @bRemoveOld表示是否释放将被替换的对象，仅当自动释放模式时有效
	// 返 回 值:  true表示替换成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool replace(int i, type*pObj,bool bRemoveOld=false)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(m_bAutoDel && bRemoveOld && p->pObj != NULL)
				{
					delete p->pObj;
				}
				p->pObj = pObj;
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指针位置i的节点，i从0开始编号
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:50
	// 参数说明:  @i待删除的节点位置，从0开始
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(int i)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(prev == NULL)//first?
				{
					m_pNode = p->pNext;
					if(p == m_pTail)
						m_pTail = m_pNode;
				}
				else
				{
					prev->pNext = p->pNext;
					if(p == m_pTail)
						m_pTail = prev;
				}
				m_iSize --;
				if(m_bAutoDel)
					delete p->pObj;
				delete p;
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除全部节点，不推荐使用，建议使用clear函数
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:04
	// 参数说明:  void
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool removeAll()
	{
		while(count()>0)
			remove(0);
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取队列的总节点数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:05
	// 参数说明:  void
	// 返 回 值:  节点数量
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列是否为空
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:06
	// 参数说明:  void
	// 返 回 值:  true表示队列为空，false表示队列非空
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  向队列末尾添加一个新的节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:06
	// 参数说明:  @pObj为新节点对应的对象
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool append(type *pObj)
	{
		lock();
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		pNew->pObj = pObj;
		pNew->pNext = NULL;
		if(p == NULL)
		{
			m_pTail = m_pNode = pNew;
		}
		else if(m_pTail != NULL)
		{
			//快速插入到尾节点
			pNew->pNext = m_pTail->pNext;
			m_pTail->pNext = pNew;
			m_pTail = pNew;
		}
		else
		{
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
			m_pTail = pNew;
		}
		m_iSize ++;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  在指定节点前插入新节点
	// 作    者:  邵凯田
	// 创建时间:  2009-2-16 9:16
	// 参数说明:  pNode为参考节点，pObj为新节点
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertBefore(type* pNode,type *pObj)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		NODE *pNew = new NODE;
		pNew->pObj = pObj;
		while(p != NULL)
		{
			if(p->pObj == pNode)
			{
				pNew->pNext = p;
				if(prev == NULL)//first?
				{
					if(m_pNode == NULL)
						m_pTail = pNew;
					m_pNode = pNew;
				}
				else
				{
					prev->pNext = pNew;
				}
				m_iSize ++;
				unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  在指定节点后插入新节点
	// 作    者:  邵凯田
	// 创建时间:  2009-2-16 9:16
	// 参数说明:  pNode为参考节点，pObj为新节点
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertAfter(type* pNode,type *pObj)
	{
		lock();
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		pNew->pObj = pObj;
		while(p != NULL && p->pNext != NULL)
		{
			if(p->pObj == pNode)
			{
				pNew->pNext = p->pNext;
				p->pNext = pNew;
				if(p == m_pTail)
					m_pTail = pNew;
				m_iSize ++;
				unlock();
				return true;
			}
			p = p->pNext;
		}
		unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定位置的节点对象
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:13
	// 参数说明:  @i表示待取对象在队列中的位置，i从0开始编号
	// 返 回 值:  对象指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * at(int i)
	{
		lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				unlock();
				return p->pObj;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定位置的节点对象
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:13
	// 参数说明:  @i表示待取对象在队列中的位置，i从0开始编号
	// 返 回 值:  对象指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int i)
	{
		return at(i);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  移动到节点的开始位置，此函数与FetchNext一起使用，
	//            如果是线程不安全的，需要在遍历前后加解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:14
	// 参数说明:  @pos表示引用返回的位置信息，为下次Fetch时使用
	// 返 回 值:  对象指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type* FetchFirst(unsigned long &pos)
	{
		if(m_pNode == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)m_pNode;
			return m_pNode->pObj;
		}
	}
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  移动到节点的指定位置，id从0开始，此函数与FetchNext一起使用，
	//            如果是线程不安全的，需要在遍历前后加解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:14
	// 参数说明:  @id为需要移动到的位置，id从0开始
	//            @pos表示引用返回的位置信息，为下次Fetch时使用
	// 返 回 值:  对象指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type* FetchToIndex(int id,unsigned long &pos)
	{
		int cnt = 0;
		type* p = FetchFirst(pos);
		while(p != NULL && cnt < id)
		{
			p = FetchNext(pos);
			cnt++;
		}
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  移动下次一个节点位置，此函数与FetchFirst或一起使用，
	//            如果是线程不安全的，需要在遍历前后加解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:14
	// 参数说明:  @id为需要移动到的位置，id从0开始
	//            @pos表示引用返回的位置信息，为下次Fetch时使用
	// 返 回 值:  对象指针，NULL表示到达底部，此时pos值不会回0，下次有新记录插入后，
	//            再次使用FetchNext(保持pos不变)将仍然能得到下一条记录
	//////////////////////////////////////////////////////////////////////////
	type* FetchNext(unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		if(p == NULL)
			return NULL;
		p = p->pNext;
		if(p == NULL)
			return NULL;
		else
		{
			pos = (unsigned long)p;
			return p->pObj;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取最后一个节点的对象指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:45
	// 参数说明:  void
	// 返 回 值:  对象指针
	//////////////////////////////////////////////////////////////////////////
	type* getTail()
	{
		if(m_pTail == NULL)
			return NULL;
		return m_pTail->pObj;
	}

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  将当前列表转换为对象数组
	// 作    者:  邵凯田
	// 创建时间:  2014-5-19 19:26
	// 参数说明:  @obj_array表示待存放数组的指针，由调用都开辟空间，并确保可以存放iMaxRows条记录
	//         :  @iSize本次读取的数量，引用返回
	//         :  @iMaxRows表示当前缓冲区的最大记录数
	//         :  @pos表示引用返回的位置信息，为下次toNextArray时使用，初次使用时为0
	// 返 回 值:  =0表示没有取到任何记录也没有未取记录，>0表示还有未取记录
	//////////////////////////////////////////////////////////////////////////
	int toNextArray(type* obj_array,int &iSize,int iMaxRows,unsigned long &pos)
	{
		NODE* p = (NODE*)pos;
		iSize = 0;
		if(p == NULL)
			p = m_pNode;
		if(p == NULL)
			return 0;
		while(p != NULL && iSize < iMaxRows)
		{
			memcpy(&obj_array[iSize++],p->pObj,sizeof(type));
			pos = (unsigned long)p->pNext;
			p = p->pNext;
		}
		if(p == NULL)
			return 0;
		return 1;
	}

	NODE *m_pNode,*m_pTail;
private:

	////////////////////////////////////////////////////////////////////////
	// 描    述:  后台释放节点的线程，当节点数量太多时，可使用线程释放
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 14:32
	// 参数说明:  this
	// 返 回 值:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadFree(void *lp)
	{
	#ifndef WIN32
		pthread_detach(pthread_self());
	#endif
		FREENODE *pNode = (FREENODE*)lp;
		NODE *next,*p = pNode->p;
#ifdef WIN32
		try
		{
#endif
			while(p)
			{
				next = p->pNext;
				if(pNode->bAutoDel)
					delete p->pObj;
				delete p;
				p = next;
			}
#ifdef WIN32
		}
		catch(...)
		{
			ASSERT(0);
		}
		try
		{
#endif
			delete pNode;
#ifdef WIN32
		}
		catch(...)
		{
			ASSERT(0);
		}
#endif
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 15:46
	// 参数说明:  @pNew新节点
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool addNode(NODE* pNew)
	{
		pNew->pNext = NULL;
		if(!m_pNode)
			m_pNode = pNew;
		else
		{
			NODE *p=m_pNode;
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		return true;
	}


	int m_iSize;
	bool m_bAutoDel;
	//bool m_bShared;//是否线程共享，如共享需要信号量互拆
	SLock *m_pLock;

};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SValueList
// 作    者:  邵凯田
// 创建时间:  2011-11-18 15:57
// 描    述:  动态值模队列板类
//////////////////////////////////////////////////////////////////////////
template <typename T>
class SValueList
{
public:
	typedef T* iterator;
	struct iterator2
	{
		void *pObj;
	};
	struct NODE
	{
		T obj;
		NODE* pNext;
	};
	SValueList()
	{
		SKTOBJECT("SValueList");
		m_pNode = NULL;
		m_iSize  = 0;
	};
	virtual ~SValueList()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清除队列
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:01
	// 参数说明:  void
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		NODE *next,*p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		while(p)
		{
			next = p->pNext;
			delete p;
			p = next;
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定的节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:02
	// 参数说明:  @pObj表示等删除的对象指针
	// 返 回 值:  true表示删除成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(T *pObj)
	{
		NODE *p=m_pNode,*prev=NULL;
		while(p != NULL && p->pNext != NULL)
		{
			if(&p->obj == pObj)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除第i个节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:03
	// 参数说明:  @i为指针的位置，i从0开始
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(int i)
	{
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除全部节点，不推荐使用，建议使用clear函数替代
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:04
	// 参数说明:  void
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool removeAll()
	{
		while(count()>0)
			remove(0);
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取队列的数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:05
	// 参数说明:  void
	// 返 回 值:  总数量
	//////////////////////////////////////////////////////////////////////////
	inline unsigned long count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列是否为空
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:05
	// 参数说明:  void
	// 返 回 值:  true表示为空，false表示非空
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一个新的节点到末尾
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:07
	// 参数说明:  @obj表示新的节点值
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool append(T &obj)
	{
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		memcpy(&pNew->obj, &obj,sizeof(T));
		pNew->pNext = NULL;
		if(p == NULL)
			m_pNode = pNew;
		else
		{
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		m_iSize ++;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  在指定节点前插入新节点
	// 作    者:  邵凯田
	// 创建时间:  2009-2-16 9:16
	// 参数说明:  pNode为参考节点，obj为新节点
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertBefore(T* pNode,T &obj)
	{
		NODE *p=m_pNode,*prev=NULL;
		NODE *pNew = new NODE;
		memcpy(&pNew->obj, &obj,sizeof(T));
		while(p != NULL && p->pNext != NULL)
		{
			if(&p->obj == pNode)
			{
				if(prev == NULL)//first?
				{
					m_pNode = pNew;
					pNew->pNext = p;
				}
				else
				{
					prev->pNext = pNew;
					pNew->pNext = p;
				}
				m_iSize ++;
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  在指定节点后插入新节点
	// 作    者:  邵凯田
	// 创建时间:  2009-2-16 9:16
	// 参数说明:  pNode为参考节点，obj为新节点
	// 返 回 值:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool insertAfter(T* pNode,T &obj)
	{
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		memcpy(&pNew->obj, &obj,sizeof(T));
		while(p != NULL && p->pNext != NULL)
		{
			if(&p->obj == pNode)
			{
				pNew->pNext = p->pNext;
				p->pNext = pNew;
				m_iSize ++;
				return true;
			}
			p = p->pNext;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定位置的节点值指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:07
	// 参数说明:  @i表示指定的位置，i从0开始编号
	// 返 回 值:  type*值指针
	//////////////////////////////////////////////////////////////////////////
	iterator at(int i)
	{
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				return &p->obj;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取指定位置的节点值指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:07
	// 参数说明:  @i表示指定的位置，i从0开始编号
	// 返 回 值:  type*值指针
	//////////////////////////////////////////////////////////////////////////
	iterator operator[](int i)
	{
		return at(i);
	}

private:
	NODE *m_pNode;
	int m_iSize;

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:08
	// 参数说明:  @pNew新节点
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool addNode(NODE* pNew)
	{
		pNew->pNext = NULL;
		if(!m_pNode)
			m_pNode = pNew;
		else
		{
			NODE *p=m_pNode;
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SClassList
// 作    者:  邵凯田
// 创建时间:  2011-11-18 15:57
// 描    述:  动态对象队列模板类
//////////////////////////////////////////////////////////////////////////
template <class T>
class SClassList
{
public:
	typedef T* iterator;
	struct iterator2
	{
		void *pObj;
	};
	struct NODE
	{
		T obj;
		NODE* pNext;
	};
	SClassList()
	{
		SKTOBJECT("SClassList");
		m_pNode = NULL;
		m_iSize  = 0;
		m_bShared = false;
	};
	virtual ~SClassList()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:09
	// 参数说明:  @b表示是否设为线程共享模式
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b)
	{
		m_bShared = b;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清除队列
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:10
	// 参数说明:  void
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *next,*p=m_pNode;
		m_pNode = NULL;
		m_iSize  = 0;
		while(p)
		{
			next = p->pNext;
			delete p;
			p = next;
		}
		if(m_bShared)
			m_Lock.unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除队列中指定的指针的节点值
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:10
	// 参数说明:  @pObj为指针
	// 返 回 值:  true表示删除成功，false表示删除失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(T *pObj)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode,*prev=NULL;
		if(p == NULL)
		{
			return false;
		}
		if(p->pNext == NULL)
		{
			m_pNode = NULL;
			m_iSize --;
			delete p;
			if(m_bShared)
				m_Lock.unlock();
			return true;
		}
		while(p->pNext)
		{
			if(&(p->obj) == pObj)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				if(m_bShared)
					m_Lock.unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
		}
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除指定位置的节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:11
	// 参数说明:  @i为待删除的位置,i从0开始编号
	// 返 回 值:  true表示删除成功，false失败
	//////////////////////////////////////////////////////////////////////////
	bool remove(int i)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(prev == NULL)//first?
					m_pNode = p->pNext;
				else
					prev->pNext = p->pNext;
				m_iSize --;
				delete p;
				if(m_bShared)
					m_Lock.unlock();
				return true;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		if(m_bShared)
			m_Lock.unlock();
		return false;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取队列数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:12
	// 参数说明:  void
	// 返 回 值:  数量
	//////////////////////////////////////////////////////////////////////////
	inline unsigned long count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  队列是否为空
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:12
	// 参数说明:  void
	// 返 回 值:  true队列为空，false为非空
	//////////////////////////////////////////////////////////////////////////
	bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  对队列末尾添加一个新的节点值
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:13
	// 参数说明:  @obj为新的节点值
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool append(const T &obj)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode;
		NODE *pNew = new NODE;
		pNew->obj = obj;
		//memcpy(&pNew->obj, &obj,sizeof(T));
		pNew->pNext = NULL;
		if(p == NULL)
			m_pNode = pNew;
		else
		{
			while(p->pNext)
				p = p->pNext;
			p->pNext = pNew;
		}
		m_iSize ++;
		if(m_bShared)
			m_Lock.unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个节点的值指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:13
	// 参数说明:  @i表示指定的位置，i从0开始编号
	// 返 回 值:  type*值指针,NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	iterator at(int i)
	{
		if(m_bShared)
			m_Lock.lock();
		NODE *p=m_pNode,*prev=NULL;
		int cnt=0;
		while(p)
		{
			if(i == cnt)
			{
				if(m_bShared)
					m_Lock.unlock();
				return &p->obj;
			}
			prev = p;
			p = p->pNext;
			cnt ++;
		}
		if(m_bShared)
			m_Lock.unlock();
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个节点的值指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:13
	// 参数说明:  @i表示指定的位置，i从0开始编号
	// 返 回 值:  type*值指针,NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	iterator operator[](int i)
	{
		return at(i);
	}

private:
	NODE *m_pNode;
	int m_iSize;
	bool m_bShared;//是否线程共享，如共享需要信号量互拆
	SLock m_Lock;
};
//#define QPtrList SPtrList
//#define QValueList SValueList
//typedef SPtrList QPtrList;
//typedef SValueList QValueList;


//////////////////////////////////////////////////////////////////////////
// 名    称:  SArray
// 作    者:  邵凯田
// 创建时间:  2011-11-18 15:58
// 描    述:  数组模板类
//////////////////////////////////////////////////////////////////////////
template <class type>
class SArray
{
public:
	SArray()
	{
		SKTOBJECT("SArray");
		m_pObjs = NULL;
		m_iSize  = 0;
		m_bShared = false;
	};
	virtual ~SArray()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否为线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:15
	// 参数说明:  void
	// 返 回 值:  true为线程共享模式，false不是
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_bShared;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:16
	// 参数说明:  @b为线程共享模式
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) {m_bShared=b;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  数组加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:16
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  数组解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:16
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_bShared)
			m_Lock.unlock();
	}
	
	////////////////////////////////////////////////////////////////////////
	// 描    述:  清除数组全部节点
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:17
	// 参数说明:  void
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		lock();
		if(m_pObjs != NULL)
		{
			delete[] m_pObjs;
			m_pObjs = NULL;
			m_iSize = 0;
		}
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  重设数组大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:17
	// 参数说明:  @iSize为新的数组大小
	// 返 回 值:  成功返回true，失败返回false
	//////////////////////////////////////////////////////////////////////////
	bool setSize(int iSize)
	{
		if(iSize<=0)
		{
			clear();
			return true;
		}
		type *pOld = m_pObjs;
		m_pObjs = new type[iSize];
		memset(m_pObjs,0,sizeof(type)*iSize);
		if(pOld != NULL)
		{
			memcpy(m_pObjs,pOld,(m_iSize>iSize?iSize:m_iSize)*sizeof(type));
			delete[] pOld;
		}
		m_iSize = iSize;
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  表示数组数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:18
	// 参数说明:  void
	// 返 回 值:  数组数量
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  数组是否为空
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:18
	// 参数说明:  void
	// 返 回 值:  true表示数组为空，false表示非空
	//////////////////////////////////////////////////////////////////////////
	bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  添加一个新的元素
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:26
	// 参数说明:  @obj新的数组元素
	// 返 回 值:  返回新数组数量
	//////////////////////////////////////////////////////////////////////////
	int append(type &obj)
	{
		int id = count()+1;
		setSize(id);
		setAt(id,obj);
		return id;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置第i个数组元素
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:27
	// 参数说明:  @i为数组位置，从0开始编号
	//            @obj表示新元素
	// 返 回 值:  true表示设置成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool setAt(int i,type &obj)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return false;
		}
		memcpy(&(m_pObjs[i]), &obj,sizeof(type));
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个数组元素指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:28
	// 参数说明:  @i为数组位置
	// 返 回 值:  指定元素的指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type* getAt(int i)
	{
		lock();
		if(i>=m_iSize || i<0)
			return NULL;
		type *p=&(m_pObjs[i]);
		unlock();
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取数组首元素的指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:29
	// 参数说明:  void
	// 返 回 值:  首元素指针
	//////////////////////////////////////////////////////////////////////////
	type * getData()
	{
		return m_pObjs;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个数组元素指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:28
	// 参数说明:  @i为数组位置
	// 返 回 值:  指定元素的指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * at(int i)
	{
		return getAt(i);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个数组元素指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:28
	// 参数说明:  @i为数组位置
	// 返 回 值:  指定元素的指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int i)
	{
		return at(i);
	}

private:
	type *m_pObjs;
	int m_iSize;
	bool m_bShared;//是否线程共享，如共享需要信号量互拆
	SLock m_Lock;

};

//////////////////////////////////////////////////////////////////////////
// 名    称:  SPtrArray
// 作    者:  邵凯田
// 创建时间:  2011-11-18 15:58
// 描    述:  指针数组模板类
//////////////////////////////////////////////////////////////////////////
template <class type>
class SPtrArray
{
public:
	SPtrArray()
	{
		SKTOBJECT("SPtrArray");
		m_pObjs = NULL;
		m_iSize  = 0;
		m_bAutoDel = false;
		m_bShared = false;
	};
	virtual ~SPtrArray()
	{
		clear();
	};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置是否自动释放
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:33
	// 参数说明:  @b是否自动释放，true为自动释放，false不自动释放
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setAutoDelete(bool b)
	{
		m_bAutoDel = b;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  是否线程共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:35
	// 参数说明:  void
	// 返 回 值:  true表示是线程共享模式，false为非共享模式
	//////////////////////////////////////////////////////////////////////////
	bool isShared() {return m_bShared;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置共享模式
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:35
	// 参数说明:  @b表示待设置的共享模式，true共享，false不共享
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void setShared(bool b) {m_bShared=b;};

	////////////////////////////////////////////////////////////////////////
	// 描    述:  数组加锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:36
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void lock()
	{
		if(m_bShared)
			m_Lock.lock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  数组解锁
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:36
	// 参数说明:  void
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	inline void unlock()
	{
		if(m_bShared)
			m_Lock.unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  清除数组全部内容
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:37
	// 参数说明:  void
	// 返 回 值:  true
	//////////////////////////////////////////////////////////////////////////
	bool clear()
	{
		lock();
		if(m_pObjs != NULL)
		{
			if(m_bAutoDel)
			{
				for(int i=0;i<m_iSize;i++)
				{
					type * pObj = m_pObjs[i];
					if(pObj != NULL)
						delete pObj;
				}
			}
			delete[] m_pObjs;
			m_iSize = 0;
			m_pObjs = NULL;
		}
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  重设数组大小
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:37
	// 参数说明:  @iSize表示数组大小
	// 返 回 值:  true表示重设成功，false表示失败
	//////////////////////////////////////////////////////////////////////////
	bool setSize(int iSize)
	{
		if(iSize<=0)
		{
			clear();
			return true;
		}
		lock();
		type **pOld = m_pObjs;
		char *pBuf = new char[sizeof(type*)*iSize];
		m_pObjs = (type**)pBuf;
		memset(m_pObjs,0,sizeof(type*)*iSize);
		if(pOld != NULL)
		{
			memcpy(m_pObjs,pOld,m_iSize>iSize?iSize:m_iSize);
			delete[] pOld;
		}
		m_iSize = iSize;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取数组元素数量
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:37
	// 参数说明:  void
	// 返 回 值:  数量
	//////////////////////////////////////////////////////////////////////////
	inline int count()
	{
		return m_iSize;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  当前数组是否为空
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:38
	// 参数说明:  void
	// 返 回 值:  true表示数组为空，false表示数组非空
	//////////////////////////////////////////////////////////////////////////
	inline bool isEmpty()
	{
		return m_iSize == 0;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  设置指定位置的指针
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:38
	// 参数说明:  @i为指定的位置
	//            @pObj为新的指针
	// 返 回 值:  true表示设置成功，false表示设置失败
	//////////////////////////////////////////////////////////////////////////
	bool setAt(int i,type *pObj)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return false;
		}
		m_pObjs[i] = pObj;
		unlock();
		return true;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个位置的数组元素
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:39
	// 参数说明:  @i待取元素的位置，i从0开始
	// 返 回 值:  对应位置的元素指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type* getAt(int i)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return NULL;
		}
		type *p=m_pObjs[i];
		unlock();
		return p;
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  删除第i个元素,i从0开始
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:40
	// 参数说明:  @i待删除元素的位置，i从0开始
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void remove(int i)
	{
		lock();
		if(i>=m_iSize || i<0)
		{
			unlock();
			return;
		}
		type *p=m_pObjs[i];
		unlock();
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个位置的数组元素
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:39
	// 参数说明:  @i待取元素的位置，i从0开始
	// 返 回 值:  对应位置的元素指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * at(int i)
	{
		return getAt(i);
	}

	////////////////////////////////////////////////////////////////////////
	// 描    述:  取第i个位置的数组元素
	// 作    者:  邵凯田
	// 创建时间:  2011-11-18 16:39
	// 参数说明:  @i待取元素的位置，i从0开始
	// 返 回 值:  对应位置的元素指针，NULL表示未找到
	//////////////////////////////////////////////////////////////////////////
	type * operator[](int i)
	{
		return at(i);
	}

private:
	type **m_pObjs;
	int m_iSize;
	bool m_bAutoDel;
	bool m_bShared;//是否线程共享，如共享需要信号量互拆
	SLock m_Lock;

};

#endif // !defined(AFX_SKTPTRLIST_H__12695A0C_F4BA_4DF7_9F35_17CD7703DDE1__INCLUDED_)
