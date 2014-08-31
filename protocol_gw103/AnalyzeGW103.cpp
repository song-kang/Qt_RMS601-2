#include "AnalyzeGW103.h"

char descType[256][128] = {
	/*  0-  4*/	{"未知类型"},{"异常告警/自检/开关量变位/压板状态变位/通讯状态变位,根据FUN/INF判断"},{"保护动作事件信息"},{"未知类型"},{"故障测距值"},
	/*  5-  9*/	{"未知类型"},{"时间同步"},{"总召唤"},{"总召唤结束"},{"未知类型"},
	/* 10- 14*/	{"通用分类数据"},{"未知类型"},{"简要录波报告"},{"召唤录波文件"},{"上送录波文件"},
	/* 15- 19*/	{"召唤录波文件列表"},{"上送录波文件列表"},{"召唤装置故障历史信息"},{"上送装置故障历史信息"},{"未知类型"},
	/* 20- 24*/	{"未知类型"},{"通用分类命令"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 25- 29*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 30- 34*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 35- 39*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 40- 44*/	{"未知类型"},{"未知类型"},{"总召唤应答,上送双点信息"},{"未知类型"},{"未知类型"},
	/* 45- 49*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 50- 54*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 55- 59*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 60- 64*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 65- 69*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 70- 74*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 75- 79*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 80- 84*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 85- 89*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 90- 94*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/* 95- 99*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/*100-104*/	{"未知类型"},{"召唤一般文件列表"},{"上送一般文件列表"},{"召唤一般文件"},{"上送一般文件"},
	/*105-109*/	{"下传一般文件信息"},{"下传文件信息确认"},{"下传一般文件内容"},{"下传文件内容确认"},{"未知类型"},
	/*110-114*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/*115-119*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/*120-124*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/*125-129*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/*130-134*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
	/*135-139*/	{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},{"未知类型"},
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

char descCause[50][64] = {
	/*  0-  4*/	{"未知原因"},{"突发"},{"循环"},{"复位帧计数位"},{"复位通讯单元"},
	/*  5-  9*/	{"启动/重新启动"},{"电源合上"},{"测试模式"},{"时间同步"},{"总召唤"},
	/* 10- 14*/	{"总召唤终止"},{"当地操作"},{"远方操作"},{"未知原因"},{"未知原因"},
	/* 15- 19*/	{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},
	/* 20- 24*/	{"一般命令/命令肯定认可"},{"命令否定认可"},{"未知原因"},{"未知原因"},{"未知原因"},
	/* 25- 29*/	{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},
	/* 30- 34*/	{"未知原因"},{"扰动数据的传输"},{"未知原因"},{"未知原因"},{"未知原因"},
	/* 35- 39*/	{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},
	/* 40- 41*/	{"通用分类写命令/通用分类写命令肯定认可"},{"通用分类写命令否定认可"},
	/* 42- 44*/ {"通用分类读命令/通用分类读命令有效数据响应"},{"通用分类读命令无效数据响应"},{"通用分类写确认"},
	/* 45- 49*/	{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},{"未知原因"},
};

char descKod[256][32] = {
	/*  0-  4*/	{"未知类别"},{"实际值"},{"缺省值"},{"量程(最小值、最大值、步长)"},{"未知类别"},
	/*  5-  9*/	{"精度(n,m)"},{"因子"},{"参比"},{"列表"},{"量纲"},
	/* 10- 14*/	{"描述"},{"未知类别"},{"口令条目"},{"只读"},{"只写"},
	/* 15- 19*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"相应的功能类型和信息序号"},
	/* 20- 24*/	{"相应的事件"},{"列表的文本阵列"},{"列表的值阵列"},{"相关联的条目"},{"未知类别"},
	/* 25- 29*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 30- 34*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 35- 39*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 40- 44*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 45- 49*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 50- 54*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 55- 59*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 60- 64*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 65- 69*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 70- 74*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 75- 79*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 80- 84*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 85- 89*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 90- 94*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/* 95- 99*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*100-104*/	{"未知类别"},{"未知类别"},{"未知类别"},{"属性结构"},{"未知类别"},
	/*105-109*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*110-114*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*115-119*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*120-124*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*125-129*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*130-134*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*135-139*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*140-144*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*145-149*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*150-154*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*155-159*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*160-164*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*165-169*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*170-174*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*175-179*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*180-184*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*185-189*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*190-194*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*195-199*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*200-204*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*205-209*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*210-214*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*215-219*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*220-224*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*225-229*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*230-234*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*235-239*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*240-244*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*245-249*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*250-254*/	{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},{"未知类别"},
	/*255-   */	{"未知类别"}
};

char descGddType[256][32] = {
	/*  0-  4*/	{"无数据"},{"ASCII8位码"},{"成组8位串"},{"无符号整数"},{"整数"},
	/*  5-  9*/	{"无符号浮点数"},{"浮点数"},{"IEEE标准754短实数"},{"IEEE标准754实数"},{"双点信息"},
	/* 10- 14*/	{"单点信息"},{"带瞬变和差错的双点信息"},{"带品质描述词的被测值"},{"未知"},{"二进制时间"},
	/* 15- 19*/	{"通用分类标识序号"},{"相对时间"},{"功能类型和信息序号"},{"带时标的报文"},{"带相对时间的时标报文"},
	/* 20- 24*/	{"带相对时间的时标的被测值"},{"外部文本序号"},{"通用分类回答码"},{"数据结构"},{"索引"},
	/* 25- 29*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 30- 34*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 35- 39*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 40- 44*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 45- 49*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 50- 54*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 55- 59*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 60- 64*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 65- 69*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 70- 74*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 75- 79*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 80- 84*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 85- 89*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 90- 94*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/* 95- 99*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*100-104*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*105-109*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*110-114*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*115-119*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*120-124*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*125-129*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*130-134*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*135-139*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*140-144*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*145-149*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*150-154*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*155-159*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*160-164*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*165-169*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*170-174*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*175-179*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*180-184*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*185-189*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*190-194*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*195-199*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*200-204*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*205-209*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*210-214*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*215-219*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*220-224*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*225-229*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*230-234*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*235-239*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*240-244*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*245-249*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*250-254*/	{"未知"},{"未知"},{"未知"},{"未知"},{"未知"},
	/*255-   */	{"未知"}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool cp16time_nw_desc(char* DrawStr,CP16Time2a* tm16)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	
	szTemp.assign("相对时间: ");
	strcat(DrawStr,szTemp.c_str());
	
	sprintf(tmp,"%d秒%d毫秒 ",tm16->parts.ms/1000,tm16->parts.ms%1000);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
    return true;
}

bool cp56time_nw_desc(char* DrawStr,CP56Time2a* tm56)
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

bool ngd_desc(char* DrawStr,NGD_t* ngd)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	
	szTemp.assign("通用分类数据集: ");
	strcat(DrawStr,szTemp.c_str());
	
	sprintf(tmp,"数目: %d, 计数器位: %d, 后续状态位: %d ",ngd->parts.num,ngd->parts.count,ngd->parts.cont);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
    return true;
}

bool asdu10DataSet_Gdd23_desc(char* DrawStr,unsigned char size,char* gid)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	unsigned char	pos = 0;
	unsigned int	uiTemp = 0;
	int				iTemp = 0;
	float			fTemp = 0;
	GDD23_t			gdd23;
	char			*data = gid;
	char			ginTemp[1024] = {'\0'};

	while (pos < size)
	{
		memset(ginTemp,0,1024);
		memcpy(&gdd23,data,sizeof(GDD23_t));
		gdd23.gid = data + sizeof(GDD23_t) - sizeof(unsigned int);
		pos  = pos + sizeof(GDD23_t) + gdd23.gdd.size*gdd23.gdd.num - sizeof(unsigned int);
		data = data + sizeof(GDD23_t) + gdd23.gdd.size*gdd23.gdd.num - sizeof(unsigned int);

		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"  ├─数据类型: %s, 宽度: %d, 数目: %d, 后续状态: %d, ",
			descGddType[gdd23.gdd.type],gdd23.gdd.size,gdd23.gdd.num,gdd23.gdd.cont);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());

		szTemp.assign("数据值: ");
		strcat(DrawStr,szTemp.c_str());
		switch(gdd23.gdd.type)
		{
		case GDD_NONE:
			szTemp.assign("无 ");
			break;
		case GDD_ASCII:
			memcpy(ginTemp,gdd23.gid,gdd23.gdd.size*gdd23.gdd.num);
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%s ",ginTemp);
			szTemp.assign(tmp);
			break;
		case GDD_UI:
			memcpy(&uiTemp,gdd23.gid,gdd23.gdd.size*gdd23.gdd.num);
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%u ",uiTemp);
			szTemp.assign(tmp);
			break;
		case GDD_I:
			memcpy(&iTemp,gdd23.gid,gdd23.gdd.size*gdd23.gdd.num);
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%d ",iTemp);
			szTemp.assign(tmp);
			break;
		case GDD_UF:
		case GDD_F:
		case GDD_IEEE754SP:
			memcpy(&fTemp,gdd23.gid,gdd23.gdd.size*gdd23.gdd.num);
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%f ",fTemp);
			szTemp.assign(tmp);
			break;
		case GDD_IEEE754DP:
			memcpy(&fTemp,gdd23.gid,gdd23.gdd.size*gdd23.gdd.num);
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%lf ",fTemp);
			szTemp.assign(tmp);
			break;
		default:
			szTemp.assign("此类型未处理 ");
			break;
		}
		strcat(DrawStr,szTemp.c_str());
		szTemp.assign("\r\n");
		strcat(DrawStr,szTemp.c_str());
	}

	return true;
}

bool asdu10DataSet_desc(char* DrawStr,ASDU10_DATASET_t* dataSet)
{
	int				i;
	unsigned int	uiTemp = 0;
	int				iTemp = 0;
	float			fTemp = 0;
	string			szTemp;
	char			ginTemp[1024] = {'\0'};	
	char			tmp[512] = {'\0'};
	
	sprintf(tmp,"组号: %d, 条目号: %d, 类别: %s, 数据类型: %s, 宽度: %d, 数目: %d, 后续状态: %d, ",
		dataSet->gin.group,dataSet->gin.entry,descKod[dataSet->kod],descGddType[dataSet->gdd.type],dataSet->gdd.size,dataSet->gdd.num,dataSet->gdd.cont);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	szTemp.assign("数据值: ");
	strcat(DrawStr,szTemp.c_str());
	switch(dataSet->gdd.type)
	{
	case GDD_NONE:
		szTemp.assign("无 ");
		break;
	case GDD_ASCII:
		memcpy(ginTemp,dataSet->gid,dataSet->gdd.size*dataSet->gdd.num);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%s ",ginTemp);
		szTemp.assign(tmp);
		break;
	case GDD_BSI:
		for (i = 0; i < dataSet->gdd.size*dataSet->gdd.num; i++)
		{
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%02xH ",dataSet->gid[i]);
			szTemp.assign(tmp);
			strcat(DrawStr,szTemp.c_str());
		}
		szTemp.clear();
		break;
	case GDD_UI:
		memcpy(&uiTemp,dataSet->gid,dataSet->gdd.size*dataSet->gdd.num);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%u ",uiTemp);
		szTemp.assign(tmp);
		break;
	case GDD_I:
		memcpy(&iTemp,dataSet->gid,dataSet->gdd.size*dataSet->gdd.num);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%d ",iTemp);
		szTemp.assign(tmp);
		break;
	case GDD_UF:
	case GDD_F:
	case GDD_IEEE754SP:
		memcpy(&fTemp,dataSet->gid,dataSet->gdd.size*dataSet->gdd.num);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%f ",fTemp);
		szTemp.assign(tmp);
		break;
	case GDD_IEEE754DP:
		memcpy(&fTemp,dataSet->gid,dataSet->gdd.size*dataSet->gdd.num);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%lf ",fTemp);
		szTemp.assign(tmp);
		break;
	case GDD_DPI:
		if (dataSet->gid[0] == 2)
			szTemp.assign("合 ");
		else if (dataSet->gid[0] == 1)
			szTemp.assign("分 ");
		else
			szTemp.assign("未知");
		break;
	case GDD_SPI:
		if (dataSet->gid[0] == 1)
			szTemp.assign("合 ");
		else if (dataSet->gid[0] == 0)
			szTemp.assign("分 ");
		else
			szTemp.assign("未知");
		break;
	case GDD_DPI_TR:
		if (dataSet->gid[0] == 2)
			szTemp.assign("合 ");
		else if (dataSet->gid[0] == 1)
			szTemp.assign("分 ");
		else if (dataSet->gid[0] == 0)
			szTemp.assign("瞬变 ");
		else if (dataSet->gid[0] == 3)
			szTemp.assign("差错 ");
		else
			szTemp.assign("未知");
		break;
	case GDD_MEQ:
		MEA		mea;
		memcpy(&mea,dataSet->gid,sizeof(MEA));
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%d ",mea.mval);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		if (mea.OV)
		{
			szTemp.assign("溢出 ");
			strcat(DrawStr,szTemp.c_str());
		}
		if (mea.ER)
		{
			szTemp.assign("差错 ");
			strcat(DrawStr,szTemp.c_str());
		}
		szTemp.clear();
		break;
	case GDD_BINTIME:
		CP56Time2a tm56;
		memcpy(&tm56,dataSet->gid,sizeof(CP56Time2a));
		cp56time_nw_desc(DrawStr,&tm56);
		szTemp.clear();
		break;
	case GDD_RET:
		CP16Time2a tm16;
		memcpy(&tm16,dataSet->gid,sizeof(CP16Time2a));
		cp16time_nw_desc(DrawStr,&tm16);
		szTemp.clear();
		break;
	case GDD_STRUCTURE:
		szTemp.assign("\r\n");
		strcat(DrawStr,szTemp.c_str());
		asdu10DataSet_Gdd23_desc(DrawStr,dataSet->gdd.size,dataSet->gid);
		szTemp.clear();
		break;
	default:
		szTemp.assign("此类型未处理 ");
		break;
	}
	strcat(DrawStr,szTemp.c_str());
	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());
	
    return true;
}

bool Analyze_GW(char* DrawStr,unsigned int Len,char* Data)
{
	char			tmp[512] = {'\0'};
	string			szTemp;
	APCI			apci;

	if (Data[0] == 0x68 && (unsigned short)Len == (unsigned short)(MAKEWORD(Data[1],Data[2])) + 3)
	{
		memcpy(&apci,Data,sizeof(APCI));
		apci.data = &Data[sizeof(APCI)-4];

		if (apci.control.U_parts.FLAG == 0x03)
		{
			szTemp.assign("U格式报文 ");
			strcat(DrawStr,szTemp.c_str());

			if (apci.control.U_parts.STARTVL)
			{
				szTemp.assign("启动生效 ");
				strcat(DrawStr,szTemp.c_str());
			}
			if (apci.control.U_parts.STARTCN)
			{
				szTemp.assign("启动确认 ");
				strcat(DrawStr,szTemp.c_str());
			}
			if (apci.control.U_parts.STOPVL)
			{
				szTemp.assign("停止生效 ");
				strcat(DrawStr,szTemp.c_str());
			}
			if (apci.control.U_parts.STOPCN)
			{
				szTemp.assign("停止确认 ");
				strcat(DrawStr,szTemp.c_str());
			}
			if (apci.control.U_parts.TESTFRVL)
			{
				szTemp.assign("测试生效 ");
				strcat(DrawStr,szTemp.c_str());
			}
			if (apci.control.U_parts.TESTFRCN)
			{
				szTemp.assign("测试确认 ");
				strcat(DrawStr,szTemp.c_str());
			}
			strcat(DrawStr,"\r\n");
			return true;
		}
		else if (apci.control.S_parts.FLAG == 0x01)
		{
			szTemp.assign("S格式报文 ");
			strcat(DrawStr,szTemp.c_str());

			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"接收序列号: %d ",apci.control.S_parts.RXNO);
			szTemp.assign(tmp);
			strcat(DrawStr,szTemp.c_str());
			strcat(DrawStr,"\r\n");
			return true;
		}
		else if (apci.control.I_parts.FLAG == 0x00)
		{
			szTemp.assign("I格式报文 ");
			strcat(DrawStr,szTemp.c_str());

			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"发送序列号: %d ",apci.control.I_parts.TXNO);
			szTemp.assign(tmp);
			strcat(DrawStr,szTemp.c_str());

			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"接收序列号: %d ",apci.control.I_parts.RXNO);
			szTemp.assign(tmp);
			strcat(DrawStr,szTemp.c_str());
		}
		else 
		{
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"控制域错误: %08XH ",apci.control.bits);
			szTemp.assign(tmp);
			strcat(DrawStr,szTemp.c_str());
			strcat(DrawStr,"\r\n");
			return true;
		}
		strcat(DrawStr,"\r\n");

		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%s(%d) ",descType[apci.type_id],apci.type_id);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		if (apci.type_id == ASDU1_M_TM_TA_3)
			strcat(DrawStr,"\r\n");

		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"可变结构限定词:%d(%02xH) ",apci.vsq.bits,apci.vsq.bits);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		
		if (apci.cause < 50) 
		{
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%s(%d) ",descCause[apci.cause],apci.cause);
			szTemp.assign(tmp);
		}
		else 
		{
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%s(%d) ","未知原因",apci.cause);
			szTemp.assign(tmp);
		}
		strcat(DrawStr,szTemp.c_str());
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"ASDU公共地址[装置地址:%d,CPU号:%d,定值区号:%d]",
			apci.asdu_addr.addr,apci.asdu_addr.cpu,apci.asdu_addr.area);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		strcat(DrawStr,"\r\n");

		Analyze_GW_UserData(DrawStr,&apci);
	}
	else
	{
		strcat(DrawStr,"报文格式不符合国家电网保护故障信息通信规范");
		return false;
	}

	return true;
}

bool Analyze_GW_UserData(char* DrawStr,APCI* apci)
{
	switch(apci->type_id)
	{
	case ASDU1_M_TM_TA_3:
		Analyze_ASDU1_UserData(DrawStr,apci);
		break;
	case ASDU2_M_TMR_TA_3:
		Analyze_ASDU2_UserData(DrawStr,apci);
		break;
	case ASDU4_M_TME_TA_3:
		Analyze_ASDU4_UserData(DrawStr,apci);
		break;
	case ASDU6_C_SYN_TA_3:
		Analyze_ASDU6_UserData(DrawStr,apci);
		break;
	case ASDU7_C_IGI_NA_3:
		Analyze_ASDU7_UserData(DrawStr,apci);
		break;
	case ASDU8_M_TGI_NA_3:
		Analyze_ASDU8_UserData(DrawStr,apci);
		break;
	case ASDU10_C_GD_NA_3:
		Analyze_ASDU10_UserData(DrawStr,apci);
		break;
	case ASDU12_M_LBS_NA_3:
		Analyze_ASDU12_UserData(DrawStr,apci);
		break;
	case ASDU13_C_LB_NA_3:
		Analyze_ASDU13_UserData(DrawStr,apci);
		break;
	case ASDU14_M_LB_NA_3:
		Analyze_ASDU14_UserData(DrawStr,apci);
		break;
	case ASDU15_C_LBL_NA_3:
		Analyze_ASDU15_UserData(DrawStr,apci);
		break;
	case ASDU16_M_LBL_NA_3:
		Analyze_ASDU16_UserData(DrawStr,apci);
		break;
	case ASDU17_C_HIS_NA_3:
		Analyze_ASDU17_UserData(DrawStr,apci);
		break;
	case ASDU18_M_HIS_NA_3:
		Analyze_ASDU18_UserData(DrawStr,apci);
		break;
	case ASDU42_M_TGI_NA_3:
		Analyze_ASDU42_UserData(DrawStr,apci);
		break;
	case ASDU101_C_FLL_NA_3:
		Analyze_ASDU101_UserData(DrawStr,apci);
		break;
	case ASDU102_M_FLL_NA_3:
		Analyze_ASDU102_UserData(DrawStr,apci);
		break;
	case ASDU103_C_FL_NA_3:
		Analyze_ASDU103_UserData(DrawStr,apci);
		break;
	case ASDU104_M_FL_NA_3:
		Analyze_ASDU104_UserData(DrawStr,apci);
		break;
	case ASDU105_C_DFI_NA_3:
		break;
	case ASDU106_M_DFI_NA_3:
		break;
	case ASDU107_C_DFD_NA_3:
		break;
	case ASDU108_M_DFD_NA_3:
		break;
	default:
		break;
	}

	return true;
}

void Analyze_ASDU1_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU1_t		asdu1;
	char		*data = apci->data;
	
	memcpy(&asdu1,data,sizeof(ASDU1_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d \r\n",asdu1.fun,asdu1.inf);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	

	switch(asdu1.dpi)
	{
	case 1:
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"状态: 分/复归/退出(%d) ",asdu1.dpi);
		szTemp.assign(tmp);
		break;
	case 2:
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"状态: 合/动作/投入(%d) ",asdu1.dpi);
		szTemp.assign(tmp);
		break;
	default:
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"状态: 不确定(%d) ",asdu1.dpi);
		szTemp.assign(tmp);
		break;
	}
	strcat(DrawStr,szTemp.c_str());	

	cp56time_nw_desc(DrawStr,&asdu1.tm56);

	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"附加信息SIN: %d \r\n",asdu1.sin);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
}

void Analyze_ASDU2_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU2_t		asdu2;
	char		*data = apci->data;
	
	memcpy(&asdu2,data,sizeof(ASDU2_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d \r\n",asdu2.fun,asdu2.inf);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	switch(asdu2.dpi)
	{
	case 1:
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"状态: 分/复归/退出(%d) ",asdu2.dpi);
		szTemp.assign(tmp);
		break;
	case 2:
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"状态: 合/动作/投入(%d) ",asdu2.dpi);
		szTemp.assign(tmp);
		break;
	default:
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"状态: 不确定(%d) ",asdu2.dpi);
		szTemp.assign(tmp);
		break;
	}
	strcat(DrawStr,szTemp.c_str());	

	cp16time_nw_desc(DrawStr,&asdu2.tm16);

	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"故障序号: %d \r\n",asdu2.fan);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	

	cp56time_nw_desc(DrawStr,&asdu2.tm56);
	
	szTemp.assign("故障相别及类型: ");
	strcat(DrawStr,szTemp.c_str());	
	if (asdu2.fpt & 0x01)
	{
		szTemp.assign("A相故障 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (asdu2.fpt & 0x02)
	{
		szTemp.assign("B相故障 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (asdu2.fpt & 0x04)
	{
		szTemp.assign("C相故障 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (asdu2.fpt & 0x08)
	{
		szTemp.assign("接地故障 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (asdu2.fpt & 0x10)
	{
		szTemp.assign("区外故障 ");
		strcat(DrawStr,szTemp.c_str());
	}
	if (asdu2.fpt & 0x80)
	{
		szTemp.assign("有效 ");
	}
	else
	{
		szTemp.assign("无/未知 ");
	}
	strcat(DrawStr,szTemp.c_str());
}

void Analyze_ASDU4_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU4_t		asdu4;
	char		*data = apci->data;
	
	memcpy(&asdu4,data,sizeof(ASDU4_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d \r\n",asdu4.fun,asdu4.inf);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	

	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"短路位置: %f ",asdu4.scl);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	
	cp16time_nw_desc(DrawStr,&asdu4.tm16);
	
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"故障序号: %d \r\n",asdu4.fan);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	cp56time_nw_desc(DrawStr,&asdu4.tm56);
}

void Analyze_ASDU6_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU6_t		asdu6;
	char		*data = apci->data;
	
	memcpy(&asdu6,data,sizeof(ASDU6_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d \r\n",asdu6.fun,asdu6.inf);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	cp56time_nw_desc(DrawStr,&asdu6.tm56);
	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());	
}

void Analyze_ASDU7_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU7_t		asdu7;
	char		*data = apci->data;
	
	memcpy(&asdu7,data,sizeof(ASDU7_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 扫描序号(scn): %d \r\n",asdu7.fun,asdu7.inf,asdu7.scn);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
}

void Analyze_ASDU8_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU8_t		asdu8;
	char		*data = apci->data;
	
	memcpy(&asdu8,data,sizeof(ASDU8_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 扫描序号(scn): %d \r\n",asdu8.fun,asdu8.inf,asdu8.scn);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
}

void Analyze_ASDU10_UserData(char* DrawStr,APCI* apci)
{
	char				tmp[512] = {'\0'};
	string				szTemp;
	ASDU10_t			asdu10;
	ASDU10_DATASET_t	dataSet;
	char				*data = apci->data;
	
	memcpy(&asdu10,data,sizeof(ASDU10_t));
	data += sizeof(ASDU10_t);
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu10.fun,asdu10.inf,asdu10.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	

	ngd_desc(DrawStr,&asdu10.ngd);
	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());

	for (int i = 0; i < asdu10.ngd.parts.num; i++)
	{
		memcpy(&dataSet,data,sizeof(ASDU10_DATASET_t));
		dataSet.gid = data + sizeof(ASDU10_DATASET_t) - sizeof(unsigned int);
		data = data + sizeof(ASDU10_DATASET_t) + dataSet.gdd.size*dataSet.gdd.num - sizeof(unsigned int);

		asdu10DataSet_desc(DrawStr,&dataSet);
	}
}

void Analyze_ASDU12_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU12_t	asdu12;
	char		name[128] = {'\0'};	
	char		*data = apci->data;

	memcpy(&asdu12,data,sizeof(ASDU12_t));

	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 故障相别(FPT): %d, 跳闸相别(JPT): %d, 故障距离(SCL): %f, 重合闸: %d, 重合闸时间: %dms\r\n",
		asdu12.fun,asdu12.inf,asdu12.fpt,asdu12.jpt,asdu12.scl,asdu12.dpi,asdu12.msh*256+asdu12.msl);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	szTemp.assign("故障");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu12.faultTm56);
	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());

	memcpy(name,asdu12.accName,sizeof(asdu12.accName));
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"录波文件名: %s\r\n",name);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	memset(name,0,sizeof(name));
	memcpy(name,asdu12.gapName,sizeof(asdu12.gapName));
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"间隔名: %s\r\n",name);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	szTemp.assign("子站接收");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu12.recvTm56);
}

void Analyze_ASDU13_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU13_t	asdu13;
	char		name[64] = {'\0'};	
	char		*data = apci->data;
	
	memcpy(&asdu13,data,sizeof(ASDU13_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu13.fun,asdu13.inf,asdu13.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	

	memcpy(name,asdu13.name,40);
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"录波文件名: %s, 起始传输位置: %d \r\n",name,asdu13.startPos);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
}

void Analyze_ASDU14_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU14_t	asdu14;
	char		*data = apci->data;
	
	memcpy(&asdu14,data,sizeof(ASDU14_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu14.fun,asdu14.inf,asdu14.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"后续位标志: %d, 起始传输位置: %d, 录波文件内容: ...... \r\n",asdu14.cont,asdu14.startPos);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
}

void Analyze_ASDU15_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU15_t	asdu15;
	char		*data = apci->data;
	
	memcpy(&asdu15,data,sizeof(ASDU15_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu15.fun,asdu15.inf,asdu15.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	szTemp.assign("开始");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu15.startTm56);

	szTemp.assign("结束");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu15.stopTm56);

	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());
}

void Analyze_ASDU16_UserData(char* DrawStr,APCI* apci)
{
	int			i;
	string		szTemp;
	ASDU16_t	asdu16;
	ASDU16_LB_t	asdu16Lb;
	char		name[64] = {'\0'};
	char		*data = apci->data;
	char		tmp[512] = {'\0'};
	
	memcpy(&asdu16,data,sizeof(ASDU16_t));
	data = data + sizeof(ASDU16_t);
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu16.fun,asdu16.inf,asdu16.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"后续位标志: %d, 录波文件个数: %d, ",asdu16.cont,asdu16.num);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	szTemp.assign("开始");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu16.startTm56);
	szTemp.assign("结束");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu16.stopTm56);
	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());

	for (i = 0; i < asdu16.num; i++)
	{
		memcpy(&asdu16Lb,data,sizeof(ASDU16_LB_t));
		data += sizeof(ASDU16_LB_t);
		memset(name,0,40);
		memcpy(name,asdu16Lb.name,40);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"├─录波装置地址: %d, 录波名称: %s, 大小: %d, ",asdu16Lb.iedNo,name,asdu16Lb.size);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		szTemp.assign("故障");
		strcat(DrawStr,szTemp.c_str());
		cp56time_nw_desc(DrawStr,&asdu16Lb.tm56);
		szTemp.assign("\r\n");
		strcat(DrawStr,szTemp.c_str());
	}
}

void Analyze_ASDU17_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU17_t	asdu17;
	char		*data = apci->data;
	
	memcpy(&asdu17,data,sizeof(ASDU17_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符: %d, 事件类型: ",asdu17.fun,asdu17.inf,asdu17.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	

	switch(asdu17.type)
	{
	case 1:
		szTemp.assign("召唤故障动作信息 ");
		break;
	case 2:
		szTemp.assign("装置自检信息 ");
		break;
	case 3:
		szTemp.assign("遥信变位信息 ");
		break;
	case 4:
		szTemp.assign("SOE信息 ");
		break;
	case 5:
		szTemp.assign("故障测距事件 ");
		break;
	case 6:
		szTemp.assign("告警事件 ");
		break;
	case 255:
		szTemp.assign("装置所有事件 ");
		break;
	default:
		szTemp.assign("保留 ");
		break;
	}
	strcat(DrawStr,szTemp.c_str());	
	
	szTemp.assign("开始");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu17.startTm56);
	
	szTemp.assign("结束");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu17.stopTm56);
	
	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());
}

void Analyze_ASDU18_UserData(char* DrawStr,APCI* apci)
{
	int				i;
	string			szTemp;
	ASDU18_t		asdu18;
	ASDU18_HIS_t	asdu18His;
	char			*data = apci->data;
	char			tmp[512] = {'\0'};
	
	memcpy(&asdu18,data,sizeof(ASDU18_t));
	data = data + sizeof(ASDU18_t);
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符: %d, 后续位标志: %d \r\n",
		asdu18.fun,asdu18.inf,asdu18.rii,asdu18.cont);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	szTemp.assign("开始");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu18.startTm56);
	szTemp.assign("结束");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu18.stopTm56);
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"历史条目数: %d \r\n",asdu18.num);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	for (i = 0; i < asdu18.num; i++)
	{
		memcpy(&asdu18His,data,sizeof(ASDU18_HIS_t));
		data = data + sizeof(ASDU18_HIS_t);

		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"├─装置地址: %d, 事件类型: ",asdu18His.iedNo);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		switch(asdu18His.type)
		{
		case 1:
			szTemp.assign("召唤故障动作信息 ");
			break;
		case 2:
			szTemp.assign("装置自检信息 ");
			break;
		case 3:
			szTemp.assign("遥信变位信息 ");
			break;
		case 4:
			szTemp.assign("SOE信息 ");
			break;
		case 5:
			szTemp.assign("故障测距事件 ");
			break;
		case 6:
			szTemp.assign("告警事件 ");
			break;
		case 255:
			szTemp.assign("装置所有事件 ");
			break;
		default:
			szTemp.assign("保留 ");
			break;
		}
		strcat(DrawStr,szTemp.c_str());	
		
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 双点信息: %d ",asdu18His.fun,asdu18His.inf,asdu18His.dpi);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		
		szTemp.assign("相对");
		strcat(DrawStr,szTemp.c_str());
		cp16time_nw_desc(DrawStr,&asdu18His.ret);

		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"故障序号: %d, 短路位置: %f, 故障相别: %d ",asdu18His.fan,asdu18His.scl,asdu18His.fpt);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
		
		szTemp.assign("故障");
		strcat(DrawStr,szTemp.c_str());
		cp56time_nw_desc(DrawStr,&asdu18His.tm56);
		
		szTemp.assign("\r\n");
		strcat(DrawStr,szTemp.c_str());
	}
}

void Analyze_ASDU42_UserData(char* DrawStr,APCI* apci)
{
	char				tmp[512] = {'\0'};
	string				szTemp;
	unsigned short		pos = sizeof(APCI) - sizeof(unsigned int);
	unsigned short		len = apci->len + 3/*syn(1)+len(2)*/ - 1/*scn(1)*/;
	ASDU42_t			asdu42;
	char				*data = apci->data;
	
	while(pos < len)
	{
		memcpy(&asdu42,data,sizeof(ASDU42_t));
		data += sizeof(ASDU42_t);
		pos  += sizeof(ASDU42_t);
		
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d ",asdu42.fun,asdu42.inf);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());	
		
		switch(asdu42.dpi)
		{
		case 1:
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"双点信息: 分(%d) ",asdu42.dpi);
			szTemp.assign(tmp);
			break;
		case 2:
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"双点信息: 合(%d) ",asdu42.dpi);
			szTemp.assign(tmp);
			break;
		default:
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"双点信息: 不确定(%d) ",asdu42.dpi);
			szTemp.assign(tmp);
			break;
		}
		strcat(DrawStr,szTemp.c_str());
		
		szTemp.assign("\r\n");
		strcat(DrawStr,szTemp.c_str());
	}
}

void Analyze_ASDU101_UserData(char* DrawStr,APCI* apci)
{
	char		tmp[512] = {'\0'};
	string		szTemp;
	ASDU101_t	asdu101;
	char		folder[128] = {'\0'};
	char		*data = apci->data;
	
	memcpy(&asdu101,data,sizeof(ASDU101_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu101.fun,asdu101.inf,asdu101.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	szTemp.assign("开始");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu101.startTm56);
	szTemp.assign("结束");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu101.stopTm56);
	
	memcpy(folder,asdu101.folder,100);
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"目录名及通配符: %s \r\n",folder);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
}

void Analyze_ASDU102_UserData(char* DrawStr,APCI* apci)
{
	int				i;
	string			szTemp;
	ASDU102_t		asdu102;
	ASDU102_FILE_t	asdu102File;
	char			name[128] = {'\0'};
	char			*data = apci->data;
	char			tmp[512] = {'\0'};
	
	memcpy(&asdu102,data,sizeof(ASDU102_t));
	data = data + sizeof(ASDU102_t);
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu102.fun,asdu102.inf,asdu102.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"后续位标志: %d, 一般文件个数: %d, ",asdu102.cont,asdu102.num);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
	szTemp.assign("开始");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu102.startTm56);
	szTemp.assign("结束");
	strcat(DrawStr,szTemp.c_str());
	cp56time_nw_desc(DrawStr,&asdu102.stopTm56);
	szTemp.assign("\r\n");
	strcat(DrawStr,szTemp.c_str());
	
	for (i = 0; i < asdu102.num; i++)
	{
		memcpy(&asdu102File,data,sizeof(ASDU102_FILE_t));
		data += sizeof(ASDU102_FILE_t);
		memset(name,0,100);
		memcpy(name,asdu102File.name,100);
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"├─文件名称: %s, ",name);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());

		cp56time_nw_desc(DrawStr,&asdu102File.tm56);

		memset(tmp,0,sizeof(tmp));
        sprintf(tmp,"长度: %d 字节",asdu102File.length);
		szTemp.assign(tmp);
		strcat(DrawStr,szTemp.c_str());
        szTemp.assign("\r\n");
        strcat(DrawStr,szTemp.c_str());
	}
}

void Analyze_ASDU103_UserData(char* DrawStr,APCI* apci)
{
	string		szTemp;
	ASDU103_t	asdu103;
	char		name[128] = {'\0'};
	char		*data = apci->data;
	char		tmp[512] = {'\0'};
	
	memcpy(&asdu103,data,sizeof(ASDU103_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d \r\n",asdu103.fun,asdu103.inf,asdu103.rii);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	memcpy(name,asdu103.name,100);
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"文件名: %s, 起始传输位置: %d \r\n",name,asdu103.startPos);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
}

void Analyze_ASDU104_UserData(char* DrawStr,APCI* apci)
{
	string		szTemp;
	ASDU104_t	asdu104;
	char		name[128] = {'\0'};
	char		*data = apci->data;
	char		tmp[512] = {'\0'};
	
	memcpy(&asdu104,data,sizeof(ASDU104_t));
	
	sprintf(tmp,"功能类型(Fun): %d, 信息序号(Inf): %d, 返回信息标识符(Rii): %d, 后续标志位: %d, 起始位置: %d \r\n",
		asdu104.fun,asdu104.inf,asdu104.rii,asdu104.cont,asdu104.startPos);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());	
	
	memcpy(name,asdu104.name,100);
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"文件名: %s ",name);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());

	cp56time_nw_desc(DrawStr,&asdu104.tm56);
	
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"长度: %d 字节, 内容: ...... \r\n",asdu104.length);
	szTemp.assign(tmp);
	strcat(DrawStr,szTemp.c_str());
}
