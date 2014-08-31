#include "AnalyzePub.h"

char DescType[256][64] = {
/*  0-  4*/	{"未知类型"},{"单点信息"},{"带时标的单点信息"},{"双点信息"},{"带时标的双点信息"},
/*  5-  9*/	{"步位置信息"},{"带时标的步位置信息"},{"32比特串"},{"带时标的32比特串"},{"测量值,规一化值"},
/* 10- 14*/	{"带时标的测量值,规一化值"},{"测量值,标度化值"},{"带时标的测量值,标度化值"},{"测量值,短浮点数"},{"带时标的测量值,短浮点数"},
/* 15- 19*/	{"累计量"},{"带时标的累计量"},{"带时标的继电保护装置事件"},{"带时标的继电保护装置成组启动事件"},{"带时标的继电保护装置成组输出电路信息"},
/* 20- 24*/	{"带变位检出的成组单点信息"},{"测量值,不带品质描述词的规一化值"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 25- 29*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 30- 34*/	{"带CP56Time2a时标的单点信息"},{"带CP56Time2a时标的双点信息"},{"带CP56Time2a时标的步位置信息"},{"带CP56Time2a时标的32比特串"},{"带CP56Time2a时标的测量值,规一化值"},
/* 35- 39*/	{"带CP56Time2a时标的测量值,标度化值"},{"带CP56Time2a时标的测量值,短浮点数"},{"带CP56Time2a时标的累计量"},{"带CP56Time2a时标的继电保护装置事件"},{"带CP56Time2a时标的继电保护装置成组启动事件"},
/* 40- 44*/	{"带CP56Time2a时标的继电保拌装置成组输出电路信息"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 45- 49*/	{"单点命令"},{"双点命令"},{"调节步命令"},{"设定值命令,规一化值"},{"设定值命令,标度化值"},
/* 50- 54*/	{"设定值命令，短浮点数"},{"设定值命令,32比特串"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 55- 59*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 60- 64*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 65- 69*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 70- 74*/	{"初始化结束"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 75- 79*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 80- 84*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 85- 89*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 90- 94*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/* 95- 99*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*100-104*/	{"总召唤命令"},{"累计量召唤命令"},{"读命令"},{"时钟同步命令"},{"测试命令"},
/*105-109*/	{"复位进程命令"},{"收集传输延时"},{"未知类型"},{"未知类型"},{"未知类型"},
/*110-114*/	{"测量值参数,规一化值"},{"测量值参数，标度化值"},{"测量值参数,短浮点数"},{"参数激活"},{"未知类型"},
/*115-119*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*120-124*/	{"文件淮备就绪"},{"节淮备就绪"},{"召唤目录,选择文件,召唤文件召唤节"},{"最后的节,最后的段"},{"认可文件,认可节"},
/*125-129*/	{"段"},{"目录"},{"未知类型"},{"未知类型"},{"未知类型"},
/*130-134*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*135-139*/	{"未知类型"},{"连续对象设定值命令"},{"未知类型"},{"未知类型"},{"未知类型"},
/*140-144*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*145-149*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*150-154*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*155-159*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*160-164*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*165-169*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*170-174*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*175-179*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*180-184*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*185-189*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*190-194*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*195-199*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*200-204*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*205-209*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*210-214*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*215-219*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*220-224*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*225-229*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*230-234*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*235-239*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*240-244*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*245-249*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*250-254*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
/*255-   */	{"未知类型"}
};

char* DescCause[50][64] = {
/*  0-  4*/	{"未知原因"},{"循环"},{"背景扫描"},{"突发"},{"初始化完成"},
/*  5-  9*/	{"请求或者被请求"},{"激活"},{"激活确认"},{"停止激活"},{"停止激活确认"},
/* 10- 14*/	{"激活终止"},{"远方命令引起的返送信息"},{"当地命令引起的返送信息"},{"文件传输"},{"未知原因"},
/* 15- 19*/	{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},
/* 20- 24*/	{"响应站召唤"},{"响应第1组召唤"},{"响应第2组召唤"},{"响应第3组召唤"},{"响应第4组召唤"},
/* 25- 29*/	{"响应第5组召唤"},{"响应第6组召唤"},{"响应第7组召唤"},{"响应第8组召唤"},{"响应第9组召唤"},
/* 30- 34*/	{"响应第10组召唤"},{"响应第11组召唤"},{"响应第12组召唤"},{"响应第13组召唤"},{"响应第14组召唤"},
/* 35- 39*/	{"响应第15组召唤"},{"响应第16组召唤"},{"响应计数量站召唤"},{"响应笫1组计数量召唤"},{"响应笫2组计数量召唤"},
/* 40- 44*/	{"响应笫3组计数量召唤"},{"响应笫4组计数量召唤"},{"未知原因"},{"未知原因"},{"未知的类型标识"},
/* 45- 49*/	{"未知的传送原因"},{"未知的应用服务数据单元公共地址"},{"未知的信息对象地址"},{"未知原因"},{"未知原因"}
};

bool cp16time_desc(char* DrawStr,CP16Time2a* tm16)
{
	char			tmp[512] = {'\0'};
	string			szTemp;

	szTemp.assign("时间: ");
	strcat(DrawStr,szTemp.c_str());

	sprintf(tmp,"%d秒%d毫秒 ",tm16->parts.ms/1000,tm16->parts.ms%1000);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

    return true;
}

bool cp24time_desc(char* DrawStr,CP24Time2a* tm24)
{
	char			tmp[512] = {'\0'};
	string			szTemp;

	szTemp.assign("时间: ");
	strcat(DrawStr,szTemp.c_str());

	if (tm24->parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}

	sprintf(tmp,"%d分%d秒%d毫秒 ",tm24->parts.min,tm24->parts.ms/1000,tm24->parts.ms%1000);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

    return true;
}

bool cp32time_desc(char* DrawStr,CP32Time2a* tm32)
{
	char			tmp[512] = {'\0'};
	string			szTemp;

	szTemp.assign("时间: ");
	strcat(DrawStr,szTemp.c_str());

	if (tm32->parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (tm32->parts.su)
	{
		szTemp.assign("夏令时 ");
		strcat(DrawStr,szTemp.c_str());
	}

	sprintf(tmp,"%d时%d分%d秒%d毫秒 ",tm32->parts.hour,tm32->parts.min,tm32->parts.ms/1000,tm32->parts.ms%1000);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

    return true;
}

bool cp56time_desc(char* DrawStr,CP56Time2a* tm56)
{
	char			tmp[512] = {'\0'};
	string			szTemp;

	szTemp.assign("时间: ");
	strcat(DrawStr,szTemp.c_str());

	if (tm56->parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (tm56->parts.su)
	{
		szTemp.assign("夏令时 ");
		strcat(DrawStr,szTemp.c_str());
	}

	sprintf(tmp,"%d年%d月%d日(星期%d)%d时%d分%d秒%d毫秒 ",
			tm56->parts.year+2000,tm56->parts.month,tm56->parts.day,tm56->parts.week,
			tm56->parts.hour,tm56->parts.min,tm56->parts.ms/1000,tm56->parts.ms%1000);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

    return true;
}

bool qds_desc(char* DrawStr,QDS* qds)
{
	string			szTemp;

	if (qds->parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qds->parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qds->parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qds->parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qds->parts.OV)
	{
		szTemp.assign("数据溢出 ");
		strcat(DrawStr,szTemp.c_str());
	}

    return true;
}

bool qdp_desc(char* DrawStr,QDP* qdp)
{
	string			szTemp;

	if (qdp->parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qdp->parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qdp->parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qdp->parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (qdp->parts.EI)
	{
		szTemp.assign("动作时间无效 ");
		strcat(DrawStr,szTemp.c_str());
	}

	return true;
}

bool qpm_desc(char* DrawStr,QPM* qpm)
{
	string			szTemp;

	if (qpm->parts.LPC)
		szTemp.assign("改变 ");
	else
		szTemp.assign("未改变 ");
	strcat(DrawStr,szTemp.c_str());

	if (qpm->parts.POP)
		szTemp.assign("未运行 ");
	else
		szTemp.assign("运行 ");
	strcat(DrawStr,szTemp.c_str());

	if (qpm->parts.KPA == 1)
		szTemp.assign("门限值 ");
	else if (qpm->parts.KPA == 2)
		szTemp.assign("平滑系统 ");
	else if (qpm->parts.KPA == 2)
		szTemp.assign("传送测量值的下限 ");
	else if (qpm->parts.KPA == 2)
		szTemp.assign("传送测量值的上限 ");
	else
		szTemp.assign("使用保留 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}

//------------------ 未知类型处理 -------------------
bool INVALID_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;

	szTemp.assign("此类型用户数据暂不解析!");
	strcat(DrawStr,szTemp.c_str());

    return true;
}
//------------------ 单点信息处理 -------------------
bool M_SP_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_SP_NA_1_t		m_sp_na;

	memcpy(&m_sp_na,udata,SizeOfType(typid));

	if (m_sp_na.siq.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_na.siq.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_na.siq.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_na.siq.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_na.siq.parts.SPI)
	{
		szTemp.assign("状态:合 ");
	}
	else
	{
		szTemp.assign("状态:分 ");
	}
	strcat(DrawStr,szTemp.c_str());	

	return true;
}
//------------------ 带时标的单点信息处理 -------------------
bool M_SP_TA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_SP_TA_1_t		m_sp_ta;

	memcpy(&m_sp_ta,udata,SizeOfType(typid));

	if (m_sp_ta.siq.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_ta.siq.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_ta.siq.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_ta.siq.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_ta.siq.parts.SPI)
	{
		szTemp.assign("状态:合 ");
	}
	else
	{
		szTemp.assign("状态:分 ");
	}
	strcat(DrawStr,szTemp.c_str());

	cp24time_desc(DrawStr,&m_sp_ta.tm);

	return true;
}
//------------------ 双点信息处理 -------------------
bool M_DP_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_DP_NA_1_t		m_dp_na;

	memcpy(&m_dp_na,udata,SizeOfType(typid));

	if (m_dp_na.diq.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_na.diq.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_na.diq.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_na.diq.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_na.diq.parts.DPI == 2)
	{
		szTemp.assign("状态:合 ");
	}
	else if (m_dp_na.diq.parts.DPI == 1)
	{
		szTemp.assign("状态:分 ");
	}
	else
	{
		szTemp.assign("状态:中态 ");
	}
	strcat(DrawStr,szTemp.c_str());	

	return true;
}
//------------------ 带时标的双点信息处理 -------------------
bool M_DP_TA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_DP_TA_1_t		m_dp_ta;

	memcpy(&m_dp_ta,udata,SizeOfType(typid));

	if (m_dp_ta.diq.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_ta.diq.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_ta.diq.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_ta.diq.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_ta.diq.parts.DPI == 2)
	{
		szTemp.assign("状态:合 ");
	}
	else if (m_dp_ta.diq.parts.DPI == 1)
	{
		szTemp.assign("状态:分 ");
	}
	else
	{
		szTemp.assign("状态:中态 ");
	}
	strcat(DrawStr,szTemp.c_str());
	
	cp24time_desc(DrawStr,&m_dp_ta.tm);

	return true;
}
//------------------ 步位置信息处理 -------------------
bool M_ST_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ST_NA_1_t		m_st_na;

	memcpy(&m_st_na,udata,SizeOfType(typid));

	if (m_st_na.vti.parts.transient)
	{
		szTemp.assign("设备在瞬间状态 ");
		strcat(DrawStr,szTemp.c_str());
	}

	sprintf(tmp,"值:%d ",m_st_na.vti.parts.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_st_na.qds);

	return true;
}
//------------------ 带时标的步位置信息处理 -------------------
bool M_ST_TA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ST_TA_1_t		m_st_ta;

	memcpy(&m_st_ta,udata,SizeOfType(typid));

	if (m_st_ta.vti.parts.transient)
	{
		szTemp.assign("设备在瞬间状态 ");
		strcat(DrawStr,szTemp.c_str());
	}

	sprintf(tmp,"值:%d ",m_st_ta.vti.parts.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_st_ta.qds);

	cp24time_desc(DrawStr,&m_st_ta.tm);

	return true;
}
//------------------ 32比特串处理 -------------------
bool M_BO_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_BO_NA_1_t		m_bo_na;

	memcpy(&m_bo_na,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d(%08XH) ",m_bo_na.bsi.bits,m_bo_na.bsi.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_bo_na.qds);

	return true;
}
//------------------ 带时标的32比特串处理 -------------------
bool M_BO_TA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_BO_TA_1_t		m_bo_ta;

	memcpy(&m_bo_ta,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d(%08XH) ",m_bo_ta.bsi.bits,m_bo_ta.bsi.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_bo_ta.qds);

	cp24time_desc(DrawStr,&m_bo_ta.tm);

	return true;
}
//------------------ 测量值,规一化值处理 -------------------
bool M_ME_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_NA_1_t		m_me_na;

	memcpy(&m_me_na,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d ",m_me_na.nva.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_na.qds);

	return true;
}
//------------------ 带时标的测量值,规一化值处理 -------------------
bool M_ME_TA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_TA_1_t		m_me_ta;

	memcpy(&m_me_ta,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d ",m_me_ta.nva.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_ta.qds);

	cp24time_desc(DrawStr,&m_me_ta.tm);

	return true;
}
//------------------ 测量值,标度化值处理 -------------------
bool M_ME_NB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_NB_1_t		m_me_nb;

	memcpy(&m_me_nb,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d ",m_me_nb.sva.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_nb.qds);

	return true;
}
//------------------ 带时标的测量值,标度化值处理 -------------------
bool M_ME_TB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_TB_1_t		m_me_tb;

	memcpy(&m_me_tb,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d ",m_me_tb.sva.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_tb.qds);

	cp24time_desc(DrawStr,&m_me_tb.tm);

	return true;
}
//------------------ 测量值,短浮点数处理 -------------------
bool M_ME_NC_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_NC_1_t		m_me_nc;

	memcpy(&m_me_nc,udata,SizeOfType(typid));

	sprintf(tmp,"值:%f ",m_me_nc.std.f);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_nc.qds);

	return true;
}
//------------------ 带时标的测量值,短浮点数处理 -------------------
bool M_ME_TC_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_TC_1_t		m_me_tc;

	memcpy(&m_me_tc,udata,SizeOfType(typid));

	sprintf(tmp,"值:%f ",m_me_tc.std.f);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_tc.qds);

	cp24time_desc(DrawStr,&m_me_tc.tm);

	return true;
}
//------------------ 累计量处理 -------------------
bool M_IT_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_IT_NA_1_t		m_it_na;

	memcpy(&m_it_na,udata,SizeOfType(typid));

	if (m_it_na.bcr.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_na.bcr.parts.CY)
	{
		szTemp.assign("进位 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_na.bcr.parts.CA)
	{
		szTemp.assign("计数器被调整 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_na.bcr.parts.SQ)
	{
		szTemp.assign("顺序号 ");
		strcat(DrawStr,szTemp.c_str());
	}
	sprintf(tmp,"值:%d ",m_it_na.bcr.parts.counter);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 带时标的累计量处理 -------------------
bool M_IT_TA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_IT_TA_1_t		m_it_ta;

	memcpy(&m_it_ta,udata,SizeOfType(typid));

	if (m_it_ta.bcr.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_ta.bcr.parts.CY)
	{
		szTemp.assign("进位 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_ta.bcr.parts.CA)
	{
		szTemp.assign("计数器被调整 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_ta.bcr.parts.SQ)
	{
		szTemp.assign("顺序号 ");
		strcat(DrawStr,szTemp.c_str());
	}
	sprintf(tmp,"值:%d ",m_it_ta.bcr.parts.counter);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	cp24time_desc(DrawStr,&m_it_ta.tm);

	return true;
}
//------------------ 带时标的继电保护装置事件处理 -------------------
bool M_EP_TA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_EP_TA_1_t		m_ep_ta;

	memcpy(&m_ep_ta,udata,SizeOfType(typid));

	if (m_ep_ta.sep.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_ta.sep.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_ta.sep.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_ta.sep.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_ta.sep.parts.EI)
	{
		szTemp.assign("动作时间无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_ta.sep.parts.ES == 2)
	{
		szTemp.assign("状态:合 ");
	}
	else if (m_ep_ta.sep.parts.ES == 1)
	{
		szTemp.assign("状态:分 ");
	}
	else
	{
		szTemp.assign("状态:中态 ");
	}
	strcat(DrawStr,szTemp.c_str());

	szTemp.assign("动作");
	strcat(DrawStr,szTemp.c_str());
	cp16time_desc(DrawStr,&m_ep_ta.tm16);

	cp24time_desc(DrawStr,&m_ep_ta.tm24);

	return true;
}
//------------------ 带时标的继电保护装置成组启动事件处理 -------------------
bool M_EP_TB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_EP_TB_1_t		m_ep_tb;

	memcpy(&m_ep_tb,udata,SizeOfType(typid));

	if (m_ep_tb.spe.parts.GS)
	{
		szTemp.assign("总启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tb.spe.parts.SL1)
	{
		szTemp.assign("A相保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tb.spe.parts.SL2)
	{
		szTemp.assign("B相保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tb.spe.parts.SL3)
	{
		szTemp.assign("C相保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tb.spe.parts.SIE)
	{
		szTemp.assign("接地电流保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tb.spe.parts.SRD)
	{
		szTemp.assign("反向保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}

	qdp_desc(DrawStr,&m_ep_tb.qdp);

	szTemp.assign("继电器动作持续");
	strcat(DrawStr,szTemp.c_str());
	cp16time_desc(DrawStr,&m_ep_tb.tm16);

	cp24time_desc(DrawStr,&m_ep_tb.tm24);

	return true;
}
//------------------ 带时标的继电保护装置成组输出电路信息处理 -------------------
bool M_EP_TC_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_EP_TC_1_t		m_ep_tc;

	memcpy(&m_ep_tc,udata,SizeOfType(typid));

	if (m_ep_tc.oci.parts.GC)
	{
		szTemp.assign("总命令输出至输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tc.oci.parts.CL1)
	{
		szTemp.assign("命令输出至A相输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tc.oci.parts.CL2)
	{
		szTemp.assign("命令输出至B相输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tc.oci.parts.CL3)
	{
		szTemp.assign("命令输出至C相输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	
	qdp_desc(DrawStr,&m_ep_tc.qdp);

	szTemp.assign("继电器动作");
	strcat(DrawStr,szTemp.c_str());
	cp16time_desc(DrawStr,&m_ep_tc.tm16);

	cp24time_desc(DrawStr,&m_ep_tc.tm24);

	return true;
}
//------------------ 带变位检出的成组单点信息处理 -------------------
bool M_PS_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_PS_NA_1_t		m_ps_na;
	int				i,j;

	memcpy(&m_ps_na,udata,SizeOfType(typid));

	for (i = 1,j = 0; i <= 16; i++,j++)
	{
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"位%02d状态:%d ",j,(m_ps_na.scd.parts.ST >> j) & 0x01);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		if (!(i % 8))
			strcat(DrawStr,"\r\n");
	}

	for (i = 1,j = 0; i <= 16; i++,j++)
	{
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"位%02d检出:%d ",j,(m_ps_na.scd.parts.CD >> j) & 0x01);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		if (!(i % 8))
			strcat(DrawStr,"\r\n");
	}
	
	qds_desc(DrawStr,&m_ps_na.qds);

	return true;
}
//------------------ 测量值,不带品质描述词的规一化值处理 -------------------
bool M_ME_ND_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_ND_1_t		m_me_nd;

	memcpy(&m_me_nd,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d ",m_me_nd.nva.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 带CP56Time2a时标的单点信息处理 -------------------
bool M_SP_TB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_SP_TB_1_t		m_sp_tb;

	memcpy(&m_sp_tb,udata,SizeOfType(typid));

	if (m_sp_tb.siq.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_tb.siq.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_tb.siq.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_tb.siq.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_sp_tb.siq.parts.SPI)
	{
		szTemp.assign("状态:合 ");
	}
	else
	{
		szTemp.assign("状态:分 ");
	}
	strcat(DrawStr,szTemp.c_str());

	cp56time_desc(DrawStr,&m_sp_tb.tm);

	return true;
}
//------------------ 带CP56Time2a时标的双点信息处理 -------------------
bool M_DP_TB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_DP_TB_1_t		m_dp_tb;

	memcpy(&m_dp_tb,udata,SizeOfType(typid));

	if (m_dp_tb.diq.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_tb.diq.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_tb.diq.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_tb.diq.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_dp_tb.diq.parts.DPI == 2)
	{
		szTemp.assign("状态:合 ");
	}
	else if (m_dp_tb.diq.parts.DPI == 1)
	{
		szTemp.assign("状态:分 ");
	}
	else
	{
		szTemp.assign("状态:中态 ");
	}
	strcat(DrawStr,szTemp.c_str());	

	cp56time_desc(DrawStr,&m_dp_tb.tm);

	return true;
}
//------------------ 带CP56Time2a时标的步位置信息处理 -------------------
bool M_ST_TB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ST_TB_1_t		m_st_tb;

	memcpy(&m_st_tb,udata,SizeOfType(typid));

	if (m_st_tb.vti.parts.transient)
	{
		szTemp.assign("设备在瞬间状态 ");
		strcat(DrawStr,szTemp.c_str());
	}

	sprintf(tmp,"值:%d ",m_st_tb.vti.parts.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_st_tb.qds);

	cp56time_desc(DrawStr,&m_st_tb.tm);

	return true;
}
//------------------ 带CP56Time2a时标的32比特串处理 -------------------
bool M_BO_TB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_BO_TB_1_t		m_bo_tb;

	memcpy(&m_bo_tb,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d(%08XH) ",m_bo_tb.bsi.bits,m_bo_tb.bsi.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_bo_tb.qds);

	cp56time_desc(DrawStr,&m_bo_tb.tm);

	return true;
}
//------------------ 带CP56Time2a时标的测量值,规一化值处理 -------------------
bool M_ME_TD_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_TD_1_t		m_me_td;

	memcpy(&m_me_td,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d ",m_me_td.nva.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_td.qds);

	cp56time_desc(DrawStr,&m_me_td.tm);

	return true;
}
//------------------ 带CP56Time2a时标的测量值,标度化值处理 -------------------
bool M_ME_TE_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_TE_1_t		m_me_te;

	memcpy(&m_me_te,udata,SizeOfType(typid));

	sprintf(tmp,"值:%d ",m_me_te.sva.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_te.qds);

	cp56time_desc(DrawStr,&m_me_te.tm);

	return true;
}
//------------------ 带CP56Time2a时标的测量值,短浮点数处理 -------------------
bool M_ME_TF_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_ME_TF_1_t		m_me_tf;

	memcpy(&m_me_tf,udata,SizeOfType(typid));

	sprintf(tmp,"值:%f ",m_me_tf.std.f);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	qds_desc(DrawStr,&m_me_tf.qds);

	cp56time_desc(DrawStr,&m_me_tf.tm);

	return true;
}
//------------------ 带CP56Time2a时标的累计量处理 -------------------
bool M_IT_TB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	M_IT_TB_1_t		m_it_tb;

	memcpy(&m_it_tb,udata,SizeOfType(typid));

	if (m_it_tb.bcr.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_tb.bcr.parts.CY)
	{
		szTemp.assign("进位 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_tb.bcr.parts.CA)
	{
		szTemp.assign("计数器被调整 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_it_tb.bcr.parts.SQ)
	{
		szTemp.assign("顺序号 ");
		strcat(DrawStr,szTemp.c_str());
	}
	sprintf(tmp,"值:%d ",m_it_tb.bcr.parts.counter);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	cp56time_desc(DrawStr,&m_it_tb.tm);

	return true;
}
//------------------ 带CP56Time2a时标的继电保护装置事件处理 -------------------
bool M_EP_TD_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_EP_TD_1_t		m_ep_td;

	memcpy(&m_ep_td,udata,SizeOfType(typid));

	if (m_ep_td.sep.parts.IV)
	{
		szTemp.assign("无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_td.sep.parts.NT)
	{
		szTemp.assign("非当前值 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_td.sep.parts.SB)
	{
		szTemp.assign("被替换 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_td.sep.parts.BL)
	{
		szTemp.assign("被闭锁 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_td.sep.parts.EI)
	{
		szTemp.assign("动作时间无效 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_td.sep.parts.ES == 2)
	{
		szTemp.assign("状态:合 ");
	}
	else if (m_ep_td.sep.parts.ES == 1)
	{
		szTemp.assign("状态:分 ");
	}
	else
	{
		szTemp.assign("状态:中态 ");
	}
	strcat(DrawStr,szTemp.c_str());

	szTemp.assign("动作");
	strcat(DrawStr,szTemp.c_str());
	cp16time_desc(DrawStr,&m_ep_td.tm16);

	cp56time_desc(DrawStr,&m_ep_td.tm56);

	return true;
}
//------------------ 带CP56Time2a时标的继电保护装置成组启动事件处理 -------------------
bool M_EP_TE_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_EP_TE_1_t		m_ep_te;

	memcpy(&m_ep_te,udata,SizeOfType(typid));

	if (m_ep_te.spe.parts.GS)
	{
		szTemp.assign("总启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_te.spe.parts.SL1)
	{
		szTemp.assign("A相保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_te.spe.parts.SL2)
	{
		szTemp.assign("B相保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_te.spe.parts.SL3)
	{
		szTemp.assign("C相保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_te.spe.parts.SIE)
	{
		szTemp.assign("接地电流保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_te.spe.parts.SRD)
	{
		szTemp.assign("反向保护启动 ");
		strcat(DrawStr,szTemp.c_str());
	}

	qdp_desc(DrawStr,&m_ep_te.qdp);

	szTemp.assign("继电器动作持续");
	strcat(DrawStr,szTemp.c_str());
	cp16time_desc(DrawStr,&m_ep_te.tm16);

	cp56time_desc(DrawStr,&m_ep_te.tm56);

	return true;
}
//------------------ 带CP56Time2a时标的继电保拌装置成组输出电路信息处理 -------------------
bool M_EP_TF_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_EP_TF_1_t		m_ep_tf;

	memcpy(&m_ep_tf,udata,SizeOfType(typid));

	if (m_ep_tf.oci.parts.GC)
	{
		szTemp.assign("总命令输出至输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tf.oci.parts.CL1)
	{
		szTemp.assign("命令输出至A相输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tf.oci.parts.CL2)
	{
		szTemp.assign("命令输出至B相输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (m_ep_tf.oci.parts.CL3)
	{
		szTemp.assign("命令输出至C相输出电路 ");
		strcat(DrawStr,szTemp.c_str());
	}
	
	qdp_desc(DrawStr,&m_ep_tf.qdp);

	szTemp.assign("继电器动作");
	strcat(DrawStr,szTemp.c_str());
	cp16time_desc(DrawStr,&m_ep_tf.tm16);

	cp56time_desc(DrawStr,&m_ep_tf.tm56);

	return true;
}
//------------------ 单点命令处理 -------------------
bool C_SC_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	C_SC_NA_1_t		c_sc_na;

	memcpy(&c_sc_na,udata,SizeOfType(typid));

	if (c_sc_na.sco.parts.QU == 1)
		szTemp.assign("短脉冲持续时间 ");
	else if (c_sc_na.sco.parts.QU == 2)
		szTemp.assign("长脉冲持续时间 ");
	else if (c_sc_na.sco.parts.QU == 2)
		szTemp.assign("持续输出 ");
	strcat(DrawStr,szTemp.c_str());

	if (c_sc_na.sco.parts.SE)
		szTemp.assign("选择 ");
	else 
		szTemp.assign("执行 ");
	strcat(DrawStr,szTemp.c_str());
	
	if (c_sc_na.sco.parts.SCS)
		szTemp.assign("遥控:合 ");
	else 
		szTemp.assign("遥控:分 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 双点命令处理 -------------------
bool C_DC_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	C_DC_NA_1_t		c_dc_na;

	memcpy(&c_dc_na,udata,SizeOfType(typid));

	if (c_dc_na.dco.parts.QU == 1)
		szTemp.assign("短脉冲持续时间 ");
	else if (c_dc_na.dco.parts.QU == 2)
		szTemp.assign("长脉冲持续时间 ");
	else if (c_dc_na.dco.parts.QU == 2)
		szTemp.assign("持续输出 ");
	strcat(DrawStr,szTemp.c_str());

	if (c_dc_na.dco.parts.SE)
		szTemp.assign("选择 ");
	else 
		szTemp.assign("执行 ");
	strcat(DrawStr,szTemp.c_str());
	
	if (c_dc_na.dco.parts.DCS == 2)
		szTemp.assign("遥控:合 ");
	else if (c_dc_na.dco.parts.DCS == 1)
		szTemp.assign("遥控:分 ");
	else
		szTemp.assign("遥控:不允许 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 调节步命令处理 -------------------
bool C_RC_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	C_RC_NA_1_t		c_rc_na;

	memcpy(&c_rc_na,udata,SizeOfType(typid));

	if (c_rc_na.rco.parts.QU == 1)
		szTemp.assign("短脉冲持续时间 ");
	else if (c_rc_na.rco.parts.QU == 2)
		szTemp.assign("长脉冲持续时间 ");
	else if (c_rc_na.rco.parts.QU == 2)
		szTemp.assign("持续输出 ");
	strcat(DrawStr,szTemp.c_str());

	if (c_rc_na.rco.parts.SE)
		szTemp.assign("选择 ");
	else 
		szTemp.assign("执行 ");
	strcat(DrawStr,szTemp.c_str());
	
	if (c_rc_na.rco.parts.RCS == 2)
		szTemp.assign("步调节:升一步 ");
	else if (c_rc_na.rco.parts.RCS == 1)
		szTemp.assign("步调节:降一步 ");
	else
		szTemp.assign("步调节:不允许 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 设定值命令,规一化值处理 -------------------
bool C_SE_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	C_SE_NA_1_t		c_se_na;

	memcpy(&c_se_na,udata,SizeOfType(typid));

	if (c_se_na.qos.parts.SE)
		szTemp.assign("选择 ");
	else 
		szTemp.assign("执行 ");
	strcat(DrawStr,szTemp.c_str());
	
	sprintf(tmp,"设值:%d ",c_se_na.nva.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 设定值命令,标度化值处理 -------------------
bool C_SE_NB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	C_SE_NB_1_t		c_se_nb;

	memcpy(&c_se_nb,udata,SizeOfType(typid));

	if (c_se_nb.qos.parts.SE)
		szTemp.assign("选择 ");
	else 
		szTemp.assign("执行 ");
	strcat(DrawStr,szTemp.c_str());
	
	sprintf(tmp,"设值:%d ",c_se_nb.sva.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 设定值命令，短浮点数处理 -------------------
bool C_SE_NC_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	C_SE_NC_1_t		c_se_nc;

	memcpy(&c_se_nc,udata,SizeOfType(typid));

	if (c_se_nc.qos.parts.SE)
		szTemp.assign("选择 ");
	else 
		szTemp.assign("执行 ");
	strcat(DrawStr,szTemp.c_str());
	
	sprintf(tmp,"设值:%f",c_se_nc.std.f);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 设定值命令,32比特串处理 -------------------
bool C_BO_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	C_BO_NA_1_t		c_bo_na;

	memcpy(&c_bo_na,udata,SizeOfType(typid));
	
	sprintf(tmp,"设值:%d(%08XH) ",c_bo_na.bsi.bits,c_bo_na.bsi.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 初始化结束处理 -------------------
bool M_EI_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	M_EI_NA_1_t		m_ei_na;

	memcpy(&m_ei_na,udata,SizeOfType(typid));
	
	if (m_ei_na.coi.parts.cause == 0)
		szTemp.assign("当地电源合上 ");
	else if (m_ei_na.coi.parts.cause == 1)
		szTemp.assign("当地手动复位 ");
	else if (m_ei_na.coi.parts.cause == 2)
		szTemp.assign("远方复位 ");
	strcat(DrawStr,szTemp.c_str());

	if (m_ei_na.coi.parts.state)
		szTemp.assign("改变当地参数后的初始化 ");
	else
		szTemp.assign("未改变当地参数后的初始化 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 总召唤命令处理 -------------------
bool C_IC_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	C_IC_NA_1_t		c_ic_na;

	memcpy(&c_ic_na,udata,SizeOfType(typid));
	
	if (c_ic_na.qoi.qoi >= 20 && c_ic_na.qoi.qoi <= 36)
	{
		if (c_ic_na.qoi.qoi == 20)
			szTemp.assign("站召唤(全局) ");
		else
		{
			sprintf(tmp,"第%d组召唤 ",c_ic_na.qoi.qoi);
			szTemp.assign(tmp);
		}
	}
	else
		szTemp.assign("使用保留 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 累计量召唤命令处理 -------------------
bool C_CI_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	C_CI_NA_1_t		c_ci_na;

	memcpy(&c_ci_na,udata,SizeOfType(typid));
	
	if (c_ci_na.qcc.parts.RQT > 0 && c_ci_na.qcc.parts.RQT <= 5)
	{
		if (c_ci_na.qcc.parts.RQT == 5)
			szTemp.assign("总的请求计数量 ");
		else
		{
			sprintf(tmp,"请求计数量第%d组 ",c_ci_na.qcc.parts.RQT);
			szTemp.assign(tmp);
		}
	}
	else
		szTemp.assign("使用保留 ");
	strcat(DrawStr,szTemp.c_str());

	if (c_ci_na.qcc.parts.FRZ == 0)
		szTemp.assign("读 ");
	else if (c_ci_na.qcc.parts.FRZ == 1)
		szTemp.assign("计数量冻结不带复位 ");
	else if (c_ci_na.qcc.parts.FRZ == 2)
		szTemp.assign("计数量冻结带复位 ");
	else if (c_ci_na.qcc.parts.FRZ == 3)
		szTemp.assign("计数量复位 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 时钟同步命令处理 -------------------
bool C_CS_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	C_CS_NA_1_t		c_cs_na;

	memcpy(&c_cs_na,udata,SizeOfType(typid));
	
	cp56time_desc(DrawStr,&c_cs_na.tm);

	return true;
}
//------------------ 测试命令处理 -------------------
bool C_TS_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	C_TS_NA_1_t		c_ts_na;

	memcpy(&c_ts_na,udata,SizeOfType(typid));
	
	sprintf(tmp,"固定测试字: %4XH ",c_ts_na.fbp.HEX);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 复位进程命令处理 -------------------
bool C_RP_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	C_RP_NA_1_t		c_rp_na;

	memcpy(&c_rp_na,udata,SizeOfType(typid));
	
	if (c_rp_na.qrp.qrp == 1)
		szTemp.assign("进程总复位 ");
	else if (c_rp_na.qrp.qrp == 2)
		szTemp.assign("复位事件缓冲区等待处理的带时标的信息 ");
	else 
		szTemp.assign("使用保留 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 收集传输延时处理 -------------------
bool C_CD_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	C_CD_NA_1_t		c_cd_na;

	memcpy(&c_cd_na,udata,SizeOfType(typid));
	
	cp16time_desc(DrawStr,&c_cd_na.tm);

	return true;
}
//------------------ 测量值参数,规一化值处理 -------------------
bool P_ME_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	P_ME_NA_1_t		p_me_na;

	memcpy(&p_me_na,udata,SizeOfType(typid));

	qpm_desc(DrawStr,&p_me_na.qpm);

	sprintf(tmp,"值:%d ",p_me_na.nva.value);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 测量值参数,标度化值处理 -------------------
bool P_ME_NB_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	P_ME_NB_1_t		p_me_nb;

	memcpy(&p_me_nb,udata,SizeOfType(typid));

	qpm_desc(DrawStr,&p_me_nb.qpm);

	sprintf(tmp,"值:%d ",p_me_nb.sva.bits);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 测量值参数,短浮点数处理 -------------------
bool P_ME_NC_desc(char* DrawStr,unsigned char typid,char* udata)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	P_ME_NC_1_t		p_me_nc;

	memcpy(&p_me_nc,udata,SizeOfType(typid));

	qpm_desc(DrawStr,&p_me_nc.qpm);

	sprintf(tmp,"值:%f ",p_me_nc.std.f);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 参数激活处理 -------------------
bool P_AC_NA_desc(char* DrawStr,unsigned char typid,char* udata)
{
	string			szTemp;
	P_AC_NA_1_t		p_ac_na;

	memcpy(&p_ac_na,udata,SizeOfType(typid));

	if (p_ac_na.qpa.qpa == 1)
		szTemp.assign("激活/停止激活这之前装载的参数 ");
	else if (p_ac_na.qpa.qpa == 2)
		szTemp.assign("激活/停止激活所寻址信息对象的参数 ");
	else if (p_ac_na.qpa.qpa == 3)
		szTemp.assign("激活/停止激活所寻址的持续循环或周期传输的信息对象 ");
	else
		szTemp.assign("使用保留 ");
	strcat(DrawStr,szTemp.c_str());

	return true;
}
//------------------ 数据类型集 -------------------
typedef bool (*USER_DATA_DESC)(char*,unsigned char,char*);
bool DescUserData(char* DrawStr,unsigned char typid,char* udata)
{
    static USER_DATA_DESC desc_handler[256] = {
        INVALID_desc, M_SP_NA_desc, M_SP_TA_desc, M_DP_NA_desc, M_DP_TA_desc,  /* 0 - 4 */
        M_ST_NA_desc, M_ST_TA_desc, M_BO_NA_desc, M_BO_TA_desc, M_ME_NA_desc,  /* 5 - 9 */
        M_ME_TA_desc, M_ME_NB_desc, M_ME_TB_desc, M_ME_NC_desc, M_ME_TC_desc,  /* 10 - 14 */
        M_IT_NA_desc, M_IT_TA_desc, M_EP_TA_desc, M_EP_TB_desc, M_EP_TC_desc,  /* 15 - 19 */
        M_PS_NA_desc, M_ME_ND_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 20 - 24 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 25 - 29 */
        M_SP_TB_desc, M_DP_TB_desc, M_ST_TB_desc, M_BO_TB_desc, M_ME_TD_desc,  /* 30 - 34 */
        M_ME_TE_desc, M_ME_TF_desc, M_IT_TB_desc, M_EP_TD_desc, M_EP_TE_desc,  /* 35 - 39 */
        M_EP_TF_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 40 - 44 */
        C_SC_NA_desc, C_DC_NA_desc, C_RC_NA_desc, C_SE_NA_desc, C_SE_NB_desc,  /* 45 - 49 */
        C_SE_NC_desc, C_BO_NA_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 50 - 54 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 55 - 59 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 60 - 64 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 65 - 69 */
        M_EI_NA_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 70 - 74 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 75 - 79 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 80 - 84 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 85 - 89 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 90 - 94 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 95 - 99 */
        C_IC_NA_desc, C_CI_NA_desc, INVALID_desc, C_CS_NA_desc, C_TS_NA_desc,  /* 100 - 104 */
        C_RP_NA_desc, C_CD_NA_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 105 - 109 */
        P_ME_NA_desc, P_ME_NB_desc, P_ME_NC_desc, P_AC_NA_desc, INVALID_desc,  /* 110 - 114 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 115 - 119 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 120 - 124 */															   
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 125 - 129 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 130 - 134 */
        INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 135 - 139 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 140 - 144 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 145 - 149 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 150 - 154 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 155 - 159 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 160 - 164 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 165 - 169 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 170 - 174 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 175 - 179 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 180 - 184 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 185 - 189 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 190 - 194 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 195 - 199 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 200 - 204 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 205 - 209 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 210 - 214 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 215 - 219 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 220 - 224 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 225 - 229 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 230 - 234 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 235 - 239 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 240 - 244 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 245 - 249 */
		INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc, INVALID_desc,  /* 250 - 254 */
		INVALID_desc														   /* 255 -     */
    };

    return desc_handler[typid](DrawStr,typid,udata);
}

size_t SizeOfType(unsigned char typid)
{
    static size_t typesize[256] = {
                          0, sizeof(M_SP_NA_1_t), sizeof(M_SP_TA_1_t), sizeof(M_DP_NA_1_t), sizeof(M_DP_TA_1_t),        /* 0 - 4 */
        sizeof(M_ST_NA_1_t), sizeof(M_ST_TA_1_t), sizeof(M_BO_NA_1_t), sizeof(M_BO_TA_1_t), sizeof(M_ME_NA_1_t),        /* 5 - 9 */
        sizeof(M_ME_TA_1_t), sizeof(M_ME_NB_1_t), sizeof(M_ME_TB_1_t), sizeof(M_ME_NC_1_t), sizeof(M_ME_TC_1_t),        /* 10 - 14 */
        sizeof(M_IT_NA_1_t), sizeof(M_IT_TA_1_t), sizeof(M_EP_TA_1_t), sizeof(M_EP_TB_1_t), sizeof(M_EP_TC_1_t),        /* 15 - 19 */
        sizeof(M_PS_NA_1_t), sizeof(M_ME_ND_1_t),                   0,                   0,                   0,        /* 20 - 24 */
                          0,                   0,                   0,                   0,                   0,        /* 25 - 29 */
        sizeof(M_SP_TB_1_t), sizeof(M_DP_TB_1_t), sizeof(M_ST_TB_1_t), sizeof(M_BO_TB_1_t), sizeof(M_ME_TD_1_t),        /* 30 - 34 */
        sizeof(M_ME_TE_1_t), sizeof(M_ME_TF_1_t), sizeof(M_IT_TB_1_t), sizeof(M_EP_TD_1_t), sizeof(M_EP_TE_1_t),        /* 35 - 39 */
        sizeof(M_EP_TF_1_t),                   0,                   0,                   0,                   0,        /* 40 - 44 */
        sizeof(C_SC_NA_1_t), sizeof(C_DC_NA_1_t), sizeof(C_RC_NA_1_t), sizeof(C_SE_NA_1_t), sizeof(C_SE_NB_1_t),        /* 45 - 49 */
        sizeof(C_SE_NC_1_t), sizeof(C_BO_NA_1_t),                   0,                   0,                   0,        /* 50 - 54 */
                          0,                   0,                   0,					 0,					  0,        /* 55 - 59 */
						  0,				   0,					0,					 0,					  0,        /* 60 - 64 */
                          0,                   0,                   0,                   0,                   0,        /* 65 - 69 */
        sizeof(M_EI_NA_1_t),                   0,                   0,                   0,                   0,        /* 70 - 74 */
                          0,                   0,                   0,                   0,                   0,        /* 75 - 79 */
                          0,                   0,                   0,                   0,                   0,        /* 80 - 84 */
                          0,                   0,                   0,                   0,                   0,        /* 85 - 89 */
                          0,                   0,                   0,                   0,                   0,        /* 90 - 94 */
                          0,                   0,                   0,                   0,                   0,        /* 95 - 99 */
        sizeof(C_IC_NA_1_t), sizeof(C_CI_NA_1_t),                   0, sizeof(C_CS_NA_1_t), sizeof(C_TS_NA_1_t),        /* 100 - 104 */
        sizeof(C_RP_NA_1_t), sizeof(C_CD_NA_1_t),					0,                   0,                   0,        /* 105 - 109 */
        sizeof(P_ME_NA_1_t), sizeof(P_ME_NB_1_t), sizeof(P_ME_NC_1_t), sizeof(P_AC_NA_1_t),                   0,        /* 110 - 114 */
                          0,                   0,                   0,                   0,                   0,        /* 115 - 119 */
						  0,				   0,					0,					 0,					  0,        /* 120 - 124 */
						  0,				   0,                   0,					 0,					  0,		/* 125 - 129 */
						  0,				   0,                   0,					 0,					  0,		/* 130 - 134 */
						  0,				   0,                   0,					 0,					  0,		/* 135 - 139 */
						  0,				   0,                   0,					 0,					  0,		/* 140 - 144 */
						  0,				   0,                   0,					 0,					  0,		/* 145 - 149 */
						  0,				   0,                   0,					 0,					  0,		/* 150 - 154 */
						  0,				   0,                   0,					 0,					  0,		/* 155 - 159 */
						  0,				   0,                   0,					 0,					  0,		/* 160 - 164 */
						  0,				   0,                   0,					 0,					  0,		/* 165 - 169 */
						  0,				   0,                   0,					 0,					  0,		/* 170 - 174 */
						  0,				   0,                   0,					 0,					  0,		/* 175 - 179 */
						  0,				   0,                   0,					 0,					  0,		/* 180 - 184 */
						  0,				   0,                   0,					 0,					  0,		/* 185 - 189 */
						  0,				   0,                   0,					 0,					  0,		/* 190 - 194 */
						  0,				   0,                   0,					 0,					  0,		/* 195 - 199 */
						  0,				   0,                   0,					 0,					  0,		/* 200 - 204 */
						  0,				   0,                   0,					 0,					  0,		/* 205 - 209 */
						  0,				   0,                   0,					 0,					  0,		/* 210 - 214 */
						  0,				   0,                   0,					 0,					  0,		/* 215 - 219 */
						  0,				   0,                   0,					 0,					  0,		/* 220 - 224 */
						  0,				   0,                   0,					 0,					  0,		/* 225 - 229 */
						  0,				   0,                   0,					 0,					  0,		/* 230 - 234 */
						  0,				   0,                   0,					 0,					  0,		/* 235 - 239 */
						  0,				   0,                   0,					 0,					  0,		/* 240 - 244 */
						  0,				   0,                   0,					 0,					  0,		/* 245 - 249 */
						  0,				   0,                   0,					 0,					  0,		/* 250 - 254 */
						  0																								/* 255 -     */

    };																												   

    return typesize[typid];
}
