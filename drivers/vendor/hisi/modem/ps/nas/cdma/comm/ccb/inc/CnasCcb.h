
#ifndef _CNAS_CCB_H_
#define _CNAS_CCB_H_

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"
#include "PsCommonDef.h"
#include "PsLogdef.h"
#include "PsTypeDef.h"
#include "cas_1x_access_ctrl_proc_nas_pif.h"

#include "CnasPrlParse.h"

#include "Nas_Mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define P_REV_IN_USE_7                                      (7)

#define P_REV_IN_USE_6                                      (6)


#define CNAS_CCB_MAX_HOME_SID_NID_LIST                      (20)

#define CNAS_CCB_ICCID_OCTET_LEN                            (10)

/* the byte num of EFRUIMID read from card  5 or 8*/
#define CNAS_CCB_EFRUIMID_OCTET_LEN_EIGHT                   (8)
#define CNAS_CCB_EFRUIMID_OCTET_LEN_FIVE                    (5)

#define CNAS_CCB_UIMID_OCTET_LEN                            (4)

#define CNAS_CCB_MAX_LOG_MSG_STATE_NUM                      (250)

#define CNAS_CCB_MAX_WHITE_LOCK_SYS_NUM                     (20)

#define CNAS_CCB_MAX_FREQ_NUM                               (20)

#define CNAS_CCB_1X_CALL_STATE_MAX_NUM                      (5)     /* match with the num of max xcc fsm entity */

#define CNAS_CCB_MEID_OCTET_LEN                             (7)
#define CNAS_CCB_ESN_OCTET_LEN                              (4)
#define CNAS_CCB_ESN_MEID_OCTET_LEN                         (8)
#define CNAS_CCB_MAX_AUTHDATA_USERNAME_LEN                  (253)   /* HRPD ppp AN鉴权用户名最长用户名，
                                                                       参考C.S0023-D section 3.4.53以及
                                                                       C.S0016-D,section 3.5.8.13,
                                                                       该长度理论最长为255-2(NAI长度以
                                                                       及鉴权算法保留字节占用的字节) */

#define CNAS_CCB_BYTE_LEN_8_BIT                             (8)

/* 根据 Pid以及usMsgName 构建EventType  */
/*
    约束: PID结构 Bit0~Bit11: 模块ID  Bit12~Bit15: 保留 Bit16~Bit19: CPU ID Bit20~Bit31: 保留
    例: PID: 0x000A0BCD  ->  0x0000ABCD  ->  0xABCD0000
        MSG: 0x0000abcd
        EVT: 0xABCDabcd
*/
#define CNAS_BuildEventType(ulSndPid,usMsgName) \
    (((VOS_UINT32)(/*lint -e778*/((ulSndPid) & 0xF0000)/*lint +e778*/ \
    | (((ulSndPid) & 0xFFF) << 4)) << 12) | (VOS_UINT16)(usMsgName))      /* 根据 Pid以及usMsgName 构建EventType  */

#define CNAS_MIN(x, y)\
        (((x)<(y))?(x):(y))

#define CNAS_MAX(x, y)\
        (((x)>(y))?(x):(y))

/*lint -e778 */
/*lint -e572 */
#ifndef CNAS_NTOHL                   /* 大小字节序转换*/
#if VOS_BYTE_ORDER==VOS_BIG_ENDIAN
#define CNAS_NTOHL(x)    (x)
#define CNAS_HTONL(x)    (x)
#define CNAS_NTOHS(x)    (x)
#define CNAS_HTONS(x)    (x)
#else
#define CNAS_NTOHL(x)    ((((x) & 0x000000ffU) << 24) | \
                         (((x) & 0x0000ff00U) <<  8) | \
                         (((x) & 0x00ff0000U) >>  8) | \
                         (((x) & 0xff000000U) >> 24))

#define CNAS_HTONL(x)    ((((x) & 0x000000ffU) << 24) | \
                         (((x) & 0x0000ff00U) <<  8) | \
                         (((x) & 0x00ff0000U) >>  8) | \
                         (((x) & 0xff000000U) >> 24))

#define CNAS_NTOHS(x)    ((((x) & 0x00ff) << 8) | \
                         (((x) & 0xff00) >> 8))

#define CNAS_HTONS(x)    ((((x) & 0x00ff) << 8) | \
                         (((x) & 0xff00) >> 8))
#endif  /* _BYTE_ORDER==_LITTLE_ENDIAN */
#endif
/*lint -e572 */
/*lint -e778 */

#define     CNAS_CAS_INVALID_SUBSTA     (0xFF)

#define CNAS_THOUSAND_MILLISECOND_UNIT                   (1000)                         /* 1000MS */


#if (FEATURE_ON == FEATURE_LTE)
#define CNAS_CCB_MAX_RAT_NUM                             (3)                     /* 当前支持的最大接入技术个数 */
#else
#define CNAS_CCB_MAX_RAT_NUM                             (2)                     /* 当前支持的最大接入技术个数 */
#endif

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
enum CNAS_CCB_CARD_STATUS_ENUM
{
    CNAS_CCB_CARD_STATUS_ABSENT         = 0x0000,
    CNAS_CCB_CARD_STATUS_UIM_PRESENT    = 0x0001,                   /* uim present */
    CNAS_CCB_CARD_STATUS_CSIM_PRESENT   = 0x0002,                   /* csim present */
    CNAS_CCB_CARD_STATUS_SIM_PRESENT    = 0x0003,                   /* sim present */
    CNAS_CCB_CARD_STATUS_USIM_PRESENT   = 0x0004,                   /* usim present */
    CNAS_CCB_CARD_STATUS_BUTT           = 0x0005
};
typedef VOS_UINT8 CNAS_CCB_CARD_STATUS_ENUM_UINT8;

/** ****************************************************************************
 * Name        : CNAS_CCB_PS_RATTYPE_ENUM
 *
 * Description :
 *******************************************************************************/
enum CNAS_CCB_PS_RATTYPE_ENUM
{
    CNAS_CCB_PS_RATTYPE_NULL            = 0x00000000,
    CNAS_CCB_PS_RATTYPE_1X              = 0x00000001,
    CNAS_CCB_PS_RATTYPE_HRPD            = 0x00000002,
    CNAS_CCB_PS_RATTYPE_EHRPD           = 0x00000003,
    CNAS_CCB_PS_RATTYPE_LTE             = 0x00000004,
    CNAS_CCB_PS_RATTYPE_BUTT            = 0x00000005
};
typedef VOS_UINT32 CNAS_CCB_PS_RATTYPE_ENUM_UINT32;


/** ****************************************************************************
 * Name        : CNAS_CCB_1X_CAS_STATE_ENUM_UINT8
 *
 * Description : When MS is in the different state, MS should perform
 * different actions.
 * Refers to  C.S0017 2.6.5.5
 *******************************************************************************/
enum CNAS_CCB_1X_CAS_STATE_ENUM
{
    CNAS_CCB_1X_CAS_INIT_STATE          = 0x00,
    CNAS_CCB_1X_CAS_IDLE_STATE          = 0x01,
    CNAS_CCB_1X_CAS_ACCESS_STATE        = 0x02,
    CNAS_CCB_1X_CAS_TCH_STATE           = 0x03,
    CNAS_CCB_1X_CAS_NO_SERVICE_STATE    = 0x04,
    CNAS_CCB_1X_CAS_STATE_BUTT          = 0x05
};
typedef VOS_UINT8 CNAS_CCB_1X_CAS_STATE_ENUM_UINT8;


/** ****************************************************************************
 * Name        : CNAS_CCB_1X_CAS_SUB_STATE_ENUM_UINT8
 *
 * Description : .
 * Refers to  C.S0017 2.6.5.5
 *******************************************************************************/
enum CNAS_CCB_1X_CAS_SUB_STATE_ENUM
{
    CNAS_CCB_1X_CAS_SUB_STATE_NULL                          = 0x00,
    CNAS_CCB_1X_CAS_INIT_DETERM_SUB_STATE                   = 0x01,
    CNAS_CCB_1X_CAS_PILOT_CH_ACQ_SUB_STATE                  = 0x02,
    CNAS_CCB_1X_CAS_SYNC_CH_ACQ_SUB_STATE                   = 0x03,
    CNAS_CCB_1X_CAS_TIMING_CHNG_SUB_STATE                   = 0x04,
    CNAS_CCB_1X_CAS_IDLE_SUB_STATE                          = 0x10,
    CNAS_CCB_1X_CAS_UPDATE_OVERHEAD_SUB_STATE               = 0x20,
    CNAS_CCB_1X_CAS_MOB_STATION_ORIG_ATTEMPT_SUB_STATE      = 0x21,
    CNAS_CCB_1X_CAS_PAG_RESP_SUB_STATE                      = 0x22,
    CNAS_CCB_1X_CAS_MOB_STATION_ORDR_RESP_SUB_STATE         = 0x23,
    CNAS_CCB_1X_CAS_REG_ACCESS_SUB_STATE                    = 0x24,
    CNAS_CCB_1X_CAS_MOB_STATION_MSG_TRANS_SUB_STATE         = 0x25,
    CNAS_CCB_1X_CAS_TCH_INIT_SUB_STATE                      = 0x30,
    CNAS_CCB_1X_CAS_REL_SUB_STATE                           = 0x31,
    CNAS_CCB_1X_CAS_WAIT_FOR_SERVICE_SUB_STATE              = 0x40,
    CNAS_CCB_1X_CAS_SUB_STATE_ENUM_BUTT                     = 0x41
};
typedef VOS_UINT8 CNAS_CCB_1X_CAS_SUB_STATE_ENUM_UINT8;

/** ****************************************************************************
 * Name        : CNAS_CCB_1X_CAS_P_REV_ENUM_UINT8
 *
 * Description : .
 * Refers to
 *******************************************************************************/
enum CNAS_CCB_1X_CAS_P_REV_ENUM
{
    CNAS_CCB_1X_CAS_P_REV_JST_008               = 0x01,
    CNAS_CCB_1X_CAS_P_REV_IS_95                 = 0x02,
    CNAS_CCB_1X_CAS_P_REV_IS_95A                = 0x03,
    CNAS_CCB_1X_CAS_P_REV_IS_95B_CUSTOM         = 0x04,
    CNAS_CCB_1X_CAS_P_REV_IS_95B                = 0x05,
    CNAS_CCB_1X_CAS_P_REV_IS_2000               = 0x06,
    CNAS_CCB_1X_CAS_P_REV_IS_2000A              = 0x07,

    CNAS_CCB_1X_CAS_P_REV_ENUM_BUTT
};
typedef VOS_UINT8 CNAS_CCB_1X_CAS_P_REV_ENUM_UINT8;



enum CNAS_CCB_1X_CALL_STATE_ENUM
{
    CNAS_CCB_1X_CALL_STATE_WAIT_FOR_ORDER,
    CNAS_CCB_1X_CALL_STATE_WAIT_FOR_ANSWER,
    CNAS_CCB_1X_CALL_STATE_CONVERSATION,
    CNAS_CCB_1X_CALL_STATE_IDLE,
    CNAS_CCB_1X_CALL_STATE_INCOMMING_CALL,
    CNAS_CCB_1X_CALL_STATE_ORIG_CALL,

    CNAS_CCB_1X_CALL_STATE_BUTT
};
typedef VOS_UINT8 CNAS_CCB_1X_CALL_STATE_ENUM_UINT8;



enum CNAS_CCB_1X_SO_TYPE_ENUM
{
    CNAS_CCB_1X_SO_TYPE_VOICE,
    CNAS_CCB_1X_SO_TYPE_SMS,
    CNAS_CCB_1X_SO_TYPE_PS,
    CNAS_CCB_1X_SO_TYPE_LOOPBACK,
    CNAS_CCB_1X_SO_TYPE_AGPS,
    CNAS_CCB_1X_SO_TYPE_BUTT
};
typedef VOS_UINT8 CNAS_CCB_1X_SO_TYPE_ENUM_UINT8;


enum CNAS_CCB_1X_RETURN_CAUSE_ENUM
{
    CNAS_CCB_1X_RETURN_CAUSE_NORMAL_ACCESS             = 0x00,
    CNAS_CCB_1X_RETURN_CAUSE_SYSTEM_NOT_ACQ            = 0x01,
    CNAS_CCB_1X_RETURN_CAUSE_PROTOCOL_MISMATCH         = 0x02,
    CNAS_CCB_1X_RETURN_CAUSE_REG_REJ                   = 0x03,
    CNAS_CCB_1X_RETURN_CAUSE_WRONG_SID                 = 0x04,
    CNAS_CCB_1X_RETURN_CAUSE_WRONG_NID                 = 0x05,

    CNAS_CCB_1X_RETURN_CAUSE_BUTT
};
typedef VOS_UINT8 CNAS_CCB_1X_RETURN_CAUSE_ENUM_UINT8;


enum CNAS_CCB_ESN_MEID_TYPE_ENUM
{
    CNAS_CCB_ESN_MEID_TYPE_ESN      = 0,
    CNAS_CCB_ESN_MEID_TYPE_MEID        ,

    CNAS_CCB_ESN_MEID_TYPE_BUTT
};
typedef VOS_UINT8 CNAS_CCB_ESN_MEID_TYPE_ENUM_UINT8;


enum CNAS_CCB_SERVICE_STATUS_ENUM
{
    CNAS_CCB_SERVICE_STATUS_NO_SERVICE                      = 0,        /* 无服务   */
    CNAS_CCB_SERVICE_STATUS_LIMITED_SERVICE                 = 1,        /* 限制服务 */
    CNAS_CCB_SERVICE_STATUS_NORMAL_SERVICE                  = 2,        /* 正常服务 */

    CNAS_CCB_SERVICE_STATUS_BUTT
};
typedef VOS_UINT32 CNAS_CCB_SERVICE_STATUS_ENUM_UINT32;

enum CNAS_CCB_NETWORK_PROPERTY_ENUM
{
    CNAS_CCB_NETWORK_PROPERTY_UNKNOWN                       = 0x0000,
    CNAS_CCB_NETWORK_PROPERTY_NATIONAL                      = 0x0001,   /* 国内网络 */
    CNAS_CCB_NETWORK_PROPERTY_INTERNATIONAL                 = 0x0002,   /* 国际网络 */

    CNAS_CCB_NETWORK_PROPERTY_BUTT
};
typedef VOS_UINT8 CNAS_CCB_NETWORK_PROPERTY_ENUM_UINT8;


enum CNAS_CCB_EMC_CB_STATE_ENUM
{
    CNAS_CCB_EMC_CB_STATE_NULL             = 0x00,          /* 非CALLBACK模式 */
    CNAS_CCB_EMC_CB_STATE_CALLBACK         = 0x01,             /* 紧急呼CALLBACK模式状态 */

    CNAS_CCB_EMC_CB_STATE_BUTT
};
typedef VOS_UINT8 CNAS_CCB_EMC_CB_STATE_ENUM_UINT8;


enum CNAS_CCB_VERSION_ENUM
{
    CNAS_CCB_VERSION_SVLTE                = 0,
    CNAS_CCB_VERSION_SRLTE                = 1,
    CNAS_CCB_VERSION_BUTT
};
typedef VOS_UINT8 CNAS_CCB_VERSION_ENUM_UINT8;


enum CNAS_CCB_PRL_SOURCE_TYPE_ENUM
{
    CNAS_CCB_PRL_TYPE_EPRL_FILE,
    CNAS_CCB_PRL_TYPE_PRL_FILE,
    CNAS_CCB_PRL_TYPE_NVRAM,
    CNAS_CCB_PRL_TYPE_HARD_CODE,

    CNAS_CCB_PRL_TYPE_BUTT
};
typedef VOS_UINT8 CNAS_CCB_PRL_SOURCE_TYPE_ENUM_UINT8;

/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/


typedef struct
{
    CNAS_CCB_1X_CALL_STATE_ENUM_UINT8                       en1xCallState;
    CNAS_CCB_1X_SO_TYPE_ENUM_UINT8                          en1xSoType;
    VOS_UINT8                                               aucRslv[2];
}CNAS_CCB_1X_CALL_STATE_STRU;



typedef struct
{
    VOS_UINT8                           aucEFRUIMID[CNAS_CCB_EFRUIMID_OCTET_LEN_EIGHT]; /* all the bytes in the EFRUIMID  */
}CNAS_CCB_UIMID_STRU;


typedef struct
{
    CNAS_CCB_ESN_MEID_TYPE_ENUM_UINT8   enEsnMeidType;

    union
    {
        VOS_UINT8                       aucEsn[CNAS_CCB_ESN_OCTET_LEN];
        VOS_UINT8                       aucMeid[CNAS_CCB_MEID_OCTET_LEN];
    }u;
}CNAS_CCB_ESNMEIDME_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucAccessAuthAvailFlag;     /* VOS_TRUE/VOS_FALSE */
    VOS_UINT8                           ucAccessAuthUserNameLen;
    VOS_UINT8                           aucReserved[1];
    VOS_UINT8                           aucAccessAuthUserName[CNAS_CCB_MAX_AUTHDATA_USERNAME_LEN];
}CNAS_CCB_HRPD_ACCESS_AUTH_INFO_STRU;


typedef struct
{
    CNAS_CCB_CARD_STATUS_ENUM_UINT8                         enCsimCardStatus;  /* CSIM卡状态 */
    CNAS_CCB_CARD_STATUS_ENUM_UINT8                         enUsimCardStatus;  /* USIM卡状态 */
    VOS_UINT8                                               aucCardIccId[CNAS_CCB_ICCID_OCTET_LEN];     /* 卡文件2fe2，卡中保存的ICCID*/
    CNAS_CCB_UIMID_STRU                                     stUIMID;                  /* 卡文件6f31，卡中保存的UIMID */

    CNAS_CCB_ESNMEIDME_INFO_STRU                            stEsnMeidMe;              /* 卡文件6f38，esn meid me */
    CNAS_CCB_HRPD_ACCESS_AUTH_INFO_STRU                     stHrpdAccessAuthInfo;     /* 卡文件6f57，当前卡文件中的hrpd ppp an 鉴权用户名 */
}CNAS_CCB_CARD_INFO_STRU;


typedef struct
{
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
    VOS_UINT16                          usBandClass;
    VOS_UINT16                          usChannel;
}CNAS_CCB_1X_SYS_INFO_STRU;


typedef struct
{
    VOS_UINT8                               ucConcurrentSupported;                  /* 并发业务是否支持 */
    VOS_UINT8                               ucPRevInUse;                            /* 协议使用版本 */
    CNAS_CCB_1X_CAS_STATE_ENUM_UINT8        enCasSta;                               /* CAS的状态 */

    CNAS_CCB_1X_CAS_SUB_STATE_ENUM_UINT8    enCasSubSta;                            /* CAS的子状态 */
    CNAS_CCB_1X_CAS_P_REV_ENUM_UINT8        enProtocolRev;                          /* CAS上报的手机支持的最高协议版本 */
    VOS_UINT8                               uc1XRfAvailFlg;                           /* RF资源可用标记:VOS_TRUE :默认RF资源可用
                                                                                                    VOS_FALSE:RF资源不可用 */

    VOS_UINT8                               aucReserved[2];

    CNAS_CCB_1X_SYS_INFO_STRU               st1xSysInfo;
}CNAS_CCB_NW_INFO_STRU;


typedef struct
{
    /* 注册模块赋值，XCC会使用 */
    VOS_UINT8                           ucIsMtCallInRoamingAcc;     /*Mobile terminated calls*/
    VOS_UINT8                           aucReserved[3];
}CNAS_CALL_CONFIG_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucPowerOffCampOnCtrlFlg;                /* 关机驻留控制标记 */
    VOS_UINT8                           aucReserved[3];
}CNAS_CCB_CUSTOM_CFG_INFO_STRU;



typedef struct
{
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
    VOS_UINT16                          usBand;
    VOS_UINT16                          usReserved;
}CNAS_CCB_1X_HOME_SID_NID_STRU;


typedef struct
{
    VOS_UINT8                           ucSysNum;
    VOS_UINT8                           aucReserved[3];
    CNAS_CCB_1X_HOME_SID_NID_STRU       astHomeSidNid[CNAS_CCB_MAX_HOME_SID_NID_LIST];
}CNAS_CCB_1X_HOME_SID_NID_LIST_STRU;


typedef struct
{
    MODEM_ID_ENUM_UINT16                enCdmaModeModemId;
    VOS_UINT8                           aucReserve[2];
}CNAS_CCB_MODEM_INFO_STRU;


typedef struct
{
    CNAS_CCB_SERVICE_STATUS_ENUM_UINT32 enCurSrvStatus; /* 存储1X当前服务状态 */
    VOS_UINT8                           aucReserved[4];
}CNAS_CCB_1X_SRV_INFO_STRU;


typedef struct
{
    VOS_UINT32                          ulReceiveTime;  /* 接收时间 */
    VOS_UINT32                          ulExitTime;     /* 结束时间 */
    VOS_UINT16                          usSendPid;      /* 发送PID */
    VOS_UINT16                          usReceivePid;   /* 接收PID */
    VOS_UINT16                          usMsgName;      /* 消息名 */
    VOS_UINT8                           ucCnasFsmId;    /* 对应模块状态机ID  */
    VOS_UINT8                           ucCnasState;    /* 对应模块的s当前状态  */
}CNAS_CCB_MSG_STATE_STRU;

typedef struct
{
    CNAS_CCB_MSG_STATE_STRU             stMsgState[CNAS_CCB_MAX_LOG_MSG_STATE_NUM];
    VOS_UINT32                          ucLatestIndex;
}CNAS_CCB_LOG_MSG_STATUS_STRU;

typedef struct
{
    CNAS_CCB_LOG_MSG_STATUS_STRU            stLogMsgState;
}CNAS_CCB_MNTN_CTX_STRU;


typedef struct
{
    VOS_UINT16                          usStartSid;
    VOS_UINT16                          usEndSid;
    VOS_UINT32                          ulMcc;
}CNAS_CCB_SYS_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucEnable;                          /* 白名单是否使能 */
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          usWhiteSysNum;                     /* 支持白名单的个数,个数为0时表示不支持白名单 */
    CNAS_CCB_SYS_INFO_STRU              astSysInfo[CNAS_CCB_MAX_WHITE_LOCK_SYS_NUM];
}CNAS_CCB_OPER_LOCK_SYS_WHITE_LIST_STRU;


typedef struct
{
    VOS_UINT16                          usChannel;
    VOS_UINT8                           aucReserved[2];
}CNAS_CCB_FREQENCY_CHANNEL_STRU;


typedef struct
{
    VOS_UINT8                           ucEnableFlg;
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          usFreqNum;
    CNAS_CCB_FREQENCY_CHANNEL_STRU      astFreqList[CNAS_CCB_MAX_FREQ_NUM];
}CNAS_CCB_CTCC_CUSTOMIZE_FREQ_LIST_STRU;


typedef struct
{
    CNAS_CCB_CTCC_CUSTOMIZE_FREQ_LIST_STRU                  stCustFreqList;
}CNAS_CCB_CTCC_CUSTOMIZE_INFO_STRU;


typedef struct
{
    VOS_UINT16                          usPrimaryA;
    VOS_UINT16                          usPrimaryB;
    VOS_UINT16                          usSecondaryA;
    VOS_UINT16                          usSecondaryB;
}CNAS_CCB_CDMA_STANDARD_CHANNELS_STRU;


typedef struct
{
    VOS_UINT8                               ucRatNum;                               /* 当前支持的接入技术个数  */
    VOS_UINT8                               aucRsv[3];
    VOS_RATMODE_ENUM_UINT32                 aulRatPrio[CNAS_CCB_MAX_RAT_NUM];       /* 接入优先级里列表 index表示优先级次序,0最高，2最低 */
}CNAS_CCB_MS_SYS_CFG_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucHrpdNetWorkSrchingFlg;     /* VOS_TRUE/VOS_FALSE，标记HRPD是否
                                                                       正在搜网，搜网时记录，退出搜网时清除 */
    VOS_UINT8                           aucReserved[3];
}CNAS_CCB_HRPD_NETWORK_SRCH_INFO_STRU;


typedef struct
{
    VOS_UINT8                                   ucSuppRatNum;
    VOS_UINT8                                   aucRsvd[3];
    VOS_RATMODE_ENUM_UINT32                     aenRatMode[VOS_RATMODE_BUTT];
}CNAS_CCB_SUPPORT_RAT_STRU;


typedef struct
{
    CNAS_CCB_CARD_INFO_STRU                                 stCardInfo;
    CNAS_CALL_CONFIG_INFO_STRU                              stCallInfoCfg;
    CNAS_CCB_NW_INFO_STRU                                   stNwInfo;
    CNAS_CCB_CUSTOM_CFG_INFO_STRU                           stCustomCfg;                    /* NVIM中的定制信息 */

    CNAS_CCB_1X_HOME_SID_NID_LIST_STRU                      stHomeSidNidList;
    CNAS_CCB_MODEM_INFO_STRU                                stModemInfo;

    CNAS_CCB_PS_RATTYPE_ENUM_UINT32                         enCurrPsRatType;
    CNAS_CCB_PS_RATTYPE_ENUM_UINT32                         enPrevPsRatType;

    CNAS_CCB_1X_CALL_STATE_STRU                             ast1xCallState[CNAS_CCB_1X_CALL_STATE_MAX_NUM];

    CNAS_CCB_1X_RETURN_CAUSE_ENUM_UINT8                     en1xReturnCause;

    VOS_UINT8                                               uc1XCallExistCount;             /* 呼叫计数器 */
    VOS_UINT8                                               ucHomeSidNidDependOnPrlFlg;
    CNAS_CCB_EMC_CB_STATE_ENUM_UINT8                        enEmcCbState;                   /* 紧急呼回呼模式标记 */

    CNAS_CCB_MNTN_CTX_STRU                                  stMntnInfo;
    CNAS_CCB_OPER_LOCK_SYS_WHITE_LIST_STRU                  stOperLockSysWhiteList;

    CNAS_CCB_CTCC_CUSTOMIZE_INFO_STRU                       stCTCCCustInfo;

    CNAS_CCB_CDMA_STANDARD_CHANNELS_STRU                    stCdmaStandardChan;

    CNAS_PRL_FREQENCY_CHANNEL_STRU                          stOhmFreq;

    CNAS_CCB_1X_SRV_INFO_STRU                               st1XSrvInfo;

    CNAS_CCB_MS_SYS_CFG_INFO_STRU                           stMsSysCfgInfo;

    CNAS_CCB_HRPD_NETWORK_SRCH_INFO_STRU                    stHrpdNetWorkSrchInfo;    /* hrpd搜网信息 */

    CNAS_CCB_PRL_SOURCE_TYPE_ENUM_UINT8                     enPrlSrcType;
    VOS_UINT8                                               aucReserved[3];
}CNAS_CCB_CTX_STRU;


typedef struct
{
    VOS_UINT32                           ulModem0Pid;
    VOS_UINT32                           ulModem1Pid;
}CNAS_CCB_MODEM_PID_TAB_STRU;


typedef struct
{
    VOS_UINT16                         usSidRangeStart;                                      /* SID 范围起始值 */
    VOS_UINT16                         usSidRangeEnd;                                        /* SID 范围结束值 */
}CNAS_CCB_NATIONAL_COUNTRY_CODE_INFO;

/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

CNAS_CCB_CARD_STATUS_ENUM_UINT8 CNAS_CCB_GetCsimCardStatus( VOS_VOID );

VOS_VOID CNAS_CCB_InitCcbCtx(VOS_VOID);

CNAS_CCB_CTX_STRU* CNAS_CCB_GetCcbCtxAddr( VOS_VOID );

VOS_VOID CNAS_CCB_SetCsimCardStatus(
    CNAS_CCB_CARD_STATUS_ENUM_UINT8     enCsimCardStatus
);
VOS_UINT16 CNAS_CCB_GetCasCardStatus(VOS_VOID);

VOS_UINT8 CNAS_CCB_GetConcurrentSupportedFlag(VOS_VOID);

VOS_VOID CNAS_CCB_SetConcurrentSupportedFlag(
    VOS_UINT8                           ucConcurrentSupported
);

VOS_UINT8 CNAS_CCB_GetPRevInUse(VOS_VOID);

VOS_VOID CNAS_CCB_SetPRevInUse(
    VOS_UINT8                           ucPRevInUse
);
VOS_VOID CNAS_CCB_SetMtCallInRoamingAccFlg(
    VOS_UINT8                           ucIsMtCallInRoamingAcc
);

VOS_UINT8 CNAS_CCB_GetMtCallInRoamingAccFlg( VOS_VOID );

VOS_VOID CNAS_CCB_SetCasState(
    CNAS_CCB_1X_CAS_STATE_ENUM_UINT8    enSta
);

CNAS_CCB_1X_CAS_STATE_ENUM_UINT8 CNAS_CCB_GetCasState(VOS_VOID);

VOS_VOID CNAS_CCB_SetPowerOffCampOnCtrlFlg(
    VOS_UINT8                           ucPowerOffCampOnFlg
);

VOS_UINT8 CNAS_CCB_GetPowerOffCampOnCtrlFlg( VOS_VOID );

VOS_VOID CNAS_CCB_InitHomeSidNidList(
    CNAS_CCB_1X_HOME_SID_NID_LIST_STRU *pstHomeSidNidList
);

CNAS_CCB_1X_HOME_SID_NID_LIST_STRU* CNAS_CCB_GetHomeSidNidList(VOS_VOID);

CNAS_CCB_1X_CAS_SUB_STATE_ENUM_UINT8 CNAS_CCB_GetCasSubState(VOS_VOID);

VOS_VOID CNAS_CCB_SetCasSubState(
    CNAS_CCB_1X_CAS_SUB_STATE_ENUM_UINT8    enSubSta
);

CNAS_CCB_1X_CAS_P_REV_ENUM_UINT8 CNAS_CCB_GetHighProRev(VOS_VOID);

VOS_VOID CNAS_CCB_SetHighProRev(
    CNAS_CCB_1X_CAS_P_REV_ENUM_UINT8    enProtocolRev
);


CNAS_CCB_1X_CALL_STATE_ENUM_UINT8 CNAS_CCB_Get1xVoiceCallState(VOS_VOID);

CNAS_CCB_1X_CALL_STATE_ENUM_UINT8 CNAS_CCB_Get1xSmsCallState(
    VOS_VOID
);
CNAS_CCB_1X_CALL_STATE_ENUM_UINT8 CNAS_CCB_Get1xPsCallState(
    VOS_VOID
);
CNAS_CCB_1X_SO_TYPE_ENUM_UINT8 CNAS_CCB_ConvertSoToServiceType(
    CAS_CNAS_1X_SERVICE_OPTION_ENUM_UINT16                  en1xSo
);

VOS_VOID CNAS_CCB_Set1xCallState(
    CNAS_CCB_1X_CALL_STATE_ENUM_UINT8                       en1xCallState,
    CNAS_CCB_1X_SO_TYPE_ENUM_UINT8                          en1xSoType
);


CNAS_CCB_1X_RETURN_CAUSE_ENUM_UINT8 CNAS_CCB_Get1xReturnCause(VOS_VOID);

VOS_VOID CNAS_CCB_Set1xReturnCause(
    CNAS_CCB_1X_RETURN_CAUSE_ENUM_UINT8                     en1xReturnCause
);


VOS_VOID CNAS_CCB_Set1XRfAvailFlg(
    VOS_UINT8                           uc1XRfAvailFlg
);

VOS_UINT8 CNAS_CCB_Get1XRfAvailFlg( VOS_VOID );


CNAS_CCB_UIMID_STRU* CNAS_CCB_GetUIMID(VOS_VOID);


VOS_VOID CNAS_CCB_SetCdmaModeModemId(
    MODEM_ID_ENUM_UINT16                enModemId
);

MODEM_ID_ENUM_UINT16 CNAS_CCB_GetCdmaModeModemId( VOS_VOID );

VOS_UINT32 CNAS_CCB_ConvertPidBasedOnModemId(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT32                          ulRcvPid
);

VOS_UINT32 CNAS_CCB_GetModem0Pid(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT32                          ulSenderPid
);

VOS_UINT8* CNAS_CCB_GetCardIccIdAddr(VOS_VOID);

VOS_VOID CNAS_CCB_InitIccId(VOS_VOID);

VOS_VOID CNAS_CCB_InitUIMID(VOS_VOID);

VOS_VOID CNAS_CCB_InitEsnMeidMeInfo(VOS_VOID);

VOS_VOID CNAS_CCB_InitHrpdAccessAuthInfo(VOS_VOID);

extern VOS_VOID CNAS_CCB_SetUsimCardStatus(
    CNAS_CCB_CARD_STATUS_ENUM_UINT8     enUsimCardStatus
);

extern CNAS_CCB_CARD_STATUS_ENUM_UINT8 CNAS_CCB_GetUsimCardStatus( VOS_VOID );

VOS_VOID CNAS_CCB_SetPrevRatType(
    CNAS_CCB_PS_RATTYPE_ENUM_UINT32     enPsRatType
);
CNAS_CCB_PS_RATTYPE_ENUM_UINT32 CNAS_CCB_GetPrevPsRatType(VOS_VOID);
VOS_VOID CNAS_CCB_SetCurrPsRatType(
    CNAS_CCB_PS_RATTYPE_ENUM_UINT32    enPsRatType
);
CNAS_CCB_PS_RATTYPE_ENUM_UINT32 CNAS_CCB_GetCurrPsRatType(VOS_VOID);


CNAS_CAS_1X_RETURN_CAUSE_ENUM_UINT8 CNAS_CCB_CovertReturnCause(
    CNAS_CCB_1X_RETURN_CAUSE_ENUM_UINT8   enCnas1xReturnCause
);

VOS_VOID CNAS_CCB_InitMntnInfo(
    CNAS_CCB_MNTN_CTX_STRU             *pstMntnInfo
);

VOS_VOID CNAS_CCB_Set1XCallExistCount(
    VOS_UINT8                           uc1XCallExistCount
);

VOS_UINT8 CNAS_CCB_Get1XCallExistCount( VOS_VOID );

VOS_UINT8 CNAS_CCB_GetHomeSidNidDependOnPrlFlg(VOS_VOID);

VOS_VOID CNAS_CCB_SetHomeSidNidDependOnPrlFlg(
    VOS_UINT8                           ucFlg
);

CNAS_CCB_OPER_LOCK_SYS_WHITE_LIST_STRU* CNAS_CCB_GetOperLockSysWhiteList(VOS_VOID);

CNAS_CCB_CTCC_CUSTOMIZE_FREQ_LIST_STRU* CNAS_CCB_GetCTCCCustomizeFreqList(VOS_VOID);

CNAS_CCB_CDMA_STANDARD_CHANNELS_STRU* CNAS_CCB_GetCdmaStandardChannels(VOS_VOID);

VOS_VOID CNAS_CCB_InitCdmaStandardChannels(
    CNAS_CCB_CDMA_STANDARD_CHANNELS_STRU                   *pstCdmaStandardChan
);

CNAS_PRL_FREQENCY_CHANNEL_STRU* CNAS_CCB_GetCdmaOhmFreq(VOS_VOID);

VOS_VOID CNAS_CCB_SetCdmaOhmFreq(
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstOhmFreq
);

VOS_VOID CNAS_CCB_ClearCdmaOhmFreq(VOS_VOID);

VOS_VOID CNAS_CCB_Set1xSysInfo(
    VOS_UINT16                          usSid,
    VOS_UINT16                          usNid,
    VOS_UINT16                          usBandClass,
    VOS_UINT16                          usChannel
);

CNAS_CCB_1X_SYS_INFO_STRU* CNAS_CCB_Get1xSysInfo(VOS_VOID);

VOS_VOID CNAS_CCB_Init1xSysInfo(
    VOS_VOID
);

CNAS_CCB_ESNMEIDME_INFO_STRU* CNAS_CCB_GetEsnMeidMeInfoAddr(VOS_VOID);

CNAS_CCB_HRPD_ACCESS_AUTH_INFO_STRU* CNAS_CCB_GetHrpdAccesAuthInfoAddr(VOS_VOID);
VOS_UINT32 CNAS_CCB_IsCurCallStateIdle( VOS_VOID );
CNAS_CCB_1X_SRV_INFO_STRU* CNAS_CCB_GetCur1XSrvInfo(VOS_VOID);

CNAS_CCB_SERVICE_STATUS_ENUM_UINT32 CNAS_CCB_GetCur1XSrvStatus(VOS_VOID);

VOS_VOID CNAS_CCB_SetCur1XSrvStatus(
    CNAS_CCB_SERVICE_STATUS_ENUM_UINT32 enSrvStatus
);

CNAS_CCB_NETWORK_PROPERTY_ENUM_UINT8 CNAS_CCB_IsCur1XCampOnNationalNetWork(
    VOS_VOID
);

VOS_VOID CNAS_CCB_SetEmcCbState(
    CNAS_CCB_EMC_CB_STATE_ENUM_UINT8    enEmcState
);

CNAS_CCB_EMC_CB_STATE_ENUM_UINT8 CNAS_CCB_GetEmcCbState(VOS_VOID);

VOS_VOID CNAS_CCB_InitMsSysCfgInfo(
    VOS_VOID
);

VOS_VOID CNAS_CCB_SetCurMsSysCfg(
    VOS_UINT8                           ucRatNum,
    VOS_RATMODE_ENUM_UINT32            *penRatMode
);

CNAS_CCB_MS_SYS_CFG_INFO_STRU *CNAS_CCB_GetCurMsSysCfgAddr(VOS_VOID);
VOS_VOID CNAS_CCB_InitHrpdNetWorkSrchInfo(VOS_VOID);

CNAS_CCB_HRPD_NETWORK_SRCH_INFO_STRU* CNAS_CCB_GetHrpdNetWorkSrchInfo(VOS_VOID);

VOS_VOID CNAS_CCB_SetHrpdNetWorkSrchingFlg(
    VOS_UINT8                                               ucHrpdNetWorkSrchFlg
);

VOS_UINT8 CNAS_CCB_GetHrpdNetWorkSrchingFlg(VOS_VOID);

VOS_UINT32 CNAS_CCB_IsSyscfgSupportLte( VOS_VOID );
CNAS_CCB_VERSION_ENUM_UINT8 CNAS_CCB_IsVersionSrlte( VOS_VOID );
VOS_VOID CNAS_CCB_Init1xCallState( VOS_VOID );

VOS_VOID CNAS_CCB_SetPrlSrcType(
    CNAS_CCB_PRL_SOURCE_TYPE_ENUM_UINT8                     enPrlSrcType
);

CNAS_CCB_PRL_SOURCE_TYPE_ENUM_UINT8 CNAS_CCB_GetPrlSrcType(VOS_VOID);
#endif

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
