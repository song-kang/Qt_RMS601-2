/**
*
* 文 件 名 : SDateTime.cpp
* 创建日期 : 2007-01-01
* 作    者 : 邵凯田(skt001@163.com)
* 修改日期 : $Date: 2011/08/31 08:43:33 $
* 当前版本 : $Revision: 1.1 $
* 功能描述 : 从SDateTime移入
* 修改记录 : 
*            $Log: SDateTime.cpp,v $
*            Revision 1.1  2011/08/31 08:43:33  shaokt
*            *** empty log message ***
*
*            Revision 1.3  2011/04/19 09:10:58  shaokt
*            *** empty log message ***
*
*            Revision 1.2  2011/03/23 01:07:49  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2011/02/25 03:12:07  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2010/08/09 03:04:43  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2009/09/15 02:14:07  shaokt
*            通迅记录分析装置项目首次提交
*
*            Revision 1.7  2009/01/31 06:37:33  shaokt
*            *** empty log message ***
*
*            Revision 1.6  2009/01/06 09:58:18  shaokt
*            *** empty log message ***
*
*            Revision 1.5  2008/12/22 08:54:13  shaokt
*            *** empty log message ***
*
*            Revision 1.4  2008/09/16 05:32:17  shaokt
*            *** empty log message ***
*
*            Revision 1.3  2008/09/12 05:04:48  shaokt
*            *** empty log message ***
*
*            Revision 1.2  2008/09/11 05:17:25  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2008/09/01 11:38:28  shaokt
*            *** empty log message ***
*
*            Revision 1.1  2008/09/01 08:58:00  shaokt
*            *** empty log message ***
*
*
**/

#include "SDateTime.h"
SDate::SDate()
{
	SKTOBJECT("SDate");
	m_year=m_month=m_day=0;
}
SDate::SDate(int y,int m,int d)
{
	SKTOBJECT("SDate");
	m_year = y;
	m_month = m;
	m_day = d;
}
SDate::~SDate()
{
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取年
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:09
// 参数说明:  void
// 返 回 值:  年
//////////////////////////////////////////////////////////////////////////
int SDate::year()		{return m_year;};

////////////////////////////////////////////////////////////////////////
// 描    述:  取月
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:09
// 参数说明:  void
// 返 回 值:  月
//////////////////////////////////////////////////////////////////////////
int SDate::month()		{return m_month;};

////////////////////////////////////////////////////////////////////////
// 描    述:  取日
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:09
// 参数说明:  void
// 返 回 值:  日
//////////////////////////////////////////////////////////////////////////
int SDate::day()		{return m_day;};

////////////////////////////////////////////////////////////////////////
// 描    述:  赋值运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:09
// 参数说明:  @d参考值
// 返 回 值:  this
//////////////////////////////////////////////////////////////////////////
SDate& SDate::operator=(const SDate &d)
{
	memcpy(this, &d, sizeof(d));return *this;
};

////////////////////////////////////////////////////////////////////////
// 描    述:  等于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:10
// 参数说明:  @other表示参考值
// 返 回 值:  true表示与参考值相等，false表示与参考值不等
//////////////////////////////////////////////////////////////////////////
bool SDate::operator==(SDate &other)
{
	if(m_year == other.m_year &&
		m_month == other.m_month &&
		m_day == other.m_day)
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  不等于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:11
// 参数说明:  @other表示参考值
// 返 回 值:  true表示与参考值不等，false表示与参数值相等
//////////////////////////////////////////////////////////////////////////
bool SDate::operator!=(SDate &other)
{
	if(m_year == other.m_year &&
		m_month == other.m_month &&
		m_day == other.m_day)
		return false;
	else
		return true;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前系统日期,静态函数
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:12
// 参数说明:  void
// 返 回 值:  SDate
//////////////////////////////////////////////////////////////////////////
SDate SDate::currentDate()
{
	SDate d;
	struct  tm *t;
	time_t now;
	time(&now);
	//localtime_s(t,&now);
	t = localtime(&now);
	d.m_year = 1900+t->tm_year;
	d.m_month=1+t->tm_mon;
	d.m_day=t->tm_mday;
	return d;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  格式化日期为一个字符串
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:12
// 参数说明:  @fmt表示格式化字符串的格式化串，yyyy表示年,MM表示月,dd表示日
// 返 回 值:  字符串格式的日期
//////////////////////////////////////////////////////////////////////////
SString SDate::toString(SString fmt)//yyyyMMdd
{
	SString s=fmt;
	s.replace("yyyy",toStr("yyyy"));
	s.replace("MM",toStr("MM"));
	s.replace("dd",toStr("dd"));
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  转换指定的字段
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:14
// 参数说明:  @fmt可以选择yyyy或MM或dd分别对应年、月日
// 返 回 值:  格式化后的字符串
//////////////////////////////////////////////////////////////////////////
SString SDate::toStr(SString fmt)
{
	int len=2;
	int i=0;
	if(fmt == "yyyy")
	{
		len=4;
		i=m_year;
	}
	else if(fmt == "MM")
		i=m_month;
	else if(fmt == "dd")
		i=m_day;
	fmt.sprintf("%d",i);
	while(fmt.length() < len)
		fmt = "0"+fmt;
	return fmt;
}

//////////////////////////////////////////////////////////////////////////
// 名    称:  STime
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:21
// 描    述:  时间类
//////////////////////////////////////////////////////////////////////////
STime::STime()
{
	SKTOBJECT("STime");
	m_hour=m_minute=m_second=m_msecond=0;
}
STime::STime(int h,int m,int s/*=0*/, int ms/*=0*/)
{
	SKTOBJECT("STime");
	m_hour=h;
	m_minute=m;
	m_second=s;
	m_msecond=ms;
}
STime::STime(time_t* pt)
{
	SKTOBJECT("STime");
	tm *t;
	t = localtime(pt);
	m_hour=t->tm_hour;
	m_minute=t->tm_min;
	m_second=t->tm_sec;
	m_msecond=0;
}
STime::~STime()
{
};

////////////////////////////////////////////////////////////////////////
// 描    述:  取时
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:23
// 参数说明:  void
// 返 回 值:  时
//////////////////////////////////////////////////////////////////////////
int STime::hour()	{return m_hour;}

////////////////////////////////////////////////////////////////////////
// 描    述:  取分
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:23
// 参数说明:  void
// 返 回 值:  分
//////////////////////////////////////////////////////////////////////////
int STime::minute(){return m_minute;}

////////////////////////////////////////////////////////////////////////
// 描    述:  取秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:23
// 参数说明:  void
// 返 回 值:  秒
//////////////////////////////////////////////////////////////////////////
int STime::second(){return m_second;}

////////////////////////////////////////////////////////////////////////
// 描    述:  取毫秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:23
// 参数说明:  void
// 返 回 值:  秒
//////////////////////////////////////////////////////////////////////////
int STime::msec()	{return m_msecond;}

////////////////////////////////////////////////////////////////////////
// 描    述:  取微秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:23
// 参数说明:  void
// 返 回 值:  微秒
//////////////////////////////////////////////////////////////////////////
int STime::usec()	{return m_usecond;}

////////////////////////////////////////////////////////////////////////
// 描    述:  赋值运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:25
// 参数说明:  @t参考时间对象
// 返 回 值:  this
//////////////////////////////////////////////////////////////////////////
STime& STime::operator=(const STime &t)
{
	memcpy(this, &t, sizeof(t));return *this;
};

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前时间，静态函数
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:25
// 参数说明:  void
// 返 回 值:  STime
//////////////////////////////////////////////////////////////////////////
STime STime::currentTime()
{
	STime tt;
	struct  tm *t;
	time_t now;
	time(&now);
	t = localtime(&now);
	tt.m_hour=t->tm_hour;
	tt.m_minute=t->tm_min;
	tt.m_second=t->tm_sec;

	#ifdef WIN32
		SYSTEMTIME st;
		GetLocalTime(&st); 
		tt.m_msecond=st.wMilliseconds;
	#else
		timeval tv;
		memset(&tv,0,sizeof(tv));
		gettimeofday(&tv,NULL);
		tt.m_msecond = tv.tv_usec/1000;
	#endif
	return tt;
}


//////////////////////////////////////////////////////////////////////////
// 名    称:  SDateTime
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:49
// 描    述:  日期时间类
//////////////////////////////////////////////////////////////////////////
SDateTime::SDateTime()
{
	SKTOBJECT("SDateTime");
};
SDateTime::SDateTime(const SDate &d)
{
	SKTOBJECT("SDateTime");
	m_date = d;
}
SDateTime::SDateTime(const SDate &d, const STime &t)
{
	SKTOBJECT("SDateTime");
	m_date = d;
	m_time = t;
}
SDateTime::SDateTime(const SDateTime &dt)
{
	SKTOBJECT("SDateTime");
	*this = dt;
}
SDateTime::SDateTime(time_t time)
{
	SKTOBJECT("SDateTime");
	struct  tm *t;
	t = localtime(&time);
	if(t == NULL)
		return;
	m_t = time;
	m_date.m_year = 1900+t->tm_year;
	m_date.m_month=1+t->tm_mon;
	m_date.m_day=t->tm_mday;
	m_time.m_hour=t->tm_hour;
	m_time.m_minute=t->tm_min;
	m_time.m_second=t->tm_sec;
	m_time.m_msecond=0;
	m_time.m_usecond=0;
}
	
////////////////////////////////////////////////////////////////////////
// 描    述:  向当前日期时间添加指定天数
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:28
// 参数说明:  @days表示天数，可以为负数
// 返 回 值:  添加后的新对象
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addDays(int days)
{
	return addHours(days*24);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  向当前日期时间添加指定小时
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:28
// 参数说明:  @hours表示小时，可以为负数
// 返 回 值:  添加后的新对象
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addHours(int hours)
{
	return addMinutes(hours*60);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  向当前日期时间添加指定分钟
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:28
// 参数说明:  @minutes表示分钟，可以为负数
// 返 回 值:  添加后的新对象
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addMinutes(int minutes)
{
	return addSecs(minutes*60);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  向当前日期时间添加指定秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:28
// 参数说明:  @secs表示秒，可以为负数
// 返 回 值:  添加后的新对象
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addSecs(int secs)
{
	SDateTime dt=*this;
	struct tm timestart;
	memset(&timestart,0,sizeof(timestart));
	timestart.tm_sec	= dt.m_time.second();
	timestart.tm_min	= dt.m_time.minute();
	timestart.tm_hour = dt.m_time.hour();
	timestart.tm_mday = dt.m_date.day();
	timestart.tm_mon	= dt.m_date.month()-1;
	timestart.tm_year = dt.m_date.year()-1900;
	
	time_t time = mktime(&timestart)+secs;
	struct tm* t = localtime(&time);
	if(t == NULL)//TODO 
		return dt;

	dt.m_t = time;
	dt.m_date.m_year = 1900+t->tm_year;
	dt.m_date.m_month=1+t->tm_mon;
	dt.m_date.m_day=t->tm_mday;
	dt.m_time.m_hour=t->tm_hour;
	dt.m_time.m_minute=t->tm_min;
	dt.m_time.m_second=t->tm_sec;
	
	return dt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  向当前日期时间添加指定毫秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:28
// 参数说明:  @minutes表示毫秒，可以为负数
// 返 回 值:  添加后的新对象
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addMSecs(int msecs)
{
	SDateTime dt=*this;
	dt.m_time.m_msecond += msecs;
	if(dt.m_time.m_msecond>100)
	{
		int secadd = dt.m_time.m_msecond/1000;
		dt.m_time.m_msecond = dt.m_time.m_msecond%1000;
		dt = dt.addSecs(secadd);
	}
	else if(dt.m_time.m_msecond<0)
	{
		int secadd = 0;
		while(dt.m_time.m_msecond<0)
		{
			secadd --;
			dt.m_time.m_msecond += 1000;
		}
		dt = dt.addSecs(secadd);			
	}
	return dt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  向当前日期时间添加指定微秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:28
// 参数说明:  @usecs表示微秒，可以为负数
// 返 回 值:  添加后的新对象
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::addUSecs(int usecs)
{
	SDateTime dt=*this;
	dt.m_time.m_usecond += usecs;
	if(dt.m_time.m_usecond>1000)
	{
		int msecadd = dt.m_time.m_usecond/1000;
		dt.m_time.m_usecond = dt.m_time.m_usecond%1000;
		dt = dt.addMSecs(msecadd);
	}
	else if(dt.m_time.m_usecond<0)
	{
		int msecadd = 0;
		while(dt.m_time.m_usecond<0)
		{
			msecadd --;
			dt.m_time.m_usecond += 1000;
		}
		dt = dt.addMSecs(msecadd);			
	}
	return dt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  通过格式字符串格式化时间：yyyyMMddhhmmsszzz
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:31
// 参数说明:  @fmt为格式化串，可以为yyyyMMddhhmmsszzzuuu
// 返 回 值:  格式化后的时间
//////////////////////////////////////////////////////////////////////////
SString SDateTime::toString(SString fmt)//yyyyMMddhhmmsszzz
{
	SString s=fmt;
	s.replace("yyyy",toStr("yyyy"));
	s.replace("MM",toStr("MM"));
	s.replace("dd",toStr("dd"));
	s.replace("hh",toStr("hh"));
	s.replace("mm",toStr("mm"));
	s.replace("ss",toStr("ss"));
	s.replace("zzz",toStr("zzz"));
	s.replace("uuu",toStr("uuu"));
	return s;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  由格式化时间转换：yyyyMMddhhmmsszzz
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:33
// 参数说明:  @dtmask为格式化串，可以为yyyyMMddhhmmsszzzuuu
//            @datetime为字符串表示的时间，必须与@dtmask格式完全一致
// 返 回 值:  生成的时间对象SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::makeDateTime(SString dtmask,SString datetime)
{
	SDateTime dt;
	dt.m_date.m_year		= getDateTimeItemByFormatStr(dtmask,datetime,"yyyy");
	dt.m_date.m_month		= getDateTimeItemByFormatStr(dtmask,datetime,"MM");
	dt.m_date.m_day			= getDateTimeItemByFormatStr(dtmask,datetime,"dd");
	dt.m_time.m_hour		= getDateTimeItemByFormatStr(dtmask,datetime,"hh");
	dt.m_time.m_minute	= getDateTimeItemByFormatStr(dtmask,datetime,"mm");
	dt.m_time.m_second	= getDateTimeItemByFormatStr(dtmask,datetime,"ss");
	dt.m_time.m_msecond	= getDateTimeItemByFormatStr(dtmask,datetime,"zzz");
	dt.m_time.m_usecond	= getDateTimeItemByFormatStr(dtmask,datetime,"uuu");
	
	struct tm t;   
	memset(&t,0,sizeof(t));
	t.tm_year  = dt.year()-1900;
	t.tm_mon   = dt.month()-1;//月要减1
	t.tm_mday  = dt.day();
	t.tm_hour  = dt.hour();
	t.tm_min   = dt.minute();
	t.tm_sec   = dt.second();
	dt.m_t=mktime(&t);   
	return dt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  由年月日时分秒等生成SDateTime对象
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:35
// 参数说明:  @y/M/d/h/m/s/ms/us表示年月日时分秒毫秒微秒
// 返 回 值:  生成的时间对象SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::makeDateTime(int y,int M,int d,int h,int m,int s,int ms/*=0*/,int us/*=0*/)
{
	SDateTime dt;
	dt.m_date.m_year	= y;
	dt.m_date.m_month	= M;
	dt.m_date.m_day		= d;
	dt.m_time.m_hour	= h;
	dt.m_time.m_minute	= m;
	dt.m_time.m_second	= s;
	dt.m_time.m_msecond	= ms;
	dt.m_time.m_usecond	= us;
	
	struct tm t;
	memset(&t,0,sizeof(t));
	t.tm_year  = dt.year()-1900;
	t.tm_mon   = dt.month()-1;//月要减1
	t.tm_mday  = dt.day();
	t.tm_hour  = dt.hour();
	t.tm_min   = dt.minute();
	t.tm_sec   = dt.second();
	dt.m_t=mktime(&t);   
	return dt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  由time_t(即SOC)生成SDateTime对象
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:36
// 参数说明:  @t表示源SOC时间
// 返 回 值:  生成的时间对象SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::makeDateTime(time_t t,int ms,int us)
{
	SDateTime dt(t);
	dt.m_time.m_msecond = ms;
	dt.m_time.m_usecond = us;
	return dt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前系统时间
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:37
// 参数说明:  void
// 返 回 值:  生成的时间对象SDateTime
//////////////////////////////////////////////////////////////////////////
SDateTime SDateTime::currentDateTime()
{
	SDateTime dt;
	struct  tm *t;
	time_t now;
	::time(&now);
	t = localtime(&now);
	dt.m_t = now;
	dt.m_date.m_year = 1900+t->tm_year;
	dt.m_date.m_month=1+t->tm_mon;
	dt.m_date.m_day=t->tm_mday;
	dt.m_time.m_hour=t->tm_hour;
	dt.m_time.m_minute=t->tm_min;
	dt.m_time.m_second=t->tm_sec;
	
	#ifdef WIN32
		SYSTEMTIME st;
		GetLocalTime(&st); 
		dt.m_time.m_msecond=st.wMilliseconds;
		dt.m_time.m_usecond=0;
	#else
		timeval tv;
		memset(&tv,0,sizeof(tv));
		gettimeofday(&tv,NULL);
		dt.m_time.m_msecond = tv.tv_usec/1000;
		dt.m_time.m_usecond = tv.tv_usec%1000;
	#endif

	return dt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取当前系统时间到整型的SOC和微秒参数
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:37
// 参数说明:  @soc用于存放SOC世纪秒
//            @usec用于存放微秒
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDateTime::getSystemTime(int &soc,int &usec)
{
#ifdef WIN32
	soc = (int)::time(NULL);
	SYSTEMTIME st;
	GetLocalTime(&st); 
	usec = st.wMilliseconds*1000;
#else
	timeval tv;
	memset(&tv,0,sizeof(tv));
	gettimeofday(&tv,NULL);
	soc = tv.tv_sec;
	usec= tv.tv_usec;
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  设置当前系统时间
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:38
// 参数说明:  @dt表示准备设置的新时间
// 返 回 值:  void
//////////////////////////////////////////////////////////////////////////
void SDateTime::setSystemTime(SDateTime dt)
{
#ifdef WIN32
	SYSTEMTIME t;
	dt = dt.addHours(-8);//北京时间与国际时间的时区差-8小时
	//memset(&t,0,sizeof(t));
	t.wYear				= dt.year();
	t.wMonth			= dt.month();
	t.wDay				= dt.day();
	t.wHour				= dt.hour();
	t.wMinute			= dt.minute();
	t.wSecond			= dt.second();
	t.wDayOfWeek  = 0;
	t.wMilliseconds = dt.msec();
	BOOL b=SetSystemTime(&t); 
#else
	struct tm t;
	memset(&t,0,sizeof(t));
	time_t newtime;   
	t.tm_year  = dt.year()-1900;
	t.tm_mon   = dt.month()-1;//还原时间要减1
	t.tm_mday  = dt.day();
	t.tm_hour  = dt.hour();
	t.tm_min   = dt.minute();
	t.tm_sec   = dt.second();
	newtime=mktime(&t);   
	stime((long*)&newtime);
#ifdef _POWERPC
	//PPC平台下时间需要用此命令同步到硬件RTC中
	::system("hwclock -w");//--systohc
#endif
#endif
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从this的时间到other的时间，有多少秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:39
// 参数说明:  @other参考时间对象
// 返 回 值:  秒时间差，this小other大时为正数，否则为负数，相同为0
//////////////////////////////////////////////////////////////////////////
int SDateTime::secsTo(SDateTime other)
{
	int secs=(int)difftime(other.m_t,m_t);
	return secs;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从this的时间到other的时间，有多少毫秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:40
// 参数说明:  @other参考时间对象
// 返 回 值:  秒时间差，this小other大时为正数，否则为负数，相同为0
//////////////////////////////////////////////////////////////////////////
int SDateTime::msecsTo(SDateTime other)
{
	int msecs=(int)difftime(other.m_t,m_t) * 1000 + 
		(other.m_time.m_msecond - m_time.m_msecond);
	return msecs;
}


////////////////////////////////////////////////////////////////////////
// 描    述:  赋值运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:58
// 参数说明:  @dt表示参考值
// 返 回 值:  this
//////////////////////////////////////////////////////////////////////////
SDateTime& SDateTime::operator=(const SDateTime &dt)
{
	m_date = dt.m_date;
	m_time = dt.m_time;
	m_t = dt.m_t;
	return *this;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  小于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  @other为参考对象
// 返 回 值:  this<other时返回true，否则返回false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator<(SDateTime &other)
{
	return secsTo(other)>0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  大于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  @other为参考对象
// 返 回 值:  this>other时返回true，否则返回false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator>(SDateTime &other)
{
	return secsTo(other)<0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  小于等于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  @other为参考对象
// 返 回 值:  this<=other时返回true，否则返回false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator<=(SDateTime &other)
{
	return secsTo(other)>=0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  大于等于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  @other为参考对象
// 返 回 值:  this>=other时返回true，否则返回false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator>=(SDateTime &other)
{
	return secsTo(other)<=0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  等于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  @other为参考对象
// 返 回 值:  this==other时返回true，否则返回false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator==(SDateTime &other)
{
	return secsTo(other)==0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  不等于比较运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  @other为参考对象
// 返 回 值:  this!=other时返回true，否则返回false
//////////////////////////////////////////////////////////////////////////
bool SDateTime::operator!=(SDateTime &other)
{
	return secsTo(other)!=0;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  减法运算符重载
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  @other为参考对象
// 返 回 值:  返回this-other的时间差秒数
//////////////////////////////////////////////////////////////////////////
int SDateTime::operator-(SDateTime &other)
{
	return other.secsTo(*this);
}

////////////////////////////////////////////////////////////////////////
// 描    述:  取日期对象
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  SDate
//////////////////////////////////////////////////////////////////////////
SDate SDateTime::date(){return m_date;}

////////////////////////////////////////////////////////////////////////
// 描    述:  取时间对象
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  STime
//////////////////////////////////////////////////////////////////////////
STime SDateTime::time(){return m_time;}


////////////////////////////////////////////////////////////////////////
// 描    述:  取年
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  年
//////////////////////////////////////////////////////////////////////////
int SDateTime::year()		{return m_date.year();};

////////////////////////////////////////////////////////////////////////
// 描    述:  取月
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  月
//////////////////////////////////////////////////////////////////////////
int SDateTime::month()		{return m_date.month();};

////////////////////////////////////////////////////////////////////////
// 描    述:  取日
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  日
//////////////////////////////////////////////////////////////////////////
int SDateTime::day()		{return m_date.day();};

////////////////////////////////////////////////////////////////////////
// 描    述:  取时
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  时
//////////////////////////////////////////////////////////////////////////
int SDateTime::hour()		{return m_time.hour();};

////////////////////////////////////////////////////////////////////////
// 描    述:  取分
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  分
//////////////////////////////////////////////////////////////////////////
int SDateTime::minute()	{return m_time.minute();};

////////////////////////////////////////////////////////////////////////
// 描    述:  取秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  秒
//////////////////////////////////////////////////////////////////////////
int SDateTime::second()	{return m_time.second();};

////////////////////////////////////////////////////////////////////////
// 描    述:  取毫秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  毫秒
//////////////////////////////////////////////////////////////////////////
int SDateTime::msec()		{return m_time.msec();};

////////////////////////////////////////////////////////////////////////
// 描    述:  取微秒
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:41
// 参数说明:  void
// 返 回 值:  微秒
//////////////////////////////////////////////////////////////////////////
int SDateTime::usec()		{return m_time.m_usecond;};

////////////////////////////////////////////////////////////////////////
// 描    述:  生成格式化字段
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:54
// 参数说明:  @fmt可以为各个字段的替代字符串：yyyyMMddhhmmsszzzuuu中之一
// 返 回 值:  格式化为字符串的指定时间字段
//////////////////////////////////////////////////////////////////////////
SString SDateTime::toStr(SString fmt)
{
	int len=2;
	int i=0;
	if(fmt == "yyyy")
	{
		len=4;
		i=m_date.m_year;
	}
	else if(fmt == "MM")
		i=m_date.m_month;
	else if(fmt == "dd")
		i=m_date.m_day;
	else if(fmt == "hh")
		i=m_time.m_hour;
	else if(fmt == "mm")
		i=m_time.m_minute;
	else if(fmt == "ss")
		i=m_time.m_second;
	else if(fmt == "zzz")
	{
		len=3;
		i=m_time.m_msecond;
	}
	else if(fmt == "uuu")
	{
		len=3;
		i=m_time.m_usecond;
	}
	fmt.sprintf("%d",i);
	while(fmt.length() < len)
		fmt = "0"+fmt;
	return fmt;
}

////////////////////////////////////////////////////////////////////////
// 描    述:  从给定的时间和格式化串到取指定字段的值
// 作    者:  邵凯田
// 创建时间:  2011-11-14 15:56
// 参数说明:  @dtmask可以为各个字段的替代字符串如：yyyyMMddhhmmsszzzuuu
//            @datetime为实际时间值
//            @fmt为等取的字段替代字符串：yyyyMMddhhmmsszzzuuu中之一
// 返 回 值:  int
//////////////////////////////////////////////////////////////////////////
int SDateTime::getDateTimeItemByFormatStr(SString &dtmask,SString &datetime,SString fmt)
{
	int p,l;
	p = dtmask.find(fmt);
	l = fmt.length();
	if(p<0)
		return 0;
	return datetime.mid(p,l).toInt();
}


