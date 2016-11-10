

#ifndef __MLOWPOWER_H__
#define __MLOWPOWER_H__

#include "vos.h"
#include "mdrv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* MLPģ��modem�ĸ��� */
#define MLP_MODEM_NUM               (MODEM_ID_BUTT)

/* ͶƱʱÿ8��bit��ʾһ��ģ����2��bit���� */
#define MLP_MODE_VOTE_NUM           (8)

/* ��¼������ε�ʱ�� */
#define MLP_BBE16_RECORD_TRACK_NUM  (3)

/* MLPģ��ģʽ�ĸ���*/
#define MLP_MODE_TYPE_BUTT          (VOS_RATMODE_BUTT)

/* ��¼ÿ��modem�ϵ�ÿ��mode���������ͶƱ���� */
#define MLP_BBE16_NOR_STATE_TRACK(enModem, enMode, enType, ulVoteMap) \
    if ((VOS_NULL_PTR != g_pstMlpBBE16NormalState) && (enModem < MLP_MODEM_NUM) && (enMode < MLP_MODE_TYPE_BUTT)) \
    { \
        g_pstMlpBBE16NormalState->astRecordItem[enModem][enMode][enType].aulSlice[g_pstMlpBBE16NormalState->astRecordItem[enModem][enMode][enType].ulCount % MLP_BBE16_RECORD_TRACK_NUM] = VOS_GetSlice(); \
        g_pstMlpBBE16NormalState->astRecordItem[enModem][enMode][enType].ulCount++; \
        g_pstMlpBBE16NormalState->astRecordItem[enModem][enMode][enType].ulVoteMap = ulVoteMap; \
    } \

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum MLP_BBE16_NORMAL_STATE_TRACK_ENUM
{
    /* Load */
    MLP_BBE16_LOAD,
    MLP_BBE16_LOAD_ACT,

    MLP_BBE16_RUN,
    MLP_BBE16_RUN_ACT,
    MLP_BBE16_STOP,
    MLP_BBE16_STOP_ACT,

    MLP_BBE16_UNRESET,
    MLP_BBE16_UNRESET_ACT,
    MLP_BBE16_RESET,
    MLP_BBE16_RESET_ACT,

    MLP_BBE16_CLK_EN,
    MLP_BBE16_CLK_EN_ACT,
    MLP_BBE16_CLK_DIS,
    MLP_BBE16_CLK_DIS_ACT,

    MLP_BBE16_RESTORE,
    MLP_BBE16_RESTORE_ACT,
    MLP_BBE16_BACKUP,
    MLP_BBE16_BACKUP_ACT,

    MLP_BBE16_WAIT_RESTORE,
    MLP_BBE16_WAIT_RESTORE_ACT,
    MLP_BBE16_WAIT_BACKUP,
    MLP_BBE16_WAIT_BACKUP_ACT,

    MLP_BBE16_PLLCLK_EN,
    MLP_BBE16_PLLCLK_EN_ACT,
    MLP_BBE16_PLLCLK_DIS,
    MLP_BBE16_PLLCLK_DIS_ACT,

    MLP_BBE16_PWR_UP,
    MLP_BBE16_PWR_UP_ACT,
    MLP_BBE16_PWR_DOWN,
    MLP_BBE16_PWR_DOWN_ACT,

    MLP_BBE16_NORMAL_BUTT
};

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

/*****************************************************************************
 �ṹ��    : SLEEP_MODE_INFO_STRU
 �ṹ˵��  : GUģ��ǰϵͳ״̬
*****************************************************************************/
typedef struct
{
    VOS_INT                             (*bbe16_enable_operator)(BSP_DSP_TYPE_E edsptype);
    VOS_INT                             (*bbe16_disable_operator)(BSP_DSP_TYPE_E edsptype);
    VOS_INT                             (*bbe16_pwrup_operator)(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId,PWC_COMM_CHANNEL_E enChannel);
    VOS_INT                             (*bbe16_pwrdown_operator)(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId,PWC_COMM_CHANNEL_E enChannel);
    VOS_UINT32                          ulVoteMap;
    VOS_UINT32                          ulState;
} MLP_BBE16_OPERATOR_INFO_STRU;

typedef struct
{
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16Load;                        /* BBE16������� */
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16RunStop;                     /* BBE16����/ֹͣ */
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16SysCtrl;                     /* BBE16�⸴λ/��λ */
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16SysClock;                    /* BBE16 sysʱ�Ӵ�/�ر� */
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16RestoreBackup;               /* BBE16����ָ�/���� */
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16WaitRestoreBackup;           /* BBE16�ȴ�����ָ�/���� */
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16PllClock;                    /* BBE16 pllʱ�Ӵ�/�ر� */
    MLP_BBE16_OPERATOR_INFO_STRU        stBBE16PowerCtrl;                   /* BBE16�ϵ�/�µ� */
} MLP_BBE16_INFO_STRU;

/*****************************************************************************
 �ṹ��    : MLP_BBE16_RECORD_TRACK_STRU
 �ṹ˵��  : ��¼MLP��BBE16�����Ĵ���ʱ��켣
*****************************************************************************/
typedef struct
{
    VOS_CHAR                           *pucTrackId;                              /* ״̬�� */
    VOS_UINT32                          ulVoteMap;                               /* Vote Value */
    VOS_UINT32                          ulCount;                                 /* ��¼�����Ĵ��� */
    VOS_UINT32                          aulSlice[MLP_BBE16_RECORD_TRACK_NUM];    /* ��¼������Sliceֵ */
} MLP_BBE16_RECORD_TRACK_STRU;

/*****************************************************************************
 �ṹ��    : MLP_BBE16_NORMAL_STATE_RECORD_STRU
 �ṹ˵��  : ����MLP����״̬�л�����
*****************************************************************************/
typedef struct
{
    MLP_BBE16_RECORD_TRACK_STRU   astRecordItem[MODEM_ID_BUTT][MLP_MODE_TYPE_BUTT][MLP_BBE16_NORMAL_BUTT];
} MLP_BBE16_NORMAL_STATE_RECORD_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID mlp_dsp_init(VOS_VOID);
VOS_VOID mlp_dsp_mntn_init(VOS_VOID);
VOS_UINT32 mlp_dsp_load(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_run(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_stop(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_unreset(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_reset(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_enable_clock(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_disable_clock(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_restore(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_backup(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_wait_restore(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_wait_backup(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_enable_pllclk(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_disable_pllclk(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_pwrup(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);
VOS_UINT32 mlp_dsp_pwrdown(MODEM_ID_ENUM_UINT16 enModem, VOS_RATMODE_ENUM_UINT32 enMode);


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

#endif /* end of mlowpoer.h */
