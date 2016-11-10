

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "CnasHsdFsmMain.h"
#include  "CnasHsdFsmMainTbl.h"
#include  "CnasHsdCtx.h"
#include  "CnasMntn.h"
#include  "CnasHsdComFunc.h"
#include  "CnasHsdSndMscc.h"
#include  "CnasHsdSndCas.h"
#include  "CnasPrlApi.h"
#include  "CnasCcb.h"
#include  "CnasHsdFsmSysAcq.h"
#include  "CnasTimerMgmt.h"
#include  "CnasHsdSysAcqStrategy.h"
#include  "CnasHsdAvoidStrategy.h"
#include  "cas_hrpd_overheadmsg_nas_pif.h"
#include  "CnasHsdSndHsm.h"
#include  "CnasHsdSndInternalMsg.h"
#include  "CnasHsdSndRrm.h"
#include  "NasOmInterface.h"
#include  "CnasHsdFsmBgSrchTbl.h"
#include  "CnasHsdFsmBgSrch.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                        PS_FILE_ID_CNAS_HSD_FSM_BG_SRCH_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ��������
*****************************************************************************/
/*lint -save -e958*/

VOS_UINT32 CNAS_HSD_RcvBgSearchReq_BgSrch_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_HSD_BG_SEARCH_REQ_STRU        *pstBgSearchReq = VOS_NULL_PTR;
    CAS_CNAS_HRPD_BSR_LTE_CNF_STRU      stBsrLteCnf = {0};

    /* ���������Ϣ */
    CNAS_HSD_SaveCurEntryMsg(ulEventType, pstMsg);

    NAS_MEM_SET_S(&stBsrLteCnf, sizeof(stBsrLteCnf), 0, sizeof(stBsrLteCnf));

    pstBgSearchReq = (MSCC_HSD_BG_SEARCH_REQ_STRU *)pstMsg;

    /* ֧������̬�����ѣ�ģʽΪenRatModeΪLTEʱ��HSD�Ÿ�CAS͸��bsr lte req��Ϣ */
    if (VOS_RATMODE_LTE == pstBgSearchReq->enRatMode)
    {
        CNAS_HSD_SndCasBsrLteReq(pstBgSearchReq);

        /* Ǩ�Ƶ� CNAS_HSD_BG_SRCH_STA_WAIT_BG_SRCH_CNF״̬, ������������ʱ��TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF */
        CNAS_HSD_SetCurrFsmState(CNAS_HSD_BG_SRCH_STA_WAIT_BG_SRCH_CNF);

        CNAS_HSD_StartTimer(TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF, TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF_LEN);
    }
    else
    {
        /* ֱ�ӻظ�FAIL */
        stBsrLteCnf.ucRslt  = CNAS_CAS_HRPD_BSR_RSLT_FAIL;
        CNAS_HSD_SndMsccBgSearchCnf(&stBsrLteCnf);
    }

    return VOS_TRUE;

}


VOS_UINT32 CNAS_HSD_RcvMsccStopBgSearchReq_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF);

    CNAS_HSD_SndStopBgSrchReqProc();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvCasBgSrchCnf_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_HRPD_BSR_LTE_CNF_STRU     *pstBsrLteCnf;

    CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF);

    pstBsrLteCnf = (CAS_CNAS_HRPD_BSR_LTE_CNF_STRU *)pstMsg;

    CNAS_HSD_SndMsccBgSearchCnf(pstBsrLteCnf);

    CNAS_HSD_SndBgSrcgRslt(CNAS_HSD_SYS_ACQ_SCENE_BUTT, CNAS_HSD_NOT_NEED_REACQ);

    /* �˳����״̬�� */
    CNAS_HSD_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvTiWaitCasBgSrchExpired_BrSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_HSD_SetStopBgSrchInd_BgSrch(VOS_TRUE);

    CNAS_HSD_SndStopBgSrchReqProc();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvCasOhmInd_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_HRPD_OVERHEAD_MSG_IND_STRU                    *pstOhmMsg;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32                         enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32                      enAcqScene;

    enAcqScene = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pstOhmMsg = (CAS_CNAS_HRPD_OVERHEAD_MSG_IND_STRU*)pstMsg;

    enAcqFlg = CNAS_HSD_ProcCasOhmInd_BgSrch(pstOhmMsg, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    CNAS_HSD_IsNeedReAcqHrpdProc();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_Rcv1xSysChgInd_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_HSD_1X_SYS_CHANGE_IND_STRU    *pst1xSysIndMsg;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32     enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32  enAcqScene;

    enAcqScene      = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pst1xSysIndMsg  = (MSCC_HSD_1X_SYS_CHANGE_IND_STRU *)pstMsg;

    enAcqFlg        = CNAS_HSD_Proc1xSysChgInd_BgSrch(pst1xSysIndMsg, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    CNAS_HSD_IsNeedReAcqHrpdProc();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvCasNetworkLostIndProc_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ͣBG SRCH��ʱ�� */
    CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF);

    CNAS_HSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_HSD_SndMsccStopBgSrchInd();

    CNAS_HSD_SndBgSrcgRslt(CNAS_HSD_SYS_ACQ_SCENE_BUTT, CNAS_HSD_NOT_NEED_REACQ);

    /* �˳����״̬�� */
    CNAS_HSD_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvMsccDataCallRedialSysAcqReq_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �Ѿ�פ�����ֱ�ӻظ��ɹ� */
    CNAS_HSD_SndMsccDataCallRedialSysAcqCnf(NAS_MSCC_PIF_ACQUIRED_RESULT_SUCC);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvCasSuspendInd_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ͣBG SRCH��ʱ�� */
    CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF);

    CNAS_HSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_HSD_SndMsccStopBgSrchInd();

    CNAS_HSD_SndBgSrcgRslt(CNAS_HSD_SYS_ACQ_SCENE_BUTT, CNAS_HSD_NOT_NEED_REACQ);

    /* �˳����״̬�� */
    CNAS_HSD_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvHsmSessionNegResultInd_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSM_HSD_SESSION_NEG_RESULT_IND_STRU                    *pstSessionNegRlstInd;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32                         enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32                      enAcqScene;

    enAcqScene = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pstSessionNegRlstInd = (HSM_HSD_SESSION_NEG_RESULT_IND_STRU*)pstMsg;

    enAcqFlg = CNAS_HSD_ProcHsmSessionNegResultInd_BgSrch(pstSessionNegRlstInd, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    CNAS_HSD_IsNeedReAcqHrpdProc();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvHsmConnOpenInd_BgSrch_WaitBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSM_HSD_CONN_OPEN_IND_STRU         *pstOpenInd;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32     enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32  enAcqScene;

    enAcqScene = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pstOpenInd = (HSM_HSD_CONN_OPEN_IND_STRU *)pstMsg;

    enAcqFlg = CNAS_HSD_ProcHsmConnOpenInd_BgSrch(pstOpenInd, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    /* �Ƿ���Ҫ������������ */
    CNAS_HSD_IsNeedReAcqHrpdProc();

    return VOS_TRUE;
}


VOS_VOID CNAS_HSD_IsNeedReAcqHrpdProc(VOS_VOID)
{
    /* �Ƿ���Ҫ�������� */
    if (CNAS_HSD_NEED_REACQ == CNAS_HSD_GetSrchFlag_BgSrch())
    {
        /* ͣBG SRCH��ʱ�� */
        CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_BG_SRCH_CNF);

        CNAS_HSD_SetStopBgSrchInd_BgSrch(VOS_TRUE);

        CNAS_HSD_SndStopBgSrchReqProc();
    }

    return;
}


VOS_VOID CNAS_HSD_SndStopBgSrchReqProc(VOS_VOID)
{
    CNAS_HSD_SndCasStopBsrLteReq();

    /* Ǩ�Ƶ� CNAS_HSD_BG_SRCH_STA_WAIT_STOP_BG_SRCH_CNF״̬, ������������ʱ��TI_CNAS_HSD_WAIT_CAS_STOP_BG_SRCH_CNF */
    CNAS_HSD_SetCurrFsmState(CNAS_HSD_BG_SRCH_STA_WAIT_STOP_BG_SRCH_CNF);

    CNAS_HSD_StartTimer(TI_CNAS_HSD_WAIT_CAS_STOP_BG_SRCH_CNF, TI_CNAS_HSD_WAIT_CAS_STOP_BG_SRCH_CNF_LEN);

    return;
}


VOS_UINT32 CNAS_HSD_RcvCasStopBgSrchCnf_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_HSD_REACQ_FALG_ENUM_UINT32     enSrchFlag;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32  enCurScene;

    /* ͣSTOP BG SRCH��ʱ�� */
    CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_STOP_BG_SRCH_CNF);

    enCurScene = CNAS_HSD_GetSrchScene_BgSrch();

    enSrchFlag = CNAS_HSD_GetSrchFlag_BgSrch();

    /* �����HSD�����ֹͣ�����ѣ�֪ͨMSCC stop ind������͸���ϱ�CNF */
    if (VOS_TRUE == CNAS_HSD_GetStopBgSrchInd_BgSrch())
    {
        CNAS_HSD_SndMsccStopBgSrchInd();
    }
    else
    {
        /* �յ�CAS�ظ�����Ϣ��ֱ�Ӹ�MSCC�ظ� */
        CNAS_HSD_SndMsccStopBgSearchCnf();
    }

    CNAS_HSD_SndBgSrcgRslt(enCurScene, enSrchFlag);

    /* �˳����״̬�� */
    CNAS_HSD_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvTiWaitCasStopBgSrchExpired_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_HSD_REACQ_FALG_ENUM_UINT32     enSrchFlag;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32  enCurScene;

    enCurScene = CNAS_HSD_GetSrchScene_BgSrch();

    enSrchFlag = CNAS_HSD_GetSrchFlag_BgSrch();

    if (VOS_TRUE == CNAS_HSD_GetStopBgSrchInd_BgSrch())
    {
        CNAS_HSD_SndMsccStopBgSrchInd();
    }
    else
    {
        /* �յ�CAS�ظ�����Ϣ��ֱ�Ӹ�MSCC�ظ� */
        CNAS_HSD_SndMsccStopBgSearchCnf();
    }

    CNAS_HSD_SndBgSrcgRslt(enCurScene, enSrchFlag);

    /* �˳����״̬�� */
    CNAS_HSD_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvCasOhmInd_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_HRPD_OVERHEAD_MSG_IND_STRU    *pstOhmMsg;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32         enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32      enAcqScene;

    enAcqScene = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pstOhmMsg = (CAS_CNAS_HRPD_OVERHEAD_MSG_IND_STRU*)pstMsg;

    enAcqFlg = CNAS_HSD_ProcCasOhmInd_BgSrch(pstOhmMsg, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_Rcv1xSysChgInd_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_HSD_1X_SYS_CHANGE_IND_STRU    *pst1xSysIndMsg;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32     enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32  enAcqScene;

    enAcqScene      = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pst1xSysIndMsg = (MSCC_HSD_1X_SYS_CHANGE_IND_STRU *)pstMsg;

    enAcqFlg = CNAS_HSD_Proc1xSysChgInd_BgSrch(pst1xSysIndMsg, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvCasNetworkLostIndProc_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ͣBG SRCH��ʱ�� */
    CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_STOP_BG_SRCH_CNF);

    CNAS_HSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_HSD_SndMsccStopBgSrchInd();

    CNAS_HSD_SndBgSrcgRslt(CNAS_HSD_SYS_ACQ_SCENE_BUTT, CNAS_HSD_NOT_NEED_REACQ);

    /* �˳����״̬�� */
    CNAS_HSD_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvMsccDataCallRedialSysAcqReq_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �Ѿ�פ�����ֱ�ӻظ��ɹ� */
    CNAS_HSD_SndMsccDataCallRedialSysAcqCnf(NAS_MSCC_PIF_ACQUIRED_RESULT_SUCC);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvCasSuspendInd_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ͣSTOP BG SRCH��ʱ�� */
    CNAS_HSD_StopTimer(TI_CNAS_HSD_WAIT_CAS_STOP_BG_SRCH_CNF);

    CNAS_HSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_HSD_SndMsccStopBgSrchInd();

    CNAS_HSD_SndBgSrcgRslt(CNAS_HSD_SYS_ACQ_SCENE_BUTT, CNAS_HSD_NOT_NEED_REACQ);

    /* �˳����״̬�� */
    CNAS_HSD_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvHsmSessionNegResultInd_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSM_HSD_SESSION_NEG_RESULT_IND_STRU                    *pstSessionNegRlstInd;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32                         enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32                      enAcqScene;

    enAcqScene      = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pstSessionNegRlstInd = (HSM_HSD_SESSION_NEG_RESULT_IND_STRU*)pstMsg;

    enAcqFlg = CNAS_HSD_ProcHsmSessionNegResultInd_BgSrch(pstSessionNegRlstInd, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_HSD_RcvHsmConnOpenInd_BgSrch_WaitStopBgSrchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    HSM_HSD_CONN_OPEN_IND_STRU         *pstOpenInd;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32     enAcqFlg;
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32  enAcqScene;

    enAcqScene = CNAS_HSD_SYS_ACQ_SCENE_BUTT;

    pstOpenInd              = (HSM_HSD_CONN_OPEN_IND_STRU *)pstMsg;

    enAcqFlg = CNAS_HSD_ProcHsmConnOpenInd_BgSrch(pstOpenInd, &enAcqScene);

    CNAS_HSD_SetSrchFlag_BgSrch(enAcqScene, enAcqFlg);

    return VOS_TRUE;
}


CNAS_HSD_REACQ_FALG_ENUM_UINT32 CNAS_HSD_ProcCasOhmInd_BgSrch(
    CAS_CNAS_HRPD_OVERHEAD_MSG_IND_STRU                    *pstOhmMsg,
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32                     *penAcqScene
)
{
    CNAS_PRL_HRPD_SYSTEM_STRU               stHrpdSys;
    CNAS_HSD_ASSN_RLST_ENUM_UINT32          enAssnRlst;
    VOS_UINT32                              ulMccFlg;

    stHrpdSys.stFreq.enBandClass = pstOhmMsg->usBandClass;
    stHrpdSys.stFreq.usChannel   = pstOhmMsg->usFreq;
    stHrpdSys.ucSubnetMask       = pstOhmMsg->ucSubNetMask;

    NAS_MEM_CPY_S(stHrpdSys.aucSubnet, CNAS_PRL_BYTES_IN_SUBNET, pstOhmMsg->aucSectorId, CNAS_PRL_BYTES_IN_SUBNET);

    /* avoidģ�鴦�� */
    CNAS_HSD_ProcAvoidEventInSubnetIdChange(&stHrpdSys);

    /* ����history list */
    CNAS_HSD_UpdateHistorySysList(&stHrpdSys);
    CNAS_HSD_UpdateSpmSysList(&pstOhmMsg->stSpmChannel);

    /* ����Hrpdפ����Ϣ */
    CNAS_HSD_UpdateHrpdSysInfo(&stHrpdSys);

    CNAS_HSD_SndMsccOverheadMsgInd(pstOhmMsg,&stHrpdSys);

    if (VOS_TRUE == CNAS_HSD_GetFreqLockMode())
    {
        /* Idle�л��ɹ��Ĵ���Ҫ�����汣��һ�� */
        /* HSD�յ�CAS��overhead��Ϣʱ��Ҫ��HSM�Լ�MSCCת��overhead��Ϣ */
        CNAS_HSD_SndHsmOverheadMsgInd(pstOhmMsg);

        /* ��פ�������籣�浽MRU */
        CNAS_HSD_UpdateMru0(&stHrpdSys);

        return CNAS_HSD_NOT_NEED_REACQ;
    }

    enAssnRlst = CNAS_HSD_CheckHrpdFreqAssnWith1X(&stHrpdSys);

    ulMccFlg = CNAS_HSD_IsMccInOperLockWhiteSidList(pstOhmMsg->usCountryCode);

    /* ��ǰϵͳ������� */
    if ((CNAS_HSD_NO_ASSN_WITH_HRPD == enAssnRlst)
     || (VOS_FALSE == ulMccFlg))
    {
        /* IDLE̬�²Ž����������� */
        if (CNAS_HSD_AT_STATUS_ENUM_IDLE == CNAS_HSD_GetATStatus())
        {
            *penAcqScene = CNAS_HSD_SYS_ACQ_SCENE_OOC_SWITCH_ON;

            return CNAS_HSD_NEED_REACQ;
        }
    }
    else if (CNAS_HSD_ASSN_SUCC == enAssnRlst)
    {
        /* Idle�л��ɹ��Ĵ���Ҫ�����汣��һ�� */
        /* HSD�յ�CAS��overhead��Ϣʱ��Ҫ��HSM�Լ�MSCCת��overhead��Ϣ */
        CNAS_HSD_SndHsmOverheadMsgInd(pstOhmMsg);

        /* ��פ�������籣�浽MRU */
        CNAS_HSD_UpdateMru0(&stHrpdSys);

        /* ��¼���ڴ�½�Ͱ��ŵ�Ƶ�� */
        CNAS_HSD_RecordHrpdCustomChannelsProc(pstOhmMsg);
    }
    else
    {
        /* �쳣���� */
        CNAS_WARNING_LOG(UEPS_PID_HSD, "CNAS_HSD_ProcCasOhmInd_BgSrch: enAssnRlst is incorrect.");;
    }

    return CNAS_HSD_NOT_NEED_REACQ;
}


CNAS_HSD_REACQ_FALG_ENUM_UINT32 CNAS_HSD_Proc1xSysChgInd_BgSrch(
    MSCC_HSD_1X_SYS_CHANGE_IND_STRU    *pst1xSysIndMsg,
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32 *penAcqScene
)
{
    CNAS_HSD_ASSN_RLST_ENUM_UINT32      enAssnRlst;
    CNAS_PRL_HRPD_SYSTEM_STRU          *pstHrpdSys;

    /* 1X�������Ϊʧ�����ý����ж� */
    if (VOS_TRUE != pst1xSysIndMsg->ucIs1xAvail)
    {
        return CNAS_HSD_NOT_NEED_REACQ;
    }

    /* ��Ҫ������̬�½���������ж� */
    if (CNAS_HSD_AT_STATUS_ENUM_IDLE != CNAS_HSD_GetATStatus())
    {
        CNAS_WARNING_LOG(UEPS_PID_HSD, "CNAS_HSD_GetATStatus: Current state isn't idle.");

        return CNAS_HSD_NOT_NEED_REACQ;
    }

    if (CNAS_HSD_HRPD_SYSTEM_AVAILABLE != CNAS_HSD_GetHrpdSysStatus())
    {
        CNAS_WARNING_LOG(UEPS_PID_HSD, "CNAS_HSD_Proc1xSysChgInd_BgSrch: Current state is incorrect.");

        /* �쳣����������CNAS_HSD_ASSN_SUCC������������������������� */
        return CNAS_HSD_NOT_NEED_REACQ;
    }

    /* ����Ϊ��Ƶ�¾Ͳ����ж� */
    if (VOS_TRUE == CNAS_HSD_GetFreqLockMode())
    {
        return CNAS_HSD_NOT_NEED_REACQ;
    }

    pstHrpdSys = CNAS_HSD_GetCampedHrpdSystemInfo();

    /*  ��ȡ��ǰ1X��HRPD�Ĺ�����ϵ */
    enAssnRlst = CNAS_HSD_CheckHrpdFreqAssnWith1X(pstHrpdSys);

    /* ��ǰû���������Ƶ�� */
    if (CNAS_HSD_NO_ASSN_FREQ == enAssnRlst)
    {
        CNAS_HSD_ClearHrpdSysInfo();

        *penAcqScene = CNAS_HSD_SYS_ACQ_SCENE_NOT_AVAILABLE_NO_SRCH;

        return CNAS_HSD_NEED_REACQ;
    }

    /* �й���Ƶ�㣬���͵�ǰ��HRPDϵͳ������� */
    if (CNAS_HSD_NO_ASSN_WITH_HRPD == enAssnRlst)
    {
        *penAcqScene = CNAS_HSD_SYS_ACQ_SCENE_OOC_SWITCH_ON;

        return CNAS_HSD_NEED_REACQ;
    }

    return CNAS_HSD_NOT_NEED_REACQ;
}


CNAS_HSD_REACQ_FALG_ENUM_UINT32 CNAS_HSD_ProcHsmSessionNegResultInd_BgSrch(
    HSM_HSD_SESSION_NEG_RESULT_IND_STRU                    *pstSessionNegRlstInd,
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32                     *penAcqScene
)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          stFreq;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32                         enReAcqFlag;
    HSD_MSCC_SESSION_RELEASE_TYPE_ENUM_UINT32               enCurrSessionRelType;

    stFreq.enBandClass   = CNAS_HSD_GetHistorySysList()->stCampedHrpdSysInfo[0].stFreq.enBandClass;
    stFreq.usChannel     = CNAS_HSD_GetHistorySysList()->stCampedHrpdSysInfo[0].stFreq.usChannel;
    enReAcqFlag          = CNAS_HSD_NOT_NEED_REACQ;

    /* ����session��� */
    CNAS_HSD_UpdateSessionRlt(pstSessionNegRlstInd->enNegResult);

    /* avoidģ�鴦�� */
    CNAS_HSD_ProcAvoidEventInSessionNegRlstInd(pstSessionNegRlstInd, &stFreq, &enReAcqFlag);

    /* �Ƿ���Ҫ�������� */
    if (CNAS_HSD_NEED_REACQ == enReAcqFlag)
    {
        *penAcqScene = CNAS_HSD_SYS_ACQ_SCENE_OOC_SWITCH_ON;

        return CNAS_HSD_NEED_REACQ;
    }
    /* �ɹ���MSCC�ϱ�session��� */
    else if ((HSM_HSD_SESSION_NEG_RSLT_SESSION_CLOSE != pstSessionNegRlstInd->enNegResult)
          && (HSM_HSD_SESSION_NEG_RSLT_NO_RF != pstSessionNegRlstInd->enNegResult))
    {
        enCurrSessionRelType = CNAS_HSD_CovertSessionType_Proc(pstSessionNegRlstInd->enSessionRelType);

        CNAS_HSD_SndMsccSessionNegRsltInd(pstSessionNegRlstInd->enNegResult,
                                          pstSessionNegRlstInd->ucIsNewSession,
                                          enCurrSessionRelType,
                                          pstSessionNegRlstInd->ucIsEhrpdSupport);
    }
    else
    {}

    return CNAS_HSD_NOT_NEED_REACQ;
}


CNAS_HSD_REACQ_FALG_ENUM_UINT32 CNAS_HSD_ProcHsmConnOpenInd_BgSrch(
    HSM_HSD_CONN_OPEN_IND_STRU         *pstOpenInd,
    CNAS_HSD_SYS_ACQ_SCENE_ENUM_UINT32 *penAcqScene
)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU      stFreq;
    CNAS_HSD_REACQ_FALG_ENUM_UINT32     enReAcqFlag;

    stFreq.enBandClass      = CNAS_HSD_GetHistorySysList()->stCampedHrpdSysInfo[0].stFreq.enBandClass;
    stFreq.usChannel        = CNAS_HSD_GetHistorySysList()->stCampedHrpdSysInfo[0].stFreq.usChannel;
    enReAcqFlag             = CNAS_HSD_NOT_NEED_REACQ;

    /* avoidģ�鴦�� */
    CNAS_HSD_ProcAvoidEventInAccessRlstInd(pstOpenInd, &stFreq, &enReAcqFlag);

    if (CNAS_HSD_NEED_REACQ == enReAcqFlag)
    {
        *penAcqScene = CNAS_HSD_SYS_ACQ_SCENE_OOC_SWITCH_ON;

        return CNAS_HSD_NEED_REACQ;
    }

    return CNAS_HSD_NOT_NEED_REACQ;
}

/*lint -restore*/

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */



