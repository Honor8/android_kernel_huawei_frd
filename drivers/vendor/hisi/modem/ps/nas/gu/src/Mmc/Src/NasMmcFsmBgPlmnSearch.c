

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "NasMmcFsmBgPlmnSearchTbl.h"
#include "NasMmcCtx.h"
#include "NasMmcFsmBgPlmnSearch.h"
#include "NasMmcSndGuAs.h"
#include "NasMmcSndLmm.h"
#include "NasMmcSndGmm.h"
#include "NasMmcSndMm.h"
#include "NasMmcProcRegRslt.h"
#include "NasMmcPlmnSelectionStrategy.h"
#include "NasMmcTimerMgmt.h"
#include "NasComm.h"
#include "NasMmlLib.h"
#include "NasMmlMsgProc.h"
#include "NasMmcSndMscc.h"
#include "NasMmcProcUsim.h"
#include "NasMmcProcSuspend.h"
#include "NasMmcSndOm.h"
#include "NasMmcProcNvim.h"
/* ɾ��ExtAppMmcInterface.h*/
#include "MmcMmInterface.h"
#include "MmcGmmInterface.h"
#include "NasMmcComFunc.h"
#include "NasUtranCtrlInterface.h"

#include "MsccMmcInterface.h"
#include "NasUtranCtrlInterface.h"
#include "NasMmcSndCss.h"

#include "NasUsimmApi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMC_FSM_BG_PLMN_SEARCH_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*lint -save -e958 */

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_UINT32    NAS_MMC_RcvTiHPlmnTimerExpired_BgPlmnSearch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstCurrCampPlmnId        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ��ǰפ�������PLMNID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ȡMML�б���ĵ�ǰEQU PLMN��Ϣ */
    pstEquPlmnInfo = NAS_MML_GetEquPlmnList();

    /* ����״̬�������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ��NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH��������ȼ��б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSelectionListInfo);

    NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);

    /* ���浱ǰפ�����絽״̬�������� */
    NAS_MMC_SetPreCampLai_BgPlmnSearch(NAS_MML_GetCurrCampLai());

    /* ���浱ǰפ�������EPLMN��״̬�������� */
    NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(pstCurrCampPlmnId, pstEquPlmnInfo);

    /* ���浱ǰ�ķ���״̬ */
    NAS_MMC_SetPreServiceInfo_BgPlmnSearch(NAS_MMC_GetServiceInfo());

#if (FEATURE_ON == FEATURE_LTE)
    /* ���浱ǰL disable״̬��״̬�������� */
    NAS_MMC_SetPreLteDisableFlg_BgPlmnSearch(NAS_MML_GetLteCapabilityStatus());
    /* �����Ƿ���Ҫenable LTE��־,�ڿ���ָ�����е��� */
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLTE_FastSpecSearch())
    {
        NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(VOS_TRUE);
    }
#endif

    /* �����ȡ�����ȼ�����ʧ���˳�״̬�� */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �Ƶ�������������֮ǰ���� */

    /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
    NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                            stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiHighPrioRatHPlmnTimerExpired_BgPlmnSearch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstCurrCampPlmnId        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
#endif

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ��ǰפ�������PLMNID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ȡMML�б���ĵ�ǰEQU PLMN��Ϣ */
    pstEquPlmnInfo = NAS_MML_GetEquPlmnList();

    /* ����״̬�������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ��NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH��������ȼ��б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSelectionListInfo);

    NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);

    /* ���浱ǰפ�����絽״̬�������� */
    NAS_MMC_SetPreCampLai_BgPlmnSearch(NAS_MML_GetCurrCampLai());

    /* ���浱ǰפ�������EPLMN��״̬�������� */
    NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(pstCurrCampPlmnId, pstEquPlmnInfo);

    /* ���浱ǰ�ķ���״̬ */
    NAS_MMC_SetPreServiceInfo_BgPlmnSearch(NAS_MMC_GetServiceInfo());

#if (FEATURE_ON == FEATURE_LTE)
    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

    /* ���浱ǰL disable״̬��״̬�������� */
    if (NAS_MML_LTE_CAPABILITY_STATUS_BUTT != enLteCapabilityStatus)
    {
        NAS_MMC_SetPreLteDisableFlg_BgPlmnSearch(enLteCapabilityStatus);
    }
    /* �����Ƿ���Ҫenable LTE��־,�ڿ���ָ�����е��� */
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLTE_FastSpecSearch())
    {
        NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(VOS_TRUE);
    }
#endif

    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    /* �����ȡ�����ȼ�����ʧ���˳�״̬�� */
    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �Ƶ�������������֮ǰ���� */

    /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
    NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                            stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);

    return VOS_TRUE;
}




VOS_UINT32    NAS_MMC_RcvTiPeriodHighPrioPlmnSearchExpired_BgPlmnSearch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstCurrCampPlmnId        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ��ǰפ�������PLMNID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ȡMML�б���ĵ�ǰEQU PLMN��Ϣ */
    pstEquPlmnInfo = NAS_MML_GetEquPlmnList();

    /* ����״̬�������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ��NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH��������ȼ��б� */
    if (NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST != NAS_MMC_GetPlmnSelectionListType())
    {
        NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_RcvTiPeriodHighPrioPlmnSearchExpired_BgPlmnSearch_Init: Plmn Selection List Type is: ", NAS_MMC_GetPlmnSelectionListType());

        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstPlmnSelectionListInfo);

        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    }

    /* ���浱ǰפ�����絽״̬�������� */
    NAS_MMC_SetPreCampLai_BgPlmnSearch(NAS_MML_GetCurrCampLai());

    /* ���浱ǰפ�������EPLMN��״̬�������� */
    NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(pstCurrCampPlmnId, pstEquPlmnInfo);

    /* ���浱ǰ�ķ���״̬ */
    NAS_MMC_SetPreServiceInfo_BgPlmnSearch(NAS_MMC_GetServiceInfo());

    /* ���浱ǰL disable״̬��״̬�������� */
#if (FEATURE_ON == FEATURE_LTE)
    /* ���浱ǰL disable״̬��״̬�������� */
    NAS_MMC_SetPreLteDisableFlg_BgPlmnSearch(NAS_MML_GetLteCapabilityStatus());
    /* �����Ƿ���Ҫenable LTE��־,�ڿ���ָ�����е��� */
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLTE_FastSpecSearch())
    {
        NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(VOS_TRUE);
    }
#endif

    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    /* �����ȡ�����ȼ�����ʧ���˳�״̬�� */
    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �Ƶ�������������֮ǰ���� */

    /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
    NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                            stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_CSG)

VOS_UINT32    NAS_MMC_RcvTiAutonomousCsgIdSearchTimerExpired_BgPlmnSearch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo            = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstCurrCampPlmnId         = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              aulCsgId[NAS_MML_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId, 0xF, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));

    PS_MEM_SET(&stCsgIdInfo, 0, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    /* ��ȡCSG�������б� */
    pstCsgIdSelectionListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ��ȡ��ǰפ�������PLMNID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ȡMML�б���ĵ�ǰEQU PLMN��Ϣ */
    pstEquPlmnInfo = NAS_MML_GetEquPlmnList();

    /* ����״̬�������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ���õ�ǰ��CSG���������� */
    NAS_MMC_SetCsgSearchingFlag_BgPlmnSearch(VOS_TRUE);

    /* ���浱ǰפ�����絽״̬�������� */
    NAS_MMC_SetPreCampLai_BgPlmnSearch(NAS_MML_GetCurrCampLai());

    /* ���浱ǰפ�������EPLMN��״̬�������� */
    NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(pstCurrCampPlmnId, pstEquPlmnInfo);

#if (FEATURE_ON == FEATURE_LTE)

    /* ���浱ǰL disable״̬��״̬�������� */
    NAS_MMC_SetPreLteDisableFlg_BgPlmnSearch(NAS_MML_GetLteCapabilityStatus());

    /* CSG������Ҳ��������enable lte */
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLTE_FastSpecSearch())
    {
        NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(VOS_TRUE);
    }
#endif

    /* �����ȡCSG IDʧ���˳�״̬�� */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchPlmnAndCsgId(pstCsgIdSelectionListInfo,
                                                       &stCsgIdInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������Ҫ������CSG ID��״̬�������� */
    NAS_MMC_SetCurSearchCsgId_BgPlmnSearch(&stCsgIdInfo.stPlmnId,
                                        stCsgIdInfo.ulCsgId,
                                        stCsgIdInfo.astPlmnRatInfo[0].enRatType);

    /* ��ǰֻ֧��ָ��������1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
    ulCsgIdNum  = 1;
    aulCsgId[0] = stCsgIdInfo.ulCsgId;

    /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
    NAS_MMC_SndBgCsgIdSearchReq_BgPlmnSearch(&stCsgIdInfo.stPlmnId,
                                        ulCsgIdNum, aulCsgId,
                                        stCsgIdInfo.astPlmnRatInfo[0].enRatType);

    return VOS_TRUE;

}


VOS_UINT32    NAS_MMC_RcvTiPeriodCsgIdSearchTimerExpired_BgPlmnSearch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ASF������PSF����������ͬ */
    (VOS_VOID)NAS_MMC_RcvTiAutonomousCsgIdSearchTimerExpired_BgPlmnSearch_Init(ulEventType, pstMsg);

    return VOS_TRUE;
}

VOS_UINT32    NAS_MMC_RcvTiPeriodTryingCsgIdSearchExpired_BgPlmnSearch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ASF������PSF����������ͬ */
    (VOS_VOID)NAS_MMC_RcvTiAutonomousCsgIdSearchTimerExpired_BgPlmnSearch_Init(ulEventType, pstMsg);

    return VOS_TRUE;
}

#endif


VOS_UINT32 NAS_MMC_RcvMmcInterBgSearchReq_BgPlmnSearch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstCurrCampPlmnId        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ��ǰפ�������PLMNID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ȡMML�б���ĵ�ǰEQU PLMN��Ϣ */
    pstEquPlmnInfo = NAS_MML_GetEquPlmnList();

    /* ����״̬�������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ��NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH��������ȼ��б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSelectionListInfo);

    NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_BG_HISTORY_PLMN_SELECTION_LIST);

    /* ���浱ǰפ�����絽״̬�������� */
    NAS_MMC_SetPreCampLai_BgPlmnSearch(NAS_MML_GetCurrCampLai());

    /* ���浱ǰפ�������EPLMN��״̬�������� */
    NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(pstCurrCampPlmnId, pstEquPlmnInfo);

    /* ���浱ǰ�ķ���״̬ */
    NAS_MMC_SetPreServiceInfo_BgPlmnSearch(NAS_MMC_GetServiceInfo());

#if (FEATURE_ON == FEATURE_LTE)
    /* ���浱ǰL disable״̬��״̬�������� */
    NAS_MMC_SetPreLteDisableFlg_BgPlmnSearch(NAS_MML_GetLteCapabilityStatus());

    /* �����Ƿ���Ҫenable LTE��־,�ڿ���ָ�����е��� */
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLTE_FastSpecSearch())
    {
        NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(VOS_TRUE);
    }
#endif

    /* �����ȡ�����ȼ�����ʧ���˳�״̬�� */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
    NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                            stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_WAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(WUEPS_PID_WRR);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSysInfoInd_BgPlmnSearch_WaitWasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;
#if (FEATURE_ON == FEATURE_LTE)

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ� */
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ж�ԭ��ֵ */

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasBgPlmnSearchCnf: ENTERED");

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);

    /* ����״̬�����Ϊʧ��,����Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvRrMmSearchedPlmnInfoInd_BgPlmnSearch_WaitWasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    RRMM_SEARCHED_PLMN_INFO_IND_STRU                       *pstSearchedPlmnInfoMsg   = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stSearchedPlmn;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus;
    VOS_UINT32                                              i;


    pstSearchedPlmnInfoMsg  = (RRMM_SEARCHED_PLMN_INFO_IND_STRU *)pstMsg;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ����bg history������ʱ������RRMM_SEARCHED_PLMN_INFO_IND */
    if (VOS_FALSE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSearchedPlmnInfoInd_BgPlmnSearch_WaitWasBgPlmnSearchCnf: is not bg history scene");

        return VOS_TRUE;
    }

    if (pstSearchedPlmnInfoMsg->ulAvailPlmnNum > NAS_RRC_MAX_AVAILPLMN_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSearchedPlmnInfoInd_BgPlmnSearch_WaitWasBgPlmnSearchCnf: ulAvailPlmnNum is out of NAS_RRC_MAX_AVAILPLMN_NUM");

        pstSearchedPlmnInfoMsg->ulAvailPlmnNum  = NAS_RRC_MAX_AVAILPLMN_NUM;
    }

    /* ����ϣ�����BG����������״̬Ϊ���������ڣ�����ָ����ʱ����ֻ���Ѵ��ڵ����� */
    for (i = 0; i < pstSearchedPlmnInfoMsg->ulAvailPlmnNum; i++)
    {
        PS_MEM_SET(&stSearchedPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

        /* ת��RRC��ʽ�Ľ��뼼�� */
        NAS_MMC_ConvertRrcRatToNasFormat(pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].ulRat, &(stSearchedPlmn.enRat));
        stSearchedPlmn.stPlmnId.ulMcc   = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMcc;
        stSearchedPlmn.stPlmnId.ulMnc   = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMnc;

        enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&stSearchedPlmn, pstPlmnSelectionListInfo);

        if (NAS_MMC_NET_STATUS_NO_SEARCHED == enNetStatus)
        {
            NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stSearchedPlmn,
                                                           NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                                           pstPlmnSelectionListInfo);
        }
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitWasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_BG_PLMN_SEARCH_CNF_ST                             *pstBgSearchCnf           = VOS_NULL_PTR;

    pstBgSearchCnf      = (RRMM_BG_PLMN_SEARCH_CNF_ST*)pstMsg;

    switch ( pstBgSearchCnf->enSearchRlt )
    {
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_ABORT :
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_SUCCESS :
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_FAIL :
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_NO_RF:

            NAS_MMC_ProcRrMmBgSpecPlmnSearchRslt_BgPlmnSearch(pstBgSearchCnf);
            break;


        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_ABORT :
        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_SUCCESS :
        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_FAIL :
        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_NO_RF:

            NAS_MMC_ProcRrMmBgHistoryPlmnSearchRslt_BgPlmnSearch(pstBgSearchCnf);
            break;


        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitWasBgPlmnSearchCnf: Unexpected Bg search result.");
            break;
    }

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitWasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchList = VOS_NULL_PTR;

    /* ��ȡ�����ȼ������б���Ϣ */
    pstHighPrioPlmnSearchList = NAS_MMC_GetHighPrioPlmnList();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitWasBgPlmnSearchCnf: ENTERED");

    if (VOS_TRUE == NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch())
    {
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    }
    else if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        /* ��bg history��������ȼ��б� */
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_BG_HISTORY_PLMN_SELECTION_LIST);
    }
    else
    {
        /* ���³�ʼ��ѡ���б� */
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    }

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(WUEPS_PID_WRR);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF);


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_GAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(UEPS_PID_GAS);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGrrMmSysInfoInd_BgPlmnSearch_WaitGasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                           *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                            stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;
#if (FEATURE_ON == FEATURE_LTE)

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();


    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();

    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvRrMmSearchedPlmnInfoInd_BgPlmnSearch_WaitGasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    RRMM_SEARCHED_PLMN_INFO_IND_STRU                       *pstSearchedPlmnInfoMsg   = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stSearchedPlmn;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus;
    VOS_UINT32                                              i;


    pstSearchedPlmnInfoMsg  = (RRMM_SEARCHED_PLMN_INFO_IND_STRU *)pstMsg;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ����bg history������ʱ������RRMM_SEARCHED_PLMN_INFO_IND */
    if (VOS_FALSE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSearchedPlmnInfoInd_BgPlmnSearch_WaitGasBgPlmnSearchCnf: is not bg history scene");

        return VOS_TRUE;
    }

    if (pstSearchedPlmnInfoMsg->ulAvailPlmnNum > NAS_RRC_MAX_AVAILPLMN_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSearchedPlmnInfoInd_BgPlmnSearch_WaitGasBgPlmnSearchCnf: ulAvailPlmnNum is out of NAS_RRC_MAX_AVAILPLMN_NUM");

        pstSearchedPlmnInfoMsg->ulAvailPlmnNum  = NAS_RRC_MAX_AVAILPLMN_NUM;
    }

    /* ����ϣ�����BG����������״̬Ϊ���������ڣ�����ָ����ʱ����ֻ���Ѵ��ڵ����� */
    for (i = 0; i < pstSearchedPlmnInfoMsg->ulAvailPlmnNum; i++)
    {
        PS_MEM_SET(&stSearchedPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

        /* ת��RRC��ʽ�Ľ��뼼�� */
        NAS_MMC_ConvertRrcRatToNasFormat(pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].ulRat, &(stSearchedPlmn.enRat));
        stSearchedPlmn.stPlmnId.ulMcc   = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMcc;
        stSearchedPlmn.stPlmnId.ulMnc   = pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMnc;

        enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&stSearchedPlmn, pstPlmnSelectionListInfo);

        if (NAS_MMC_NET_STATUS_NO_SEARCHED == enNetStatus)
        {
            NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stSearchedPlmn,
                                                           NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                                           pstPlmnSelectionListInfo);
        }
    }

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitGasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_BG_PLMN_SEARCH_CNF_ST                             *pstBgSearchCnf           = VOS_NULL_PTR;

    pstBgSearchCnf      = (RRMM_BG_PLMN_SEARCH_CNF_ST*)pstMsg;

    switch ( pstBgSearchCnf->enSearchRlt )
    {
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_ABORT :
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_SUCCESS :
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_FAIL :
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_NO_RF:

            NAS_MMC_ProcRrMmBgSpecPlmnSearchRslt_BgPlmnSearch(pstBgSearchCnf);
            break;


        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_ABORT :
        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_SUCCESS :
        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_FAIL :
        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_NO_RF:

            NAS_MMC_ProcRrMmBgHistoryPlmnSearchRslt_BgPlmnSearch(pstBgSearchCnf);
            break;


        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitGasBgPlmnSearchCnf: Unexpected Bg search result.");
            break;
    }

    return VOS_TRUE;

}


VOS_UINT32    NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitGasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchList = VOS_NULL_PTR;

    /* ��ȡ�����ȼ������б���Ϣ */
    pstHighPrioPlmnSearchList = NAS_MMC_GetHighPrioPlmnList();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitGasBgPlmnSearchCnf: ENTERED");

    if (VOS_TRUE == NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch())
    {
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    }
    else if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        /* ��bg history��������ȼ��б� */
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);

        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_BG_HISTORY_PLMN_SELECTION_LIST);
    }
    else
    {
        /* ���³�ʼ��ѡ���б� */
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    }


    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_GAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(UEPS_PID_GAS);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_CSG)

VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_WAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(WUEPS_PID_WRR);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}

VOS_UINT32    NAS_MMC_RcvRrMmSysInfoInd_BgPlmnSearch_WaitWasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Ŀǰ����ͨ�������յ�ϵͳ��Ϣ����һ�£������������֧��GU�µ�CSG�ٽ������⴦�� */

    NAS_MML_CAMP_PLMN_INFO_STRU         *pstCurCampInfo       = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU          stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU     *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU      stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU     *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU      stOldPsRestrictInfo;

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ� */
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}

VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasCsgBgPlmnSearchCnf: ENTERED");

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);

    /* ����״̬�����Ϊʧ��,����Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

VOS_UINT32    NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitWasCsgBgSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_CSG_BG_SEARCH_CNF_STRU                            *pstCsgBgSearchCnf        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId        = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              aulCsgId[NAS_MML_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       astTmpCsgSearchInfo[NAS_MML_MAX_PLMN_CSG_ID_NUM];

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId,            0xF, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));
    PS_MEM_SET(astTmpCsgSearchInfo, 0,   NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    pstCsgBgSearchCnf        = (RRMM_CSG_BG_SEARCH_CNF_STRU*)pstMsg;

    /* ��ȡCSG�������������б� */
    pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
    pstCurSearchCsgId        = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);

    /* CSG���������б����õ�����ͬ�Ľ��ö�٣�������ֻ��SUCC��REJ��FAIL������� */
    switch (pstCsgBgSearchCnf->enSearchRlt)
    {
        case RRC_NAS_CSG_SEARCH_RLT_SUCC :

            /* ����BG����������״̬Ϊ���������� */
            NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(pstCurSearchCsgId,
                                        NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                        pstCsgIdBgSearchListInfo);

            /* ����������Ӵ���(WAS�������P̬),���ͷ��������� */
            if (VOS_TRUE == NAS_MML_IsRrcConnExist())
            {
                NAS_MMC_SndRrMmRelReq(RRC_NAS_PS_DOMAIN);
                NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
            }
            else
            {
                /* ��MM��GMM��������ָʾ */
                NAS_MMC_SndGmmPlmnSchInit();
                NAS_MMC_SndMmPlmnSchInit();

                NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

                NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                        NAS_MML_GetSimPsRegStatus());

                /* ��ǰֻ֧�ֿ���ָ����1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
                ulCsgIdNum  = 1;
                aulCsgId[0] = pstCurSearchCsgId->ulCsgId;

                /* ����CSG����ָ������ */
                NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(&pstCurSearchCsgId->stPlmnWithRat.stPlmnId,
                                        ulCsgIdNum, aulCsgId,
                                        pstCurSearchCsgId->stPlmnWithRat.enRat);
            }

            break;

        /* REJ���ϱ������뱳��PLMN������ABORT��һ�µ� */
        case RRC_NAS_CSG_SEARCH_RLT_REJ :

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        case RRC_NAS_CSG_SEARCH_RLT_FAIL :

            /* ��CSG BG�������ת��Ϊ�ڲ�CSG������� */
            NAS_MMC_ConvertRrcCsgBgSearchRslt2NasSearchCsgList(pstCsgBgSearchCnf, &ulCsgIdNum, astTmpCsgSearchInfo);

            /* ���������б��������״̬ */
            NAS_MMC_UpdateAllCsgIdNetStatusInCsgIdSelectionList(ulCsgIdNum,
                                        astTmpCsgSearchInfo,
                                        pstCsgIdBgSearchListInfo,
                                        VOS_TRUE);

            /* ����BG������ʧ�ܽ�� */
            NAS_MMC_ProcCsgBgSearchRlstFail_BgPlmnSearch();

            break;

        case RRC_NAS_CSG_SEARCH_RLT_NO_RF :

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitWasCsgBgSearchCnf: Unexpected Bg search result.");
            break;
    }

    return VOS_TRUE;
}

VOS_UINT32    NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitWasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSearchList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitWasCsgBgPlmnSearchCnf: ENTERED");

    /* ��ȡCSG���������б� */
    pstCsgIdSearchList = NAS_MMC_GetCsgIdBgSearchList();

    if (NAS_MMC_USE_CSG_SEARCH_LIST_PROC_ASF_SEARCH == NAS_MMC_GetUseCsgIdSearchListProc())
    {
        enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_ASF_CSG_SEARCH;
    }
    else
    {
        enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_PSF_CSG_SEARCH;
    }

    /* ��ʼ��CSG���������б� */
    NAS_MMC_InitPlmnSelectionList(enPlmnSearchScene,
                                  VOS_NULL_PTR,
                                  pstCsgIdSearchList);

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(WUEPS_PID_WRR);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}

VOS_UINT32    NAS_MMC_RcvRrMmAreaLostInd_BgPlmnSearch_WaitWasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_AREA_LOST_IND_STRU                                *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (RRMM_AREA_LOST_IND_STRU*)pstMsg;

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);

    NAS_MMC_SaveLastCampedPlmnWithRat(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.enCampPlmnNetRat);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    NAS_MMC_BuildPlmnListInfoFromGuAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF);

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_GAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(UEPS_PID_GAS);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGrrMmSysInfoInd_BgPlmnSearch_WaitGasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Ŀǰ����ͨ�������յ�ϵͳ��Ϣ����һ�£������������֧��GU�µ�CSG�ٽ������⴦�� */

    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo       = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU    *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU     stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU    *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU     stOldPsRestrictInfo;
#if (FEATURE_ON == FEATURE_LTE)

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();


    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitGasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_CSG_BG_SEARCH_CNF_STRU                            *pstCsgBgSearchCnf        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId        = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              aulCsgId[NAS_MML_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       astTmpCsgSearchInfo[NAS_MML_MAX_PLMN_CSG_ID_NUM];

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId,            0xF, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));
    PS_MEM_SET(astTmpCsgSearchInfo, 0,   NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    pstCsgBgSearchCnf        = (RRMM_CSG_BG_SEARCH_CNF_STRU*)pstMsg;

    /* ��ȡCSG�������������б� */
    pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
    pstCurSearchCsgId        = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF);

    /* CSG���������б����õ�����ͬ�Ľ��ö�٣�������ֻ��SUCC��REJ��FAIL������� */
    switch (pstCsgBgSearchCnf->enSearchRlt)
    {
        case RRC_NAS_CSG_SEARCH_RLT_SUCC :

            /* ����BG����������״̬Ϊ���������� */
            NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(pstCurSearchCsgId,
                                        NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                        pstCsgIdBgSearchListInfo);

            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                    NAS_MML_GetSimPsRegStatus());

            /* ��ǰֻ֧�ֿ���ָ����1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
            ulCsgIdNum  = 1;
            aulCsgId[0] = pstCurSearchCsgId->ulCsgId;

            /* ����CSG����ָ������ */
            NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(&pstCurSearchCsgId->stPlmnWithRat.stPlmnId,
                                        ulCsgIdNum, aulCsgId,
                                        pstCurSearchCsgId->stPlmnWithRat.enRat);

            break;

        /* REJ���ϱ������뱳��PLMN������ABORT��һ�µ� */
        case RRC_NAS_CSG_SEARCH_RLT_REJ :

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        case RRC_NAS_CSG_SEARCH_RLT_FAIL :

            /* ��CSG BG�������ת��Ϊ�ڲ�CSG������� */
            NAS_MMC_ConvertRrcCsgBgSearchRslt2NasSearchCsgList(pstCsgBgSearchCnf, &ulCsgIdNum, astTmpCsgSearchInfo);

            /* ���������б��������״̬ */
            NAS_MMC_UpdateAllCsgIdNetStatusInCsgIdSelectionList(ulCsgIdNum,
                                        astTmpCsgSearchInfo,
                                        pstCsgIdBgSearchListInfo,
                                        VOS_TRUE);

            /* ����BG������ʧ�ܽ�� */
            NAS_MMC_ProcCsgBgSearchRlstFail_BgPlmnSearch();

            break;

        case RRC_NAS_CSG_SEARCH_RLT_NO_RF :

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitGasCsgBgPlmnSearchCnf: Unexpected Bg search result.");
            break;
    }


    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitGasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSearchList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitGasCsgBgPlmnSearchCnf: ENTERED");

    /* ��ȡCSG���������б� */
    pstCsgIdSearchList = NAS_MMC_GetCsgIdBgSearchList();

    if (NAS_MMC_USE_CSG_SEARCH_LIST_PROC_ASF_SEARCH == NAS_MMC_GetUseCsgIdSearchListProc())
    {
        enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_ASF_CSG_SEARCH;
    }
    else
    {
        enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_PSF_CSG_SEARCH;
    }

    /* ��ʼ��CSG���������б� */
    NAS_MMC_InitPlmnSelectionList(enPlmnSearchScene,
                                  VOS_NULL_PTR,
                                  pstCsgIdSearchList);

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_GAS_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndAsBgStopPlmnSearchReq(UEPS_PID_GAS);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}

VOS_UINT32    NAS_MMC_RcvRrMmAreaLostInd_BgPlmnSearch_WaitGasCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_AREA_LOST_IND_STRU                                *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (RRMM_AREA_LOST_IND_STRU*)pstMsg;

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);

    NAS_MMC_SaveLastCampedPlmnWithRat(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.enCampPlmnNetRat);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );

    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    NAS_MMC_BuildPlmnListInfoFromGuAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

#endif




VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitWasSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf      = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstCurHighPrioPlmn = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;

#if (FEATURE_ON == FEATURE_CSG)
    VOS_UINT32                          ulCsgIdNum;
    VOS_UINT32                          aulCsgId[NAS_MML_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU  *pstCurSearchCsgId  = VOS_NULL_PTR;

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId, 0xF, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));
#endif

    pstSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF);

    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitWasSuspendCnf: Suspend fail");

        /* ���ɻָ�����,���õ���ӿ�RESET */

        if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
          && (NAS_MML_NET_RAT_TYPE_WCDMA   == enCurRat) )
        {
            NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_WAS_SUSPEND_FAILURE);
        }
        else
        {
            NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_TD_SUSPEND_FAILURE);
        }

        return VOS_TRUE;
    }

    /* ���õ�ǰ���뼼��ΪBUTT */
    NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

    /* ��ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_CSG)

    /* �����ǰ��CSG���������̣���ֱ��������һ��CSG ID������ */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        pstCurSearchCsgId = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

        /* ��ǰֻ֧�ֿ���ָ����1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
        ulCsgIdNum  = 1;
        aulCsgId[0] = pstCurSearchCsgId->ulCsgId;

        /* ����CSG����ָ������ */
        NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(&pstCurSearchCsgId->stPlmnWithRat.stPlmnId,
                                        ulCsgIdNum, aulCsgId,
                                        pstCurSearchCsgId->stPlmnWithRat.enRat);

        return VOS_TRUE;
    }
#endif

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ���п���ָ������ */
    NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(pstCurHighPrioPlmn->astPlmnId[0].stPlmnId),
                                               pstCurHighPrioPlmn->astPlmnId[0].enRat);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitAsSuspendCnfExpired_BgPlmnSearch_WaitAsSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitAsSuspendCnfExpired_BgPlmnSearch_WaitAsSuspendCnf: ENTERED");

    /* ���ɻָ����ϣ���λ */

    if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
      && (NAS_MML_NET_RAT_TYPE_WCDMA   == enCurRat) )
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_WAS_SUSPEND_CNF_EXPIRED);
    }
    else if ( NAS_MML_NET_RAT_TYPE_GSM == enCurRat)
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_GAS_SUSPEND_CNF_EXPIRED);
    }
    else if ( NAS_MML_NET_RAT_TYPE_LTE == enCurRat)
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_LMM_SUSPEND_CNF_EXPIRED);
    }
    else
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_TD_SUSPEND_CNF_EXPIRED);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitGasSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf      = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstCurHighPrioPlmn = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_CSG)
    VOS_UINT32                          ulCsgIdNum;
    VOS_UINT32                          aulCsgId[NAS_MML_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU  *pstCurSearchCsgId  = VOS_NULL_PTR;

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId, 0xF, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));
#endif

    pstSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF);

    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitGasSuspendCnf: Suspend fail");

        /* ���ɻָ�����,���õ���ӿ�RESET */

        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_GAS_SUSPEND_FAILURE);

        return VOS_TRUE;
    }

    /* ���õ�ǰ���뼼��ΪBUTT */
    NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_CSG)

    /* �����ǰ��CSG���������̣���ֱ��������һ��CSG ID������ */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        pstCurSearchCsgId = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

        /* ��ǰֻ֧�ֿ���ָ����1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
        ulCsgIdNum  = 1;
        aulCsgId[0] = pstCurSearchCsgId->ulCsgId;

        /* ����CSG����ָ������ */
        NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(&pstCurSearchCsgId->stPlmnWithRat.stPlmnId,
                                        ulCsgIdNum, aulCsgId,
                                        pstCurSearchCsgId->stPlmnWithRat.enRat);

        return VOS_TRUE;
    }
#endif

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ���п���ָ������ */
    NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(pstCurHighPrioPlmn->astPlmnId[0].stPlmnId),
                                               pstCurHighPrioPlmn->astPlmnId[0].enRat);

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSysInfoInd_BgPlmnSearch_WaitWasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;
#if (FEATURE_ON == FEATURE_LTE)

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);


    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */

        NAS_MMC_SndMsccSysInfo();
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMSCCģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ� */
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ж�ԭ��ֵ */

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasBgStopCnf: ENTERED");

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊʧ��,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvBgPlmnStopCnf_BgPlmnSearch_WaitWasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitAsBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_BG_PLMN_SEARCH_CNF_ST                             *pstBgSearchCnf;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;

    pstBgSearchCnf      = (RRMM_BG_PLMN_SEARCH_CNF_ST*)pstMsg;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    NAS_MMC_ConvertRrcBgPlmnSearchRslt2SearchedPlmnListInfo(pstCurHighPrioPlmn->astPlmnId[0].enRat,
                                                            pstBgSearchCnf,
                                                            &stSearchedPlmnListInfo);

    if (RRC_BG_SPEC_PLMN_SEARCH_RLT_FAIL    == pstBgSearchCnf->enSearchRlt)
    {

        /* ���������б��������״̬:���ĸ�����:���й�ȫƵ�� */
        NAS_MMC_UpdatePlmnListInPlmnSelectionList(&stSearchedPlmnListInfo,
                                                  pstPlmnSelectionListInfo,
                                                  VOS_FALSE,
                                                  VOS_TRUE);

    }
    else if (RRC_BG_HISTORY_PLMN_SEARCH_RLT_FAIL == pstBgSearchCnf->enSearchRlt)
    {
        /* ���������б��������״̬:���ĸ�����:δ���й�ȫƵ�� */
        NAS_MMC_UpdatePlmnListInPlmnSelectionList(&stSearchedPlmnListInfo,
                                                  pstPlmnSelectionListInfo,
                                                  VOS_FALSE,
                                                  VOS_FALSE);
    }
    else
    {
    }

    /* ��ά�ɲ�,��ѡ���б���� */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);



    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF);

    /*GU�����ô˺�����������ʱ����Ҫͣ*/
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF);


    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiBgStopCnfExpired_BgPlmnSearch_WaitWasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiBgStopCnfExpired_BgPlmnSearch_WaitWasBgStopCnf: ENTERED");

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasBgPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGrrMmSysInfoInd_BgPlmnSearch_WaitGasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                           *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                            stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();

    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvBgPlmnStopCnf_BgPlmnSearch_WaitGasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvTiBgStopCnfExpired_BgPlmnSearch_WaitGasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiBgStopCnfExpired_BgPlmnSearch_WaitGasBgStopCnf: ENTERED");

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_CSG)

VOS_UINT32    NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitAsBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_CSG_BG_SEARCH_CNF_STRU                            *pstCsgBgSearchCnf        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       astTmpCsgSearchInfo[NAS_MML_MAX_PLMN_CSG_ID_NUM];

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(astTmpCsgSearchInfo, 0,   NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    pstCsgBgSearchCnf        = (RRMM_CSG_BG_SEARCH_CNF_STRU*)pstMsg;

    /* ��ȡCSG�������������б� */
    pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ֹͣ������ʱ����GU�����ô˺�����������ʱ����Ҫͣ */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF);

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF);

    /* ��CSG BG�������ת��Ϊ�ڲ�CSG������� */
    NAS_MMC_ConvertRrcCsgBgSearchRslt2NasSearchCsgList(pstCsgBgSearchCnf, &ulCsgIdNum, astTmpCsgSearchInfo);

    /* ���������б��������״̬ */
    NAS_MMC_UpdateAllCsgIdNetStatusInCsgIdSelectionList(ulCsgIdNum,
                                astTmpCsgSearchInfo,
                                pstCsgIdBgSearchListInfo,
                                VOS_TRUE);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊʧ��,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();
    return VOS_TRUE;

}
#endif



VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ж�ԭ��ֵ */

    RRMM_REL_IND_STRU                  *pstRrmmRelIndMsg    = VOS_NULL_PTR;

    pstRrmmRelIndMsg = (RRMM_REL_IND_STRU *)pstMsg;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasFastPlmnStopCnf: ENTERED");

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF);

    if (RRC_REL_CAUSE_NO_RF == pstRrmmRelIndMsg->ulRelCause)
    {
        /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);

        return VOS_TRUE;
    }

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSrchStopCnf_BgPlmnSearch_WaitWasFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF);

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitWasStopCnfExpired_BgPlmnSearch_WaitWasFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasStopCnfExpired_BgPlmnSearch_WaitWasFastPlmnStopCnf: ENTERED");

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
;

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }


    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSearchStopCnf_BgPlmnSearch_WaitGasFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF);

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSrchCnf_BgPlmnSearch_WaitAsFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU                              *pstSrchCnfMsg            = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF);

    /*GU�����ô˺�������ʱ����Ҫͣ*/
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF);

    if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_GetCurrNetRatType());
        }

        return VOS_TRUE;
    }

    if (RRC_PLMN_SEARCH_RLT_SPEC_FAIL == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ���µ�ǰ��������״̬Ϊ������������ */
        /* bg history���������н�����ϱ�plmn info ind������ָ����ֻ��һ��plmn:�б��е�һ�����ڵ����� */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                                       NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                                       pstPlmnSelectionListInfo);
    }



    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvTiWaitGasStopCnfExpired_BgPlmnSearch_WaitGasFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasStopCnfExpired_BgPlmnSearch_WaitGasFastPlmnStopCnf: ENTERED");

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
;

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }


    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndAsPlmnSrchStopReq(WUEPS_PID_WRR);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_FAST_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSrchCnf_BgPlmnSearch_WaitWasPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU                              *pstSrchCnfMsg            = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ֹͣ�ȿ�����������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);

    if (RRC_PLMN_SEARCH_RLT_SPEC == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_SYSINFO_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_SYS_INFO, TI_NAS_MMC_WAIT_WAS_SYS_INFO_LEN);
    }
    else if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);
        }

        return VOS_TRUE;
    }
    else
    {
        /* ���µ�ǰ��������״̬Ϊ������������ */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                                NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                                pstPlmnSelectionListInfo);

        /* ������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
        }
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    /* �����ж�ԭ��ֵ */

    RRMM_REL_IND_STRU                  *pstRrmmRelIndMsg    = VOS_NULL_PTR;

    pstRrmmRelIndMsg = (RRMM_REL_IND_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasPlmnFastSearchCnf: ENTERED");

    /* ֹͣ�ȿ�����������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);

    /* ���µ�ǰ��������״̬Ϊ������������ */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                            NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                            pstPlmnSelectionListInfo);

    if (RRC_REL_CAUSE_NO_RF == pstRrmmRelIndMsg->ulRelCause)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);
        }

        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitWasPlmnSearchCnfExpired_BgPlmnSearch_WaitWasPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasPlmnSearchCnfExpired_BgPlmnSearch_WaitWasPlmnFastSearchCnf: ENTERED");

    /* ���µ�ǰ��������״̬Ϊ������������ */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                            NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                            pstPlmnSelectionListInfo);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndAsPlmnSrchStopReq(WUEPS_PID_WRR);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_FAST_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF);


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndAsPlmnSrchStopReq(UEPS_PID_GAS);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_FAST_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSrchCnf_BgPlmnSearch_WaitGasPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU                              *pstSrchCnfMsg            = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ֹͣ�ȿ�����������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF);

    if (RRC_PLMN_SEARCH_RLT_SPEC == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_SYSINFO_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_SYS_INFO, TI_NAS_MMC_WAIT_GAS_SYS_INFO_LEN);
    }
    else if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_GSM);
        }


        return VOS_TRUE;
    }
    else
    {
        /* ���µ�ǰ��������״̬Ϊ������������ */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                                NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                                pstPlmnSelectionListInfo);

        /* ������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitGasPlmnSearchCnfExpired_BgPlmnSearch_WaitGasPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasPlmnSearchCnfExpired_BgPlmnSearch_WaitGasPlmnFastSearchCnf: ENTERED");

    /* ���µ�ǰ��������״̬Ϊ������������ */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                            NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                            pstPlmnSelectionListInfo);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndAsPlmnSrchStopReq(UEPS_PID_GAS);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_FAST_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSysInfoInd_BgPlmnSearch_WaitWasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_SYS_INFO);

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* ��������״̬���ȱ���ע��������ϱ�ϵͳ��Ϣ */
        if (VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
        {
            NAS_MMC_SetNeedSndSysInfo_BgPlmnSearch(VOS_TRUE);
        }
        else
        {
            /* �����ϱ� */
            NAS_MMC_SndMsccSysInfo();
        }
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ� */
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmWasSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    /* ���µ�ǰ�ȴ�CSPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ����CSPS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_CSPS_REG_IND);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitWasSysInfoExpired_BgPlmnSearch_WaitWasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasSysInfoExpired_BgPlmnSearch_WaitWasSysInfoInd: ENTERED");

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasSysInfo(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGrrMmSysInfoInd_BgPlmnSearch_WaitGasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_SYS_INFO);

    /* ����פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
            /* ��������״̬���ȱ���ע��������ϱ�ϵͳ��Ϣ */
        if (VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
        {
            NAS_MMC_SetNeedSndSysInfo_BgPlmnSearch(VOS_TRUE);
        }
        else
        {
            /* �����ϱ� */
            NAS_MMC_SndMsccSysInfo();
        }
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMSCCģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))
    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
    NAS_MMC_SndMmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmGsmSysInfoInd (NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    /* ���µ�ǰ�ȴ�CSPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ����CSPS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_CSPS_REG_IND);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitGasSysInfoExpired_BgPlmnSearch_WaitGasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasSysInfoExpired_BgPlmnSearch_WaitGasSysInfoInd: ENTERED");

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }


    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU                *pstAbortMsg = VOS_NULL_PTR;
    NAS_MMC_WAIT_REG_RSLT_IND_ENUM_UINT8   enRegRslt;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;
    /* ��ȡ���� */
    NAS_MMC_SndMsccSysInfo_BgPlmnSearch();


    /* ������������û�����Ĺػ�����ֱ���˳���ע����״̬�� */
    if (NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* �����ǰ����������ϱ�־���������ͷ� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
    {
        /* ֹͣ�ȴ�ע������ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);


        /* ��ע��������յ�������ϣ���ζ��ע����δ��ɣ�����δ��ɵ���ķ���
           ״̬ΪLIMIT SERVICE������ǰ�ķ���״̬����ΪNORMAL_SERVICE,���������û�
           ָ���Ѵ�ϣ���������뼼������ͬ������£��Ͳ����������ע���� */
        enRegRslt = NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch();
        if ( enRegRslt>= NAS_MMC_WAIT_REG_RESULT_IND_BUTT )
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch:NAS_MMC_WAIT_REG_RESULT_IND_BUTT");
        }
        NAS_MMC_UpdateDomainServiceStatus(enRegRslt,
                                          NAS_MMC_LIMITED_SERVICE);


        /* �����ǰ���������ӣ����˳�״̬�� */
        if (VOS_FALSE == NAS_MML_IsRrcConnExist())
        {
            /* ����״̬�����ΪABORT,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                                VOS_FALSE, VOS_NULL_PTR );

            /* ֱ���˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_TRUE;
        }

        /* �����ǰ��������,���������ͷű�־ */
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);

        /* ��MM��GMM���������ͷ����� */
        NAS_MMC_SndMmRelReq();
        NAS_MMC_SndGmmRelReq();

        NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

        /* ״̬Ǩ�Ƶ��ȴ������ͷ� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);

        /* �����ȴ������ͷŶ�ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGmmPsRegResultInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_PS_REG_RESULT_IND_STRU      *pstPsRegRslt = VOS_NULL_PTR;

    pstPsRegRslt = (GMMMMC_PS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstPsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ���PS��ĵȴ���־ */
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    if(GMM_MMC_ACTION_RESULT_SUCCESS == pstPsRegRslt->enActionResult)
    {
        /*ע����ΪSUCCESS�����ȱ�ϵͳ��Ϣ���ٱ�ע����*/
        NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
    }
    else
    {
        /*ע����Ϊfailure�����ע�������룬���ȱ�ϵͳ��Ϣ*/
        if (NAS_MMC_WAIT_REG_RESULT_IND_NULL == NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
        {
            NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
        }
    }

    /* ����PSע���� */
    NAS_MMC_ProcPsRegRslt_BgPlmnSearch(pstPsRegRslt);

    /* ���ע������δ���룬�˳�����, �����ȴ� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* �����ҪAbort */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmCsRegResultInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_CS_REG_RESULT_IND_STRU       *pstCsRegRslt = VOS_NULL_PTR;

    pstCsRegRslt = (MMMMC_CS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstCsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ���CS��ĵȴ���־ */
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_CS);

    /*ע����Ϊ successful ������ϵͳ��Ϣ�ٱ�״̬�����������*/
    if (MM_MMC_LU_RESULT_SUCCESS == pstCsRegRslt->enLuResult)
    {
        NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
    }
    else
    {
        /*ע����Ϊfailure������������ϱ�ϵͳ��Ϣ*/
        if (NAS_MMC_WAIT_REG_RESULT_IND_NULL == NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
        {
            NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
        }
    }

    /* ����CSע���� */
    NAS_MMC_ProcCsRegRslt_BgPlmnSearch(pstCsRegRslt);

    /* ���ע������δ���룬�˳�����, �����ȴ� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* �����ҪAbort */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmAbortInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_ABORT_IND_STRU               *pstMmAbortInd   = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU    stActionRslt;
#endif

    pstMmAbortInd = (MMMMC_ABORT_IND_STRU*)pstMsg;

    /* ���ԭ��ֵ��Ϊ#6���Ҳ�Ϊ#17��ֱ�ӷ��� */
    if ((NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME != pstMmAbortInd->enAbortCause)
     && (NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE != pstMmAbortInd->enAbortCause))
    {
        return VOS_TRUE;
    }

    /* ���ԭ��ֵ#6������CS AdditionΪAnycellפ�� */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstMmAbortInd->enAbortCause)
    {
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* ��LMM����ID_MMC_LMM_ACTION_RESULT_REQ */
    /* ����ActionRslt��Ϣ�����ΪCN_Rej,������Ϊ����NAS_MML_PROC_MM_ABORT����
       ����ͽ������CS��ԭ��ֵΪAbort����ԭ��ֵ */
    stActionRslt.enProcType     = NAS_MML_PROC_MM_ABORT;
    stActionRslt.enRegRst       = NAS_MML_REG_RESULT_CN_REJ;
    stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enCnCause      = pstMmAbortInd->enAbortCause;
    stActionRslt.ulAttemptCount = 0x0;

    NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
#endif

    return VOS_TRUE;
}





VOS_UINT32    NAS_MMC_RcvGmmNetworkDetachInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCGMM_NETWORK_DETACH_IND_STRU                         *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstDetachMsg = (MMCGMM_NETWORK_DETACH_IND_STRU*)pstMsg;

    /* ���PS��ĵȴ���־ */
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ע���������ˣ����ȱ�ϵͳ��Ϣ */
    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL == NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
    }

    /* ����Detach��� */
    enAdditionalAction = NAS_MMC_ProcGmmNetworkDetachInd(pstDetachMsg);

    /* AdditionalAction��Ч������PS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    /* ��Ҫ����ע��ʱ������Ҫִ�к����Ķ��� */
    if (NAS_MMC_GMM_NT_DETACH_REATTACH == pstDetachMsg->ulDetachType)
    {
        return VOS_TRUE;
    }

#if   (FEATURE_ON == FEATURE_LTE)
    /* ��LMM����detach�����Ϣ */
    NAS_MMC_SndLmmMtDetachIndActionResultReq(pstDetachMsg->ulDetachType,
        (NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstDetachMsg->ulDetachCause);
#endif

    /* ���ע������δ���룬�˳�����, �����ȴ� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }


    /* �����ҪAbort */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGmmServiceRequestResultInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU                 *pstServiceRsltInd = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
#endif

    pstServiceRsltInd = (GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU*)pstMsg;

    /*���PS�ȴ�ע������ʾ*/
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    if (GMM_MMC_ACTION_RESULT_SUCCESS == pstServiceRsltInd->ulServiceSts)
    {
        /*ע����Ϊ successful ������ϵͳ��Ϣ�ٱ�״̬�����������*/
        NAS_MMC_SndMsccSysInfo_BgPlmnSearch();

        /* ����PS��AdditionalAction */
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
        /* ��LMM����Service requestResult�����Ϣ����*/
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
        stActionRslt.ulAttemptCount = 0;

        NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);

#endif
    }
    else
    {
        /*ע���������ˣ����ȱ�ϵͳ��Ϣ*/
        if (NAS_MMC_WAIT_REG_RESULT_IND_NULL == NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
        {
            NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
        }
        /* �������ʧ�ܣ�����ͬ�ܾ�ԭ��ֵ������PS additional Action */
        enAdditionalAction = NAS_MMC_ProcPsServiceRegFail(pstServiceRsltInd);

        /* AdditionalAction��Ч������PS����һ���Ķ������� */
        if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
        {
            NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
        }

#if (FEATURE_ON == FEATURE_LTE)

        /* ��LMM����Service requestResult�����Ϣ���� */
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = pstServiceRsltInd->enRegFailCause;
        stActionRslt.ulAttemptCount = 0;

        NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);

#endif


    }

    /* ���ע������δ���룬�˳�����, �����ȴ� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* �����ҪAbort */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvCmServiceRejectInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_CM_SERVICE_REJECT_IND_STRU   *pstCmServiceRejMsg = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU    stActionRslt;
#endif

    pstCmServiceRejMsg = (MMCMM_CM_SERVICE_REJECT_IND_STRU*)pstMsg;

#if (FEATURE_ON == FEATURE_LTE)
    stActionRslt.enProcType     = NAS_MML_PROC_CM_SER;
    stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
    stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enCnCause      = (NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstCmServiceRejMsg->ulCause;
    stActionRslt.ulAttemptCount = 0;

    NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);

#endif

    /* ����ܾ�ԭ��ΪNAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstCmServiceRejMsg->ulCause)
    {
        /* ���CS��ĵȴ���־ */
        NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_CS);

        /* ����CS additional ActionΪ����פ�� */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);
    }

    /* ���ע������δ���룬�˳�����, �����ȴ� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* ϵͳ��Ϣ��Ԥ�����з��� */

    /* �����ҪAbort */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvWasSuspendInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_IND_ST                *pstSuspendMsg = VOS_NULL_PTR;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;

    if ((MMC_SUSPEND_CAUSE_PLMNSEARCH == pstSuspendMsg->ucSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= pstSuspendMsg->ucSuspendCause))
    {
        /* �����OOS���������ع���ʧ�� */
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, WUEPS_PID_WRR);
    }
    else
    {
        /* ������ݹ���ԭ��ֵ��������ͬ��״̬�� */
        NAS_MMC_LoadInterSysFsm_BgPlmnSearch(pstSuspendMsg->ucSuspendCause);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGasSuspendInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_IND_ST                *pstSuspendMsg = VOS_NULL_PTR;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;

    if ((MMC_SUSPEND_CAUSE_PLMNSEARCH == pstSuspendMsg->ucSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= pstSuspendMsg->ucSuspendCause))
    {
        /* �����OOS���������ع���ʧ�� */
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, UEPS_PID_GAS);
    }
    else
    {
        /* ������ݹ���ԭ��ֵ��������ͬ��״̬�� */
        NAS_MMC_LoadInterSysFsm_BgPlmnSearch(pstSuspendMsg->ucSuspendCause);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcMmcSuspendRslt_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRsltMsg = VOS_NULL_PTR;

    pstSuspendRsltMsg = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

    /* �����ѡ���Ϊʧ��,ԭ��Ϊ��������,����ע������г����������� */
    if ((NAS_MMC_SUSPEND_FAIL == pstSuspendRsltMsg->enRslt)
     && (NAS_MMC_SUSPEND_FAIL_CAUSE_AREA_LOST == pstSuspendRsltMsg->enFailCause))
    {
        /* ֹͣ�ȴ�ע������ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        if (VOS_FALSE == NAS_MML_GetRfAvailFlg())
        {
            /* ���ϲ�״̬�����ͱ�������ʧ�ܣ�����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_TRUE;
        }

        /* �����ǰ��ABORT��־�����VPLMN */
        if ( NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch() )
        {
            /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
               disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

            if (VOS_TRUE == NAS_MMC_IsVplmnValid())
            {
                /* ��MM��GMM��������ָʾ */
                NAS_MMC_SndGmmPlmnSchInit();
                NAS_MMC_SndMmPlmnSchInit();

                NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

                NAS_MMC_ReCampVPlmn_BgPlmnSearch();
                return VOS_TRUE;
            }

            /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }
        else
        {
            /* ������һ�������ȼ����� */
            if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
            {
                /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

                /* �˳�״̬�� */
                NAS_MMC_FSM_QuitFsmL2();
                return VOS_TRUE;
            }

            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
        }

        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        /* ֹͣ��ǰ�ȴ�CS/PSע������״̬ */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* ���µ�ǰ�ȴ�EPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
        NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

#if (FEATURE_ON == FEATURE_IMS)
        /* ���ӵȴ�IMS�Ƿ���ý��FLG,MMA��֤�յ�PSע�����ϱ�IMS�Ƿ����ָʾ��MMC */
        NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_IMS);
#endif

        /* ����CS��ע��Cause��AdditionalAction */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* Ǩ�Ƶ��ȴ�ע������״̬:NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_REG_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);

        return VOS_TRUE;
    }
#endif

    return VOS_TRUE;
}

 
VOS_UINT32    NAS_MMC_RcvAreaLostInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurPlmn;

    NAS_MML_PLMN_WITH_CNT_AND_RAT_STRU                     *pstLastAreaLostPlmnInfo = VOS_NULL_PTR;
    RRMM_AREA_LOST_IND_STRU                                *pstAreaLostMsg          = VOS_NULL_PTR;
    VOS_UINT32                                              ulIsRrcSearchPlmn;

    /* ֹͣ�ȴ�ע������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

    NAS_MMC_SaveLastCampedPlmnWithRat(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.enCampPlmnNetRat);

    NAS_MMC_SndMsccSysInfo_BgPlmnSearch();

    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo     = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId = pstCurCampInfo->stLai.stPlmnId;
    stCurPlmn.enRat    = pstCurCampInfo->enNetRatType;

    pstAreaLostMsg          = (RRMM_AREA_LOST_IND_STRU*)pstMsg;
    ulIsRrcSearchPlmn       = NAS_MMC_IsGuRrcSearchedAreaLostPlmn(pstAreaLostMsg);
    pstLastAreaLostPlmnInfo = NAS_MMC_GetLastAreaLostPLmnInfoCtx_BgPlmnSearch();

    NAS_MMC_UpdateLastAreaLostPlmnWIthRatAndCnt(pstLastAreaLostPlmnInfo, &stCurPlmn.stPlmnId, stCurPlmn.enRat);

    /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                            NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                            NAS_MMC_GetHighPrioPlmnList());

    NAS_WARNING_LOG2(WUEPS_PID_MMC,"NAS_MMC_RcvAreaLostInd_BgPlmnSearch_WaitCsPsRegRsltInd: ulIsRrcSearchPlmn = %d , %d",
                                            ulIsRrcSearchPlmn, NAS_MML_GetLastAreaLostPlmnCnt(pstLastAreaLostPlmnInfo));

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());


    /* ֪ͨMM/GMM���붪��,MM���ͷ�CSҵ�� */
    NAS_MMC_SndMmCoverageLostInd();
    NAS_MMC_SndGmmCoverageLostInd();


    /* �����ǰ����NO RF����״̬��������ʧ����Ϣ��Ǩ�Ƶ�OOC״̬ */
    if (VOS_FALSE == NAS_MML_GetRfAvailFlg())
    {
         NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvAreaLostInd_BgPlmnSearch_WaitCsPsRegRsltInd: RF FALSE");
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ�����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
    /* �����ǰ��ABORT��־�����VPLMN */
    if ( NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch() )
    {
         NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvAreaLostInd_BgPlmnSearch_WaitCsPsRegRsltInd: NAS_MMC_GetAbortType_BgPlmnSearch");
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ��յȴ�ע������־ */
    NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();

    if ( (NAS_MML_GetLastAreaLostPlmnCnt(pstLastAreaLostPlmnInfo) >= NAS_MMC_AREA_LOST_SEARCH_MAX_NUM)
      || (VOS_TRUE == ulIsRrcSearchPlmn) )
    {
        /* ������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
             NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvAreaLostInd_BgPlmnSearch_WaitCsPsRegRsltInd: NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch false");
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;

        }
    }
    else
    {
        /* ���п���ָ������ */
        NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&pstLastAreaLostPlmnInfo->stPlmnId,
                                                   pstLastAreaLostPlmnInfo->enRat);
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSysInfoInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

    /* פ��С���仯ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �������յ�ϵͳ��Ϣʱ����ǰ���Ʒ����Ѿ���MMA����ϵͳ��Ϣ���˴����Ը�MMA����
           ����Ҫ�ȵ��յ�ע�������ٸ�MMA����ϵͳ��Ϣ */
        if (VOS_FALSE == NAS_MMC_GetNeedSndSysInfo_BgPlmnSearch())
        {
            /* �����ϱ� */
            NAS_MMC_SndMsccSysInfo();

        }
    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);

    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);

    }

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* �������õȴ�CS��PSע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                               VOS_FALSE,
                               pstMsg);

    NAS_MMC_SndGmmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                               VOS_FALSE,
                               pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGrrMmSysInfoInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �������յ�ϵͳ��Ϣʱ����ǰ���Ʒ����Ѿ���MSCC����ϵͳ��Ϣ���˴����Ը�MSCC����
           ����Ҫ�ȵ��յ�ע�������ٸ�MSCC����ϵͳ��Ϣ */
        if (VOS_FALSE == NAS_MMC_GetNeedSndSysInfo_BgPlmnSearch())
        {
            /* �����ϱ� */
            NAS_MMC_SndMsccSysInfo();
        }
    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMSCCģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);

    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);

    }

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* �������õȴ�CS��PSע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                               VOS_FALSE,
                               pstMsg);

    NAS_MMC_SndGmmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                               VOS_FALSE,
                               pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTafSysCfgReq_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcSyscfgRsltCnf_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���VPLMN���뼼����ǰ��֧�֣����轫VPLMN���뼼������ΪSYSCFG���õ�������Ƚ��뼼����
       �Ȼ�ȡ��һ������ʱ���ж�vplmn���뼼���Ƿ���Ч��vplmn��Чʱ��Ҫ��״̬�����Ϊ��Ҫ������
       ����פ�����ٽ��б����ѣ��ú����ֲ�����������������չʹ�� */
    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitCsPsRegRsltIndExpired_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitCsPsRegRsltIndExpired_BgPlmnSearch_WaitCsPsRegRsltInd: ENTERED");

    NAS_MMC_SndMsccSysInfo_BgPlmnSearch();

    /* ��������������� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        /* ״̬Ǩ�Ƶ��ȴ������ͷ� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);

        /* �����ȴ������ͷŶ�ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* �����ҪAbort�����л�VPLMN���� */
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }
    else
    {
        /* ��յȴ�ע������־ */
        NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();

        /* �����ȡ��һ�������ȼ������������ */
        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;

        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;
    VOS_UINT8                           ucRelRequestFlg;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ������������û�����Ĺػ�����ֱ���˳��������ͷŽ��״̬�� */
    if (NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)

    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    ucRelRequestFlg = NAS_MMC_GetRelRequestFlag_BgPlmnSearch();

    /* �����ǰ����������ϱ�־���Ҳ��������ͷţ��������ͷ� */
    if ((NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
     && (VOS_FALSE == ucRelRequestFlg))
    {
        /* ���������ͷű�־ */
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);

        /* ��MM��GMM���������ͷ����� */
        NAS_MMC_SndMmRelReq();
        NAS_MMC_SndGmmRelReq();

        NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGmmSignalingStatusInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����Ȼ�����������ӣ������ȴ� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ����������ͷű�����ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    /* ���Abor����Ϊ�����˳������˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort�������ӳ��˳������л�VPLMN���� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ����������ͷ�(����ȴ������ͷų�ʱ),������һ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;

        }


        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }
    else
    {
        /* ��ǰΪ�����ͷţ�����CS��PS��additional Action��һ������ */
        NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();
    }


    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGmmNetworkDetachInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCGMM_NETWORK_DETACH_IND_STRU                         *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstDetachMsg = (MMCGMM_NETWORK_DETACH_IND_STRU *)pstMsg;

    /* ����Detach��� */
    enAdditionalAction = NAS_MMC_ProcGmmNetworkDetachInd(pstDetachMsg);

    /* AdditionalAction��Ч������PS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGmmTbfRelInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����Ȼ�����������ӣ������ȴ� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ����������ͷű�����ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    /* ���Abor����Ϊ�����˳������˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort�������ӳ��˳������л�VPLMN���� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ����������ͷ�(����ȴ������ͷų�ʱ),������һ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;

        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }
    else
    {
        /* ��ǰΪ�����ͷţ�����CS��PS��additional Action��һ������ */
        NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGmmServiceRequestResultInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU                 *pstServiceRsltInd = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
#endif

    pstServiceRsltInd = (GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU*)pstMsg;

    /* �������ɹ� */
    if (GMM_MMC_ACTION_RESULT_SUCCESS == pstServiceRsltInd->ulServiceSts)
    {
        /* ����PS��AdditionalAction */
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON);
#if (FEATURE_ON == FEATURE_LTE)
        /* ��LMM����Service requestResult�����Ϣ���� */
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
        stActionRslt.ulAttemptCount = 0;

        NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
#endif
    }
    else
    {
        /* �������ʧ�ܣ�����ͬ�ܾ�ԭ��ֵ������PS additional Action */
        enAdditionalAction = NAS_MMC_ProcPsServiceRegFail(pstServiceRsltInd);

        /* AdditionalAction��Ч������PS����һ���Ķ������� */
        if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
        {
            NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
        }


#if (FEATURE_ON == FEATURE_LTE)
        /* ��LMM����Service requestResult�����Ϣ���� */
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = pstServiceRsltInd->enRegFailCause;
        stActionRslt.ulAttemptCount = 0;

        NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
#endif
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmConnInfoInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����Ȼ�����������ӣ������ȴ� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ����������ͷű�����ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    /* ���Abor����Ϊ�����˳������˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort�������ӳ��˳������л�VPLMN���� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ����������ͷ�(����ȴ������ͷų�ʱ),������һ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }
    else
    {
        /* ��ǰΪ�����ͷţ�����CS��PS��additional Action��һ������ */
        NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmRrRelInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����Ȼ�����������ӣ������ȴ� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ����������ͷű�����ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    /* ���Abor����Ϊ�����˳������˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort�������ӳ��˳������л�VPLMN���� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ����������ͷ�(����ȴ������ͷų�ʱ),������һ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        /* ������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }
    else
    {
        /* ��ǰΪ�����ͷţ�����CS��PS��additional Action��һ������ */
        NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();
    }
    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvCmServiceRejectInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_CM_SERVICE_REJECT_IND_STRU   *pstCmServiceRejMsg = VOS_NULL_PTR;

    pstCmServiceRejMsg = (MMCMM_CM_SERVICE_REJECT_IND_STRU*)pstMsg;

    /* ����ܾ�ԭ��ΪNAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstCmServiceRejMsg->ulCause)
    {
        /* ����CS additional ActionΪ����פ�� */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGmmPsRegResultInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_PS_REG_RESULT_IND_STRU      *pstPsRegRslt = VOS_NULL_PTR;

    pstPsRegRslt = (GMMMMC_PS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstPsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ����PSע���� */
    NAS_MMC_ProcPsRegRslt_BgPlmnSearch(pstPsRegRslt);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmCsRegResultInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_CS_REG_RESULT_IND_STRU       *pstCsRegRslt = VOS_NULL_PTR;

    pstCsRegRslt = (MMMMC_CS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstCsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ����CSע���� */
    NAS_MMC_ProcCsRegRslt_BgPlmnSearch(pstCsRegRslt);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmAbortInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_ABORT_IND_STRU               *pstMmAbortInd   = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU    stActionRslt;
#endif

    pstMmAbortInd = (MMMMC_ABORT_IND_STRU*)pstMsg;

    /* ���ԭ��ֵ��Ϊ#6���Ҳ�Ϊ#17��ֱ�ӷ��� */
    if ((NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME != pstMmAbortInd->enAbortCause)
     && (NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE != pstMmAbortInd->enAbortCause))
    {
        return VOS_TRUE;
    }

    /* ���ԭ��ֵ#6������CS AdditionΪAnycellפ�� */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstMmAbortInd->enAbortCause)
    {
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* ��LMM����ID_MMC_LMM_ACTION_RESULT_REQ */
    /* ����ActionRslt��Ϣ�����ΪCN_Rej,������Ϊ����NAS_MML_PROC_MM_ABORT����
       ����ͽ������CS��ԭ��ֵΪAbort����ԭ��ֵ */
    stActionRslt.enProcType     = NAS_MML_PROC_MM_ABORT;
    stActionRslt.enRegRst       = NAS_MML_REG_RESULT_CN_REJ;
    stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enCnCause      = pstMmAbortInd->enAbortCause;
    stActionRslt.ulAttemptCount = 0x0;

    NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
#endif

    return VOS_TRUE;
}




VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_LAI_STRU                   *pstVplmn;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;
    VOS_UINT32                          ulIsVplmnNetRatValid;

    RRMM_REL_IND_STRU                  *pstRrmmRelIndMsg    = VOS_NULL_PTR;

    pstRrmmRelIndMsg = (RRMM_REL_IND_STRU *)pstMsg;

    pstVplmn             = NAS_MMC_GetPreCampLai_BgPlmnSearch();

    enRatType            = NAS_MML_GetCurrNetRatType();

    /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
       disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

    ulIsVplmnNetRatValid = NAS_MMC_IsVplmnValid();


    if (RRC_REL_CAUSE_NO_RF == pstRrmmRelIndMsg->ulRelCause)
    {
        /* ֹͣ�ȴ����������ͷű�����ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ�����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����Ȼ�����������ӣ������ȴ� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ����������ͷű�����ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    /* ���Abor����Ϊ�����˳������˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort�������ӳ��˳������л�VPLMN���� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* W��P̬���Է��𱳾��ѣ�����ָ����ǰ�����rrc���ӻ�Ǩ���������ͷţ�
           ��ʱ�յ�list���������ӳٴ��ʱ�������vplmn����ֱ���˳�״̬�� */
        if (((VOS_FALSE == NAS_MML_CompareBcchPlmnwithSimPlmn(NAS_MML_GetCurrCampPlmnId(), &pstVplmn->stPlmnId))
           || (enRatType != pstVplmn->enCampPlmnNetRat))
         && (VOS_TRUE == ulIsVplmnNetRatValid))
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ����������ͷ�(����ȴ������ͷų�ʱ),������һ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }


        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }
    else
    {
        /* ��ǰΪ�����ͷţ�����CS��PS��additional Action��һ������ */
        NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSuspendInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_IND_ST                *pstSuspendMsg = VOS_NULL_PTR;
    VOS_UINT32                          ucRelRequestFlg;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;

    ucRelRequestFlg = NAS_MMC_GetRelRequestFlag_BgPlmnSearch();

    /* ��������ͷŻ��ߴ���Abort��־����OOS���������ع���ʧ�� */
    if ((NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
     || (VOS_TRUE == ucRelRequestFlg)
     || (MMC_SUSPEND_CAUSE_PLMNSEARCH == pstSuspendMsg->ucSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= pstSuspendMsg->ucSuspendCause))
    {
        /* ���ع���ʧ�� */
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, pstSuspendMsg->MsgHeader.ulSenderPid);

        return VOS_TRUE;
    }

    /* ������ݹ���ԭ��ֵ��������ͬ��״̬�� */
    NAS_MMC_LoadInterSysFsm_BgPlmnSearch(pstSuspendMsg->ucSuspendCause);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcSuspendRslt_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRsltMsg = VOS_NULL_PTR;

    pstSuspendRsltMsg = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

    /* �����ѡ���Ϊʧ��,ԭ��Ϊ��������,�����������ͷŴ��� */
    if ((NAS_MMC_SUSPEND_FAIL == pstSuspendRsltMsg->enRslt)
     && (NAS_MMC_SUSPEND_FAIL_CAUSE_AREA_LOST == pstSuspendRsltMsg->enFailCause))
    {
        /* ֹͣ�ȴ����������ͷű�����ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

        if (VOS_FALSE == NAS_MML_GetRfAvailFlg())
        {
            /* ���ϲ�״̬�����ͱ�������ʧ�ܣ�����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_TRUE;
        }

        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        /* ֹͣ��ǰ�ȴ�CS/PS�����ͷŵ�״̬ */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

        /* ���µ�ǰ�ȴ�EPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
        NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

#if (FEATURE_ON == FEATURE_IMS)
        /* ���ӵȴ�IMS�Ƿ���ý��FLG,MSCC��֤�յ�PSע�����ϱ�IMS�Ƿ����ָʾ��MMC */
        NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_IMS);
#endif

        /* ����CS��AdditionalAction */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* Ǩ�Ƶ��ȴ�ע������״̬PLMN_SRCH:NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_REG_IND */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_REG_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);

        return VOS_TRUE;
    }
#endif

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    /* ����ѡ֮ǰ�Ѿ��ж�,��ʱ�����ܴ���Abort���������ͷ�,����������Ӳ����ڿ��ܷ������� */
    NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();


    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSysInfoInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ��������������ͷ�,������Ҫ������������,ϵͳ��Ϣ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMSCCģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);

    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);

    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvGrrMmSysInfoInd_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

    /* ��������������ͷ�,������Ҫ������������,ϵͳ��Ϣ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

     pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
     pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

     PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
     PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();


    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMSCCģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTafSysCfgReq_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcSyscfgRsltCnf_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���VPLMN���뼼����ǰ��֧�֣����轫VPLMN���뼼������ΪSYSCFG���õ�������Ƚ��뼼����
       �Ȼ�ȡ��һ������ʱ���ж�vplmn���뼼���Ƿ���Ч��vplmn��Чʱ��Ҫ��״̬�����Ϊ��Ҫ������
       ����פ�����ٽ��б����ѣ��ú����ֲ�����������������չʹ�� */
    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitRrcConnRelExpired_BgPlmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucRelRequestFlg;
    NAS_MMC_ABORT_FSM_TYPE_UINT8        enAbortType;
    VOS_UINT32                          ulCsServiceExist;
    VOS_UINT32                          ulPsServiceExist;

    enAbortType = NAS_MMC_GetAbortType_BgPlmnSearch();

    ucRelRequestFlg = NAS_MMC_GetRelRequestFlag_BgPlmnSearch();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitRrcConnRelExpired_BgPlmnSearch_WaitRrcConnRelInd: ENTERED");

    /* ���Abort����Ϊ������,���˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == enAbortType)
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort����Ϊ�ӳ���,�����Ƿ������ͷŷֱ��� */
    if (NAS_MMC_ABORT_FSM_DELAY == enAbortType)
    {
        if (VOS_TRUE == ucRelRequestFlg)
        {
            /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
               disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

            if (VOS_TRUE == NAS_MMC_IsVplmnValid())
            {
                /* ��MM��GMM��������ָʾ */
                NAS_MMC_SndGmmPlmnSchInit();
                NAS_MMC_SndMmPlmnSchInit();

                NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

                NAS_MMC_ReCampVPlmn_BgPlmnSearch();
                return VOS_TRUE;
            }

            /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }
        else
        {
            /* ���������ͷ�����MM��GMM�����������ȴ������ͷŶ�ʱ�� */
            NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);
            NAS_MMC_SndMmRelReq();
            NAS_MMC_SndGmmRelReq();

            NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
        }

        return VOS_TRUE;
    }

    /* ����������ͷ��Ҳ���ҪAbort��������һ�������ȼ����� */
    if (VOS_TRUE == ucRelRequestFlg)
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }


        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        return VOS_TRUE;
    }

    ulCsServiceExist         = NAS_MML_GetCsServiceExistFlg();
    ulPsServiceExist         = NAS_MML_GetPsServiceExistFlg();

    /* ������������ͷŵ�����ҵ���˳�״̬�� */
    if ((VOS_TRUE == ulCsServiceExist)
     || (VOS_TRUE == ulPsServiceExist))
    {
        /* ע�������ע��ʧ����Ҫ����,�ȴ�RRC����ʱ�������CS/PSҵ��,�˳���Ҫ�ȴ�ҵ����������� */
        if (VOS_TRUE == NAS_MMC_IsAdditionalTriggerPlmnSearch_BgPlmnSearch())
        {
            /* ����״̬�����Ϊʧ��,��Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
        }
        else
        {
            /* W��P̬ʱ�����ͷ�RRC���ӳ�ʱ,����״̬�����Ϊ�ɹ�,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
        }

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }
    else
    {
        /* ���������ͷ�����MM��GMM�����������ȴ������ͷŶ�ʱ�� */
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);
        NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);
        NAS_MMC_SndMmRelReq();
        NAS_MMC_SndGmmRelReq();
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasSuspendCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitWasSuspendCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;

    pstSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* �������ʧ�ܣ���λ���� */
    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitWasSuspendCnfReCampVPlmn: Suspend fail");

        if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
          && (NAS_MML_NET_RAT_TYPE_WCDMA   == enCurRat) )
        {
            NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_WAS_SUSPEND_FAILURE);
        }
        else
        {
            NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_TD_SUSPEND_FAILURE);
        }

        return VOS_TRUE;
    }

    /* ֹͣ�ȴ�����ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF);

    /* ���õ�ǰ���뼼��ΪBUTT */
    NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

    /* ���Abort����Ϊ������,�˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
       disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

    if (VOS_TRUE == NAS_MMC_IsVplmnValid())
    {
        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        NAS_MMC_ReCampVPlmn_BgPlmnSearch();
        return VOS_TRUE;
    }

    /* ���Abort����Ϊ�ӳ���,�˳�״̬��,�������� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* �޴�ϱ��,���ϲ�״̬������FAIL,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();
    return VOS_TRUE;

}


VOS_UINT32    NAS_MMC_RcvTiWaitAsSuspendCnfExpired_BgPlmnSearch_WaitAsSuspendCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitAsSuspendCnfExpired_BgPlmnSearch_WaitAsSuspendCnfReCampVPlmn: ENTERED");

    /* ����ʱ����λ���� */

    if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
      && (NAS_MML_NET_RAT_TYPE_WCDMA   == enCurRat) )
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_WAS_SUSPEND_CNF_EXPIRED);
    }
    else if ( NAS_MML_NET_RAT_TYPE_GSM == enCurRat )
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_GAS_SUSPEND_CNF_EXPIRED);
    }
    else if ( NAS_MML_NET_RAT_TYPE_LTE == enCurRat )
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_LMM_SUSPEND_CNF_EXPIRED);
    }
    else
    {
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_TD_SUSPEND_CNF_EXPIRED);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasSuspendCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitGasSuspendCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf = VOS_NULL_PTR;

    pstSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

    /* �������ʧ�ܣ���λ���� */
    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSuspendCnf_BgPlmnSearch_WaitGasSuspendCnfReCampVPlmn: Suspend fail");

        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_GAS_SUSPEND_FAILURE);

        return VOS_TRUE;
    }

    /* ֹͣ�ȴ�����ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF);

    /* ���õ�ǰ���뼼��ΪBUTT */
    NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

    /* ���Abort����Ϊ������,�˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
       disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

    if (VOS_TRUE == NAS_MMC_IsVplmnValid())
    {
        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        NAS_MMC_ReCampVPlmn_BgPlmnSearch();
        return VOS_TRUE;
    }

    /* ���Abort����Ϊ�ӳ���,�˳�״̬��,�������� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* �޴�ϱ��,���ϲ�״̬������FAIL,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();
    return VOS_TRUE;

}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ���Abort������������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
    {
        /* ֹͣ�ȴ������ظ���ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);

        /* ��ǰ���뼼������ֹͣ�������� */
        NAS_MMC_SndAsPlmnSrchStopReq(WUEPS_PID_WRR);

        /* ״̬Ǩ�Ƶ���VPlmnʱ�ȴ�ֹͣ�����ظ�״̬ */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_PLMN_STOP_CNF_RECAMP_VPLMN);

        /* �����ȴ�ֹͣ�����ظ���ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF_LEN);
    }

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSrchCnf_BgPlmnSearch_WaitWasPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU                              *pstSrchCnfMsg = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList   = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astSearchedPlmnListInfo[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                                              i;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);

    if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);

        return VOS_TRUE;
    }

    /* ��������ɹ� */
    if (RRC_PLMN_SEARCH_RLT_SPEC == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ״̬Ǩ�Ƶ���VPlmnʱ�ȴ�WASϵͳ��Ϣ״̬ */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_SYSINFO_IND_RECAMP_VPLMN);

        /* �����ȴ�ϵͳ��Ϣ��ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_SYS_INFO, TI_NAS_MMC_WAIT_WAS_SYS_INFO_LEN);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        pstPrioRatList  = NAS_MML_GetMsPrioRatList();

        NAS_MMC_InitSearchedPlmnListInfo(astSearchedPlmnListInfo);

        for ( i = 0; i < pstPrioRatList->ucRatNum; i++ )
        {
            if (NAS_MML_NET_RAT_TYPE_WCDMA == pstPrioRatList->aucRatPrio[i])
            {
                stSearchedPlmnListInfo.stSearchedType.ucHistorySearchedFlg  = VOS_TRUE;
                stSearchedPlmnListInfo.stSearchedType.ucPrefBandSearchedFlg = VOS_TRUE;
                stSearchedPlmnListInfo.stSearchedType.ucSpecSearchedFlg     = VOS_TRUE;
                stSearchedPlmnListInfo.stSearchedType.ucReserve             = 0;

                astSearchedPlmnListInfo[i].enRatType    = NAS_MML_NET_RAT_TYPE_WCDMA;

                NAS_MMC_UpdateSearchedPlmnListInfo(&(stSearchedPlmnListInfo.stSearchedType),
                                                   &(pstSrchCnfMsg->PlmnIdList),
                                                   astSearchedPlmnListInfo);

                /* ��ģ��CoverageType�ý�������,�����ݸߵ�������PLMN */
                NAS_MMC_ConvertRrcCoverTypeToNasFormat(pstSrchCnfMsg->enCoverageType, &(astSearchedPlmnListInfo[i].enCoverType));
            }
            else
            {
                astSearchedPlmnListInfo[i].enRatType    = pstPrioRatList->aucRatPrio[i];
                astSearchedPlmnListInfo[i].stSearchedType.ucHistorySearchedFlg  = NAS_MMC_GetHistorySearchedFlag_BgPlmnSearch(pstPrioRatList->aucRatPrio[i]);
            }
        }

        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astSearchedPlmnListInfo);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ж�ԭ��ֵ */

    RRMM_REL_IND_STRU                  *pstRrmmRelIndMsg    = VOS_NULL_PTR;

    pstRrmmRelIndMsg = (RRMM_REL_IND_STRU *)pstMsg;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasPlmnSearchCnfReCampVPlmn: ENTERED");

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);

    if (RRC_REL_CAUSE_NO_RF == pstRrmmRelIndMsg->ulRelCause)
    {
        /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitWasPlmnSearchCnf_BgPlmnSearch_WaitWasPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasPlmnSearchCnf_BgPlmnSearch_WaitWasPlmnSearchCnfReCampVPlmn: ENTERED");

    /* ֪ͨASָֹͣ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndAsPlmnSrchStopReq(WUEPS_PID_WRR);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_PLMN_STOP_CNF_RECAMP_VPLMN);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ���Abort������������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
    {
        /* ֹͣ�ȴ������ظ���ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF);

        /* ��ǰ���뼼������ֹͣ�������� */
        NAS_MMC_SndAsPlmnSrchStopReq(UEPS_PID_GAS);

        /* ״̬Ǩ�Ƶ���VPlmnʱ�ȴ�ֹͣ�����ظ�״̬ */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_PLMN_STOP_CNF_RECAMP_VPLMN);

        /* �����ȴ�ֹͣ�����ظ���ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF_LEN);
    }

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvRrMmPlmnSrchCnf_BgPlmnSearch_WaitGasPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU                              *pstSrchCnfMsg = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList= VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astSearchedPlmnListInfo[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                                              i;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF);

    if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_GSM);

        return VOS_TRUE;
    }

    /* ��������ɹ� */
    if (RRC_PLMN_SEARCH_RLT_SPEC == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ״̬Ǩ�Ƶ���VPlmnʱ�ȴ�GASϵͳ��Ϣ״̬ */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_SYSINFO_IND_RECAMP_VPLMN);

        /* �����ȴ�ϵͳ��Ϣ��ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_SYS_INFO, TI_NAS_MMC_WAIT_GAS_SYS_INFO_LEN);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        pstPrioRatList  = NAS_MML_GetMsPrioRatList();

        NAS_MMC_InitSearchedPlmnListInfo(astSearchedPlmnListInfo);

        for ( i = 0; i < pstPrioRatList->ucRatNum; i++ )
        {
            if (NAS_MML_NET_RAT_TYPE_WCDMA == pstPrioRatList->aucRatPrio[i])
            {
                stSearchedPlmnListInfo.stSearchedType.ucHistorySearchedFlg  = VOS_TRUE;
                stSearchedPlmnListInfo.stSearchedType.ucPrefBandSearchedFlg = VOS_TRUE;
                stSearchedPlmnListInfo.stSearchedType.ucSpecSearchedFlg     = VOS_TRUE;
                stSearchedPlmnListInfo.stSearchedType.ucReserve             = 0;

                astSearchedPlmnListInfo[i].enRatType    = NAS_MML_NET_RAT_TYPE_WCDMA;

                NAS_MMC_UpdateSearchedPlmnListInfo(&(stSearchedPlmnListInfo.stSearchedType),
                                                   &(pstSrchCnfMsg->PlmnIdList),
                                                   astSearchedPlmnListInfo);

                /* ��ģ��CoverageType�ý�������,�����ݸߵ�������PLMN */
                NAS_MMC_ConvertRrcCoverTypeToNasFormat(pstSrchCnfMsg->enCoverageType, &(astSearchedPlmnListInfo[i].enCoverType));
            }
            else
            {
                astSearchedPlmnListInfo[i].enRatType    = pstPrioRatList->aucRatPrio[i];
                astSearchedPlmnListInfo[i].stSearchedType.ucHistorySearchedFlg  = NAS_MMC_GetHistorySearchedFlag_BgPlmnSearch(pstPrioRatList->aucRatPrio[i]);
            }
        }

        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astSearchedPlmnListInfo);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvTiWaitGasPlmnSearchCnf_BgPlmnSearch_WaitGasPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasPlmnSearchCnf_BgPlmnSearch_WaitGasPlmnSearchCnfReCampVPlmn: ENTERED");

    /* ֪ͨASָֹͣ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndAsPlmnSrchStopReq(UEPS_PID_GAS);
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_PLMN_STOP_CNF_RECAMP_VPLMN);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ж�ԭ��ֵ */

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_BgPlmnSearch_WaitWasPlmnStopCnfReCampVPlmn: ENTERED");

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSrchStopCnf_BgPlmnSearch_WaitWasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSearchCnf_BgPlmnSearch_WaitWasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU          *pstSrchCnfMsg = VOS_NULL_PTR;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF);

    if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitWasStopCnfExpired_BgPlmnSearch_WaitWasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasStopCnfExpired_BgPlmnSearch_WaitWasPlmnStopCnfReCampVPlmn: ENTERED");

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSearchStopCnf_BgPlmnSearch_WaitGasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmPlmnSearchCnf_BgPlmnSearch_WaitGasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU          *pstSrchCnfMsg = VOS_NULL_PTR;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF);

    /* ������no rfʧ�ܣ�����״̬�� */
    if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_GSM);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitGasStopCnfExpired_BgPlmnSearch_WaitGasPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasStopCnfExpired_BgPlmnSearch_WaitGasPlmnStopCnfReCampVPlmn: ENTERED");

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitWasSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmSysInfoInd_BgPlmnSearch_WaitWasSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MMC_ABORT_FSM_TYPE_UINT8                            enAbortType;
    NAS_MMC_BG_PLMN_SEARCH_RSLT_ENUM_UINT32                 enSearchRslt;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ��ȡBG PLMN SEARCH״̬���������е�ABORT��־ */
    enAbortType = NAS_MMC_GetAbortType_BgPlmnSearch();

    /* ֹͣ�ȴ�ϵͳ��Ϣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    /* ����CSPS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* ���Abort����Ϊ������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == enAbortType)
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort����Ϊ�ӳ��� */
    if (NAS_MMC_ABORT_FSM_DELAY == enAbortType)
    {
        enSearchRslt = NAS_MMC_BG_PLMN_SEARCH_ABORTED;
    }
    else
    {
        enSearchRslt = NAS_MMC_BG_PLMN_SEARCH_SUCC;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);
    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMSCCģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    /* ����״̬�����,����Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(enSearchRslt, VOS_FALSE, VOS_NULL_PTR);

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvTiWaitWasSysInfoExpired_BgPlmnSearch_WaitWasSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasSysInfoExpired_BgPlmnSearch_WaitWasSysInfoIndReCampVPlmn: ENTERED");

    /* �����ҪAbort */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitGasSysInfoReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvGrrMmSysInfoInd_BgPlmnSearch_WaitGasSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MMC_ABORT_FSM_TYPE_UINT8                            enAbortType;
    NAS_MMC_BG_PLMN_SEARCH_RSLT_ENUM_UINT32                 enSearchRslt;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    PS_MEM_SET(&stOldCsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_SET(&stOldPsRestrictInfo, 0, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ��ȡBG PLMN SEARCH״̬���������е�ABORT��־ */
    enAbortType = NAS_MMC_GetAbortType_BgPlmnSearch();

    /* ֹͣ�ȴ�ϵͳ��Ϣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* ����CSPS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* ���Abort����Ϊ������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == enAbortType)
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort����Ϊ�ӳ��� */
    if (NAS_MMC_ABORT_FSM_DELAY == enAbortType)
    {
        enSearchRslt = NAS_MMC_BG_PLMN_SEARCH_ABORTED;
    }
    else
    {
        enSearchRslt = NAS_MMC_BG_PLMN_SEARCH_SUCC;
    }


    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

     pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
     pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

     PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
     PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMSCCģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {

        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_CS, pstCurCsRestrictInfo);

    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMsccAcInfoChangeInd(NAS_MSCC_PIF_SRVDOMAIN_PS, pstCurPsRestrictInfo);

    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    /* ����״̬�����,����Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(enSearchRslt, VOS_FALSE, VOS_NULL_PTR);

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32    NAS_MMC_RcvTiWaitGasSysInfoExpired_BgPlmnSearch_WaitGasSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasSysInfoExpired_BgPlmnSearch_WaitGasSysInfoIndReCampVPlmn: ENTERED");

    /* �����ҪAbort */
    if ( NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch() )
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}




VOS_VOID NAS_MMC_ProcPsRegRslt_BgPlmnSearch(
    GMMMMC_PS_REG_RESULT_IND_STRU      *pstPsRegRsltInd
)
{
    NAS_MML_LAI_STRU                                       *pstLai         = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
    NAS_MML_PROC_TYPE_ENUM_U32                              enProcType;

    /* ת�����͸�L��proctype */
    enProcType = NAS_MML_PROC_BUTT;

    NAS_MMC_ConverGmmActionTypeToMml(pstPsRegRsltInd->enGmmActionType, &enProcType);


	NAS_MMC_ConvertGmmRegDomainToMmcDomain(pstPsRegRsltInd->enReqDomain, &stActionRslt.enReqDomain);

	NAS_MMC_ConvertGmmRegDomainToMmcDomain(pstPsRegRsltInd->enRsltDomain, &stActionRslt.enRstDomain);



#endif

    pstLai              = NAS_MML_GetCurrCampLai();

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));



    if (GMM_MMC_ACTION_RESULT_SUCCESS == pstPsRegRsltInd->enActionResult)
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        /* ����ע������Ϣ */
        NAS_MMC_SaveRegRsltCtx(GMM_MMC_PS_REG_RESULT_IND, pstPsRegRsltInd);

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_PS, NAS_MML_REG_FAIL_CAUSE_NULL);
        NAS_MMC_ClearHplmnRejDomainInfo(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_PS);

        if (VOS_TRUE == NAS_MML_GetKeepCsForbInfoCfg())
        {
            /* ֻɾ��Forb GPRS PLMN��Ϣ��CS���FORB��Ϣ��Ȼ���� */
            NAS_MML_DelForbGprsPlmn(&(pstLai->stPlmnId));
        }
        else
        {
            /* ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbLa,ForbGprs����Ϣ */
            NAS_MMC_DelForbInfo_GuRegRsltSucc(NAS_MMC_REG_DOMAIN_PS);
        }
#if (FEATURE_ON == FEATURE_CSG)
        /* פ����PLMN1 CSG ID1��С���Ϻ�����ѡ����������CSGС��ע��ɹ�ʱ�����duplicate RPLMN��EPLMN��Ϣ��*/
        if (VOS_TRUE == NAS_MMC_IsNeedInitCsgSpecSrchDuplicateRplmnAndEplmnInfo())
        {
            NAS_MMC_InitCsgSpecSrchDuplicateRplmnAndEplmnInfo();
        }
#endif

        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                             NAS_MML_GetSimPsRegStatus());


        NAS_MMC_UpdatePsRegStatePsRegSucc();

        NAS_MMC_ChangeServiceState(NAS_MSCC_PIF_SRVDOMAIN_PS, NAS_MMC_NORMAL_SERVICE);

#if   (FEATURE_ON == FEATURE_LTE)

        /* �����̲�֪ͨ��LMM */
        if ( VOS_TRUE == NAS_MML_IsRegFailCauseNotifyLmm(pstPsRegRsltInd->enRegFailCause) )
        {
            stActionRslt.enProcType     = enProcType;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
            stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
            stActionRslt.ulAttemptCount = 0;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }
#endif

        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_PS, VOS_TRUE, pstPsRegRsltInd->enRegFailCause);

    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcPsRegFail(GMM_MMC_PS_REG_RESULT_IND,
                                                   pstPsRegRsltInd);

        if ( VOS_TRUE == NAS_MML_IsNetworkRegFailCause(pstPsRegRsltInd->enRegFailCause) )
        {
            /* ����ע��������ע����Ϣ�� */
            NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_PS, pstPsRegRsltInd->enRegFailCause);
        }
#if   (FEATURE_ON == FEATURE_LTE)

        /* С�� NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE�ľܾ�ԭ����UE���Է���ע���ԭ��ֵ����Ҫ֪ͨLMM */
        if ( VOS_TRUE == NAS_MML_IsRegFailCauseNotifyLmm(pstPsRegRsltInd->enRegFailCause) )
        {
            stActionRslt.enProcType     = enProcType;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
            stActionRslt.enCnCause      = pstPsRegRsltInd->enRegFailCause;
            stActionRslt.ulAttemptCount = pstPsRegRsltInd->ulRegCounter;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }
#endif
    }

    /* AdditionalAction��Ч������PS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));

        NAS_MMC_BuildEquPlmnInfo(&stSndEquPlmnInfo);

        NAS_MMC_SndGuAsEquPlmnReq(&stSndEquPlmnInfo, NAS_MML_GetCurrNetRatType());
        NAS_MMC_SndOmEquPlmn();

        NAS_MMC_SndMsccEplmnInfoInd(&stSndEquPlmnInfo);

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}


VOS_VOID NAS_MMC_ProcCsRegRslt_BgPlmnSearch(
    MMMMC_CS_REG_RESULT_IND_STRU       *pstCsRegRsltInd
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
#endif

    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU                      *pstDPlmnNPlmnCfgInfo = VOS_NULL_PTR;
    pstDPlmnNPlmnCfgInfo  = NAS_MMC_GetDPlmnNPlmnCfgInfo();



    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));

    if (MM_MMC_LU_RESULT_SUCCESS == pstCsRegRsltInd->enLuResult)
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, NAS_MML_REG_FAIL_CAUSE_NULL);

        /* ����DPLMN NPLMN�б� */
        if (VOS_TRUE == NAS_MMC_IsRoam())
        {
            NAS_MMC_UpdateDPlmnNPlmnList(NAS_MML_GetCurrCampLai(), NAS_MMC_REG_DOMAIN_CS, NAS_MMC_MAX_CFG_DPLMN_NUM, &pstDPlmnNPlmnCfgInfo->usDplmnListNum, pstDPlmnNPlmnCfgInfo->astDPlmnList);
            NAS_MMC_DeleteDPlmnNPlmnList(NAS_MML_GetCurrCampLai(), NAS_MMC_REG_DOMAIN_CS, &pstDPlmnNPlmnCfgInfo->usNplmnListNum, pstDPlmnNPlmnCfgInfo->astNPlmnList);

            NAS_MMC_WriteSelfLearnDplmnNplmnToNvim(pstDPlmnNPlmnCfgInfo);

            NAS_MMC_LogDplmnNplmnList();
        }

        /* ��Hplmn��Rej Lai��Ϣ����� */
        NAS_MMC_ClearHplmnRejDomainInfo(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS);

        /* ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbLa,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_GuRegRsltSucc(NAS_MMC_REG_DOMAIN_CS);


#if (FEATURE_ON == FEATURE_CSG)
        /* פ����PLMN1 CSG ID1��С���Ϻ�����ѡ����������CSGС��ע��ɹ�ʱ�����duplicate RPLMN��EPLMN��Ϣ��*/
        if (VOS_TRUE == NAS_MMC_IsNeedInitCsgSpecSrchDuplicateRplmnAndEplmnInfo())
        {
            NAS_MMC_InitCsgSpecSrchDuplicateRplmnAndEplmnInfo();
        }
#endif

        NAS_MMC_UpdateCsRegStateCsRegSucc();

        NAS_MMC_ChangeServiceState(NAS_MSCC_PIF_SRVDOMAIN_CS, NAS_MMC_NORMAL_SERVICE);

#if   (FEATURE_ON == FEATURE_LTE)
        /* ������ע��ʱ����Ϊ�����̲�֪ͨLMM��ʱLU�Ľ�� */
        if ((VOS_TRUE  == NAS_MML_IsRegFailCauseNotifyLmm(pstCsRegRsltInd->enRegFailCause))
         && (VOS_FALSE == pstCsRegRsltInd->ucIsComBined))
        {
            stActionRslt.enProcType     = NAS_MML_PROC_LAU;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
            stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
            stActionRslt.ulAttemptCount = 0;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }
#endif

        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_CS, VOS_TRUE, pstCsRegRsltInd->enRegFailCause);

    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcCsRegFail(MM_MMC_CS_REG_RESULT_IND,
                                                   pstCsRegRsltInd);

        if ( VOS_TRUE == NAS_MML_IsNetworkRegFailCause(pstCsRegRsltInd->enRegFailCause) )
        {
            /* ����ע��������ע����Ϣ�� */
            NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, pstCsRegRsltInd->enRegFailCause);

            /* ����DPLMN NPLMN�б� */
            if (VOS_TRUE == NAS_MMC_IsRoam())
            {
                NAS_MMC_UpdateDPlmnNPlmnList(NAS_MML_GetCurrCampLai(), NAS_MMC_REG_DOMAIN_CS, NAS_MMC_MAX_CFG_NPLMN_NUM, &pstDPlmnNPlmnCfgInfo->usNplmnListNum,pstDPlmnNPlmnCfgInfo->astNPlmnList);
                NAS_MMC_DeleteDPlmnNPlmnList(NAS_MML_GetCurrCampLai(), NAS_MMC_REG_DOMAIN_CS, &pstDPlmnNPlmnCfgInfo->usDplmnListNum, pstDPlmnNPlmnCfgInfo->astDPlmnList);
                NAS_MMC_WriteSelfLearnDplmnNplmnToNvim(pstDPlmnNPlmnCfgInfo);

                NAS_MMC_LogDplmnNplmnList();
            }
        }
#if   (FEATURE_ON == FEATURE_LTE)
        /* С�� NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE�ľܾ�ԭ����UE���Է���ע���ԭ��ֵ����Ҫ֪ͨLMM */
        /* ����ע��ʱ��֪ͨLMM��ʱLU�Ľ�� */
        if ((VOS_TRUE  == NAS_MML_IsRegFailCauseNotifyLmm(pstCsRegRsltInd->enRegFailCause))
         && (VOS_FALSE == pstCsRegRsltInd->ucIsComBined))
        {
            stActionRslt.enProcType     = NAS_MML_PROC_LAU;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
            stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enCnCause      = pstCsRegRsltInd->enRegFailCause;
            stActionRslt.ulAttemptCount = pstCsRegRsltInd->ulLuAttemptCnt;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }
#endif
    }


    /* AdditionalAction��Ч������CS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndGuAsEquPlmnReq(&stSndEquPlmnInfo, NAS_MML_GetCurrNetRatType());
        NAS_MMC_SndOmEquPlmn();

        NAS_MMC_SndMsccEplmnInfoInd(&stSndEquPlmnInfo);

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}


VOS_VOID NAS_MMC_SndSpecSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU                stForbRoamLaInfo;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbPlmnInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                         stDestPlmnList;
    VOS_UINT32                                              ulWaitWasPlmnSrchCnfTimerLen;

    ulWaitWasPlmnSrchCnfTimerLen = NAS_MMC_GetWaitWasPlmnSrchCnfTimerLen();

    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();
    stForbRoamLaInfo.ucForbRoamLaNum = pstForbPlmnInfo->ucForbRoamLaNum;
    PS_MEM_CPY( stForbRoamLaInfo.astForbRomLaList,
                pstForbPlmnInfo->astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList) );
    NAS_MML_AddCustomizedForbLaToDestLaList(&(stForbRoamLaInfo.ucForbRoamLaNum),
                                                   stForbRoamLaInfo.astForbRomLaList);

    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    PS_MEM_SET(&stDestPlmnList, 0x0, sizeof(stDestPlmnList));

    stDestPlmnList.ulPlmnNum = 1;
    PS_MEM_CPY(&stDestPlmnList.astPlmnId[0], pstDestPlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ���ݲ�ͬ�Ľ��뼼�������������� */
    switch (pstDestPlmn->enRat)
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsSpecPlmnSearchReq(&stDestPlmnList, &stForbRoamLaInfo);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_PLMN_SEARCH_CNF_RECAMP_VPLMN);


            /* ��ǰΪ�ػ����͵Ĵ�ϣ�Ϊ�˿�����Ӧ�ػ����������Ȼظ��Ƚ϶̵Ķ�ʱ�� */
            if ( NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_POWER_OFF_REQ) ==
                                NAS_MMC_GetAbortEventType_BgPlmnSearch() )
            {
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF,
                                   TI_NAS_MMC_FAST_RECAMP_VPLMN_WAIT_WAS_PLMN_SEARCH_CNF_LEN);
            }
            else
            {
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF,
                                   ulWaitWasPlmnSrchCnfTimerLen);
            }

            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsSpecPlmnSearchReq(&stDestPlmnList, &stForbRoamLaInfo);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_PLMN_SEARCH_CNF_RECAMP_VPLMN);


            /* ��ǰΪ�ػ����͵Ĵ�ϣ�Ϊ�˿�����Ӧ�ػ����������Ȼظ��Ƚ϶̵Ķ�ʱ�� */
            if ( NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_POWER_OFF_REQ) ==
                                NAS_MMC_GetAbortEventType_BgPlmnSearch() )
            {
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF,
                                   TI_NAS_MMC_FAST_RECAMP_VPLMN_WAIT_GAS_PLMN_SEARCH_CNF_LEN);
            }
            else
            {
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF,
                                   NAS_MMC_GetWaitGasPlmnSearchCnfTimerLen(VOS_FALSE));
            }

            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmSpecPlmnSearchReq(&stDestPlmnList, VOS_FALSE, NAS_MMC_PLMN_SEARCH_SCENE_BUTT);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_PLMN_SEARCH_CNF_RECAMP_VPLMN);

            /* ��ǰ���ڹػ����͵Ĵ�ϣ�Ϊ�˿�����Ӧ�ػ����������Ȼظ��Ƚ϶̵Ķ�ʱ�� */
            if ( NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_POWER_OFF_REQ) ==
                                NAS_MMC_GetAbortEventType_BgPlmnSearch() )
            {
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF,
                                   TI_NAS_MMC_FAST_RECAMP_VPLMN_WAIT_LMM_PLMN_SEARCH_CNF_LEN);
            }
            else
            {
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF,
                                   TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF_LEN);
            }

            break;
#endif

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSpecSearchReq_BgPlmnSearch: Rat not support.");
            break;
    }


    return;
}


VOS_UINT32 NAS_MMC_IsPreCampPlmnValidInBgHistoryScene_BgPlmnSearch(VOS_VOID)
{
    NAS_MML_LAI_STRU                                       *pstPreCampLai = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
#endif

    pstPreCampLai        = NAS_MMC_GetPreCampLai_BgPlmnSearch();

     /* ���VPLMN���뼼����ǰ��֧��,����vplmn���뼼����Ч */
    if (VOS_FALSE == NAS_MML_IsNetRatSupported(pstPreCampLai->enCampPlmnNetRat))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsPreCampPlmnValidInBgHistoryScene_BgPlmnSearch: PreCamp Rat is not support");

        return VOS_FALSE;
    }


#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE  == pstPreCampLai->enCampPlmnNetRat)
    {
        /* ��ǰLTE������״̬Ϊdisable */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus)
          || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
        {
            if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsPreCampPlmnValidInBgHistoryScene_BgPlmnSearch: Disable LTE and PS Attach not allowed");

                return VOS_FALSE;
            }
        }
    }

#endif

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_IsVplmnValid(VOS_VOID)
{
    NAS_MML_LAI_STRU                                       *pstPreCampLai = VOS_NULL_PTR;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT8                                               ucSimPsRegStatus;
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
    ucSimPsRegStatus      = NAS_MML_GetSimPsRegStatus();
#endif

    pstPreCampLai        = NAS_MMC_GetPreCampLai_BgPlmnSearch();

    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        /* bg history�������������ԭ����PLMN��FPLMN����������פ�� */
        return NAS_MMC_IsPreCampPlmnValidInBgHistoryScene_BgPlmnSearch();
    }


     /* ���VPLMN���뼼����ǰ��֧��,����vplmn���뼼����Ч */
    if (VOS_FALSE == NAS_MML_IsNetRatSupported(pstPreCampLai->enCampPlmnNetRat))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsVplmnValid: PreCamp Rat is not support");
        return VOS_FALSE;
    }

    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    /* ��ֹ���磬��forbid plmn�б��� */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(&(pstPreCampLai->stPlmnId),
                                          pstForbidPlmnInfo->ucForbPlmnNum,
                                          pstForbidPlmnInfo->astForbPlmnIdList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsVplmnValid: PreCamp PLMN is Forbidden PLMN");
        return VOS_FALSE;
    }

#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE  == pstPreCampLai->enCampPlmnNetRat)
    {
        /* ��ǰLTE������״̬Ϊdisable */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus)
          || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
        {
            if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
            {
                NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsVplmnValid: Disable LTE and PS Attach not allowed");
                return VOS_FALSE;
            }
        }

        /* ��forbid plmn for gprs�б��� */
        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(&(pstPreCampLai->stPlmnId),
                                          pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                          pstForbidPlmnInfo->astForbGprsPlmnList))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsVplmnValid: PreCamp PLMN is Forbidden PLMN for GPRS");
            return VOS_FALSE;
        }

        /* PS����Ч */
        if (VOS_FALSE == ucSimPsRegStatus)
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsVplmnValid: Ps Reg Status is false");
            return VOS_FALSE;
        }
    }

#endif

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_ReCampVPlmn_BgPlmnSearch()
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enNetRatType;
    NAS_MML_PLMN_WITH_RAT_STRU          stPreCampPlmn;

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32              ulDisablteRoamFlg;

    NAS_MML_RAT_CAPABILITY_STATUS_ENUM_UINT8 enLteForbiddenStatus;

    enLteForbiddenStatus = NAS_MML_GetLteForbiddenStatusFlg();

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();
#endif

    enNetRatType = NAS_MML_GetCurrNetRatType();

    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPreCampPlmn);

    /* ���Vplmn��LTE����,�������������ȼ���LTE����ʱ,�ڸ����ȼ���LTE������Disable Lte,
       �ٻ�Vplmnʱ����Ҫ��Enable Lte */
#if (FEATURE_ON == FEATURE_LTE)
    if ((NAS_MML_NET_RAT_TYPE_LTE == stPreCampPlmn.enRat)
     && (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE != enLteForbiddenStatus))
    {
        /* Enable Lte */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            NAS_MML_EnableLteCommProc();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    if (VOS_FALSE == NAS_MMC_IsCurrLtePlmnAllowSrchInCLMode(&(stPreCampPlmn.stPlmnId)))
    {
        NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_ReCampVPlmn_BgPlmnSearch:WARNING: Current Lte Plmn is not Allowed Srch In CL Mode,ulMcc and ulMnc are",
                                             stPreCampPlmn.stPlmnId.ulMcc, stPreCampPlmn.stPlmnId.ulMnc);

        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return;
    }
#endif
    /* �����Ҫ�����������뵱ǰ���뼼����ͬ,���ߵ�ǰ���뼼��ΪButt,��Ӧ�ĳ�����
       �����ظ�SUSPNED CNF��Ὣ��ǰ����ģʽ����ΪButt,��ʾ�������κν��뼼��ֱ�ӷ������� */
    if ((enNetRatType == stPreCampPlmn.enRat)
     || (NAS_MML_NET_RAT_TYPE_BUTT == enNetRatType))
    {
        NAS_MML_SetCurrNetRatType(stPreCampPlmn.enRat);

        /* ����ָ������,��������ʱ����Ǩ��״̬ */
        NAS_MMC_SndSpecSearchReq_BgPlmnSearch(&stPreCampPlmn);
    }
    else
    {
        /* ���͹�������,��������ʱ����Ǩ��״̬ */
        NAS_MMC_SndSuspendReqReCampVPlmn_BgPlmnSearch(enNetRatType);
    }

    return;
}



VOS_UINT32 NAS_MMC_IsAdditionalTriggerPlmnSearch_BgPlmnSearch()
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    /* ���Ӳ���Ϊפ����ǰ���������ȴ�ע����ʱ����Ҫ���� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON   == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION   == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON  == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED    == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_BUTT             == enAdditionalAction))
    {
        return VOS_FALSE;
    }

    /* ���������Ҫ���� */
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch()
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;

    VOS_UINT32                                              ulIsVplmnNetRatValid;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
       disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

    ulIsVplmnNetRatValid     = NAS_MMC_IsVplmnValid();


    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ���vplmn����syscfg���û�L disable�����Ч�������ȡ��һ�������ȼ����磬
       ֱ���˳�״̬������������פ�����ٸ��������ж��Ƿ���б�����*/
    if (VOS_FALSE == ulIsVplmnNetRatValid)
    {
        return VOS_FALSE;
    }

    /* �����ȡ�����ȼ�����ʧ�ܻ�VPLMN */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        NAS_MMC_ReCampVPlmn_BgPlmnSearch();
        return VOS_TRUE;
    }

    /* ����״̬Ϊδ������VPLMN����BG�� */
    if (NAS_MMC_NET_STATUS_NO_SEARCHED == stHighPrioPlmnInfo.astPlmnRatInfo[0].enNetStatus)
    {
        NAS_MMC_ReCampVPlmn_BgPlmnSearch();
        return VOS_TRUE;
    }
    else
    {

        /* �Ƶ�������������֮ǰ���� */

        /* ���п���ָ������ */
        NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                                   stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);
    }

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_PerformAbortRegFinished_BgPlmnSearch()
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    NAS_MMC_UpdateRegFinishedSelectionList_BgPlmnSearch();

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    /* ����L��GU��ע�������������ô˺�������������ʱ����ͣһ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

    /* �������Ҫ��VPLMN��ֱ���˳�״̬�� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON        == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION        == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON       == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED         == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL          == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL == enAdditionalAction))
    {
        /* ���浱ǰפ�����缰��EPLMN��״̬�������� */
        NAS_MMC_SetPreCampLai_BgPlmnSearch( NAS_MML_GetCurrCampLai() );
        NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch( NAS_MML_GetCurrCampPlmnId(), NAS_MML_GetEquPlmnList() );

        /* ���浱ǰ�ķ���״̬ */
        NAS_MMC_SetPreServiceInfo_BgPlmnSearch(NAS_MMC_GetServiceInfo());

        /* ����״̬���˳��ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* ����������Ӵ��� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        /* ��ȡ���� */
        NAS_MMC_PerformRrcRelRegFinished_BgPlmnSearch();
    }
    else
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return;
    }

    return;
}


VOS_VOID NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch()
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    /* ע������󣬸��±��������б� */
    NAS_MMC_UpdateRegFinishedSelectionList_BgPlmnSearch();

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
        return;
    }

    /* ����L��GU��ע�������������ô˺�������������ʱ����ͣһ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED == enAdditionalAction))
    {
        /* ע����ɺ�,����Ҫ��������ʱ,���浱ǰפ������͵�Ч���絽״̬�������� */
        NAS_MMC_SetPreCampLai_BgPlmnSearch(NAS_MML_GetCurrCampLai());

        NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(NAS_MML_GetCurrCampPlmnId(), NAS_MML_GetEquPlmnList());

        /* ���浱ǰ�ķ���״̬ */
        NAS_MMC_SetPreServiceInfo_BgPlmnSearch(NAS_MMC_GetServiceInfo());

        /* ����״̬���˳��ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }


    /* bg history ��Ҫ�����Ĵ��� */
    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        NAS_MMC_PerformAdditonActionRegFinishedInBgHistoryScene_BgPlmnSearch(enCsAdditionalAction, enPsAdditionalAction);

        return;
    }

    /* bg spec ��Ҫ�����Ĵ��� */
    NAS_MMC_PerformAdditonActionRegFinishedInBgSpecScene_BgPlmnSearch(enCsAdditionalAction, enPsAdditionalAction);

    return;
}


VOS_VOID NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch()
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enPsAdditionalAction;

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
#if (FEATURE_ON == FEATURE_LTE)
        if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_REG_IND);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);
        }
        else
#endif
        {
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_CSPS_REG_IND);

            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);
        }
        return;
    }

    /* ע����̽�����ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED == enAdditionalAction))
    {
        /* ע����ɺ�,����Ҫ��������ʱ,���浱ǰפ������͵�Ч���絽״̬�������� */
        NAS_MMC_SetPreCampLai_BgPlmnSearch(NAS_MML_GetCurrCampLai());

        NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(NAS_MML_GetCurrCampPlmnId(), NAS_MML_GetEquPlmnList());

        /* ���浱ǰ�ķ���״̬ */
        NAS_MMC_SetPreServiceInfo_BgPlmnSearch(NAS_MMC_GetServiceInfo());

        /* ����״̬���˳��ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    if ((NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL == enAdditionalAction))
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ��,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* bg history�ѳ��� */
    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {

        NAS_MMC_PerformAdditonActionConnRelInBgHistoryScene_BgPlmnSearch(enCsAdditionalAction, enPsAdditionalAction);

        return;
    }


    /* bg spec�ѳ��� */
    if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();


    NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

    return;
}


VOS_UINT32 NAS_MMC_IsBgSearchFinished_BgPlmnSearch(VOS_VOID)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

#if (FEATURE_ON == FEATURE_CSG)
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;

    pstCsgIdSelectionListInfo = NAS_MMC_GetCsgIdBgSearchList();
#endif

    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

#if (FEATURE_ON == FEATURE_CSG)
    /* �����ǰ��CSG���������̣���ֱ�ӻ�ȡ��һ��CSG ID */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        /* �����ȡCSG IDʧ�ܱ���BG��������� */
        if (VOS_FALSE == NAS_MMC_GetNextSearchPlmnAndCsgId(pstCsgIdSelectionListInfo,
                                                           &stCsgIdInfo,
                                                           &stPlmnWithRat,
                                                           NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
        {
            return VOS_TRUE;
        }

        return VOS_FALSE;
    }
#endif


    /* �����ȡ��һ�������ȼ�����ʧ�ܱ���BG��������� */
    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_PerformAdditonActionRegFinishedInBgHistoryScene_BgPlmnSearch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_SERVICE_INFO_CTX_STRU                          *pstPreSrvInfo            = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    VOS_UINT32                                              ulIsVplmnNetRatValid;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ���vplmn����syscfg���û�L disable�����Ч�������ȡ��һ�������ȼ����磬
     ֱ���˳�״̬������������פ�����ٸ��������ж��Ƿ���б�����*/
    ulIsVplmnNetRatValid     = NAS_MMC_IsVplmnValid();

    /***********************************ע��ʧ�ܴ���anycell��***********************************/

    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    /* ע��ʧ�ܴ���anycell�ѣ�ֱ����״̬�� */
    if ((NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL == enAdditionalAction))
    {
        if (VOS_TRUE == NAS_MML_IsRrcConnExist())
        {
            NAS_MMC_PerformRrcRelRegFinished_BgPlmnSearch();

            return;
        }

        /* �������ڣ����ϲ�״̬��ָʾѡ��ʧ��,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /*********************************** ��ȡ�����ȼ�����ʧ��***********************************/
    pstPreSrvInfo   = NAS_MMC_GetPreServiceInfo_BgPlmnSearch();

    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        if (VOS_TRUE == NAS_MML_IsRrcConnExist())
        {
            NAS_MMC_PerformRrcRelRegFinished_BgPlmnSearch();

            return;
        }

        /* VPLMN���ṩ�������񣬻�VPLMN����ӦCS PS mode 1��LTE����ɹ�����GU���磬����פ��LTE����Ҫ��������GU�ĳ��� */
        if ((NAS_MMC_NORMAL_SERVICE == pstPreSrvInfo->enCsCurrService)
         || (NAS_MMC_NORMAL_SERVICE == pstPreSrvInfo->enPsCurrService))
        {
            /* VPLMN��Чֱ����״̬�� */
            if (VOS_FALSE == ulIsVplmnNetRatValid)
            {
                /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

                /* �˳�״̬�� */
                NAS_MMC_FSM_QuitFsmL2();

                return;
            }

            /* VPLMN��Ч����VPLMN */
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();

            return;
        }

        /* VPLMN�����ṩ��������ʱ������פ����ע��ʧ�ܵ�������:BG��ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /*********************************** ��ȡ�����ȼ�����ɹ�������״̬Ϊδ����***********************************/

    /* ����״̬Ϊδ����:��ע��ʧ�ܵ������ϼ���BG�� */

    if (NAS_MMC_NET_STATUS_NO_SEARCHED == stHighPrioPlmnInfo.astPlmnRatInfo[0].enNetStatus)
    {
        if (VOS_TRUE == NAS_MMC_IsNeedWaitRrcConnRelPerformBgPlmnSearch())
        {
            NAS_MMC_PerformRrcRelRegFinished_BgPlmnSearch();

            return;
        }

        /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
        NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                                stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);
        return;
    }

    /*********************************** ��ȡ�����ȼ�����ɹ�������״̬Ϊ����***********************************/
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_PerformRrcRelRegFinished_BgPlmnSearch();

        return;
    }

    /* ����״̬Ϊ����:��������ָ���� */

    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                             NAS_MML_GetSimPsRegStatus());

    /* ���п���ָ������ */
    NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                               stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);

    return;
}


VOS_VOID NAS_MMC_PerformAdditonActionRegFinishedInBgSpecScene_BgPlmnSearch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    /* �������Ӵ��ڣ��ҵ�ǰ��Ҫ��������Ǩ�Ƶ��ȴ������ͷŵ�״̬ */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_PerformRrcRelRegFinished_BgPlmnSearch();

        return;
    }

    if ((NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL == enAdditionalAction))
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ��,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return;

    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

    return;

}


VOS_VOID NAS_MMC_PerformAdditonActionConnRelInBgHistoryScene_BgPlmnSearch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_SERVICE_INFO_CTX_STRU                          *pstPreSrvInfo            = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    VOS_UINT32                                              ulIsVplmnNetRatValid;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    pstPlmnSelectionListInfo    = NAS_MMC_GetHighPrioPlmnList();

    /*********************************** ��ȡ�����ȼ�����ʧ��***********************************/

    /* ���vplmn����syscfg���û�L disable�����Ч�������ȡ��һ�������ȼ����磬
     ֱ���˳�״̬������������פ�����ٸ��������ж��Ƿ���б�����*/
    ulIsVplmnNetRatValid     = NAS_MMC_IsVplmnValid();

    pstPreSrvInfo   = NAS_MMC_GetPreServiceInfo_BgPlmnSearch();

    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                                   &stHighPrioPlmnInfo,
                                                                   &stPlmnWithRat,
                                                                   NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {

        /* VPLMN���ṩ�������񣬻�VPLMN����ӦCS PS mode 1��LTE����ɹ�����GU���磬����פ��LTE����Ҫ��������GU�ĳ��� */
        if ((NAS_MMC_NORMAL_SERVICE == pstPreSrvInfo->enCsCurrService)
         || (NAS_MMC_NORMAL_SERVICE == pstPreSrvInfo->enPsCurrService))
        {
            /* VPLMN��Чֱ����״̬�� */
            if (VOS_FALSE == ulIsVplmnNetRatValid)
            {
                /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

                /* �˳�״̬�� */
                NAS_MMC_FSM_QuitFsmL2();

                return;
            }

            /* VPLMN��Ч����VPLMN */
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();

            return;
        }

        /* VPLMN�����ṩ��������ʱ������פ����ע��ʧ�ܵ�������:BG��ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }






    /*********************************** ��ȡ�����ȼ�����ɹ�������״̬Ϊδ����***********************************/

    /* ����״̬Ϊδ����:��ע��ʧ�ܵ������ϼ���BG�� */

    if (NAS_MMC_NET_STATUS_NO_SEARCHED == stHighPrioPlmnInfo.astPlmnRatInfo[0].enNetStatus)
    {
        /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
        NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                                stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);
        return;
    }




    /*********************************** ��ȡ�����ȼ�����ɹ�������״̬Ϊ����***********************************/

    /* ����״̬Ϊ����:��������ָ���� */

    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                             NAS_MML_GetSimPsRegStatus());

    /* ���п���ָ������ */
    NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                               stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);


    return;

}


VOS_VOID NAS_MMC_SndRslt_BgPlmnSearch(
    NAS_MMC_BG_PLMN_SEARCH_RSLT_ENUM_UINT32                 enRslt,
    VOS_UINT32                                              ulNeedPlmnSearchFlg,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                                              ulBgSearchFinishedFlg;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astSearchedPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    NAS_MMC_BG_SEARCH_TYPE_ENUM_UINT8                       enFsmBgSearchType;

    NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM_UINT8                 enBgPlmnSearchScene;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstRatPrioList  = VOS_NULL_PTR;
    NAS_MMCMMC_BG_PLMN_SEARCH_RSLT_STRU                    *pstBgRsltCnf    = VOS_NULL_PTR;
    VOS_UINT32                                              i;


    enBgPlmnSearchScene = NAS_MMC_BG_PLMN_SEARCH_SCENE_BUTT;

    /* ��ȡ�����ȼ������б� */
    pstRatPrioList  = NAS_MML_GetMsPrioRatList();

    enFsmBgSearchType   = NAS_MMC_BG_SEARCH_TYPE_PLMN_SEARCH;

    pstBgRsltCnf = PS_MEM_ALLOC(WUEPS_PID_MMC, sizeof(NAS_MMCMMC_BG_PLMN_SEARCH_RSLT_STRU));

    if (VOS_NULL_PTR == pstBgRsltCnf)
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndRslt_BgPlmnSearch:mem alloc fail");
        return;
    }

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        enFsmBgSearchType = NAS_MMC_BG_SEARCH_TYPE_CSG_SEARCH;
    }
#endif

    /* �ж�BG��������� */
    ulBgSearchFinishedFlg = NAS_MMC_IsBgSearchFinished_BgPlmnSearch();

    /* �������������������ת��Ϊ�ڲ������б���Ϣ */
    if (VOS_NULL_PTR == pstSearchedPlmnListInfo)
    {
        PS_MEM_SET(astSearchedPlmnListInfo, 0, sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU) * NAS_MML_MAX_RAT_NUM);

        for (i = 0; i < pstRatPrioList->ucRatNum; i++)
        {
            astSearchedPlmnListInfo[i].enRatType = NAS_MML_NET_RAT_TYPE_BUTT;

            if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
            {
                astSearchedPlmnListInfo[i].enRatType                            = pstRatPrioList->aucRatPrio[i];
                astSearchedPlmnListInfo[i].stSearchedType.ucHistorySearchedFlg  = NAS_MMC_GetHistorySearchedFlag_BgPlmnSearch(pstRatPrioList->aucRatPrio[i]);
                astSearchedPlmnListInfo[i].stSearchedType.ucPrefBandSearchedFlg = VOS_FALSE;
                astSearchedPlmnListInfo[i].stSearchedType.ucSpecSearchedFlg     = VOS_FALSE;
            }
        }
    }
    else
    {
        PS_MEM_CPY(astSearchedPlmnListInfo, pstSearchedPlmnListInfo, sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU) * NAS_MML_MAX_RAT_NUM);
    }



    enBgPlmnSearchScene = NAS_MMC_GetBgSearchScene_BgPlmnSearch(ulNeedPlmnSearchFlg);

    /* �����Ϣ����,��Ϣͷ��NAS_MMC_SndBgPlmnSearchRslt��� */
    PS_MEM_SET(pstBgRsltCnf, 0, sizeof(NAS_MMCMMC_BG_PLMN_SEARCH_RSLT_STRU));

    pstBgRsltCnf->enRslt                        = enRslt;
    pstBgRsltCnf->ulNeedPlmnSearch              = ulNeedPlmnSearchFlg;
    pstBgRsltCnf->ulSearchCompleted             = ulBgSearchFinishedFlg;
    pstBgRsltCnf->enBgPlmnSearchScene           = enBgPlmnSearchScene;
    pstBgRsltCnf->enFsmBgSearchType             = enFsmBgSearchType;
    pstBgRsltCnf->enCsRegAdditionalAction       = NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
    pstBgRsltCnf->enPsRegAdditionalAction       = NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();

    PS_MEM_CPY(pstBgRsltCnf->astSearchedPlmnListInfo, astSearchedPlmnListInfo,
               sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU) * NAS_MML_MAX_RAT_NUM);

    /* ����BG������� */
    NAS_MMC_SndBgPlmnSearchRslt(pstBgRsltCnf);

    PS_MEM_FREE(WUEPS_PID_MMC, pstBgRsltCnf);

    return;
}


VOS_VOID NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* bg history */
    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        NAS_MMC_SndBgHistoryPlmnSearchReq_BgPlmnSearch(pstPlmnId, enRat);
        return;
    }

    /* bg spec */
    NAS_MMC_SndBgSpecPlmnSearchReq_BgPlmnSearch(pstPlmnId, enRat);

    return;
}


VOS_VOID NAS_MMC_SndBgSpecPlmnSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU     stDestPlmnList;

    stDestPlmn.stPlmnId = *pstPlmnId;
    stDestPlmn.enRat    = enRat;

    /* ������Ҫ�����������б�״̬�������� */
    PS_MEM_SET(&stDestPlmnList, 0, sizeof(NAS_MML_PLMN_LIST_WITH_RAT_STRU));
    stDestPlmnList.ulPlmnNum                = 1;
    stDestPlmnList.astPlmnId[0].enRat       = enRat;
    stDestPlmnList.astPlmnId[0].stPlmnId    = *pstPlmnId;

    NAS_MMC_SetCurHighPrioPlmnList_BgPlmnSearch(&stDestPlmnList);

    /* ���ݲ�ͬ�Ľ��뼼������BG�������� */
    switch (NAS_MML_GetCurrNetRatType())
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsBgSpecPlmnSearchReq(WUEPS_PID_WRR, &stDestPlmn);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF_LEN);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsBgSpecPlmnSearchReq(UEPS_PID_GAS, &stDestPlmn);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF_LEN);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmBgPlmnSearchReq(&stDestPlmn);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF_LEN);
            break;
#endif

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch: Rat not support.");
            break;
    }

    return;
}



VOS_VOID NAS_MMC_SndBgHistoryPlmnSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU     stDestPlmnList;

    PS_MEM_SET(&stDestPlmn, 0x00, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    stDestPlmn.stPlmnId.ulMcc = pstPlmnId->ulMcc;
    stDestPlmn.stPlmnId.ulMnc = pstPlmnId->ulMnc;
    stDestPlmn.enRat    = enRat;

    PS_MEM_SET(&stDestPlmnList, 0x0, sizeof(stDestPlmnList));
    stDestPlmnList.ulPlmnNum = 1;
    PS_MEM_CPY(&stDestPlmnList.astPlmnId[0], &stDestPlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* bg history�ѿ��Դ����PLMN�����PLMN���췽��ͬNAS_MMC_BuildDestPlmnListWhenSearchPlmn_PlmnSelection */
    NAS_MMC_BuildDestPlmnListInBgHistoryScene_BgPlmnSearch(&stDestPlmnList, &stDestPlmn);

    /* ������Ҫ�����������б�״̬�������� */
    NAS_MMC_SetCurHighPrioPlmnList_BgPlmnSearch(&stDestPlmnList);

    /* ���ݲ�ͬ�Ľ��뼼������BG�������� */
    switch (NAS_MML_GetCurrNetRatType())
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsBgHistoryPlmnSearchReq(WUEPS_PID_WRR, &stDestPlmnList);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF_LEN);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsBgHistoryPlmnSearchReq(UEPS_PID_GAS, &stDestPlmnList);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF_LEN);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmBgHistoryPlmnSearchReq(&stDestPlmnList);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF_LEN);
            break;
#endif

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndBgHistoryPlmnSearchReq_BgPlmnSearch: Rat not support.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_PerformRrcRelRegFinished_BgPlmnSearch(VOS_VOID)
{

#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        if (VOS_TRUE == NAS_MMC_IsDisableLteNeedLocalReleaseEpsConn())
        {
            NAS_MMC_SndLmmRelReq();

            /* bg history��������ֱ����ע�ᱻ�ܵ��������BG�ѻ����ָ���ѣ���һ����VPLMN */
            if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
            {
                NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);
            }
        }

        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_CONN_REL_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
    }
    else
#endif
    {
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
    }

    return;
}


VOS_VOID NAS_MMC_BuildDestPlmnListInBgHistoryScene_BgPlmnSearch(
    NAS_MML_PLMN_LIST_WITH_RAT_STRU     *pstDestPlmnList,
    NAS_MML_PLMN_WITH_RAT_STRU          *pstDestPlmn
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    NAS_MML_PLMN_LIST_WITH_RAT_STRU                         stTempDestPlmnList;
    NAS_MML_PLMN_WITH_RAT_STRU                              stTempPlmnId;
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;

    if(VOS_NULL_PTR == pstDestPlmnList || VOS_NULL_PTR == pstDestPlmn)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_BuildDestPlmnListInBgHistoryScene_BgPlmnSearch pstDestPlmnList or pstDestPlmn is VOS_NULL_PTR");
        return;
    }

    PS_MEM_CPY(&stTempDestPlmnList, pstDestPlmnList, sizeof(NAS_MML_PLMN_LIST_WITH_RAT_STRU));

    pstPlmnSelectionList             = NAS_MMC_GetHighPrioPlmnList();

    if ( VOS_TRUE == NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_BgPlmnSearch(pstDestPlmn) )
    {
        NAS_MMC_AddEHPlmnInDestPlmnList(&stTempDestPlmnList, NAS_MML_GetSimEhplmnList());

        NAS_MMC_SetSearchRplmnAndHplmnFlg_BgPlmnSearch(pstDestPlmn->enRat, VOS_TRUE);
    }
    else if (VOS_TRUE == NAS_MMC_IsNeedAddEhplmnWhenSeachDplmn_BgPlmnSearch(pstDestPlmn))
    {
        NAS_MMC_AddEHPlmnInDestPlmnList(&stTempDestPlmnList, NAS_MML_GetSimEhplmnList());

        NAS_MMC_SetSearchDplmnAndHplmnFlg_BgPlmnSearch(pstDestPlmn->enRat, VOS_TRUE);
    }
    else if ( VOS_TRUE == NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch(pstDestPlmn) )
    {
        NAS_MMC_AddEHPlmnInDestPlmnList(&stTempDestPlmnList, NAS_MML_GetSimEhplmnList());

        NAS_MMC_SetSearchRplmnAndHplmnFlg_BgPlmnSearch(pstDestPlmn->enRat, VOS_TRUE);

    }
    else
    {
    }


    if (VOS_TRUE == NAS_MMC_IsNeedAddEplmnWhenSearchRplmn_BgPlmnSearch(pstDestPlmn))
    {
        NAS_MMC_AddEPlmnInDestPlmnList(&stTempDestPlmnList);

        NAS_MMC_SetSearchRplmnAndEplmnFlg_BgPlmnSearch(pstDestPlmn->enRat, VOS_TRUE);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch(pstDestPlmn))
    {
        NAS_MMC_AddDplmnInDestPlmnList(pstDestPlmn, &stTempDestPlmnList, pstPlmnSelectionList);
    }

    /* ɾ��֮ǰפ����ע�ᱻ�ܵ����� */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_TRUE == NAS_MMC_IsNeedDelCurrCampPlmn_PlmnSelectionStrategy(NAS_MMC_GetPreCampLai_BgPlmnSearch()))
    {
        NAS_MML_DelSpecPlmnWithRatInPlmnListWithRat(&stPlmnWithRat,
                                                    &stTempDestPlmnList);
    }

    for (i = 0, j = 0; i < stTempDestPlmnList.ulPlmnNum; i++)
    {
        stTempPlmnId.stPlmnId.ulMcc = stTempDestPlmnList.astPlmnId[i].stPlmnId.ulMcc;
        stTempPlmnId.stPlmnId.ulMnc = stTempDestPlmnList.astPlmnId[i].stPlmnId.ulMnc;
        stTempPlmnId.enRat          = stTempDestPlmnList.astPlmnId[i].enRat;

        if (VOS_FALSE == NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy(&stTempPlmnId.stPlmnId, stTempPlmnId.enRat ,
                                                                        NAS_MMC_NET_STATUS_NO_SEARCHED, VOS_FALSE))
        {
            NAS_WARNING_LOG3(WUEPS_PID_MMC, "NAS_MMC_BuildDestPlmnListInBgHistoryScene_BgPlmnSearch:WARNING: Current Lte Plmn is not Allowed Srch ulMcc�� ulMnc and enRat are",
                                             stTempPlmnId.stPlmnId.ulMcc, stTempPlmnId.stPlmnId.ulMnc, stTempPlmnId.enRat);
            continue;
        }

        PS_MEM_CPY(&pstDestPlmnList->astPlmnId[j], &stTempDestPlmnList.astPlmnId[i], sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
        j++;
        pstDestPlmnList->ulPlmnNum = j;
    }

    return;
}



VOS_VOID NAS_MMC_SndSuspendReq_BgPlmnSearch(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* �����뼼�����͹�������,����������ʱ����Ǩ��״̬ */
    switch (enRat)
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsSuspendReq(WUEPS_PID_WRR);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_SUSPEND_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF_LEN);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsSuspendReq(UEPS_PID_GAS);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_SUSPEND_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF_LEN);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmSuspendReq();
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_SUSPEND_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF, TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF_LEN);
            break;
#endif

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSuspendReq_BgPlmnSearch: Rat not support.");
            break;
    }

    return;
}



VOS_VOID NAS_MMC_SndSuspendReqReCampVPlmn_BgPlmnSearch(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* �����뼼�����͹�������,����������ʱ����Ǩ��״̬ */
    switch (enRat)
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsSuspendReq(WUEPS_PID_WRR);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_SUSPEND_CNF_RECAMP_VPLMN);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF_LEN);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsSuspendReq(UEPS_PID_GAS);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_SUSPEND_CNF_RECAMP_VPLMN);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF_LEN);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmSuspendReq();
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_SUSPEND_CNF_RECAMP_VPLMN);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF, TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF_LEN);
            break;
#endif

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSuspendReqReCampVPlmn_BgPlmnSearch: Rat not support.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_SndAsSpecPlmnSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU                stForbRoamLaInfo;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbPlmnInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                         stDestPlmnList;


    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();
    stForbRoamLaInfo.ucForbRoamLaNum = pstForbPlmnInfo->ucForbRoamLaNum;
    PS_MEM_CPY( stForbRoamLaInfo.astForbRomLaList,
                pstForbPlmnInfo->astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList) );

    NAS_MML_AddCustomizedForbLaToDestLaList(&(stForbRoamLaInfo.ucForbRoamLaNum),
                                                   stForbRoamLaInfo.astForbRomLaList);

    PS_MEM_SET(&stDestPlmnList, 0x0, sizeof(stDestPlmnList));

    stDestPlmnList.ulPlmnNum            = 1;
    stDestPlmnList.astPlmnId[0].enRat   = enRat;
    PS_MEM_CPY(&stDestPlmnList.astPlmnId[0].stPlmnId, pstPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));

    NAS_MMC_SndAsSpecPlmnSearchReq(&stDestPlmnList, &stForbRoamLaInfo);
}




VOS_VOID NAS_MMC_SndFastSpecSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* ��¼��RAT�½��й�bg history�� */
    NAS_MMC_SetHistorySearchedFlag_BgPlmnSearch(enRat, VOS_TRUE);

    /* �����뼼�����Ϳ���ָ������,����������ʱ����Ǩ��״̬ */
    switch (enRat)
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsFastPlmnSearchReq(WUEPS_PID_WRR, pstPlmnId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_PLMN_FAST_SEARCH_CNF);

            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_WAS_FAST_PLMN_SEARCH_CNF_LEN);

            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
            {
                NAS_MMC_SndAsSpecPlmnSearchReq_BgPlmnSearch(pstPlmnId, enRat);
            }
            else
            {
                NAS_MMC_SndAsFastPlmnSearchReq(UEPS_PID_GAS, pstPlmnId);
            }

            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_PLMN_FAST_SEARCH_CNF);

            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF, NAS_MMC_GetWaitGasPlmnSearchCnfTimerLen(VOS_FALSE));
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmFastPlmnSearchReq(pstPlmnId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_PLMN_FAST_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF_LEN);
            break;
#endif
        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndFastSpecSearchReq_BgPlmnSearch: Rat not support.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enNetRatType;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU     stPlmnIdList;

    enNetRatType = NAS_MML_GetCurrNetRatType();

#if (FEATURE_ON == FEATURE_LTE)
    if (VOS_TRUE == NAS_MMC_GetNeedEnableLteFlg_BgPlmnSearch())
    {
        NAS_MMC_SendEnableLTE_FastSpecSearch();
        NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(VOS_FALSE);
        /* �ñ�־��ά��Ϊ:��BG��ʱ��ʼ��ΪFALSE,BG״̬�����������Enable��ΪTRUE;
           BG�ѽ�������������ʱ����ΪFALSE;
           BG�ѽ��������б��ѣ��б��ѽ����ٽ�ѡ��״̬����ѡ��״̬���������Ƿ���ҪDisable LTE���ٳ�ʼ��ΪFALSE
           BG�ѽ���������ѡ��״̬����ѡ��״̬���������Ƿ���ҪDisable LTE���ٳ�ʼ��ΪFALSE
           BG�ѽ��������б��ѣ��б��ѽ�������ѡ��״̬������ΪFALSE
           */
        NAS_MMC_SetLteEnabledFlgWhenFastSpecSrch(VOS_TRUE);
    }
#endif

    PS_MEM_SET(&stPlmnIdList, 0, sizeof(NAS_MML_PLMN_LIST_WITH_RAT_STRU));

    stPlmnIdList.ulPlmnNum  = 1;
    stPlmnIdList.astPlmnId[0].stPlmnId  = *pstPlmnId;
    stPlmnIdList.astPlmnId[0].enRat     = enRat;

    /* ���������б� */
    NAS_MMC_SetCurHighPrioPlmnList_BgPlmnSearch(&stPlmnIdList);

    /* �����Ҫ�����������뵱ǰ���뼼����ͬ,,���ߵ�ǰ���뼼��ΪButt,��Ӧ�ĳ�����
       �����ظ�SUSPNED CNF��Ὣ��ǰ����ģʽ����ΪButt,��ʾ�������κν��뼼��ֱ�ӷ������� */
    if ((enNetRatType == enRat)
     || (NAS_MML_NET_RAT_TYPE_BUTT == enNetRatType))
    {
        NAS_MML_SetCurrNetRatType(enRat);

        NAS_MMC_SndFastSpecSearchReq_BgPlmnSearch(pstPlmnId, enRat);
    }
    else
    {
        NAS_MMC_SndSuspendReq_BgPlmnSearch(enNetRatType);
    }

    return;
}


VOS_VOID NAS_MMC_UpdateRegFinishedSelectionList_BgPlmnSearch(VOS_VOID)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurPlmn;
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_CSG)
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
#endif

    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo           = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId       = pstCurCampInfo->stLai.stPlmnId;
    stCurPlmn.enRat          = pstCurCampInfo->enNetRatType;

    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��֧��IMSʱ�������CS��PSע����������֧��IMSʱ����CS��PS��IMSע������
    �����ע�����ж�CSG���������Ƿ�ɹ� */
#if (FEATURE_ON == FEATURE_CSG)

    /* �����ǰ��CSG���������̣�����CSG������ע���� */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        /* ��ȡCSG�������������б� */
        pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();

        /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
        pstCurSearchCsgId        = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

        /* ����CSG BG����������״̬Ϊ��ע�ᣬ������ʲô���������������������CSG ID�� */
        NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(pstCurSearchCsgId,
                                        NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                        pstCsgIdBgSearchListInfo);
        return;
    }
#endif

    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                                NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                pstPlmnSelectionListInfo);
        return;
    }


    /* ѡ��״̬���£�HPLMNע�ᱻ#13�ܾ������Ҫ���ѵ�ǰHPLMN���򲻸��µ�ǰ���������״̬ */
    if (VOS_TRUE == NAS_MMC_IsNeedSrchCurrCampHpmnRejByCause13())
    {
        /* ���µ�ǰ���������״̬Ϊ���������Ҵ��ڣ��ڴ��ָ����ȼ������У��������Ѵ����� */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                                NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                                pstPlmnSelectionListInfo);
    }
    else
    {
        /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                                NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                pstPlmnSelectionListInfo);
    }

    return;
}


VOS_UINT32 NAS_MMC_ContinueBgSearchProc_BgPlmnSearch(VOS_VOID)
{
#if (FEATURE_ON == FEATURE_CSG)

    /* �����ǰ��CSG���������̣��������һ��CSG ID���� */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        return NAS_MMC_SearchNextCsgId_BgPlmnSearch();
    }
#endif

    return NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch();
}

#if (FEATURE_ON == FEATURE_CSG)

VOS_VOID NAS_MMC_SndBgCsgIdSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    VOS_UINT32                          ulCsgIdNum,
    VOS_UINT32                         *pulCsgId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;

    stDestPlmn.stPlmnId = *pstPlmnId;
    stDestPlmn.enRat    = enRat;

    /* ���ݲ�ͬ�Ľ��뼼������CSG BG�������� */
    switch (NAS_MML_GetCurrNetRatType())
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsCsgBgSearchReq(WUEPS_PID_WRR, &stDestPlmn, ulCsgIdNum, pulCsgId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_CSG_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF_LEN);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsCsgBgSearchReq(UEPS_PID_GAS, &stDestPlmn, ulCsgIdNum, pulCsgId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_CSG_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF_LEN);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmCsgBgSearchReq(&stDestPlmn, ulCsgIdNum, pulCsgId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_CSG_BG_PLMN_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF_LEN);
            break;
#endif

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndBgCsgIdSearchReq_BgPlmnSearch: Rat not support.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_SndFastCsgSpecSearchReq_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    VOS_UINT32                          ulCsgIdNum,
    VOS_UINT32                         *pulCsgId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* �����뼼�����Ϳ���ָ������,����������ʱ����Ǩ��״̬ */
    switch (enRat)
    {
        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsFastCsgSearchReq(WUEPS_PID_WRR, pstPlmnId, ulCsgIdNum, pulCsgId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_PLMN_FAST_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_WAS_FAST_PLMN_SEARCH_CNF_LEN);

            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsFastCsgSearchReq(UEPS_PID_GAS, pstPlmnId, ulCsgIdNum, pulCsgId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_PLMN_FAST_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF, NAS_MMC_GetWaitGasPlmnSearchCnfTimerLen(VOS_FALSE));
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmFastCsgSearchReq(pstPlmnId, ulCsgIdNum, pulCsgId);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_CSG_FAST_SEARCH_CNF);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF_LEN);
            break;
#endif
        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndFastCsgSpecSearchReq_BgPlmnSearch: Rat not support.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    VOS_UINT32                          ulCsgIdNum,
    VOS_UINT32                         *pulCsgId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enNetRatType;

    enNetRatType = NAS_MML_GetCurrNetRatType();

#if (FEATURE_ON == FEATURE_LTE)

    /* ����ͨPLMN����������ͬ���ѵ�LTE��CSG ID�ſ���enable LTE */
    if ((VOS_TRUE == NAS_MMC_GetNeedEnableLteFlg_BgPlmnSearch())
      &&(NAS_MML_NET_RAT_TYPE_LTE == enRat))
    {
        NAS_MMC_SendEnableLTE_FastSpecSearch();
        NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(VOS_FALSE);
    }
#endif

    /* �����Ҫ�����������뵱ǰ���뼼����ͬ,,���ߵ�ǰ���뼼��ΪButt,��Ӧ�ĳ�����
       �����ظ�SUSPNED CNF��Ὣ��ǰ����ģʽ����ΪButt,��ʾ�������κν��뼼��ֱ�ӷ������� */
    if ((enNetRatType == enRat)
     || (NAS_MML_NET_RAT_TYPE_BUTT == enNetRatType))
    {
        NAS_MML_SetCurrNetRatType(enRat);
        NAS_MMC_SndFastCsgSpecSearchReq_BgPlmnSearch(pstPlmnId, ulCsgIdNum, pulCsgId, enRat);
    }
    else
    {
        NAS_MMC_SndSuspendReq_BgPlmnSearch(enNetRatType);
    }

    return;
}

VOS_VOID NAS_MMC_ProcCsgBgSearchRlstFail_BgPlmnSearch(VOS_VOID)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_CONN_STATUS_INFO_STRU                          *pstConnStatus = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              aulCsgId[RRC_NAS_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId, 0xF, RRC_NAS_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));

    pstConnStatus = NAS_MML_GetConnStatus();

    /* ��ȡCSG���������б� */
    pstCsgIdSelectionListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ��ά�ɲ�,��ѡ���б���� */
    NAS_MMC_LogPlmnSelectionList(pstCsgIdSelectionListInfo);

    /* �����ȡCSG IDʧ���˳�״̬�� */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchPlmnAndCsgId(pstCsgIdSelectionListInfo,
                                                       &stCsgIdInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* ������Ҫ������CSG ID��״̬�������� */
    NAS_MMC_SetCurSearchCsgId_BgPlmnSearch(&stCsgIdInfo.stPlmnId,
                                        stCsgIdInfo.ulCsgId,
                                        stCsgIdInfo.astPlmnRatInfo[0].enRatType);

    /* ����״̬Ϊδ��������BG�� */
    if (NAS_MMC_NET_STATUS_NO_SEARCHED == stCsgIdInfo.astPlmnRatInfo[0].enNetStatus)
    {
        /* ��ǰֻ֧��ָ��������1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
        ulCsgIdNum  = 1;
        aulCsgId[0] = stCsgIdInfo.ulCsgId;

        /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
        NAS_MMC_SndBgCsgIdSearchReq_BgPlmnSearch(&stCsgIdInfo.stPlmnId,
                                            ulCsgIdNum, aulCsgId,
                                            stCsgIdInfo.astPlmnRatInfo[0].enRatType);

        return;
    }

    /* ��ȡ��ǰ���������� */
    enRatType = NAS_MML_GetCurrNetRatType();

    /* ���RRC���Ӵ���(WAS�������P̬),���ͷ�RRC����,G��L�����ڸó���*/
    if ((VOS_TRUE == pstConnStatus->ucRrcStatusFlg)
     && (NAS_MML_NET_RAT_TYPE_WCDMA == enRatType))
    {
        NAS_MMC_SndRrMmRelReq(RRC_NAS_PS_DOMAIN);
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
    }
    else
    {
        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                 NAS_MML_GetSimPsRegStatus());

        /* ��ǰֻ֧�ֿ���ָ����1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
        ulCsgIdNum  = 1;
        aulCsgId[0] = stCsgIdInfo.ulCsgId;

        /* ����CSG����ָ������ */
        NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(&stCsgIdInfo.stPlmnId,
                                ulCsgIdNum, aulCsgId,
                                stCsgIdInfo.astPlmnRatInfo[0].enRatType);

    }

    return;
}

VOS_UINT32 NAS_MMC_SearchNextCsgId_BgPlmnSearch(VOS_VOID)
{
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stCsgIdInfo;
    VOS_UINT32                                              ulIsVplmnNetRatValid;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              aulCsgId[NAS_MML_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId, 0xF, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));

    /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
       disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */
    ulIsVplmnNetRatValid     = NAS_MMC_IsVplmnValid();

    /* ��ȡCSG�������������б� */
    pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ���vplmn����syscfg���û�L disable�����Ч�������ȡ��һ��CSG ID��
       ֱ���˳�״̬������������פ�����ٸ��������ж��Ƿ���б�����*/
    if (VOS_FALSE == ulIsVplmnNetRatValid)
    {
        return VOS_FALSE;
    }

    /* �����ȡCSG IDʧ�ܻ�VPLMN */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchPlmnAndCsgId(pstCsgIdBgSearchListInfo,
                                        &stCsgIdInfo,
                                        &stPlmnWithRat,
                                        NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        NAS_MMC_ReCampVPlmn_BgPlmnSearch();
        return VOS_TRUE;
    }

    /* ����״̬Ϊδ������VPLMN����BG�� */
    if (NAS_MMC_NET_STATUS_NO_SEARCHED == stCsgIdInfo.astPlmnRatInfo[0].enNetStatus)
    {
        NAS_MMC_ReCampVPlmn_BgPlmnSearch();
        return VOS_TRUE;
    }
    else
    {
        /* ���浱ǰ��Ҫ������CSG ID��״̬�������� */
        NAS_MMC_SetCurSearchCsgId_BgPlmnSearch(&stCsgIdInfo.stPlmnId,
                                        stCsgIdInfo.ulCsgId,
                                        stCsgIdInfo.astPlmnRatInfo[0].enRatType);

        /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
        pstCurSearchCsgId = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

        /* ��ǰֻ֧�ֿ���ָ����1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
        ulCsgIdNum  = 1;
        aulCsgId[0] = pstCurSearchCsgId->ulCsgId;

        /* ����CSG����ָ������ */
        NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(&pstCurSearchCsgId->stPlmnWithRat.stPlmnId,
                                        ulCsgIdNum, aulCsgId,
                                        pstCurSearchCsgId->stPlmnWithRat.enRat);

    }

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_LTE)


VOS_UINT32 NAS_MMC_IsCsgBgSearchSucc_BgPlmnSearch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8  enPsAction
)
{
    VOS_UINT32                          ucImsVoiceAvailFlag;
    VOS_UINT32                          ulIsNeedDisableLteImsVoiceNotAvail;

    /* �����ǰפ���Ĳ���CSGС������ΪCSG�����������ɹ� */
    if (NAS_MML_CELL_TYPE_CSG != NAS_MML_GetCurrCampCellType())
    {
        return VOS_FALSE;
    }

    /* ע����Ϊims����,��ims�����ò�����disable lte��������ΪCSG���������ɹ� */
    ucImsVoiceAvailFlag                = NAS_MML_GetImsVoiceAvailFlg();
    ulIsNeedDisableLteImsVoiceNotAvail = NAS_MMC_IsNeedDisableLte_ImsVoiceNotAvail();

    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enPsAction)
        && ((VOS_TRUE == ucImsVoiceAvailFlag)
           ||(VOS_FALSE == ulIsNeedDisableLteImsVoiceNotAvail)))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}


VOS_VOID NAS_MMC_ProcLteCsgRegRslt_BgPlmnSearch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8  enCsAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8  enPsAction
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MML_PLMN_WITH_CSG_ID_STRU                           stCurrCampPlmnWithCsgId;
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              ulLoop;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       stSpecCsgId;

    /* ��ȡCSG�������������б� */
    pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();
    ulCsgIdNum = 0;

    pstCurCampInfo = NAS_MML_GetCurrCampPlmnInfo();

    enCsAdditionalAction               = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    enPsAdditionalAction               = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    enAdditionalAction                 = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    PS_MEM_SET(&stCurrCampPlmnWithCsgId, 0, sizeof(stCurrCampPlmnWithCsgId));

    if (VOS_TRUE == NAS_MMC_IsCsgBgSearchSucc_BgPlmnSearch(enPsAction))
    {
        stCurrCampPlmnWithCsgId.stPlmnId.ulMcc = pstCurCampInfo->stLai.stPlmnId.ulMcc;
        stCurrCampPlmnWithCsgId.stPlmnId.ulMnc = pstCurCampInfo->stLai.stPlmnId.ulMnc;
        stCurrCampPlmnWithCsgId.ulCsgId        = pstCurCampInfo->ulCsgId;

        /* ��ΪCSG������ע��ɹ���CSG ID��Ҫ����Allowed CSG List�� */
        if (VOS_TRUE == NAS_MMC_IsRegSuccNeedAddCsgIdInAllowedCsgList(&stCurrCampPlmnWithCsgId, NAS_MML_GetCurrCampCsgIdHomeNodeBName()))
        {
            NAS_MMC_AddCsgIdInfoInAllowedCsgList(&stCurrCampPlmnWithCsgId, NAS_MML_GetCurrCampCsgIdHomeNodeBName());

            /* CSG�����ѳɹ����϶������ǽ�ֹOperator CSG List�е�CSG ID����ֹ�б����账�� */

            /* �����Ƿ����Allowed CSG List��������֪ͨ��������������������ޱ仯 */

        }

        /* �������Ҫ����Allowed CSG List������֪ͨ��������������Ϊ�����Ѳ�������hybrid CSG ID */

        for (ulCsgIdNum = 0; ulCsgIdNum < pstCsgIdBgSearchListInfo->usSearchPlmnNum; ulCsgIdNum++)
        {
            stSpecCsgId.stPlmnWithRat.stPlmnId.ulMcc = pstCsgIdBgSearchListInfo->astPlmnSelectionList[ulCsgIdNum].stPlmnId.ulMcc;
            stSpecCsgId.stPlmnWithRat.stPlmnId.ulMnc = pstCsgIdBgSearchListInfo->astPlmnSelectionList[ulCsgIdNum].stPlmnId.ulMnc;
            stSpecCsgId.stPlmnWithRat.enRat          = pstCsgIdBgSearchListInfo->astPlmnSelectionList[ulCsgIdNum].ucRatNum;
            stSpecCsgId.ulCsgId                      = pstCsgIdBgSearchListInfo->astPlmnSelectionList[ulCsgIdNum].ulCsgId;

            for (ulLoop = 0; ulLoop < pstCsgIdBgSearchListInfo->astPlmnSelectionList[ulCsgIdNum].ucRatNum; ulLoop++)
            {
                stSpecCsgId.stPlmnWithRat.enRat          = pstCsgIdBgSearchListInfo->astPlmnSelectionList[ulCsgIdNum].astPlmnRatInfo[ulLoop].enRatType;
                NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(&stSpecCsgId,
                                                NAS_MMC_NET_STATUS_BUTT,
                                                pstCsgIdBgSearchListInfo);
            }
        }

        return;
    }

    /* CSGע��ʧ����Ҫ��ȡ��һ��CSG ID�������������CS��PS��AdditionalActionΪ����������
    ��Ҫ����Ϊ��Ҫ���� */
    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    if ((NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION == enAdditionalAction)
        || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
        || (NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enAdditionalAction)
        || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED == enAdditionalAction))
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION);
    }

    return;
}
#endif
#endif


VOS_VOID NAS_MMC_LoadInterSysFsm_BgPlmnSearch(
    MMC_SUSPEND_CAUSE_ENUM_UINT8        enSuspendCause
)
{
    switch ( enSuspendCause )
    {
        /* ����HANDOVER״̬�� */
        case MMC_SUSPEND_CAUSE_HANDOVER:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_HO);
            break;

        /* ���� CCO״̬�� */
        case MMC_SUSPEND_CAUSE_CELLCHANGE:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_CCO);
            break;

        /* ���� С����ѡ ״̬�� */
        case MMC_SUSPEND_CAUSE_CELLRESELECT:
        case MMC_SUSPEND_CAUSE_REDIRECTION:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_CELLRESEL);
            break;

        /* ���� ϵͳ������������ ״̬�� */
        case MMC_SUSPEND_CAUSE_PLMNSEARCH:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_OOS);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_LoadInterSysFsm_BgPlmnSearch():Unexpected suspend cause.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_ProcBgSpecPlmnSearchRlstFail_BgPlmnSearch(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType;
    NAS_MML_CONN_STATUS_INFO_STRU                          *pstConnStatus = VOS_NULL_PTR;

    pstConnStatus = NAS_MML_GetConnStatus();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ���������б��������״̬ */

    NAS_MMC_UpdatePlmnListInPlmnSelectionList(pstSearchedPlmnListInfo,
                                              pstPlmnSelectionListInfo,
                                              VOS_FALSE,
                                              VOS_TRUE);

    /* ��ά�ɲ�,��ѡ���б���� */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    /* �����ȡ�����ȼ�����ʧ���˳�״̬�� */
    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnWithRat);

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* �Ƶ�������������֮ǰ���� */

    /* ����״̬Ϊδ��������BG�� */
    if (NAS_MMC_NET_STATUS_NO_SEARCHED == stHighPrioPlmnInfo.astPlmnRatInfo[0].enNetStatus)
    {
        /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
        NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                                stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);

        return;
    }

    /* ��ȡ��ǰ���������� */
    enRatType = NAS_MML_GetCurrNetRatType();

    /* ���RRC���Ӵ���(WAS�������P̬),���ͷ�RRC����,G��L�����ڸó���*/
    if ((VOS_TRUE == pstConnStatus->ucRrcStatusFlg)
     && (NAS_MML_NET_RAT_TYPE_WCDMA == enRatType))
    {
        NAS_MMC_SndRrMmRelReq(RRC_NAS_PS_DOMAIN);
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
    }
    else
    {
        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                 NAS_MML_GetSimPsRegStatus());

        /* ���п���ָ������ */
        NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                                   stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);
    }
}


VOS_VOID NAS_MMC_ProcBgHistoryPlmnSearchRlstFail_BgPlmnSearch(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_CONN_STATUS_INFO_STRU                          *pstConnStatus            = VOS_NULL_PTR;
    NAS_MMC_SERVICE_INFO_CTX_STRU                          *pstPreSrvInfo            = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType;
    VOS_UINT32                                              ulIsVplmnNetRatValid;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCampPlmnWithRat;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPreCampPlmnWithRat;
    VOS_UINT8                                               ucIsSamePlmn;


    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡrrc����״̬ */
    pstConnStatus            = NAS_MML_GetConnStatus();


    ucIsSamePlmn             = VOS_FALSE;

    NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPreCampPlmnWithRat);

    /* ���������б��������״̬ */
    NAS_MMC_UpdatePlmnListInPlmnSelectionList(pstSearchedPlmnListInfo,
                                              pstPlmnSelectionListInfo,
                                              VOS_FALSE,
                                              VOS_FALSE);

    /* ��ά�ɲ�,��ѡ���б���� */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);


    /* �Ƚ�Ŀ�������פ���������Ƿ���ͬ */

    PS_MEM_SET(&stCampPlmnWithRat, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    stCampPlmnWithRat.enRat             = NAS_MML_GetCurrNetRatType();
    stCampPlmnWithRat.stPlmnId.ulMcc    = NAS_MML_GetCurrCampPlmnId()->ulMcc;
    stCampPlmnWithRat.stPlmnId.ulMnc    = NAS_MML_GetCurrCampPlmnId()->ulMnc;


    if ((VOS_TRUE  == NAS_MML_CompareBcchPlmnwithSimPlmn(&(stPreCampPlmnWithRat.stPlmnId), &(stCampPlmnWithRat.stPlmnId)))
     && (stPreCampPlmnWithRat.enRat == stCampPlmnWithRat.enRat))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcBgHistoryPlmnSearchRlstFail_BgPlmnSearch:Current Camp Plmn is same as Pre Camp Plmn.");

        ucIsSamePlmn    = VOS_TRUE;
    }



    /* ========================== �޸����ȼ������磬�˳�bg search״̬�� ========================== */

    /* ���vplmn����syscfg���û�L disable�����Ч�������ȡ��һ�������ȼ����磬
     ֱ���˳�״̬������������פ�����ٸ��������ж��Ƿ���б�����*/
    ulIsVplmnNetRatValid     = NAS_MMC_IsVplmnValid();

    pstPreSrvInfo   = NAS_MMC_GetPreServiceInfo_BgPlmnSearch();

    if (VOS_FALSE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                       &stHighPrioPlmnInfo,
                                                       &stPreCampPlmnWithRat,
                                                       NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
    {

        /* ��ǰפ���������VPLMN��ͬ�����ϲ�״̬�����͸����ȼ������ɹ�������Ҫ���� */
        if (VOS_TRUE == ucIsSamePlmn)
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return;
        }

        /* VPLMN���ṩ�������񣬻�VPLMN����ӦCS PS mode 1��LTE����ɹ�����GU���磬����פ��LTE����Ҫ��������GU�ĳ��� */
        if ((NAS_MMC_NORMAL_SERVICE == pstPreSrvInfo->enCsCurrService)
         || (NAS_MMC_NORMAL_SERVICE == pstPreSrvInfo->enPsCurrService))
        {
            /* VPLMN��Чֱ����״̬�� */
            if (VOS_FALSE == ulIsVplmnNetRatValid)
            {
                /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

                /* �˳�״̬�� */
                NAS_MMC_FSM_QuitFsmL2();

                return;
            }

            /* VPLMN��Ч����VPLMN */
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();

            return;
        }

        /* VPLMN�����ṩ��������ʱ������פ����ע��ʧ�ܵ�������:BG��ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }




    /* ========================== ��һ�������ȼ���������no search�ģ�����bg�� ========================== */

    if (NAS_MMC_NET_STATUS_NO_SEARCHED == stHighPrioPlmnInfo.astPlmnRatInfo[0].enNetStatus)
    {
        /* �����㷢��BG����,����������ʱ��,Ǩ��״̬ */
        NAS_MMC_SndBgPlmnSearchReq_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                                stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);

        return;
    }




    /* ========================== ��һ�������ȼ���������searched exist�ģ�����ָ���� ========================== */

    /* ��ȡ��ǰ���������� */
    enRatType = NAS_MML_GetCurrNetRatType();

    /* ���RRC���Ӵ���(WAS�������P̬),���ͷ�RRC����,G��L�����ڸó���*/
    if ((VOS_TRUE == pstConnStatus->ucRrcStatusFlg)
     && (NAS_MML_NET_RAT_TYPE_WCDMA == enRatType))
    {
        NAS_MMC_SndRrMmRelReq(RRC_NAS_PS_DOMAIN);
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
    }
    else
    {
        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                 NAS_MML_GetSimPsRegStatus());

        /* ���п���ָ������ */
        NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(stHighPrioPlmnInfo.stPlmnId),
                                                   stHighPrioPlmnInfo.astPlmnRatInfo[0].enRatType);
    }

    return;
}


VOS_VOID NAS_MMC_ProcBgHistoryPlmnSearchRsltSucc_BgPlmnSearch(VOS_VOID)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    VOS_UINT32                                              ulRslt;

    ulRslt                      = VOS_TRUE;

    /* Ŀ������:�б��е�һ��searched exist������ */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    stDestPlmn.enRat            = NAS_MML_NET_RAT_TYPE_BUTT;
    stDestPlmn.stPlmnId.ulMcc   = NAS_MML_INVALID_MCC;
    stDestPlmn.stPlmnId.ulMnc   = NAS_MML_INVALID_MNC;

    ulRslt  = NAS_MMC_GetDestFastSpecSearchPlmnRcvBgHistorySucc_BgPlmnSearch(&stDestPlmn);

    /* ��ȡĿ������ʧ�ܣ��˳�״̬�� */
    if (VOS_FALSE   == ulRslt)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitWasBgPlmnSearchCnf:Get DestPlmn Fail or DestPlmn same as PreCampPlmn QuitFsmL2.");

        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        NAS_MMC_FSM_QuitFsmL2();

        return;
    }


    /* ����������Ӵ���(WAS�������P̬),���ͷ��������� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_SndRrMmRelReq(RRC_NAS_PS_DOMAIN);
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
    }
    else
    {

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                 NAS_MML_GetSimPsRegStatus());

        /* ���п���ָ�����Ѵ��ڵ����� */
        NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(stDestPlmn.stPlmnId),
                                                   stDestPlmn.enRat);
    }

    return;

}


VOS_VOID NAS_MMC_ProcRrMmBgSpecPlmnSearchRslt_BgPlmnSearch(
    RRMM_BG_PLMN_SEARCH_CNF_ST                             *pstBgSearchCnf
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                                       *pstPreCampLai            = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;
    VOS_UINT8                                               ucHighRatBgProc;

    ucHighRatBgProc = VOS_FALSE;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    if (pstBgSearchCnf->ucPlmnNum > NAS_RRC_MAX_BG_SRCH_PLMN_NUM)
    {
        pstBgSearchCnf->ucPlmnNum = NAS_RRC_MAX_BG_SRCH_PLMN_NUM;
    }

    /* GU�����ô˺�����ֹͣ����������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF);

    switch ( pstBgSearchCnf->enSearchRlt )
    {
        case RRC_BG_SPEC_PLMN_SEARCH_RLT_ABORT :

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        case RRC_BG_SPEC_PLMN_SEARCH_RLT_SUCCESS :

            /* ����BG����������״̬Ϊ���������� */
            NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                                    NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                                    pstPlmnSelectionListInfo);
            ucHighRatBgProc = NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch();

            /* ��ǰBG�Ѻ�VPLMN��PLMNһ��������RAT��ͬ�����������ָ���ѣ��˳�״̬��*/
            pstPreCampLai = NAS_MMC_GetPreCampLai_BgPlmnSearch();

            if ((VOS_TRUE  == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPreCampLai->stPlmnId), &(pstCurHighPrioPlmn->astPlmnId[0].stPlmnId)))
             && (VOS_FALSE == ucHighRatBgProc))
            {
                /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

                /* �˳���ǰ״̬�� */
                NAS_MMC_FSM_QuitFsmL2();

                return;
            }

            /* ����������Ӵ���(WAS�������P̬),���ͷ��������� */
            if (VOS_TRUE == NAS_MML_IsRrcConnExist())
            {
                NAS_MMC_SndRrMmRelReq(RRC_NAS_PS_DOMAIN);
                NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);
                (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
            }
            else
            {

                /* ��MM��GMM��������ָʾ */
                NAS_MMC_SndGmmPlmnSchInit();
                NAS_MMC_SndMmPlmnSchInit();

                NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

                NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                         NAS_MML_GetSimPsRegStatus());

                /* ���п���ָ������ */
                NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(pstCurHighPrioPlmn->astPlmnId[0].stPlmnId),
                                                           pstCurHighPrioPlmn->astPlmnId[0].enRat);
            }

            break;

        case RRC_BG_SPEC_PLMN_SEARCH_RLT_FAIL :


            PS_MEM_SET(&stSearchedPlmnListInfo, 0x00, sizeof(stSearchedPlmnListInfo));

            /* ��BG�������ת��Ϊ�ڲ�������Ϣ */
            NAS_MMC_ConvertRrcBgPlmnSearchRslt2SearchedPlmnListInfo(pstCurHighPrioPlmn->astPlmnId[0].enRat,
                                                                    pstBgSearchCnf,
                                                                    &stSearchedPlmnListInfo);
            /* ����BG������ʧ�ܽ�� */
            NAS_MMC_ProcBgSpecPlmnSearchRlstFail_BgPlmnSearch(&stSearchedPlmnListInfo);

            break;


        case RRC_BG_SPEC_PLMN_SEARCH_RLT_NO_RF:

        /* ��פ��̬ */
        if (NAS_MMC_AS_CELL_NOT_CAMP_ON == NAS_MMC_GetAsCellCampOn())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_GetCurrNetRatType());
        }
        else
        {
            /* NO RF��������ABORT������������10�뱳���ѳ��Զ�ʱ�� */
            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;
        }
        break;


        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitWasBgPlmnSearchCnf: Unexpected Bg search result.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_ProcRrMmBgHistoryPlmnSearchRslt_BgPlmnSearch(
    RRMM_BG_PLMN_SEARCH_CNF_ST                             *pstBgSearchCnf
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ����������PLMN�б� */
    if (pstBgSearchCnf->ucPlmnNum > NAS_RRC_MAX_BG_SRCH_PLMN_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcRrMmBgHistoryPlmnSearchRslt_BgPlmnSearch: ucPlmnNum is out of NAS_RRC_MAX_BG_SRCH_PLMN_NUM.");

        pstBgSearchCnf->ucPlmnNum = NAS_RRC_MAX_BG_SRCH_PLMN_NUM;
    }

    /* GU�����ô˺�����ֹͣ����������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF);

    switch ( pstBgSearchCnf->enSearchRlt )
    {
        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_ABORT :

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_SUCCESS :

            /* BG�ѳɹ�ʱ�Ȳ���history�ѱ�־��P̬����BG�ѣ�BG�����п����ȵ������ͷţ������ͷŽ�����ȡ����ָ���ѵ�����ʱ���õ��ñ�־ */

            NAS_MMC_ProcBgHistoryPlmnSearchRsltSucc_BgPlmnSearch();

            break;

        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_FAIL :

            /* �����Ѿ����й�bg history�� */
            NAS_MMC_SetHistorySearchedFlag_BgPlmnSearch(pstCurHighPrioPlmn->astPlmnId[0].enRat, VOS_TRUE);

            PS_MEM_SET(&stSearchedPlmnListInfo, 0x00, sizeof(stSearchedPlmnListInfo));

            /* ��BG�������ת��Ϊ�ڲ�������Ϣ */
            NAS_MMC_ConvertRrcBgPlmnSearchRslt2SearchedPlmnListInfo(pstCurHighPrioPlmn->astPlmnId[0].enRat,
                                                                    pstBgSearchCnf,
                                                                    &stSearchedPlmnListInfo);
            /* ����BG������ʧ�ܽ�� */
            NAS_MMC_ProcBgHistoryPlmnSearchRlstFail_BgPlmnSearch(&stSearchedPlmnListInfo);

            break;


        case RRC_BG_HISTORY_PLMN_SEARCH_RLT_NO_RF:


            /* ��פ��̬ */
            if (NAS_MMC_AS_CELL_NOT_CAMP_ON == NAS_MMC_GetAsCellCampOn())
            {
                NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_GetCurrNetRatType());
            }
            else
            {
                /* ͬfail�Ĵ��� */
                PS_MEM_SET(&stSearchedPlmnListInfo, 0x00, sizeof(stSearchedPlmnListInfo));

                /* ��BG�������ת��Ϊ�ڲ�������Ϣ */
                NAS_MMC_ConvertRrcBgPlmnSearchRslt2SearchedPlmnListInfo(pstCurHighPrioPlmn->astPlmnId[0].enRat,
                                                                        pstBgSearchCnf,
                                                                        &stSearchedPlmnListInfo);
                /* ��������״̬ */
                NAS_MMC_UpdatePlmnListInPlmnSelectionList(&stSearchedPlmnListInfo,
                                                          pstPlmnSelectionListInfo,
                                                          VOS_FALSE,
                                                          VOS_FALSE);

                /* ��ά�ɲ�,��ѡ���б���� */
                NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

                /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

                /* �˳�״̬�� */
                NAS_MMC_FSM_QuitFsmL2();
            }

            break;

        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcRrMmBgHistoryPlmnSearchRslt_BgPlmnSearch: Unexpected Bg search result.");
            break;
    }

    return;
}



VOS_UINT32    NAS_MMC_RcvRrMmAreaLostInd_BgPlmnSearch_WaitWasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_AREA_LOST_IND_STRU                                *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (RRMM_AREA_LOST_IND_STRU*)pstMsg;

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_SEARCH_CNF);

    NAS_MMC_SaveLastCampedPlmnWithRat(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.enCampPlmnNetRat);


    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);


    NAS_MMC_BuildPlmnListInfoFromGuAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmAreaLostInd_BgPlmnSearch_WaitGasBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_AREA_LOST_IND_STRU                                *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (RRMM_AREA_LOST_IND_STRU*)pstMsg;

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_SEARCH_CNF);

    NAS_MMC_SaveLastCampedPlmnWithRat(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.enCampPlmnNetRat);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    NAS_MMC_BuildPlmnListInfoFromGuAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmAreaLostInd_BgPlmnSearch_WaitWasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_AREA_LOST_IND_STRU                                *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (RRMM_AREA_LOST_IND_STRU*)pstMsg;

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_BG_PLMN_STOP_CNF);

    NAS_MMC_SaveLastCampedPlmnWithRat(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.enCampPlmnNetRat);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);


    NAS_MMC_BuildPlmnListInfoFromGuAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32    NAS_MMC_RcvRrMmAreaLostInd_BgPlmnSearch_WaitGasBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_AREA_LOST_IND_STRU                                *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (RRMM_AREA_LOST_IND_STRU*)pstMsg;

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_BG_PLMN_STOP_CNF);

    NAS_MMC_SaveLastCampedPlmnWithRat(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.enCampPlmnNetRat);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    NAS_MMC_BuildPlmnListInfoFromGuAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvRrmmLimitServiceCampInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    return NAS_MMC_RcvRrmmLimitServiceCampInd_BgPLmnSearch(ulEventType, pstMsg);
}


VOS_UINT32 NAS_MMC_RcvMsccImsVoiceCapInd_BgPlmnSearch_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* GU���յ�IMS VOICE�Ƿ����Ŀǰ�ݲ�����
       ������ϵͳ�������յ���IMS VOICE�Ƿ���õĳ������ڿ�������ϵͳ���µ�IMS VOICE
       ������,��ʱ��������disable Lģ��׼ȷ,�ݲ����Ǹó��� */

    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvRrmmLimitServiceCampInd_BgPLmnSearch_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    return NAS_MMC_RcvRrmmLimitServiceCampInd_BgPLmnSearch(ulEventType, pstMsg);
}



VOS_UINT32  NAS_MMC_RcvRrmmLimitServiceCampInd_BgPLmnSearch(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pastIntraPlmnSrchInfo = VOS_NULL_PTR;
    RRMM_LIMIT_SERVICE_CAMP_IND_STRU                       *pstRrmmLimitCampOnMsg;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stHighPrioPlmnInfo;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnIdWithRat;
    VOS_UINT32                                              ulIndex;

    pstRrmmLimitCampOnMsg = (RRMM_LIMIT_SERVICE_CAMP_IND_STRU *)pstMsg;


    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();


    pastIntraPlmnSrchInfo = (NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                                             sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU) * NAS_MML_MAX_RAT_NUM);
    /* �����ڴ�ʧ�� */
    if (VOS_NULL_PTR == pastIntraPlmnSrchInfo)
    {
        return VOS_TRUE;
    }

    NAS_MMC_InitSearchedPlmnListInfo(pastIntraPlmnSrchInfo);

    /* ת��W�µ������ʽ */
    NAS_MMC_UpdateSearchedPlmnListInfo(VOS_NULL_PTR, &(pstRrmmLimitCampOnMsg->stPlmnIdList), pastIntraPlmnSrchInfo);

    /* ɾ���б��н�ֹ��PLMN */
    NAS_MMC_DelForbPlmnFromAsSrchedPlmnList(pastIntraPlmnSrchInfo);

    /* �ֶ�����ģʽ��ɾ�����û�ָ����PLMN */
    NAS_MMC_DelNotUserSpecPlmnFromSrchedPlmnList(pastIntraPlmnSrchInfo);

    /* ��ά�ɲ���Ϣ���� */
    NAS_MMC_LogAsPlmnList(pastIntraPlmnSrchInfo);

    /* ����available PLMNʱ,�˳���ǰ״̬��,�������� */
    if (VOS_TRUE == NAS_MMC_IsExistAvailPlmnInSearchedPlmnList(pastIntraPlmnSrchInfo))
    {
        /* ���������б��������״̬ */
        for (ulIndex = 0; ulIndex < NAS_MML_MAX_RAT_NUM; ulIndex++)
        {
            NAS_MMC_UpdatePlmnListInPlmnSelectionList(&(pastIntraPlmnSrchInfo[ulIndex]),
                                                        pstPlmnSelectionListInfo,
                                                        VOS_FALSE,
                                                        VOS_TRUE);
        }

        /* ��ά�ɲ�,��ѡ���б���� */
        NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

        NAS_MMC_GetPreCampPlmn_BgPlmnSearch(&stPlmnIdWithRat);

        /* �������Ӵ��ڣ��ҵ�ǰ��Ҫ��������Ǩ�Ƶ��ȴ������ͷŵ�״̬ */
        if (VOS_TRUE == NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(pstPlmnSelectionListInfo,
                                                           &stHighPrioPlmnInfo,
                                                           &stPlmnIdWithRat,
                                                           NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch()))
        {

            /* �Ƶ�������������֮ǰ���� */

            /* ���������ͷű�־ */
            NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);

            /* ����ǰ��ļ��,��ǰӦ�ô������� */
            NAS_MMC_SndMmRelReq();
            NAS_MMC_SndGmmRelReq();
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_RRC_CONN_REL_IND);
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
        }

    }

    PS_MEM_FREE(WUEPS_PID_MMC, pastIntraPlmnSrchInfo);

    return VOS_TRUE;
}


#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_LMM_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndLmmBgStopPlmnSearchReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLmmSysInfoInd_BgPlmnSearch_WaitLmmBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /*�����Ż���LTE�µ�С�������仯��LMM��ͨ��ϵͳ��Ϣ֪ͨMMC*/
    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_IsNeedUpdateCsgSpecSrchDuplicateRplmnAndEplmn(&stOldCampInfo))
    {
        NAS_MMC_UpdateCsgSpecSrchDuplicateRplmnAndEplmn(NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo());
    }
#endif

    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();
    }


    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ��GMM,��RABMʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSearchedPlmnInfoInd_BgPlmnSearch_WaitLmmBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU                    *pstSearchedPlmnInfoMsg   = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stSearchedPlmn;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus;
    VOS_UINT32                                              i;

    pstSearchedPlmnInfoMsg  = (LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU *)pstMsg;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ����bg history������ʱ������RRMM_SEARCHED_PLMN_INFO_IND */
    if (VOS_FALSE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmSearchedPlmnInfoInd_BgPlmnSearch_WaitLmmBgPlmnSearchCnf: is not bg history scene");

        return VOS_TRUE;
    }


    if (pstSearchedPlmnInfoMsg->ulTaiNum > LMM_MMC_MAX_SEARCHED_TAI_NUM)
    {
        pstSearchedPlmnInfoMsg->ulTaiNum  = LMM_MMC_MAX_SEARCHED_TAI_NUM;
    }

    /* ����ϣ�����BG����������״̬Ϊ���������ڣ�����ָ����ʱ����ֻ���Ѵ��ڵ����� */
    for (i = 0; i < pstSearchedPlmnInfoMsg->ulTaiNum; i++)
    {
        PS_MEM_SET(&stSearchedPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

        NAS_MMC_ConvertLmmRatToNasFormat(pstSearchedPlmnInfoMsg->stTaiList[i].enRat, &(stSearchedPlmn.enRat));
        NAS_MMC_ConvertLmmPlmnToGUNasFormat(&pstSearchedPlmnInfoMsg->stTaiList[i].stPlmnId,
                                            &stSearchedPlmn.stPlmnId);

        enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&stSearchedPlmn, pstPlmnSelectionListInfo);

        if (NAS_MMC_NET_STATUS_NO_SEARCHED == enNetStatus)
        {
            NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stSearchedPlmn,
                                                           NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                                           pstPlmnSelectionListInfo);
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitLmmBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_BG_PLMN_SEARCH_CNF_STRU                        *pstBgSearchCnf           = VOS_NULL_PTR;

    pstBgSearchCnf = (LMM_MMC_BG_PLMN_SEARCH_CNF_STRU*)pstMsg;

    switch ( pstBgSearchCnf->enRlst)
    {
        /* bg spec */
        case MMC_LMM_BG_SPEC_SRCH_RLT_ABORT:
        case MMC_LMM_BG_SPEC_SRCH_RLT_SUCCESS :
        case MMC_LMM_BG_SPEC_SRCH_RLT_FAIL:
        case MMC_LMM_BG_SPEC_SRCH_RLT_NO_RF:

            NAS_MMC_ProcLmmBgSpecPlmnSearchRlst_BgPlmnSearch(pstBgSearchCnf);

            break;


        /* bg history */
        case MMC_LMM_BG_HISTORY_SRCH_RLT_ABORT:
        case MMC_LMM_BG_HISTORY_SRCH_RLT_SUCCESS :
        case MMC_LMM_BG_HISTORY_SRCH_RLT_FAIL:
        case MMC_LMM_BG_HISTORY_SRCH_RLT_NO_RF:

            NAS_MMC_ProcLmmBgHistoryPlmnSearchRlst_BgPlmnSearch(pstBgSearchCnf);

            break;

        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitLmmBgPlmnSearchCnf: Unexpected Bg search result.");
            break;
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchList = VOS_NULL_PTR;

    /* ��ȡ�����ȼ������б���Ϣ */
    pstHighPrioPlmnSearchList = NAS_MMC_GetHighPrioPlmnList();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmBgPlmnSearchCnf: ENTERED");

    if (VOS_TRUE == NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch())
    {
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    }
    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        /* ��bg history��ʼ���б� */
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_BG_HISTORY_PLMN_SELECTION_LIST);
    }
    else
    {
        /* ���³�ʼ��ѡ���б� */
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);
        NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    }


    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_LMM_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndLmmBgStopPlmnSearchReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcAreaLostInd_BgPlmnSearch_WaitLmmBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_AREA_LOST_IND_STRU                             *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (LMM_MMC_AREA_LOST_IND_STRU*)pstMsg;

    PS_MEM_SET(astPlmnListInfo, 0x00, sizeof(astPlmnListInfo));

     /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ �ϱ��޷���*/

    /* ֪ͨTAF��ǰ�ķ���״̬*/

    /* ��TAF�ϱ��������� */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                    VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    NAS_MMC_BuildPlmnListInfoFromLmmAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSysInfoInd_BgPlmnSearch_WaitLmmBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);


    /*�����Ż���LTE�µ�С�������仯��LMM��ͨ��ϵͳ��Ϣ֪ͨMMC*/

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_IsNeedUpdateCsgSpecSrchDuplicateRplmnAndEplmn(&stOldCampInfo))
    {
        NAS_MMC_UpdateCsgSpecSrchDuplicateRplmnAndEplmn(NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo());
    }
#endif

    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);


    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ��GMM,��RABMʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvBgPlmnStopCnf_BgPlmnSearch_WaitLmmBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    NAS_MML_PLMN_ID_STRU                stPlmnId;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstPlmnWithRat;

    pstPlmnWithRat = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();
    stPlmnId.ulMcc = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMcc;
    stPlmnId.ulMnc = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMnc;
#endif
    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
        /* �����ǰplmn�������������˱�����״̬�������������� */
        if (VOS_FALSE == NAS_MMC_IsCurrLtePlmnAllowSrchInCLMode(&stPlmnId))
        {
            NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_RcvBgPlmnStopCnf_BgPlmnSearch_WaitLmmBgStopCnf:WARNING: Current Lte Plmn is not Allowed Srch In CL Mode,ulMcc and ulMnc are",
                                                 stPlmnId.ulMcc, stPlmnId.ulMnc);

            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
        }
        else
#endif
        {
            /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
        }
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitLmmBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_BG_PLMN_SEARCH_CNF_STRU                        *pstBgSearchCnf           = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;

    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    pstBgSearchCnf = (LMM_MMC_BG_PLMN_SEARCH_CNF_STRU*)pstMsg;

    if (pstBgSearchCnf->ulPlmnNum > MMC_LMM_MAX_BG_SRCH_PLMN_NUM)
    {
        pstBgSearchCnf->ulPlmnNum = MMC_LMM_MAX_BG_SRCH_PLMN_NUM;
    }

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    NAS_MMC_ConvertLmmBgPlmnSearchRslt2SearchedPlmnListInfo(pstBgSearchCnf,
                                                           &stSearchedPlmnListInfo,
                                                           pstCurHighPrioPlmn->astPlmnId[0].enRat);

    if (MMC_LMM_BG_SPEC_SRCH_RLT_FAIL    == pstBgSearchCnf->enRlst)
    {
        /* ���������б��������״̬:���ĸ�����ΪȫƵ�� */
        NAS_MMC_UpdatePlmnListInPlmnSelectionList(&stSearchedPlmnListInfo,
                                                  pstPlmnSelectionListInfo,
                                                  VOS_FALSE,
                                                  VOS_TRUE);

    }
    else if (MMC_LMM_BG_HISTORY_SRCH_RLT_FAIL == pstBgSearchCnf->enRlst)
    {
        /* ���������б��������״̬:���ĸ�����Ϊδ����ȫƵ�� */
        NAS_MMC_UpdatePlmnListInPlmnSelectionList(&stSearchedPlmnListInfo,
                                                  pstPlmnSelectionListInfo,
                                                  VOS_FALSE,
                                                  VOS_FALSE);
    }
    else
    {
    }
    /* ��ά�ɲ�,��ѡ���б���� */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiBgStopCnfExpired_BgPlmnSearch_WaitLmmBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiBgStopCnfExpired_BgPlmnSearch_WaitLmmBgStopCnf: ENTERED");

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLmmMmcAreaLostInd_BgPlmnSearch_WaitLmmBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_AREA_LOST_IND_STRU                             *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (LMM_MMC_AREA_LOST_IND_STRU*)pstMsg;

    PS_MEM_SET(astPlmnListInfo, 0x00, sizeof(astPlmnListInfo));

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ���·���״̬ �ϱ��޷���*/

    /* ֪ͨTAF��ǰ�ķ���״̬*/

    /* ��TAF�ϱ��������� */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                    VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    NAS_MMC_BuildPlmnListInfoFromLmmAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_CSG)

VOS_UINT32 NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitLmmBgStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_CSG_BG_SEARCH_CNF_STRU                         *pstCsgBgSearchCnf        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       astTmpCsgSearchInfo[NAS_MML_MAX_PLMN_CSG_ID_NUM];

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(astTmpCsgSearchInfo, 0,   NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    pstCsgBgSearchCnf        = (LMM_MMC_CSG_BG_SEARCH_CNF_STRU*)pstMsg;

    /* ��ȡCSG�������������б� */
    pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF);

    /* ��CSG BG�������ת��Ϊ�ڲ�CSG������� */
    NAS_MMC_ConvertLmmCsgBgSearchRslt2NasSearchCsgList(pstCsgBgSearchCnf, &ulCsgIdNum, astTmpCsgSearchInfo);

    /* ���������б��������״̬ */
    NAS_MMC_UpdateAllCsgIdNetStatusInCsgIdSelectionList(ulCsgIdNum,
                                astTmpCsgSearchInfo,
                                pstCsgIdBgSearchListInfo,
                                VOS_TRUE);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊʧ��,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();
    return VOS_TRUE;

}
#endif



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ�ȿ���ָ����������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndLmmPlmnSrchStopReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_FAST_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnSrchCnf_BgPlmnSearch_WaitLmmPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstSrchCnfMsg            = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    pstSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ֹͣ�ȿ�����������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_SUCC == pstSrchCnfMsg->enRlst)
    {
        /* ���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_SYSINFO_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO, TI_NAS_MMC_WAIT_LMM_SYS_INFO_LEN);
    }
    else if (MMC_LMM_PLMN_SRCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->enRlst)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);
        }

        return VOS_TRUE;
    }
    else
    {
        /* ���µ�ǰ��������״̬Ϊ������������ */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                                NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                                pstPlmnSelectionListInfo);

        /* ������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmPlmnFastSearchCnf: ENTERED");

    /* ���µ�ǰ��������״̬Ϊ������������ */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                            NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                            pstPlmnSelectionListInfo);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndLmmPlmnSrchStopReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_FAST_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnSrchStopCnf_BgPlmnSearch_WaitLmmFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF);

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvBgPlmnSearchCnf_BgPlmnSearch_WaitLmmFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstSrchCnfMsg            = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    pstSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF);

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->enRlst)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);
        }

        return VOS_TRUE;
    }

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_FAIL == pstSrchCnfMsg->enRlst)
    {
        /* ���µ�ǰ��������״̬Ϊ������������ */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                                       NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                                       pstPlmnSelectionListInfo);
    }


    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvTiWaitLmmStopCnfExpired_BgPlmnSearch_WaitLmmFastPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmStopCnfExpired_BgPlmnSearch_WaitLmmFastPlmnStopCnf: ENTERED");

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_CSG)

VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ��BG��������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨLMMֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_LMM_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndLmmBgStopPlmnSearchReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSysInfoInd_BgPlmnSearch_WaitLmmCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �ο�ԭ����PLMN�����յ�ϵͳ��Ϣ�Ĵ���ֻ��һ����ͬ�����פ����CSG ID�ϣ�
    ��ֹͣCSG�����ѣ��˳�״̬������Ҫ���� */

    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* �����Ż���LTE�µ�С�������仯��LMM��ͨ��ϵͳ��Ϣ֪ͨMMC */
    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);

    if (VOS_TRUE == NAS_MMC_IsNeedUpdateCsgSpecSrchDuplicateRplmnAndEplmn(&stOldCampInfo))
    {
        NAS_MMC_UpdateCsgSpecSrchDuplicateRplmnAndEplmn(NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo());
    }

    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();
    }

    /* CBS�ϱ� */
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ��GMM,��RABMʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);
    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);
    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    /* ���פ����CSG ID�ϣ���ֹͣCSG�������� */
    if (NAS_MML_CELL_TYPE_CSG == NAS_MML_GetCurrCampCellType())
    {
        /* ֹͣ��BG��������ظ���ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);

        /* ����Abort����Ϊ�ӳٴ�ϣ��յ�LMM�ϱ�ֹͣCNFʱ�˳�״̬������Ҫ���� */
        NAS_MMC_SetAbortType_BgPlmnSearch(NAS_MMC_ABORT_FSM_IMMEDIATELY);

        /* ֪ͨLMMֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_LMM_BG_PLMN_STOP_CNF������������ʱ�� */
        NAS_MMC_SndLmmBgStopPlmnSearchReq();
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_STOP_CNF);
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF_LEN);
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitLmmCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_CSG_BG_SEARCH_CNF_STRU                         *pstCsgBgSearchCnf        = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdBgSearchListInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId        = VOS_NULL_PTR;
    VOS_UINT32                                              ulCsgIdNum;
    VOS_UINT32                                              aulCsgId[NAS_MML_MAX_PLMN_CSG_ID_NUM];
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                       astTmpCsgSearchInfo[NAS_MML_MAX_PLMN_CSG_ID_NUM];

    /* ��ʼ���ֲ����� */
    ulCsgIdNum = 0;
    PS_MEM_SET(aulCsgId,            0xFF, NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(VOS_UINT32));
    PS_MEM_SET(astTmpCsgSearchInfo, 0,   NAS_MML_MAX_PLMN_CSG_ID_NUM*sizeof(NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU));

    pstCsgBgSearchCnf        = (LMM_MMC_CSG_BG_SEARCH_CNF_STRU*)pstMsg;

    /* ��ȡCSG�������������б� */
    pstCsgIdBgSearchListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
    pstCurSearchCsgId        = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);

    switch (pstCsgBgSearchCnf->enRlst)
    {
        case MMC_LMM_BG_SPEC_SRCH_RLT_ABORT:

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        case MMC_LMM_BG_SPEC_SRCH_RLT_SUCCESS :

            /* ����BG����������״̬Ϊ���������� */
            NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(pstCurSearchCsgId,
                                        NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                        pstCsgIdBgSearchListInfo);

            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                     NAS_MML_GetSimPsRegStatus());

            /* ��ǰֻ֧�ֿ���ָ����1��CSG ID������Ժ�֧�ֶ�����ڴ���չ */
            ulCsgIdNum  = 1;
            aulCsgId[0] = pstCurSearchCsgId->ulCsgId;

            /* ����CSG����ָ������ */
            NAS_MMC_PerformFastCsgSpecSearch_BgPlmnSearch(&pstCurSearchCsgId->stPlmnWithRat.stPlmnId,
                                        ulCsgIdNum, aulCsgId,
                                        pstCurSearchCsgId->stPlmnWithRat.enRat);

            break;

        case MMC_LMM_BG_SPEC_SRCH_RLT_FAIL:

            /* ��CSG BG�������ת��Ϊ�ڲ�CSG������� */
            NAS_MMC_ConvertLmmCsgBgSearchRslt2NasSearchCsgList(pstCsgBgSearchCnf, &ulCsgIdNum, astTmpCsgSearchInfo);

            /* ���������б��������״̬ */
            NAS_MMC_UpdateAllCsgIdNetStatusInCsgIdSelectionList(ulCsgIdNum,
                                        astTmpCsgSearchInfo,
                                        pstCsgIdBgSearchListInfo,
                                        VOS_TRUE);

            /* ����BG������ʧ�ܽ�� */
            NAS_MMC_ProcCsgBgSearchRlstFail_BgPlmnSearch();

            break;

        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvCsgBgSearchCnf_BgPlmnSearch_WaitLmmCsgBgPlmnSearchCnf: Unexpected Bg search result.");
            break;
    }

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSearchList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitBgPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmCsgBgPlmnSearchCnf: ENTERED");

    /* ��ȡCSG���������б� */
    pstCsgIdSearchList = NAS_MMC_GetCsgIdBgSearchList();

    if (NAS_MMC_USE_CSG_SEARCH_LIST_PROC_ASF_SEARCH == NAS_MMC_GetUseCsgIdSearchListProc())
    {
        enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_ASF_CSG_SEARCH;
    }
    else
    {
        enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_PSF_CSG_SEARCH;
    }

    /* ��ʼ��CSG���������б� */
    NAS_MMC_InitPlmnSelectionList(enPlmnSearchScene,
                                  VOS_NULL_PTR,
                                  pstCsgIdSearchList);


    /* ֪ͨASֹͣBG����,Ǩ��״̬NAS_MMC_BG_PLMN_SELECTION_STA_WAIT_LMM_BG_PLMN_STOP_CNF������������ʱ�� */
    NAS_MMC_SndLmmBgStopPlmnSearchReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcAreaLostInd_BgPlmnSearch_WaitLmmCsgBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_AREA_LOST_IND_STRU                             *pstAreaLostInd = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnListInfo[NAS_MML_MAX_RAT_NUM];

    pstAreaLostInd = (LMM_MMC_AREA_LOST_IND_STRU*)pstMsg;

    PS_MEM_SET(astPlmnListInfo, 0x00, sizeof(astPlmnListInfo));

     /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ��MM/GMM/TAF�������������� */
    NAS_MMC_SndMmCoverageLostInd();

    NAS_MMC_SndGmmCoverageLostInd();

    /* ��TAF�ϱ��������� */
    NAS_MMC_SndMsccCoverageInd( NAS_MSCC_PIF_COVERAGE_AREA_CHANGE_MODE_LOST );


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_COVERAGE_LOST,
                    VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    NAS_MMC_BuildPlmnListInfoFromLmmAreaLostInd(pstAreaLostInd, astPlmnListInfo);

    /* ����״̬�����Ϊʧ��,��Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astPlmnListInfo);

    /* �˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmCsgFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ֹͣ�ȿ���ָ����������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndLmmPlmnSrchStopReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_CSG_FAST_SEARCH_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnSrchCnf_BgPlmnSearch_WaitLmmCsgFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstSrchCnfMsg             = VOS_NULL_PTR;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId         = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo = VOS_NULL_PTR;

    pstSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
    pstCurSearchCsgId = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

    /* ��ȡCSG�������б� */
    pstCsgIdSelectionListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ֹͣ�ȿ�����������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_CSG_SUCC == pstSrchCnfMsg->enRlst)
    {
        /* ���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_SYSINFO_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO, TI_NAS_MMC_WAIT_LMM_SYS_INFO_LEN);
    }
    else if (MMC_LMM_PLMN_SRCH_RLT_SPEC_CSG_NO_RF == pstSrchCnfMsg->enRlst)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);
        }

        return VOS_TRUE;
    }
    else
    {
        /* ���µ�ǰBG����������״̬Ϊ������ */
        NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(pstCurSearchCsgId,
                                        NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                        pstCsgIdSelectionListInfo);

        /* ������һ��CSG ID */
        if (VOS_FALSE == NAS_MMC_SearchNextCsgId_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmCsgFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId         = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo = VOS_NULL_PTR;

    /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
    pstCurSearchCsgId = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

    /* ��ȡCSG�������б� */
    pstCsgIdSelectionListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_BgPlmnSearch_WaitLmmCsgFastSearchCnf: ENTERED");

    /* ���µ�ǰBG����������״̬Ϊ������ */
    NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(pstCurSearchCsgId,
                                        NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                        pstCsgIdSelectionListInfo);

    /* ֪ͨASֹͣ����ָ������,Ǩ��״̬,����������ʱ�� */
    NAS_MMC_SndLmmPlmnSrchStopReq();
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_CSG_FAST_SEARCH_STOP_CNF);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmCsgFastSearchStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnSrchStopCnf_BgPlmnSearch_WaitLmmCsgFastSearchStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF);

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������һ��CSG ID */
    if (VOS_FALSE == NAS_MMC_SearchNextCsgId_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvPlmnSearchCnf_BgPlmnSearch_WaitLmmCsgFastSearchStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstSrchCnfMsg             = VOS_NULL_PTR;
    NAS_MML_PLMN_CSG_ID_WITH_RAT_STRU                      *pstCurSearchCsgId         = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstCsgIdSelectionListInfo = VOS_NULL_PTR;

    pstSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU *)pstMsg;

    /* ��ȡ��ǰ���ڳ��Ե�CSG ID���������Ϣ */
    pstCurSearchCsgId = NAS_MMC_GetCurSearchCsgId_BgPlmnSearch();

    /* ��ȡCSG�������б� */
    pstCsgIdSelectionListInfo = NAS_MMC_GetCsgIdBgSearchList();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF);

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_CSG_NO_RF == pstSrchCnfMsg->enRlst)
    {
        /* ��NO RF���д��� */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MMC_GetPreCampLai_BgPlmnSearch()->enCampPlmnNetRat);
        }
        else
        {
            NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);
        }

        return VOS_TRUE;
    }

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_CSG_FAIL == pstSrchCnfMsg->enRlst)
    {
        /* ���µ�ǰBG����������״̬Ϊ������ */
        NAS_MMC_UpdateSpecCsgIdNetStatusInCsgIdSelectionList(pstCurSearchCsgId,
                                        NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,
                                        pstCsgIdSelectionListInfo);
    }

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������һ��CSG ID */
    if (VOS_FALSE == NAS_MMC_SearchNextCsgId_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvTiWaitLmmStopCnfExpired_BgPlmnSearch_WaitLmmCsgFastSearchStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmStopCnfExpired_BgPlmnSearch_WaitLmmCsgFastSearchStopCnf: ENTERED");

    /* �����ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ǰ�����ӳٴ�ϱ�־�����VPLMN */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */
        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������һ��CSG ID */
    if (VOS_FALSE == NAS_MMC_SearchNextCsgId_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}

#endif


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSuspendCnf_BgPlmnSearch_WaitLmmSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SUSPEND_CNF_STRU           *pstSuspendCnf = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstPlmn       = VOS_NULL_PTR;

    pstSuspendCnf = (LMM_MMC_SUSPEND_CNF_STRU*)pstMsg;

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF);

    if (MMC_LMM_FAIL == pstSuspendCnf->ulRst)
    {
        /* �쳣��ӡ */
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmSuspendCnf_BgPlmnSearch_WaitLmmSuspendCnf: SUSPEND FAIL !");

        /* ���ɻָ�����,���õ���ӿ�RESET */
        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_LMM_SUSPEND_FAILURE);

        return VOS_TRUE;
    }

    /* ���õ�ǰ���뼼��ΪBUTT */
    NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

    /* ��ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ��ǰ�����ӳٴ�ϱ�־�����VPlmn */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }


    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ���п���ָ������ */
    NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(pstPlmn->astPlmnId[0].stPlmnId),
                                                 pstPlmn->astPlmnId[0].enRat);
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLSysInfoInd_BgPlmnSearch_WaitLSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ֹͣ�ȴ�ϵͳ��Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY  == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort�������ӳ��˳������л�VPLMN���� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_IsNeedUpdateCsgSpecSrchDuplicateRplmnAndEplmn(&stOldCampInfo))
    {
        NAS_MMC_UpdateCsgSpecSrchDuplicateRplmnAndEplmn(NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo());
    }
#endif


    /* ���µ�ǰ�ȴ�EPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

#if (FEATURE_ON == FEATURE_IMS)
    /* ���ӵȴ�IMS�Ƿ���ý��FLG,MSCC��֤�յ�PSע�����ϱ�IMS�Ƿ����ָʾ��MMC */
    NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_IMS);
#endif

    /* ����CS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /*�����Ż���LTE�µ�С�������仯��LMM��ͨ��ϵͳ��Ϣ֪ͨMMC*/
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        if (VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
        {
            NAS_MMC_SetNeedSndSysInfo_BgPlmnSearch(VOS_TRUE);
        }
        else
        {
           /* �����ϱ� */
           NAS_MMC_SndMsccSysInfo();
        }
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* ת��ϵͳ��Ϣ��GMM,��RABMʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    /* Ǩ�Ƶ��ȴ�ע������״̬:NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_REG_IND */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_REG_IND);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    return VOS_TRUE;
}


/* ɾ��LTE�ϱ�����״̬��ע��״̬�Ľӿ�   NAS_MMC_RcvLmmMmcRegStatusInd_BgPlmnSearch_WaitLSysInfoInd()������ */



VOS_UINT32 NAS_MMC_RcvTiWaitLSysInfoExpired_BgPlmnSearch_WaitLSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ��ǰ�����ӳٴ�ϱ�־�����VPlmn */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* �����ڴ�ϱ�־����������һ�������ȼ����� */
    if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcStatusInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_STATUS_IND_STRU            *pstLmmStatusIndMsg = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstPlmnWithRat     = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                stPlmnId;
#endif

    pstLmmStatusIndMsg = (LMM_MMC_STATUS_IND_STRU*)pstMsg;

    if ( VOS_FALSE == pstLmmStatusIndMsg->bitOpConnState)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcStatusInd_BgPlmnSearch_WaitEpsRegRsltInd:bitOpConnState IS VOS_FALSE");
        return VOS_TRUE;
    }

    /* ����LMM������״̬ */
    NAS_MML_UpdateEpsConnectionStatus(pstLmmStatusIndMsg->ulConnState);
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    pstPlmnWithRat      = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();
    stPlmnId.ulMcc      = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMcc;
    stPlmnId.ulMnc      = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMnc;

    /* ���Ӳ����ڣ��жϵ�ǰlte plmn�Ƿ������� */
    if (VOS_FALSE == NAS_MMC_IsCurrLtePlmnAllowSrchInCLMode(&stPlmnId))
    {
        NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcStatusInd_BgPlmnSearch_WaitEpsRegRsltInd:WARNING: Current Lte Plmn is not Allowed Srch In CL Mode,ulMcc and ulMnc are",
                                             stPlmnId.ulMcc, stPlmnId.ulMnc);

        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ���ϲ�״̬�����͸����ȼ�����ʧ�� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }
#endif

    return VOS_TRUE;
}


/* ɾ��LTE�ϱ�����״̬��ע��״̬�Ľӿ�   NAS_MMC_RcvLmmMmcRegStatusInd_BgPlmnSearch_WaitEpsRegRsltInd()������ */


VOS_UINT32 NAS_MMC_RcvLmmMmcAttachInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg = VOS_NULL_PTR;

    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enUeOperationMode;

    enUeOperationMode   =  NAS_MML_GetLteUeOperationMode();
    pstLmmAttachIndMsg  = (LMM_MMC_ATTACH_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* LMM������δ�ظ�search cnfʱ�յ��û�detach ps����󣬺���������ע���mmc�ظ�attach result
       ΪMMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW���������������MMC_LMM_ATT_TYPE_BUTT��
       MMC��Ҫ���ݵ�ǰue operation mode�������������¸�ֵ */
    if ((MMC_LMM_ATT_TYPE_BUTT == pstLmmAttachIndMsg->ulReqType)
     && (MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW == pstLmmAttachIndMsg->ulAttachRslt))
    {
        pstLmmAttachIndMsg->ulReqType = NAS_MMC_ConvertLmmAttachReqType(enUeOperationMode);
    }

    /*ע��ɹ����ȱ�ϵͳ��Ϣ*/
    if (MMC_LMM_ATT_RSLT_SUCCESS == pstLmmAttachIndMsg->ulAttachRslt)
    {
        NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
    }
    else
    {
        /*ע��ǳɹ��������������ϱ�ϵͳ��Ϣ*/
        if (NAS_MMC_WAIT_REG_RESULT_IND_NULL == NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
        {
            NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
        }
    }

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmAttachInd(pstLmmAttachIndMsg);
    NAS_MMC_SndGmmLmmAttachInd(pstLmmAttachIndMsg);

    /* ����attach��� */
    NAS_MMC_ProcLmmAttachInd_BgPlmnSearch(pstLmmAttachIndMsg);

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* ��֧��IMSʱ�������CS��PSע����������֧��IMSʱ����CS��PS��IMSע������
    �����ע�����ж�CSG���������Ƿ�ɹ� */
#if (FEATURE_ON == FEATURE_CSG)

    /* �����ǰ��CSG���������̣�����CSG������ע���� */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        NAS_MMC_ProcLteCsgRegRslt_BgPlmnSearch(NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch(),
                                        NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch());
    }
#endif

    /* �����ҪAbort,��ʱ�Ĵ������ֻ����Ϊ�ӳٴ�� */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ע������ɺ��յ�ABORT�Ĵ��� */
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcTauResultInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg = VOS_NULL_PTR;

    pstLmmTauIndMsg = (LMM_MMC_TAU_RESULT_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /*ע��ɹ����ȱ�ϵͳ��Ϣ*/
    if (MMC_LMM_TAU_RSLT_SUCCESS == pstLmmTauIndMsg->ulTauRst)
    {
        NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
    }
    else
    {
        /*ע��ǳɹ��������������ϱ�ϵͳ��Ϣ*/
        if (NAS_MMC_WAIT_REG_RESULT_IND_NULL == NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
        {
            NAS_MMC_SndMsccSysInfo_BgPlmnSearch();
        }
    }

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmTauResultInd(pstLmmTauIndMsg);
    NAS_MMC_SndGmmLmmTauResultInd(pstLmmTauIndMsg);

    /* ����tau��� */
    NAS_MMC_ProcLmmTauResultInd_BgPlmnSearch(pstLmmTauIndMsg);

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* ��֧��IMSʱ�������CS��PSע����������֧��IMSʱ����CS��PS��IMSע������
    �����ע�����ж�CSG���������Ƿ�ɹ� */
#if (FEATURE_ON == FEATURE_CSG)

    /* �����ǰ��CSG���������̣�����CSG������ע���� */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        NAS_MMC_ProcLteCsgRegRslt_BgPlmnSearch(NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch(),
                                        NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch());
    }
#endif

    /* �����ҪAbort,��ʱ�Ĵ������ֻ����Ϊ�ӳٴ�� */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ע������ɺ��յ�ABORT�Ĵ��� */
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcDetachInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));
    pstDetachMsg        = (LMM_MMC_DETACH_IND_STRU*)pstMsg;

    /* ����DETACH��Ϣ��������һ������ */
    NAS_MMC_ProcLmmDetachInd(pstDetachMsg, &enCsAdditionalAction, &enPsAdditionalAction);

    /*����EPS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enPsAdditionalAction);
    }

    /*����CS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction)
    {
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(enCsAdditionalAction);
    }

    /* �����ҪAbort,��ʱ�Ĵ������ֻ����Ϊ�ӳٴ�� */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ע������ɺ��յ�ABORT�Ĵ��� */
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcServiceRsltInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SERVICE_RESULT_IND_STRU                        *pstSerRsltMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstSerRsltMsg     = (LMM_MMC_SERVICE_RESULT_IND_STRU*)pstMsg;

    /* ����LMM��service result���������һ�������� */
    enAdditionalAction = NAS_MMC_ProcLmmServiceRsltInd(pstSerRsltMsg);

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    /* �����ҪAbort,��ʱ�Ĵ������ֻ����Ϊ�ӳٴ�� */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ע������ɺ��յ�ABORT�Ĵ��� */
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSuspendInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SUSPEND_IND_STRU           *pstLSuspendMsg = VOS_NULL_PTR;
    MMC_SUSPEND_CAUSE_ENUM_UINT8        enSuspendCause;

    pstLSuspendMsg = (LMM_MMC_SUSPEND_IND_STRU*)pstMsg;

    /* Lģ�£�����ԭ������GU�²�ͬ��ת��һ�¹���ԭ���� */
    NAS_MMC_ConvertLmmSysChngTypeToMmcType(pstLSuspendMsg->ulSysChngType, &enSuspendCause);

    /* ���ΪOOS���߲�������ȷ, �����������ϵͳ�任 */
    if ((MMC_SUSPEND_CAUSE_PLMNSEARCH == enSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= enSuspendCause))
    {
        NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);
    }
    else
    {
        /* ����SUSPEND��Ϣ�еĲ�ͬ��������Ӧ��״̬�� */
        NAS_MMC_LoadInterSysFsm_BgPlmnSearch(enSuspendCause);
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcMmcSuspendRslt_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;
    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRsltMsg = VOS_NULL_PTR;

    pstSuspendRsltMsg = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

    /* �����ѡ���Ϊʧ��,ԭ��Ϊ��������,����ע������г����������� */
    if ((NAS_MMC_SUSPEND_FAIL == pstSuspendRsltMsg->enRslt)
     && (NAS_MMC_SUSPEND_FAIL_CAUSE_AREA_LOST == pstSuspendRsltMsg->enFailCause))
    {
        /* ֹͣ�ȴ�ע������ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        if (VOS_FALSE == NAS_MML_GetRfAvailFlg())
        {
            /* ���ϲ�״̬�����ͱ�������ʧ�ܣ�����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_TRUE;
        }


        /* �����ǰ��ABORT��־�����˳���ǰ״̬�� */
        if ( NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch() )
        {
            /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
               disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

            if (VOS_TRUE == NAS_MMC_IsVplmnValid())
            {
                /* ��MM��GMM��������ָʾ */
                NAS_MMC_SndGmmPlmnSchInit();
                NAS_MMC_SndMmPlmnSchInit();

                NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

                NAS_MMC_ReCampVPlmn_BgPlmnSearch();
                return VOS_TRUE;
            }

            /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��յȴ�ע������־ */
        NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();

        /* û��ABORT��־��������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        return VOS_TRUE;
    }

    /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */
    enRatType = NAS_MML_GetCurrNetRatType();
    if ((NAS_MML_NET_RAT_TYPE_WCDMA == enRatType)
     || (NAS_MML_NET_RAT_TYPE_GSM   == enRatType))
    {
        /* CSG������ʱ���öԴ������⴦����GU���յ�ע���������ݽ����Ȼ���к������� */

        /* ֹͣ��ǰ�ȴ�EPSע�����Ķ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ���µ�ǰ�ȴ�CSPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
        NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_CS);
        NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

        /* ����CSPS��AdditionalAction */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_CSPS_REG_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);

    }

#if ((FEATURE_ON == FEATURE_CL_INTERWORK) || (FEATURE_ON == FEATURE_UE_MODE_CDMA))
    /* ��ѡ��HRPD��,�˳���������״̬�� */
    if ((NAS_MML_NET_RAT_TYPE_BUTT == enRatType)
     && (NAS_MMC_SUSPEND_SUCCESS == pstSuspendRsltMsg->enRslt))
    {
        /* ֹͣ��ǰ�ȴ�EPSע�����Ķ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_INTER_SYS_HRPD, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
#endif

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitEpsRegRsltIndExpired_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_SndMsccSysInfo_BgPlmnSearch();

    /* ��������������� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        /* ״̬Ǩ�Ƶ��ȴ������ͷ� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_CONN_REL_IND);

        /* �����ȴ������ͷŶ�ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* �����ҪAbort�����л�VPLMN���� */
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
;

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }
    else
    {
        /* ��յȴ�ע������־ */
        NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();

        /* �����ȡ��һ�������ȼ������������ */
        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmAreaLostInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo              = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_CNT_AND_RAT_STRU *pstLastAreaLostPlmnInfo     = VOS_NULL_PTR;
    LMM_MMC_AREA_LOST_IND_STRU         *pstLmmAreaLostMsg           = VOS_NULL_PTR;
    VOS_UINT32                          ulIsRrcSearchPlmn;
    NAS_MML_PLMN_WITH_RAT_STRU          stCurPlmn;

    NAS_MMC_SndMsccSysInfo_BgPlmnSearch();


    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo     = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId = pstCurCampInfo->stLai.stPlmnId;
    stCurPlmn.enRat    = pstCurCampInfo->enNetRatType;

    pstLmmAreaLostMsg       = (LMM_MMC_AREA_LOST_IND_STRU*)pstMsg;
    ulIsRrcSearchPlmn       = NAS_MMC_IsLmmSearchedAreaLostPlmn(pstLmmAreaLostMsg);
    pstLastAreaLostPlmnInfo = NAS_MMC_GetLastAreaLostPLmnInfoCtx_BgPlmnSearch();

    NAS_MMC_UpdateLastAreaLostPlmnWIthRatAndCnt(pstLastAreaLostPlmnInfo, &stCurPlmn.stPlmnId, stCurPlmn.enRat);

    /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                                    NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                    NAS_MMC_GetHighPrioPlmnList());

    NAS_WARNING_LOG2(WUEPS_PID_MMC,"NAS_MMC_RcvLmmAreaLostInd_BgPlmnSearch_WaitEpsRegRsltInd: ulIsRrcSearchPlmn = %d , %d",
                                            ulIsRrcSearchPlmn, NAS_MML_GetLastAreaLostPlmnCnt(pstLastAreaLostPlmnInfo));


    /* ֹͣ�ȴ�ע������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());


    /* �����ǰ����NO RF����״̬��������ʧ����Ϣ��Ǩ�Ƶ�OOC״̬ */
    if (VOS_FALSE == NAS_MML_GetRfAvailFlg())
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvLmmAreaLostInd_BgPlmnSearch_WaitEpsRegRsltInd: RF FALSE");
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ�����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
    /* ���·���״̬ */

    /* ֪ͨTAF��ǰ�ķ���״̬ */

    /* �����ǰ��ABORT��־�����VPLMN */
    if ( NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch() )
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvLmmAreaLostInd_BgPlmnSearch_WaitEpsRegRsltInd: Abort");
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* ���ϲ�״̬�����͸����ȼ���������ϣ�����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ��յȴ�ע������־ */
    NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();
    
    if ( (NAS_MML_GetLastAreaLostPlmnCnt(pstLastAreaLostPlmnInfo) >= NAS_MMC_AREA_LOST_SEARCH_MAX_NUM)
      || (VOS_TRUE == ulIsRrcSearchPlmn) )
    {

        /* ������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvLmmAreaLostInd_BgPlmnSearch_WaitEpsRegRsltInd: NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch false");
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }
    }
    else
    {
        /* ���п���ָ������ */
        NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&pstLastAreaLostPlmnInfo->stPlmnId,
                                                   pstLastAreaLostPlmnInfo->enRat);
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();


    NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLSysInfoInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));
    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_IsNeedUpdateCsgSpecSrchDuplicateRplmnAndEplmn(&stOldCampInfo))
    {
        NAS_MMC_UpdateCsgSpecSrchDuplicateRplmnAndEplmn(NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo());
    }
#endif

    /* פ��С���仯ʱ���ϱ�ϵͳ��Ϣ */


    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);


    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    /*�����Ż���LTE�µ�С�������仯��LMM��ͨ��ϵͳ��Ϣ֪ͨMMC*/
    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo,pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        if (VOS_FALSE == NAS_MMC_GetNeedSndSysInfo_BgPlmnSearch())
        {
            NAS_MMC_SndMsccSysInfo();
        }
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* ת��ϵͳ��Ϣ��GMM,��RABMʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    /* �����CSG�����ѹ�������CSG�ϵ�ע����������ܵ������ϣ��˴��������⴦���յ�ע��
    ����󣬷��ֵ�ǰפ���ڹ����ϣ���Ȼ������Ϊ�����ѳɹ��������������һ��CSG ID������ */

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���VPLMN���뼼����ǰ��֧�֣����轫VPLMN���뼼������ΪSYSCFG���õ�������Ƚ��뼼����
       �Ȼ�ȡ��һ������ʱ���ж�vplmn���뼼���Ƿ���Ч��vplmn��Чʱ��Ҫ��״̬�����Ϊ��Ҫ������
       ����פ�����ٽ��б����ѣ��ú����ֲ�����������������չʹ�� */
    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvMsccImsVoiceCapInd_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMC_IMS_VOICE_CAP_NOTIFY_STRU *pstImsVoiceInd = VOS_NULL_PTR;
    VOS_UINT8                           ucWaitRegRsltFlag;

#if (FEATURE_ON == FEATURE_CSG)
    NAS_MML_CELL_TYPE_ENUM_UINT8        enCurrCampCellType;

    enCurrCampCellType = NAS_MML_GetCurrCampCellType();
#endif

    ucWaitRegRsltFlag   = NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch();
    pstImsVoiceInd      = (MSCC_MMC_IMS_VOICE_CAP_NOTIFY_STRU *)pstMsg;

    /* ����ڵȴ�PS��ע����������ȴ�PSע���� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_PS == (ucWaitRegRsltFlag & NAS_MMC_WAIT_REG_RESULT_IND_PS))
    {
        return VOS_TRUE;
    }

    /* �����ǰ�յ�IMS VOICE����������� */
    if (VOS_TRUE != pstImsVoiceInd->ucRcvServiceChangeIndFlag)
    {
        return VOS_TRUE;
    }

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_IMS);

    /* IMS������ʱ�ж��Ƿ���Ҫdisable LTE */
    if (VOS_FALSE  == pstImsVoiceInd->ucAvail)
    {
        if ((VOS_TRUE == NAS_MMC_IsNeedDisableLte_ImsVoiceNotAvail())
#if (FEATURE_ON == FEATURE_CSG)
          &&(MMC_LMM_CELL_TYPE_MACRO == enCurrCampCellType)
#endif
           )
        {
            NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION);

            NAS_MML_DisableLteCommProc(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS,
                                       MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);            
            
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMsccImsVoiceCapInd_BgPlmnSearch_WaitEpsRegRsltInd: MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE");
        }
    }

    NAS_MMC_SndMsccSysInfo_BgPlmnSearch();

#if (FEATURE_ON == FEATURE_CSG)

    /* �����ǰ��CSG���������̣�����CSG������ע���� */
    if (VOS_TRUE == NAS_MMC_GetCsgSearchingFlag_BgPlmnSearch())
    {
        NAS_MMC_ProcLteCsgRegRslt_BgPlmnSearch(NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch(),
                                        NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch());
    }
#endif


    /* �����ҪAbort,��ʱ�Ĵ������ֻ����Ϊ�ӳٴ�� */
    if (NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ע������ɺ��յ�ABORT�Ĵ��� */
        NAS_MMC_PerformAbortRegFinished_BgPlmnSearch();
    }
    else
    {
        /* �������ҪAbort������CS��PS��dditional Action��һ������ */
        NAS_MMC_PerformAdditionalActionRegFinished_BgPlmnSearch();
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU                *pstAbortMsg = VOS_NULL_PTR;
    NAS_MMC_WAIT_REG_RSLT_IND_ENUM_UINT8   enRegRslt;


    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;

    NAS_MMC_SndMsccSysInfo_BgPlmnSearch();



    /* ������������û�����Ĺػ�����ֱ���˳���ע����״̬�� */
    if (NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ��ǰ����������ϱ�־������״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
    {
        /* ֹͣ�ȴ�ע������ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);


        /* ��ע��������յ�������ϣ���ζ��ע����δ��ɣ�����δ��ɵ���ķ���
           ״̬ΪLIMIT SERVICE������ǰ�ķ���״̬����ΪNORMAL_SERVICE,���������û�
           ָ���Ѵ�ϣ���������뼼������ͬ������£��Ͳ����������ע���� */
        enRegRslt = NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch();
        if ( enRegRslt>= NAS_MMC_WAIT_REG_RESULT_IND_BUTT )
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch:NAS_MMC_WAIT_REG_RESULT_IND_BUTT");
        }
        NAS_MMC_UpdateDomainServiceStatus(enRegRslt,
                                          NAS_MMC_LIMITED_SERVICE);


        if (VOS_TRUE == NAS_MML_IsRrcConnExist())
        {
            /* ���������ͷű�־ */
            NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);

            /* ��LMM���������ͷ����� */
            NAS_MMC_SndLmmRelReq();

            /* ״̬Ǩ�Ƶ��ȴ������ͷ� */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_CONN_REL_IND);

            /* �����ȴ������ͷŶ�ʱ�� */
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
            return VOS_TRUE;
        }

        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLmmMmcStatusInd_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    LMM_MMC_STATUS_IND_STRU            *pstLmmStatusIndMsg = VOS_NULL_PTR;

    pstLmmStatusIndMsg = (LMM_MMC_STATUS_IND_STRU*)pstMsg;

    if ( VOS_FALSE == pstLmmStatusIndMsg->bitOpConnState)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcStatusInd_BgPlmnSearch_WaitEpsConnRelInd:bitOpConnState IS VOS_FALSE");
        return VOS_TRUE;
    }

    /* ����LMM������״̬ */
    NAS_MML_UpdateEpsConnectionStatus(pstLmmStatusIndMsg->ulConnState);

    /* �����Ȼ�����������ӣ������ȴ� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ����������ͷű�����ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

    /* ���Abor����Ϊ�����˳������˳�״̬�� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort�������ӳ��˳������л�VPLMN���� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
           disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

        if (VOS_TRUE == NAS_MMC_IsVplmnValid())
        {
            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

            NAS_MMC_ReCampVPlmn_BgPlmnSearch();
            return VOS_TRUE;
        }

        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
;

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);
    }
    else
    {
        /* ��ǰΪ�����ͷţ�����CS��PS��additional Action��һ������ */
        NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitEpsConnRelIndExpired_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucRelRequestFlg;
    NAS_MMC_ABORT_FSM_TYPE_UINT8        enAbortType;
    VOS_UINT32                          ulCsServiceExist;
    VOS_UINT32                          ulPsServiceExist;

    enAbortType     = NAS_MMC_GetAbortType_BgPlmnSearch();
    ucRelRequestFlg = NAS_MMC_GetRelRequestFlag_BgPlmnSearch();

    /* ��ӡ�쳣 */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitEpsConnRelIndExpired_BgPlmnSearch_WaitEpsConnRelInd: ENTERED");

    /* ���Abort����Ϊ������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == enAbortType)
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��ǰ�����ӳٴ�ϱ�־�����VPlmn */
    if (NAS_MMC_ABORT_FSM_DELAY == enAbortType)
    {
        if (VOS_TRUE == ucRelRequestFlg)
        {
            /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
               disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

            if (VOS_TRUE == NAS_MMC_IsVplmnValid())
            {
                /* ��MM��GMM��������ָʾ */
                NAS_MMC_SndGmmPlmnSchInit();
                NAS_MMC_SndMmPlmnSchInit();

                NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

                NAS_MMC_ReCampVPlmn_BgPlmnSearch();
                return VOS_TRUE;
            }

            /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
;

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }
        else
        {
            /* ���������ͷ�����LMM�����������ȴ������ͷŶ�ʱ�� */
            NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);
            NAS_MMC_SndLmmRelReq();
            (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
        }

        return VOS_TRUE;
    }

    /* ����������ͷ��Ҳ���ҪAbort��������һ�������ȼ����� */
    if (VOS_TRUE == ucRelRequestFlg)
    {
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_FALSE);

        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        return VOS_TRUE;
    }

    ulCsServiceExist = NAS_MML_GetCsServiceExistFlg();
    ulPsServiceExist = NAS_MML_GetPsServiceExistFlg();

    /* ������������ͷŵ�����ҵ���˳�״̬�� */
    if ((VOS_TRUE == ulCsServiceExist)
     || (VOS_TRUE == ulPsServiceExist))
    {
        /* ����״̬�����Ϊ�ɹ�,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }
    else
    {
        /* ���������ͷ�����LMM�����������ȴ������ͷŶ�ʱ�� */
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);
        NAS_MMC_SndLmmRelReq();
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLSysInfoInd_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ��������������ͷ�,������Ҫ������������,ϵͳ��Ϣ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_BgPlmnSearch())
    {
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_IsNeedUpdateCsgSpecSrchDuplicateRplmnAndEplmn(&stOldCampInfo))
    {
        NAS_MMC_UpdateCsgSpecSrchDuplicateRplmnAndEplmn(NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo());
    }
#endif

    /*�����Ż���LTE�µ�С�������仯��LMM��ͨ��ϵͳ��Ϣ֪ͨMMC*/
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        NAS_MMC_SndMsccSysInfo();

    }


    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ��GMM,��RABMʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSuspendInd_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SUSPEND_IND_STRU           *pstLSuspendMsg = VOS_NULL_PTR;
    MMC_SUSPEND_CAUSE_ENUM_UINT8        enSuspendCause;
    VOS_UINT32                          ulRelRequestFlg;

    pstLSuspendMsg = (LMM_MMC_SUSPEND_IND_STRU*)pstMsg;

    ulRelRequestFlg = NAS_MMC_GetRelRequestFlag_BgPlmnSearch();

    /* Lģ�£�����ԭ������GU�²�ͬ��ת��һ�¹���ԭ���� */
    NAS_MMC_ConvertLmmSysChngTypeToMmcType(pstLSuspendMsg->ulSysChngType, &enSuspendCause);

    /* ����������ͷŻ����Ѿ����յ�Abort��Ϣ,�����������ϵͳ��ѡ */
    if ((NAS_MMC_ABORT_BUTT != NAS_MMC_GetAbortType_BgPlmnSearch())
     || (VOS_TRUE == ulRelRequestFlg)
     || (MMC_SUSPEND_CAUSE_PLMNSEARCH == enSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= enSuspendCause))
    {
        NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);
    }
    else
    {
        /* ����SUSPEND��Ϣ�еĲ�ͬ��������Ӧ��״̬�� */
        NAS_MMC_LoadInterSysFsm_BgPlmnSearch(enSuspendCause);
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcSuspendRslt_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;
    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRsltMsg = VOS_NULL_PTR;

    pstSuspendRsltMsg = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

    /* �����ѡ���Ϊʧ��,ԭ��Ϊ��������,�����������ͷŴ��� */
    if ((NAS_MMC_SUSPEND_FAIL == pstSuspendRsltMsg->enRslt)
     && (NAS_MMC_SUSPEND_FAIL_CAUSE_AREA_LOST == pstSuspendRsltMsg->enFailCause))
    {
        /* ֹͣ��ǰ�ȴ�eps�����ͷŶ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        if (VOS_FALSE == NAS_MML_GetRfAvailFlg())
        {
            /* ���ϲ�״̬�����ͱ�������ʧ�ܣ�����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_TRUE;
        }

        /* �����ͷ�ʱ�յ�SUSPEND CNF��϶�û��ABORT��־��������һ�������ȼ����� */
        if (VOS_FALSE == NAS_MMC_ContinueBgSearchProc_BgPlmnSearch())
        {
            /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

            /* �˳�״̬�� */
            NAS_MMC_FSM_QuitFsmL2();
            return VOS_TRUE;
        }

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        return VOS_TRUE;
    }

    /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */
    enRatType = NAS_MML_GetCurrNetRatType();
    if ((NAS_MML_NET_RAT_TYPE_WCDMA == enRatType)
     || (NAS_MML_NET_RAT_TYPE_GSM == enRatType))
    {
        /* CSG������ʱ���öԴ������⴦����GU���յ�ע���������ݽ����Ȼ���к������� */

        /* ֹͣ��ǰ�ȴ�eps�����ͷŶ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* ���µ�ǰ�ȴ�CSPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch();

        /* ����CSPS��AdditionalAction */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_CSPS_REG_IND);

        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);

        return VOS_TRUE;
    }

#if ((FEATURE_ON == FEATURE_CL_INTERWORK) || (FEATURE_ON == FEATURE_UE_MODE_CDMA))
    /* ��ѡ��HRPD��,�˳���������״̬�� */
    if ((NAS_MML_NET_RAT_TYPE_BUTT == enRatType)
        && (NAS_MMC_SUSPEND_SUCCESS == pstSuspendRsltMsg->enRslt))
    {
        /* ֹͣ��ǰ�ȴ�EPS�����ͷŵĶ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ�����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_INTER_SYS_HRPD, VOS_FALSE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
#endif

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

    /* ����������Ӳ����ڿ��ܷ������� */
    NAS_MMC_PerformAdditionalActionConnRel_BgPlmnSearch();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���VPLMN���뼼����ǰ��֧�֣����轫VPLMN���뼼������ΪSYSCFG���õ�������Ƚ��뼼����
       �Ȼ�ȡ��һ������ʱ���ж�vplmn���뼼���Ƿ���Ч��vplmn��Чʱ��Ҫ��״̬�����Ϊ��Ҫ������
       ����פ�����ٽ��б����ѣ��ú����ֲ�����������������չʹ�� */
    return VOS_TRUE;

}



VOS_UINT32 NAS_MMC_RcvLmmMmcDetachInd_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));
    pstDetachMsg        = (LMM_MMC_DETACH_IND_STRU*)pstMsg;

    /* ����DETACH��Ϣ��������һ������ */
    NAS_MMC_ProcLmmDetachInd(pstDetachMsg, &enCsAdditionalAction, &enPsAdditionalAction);

    /*����EPS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enPsAdditionalAction);
    }

    /*����CS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction)
    {
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(enCsAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvLmmMmcDetachCnf_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_CNF_STRU            *pstDetachCnfMsg = VOS_NULL_PTR;

    pstDetachCnfMsg = (LMM_MMC_DETACH_CNF_STRU*)pstMsg;

    /* ����detach������Ǽ�Ȩ���ܣ�����Ҫ��״̬����Ԥ�����оͿ��Դ��� */
    if ( MMC_LMM_DETACH_RSLT_AUTH_REJ != pstDetachCnfMsg->ulDetachRslt)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcDetachCnf_BgPlmnSearch_WaitEpsConnRelInd: Unexpected detach result!");

        return VOS_TRUE;
    }

    /*����EPS����һ���Ķ�������*/
    NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL);

    /*����CS����һ���Ķ�������*/
    NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;
    VOS_UINT8                           ucRelRequestFlg;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ������������û�����Ĺػ�����ֱ���˳��������ͷŽ��״̬�� */
    if (NAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)

    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    ucRelRequestFlg = NAS_MMC_GetRelRequestFlag_BgPlmnSearch();

    /* �����ǰ����������ϱ�־���Ҳ��������ͷţ��������ͷ� */
    if ((NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
     && (VOS_FALSE == ucRelRequestFlg))
    {
        /* ���������ͷű�־ */
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);

        /* ��LMM���������ͷ����� */
        NAS_MMC_SndLmmRelReq();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcTauResultInd_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg = VOS_NULL_PTR;

    pstLmmTauIndMsg = (LMM_MMC_TAU_RESULT_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmTauResultInd(pstLmmTauIndMsg);
    NAS_MMC_SndGmmLmmTauResultInd(pstLmmTauIndMsg);

    /* L�����ȼ�����ע������� ,�û�����syscfg��Ҫ����������MMC�������ͷ����ӣ�
      �ڵȴ������ͷ�״̬��L���ܻ���ע��������Ҫ���� */
    NAS_MMC_ProcLmmTauResultInd_BgPlmnSearch(pstLmmTauIndMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcAttachInd_BgPlmnSearch_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg = VOS_NULL_PTR;
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enUeOperationMode;

    enUeOperationMode   =  NAS_MML_GetLteUeOperationMode();
    pstLmmAttachIndMsg  = (LMM_MMC_ATTACH_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* LMM������δ�ظ�search cnfʱ�յ��û�detach ps����󣬺���������ע���mmc�ظ�attach result
       ΪMMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW���������������MMC_LMM_ATT_TYPE_BUTT��
       MMC��Ҫ���ݵ�ǰue operation mode�������������¸�ֵ */
    if ((MMC_LMM_ATT_TYPE_BUTT == pstLmmAttachIndMsg->ulReqType)
     && (MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW == pstLmmAttachIndMsg->ulAttachRslt))
    {
        pstLmmAttachIndMsg->ulReqType = NAS_MMC_ConvertLmmAttachReqType(enUeOperationMode);
    }

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmAttachInd(pstLmmAttachIndMsg);
    NAS_MMC_SndGmmLmmAttachInd(pstLmmAttachIndMsg);

    /* L�����ȼ�����ע������� ,�û�����syscfg��Ҫ����������MMC�������ͷ����ӣ�
      �ڵȴ������ͷ�״̬��L���ܻ���ע��������Ҫ���� */
    NAS_MMC_ProcLmmAttachInd_BgPlmnSearch(pstLmmAttachIndMsg);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    /* ���Abort������������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
    {
        /* ֹͣ�ȴ������ظ���ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

        /* ��ǰ���뼼������ֹͣ�������� */
        NAS_MMC_SndLmmPlmnSrchStopReq();

        /* ״̬Ǩ�Ƶ���VPlmnʱ�ȴ�ֹͣ�����ظ�״̬ */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_PLMN_STOP_CNF_RECAMP_VPLMN);

        /* �����ȴ�ֹͣ�����ظ���ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);
    }


    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnCnf_BgPlmnSearch_WaitLmmPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstLmmSrchCnfMsg = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList   = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnList;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astSearchedPlmnListInfo[NAS_MML_MAX_RAT_NUM];
    RRC_PLMN_ID_LIST_STRU                                  *pstRrcPlmnList = VOS_NULL_PTR;
    VOS_UINT32                                              i;

    pstLmmSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU *)pstMsg;

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_NO_RF == pstLmmSrchCnfMsg->enRlst)
    {
       /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);

        return VOS_TRUE;
    }

    /* ��������ɹ� */
    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_SUCC == pstLmmSrchCnfMsg->enRlst)
    {
        /* ״̬Ǩ�Ƶ���VPlmnʱ�ȴ�Lϵͳ��Ϣ״̬ */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_SYSINFO_IND_RECAMP_VPLMN);

        /* �����ȴ�ϵͳ��Ϣ��ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO, TI_NAS_MMC_WAIT_LMM_SYS_INFO_LEN);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        pstPrioRatList  = NAS_MML_GetMsPrioRatList();

        NAS_MMC_InitSearchedPlmnListInfo(astSearchedPlmnListInfo);

        for ( i = 0; i < pstPrioRatList->ucRatNum; i++ )
        {
            if (NAS_MML_NET_RAT_TYPE_LTE == pstPrioRatList->aucRatPrio[i])
            {
                pstRrcPlmnList   = (RRC_PLMN_ID_LIST_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                               sizeof(RRC_PLMN_ID_LIST_STRU));

                if (VOS_NULL_PTR == pstRrcPlmnList)
                {
                    NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmPlmnCnf_BgPlmnSearch_WaitLmmPlmnSearchCnfReCampVPlmn:ERROR: Memory Alloc Error");
                    return VOS_TRUE;
                }

                /* ��������Ϣ��Я����������Ϣת��Ϊ�ڲ���Ϣ��������Ϣ */
                NAS_MMC_ConvertLmmPlmnIdListToGUFormat(&(pstLmmSrchCnfMsg->stPlmnIdList),
                                                       pstRrcPlmnList);

                stSearchedPlmnList.stSearchedType.ucHistorySearchedFlg  = VOS_TRUE;
                stSearchedPlmnList.stSearchedType.ucPrefBandSearchedFlg = VOS_TRUE;
                stSearchedPlmnList.stSearchedType.ucSpecSearchedFlg     = VOS_TRUE;
                stSearchedPlmnList.stSearchedType.ucReserve             = 0;

                astSearchedPlmnListInfo[i].enRatType    = NAS_MML_NET_RAT_TYPE_LTE;

                NAS_MMC_UpdateSearchedPlmnListInfo(&(stSearchedPlmnList.stSearchedType),
                                                   pstRrcPlmnList,
                                                   astSearchedPlmnListInfo);

                /* ��ģ��CoverageType�ý�������,�����ݸߵ�������PLMN */
                NAS_MMC_ConvertLmmCoverTypeToNasFormat(pstLmmSrchCnfMsg->enCoverageType, &(astSearchedPlmnListInfo[i].enCoverType));
                PS_MEM_FREE(WUEPS_PID_MMC, pstRrcPlmnList);
            }
            else
            {
                astSearchedPlmnListInfo[i].enRatType    = pstPrioRatList->aucRatPrio[i];
                astSearchedPlmnListInfo[i].stSearchedType.ucHistorySearchedFlg  = NAS_MMC_GetHistorySearchedFlag_BgPlmnSearch(pstPrioRatList->aucRatPrio[i]);
            }
        }


        /* ���ϲ�״̬������BG����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, astSearchedPlmnListInfo);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitLmmSrchCnf_BgPlmnSearch_WaitLmmPlmnSearchCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmSrchCnf_BgPlmnSearch_WaitLmmPlmnSearchCnfReCampVPlmn: Timer Expired");

    /* ��LMM����ֹͣ����������Ϣ */
    NAS_MMC_SndLmmPlmnSrchStopReq();

    /* Ǩ�Ƶ��ȴ�ֹͣ�����ظ�״̬,�����ȴ�ֹͣ�����ظ�״̬��ʱ�� */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_PLMN_STOP_CNF_RECAMP_VPLMN);
    (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmSuspendCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSuspendCnf_BgPlmnSearch_WaitLmmSuspendCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SUSPEND_CNF_STRU           *pstSuspendCnf = VOS_NULL_PTR;

    pstSuspendCnf = (LMM_MMC_SUSPEND_CNF_STRU*)pstMsg;

    /* ֹͣ�ȴ�����ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF);

    /* �������ʧ�ܣ���λ���� */
    if (MMC_LMM_FAIL == pstSuspendCnf->ulRst)
    {
        /* �쳣��ӡ */
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmSuspendCnf_BgPlmnSearch_WaitLmmSuspendCnfReCampVPlmn: SUSPEND FAIL !");

        /* ���ɻָ�����,���õ���ӿ�RESET */

        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_RECEIVE_LMM_SUSPEND_FAILURE);

        return VOS_TRUE;
    }

    /* ���õ�ǰ���뼼��ΪBUTT */
    NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

    /* ���Abort����Ϊ������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
        return VOS_TRUE;
    }

    /* ��VPLMNʱ��Ҫ�жϵ�ǰ��VPLMN�Ľ��뼼��ʱ�Ƿ���Ч,��Ϊ�п��ܵ�ǰ
       disable LTE��SYSCFG���ú�VPLMN�Ľ��뼼����Ϊ��Ч */

    if (VOS_TRUE == NAS_MMC_IsVplmnValid())
    {
        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

        NAS_MMC_ReCampVPlmn_BgPlmnSearch();
        return VOS_TRUE;
    }

    /* ���Abort����Ϊ�ӳ���,�˳�״̬��,�������� */
    if (NAS_MMC_ABORT_FSM_DELAY == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* �д�ϱ��,���ϲ�״̬������ABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }
    else
    {
        /* �޴�ϱ��,���ϲ�״̬������FAIL,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();
    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLmmPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU *)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ����Abort���� */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnStopCnf_BgPlmnSearch_WaitLmmPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF);

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnSearchCnf_BgPlmnSearch_WaitLmmPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstSrchCnfMsg            = VOS_NULL_PTR;

    pstSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU *)pstMsg;

    /* ֹͣ�ȴ������ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF);

    /* ������no rf,����״̬�� */
    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->enRlst)
    {
        /* ��NO RF���д��� */
        NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);

        return VOS_TRUE;
    }

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitLmmStopCnf_BgPlmnSearch_WaitLmmPlmnStopCnfReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmStopCnf_BgPlmnSearch_WaitLmmPlmnStopCnfReCampVPlmn: Timer Expired");

    if (NAS_MMC_ABORT_BUTT == NAS_MMC_GetAbortType_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);
    }
    else
    {
        /* ����״̬�����Ϊ����ֹ,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_ABORTED, VOS_FALSE, VOS_NULL_PTR);
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLSysInfoInd_BgPlmnSearch_WaitLSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MMC_ABORT_FSM_TYPE_UINT8                            enAbortType;
    NAS_MMC_HIGH_PRIO_PLMN_SEARCH_RSLT_ENUM_UINT32          enSearchRslt;
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

    PS_MEM_SET(&stOldCampInfo, 0, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ֹͣ�ȴ�ϵͳ��Ϣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ����CSPS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* ��ȡHIGH_PRIO_PLMN_SEARCH״̬���������е�ABORT��־ */
    enAbortType = NAS_MMC_GetAbortType_BgPlmnSearch();

    /* ���Abort����Ϊ������ */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == enAbortType)
    {
        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���Abort����Ϊ�ӳ��� */
    if (NAS_MMC_ABORT_FSM_DELAY == enAbortType)
    {
        /* �����ȼ�����״̬�����Ϊ��ֹ */
        enSearchRslt = NAS_MMC_BG_PLMN_SEARCH_ABORTED;
    }
    else
    {
        /* �����ȼ�����״̬�����Ϊ�ɹ� */
        enSearchRslt = NAS_MMC_BG_PLMN_SEARCH_SUCC;
    }


    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_CSG)
    if (VOS_TRUE == NAS_MMC_IsNeedUpdateCsgSpecSrchDuplicateRplmnAndEplmn(&stOldCampInfo))
    {
        NAS_MMC_UpdateCsgSpecSrchDuplicateRplmnAndEplmn(NAS_MMC_GetCsgSpecSrchDuplicateRplmnAndEplmnInfo());
    }

#endif


    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();


    /*�����Ż���LTE�µ�С�������仯��LMM��ͨ��ϵͳ��Ϣ֪ͨMMC*/
    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMsccSysInfo();

    }


    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMsccCampOnInd(VOS_TRUE);

    /* ת��ϵͳ��Ϣ��GMM,��RABMʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    /* ����״̬�����,����Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(enSearchRslt, VOS_FALSE, VOS_NULL_PTR);

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();


    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitLSysInfoExpired_BgPlmnSearch_WaitLSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_TYPE_UINT8        enAbortType;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLSysInfoExpired_BgPlmnSearch_WaitLSysInfoIndReCampVPlmn: Timer Expired");

    enAbortType = NAS_MMC_GetAbortType_BgPlmnSearch();

    /* �����ҪAbort */
    if ( NAS_MMC_ABORT_BUTT != enAbortType)
    {
        /* ����״̬�����ΪABORT,����Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_ABORTED,
                                      VOS_FALSE, VOS_NULL_PTR );
    }
    else
    {
        /* ����״̬�����Ϊʧ��,��Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch( NAS_MMC_BG_PLMN_SEARCH_FAIL,
                                      VOS_TRUE, VOS_NULL_PTR );
    }

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_BgPlmnSearch_WaitLSysInfoIndReCampVPlmn(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ��¼״̬����������Abort�¼����� */
    NAS_MMC_SetAbortEventType_BgPlmnSearch(pstAbortMsg->ulEventType);

    /* ��¼Abort��־ */
    NAS_MMC_SetAbortType_BgPlmnSearch(pstAbortMsg->enAbortFsmType);

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_ProcLmmCombinedAttachInd_BgPlmnSearch(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegRsltCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));

    enPsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;

    NAS_MMC_ConverLmmCombinedAttachRsltToMMLCause(pstLmmAttachIndMsg,&enPsRegRsltCause, &enCsRegRsltCause);

    /* ���EPS�����ATTACH��־ */
    NAS_MML_SetEpsEmcAttachFlg(VOS_FALSE);

    switch (pstLmmAttachIndMsg->ulAttachRslt)
    {
        case MMC_LMM_ATT_RSLT_SUCCESS:
            NAS_MMC_ProcLmmCombinedAttachInd_AttRsltSucc(pstLmmAttachIndMsg,
                       enCsRegRsltCause, &enCsAdditionalAction, &enPsAdditionalAction);

            break;

        case MMC_LMM_ATT_RSLT_FAILURE:
            if (VOS_FALSE == pstLmmAttachIndMsg->bitOpAtmpCnt)
            {
                enPsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
                enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;


                NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_CS_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
            }
            else
            {
                /* EPS/CS ��ע��ʧ�� */
                NAS_MMC_ProcCombinedAttachEpsRegFail(LMM_MMC_ATTACH_IND,
                                                    pstLmmAttachIndMsg,
                                                    &enPsAdditionalAction,
                                                    &enCsAdditionalAction);
            }
            break;

        case MMC_LMM_ATT_RSLT_ESM_FAILURE:
        case MMC_LMM_ATT_RSLT_MO_DETACH_FAILURE:
        case MMC_LMM_ATT_RSLT_MT_DETACH_FAILURE:
        case MMC_LMM_ATT_RSLT_T3402_RUNNING:
        case MMC_LMM_ATT_RSLT_TIMER_EXP:
        case MMC_LMM_ATT_RSLT_ACCESS_BAR:
        case MMC_LMM_ATT_RSLT_FORBID_PLMN:
        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_RPOS:
        case MMC_LMM_ATT_RSLT_FORBID_PLMN_FOR_GPRS:
        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_ROAMING:
        case MMC_LMM_ATT_RSLT_AUTH_REJ:
        case MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW:
            /* EPS/CS ��ע��ʧ�� */
            NAS_MMC_ProcCombinedAttachEpsRegFail(LMM_MMC_ATTACH_IND,
                                                pstLmmAttachIndMsg,
                                                &enPsAdditionalAction,
                                                &enCsAdditionalAction);
            break;

        case MMC_LMM_ATT_RSLT_CN_REJ:
            if ((VOS_TRUE == pstLmmAttachIndMsg->bitOpCnRslt)
             && (MMC_LMM_ATT_CN_RSLT_EPS_ONLY == pstLmmAttachIndMsg->ulCnRslt))
            {
                /* ��ATTACH��������Ϊ���ϣ�������Ӧ��ATTACH�������ΪEPS ONLY��
                   ��ԭ��ֵ��ΪЭ��24301 5.5.1.3.4.3�½����л���û��Я��ԭ��ֵ��
                   ��LMMͨ��ATTACH�����MMC_LMM_ATT_RSLT_CN_REJ֪ͨMMC��
                   ��Я����Ӧ��ATTACH�������͡�������Ӧ��ATTCH������͡�ԭ��ֵ��
                   ��Я�������ԭ����attach attempt counterֵ */
                NAS_MMC_ProcLmmCombinedAttachInd_AttRsltEpsOnlySucc(pstLmmAttachIndMsg,enCsRegRsltCause,
                                  &enCsAdditionalAction, &enPsAdditionalAction);
            }
            else
            {
                /* EPS/CS ��ע��ʧ�� */
                NAS_MMC_ProcCombinedAttachEpsRegFail(LMM_MMC_ATTACH_IND,
                                                    pstLmmAttachIndMsg,
                                                    &enPsAdditionalAction,
                                                    &enCsAdditionalAction);
            }
            break;

        default:
            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedAttachInd_BgPlmnSearch:Unexpected attach result!");
            enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();
            enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
            break;

    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enPsAdditionalAction);
    }

    /* AdditionalAction��Ч������CS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction )
    {
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(enCsAdditionalAction);
    }


    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}



VOS_VOID NAS_MMC_ProcLmmEpsOnlyAttachInd_BgPlmnSearch(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    VOS_UINT32                                              ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU                                   *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stLEplmnInfo;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;
    VOS_UINT8                                               ucSimCsRegStatus;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));

    NAS_MMC_ConverLmmAttachRsltToMMLCause(pstLmmAttachIndMsg, &enRegRsltCause);

    /* ���EPS�����ATTACH��־ */
    NAS_MML_SetEpsEmcAttachFlg(VOS_FALSE);

    if (MMC_LMM_ATT_RSLT_SUCCESS == pstLmmAttachIndMsg->ulAttachRslt)
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        NAS_MMC_SaveRegRsltCtx(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);

        /* �ȴ���EPLMN LIST���ڴ���ע��״̬���ϱ�*/
        if ( VOS_TRUE == pstLmmAttachIndMsg->bitOpEplmn)
        {
            ulEquPlmnNum        = pstLmmAttachIndMsg->stEplmnList.ulPlmnNum;
            pstLmmEquPlmnList   = pstLmmAttachIndMsg->stEplmnList.astEplmnList;

            /* ��LMM��PLMN ID��ʽת��ΪGU�ĸ�ʽ */
            NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnInfo.astEquPlmnAddr));
            NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnInfo.astEquPlmnAddr);

            NAS_MMC_WriteEplmnNvim();
        }

        NAS_NORMAL_LOG(WUEPS_PID_MMC,"NAS_MMC_ProcLmmEpsOnlyAttachInd_BgPlmnSearch: Update Reg state and Service state");
        NAS_MMC_UpdateServiceState_RegSucc(NAS_MSCC_PIF_SRVDOMAIN_PS, NAS_MMC_NORMAL_SERVICE);

        /* EPS����ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR, NAS_MMC_IsEpsEmergencyAttachType(pstLmmAttachIndMsg->ulReqType));

        /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
        if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
         && (VOS_TRUE == ucSimCsRegStatus))
        {
            NAS_MMC_RecordOosEvent(NAS_ERR_LOG_OOS_EVENT_EPS_ONLY_ATTACH);

            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        /*ע��: ��Ҫ�ȸ���EPLMN LIST���ڴ���ע��״̬���ϱ�,��ΪNAS_MMC_UpdatePsRegStatePsRegSucc()�����л����EPLMN LIST�ϱ�ע��״̬ */

        NAS_MMC_UpdateGURegRlstRPlmnIdInNV();

#if (FEATURE_ON == FEATURE_CSG)
        /* פ����PLMN1 CSG ID1��С���Ϻ�����ѡ����������CSGС��ע��ɹ�ʱ�����duplicate RPLMN��EPLMN��Ϣ��*/
        if (VOS_TRUE == NAS_MMC_IsNeedInitCsgSpecSrchDuplicateRplmnAndEplmnInfo())
        {
            NAS_MMC_InitCsgSpecSrchDuplicateRplmnAndEplmnInfo();
        }
#endif

        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                                  NAS_MML_GetSimPsRegStatus());
    }
    else if ((MMC_LMM_ATT_RSLT_FAILURE == pstLmmAttachIndMsg->ulAttachRslt)
          && (VOS_FALSE == pstLmmAttachIndMsg->bitOpAtmpCnt))
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
    }
    else
    {
        enAdditionalAction= NAS_MMC_ProcEpsAttachRegFail(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);
    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}



VOS_VOID NAS_MMC_ProcLmmAttachInd_BgPlmnSearch(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    switch (pstLmmAttachIndMsg->ulReqType)
    {
        case MMC_LMM_ATT_TYPE_COMBINED_EPS_IMSI:

            /* ����lmm������attach��� */
            NAS_MMC_ProcLmmCombinedAttachInd_BgPlmnSearch(pstLmmAttachIndMsg);
            break;

        case MMC_LMM_ATT_TYPE_EPS_ONLY:

            /* ����lmm�ķ�����attach��EPS onlyע��Ľ�� */
            NAS_MMC_ProcLmmEpsOnlyAttachInd_BgPlmnSearch(pstLmmAttachIndMsg);
            break;

        case MMC_LMM_ATT_TYPE_EPS_EMERGENCY:

            /* ����LMM��EPS�����ATTACH�Ľ�� */
            NAS_MMC_ProcLmmEpsEmergencyAttachInd_BgPlmnSearch(pstLmmAttachIndMsg);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmAttachInd_BgPlmnSearch:Unexpected attach req type!");
            break;

    }

    return;
}


VOS_VOID  NAS_MMC_ProcLmmCombinedTauResultInd_BgPlmnSearch(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegRsltCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));
    enPsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;

    NAS_MMC_ConverLmmCombinedTauRsltToMMLCause(pstLmmTauIndMsg, &enPsRegRsltCause, &enCsRegRsltCause);

    NAS_MMC_ProcLmmCombinedTauResult(pstLmmTauIndMsg, enCsRegRsltCause,
                                        &enPsAdditionalAction, &enCsAdditionalAction);

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enPsAdditionalAction);
    }

    /* AdditionalAction��Ч������CS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction )
    {
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(enCsAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}


VOS_VOID  NAS_MMC_ProcLmmEpsOnlyTauResultInd_BgPlmnSearch(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    VOS_UINT32                          ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU               *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stLEplmnAddr;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stSndEquPlmnInfo;
    VOS_UINT8                           ucSimCsRegStatus;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8          enCsRegAdditionalAction;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));

    NAS_MMC_ConverTauResultToMMLCause(pstLmmTauIndMsg, &enRegRsltCause);

    if (MMC_LMM_TAU_RSLT_SUCCESS == pstLmmTauIndMsg->ulTauRst)
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        /* ����ע������Ϣ */
        NAS_MMC_SaveRegRsltCtx(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);

        /*ע��: ��Ҫ�ȸ���EPLMN LIST���ڴ���ע��״̬���ϱ�,��ΪNAS_MMC_UpdatePsRegStatePsRegSucc()�����л����EPLMN LIST�ϱ�ע��״̬ */
        /* ����EPLMN */
        if (VOS_TRUE == pstLmmTauIndMsg->bitOpEplmn)
        {
            ulEquPlmnNum        = pstLmmTauIndMsg->stEplmnList.ulPlmnNum;
            pstLmmEquPlmnList   = pstLmmTauIndMsg->stEplmnList.astEplmnList;

            /* ��LMM��PLMN ID��ʽת��ΪMMC�ĸ�ʽ */
            NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnAddr.astEquPlmnAddr));

            NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnAddr.astEquPlmnAddr);

            NAS_MMC_WriteEplmnNvim();
        }

        NAS_NORMAL_LOG(WUEPS_PID_MMC,"NAS_MMC_ProcLmmEpsOnlyTauResultInd_BgPlmnSearch: Update Reg state and Service state");
        NAS_MMC_UpdateServiceState_RegSucc(NAS_MSCC_PIF_SRVDOMAIN_PS, NAS_MMC_NORMAL_SERVICE);

        /* EPS����ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR, VOS_FALSE);

        /* �˴�����CS additionԭ��������TAU��EPS�ɹ�ʱ,LMM����T3411�ٴγ���TAU������,
           �û�Detach CS,LMM��ֱ�ӱ�TAU�ɹ�,���ٳ�������TAU */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON);

        /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
        if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
         && (VOS_TRUE == ucSimCsRegStatus))
        {
            NAS_MMC_RecordOosEvent(NAS_ERR_LOG_OOS_EVENT_EPS_ONLY_ATTACH);

            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        /*ע��: ��Ҫ�ȸ���EPLMN LIST���ڴ���ע��״̬���ϱ�,��ΪNAS_MMC_UpdatePsRegStatePsRegSucc()�����л����EPLMN LIST�ϱ�ע��״̬ */

        NAS_MMC_UpdateGURegRlstRPlmnIdInNV();

#if (FEATURE_ON == FEATURE_CSG)
        /* פ����PLMN1 CSG ID1��С���Ϻ�����ѡ����������CSGС��ע��ɹ�ʱ�����duplicate RPLMN��EPLMN��Ϣ��*/
        if (VOS_TRUE == NAS_MMC_IsNeedInitCsgSpecSrchDuplicateRplmnAndEplmnInfo())
        {
            NAS_MMC_InitCsgSpecSrchDuplicateRplmnAndEplmnInfo();
        }
#endif

        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                                  NAS_MML_GetSimPsRegStatus());
    }
    else if ((MMC_LMM_TAU_RSLT_FAILURE == pstLmmTauIndMsg->ulTauRst)
          && (VOS_FALSE == pstLmmTauIndMsg->bitOpAtmpCnt))
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;


        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
    }
    else if (MMC_LMM_TAU_RSLT_T3402_RUNNING_EPS_ONLY_SUCC == pstLmmTauIndMsg->ulTauRst)
    {
        NAS_MMC_ProcCombinedEpsRegFailCauseT3402RunningEpsOnlySucc(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg,
                                          &enAdditionalAction, &enCsRegAdditionalAction);
    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcEpsTauRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);
    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}


VOS_VOID  NAS_MMC_ProcLmmTauResultInd_BgPlmnSearch(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    switch (pstLmmTauIndMsg->ulReqType)
    {
        case MMC_LMM_COMBINED_TA_LA_UPDATING:
        case MMC_LMM_COMBINED_TA_LA_WITH_IMSI:
        case MMC_LMM_CS_PS_PERIODIC_UPDATING:

            /* ����lmm������tau��� */
            NAS_MMC_ProcLmmCombinedTauResultInd_BgPlmnSearch(pstLmmTauIndMsg);
            break;

        case MMC_LMM_TA_UPDATING:
        case MMC_LMM_PS_PERIODIC_UPDATING:
            if (VOS_TRUE == NAS_MML_GetEpsEmcAttachFlg())
            {
                /* ����LMM��EPS�����ATTACH���ϱ���TAU��� */
                NAS_MMC_ProcLmmEpsEmergencyTauResultInd_BgPlmnSearch(pstLmmTauIndMsg);
            }
            else
            {
                /* ����lmm�ķ�����tau��EPS onlyע��Ľ�� */
                NAS_MMC_ProcLmmEpsOnlyTauResultInd_BgPlmnSearch(pstLmmTauIndMsg);
            }
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmTauResultInd_BgPlmnSearch:Unexpected tau req type!");
            break;

    }

    return;
}


VOS_VOID NAS_MMC_ProcLmmEpsEmergencyAttachInd_BgPlmnSearch(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    VOS_UINT32                                              ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU                                   *pstLmmEquPlmnList   = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stLEplmnInfo;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo      = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;
    VOS_UINT8                                               ucSimCsRegStatus;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));

    NAS_MMC_ConverLmmAttachRsltToMMLCause(pstLmmAttachIndMsg, &enRegRsltCause);

    /* ���EPS�����ATTACH��־ */
    NAS_MML_SetEpsEmcAttachFlg(VOS_FALSE);

    if (MMC_LMM_ATT_RSLT_SUCCESS == pstLmmAttachIndMsg->ulAttachRslt)
    {
        /* ����ATTACH�ɹ�������EPS�����ATTACH��־ */
        NAS_MML_SetEpsEmcAttachFlg(VOS_TRUE);

        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        NAS_MMC_SaveRegRsltCtx(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);

        /* �ȴ���EPLMN LIST���ڴ���ע��״̬���ϱ�*/
        if ( VOS_TRUE == pstLmmAttachIndMsg->bitOpEplmn)
        {
            ulEquPlmnNum        = pstLmmAttachIndMsg->stEplmnList.ulPlmnNum;
            pstLmmEquPlmnList   = pstLmmAttachIndMsg->stEplmnList.astEplmnList;

            /* ��LMM��PLMN ID��ʽת��ΪGU�ĸ�ʽ */
            NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnInfo.astEquPlmnAddr));

            /* ��EPLMN��Ϣ�������ڴ��� */
            NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnInfo.astEquPlmnAddr);
        }

        /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
        if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
         && (VOS_TRUE == ucSimCsRegStatus))
        {
            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                                  NAS_MML_GetSimPsRegStatus());

    }
    else if ((MMC_LMM_ATT_RSLT_FAILURE == pstLmmAttachIndMsg->ulAttachRslt)
          && (VOS_FALSE == pstLmmAttachIndMsg->bitOpAtmpCnt))
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
    }
    else
    {
        enAdditionalAction= NAS_MMC_ProcEpsAttachRegFail(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);
    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}


VOS_VOID  NAS_MMC_ProcLmmEpsEmergencyTauResultInd_BgPlmnSearch(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    VOS_UINT32                                              ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU                                   *pstLmmEquPlmnList   = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stLEplmnAddr;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo      = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;
    VOS_UINT8                                               ucSimCsRegStatus;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(NAS_MML_EQUPLMN_INFO_STRU));

    NAS_MMC_ConverTauResultToMMLCause(pstLmmTauIndMsg, &enRegRsltCause);

    if (MMC_LMM_TAU_RSLT_SUCCESS == pstLmmTauIndMsg->ulTauRst)
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

        /* ����ע������Ϣ */
        NAS_MMC_SaveRegRsltCtx(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);

        /*ע��: ��Ҫ�ȸ���EPLMN LIST���ڴ���ע��״̬���ϱ�,��ΪNAS_MMC_UpdatePsRegStatePsRegSucc()�����л����EPLMN LIST�ϱ�ע��״̬ */
        /* ����EPLMN */
        if (VOS_TRUE == pstLmmTauIndMsg->bitOpEplmn)
        {
            ulEquPlmnNum        = pstLmmTauIndMsg->stEplmnList.ulPlmnNum;
            pstLmmEquPlmnList   = pstLmmTauIndMsg->stEplmnList.astEplmnList;

            /* ��LMM��PLMN ID��ʽת��ΪMMC�ĸ�ʽ */
            NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnAddr.astEquPlmnAddr));

            NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnAddr.astEquPlmnAddr);
        }

        /* �˴�����CS additionԭ��������TAU��EPS�ɹ�ʱ,LMM����T3411�ٴγ���TAU������,
           �û�Detach CS,LMM��ֱ�ӱ�TAU�ɹ�,���ٳ�������TAU */
        NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON);

        /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
        if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
         && (VOS_TRUE == ucSimCsRegStatus))
        {
            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                                  NAS_MML_GetSimPsRegStatus());
    }
    else if ((MMC_LMM_TAU_RSLT_FAILURE == pstLmmTauIndMsg->ulTauRst)
          && (VOS_FALSE == pstLmmTauIndMsg->bitOpAtmpCnt))
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

        NAS_MMC_SndMsccRegResultInd(NAS_MSCC_PIF_SRVDOMAIN_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcEpsTauRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);
    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();

        /* ֪ͨCSS GEO��Ϣ */
        NAS_MMC_SndCssCurrGeoInd(VOS_TRUE, stSndEquPlmnInfo.ucEquPlmnNum, stSndEquPlmnInfo.astEquPlmnAddr);
    }

    return;
}


#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

VOS_UINT32 NAS_MMC_RcvMsccCLAssociatedInfoNtf_BgPlmnSearch_WaitLmmPlmnFastSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_ID_STRU                stPlmnId;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstPlmnWithRat;

    pstPlmnWithRat  = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();
    stPlmnId.ulMcc  = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMcc;
    stPlmnId.ulMnc  = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMnc;

    if (VOS_FALSE == NAS_MMC_IsCurrLtePlmnAllowSrchInCLMode(&stPlmnId))
    {
        NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_RcvMsccCLAssociatedInfoNtf_BgPlmnSearch_WaitLmmPlmnFastSearchCnf:WARNING: Current Lte Plmn is not Allowed Srch In CL Mode,ulMcc and ulMnc are",
                                             stPlmnId.ulMcc, stPlmnId.ulMnc);

        /* ֹͣ�� LMM PLMN SEARCH CNF�Ķ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

        /* ��LTE����ֹͣ����ָ��������Ϣ */
        NAS_MMC_SndLmmPlmnSrchStopReq();

        /*Ǩ�Ƶ��ȴ�ֹͣ����ȷ��״̬������������ʱ��*/
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_FAST_PLMN_STOP_CNF);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMsccCLAssociatedInfoNtf_BgPlmnSearch_WaitLmmBgPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_ID_STRU                stPlmnId;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstPlmnWithRat;

    pstPlmnWithRat = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();
    stPlmnId.ulMcc = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMcc;
    stPlmnId.ulMnc = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMnc;

    if (VOS_FALSE == NAS_MMC_IsCurrLtePlmnAllowSrchInCLMode(&stPlmnId))
    {
        NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_RcvMsccCLAssociatedInfoNtf_BgPlmnSearch_WaitLmmBgPlmnSearchCnf:WARNING: Current Lte Plmn is not Allowed Srch In CL Mode,ulMcc and ulMnc are",
                                             stPlmnId.ulMcc, stPlmnId.ulMnc);

        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);

        /* ��LTE����ֹͣ����������Ϣ */
        NAS_MMC_SndLmmBgStopPlmnSearchReq();

        /*Ǩ�Ƶ��ȴ�ֹͣ����ȷ��״̬������������ʱ��*/
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_STOP_CNF);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_BG_PLMN_STOP_CNF_LEN);
    }

    return  VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMsccCLAssociatedInfoNtf_BgPlmnSearch_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_ID_STRU                stPlmnId;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstPlmnWithRat;

    pstPlmnWithRat = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();
    stPlmnId.ulMcc = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMcc;
    stPlmnId.ulMnc = pstPlmnWithRat->astPlmnId[0].stPlmnId.ulMnc;

    if (VOS_TRUE == NAS_MMC_IsCurrLtePlmnAllowSrchInCLMode(&stPlmnId))
    {
        return VOS_TRUE;
    }

    NAS_WARNING_LOG2(WUEPS_PID_MMC, "NAS_MMC_RcvMsccCLAssociatedInfoNtf_BgPlmnSearch_WaitEpsRegRsltInd:WARNING: Current Lte Plmn is not Allowed Srch In CL Mode,ulMcc and ulMnc are",
                                         stPlmnId.ulMcc, stPlmnId.ulMnc);

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        /* ���������ͷű�־ */
        NAS_MMC_SetRelRequestFlag_BgPlmnSearch(VOS_TRUE);

        /* ��LMM���������ͷ����� */
        NAS_MMC_SndLmmRelReq();

        /* ״̬Ǩ�Ƶ��ȴ������ͷ� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_EPS_CONN_REL_IND);

        /* �����ȴ������ͷŶ�ʱ�� */
        (VOS_VOID)NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

        return VOS_TRUE;
    }

    if (VOS_FALSE == NAS_MMC_SearchNextHighPrioPlmn_BgPlmnSearch())
    {
        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_TRUE, VOS_NULL_PTR);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return  VOS_TRUE;
}

#endif


VOS_VOID NAS_MMC_ProcLmmBgSpecPlmnSearchRlst_BgPlmnSearch(
    LMM_MMC_BG_PLMN_SEARCH_CNF_STRU                        *pstBgSearchCnf
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;
    NAS_MML_LAI_STRU                                       *pstPreCampLai            = VOS_NULL_PTR;
    VOS_UINT8                                               ucHighRatBgProc;

    ucHighRatBgProc = VOS_FALSE;

    if (pstBgSearchCnf->ulPlmnNum > MMC_LMM_MAX_BG_SRCH_PLMN_NUM)
    {
        pstBgSearchCnf->ulPlmnNum = MMC_LMM_MAX_BG_SRCH_PLMN_NUM;
    }

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);


    switch ( pstBgSearchCnf->enRlst)
    {
        case MMC_LMM_BG_SPEC_SRCH_RLT_ABORT:

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        case MMC_LMM_BG_SPEC_SRCH_RLT_SUCCESS :

            /* ����BG����������״̬Ϊ���������� */
            NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&(pstCurHighPrioPlmn->astPlmnId[0]),
                                                    NAS_MMC_NET_STATUS_SEARCHED_EXIST,
                                                    pstPlmnSelectionListInfo);

            ucHighRatBgProc = NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch();

            /* ��ǰBG�Ѻ�VPLMN��PLMNһ��������RAT��ͬ�����������ָ���ѣ��˳�״̬��*/
            pstPreCampLai = NAS_MMC_GetPreCampLai_BgPlmnSearch();

            if ((VOS_TRUE  == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPreCampLai->stPlmnId), &(pstCurHighPrioPlmn->astPlmnId[0].stPlmnId)))
             && (VOS_FALSE == ucHighRatBgProc))
            {
                /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

                /* �˳���ǰ״̬�� */
                NAS_MMC_FSM_QuitFsmL2();

                return;
            }

            {
                /* ��MM��GMM��������ָʾ */
                NAS_MMC_SndGmmPlmnSchInit();
                NAS_MMC_SndMmPlmnSchInit();

                NAS_MMC_SndMsccCampOnInd(VOS_FALSE);

                NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,
                                         NAS_MML_GetSimPsRegStatus());

                /* ���п���ָ������ */
                NAS_MMC_PerformFastSpecSearch_BgPlmnSearch(&(pstCurHighPrioPlmn->astPlmnId[0].stPlmnId),
                                                           pstCurHighPrioPlmn->astPlmnId[0].enRat);
            }
            break;

        case MMC_LMM_BG_SPEC_SRCH_RLT_FAIL:


            PS_MEM_SET(&stSearchedPlmnListInfo, 0x00, sizeof(stSearchedPlmnListInfo));

            NAS_MMC_ConvertLmmBgPlmnSearchRslt2SearchedPlmnListInfo(pstBgSearchCnf,
                                                    &stSearchedPlmnListInfo,
                                                     pstCurHighPrioPlmn->astPlmnId[0].enRat);

            /* ����BG������ʧ�ܽ�� */
            NAS_MMC_ProcBgSpecPlmnSearchRlstFail_BgPlmnSearch(&stSearchedPlmnListInfo);

            break;

        case MMC_LMM_BG_SPEC_SRCH_RLT_NO_RF:

            /* ��פ��̬ */
            if (NAS_MMC_AS_CELL_NOT_CAMP_ON == NAS_MMC_GetAsCellCampOn())
            {
                NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_GetCurrNetRatType());
            }
            else
            {
                /* NO RF��������ABORT������������10�뱳���ѳ��Զ�ʱ�� */
                /* ����״̬�����Ϊʧ��,����Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);
                
                /* �˳���ǰ״̬�� */
                NAS_MMC_FSM_QuitFsmL2();
            }

            break;

        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmBgSpecPlmnSearchRlst_BgPlmnSearch: Unexpected Bg search result.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_ProcLmmBgHistoryPlmnSearchRlst_BgPlmnSearch(
    LMM_MMC_BG_PLMN_SEARCH_CNF_STRU                        *pstBgSearchCnf
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstCurHighPrioPlmn       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stSearchedPlmnListInfo;

    if (pstBgSearchCnf->ulPlmnNum > MMC_LMM_MAX_BG_SRCH_PLMN_NUM)
    {
        pstBgSearchCnf->ulPlmnNum = MMC_LMM_MAX_BG_SRCH_PLMN_NUM;
    }

    /* ��ȡ��ǰ���ڳ��Եĸ����ȼ��������Ϣ */
    pstCurHighPrioPlmn = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_BG_PLMN_SEARCH_CNF);


    switch ( pstBgSearchCnf->enRlst)
    {
        case MMC_LMM_BG_HISTORY_SRCH_RLT_ABORT:

            /* ����״̬�����Ϊʧ��,����Ҫ���� */
            NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

            /* �˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            break;

        case MMC_LMM_BG_HISTORY_SRCH_RLT_SUCCESS :

            /* BG�ѳɹ�ʱ�Ȳ���history�ѱ�־��P̬����BG�ѣ�BG�����п����ȵ������ͷţ������ͷŽ�����ȡ����ָ���ѵ�����ʱ���õ��ñ�־ */

            NAS_MMC_ProcBgHistoryPlmnSearchRsltSucc_BgPlmnSearch();

            break;

        case MMC_LMM_BG_HISTORY_SRCH_RLT_FAIL:

            /* ��¼�Ѿ����й�bg history�� */
            NAS_MMC_SetHistorySearchedFlag_BgPlmnSearch(pstCurHighPrioPlmn->astPlmnId[0].enRat, VOS_TRUE);

            PS_MEM_SET(&stSearchedPlmnListInfo, 0x00, sizeof(stSearchedPlmnListInfo));

            NAS_MMC_ConvertLmmBgPlmnSearchRslt2SearchedPlmnListInfo(pstBgSearchCnf,
                                                    &stSearchedPlmnListInfo,
                                                     pstCurHighPrioPlmn->astPlmnId[0].enRat);

            /* ����BG������ʧ�ܽ�� */
            NAS_MMC_ProcBgHistoryPlmnSearchRlstFail_BgPlmnSearch(&stSearchedPlmnListInfo);

            break;

        case MMC_LMM_BG_HISTORY_SRCH_RLT_NO_RF:

            /* ��פ��̬ */
            if (NAS_MMC_AS_CELL_NOT_CAMP_ON == NAS_MMC_GetAsCellCampOn())
            {
                NAS_MMC_ProcNoRf_BgPlmnSelection(NAS_MML_GetCurrNetRatType());
            }
            else
            {
                PS_MEM_SET(&stSearchedPlmnListInfo, 0x00, sizeof(stSearchedPlmnListInfo));

                NAS_MMC_ConvertLmmBgPlmnSearchRslt2SearchedPlmnListInfo(pstBgSearchCnf,
                                                        &stSearchedPlmnListInfo,
                                                         pstCurHighPrioPlmn->astPlmnId[0].enRat);

                /* �����б� */
                NAS_MMC_UpdatePlmnListInPlmnSelectionList(&stSearchedPlmnListInfo,
                                                          pstPlmnSelectionListInfo,
                                                          VOS_FALSE,
                                                          VOS_FALSE);

                /* ��ά�ɲ�,��ѡ���б���� */
                NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

                /* ����״̬�����Ϊ�ɹ�,����Ҫ���� */
                NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_SUCC, VOS_FALSE, VOS_NULL_PTR);

                /* �˳�״̬�� */
                NAS_MMC_FSM_QuitFsmL2();
            }

            break;

        default:

            /* ��ӡ�쳣 */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmBgHistoryPlmnSearchRlst_BgPlmnSearch: Unexpected Bg search result.");
            break;
    }

    return;
}
#endif



VOS_UINT8  NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch(VOS_VOID)
{
    NAS_MMC_ENTRY_MSG_STRU             *pstEntryMsg    = VOS_NULL_PTR;

   /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg     = NAS_MMC_GetCurrFsmMsgAddr();

    if (TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER == NAS_ExtractMsgNameFromEvtType(pstEntryMsg->ulEventType))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT8 NAS_MMC_IsBgHistoryScene_BgPlmnSearch(VOS_VOID)
{
    NAS_MMC_ENTRY_MSG_STRU             *pstEntryMsg     = VOS_NULL_PTR;
    NAS_MMC_INTER_BG_SEARCH_REQ_STRU   *pstInternalMsg  = VOS_NULL_PTR;

   /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg     = NAS_MMC_GetCurrFsmMsgAddr();

    if (MMCMMC_INTER_BG_SEARCH_REQ == NAS_ExtractMsgNameFromEvtType(pstEntryMsg->ulEventType))
    {
        pstInternalMsg  = (NAS_MMC_INTER_BG_SEARCH_REQ_STRU *)pstEntryMsg->aucEntryMsgBuffer;

        /* ��Ҫ���ڲ�bg history�ѵĳ��� */
        if ((NAS_MMC_BG_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_HISTORY_PLMN_SEARCH  == pstInternalMsg->enBgPlmnSearchScene)
         || (NAS_MMC_BG_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH == pstInternalMsg->enBgPlmnSearchScene)
         || (NAS_MMC_BG_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_FULLBAND_PLMN_SEARCH == pstInternalMsg->enBgPlmnSearchScene))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_BgPlmnSearch(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;
    VOS_UINT8                                               ucSearchRplmnAndHplmnFlg;

    pstPlmnSelectionList             = NAS_MMC_GetHighPrioPlmnList();
    enPlmnType                       = NAS_MMC_GetPlmnTypeInPlmnSelectionList(pstDestPlmn, pstPlmnSelectionList);
    ucSearchRplmnAndHplmnFlg         = NAS_MMC_GetSearchRplmnAndHplmnFlg_BgPlmnSearch(pstDestPlmn->enRat);

    /* �ֶ�����ģʽ��8464 NV���Թر�ʱ������EHPLMN/HPLMN */
    if (VOS_FALSE == NAS_MMC_IsRPlmnFirstSearchWithHPlmn())
    {
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_BgPlmnSearch: enPlmnType is: ", enPlmnType);

    /* Ŀ�����粻��RPLMNʱ������EHPLMN/HPLMN */
    if (NAS_MMC_PLMN_TYPE_RPLMN != enPlmnType)
    {
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_BgPlmnSearch: ucSearchRplmnAndHplmnFlg is: ", ucSearchRplmnAndHplmnFlg);

    /* �ý��뼼������RPLMNʱ�Ѿ�����EHPLMN/HPLMN����״̬��ǰ�´���ȥ�ý��뼼�����ѵ�ʱ�򲻴�EHPLMN/HPLMN */
    if (VOS_TRUE == ucSearchRplmnAndHplmnFlg)
    {
        return VOS_FALSE;
    }

    /* available timer��ʱ����bg history����ʱ��EHPLMN/HPLMN */
    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_BgPlmnSearch: is bg history scene");
        return VOS_TRUE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_BgPlmnSearch: is not bg history scene");

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedAddEhplmnWhenSeachDplmn_BgPlmnSearch(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU                      *pstDplmnNplmnCfgInfo = VOS_NULL_PTR;
    VOS_UINT8                                               ucSearchDplmnAndHplmnFlg;

    pstDplmnNplmnCfgInfo                = NAS_MMC_GetDPlmnNPlmnCfgInfo();
    ucSearchDplmnAndHplmnFlg            = NAS_MMC_GetSearchDplmnAndHplmnFlg_BgPlmnSearch(pstDestPlmn->enRat);

    /* ���������Ż����ƹرղ����� */
    if (VOS_FALSE == NAS_MMC_GetRoamPlmnSelectionSortActiveFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSeachDplmn_BgPlmnSearch: Roam Sort is Deactive");
        return VOS_FALSE;
    }

    /* Ŀ�����粻��DPLMNʱ������EHPLMN/HPLMN */
    if (VOS_FALSE == NAS_MMC_IsPlmnIdWithRatInSimPlmnWithRegDomainList(pstDestPlmn,
                           pstDplmnNplmnCfgInfo->usDplmnListNum, pstDplmnNplmnCfgInfo->astDPlmnList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSeachDplmn_BgPlmnSearch: Dest Plmn is not DPLMN");
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSeachDplmn_BgPlmnSearch: ucSearchDplmnAndHplmnFlg is: ", ucSearchDplmnAndHplmnFlg);

    /* �Զ�ģʽ��Я��HPLMN */
    if ( (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
      && (VOS_FALSE == ucSearchDplmnAndHplmnFlg))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSeachDplmn_BgPlmnSearch: return VOS_TRUE");
        return VOS_TRUE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSeachDplmn_BgPlmnSearch: return VOS_FALSE");

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    NAS_MML_HPLMN_CFG_INFO_STRU                            *pstHplmnCfgInfo      = VOS_NULL_PTR;
    VOS_UINT8                                               ucSearchRplmnAndHplmnFlg;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;


    pstHplmnCfgInfo                     = NAS_MML_GetHplmnCfg();
    pstPlmnSelectionList                = NAS_MMC_GetHighPrioPlmnList();
    enPlmnType                          = NAS_MMC_GetPlmnTypeInPlmnSelectionList(pstDestPlmn, pstPlmnSelectionList);
    ucSearchRplmnAndHplmnFlg            = NAS_MMC_GetSearchRplmnAndHplmnFlg_BgPlmnSearch(pstDestPlmn->enRat);

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch: ucAddEhplmnWhenSrchHplmnFlg is: ", pstHplmnCfgInfo->ucAddEhplmnWhenSrchHplmnFlg);

    /* NV���Թر�ʱ������EHPLMN/HPLMN */
    if (VOS_FALSE == pstHplmnCfgInfo->ucAddEhplmnWhenSrchHplmnFlg)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch(): ucAddEhplmnWhenSrchHplmnFlg is false");
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch: ucSearchRplmnAndHplmnFlg is: ", ucSearchRplmnAndHplmnFlg);

    /* �ý��뼼������RPLMNʱ�Ѿ�����EHPLMN/HPLMN������EHPLMN/HPLMN */
    if (VOS_TRUE == ucSearchRplmnAndHplmnFlg)
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch(): ucSearchRplmnAndHplmnFlg is true");

        return VOS_FALSE;
    }

    /* ���Կ�ʱ������EHPLMN/HPLMN */
    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch(): is test card");
        return VOS_FALSE;
    }

    /* Ŀ�����粻��HPLMNʱ������EHPLMN/HPLMN */
    if (NAS_MMC_PLMN_TYPE_HPLMN != enPlmnType)
    {
        NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEhplmnWhenSearchHplmn_BgPlmnSearch(): PlmnType is not HPLMN:", enPlmnType);
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_IsNeedAddEplmnWhenSearchRplmn_BgPlmnSearch(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    VOS_UINT8                                               ucSearchRplmnAndEplmnFlg;
    VOS_UINT8                                               ucIsRoam;

    pstPlmnSelectionList             = NAS_MMC_GetHighPrioPlmnList();
    enPlmnType                       = NAS_MMC_GetPlmnTypeInPlmnSelectionList(pstDestPlmn, pstPlmnSelectionList);
    ucSearchRplmnAndEplmnFlg         = NAS_MMC_GetSearchRplmnAndEplmnFlg_BgPlmnSearch(pstDestPlmn->enRat);

    ucIsRoam = VOS_FALSE;

    /* �ж�Ŀ�������Ƿ���EPLMN�б��� */
    if (VOS_FALSE == NAS_MML_IsPlmnInEplmnList(&(pstDestPlmn->stPlmnId)))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEplmnWhenSearchRplmn_BgPlmnSearch: Dest Plmn is not in EPLMN LIST");
        return VOS_FALSE;
    }

    if (VOS_FALSE == NAS_MML_ComparePlmnIdWithHplmn(&pstDestPlmn->stPlmnId))
    {
        ucIsRoam = VOS_TRUE;
    }

    NAS_NORMAL_LOG3(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddEplmnWhenSearchRplmn_BgPlmnSearch: ucIsRoam, enPlmnType, ucSearchRplmnAndEplmnFlg are: ", ucIsRoam, enPlmnType, enPlmnType);

    /* ����NV�ж��Ƿ�Я��eplmn��Ϣ���������ٶ� */
    if ( (NAS_MMC_PLMN_TYPE_RPLMN == enPlmnType)
      && (VOS_FALSE == ucSearchRplmnAndEplmnFlg))
    {
        if (VOS_TRUE == NAS_MMC_GetSpecSceneSrchRplmnCarryEplmnFlag(NAS_MMC_PLMN_SEARCH_SCENE_BG_HISTORY_PLMN_SEARCH, ucIsRoam))
        {
            return VOS_TRUE;
        }

    }

    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MMC_DPLMN_NPLMN_CFG_INFO_STRU            *pstDplmnNplmnCfgInfo  = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU        *pstPlmnSelectionList  = VOS_NULL_PTR;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                  enPlmnType;

    pstPlmnSelectionList             = NAS_MMC_GetHighPrioPlmnList();
    enPlmnType                       = NAS_MMC_GetPlmnTypeInPlmnSelectionList(pstDestPlmn, pstPlmnSelectionList);
    pstDplmnNplmnCfgInfo             = NAS_MMC_GetDPlmnNPlmnCfgInfo();

    /* ���������Ż����ƹرղ����� */
    if (VOS_FALSE == NAS_MMC_GetRoamPlmnSelectionSortActiveFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch: Roam Sort is Deactive");
        return VOS_FALSE;
    }

    /* �ֶ�ģʽ�������� */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch: is manual mode");
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch: enPlmnType is ", enPlmnType);

    /* �����RPLMN�������� */
    if (NAS_MMC_PLMN_TYPE_RPLMN == enPlmnType)
    {
        return VOS_FALSE;
    }

    /* Ŀ�����粻��DPLMNʱ�������� */
    if (VOS_FALSE == NAS_MMC_IsPlmnIdWithRatInSimPlmnWithRegDomainList(pstDestPlmn,
                           pstDplmnNplmnCfgInfo->usDplmnListNum, pstDplmnNplmnCfgInfo->astDPlmnList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch: is not DPLMN");
        return VOS_FALSE;
    }

    /* ������������UPLMN->OPLMN->DPLMN->AVAILABLE��˳��ִ�е����Դ� */
    if (VOS_TRUE == NAS_MMC_GetSrchUOplmnPriorToDplmnFlg())
    {
        /* Ŀ��������UPLMNʱ�������� */
        if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithUplmn(pstDestPlmn))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch: is UPLMN");
            return VOS_FALSE;
        }

        /* Ŀ��������OPLMNʱ�������� */
        if (VOS_TRUE == NAS_MMC_ComparePlmnIdWithRatWithOplmn(pstDestPlmn))
        {
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch: is OPLMN");
            return VOS_FALSE;
        }
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedAddDplmnOfSameCountryWhenSearchDPlmn_BgPlmnSearch: return VOS_TRUE");

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_GetNextSearchHighPrioPlmn_BgPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo,
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstVPlmn,
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnList
)
{
    /* bg history��ʱ */
    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        return NAS_MMC_GetNextHistorySearchHighPrioPlmn_BgPlmnSearch(pstHighPrioPlmnSearchListInfo,
                                                                     pstHighPrioPlmnInfo,
                                                                     pstVPlmn,
                                                                     pstEplmnList);
    }

    /* bg spec��ʱ */
    return NAS_MMC_GetNextSpecSearchHighPrioPlmn_BgPlmnSearch(pstHighPrioPlmnSearchListInfo,
                                                              pstHighPrioPlmnInfo,
                                                              pstVPlmn,
                                                              pstEplmnList);

}


VOS_UINT32 NAS_MMC_GetNextHistorySearchHighPrioPlmn_BgPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo,
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstVPlmn,
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnList
)
{
    /* ���ѹ���ratҪ�ų�����Ҫ��MCC��ͬ��������NAS_MMC_GetNextSpecSearchHighPrioPlmn_BgPlmnSearch���� */
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList       = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    VOS_UINT32                                              i;

    /* ��ȡ��ǰ���뼼�����ȼ���Ϣ */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* bg�Ѻ�fast spec�Ѷ�����õ����������
       ��һ�������ȼ���������no search�ģ�����bg��,
       ��һ�������ȼ���������exist�ģ�����fast spec�� */


    PS_MEM_SET(&stPrioRatList, 0X00, sizeof(stPrioRatList));

    /* ÿ��ģֻ�������һ��bg history�� */
    for (i = 0 ; i < pstPrioRatList->ucRatNum; i++)
    {
#if (FEATURE_ON == FEATURE_LTE)
        /* ��ԭ��ֵ#14�ܾ�������Disable LTEʱ����LTE������ */
        if ( (VOS_FALSE                 == NAS_MMC_IsNeedEnableLteWhenLteDisabledInBgHistoryScene_BgPlmnSearch())
          && (NAS_MML_NET_RAT_TYPE_LTE  == pstPrioRatList->aucRatPrio[i]))
        {
            continue;
        }
#endif

        if (VOS_FALSE == NAS_MMC_GetHistorySearchedFlag_BgPlmnSearch(pstPrioRatList->aucRatPrio[i]))
        {
            NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetNextHistorySearchHighPrioPlmn_BgPlmnSearch: the rat is not history searched", pstPrioRatList->aucRatPrio[i]);

            stPrioRatList.aucRatPrio[stPrioRatList.ucRatNum] = pstPrioRatList->aucRatPrio[i];
            stPrioRatList.ucRatNum ++;
        }
    }


    for ( i = 0 ; i < pstHighPrioPlmnSearchListInfo->usSearchPlmnNum; i++ )
    {
        pstPlmnSelectionPlmnInfo = &(pstHighPrioPlmnSearchListInfo->astPlmnSelectionList[i]);

        /* ��ȡ��һ�������ȼ�������ɹ����˳�ѭ������ȡ��һ������ʧ�ܣ������Ҹ����ȼ��б��е���һ������ */
        if (VOS_TRUE == NAS_MMC_GetNextSearchHighPrioPlmn(pstPlmnSelectionPlmnInfo,
                                                          pstHighPrioPlmnInfo,
                                                          pstVPlmn,
                                                          &stPrioRatList))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MMC_GetNextSpecSearchHighPrioPlmn_BgPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo,
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstVPlmn,
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnList
)
{
    VOS_UINT32                                              i;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulCampHplmnFlg;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    enLteCapabilityStatus       = NAS_MML_GetLteCapabilityStatus();

    ulCampHplmnFlg              = NAS_MML_ComparePlmnIdWithHplmn(&pstVPlmn->stPlmnId);

    /* ��ȡ��ǰ���뼼�����ȼ���Ϣ */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    for ( i = 0 ; i < pstHighPrioPlmnSearchListInfo->usSearchPlmnNum; i++ )
    {
        pstPlmnSelectionPlmnInfo = &(pstHighPrioPlmnSearchListInfo->astPlmnSelectionList[i]);

        PS_MEM_SET(&stPrioRatList, 0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

        PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

        /* ��PLMN��VPLMN�������Ƿ���ͬ,�������ͬ,�򲻽������� */
        if ( VOS_FALSE == NAS_MMC_IsHighPrioPlmnSameCountry(pstPlmnSelectionPlmnInfo,
                                                                 &pstVPlmn->stPlmnId))
        {
            continue;
        }

        /* ��ǰVPLMN��HPLMN,HPLMN��BG�ѱȽ�����,��Ҫ���⴦�� */
        if (VOS_TRUE == ulCampHplmnFlg)
        {
            /* ��ȡ���ĸ����ȼ�PLMN����HPLMN������ */
            if (VOS_FALSE == NAS_MML_ComparePlmnIdWithHplmn(&pstPlmnSelectionPlmnInfo->stPlmnId))
            {
                continue;
            }

            if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus)
             || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus))

            {
                NAS_MML_DelSpecRatInPrioRatList(NAS_MML_NET_RAT_TYPE_LTE, &stPrioRatList);
            }
        }
        else
        {
            /* �Ǹ����ȼ�RAT��HPLMN����ʱ,�Ƚ�VPLMN�͸����ȼ��������ȼ��Ƿ���ͬ�������ͬ����VOS_FALSE */
            if (VOS_TRUE == NAS_MMC_CompareVPlmnPrioWithHighPrioPlmn(&pstVPlmn->stPlmnId,
                                                                      pstVPlmn->enRat, pstEplmnList, pstPlmnSelectionPlmnInfo))
            {
                return VOS_FALSE;
            }
        }


        /* ��ȡ��һ�������ȼ�������ɹ����˳�ѭ������ȡ��һ������ʧ�ܣ������Ҹ����ȼ��б��е���һ������ */
        if (VOS_TRUE == NAS_MMC_GetNextSearchHighPrioPlmn(pstPlmnSelectionPlmnInfo,
                                                          pstHighPrioPlmnInfo,
                                                          pstVPlmn,
                                                          &stPrioRatList))
        {
            return VOS_TRUE;
        }


    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_GetDestFastSpecSearchPlmnRcvBgHistorySucc_BgPlmnSearch(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstDestPlmn
)
{
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstSearchingPlmnList     = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus;
    VOS_UINT32                                              i;

    /* ������㷢bg�������PLMN�б� */
    pstSearchingPlmnList     = NAS_MMC_GetCurHighPrioPlmnList_BgPlmnSearch();

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetHighPrioPlmnList();

    for (i = 0; i < pstSearchingPlmnList->ulPlmnNum; i++)
    {
        enNetStatus = NAS_MMC_GetSpecPlmnWithRatNetStatusInPlmnSelectionList(&pstSearchingPlmnList->astPlmnId[i], pstPlmnSelectionListInfo);

        /* bg history�ѹ����н������ϱ�searched plmn info ind�����б���������״̬Ϊsearched exist��������Ϊ����ָ���ѵ����磬
           ����ָ����ʱָֻ��һ��PLMN���ӿ������ٶ� */
        if (NAS_MMC_NET_STATUS_SEARCHED_EXIST == enNetStatus)
        {
            pstDestPlmn->enRat  = pstSearchingPlmnList->astPlmnId[i].enRat;
            pstDestPlmn->stPlmnId.ulMcc = pstSearchingPlmnList->astPlmnId[i].stPlmnId.ulMcc;
            pstDestPlmn->stPlmnId.ulMnc = pstSearchingPlmnList->astPlmnId[i].stPlmnId.ulMnc;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM_UINT8 NAS_MMC_GetBgSearchScene_BgPlmnSearch(
    VOS_UINT32                                              ulNeedPlmnSearchFlg
)
{
    NAS_MMC_ENTRY_MSG_STRU                                 *pstEntryMsg     = VOS_NULL_PTR;
    NAS_MMC_INTER_BG_SEARCH_REQ_STRU                       *pstInternalMsg  = VOS_NULL_PTR;
    NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM_UINT8                 enBgPlmnSearchScene;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapStatus;

    enLteCapStatus      = NAS_MML_GetLteCapabilityStatus();
#endif

    enBgPlmnSearchScene = NAS_MMC_BG_PLMN_SEARCH_SCENE_BUTT;

    /* bg history�ѣ�ֱ�Ӱ�������������ȥ */
    if (VOS_TRUE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        /* ����FSM ID��ȡ��FSM �����Ϣ */
        pstEntryMsg     = NAS_MMC_GetCurrFsmMsgAddr();
        pstInternalMsg  = (NAS_MMC_INTER_BG_SEARCH_REQ_STRU *)pstEntryMsg->aucEntryMsgBuffer;

        enBgPlmnSearchScene = pstInternalMsg->enBgPlmnSearchScene;

        return enBgPlmnSearchScene;
    }


    /* �����Ҫ����������Ҫ������������ */
    if (VOS_TRUE == ulNeedPlmnSearchFlg)
    {
        /* BG��֮ǰLte û��disable���ڱ�����״̬���﷢��disable lte����ע�ᱻ��#7,����������Ϊdisable lte */
#if (FEATURE_ON == FEATURE_LTE)
        if ((VOS_FALSE == NAS_MMC_GetPreLteDisableFlg_BgPlmnSearch())
         && ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapStatus)
          || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapStatus)))
        {
            enBgPlmnSearchScene = NAS_MMC_BG_PLMN_SEARCH_SCENE_DISABLE_LTE;
        }
        else
#endif
        {
            enBgPlmnSearchScene = NAS_MMC_BG_PLMN_SEARCH_SCENE_AREA_LOST;
        }
    }

    return enBgPlmnSearchScene;
}
#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_MMC_IsNeedEnableLTE_FastSpecSearch(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    VOS_UINT32                                              ulIsPlmnSupportDam;
    NAS_MML_PLMN_ID_STRU                                   *pstCurrPlmn;
    VOS_UINT8                                               ucHighRatBgProc;

    ucHighRatBgProc = NAS_MMC_IsHighPrioRatHplmnSearch_BgPlmnSearch();

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

    /* �����SIM��������ҪEnable Lte */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        return VOS_FALSE;
    }

    /* PS����Ч, ����Ҫ���� Enable L */
    if ( VOS_FALSE == NAS_MML_GetSimPsRegStatus() )
    {
        return VOS_FALSE;
    }

    /* PS��������ע��,����Ҫ���� Enable L */
    if ( VOS_FALSE == NAS_MML_GetPsAttachAllowFlg() )
    {
        return VOS_FALSE;
    }

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* nv�Ѿ����ò���ҪENABLE LTE�����Ǹ�����RAT������ʱ����ENABLE LTE */
    if ( (NAS_MML_HIGH_PRIO_RAT_BG_SEARCH_PROC_LTE_NOT_ENABLE_LTE == NAS_MML_GetHighPrioRatBgEnableLteFlag())
      && (VOS_TRUE == ucHighRatBgProc))
    {
        return VOS_FALSE;
    }

    /* ��ǰפ��VPLMN��enable LTE������enable */
    pstCurrPlmn = NAS_MML_GetCurrCampPlmnId();
    if (VOS_TRUE ==  NAS_MML_ComparePlmnIdWithHplmn(pstCurrPlmn))
    {
        return VOS_FALSE;
    }

    /* �Ѿ�Disable L,��Ҫ���� Enable L */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
     || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus))
    {
        /* ���֮ǰdisable lte��������֧��DAM�����磬�ҽ�ֹ���뼼�������б�����9014 nvδ������
           ������enable lte,AT&T LTE-BTR-1-1840����Ҫ��ֻ��ֹ310410 lte���磬����lte������ã�
           �������ɾ���������������Ҫ���ԣ�����DAM nv����в��� */
        ulIsPlmnSupportDam = NAS_MML_IsPlmnSupportDam(NAS_MMC_GetDisabledLtePlmnId());

        if ((VOS_FALSE == NAS_MML_GetAddDamPlmnInDisablePlmnWithRatListFlag())
          && (VOS_TRUE == ulIsPlmnSupportDam))
        {
            return VOS_FALSE;
        }

        /* bg history��������#14�ܾ���Ҳ����ҪEnable LTE */
        if (VOS_FALSE == NAS_MMC_IsNeedEnableLteWhenLteDisabledInBgHistoryScene_BgPlmnSearch())
        {
            return VOS_FALSE;
        }

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedEnableLteWhenLteDisabledInBgHistoryScene_BgPlmnSearch(VOS_VOID)
{
    VOS_UINT8                                               ucEmcFlg;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enTimerStatus;
    MMC_LMM_DISABLE_LTE_REASON_ENUM_UINT32                  enDisableLteReason;
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    ucEmcFlg                = NAS_MML_GetCsEmergencyServiceFlg();
    enTimerStatus           = NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
    enDisableLteReason      = NAS_MML_GetDisableLteReason();
    enLteCapabilityStatus   = NAS_MML_GetLteCapabilityStatus();


    /* ����bg history�ĳ��� */
    if (VOS_FALSE == NAS_MMC_IsBgHistoryScene_BgPlmnSearch())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedEnableLteWhenLteDisabledInBgHistoryScene_BgPlmnSearch: Not BG Hisotry Search, return TRUE");
        return VOS_TRUE;
    }

    /* ����Disable LTE���� */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS != enLteCapabilityStatus)
     && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != enLteCapabilityStatus))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedEnableLteWhenLteDisabledInBgHistoryScene_BgPlmnSearch: LTE not disabled, return TRUE");
        return VOS_TRUE;
    }

    /* ��ѡ��״̬����Enable LTE����������һ��:����������Ҫ����Enable */
    if (( (MMC_LMM_DISABLE_LTE_REASON_EMERGENCY_CALL   == enDisableLteReason)
       || (MMC_LMM_DISABLE_LTE_REASON_LTE_REJ_CAUSE_14 == enDisableLteReason)
       || (VOS_TRUE == ucEmcFlg))
     && (NAS_MMC_TIMER_STATUS_RUNING == enTimerStatus))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedEnableLteWhenLteDisabledInBgHistoryScene_BgPlmnSearch: EMC call exists, return FALSE");
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_IsNeedEnableLteWhenLteDisabledInBgHistoryScene_BgPlmnSearch: return TRUE");
    return VOS_TRUE;
}



VOS_VOID NAS_MMC_SendEnableLTE_FastSpecSearch(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

    if ((VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
     && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
    {
        NAS_MML_EnableLteCommProc();

    }

    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
    }

    /* ����disable LTE������� */
    NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);


    return;
}


#endif



VOS_VOID NAS_MMC_ProcNoRf_BgPlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enNasRat
)
{
    /* ��¼NO RF */
    NAS_MML_SetCurrRatRfAvailFlg(enNasRat, VOS_FALSE);

    /* ��MSCC֪ͨ��ǰNO RF��Ϣ  */
    NAS_MMC_SndMsccRfAvailInd(VOS_FALSE);

    /* ��RRM����ע�� */
    NAS_MMC_RegisterRrmResourceNtf(enNasRat, RRM_PS_TASK_TYPE_NAS_SEARCH);

    /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ�����Ҫ���� */
    NAS_MMC_SndRslt_BgPlmnSearch(NAS_MMC_BG_PLMN_SEARCH_FAIL, VOS_FALSE, VOS_NULL_PTR);

    /* �˳�״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return;
}

/*lint -restore */


VOS_VOID  NAS_MMC_SndMsccSysInfo_BgPlmnSearch(VOS_VOID)
{
    /* ����ϵͳ��Ϣ��־���ϵ�ʱ��֪ͨMMAϵͳ��Ϣ���������־ */
    if (VOS_TRUE == NAS_MMC_GetNeedSndSysInfo_BgPlmnSearch())
    {
        NAS_MMC_SndMsccSysInfo();

        NAS_MMC_SetNeedSndSysInfo_BgPlmnSearch(VOS_FALSE);
    }

    return;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
