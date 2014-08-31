#ifndef _RMS_ANALYZE_GW103_
#define _RMS_ANALYZE_GW103_

#include "AnalyzePub.h"

#define MAKEWORD(a, b) ((unsigned short)(((unsigned char)(a)) | ((unsigned short)((unsigned char)(b))) << 8))

#pragma pack(push,1)
struct Icontrol_t {   
	unsigned int   FLAG:1;
	unsigned int   TXNO:15;
	unsigned int   RXNR:1;
	unsigned int   RXNO:15;
};
struct Scontrol_t {   
	unsigned int   FLAG:2;
	unsigned int   RES:14;
	unsigned int   RXNR:1;
	unsigned int   RXNO:15;
};
struct Ucontrol_t {   
	unsigned int   FLAG:2;
	unsigned int   STARTVL:1;
	unsigned int   STARTCN:1;
	unsigned int   STOPVL:1;
	unsigned int   STOPCN:1;
	unsigned int   TESTFRVL:1;
	unsigned int   TESTFRCN:1;
	unsigned int   RES:24;
};

struct MEA {  
    unsigned short  OV:1;     /**< 溢出位 */
    unsigned short  ER:1;     /**< 差错位 */
    unsigned short  RES:1;    /**< 备用位 */
    unsigned short  mval:13;  /**< 被测值 */
};
struct ASDUADDR {
	unsigned short	cpu:3;
	unsigned short	area:5;
	unsigned short	addr:8;
};

typedef union CONTROL104 CONTROL104;      
union CONTROL104 {      
	unsigned int		 bits;
	Icontrol_t   I_parts;
	Scontrol_t   S_parts;
	Ucontrol_t   U_parts;
};

struct APCI
{
	unsigned char		sync;
	unsigned short		len;
	CONTROL104			control;
	unsigned char		type_id;
	VSQ					vsq;
	unsigned char		cause;
	ASDUADDR			asdu_addr;
	char				*data;
};
typedef struct APCI APCI;

struct ASDU1_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		dpi;
	CP56Time2a			tm56;
	unsigned char		sin;
};

struct ASDU2_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		dpi;
	CP16Time2a			tm16;
	unsigned short		fan;
	CP56Time2a			tm56;
	unsigned char		fpt;
};

struct ASDU4_t
{
	unsigned char		fun;
	unsigned char		inf;
	float				scl;
	CP16Time2a			tm16;
	unsigned short		fan;
	CP56Time2a			tm56;
};

struct ASDU6_t
{
	unsigned char		fun;
	unsigned char		inf;
	CP56Time2a			tm56;
};

struct ASDU7_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		scn;
};

struct ASDU8_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		scn;
};

struct ASDU10_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	NGD_t				ngd;
};
struct ASDU10_DATASET_t
{
	GIN_t				gin;
	unsigned char		kod;
	GDD_t				gdd;
	char				*gid;
};
struct GDD23_t
{
	GDD_t				gdd;
	char				*gid;
};

struct ASDU12_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		fpt;
	unsigned char		jpt;
	float				scl;
	unsigned char		dpi;
	unsigned char		msl;
	unsigned char		msh;
	char				tmp[96];
	CP56Time2a			faultTm56;
	char				accName[64];
	char				gapName[40];
	CP56Time2a			recvTm56;
};

struct ASDU13_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	char				name[40];
	unsigned int		startPos;
};

struct ASDU14_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	unsigned char		cont;
	unsigned int		startPos;
	char				*data;
};

struct ASDU15_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	CP56Time2a			startTm56;
	CP56Time2a			stopTm56;
};

struct ASDU16_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	unsigned char		cont;
	unsigned short		num;
	CP56Time2a			startTm56;
	CP56Time2a			stopTm56;
};
struct ASDU16_LB_t
{
	unsigned char		iedNo;
	char				name[40];
	CP56Time2a			tm56;
	unsigned int		size;
};

struct ASDU17_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	unsigned char		type;
	CP56Time2a			startTm56;
	CP56Time2a			stopTm56;
};

struct ASDU18_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	unsigned char		cont;
	CP56Time2a			startTm56;
	CP56Time2a			stopTm56;
	unsigned char		num;
};
struct ASDU18_HIS_t
{
	unsigned char		iedNo;
	unsigned char		type;
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		dpi;
	CP16Time2a			ret;
	unsigned short		fan;
	float				scl;
	unsigned char		fpt;
	CP56Time2a			tm56;
};

struct ASDU42_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		dpi;
};

struct ASDU101_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	CP56Time2a			startTm56;
	CP56Time2a			stopTm56;
	char				folder[100];
};

struct ASDU102_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	unsigned char		cont;
	unsigned short		num;
	CP56Time2a			startTm56;
	CP56Time2a			stopTm56;
};
struct ASDU102_FILE_t
{
	char				name[100];
	CP56Time2a			tm56;
	unsigned int		length;
};

struct ASDU103_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	char				name[100];
	unsigned int		startPos;
};

struct ASDU104_t
{
	unsigned char		fun;
	unsigned char		inf;
	unsigned char		rii;
	unsigned char		cont;
	unsigned int		startPos;
	char				name[100];
	CP56Time2a			tm56;
	unsigned int		length;
	char				*data;
};
#pragma pack(pop)

bool Analyze_GW(char* DrawStr,unsigned int Len,char* Data);
bool Analyze_GW_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU1_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU2_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU4_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU6_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU7_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU8_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU10_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU12_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU13_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU14_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU15_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU16_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU17_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU18_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU21_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU42_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU101_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU102_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU103_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU104_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU105_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU106_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU107_UserData(char* DrawStr,APCI* apci);
void Analyze_ASDU108_UserData(char* DrawStr,APCI* apci);

#endif //_RMS_ANALYZE_GW103_
