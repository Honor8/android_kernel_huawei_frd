

#ifndef __CNAS_XSD_MNTN_H__
#define __CNAS_XSD_MNTN_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include  "PsCommonDef.h"
#include  "PsLogdef.h"
#include  "CnasXsdCtx.h"
#include  "CnasMntn.h"
#include  "CnasPrlApi.h"
#include  "CnasCcb.h"
#include "Nas_Mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)


/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define CNAS_XSD_MAX_LOG_CHANNEL_NUM_IN_SCAN_LIST           (512)

#define CNAS_XSD_MAX_LOG_SYSTEM_NUM_IN_ACQ_LIST             (256)




/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum CNAS_XSD_MNTN_MSG_ID_ENUM
{
    ID_CNAS_XSD_MNTN_LOG_SCAN_CHAN_LIST_IND                 = ID_CNAS_MNTN_XSD_MSG_BASE,    /*_H2ASN_MsgChoice  CNAS_XSD_LOG_SCAN_CHNNEL_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_ACQ_SYS_LIST_IND,                                                  /*_H2ASN_MsgChoice  CNAS_XSD_LOG_ACQ_SYSTEM_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_AVAIL_SYS_LIST_IND,                                                /*_H2ASN_MsgChoice  CNAS_XSD_LOG_AVAIL_SYSTEM_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_MRU_LIST_IND,                                                      /*_H2ASN_MsgChoice  CNAS_XSD_LOG_MRU_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_AVOID_FREQ_LIST_IND,                                               /*_H2ASN_MsgChoice  CNAS_XSD_LOG_AVOID_FREQ_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_PRL_SOURCE_TYPE_IND,                                               /*_H2ASN_MsgChoice  CNAS_XSD_LOG_PRL_SOURCE_TYPE_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_BUFFER_MSG_INFO_IND,                                               /*_H2ASN_MsgChoice  CNAS_XSD_LOG_BUffER_MSG_INFO_STRU */

    ID_CNAS_XSD_MNTN_LOG_CALL_REDIAL_SCAN_CHAN_LIST_IND,                                    /*_H2ASN_MsgChoice  CNAS_XSD_LOG_SCAN_CHNNEL_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_BANDClASS_LIST_IND,                                                /*_H2ASN_MsgChoice  CNAS_XSD_LOG_BANDCLASS_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_REDIR_SCAN_CHAN_LIST_IND,                                          /*_H2ASN_MsgChoice  CNAS_XSD_LOG_SCAN_CHNNEL_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_CFREQLOCK_SCAN_CHAN_LIST_IND,                                      /*_H2ASN_MsgChoice  CNAS_XSD_LOG_SCAN_CHNNEL_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_HOME_SID_NID_LIST_IND,                                             /*_H2ASN_MsgChoice  CNAS_XSD_LOG_HOME_SID_NID_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_OOC_TIMER_SCHEDULE_IND,                                            /*_H2ASN_MsgChoice  CNAS_XSD_LOG_OOC_TIMER_SCHEDULE_INFO_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_OOC_CTX_INFO_IND,                                                  /*_H2ASN_MsgChoice  CNAS_XSD_LOG_OOC_CTX_INFO_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_HOME_SID_NID_PRI_FLG,                                              /*_H2ASN_MsgChoice  CNAS_XSD_LOG_HOME_SID_NID_MOST_PRI_FLG_STRU */

    ID_CNAS_XSD_MNTN_LOG_OPER_LOCK_WHITE_SID_LIST_IND,                                      /*_H2ASN_MsgChoice  CNAS_XSD_LOG_OPER_LOCK_SYS_WHITE_LIST_STRU */

    ID_CNAS_XSD_MNTN_LOG_CTCC_CUSTOMIZE_FREQ_LIST_IND,                                      /*_H2ASN_MsgChoice  CNAS_XSD_LOG_CTCC_CUSTOMIZE_FREQ_LIST_STRU */

    ID_CNAS_XSD_MNTN_LOG_EMC_CALL_REDIAL_SCAN_CHAN_LIST_IND,                                /*_H2ASN_MsgChoice  CNAS_XSD_LOG_SCAN_CHNNEL_LIST_MSG_STRU */

    ID_CNAS_XSD_MNTN_LOG_GEO_LIST_SRCH_STATUS_IND,                                          /*_H2ASN_MsgChoice  CNAS_XSD_LOG_GEO_LIST_SRCH_STATUS_STRU */

    ID_CNAS_XSD_MNTN_LOG_CDMA_STANDARD_CHANNELS_IND,                                        /*_H2ASN_MsgChoice  CNAS_XSD_LOG_CDMA_STANDARD_CHANNELS_STRU */

    ID_CNAS_XSD_MNTN_LOG_CALLBACK_CFG_IND,                                                  /*_H2ASN_MsgChoice  CNAS_XSD_LOG_CALLBACK_CFG_STRU */

    ID_CNAS_XSD_MNTN_LOG_ENTER_CALLBACK_IND,                                                /*_H2ASN_MsgChoice  CNAS_XSD_LOG_CALLBACK_STATUS_STRU */

    ID_CNAS_XSD_MNTN_LOG_QUIT_CALLBACK_IND,                                                 /*_H2ASN_MsgChoice  CNAS_XSD_LOG_CALLBACK_STATUS_STRU */

    ID_CNAS_XSD_MNTN_LOG_CDMA_1X_CUSTOM_PREF_CHANNELS_IND,                                  /*_H2ASN_MsgChoice  CNAS_XSD_LOG_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU */

    ID_CNAS_XSD_MNTN_LOG_DRX_TIMER_STATUS_IND,

    ID_CNAS_XSD_MNTN_LOG_1X_AVOID_SYS_ACQ,                                                  /*_H2ASN_MsgChoice  CNAS_XSD_LOG_1X_AVOID_SYS_ACQ_STRU */

    ID_CNAS_XSD_MNTN_MSG_ID_BUTT
};
typedef VOS_UINT32 CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32;


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usChanNum;
    VOS_UINT16                          usCurScanIndex;
    CNAS_XSD_SCAN_CHANNEL_INFO_STRU     astScanChanList[CNAS_XSD_MAX_LOG_CHANNEL_NUM_IN_SCAN_LIST];/* Ƶ��ɨ���б� */
}CNAS_XSD_LOG_SCAN_CHNNEL_LIST_MSG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                          usSysNum;
    VOS_UINT8                           aucRsv[2];
    CNAS_XSD_ACQUIRE_SYSTEM_INFO_STRU   astAcqSysList[CNAS_XSD_MAX_LOG_SYSTEM_NUM_IN_ACQ_LIST];   /* ϵͳ�����б� */
}CNAS_XSD_LOG_ACQ_SYSTEM_LIST_MSG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucAvailSysNum;
    VOS_UINT8                           ucNextSrchIndex;
    VOS_UINT8                           aucRsved[2];
    CNAS_XSD_AVAIL_SYSTEM_INFO_STRU     astAvailSystem[CNAS_XSD_MAX_AVAILABLE_SYS_NUM];
}CNAS_XSD_LOG_AVAIL_SYSTEM_LIST_MSG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucSysNum;
    VOS_UINT8                           aucReserve[3];
    CNAS_PRL_1X_SYSTEM_STRU             astSystem[CNAS_XSD_MAX_MRU_SYS_NUM];
}CNAS_XSD_LOG_MRU_LIST_MSG_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT32                          ulBandClass;
}CNAS_XSD_LOG_BANDCLASS_LIST_MSG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucSysNum;
    VOS_UINT8                           aucReserve[3];
    CNAS_CCB_1X_HOME_SID_NID_STRU       astHomeSidNid[CNAS_CCB_MAX_HOME_SID_NID_LIST];
}CNAS_XSD_LOG_HOME_SID_NID_LIST_MSG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32                        enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT16                                              usMru0SearchTimerLen;
    CNAS_XSD_OOC_SCAN_STRATEGY_ENUM_UINT8                   enChanRepeatScanStrategy;   /* Ƶ���ظ��������� */
    VOS_UINT8                                               ucInsertOrigChanFlg;        /* ��ͬ������ǰ�Ƿ������ǰפ��Ƶ�� */
    VOS_UINT8                                               ucPhaseNum;
    VOS_UINT8                                               uc1xOocDoTchPhase1TimerLen;                         /* Do TCH��ǰ4�γ��� Ooc Timer ���ʱ�� */
    VOS_UINT8                                               uc1xOocDoTchPhase2TimerLen;                         /* Do TCH��4�����ϳ��� Ooc Timer ���ʱ�� */
    VOS_UINT8                                               aucReserved[1];
    CNAS_XSD_OOS_SYS_ACQ_STRATEGY_PATTERN_STRU              astOocTimerInfo[CNAS_XSD_MAX_OOC_SCHEDULE_PHASE_NUM];
}CNAS_XSD_LOG_OOC_TIMER_SCHEDULE_INFO_MSG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucAvoidFreqNum;
    VOS_UINT8                           ucNextAcqIndex;    /* �´���Ҫ��ȡ��AvoidƵ��������Ϣ */
    VOS_UINT8                           aucReserve[2];
    CNAS_XSD_AVOID_FREQ_INFO_STRU       astAvoidFreqInfo[CNAS_XSD_MAX_AVOID_FREQ_NUM];
}CNAS_XSD_LOG_AVOID_FREQ_LIST_MSG_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    CNAS_CCB_PRL_SOURCE_TYPE_ENUM_UINT8 enPrlSrcType;
    VOS_UINT8                           aucReserve[3];
}CNAS_XSD_LOG_PRL_SOURCE_TYPE_MSG_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT32                          ulFullFlg;
    CNAS_XSD_CACHE_MSG_QUEUE_STRU       stMsgQueue;
}CNAS_XSD_LOG_BUffER_MSG_INFO_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucWaitSearchFlag;
    VOS_UINT8                           ucCurrentPhase;
    VOS_UINT16                          usCurrentTimes;
    VOS_UINT8                           ucReserved;
    VOS_UINT8                           ucFirstFourAcqArrivedFlag;
    VOS_UINT8                           ucAttemptTimesInDoTraffic;
    VOS_UINT8                           ucSceneSetFlag;
}CNAS_XSD_LOG_OOC_CTX_INFO_MSG_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucHomeSidNidMostPriFlg;
    VOS_UINT8                           aucReserved[3];
}CNAS_XSD_LOG_HOME_SID_NID_MOST_PRI_FLG_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucEnable;                          /* �������Ƿ�ʹ�� */
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          usWhiteSysNum;
    CNAS_CCB_SYS_INFO_STRU              astSysInfo[CNAS_CCB_MAX_WHITE_LOCK_SYS_NUM];
}CNAS_XSD_LOG_OPER_LOCK_SYS_WHITE_LIST_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
    VOS_UINT8                           ucEnableFlg;
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          usFreqNum;
    CNAS_CCB_FREQENCY_CHANNEL_STRU      astFreqList[CNAS_CCB_MAX_FREQ_NUM];
}CNAS_XSD_LOG_CTCC_CUSTOMIZE_FREQ_LIST_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32                        enMsgId;            /* _H2ASN_Skip */
    VOS_UINT16                                              usGeoNum;       /* GEO��Ŀ */
    VOS_UINT16                                              usReserved;
    CNAS_XSD_GEO_SRCH_STATUS_INFO_STRU                      astGeoSrchInfo[CNAS_XSD_MAX_LOG_SYSTEM_NUM_IN_ACQ_LIST]; /* ָ��GEO������Ϣ�׵�ַ */
}CNAS_XSD_LOG_GEO_LIST_SRCH_STATUS_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32                        enMsgId;            /* _H2ASN_Skip */
    CNAS_CCB_CDMA_STANDARD_CHANNELS_STRU                    stCdmaStandardChan; /* cdma��׼Ƶ����Ϣ */
}CNAS_XSD_LOG_CDMA_STANDARD_CHANNELS_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32                        enMsgId;            /* _H2ASN_Skip */
    CNAS_XSD_CALLBACK_CFG_STRU                              stCallBackCfg;      /* ������CallBack���������Ϣ */
} CNAS_XSD_LOG_CALLBACK_CFG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;                                /* _H2ASN_Skip */
} CNAS_XSD_LOG_CALLBACK_STATUS_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32                        enMsgId;            /* _H2ASN_Skip */
    CNAS_XSD_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU              stCdma1XPrefChannels;
} CNAS_XSD_LOG_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU;


typedef struct
{
    VOS_MSG_HEADER                                          /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;            /* _H2ASN_Skip */
    VOS_UINT8                           ucDrxTimerStatus;   /*  DRX��ʱ����״̬ */
    VOS_UINT8                           aucReserve[3];
} CNAS_XSD_LOG_DRX_TIMER_STATUS_IND;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32                        enMsgId;            /* _H2ASN_Skip */
    VOS_UINT32                                              ulIsAvoidSysAcq;
} CNAS_XSD_LOG_1X_AVOID_SYS_ACQ_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
/*****************************************************************************
  H2ASN������Ϣ�ṹ����
*****************************************************************************/
typedef struct
{
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32    enMsgId;            /* _H2ASN_MsgChoice_Export CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32 */

    VOS_UINT8                           aucMsgBlock[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32
    ****************************************************************************/
}CNAS_XSD_MNTN_MSG_DATA;
/*_H2ASN_Length UINT32*/


typedef struct
{
    VOS_MSG_HEADER
    CNAS_XSD_MNTN_MSG_DATA              stMsgData;
}CNAS_XSD_MNTN_MSG;



/*****************************************************************************
  10 ��������
*****************************************************************************/
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
VOS_VOID CNAS_XSD_LogScanChannelList(
    VOS_UINT32                          ulMsgName,
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanChanList
);

VOS_VOID CNAS_XSD_LogAcqSystemList(
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList
);

VOS_VOID CNAS_XSD_LogAvailSystemList(
    CNAS_XSD_AVAILABLE_LIST_STRU       *pstAvailSysList
);

VOS_VOID CNAS_XSD_LogMruList(
    CNAS_XSD_MRU_LIST_STRU             *pstMruList
);

VOS_VOID CNAS_XSD_LogAvoidFreqList(
    CNAS_XSD_AVOID_FREQ_LIST_STRU      *pstAvoidFreqList
);

VOS_VOID CNAS_XSD_LogPrlSourceType(
    CNAS_CCB_PRL_SOURCE_TYPE_ENUM_UINT8 enPrlSrcType
);

VOS_VOID  CNAS_XSD_LogBufferQueueMsg(
    VOS_UINT32                          ulFullFlg
);

VOS_VOID CNAS_XSD_LogUeSupportedBandClass(
    VOS_UINT32                          ulBandClass
);


VOS_VOID CNAS_XSD_LogHomeSidNidList(
    CNAS_CCB_1X_HOME_SID_NID_LIST_STRU *pstHomeSidNidList
);

VOS_VOID CNAS_XSD_LogOocScheduleInfo(
    CNAS_NVIM_1X_OOS_SYS_ACQ_STRATEGY_CFG_STRU             *pstOocTimerScheduleInfo
);

VOS_VOID CNAS_XSD_LogOocCtxInfo(
    CNAS_XSD_OOC_CTX_INFO_STRU         *pstOocCtxInfo
);

VOS_VOID CNAS_XSD_LogHomeSidNidDependOnPrlFlg(
    VOS_UINT8                           ucFlg
);

VOS_VOID CNAS_XSD_LogOperLockSysWhiteList(
    CNAS_CCB_OPER_LOCK_SYS_WHITE_LIST_STRU                 *pstMsg
);

VOS_VOID CNAS_XSD_LogCTCCCustomizeFreqList(
    CNAS_CCB_CTCC_CUSTOMIZE_FREQ_LIST_STRU                 *pstMsg
);

VOS_VOID CNAS_XSD_LogGeoListSrchStatus(
    CNAS_XSD_GEO_LIST_SRCH_INFO_STRU    *pstGeoSrchStatus
);

VOS_VOID CNAS_XSD_LogCdmaStandardChannels(
    CNAS_CCB_CDMA_STANDARD_CHANNELS_STRU                   *pstCdmaStandardChan
);

VOS_VOID CNAS_XSD_LogCallBackConfig(
    CNAS_XSD_CALLBACK_CFG_STRU                             *pstCallBackCfg
);

VOS_VOID CNAS_XSD_LogCallBackStatusInd(
    CNAS_XSD_MNTN_MSG_ID_ENUM_UINT32                        enMsgId
);

VOS_VOID CNAS_XSD_LogCdma1XCustomPrefChannels(
    CNAS_XSD_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU             *pstCdma1XPrefChannels
);

VOS_VOID  CNAS_XSD_LogDrxTimerStatus(
    VOS_UINT8                           ucDrxTimerStatus
);
VOS_VOID CNAS_XSD_Log1xAvoidSysAcqState(
    VOS_UINT8                           ucFlg
);

VOS_VOID CNAS_XSD_LogPcReplayFixedContext(VOS_VOID);

VOS_VOID CNAS_XSD_LogPcReplayPreCfgParaSwitchOn(VOS_VOID);

#endif

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of CnasXsdMntn.h */
