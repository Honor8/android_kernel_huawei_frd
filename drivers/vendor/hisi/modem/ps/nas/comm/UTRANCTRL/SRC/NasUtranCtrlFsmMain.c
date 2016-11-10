

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/


#include "NasComm.h"

#include "NasUtranCtrlCtx.h"
#include "NasUtranCtrlCommFunc.h"
#include "NasUtranCtrlFsmMain.h"
#include "Nasrrcinterface.h"
#include "NasUtranCtrlProcNvim.h"
#include "NasMmcTimerMgmt.h"
#include "NasMmlLib.h"
#include "MsccMmcInterface.h"
#include "NasMmcComFunc.h"
#include "NVIM_Interface.h"
#include "NasMmcSndLmm.h"
#include "siappstk.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/

#define    THIS_FILE_ID        PS_FILE_ID_NAS_UTRANCTRL_FSM_MAIN_C

/*****************************************************************************
  3 ���Ͷ���
*****************************************************************************/

/*lint -save -e958 */

/*****************************************************************************
  4 ��������
*****************************************************************************/

/*****************************************************************************
  5 ��������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
#if (FEATURE_ON == FEATURE_UE_MODE_TDS)

#if defined (__PS_WIN32_RECUR__)


VOS_VOID NAS_UTRANCTRL_ReloadFsmDesc(
    NAS_UTRANCTRL_FSM_CTX_STRU         *pstFsmCtx,
    VOS_UINT32                          ulPcRecurFsmId
)
{
    switch ( ulPcRecurFsmId )
    {
        case NAS_UTRANCTRL_FSM_MAIN:
            pstFsmCtx->pstFsmDesc       = NAS_UTRANCTRL_GetMainFsmDescAddr();
            break;

        case NAS_UTRANCTRL_FSM_SWITCH_ON:
            pstFsmCtx->pstFsmDesc       = NAS_UTRANCTRL_GetSwitchOnFsmDescAddr();
            break;

        case NAS_UTRANCTRL_FSM_POWER_OFF:
            pstFsmCtx->pstFsmDesc       = NAS_UTRANCTRL_GetPowerOffFsmDescAddr();
            break;

        case NAS_UTRANCTRL_FSM_PLMN_SELECTION:
            pstFsmCtx->pstFsmDesc       = NAS_UTRANCTRL_GetPlmnSelectionFsmDescAddr();
            break;

        case NAS_UTRANCTRL_FSM_SYSCFG:
            pstFsmCtx->pstFsmDesc       = NAS_UTRANCTRL_GetSysCfgFsmDescAddr();
            break;

        case NAS_UTRANCTRL_FSM_MODE_CHANGE:
            pstFsmCtx->pstFsmDesc       = NAS_UTRANCTRL_GetModeChangeFsmDescAddr();
            break;

        default:
            break;
    }

}



VOS_UINT32 NAS_UTRANCTRL_RestoreContextData_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_UTRANCTRL_CTX_STRU                                  *pstUtranCtrlCtx;
    NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU                 *pstRcMsg;
    VOS_UINT32                                               ulExpectCount;
    VOS_UINT32                                               ulCount;
    VOS_UINT32                                               ulItemCount;
    VOS_UINT8                                               *pucDest;
    VOS_UINT8                                               *pucSrc;
    VOS_UINT32                                               ulDestLen;

    pstUtranCtrlCtx     = NAS_UTRANCTRL_GetUtranCtrlCtx();

    ulExpectCount = 0;
    ulCount       = 0;
    ulItemCount   = 0;
    ulExpectCount = sizeof(NAS_UTRANCTRL_CTX_STRU);

    /*ע��״̬��*/
    NAS_UTRANCTRL_RegFsm();

    pstRcMsg = (NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU *)pstMsg;
    pucSrc   = pstRcMsg->aucData;

    /* �����ܳ��� */
    PS_MEM_CPY(&ulCount, pucSrc, sizeof(VOS_UINT32));
    pucSrc += sizeof(VOS_UINT32);

    if( ulExpectCount != ulCount )
    {
        return VOS_FALSE;
    }

    /* g_stNasUtranCtrlCtxĿ���ַ */
    pucDest     = (VOS_UINT8 *)pstUtranCtrlCtx;
    ulDestLen   = sizeof(NAS_UTRANCTRL_CTX_STRU);

    /* ��ѹ�� */
    if (VOS_FALSE == NAS_MML_UnCompressData(pucDest, &ulDestLen, pucSrc, &ulItemCount) )
    {
        return VOS_FALSE;
    }

    pucSrc += (ulItemCount);

    NAS_UTRANCTRL_ReloadFsmDesc(NAS_UTRANCTRL_GetCurFsmAddr(), pstUtranCtrlCtx->stCurFsm.enFsmId);


    NAS_INFO_LOG(WUEPS_PID_MMC, "UTRANCTRL: NAS_UTRANCTRL_RestoreContextData_Main - data is restored.");

    return VOS_TRUE;
}

#endif  /* END OF #if defined (__PS_WIN32_RECUR__) */




VOS_UINT32 NAS_UTRANCTRL_RcvWasSysInfo_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SYS_INFO_IND_STRU             *pstRrMmSysInfoInd;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enPreUtranMode;

    enPreUtranMode     = NAS_UTRANCTRL_GetPreUtranMode();
    pstRrMmSysInfoInd  = (RRMM_SYS_INFO_IND_STRU *)pstMsg;

    /* �Զ�ģʽ�յ�WAS��Ϣ,ͳһ�л���FDDģʽ,д��NV�� */
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        /* �����ǰ��MCC����TD��PLMN LIST�б���,UTRAN MODEΪFDD */
        if (VOS_FALSE == NAS_MML_IsMccInDestMccList(pstRrMmSysInfoInd->PlmnId.ulMcc,
                                                    NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                    NAS_UTRANCTRL_GetSpecTdMccList()))
        {
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND);
        }
        else
        {
            NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
        }
        /* ���µ�ǰ�Ĺ���ģʽ */
        NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_FDD);
    }

    /* ģʽ���䲻֪ͨRRC */
    if (enPreUtranMode == NAS_UTRANCTRL_GetCurrUtranMode())
    {
        /* ��Ϣδ������������������MMC״̬������ */
        return VOS_FALSE;
    }

    /* �ѵ�ǰUtranModeд��NV�� */
    NAS_UTRANCTRL_WriteUtranModeNvim(NAS_UTRANCTRL_UTRAN_MODE_FDD);

    /* �����ϴη���������UtranMode */
    NAS_UTRANCTRL_SetPreUtranMode(NAS_UTRANCTRL_GetCurrUtranMode());

    /* ����ModeChange״̬�� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_MODE_CHANGE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvGasSysInfo_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GRRMM_SYS_INFO_IND_ST              *pstGrrSysInfo;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enPreUtranMode;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enCurrUtranMode;

    pstGrrSysInfo      = (GRRMM_SYS_INFO_IND_ST *)pstMsg;
    enPreUtranMode     = NAS_UTRANCTRL_GetPreUtranMode();

    /* �Զ�ģʽ�յ�GAS��Ϣ������ϵͳ��Ϣ�е�MCC�����µ�ǰģʽ��дNV */
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        /* �����ǰ��MCC����TD��PLMN LIST�б��У�UTRAN MODEΪFDD */
        if (VOS_FALSE == NAS_MML_IsMccInDestMccList(pstGrrSysInfo->PlmnId.ulMcc,
                                                NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                NAS_UTRANCTRL_GetSpecTdMccList()))
        {
            enCurrUtranMode = NAS_UTRANCTRL_UTRAN_MODE_FDD;
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND);
        }
        else
        {
            enCurrUtranMode = NAS_UTRANCTRL_UTRAN_MODE_TDD;

            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
            NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
        }
        NAS_UTRANCTRL_SetCurrUtranMode(enCurrUtranMode);
    }

    /* ģʽ���䲻֪ͨRRC */
    if (enPreUtranMode == NAS_UTRANCTRL_GetCurrUtranMode())
    {
        /* ��Ϣδ������������������MMC״̬������ */
        return VOS_FALSE;
    }

    /* �ѵ�ǰUtranModeд��NV�� */
    NAS_UTRANCTRL_WriteUtranModeNvim(NAS_UTRANCTRL_GetCurrUtranMode());

    /* �����ϴη���������UtranMode */
    NAS_UTRANCTRL_SetPreUtranMode(NAS_UTRANCTRL_GetCurrUtranMode());

    /* ����ModeChange״̬�� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_MODE_CHANGE);

    return VOS_TRUE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvTdSysInfo_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enPreUtranMode;

    enPreUtranMode   = NAS_UTRANCTRL_GetPreUtranMode();

    /* �Զ�ģʽ�յ�TDS��Ϣ��������FDD  ģʽ�л���TDDģʽ,��Ҫ���µ�ǰģʽ */
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_TDD);

        NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
        NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
    }

    /* ģʽ���䲻֪ͨRRC */
    if (enPreUtranMode == NAS_UTRANCTRL_GetCurrUtranMode())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdSysInfo_Main(): UTRANMODE not change");
        /* ��Ϣδ������������������ */
        return VOS_FALSE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdSysInfo_Main(): Write NV as TDD and enter NAS_UTRANCTRL_FSM_MODE_CHANGE");

    /* �ѵ�ǰUtranModeд��NV�� */
    NAS_UTRANCTRL_WriteUtranModeNvim(NAS_UTRANCTRL_UTRAN_MODE_TDD);

    /* �����ϴη���������UtranMode */
    NAS_UTRANCTRL_SetPreUtranMode(NAS_UTRANCTRL_GetCurrUtranMode());

    /* ����ModeChange״̬�� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_MODE_CHANGE);

    return VOS_TRUE;
}


#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_UTRANCTRL_RcvLmmSysInfo_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SYS_INFO_IND_STRU          *pstLmmSysInfoMsg;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enPreUtranMode;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enCurrUtranMode;
    MMC_LMM_PLMN_ID_STRU                stLmmPlmn;
    NAS_MML_PLMN_ID_STRU                stGUNasPlmn;

    pstLmmSysInfoMsg = (LMM_MMC_SYS_INFO_IND_STRU*)pstMsg;
    enPreUtranMode   = NAS_UTRANCTRL_GetPreUtranMode();
    stLmmPlmn        = (pstLmmSysInfoMsg->stLteSysInfo.stSpecPlmnIdList.astSuitPlmnList[0]);

    if (VOS_FALSE == NAS_MML_IsGULModeConfigured())
    {
        return VOS_FALSE;
    }

    /* ��LMM��PLMNת����GU NAS��PLMN��ʽ */
    NAS_MMC_ConvertLmmPlmnToGUNasFormat(&stLmmPlmn,
                                        &stGUNasPlmn);

    /* �Զ�ģʽ�յ�GAS��Ϣ����Ҫ���µ�ǰģʽ */
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        /* �����ǰ��MCC����TD��PLMN LIST�б���,UTRAN MODEΪFDD */
        if (VOS_FALSE == NAS_MML_IsMccInDestMccList(stGUNasPlmn.ulMcc,
                                                    NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                    NAS_UTRANCTRL_GetSpecTdMccList()))
        {
            enCurrUtranMode = NAS_UTRANCTRL_UTRAN_MODE_FDD;
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND);
        }
        else
        {
            enCurrUtranMode = NAS_UTRANCTRL_UTRAN_MODE_TDD;

            NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
        }

        NAS_UTRANCTRL_SetCurrUtranMode(enCurrUtranMode);
    }

    /* ģʽ���䲻֪ͨRRC */
    if (enPreUtranMode == NAS_UTRANCTRL_GetCurrUtranMode())
    {
        /* ��Ϣδ������������������ */
        return VOS_FALSE;
    }

    /* �ѵ�ǰUtranModeд��NV�� */
    NAS_UTRANCTRL_WriteUtranModeNvim(NAS_UTRANCTRL_GetCurrUtranMode());

    /* �����ϴη���������UtranMode */
    NAS_UTRANCTRL_SetPreUtranMode(NAS_UTRANCTRL_GetCurrUtranMode());

    /* ����ModeChange״̬�� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_MODE_CHANGE);

    return VOS_TRUE;
}
#endif


VOS_UINT32 NAS_UTRANCTRL_RcvMmcGmmStartCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_SIM_MS_IDENTITY_STRU       *pstMsIdentity = VOS_NULL_PTR;
    VOS_UINT8                           ucSimPresentStatus;

    pstMsIdentity                       = NAS_MML_GetSimMsIdentity();
    ucSimPresentStatus                  = NAS_MML_GetSimPresentStatus();


    /* ������ȡ en_NV_Item_SMC_Ctrl_Flg NV�� */
    NAS_UTRANCTRL_ReadSmcCtrlFlgNvim();

    /* ������ȡ en_NV_Item_Utran_Mode_Auto_Switch NV�� */
    NAS_UTRANCTRL_ReadUtranModeAutoSwitchNvim();

    /* ������ȡ en_NV_Item_Current_Utran_Mode NV�� */
    NAS_UTRANCTRL_ReadUtranModeNvim();

    /* ����IMSI��en_NV_Item_Utran_Mode_Auto_Switch NV�������µ�ǰ��UTRAN�л�ģʽ */
    NAS_UTRANCTRL_UpdateCurUtranSwitchMode( pstMsIdentity->aucImsi,
                                            ucSimPresentStatus,
                                            NAS_UTRANCTRL_GetUtranModeAutoSwtich() );

    /* ����ʱ��֪ͨRRC��ǰ��UTRAN MODE,��ʱ��¼����,�����յ�ϵͳ��Ϣ����֪ͨ */
    if (VOS_TRUE == NAS_MML_IsGULModeConfigured())
    {
        NAS_UTRANCTRL_SetPreUtranMode(NAS_UTRANCTRL_GetCurrUtranMode());
    }
    else
    {
        NAS_UTRANCTRL_SetPreUtranMode(NAS_UTRANCTRL_UTRAN_MODE_NULL);
    }

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvPlmnSearchReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMC_PLMN_SEARCH_REQ_STRU      *pstMsccPlmnSrchReq;
    VOS_UINT32                         *pulTdMccList;
    VOS_UINT32                          ulTdMccNum;

    pstMsccPlmnSrchReq = (MSCC_MMC_PLMN_SEARCH_REQ_STRU *)pstMsg;
    ulTdMccNum         = NAS_UTRANCTRL_GetSpecTdMccListNum();
    pulTdMccList       = NAS_UTRANCTRL_GetSpecTdMccList();

    if (NAS_MSCC_PIF_ACQ_REASON_POWER_UP == pstMsccPlmnSrchReq->enAcqReason)
    {
        NAS_UTRANCTRL_SetSwithOnPlmnSearchFlag(VOS_TRUE);
    }

    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&(pstMsccPlmnSrchReq->stInitAcqPlmnInfo.stInitAcqPlmnId)))
    {
        if (VOS_TRUE == NAS_MML_IsMccInDestMccList(pstMsccPlmnSrchReq->stInitAcqPlmnInfo.stInitAcqPlmnId.ulMcc, ulTdMccNum, pulTdMccList))
        {
             /* �����й������� */
            NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
        }
        else
        {
            NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_FALSE);
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND);
        }
    }

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvMsccAcqReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_UTRANCTRL_SetSwithOnPlmnSearchFlag(VOS_FALSE);

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}




VOS_UINT32 NAS_UTRANCTRL_RcvPlmnSpecialReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_FALSE);

        NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_UNKNOWN);
    }

    NAS_UTRANCTRL_SetSwithOnPlmnSearchFlag(VOS_FALSE);

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}
#if (FEATURE_ON == FEATURE_CSG)

VOS_UINT32 NAS_UTRANCTRL_RcvCsgSpecSearchReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_FALSE);
        NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_UNKNOWN);
    }

    NAS_UTRANCTRL_SetSwithOnPlmnSearchFlag(VOS_FALSE);

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}
#endif


VOS_UINT32 NAS_UTRANCTRL_RcvWasStartCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_START_CNF_STRU                *pstRrStartCnfMsg;

    pstRrStartCnfMsg = (RRMM_START_CNF_STRU *)pstMsg;

    /* �����ǰUTRANCTRLģ�ĵȿ����ظ�ʱ״̬��ʱ����������Ϊ�쳣 */
    if ( NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_WAS_START_CNF))
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasStartCnf_Main: Unexpected Plmnsrch msg!");

        /* �������쳣��Ϣ */
        return VOS_TRUE;
    }

    /* ֹͣMMCͨ��UTRANCTRLģ���UTRANģ���Ϳ�����Ϣʱ����Ķ�ʱ�� */
    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_WAS_START_CNF);

    if (MMC_START_FAILURE == pstRrStartCnfMsg->ulResult)
    {
        /* ��UTRANģ�鿪��ʧ�ܣ���ֱ�ӽ�MMC״̬�����д��� */

        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_WAS_START_CNF_FAIL);

        return VOS_FALSE;
    }

    /* �л�������ģ��Ŀ���״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_SWITCH_ON);

    /* WAS�Ŀ����ظ���Ϣ������ģ��Ŀ���״̬�����д���  */
    return VOS_TRUE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitWasStartCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��������ʧ����Ϣ����ԭMMC״̬�����д��� */
    RRMM_START_CNF_STRU                *pstTempMsg      = VOS_NULL_PTR;
    VOS_UINT32                          ulTempMsgEventType;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitWasStartCnfExpired_Main: ENTERED");

    /* �����ڴ� */
    pstTempMsg = (RRMM_START_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                               sizeof(RRMM_START_CNF_STRU));
    if( VOS_NULL_PTR == pstTempMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitWasStartCnfExpired_Main: Alloc memeory Error");
        return VOS_FALSE;
    }

    /* ���쿪���ظ�ʧ����Ϣ */
    NAS_UTRANCTRL_BuildRrMmStartCnfMsg(WUEPS_PID_WRR,
                                       WUEPS_PID_MMC,
                                       MMC_START_FAILURE,
                                       pstTempMsg);

    /* ȡ�õ�ǰ������Ϣ������ */
    ulTempMsgEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstTempMsg );

    /* �����Ĺ��ĳ�ʱ��Ϣ�ŵ���������� */
    NAS_UTRANCTRL_SaveBufferUtranSndMmcMsg(ulTempMsgEventType,
                                           (struct MsgCB *)pstTempMsg);

    /* ������Ҫ�滻�����Ϣ��� */
    NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

    /* �ͷ��������Ϣ */
    PS_MEM_FREE(WUEPS_PID_MMC, pstTempMsg);

    /* ��ǰ��Ϣ�������MMC״̬������ */

    NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_WAS_START_CNF_EXPIRED);

    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvTdStartCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_START_CNF_STRU                *pstRrStartCnfMsg;

    pstRrStartCnfMsg = (RRMM_START_CNF_STRU *)pstMsg;

    /* �����ǰUTRANCTRLģ�ĵȿ����ظ�ʱ״̬��ʱ����������Ϊ�쳣 */
    if ( NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_TD_START_CNF))
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdStartCnf_Main: Unexpected Plmnsrch msg!");

        /* �������쳣��Ϣ */
        return VOS_TRUE;
    }

    /* ֹͣMMCͨ��UTRANCTRLģ���UTRANģ���Ϳ�����Ϣʱ����Ķ�ʱ�� */
    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_TD_START_CNF);

    if (MMC_START_FAILURE == pstRrStartCnfMsg->ulResult)
    {
        /* ��UTRANģ�鿪��ʧ�ܣ���ֱ�ӽ�MMC״̬�����д��� */

        NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_TDS_START_CNF_FAIL);

        return VOS_FALSE;
    }

    /* �л�������ģ��Ŀ���״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_SWITCH_ON);

    /* TD�Ŀ����ظ���Ϣ������ģ��Ŀ���״̬�����д���  */
    return VOS_TRUE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitTdStartCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��������ʧ����Ϣ����ԭMMC״̬�����д��� */
    RRMM_START_CNF_STRU                *pstTempMsg      = VOS_NULL_PTR;
    VOS_UINT32                          ulTempMsgEventType;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitTdStartCnfExpired_Main: ENTERED");

    /* �����ڴ�  */
    pstTempMsg = (RRMM_START_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                               sizeof(RRMM_START_CNF_STRU));
    if( VOS_NULL_PTR == pstTempMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitTdStartCnfExpired_Main: Alloc memeory Error");
        return VOS_FALSE;

    }

    /* ���쿪���ظ�ʧ����Ϣ */
    NAS_UTRANCTRL_BuildRrMmStartCnfMsg(WUEPS_PID_WRR,
                                       WUEPS_PID_MMC,
                                       MMC_START_FAILURE,
                                       pstTempMsg);

    /* ȡ�õ�ǰ������Ϣ������ */
    ulTempMsgEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstTempMsg );

    /* �����Ĺ��ĳ�ʱ��Ϣ�ŵ���������� */
    NAS_UTRANCTRL_SaveBufferUtranSndMmcMsg(ulTempMsgEventType,
                                           (struct MsgCB *)pstTempMsg);

    /* ������Ҫ�滻�����Ϣ��� */
    NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

    /* �ͷ��������Ϣ */
    PS_MEM_FREE(WUEPS_PID_MMC, pstTempMsg);

    /* ��ǰ��Ϣ�������MMC״̬������ */

    NAS_MML_SoftReBoot(NAS_MML_REBOOT_SCENE_MMC_WAIT_TDS_START_CNF_EXPIRED);

    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvWasPowerOffCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ǰUTRANCTRLģ�ĵȿ����ظ�ʱ״̬��ʱ����������Ϊ�쳣 */
    if ( NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_WAS_POWER_OFF_CNF))
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasPowerOffCnf_Main: Unexpected Plmnsrch msg!");

        /* �������쳣��Ϣ */
        return VOS_TRUE;
    }

    /* ֹͣMMCͨ��UTRANCTRLģ���UTRANģ���͹ػ���Ϣʱ����Ķ�ʱ�� */
    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_WAS_POWER_OFF_CNF);

    /* �л�������ģ��Ĺػ�״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_POWER_OFF);

    /* Was�Ĺػ��ظ���Ϣ������ģ��Ĺػ�״̬�����д���  */
    return VOS_TRUE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitWasPowerOffCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitWasPowerOffCnfExpired_Main: ENTERED");

    /* �����ǰΪUTRAN MODE AUTO SWITCHģʽ������Ҫ����UTRANCTRLģ��Ĺػ�״̬�� */
    /* �л�������ģ��Ĺػ�״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_POWER_OFF);

    /* Was�Ĺػ��ظ���Ϣ������ģ��Ĺػ�״̬�����д���  */
    return VOS_TRUE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvTdPowerOffCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ǰUTRANCTRLģ�ĵȿ����ظ�ʱ״̬��ʱ����������Ϊ�쳣 */
    if ( NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_TD_POWER_OFF_CNF))
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdPowerOffCnf_Main: Unexpected Plmnsrch msg!");

        /* �������쳣��Ϣ */
        return VOS_TRUE;
    }

    /* ֹͣMMCͨ��UTRANCTRLģ���UTRANģ���͹ػ���Ϣʱ����Ķ�ʱ�� */
    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_TD_POWER_OFF_CNF);

    /* �л�������ģ��Ĺػ�״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_POWER_OFF);

    /* td�Ĺػ��ظ���Ϣ������ģ��Ĺػ�״̬�����д���  */
    return VOS_TRUE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitTdPowerOffCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitTdPowerOffCnfExpired_Main: ENTERED");

    /* �л�������ģ��Ĺػ�״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_POWER_OFF);

    /* Td�Ĺػ��ظ���Ϣ������ģ��Ĺػ�״̬�����д���  */
    return VOS_TRUE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvWasSysCfgCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SYS_CFG_CNF_STRU              *pstSysCfgCnf;

    pstSysCfgCnf = (RRMM_SYS_CFG_CNF_STRU *)pstMsg;

    /* �����ǰUTRANCTRLģ�ĵȿ����ظ�ʱ״̬��ʱ����������Ϊ�쳣 */
    if ( NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_WAS_SYSCFG_CNF))
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasSysCfgCnf_Main: Unexpected Plmnsrch msg!");

        /* �������쳣��Ϣ */
        return VOS_TRUE;
    }

    /* ֹͣMMCͨ��UTRANCTRLģ���UTRANģ����SYSCFG������Ϣʱ����Ķ�ʱ�� */
    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_WAS_SYSCFG_CNF);

    if ( RRMM_SYS_CFG_RESULT_FAILURE == pstSysCfgCnf->ulResult)
    {
        /* ��syscfg����ʧ�ܣ���ֱ�ӽ�MMC״̬�����д��� */
        return VOS_FALSE;
    }

    /* ��syscfg���ûظ��ɹ����л�������ģ���SYSCFG״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_SYSCFG);

    /* Was��SYSCFG�ظ���Ϣ������ģ���SYSCFG״̬�����д���  */
    return VOS_TRUE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitWasSysCfgCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SYS_CFG_CNF_STRU              *pstTempMsg      = VOS_NULL_PTR;
    VOS_UINT32                          ulTempMsgEventType;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitWasSysCfgCnfExpired_Main: ENTERED");

    /* ����SYSCFG����ʧ����Ϣ����ԭMMC״̬�����д��� */
    pstTempMsg = (RRMM_SYS_CFG_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                               sizeof(RRMM_SYS_CFG_CNF_STRU));
    if( VOS_NULL_PTR == pstTempMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitWasSysCfgCnfExpired_Main: Alloc memeory Error");
        return VOS_TRUE ;
    }

    /* ����SYSCFG���ûظ���Ϣ */
    NAS_UTRANCTRL_BuildRrMmSysCfgCnfMsg(WUEPS_PID_WRR,
                                        WUEPS_PID_MMC,
                                        RRMM_SYS_CFG_RESULT_FAILURE,
                                        pstTempMsg);

    ulTempMsgEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstTempMsg );

    /* �����Ĺ��ĳ�ʱ��Ϣ�ŵ���������� */
    NAS_UTRANCTRL_SaveBufferUtranSndMmcMsg(ulTempMsgEventType,
                                           (struct MsgCB *)pstTempMsg);

    /* ������Ҫ�滻�����Ϣ��� */
    NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

    /* �ͷ��������Ϣ */
    PS_MEM_FREE(WUEPS_PID_MMC, pstTempMsg);

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvTdSysCfgCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SYS_CFG_CNF_STRU              *pstSysCfgCnf;

    pstSysCfgCnf = (RRMM_SYS_CFG_CNF_STRU *)pstMsg;

    /* �����ǰUTRANCTRLģ�ĵȿ����ظ�ʱ״̬��ʱ����������Ϊ�쳣 */
    if ( NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_TD_SYSCFG_CNF))
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdSysCfgCnf_Main: Unexpected Plmnsrch msg!");

        /* �������쳣��Ϣ */
        return VOS_TRUE;
    }

    /* ֹͣMMCͨ��UTRANCTRLģ���UTRANģ����SYSCFG������Ϣʱ����Ķ�ʱ�� */
    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_TD_SYSCFG_CNF);

    if ( RRMM_SYS_CFG_RESULT_FAILURE == pstSysCfgCnf->ulResult)
    {
        /* ��syscfg����ʧ�ܣ���ֱ�ӽ�MMC״̬�����д��� */
        return VOS_FALSE;
    }

    /* �л�������ģ���SYSCFG״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_SYSCFG);

    /* Td��SYSCFG�ظ���Ϣ������ģ���SYSCFG״̬�����д���  */
    return VOS_TRUE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitTdSysCfgCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SYS_CFG_CNF_STRU              *pstTempMsg      = VOS_NULL_PTR;
    VOS_UINT32                          ulTempMsgEventType;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitTdSysCfgCnfExpired_Main: ENTERED");

    /* ����SYSCFG����ʧ����Ϣ����ԭMMC״̬�����д��� */
    pstTempMsg = (RRMM_SYS_CFG_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                               sizeof(RRMM_SYS_CFG_CNF_STRU));
    if( VOS_NULL_PTR == pstTempMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitTdSysCfgCnfExpired_Main: Alloc memeory Error");
        return VOS_TRUE ;
    }

    /* ����SYSCFG���ûظ���Ϣ */
    NAS_UTRANCTRL_BuildRrMmSysCfgCnfMsg(WUEPS_PID_WRR,
                                        WUEPS_PID_MMC,
                                        RRMM_SYS_CFG_RESULT_FAILURE,
                                        pstTempMsg);

    ulTempMsgEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstTempMsg );

    /* �����Ĺ��ĳ�ʱ��Ϣ�ŵ���������� */
    NAS_UTRANCTRL_SaveBufferUtranSndMmcMsg(ulTempMsgEventType,
                                           (struct MsgCB *)pstTempMsg);

    /* ������Ҫ�滻�����Ϣ��� */
    NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

    /* �ͷ��������Ϣ */
    PS_MEM_FREE(WUEPS_PID_MMC, pstTempMsg);

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvWasPlmnSrchCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU          *pstSrchCnfMsg;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enUtranMode;


    enUtranMode                 = NAS_UTRANCTRL_GetCurrUtranMode();

    pstSrchCnfMsg   = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* �����ǰUTRANCTRLģ�ĵ�ָ�������ظ�ʱ״̬��ʱ��������ֹͣ��ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_WAS_PLMN_SEARCH_CNF))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasPlmnSrchCnf_Main:TI_NAS_UTRANCTRL_WAIT_WAS_PLMN_SEARCH_CNF not running");
        return VOS_FALSE;
    }

    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_WAS_PLMN_SEARCH_CNF);

    /* ��Ȧ���Ӷ� */
    if (VOS_FALSE == NAS_UTRANCTRL_IsNeedProcGuPlmnSearchCnf(NAS_MML_NET_RAT_TYPE_WCDMA, pstSrchCnfMsg))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasPlmnSrchCnf_Main: No need process in UTRANCTRL, Clear Buffered Msg and enter MMC FSM");
        /* �б��ѻ���PrefBand������Ҫ���� enRoamingZone ��־ */
        if ((RRC_PLMN_SEARCH_RLT_USER_LIST== pstSrchCnfMsg->ulSearchRlt)
         || (RRC_PLMN_SEARCH_RLT_PREF_BAND_FAIL == pstSrchCnfMsg->ulSearchRlt))
        {
            /* ָ������ʧ�ܸ��������б�����й������� */
            NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                     NAS_UTRANCTRL_GetSpecTdMccList(),
                                                     &(pstSrchCnfMsg->PlmnIdList));
        }

        /* ������� */
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* �Զ��л�ģʽ�£�CSFB��־����ʱ�����ҵ�ǰ��FDDģʽ��������TD */
    if ((VOS_TRUE                       == NAS_MML_IsCsfbServiceStatusExist())
     && (NAS_UTRANCTRL_UTRAN_MODE_FDD   == enUtranMode))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasPlmnSrchCnf_Main: CSFB and FDD mode, Clear Buffered Msg and enter MMC FSM");
        /* ������������������Ϣ */
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* ָ������ʧ�ܸ��������б�����й������� */
    NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                             NAS_UTRANCTRL_GetSpecTdMccList(),
                                             &(pstSrchCnfMsg->PlmnIdList));

    if (NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND == NAS_UTRANCTRL_GetRoamingZone())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasPlmnSrchCnf_Main: MCC not in TDS MCC LIST and not in Hongkong, Clear buffer and enter MMC FSM");

        /* �����������������was���������û��һ������mcc��tds mcc�б����Ҳ�����������磬�Ҹߵ��������������ȫΪ0��
           ������tds�������������*/
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* Ϊ�˽��͹��ģ�history�ѻ�pref band��failʱW��TD�������� */
    if ((RRC_PLMN_SEARCH_RLT_HISTORY_FAIL   == pstSrchCnfMsg->ulSearchRlt)
     || (RRC_PLMN_SEARCH_RLT_PREF_BAND_FAIL == pstSrchCnfMsg->ulSearchRlt))
    {
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* �л�������ģ���ѡ��״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_PLMN_SELECTION);

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasPlmnSrchCnf_Main: enter NAS_UTRANCTRL_FSM_PLMN_SELECTION");

    /* was�������ظ���Ϣ������ģ���ѡ��״̬�����д���  */
    return VOS_TRUE;
}


VOS_UINT32  NAS_UTRANCTRL_RcvWasSearchedPlmnInfoInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SEARCHED_PLMN_INFO_IND_STRU   *pstSearchedPlmnInfoMsg = VOS_NULL_PTR;
    VOS_UINT32                         *pulTdMccList  = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          ulIsNonTdMccFound;
    VOS_UINT8                           ucTdMccListNum;
    NAS_UTRANCTRL_ROAM_ZONE_ENUM_UINT8  enRoamingZone;

    ulIsNonTdMccFound                   = VOS_FALSE;
    pulTdMccList                        = NAS_UTRANCTRL_GetSpecTdMccList();
    ucTdMccListNum                      = NAS_UTRANCTRL_GetSpecTdMccListNum();
    enRoamingZone                       = NAS_UTRANCTRL_GetRoamingZone();
    pstSearchedPlmnInfoMsg              = (RRMM_SEARCHED_PLMN_INFO_IND_STRU*)pstMsg;

    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        for (i = 0; i < pstSearchedPlmnInfoMsg->ulAvailPlmnNum; i++)
        {
            /* �ж�MCC�Ƿ���TD MCC�б��� */
            if (VOS_TRUE == NAS_MML_IsMccInDestMccList(pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMcc, ucTdMccListNum, pulTdMccList))
            {
                NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
            }
            else
            {
                ulIsNonTdMccFound = VOS_TRUE;
            }
        }

        /* ���������TdMcc������ݵ�ǰenRoamingZone��ֵ������ΪCHINA_MAINLAND����CHINA_BORDER
         * ��������ѵ�������Mcc��������ΪOUTSIDE_CHINA_MAINLAND
         */
        if (VOS_TRUE == NAS_UTRANCTRL_GetSearchedSpecTdMccFlg())
        {
            if ((NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND == enRoamingZone)
             || (VOS_TRUE == ulIsNonTdMccFound))
            {
                NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_BORDER);
            }
            else
            {
                NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
            }
        }
        else if (VOS_TRUE == ulIsNonTdMccFound)
        {
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND);
        }

        NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvWasSearchedPlmnInfoInd_Main:", NAS_UTRANCTRL_GetRoamingZone());
    }

    /* ����MMC״̬������ */
    return VOS_FALSE;
}


VOS_UINT32  NAS_UTRANCTRL_RcvGasSearchedPlmnInfoInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SEARCHED_PLMN_INFO_IND_STRU   *pstSearchedPlmnInfoMsg = VOS_NULL_PTR;
    VOS_UINT32                         *pulTdMccList  = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          ulIsNonTdMccFound;
    VOS_UINT8                           ucTdMccListNum;
    NAS_UTRANCTRL_ROAM_ZONE_ENUM_UINT8  enRoamingZone;

    ulIsNonTdMccFound                   = VOS_FALSE;
    pulTdMccList                        = NAS_UTRANCTRL_GetSpecTdMccList();
    ucTdMccListNum                      = NAS_UTRANCTRL_GetSpecTdMccListNum();
    enRoamingZone                       = NAS_UTRANCTRL_GetRoamingZone();
    pstSearchedPlmnInfoMsg              = (RRMM_SEARCHED_PLMN_INFO_IND_STRU*)pstMsg;

    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        for (i = 0; i < pstSearchedPlmnInfoMsg->ulAvailPlmnNum; i++)
        {
            /* �ж�MCC�Ƿ���TD MCC�б��� */
            if (VOS_TRUE == NAS_MML_IsMccInDestMccList(pstSearchedPlmnInfoMsg->astPlmnWithLacList[i].stPlmnId.ulMcc, ucTdMccListNum, pulTdMccList))
            {
                NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
            }
            else
            {
                ulIsNonTdMccFound = VOS_TRUE;
            }
        }

        /* ���������TdMcc������ݵ�ǰenRoamingZone��ֵ������ΪCHINA_MAINLAND����CHINA_BORDER
         * ��������ѵ�������Mcc��������ΪOUTSIDE_CHINA_MAINLAND
         */
        if (VOS_TRUE == NAS_UTRANCTRL_GetSearchedSpecTdMccFlg())
        {
            if ((NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND == enRoamingZone)
             || (VOS_TRUE == ulIsNonTdMccFound))
            {
                NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_BORDER);
            }
            else
            {
                NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
                NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_TDD);
            }
        }
        else if (VOS_TRUE == ulIsNonTdMccFound)
        {
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND);
            NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_FDD);
        }

        NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvGasSearchedPlmnInfoInd_Main:", NAS_UTRANCTRL_GetRoamingZone());
    }

    /* ����MMC״̬������ */
    return VOS_FALSE;
}


VOS_UINT32  NAS_UTRANCTRL_RcvLmmSearchedPlmnInfoInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU                    *pstSearchedPlmnInfoMsg = VOS_NULL_PTR;
    VOS_UINT32                                             *pulTdMccList  = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulIsNonTdMccFound;
    VOS_UINT8                                               ucTdMccListNum;
    NAS_UTRANCTRL_ROAM_ZONE_ENUM_UINT8                      enRoamingZone;
    NAS_MML_PLMN_ID_STRU                                    stGuNasPlmn;

    ulIsNonTdMccFound                   = VOS_FALSE;
    pulTdMccList                        = NAS_UTRANCTRL_GetSpecTdMccList();
    ucTdMccListNum                      = NAS_UTRANCTRL_GetSpecTdMccListNum();
    enRoamingZone                       = NAS_UTRANCTRL_GetRoamingZone();
    pstSearchedPlmnInfoMsg              = (LMM_MMC_SEARCHED_PLMN_INFO_IND_STRU*)pstMsg;

    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        for (i = 0; i < pstSearchedPlmnInfoMsg->ulTaiNum; i++)
        {
            /* ��LMM��PLMNת����GU NAS��PLMN��ʽ */
            NAS_MMC_ConvertLmmPlmnToGUNasFormat(&(pstSearchedPlmnInfoMsg->stTaiList[i].stPlmnId),
                                                &stGuNasPlmn);

            /* �ж�MCC�Ƿ���TD MCC�б��� */
            if (VOS_TRUE == NAS_MML_IsMccInDestMccList(stGuNasPlmn.ulMcc, ucTdMccListNum, pulTdMccList))
            {
                NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
            }
            else
            {
                ulIsNonTdMccFound = VOS_TRUE;
            }
        }

        /* ���������TdMcc������ݵ�ǰenRoamingZone��ֵ������ΪCHINA_MAINLAND����CHINA_BORDER
         * ��������ѵ�������Mcc��������ΪOUTSIDE_CHINA_MAINLAND
         */
        if (VOS_TRUE == NAS_UTRANCTRL_GetSearchedSpecTdMccFlg())
        {
            if ((NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND == enRoamingZone)
             || (VOS_TRUE == ulIsNonTdMccFound))
            {
                NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_BORDER);
            }
            else
            {
                NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);
                NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_TDD);
            }
        }
        else if (VOS_TRUE == ulIsNonTdMccFound)
        {
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND);
            NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_FDD);
        }

        NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvLmmSearchedPlmnInfoInd_Main:", NAS_UTRANCTRL_GetRoamingZone());
    }

    /* ����MMC״̬������ */
    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvInterAbortUtranCtrlPlmnSearchReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* �����ǰUTRANCTRLģ�ĵ�ָ�������ظ�ʱ״̬��ʱ��������ֹͣ��ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_WAS_PLMN_SEARCH_CNF))
    {
        NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_WAS_PLMN_SEARCH_CNF);
    }

    /* ����utran modeΪfdd��utranctrl ����״̬���˳�����Ϣ��mmc��������֪ͨ���������mmc���� */
    NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_FDD);

    /* ֪ͨmmc MMCMMC_INTER_ABORT_UTRAN_CTRL_PLMN_SEARCH_CNF*/
    NAS_MMC_SndInterAbortUtranCtrlPlmnSearchCnfMsg();

    return VOS_TRUE;

}









VOS_UINT32 NAS_UTRANCTRL_RcvMmcInterSkipSearchWasIndMsg_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���������TD���Ͳ���Ҫ��״̬���������ѵ�TD search cnf��ʱ�������W */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_TD_PLMN_SEARCH_CNF))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvMmcInterSkipSearchWasIndMsg_Main: TI_NAS_UTRANCTRL_WAIT_TD_PLMN_SEARCH_CNF runing, return");
        return VOS_TRUE;
    }

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvMmcInterSkipSearchWasIndMsg_Main: Set to NAS_UTRANCTRL_UTRAN_MODE_TDD and enter NAS_UTRANCTRL_FSM_PLMN_SELECTION");

    /* SKIP Wʱ�������õ�ǰΪTDDģʽ */
    NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_TDD);

    NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_TRUE);
    NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND);

    /* �л�������ģ���ѡ��״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_PLMN_SELECTION);

    /* was�������ظ���Ϣ������ģ���ѡ��״̬�����д���  */
    return VOS_TRUE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvMmcInterSkipSearchTdsIndMsg_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* SKIP TDSʱ�������õ�ǰΪFDDģʽ */
    NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_FDD);

    /* �л�������ģ���ѡ��״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_PLMN_SELECTION);

    /* tds�������ظ���Ϣ������ģ���ѡ��״̬�����д���  */
    return VOS_TRUE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitWasPlmnSearchCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_UTRANCTRL_ENTRY_MSG_STRU       *pstBufferEntryMsg = VOS_NULL_PTR;

    NAS_UTRANCTRL_ROAM_ZONE_ENUM_UINT8  enRoamingZone;

    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enUtranMode;
    RRC_PLMN_SEARCH_TYPE_ENUM_UINT32    enPlmnSearchType;

    enPlmnSearchType    = RRC_PLMN_SEARCH_TYPE_BUTT;

    enUtranMode     = NAS_UTRANCTRL_GetCurrUtranMode();
    enRoamingZone   = NAS_UTRANCTRL_GetRoamingZone();

    /* ��õ�ǰ����Ļ�������ַ */
    pstBufferEntryMsg = NAS_UTRANCTRL_GetBufferUtranSndMmcMsgAddr();

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitWasPlmnSearchCnfExpired_Main: ENTERED");

    /* CSFB��־����ʱ�����ҵ�ǰ��FDDģʽ��������TD */
    if ((VOS_TRUE                       == NAS_MML_IsCsfbServiceStatusExist())
     && (NAS_UTRANCTRL_UTRAN_MODE_FDD   == enUtranMode))
    {
        /* ����ָ�������ظ�ʧ����Ϣ���ڴ�Ļ������� */
        NAS_UTRANCTRL_BuildRrMmPlmnSearchCnfFailMsg(WUEPS_PID_WRR,
                                                        WUEPS_PID_MMC,
                                                        NAS_UTRANCTRL_GetBufferedPlmnSearchReqType(),
                                                       (RRMM_PLMN_SEARCH_CNF_STRU *)pstBufferEntryMsg->aucEntryMsgBuffer);

        /* ���湹��ָ�������ظ�ʧ����Ϣ�¼����� */
        pstBufferEntryMsg->ulEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstBufferEntryMsg->aucEntryMsgBuffer );

        /* ������Ҫ�滻�����Ϣ��� */
        NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

        /* ��ǰ��Ϣ�������MMC״̬������ */
        return VOS_FALSE;
    }

    /* Ϊ��ʡ���ģ�history��pref band��ʱW��TD�������� */
    enPlmnSearchType    = NAS_UTRANCTRL_GetBufferedPlmnSearchReqType();
    if ((RRC_PLMN_SEARCH_HISTORY == enPlmnSearchType)
     || (RRC_PLMN_SEARCH_PREF_BAND == enPlmnSearchType))
    {
        NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitWasPlmnSearchCnfExpired_Main: enPlmnSearchType is ", enPlmnSearchType);

        /* ����W�����ظ���ʱ����ʱ����Ϣ��MMC���� */
        NAS_UTRANCTRL_BuildTimerExpireMsg(WUEPS_PID_MMC,
                                          TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF,
                                          (REL_TIMER_MSG *)pstBufferEntryMsg->aucEntryMsgBuffer);

        /* ���湹��ָ�������ظ�ʧ����Ϣ�¼����� */
        pstBufferEntryMsg->ulEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstBufferEntryMsg->aucEntryMsgBuffer );

        /* ������Ҫ�滻�����Ϣ��� */
        NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

        /* ��ǰ��Ϣ�������MMC״̬������ */
        return VOS_FALSE;
    }

    /* �����ǰΪUTRAN MODE AUTO SWITCHģʽ������Ҫ����UTRANCTRLģ�������״̬�� */
    if ((NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
     && (NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND != enRoamingZone))
    {
        /* �л�������ģ���ѡ��״̬�����д��� */
        NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_PLMN_SELECTION);

        /* ���������ѡ��״̬������  */
        return VOS_TRUE;
    }

    /* ����ָ�������ظ�ʧ����Ϣ���ڴ�Ļ������� */
    NAS_UTRANCTRL_BuildRrMmPlmnSearchCnfFailMsg(WUEPS_PID_WRR,
                                                WUEPS_PID_MMC,
                                                enPlmnSearchType,
                                                (RRMM_PLMN_SEARCH_CNF_STRU *)pstBufferEntryMsg->aucEntryMsgBuffer);

    /* ���湹��ָ�������ظ�ʧ����Ϣ�¼����� */
    pstBufferEntryMsg->ulEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstBufferEntryMsg->aucEntryMsgBuffer );

    /* ������Ҫ�滻�����Ϣ��� */
    NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvWasRrMmRelInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_REL_IND_STRU                  *pstRrmmRelIndMsg    = VOS_NULL_PTR;
    NAS_UTRANCTRL_ROAM_ZONE_ENUM_UINT8  enRoamingZone;

    pstRrmmRelIndMsg = (RRMM_REL_IND_STRU *)pstMsg;

    /* �����ǰUTRANCTRLģ�ĵ������ظ�ʱ״̬��ʱ�����У���ֹͣ��ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_WAS_PLMN_SEARCH_CNF))
    {
        /* ��ǰ��Ϣ�������MMC״̬������ */
        return VOS_FALSE;
    }

    /* WAS����ʱ����㸴λ����TDģ */
    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_WAS_PLMN_SEARCH_CNF);


    /* ���ԭ��ΪNO RF�������MMC״̬�����д��� */
    if (RRC_REL_CAUSE_NO_RF == pstRrmmRelIndMsg->ulRelCause)
    {
        return VOS_FALSE;
    }

    enRoamingZone  = NAS_UTRANCTRL_GetRoamingZone();

    if ((NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
     && (NAS_UTRANCTRL_ROAM_ZONE_OUTSIDE_CHINA_MAINLAND != enRoamingZone))
    {
        /* �л�������ģ���ѡ��״̬�����д��� */
        NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_PLMN_SELECTION);

        /* ��ǰ��Ϣ���������ѡ��״̬������ */
        return VOS_TRUE;
    }

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvGasPlmnSrchCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU          *pstSrchCnfMsg = VOS_NULL_PTR;

    pstSrchCnfMsg   = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* ��Ȧ���Ӷ� */
    if (VOS_FALSE == NAS_UTRANCTRL_IsNeedProcGuPlmnSearchCnf(NAS_MML_NET_RAT_TYPE_GSM, pstSrchCnfMsg))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvGasPlmnSrchCnf_Main: No need process in UTRANCTRL, Clear Buffered Msg and enter MMC FSM");

        /* �б��ѻ���PrefBand������Ҫ���� enRoamingZone ��־ */
        if (RRC_PLMN_SEARCH_RLT_USER_LIST == pstSrchCnfMsg->ulSearchRlt)
        {
            /* ָ������ʧ�ܸ��������б�����й������� */
            NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                     NAS_UTRANCTRL_GetSpecTdMccList(),
                                                     &(pstSrchCnfMsg->PlmnIdList));
        }
        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* ���������б�����й������� */
    NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                             NAS_UTRANCTRL_GetSpecTdMccList(),
                                             &(pstSrchCnfMsg->PlmnIdList));

    /* ����״̬������ */
    return VOS_FALSE;
}

#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_UTRANCTRL_RcvLmmPlmnSrchCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstLmmSrchCnfMsg;
    RRC_PLMN_ID_LIST_STRU                                  *pstGuPlmnIdList;

    pstLmmSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU*)pstMsg;

    pstGuPlmnIdList   = (RRC_PLMN_ID_LIST_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                                                        sizeof(RRC_PLMN_ID_LIST_STRU));
    if (VOS_NULL_PTR == pstGuPlmnIdList)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvLmmPlmnSrchCnf_Main:Invalid pstGuPlmnIdList");
        return VOS_FALSE;
    }

    /* ��ȡ������utranctrl����Ҫ�������Ϣ�򳡾������ں����� */
    if (VOS_FALSE == NAS_UTRANCTRL_IsNeedProcLmmPlmnSearchCnf(NAS_MML_NET_RAT_TYPE_LTE, pstLmmSrchCnfMsg))
    {
        /* �б��ѻ���PrefBand������Ҫ���� enRoamingZone */
        if (MMC_LMM_PLMN_SRCH_RLT_USER_LIST_SUCC == pstLmmSrchCnfMsg->enRlst)
        {
            /*ת��L��ʽ�������б��ΪGU��ʽ��*/
            NAS_MMC_ConvertLmmPlmnIdListToGUFormat(&(pstLmmSrchCnfMsg->stPlmnIdList), pstGuPlmnIdList);

            /* ָ������ʧ�ܸ��������б�����й������� */
            NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                     NAS_UTRANCTRL_GetSpecTdMccList(),
                                                     pstGuPlmnIdList);
        }

        PS_MEM_FREE(WUEPS_PID_MMC, pstGuPlmnIdList);

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /*ת��L��ʽ�������б��ΪGU��ʽ��*/
    NAS_MMC_ConvertLmmPlmnIdListToGUFormat(&(pstLmmSrchCnfMsg->stPlmnIdList), pstGuPlmnIdList);

    /* ���������б�����й������� */
    NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                             NAS_UTRANCTRL_GetSpecTdMccList(),
                                             pstGuPlmnIdList);

    PS_MEM_FREE(WUEPS_PID_MMC, pstGuPlmnIdList);

    /* ����MMC״̬������ */
    return VOS_FALSE;
}
#endif


VOS_UINT32 NAS_UTRANCTRL_RcvTdPlmnSrchCnf_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU          *pstSrchCnfMsg = VOS_NULL_PTR;

    VOS_UINT32                          ulIsOtherModemPlmnInTdMccList;
    NAS_MML_PLMN_ID_STRU               *pstOtherModemPlmnId = VOS_NULL_PTR;

    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enUtranMode;

    enUtranMode     = NAS_UTRANCTRL_GetCurrUtranMode();

    pstSrchCnfMsg   = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

    /* �����ǰUTRANCTRLģ�ĵ�ָ�������ظ�ʱ״̬��ʱ��������ֹͣ��ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING != NAS_UTRANCTRL_GetTimerStatus(TI_NAS_UTRANCTRL_WAIT_TD_PLMN_SEARCH_CNF))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdPlmnSrchCnf_Main TI_NAS_UTRANCTRL_WAIT_TD_PLMN_SEARCH_CNF NOT RUNNING");
        return VOS_FALSE;
    }

    NAS_UTRANCTRL_StopTimer(TI_NAS_UTRANCTRL_WAIT_TD_PLMN_SEARCH_CNF);

    /* ��Ȧ���Ӷ� */
    if (VOS_FALSE == NAS_UTRANCTRL_IsNeedProcGuPlmnSearchCnf(NAS_MML_NET_RAT_TYPE_WCDMA, pstSrchCnfMsg))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdPlmnSrchCnf_Main: No need process in UTRANCTRL, Clear Buffered Msg and enter MMC FSM");

        /* �б��ѻ���PrefBand������Ҫ���� enRoamingZone ��־ */
        if (RRC_PLMN_SEARCH_RLT_USER_LIST == pstSrchCnfMsg->ulSearchRlt)
        {
            /* ָ������ʧ�ܸ��������б�����й������� */
            NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                     NAS_UTRANCTRL_GetSpecTdMccList(),
                                                     &(pstSrchCnfMsg->PlmnIdList));
        }
        /* ������������������Ϣ */
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* �Զ��л�ģʽ�£�CSFB��־����ʱ�����ҵ�ǰ��TDDģʽ��������W */
    if ((VOS_TRUE                       == NAS_MML_IsCsfbServiceStatusExist())
     && (NAS_UTRANCTRL_UTRAN_MODE_TDD   == enUtranMode))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdPlmnSrchCnf_Main: CSFB and FDD mode, Clear Buffered Msg and enter MMC FSM");
        /* ������������������Ϣ */
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* ���������б�����й������� */
    NAS_UTRANCTRL_UpdateRoamZoneRelatedFlags(NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                             NAS_UTRANCTRL_GetSpecTdMccList(),
                                             &(pstSrchCnfMsg->PlmnIdList));

    /* �����û����ε���������utran����ģʽ */
    if (NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND == NAS_UTRANCTRL_GetRoamingZone())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdPlmnSrchCnf_Main: Set to NAS_UTRANCTRL_UTRAN_MODE_TDD");
        NAS_UTRANCTRL_SetCurrUtranMode(NAS_UTRANCTRL_UTRAN_MODE_TDD);

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    pstOtherModemPlmnId = NAS_MMC_GetOtherModemPlmnId();
    ulIsOtherModemPlmnInTdMccList = NAS_MML_IsMccInDestMccList(pstOtherModemPlmnId->ulMcc,
                                                NAS_UTRANCTRL_GetSpecTdMccListNum(),
                                                NAS_UTRANCTRL_GetSpecTdMccList());

    if ((VOS_TRUE == NAS_MMC_GetUtranSkipWPlmnSearchFlag())
     && (VOS_TRUE == ulIsOtherModemPlmnInTdMccList))
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdPlmnSrchCnf_Main: Other modem registered on TDD NW, clear buffer and enter MMC FSM");
        /* ���������פ���й�����������w���磬������������������Ϣ */
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* Ϊ�˽��͹��ģ�history�Ѻ�pref band��failʱֻ���±�־����W��TD�������� */
    if ((RRC_PLMN_SEARCH_RLT_HISTORY_FAIL   == pstSrchCnfMsg->ulSearchRlt)
     || (RRC_PLMN_SEARCH_RLT_PREF_BAND_FAIL == pstSrchCnfMsg->ulSearchRlt))
    {
        NAS_UTRANCTRL_ClearBufferedSndUtranReqMsg();

        /* ����MMC״̬������ */
        return VOS_FALSE;
    }

    /* �л�������ģ���ѡ��״̬�����д��� */
    NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_PLMN_SELECTION);

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTdPlmnSrchCnf_Main: enter NAS_UTRANCTRL_FSM_PLMN_SELECTION");

    /* tdrrc�������ظ���Ϣ������ģ���ѡ��״̬�����д��� */
    return VOS_TRUE;
}



VOS_UINT32 NAS_UTRANCTRL_RcvTiWaitTdPlmnSearchCnfExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_UTRANCTRL_ENTRY_MSG_STRU                           *pstBufferEntryMsg = VOS_NULL_PTR;
    NAS_UTRANCTRL_UTRAN_SWITCH_MODE_ENUM_UINT8              enCurUtranSwitchMode;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8                     enUtranMode;
    RRC_PLMN_SEARCH_TYPE_ENUM_UINT32                        enPlmnSearchType;

    enPlmnSearchType    = RRC_PLMN_SEARCH_TYPE_BUTT;

    enUtranMode       = NAS_UTRANCTRL_GetCurrUtranMode();

    /* ��õ�ǰ����Ļ�������ַ */
    pstBufferEntryMsg = NAS_UTRANCTRL_GetBufferUtranSndMmcMsgAddr();

    enCurUtranSwitchMode = NAS_UTRANCTRL_GetCurUtranSwitchMode();

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitTdPlmnSearchCnfExpired_Main: ENTERED");

    /* �Զ��л�ģʽ�£�CSFB��־����ʱ�����ҵ�ǰ��TDDģʽ��������W */
    if ((VOS_TRUE                       == NAS_MML_IsCsfbServiceStatusExist())
     && (NAS_UTRANCTRL_UTRAN_MODE_TDD   == enUtranMode))
    {
        /* ����ָ�������ظ�ʧ����Ϣ���ڴ�Ļ������� */
        NAS_UTRANCTRL_BuildRrMmPlmnSearchCnfFailMsg(WUEPS_PID_WRR,
                                                    WUEPS_PID_MMC,
                                                    NAS_UTRANCTRL_GetBufferedPlmnSearchReqType(),
                                                   (RRMM_PLMN_SEARCH_CNF_STRU *)pstBufferEntryMsg->aucEntryMsgBuffer);

        /* ���湹��ָ�������ظ�ʧ����Ϣ�¼����� */
        pstBufferEntryMsg->ulEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstBufferEntryMsg->aucEntryMsgBuffer );

        /* ������Ҫ�滻�����Ϣ��� */
        NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

        /* ��ǰ��Ϣ�������MMC״̬������ */
        return VOS_FALSE;
    }


    /* Ϊ��ʡ���ģ�history��pref band��ʱW��TD�������� */
    enPlmnSearchType    = NAS_UTRANCTRL_GetBufferedPlmnSearchReqType();
    if ((RRC_PLMN_SEARCH_HISTORY == enPlmnSearchType)
     || (RRC_PLMN_SEARCH_PREF_BAND == enPlmnSearchType))
    {
        NAS_NORMAL_LOG1(WUEPS_PID_MMC, "NAS_UTRANCTRL_RcvTiWaitTdPlmnSearchCnfExpired_Main: enPlmnSearchType is ", enPlmnSearchType);

        /* ����W�����ظ���ʱ����ʱ����Ϣ��MMC���� */
        NAS_UTRANCTRL_BuildTimerExpireMsg(WUEPS_PID_MMC,
                                          TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF,
                                          (REL_TIMER_MSG *)pstBufferEntryMsg->aucEntryMsgBuffer);

        /* ���湹��ָ�������ظ�ʧ����Ϣ�¼����� */
        pstBufferEntryMsg->ulEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstBufferEntryMsg->aucEntryMsgBuffer );

        /* ������Ҫ�滻�����Ϣ��� */
        NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

        /* ��ǰ��Ϣ�������MMC״̬������ */
        return VOS_FALSE;
    }


    /* �����ǰΪUTRAN MODE AUTO SWITCHģʽ������Ҫ����UTRANCTRLģ�������״̬�� */
    if ((NAS_UTRANCTRL_ROAM_ZONE_CHINA_MAINLAND != NAS_UTRANCTRL_GetRoamingZone())
     && (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == enCurUtranSwitchMode))
    {
        /* �л�������ģ���ѡ��״̬�����д��� */
        NAS_UTRANCTRL_SwitchCurrFsmCtx(NAS_UTRANCTRL_FSM_PLMN_SELECTION);

        /* ���������ѡ��״̬������ */
        return VOS_TRUE;
    }

    /* ����ָ�������ظ�ʧ����Ϣ���ڴ�Ļ������� */
    NAS_UTRANCTRL_BuildRrMmPlmnSearchCnfFailMsg(WUEPS_PID_WRR,
                                                WUEPS_PID_MMC,
                                                enPlmnSearchType,
                                               (RRMM_PLMN_SEARCH_CNF_STRU *)pstBufferEntryMsg->aucEntryMsgBuffer);

    /* ���湹��ָ�������ظ�ʧ����Ϣ�¼����� */
    pstBufferEntryMsg->ulEventType = NAS_UTRANCTRL_GetMsgEventType( (struct MsgCB *)pstBufferEntryMsg->aucEntryMsgBuffer );

    /* ������Ҫ�滻�����Ϣ��� */
    NAS_UTRANCTRL_SetReplaceMmcMsgFlg(VOS_TRUE);

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvMmcInterPlmnSearchReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU                     *pstInterPlmnSearchReq;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;

    pstInterPlmnSearchReq = (NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU*)pstMsg;

    enPlmnSearchScene     = pstInterPlmnSearchReq->enPlmnSearchScene;

    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_FALSE);

        /* ������ Available Timer Expired (Except Ncell) scenes ������ѡ�������� Roaming zone��RF enable scene */
        /* ��Ϊ��֮ǰ�Ѿ��޸ģ����Բ���Ҫ�޸ģ�������������ֵ���䣬��֤����������ȥ�� TD ���� Wcdma */
        if ((VOS_FALSE == NAS_MML_IsUsimStausValid())
         || (NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED == enPlmnSearchScene)
         || (NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_HISTORY_PLMN_SEARCH == enPlmnSearchScene)
         || (NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_PREFBAND_PLMN_SEARCH == enPlmnSearchScene))
        {
            NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_UNKNOWN);
        }
    }

    NAS_UTRANCTRL_SetSwithOnPlmnSearchFlag(VOS_FALSE);

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvMmcInterAnycellSearchReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_FALSE);
        NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_UNKNOWN);
    }

    NAS_UTRANCTRL_SetSwithOnPlmnSearchFlag(VOS_FALSE);

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvMmcInterPlmnListReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (NAS_UTRANCTRL_UTRAN_SWITCH_MODE_AUTO == NAS_UTRANCTRL_GetCurUtranSwitchMode())
    {
        NAS_UTRANCTRL_SetSearchedSpecTdMccFlg(VOS_FALSE);
        NAS_UTRANCTRL_SetRoamingZone(NAS_UTRANCTRL_ROAM_ZONE_UNKNOWN);
    }

    NAS_UTRANCTRL_SetSwithOnPlmnSearchFlag(VOS_FALSE);

    /* ��ǰ��Ϣ�������MMC״̬������ */
    return VOS_FALSE;
}




VOS_UINT32 NAS_UTRANCTRL_RcvTiHplmnTimerExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}


VOS_UINT32 NAS_UTRANCTRL_RcvTiTryingHighPrioPlmnSearchExpired_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}


/*lint -restore */


#endif  /* END OF #if (FEATURE_ON == FEATURE_UE_MODE_TDS) */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
