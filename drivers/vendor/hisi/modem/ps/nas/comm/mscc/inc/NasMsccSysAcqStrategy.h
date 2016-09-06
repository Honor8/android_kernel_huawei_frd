
#ifndef __NAS_MSCC_SYS_ACQ_STRATEGY_H__
#define __NAS_MSCC_SYS_ACQ_STRATEGY_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "NasCommDef.h"
#include "NasMsccCtx.h"
#include "NasMsccMlplMsplApi.h"

#include "xsd_mscc_pif.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 �궨��
*****************************************************************************/


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM
{
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_SWITCH_ON                 = 0,/* ���� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_SWITCH_ON_INIT_ACQ_SUCC,      /* ������ʼ����ɹ� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_SWITCH_ON_INIT_ACQ_FAIL,      /* ������ʼ������ʧ�� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_AVAILABLE_TIMER_EXPIRED,      /* available��ʱ����ʱ*/
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_HRPD_LOST,                    /* hrpd ���� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_LTE_LOST,                     /* lte  ���� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_SLEEP_TIMER_EXPIRED,          /* sleep ��ʱ����ʱ */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_SYS_CFG_SET,            /* SYSCFG�������� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_LTE_RF_AVAILABLE,             /* lte rf��Դ���� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_HRPD_RF_AVAILABLE,            /* hrpd rf��Դ���� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_MO_TRIGGER,                       /*  ���� */

    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_LTE_NOT_AVAILABLE,            /* Lte������ */

    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_QUICK_SEARCH_HRPD_MRUO,           /* ����ҵ�񴥷���HRPD��MRU0���� */

    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_LTE_HISTORY_PLUS_LTE_LOST,    /* ��LTE��ʷ�Ѽ�LTE�������� */
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_AVAILABLE_TIMER_EXPIRED_SRLTE_1X_TCH,  /* ����available��ʱ����ʱ��1X����TCH̬ */

    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_BUTT
};
typedef VOS_UINT32 NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_UINT32;

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


typedef VOS_VOID  (*pNasMsccSysAcqListBuildFunc)(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);




typedef struct
{
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_UINT32           enSysAcqScene;           /* system acquire scene */
    pNasMsccSysAcqListBuildFunc                             pfunSysAcqListBuild;     /* build system aqurie list function pointer */
}NAS_MSCC_SYS_ACQ_LIST_BUILD_TBL_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID NAS_MSCC_BuildSysAcqList(
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_UINT32           enSysAcqScene,
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_SwitchOn(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_InitAcqSuccInSwitchOn(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_InitAcqFailInSwitchOn(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_AvailableTimerExpired(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);


VOS_VOID NAS_MSCC_BuildSysAcqList_HrpdLost(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_LteLost(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_LteNotAvailable(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_SleepTimerExpired(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_MoTrigger(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);



VOS_VOID NAS_MSCC_BuildSysAcqList_SysCfgSet(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_LteRfAvailable(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_HrpdRfAvailable(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_LteHistoryPlusLteLost(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

VOS_VOID NAS_MSCC_BuildSysAcqList_AvailableTimerExpiredSrlte1xTch(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

NAS_MSCC_PRIOR_SYS_ENUM_UINT8  NAS_MSCC_GetPrefRatWith1xLocInfo(
    NAS_MSCC_1X_LOC_INFO_STRU          *pst1xLocationInfo
);

NAS_MSCC_PRIOR_SYS_ENUM_UINT8  NAS_MSCC_GetPrefRatWithLteLocInfo(
    NAS_MSCC_LTE_LOC_INFO_STRU         *pstLteLocInfo
);

VOS_UINT8  NAS_MSCC_IsLteMorePreferredThanHrpdInMspl(
    NAS_MSCC_MSPL_SYS_LIST_INFO_STRU   *pstMsplSysList
);

NAS_MSCC_PRIOR_SYS_ENUM_UINT8  NAS_MSCC_GetPrefRatWithLastLocInfo(VOS_VOID);



VOS_UINT8  NAS_MSCC_IsLteSysTypeExistInSysList(
    NAS_MSCC_MSPL_SYS_LIST_INFO_STRU   *pstSysList
);

VOS_UINT8  NAS_MSCC_IsHrpdSysTypeExistInSysList(
    NAS_MSCC_MSPL_SYS_LIST_INFO_STRU   *pstSysList
);

VOS_UINT32 NAS_MSCC_GetNextNormalAcquireSysInAcqSysList(
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstAcqSysListInfo,
    NAS_MSCC_SYS_ACQ_SYS_INFO_STRU                         *pstNextAcqSysInfo
);

VOS_UINT32 NAS_MSCC_GetNextInitialAcquireSysInAcqSysList(
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstAcqSysListInfo,
    NAS_MSCC_SYS_ACQ_SYS_INFO_STRU                         *pstNextAcqSysInfo
);

VOS_VOID NAS_MSCC_UpdateAcqRatStatusInNormalAcqSysList(
    VOS_RATMODE_ENUM_UINT32                                 ulRatMode,
    NAS_MSCC_SYS_ACQ_STATUS_ENUM_UINT8                      enSysAcqStatus,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstAcqSysListInfo
);

VOS_VOID NAS_MSCC_ResetAcqRatStatusInNormalAcqSysList(
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstAcqSysListInfo
);

VOS_VOID  NAS_MSCC_ConvertSysAcqSceneToBuildSysAcqListScene(
    NAS_MSCC_SYS_ACQ_SCENE_ENUM_UINT32                      enSysAcqScene,
    NAS_MSCC_BUILD_SYS_ACQ_LIST_SCENE_ENUM_UINT32          *penBuildSysAcqListScene
);

VOS_VOID NAS_MSCC_GetDestBsrSysPriClassAndSysTypeInSysList(
    VOS_RATMODE_ENUM_UINT32                                 enCurSysRat,
    NAS_MSCC_MSPL_SYS_LIST_INFO_STRU                       *pstOutSysList,
    NAS_MSCC_MSPL_SYS_PRI_CLASS_ENUM_UINT8                 *penSysPriClass,
    NAS_MSCC_MLPL_MSPL_SYS_TYPE_ENUM_UINT8                 *penSysType
);

VOS_UINT8 NAS_MSCC_GetAllowedSrchLtePriClassWith1xLocInfo(
    XSD_MSCC_1X_SYSTEM_SERVICE_INFO_IND_STRU               *pst1xSysSrvInfoInd,
    NAS_MSCC_PIF_PRI_CLASS_ENUM_UINT8                      *penLtePriClass
);

VOS_UINT8 NAS_MSCC_GetLtePriClassInMsplListWith1xIndex(
    NAS_MSCC_MSPL_SYS_LIST_INFO_STRU                       *pstMsplList,
    NAS_MSCC_PIF_PLMN_PRIORITY_CLASS_ENUM_UINT8            *penLtePriClass,
    VOS_UINT8                                               uc1xIndex
);

VOS_UINT8 NAS_MSCC_GetLtePriClassInMsplListBehind1xIndex(
    NAS_MSCC_MSPL_SYS_LIST_INFO_STRU                       *pstMsplList,
    NAS_MSCC_PIF_PLMN_PRIORITY_CLASS_ENUM_UINT8            *penLtePriClass,
    VOS_UINT8                                               uc1xIndex
);

NAS_MSCC_PRIOR_SYS_ENUM_UINT8 NAS_MSCC_GetPrefRatWithNoCardInitSearchLocInfo(VOS_VOID);

VOS_VOID NAS_MSCC_BuildSysAcqList_QuickSearchHrpdMru0(
    NAS_MSCC_1X_LOC_INFO_STRU                              *pst1xLocInfo,
    NAS_MSCC_LTE_LOC_INFO_STRU                             *pstLteLocInfo,
    NAS_MSCC_SYS_ACQ_SYS_LIST_INFO_STRU                    *pstSysAcqListInfo
);

#endif

#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif
#endif

