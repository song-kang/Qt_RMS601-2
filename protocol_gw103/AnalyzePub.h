#ifndef _RMS_ANALYZE_PUB_
#define _RMS_ANALYZE_PUB_

#include <QtGui>
#include <string>

using namespace std;

#define  INVALID_0  0	/**<   Invalid data type                                                       */
/* Process information in monitor direction (station-specific parameter)                               */
#define  M_SP_NA_1  1	/**<   Single-point information                                                */
#define  M_SP_TA_1  2	/**<   Single-point information with time tag                                  */
#define  M_DP_NA_1  3	/**<   Double-point information                                                */
#define  M_DP_TA_1  4	/**<   Double-point information with time tag                                  */
#define  M_ST_NA_1  5	/**<   Step position information                                               */
#define  M_ST_TA_1  6	/**<   Step position information with time tag                                 */
#define  M_BO_NA_1  7	/**<   Bitstring of 32 bit                                                     */
#define  M_BO_TA_1  8	/**<   Bitstring of 32 bit with time tag                                       */
#define  M_ME_NA_1  9	/**<   Measured value, normalised value                                        */
#define  M_ME_TA_1  10	/**<   Measured value, normalised value with time tag                          */
#define  M_ME_NB_1  11	/**<   Measured value, scaled value                                            */
#define  M_ME_TB_1  12	/**<   Measured value, scaled value with time tag                              */
#define  M_ME_NC_1  13	/**<   Measured value, short floating point value                              */
#define  M_ME_TC_1  14	/**<   Measured value, short floating point value with time tag                */
#define  M_IT_NA_1  15	/**<   Integrated totals                                                       */
#define  M_IT_TA_1  16	/**<   Integrated totals with time tag                                         */
#define  M_EP_TA_1  17	/**<   Event of protection equipment with time tag                             */
#define  M_EP_TB_1  18	/**<   Packed start events of protection equipment with time tag               */
#define  M_EP_TC_1  19	/**<   Packed output circuit information of protection equipment with time tag */
#define  M_PS_NA_1  20	/**<   Packed single point information with time tag                           */
#define  M_ME_ND_1  21	/**<   Measured value, normalised value without quality descriptor             */
#define  M_SP_TB_1  30	/**<   Single point information with time tag CP56Time2a                       */
#define  M_DP_TB_1  31	/**<   Double point information with time tag CP56Time2a                       */
#define  M_ST_TB_1  32	/**<   Step position information with time tag CP56Time2a                      */
#define  M_BO_TB_1  33	/**<   Bitstring of 32 bit with time tag CP56Time2a                            */
#define  M_ME_TD_1  34	/**<   Measured value, normalised value with time tag CP56Time2a               */
#define  M_ME_TE_1  35	/**<   Measured value, scaled value with time tag CP56Time2a                   */
#define  M_ME_TF_1  36	/**<   Measured value, short floating point value with time tag CP56Time2a     */ 
#define  M_IT_TB_1  37	/**<   Integrated totals with time tag CP56Time2a                              */
#define  M_EP_TD_1  38	/**<   Event of protection equipment with time tag CP56Time2a                  */
#define  M_EP_TE_1  39	/**<   Packed start events of protection equipment with time tag CP56Time2a    */
#define  M_EP_TF_1  40	/**<   Packed tripping events of protection equipment with time tag CP56Time2a */
/* Process information in control direction (station-specific parameter)                               */
#define  C_SC_NA_1  45	/**<   Single command                                                          */
#define  C_DC_NA_1  46	/**<   Double command                                                          */
#define  C_RC_NA_1  47	/**<   Regulating step command                                                 */
#define  C_SE_NA_1  48	/**<   Set point command, normalised value                                     */
#define  C_SE_NB_1  49	/**<   Set point command, scaled value                                         */
#define  C_SE_NC_1  50	/**<   Set point command, short floating point value                           */
#define  C_BO_NA_1  51	/**<   Bitstring of 32 bit                                                     */
#define  C_SC_TA_1  58	/**<   Single command with time tag CP56Time2a                                 */
#define  C_DC_TA_1  59	/**<   Double command with time tag CP56Time2a                                 */
#define  C_RC_TA_1  60	/**<   Regulating step command with time tag CP56Time2a                        */
#define  C_SE_TA_1  61	/**<   Set point command, normalised value with time tag CP56Time2a            */
#define  C_SE_TB_1  62	/**<   Set point command, scaled value with time tag CP56Time2a                */
#define  C_SE_TC_1  63	/**<   Set point command, short floating point value with time tag CP56Time2a  */
#define  C_BO_TA_1  64	/**<   Bitstring of 32 bit with time tag CP56Time2a                            */
/* System information in monitor direction (station-specific parameter)                                */
#define  M_EI_NA_1  70	/**<   End of initialisation                                                   */
/*   System information in control direction (station-specific parameter)                              */
#define  C_IC_NA_1  100	/**<   Interrogation command    定值召唤                                       */
#define  C_CI_NA_1  101	/**<   Counter interrogation command                                           */
#define  C_RD_NA_1  102	/**<   Read command                                                            */
#define  C_CS_NA_1  103	/**<   Clock synchronisation command                                           */
#define  C_TS_NA_1  104	/**<   Test command                                                            */
#define  C_RP_NA_1  105	/**<   Reset process command                                                   */
#define  C_CD_NA_1  106	/**<   Delay acquisition command                                               */
#define  C_TS_TA_1  107	/**<   Test command with time tag CP56Time2a                                   */
/* Parameter in control direction (station-specific parameter)                                         */
#define  P_ME_NA_1  110	/**<   Parameter of measured value, normalised value                           */
#define  P_ME_NB_1  111	/**<   Parameter of measured value, scaled value                               */
#define  P_ME_NC_1  112	/**<   Parameter of measured value, short floating point value                 */
#define  P_AC_NA_1  113	/**<   Active parameter                                                        */
/* File Transfer (station-specific parameter)                                                          */
#define  F_FR_NA_1  120	/**<   File ready                                                              */
#define  F_SR_NA_1  121	/**<   Section ready                                                           */
#define  F_SC_NA_1  122	/**<   Call directory, select file, call file, call section                    */
#define  F_LS_NA_1  123	/**<   Last section, last segment                                              */
#define  F_AF_NA_1  124	/**<   Ack file, ack section                                                   */
#define  F_SG_NA_1  125	/**<   Segment                                                                 */
#define  F_DR_TA_1  126	/**<   Directory                                                               */

#define	ASDU0_DONOTHING	         0	    /**< (00)保留                              */

/** IEC60870-5-103规约中的报文类型码 ( 主站 ==> IED )                              */
#define	ASDU6_C_SYN_TA_3		 6      /**< (06)时间同步                          */ 
#define	ASDU7_C_IGI_NA_3		 7      /**< (07)总查询                            */ 
#define	ASDU10_C_GD_NA_3		 10	    /**< (0A)通用分类数据                      */ 
#define ASDU13_C_LB_NA_3		 13		/**< (0D)召唤录波文件                      */
#define ASDU15_C_LBL_NA_3		 15		/**< (0F)召唤录波文件列表                  */
#define ASDU17_C_HIS_NA_3		 17		/**< (11)召唤装置故障历史信息              */
#define	ASDU20_C_GRC_NA_3		 20	    /**< (14)一般命令                          */
#define	ASDU21_C_GC_NA_3		 21	    /**< (15)通用分类命令                      */
#define	ASDU24_C_ODT_NA_3		 24	    /**< (18)扰动数据传输命令                  */
#define	ASDU25_C_ADT_NA_3   	 25	    /**< (19)扰动数据传输许可                  */
#define	ASDU101_C_FLL_NA_3   	 101    /**< (65)召唤一般文件列表                  */
#define	ASDU103_C_FL_NA_3   	 103    /**< (67)召唤一般文件                      */
#define	ASDU105_C_DFI_NA_3   	 105    /**< (69)下传一般文件的信息                */
#define	ASDU107_C_DFD_NA_3   	 107    /**< (6B)下传一般文件的内容                */

/** IEC60870-5-103规约中的报文类型码 ( IED ==> 主站 )                              */
#define	ASDU1_M_TM_TA_3			 1	    /**< (01)带时标的报文                      */
#define	ASDU2_M_TMR_TA_3		 2	    /**< (02)具有相对时间的带时标的报文        */ 
#define	ASDU3_M_MEI_NA_3		 3	    /**< (03)被测值I                           */ 
#define	ASDU4_M_TME_TA_3	     4	    /**< (04)具有相对时间的带时标的被测值      */ 
#define	ASDU5_M_IRC_NA_3 		 5	    /**< (05)标识                              */
#define	ASDU5_M_IRF_NA_3 		 5	    /**< (05)标识                              */
#define	ASDU5_M_IRS_NA_3 		 5	    /**< (05)标识                              */
#define	ASDU6_M_SYN_TA_3    	 6	    /**< (06)时间同步                          */ 
#define	ASDU8_M_TGI_NA_3		 8	    /**< (08)总查询终止                        */ 
#define	ASDU9_M_MEII_NA_3		 9	    /**< (09)被测值II                          */ 
#define	ASDU10_M_GD_NA_3    	 10	    /**< (0A)通用分类数据                      */
#define	ASDU10_M_GD_TA_3    	 10	    /**< (0A)通用分类数据                      */
#define	ASDU11_M_GI_NA_3  		 11	    /**< (0B)通用分类标识                      */
#define	ASDU11_M_GI_TA_3  		 11	    /**< (0B)通用分类标识                      */
#define	ASDU12_M_LBS_NA_3  		 12	    /**< (0C)简要录波报告                      */
#define ASDU14_M_LB_NA_3		 14		/**< (0E)上送录波文件                      */
#define ASDU16_M_LBL_NA_3		 16		/**< (10)上送录波文件列表                  */
#define ASDU18_M_HIS_NA_3		 18		/**< (12)上送装置故障历史信息              */
#define	ASDU23_M_LDR_TA_3		 23	    /**< (17)被记录的扰动表	                   */ 
#define	ASDU26_M_PTD_TA_3		 26	    /**< (1A)扰动数据传输准备就绪	           */ 
#define	ASDU27_M_RTC_NA_3	     27	    /**< (1B)被记录的通道传输准备就绪	       */ 
#define	ASDU28_M_RTT_NA_3	     28	    /**< (1C)带标志的状态变位传输准备就绪      */
#define	ASDU29_M_TOT_TA_3		 29	    /**< (1D)传送带标志的状态变位	           */
#define	ASDU30_M_TDN_NA_3		 30	    /**< (1E)传送扰动值	                       */
#define	ASDU31_M_EOT_NA_3		 31	    /**< (1F)传送结束	                       */
#define	ASDU42_M_TGI_NA_3		 42	    /**< (2A)总召唤应答,上送双点信息           */
#define	ASDU102_M_FLL_NA_3		 102    /**< (66)上送一般文件列表                  */
#define	ASDU104_M_FL_NA_3		 104    /**< (68)上送一般文件                      */
#define	ASDU106_M_DFI_NA_3		 106    /**< (6A)下传文件信息的确认                */
#define	ASDU108_M_DFD_NA_3		 108    /**< (6C)下传文件内容的确认                */

/** IEC60870-5-103传送原因(COT)( 主站 ==> IED )                                    */
#define COT_C_SYN_8              8	    /**< (08)时间同步                          */ 
#define COT_C_GI_START_9         9	    /**< (09)总查询启动                        */ 
#define COT_C_GRC_20             20     /**< (14)一般命令                          */ 
#define COT_C_DIST_DATA_31       31     /**< (1F)扰动数据的传输                    */
#define COT_C_G_WRITE_40         40     /**< (28)通用分类写命令                    */
#define COT_C_G_READ_42          42     /**< (2A)通用分类读命令                    */

/** IEC60870-5-103传送原因(COT) ( IED ==> 主站 )                                   */
#define COT_M_NOTUSED_0           0     /**< (00)not used                          */
#define COT_M_SPONT_1             1	    /**< (01)自发 spontaneous                  */
#define COT_M_CYC_2               2	    /**< (02)循环 periodic, cyclic             */
#define COT_M_RESET_FCB_3         3	    /**< (03)复位帧计数位                      */
#define COT_M_RESET_CU_4          4	    /**< (04)复位通信单元                      */
#define COT_M_RESET_5             5	    /**< (05)启动/重新启动                     */
#define COT_M_TEST_7              7	    /**< (07)测试模式                          */
#define COT_M_SYN_8               8	    /**< (08)时间同步                          */
#define COT_M_GI_9                9	    /**< (09)总查询                            */
#define COT_M_GI_END_10           10    /**< (0A)总查询停止                        */
#define COT_M_LOCAL_11            11    /**< (0B)当地操作                          */
#define COT_M_REMOTE_12           12    /**< (0C)远方操作                          */
#define COT_M_COMMAND_ACK_20      20    /**< (14)命令的肯定认可                    */
#define COT_M_COMMAND_NCK_21      21    /**< (15)命令的否定认可                    */
#define COT_M_DIST_DATA_31        31    /**< (1F)扰动数据的传送                    */
#define COT_M_G_WRITE_ACK_40      40    /**< (28)通用分类写命令的肯定认可          */ 
#define COT_M_G_WRITE_NCK_41      41    /**< (29)通用分类写命令的否定认可          */
#define COT_M_G_READ_ACK_42       42    /**< (2A)对通用分类读命令有效数据响应      */
#define COT_M_G_READ_NCK_43       43    /**< (2B)对通用分类读命令无效数据响应      */
#define COT_M_G_WRITE_CONFIRM_44  44    /**< (2C)通用分类写确认                    */
                                                                          
/** IEC60870-5-103专有范围功能类型(FUN)                                            */
/** ... ... etc.                                                                   */

/** IEC60870-5-103兼容范围功能类型(FUN)                                            */
#define FUN_DPR_128               128	/**< (80)距离保护                          */
#define FUN_OPR_160               160	/**< (A0)过流保护                          */
#define FUN_TDP_176               176	/**< (B0)变压器差动保护                    */
#define FUN_LDP_192               192	/**< (C0)线路差动保护                      */
#define FUN_GEN_254               254	/**< (FE)通用分类功能类型                  */
#define FUN_GLB_255               255	/**< (FF)全局功能类型                      */

/** IEC60870-5-103专有范围信息序号(INF)                                            */
/** ... ... etc.                                                                   */

/** IEC60870-5-103兼容范围信息序号(INF)( IED ==> 主站 )                            */
#define INF_M_GLB_ZERO            0	    /**< (00)总查询结束/时间同步               */
#define INF_M_RESET_FCB_02        2	    /**< (02)复位帧计数器                      */
#define INF_M_RESET_CU_03         3	    /**< (03)复位通信单元                      */
#define INF_M_RESTART_04          4	    /**< (04)启动/重新启动                     */
#define INF_M_POWERON_05          5	    /**< (05)电源合上                          */
#define INF_M_READ_G_TITLE_240    240   /**< (F0)读组全部被定义的组的标题          */
#define INF_M_READ_G_VALUE_241    241   /**< (F1)读一个组的全部条目的值或属性      */
#define INF_M_READ_E_LIST_243     243   /**< (F3)读单个条目的目录                  */
#define INF_M_READ_E_VALUE_244    244   /**< (F4)读单个条目的值或属性              */
#define INF_M_GI_245              245   /**< (F5)通用分类数据总查询终止            */
#define INF_M_WRITE_CONFIRM_249   249   /**< (F9)带确认的写条目                    */
#define INF_M_WRITE_EXECUTE_250   250   /**< (FA)带执行的写条目                    */
#define INF_M_WRITE_TERM_251      251   /**< (FB)带中止的写条目                    */
                                                                              
/** IEC60870-5-103兼容范围信息序号(INF)( 主站 ==> IED )                            */
#define INF_C_GLB_ZERO            0	    /**< (00)总查询（总召唤）启动/时间同步     */
#define INF_C_RESET_CU_03         3	    /**< (03)复位通信单元                      */
#define INF_C_AUTO_CLOSING_16     16	/**< (10)自动重合闸投入/推出               */
#define INF_C_REMOTE_PT_17        17	/**< (11)远方保护投入/推出                 */
#define INF_C_PT_18               18    /**< (12)保护投入/推出                     */
#define INF_C_LED_19              19	/**< (13)LED复位                           */
#define INF_C_ACT1_23             23	/**< (17)激活特性1                         */
#define INF_C_ACT2_24             24	/**< (18)激活特性2                         */
#define INF_C_ACT3_25             25	/**< (19)激活特性3                         */
#define INF_C_ACT4_26             26	/**< (1A)激活特性4                         */
#define INF_C_READ_G_TITLE_240    240   /**< (F0)读组全部被定义的组的标题          */
#define INF_C_READ_G_VALUE_241    241   /**< (F1)读一个组的全部条目的值或属性      */
#define INF_C_READ_E_LIST_243     243   /**< (F3)读单个条目的目录                  */
#define INF_C_READ_E_VALUE_244    244   /**< (F4)读单个条目的值或属性              */
#define INF_C_GI_245              245   /**< (F5)对通用分类数据的总查询（总召唤）  */
#define INF_C_WRITE_248           248   /**< (F8)写条目                            */
#define INF_C_WRITE_CONFIRM_249   249   /**< (F9)带确认的写条目                    */
#define INF_C_WRITE_EXECUTE_250   250   /**< (FA)带执行的写条目                    */
#define INF_C_WRITE_TERM_251      251   /**< (FB)写条目中止                        */

/** IEC60870-5-103通用分类数据描述                                                 */
#define GDD_NONE                  0     /**< 无数据                                */ 
#define GDD_ASCII                 1     /**< OS8ASCII                              */ 
#define GDD_BSI                   2     /**< 成组8位串                             */ 
#define GDD_UI                    3     /**< 无符号整数                            */ 
#define GDD_I                     4     /**< 有符号整数                            */ 
#define GDD_UF                    5     /**< 无符号浮点数                          */ 
#define GDD_F                     6     /**< 有符号浮点数                          */ 
#define GDD_IEEE754SP             7     /**< R32.23IEEE标准的754短实数             */ 
#define GDD_IEEE754DP             8     /**< R64.53IEEE标准的754实数               */ 
#define GDD_DPI                   9     /**< 双点信息                              */ 
#define GDD_SPI                   10    /**< 单点信息                              */ 
#define GDD_DPI_TR                11    /**< 带瞬变和差错的双点信息                */ 
#define GDD_MEQ                   12    /**< 带品质描述词的被测值                  */ 
#define GDD_BINTIME               14    /**< 二进制时间                            */ 
#define GDD_GIN                   15    /**< 通用分类标志序号                      */ 
#define GDD_RET                   16    /**< 相对时间                              */ 
#define GDD_FUN_INF               17    /**< 攻能类型和信息序号                    */ 
#define GDD_TIMETAG               18    /**< 带时标的报文                          */ 
#define GDD_RETTAG                19    /**< 带相对时间的时标报文                  */ 
#define GDD_ME_RETTAG             20    /**< 带相对时间的时标的被测值              */ 
#define GDD_EXT_FILENO            21    /**< 外部文本序号                          */ 
#define GDD_GRC                   22    /**< 通用分类回答码                        */ 
#define GDD_STRUCTURE             23    /**< 数据结构                              */
#define GDD_INDEX                 24    /**< 索引                                  */

/** IEC60870-5-103描述类别 */
#define KOD_NONE                  0     /**< 无所指定的描述类别(NO KOD SPECIFIED)  */
#define KOD_ACTUAL_VALUE          1     /**< 实际值(ACTUAL VALUE)                  */
#define KOD_DEFAULT_VALUE         2     /**< 缺省值(DEFAULT VALUE)                 */
#define KOD_RANGE                 3     /**< 量程(最小值、最大值、步长)(RANGE)     */
#define KOD_PRECISION             5     /**< 精度(n，m)(PRECISION n，m)            */
#define KOD_FACTOR                6     /**< 因子(FACTOR)                          */
#define KOD_REFERENCE             7     /**< ％参比(%REFERENCE)                    */
#define KOD_ENUMERATION           8     /**< 列表(ENUMERATION)                     */
#define KOD_DIMENSION             9     /**< 量纲(DIMENSION)                       */
#define KOD_DESCRIPTION           10    /**< 描述(DESCRIPTION)                     */
#define KOD_PASSWORD_ENTRY        12    /**< 口令条目(PASSWORD ENTRY)              */
#define KOD_IS_RDONLY             13    /**< 只读(IS READ ONLY)                    */
#define KOD_IS_WDONLY             14    /**< 只写(IS WRITE ONLY)                   */
#define KOD_TYP_INF               19    /**< 相应的功能类型和信息序号              */
#define KOD_EVENTS                20    /**< 相应的事件(CORRESPONDING EVENTS)      */
#define KOD_ENUM_TEXT_ARRAY       21    /**< 列表的文本阵列(ENUMERATED TEXT ARRAY) */
#define KOD_ENUM_VAL_ARRAY        22    /**< 列表的值阵列(ENUMERATED VALUE ARRAY)  */
#define KOD_RELATED_ENTRIES       23    /**< 相关联的条目(RELATED ENTRIES)         */
                                                                                                                    
/** IEC60870-5-103通用分类回答码                                                   */
#define GRC_ACCEPT                0     /**< 认可                                  */
#define GRC_INVALID_GIN           1     /**< 无效的通用分类标识序号(GIN)           */
#define GRC_NOREQS_DATA           2     /**< 不存在所请求的数据                    */
#define GRC_DATA_INVALID          3     /**< 数据不能用，过后再来一次              */
#define GRC_CONFIRM_ERR           4     /**< 改变设定时确认出错                    */
#define GRC_RANGE_OV              5     /**< 改变设定时超出量程                    */
#define GRC_ENTRY_OV              6     /**< 条目的范围太大                        */
#define GRC_COMMANDNUM_OV         7     /**< 太多的命令                            */
#define GRC_ENTRY_RDONLY          8     /**< 条目是只读的(IS READ ONLY)            */
#define GRC_NEED_PASSWORD         9     /**< 设定受口令保护                        */
#define GRC_OP_BUSY               10    /**< 当地设定在进行中                      */
#define GRC_ERR_DES               11    /**< 带有如下所描述的差错                  */

/** IEC60870-5-103命令类型                                                                                 */
#define TOO_FAULT_SELECT_1        1     /**< 故障的选择                                                    */ 
#define TOO_REQ_TD_2              2     /**< 请求扰动数据                                                  */ 
#define TOO_BRK_TD_3              3     /**< 中止扰动数据                                                  */ 
#define TOO_CHANNEL_REQ_8         8     /**< 通道的请求                                                    */ 
#define TOO_CHANNEL_BRK_9         9     /**< 通道的中止                                                    */ 
#define TOO_REQ_COS_16            16    /**< 请求带标志的状态变位                                          */ 
#define TOO_BRK_COS_17            17    /**< 中止带标志的状态变位                                          */ 
#define TOO_REQ_RECTD_24          24    /**< 请求被记录的扰动表                                            */ 
#define TOO_NOBRK_TD_END_32       32    /**< 不带中止的扰动数据传输的结束                                  */ 
#define TOO_CSBRK_TD_END_33       33    /**< 由控制系统所中止的扰动数据传输的结束                          */ 
#define TOO_PTBRK_TD_END_34       34    /**< 由继电保护设备(或间隔单元)所中止的扰动数据传输的结束          */ 
#define TOO_NOBRK_CT_END_35       35    /**< 不带中止的通道传输的结束                                      */ 
#define TOO_CSBRK_CT_END_36       36    /**< 由控制系统所中止的通道传输的结束                              */ 
#define TOO_PTBRK_CT_END_37       37    /**< 由继电保护设备(或间隔单元)所中止的通道传输的结束              */ 
#define TOO_NOBRK_COS_END_38      38    /**< 不带中止的带标志的状态变位的传输的结束                        */ 
#define TOO_CSBRK_COS_END_39      39    /**< 由控制系统所中止的带标志的状态变位的传输的结束                */ 
#define TOO_PTBRK_COS_END_40      40    /**< 由继电保护设备(或间隔单元)所中止的带标志的状态变位传输的结束  */ 
#define TOO_TRANS_TD_ACK_64       64    /**< 成功地扰动数据传输(肯定)                                      */ 
#define TOO_TRANS_TD_NCK_65       65    /**< 不成功地扰动数据传输(否定)                                    */ 
#define TOO_CHANNEL_ACK_66        66    /**< 成功地通道传输(肯定)                                          */ 
#define TOO_CHANNEL_NCK_67        67    /**< 不成功地通道传输(否定)                                        */ 
#define TOO_TCOS_ACK_68           68    /**< 成功地带标志的状态变位传输(肯定)                              */ 
#define TOO_TCOS_NCK_69           69    /**< 不成功地带标志的状态变位传输(否定)                            */

#define GDD_LEN         3
#define GIN_LEN         2
#define KOD_LEN         1

#pragma pack(1)
struct vsq_t {   
	unsigned char  NUM:7;
	unsigned char  SQ:1;
};
typedef union VSQ VSQ;      
union VSQ {      
	unsigned char		bits;
	vsq_t		parts;
};

struct cp56time2a_t {
    unsigned short   ms   :16;   /**< Milliseconds 0..59,999 ms     */
    unsigned short	 min  :6;    /**< Minutes 0..59 min             */
    unsigned short	 RES1 :1;    /**< Spare bit                     */
    unsigned short	 IV   :1;    /**< Invalid time                  */
    unsigned short	 hour :5;    /**< Hours 0..23h                  */
    unsigned short	 RES2 :2;    /**< Spare bit                     */
    unsigned short	 su   :1;    /**< Summer time                   */
    unsigned short	 day  :5;    /**< Days of month 1..31           */
    unsigned short	 week :3;    /**< Days of week 1..7(Not used 0) */
    unsigned short	 month:4;    /**< Months 1..12                  */
    unsigned short	 RES3 :4;    /**< Spare bit                     */
    unsigned char    year :7;    /**< Years 0..99 since 2000        */
    unsigned char    RES4 :1;    /**< Spare bit                     */
};
typedef union CP56Time2a CP56Time2a;
union CP56Time2a {
	unsigned char        bits[7];
	cp56time2a_t parts;
};

struct cp32time2a_t {
	unsigned short	 ms   :16;   /**< Milliseconds 0..59,999 ms     */
    unsigned short	 min  :6;    /**< Minutes 0..59 min             */
    unsigned short	 RES1 :1;    /**< Spare bit                     */
    unsigned short	 IV   :1;    /**< Invalid time                  */
    unsigned short	 hour :5;    /**< Hours 0..23h                  */
    unsigned short	 RES2 :2;    /**< Spare bit                     */
    unsigned short	 su   :1;    /**< Summer time                   */
};
typedef union CP32Time2a CP32Time2a;
union CP32Time2a {
	unsigned char        bits[4];
	cp32time2a_t parts;
};

struct cp24time2a_t {      
    unsigned short   ms   :16;   /**< Milliseconds 0..59,999 ms */
    unsigned char    min  :6;    /**< Minutes 0..59 min         */
    unsigned char    RES1 :1;    /**< Spare bit                 */
    unsigned char    IV   :1;    /**< Invalid time              */
};
typedef union CP24Time2a CP24Time2a;
union CP24Time2a {
	unsigned char        bits[3];
	cp24time2a_t parts;
};

struct cp16time2a_t {      
    unsigned short	  ms;        /**< Milliseconds 0..59,999 ms */
};
typedef union CP16Time2a CP16Time2a;
union CP16Time2a {
	unsigned char        bits[2];
	cp16time2a_t parts;
};

struct siq_t {      
	unsigned char  SPI:1;
	unsigned char  RES:3;
	unsigned char  BL:1;
	unsigned char  SB:1;
	unsigned char  NT:1;
	unsigned char  IV:1;
};
typedef union SIQ SIQ;      
union SIQ {      
	unsigned char   bits;
	siq_t   parts;
};

struct diq_t {      
	unsigned char  DPI:2;
	unsigned char  RES:2;
	unsigned char  BL:1;
	unsigned char  SB:1;
	unsigned char  NT:1;
	unsigned char  IV:1;
};
typedef union DIQ DIQ;      
union DIQ {      
	unsigned char  bits;
	diq_t  parts;
};

struct vti_t {      
	unsigned char  value:7;
	unsigned char  transient:1;  //设备在瞬间状态
};
typedef union VTI VTI;      
union VTI {      
	unsigned char  bits;
	vti_t  parts;
};

struct qds_t {      
	unsigned char  OV:1;
	unsigned char  RES:3;
	unsigned char  BL:1;
	unsigned char  SB:1;
	unsigned char  NT:1;
	unsigned char  IV:1;
};
typedef union QDS QDS;      
union QDS {      
	unsigned char  bits;
	qds_t  parts;
};

typedef union BSI BSI;
union BSI {      
	unsigned int	  bits;
	unsigned char     parts[4];
};

struct NVA {      
	short	  value;
};

struct sva_t {      
	unsigned short value:15;
	unsigned short sign:1;
};
typedef union SVA SVA;      
union SVA {      
	short	bits;
	sva_t   parts;
};

struct ieee754sp_t {
	unsigned int	 mant:23;
	unsigned int	 bexp:8;
	unsigned int	 sign:1;
};
typedef union IEEE754SP IEEE754SP;
union IEEE754SP {
	unsigned int	    bits;
	ieee754sp_t parts;
	float       f;
};

struct bcr_t {      
	unsigned int	 counter;
	unsigned char	 SQ:5;
	unsigned char    CY:1;
	unsigned char    CA:1;
	unsigned char    IV:1;
};
typedef union BCR BCR;      
union BCR {      
	unsigned char   bits[5];
	bcr_t   parts;
};

struct sep_t {      
	unsigned char ES:2;
	unsigned char RES:1;
	unsigned char EI:1;
	unsigned char BL:1;
	unsigned char SB:1;
	unsigned char NT:1;
	unsigned char IV:1;
};
typedef union SEP SEP;      
union SEP {      
	unsigned char   bits;
	sep_t   parts;
};

struct spe_t {      
	unsigned char GS:1;
	unsigned char SL1:1;
	unsigned char SL2:1;
	unsigned char SL3:1;
	unsigned char SIE:1;
	unsigned char SRD:1;
	unsigned char RES:2;
};
typedef union SPE SPE;      
union SPE {      
	unsigned char   bits;
	spe_t   parts;
};

struct qdp_t {      
	unsigned char  RES:3;
	unsigned char  EI:1;
	unsigned char  BL:1;
	unsigned char  SB:1;
	unsigned char  NT:1;
	unsigned char  IV:1;
};
typedef union QDP QDP;      
union QDP {      
	unsigned char   bits;
	qdp_t   parts;
};

struct oci_t {      
	unsigned char GC:1;
	unsigned char CL1:1;
	unsigned char CL2:1;
	unsigned char CL3:1;
	unsigned char RES:4;
};
typedef union OCI OCI;      
union OCI {      
	unsigned char   bits;
	oci_t   parts;
};

struct scd_t {      
	unsigned short	 ST;
	unsigned short	 CD;
};
typedef union SCD SCD;      
union SCD {      
	unsigned int	 bits;
	scd_t    parts;
};

struct sco_t {      
	unsigned char   SCS:1;
	unsigned char   RES:1;
	unsigned char   QU:5;
	unsigned char   SE:1;
};
typedef union SCO SCO;      
union SCO {      
	unsigned char   bits;
	sco_t   parts;
};

struct dco_t {      
	unsigned char   DCS:2;
	unsigned char   QU:5;
	unsigned char   SE:1;
};
typedef union DCO DCO;      
union DCO {      
	unsigned char   bits;
	dco_t   parts;
};

struct rco_t {      
	unsigned char   RCS:2;
	unsigned char   QU:5;
	unsigned char   SE:1;
};
typedef union RCO RCO;      
union RCO {      
	unsigned char   bits;
	rco_t   parts;
};

struct qos_t {      
	unsigned char   QL:7;
	unsigned char   SE:1;
};
typedef union QOSP QOSP;      
union QOSP {      
	unsigned char   bits;
	qos_t   parts;
};

struct coi_t {      
	unsigned char   cause:7;
	unsigned char   state:1;
};
typedef union COI COI;      
union COI {      
	unsigned char   bits;
	coi_t   parts;
};

struct QOI {      
	unsigned char   qoi;
};

struct qcc_t {
	unsigned char   RQT:6;
	unsigned char   FRZ:2;
};
typedef union QCC QCC;      
union QCC {      
	unsigned char   bits;
	qcc_t   parts;
};

struct FBP {
	unsigned short  HEX;
};

struct QRP {
	unsigned char  qrp;
};

struct qpm_t {
	unsigned char  KPA:6;
	unsigned char  LPC:1;
	unsigned char  POP:1;
};
typedef union QPM QPM;  
union QPM {      
	unsigned char   bits;
	qpm_t   parts;
};

struct QPA {
	unsigned char  qpa;
};

typedef unsigned short NOF;

typedef unsigned char NOS;

struct LOF {
	unsigned char lof[3];
};

typedef unsigned char LOS;

typedef unsigned char CHS; 

struct sof_t {
	unsigned char STATUS:5;
	unsigned char LFD:1;
	unsigned char FOR:1;
	unsigned char FA:1;
};
typedef union SOF SOF;      
union SOF {      
	unsigned char   bits;
	sof_t   parts;
};

struct frq_t {
	unsigned char FILES:7;
	unsigned char ACK:1;      /**< =0 ack; =1 deny */
};
typedef union FRQ FRQ;      
union FRQ {      
	unsigned char   bits;
	frq_t   parts;
};

struct srq_t {
	unsigned char  SESSION:7;
	unsigned char  READY:1;
};
typedef union SRQ SRQ;      
union SRQ {      
	unsigned char   bits;
	srq_t   parts;
};

struct scq_t {
	unsigned char  REQUEST:4;
	unsigned char  RESPONSE:4;
};
typedef union SCQ SCQ;      
union SCQ {      
	unsigned char   bits;
	scq_t   parts;
};

typedef unsigned char LSQ;

struct afq_t {
	unsigned char  ACK:4;
	unsigned char  STATUS:4;
};
typedef union AFQ AFQ;      
union AFQ {      
	unsigned char   bits;
	afq_t   parts;
};

//单点信息结构
struct M_SP_NA_1_t {
	SIQ      siq;
};
//带时标的单点信息结构
struct M_SP_TA_1_t {
	SIQ        siq;
	CP24Time2a tm;
};
//双点信息结构
struct M_DP_NA_1_t {
	DIQ      diq;
};
//带时标的双点信息结构
struct M_DP_TA_1_t {
	DIQ        diq;
	CP24Time2a tm;
};
//步位置信息结构
struct M_ST_NA_1_t {
	VTI      vti;
	QDS      qds;
};
//带时标的步位置信息结构
struct M_ST_TA_1_t {
	VTI        vti;
	QDS        qds;
	CP24Time2a tm;
};	
//32比特串结构
struct M_BO_NA_1_t {	
	BSI        bsi;
	QDS        qds;
};		
//带时标的32比特串结构
struct M_BO_TA_1_t {
	BSI        bsi;
	QDS        qds;
	CP24Time2a tm;
};
//测量值,规一化值结构
struct M_ME_NA_1_t {
	NVA      nva;
	QDS      qds;
};
//带时标的测量值,规一化值结构
struct M_ME_TA_1_t {
	NVA        nva;
	QDS        qds;
	CP24Time2a tm;
};	
//测量值,标度化值结构
struct M_ME_NB_1_t {
	SVA        sva;
	QDS        qds;
};	
//带时标的测量值,标度化值结构
struct M_ME_TB_1_t {
	SVA        sva;
	QDS        qds;
	CP24Time2a tm;
};	
//测量值,短浮点数结构
struct M_ME_NC_1_t {
	IEEE754SP std;
	QDS       qds;
};	
//带时标的测量值,短浮点数结构
struct M_ME_TC_1_t {
	IEEE754SP   std;
	QDS         qds;
	CP24Time2a  tm;
};	
//累计量结构
struct M_IT_NA_1_t {
	BCR        bcr;
};
//带时标的累计量结构
struct M_IT_TA_1_t {
	BCR         bcr;
	CP24Time2a  tm;
};	
//带时标的继电保护装置事件结构
struct M_EP_TA_1_t {
	SEP         sep;
	CP16Time2a  tm16;
	CP24Time2a  tm24;
};	
//带时标的继电保护装置成组启动事件结构
struct M_EP_TB_1_t {
	SPE         spe;
	QDP         qdp;
	CP16Time2a  tm16;
	CP24Time2a  tm24;
};	
//带时标的继电保护装置成组输出电路信息结构
struct M_EP_TC_1_t {
	OCI         oci;
	QDP         qdp;
	CP16Time2a  tm16;
	CP24Time2a  tm24;
};	
//带变位检出的成组单点信息结构
struct M_PS_NA_1_t {
	SCD      scd;
	QDS      qds;
};	
//测量值,不带品质描述词的规一化值结构
struct M_ME_ND_1_t {
	NVA      nva;
};
//带CP56Time2a时标的单点信息结构
struct M_SP_TB_1_t {
	SIQ        siq;
	CP56Time2a tm;
};	
//带CP56Time2a时标的双点信息结构
struct M_DP_TB_1_t {
	DIQ        diq;
	CP56Time2a tm;
};	
//带CP56Time2a时标的步位置信息结构
struct M_ST_TB_1_t {
	VTI        vti;
	QDS        qds;
	CP56Time2a tm;
};	
//带CP56Time2a时标的32比特串结构
struct M_BO_TB_1_t {
	BSI        bsi;
	QDS        qds;
	CP56Time2a tm;
};
//带CP56Time2a时标的测量值,规一化值结构
struct M_ME_TD_1_t {
	NVA        nva;
	QDS        qds;
	CP56Time2a tm;
};	
//带CP56Time2a时标的测量值,标度化值结构
struct M_ME_TE_1_t {
	SVA        sva;
	QDS        qds;
	CP56Time2a tm;
};	
//带CP56Time2a时标的测量值,短浮点数结构
struct M_ME_TF_1_t {
	IEEE754SP   std;
	QDS         qds;
	CP56Time2a  tm;
};	
//带CP56Time2a时标的累计量结构
struct M_IT_TB_1_t {
	BCR        bcr;
	CP56Time2a tm;
};	
//带CP56Time2a时标的继电保护装置事件结构
struct M_EP_TD_1_t {
	SEP         sep;
	CP16Time2a  tm16;
	CP56Time2a  tm56;
};	
//带CP56Time2a时标的继电保护装置成组启动事件结构
struct M_EP_TE_1_t {
	SPE         spe;
	QDP         qdp;
	CP16Time2a  tm16;
	CP56Time2a  tm56;
};	
//带CP56Time2a时标的继电保拌装置成组输出电路信息结构
struct M_EP_TF_1_t {
	OCI         oci;
	QDP         qdp;
	CP16Time2a  tm16;
	CP56Time2a  tm56;
};
//单点命令
struct C_SC_NA_1_t {
	SCO      sco;
};	
//双点命令
struct C_DC_NA_1_t {
	DCO      dco;
};	
//调节步命令
struct C_RC_NA_1_t {
	RCO      rco;
};	
//设定值命令,规一化值
struct C_SE_NA_1_t {
	NVA      nva;
	QOSP     qos;
};		
//设定值命令,标度化值
struct C_SE_NB_1_t {
	SVA      sva;
	QOSP     qos;
};	
//设定值命令，短浮点数
struct C_SE_NC_1_t {
	IEEE754SP std;
	QOSP      qos;
};	
//设定值命令,32比特串
struct C_BO_NA_1_t {
	BSI       bsi;
};
//初始化结束
struct M_EI_NA_1_t {
	COI      coi;
};	
//总召唤命令
struct C_IC_NA_1_t {
	QOI      qoi;
};	
//累计量召唤命令
struct C_CI_NA_1_t {
	QCC      qcc;
};	
//时钟同步命令
struct C_CS_NA_1_t {
	CP56Time2a tm;
};	
//测试命令
struct C_TS_NA_1_t {
	FBP      fbp;
};	
//复位进程命令
struct C_RP_NA_1_t {
	QRP      qrp;
};	
//收集传输延时
struct C_CD_NA_1_t {
	CP16Time2a  tm;
};	
//测量值参数,规一化值
struct P_ME_NA_1_t {
	NVA      nva;
	QPM      qpm;
};	
//测量值参数，标度化值
struct P_ME_NB_1_t {
	SVA      sva;
	QPM      qpm;
};	
//测量值参数,短浮点数
struct P_ME_NC_1_t {
	IEEE754SP std;
	QPM       qpm;
};	
//参数激活
struct P_AC_NA_1_t {
	QPA      qpa;
};
//文件淮备就绪
struct F_FR_NA_1_t {
	NOF		nof;
	LOF		lof;
	FRQ		frq;
};
//节淮备就绪
struct F_SR_NA_1_t {
	NOF     nof;
	NOS     nos;
	LOF		lof;
};	
//召唤目录,选择文件,召唤文件召唤节
struct F_SC_NA_1_t {
	NOF     nof;
	NOS     nos;
	SCQ     scq;
};	
//最后的节,最后的段
struct F_LS_NA_1_t {
	NOF     nof;
	NOS     nos;
    LSQ     lsq;
    CHS     chs;
};	
//认可文件,认可节
struct F_AF_NA_1_t {
	NOF     nof;
	NOS     nos;
    AFQ     afq;
};	
//段
struct F_SG_NA_1_t {
	NOF     nof;
	NOS     nos;
    LOS     los;
    unsigned char   seg[1];
};	
//目录
struct F_DR_TA_1_t {
	NOF        nof;
    LOF        lof;
	SOF        nos;
    CP56Time2a tm;
};	

/** IEC60870-5-103规约 */
struct GIN_t 
{      
    unsigned char  group;    /**< group */
    unsigned char  entry;    /**< entry */
};

struct GDD_t 
{      
    unsigned char  type;      /**< GDD type  */
    unsigned char  size;      /**< data size */
    unsigned char  num:7;     /**< number    */
    unsigned char  cont:1;    /**< continue  */
};

struct NGD 
{  
    unsigned char  num:6;    /**< number   */
    unsigned char  count:1;  /**< count    */
    unsigned char  cont:1;   /**< continue */
};
union NGD_t 
{  
    unsigned char  bits; 
    NGD    parts;
};
#pragma pack()

//接品函数
bool DescUserData(char* DrawStr,unsigned char typid,char* udata);
size_t SizeOfType(unsigned char typid);

#endif // _RMS_ANALYZE_PUB_
