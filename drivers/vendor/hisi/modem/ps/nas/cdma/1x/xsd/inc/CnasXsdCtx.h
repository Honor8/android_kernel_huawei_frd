

#ifndef __CNAS_XSD_CTX_H__
#define __CNAS_XSD_CTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "NasFsm.h"
#include "CnasPrlApi.h"
#include "xsd_mscc_pif.h"

#include "NasErrorLogCdma.h"

#include  "CnasCcb.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define CNAS_XSD_MAX_FSM_STACK_DEPTH                        (4)                 /* ״̬��ջ����� */

#define CNAS_XSD_MAX_CACHE_MSG_QUEUE_NUM                    (8)                 /* ��󻺴���Ϣ�ĸ��� */

#define CNAS_XSD_MAX_MSG_BUFFER_LEN                         (1600)              /* ��Ϣ����󳤶ȣ�����״̬������������Ϣ��CACHE��Ϣ */

/* �洢������MRU������CGD 130 5.2.2 The number of entries in an MRU list is implementation dependent but a typical number may be 12 */
#define CNAS_XSD_MAX_MRU_SYS_NUM                            (12)

#define CNAS_XSD_MAX_AVOID_FREQ_NUM                         (100)

#define CNAS_XSD_MAX_REDIR_CHANNEL_NUM                      (16)

#define CNAS_XSD_MAX_PREFERRED_SYSTEM_NUM                   (50)

#define CNAS_XSD_BAND0_BAND10_PREFERRED_SYSTEM_NUM          (21)

#define CNAS_XSD_MAX_AVAILABLE_SYS_NUM                      (50)

#define CNAS_XSD_MAX_REDIAL_SYSTEM_NUM                      (600)

#define CNAS_XSD_MAX_NEG_SYS_NUM                            (50)

/* ��CNAS_CAS_1X_MAX_SAME_PRIO_SYS_NUM��ͬ */
#define CNAS_XSD_1X_MAX_SAME_PRIO_SYS_NUM                   (8)

/* ��CNAS_CAS_1X_MAX_SYSTEM_NUM��ͬ */
#define CNAS_XSD_1X_SND_CAS_MAX_SYSTEM_NUM                  (16)

#define CNAS_XSD_1X_SYNC_SUCC_FREQ_MAX_NUM                  (128)

#define CNAS_XSD_MAX_INT_MSG_QUEUE_NUM                      (8)

#define CNAS_XSD_MAX_SLICE_VALUE                            (0xFFFFFFFF)

#define CNAS_XSD_WILDCARD_CDMAPN_VALUE                      (0xFFFF)

/* XSD�ڲ�ϵͳ���������д���Ƶ�������� */
#define CNAS_XSD_INTERNAL_SYSTEM_ACQ_REQ_CHAN_MAX_NUM       (40)

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_TOTAL_PHASE_NUM       (6) /* OOS����ϵͳ����Ĭ���ܽ׶��� */

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_ONE_SLEEP_TIME_LEN        (0)  /* OOS����ϵͳ����1�׶�Ĭ��sleepʱ��,��λ�� */
#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_ONE_MAX_NUM_ONE_CYCLE     (2)  /* OOS����ϵͳ����1�׶�Ĭ��һ���������� */

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_TWO_SLEEP_TIME_LEN        (2)  /* OOS����ϵͳ����2�׶�Ĭ��sleepʱ��,��λ�� */
#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_TWO_MAX_NUM_ONE_CYCLE     (2)  /* OOS����ϵͳ����2�׶�Ĭ��һ���������� */

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_THREE_SLEEP_TIME_LEN      (10)  /* OOS����ϵͳ����3�׶�Ĭ��sleepʱ��,��λ�� */
#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_THREE_MAX_NUM_ONE_CYCLE   (4)  /* OOS����ϵͳ����3�׶�Ĭ��һ���������� */

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_FOUR_SLEEP_TIME_LEN       (30) /* OOS����ϵͳ����4�׶�Ĭ��sleepʱ��,��λ�� */
#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_FOUR_MAX_NUM_ONE_CYCLE    (4)  /* OOS����ϵͳ����4�׶�Ĭ��һ���������� */

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_FIVE_SLEEP_TIME_LEN       (60) /* OOS����ϵͳ����5�׶�Ĭ��sleepʱ��,��λ�� */
#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_FIVE_MAX_NUM_ONE_CYCLE    (4)  /* OOS����ϵͳ����5�׶�Ĭ��һ���������� */

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_SIX_SLEEP_TIME_LEN        (180)  /* OOS����ϵͳ����6�׶�Ĭ��sleepʱ��,��λ�� */
#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_PHASE_SIX_MAX_NUM_ONE_CYCLE     (255)  /* OOS����ϵͳ����6�׶�Ĭ��һ���������� */


#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_DO_TCH_PHASE_ONE_SLEEP_TIME_LEN  (10) /* OOS����ϵͳ����Ĭ��DO TCH̬�׶�1����˯��ʱ�䣬��λ�� */
#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_DO_TCH_PHASE_TWO_SLEEP_TIME_LEN  (20) /* OOS����ϵͳ����Ĭ��DO TCH̬�׶�2����˯��ʱ�䣬��λ�� */

#define CNAS_XSD_DEFAULT_OOS_SYS_ACQ_MRUO_SEARCH_SLEEP_TIME_LEN       (15) /* OOS����ϵͳ����Ĭ��MRUO����˯��ʱ�� ����λ�� */

#define CNAS_XSD_MAX_OOC_SCHEDULE_PHASE_NUM                 (8)

#define CNAS_XSD_ATTEMP_IN_DO_TRAFFIC_4_TIMES               (4)
#define CNAS_XSD_ATTEMP_UNLIMITED_TIMES                     (65535)

#define CNAS_XSD_EF_HOME_SID_NID_REC_LEN                    (5)
#define CNAS_XSD_EF_HOME_SID_OCTET_LEN                      (2)
#define CNAS_XSD_EF_HOME_NID_OCTET_LEN                      (2)
#define CNAS_XSD_EF_HOME_BAND_OCTET_LEN                     (1)

#define CNAS_XSD_BIT_LEN_8_BIT                              (8)
#define CNAS_XSD_BIT_LEN_16_BIT                             (16)
#define CNAS_XSD_HOME_SID_NID_LEN                           (5)
#define CNAS_XSD_HOME_SID_NID_1_OFFSET                      (1)
#define CNAS_XSD_HOME_SID_NID_2_OFFSET                      (2)
#define CNAS_XSD_HOME_SID_NID_3_OFFSET                      (3)
#define CNAS_XSD_HOME_SID_NID_4_OFFSET                      (4)

#define CNAS_XSD_CALL_REL_SYNC_MAX_FREQ_NUM                 (2)

#define CNAS_XSD_MAX_CDMA_1X_CUSTOM_PREF_CHANNELS_NUM       (20)

#define CNAS_XSD_AVOID_MAX_PHASE_NUM                        (8)
#define CNAS_XSD_MAX_CANDIDATE_FREQ_NUM                     (3)

#define CNAS_XSD_MAX_INSERTED_MRU0_NUM                      (5)

#define CNAS_XSD_MAX_FREQ_NUM_WHEN_LTE_DO_CONN              ( 1 )

#define CNAS_XSD_REPLAY_MAX_CACHE_MSG_QUEUE_NUM                    (4)                 /* �ط��У���󻺴���Ϣ�ĸ��� */
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum CNAS_XSD_FSM_ID_ENUM
{
    /***********************************************************************
     * XSD L1״̬����ʶ
     ***********************************************************************/
    /* CNAS XSD L1 main״̬���ı�ʶ */
    CNAS_XSD_FSM_L1_MAIN                                    =0x00,

    /***********************************************************************
     * XSD L2״̬���ı�ʶ
     ***********************************************************************/
    /* Э��ջ��ʼ��L2״̬�� */
    CNAS_XSD_FSM_SWITCH_ON                                  =0x01,

    /* Э��ջ�ػ�L2״̬�� */
    CNAS_XSD_FSM_POWER_OFF                                  =0x02,

    /* Э��ջSYSTEM ACQUIRE L2״̬�� */
    CNAS_XSD_FSM_SYS_ACQ                                    =0x03,

    CNAS_XSD_FSM_REDIRECTION                                =0X04,
    CNAS_XSD_BUTT

};
typedef VOS_UINT32 CNAS_XSD_FSM_ID_ENUM_UINT32;


/** ****************************************************************************
 * Name        : CNAS_XSD_ACQUIRED_CAUSE_ENUM
 * Description :
 *******************************************************************************/
enum CNAS_XSD_ACQUIRED_CAUSE_ENUM
{
    CNAS_XSD_ACQ_FAIL_CAUSE_ACQUISITION_FAIL                = 0x00,
    CNAS_XSD_ACQ_FAIL_CAUSE_PROTO_MISMATCH                  = 0x01,
    CNAS_XSD_ACQ_FAIL_CAUSE_PAGE_CH_FAIL                    = 0x02,
    CNAS_XSD_ACQ_FAIL_CAUSE_FREQ_SRCH_FAIL                  = 0x03,
    CNAS_XSD_ACQ_FAIL_CAUSE_BUTT                            = 0x04
};
typedef VOS_UINT8 CNAS_XSD_ACQUIRED_CAUSE_ENUM_UINT8;


enum CNAS_XSD_INIT_CTX_TYPE_ENUM
{
    CNAS_XSD_INIT_CTX_STARTUP                    = 0,
    CNAS_XSD_INIT_CTX_POWEROFF                   = 1,
    CNAS_XSD_INIT_CTX_BUTT
};
typedef VOS_UINT8 CNAS_XSD_INIT_CTX_TYPE_ENUM_UINT8;


enum   CNAS_XSD_READ_CARD_FILES_CNF_ENUM
{
    CNAS_XSD_READ_CARD_FILE_FLG_NULL                        = 0x00,

    CNAS_XSD_READ_CARD_FILE_PRL_FLG                         = 0x01,

    CNAS_XSD_READ_CARD_FILE_EPRL_FLG                        = 0x02,

    CNAS_XSD_READ_CARD_FILE_HOME_SID_NID_FLG                = 0x04,

    CNAS_XSD_READ_CARD_FILE_CNF_BUTT
};

typedef VOS_UINT32 CNAS_XSD_READ_CARD_FILES_CNF_ENUM_UINT32;


enum CNAS_XSD_SYS_ACQ_SCENE_ENUM
{
    CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON,                       /* ���� */
    CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST,                     /* �������� */
    CNAS_XSD_SYS_ACQ_SCENE_CALL_RELEASED,                   /* �����ͷŴ������� */
    CNAS_XSD_SYS_ACQ_SCENE_NORMAL_CALL_REDIAL,              /* ���������ز���������*/
    CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL,                  /* ��������������*/

    CNAS_XSD_SYS_ACQ_SCENE_CALL_RELEASED_WITH_REDIR,        /* call released while redirction flag is true */
    CNAS_XSD_SYS_ACQ_SCENE_REDIRECTION_FAIL,                /* redirection fail we must trigger system acquire */
    CNAS_XSD_SYS_ACQ_SCENE_REDIRECTION_WITH_INVALID_GSRDM,  /* redirection with invalid GSRDM */
    CNAS_XSD_SYS_ACQ_SCENE_RESCAN,                          /* SD ind with rescan reason */
    CNAS_XSD_SYS_ACQ_SCENE_PROTO_MISMATCH,                  /* SD ind with proto mismatch reason with redirection flag is false */
    CNAS_XSD_SYS_ACQ_SCENE_ACCESS_DENINED,                  /* SD ind with access denied reason */
    CNAS_XSD_SYS_ACQ_SCENE_ACCT_BLOCKED,                    /* SD ind with acct blocked reason */
    CNAS_XSD_SYS_ACQ_SCENE_NDSS_OFF,                        /* SD ind with ndss off reason */
    CNAS_XSD_SYS_ACQ_SCENE_ERR,                             /* SD ind with error ind reason */
    CNAS_XSD_SYS_ACQ_SCENE_REGIS_REJ_WITH_REDIR_AND_RTF,    /* SD ind with register reject reason with redirection flag is ture and return_if_fail is true */
    CNAS_XSD_SYS_ACQ_SCENE_REGIS_REJ,                       /* SD ind with regis rej reason */
    CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_IN_PRL_AND_PREF,       /* SD ind with system resel reason while the current system is in PRL and is prefered system */
    CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_NOT_IN_PRL_OR_NEG,     /* SD ind with system resel reason while the current system is not in PRL or negative in PRL */
    CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_IN_PRL,       /* SD ind with max access failure reason while the current system is in PRL */
    CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_NOT_IN_PRL,   /* SD ind with max access failure reason while the current system is not in PRL */
    CNAS_XSD_SYS_ACQ_SCENE_ABNORMAL,                        /* SD ind with abnormal ind reason */
    CNAS_XSD_SYS_ACQ_SCENE_CFREQ_LOCK,                      /* ��Ƶ������������ */
    CNAS_XSD_SYS_ACQ_SCENE_CALL_MRU0,                       /* MRU0���� */

    CNAS_XSD_SYS_ACQ_SCENE_NO_RF,                           /* ����Ƶ��Դ���� */

    CNAS_XSD_SYS_ACQ_SCENE_EMC_CALLBACK_SYSTEM_LOST,        /* CallBackģʽ��15s���� */
    CNAS_XSD_SYS_ACQ_SCENE_EMC_CALL_RELEASED,               /* �������ͷų��� */

    CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_SKIP_MRU0,            /* ͬsystem lost��ֻ������ʱ����MRU0  */
    CNAS_XSD_SYS_ACQ_SCENE_REGIS_REJ_WITH_REDIR_AND_RTF_EMC,    /* ������������CNAS_XSD_SYS_ACQ_SCENE_REGIS_REJ_WITH_REDIR_AND_RTF���� */
    CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_IN_PRL_AND_PREF_EMC,       /* ������������CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_IN_PRL_AND_PREF���� */
    CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_NOT_IN_PRL_OR_NEG_EMC,     /* ������������CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_NOT_IN_PRL_OR_NEG���� */
    CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_SKIP_MRU0_EMC,           /* ������������CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_SKIP_MRU0���� */
    CNAS_XSD_SYS_ACQ_SCENE_REDIRECTION_WITH_INVALID_GSRDM_EMC,  /* ������������CNAS_XSD_SYS_ACQ_SCENE_REDIRECTION_WITH_INVALID_GSRDM���� */
    CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_IN_PRL_EMC,       /* ������������CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_IN_PRL���� */
    CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_NOT_IN_PRL_EMC,   /* ������������CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_NOT_IN_PRL���� */
    CNAS_XSD_SYS_ACQ_SCENE_CALL_RELEASE_SYSTEM_LOST,

    CNAS_XSD_SYS_ACQ_SCENE_POWER_ON_INIT_SEARCH,

    CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN,     /* SRLTEģʽ�£�lte ����hrpd����̬��ʱ��1x����������SVLTEģʽ�£�hrpd����̬��ʱ��1x��������   */

    CNAS_XSD_SYS_ACQ_SCENE_SERVICE_ACQ,                         /* ҵ�񴥷������� */

    CNAS_XSD_SYS_ACQ_SCENE_BUTT
};
typedef VOS_UINT32 CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32;


enum CNAS_XSD_SCAN_CHAN_STATUS_ENUM
{
    CNAS_XSD_SCAN_CHAN_STATUS_NO_SYNCED,                        /* δ��������ǰ�������Ƿ���ڸ�Ƶ�㻹��ȷ�� */
    CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_EXIST,                     /* ����������ǰ�����д��ڶ�ӦƵ�� */
    CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,                  /* ����������ǰ�����в����ڶ�ӦƵ�㣬����δ���Բ��� */

    CNAS_XSD_SCAN_CHAN_STATUS_BUTT
};
typedef VOS_UINT8 CNAS_XSD_SCAN_CHAN_STATUS_ENUM_UINT8;


enum CNAS_XSD_AVAIL_SYS_STATUS_ENUM
{
    CNAS_XSD_AVAIL_SYS_STATUS_NO_ACQUIRED,                  /* δ����*/
    CNAS_XSD_AVAIL_SYS_STATUS_ACQUIRED_SUCC,                /* �Ѳ���ɹ� */
    CNAS_XSD_AVAIL_SYS_STATUS_ACQUIRED_FAIL,                /* �Ѳ���ʧ�� */
    CNAS_XSD_AVAIL_SYS_STATUS_BUTT
};
typedef VOS_UINT8 CNAS_XSD_AVAIL_SYS_STATUS_ENUM_UINT8;


enum CNAS_XSD_SYS_TYPE_ENUM
{
    CNAS_XSD_SYS_TYPE_PREFERRED,
    CNAS_XSD_SYS_TYPE_AVAILABLE,
    CNAS_XSD_SYS_TYPE_NEGATIVE,
    CNAS_XSD_SYS_TYPE_BUTT
};
typedef VOS_UINT8 CNAS_XSD_SYS_TYPE_ENUM_UINT8;



enum CNAS_XSD_ACQ_SYS_REGION_IND_ENUM
{
    CNAS_XSD_ACQ_SYS_REGION_IND_NEW = 0,  /* ����ϵͳ�б��еĵ�һ��ϵͳ��¼,����Ϊ0,����������������¼ʱ�µ�ϵͳ���¼Ҳ����Ϊ0 */
    CNAS_XSD_ACQ_SYS_REGION_IND_SAME,

    CNAS_XSD_ACQ_SYS_REGION_IND_BUTT
};
typedef VOS_UINT8 CNAS_XSD_ACQ_SYS_REGION_IND_ENUM_UINT8;


enum CNAS_XSD_CALL_EXIST_FLAG_ENUM
{
    CNAS_XSD_CALL_EXIST_NULL_FLAG                    = 0x00,
    CNAS_XSD_NORMAL_VOICE_CALL_EXIST_FLAG            = 0x01,
    CNAS_XSD_EMC_VOICE_CALL_EXIST_FLAG               = 0x02,
    CNAS_XSD_NORMAL_DATA_CALL_EXIST_FLAG             = 0x04,
};
typedef VOS_UINT8 CNAS_XSD_CALL_EXIST_FLAG_ENUM_UINT8;


enum CNAS_XSD_SYSTEM_ACQUIRE_MODE_ENUM
{
    CNAS_XSD_SYSTEM_ACQUIRE_MODE_ANY                    = 0,   /* try to acquire any system without regard to
                                                                  PRL designation(event if the negative system in PRL or
                                                                  a available system when pref_only is true) */

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_ALLOWED                = 1,   /* try to acquire any system that is allowed by PRL
                                                                  (should ignore negative system and avaiable system when
                                                                  pref_only is true)*/

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_SPECIFIC               = 2,   /* try to acquire only a system for which the
                                                                  SID/NID is known(for example frequency lock) without regard to
                                                                  PRL designation(event if the negative system in PRL or
                                                                  a available system when pref_only is true) */

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_SPECIFIC_PREFEREED     = 3,   /* try to acquire only a system for which the
                                                                  SID/NID is known and the system must be allowed by the PRL
                                                                  (should ignore negative system) */

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_PREFERED               = 4,   /* try to acquire only a prefered PRL system */

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_BEST_PREF              = 5,   /* try to acquire only a system which is the most prefered in the PRL */

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_MORE_PREF              = 6,   /* try to acquire only a system that is more
                                                                   prefered than the idle system */

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_NOT_LESS_PREF          = 7,   /* try to acquire only a system that is not less
                                                                   prefered than the idle system(for example silent redial) */

    CNAS_XSD_SYSTEM_ACQUIRE_MODE_BUTT
};
typedef VOS_UINT8 CNAS_XSD_SYSTEM_ACQUIRE_MODE_ENUM_UINT8;



enum CNAS_XSD_OOC_SCAN_STRATEGY_ENUM
{
    CNAS_XSD_OOC_SCAN_NORMAL,                           /* OOC��scan list�б�������ʽ���� */
    CNAS_XSD_OOC_SCAN_REACQ_0_1_2_3_4S,                 /* OOC��scan list�б�����ÿ0��1��2��3��4������Ƶ������һ��MRU0Ƶ�㣬�Դ����� */
    CNAS_XSD_OOC_SCAN_PING_5,                           /* OOC��scan list�б�����ÿ5������Ƶ������1��MRU0Ƶ�㣬�Դ����� */
    CNAS_XSD_OOC_SCAN_2_7,                              /* OOC��scan list�б�����ÿ5������Ƶ������2��MRU0Ƶ�㣬�Դ����� */

    CNAS_XSD_OOC_SCAN_BUTT
};
typedef VOS_UINT8 CNAS_XSD_OOC_SCAN_STRATEGY_ENUM_UINT8;


enum CNAS_XSD_NEG_PREF_SYS_CMP_CTRL_ENUM
{
    CNAS_XSD_NEG_PREF_SYS_CMP_BAND_CHAN_AMBIGUOUS_MATCH,                /* Band Channel ģ��ƥ�� */
    CNAS_XSD_NEG_PREF_SYS_CMP_BAND_CHAN_ACCURATE_MATCH,                 /* Band Channel ��ȷƥ��*/
    CNAS_XSD_NEG_PREF_SYS_CMP_BUTT
};
typedef VOS_UINT8 CNAS_XSD_NEG_PREF_SYS_CMP_TYPE_ENUM_UINT8;


enum CNAS_XSD_EMC_STATE_ENUM
{
    CNAS_XSD_EMC_STATE_NULL             = 0x00,             /* ��ս���������״̬ */
    CNAS_XSD_EMC_STATE_CALLBACK         = 0x01,             /* ������CALLBACKģʽ״̬ */

    CNAS_XSD_EMC_STATE_BUTT
};
typedef VOS_UINT8 CNAS_XSD_EMC_STATE_ENUM_UINT8;


enum CNAS_XSD_SERVICE_STATUS_ENUM
{
    CNAS_XSD_SERVICE_STATUS_NO_SERVICE                      = 0,        /* �޷���   */
    CNAS_XSD_SERVICE_STATUS_LIMITED_SERVICE                 = 1,        /* ���Ʒ��� */
    CNAS_XSD_SERVICE_STATUS_NORMAL_SERVICE                  = 2,        /* �������� */
    CNAS_XSD_SERVICE_STATUS_BUTT
};
typedef VOS_UINT32 CNAS_XSD_SERVICE_STATUS_ENUM_UINT32;


enum CNAS_XSD_AVOID_REASON_ENUM
{
    CNAS_XSD_AVOID_P_REV_MISMATCH,                  /* P_REV mismatch */
    CNAS_XSD_AVOID_SID_NID_IS_REJ_BY_PRL,           /* System's SID/NID is rejected by PRL */
    CNAS_XSD_AVOID_SYNC_CH_ACQUIRED_PAGE_CH_FAIL,   /* Sync channel is acquired but not the paging channel */
    CNAS_XSD_AVOID_BAD_OVERHEAD_MESSAGES,           /* Bad overhead messages */
    CNAS_XSD_AVOID_REG_REJECTED,                    /* Registeration is rejected */
    CNAS_XSD_AVOID_MAX_ACCESS_PROBES,               /* Max access probes */
    CNAS_XSD_AVOID_ACCT_BLOCKED_IND,                /* ACCT blocked indication */
    CNAS_XSD_AVOID_ERROR_IND,                       /* ERROR indication */
    CNAS_XSD_AVOID_REDIRECTION_FAIL,                                    /* redirection fail with return_if_fail is false */
    CNAS_XSD_AVOID_REDIRECTION_WITH_INVALID_GSRDM,                      /* redirection with invalid GSRDM and return_if_fail is false */
    CNAS_XSD_AVOID_ACCESS_DENIED,                                       /* access denied */
    CNAS_XSD_AVOID_SYSTEM_RESELECTION,                                  /* system reselection */
    CNAS_XSD_AVOID_REASON_MAX,

    CNAS_XSD_AVOID_REASON_BUTT
};
typedef VOS_UINT8 CNAS_XSD_AVOID_REASON_ENUM_UINT8;

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
    VOS_UINT32                          ulEventType;
    VOS_UINT8                           aucMsgBuffer[CNAS_XSD_MAX_MSG_BUFFER_LEN];
}CNAS_XSD_MSG_STRU;


typedef struct
{
    CNAS_XSD_MSG_STRU                   stCacheMsg;         /* ����ľ������� */
}CNAS_XSD_CACHE_MSG_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucCacheMsgNum;                          /* ������Ϣ�ĸ��� */
    VOS_UINT8                           aucReserve[3];
    VOS_UINT8                          *pastCacheMsg[CNAS_XSD_MAX_CACHE_MSG_QUEUE_NUM]; /* XSD�Ļ�����Ϣָ������ */
}CNAS_XSD_CACHE_MSG_QUEUE_STRU;

/** ****************************************************************************
 * Name        : CNAS_XSD_MRU_LIST_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucSysNum;
    VOS_UINT8                           aucReserve[3];
    CNAS_PRL_1X_SYSTEM_STRU             astSystem[CNAS_XSD_MAX_MRU_SYS_NUM];
}CNAS_XSD_MRU_LIST_STRU;


typedef struct
{
    VOS_UINT8                           ucReadNvPrlDirectly;
    VOS_UINT8                           ucReadDefaultPrl;        /* ��ȡDefault Prl */
    VOS_UINT8                           ucNoCardModeCfgFlg;
    VOS_UINT8                           ucIsMod1xAvailTimerLen;

    VOS_UINT8                           ucPrlCombinedFlag;      /* NV PRL������־ */
    VOS_UINT8                           aucReserved[3];
}CNAS_XSD_TEST_CONFIG_STRU;



typedef struct
{
    VOS_UINT32                          ulCallBackEnableFlg;        /* �Ƿ�֧��CallBackģʽ */
    VOS_UINT32                          ulCallBackModeTimerLen;     /* CallBack��ʱ��ʱ�� */
} CNAS_XSD_CALLBACK_CFG_STRU;


typedef struct
{
    VOS_UINT16                          usChannel;
    CNAS_PRL_BAND_CLASS_ENUM_UINT16     enBandClass;
} CNAS_XSD_FREQUENCY_CHANNEL_STRU;


typedef struct
{
    VOS_UINT8                           ucEnableFlg;
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          usFreqNum;
    CNAS_XSD_FREQUENCY_CHANNEL_STRU     astFreqList[CNAS_XSD_MAX_CDMA_1X_CUSTOM_PREF_CHANNELS_NUM];
} CNAS_XSD_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU;


typedef struct
{
    VOS_UINT8                           ucIsNegSysAdd;
    VOS_UINT8                           aucReserve[15];
}CNAS_XSD_ADD_AVOID_LIST_CFG_STRU;



typedef struct
{
    CNAS_XSD_NEG_PREF_SYS_CMP_TYPE_ENUM_UINT8               enNegPrefSysCmpType;
    VOS_UINT8                                               aucReserve[15];
}CNAS_XSD_NEG_PREF_SYS_CMP_CTRL_STRU;


typedef struct
{
    CNAS_XSD_ADD_AVOID_LIST_CFG_STRU                        stAddAvoidListCfg;
    CNAS_XSD_NEG_PREF_SYS_CMP_CTRL_STRU                     stNegPrefSysCmpCtrl;
}CNAS_XSD_SYS_ACQ_NVIM_CFG_STRU;



typedef struct
{
    VOS_UINT8                                                   ucFreqLockMode;
    VOS_UINT8                                                   aucReserve[1];
    VOS_UINT16                                                  usCdmaBandClass;
    VOS_UINT16                                                  usSid;
    VOS_UINT16                                                  usNid;
    VOS_UINT16                                                  usCdmaFreq;
    VOS_UINT16                                                  usCdmaPn;
}CNAS_XSD_CFREQ_LOCK_SET_PARA_STRU;


typedef struct
{
    VOS_UINT32                          BandCDMA_0            :1;/* 800 MHz cellular band */

    VOS_UINT32                          BandCDMA_1            :1;/* 1.8 to 2.0 GHz PCS band */

    VOS_UINT32                          BandCDMA_2            :1;/* 872 to 960 MHz TACS band */

    VOS_UINT32                          BandCDMA_3            :1;/* 832 to 925 MHz JTACS band */

    VOS_UINT32                          BandCDMA_4            :1;/* 1.75 to 1.87 GHz Korean PCS band */

    VOS_UINT32                          BandCDMA_5            :1;/* 450 MHz NMT band */

    VOS_UINT32                          BandCDMA_6            :1;/* 2 GHz IMT-2000 band */

    VOS_UINT32                          BandCDMA_7            :1;/* 700 MHz band */

    VOS_UINT32                          BandCDMA_8            :1;/* 1800 MHz band */

    VOS_UINT32                          BandCDMA_9            :1;/* 900 MHz band */

    VOS_UINT32                          BandCDMA_10           :1;/* Secondary 800 MHz band */

    VOS_UINT32                          BandCDMA_11           :1;/* 400 MHz European PAMR band */

    VOS_UINT32                          BandCDMA_12           :1;/* 800 MHz PAMR band */

    VOS_UINT32                          BandCDMA_13           :1;/* 2.5 GHz IMT-2000 Extension Band */

    VOS_UINT32                          BandCDMA_14           :1;/* US PCS 1.9GHz Band */

    VOS_UINT32                          BandCDMA_15           :1;/* AWS Band */

    VOS_UINT32                          BandCDMA_16           :1;/* US 2.5GHz Band */

    VOS_UINT32                          BandCDMA_17           :1;/* US 2.5GHz Forward Link Only Band */

    VOS_UINT32                          BandCDMA_18           :1;/* 700 MHz Public Safety Band */

    VOS_UINT32                          BandCDMA_19           :1;/* Lower 700 MHz Band */

    VOS_UINT32                          BandCDMA_20           :1;/* L-Band */

    VOS_UINT32                          BandCDMA_21           :1;/* S-Band */

    VOS_UINT32                          BandCDMA_22           :1;

    VOS_UINT32                          BandCDMA_23           :1;

    VOS_UINT32                          BandCDMA_24           :1;

    VOS_UINT32                          BandCDMA_25           :1;

    VOS_UINT32                          BandCDMA_26           :1;

    VOS_UINT32                          BandCDMA_27           :1;

    VOS_UINT32                          BandCDMA_28           :1;

    VOS_UINT32                          BandCDMA_29           :1;

    VOS_UINT32                          BandCDMA_30           :1;

    VOS_UINT32                          BandCDMA_31           :1;/* Wildcard Band Class */
}CNAS_XSD_1X_BAND_STRU;


typedef union
{
    VOS_UINT32                          ulBand;

    CNAS_XSD_1X_BAND_STRU               stBitBand;
}CNAS_XSD_1X_BAND_UN;

/** ****************************************************************************
 * Name        : CNAS_XSD_ACQUIRED_SYS_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    CNAS_PRL_1X_SYSTEM_STRU             stSystem;
    CNAS_XSD_ACQUIRED_CAUSE_ENUM_UINT8  enCause;
    VOS_UINT8                           ucAcqCounter;
    VOS_UINT8                           aucReserve[2];
}CNAS_XSD_ACQUIRED_SYS_STRU;


/** ****************************************************************************
 * Name        : CNAS_XSD_AVOID_SYSTEM_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucAvoidFlag;                 /* ʹ�ñ�־ */
    VOS_UINT8                           aucReserved[3];
    VOS_UINT32                          ulStartSlice;               /* ��ʼslice */
    VOS_UINT32                          ulExpiredSliceNum;          /* ���ڵ�slice���� */
    CNAS_PRL_FREQENCY_CHANNEL_STRU      stAvoidFreq;
    VOS_UINT8                           aucAvoidTimes[CNAS_XSD_AVOID_REASON_MAX];  /* CNAS_XSD_AVOID_REASON_MAX = 12*/
}CNAS_XSD_AVOID_FREQ_INFO_STRU;


typedef struct
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU          stFreqChan;
    CNAS_XSD_SCAN_CHAN_STATUS_ENUM_UINT8    enChanStatus;                       /* Ƶ�������״̬ */
    VOS_UINT8                               aucRsv[3];
}CNAS_XSD_SCAN_CHANNEL_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_XSD_REDIRECTION_INFO_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucRedirection;
    VOS_UINT8                           ucReturnIfFail;
    VOS_UINT16                          usExpectSid;
    VOS_UINT16                          usExpectNid;

    VOS_UINT8                           ucNdssInd;
    VOS_UINT8                           ucChanNum;       /**<  record the number of frequency channels in astFreqChanList */
    CNAS_XSD_SCAN_CHANNEL_INFO_STRU     astChanInfo[CNAS_XSD_MAX_REDIR_CHANNEL_NUM];
    CNAS_PRL_1X_SYSTEM_STRU             stOriginalSystem;
    VOS_UINT16                          usCurScanIndex;
    VOS_UINT8                           ucIsEmcRedir;    /* �ض�������н�������ʶ */
    VOS_UINT8                           ucReserved;
}CNAS_XSD_REDIRECTION_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_XSD_AVAIL_SYSTEM_INFO_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          stFreq;
}CNAS_XSD_AVAIL_SYSTEM_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_XSD_AVAILABLE_LIST_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                                               ucAvailSysNum;
    VOS_UINT8                                               ucNextSrchIndex;    /* �´ε�������������ʼΪ0 */
    VOS_UINT8                                               aucRsved[2];
    CNAS_XSD_AVAIL_SYSTEM_INFO_STRU                         astAvailSystem[CNAS_XSD_MAX_AVAILABLE_SYS_NUM];
}CNAS_XSD_AVAILABLE_LIST_STRU;

/** ****************************************************************************
 * Name        : CNAS_XSD_BSR_INFO_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucIsPrlAcqTable;
    VOS_UINT8                           aucReserve[1];
    VOS_UINT16                          usIndex;            /**<  record last acquired preferred system index */
    CNAS_PRL_1X_SYSTEM_STRU             astPreferredSystem[CNAS_XSD_MAX_PREFERRED_SYSTEM_NUM];
}CNAS_XSD_BSR_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_XSD_AVOID_FREQ_LIST_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucAvoidFreqNum;
    VOS_UINT8                           ucNextAcqIndex;    /* �´���Ҫ��ȡ��AvoidƵ��������Ϣ */
    VOS_UINT8                           aucReserve[2];
    CNAS_XSD_AVOID_FREQ_INFO_STRU       astAvoidFreqInfo[CNAS_XSD_MAX_AVOID_FREQ_NUM];
}CNAS_XSD_AVOID_FREQ_LIST_STRU;

/** ****************************************************************************
 * Name        : CNAS_XSD_CALL_REDIAL_SYS_LIST_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usSysNum;
    VOS_UINT16                          usIndex;         /**<  record last acquired preferred system index */
    CNAS_PRL_1X_SYSTEM_STRU             astRedialSystem[CNAS_XSD_MAX_REDIAL_SYSTEM_NUM];
}CNAS_XSD_CALL_REDIAL_SYS_LIST_STRU;



typedef struct
{
    CNAS_PRL_1X_SYSTEM_STRU                                 stAcqSys;                           /* ��Ҫ�����ϵͳID */
    VOS_UINT16                                              usLevel;                            /* ϵͳ��GEO��Level, level0Ϊ������ȼ� */
    CNAS_XSD_SCAN_CHAN_STATUS_ENUM_UINT8                    enSysStatus;
    CNAS_PRL_PREF_NEG_SYS_ENUM_UINT8                        enPrefNegSys;                       /* ��Ƶ��ɨ���б��е�����,�����Ӧ��Ƶ��״̬ˢ�� */
    CNAS_PRL_SYS_ROAMING_IND_ENUM_UINT8                     enRoamingInd;                       /* ָʾϵͳ�������ԣ����Ρ������� */

    VOS_UINT8                                               aucReserved[3];
}CNAS_XSD_ACQUIRE_SYSTEM_INFO_STRU;


typedef struct
{
    VOS_UINT16                          usTotalNum;                             /* �б��б����chan���� */
    VOS_UINT16                          usCurScanIndex;                         /* ��ǰ��Ҫɨ���Ƶ������,Ĭ��Ϊ0,ͬ�����һ���¼��ָ���¸�����
                                                                                   ,����ϵͳ��������Ҳ��ˢ��Ƶ��ɨ��״̬,��˵�ǰ��ɨ����������ȷ��
                                                                                    ����ͬ������Ƶ��,ֻ��ʾ����λ��,����б����Ч�� */

    VOS_UINT16                          usMaxScanChanSize;                      /* ���ɨ��Ƶ����� */
    VOS_UINT8                           aucReserve[2];
    CNAS_XSD_SCAN_CHANNEL_INFO_STRU    *pstScanChanInfo;                        /* ��Ҫɨ���Ƶ���б��׵�ַ:�����ܸ��������ڴ� */

}CNAS_XSD_SCAN_CHANNEL_LIST_STRU;


typedef struct
{
    VOS_UINT8                                               ucRedialCount;
    VOS_UINT8                                               aucReserve[3];
    VOS_UINT8                                               aucRedialTimes[CNAS_XSD_MAX_MRU_SYS_NUM];  /* ��������ʧ��ʱ��ǰפ����Ƶ����mru���ز���������������������*/
    CNAS_PRL_1X_SYSTEM_STRU                                 stLastCampedSys;
    CNAS_XSD_MRU_LIST_STRU                                  stEmcRedialMruList;
}CNAS_XSD_EMC_REDIAL_SYS_ACQ_CFG_STRU;


typedef struct
{
    VOS_UINT16                          usCfChannelNum;
    VOS_UINT8                           aucReserved[2];
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astChannelList[CNAS_XSD_MAX_CANDIDATE_FREQ_NUM];    /* ��Ƶ����Ƶ����Ϣ */
}CNAS_XSD_EMC_CALLBACK_CANDIDATE_FREQ_INFO_STRU;


typedef struct
{
    VOS_UINT16                                              usGeoNum;
    VOS_UINT16                                              usReserved;
    VOS_UINT16                                              ausGeoFirstIndex[CNAS_PRL_MAX_MATCHING_GEO_NUM];
}CNAS_XSD_CUR_SYNC_GEO_LIST_INFO_STRU;


typedef struct
{
    VOS_UINT16                                              usTotalNum;                             /* �б��б����sys�ܸ��� */
    VOS_UINT16                                              usMaxAcqSysSize;                        /* ��󲶻�ϵͳ���� */
    VOS_UINT16                                              usCurSrchLevel;                         /* ��ǰ�������� */
    VOS_UINT16                                              usReserved;
    CNAS_XSD_CUR_SYNC_GEO_LIST_INFO_STRU                    stCurSyncGeoInfo;                       /* ��ǰͬ����GEO list��Ϣ */
    CNAS_XSD_ACQUIRE_SYSTEM_INFO_STRU                      *pstAcqSysInfo;                         /* ��Ҫ�����ϵͳ��¼�б�ָ��:�����ܸ��������ڴ� */
}CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU;



typedef struct
{
    CNAS_PRL_1X_SYSTEM_STRU             stSystem;
    VOS_UINT32                          ulMcc;
    CNAS_XSD_SERVICE_STATUS_ENUM_UINT32 enSrvStatus;
    VOS_UINT16                          usImsi11_12;
    VOS_UINT8                           ucRoamingInd;
    CNAS_XSD_SYS_TYPE_ENUM_UINT8        enSysType;
    VOS_UINT8                           ucCampOnFlag;
    VOS_UINT8                           aucRsved[3];
}CNAX_XSD_CAMPED_SYS_INFO_STRU;



typedef struct
{
    VOS_UINT8                           ucCallExistFlg;              /* call exist flag */
    VOS_UINT8                           ucOrigSysExistFlg;           /* ���з���ϵͳ�Ƿ���� */
    VOS_UINT8                           aucReserved[2];              /* reserved bytes */
    CNAS_PRL_1X_SYSTEM_STRU             stCallOrignalSys;            /* ���п�ʼʱ��פ��ϵͳ */
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU     stCallRedialChanScanList;    /* �����ز������µ�Ƶ���б� */
}CNAS_XSD_CALL_REDIAL_CTRL_INFO_STRU;


typedef struct
{
    CNAS_XSD_EMC_STATE_ENUM_UINT8                           enEmcState;                  /* ����������״̬ */
    VOS_UINT8                                               ucCallBackSrchCounter;       /* CallBack 15s���������� */
    VOS_UINT8                                               aucReserved[2];              /* reserved bytes */
    CNAS_XSD_CALLBACK_CFG_STRU                              stCallBackCfg;               /* ������CALLBACKģʽ���� */
    CNAS_PRL_1X_SYSTEM_STRU                                 stCallOrignalSys;            /* ��������ͨʱ��פ��ϵͳ */
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                         stEmcCallRedialChanScanList; /* �������ز������µ�Ƶ���б� */
    CNAS_XSD_EMC_REDIAL_SYS_ACQ_CFG_STRU                    stEmcRedialSysAcqCfg;        /* ������������������ */
    CNAS_XSD_EMC_CALLBACK_CANDIDATE_FREQ_INFO_STRU          stEmcCallBackCfInfo;         /* �������غ�ģʽ��Ƶ����Ƶ����Ϣ */
} CNAS_XSD_EMC_CALL_CTRL_INFO_STRU;



typedef struct
{
    VOS_UINT8                                               ucWaitSearchFlag;           /* OOC�ȴ�������־������MRU0����ʱ��OOC��ʱ������ */
    VOS_UINT8                                               ucCurrentPhase;             /* ��¼��ǰ�����׶� */
    VOS_UINT16                                              usCurrentTimes;             /* ��¼�ڵ�ǰ�׶��Ѿ��������� */
    VOS_UINT8                                               ucFirstFourAcqArrivedFlag;  /* ��¼DO��Traffic�£�1Xǰ4�β����Ƿ�ﵽ */
    VOS_UINT8                                               ucAttemptTimesInDoTraffic;  /* ��¼DO��traffic�£�1X�Ѿ����ԵĴ����������ĴΣ�����ά���ñ�����
                                                                                           ͨ��ucFirstFourAcqFlagά�� */
    VOS_UINT8                                               ucSceneSetFlag;
    VOS_UINT8                                               ucTotalTimerExpiredFlag;    /* ��ʱ����ʱ����ʱ��� */
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enOocSearchScene;
}CNAS_XSD_OOC_CTX_INFO_STRU;




typedef struct
{
    VOS_UINT16                          usTotalTimeLen;         /* 1xһ���׶�������ʱ��,��λΪ�� */
    VOS_UINT16                          usSleepTimeLen;         /* 1x����һ�ּ��ʱ��,��λΪ�� */
    VOS_UINT8                           ucSrchNum;              /* 1xһ���׶��������� */
    VOS_UINT8                           aucRsv[3];
}CNAS_XSD_OOS_SYS_ACQ_STRATEGY_PATTERN_STRU;


typedef struct
{
    VOS_UINT16                                              usMru0SearchTimerLen;       /* ����MRU0ʱavailable��ʱ����ʱ��,ÿ���׶ε�ʱ��,��λΪ��,Ĭ��ֵΪ0 */
    VOS_UINT8                                               ucPhaseNum;                 /* ����������Ŀ */
    CNAS_XSD_OOC_SCAN_STRATEGY_ENUM_UINT8                   enOocScanStrategy;          /* Ƶ���ظ��������� */
    VOS_UINT8                                               uc1xOocDoTchPhase1TimerLen; /* Do TCH��ǰ4�γ��� available Timer ���ʱ��,��λ�� */
    VOS_UINT8                                               uc1xOocDoTchPhase2TimerLen; /* Do TCH��4�����ϳ��� available Timer ���ʱ��,��λ�� */
    VOS_UINT8                                               aucReserved[2];
    CNAS_XSD_OOS_SYS_ACQ_STRATEGY_PATTERN_STRU              astPhaseCfgInfo[CNAS_XSD_MAX_OOC_SCHEDULE_PHASE_NUM];
}CNAS_XSD_OOS_SYS_ACQ_STRTEGY_CFG_STRU;



typedef struct
{
    CNAS_XSD_OOC_CTX_INFO_STRU                              stOocCtxInfo;
    CNAS_XSD_OOS_SYS_ACQ_STRTEGY_CFG_STRU                   stStrategyCfg;              /* ����OOS�����£�����������Ϣ */
}CNAS_XSD_OOC_SCHEDULE_INFO_STRU;


typedef struct
{
    VOS_UINT16                                              usGeoBeginIndex;
    VOS_UINT16                                              usGeoSrchStatus;
}CNAS_XSD_GEO_SRCH_STATUS_INFO_STRU;


typedef struct
{
    VOS_UINT16                                              usGeoNum;                   /* GEO��Ŀ */
    VOS_UINT16                                              usReserved;
    CNAS_XSD_GEO_SRCH_STATUS_INFO_STRU                     *pstGeoSrchInfo;             /* ָ��GEO������Ϣ�׵�ַ */
}CNAS_XSD_GEO_LIST_SRCH_INFO_STRU;


typedef struct
{
    VOS_UINT32                          ulTotalNum;                                     /* ���ܱ���Ƶ��������� */
    VOS_UINT32                          ulCurNum;                                       /* ��ǰͬ������Ƶ����Ŀ */
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astFreq[CNAS_XSD_1X_SYNC_SUCC_FREQ_MAX_NUM];    /* ����ͬ����ϵͳ��Ƶ�� */
}CNAS_XSD_HISTORY_SYNCED_SYS_FREQ_LIST_STRU;


typedef struct
{
    VOS_UINT8                           aucAvoidTimerLen[CNAS_XSD_AVOID_REASON_MAX][CNAS_XSD_AVOID_MAX_PHASE_NUM];
}CNAS_XSD_AVOID_SCHEDULE_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucEnable;
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          usMru0Times;
}CNAS_XSD_MRU0_SWITCH_ON_OOC_STRATEGY_CFG_STRU;


typedef struct
{
    VOS_UINT8                           ucHrpdConnExistFlg;
    VOS_UINT8                           ucLteEpsConnExistFlg;
    VOS_UINT8                           aucRsv[2];
}CNAS_XSD_LTE_HRPD_CONN_STATUS_INFO_STRU;

typedef struct
{
    VOS_UINT32                                              ul1xSysAcqNoRfProtectTimerLen;
    VOS_UINT32                                              ul1xRedirNoRfProtectTimerLen;
}CNAS_XSD_1X_SYS_ACQ_NO_RF_PROTECT_TIMER_INFO_STRU;


typedef struct
{
    VOS_UINT8                                               uc1xSysAcqSyncDelayEnable;          /* �ӳ�ͬ��nv���ƿ���  */
    VOS_UINT8                                               ucRcv;
    VOS_UINT16                                              us1xSysAcqSyncDelayFreqNum;         /* �ӳ�ͬ��Ƶ����� */
    VOS_UINT32                                              ul1xSysAcqDelayTimerLen;            /* ����ͬ������֮��Ļ��嶨ʱ��ʱ�� */
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          austSysAcqSyncDelayFreqInfo[CNAS_XSD_MAX_FREQ_NUM_WHEN_LTE_DO_CONN];               /* ������һ����Ҫͬ����Ƶ�� */
}CNAS_XSD_1X_SYS_ACQ_SYNC_DELAY_INFO_WHEN_LTE_OR_DO_CONN_STRU;
/** ****************************************************************************
 * Name        : CNAS_XSD_SYSTEM_ACQUIRE_CTRL_STRU
 * Description :
 *******************************************************************************/
typedef struct
{
    CNAS_XSD_REDIRECTION_INFO_STRU                          stRedirInfo;
    CNAS_XSD_BSR_INFO_STRU                                  stBsrInfo;
    CNAS_XSD_AVOID_FREQ_LIST_STRU                           stAvoidFreqList;
    CNAS_XSD_MRU_LIST_STRU                                  stMruList;
    CNAS_XSD_CALL_REDIAL_SYS_LIST_STRU                      stCallList;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                         stChanScanList;     /* ϵͳͬ��ʱʹ�õ���Ƶ���б�,ȫ��ά��,���ܲ�ͬ����
                                                                                   �����Ƶ���б�һ��:��ʼ��ʱ����ѿռ�,�ػ�ʱ��Ҫ�ͷ� */

    CNAS_XSD_HISTORY_SYNCED_SYS_FREQ_LIST_STRU              stHisSyncedSysFreqList;     /* ͬ��������ʷϵͳƵ���б� */
    CNAS_XSD_GEO_LIST_SRCH_INFO_STRU                        stGeoListSrchInfo;          /* GEO list������Ϣ����¼GEO������״̬ */
    CNAX_XSD_CAMPED_SYS_INFO_STRU                           stCurCampedSysInfo;
    CNAS_XSD_1X_BAND_UN                                     unUeSupportedBand;

    CNAS_XSD_CALL_REDIAL_CTRL_INFO_STRU                     stCallRedialInfo;

    CNAS_XSD_CFREQ_LOCK_SET_PARA_STRU                       stFreqLockPara;

    CNAS_XSD_OOC_SCHEDULE_INFO_STRU                         stOocScheduleInfo;

    CNAS_XSD_EMC_CALL_CTRL_INFO_STRU                        stEmcCallInfo;      /* ��������Ϣ */

    CNAS_XSD_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU              stCdma1XCustomPrefChan; /* ���ų���Ƶ���Ƶ����Ϣ */

    CNAS_XSD_AVOID_SCHEDULE_INFO_STRU                       stAvoidScheduInfo;

    NAS_MSCC_PIF_1X_OOS_CAUSE_ENUM_UINT32                   enOosSpecialSdReason;
    NAS_ERR_LOG_1X_SEARCH_RSLT_ENUM_UINT32                  enAbortedCause;

    VOS_UINT8                                               ucSrvAcqFlg;        /* ��¼��ǰ�Ƿ���ҵ�񴥷������ı�ʶ */
    VOS_UINT8                                               ucIsConsiderRoamIndInPRL; /* �ж�����ϵͳ���Ƿ���ROMA IND���Եı�־ */

    VOS_UINT8                                               aucReserved[2];           /* ����λ */

    CNAS_XSD_MRU0_SWITCH_ON_OOC_STRATEGY_CFG_STRU           stMru0StrategyCfg;

    CNAS_XSD_LTE_HRPD_CONN_STATUS_INFO_STRU                 stLteHrpdConnStatus;        /* ��ǰLTE\HRPD����̬��־ */
    CNAS_XSD_1X_SYS_ACQ_NO_RF_PROTECT_TIMER_INFO_STRU       stSysAcqNoRfProtectTimerInfo;
    CNAS_XSD_1X_SYS_ACQ_SYNC_DELAY_INFO_WHEN_LTE_OR_DO_CONN_STRU                st1xSysAcqSyncDelayInfo;    /* HRPD����̬����SRLTE��LTE����̬������1x���磬��ʱͬ�������Ϣ */

    VOS_UINT32                                              ulSrlteSyncOneChanAddTimerLen;   /* ��λ:ms srlte�£�����Ƶ��ͬ������ʱ������ȡcas��3514 nv�ĵڶ������� */
}CNAS_XSD_SYSTEM_ACQUIRE_CTRL_STRU;


typedef struct
{
    VOS_UINT8                           ucExistNorVoiceCall;
    VOS_UINT8                           ucExistEmcVoiceCall;
    VOS_UINT8                           ucExistEmcDataCall;
    VOS_UINT8                           ucRsved;
}CNAS_XSD_CALL_INFO_STRU;


typedef struct
{
    VOS_UINT32                          ulXsdReadCardFileFlg;                   /* ��ǿ��ļ��Ƿ��ȡ��� */
    VOS_UINT8                           aucReserved[4];
}CNAS_XSD_FSM_SWITCH_ON_CTX_STRU;


typedef struct
{
    VOS_UINT8                           ucCampOnNeedFlg;                    /* �Ƿ���Ҫפ����� */
    VOS_UINT8                           ucBufCallRelIndFlg;                 /* ����ĺ����ͷ�ָʾ��� */
    VOS_UINT8                           aucReserved[2];
    CNAS_PRL_FREQENCY_CHANNEL_STRU      stBuffCallRelFreq;                  /* ����ĺ����ͷ�ָʾ��Ƶ����Ϣ */
}CNAS_XSD_FSM_POWER_OFF_CTX_STRU;



typedef struct
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU      stFreq;
    VOS_UINT16                          usSysNum;
    VOS_UINT8                           aucRsv[2];
    CNAS_PRL_1X_SYSTEM_ID_STRU          astSysId[CNAS_XSD_1X_MAX_SAME_PRIO_SYS_NUM];            /* ��Ҫ�����ϵͳID */
}CNAS_XSD_SYSTEM_WITH_SAME_CHANNEL_STRU;


typedef struct
{
    VOS_UINT16                              usSndSysNum;                        /* �����ϴη���CAS����Ҫ����ϵͳ�ĸ��� */
    VOS_UINT8                               aucRsved[2];
    CNAS_XSD_SYSTEM_WITH_SAME_CHANNEL_STRU  astSndSys[CNAS_XSD_1X_SND_CAS_MAX_SYSTEM_NUM];  /* �����ϴη���CAS����Ҫ�����ϵͳ */
}CNAS_XSD_LATEST_SND_CAS_ACQ_SYSTEM_INFO;


typedef struct
{
    VOS_UINT16                          usChanNum;       /* �����ͷ�ͬ����Ƶ����� */
    VOS_UINT8                           aucReserved[2];
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astFreq[CNAS_XSD_CALL_REL_SYNC_MAX_FREQ_NUM];   /* ��������ͷź��״���Ҫ������Ƶ�� */
}CNAS_XSD_CALL_REL_ANY_CAMP_ON_FREQ_STRU;


typedef struct
{
    VOS_UINT8                                               ucIsAvailSysAcqList;/* ��ǵ�ǰ�����б��Ƿ�Ϊavailable system list��Ĭ��ΪVOS_FALSE */
    VOS_UINT8                                               ucAbortFlg;         /* ��ǰ״̬����־�Ƿ��յ���ֹҪ��,VOS_TRUE:�յ�, VOS_FALSE:δ�յ� */
    VOS_UINT8                                               ucOocTotalTimerExpiredFlg; /* OOC��������ʱ����ʱ����ʱ��� */
    VOS_UINT8                                               ucIsAvoidSysAcqList;/* ��ǵ�ǰ�����б��Ƿ�Ϊavoid system list��Ĭ��ΪVOS_FALSE */

    CNAS_PRL_1X_SYSTEM_ID_STRU                              stLastSyncedSys;    /* ���ڼ�¼���һ��ͬ������ϵͳ��Ŀ��ΪSID����ϱ�SID��Ϣ */
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                  stGeoSysSrchList;
    /* ��available list��״̬�������� */
    CNAS_XSD_AVAILABLE_LIST_STRU                            stAvailSysList;
    CNAS_XSD_CALL_REL_ANY_CAMP_ON_FREQ_STRU                 stCallRelFirstSyncFreq;
}CNAS_XSD_FSM_SYS_ACQ_CTX_STRU;


typedef struct
{
    VOS_UINT32                          ulCurAvailableTimerCount;               /* ��ǰAvailable Timer�������� */
}CNAS_XSD_FSM_L1_MAIN_CTX_STRU;




typedef struct
{
    VOS_UINT8                               ucAbortFlag ;
    VOS_UINT8                               aucReserve[3];
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU         stRedireScanList;
}CNAS_XSD_FSM_REDIR_CTX_STRU;



typedef union
{
    CNAS_XSD_FSM_SWITCH_ON_CTX_STRU     stSwitchOnCtx;
    CNAS_XSD_FSM_POWER_OFF_CTX_STRU     stPowerOffCtx;
    CNAS_XSD_FSM_SYS_ACQ_CTX_STRU       stSysAcqCtx;
    CNAS_XSD_FSM_L1_MAIN_CTX_STRU       stL1MainCtx;
    CNAS_XSD_FSM_REDIR_CTX_STRU         stRedirCtx;
}CNAS_XSD_FSM_EXTRA_CTX_UNION;


typedef struct
{
    /* ״̬������ָ��,ָ��ΪNULL��ʾ��ǰ״̬����Ч  */
    NAS_FSM_DESC_STRU                  *pstFsmDesc;

    /* ��ǰ״̬����ʶ */
    CNAS_XSD_FSM_ID_ENUM_UINT32         enFsmId;

    /* �ϲ�״̬����ʶ */
    CNAS_XSD_FSM_ID_ENUM_UINT32         enParentFsmId;

    /* �ϲ�״̬�������Ϣ���� */
    VOS_UINT32                          ulParentEventType;

    /* ��ǰ״̬ */
    VOS_UINT32                          ulState;

    /* �����ϢMSGID������  */
    CNAS_XSD_MSG_STRU                   stEntryMsg;

    /* ״̬������ */
    CNAS_XSD_FSM_EXTRA_CTX_UNION        unFsmCtx;
}CNAS_XSD_FSM_CTX_STRU;


typedef struct
{
    VOS_UINT16                          usStackDepth;                           /* ��ǰѹջ��״̬���� */
    VOS_UINT16                          usStackPopFlg;                          /* ��ǰջpop��־ */
    CNAS_XSD_FSM_CTX_STRU               astFsmStack[CNAS_XSD_MAX_FSM_STACK_DEPTH];  /* ״̬��ջ����� */
}CNAS_XSD_FSM_STACK_STRU;


typedef struct
{
    VOS_UINT8                           ucIntMsgNum;                                /* ������Ϣ�ĸ��� */
    VOS_UINT8                           aucReserve[3];
    VOS_UINT8                          *pastIntMsg[CNAS_XSD_MAX_INT_MSG_QUEUE_NUM]; /* XSD����Ϣ�������飬�����Ϣָ�� */
}CNAS_XSD_INT_MSG_QUEUE_STRU;



typedef struct
{
    /**************************************************************************
                        CNAS XSD״̬��״̬����״̬ջ
    **************************************************************************/
    CNAS_XSD_FSM_CTX_STRU               stCurFsm;                               /* ״̬����ǰ��״̬������ */
    CNAS_XSD_FSM_STACK_STRU             stFsmStack;                             /* ״̬����״̬��ջ����   */

    /* CNAS XSD������Ϣ���� */
    CNAS_XSD_CACHE_MSG_QUEUE_STRU       stCacheMsgQueue;

    CNAS_XSD_INT_MSG_QUEUE_STRU         stIntMsgQueue;

    CNAS_XSD_SYSTEM_ACQUIRE_CTRL_STRU   stSysAcqCtrl;

    CNAS_XSD_TEST_CONFIG_STRU           stTestConfig;

    CNAS_XSD_SYS_ACQ_NVIM_CFG_STRU      stSysAcqNvimCfg;
}CNAS_XSD_CTX_STRU;

/*****************************************************************************
 Structure      : CNAS_XSD_PC_REPLAY_FIXED_CONTEXT_MSG_STRU
 Description    : PC�طŹ��̣�XSD��صĹ̶�ȫ�ֱ���
 Message origin :
*****************************************************************************/
typedef struct
{
    CNAS_XSD_MRU_LIST_STRU                                  stMruList;
    CNAS_XSD_OOS_SYS_ACQ_STRTEGY_CFG_STRU                   stStrategyCfg;                  /* ����OOS�����£�����������Ϣ */
    CNAS_XSD_TEST_CONFIG_STRU                               stTestConfig;
    CNAS_XSD_ADD_AVOID_LIST_CFG_STRU                        stAddAvoidListCfg;
    CNAS_XSD_NEG_PREF_SYS_CMP_CTRL_STRU                     stNegPrefSysCmpCtrl;
    CNAS_XSD_CALLBACK_CFG_STRU                              stCallBackCfg;                  /* ������CALLBACKģʽ���� */
    CNAS_XSD_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU              stCdma1XCustomPrefChan;         /* ���ų���Ƶ���Ƶ����Ϣ */
    CNAS_XSD_AVOID_SCHEDULE_INFO_STRU                       stAvoidScheduInfo;
    CNAS_XSD_MRU0_SWITCH_ON_OOC_STRATEGY_CFG_STRU           stMru0StrategyCfg;
    CNAS_XSD_1X_SYS_ACQ_NO_RF_PROTECT_TIMER_INFO_STRU       stSysAcqNoRfProtectTimerInfo;

    CNAS_CCB_OPER_LOCK_SYS_WHITE_LIST_STRU                  stOperLockSysWhiteList;
    CNAS_CCB_CTCC_CUSTOMIZE_FREQ_LIST_STRU                  stCustFreqList;
    CNAS_CCB_CDMA_STANDARD_CHANNELS_STRU                    stCdmaStandardChan;
    CNAS_CCB_CUSTOM_CFG_INFO_STRU                           stCustomCfg;                    /* NVIM�еĶ�����Ϣ */

    VOS_UINT8                                               ucIsConsiderRoamIndInPRL;       /* �ж�����ϵͳ���Ƿ���ROMA IND���Եı�־ */
    VOS_UINT8                                               uc1xSysAcqSyncDelayEnable;          /* �ӳ�ͬ��nv���ƿ���  */
    VOS_UINT8                                               aucRcv[2];
    VOS_UINT32                                              ul1xSysAcqDelayTimerLen;            /* ����ͬ������֮��Ļ��嶨ʱ��ʱ�� */
    VOS_UINT32                                              ulUeSupportedBand;
    VOS_UINT8                                               aucEmcRedialTimes[CNAS_XSD_MAX_MRU_SYS_NUM];  /* ��������ʧ��ʱ��ǰפ����Ƶ����mru���ز���������������������*/
}CNAS_XSD_PC_REPLAY_FIXED_CONTEXT_MSG_STRU;


typedef struct
{
    VOS_UINT32                                  ulIsPrlValid;
}CNAS_XSD_LOG_PRE_PARA_AHEAD_SWITCH_ON_STRU;


typedef struct
{
    VOS_UINT8                           ucCacheMsgNum;                                  /* ������Ϣ�ĸ��� */
    VOS_UINT8                           aucReserve[3];
    VOS_UINT8                          *pastCacheMsg[CNAS_XSD_REPLAY_MAX_CACHE_MSG_QUEUE_NUM]; /* XSD�Ļ�����Ϣָ������ */
}CNAS_XSD_REPLAY_CACHE_MSG_QUEUE_STRU;

typedef struct
{
    VOS_UINT8                                               ucPreCfgParaRcvd;              /* ������Ϣǰ��Ԥ�ò����Ƿ��Ѿ��յ� */
    VOS_UINT8                                               aucReserve[3];
    VOS_UINT16                                              usPrlAcqRecRcvdNum;         /* PRL ACQ REC �յ����� */
    VOS_UINT16                                              usPrlSysRecRcvdNum;         /* PRL SYS REC �յ����� */
    CNAS_XSD_REPLAY_CACHE_MSG_QUEUE_STRU                    stCacheMsg;                 /* replay�Ļ�����Ϣָ������ */
}CNAS_XSD_REPLAY_CTX_STRU;


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

VOS_VOID  CNAS_XSD_LoadSubFsm(
    CNAS_XSD_FSM_ID_ENUM_UINT32         enFsmId,
    CNAS_XSD_FSM_CTX_STRU              *pstCurFsm
);

VOS_VOID CNAS_XSD_InitFsmL2(
    CNAS_XSD_FSM_ID_ENUM_UINT32          enFsmId
);
VOS_VOID CNAS_XSD_PopFsm( VOS_VOID );

VOS_VOID CNAS_XSD_PushFsm(
    CNAS_XSD_FSM_STACK_STRU             *pstFsmStack,
    CNAS_XSD_FSM_CTX_STRU               *pstNewFsm
);

VOS_VOID CNAS_XSD_QuitFsmL2(VOS_VOID);

VOS_VOID  CNAS_XSD_SetCurrFsmState(
    VOS_UINT32                          ulCurrState
);

VOS_UINT32 CNAS_XSD_GetFsmTopState(VOS_VOID);

CNAS_XSD_CACHE_MSG_QUEUE_STRU* CNAS_XSD_GetCacheMsgAddr(VOS_VOID);

VOS_UINT32 CNAS_XSD_GetCacheMsgNum(VOS_VOID);

VOS_UINT32  CNAS_XSD_SaveCacheMsg(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pstMsg
);
VOS_UINT32 CNAS_XSD_GetNextCachedMsg(
    CNAS_XSD_MSG_STRU                  *pstEntryMsg
);

CNAS_XSD_FSM_CTX_STRU* CNAS_XSD_GetCurFsmAddr(VOS_VOID);

NAS_FSM_DESC_STRU* CNAS_XSD_GetCurFsmDesc(VOS_VOID);

CNAS_XSD_FSM_ID_ENUM_UINT32 CNAS_XSD_GetCurrFsmId(VOS_VOID);

CNAS_XSD_MSG_STRU* CNAS_XSD_GetCurrFsmEntryMsgAddr(VOS_VOID);

CNAS_XSD_FSM_STACK_STRU* CNAS_XSD_GetFsmStackAddr(VOS_VOID);

VOS_UINT16 CNAS_XSD_GetFsmStackDepth(VOS_VOID);

VOS_UINT16 CNAS_XSD_GetFsmStackPopFlg(VOS_VOID);

CNAS_XSD_READ_CARD_FILES_CNF_ENUM_UINT32 CNAS_XSD_GetWaitCardFilesCnfFlg_SwitchOn(VOS_VOID);

CNAS_XSD_CTX_STRU* CNAS_XSD_GetXsdCtxAddr(VOS_VOID);

VOS_VOID CNAS_XSD_InitFsmCtx_Redirection(VOS_VOID);

VOS_VOID CNAS_XSD_InitCtx(
    CNAS_XSD_INIT_CTX_TYPE_ENUM_UINT8   enInitType
);

VOS_VOID CNAS_XSD_InitAvailSysList(
    CNAS_XSD_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    CNAS_XSD_AVAILABLE_LIST_STRU        *pstAvailList
);

VOS_VOID CNAS_XSD_InitAvoidSysListSrchIndex(
    VOS_VOID
);

VOS_VOID CNAS_XSD_SaveCurEntryMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_VOID CNAS_XSD_SetFsmStackPopFlg(
    VOS_UINT16                          ucStackPopFlg
);

VOS_VOID CNAS_XSD_InitFsmCtx_L1Main(VOS_VOID);

VOS_VOID CNAS_XSD_InitFsmCtx_SwitchOn(VOS_VOID);

VOS_VOID CNAS_XSD_InitFsmCtx_PowerOff(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetCampOnNeedFlg_PowerOff(VOS_VOID);

VOS_VOID CNAS_XSD_SetCampOnNeedFlg_PowerOff(
    VOS_UINT8                           ucCampOnNeedFlg
);

VOS_UINT8 CNAS_XSD_GetBufCallRelIndFlg_PowerOff(VOS_VOID);
VOS_VOID CNAS_XSD_SetBufCallRelIndFlg_PowerOff(
    VOS_UINT8                           ucBufCallRelIndFlg
);

CNAS_PRL_FREQENCY_CHANNEL_STRU* CNAS_XSD_GetBufCallRelFreqInfo_PowerOff(VOS_VOID);
VOS_VOID CNAS_XSD_SaveBufCallRelFreqInfo_PowerOff(
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstFreq
);


VOS_VOID CNAS_XSD_InitFsmCtx_SysAcq(VOS_VOID);

VOS_VOID CNAS_XSD_SetWaitCardFilesCnfFlg_SwitchOn(
    CNAS_XSD_READ_CARD_FILES_CNF_ENUM_UINT32                enReadCardFileCnfFlg
);

VOS_VOID CNAS_XSD_ClearWaitCardFilesCnfFlg_SwitchOn(
    CNAS_XSD_READ_CARD_FILES_CNF_ENUM_UINT32                enReadCardFileCnfFlg
);

CNAS_XSD_SCAN_CHANNEL_LIST_STRU* CNAS_XSD_GetScanChanListAddr(VOS_VOID);

VOS_UINT16 CNAS_XSD_GetScanChannelNum(VOS_VOID);

VOS_UINT16 CNAS_XSD_GetCurChannelScanIndex(VOS_VOID);

VOS_VOID CNAS_XSD_SetCurChannelScanIndex(
    VOS_UINT16                          usCurScanIndex
);
CNAS_XSD_MRU_LIST_STRU* CNAS_XSD_GetMruList(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetMruListNum(VOS_VOID);
VOS_UINT32 CNAS_XSD_GetMru0SysId(
    CNAS_PRL_1X_SYSTEM_STRU         *pstMru0SysId
);

VOS_UINT32 CNAS_XSD_GetMru1SysId(
    CNAS_PRL_1X_SYSTEM_STRU         *pstMru1SysId
);

CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU* CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq(VOS_VOID);

VOS_UINT16 CNAS_XSD_GetCurSysAcqListNum_SysAcq(VOS_VOID);
VOS_VOID CNAS_XSD_SetAvailSysAcqListFlg_SysAcq(
    VOS_UINT8                           ucFlg
);

VOS_VOID CNAS_XSD_SetAvoidSysAcqListFlg_SysAcq(
    VOS_UINT8                           ucFlg
);
VOS_UINT8 CNAS_XSD_GetAvailSysAcqListFlg_SysAcq(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq(VOS_VOID);

VOS_VOID CNAS_XSD_SetAbortFlg_SysAcq(
    VOS_UINT8                           ucFlg
);

VOS_UINT8 CNAS_XSD_GetAbortFlg_SysAcq(VOS_VOID);

VOS_VOID CNAS_XSD_SetOocTotalTimerExpiredFlg_SysAcq(
    VOS_UINT8                           ucFlg
);

VOS_UINT8 CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq(VOS_VOID);



CNAS_XSD_AVAILABLE_LIST_STRU* CNAS_XSD_GetAvailSysFreqListAddr(VOS_VOID);

VOS_VOID CNAS_XSD_AddAvailSysFreqList(
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstFreq
);

VOS_VOID CNAS_XSD_SetAvailSysFeqListNextSrchIndex(
    VOS_UINT8                           ucNextSrchIndex
);

VOS_UINT8 CNAS_XSD_GetAvailSysFreqListNextSrchIndex(VOS_VOID);

VOS_VOID CNAS_XSD_SetAvoidSysFeqListNextSrchIndex(
    VOS_UINT8                           ucNextSrchIndex
);

VOS_UINT8 CNAS_XSD_GetAvailSysFreqNum(VOS_VOID);

VOS_VOID CNAS_XSD_UpdateMru0(
    CNAS_PRL_1X_SYSTEM_STRU            *pstCurSysId
);


VOS_UINT32 CNAS_XSD_GetUeSupportedBandClass(VOS_VOID);

VOS_VOID CNAS_XSD_SetUeSupportedBandClass(
    VOS_UINT32                          ulUeSupportedBand
);

VOS_VOID CNAS_XSD_UpdateCurCampedSysInfo(
    CNAX_XSD_CAMPED_SYS_INFO_STRU      *pstCurCampedSysInfo
);

CNAX_XSD_CAMPED_SYS_INFO_STRU* CNAS_XSD_GetCurCampedSysInfo(VOS_VOID);


VOS_VOID CNAS_XSD_SetAbortFlag_Redir(
    VOS_UINT8                                ucAbortFlag
);

VOS_UINT8 CNAS_XSD_GetAbortFlag_Redir(VOS_VOID);

CNAS_XSD_SCAN_CHANNEL_LIST_STRU *CNAS_XSD_GetScanChanListAddr_Redir(VOS_VOID);

VOS_UINT16 CNAS_XSD_GetScanChannelNum_Redir(VOS_VOID);


CNAS_XSD_SCAN_CHANNEL_LIST_STRU* CNAS_XSD_GetCallRedialScanChanListAddr(VOS_VOID);

CNAS_XSD_SCAN_CHANNEL_LIST_STRU *CNAS_XSD_GetEmcCallRedialScanChanListAddr(VOS_VOID);

CNAS_XSD_SERVICE_STATUS_ENUM_UINT32 CNAS_XSD_GetCurSrvStatus(VOS_VOID);

VOS_VOID CNAS_XSD_SetCurSrvStatus(
    CNAS_XSD_SERVICE_STATUS_ENUM_UINT32                     enSrvStatus
);

CNAS_XSD_INT_MSG_QUEUE_STRU* CNAS_XSD_GetIntMsgQueueAddr(VOS_VOID);

VOS_VOID CNAS_XSD_InitInternalBuffer(
    CNAS_XSD_INT_MSG_QUEUE_STRU        *pstIntMsgQueue
);

VOS_VOID CNAS_XSD_PutMsgInIntMsgQueue(
    VOS_UINT8                          *pstMsg
);

VOS_UINT8 *CNAS_XSD_GetNextIntMsg(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetIntMsgNum(VOS_VOID);


CNAS_XSD_AVOID_FREQ_LIST_STRU* CNAS_XSD_GetAvoidFreqListAddr(VOS_VOID);



VOS_UINT8 CNAS_XSD_GetAvoidFreqListNum(VOS_VOID);

VOS_VOID CNAS_XSD_DeleteAvoidFlagFromAvoidList(
    VOS_UINT8                           ucIndex
);

CNAS_XSD_AVOID_FREQ_INFO_STRU *CNAS_XSD_GetAvoidFreqFromAvoidList(
    VOS_UINT8                           ucIndex
);

VOS_VOID CNAS_XSD_ClearCacheMsg(
    VOS_UINT8                           ucIndex
);

VOS_VOID CNAS_XSD_SaveCacheMsgInMsgQueue(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pstMsg
);

VOS_UINT32 CNAS_XSD_ClearCacheMsgByMsgName(
    VOS_UINT32                          ulSndPid,
    VOS_UINT16                          usMsgName
);


VOS_VOID CNAS_XSD_SetCallExistFlg(
    VOS_UINT8                           ucCallExistFlg
);

VOS_UINT8 CNAS_XSD_GetCallExistFlg(VOS_VOID);

VOS_VOID CNAS_XSD_ClearCallExistFlg(
    VOS_UINT8                           ucCallExistFlg
);

VOS_VOID CNAS_XSD_SaveCallOrignalSys(
    CNAS_PRL_1X_SYSTEM_STRU            *pstCurSysId
);

CNAS_PRL_1X_SYSTEM_STRU *CNAS_XSD_GetCallOrignalSys(VOS_VOID);

VOS_VOID CNAS_XSD_SaveEmcCallOriginalSys(
    CNAS_PRL_1X_SYSTEM_STRU            *pstCurSysId
);

CNAS_PRL_1X_SYSTEM_STRU *CNAS_XSD_GetEmcCallOriginalSys(VOS_VOID);

VOS_VOID CNAS_XSD_ClearEmcCallOriginalSys( VOS_VOID );

VOS_UINT32 CNAS_XSD_IsEmcCallOriginalSysExist( VOS_VOID );

VOS_VOID CNAS_XSD_InitCacheMsgQueue(
    CNAS_XSD_INIT_CTX_TYPE_ENUM_UINT8   enInitType,
    CNAS_XSD_CACHE_MSG_QUEUE_STRU      *pstCacheMsgQueue
);

VOS_VOID CNAS_XSD_SetCallOrigSysExistFlg(
    VOS_UINT8                           ucFlg
);

VOS_UINT8 CNAS_XSD_GetCallOrigSysExistFlg(VOS_VOID);

VOS_VOID CNAS_XSD_SetSystemCampOnFlag(
    VOS_UINT8                           ucCampOnFlg
);

VOS_UINT8 CNAS_XSD_GetSystemCampOnFlag(VOS_VOID);

VOS_VOID CNAS_XSD_InitCallRedialCtrlInfo(
    CNAS_XSD_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    CNAS_XSD_CALL_REDIAL_CTRL_INFO_STRU *pstCallRedialInfo
);


VOS_VOID CNAS_XSD_InitEmcCallCtrlInfo(
    CNAS_XSD_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    CNAS_XSD_EMC_CALL_CTRL_INFO_STRU    *pstEmcCallInfo
);

CNAS_XSD_REDIRECTION_INFO_STRU *CNAS_XSD_GetRedirectionInfoAddr(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetRedirectionFlag(VOS_VOID);

VOS_VOID CNAS_XSD_SetRedirectionFlag(
    VOS_UINT8               ucRedirection
);

VOS_VOID CNAS_XSD_SetEmcRedirectionFlg(
    VOS_VOID
);

VOS_UINT8 CNAS_XSD_GetRedirectionReturnIfFailFlag(VOS_VOID);

VOS_VOID CNAS_XSD_SaveRedirectionOrignalSys(
    CNAS_PRL_1X_SYSTEM_STRU         *pstOriginalSystem
);

CNAS_PRL_1X_SYSTEM_STRU *CNAS_XSD_GetRedirectionOrignalSys(VOS_VOID);

CNAS_XSD_SCAN_CHANNEL_INFO_STRU *CNAS_XSD_GetRedirectionChanList(VOS_VOID);

VOS_VOID CNAS_XSD_InitRedirectionInfo(
    CNAS_XSD_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    CNAS_XSD_REDIRECTION_INFO_STRU      *pstRedirInfo
);

CNAS_XSD_CFREQ_LOCK_SET_PARA_STRU* CNAS_XSD_GetFreqLockAddr(VOS_VOID);

VOS_VOID CNAS_XSD_InitCFreqLockValue(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetFreqLockMode(VOS_VOID);


CNAS_XSD_OOC_SCHEDULE_INFO_STRU *CNAS_XSD_GetOocScheduleInfo(VOS_VOID);

VOS_VOID CNAS_XSD_InitOocScheduleInfo(
    CNAS_XSD_OOC_SCHEDULE_INFO_STRU    *pstOocScheduleInfo
);

VOS_VOID CNAS_XSD_InitOosSysAcqStrategyCfgInfo(
    CNAS_XSD_OOS_SYS_ACQ_STRTEGY_CFG_STRU                  *pstSysAcqStrategyCfg
);

VOS_VOID CNAS_XSD_InitOocCtxInfo(VOS_VOID);

VOS_VOID CNAS_XSD_SetNdssIndFlag(
    VOS_UINT8                           ucNdssInd
);

VOS_UINT8 CNAS_XSD_GetNdssIndFlag(VOS_VOID);


VOS_VOID CNAS_XSD_SetOocWaitSearchFlag(VOS_UINT8 ucWaitSearchFlag);

VOS_UINT8 CNAS_XSD_GetOocWaitSearchFlag(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetOocInsertOrigChanFlg(VOS_VOID);

CNAS_XSD_OOS_SYS_ACQ_STRTEGY_CFG_STRU *CNAS_XSD_GetOosSysAcqStrategyCfg(VOS_VOID);


CNAS_XSD_TEST_CONFIG_STRU* CNAS_XSD_GetTestConfig(VOS_VOID);

CNAS_XSD_SYS_ACQ_NVIM_CFG_STRU* CNAS_XSD_Get1xSysAcqNvimConfig(VOS_VOID);

CNAS_XSD_NEG_PREF_SYS_CMP_TYPE_ENUM_UINT8 CNAS_XSD_GetNegPrefSysCmpType(VOS_VOID);

VOS_VOID CNAS_XSD_InitTestConfig(
    CNAS_XSD_TEST_CONFIG_STRU          *pstTestConfig
);

VOS_VOID CNAS_XSD_InitSysAcqNvimCfg(
    CNAS_XSD_SYS_ACQ_NVIM_CFG_STRU                         *pstSysAcqNvimCfg
);

VOS_UINT8 CNAS_XSD_GetSrvAcqFlg(VOS_VOID);

VOS_VOID CNAS_XSD_SetSrvAcqFlg(
    VOS_UINT8                           ucSrvAcqFlg
);

VOS_UINT8 CNAS_XSD_GetIsConsider1xRoamIndInSysDeterminationFlg(VOS_VOID);

VOS_VOID CNAS_XSD_SetIsConsider1xRoamIndInSysDeterminationFlg(
    VOS_UINT8                           ucIsConsiderRoamIndInPRLFlg
);

VOS_VOID CNAS_XSD_ClearEmcCallRedialScanChanList( VOS_VOID );

CNAS_XSD_EMC_STATE_ENUM_UINT8 CNAS_XSD_GetEmcState(VOS_VOID);

VOS_VOID CNAS_XSD_SetEmcState(
    CNAS_XSD_EMC_STATE_ENUM_UINT8       enEmcState
);

VOS_VOID CNAS_XSD_SetEmcCallBackSrchCounter(
    VOS_UINT8                           ucEmcCallBackCounter
);

CNAS_XSD_EMC_CALLBACK_CANDIDATE_FREQ_INFO_STRU* CNAS_XSD_GetEmcCallBackCandidateFreqInfo(VOS_VOID);

VOS_VOID CNAS_XSD_ClearEmcCallBackCandidateFreq(
    VOS_VOID
);

VOS_VOID CNAS_XSD_SetEmcCallBackCandidateFreq(
    VOS_UINT16                          usCfFreqNum,
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstCfInfo
);

VOS_UINT8 CNAS_XSD_GetEmcCallBackSrchCounter(VOS_VOID);


CNAS_XSD_CALLBACK_CFG_STRU *CNAS_XSD_GetCallBackCfg( VOS_VOID );

VOS_VOID CNAS_XSD_InitGeoSrchListInfo(
    CNAS_XSD_GEO_LIST_SRCH_INFO_STRU   *pstGeoListSrchInfo
);

CNAS_XSD_GEO_LIST_SRCH_INFO_STRU *CNAS_XSD_GetGeoSrchListInfoAddr(VOS_VOID);

VOS_VOID CNAS_XSD_ClearGeoSrchListStatus(VOS_VOID);

VOS_VOID CNAS_XSD_FreeBufferOfGeoSrchList(VOS_VOID);

VOS_VOID CNAS_XSD_InitHistorySyncedSysFreqList(
    CNAS_XSD_HISTORY_SYNCED_SYS_FREQ_LIST_STRU             *pstHisSyncedSysFreqList
);

CNAS_XSD_HISTORY_SYNCED_SYS_FREQ_LIST_STRU *CNAS_XSD_GetHistorySyncedSysFreqList(VOS_VOID);

VOS_VOID CNAS_XSD_UpdateHistorySyncedSysFreqList(
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstFreq
);

CNAS_PRL_1X_SYSTEM_ID_STRU* CNAS_XSD_GetLastSyncedSys(VOS_VOID);

CNAS_PRL_FREQENCY_CHANNEL_STRU* CNAS_XSD_GetLastSyncedFreq(VOS_VOID);

VOS_VOID CNAS_XSD_UpdateLastSyncedSys(
    CNAS_PRL_1X_SYSTEM_ID_STRU         *pstCurSyncedSys
);

CNAS_XSD_CALL_REL_ANY_CAMP_ON_FREQ_STRU* CNAS_XSD_GetCallRelAnyCampOnFreqListAddr(VOS_VOID);

VOS_UINT16 CNAS_XSD_GetCurChannelScanIndex_Redir(VOS_VOID);
VOS_VOID CNAS_XSD_SetCurChannelScanIndex_Redir(VOS_UINT16 usIndex);

CNAS_XSD_CDMA_1X_CUSTOM_PREF_CHANNELS_STRU* CNAS_XSD_GetCdma1XCustomPrefChannels(VOS_VOID);

VOS_UINT32 CNAS_XSD_IsCurFreqInCdma1XCustomPrefChannels(
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstFreq
);

VOS_UINT32 CNAS_XSD_AddCurFreqToCdma1XCustomPrefChannels(
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstFreq
);

VOS_UINT8 CNAS_XSD_GetNoCardModeCfgFlg(VOS_VOID);

VOS_VOID CNAS_XSD_SetNoCardModeCfgFlg(
    VOS_UINT8                           ucNoCardModeCfgFlg
);

CNAS_XSD_EMC_REDIAL_SYS_ACQ_CFG_STRU* CNAS_XSD_GetEmcRedialSysAcqCfgInfo(VOS_VOID);

CNAS_XSD_AVOID_SCHEDULE_INFO_STRU* CNAS_XSD_GetAvoidScheduInfoAddr(VOS_VOID);

VOS_UINT8 CNAS_XSD_GetAvoidFreqIndexOfAvoidlist(
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstAvoidFreq,
    VOS_UINT8                          *pucIndex
);

VOS_UINT8 CNAS_XSD_GetSwitchOnOocMru0EnableFlg(VOS_VOID);

VOS_VOID CNAS_XSD_SetSwitchOnOocMru0EnableFlg(VOS_UINT8 ucEnable);

VOS_UINT16 CNAS_XSD_GetSwitchOnOocMru0Times(VOS_VOID);

VOS_VOID CNAS_XSD_SetSwitchOnOocMru0Times(VOS_UINT16 usTimes);

VOS_VOID CNAS_XSD_SetOosSpecialSdReason(
    NAS_MSCC_PIF_1X_OOS_CAUSE_ENUM_UINT32                   ulOosSpecialSdReason
);

NAS_MSCC_PIF_1X_OOS_CAUSE_ENUM_UINT32 CNAS_XSD_GetOosSpecialSdReason(VOS_VOID);

VOS_VOID CNAS_XSD_SetSystemAcquiredAbortedCause(
    NAS_ERR_LOG_1X_SEARCH_RSLT_ENUM_UINT32      enAbortedCause
);

NAS_ERR_LOG_1X_SEARCH_RSLT_ENUM_UINT32 CNAS_XSD_GetSystemAcquiredAbortedCause(VOS_VOID);


VOS_VOID CNAS_XSD_SetHrpdConnExistFlag(
    VOS_UINT8          ucHrpdConnExistFlag
);
VOS_UINT8 CNAS_XSD_GetHrpdConnExistFlag( VOS_VOID );
VOS_VOID CNAS_XSD_SetLteConnExistFlag(
    VOS_UINT8          ucLteConnExistFlag
);
VOS_UINT8 CNAS_XSD_GetLteConnExistFlag( VOS_VOID );

VOS_UINT32 CNAS_XSD_GetSysAcqNoRfProtectTimerLen(VOS_VOID);
VOS_VOID CNAS_XSD_SetSysAcqNoRfProtectTimerLen(
    VOS_UINT32                          ul1xSysAcqNoRfProtectTimerLen
);
VOS_VOID CNAS_XSD_SetRedirNoRfProtectTimerLen(
    VOS_UINT32                          ul1xRedirNoRfProtectTimerLen
);
VOS_UINT32 CNAS_XSD_GetRedirNoRfProtectTimerLen(VOS_VOID);

VOS_UINT32 CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen(VOS_VOID);
VOS_VOID CNAS_XSD_SetLteOrDoConn1xSysAcqSyncDelayTimerLen(
    VOS_UINT32                          ul1xSysAcqDelayTimerLen
);
VOS_VOID CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(
    VOS_UINT16                          usFreqNum,
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstNextSysAcqFreqInfo
);
CNAS_XSD_1X_SYS_ACQ_SYNC_DELAY_INFO_WHEN_LTE_OR_DO_CONN_STRU* CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayInfoAddr(VOS_VOID);
VOS_VOID CNAS_XSD_SetLteOrDoConn1xSysAcqSyncDelayFlag(
    VOS_UINT8                           uc1xSysAcqDelayFlag
);
VOS_UINT8 CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayFlag(VOS_VOID);


#ifdef __PS_WIN32_RECUR__
CNAS_XSD_REPLAY_CTX_STRU* CNAS_XSD_ReplayGetCtxAddr(VOS_VOID);

CNAS_XSD_REPLAY_CACHE_MSG_QUEUE_STRU* CNAS_XSD_ReplayGetCacheMsgQueueAddr(VOS_VOID);

VOS_VOID CNAS_XSD_ReplayInitCacheMsgQueue(VOS_VOID);

VOS_VOID CNAS_XSD_ReplayInitCtx(VOS_VOID);

VOS_VOID CNAS_XSD_RepalySaveCacheMsgInMsgQueue(
    VOS_VOID                           *pstMsg
);

VOS_VOID CNAS_XSD_ReplayClearCacheMsg(
    VOS_UINT8                           ucIndex
);

VOS_UINT32 CNAS_XSD_ReplayGetNextCachedMsg(
    CNAS_XSD_MSG_STRU                  *pstEntryMsg
);

#endif

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

#endif /* end of CnasXsdCtx.h */
