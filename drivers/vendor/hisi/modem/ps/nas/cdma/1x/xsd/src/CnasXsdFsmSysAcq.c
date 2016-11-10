

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "CnasCcb.h"
#include  "CnasPrlApi.h"
#include  "CnasXsdComFunc.h"
#include  "CnasXsdSndInternalMsg.h"
#include  "CnasXsdSndMscc.h"
#include  "CnasXsdSndCas.h"
#include  "CnasXsdFsmSysAcq.h"
#include  "CnasXsdFsmSysAcqTbl.h"
#include  "CnasXsdSysAcqStrategy.h"
#include  "CnasMntn.h"
#include  "CnasXsdMntn.h"
#include  "CnasTimerMgmt.h"
#include  "CnasXsdSndXreg.h"

#include "CnasXsdSndXcc.h"

#include "NasMntn.h"
#include "OmApi.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_CNAS_XSD_FSM_SYS_ACQ_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
EXTERN VOS_UINT32                       g_CtrlCallRelScanList;

/*****************************************************************************
  3 ��������
*****************************************************************************/
/*lint -save -e958*/

VOS_UINT32 CNAS_XSD_RcvMsccSysAcqReq_SysAcq_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanList = VOS_NULL_PTR;
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usCurScanListIndex;
    VOS_UINT16                          usDstChanNum;
    MSCC_XSD_SYSTEM_ACQUIRE_REQ_STRU   *pstSysAcqReq = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enScanScene;
    VOS_UINT32                          ulWaitCasSyncCnfTimerLen;

    enScanScene  = CNAS_XSD_SYS_ACQ_SCENE_BUTT;
    usDstChanNum = 0;
    NAS_MEM_SET_S(&astDstChan[0],
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                  0x00,
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

#if (FEATURE_PTM == FEATURE_ON)
    CNAS_XSD_RecordChr1xOosSrchInfo();
#endif

    /* ���������Ϣ */
    CNAS_XSD_SaveCurEntryMsg(ulEventType, pstMsg);

    pstScanList = CNAS_XSD_GetScanChanListAddr();

    /* ����scan list */
    pstSysAcqReq = (MSCC_XSD_SYSTEM_ACQUIRE_REQ_STRU*)pstMsg;

    /* enSysAcqType����NORMAL����ʾ������פ�����󣬷������޿�������Xģ���ѣ�ֻΪ�˻�ȡ��ǰ�ֻ���λ����Ϣ
       ֻͬ�����ų���Ƶ�㣬������Ƶ��ͬ����ͬ����������ȴ�ϵͳ��Ϣ
    */
    if (MSCC_XSD_SYS_ACQ_TYPE_NORMAL == pstSysAcqReq->enSysAcqType)
    {
        CNAS_XSD_BuildScanChanList(CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON, 0, VOS_NULL_PTR, pstScanList);

        enScanScene = CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON;
    }
    else
    {
        CNAS_XSD_BuildScanChanList(CNAS_XSD_SYS_ACQ_SCENE_POWER_ON_INIT_SEARCH, 0, VOS_NULL_PTR, pstScanList);

        enScanScene = CNAS_XSD_SYS_ACQ_SCENE_POWER_ON_INIT_SEARCH;
    }

    /* �����scan listΪ�� */
    if (0 == pstScanList->usTotalNum)
    {
        /* ���CCB�д洢��1Xϵͳ��Ϣ */
        CNAS_CCB_Init1xSysInfo();

        if (VOS_TRUE == CNAS_XSD_IsAcquireAvoidSysNeeded(enScanScene))
        {
            CNAS_XSD_SetAvoidSysAcqListFlg_SysAcq(VOS_TRUE);

            CNAS_XSD_SndMsccSysAcqStartInd();

            CNAS_XSD_SetSystemCampOnFlag(VOS_FALSE);

            /* ����ͬ������ͬ��avoidϵͳ */
            NAS_MEM_SET_S(&astDstChan[0],
                          sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                          0x00,
                          sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

            CNAS_XSD_GetNextAvoidSysAcqList_SysAcq(&usDstChanNum, &astDstChan[0]);

            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);

            return VOS_TRUE;
        }

        /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
        CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_NO_FREQ, CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON,VOS_FALSE, CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

        /* �˳����״̬�� */
        CNAS_XSD_QuitFsmSysAcq_SysAcq();

        return VOS_TRUE;
    }

    /* ��״̬���������г�ʼ��available list */

    usCurScanListIndex = CNAS_XSD_GetCurChannelScanIndex();

    usDstChanNum       = 0;

    /* ��ȡ�������ɨ���Ƶ���б� */
    CNAS_XSD_GetNextScanChanList_SysAcq(usCurScanListIndex, &usDstChanNum, astDstChan, pstScanList);

    /* ��CAS����ID_CNAS_CAS_1X_SYSTEM_SYNC_REQ */
    CNAS_XSD_SndCasSysSyncReq(usDstChanNum, astDstChan);

    /* ��MSCC����ID_XSD_MSCC_SYSTEM_ACQUIRE_START_IND */
    CNAS_XSD_SndMsccSysAcqStartInd();

    /* ���CCB�д洢��1Xϵͳ��Ϣ */
    CNAS_CCB_Init1xSysInfo();

    if (CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
    {
        /* SRLTEģʽ�£�1x������������Դ���𱣻���ʱ�� */
        if (CNAS_TIMER_STATUS_RUNNING == CNAS_GetSpecifiedTimerStatus(UEPS_PID_XSD, TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER, 0))
        {
            CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER);
        }
        CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER, CNAS_XSD_GetSysAcqNoRfProtectTimerLen());
    }

    ulWaitCasSyncCnfTimerLen = CNAS_XSD_GetWaitCasSystemSyncCnfTimerLen(usDstChanNum);

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_SYNC_CNF״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_SYNC_CNF);

    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF, ulWaitCasSyncCnfTimerLen);

    CNAS_XSD_SetSystemCampOnFlag(VOS_FALSE);
    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvXsdSysAcqReq_SysAcq_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_XSD_SYSTEM_ACQUIRED_REQ_STRU                  *pstIntSysAcqReq         = VOS_NULL_PTR;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList             = VOS_NULL_PTR;
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                                              usCurScanListIndex;
    VOS_UINT16                                              usDstChanNum;
    VOS_UINT32                                              ulWaitCasSyncCnfTimerLen;

    usDstChanNum = 0;
    NAS_MEM_SET_S(&astDstChan[0],
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                  0x00,
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);
#if (FEATURE_PTM == FEATURE_ON)
    CNAS_XSD_RecordChr1xOosSrchInfo();
#endif

    /* ���������Ϣ */
    CNAS_XSD_SaveCurEntryMsg(ulEventType, pstMsg);

    pstIntSysAcqReq   = (CNAS_XSD_XSD_SYSTEM_ACQUIRED_REQ_STRU *)pstMsg;
    pstScanList       = CNAS_XSD_GetScanChanListAddr();

    CNAS_XSD_BuildScanChanList(pstIntSysAcqReq->enSysAcqScene, (VOS_UINT16)(pstIntSysAcqReq->ulFreqNum), &pstIntSysAcqReq->astFreq[0], pstScanList);

    /* �����scan listΪ�� */
    if (0 == pstScanList->usTotalNum)
    {
        /* ���CCB�д洢��1Xϵͳ��Ϣ */
        CNAS_CCB_Init1xSysInfo();

        if (VOS_TRUE == CNAS_XSD_IsAcquireAvoidSysNeeded(pstIntSysAcqReq->enSysAcqScene))
        {
            CNAS_XSD_SetAvoidSysAcqListFlg_SysAcq(VOS_TRUE);

            CNAS_XSD_SndMsccSysAcqStartInd();

            CNAS_XSD_SetSystemCampOnFlag(VOS_FALSE);

            /* ����ͬ������ͬ��avoidϵͳ */
            NAS_MEM_SET_S(&astDstChan[0],
                          sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                          0x00,
                          sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

            CNAS_XSD_GetNextAvoidSysAcqList_SysAcq(&usDstChanNum, &astDstChan[0]);

            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);

            return VOS_TRUE;
        }

        /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
        CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_NO_FREQ, pstIntSysAcqReq->enSysAcqScene,CNAS_XSD_IsImmediateSysAcq_SysAcq(), CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

        /* �˳����״̬�� */
        CNAS_XSD_QuitFsmSysAcq_SysAcq();

        return VOS_TRUE;
    }

    /* ��״̬���������г�ʼ��available list */

    usCurScanListIndex = CNAS_XSD_GetCurChannelScanIndex();
    usDstChanNum       = 0;

    /* ��ȡ�������ɨ���Ƶ���б� */
    CNAS_XSD_GetNextScanChanList_SysAcq(usCurScanListIndex, &usDstChanNum, astDstChan, pstScanList);


    if (0 == usDstChanNum)
    {
        /* ���CCB�д洢��1Xϵͳ��Ϣ */
        CNAS_CCB_Init1xSysInfo();

        if (VOS_TRUE == CNAS_XSD_IsAcquireAvoidSysNeeded(pstIntSysAcqReq->enSysAcqScene))
        {
            CNAS_XSD_SetAvoidSysAcqListFlg_SysAcq(VOS_TRUE);

            CNAS_XSD_SndMsccSysAcqStartInd();

            CNAS_XSD_SetSystemCampOnFlag(VOS_FALSE);

            /* ����ͬ������ͬ��avoidϵͳ */
            NAS_MEM_SET_S(&astDstChan[0],
                          sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                          0x00,
                          sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

            CNAS_XSD_GetNextAvoidSysAcqList_SysAcq(&usDstChanNum, &astDstChan[0]);

            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);

            return VOS_TRUE;
        }

        /* If Scan list is empty, quit the FSM with Fail Result */
        CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_NO_FREQ, pstIntSysAcqReq->enSysAcqScene,CNAS_XSD_IsImmediateSysAcq_SysAcq(), CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

        CNAS_XSD_QuitFsmSysAcq_SysAcq();
        return VOS_TRUE;
    }

    /* ��CAS����ID_CNAS_CAS_1X_SYSTEM_SYNC_REQ */
    CNAS_XSD_SndCasSysSyncReq(usDstChanNum, astDstChan);

    /* ��MSCC����ID_XSD_MSCC_SYSTEM_ACQUIRE_START_IND */
    CNAS_XSD_SndMsccSysAcqStartInd();

    /* ���CCB�д洢��1Xϵͳ��Ϣ */
    CNAS_CCB_Init1xSysInfo();

    if (CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
    {
        /* SRLTEģʽ�£�1x������������Դ���𱣻���ʱ�� */
        if (CNAS_TIMER_STATUS_RUNNING == CNAS_GetSpecifiedTimerStatus(UEPS_PID_XSD, TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER, 0))
        {
            CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER);
        }
        CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER, CNAS_XSD_GetSysAcqNoRfProtectTimerLen());
    }

    CNAS_NORMAL_LOG1(UEPS_PID_XSD, "CNAS_XSD_RcvXsdSysAcqReq_SysAcq_Init: Curr Version Type is", CNAS_CCB_IsVersionSrlte());

    ulWaitCasSyncCnfTimerLen = CNAS_XSD_GetWaitCasSystemSyncCnfTimerLen(usDstChanNum);

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_SYNC_CNF״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_SYNC_CNF);

    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF, ulWaitCasSyncCnfTimerLen);

    CNAS_XSD_SetSystemCampOnFlag(VOS_FALSE);
    return VOS_TRUE;
}




VOS_UINT32 CNAS_XSD_RcvCasSysSyncCnf_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf = VOS_NULL_PTR;

    pstSyncCnf = (CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU *)pstMsg;

    /* ֹͣ������ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);



    /* ����ͬ��ȷ�Ͻ�� */
    switch (pstSyncCnf->enSyncRslt)
    {
        case CNAS_CAS_1X_RSLT_FAILURE:
        default:
            CNAS_XSD_ProcCasSyncCnfFail_SysAcq(pstSyncCnf);
            break;

        case CNAS_CAS_1X_RSLT_SUCCESS:
            CNAS_XSD_ProcCasSyncCnfSucc_SysAcq(pstSyncCnf);
            break;

        case CNAS_CAS_1X_RSLT_NO_RF:
            CNAS_XSD_ProcCasSyncCnfNoRf_SysAcq(pstSyncCnf);

            break;
    }

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiWaitCasSysSyncCnfExpired_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvTiWaitCasSysSyncCnfExpired_SysAcq_WaitCasSysSyncCnf: timer out");

    /* ��CAS����ID_CNAS_CAS_1X_STOP_SYSTEM_SYNC_REQ��Ϣֹͣͬ�� */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccPowerOffReq_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    /* ����ػ����� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_StartTimer(TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER, TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER_LEN);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvTiPowerOffCampOnProtectTimerExpired_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_OFF);

    /* ��CAS����ID_CNAS_CAS_1X_STOP_SYSTEM_SYNC_REQ��Ϣֹͣͬ�� */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvTiAvailableTimerExpired_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SetOocWaitSearchFlag(VOS_TRUE);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvTiOosSysAcqCurPhaseTotalTimerExpired_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���ó�ʱ��� */
    CNAS_XSD_SetOocTotalTimerExpiredFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}




VOS_UINT32 CNAS_XSD_RcvMsccPowerSaveReq_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��������ȼ���Ϻ�֮ǰ��ϵĻ�����Ϣ */
    if (VOS_TRUE == CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ))
    {
        CNAS_XSD_SndMsccSrvAcqCnf(NAS_MSCC_PIF_SRV_ACQ_RESULT_FAIL);

        CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);
    }

    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    /* Stop Sync Cnf protect timer */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

    /* Save the Power Save Request in cache */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_SAVE);

    /* Set Abort flag to true */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* Send Stop Sync Req to CAS */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* Set next State to Wait CAS Stop Sync Cnf and start the protect timer */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvCasOhmInd_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_PRL_1X_SYSTEM_STRU             stCurSysInfo;
    VOS_UINT32                          ulIsNegSys;
    CAS_CNAS_1X_OHM_IND_STRU           *pstOhmInd = VOS_NULL_PTR;

    pstOhmInd = (CAS_CNAS_1X_OHM_IND_STRU *)pstMsg;

    /* Stop the Timer */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

    stCurSysInfo.stFreq.enBandClass = pstOhmInd->usBandClass;
    stCurSysInfo.stFreq.usChannel   = pstOhmInd->usFreq;
    stCurSysInfo.usSid              = pstOhmInd->usSid;
    stCurSysInfo.usNid              = pstOhmInd->usNid;

    /* �������availableϵͳ��ˢ��overheader�ϱ���Ƶ����Ϣ */
    if (VOS_FALSE == CNAS_XSD_GetAvailSysAcqListFlg_SysAcq())
    {
        /* ����overheader�ϱ���Ƶ�㵽CCB�� */
        CNAS_CCB_SetCdmaOhmFreq(&(stCurSysInfo.stFreq));
    }

    ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(&stCurSysInfo,
                                                 pstOhmInd->enServiceInfoIncl,
                                                 pstOhmInd->stServiceInfo.usMcc);

    if (VOS_TRUE == ulIsNegSys)
    {
        CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvCasOhmInd_SysAcq_WaitCasOhmInd: The CAS Ind System is Neg!");
    }

    CNAS_XSD_ProcCasOhmInd(pstOhmInd, ulIsNegSys);

    if(VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_ABORTED,
                               CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                               CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                               CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());
    }
    else
    {
        /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
        CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_SUCCESS, CNAS_XSD_GetCurSysAcqScene_SysAcq(), VOS_FALSE, CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());
    }

    OM_DelDrxTimerWakeSrc(CNAS_CCB_GetCdmaModeModemId(), VOS_RATMODE_1X);

    CNAS_XSD_LogDrxTimerStatus(VOS_FALSE);

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvCasSysDeterminInd_SysAcq_WaitCasOhmInd
(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_1X_SYSTEM_DETERMIN_IND_STRU                   *pstSysDeterminInd = VOS_NULL_PTR;
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstFreq           = VOS_NULL_PTR;

    pstSysDeterminInd = (CAS_CNAS_1X_SYSTEM_DETERMIN_IND_STRU *)pstMsg;

    /* �д�ϱ����Ҫ�˳� */
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        /* ֹͣ������ʱ�� */
        CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

        CNAS_XSD_PerformAbortProcedure_SysAcq();

        return VOS_TRUE;
    }

    if (VOS_TRUE == CNAS_XSD_GetNdssIndFlag())
    {
        CNAS_XSD_SetNdssIndFlag(VOS_FALSE);

        CNAS_XSD_SndXccNdssResultInd(XSD_XCC_NDSS_RESULT_FAILURE);
    }

    /* ��ȡ�ϴ�ͬ���ɹ���Ƶ�� */
    pstFreq = CNAS_XSD_GetLastSyncedFreq();

    /***********************************************************************************************
     * 1. ˢ��scan list���ϴ�ͬ���ɹ���Ƶ��Ϊ����������
     **********************************************************************************************/
    CNAS_XSD_UpdateChanStatusUponRcvCasDeterminInd_SysAcq(pstFreq);



    /***********************************************************************************************
     * 2. GEOϵͳ��¼�����б����ʱ��ˢ��GEOϵͳ��¼�����б��е�Ƶ��Ϊ����������
     **********************************************************************************************/
    if (VOS_NULL_PTR != CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo)
    {
        CNAS_XSD_UpdateGeoSysRecordStatusUponReceivedDeterminInd(pstFreq);
    }

    switch (pstSysDeterminInd->enSdReason)
    {
        case CAS_CNAS_1X_SD_SYS_LOST_IND:
        case CAS_CNAS_1X_SD_NEW_SYSTEM_IND:
        case CAS_CNAS_1X_SD_ABNORMAL_IND:
        case CAS_CNAS_1X_SD_LOCK_IND:
        case CAS_CNAS_1X_SD_UNLOCK_IND:
        case CAS_CNAS_1X_SD_RESCAN_IND:
        case CAS_CNAS_1X_SD_ACQUISITION_FAIL:
            CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

            CNAS_XSD_SetRedirectionFlag(VOS_FALSE);

            CNAS_XSD_ContinueSysSync_SysAcq();
            break;

        case CAS_CNAS_1X_SD_PROTO_MISMATCH_IND:
            CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

            CNAS_XSD_ProcCasSystemDetermineIndWithProtoMisReason_SysAcq(ulEventType, pstMsg);
            break;

        /* ���ΪNO RF������ʧ�ܵĴ��� */
        case CAS_CNAS_1X_SD_NO_RF:
            CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

            CNAS_XSD_SetRedirectionFlag(VOS_FALSE);

            /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
            CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_FAILURE, CNAS_XSD_SYS_ACQ_SCENE_NO_RF, VOS_FALSE, CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

            /* �˳����״̬�� */
            CNAS_XSD_QuitFsmSysAcq_SysAcq();
            break;

        default:

            /* other reasons we think cas will continue send overhead message to us,so we still
               stay on the current state. */
            CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvCasSysDeterminInd_SysAcq_WaitCasOhmInd: unknown reason");
            break;
    }


    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiWaitCasOhmIndExpired_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvTiWaitCasOhmIndExpired_SysAcq_WaitCasOhmInd: timer out");

    /* ��ˢ��SCAN LISTƵ��״̬ */

    /* �д�ϱ����Ҫ�˳� */
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        CNAS_XSD_PerformAbortProcedure_SysAcq();

        return VOS_TRUE;
    }

    CNAS_XSD_ContinueSysSync_SysAcq();

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvMsccPowerOffReq_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_PRL_1X_SYSTEM_ID_STRU         *pstLastSyncedSys       = VOS_NULL_PTR;
    CNAS_PRL_1X_SYSTEM_STRU             st1xSysInfo;

    NAS_MEM_SET_S(&st1xSysInfo, sizeof(st1xSysInfo), 0x00, sizeof(CNAS_PRL_1X_SYSTEM_STRU));
    pstLastSyncedSys                    = CNAS_XSD_GetLastSyncedSys();

    st1xSysInfo.usSid                   = pstLastSyncedSys->usSid;
    st1xSysInfo.usNid                   = pstLastSyncedSys->usNid;
    st1xSysInfo.stFreq                  = CNAS_XSD_GetHistorySyncedSysFreqList()->astFreq[0];

    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    /* ����ػ����� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* ����ǰϵͳ��Ч,������� */
    if (VOS_TRUE == CNAS_XSD_IsCurSysNotSuitableSys(&st1xSysInfo, VOS_FALSE, 0))
    {
        CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

        CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_ABORTED,
                               CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                               CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                               CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

        /* �˳����״̬�� */
        CNAS_XSD_QuitFsmSysAcq_SysAcq();

        return VOS_TRUE;
    }

    CNAS_XSD_StartTimer(TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER, TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER_LEN);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvTiPowerOffCampOnProtectTimerExpired_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_OFF);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

    CNAS_XSD_PerformAbortProcedure_SysAcq();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiOosSysAcqCurPhaseTotalTimerExpired_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���ó�ʱ��� */
    CNAS_XSD_SetOocTotalTimerExpiredFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiOosSysAcqCurPhaseTotalTimerExpired_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���ó�ʱ��� */
    CNAS_XSD_SetOocTotalTimerExpiredFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}





VOS_UINT32 CNAS_XSD_RcvTiAvailableTimerExpired_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SetOocWaitSearchFlag(VOS_TRUE);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvMsccPowerSaveReq_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_PRL_1X_SYSTEM_ID_STRU         *pstLastSyncedSys       = VOS_NULL_PTR;
    CNAS_PRL_1X_SYSTEM_STRU             st1xSysInfo;

    NAS_MEM_SET_S(&st1xSysInfo, sizeof(st1xSysInfo), 0x00, sizeof(CNAS_PRL_1X_SYSTEM_STRU));
    pstLastSyncedSys                    = CNAS_XSD_GetLastSyncedSys();

    /* ��������ȼ���Ϻ�֮ǰ��ϵĻ�����Ϣ */
    if (VOS_TRUE == CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ))
    {
        CNAS_XSD_SndMsccSrvAcqCnf(NAS_MSCC_PIF_SRV_ACQ_RESULT_FAIL);

        CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);
    }

    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    st1xSysInfo.usSid                   = pstLastSyncedSys->usSid;
    st1xSysInfo.usNid                   = pstLastSyncedSys->usNid;
    st1xSysInfo.stFreq                  = CNAS_XSD_GetHistorySyncedSysFreqList()->astFreq[0];

    /* �����������У���ϴ��� */
    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_SAVE);

    /* ����ǰϵͳ��Ч���ӳٴ�� */
    if (VOS_TRUE != CNAS_XSD_IsCurSysNotSuitableSys(&st1xSysInfo, VOS_FALSE, 0))
    {
        /* ����״̬����ϱ�� */
        CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

        CNAS_XSD_StartTimer(TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER, TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER_LEN);

        return VOS_TRUE;
    }

    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

    CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_ABORTED,
                           CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                           CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                           CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvCasStopSysSyncCnf_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* ֹͣ������ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF);

    /***********************************************************************************************
     * 1. ����������
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        CNAS_XSD_PerformAbortProcedure_SysAcq();

        return VOS_TRUE;
    }

    /***********************************************************************************************
     * 2. ��������ͬ��
     **********************************************************************************************/
    CNAS_XSD_ContinueSysSync_SysAcq();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiWaitCasStopSysSyncCnfExpired_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvTiWaitCasStopSysSyncCnfExpired_SysAcq_WaitCasStopSysSyncCnf: timer out");

    /***********************************************************************************************
     * 1. ����������
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        CNAS_XSD_PerformAbortProcedure_SysAcq();

        return VOS_TRUE;
    }

    /***********************************************************************************************
     * 2. ��������ͬ��
     **********************************************************************************************/
    CNAS_XSD_ContinueSysSync_SysAcq();


    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccPowerOffReq_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    /* ����ػ����� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_StartTimer(TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER, TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER_LEN);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvTiPowerOffCampOnProtectTimerExpired_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_OFF);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiAvailableTimerExpired_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SetOocWaitSearchFlag(VOS_TRUE);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvMsccPowerSaveReq_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��������ȼ���Ϻ�֮ǰ��ϵĻ�����Ϣ */
    if (VOS_TRUE == CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ))
    {
        CNAS_XSD_SndMsccSrvAcqCnf(NAS_MSCC_PIF_SRV_ACQ_RESULT_FAIL);

        CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);
    }

    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    /* Save the Power Save Request in cache */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_SAVE);

    /* Set Abort flag to true */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_VOID CNAS_XSD_PerformBestPreferedSystemSelectionAfterSyncSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;

    /***********************************************************************************************
     * 1.����GEOϵͳ��¼�����б����ڵĳ���
     **********************************************************************************************/
    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    if (VOS_NULL_PTR == pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        /* GEOϵͳ��¼�����б����ڵĴ��� */
        CNAS_XSD_PreformBestPreferedSystemSelectionAndNoGeoSysRecordSearchList_SysAcq(pstSyncCnf);

        return;
    }

    /***********************************************************************************************
     * 2.����GEOϵͳ��¼�����б���ڵĳ���
     **********************************************************************************************/
    CNAS_XSD_PreformBestPreferedSystemSelectionAndGeoSysRecordSearchListExist_SysAcq(pstSyncCnf);

    return;
}


VOS_UINT32 CNAS_XSD_PerformNormalCallRedialSystemSeletion_SysAcq(
    CNAS_PRL_1X_SYSTEM_STRU            *pstCurSysInfo
)
{
    CNAS_PRL_SYS_PRI_TYPE_ENUM_UINT32                       enSysPref;
    CNAS_PRL_SYS_ROAMING_IND_ENUM_UINT8                     enRoamingInd;
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                     stCurSysGeoListInfo;
    CNAS_PRL_1X_SYSTEM_STRU                                *pstOrigSysInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulIsNegSys;

    ulIsNegSys = VOS_FALSE;

    /* reference from chapter 4.7.2 in CDG 143:
       The MS shall only stay and try to place the call on systems that comply with one
       of the following criteria:

       1. Systems that are same or more preferred than the original system;

       2. Systems have a home roaming indicator;

       3. The MS is allowed to place the call on a system from a different GEO (i.e., jump a GEO)
          as long this system meets one of the above criteria.

          Note: The criteria 3 has been included in the aboving criteria 1.
    */

    pstOrigSysInfo = CNAS_XSD_GetCallOrignalSys();

    /* check if the current system is orignal system */
    if (VOS_TRUE == CNAS_PRL_Is1xSysMatched(pstOrigSysInfo, pstCurSysInfo))
    {
        return VOS_TRUE;
    }

    /* HOME SID/NID �б�ֻӰ��ϵͳ���������ԣ�����Ϊ�ʺ�פ�������� */
    /* check��ǰϵͳ�Ƿ���homeϵͳ����������PRL */

    /* check if the current system is negative system in PRL system table */
    ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(pstCurSysInfo, VOS_FALSE, 0);

    if (VOS_TRUE == ulIsNegSys)
    {
        return VOS_FALSE;
    }

    ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(pstOrigSysInfo, VOS_FALSE, 0);

    if (VOS_TRUE == ulIsNegSys)
    {
        return VOS_TRUE;
    }

    NAS_MEM_SET_S(&stCurSysGeoListInfo, sizeof(stCurSysGeoListInfo), 0, sizeof(CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU));

    CNAS_PRL_Get1xSysGeoList(pstCurSysInfo, &stCurSysGeoListInfo);

    /* check if current system is home roaming indicator */
    if (VOS_TRUE == CNAS_PRL_IsCurrentSystemHomeRoamingIndicator(pstCurSysInfo,
                                                                 &stCurSysGeoListInfo,
                                                                 &enRoamingInd))
    {
        return VOS_TRUE;
    }

    /* compare the priority between current system and orignal system */
    enSysPref = CNAS_PRL_CompareSystemPreference(pstCurSysInfo, pstOrigSysInfo);

    if ((CNAS_PRL_SYS_PRI_HIGH == enSysPref)
     || (CNAS_PRL_SYS_PRI_SAME == enSysPref))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID CNAS_XSD_PerformNotLessPrefSystemSelectionAfterSyncSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU                       *pstSyncCnf
)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList = VOS_NULL_PTR;
    VOS_UINT16                                              usDstChanNum;
    VOS_UINT16                                              usCurScanListIndex;
    CNAS_PRL_1X_SYSTEM_STRU                                 stCurSysInfo;

    stCurSysInfo.stFreq.enBandClass = pstSyncCnf->stFreq.usBandClass;
    stCurSysInfo.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;
    stCurSysInfo.usNid              = pstSyncCnf->stSyncedSysId.usNid;
    stCurSysInfo.usSid              = pstSyncCnf->stSyncedSysId.usSid;

    /* check if orignal system is exist */
    if (VOS_FALSE == CNAS_XSD_GetCallOrigSysExistFlg())
    {
        /* if orignal system is not exist, here use procedure of switch on system selection */
        CNAS_XSD_PerformBestPreferedSystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);

        return;
    }

    /* check the current system is fit to camp on */
    if (VOS_TRUE == CNAS_XSD_PerformNormalCallRedialSystemSeletion_SysAcq(&stCurSysInfo))
    {
        /* notify CAS to camp on the current system */
        CNAS_XSD_SndCas1xCampSyncedChannelNtf();

        /* transfer to CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND state, start timer */
        CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);
        CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);
    }
    else
    {
        /* contine to request CAS synchronize system */
        usDstChanNum       = 0;
        usCurScanListIndex = CNAS_XSD_GetCurChannelScanIndex();
        pstScanList        = CNAS_XSD_GetScanChanListAddr();

        CNAS_XSD_GetNextScanChanList_SysAcq(usCurScanListIndex, &usDstChanNum, astDstChan, pstScanList);

        if (0 == usDstChanNum)
        {
            /* No frequencies need to synchronize */
            CNAS_XSD_ProcScanListSyncComplete_SysAcq();
        }
        else
        {
            /* contine to synchronize the remained frequencies */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, astDstChan);
        }
    }

    return;
}


VOS_VOID CNAS_XSD_PerformSystemSelectionAfterSyncSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enScanScene;
    CNAS_XSD_SYSTEM_ACQUIRE_MODE_ENUM_UINT8                 enSysAcqMode;
    VOS_UINT32                                              ulIsAnyCampOnFreq;
    CNAS_CCB_CARD_STATUS_ENUM_UINT8                         enCardStatus;

    enScanScene  = CNAS_XSD_GetCurSysAcqScene_SysAcq();
    enSysAcqMode = CNAS_XSD_GetSysAcqModeOfSpecialAcqScene(enScanScene);

    if (VOS_TRUE == g_CtrlCallRelScanList)
    {
        /* �����call release�������ҵ�ǰͬ���ɹ���Ƶ�㲻���κ�ϵͳ����פ����Ƶ�㣬��Best Prefer����
           ����ѡ��:
           ����Ŀǰcall release��������scan list�������һ�ι���(��call release�����µ�scan list����),
           ��������TCH״̬�µ�Ƶ���Լ�MRU0Ƶ���ϵ�ϵͳ����ֱ��פ����������Ƶ����Ҫ��BEST PREFERѡ�����
           �˴������⴦��
         */
        ulIsAnyCampOnFreq = CNAS_XSD_IsCallRelAnyCampOnFreq_SysAcq(&(pstSyncCnf->stFreq));

        if ((CNAS_XSD_SYS_ACQ_SCENE_CALL_RELEASED == CNAS_XSD_GetCurSysAcqScene_SysAcq())
         && (VOS_FALSE                            == ulIsAnyCampOnFreq))
        {
            enSysAcqMode = CNAS_XSD_SYSTEM_ACQUIRE_MODE_BEST_PREF;
        }
    }

    enCardStatus = CNAS_CCB_GetCsimCardStatus();

    if (CNAS_CCB_CARD_STATUS_ABSENT == enCardStatus)
    {
        enSysAcqMode = CNAS_XSD_SYSTEM_ACQUIRE_MODE_ANY;
    }

    CNAS_MNTN_LogCommonStatusInfo(UEPS_PID_XSD, (VOS_UINT8)enCardStatus);

    switch (enSysAcqMode)
    {
        case CNAS_XSD_SYSTEM_ACQUIRE_MODE_NOT_LESS_PREF:

            CNAS_XSD_PerformNotLessPrefSystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);
            break;

        case CNAS_XSD_SYSTEM_ACQUIRE_MODE_ALLOWED:

            CNAS_XSD_PerformAllowedSystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);
            break;

        case CNAS_XSD_SYSTEM_ACQUIRE_MODE_BEST_PREF:

            CNAS_XSD_PerformBestPreferedSystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);
            break;

        case CNAS_XSD_SYSTEM_ACQUIRE_MODE_SPECIFIC:

            CNAS_XSD_PerformSpecificSystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);
            break;

        case CNAS_XSD_SYSTEM_ACQUIRE_MODE_ANY:

            CNAS_XSD_PerformAnySystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);
            break;

        case CNAS_XSD_SYSTEM_ACQUIRE_MODE_PREFERED:

            CNAS_XSD_PerformPreferedSystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);
            break;

        default:

            CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_PerformSystemSelectionAfterSyncSucc_SysAcq: unsupported system acquire mode");
            break;
    }
}



VOS_VOID CNAS_XSD_ProcCasSyncCnfSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    /***********************************************************************************************
     * 1. �����ǰRF availableΪFALSE��֪ͨ�ϲ�RF available
     **********************************************************************************************/
    if (VOS_FALSE == CNAS_CCB_Get1XRfAvailFlg())
    {
        /* �ϱ�MSCC NO RF��Ϣ */
        CNAS_XSD_SndMsccRfAvailInd(VOS_TRUE);

        /* ���õ�ǰRF���ñ��Ϊ:RF���� */
        CNAS_CCB_Set1XRfAvailFlg(VOS_TRUE);
    }

    /***********************************************************************************************
     * 2. ����Ƶʱ���������avoidƵ��
     **********************************************************************************************/
    if (VOS_FALSE == CNAS_XSD_GetFreqLockMode())
    {
        /* ��Avoid����������Ƶģʽ�²Ų���AvoidƵ�� */
        if (VOS_TRUE != CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
        {
            /* ����ϵͳͬ��ȷ����Ϣ����Ҫ������Ƶ�� */
            CNAS_XSD_ProcFreqAvoidedInCasSyncCnf(pstSyncCnf);
        }
    }

    /***********************************************************************************************
     * 3. ֪ͨ�ϲ㵱ǰͬ����1Xϵͳ��SID��Ϣ
     **********************************************************************************************/
    /* ����ͬ���ɹ���ϵͳ��Ϣ */
    CNAS_CCB_Set1xSysInfo(pstSyncCnf->stSyncedSysId.usSid,
                          pstSyncCnf->stSyncedSysId.usNid,
                          pstSyncCnf->stFreq.usBandClass,
                          pstSyncCnf->stFreq.usChannel);

    if (pstSyncCnf->stSyncedSysId.usSid != CNAS_XSD_GetLastSyncedSys()->usSid)
    {
        /* �����ϴ�ͬ������ϵͳ */
        CNAS_XSD_UpdateLastSyncedSys((CNAS_PRL_1X_SYSTEM_ID_STRU *)&pstSyncCnf->stSyncedSysId);

        /* ֪ͨMSCC�µ�SID,NIDָʾ */
        CNAS_XSD_SndMsccSidNidInd(pstSyncCnf->stSyncedSysId.usSid, pstSyncCnf->stSyncedSysId.usNid);
    }

   /***********************************************************************************************
     * 4. ����ͬ����ϵͳ����ʷͬ����ϵͳ�б���
     **********************************************************************************************/
    CNAS_XSD_UpdateHistorySyncedSysFreqList((CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncCnf->stFreq));

    /***********************************************************************************************
     * 5. ˢ��ͬ���ɹ������µ�scan list��Ƶ��״̬
     **********************************************************************************************/
    CNAS_XSD_UpdateChanStatusUponRcvCasSyncCnfSucc_SysAcq(pstSyncCnf);

    /***********************************************************************************************
     * 6. ���и��ݳ�������ϵͳѡ��
     **********************************************************************************************/
    CNAS_XSD_PerformSystemSelectionAfterSyncSucc_SysAcq(pstSyncCnf);

    return;
}



VOS_VOID CNAS_XSD_ProcCasSyncCnfFail_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstEmcScanChanList   = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enCurAcqScene;
    VOS_UINT16                                              usUpdateSyncFailFreqNum;
    VOS_UINT32                                              ulIsCallAbort;

    usUpdateSyncFailFreqNum             = (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum;
    ulIsCallAbort                       = VOS_FALSE;

    pstEmcScanChanList                  = CNAS_XSD_GetEmcCallRedialScanChanListAddr();

    /***********************************************************************************************
     * 1. �����ǰRF availableΪFALSE��֪ͨ�ϲ�RF available
     **********************************************************************************************/
    /* SRLTE�£�sync cnf no rf�ĳ����·��õ�ǰ�ӿڣ�no rf�ĳ������ϱ�mscc rf���� */
    if ((VOS_FALSE              == CNAS_CCB_Get1XRfAvailFlg())
     && (CNAS_CAS_1X_RSLT_NO_RF != pstSyncCnf->enSyncRslt))
    {
        /* �ϱ�MSCC NO RF��Ϣ */
        CNAS_XSD_SndMsccRfAvailInd(VOS_TRUE);

        /* ���õ�ǰRF���ñ��Ϊ:RF���� */
        CNAS_CCB_Set1XRfAvailFlg(VOS_TRUE);
    }

    /***********************************************************************************************
     * 2. ����Ƶʱ,�ҷ�AvoidƵ��ͬ���������������avoidƵ��
     **********************************************************************************************/
    if (VOS_FALSE == CNAS_XSD_GetFreqLockMode())
    {
        /* ��Avoid����������Ƶģʽ�²Ų���AvoidƵ�� */
        if (VOS_TRUE != CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
        {
            /* ����ϵͳͬ��ȷ����Ϣ����Ҫ������Ƶ�� */
            CNAS_XSD_ProcFreqAvoidedInCasSyncCnf(pstSyncCnf);
        }
    }

    /***********************************************************************************************
     * 3. ˢ��ͬ��ʧ�ܵ�Ƶ��scan list
     **********************************************************************************************/
    CNAS_XSD_UpdateChanStatusUponRcvCasSyncCnfFail_SysAcq(pstSyncCnf);

    /* SRLTE 1x������������Դ��SYNC CNF�����NO RF����ˢ��Ƶ��״̬����һ�ּ���ͬ����Ƶ�� */
    if ((CNAS_CCB_VERSION_SRLTE                 == CNAS_CCB_IsVersionSrlte())
     && (CNAS_CAS_1X_RSLT_NO_RF                 == pstSyncCnf->enSyncRslt)
     && (0                                      != usUpdateSyncFailFreqNum))
    {
        usUpdateSyncFailFreqNum = (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum - 1;
    }

    /* ����������SD������ˢ�½�������Ƶ���б���������ظ���ͬ��ʧ�ܵ�Ƶ�� */
    if (VOS_NULL_PTR != pstEmcScanChanList->pstScanChanInfo)
    {
        if (VOS_TRUE == CNAS_XSD_IsEmcSDSysAcq_SysAcq(CNAS_XSD_GetCurSysAcqScene_SysAcq()))
        {

            (VOS_VOID)CNAS_XSD_UpdateSyncFailChanStatus(pstEmcScanChanList->usCurScanIndex,
                                                        usUpdateSyncFailFreqNum,
                                                        pstSyncCnf->astSyncRsltList,
                                                        pstEmcScanChanList);
        }
    }

    /***********************************************************************************************
     * 4. ����GEOϵͳ��¼�����б�״̬
     **********************************************************************************************/
    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    if (VOS_NULL_PTR != pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        /***********************************************************************************************
         * 4.1. ˢ��GEOϵͳ��¼�����б��е�Ƶ��Ϊ����������
         **********************************************************************************************/
        CNAS_XSD_UpdateSyncFailFreqStatusInGeoSysRecordSrchList_SysAcq( usUpdateSyncFailFreqNum,
                                                                       &pstSyncCnf->astSyncRsltList[0]);



        /*******************************************************************************************
         * 4.2. ����GEOϵͳ��¼�����б�����
         ******************************************************************************************/
        CNAS_XSD_ContinueGeoSysRecordSrch();

        return;
    }

    /***********************************************************************************************
     * 5. ��ǰ�Ƿ�GEO������������ʧ�ܣ�����������һ�������˳���ǰ����
        a)�����ز������б�Ϊ�գ�����������LTE\HRPD����̬�µ�1X����
        b)LTE\HRPD��idle̬������̬��
     **********************************************************************************************/
    enCurAcqScene = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    if (VOS_TRUE == CNAS_XSD_IsAllowedPerformAbortProcedure_SysAcq(enCurAcqScene, &ulIsCallAbort))
    {
        CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(ulIsCallAbort);
        return;
    }


    /***********************************************************************************************
     * 6. ����GEOϵͳ��¼����list�����ڵĴ�������
     **********************************************************************************************/
    CNAS_XSD_ProcSyncFailAndNoGeoSysRecorSearchList_SysAcq(pstSyncCnf);

    return;
}


VOS_VOID CNAS_XSD_ProcCasSyncCnfNoRf_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    /***********************************************************************************************
     * �����ǰ��SRLTE���鿴ͬ��ʧ�ܵ�Ƶ�����Ƿ���NO RF
     **********************************************************************************************/
    if (CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
    {
        if (CNAS_TIMER_STATUS_RUNNING == CNAS_GetSpecifiedTimerStatus(UEPS_PID_XSD, TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER, 0))
        {
            /* �����ǰ��SRLTE��ͬ��ʧ�ܵ�Ƶ������NO RF�ģ����Ҵ�ʱ������ʱ��δ��ʱ����Ҫ����ͬ����
             * ��NO RFƵ�㲻ˢ�£���һ�ּ���ͬ��*/
            CNAS_XSD_ProcCasSyncCnfFail_SysAcq(pstSyncCnf);
            return;
        }
    }


    /***********************************************************************************************
     * ����NO RF������ֱ���˳���ǰ״̬��
     **********************************************************************************************/
    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_FAILURE, CNAS_XSD_SYS_ACQ_SCENE_NO_RF, VOS_FALSE, CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();
}



VOS_VOID CNAS_XSD_ProcScanListSyncComplete_SysAcq(VOS_VOID)
{
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enScanScene;
    VOS_UINT16                                              usFreqNum;
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          astFreqList[CNAS_CAS_1X_MAX_FREQ_NUM];

    enScanScene   = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    usFreqNum     = 0;

    NAS_MEM_SET_S(&astFreqList[0],
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                  0x00,
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

    /***********************************************************************************************
    * 1. ͬ��availableϵͳ
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_IsAcquireAvaialSysNeeded(enScanScene))
    {
        /* ���ñ�־: ��ǰ����Ƶ��������available system list */
        CNAS_XSD_SetAvailSysAcqListFlg_SysAcq(VOS_TRUE);

        CNAS_XSD_GetNextAvailSysAcqList_SysAcq(&usFreqNum, &astFreqList[0]);

        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usFreqNum, &astFreqList[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            CNAS_XSD_ContinueSyncScanList_SysAcq(usFreqNum, &astFreqList[0]);
        }

        return;
    }

    /***********************************************************************************************
     * 2. ��availableϵͳ����ͬ��Avoidϵͳ
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_IsAcquireAvoidSysNeeded(enScanScene))
    {
        /* ����ͬ������ͬ��Avoidϵͳ */
        CNAS_XSD_GetNextAvoidSysAcqList_SysAcq(&usFreqNum, &astFreqList[0]);

        if (0 != usFreqNum)
        {
            CNAS_XSD_SetAvoidSysAcqListFlg_SysAcq(VOS_TRUE);

            /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
            if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
            {
                /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
                CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usFreqNum, &astFreqList[0]);
                CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
                CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                    CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
            }
            else
            {
                CNAS_XSD_ContinueSyncScanList_SysAcq(usFreqNum, &astFreqList[0]);
            }

            return;
        }
    }

    /***********************************************************************************************
     * 3. �������Avoid��Availableϵͳͬ��������ʧ��
     **********************************************************************************************/
    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_FAILURE,
                           CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                           CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                           CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();

    return;
}


VOS_VOID CNAS_XSD_ContinueSyncScanList_SysAcq(
    VOS_UINT16                          usDstChanNum,
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstDstChan
)
{
    VOS_UINT32                          ulWaitCasSyncCnfTimerLen;

    ulWaitCasSyncCnfTimerLen = CNAS_XSD_GetWaitCasSystemSyncCnfTimerLen(usDstChanNum);

    CNAS_XSD_SndCasSysSyncReq(usDstChanNum, pstDstChan);

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_SYNC_CNF״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF, ulWaitCasSyncCnfTimerLen);

    return;
}


VOS_VOID CNAS_XSD_PeformBestPreferedSystemSelectionUsingNewGeo_SysAcq(
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                    *pstGeoListInfo,
    CNAS_PRL_1X_SYSTEM_STRU                                *pstCurSysInfo,
    VOS_UINT32                                              ulIsNegSys
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;
    CNAS_CAS_1X_SYSTEM_IDENTITY_STRU                        stCurrSysId;
    CNAS_CAS_1X_SYNC_RSLT_INFO_STRU                         stSyncFailChan;
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                       stFreq;
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                                              usDstChanNum;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enCurAcqScene;
    VOS_UINT32                                              ulIsCallAbort;

    /***********************************************************************************************
     * 1. ˢ��GEO�����б��б�������GEOΪ������
     **********************************************************************************************/
    CNAS_XSD_UpdateGeoListSrchStatus(pstGeoListInfo, VOS_TRUE);

    /***********************************************************************************************
     * 2. �����µ�GEO�����б�
     **********************************************************************************************/
    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    NAS_MEM_SET_S(pstGeoSysRecSrchList, sizeof(CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU), 0, sizeof(CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU));

    CNAS_XSD_ConstructGeoSysRecordSrchList(pstGeoListInfo, pstCurSysInfo, pstGeoSysRecSrchList);

    if (VOS_NULL_PTR == pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        /* �ڴ����ʧ�ܣ�ֱ�ӷ��� */
        return;
    }

    CNAS_XSD_LogAcqSystemList(pstGeoSysRecSrchList);

    if (VOS_FALSE == ulIsNegSys)
    {
        stFreq.usBandClass = pstCurSysInfo->stFreq.enBandClass;
        stFreq.usChannel   = pstCurSysInfo->stFreq.usChannel;

        stCurrSysId.usNid  = pstCurSysInfo->usNid;
        stCurrSysId.usSid  = pstCurSysInfo->usSid;

        /* ˢ�µ�ǰͬ���ɹ���Ƶ��״̬��GEO��ϵͳ��¼�����б�����match��ϵͳƵ��ˢ��Ϊ�������ڣ�
         * ����ϵͳ��¼��Ƶ��ˢ��Ϊ���������� */
        CNAS_XSD_UpdateSyncSuccFreqStatusInGeoSysRecordSrchList_SysAcq(&stFreq,
                                                                       &stCurrSysId);

        /* ����ǰ֪ͨ�ϲ����״̬ */
        CNAS_XSD_ReportSrvAvailableInd_SysAcq(pstCurSysInfo);
    }
    else
    {
        NAS_MEM_SET_S(&stSyncFailChan, sizeof(stSyncFailChan), 0, sizeof(CNAS_CAS_1X_SYNC_RSLT_INFO_STRU));

        stSyncFailChan.stFreq.usBandClass = pstCurSysInfo->stFreq.enBandClass;
        stSyncFailChan.stFreq.usChannel   = pstCurSysInfo->stFreq.usChannel;

        /* ��ǰϵͳ��negativeϵͳ��ˢ��GEO��ϵͳ��¼�����б��ж�Ӧ��Ƶ��Ϊ���������� */
        CNAS_XSD_UpdateSyncFailFreqStatusInGeoSysRecordSrchList_SysAcq(1, &stSyncFailChan);

        /* ����ϵͳ��negʱ����ˢ�¸�Ƶ��Ϊ�����ڣ������ˢ�µĻ�������ͬ����Ȼ��ͬ����Ƶ��
           ��ȡ��һ��ͬ��Ƶ��ʱ���߼�Ϊδͬ������ͬ������ */
        CNAS_XSD_UpdateAvailOrNegSystemChanStatusInScanChanList_SysAcq(&pstCurSysInfo->stFreq,
                                                                       CNAS_XSD_GetScanChanListAddr());
    }

    /***********************************************************************************************
     * 3. �����ǰϵͳ����������ȼ�ϵͳ��֪ͨ�����פ��
     **********************************************************************************************/
    /* ����scan list���б��е�Ƶ��״̬��ˢ��GEOϵͳ��¼�����б��е�Ƶ��״̬���´��������� */
    CNAS_XSD_UpdateGeoSysRecordListInfoAccordingToScanChanList(pstGeoSysRecSrchList);

    if (VOS_TRUE == CNAS_XSD_IsCurrSysMostPref1xSysInGeoSysRecordSrchList(pstCurSysInfo, pstGeoSysRecSrchList))
    {

        /* ����ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF */
        CNAS_XSD_SndCas1xCampSyncedChannelNtf();

        /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
        CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

        /* ����������ʱ�� */
        CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);

        return;
    }

    /*******************************************************************************************
     * 4. ����GEOϵͳ��¼�����б�����
     ******************************************************************************************/
    usDstChanNum = 0;

    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    CNAS_XSD_GetNextSysSyncListFromGeoSysRecordSrchList(CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq(),
                                                        &usDstChanNum,
                                                        &astDstChan[0]);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_TRUE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* ����GEOϵͳ��¼Ƶ��ͬ������ */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
        }

        return;
    }

    /*******************************************************************************************
     * 5. ���GEO�����б�״̬�Լ��ͷ�GEOϵͳ��¼�����б��ڴ�
     ******************************************************************************************/
    /* �����GEO�����б�״̬ */
    CNAS_XSD_ClearGeoSrchListStatus();

    /* LOG GEO list����״̬��Ϣ */
    CNAS_XSD_LogGeoListSrchStatus(CNAS_XSD_GetGeoSrchListInfoAddr());

    /* �ͷ�GEOϵͳ��¼�����б���ڴ� */
    if (VOS_NULL_PTR != CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo)
    {
        PS_MEM_FREE(UEPS_PID_XSD, CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo);
        CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo = VOS_NULL_PTR;
    }

    /*******************************************************************************************
     * 6. GEO������������Ҫ����ͬ��scanlist������������һ��������ϵ�ǰ�������̡�
       a)���֮ǰ���յ������ز�������
       b)��ǰ������������CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN���ҵ�ǰ��LTE\HRPD����̬
     ******************************************************************************************/
    enCurAcqScene       = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    if (VOS_TRUE == CNAS_XSD_IsAllowedPerformAbortProcedure_SysAcq(enCurAcqScene, &ulIsCallAbort))
    {
        CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(ulIsCallAbort);
        return;
    }

    /*******************************************************************************************
     * 7. ����scan listƵ���б�����
     ******************************************************************************************/
    usDstChanNum = 0;
    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    CNAS_XSD_GetNextScanChanList_SysAcq(CNAS_XSD_GetCurChannelScanIndex(),
                                        &usDstChanNum,
                                        astDstChan,
                                        CNAS_XSD_GetScanChanListAddr());

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* scan listδȫ��ͬ���꣬������ͬ������ */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, astDstChan);
        }

        return;
    }

    /*******************************************************************************************
     * 8. ����scan listͬ����ɵĺ�������
     ******************************************************************************************/
    /* scan list��ȫ��ͬ���� */
    CNAS_XSD_ProcScanListSyncComplete_SysAcq();
}


VOS_VOID CNAS_XSD_ContinueSysSync_SysAcq(VOS_VOID)
{
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList          = VOS_NULL_PTR;
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                                              usCurScanListIndex;
    VOS_UINT16                                              usDstChanNum;

    /***********************************************************************************************
     * 1. �����availableϵͳͬ��������availableϵͳͬ��
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_GetAvailSysAcqListFlg_SysAcq())
    {
        CNAS_XSD_ContinueAvailSysListSrch();

        return;
    }

    /* ���avoidϵͳͬ��������avoidϵͳͬ�� */
    if (VOS_TRUE == CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
    {
        CNAS_XSD_ContinueAvoidSysListSrch();

        return;
    }

    /***********************************************************************************************
     * 2. ����GEOϵͳ��¼�����б���ڵĳ���
     **********************************************************************************************/
    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    if (VOS_NULL_PTR != pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        CNAS_XSD_ContinueGeoSysRecordSrch();

        return;
    }

    /***********************************************************************************************
     * 3. ����scan listƵ������
     **********************************************************************************************/
    usDstChanNum       = 0;
    pstScanList        = CNAS_XSD_GetScanChanListAddr();
    usCurScanListIndex = CNAS_XSD_GetCurChannelScanIndex();

    CNAS_XSD_GetNextScanChanList_SysAcq(usCurScanListIndex, &usDstChanNum, astDstChan, pstScanList);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
        }

        return;
    }

    /***********************************************************************************************
     * 4. scan list������ɴ���
     **********************************************************************************************/
    CNAS_XSD_ProcScanListSyncComplete_SysAcq();

    return;
}


CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32 CNAS_XSD_GetCurSysAcqScene_SysAcq(VOS_VOID)
{
    CNAS_XSD_MSG_STRU                                      *pstEntryMsg  = VOS_NULL_PTR;
    CNAS_XSD_XSD_SYSTEM_ACQUIRED_REQ_STRU                  *pstIntSysAcqReq = VOS_NULL_PTR;
    MSCC_XSD_SYSTEM_ACQUIRE_REQ_STRU                       *pstSysAqcReq    = VOS_NULL_PTR;

    /* ��õ�ǰ����CNAS XSD�����Ϣ�Ļ�������ַ */
    pstEntryMsg                 = CNAS_XSD_GetCurrFsmEntryMsgAddr();

    if (CNAS_BuildEventType(UEPS_PID_MSCC, ID_MSCC_XSD_SYSTEM_ACQUIRE_REQ)
            == pstEntryMsg->ulEventType)
    {
        pstSysAqcReq = (MSCC_XSD_SYSTEM_ACQUIRE_REQ_STRU *)pstEntryMsg->aucMsgBuffer;

        if (MSCC_XSD_SYS_ACQ_TYPE_NORMAL == pstSysAqcReq->enSysAcqType)
        {
            return CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON;
        }
        else
        {
            return CNAS_XSD_SYS_ACQ_SCENE_POWER_ON_INIT_SEARCH;
        }
    }

    if (CNAS_BuildEventType(UEPS_PID_XSD, ID_CNAS_XSD_XSD_SYSTEM_ACQUIRED_REQ)
            == pstEntryMsg->ulEventType)
    {
        pstIntSysAcqReq = (CNAS_XSD_XSD_SYSTEM_ACQUIRED_REQ_STRU *)pstEntryMsg->aucMsgBuffer;

        return pstIntSysAcqReq->enSysAcqScene;
    }

    return CNAS_XSD_SYS_ACQ_SCENE_BUTT;
}


VOS_UINT8 CNAS_XSD_IsImmediateSysAcq_SysAcq(VOS_VOID)
{
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enSysAcqScene;

    enSysAcqScene = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    /* call release����scan list�����Ѿ��޸ģ�����һ�ι���(��call release�����µ�build scan list����)
     * ���ʧ��ʱ��������������
     */
    /* call release with redir�ĳ�����Ҫ������ */
    if (CNAS_XSD_SYS_ACQ_SCENE_CALL_RELEASED_WITH_REDIR == enSysAcqScene)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 CNAS_XSD_IsEmergencySysAcq_SysAcq(
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enSysAcqScene
)
{
    switch(enSysAcqScene)
    {
        case CNAS_XSD_SYS_ACQ_SCENE_EMC_CALLBACK_SYSTEM_LOST:
        case CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL:
        case CNAS_XSD_SYS_ACQ_SCENE_EMC_CALL_RELEASED:
            return VOS_TRUE;

        default:
            return CNAS_XSD_IsEmcSDSysAcq_SysAcq(enSysAcqScene);
    }
}

VOS_UINT32 CNAS_XSD_IsEmcSDSysAcq_SysAcq(
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enSysAcqScene
)
{
    switch(enSysAcqScene)
    {
        case CNAS_XSD_SYS_ACQ_SCENE_REGIS_REJ_WITH_REDIR_AND_RTF_EMC:
        case CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_NOT_IN_PRL_OR_NEG_EMC:
        case CNAS_XSD_SYS_ACQ_SCENE_SYS_RESEL_IN_PRL_AND_PREF_EMC:
        case CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_SKIP_MRU0_EMC:
        case CNAS_XSD_SYS_ACQ_SCENE_REDIRECTION_WITH_INVALID_GSRDM_EMC:
        case CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_IN_PRL_EMC:
        case CNAS_XSD_SYS_ACQ_SCENE_MAX_ACCESS_FAILURE_NOT_IN_PRL_EMC:
            return VOS_TRUE;

        default:
            return VOS_FALSE;
    }
}


VOS_VOID CNAS_XSD_GetNextAvailSysAcqList_SysAcq(
    VOS_UINT16                                             *pusFreqNum,
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstFreqList
)
{
    CNAS_XSD_AVAILABLE_LIST_STRU       *pstAvailSysList = VOS_NULL_PTR;
    VOS_UINT16                          i;

    VOS_UINT16                          usMaxFreqNum;

    usMaxFreqNum    = CNAS_CAS_1X_MAX_FREQ_NUM;

    pstAvailSysList = CNAS_XSD_GetAvailSysFreqListAddr();

    *pusFreqNum = 0;

    usMaxFreqNum = CNAS_XSD_GetMaxSyncFreqNumOfScanList(usMaxFreqNum);

    for (i = pstAvailSysList->ucNextSrchIndex;
         i < CNAS_MIN(pstAvailSysList->ucAvailSysNum, CNAS_XSD_MAX_AVAILABLE_SYS_NUM);
         i++
         )
    {

        pstFreqList->enBandClass = pstAvailSysList->astAvailSystem[i].stFreq.enBandClass;
        pstFreqList->usChannel   = pstAvailSysList->astAvailSystem[i].stFreq.usChannel;

        pstFreqList++;
        (*pusFreqNum)++;

        if (*pusFreqNum == usMaxFreqNum)
        {
            return;
        }
    }

    return;
}



VOS_VOID CNAS_XSD_GetNextAvoidSysAcqList_SysAcq(
    VOS_UINT16                                             *pusFreqNum,
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstFreqList
)
{
    CNAS_XSD_AVOID_FREQ_LIST_STRU      *pstAvoidSysList = VOS_NULL_PTR;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanChanList = VOS_NULL_PTR;
    VOS_UINT16                          i;
    VOS_UINT16                          usMaxChanScanNum;

    pstAvoidSysList = CNAS_XSD_GetAvoidFreqListAddr();
    pstScanChanList = CNAS_XSD_GetScanChanListAddr();

    *pusFreqNum = 0;
    usMaxChanScanNum = CNAS_XSD_GetMaxSyncFreqNumOfScanList(CNAS_CAS_1X_MAX_FREQ_NUM);

    CNAS_XSD_LogAvoidFreqList(CNAS_XSD_GetAvoidFreqListAddr());

    for (i = pstAvoidSysList->ucNextAcqIndex; i < CNAS_MIN(pstAvoidSysList->ucAvoidFreqNum, CNAS_XSD_MAX_AVOID_FREQ_NUM); i++)
    {
        if (VOS_FALSE == pstAvoidSysList->astAvoidFreqInfo[i].ucAvoidFlag)
        {
            continue;
        }

        /* ���������Ѿ�ͬ������Ƶ�㱻AVOID�������ظ�ͬ�� */
        if (VOS_TRUE == CNAS_XSD_IsCurFreqSynced(&(pstAvoidSysList->astAvoidFreqInfo[i].stAvoidFreq),
                                                 pstScanChanList))
        {
            continue;
        }

        pstFreqList->enBandClass = pstAvoidSysList->astAvoidFreqInfo[i].stAvoidFreq.enBandClass;
        pstFreqList->usChannel   = pstAvoidSysList->astAvoidFreqInfo[i].stAvoidFreq.usChannel;

        pstFreqList++;
        (*pusFreqNum)++;

        if (*pusFreqNum == usMaxChanScanNum)
        {
            return;
        }
    }

    return;
}


VOS_VOID CNAS_XSD_UpdateSyncFailFreqStatusInGeoSysRecordSrchList_SysAcq(
    VOS_UINT16                          usSyncFailChanNum,
    CNAS_CAS_1X_SYNC_RSLT_INFO_STRU    *pstSyncFailChanList
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;
    CNAS_XSD_SCAN_CHAN_STATUS_ENUM_UINT8                    enSysStatus;
    VOS_UINT16                                              i;

    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    enSysStatus   = CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST;

    for (i = 0; i < CNAS_MIN(usSyncFailChanNum, CNAS_CAS_1X_MAX_FREQ_NUM); i++)
    {
        /* ����ȡ�б���ָ��Ƶ��Ķ�Ӧ�б�ȫ��ˢ��Ϊ����������� */
        CNAS_XSD_UpdateSysFreqStatusByFreqChanInGeoSysRecordSrchList((CNAS_PRL_FREQENCY_CHANNEL_STRU *)&pstSyncFailChanList[i].stFreq,
                                                                     enSysStatus,
                                                                     pstGeoSysRecSrchList);
    }

    CNAS_XSD_LogAcqSystemList(pstGeoSysRecSrchList);

    return;
}


VOS_VOID CNAS_XSD_UpdateSyncSuccFreqStatusInGeoSysRecordSrchList_SysAcq(
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                      *pstFreq,
    CNAS_CAS_1X_SYSTEM_IDENTITY_STRU                       *pstSyncedSysId
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;
    CNAS_PRL_1X_SYSTEM_STRU                                 stAcqSuccSys;
    CNAS_XSD_SCAN_CHAN_STATUS_ENUM_UINT8                    enSysStatus;
    VOS_UINT16                                              i;

    stAcqSuccSys.stFreq.enBandClass = pstFreq->usBandClass;
    stAcqSuccSys.stFreq.usChannel   = pstFreq->usChannel;
    stAcqSuccSys.usNid              = pstSyncedSysId->usNid;
    stAcqSuccSys.usSid              = pstSyncedSysId->usSid;

    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();
    enSysStatus   = CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_EXIST;

    CNAS_XSD_UpdateSysFreqStatusBySysInGeoSysRecordSrchList(&stAcqSuccSys, enSysStatus, pstGeoSysRecSrchList);

    /* ���ڷ���AS����ͬƵ�������SID+NID,Ӧ���ǲ���ʧ�ܵ�,���¶�Ӧ�Ĳ���ϵͳ״̬ */
    enSysStatus   = CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST;
    for (i = 0; i < pstGeoSysRecSrchList->usTotalNum; i++)
    {
        if ((stAcqSuccSys.stFreq.enBandClass == pstGeoSysRecSrchList->pstAcqSysInfo[i].stAcqSys.stFreq.enBandClass)
         && (stAcqSuccSys.stFreq.usChannel   == pstGeoSysRecSrchList->pstAcqSysInfo[i].stAcqSys.stFreq.usChannel))
        {
            /* ����ɹ���ϵͳ��ˢ��״̬ */
            if (VOS_FALSE == CNAS_PRL_Is1xSysIdMatched(stAcqSuccSys.usSid,
                                                       stAcqSuccSys.usNid,
                                                       pstGeoSysRecSrchList->pstAcqSysInfo[i].stAcqSys.usSid,
                                                       pstGeoSysRecSrchList->pstAcqSysInfo[i].stAcqSys.usNid))

            {
                pstGeoSysRecSrchList->pstAcqSysInfo[i].enSysStatus = enSysStatus;
            }
        }
    }

    CNAS_XSD_LogAcqSystemList(pstGeoSysRecSrchList);

    return;
}


VOS_VOID CNAS_XSD_QuitFsmSysAcq_SysAcq(VOS_VOID)
{
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstCallRedialScanChanList = VOS_NULL_PTR;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanChanList           = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enCurAcqScene;

    enCurAcqScene                       = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    /* ���GEO����״̬ */
    CNAS_XSD_ClearGeoSrchListStatus();

    /* �˳�״̬��ʱ���avoid list ����*/
    CNAS_XSD_InitAvoidSysListSrchIndex();

    /* LOG GEO list����״̬��Ϣ */
    CNAS_XSD_LogGeoListSrchStatus(CNAS_XSD_GetGeoSrchListInfoAddr());

    /* �ͷ�GEOϵͳ��¼�����б���ڴ� */
    if (VOS_NULL_PTR != CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo)
    {
        PS_MEM_FREE(UEPS_PID_XSD, CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo);
        CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo = VOS_NULL_PTR;
    }

    pstScanChanList           = CNAS_XSD_GetScanChanListAddr();
    switch (enCurAcqScene)
    {
        case CNAS_XSD_SYS_ACQ_SCENE_NORMAL_CALL_REDIAL:
            {
                /* for normal call redial scene, update the call redial channle list by the current
                   scan channle list */

                pstCallRedialScanChanList = CNAS_XSD_GetCallRedialScanChanListAddr();

                CNAS_XSD_UpdateScanChanList(pstCallRedialScanChanList, pstScanChanList);

                CNAS_XSD_LogScanChannelList(ID_CNAS_XSD_MNTN_LOG_CALL_REDIAL_SCAN_CHAN_LIST_IND,
                                            pstCallRedialScanChanList);
            }
            break;


        case CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL:
            {
                /* for emc call redial scene, update the call redial channle list by the current
                   scan channle list */
                pstCallRedialScanChanList = CNAS_XSD_GetEmcCallRedialScanChanListAddr();

                CNAS_XSD_UpdateScanChanList(pstCallRedialScanChanList, pstScanChanList);

                CNAS_XSD_LogScanChannelList(ID_CNAS_XSD_MNTN_LOG_EMC_CALL_REDIAL_SCAN_CHAN_LIST_IND,
                                            pstCallRedialScanChanList);
            }
            break;

        case CNAS_XSD_SYS_ACQ_SCENE_CALL_RELEASED_WITH_REDIR:
            {
                /* ����scan listƵ��״̬��ˢ��redirectionƵ��״̬ */
                CNAS_XSD_UpdateRedirectionFreqStatusAccordingToScanChanList(pstScanChanList);
            }
            break;

        default:
            break;
    }

    CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_NO_RF_PROTECT_TIMER);

    CNAS_XSD_QuitFsmL2();
}


VOS_VOID CNAS_XSD_ProcFreqAvoidedInCasSyncCnf(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    VOS_UINT8                           i;
    CNAS_PRL_1X_SYSTEM_STRU             stSysInfo;
    VOS_UINT32                          ulIsNegSys;

    ulIsNegSys = VOS_FALSE;

    if (CNAS_CAS_1X_RSLT_SUCCESS == pstSyncCnf->enSyncRslt)
    {
        /* ����ɹ���Ƶ�� */
        stSysInfo.stFreq.enBandClass = pstSyncCnf->stFreq.usBandClass;
        stSysInfo.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;

        stSysInfo.usNid  = pstSyncCnf->stSyncedSysId.usNid;
        stSysInfo.usSid  = pstSyncCnf->stSyncedSysId.usSid;

        ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(&stSysInfo, VOS_FALSE, 0);

        /* �����negativeϵͳ�����Ƶ���avoid�б��� */
        if (VOS_TRUE == ulIsNegSys)
        {
            if (VOS_TRUE == CNAS_XSD_IsAddInAvoidList(CNAS_XSD_AVOID_SID_NID_IS_REJ_BY_PRL))
            {
                CNAS_XSD_UpdataAvoidListInfo(&(stSysInfo.stFreq), CNAS_XSD_AVOID_SID_NID_IS_REJ_BY_PRL);
            }
        }
    }


    for (i = 0; i < pstSyncCnf->ulSyncFailFreqNum; i++)
    {
        if (CNAS_CAS_1X_SYS_SRCH_SYNC_FAIL_PROTO_MISMATCH != pstSyncCnf->astSyncRsltList[i].enFailReason)
        {
            continue;
        }

        /* ����avoid�б�ǰ�����slice�Ƿ��Ѿ����� */
        stSysInfo.stFreq.enBandClass = pstSyncCnf->astSyncRsltList[i].stFreq.usBandClass;
        stSysInfo.stFreq.usChannel   = pstSyncCnf->astSyncRsltList[i].stFreq.usChannel;

        if (VOS_TRUE == CNAS_XSD_IsAddSyncFailFreqInAvoidList(&(stSysInfo.stFreq),
                                                              pstSyncCnf->astSyncRsltList[i].ulSlice,
                                                              CNAS_XSD_AVOID_P_REV_MISMATCH))
        {
            CNAS_XSD_UpdataAvoidListInfo(&(stSysInfo.stFreq), CNAS_XSD_AVOID_P_REV_MISMATCH);
        }

    }


    /*���avoid�б��д��ڽ��ñ�ǣ���ʾ���ڱ����õ�Ƶ�㣬������ʱ�� */
    if (VOS_TRUE == CNAS_XSD_IsExistAvoidFlagInAvoidlist())
    {
        /* ���avoid timerδ����������avoid timer��ʱ�� */
        if (CNAS_TIMER_STATUS_RUNNING != CNAS_GetSpecifiedTimerStatus(UEPS_PID_XSD,
                                                                      TI_CNAS_XSD_SLICE_REVERSE_PROTECT_TIMER,
                                                                      0))
        {
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SLICE_REVERSE_PROTECT_TIMER,
                                TI_CNAS_XSD_SLICE_REVERSE_PROTECT_TIMER_LEN);
        }
    }

    CNAS_XSD_LogAvoidFreqList(CNAS_XSD_GetAvoidFreqListAddr());
}


VOS_VOID CNAS_XSD_PerformAbortProcedure_SysAcq(VOS_VOID)
{
    CNAS_XSD_StopTimer(TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER);

    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_ABORTED,
                           CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                           CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                           CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();

    return;
}


VOS_UINT32 CNAS_XSD_RcvMsccCallRedialSystemAcquireNtf_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU            *pstCallRedialNtfMsg = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                       enCurScene;

    pstCallRedialNtfMsg = (MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU*)pstMsg;

    enCurScene = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    /* ��������ȼ���Ϻ�֮ǰ��ϵĻ�����Ϣ */
    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_NETWORK_SRCH_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_MODE_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_END_EMC_CALLBACK_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_MO_CALL_END_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF);

    /* ����ǰ���ڸ������ȼ��Ĵ�ϵĻ�����Ϣ�����Ա���Ϣ */
    if (0 != CNAS_XSD_GetCacheMsgNum())
    {
        return VOS_TRUE;
    }

    CNAS_XSD_SetAbortFlg_SysAcq(VOS_FALSE);

    /* �ų�һЩ��Ҫ������Ϣ�ĳ��� */
    /* ���������� */
    if (NAS_MSCC_PIF_CDMA_CALL_TYPE_1X_EMC_VOICE_CALL       == pstCallRedialNtfMsg->enCallType)
    {
        if (CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL           == enCurScene)
        {
            /* ��ǰ�Ѿ��ڽ��������ز���������Ϣ������������ */
            return VOS_TRUE;
        }
    }
    /* ��ͨ�������ݺ����� */
    else
    {
        if ((CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN != enCurScene)
        &&  ((NAS_MSCC_PIF_CDMA_CALL_CAUSE_NO_SERVICE                == pstCallRedialNtfMsg->enCause)
          || (NAS_MSCC_PIF_CDMA_CALL_CAUSE_INTERNAL_NO_SERVICE       == pstCallRedialNtfMsg->enCause)))
        {
            /* ��ǰ�Ѿ��ں����ز���������Ϣ������������ */
            return VOS_TRUE;
        }


        if ((CNAS_XSD_SYS_ACQ_SCENE_NORMAL_CALL_REDIAL      == enCurScene)
        ||  (CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON               == enCurScene))
        {
            /* ��ǰ�Ѿ��ں����ز���������Ϣ������������ */
            return VOS_TRUE;
        }

    }

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALL_REDIAL);

    /* ������������,����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* ��������ز�ָʾ��Ϣ */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    if (CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN != enCurScene)
    {

        /* ����ǰ�����ڸ������ȼ��Ļ�����Ϣ�������µĴ�� */
        CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

        /* ��CAS����ID_CNAS_CAS_1X_STOP_SYSTEM_SYNC_REQ��Ϣֹͣͬ�� */
        CNAS_XSD_SndCasStopSysSyncReq();

        /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF */
        CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
        CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);
    }

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccCallRedialSystemAcquireNtf_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU           *pstCallRedialNtfMsg = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enCurScene;

    pstCallRedialNtfMsg = (MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU*)pstMsg;

    enCurScene = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    /* ��������ȼ���Ϻ�����Ļ�����Ϣ */
    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_NETWORK_SRCH_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_MODE_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_END_EMC_CALLBACK_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_MO_CALL_END_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF);

    /* ����ǰ���ڸ������ȼ��Ĵ�ϵĻ�����Ϣ�����Ա���Ϣ */
    if (0 != CNAS_XSD_GetCacheMsgNum())
    {
        return VOS_TRUE;
    }

    CNAS_XSD_SetAbortFlg_SysAcq(VOS_FALSE);

    /* �ų�һЩ��Ҫ������Ϣ�ĳ��� */
    /* ���������� */
    if (NAS_MSCC_PIF_CDMA_CALL_TYPE_1X_EMC_VOICE_CALL       == pstCallRedialNtfMsg->enCallType)
    {
        if (CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL           == enCurScene)
        {
            /* ��ǰ�Ѿ��ڽ��������ز���������Ϣ������������ */
            return VOS_TRUE;
        }
    }
    /* ��ͨ�������ݺ����� */
    else
    {
        if ((CNAS_XSD_SYS_ACQ_SCENE_NORMAL_CALL_REDIAL      == enCurScene)
        ||  (CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON               == enCurScene))
        {
            /* ��ǰ�Ѿ��ں����ز���������Ϣ������������ */
            return VOS_TRUE;
        }
    }

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALL_REDIAL);

    /* ����ǰ�����ڸ������ȼ��Ļ�����Ϣ�������µĴ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* ��������ز�ָʾ��Ϣ */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccCallRedialSystemAcquireNtf_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU           *pstCallRedialNtfMsg = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enCurScene;

    pstCallRedialNtfMsg = (MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU*)pstMsg;

    enCurScene = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    /* ��������ȼ���Ϻ�����Ļ�����Ϣ */
    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_NETWORK_SRCH_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_MODE_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_END_EMC_CALLBACK_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_MO_CALL_END_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_CALL_REDIAL_SYSTEM_ACQUIRE_NTF);

    /* ����ǰ���ڸ������ȼ��Ĵ�ϵĻ�����Ϣ�����Ա���Ϣ */
    if (0 != CNAS_XSD_GetCacheMsgNum())
    {
        return VOS_TRUE;
    }

    CNAS_XSD_SetAbortFlg_SysAcq(VOS_FALSE);

    /* �ų�һЩ��Ҫ������Ϣ�ĳ��� */
    /* ���������� */
    if (NAS_MSCC_PIF_CDMA_CALL_TYPE_1X_EMC_VOICE_CALL       == pstCallRedialNtfMsg->enCallType)
    {
        if (CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL           == enCurScene)
        {
            /* ��ǰ�Ѿ��ڽ��������ز���������Ϣ������������ */
            return VOS_TRUE;
        }
    }
    /* ��ͨ�������ݺ����� */
    else
    {
        if ((CNAS_XSD_SYS_ACQ_SCENE_NORMAL_CALL_REDIAL      == enCurScene)
        ||  (CNAS_XSD_SYS_ACQ_SCENE_SWITCH_ON               == enCurScene))
        {
            /* ��ǰ�Ѿ��ں����ز���������Ϣ������������ */
            return VOS_TRUE;
        }
    }

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALL_REDIAL);

    /* ����ǰ�����ڸ������ȼ��Ļ�����Ϣ�������µĴ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* ��������ز�ָʾ��Ϣ */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    return VOS_TRUE;
}




VOS_VOID CNAS_XSD_PerformAllowedSystemSelectionAfterSyncSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usCurScanListIndex;
    VOS_UINT16                          usDstChanNum;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanList = VOS_NULL_PTR;
    CNAS_PRL_1X_SYSTEM_STRU             stCurSysInfo;
    VOS_UINT32                          ulIsNegSys;
    VOS_UINT32                          ulHrpdConnExistFlag;
    VOS_UINT32                          ulLteConnExistFlag;

    ulLteConnExistFlag      = CNAS_XSD_GetLteConnExistFlag();
    ulHrpdConnExistFlag     = CNAS_XSD_GetHrpdConnExistFlag();

    stCurSysInfo.stFreq.enBandClass = pstSyncCnf->stFreq.usBandClass;
    stCurSysInfo.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;
    stCurSysInfo.usNid              = pstSyncCnf->stSyncedSysId.usNid;
    stCurSysInfo.usSid              = pstSyncCnf->stSyncedSysId.usSid;

    ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(&stCurSysInfo, VOS_FALSE, 0);

    /* first check the system is allowed or not */
    if (VOS_TRUE == ulIsNegSys)
    {
        usCurScanListIndex = CNAS_XSD_GetCurChannelScanIndex();
        pstScanList        = CNAS_XSD_GetScanChanListAddr();

        usDstChanNum       = 0;
        NAS_MEM_SET_S(astDstChan, sizeof(astDstChan), 0x0, sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

        CNAS_XSD_GetNextScanChanList_SysAcq(usCurScanListIndex, &usDstChanNum, astDstChan, pstScanList);

        /* scan list��ȫ��ͬ���� */
        if (0 == usDstChanNum)
        {
            CNAS_XSD_ProcScanListSyncComplete_SysAcq();
        }
        /* scan listδȫ��ͬ���꣬������ͬ������ */
        else
        {
            /* �����ǰhrpd����������srlte��lte���������˳���ǰ�������̣����½�����������̬�µ�1x���� */
            if ((VOS_TRUE                == ulHrpdConnExistFlag)
             || ((CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
              && (VOS_TRUE               == ulLteConnExistFlag)))
            {
                CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(VOS_FALSE);
                return;
            }
            else
            {
                CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, astDstChan);
            }
        }

        return;
    }

    /* ����ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF */
    CNAS_XSD_SndCas1xCampSyncedChannelNtf();

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);
}


VOS_VOID CNAS_XSD_PerformAnySystemSelectionAfterSyncSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_PRL_1X_SYSTEM_STRU            *pstEmcCallOrigSys   = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enCurSysAcqScene;
    CNAS_PRL_1X_SYSTEM_STRU             stCurSysInfo;

    CNAS_CCB_CARD_STATUS_ENUM_UINT8     enCardStatus;

    enCurSysAcqScene    = CNAS_XSD_GetCurSysAcqScene_SysAcq();
    NAS_MEM_SET_S(&stCurSysInfo, sizeof(stCurSysInfo), 0x00, sizeof(CNAS_PRL_1X_SYSTEM_STRU));

    if (CNAS_XSD_SYS_ACQ_SCENE_EMC_CALLBACK_SYSTEM_LOST == enCurSysAcqScene)
    {
        pstEmcCallOrigSys   = CNAS_XSD_GetEmcCallOriginalSys();

        if ((pstEmcCallOrigSys->usSid != pstSyncCnf->stSyncedSysId.usSid)
         || (pstEmcCallOrigSys->usNid != pstSyncCnf->stSyncedSysId.usNid))
        {
            CNAS_INFO_LOG4(UEPS_PID_XSD,
                           "CNAS_XSD_PerformAnySystemSelectionAfterSyncSucc_SysAcq:CallBack System Lost Srch Not Match(Req/Rsp):",
                           pstEmcCallOrigSys->usSid,
                           pstEmcCallOrigSys->usNid,
                           pstSyncCnf->stSyncedSysId.usSid,
                           pstSyncCnf->stSyncedSysId.usNid);

            /* ͬ���ɹ���ϵͳ������Ҫ�󣬼���Scan Listͬ�� */
            CNAS_XSD_ContinueSysSync_SysAcq();

            return;
        }
    }

    /* �ǽ������ͽ�����CallBack����,��ϵͳΪNegativeϵͳ������ͬ��ScanListʣ��Ƶ�� */
    enCardStatus = CNAS_CCB_GetCsimCardStatus();

    if ((VOS_FALSE == CNAS_XSD_IsEmergencySysAcq_SysAcq(enCurSysAcqScene))
     && (CNAS_CCB_CARD_STATUS_ABSENT != enCardStatus))
    {
        stCurSysInfo.stFreq.enBandClass = pstSyncCnf->stFreq.usBandClass;
        stCurSysInfo.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;
        stCurSysInfo.usNid              = pstSyncCnf->stSyncedSysId.usNid;
        stCurSysInfo.usSid              = pstSyncCnf->stSyncedSysId.usSid;

        if (VOS_TRUE == CNAS_XSD_IsCurSysNotSuitableSys(&stCurSysInfo,
                                                         VOS_FALSE,
                                                         0))
        {
            CNAS_XSD_ContinueSysSync_SysAcq();

            return;
        }
    }

    /* ����ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF */
    CNAS_XSD_SndCas1xCampSyncedChannelNtf();

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);

    return;
}


VOS_VOID CNAS_XSD_PerformPreferedSystemSelectionAfterSyncSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU stGeoListInfo;
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usCurScanListIndex;
    VOS_UINT16                          usDstChanNum;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanList = VOS_NULL_PTR;
    VOS_UINT32                          ulIsNegSys;
    CNAS_PRL_1X_SYSTEM_STRU             stCurSysInfo;
    VOS_UINT32                          ulHrpdConnExistFlag;
    VOS_UINT32                          ulLteConnExistFlag;

    ulLteConnExistFlag              = CNAS_XSD_GetLteConnExistFlag();
    ulHrpdConnExistFlag             = CNAS_XSD_GetHrpdConnExistFlag();

    stCurSysInfo.stFreq.enBandClass = pstSyncCnf->stFreq.usBandClass;
    stCurSysInfo.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;
    stCurSysInfo.usNid              = pstSyncCnf->stSyncedSysId.usNid;
    stCurSysInfo.usSid              = pstSyncCnf->stSyncedSysId.usSid;


    ulIsNegSys = VOS_FALSE;

    NAS_MEM_SET_S(&stGeoListInfo, sizeof(stGeoListInfo), 0, sizeof(CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU));

    CNAS_PRL_Get1xSysGeoList(&stCurSysInfo, &stGeoListInfo);

    ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(&stCurSysInfo, VOS_FALSE, 0);

    /* negative system or not in PRL, could not select,should continue sync */
    if ((VOS_TRUE == ulIsNegSys)
     || (0        == stGeoListInfo.ucGeoNum))
    {
        usCurScanListIndex = CNAS_XSD_GetCurChannelScanIndex();
        pstScanList        = CNAS_XSD_GetScanChanListAddr();

        usDstChanNum       = 0;
        NAS_MEM_SET_S(astDstChan, sizeof(astDstChan), 0x0, sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);
        CNAS_XSD_GetNextScanChanList_SysAcq(usCurScanListIndex,
                                            &usDstChanNum,
                                            astDstChan,
                                            pstScanList);

        /* scan list��ȫ��ͬ���� */
        if (0 == usDstChanNum)
        {
            CNAS_XSD_ProcScanListSyncComplete_SysAcq();
        }
        /* scan listδȫ��ͬ���꣬������ͬ������ */
        else
        {
            /* �����ǰhrpd����������srlte��lte���������˳���ǰ�������̣����½�����������̬�µ�1x���� */
            if ((VOS_TRUE                == ulHrpdConnExistFlag)
             || ((CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
              && (VOS_TRUE               == ulLteConnExistFlag)))
            {
                CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(VOS_FALSE);
                return;
            }
            else
            {
                CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, astDstChan);
            }
        }

        return;
    }

    /* ����ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF */
    CNAS_XSD_SndCas1xCampSyncedChannelNtf();

    /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);
}


VOS_VOID CNAS_XSD_PerformSpecificSystemSelectionAfterSyncSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_XSD_CFREQ_LOCK_SET_PARA_STRU  *pstFreqLockPara;
    VOS_UINT32                          ulRlst;

    /* ��ȡ��Ƶ��Ϣ */
    pstFreqLockPara = CNAS_XSD_GetFreqLockAddr();

    ulRlst = CNAS_PRL_Is1xSysIdMatched(pstSyncCnf->stSyncedSysId.usSid,
                                       pstSyncCnf->stSyncedSysId.usNid,
                                       pstFreqLockPara->usSid,
                                       pstFreqLockPara->usNid);

    if (VOS_TRUE == ulRlst)
    {
        CNAS_XSD_SndCas1xCampSyncedChannelNtf();

        /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
        CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

        CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);
    }
    else
    {
        CNAS_XSD_ProcScanListSyncComplete_SysAcq();
    }

    return;
}


VOS_UINT32 CNAS_XSD_ProcCasSystemDetermineIndWithProtoMisReason_SysAcq(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_1X_SYSTEM_DETERMIN_IND_STRU                   *pstSysDeterminInd = VOS_NULL_PTR;

    pstSysDeterminInd   = (CAS_CNAS_1X_SYSTEM_DETERMIN_IND_STRU *)pstMsg;

    CNAS_XSD_ProcRequiredAvoidedFreqByAvoidReason(CNAS_XSD_AVOID_P_REV_MISMATCH,
                                                  (CNAS_PRL_FREQENCY_CHANNEL_STRU *)(&(pstSysDeterminInd->stCurChannelInfo)));

    CNAS_XSD_SetRedirectionFlag(VOS_FALSE);

    /* continue sync */
    CNAS_XSD_ContinueSysSync_SysAcq();

    return VOS_TRUE;
}



VOS_VOID CNAS_XSD_GetNextScanChanList_SysAcq(
    VOS_UINT16                          usCurScanChanIndex,
    VOS_UINT16                         *pusDstChanNum,
    CNAS_PRL_FREQENCY_CHANNEL_STRU     *pstDstChan,
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanChanList
)
{
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enCurAcqScene;
    VOS_UINT32                          ulIsAvoidListEnable;

    enCurAcqScene       = CNAS_XSD_GetCurSysAcqScene_SysAcq();
    ulIsAvoidListEnable = VOS_TRUE;

    if ((CNAS_XSD_SYS_ACQ_SCENE_EMC_CALLBACK_SYSTEM_LOST == enCurAcqScene)
     || (CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL           == enCurAcqScene)
     || (CNAS_XSD_SYS_ACQ_SCENE_EMC_CALL_RELEASED        == enCurAcqScene))
    {
        ulIsAvoidListEnable = VOS_FALSE;
    }

    CNAS_XSD_GetNextScanChanList(usCurScanChanIndex,
                                 pusDstChanNum,
                                 pstDstChan,
                                 pstScanChanList,
                                 ulIsAvoidListEnable);
}


VOS_UINT32 CNAS_XSD_RcvMsccSrvAcqReq_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �ǽ�����ʹ�õ�ǰ��������� */
    if (NAS_MSCC_PIF_SRV_TYPE_CS_MO_EMERGENCY_CALL != ((MSCC_XSD_SRV_ACQ_REQ_STRU *)pstMsg)->enSrvType)
    {
        return VOS_TRUE;
    }

    /* ��������ȼ���ϵĻ�����Ϣ */
    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_NETWORK_SRCH_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_MODE_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_END_EMC_CALLBACK_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_MO_CALL_END_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ);

    /* ����ǰ���ڸ������ȼ��Ĵ�ϵĻ�����Ϣ�����Ա���Ϣ */
    if (0 != CNAS_XSD_GetCacheMsgNum())
    {
        return VOS_TRUE;
    }

    /* ���ҵ�������ǽ����������������������б� */
    CNAS_XSD_BuildEmcCallRedialScanChanList();

    /* ����ǰ�����ڸ������ȼ��Ļ�����Ϣ�������µĴ�� */
    /* ֹͣͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

    /* ����ֹͣͬ������ */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* �����д������ҵ�񴥷�������� */
    CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_SRV_ACQ);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* Ǩ״̬���ñ�����ʱ�� */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccCallEndNtf_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enAcqScene;

    enAcqScene                          = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    /* �ǽ�������������� */
    if ((VOS_FALSE == CNAS_XSD_IsEmcSDSysAcq_SysAcq(enAcqScene))
     && (CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL != enAcqScene))
    {
        return VOS_TRUE;
    }

    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* �����������У���ϴ��� */
    /* ֹͣͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

    /* ����ֹͣͬ������ */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_MO_CALL_END);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* Ǩ״̬���ñ�����ʱ�� */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiEmcCallBackNetWorkSrchTimerExpired_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* ֹͣͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

    /* ����ֹͣͬ������ */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALLBACK_SRCH_TIMEOUT);

    /* Ǩ״̬���ñ�����ʱ�� */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiEmcCallBackModeProtectTimerExpired_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* ֹͣͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

    /* ����ֹͣͬ������ */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALLBACK_MODE_TIMEOUT);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* Ǩ״̬���ñ�����ʱ�� */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccEndEmcCallBackReq_SysAcq_WaitCasSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* ֹͣͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_SYSTEM_SYNC_CNF);

    /* ����ֹͣͬ������ */
    CNAS_XSD_SndCasStopSysSyncReq();

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_END_EMC_CALLBACK);

    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    /* Ǩ״̬���ñ�����ʱ�� */
    CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_STOP_SYNC_CNF);
    CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF, TI_CNAS_XSD_WAIT_CAS_STOP_SYSTEM_SYNC_CNF_LEN);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvMsccSrvAcqReq_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �ǽ�����ʹ�õ�ǰ��������� */
    if (NAS_MSCC_PIF_SRV_TYPE_CS_MO_EMERGENCY_CALL != ((MSCC_XSD_SRV_ACQ_REQ_STRU *)pstMsg)->enSrvType)
    {
        return VOS_TRUE;
    }

    /* ��������ȼ���ϵĻ�����Ϣ */
    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_NETWORK_SRCH_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_MODE_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_END_EMC_CALLBACK_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_MO_CALL_END_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ);

    /* ����ǰ���ڸ������ȼ��Ĵ�ϵĻ�����Ϣ�����Ա���Ϣ */
    if (0 != CNAS_XSD_GetCacheMsgNum())
    {
        return VOS_TRUE;
    }

    /* ���ҵ�������ǽ����������������������б� */
    CNAS_XSD_BuildEmcCallRedialScanChanList();

    /* ����ǰ�����ڸ������ȼ��Ļ�����Ϣ�������µĴ�� */
    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* �����д�����Ϣ�����ҵ�񴥷�������� */
    CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_SRV_ACQ);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccCallEndNtf_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enAcqScene;

    enAcqScene                          = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    /* �ǽ�������������� */
    if ((VOS_FALSE == CNAS_XSD_IsEmcSDSysAcq_SysAcq(enAcqScene))
     && (CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL != enAcqScene))
    {
        return VOS_TRUE;
    }

    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* �����������У���ϴ��� */
    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_MO_CALL_END);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiEmcCallBackNetWorkSrchTimerExpired_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALLBACK_SRCH_TIMEOUT);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiEmcCallBackModeProtectTimerExpired_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALLBACK_MODE_TIMEOUT);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccEndEmcCallBackReq_SysAcq_WaitCasStopSysSyncCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_END_EMC_CALLBACK);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvMsccSrvAcqReq_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �ǽ�����ʹ�õ�ǰ��������� */
    if (NAS_MSCC_PIF_SRV_TYPE_CS_MO_EMERGENCY_CALL != ((MSCC_XSD_SRV_ACQ_REQ_STRU *)pstMsg)->enSrvType)
    {
        return VOS_TRUE;
    }

    /* ��������ȼ���ϵĻ�����Ϣ */
    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_NETWORK_SRCH_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(VOS_PID_TIMER, TI_CNAS_XSD_EMC_CALLBACK_MODE_PROTECT_TIMER);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_END_EMC_CALLBACK_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_MO_CALL_END_NTF);

    (VOS_VOID)CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ);

    /* ����ǰ���ڸ������ȼ��Ĵ�ϵĻ�����Ϣ�����Ա���Ϣ */
    if (0 != CNAS_XSD_GetCacheMsgNum())
    {
        return VOS_TRUE;
    }

    /* ���ҵ�������ǽ����������������������б� */
    CNAS_XSD_BuildEmcCallRedialScanChanList();

    /* ����ǰ�����ڸ������ȼ��Ļ�����Ϣ�������µĴ�� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* �����д�����Ϣ�����ҵ�񴥷�������� */
    CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_SRV_ACQ);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccCallEndNtf_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_PRL_1X_SYSTEM_ID_STRU         *pstLastSyncedSys       = VOS_NULL_PTR;
    CNAS_PRL_1X_SYSTEM_STRU             st1xSysInfo;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enAcqScene;

    enAcqScene                          = CNAS_XSD_GetCurSysAcqScene_SysAcq();
    NAS_MEM_SET_S(&st1xSysInfo, sizeof(st1xSysInfo), 0x00, sizeof(CNAS_PRL_1X_SYSTEM_STRU));
    pstLastSyncedSys                    = CNAS_XSD_GetLastSyncedSys();

    /* �ǽ�������������� */
    if ((VOS_FALSE == CNAS_XSD_IsEmcSDSysAcq_SysAcq(enAcqScene))
     && (CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL != enAcqScene))

    {
        return VOS_TRUE;
    }

    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    st1xSysInfo.usSid                   = pstLastSyncedSys->usSid;
    st1xSysInfo.usNid                   = pstLastSyncedSys->usNid;
    st1xSysInfo.stFreq                  = CNAS_XSD_GetHistorySyncedSysFreqList()->astFreq[0];

    /* �����������У���ϴ��� */
    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* ����ǰϵͳ��Ч���ӳٴ�� */
    if (VOS_TRUE != CNAS_XSD_IsCurSysNotSuitableSys(&st1xSysInfo, VOS_FALSE, 0))
    {
        CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_MO_CALL_END);

        /* ����״̬����ϱ�� */
        CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

        return VOS_TRUE;
    }

    CNAS_XSD_StopTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND);

    CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_ABORTED,
                           CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                           CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                           CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiEmcCallBackNetWorkSrchTimerExpired_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALLBACK_SRCH_TIMEOUT);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiEmcCallBackModeProtectTimerExpired_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALLBACK_MODE_TIMEOUT);

    /* ����ҵ�񴥷��������� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccEndEmcCallBackReq_SysAcq_WaitCasOhmInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == CNAS_XSD_GetAbortFlg_SysAcq())
    {
        return VOS_TRUE;
    }

    /* �����˳�CallBackģʽ���� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_END_EMC_CALLBACK);

    /* ����״̬����ϱ�� */
    CNAS_XSD_SetAbortFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}


VOS_VOID CNAS_XSD_UpdateScanListChanStatusUponReceivedDeterminInd(
    VOS_UINT16                                              usBeginScanListIndex,
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstFreq,
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanChanList
)
{
    VOS_UINT16                          usCurFreqIndex;

    usCurFreqIndex = usBeginScanListIndex;

    if (usCurFreqIndex > 0)
    {
        usCurFreqIndex--;
    }

    /* �������OOC�����µ�MRU0����Ƶ�㣬��scan list��ǰ������ʼ���ң�ֱ���ҵ���Ƶ��ˢ��Ϊͬ�������� */
    if (VOS_FALSE == CNAS_XSD_IsMru0FreqInOocScene(pstFreq))
    {
        CNAS_XSD_UpdateChanStatusInScanChanList(usCurFreqIndex,
                                                pstFreq,
                                                CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                pstScanChanList);

        return;
    }

    /* �����OOC�����µ�MRU0����Ƶ�㣬ֻ�ܴ�scan list��ʼλ��0����ǰ�������ң�������Ƶ�㲻Ҫˢ�� */
    CNAS_XSD_UpdateFreqStatusBeforeCurIndexInScanChanList(CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                          usCurFreqIndex,
                                                          pstFreq,
                                                          pstScanChanList);

    return;
}


VOS_VOID CNAS_XSD_UpdateGeoSysRecordStatusUponReceivedDeterminInd(
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstFreq
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;

    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    /* TO DO: ����ԭ������Ƿ���Ҫ���֣����� */
    CNAS_XSD_UpdateSysFreqStatusByFreqChanInGeoSysRecordSrchList(pstFreq,
                                                                 CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                                 pstGeoSysRecSrchList);

    return;
}


VOS_UINT32 CNAS_XSD_IsCurGeoListSrchedInGeoListSrchInfo(
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                    *pstGeoListInfo
)
{
    CNAS_XSD_GEO_LIST_SRCH_INFO_STRU   *pstGeoListSrchInfo = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          j;

    pstGeoListSrchInfo = CNAS_XSD_GetGeoSrchListInfoAddr();

    for (i = 0; i< pstGeoListInfo->ucGeoNum; i++)
    {
        for (j = 0; j < pstGeoListSrchInfo->usGeoNum; j++)
        {
            if (pstGeoListInfo->astGeoInfoList[i].usGeoFirstSysRecIndex == pstGeoListSrchInfo->pstGeoSrchInfo[j].usGeoBeginIndex)
            {
                /* ֻҪ�����κ�һ��GEOδ������������Ϊδ������ */
                if (VOS_FALSE == pstGeoListSrchInfo->pstGeoSrchInfo[j].usGeoSrchStatus)
                {
                    return VOS_FALSE;
                }
            }
        }
    }

    return VOS_TRUE;
}


VOS_VOID CNAS_XSD_GetMostMatched1xGeoList(
    CNAS_PRL_1X_SYSTEM_STRU                                *pstCurSysInfo,
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                    *pstMostMatchGeoListInfo
)
{
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                     stGeoListInfo;

    /* ��ȡ����ƥ���GEO list */
    NAS_MEM_SET_S(&stGeoListInfo, sizeof(stGeoListInfo), 0, sizeof(CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU));

    CNAS_PRL_Get1xSysGeoList(pstCurSysInfo, &stGeoListInfo);

    /* ��GEO list����match��GEO list */
    CNAS_PRL_GetAllMostMatched1xGeoFrom1xGeoList(pstCurSysInfo, &stGeoListInfo, pstMostMatchGeoListInfo);

    return;
}


VOS_VOID CNAS_XSD_ConstructGeoSysRecordSrchList(
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                    *pstGeoListInfo,
    CNAS_PRL_1X_SYSTEM_STRU                                *pstCurSysInfo,
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList
)
{
    CNAS_PRL_EXT_SYS_RECORD_STRU       *pstSysRecord        = VOS_NULL_PTR;
    VOS_UINT16                          usMorePrefSysNum;
    VOS_UINT16                          usMaxSysRecNum;
    VOS_UINT16                          usSysRecNum;
    VOS_UINT32                          i;
    CNAS_PRL_EXT_SYS_RECORD_STRU       *pstPrlSysInfo   = VOS_NULL_PTR;
    CNAS_PRL_SYS_ROAMING_IND_ENUM_UINT8                     enCurRoamingInd;

    pstPrlSysInfo                       = CNAS_PRL_GetPrlSysInfoAddr()->pstSysRecord;
    enCurRoamingInd                     = CNAS_PRL_SYS_ROAMING_STATUS_ON;

    usSysRecNum        = 0;
    usMorePrefSysNum = 0;
    usMaxSysRecNum     = 0;

    /* ��������GEO�б��а�����ϵͳ��¼���� */
    for (i = 0; i < pstGeoListInfo->ucGeoNum; i++)
    {
        usMaxSysRecNum += pstGeoListInfo->astGeoInfoList[i].usGeoSysRecNum;

        if (CNAS_PRL_SYS_ROAMING_STATUS_OFF == (pstPrlSysInfo + pstGeoListInfo->astGeoInfoList[i].usMostMatchedIndex)->enRoamingInd)
        {
            enCurRoamingInd = CNAS_PRL_SYS_ROAMING_STATUS_OFF;
        }
    }

    /* �����ڴ�ռ� */
    pstSysRecord = (CNAS_PRL_EXT_SYS_RECORD_STRU *)PS_MEM_ALLOC(UEPS_PID_XSD,
                                                                usMaxSysRecNum * sizeof(CNAS_PRL_EXT_SYS_RECORD_STRU));

    if (VOS_NULL_PTR == pstSysRecord)
    {
        return;
    }

    CNAS_XSD_BuildCompsiteGeoFromMostMatchedGeoList(pstGeoListInfo,
                                                    &usSysRecNum,
                                                    pstSysRecord,
                                                    &usMorePrefSysNum);

    if (VOS_FALSE == CNAS_XSD_GetIsConsider1xRoamIndInSysDeterminationFlg())
    {
        /* ����ϵͳ�����б� */
        CNAS_XSD_BuildGeoSysRecordSrchList(pstCurSysInfo,
                                           usSysRecNum,
                                           pstSysRecord,
                                           pstGeoSysRecSrchList,
                                           usMorePrefSysNum);
    }
    else
    {
        CNAS_XSD_BuildGeoSysRecordSrchListWithRoamIndStrategy(pstCurSysInfo,
                                                              usSysRecNum,
                                                              pstSysRecord,
                                                              pstGeoSysRecSrchList,
                                                              usMorePrefSysNum,
                                                              enCurRoamingInd);
    }

    /* ��¼����GEOϵͳ��¼�����б�������Щ���ٸ�GEO��ɼ����ǵ��׸����� */
    if (VOS_NULL_PTR != pstGeoSysRecSrchList->pstAcqSysInfo)
    {

        /* ��ǰ������������ΪGEOϵͳ��¼�����б����׸�ϵͳ��¼���� */
        pstGeoSysRecSrchList->usCurSrchLevel            = pstGeoSysRecSrchList->pstAcqSysInfo[0].usLevel;

        pstGeoSysRecSrchList->stCurSyncGeoInfo.usGeoNum = pstGeoListInfo->ucGeoNum;

        for (i = 0; i < pstGeoListInfo->ucGeoNum; i++)
        {
            pstGeoSysRecSrchList->stCurSyncGeoInfo.ausGeoFirstIndex[i] = pstGeoListInfo->astGeoInfoList[i].usGeoFirstSysRecIndex;
        }
    }

    /* ��Ҫ�ͷ�CNAS_PRL_BuildGeoSysRecList�й����system record */
    PS_MEM_FREE(UEPS_PID_XSD, pstSysRecord);
}


VOS_VOID CNAS_XSD_ContinueGeoSysRecordSrch(VOS_VOID)
{
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanList = VOS_NULL_PTR;
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usDstChanNum;

    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enCurAcqScene;
    VOS_UINT32                          ulIsCallAbort;

    ulIsCallAbort = VOS_FALSE;

    /*******************************************************************************************
     * 1. ����GEOϵͳ��¼Ƶ���б�����
     ******************************************************************************************/
    usDstChanNum = 0;

    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    CNAS_XSD_GetNextSysSyncListFromGeoSysRecordSrchList(CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq(),
                                                        &usDstChanNum,
                                                        &astDstChan[0]);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_TRUE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* ����GEOϵͳ��¼Ƶ��ͬ������ */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
        }

        return;
    }

    /*******************************************************************************************
     * 2. ���GEO�����б�״̬�Լ��ͷ�GEOϵͳ��¼�����б��ڴ�
     ******************************************************************************************/
    /* �����GEO�����б�״̬ */
    CNAS_XSD_ClearGeoSrchListStatus();

    /* �ͷ�GEOϵͳ��¼�����б���ڴ� */
    if (VOS_NULL_PTR != CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo)
    {
        PS_MEM_FREE(UEPS_PID_XSD, CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo);
        CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo = VOS_NULL_PTR;
    }

    /*******************************************************************************************
     * 3. GEO������������ͬ��ʧ�ܣ���ϵ�ǰ�������̡�
       a)���֮ǰ���յ������ز�������
       b)��ǰ������������CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN���ҵ�ǰ��LTE\HRPD����̬
     ******************************************************************************************/
    enCurAcqScene       = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    if (VOS_TRUE == CNAS_XSD_IsAllowedPerformAbortProcedure_SysAcq(enCurAcqScene, &ulIsCallAbort))
    {
        CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(ulIsCallAbort);
        return;
    }

    /*******************************************************************************************
     * 4. ����scan listƵ���б�����
     ******************************************************************************************/
    pstScanList        = CNAS_XSD_GetScanChanListAddr();

    CNAS_XSD_GetNextScanChanList_SysAcq(CNAS_XSD_GetCurChannelScanIndex(),
                                        &usDstChanNum,
                                        astDstChan,
                                        pstScanList);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* scan listδȫ��ͬ���꣬������ͬ������ */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, astDstChan);
        }

        return;
    }

    /*******************************************************************************************
     * 5. ����scan listͬ����ɵĺ�������
     ******************************************************************************************/
    /* scan list��ȫ��ͬ���� */
    CNAS_XSD_ProcScanListSyncComplete_SysAcq();

    return;
}


VOS_VOID CNAS_XSD_ContinueAvailSysListSrch(VOS_VOID)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usDstChanNum;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enScanScene;

    enScanScene   = CNAS_XSD_GetCurSysAcqScene_SysAcq();

    usDstChanNum = 0;

    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    /***********************************************************************************************
     * 1. ����available listͬ��
     **********************************************************************************************/
    /* ��ȡ�´�ͬ��availableϵͳ */
    CNAS_XSD_GetNextAvailSysAcqList_SysAcq(&usDstChanNum, &astDstChan[0]);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* ����avoid system listƵ��ͬ�� */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
        }

        return;
    }

    /***********************************************************************************************
     * 2. ��availableϵͳ����ͬ��Avoidϵͳ
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_IsAcquireAvoidSysNeeded(enScanScene))
    {
        CNAS_XSD_SetAvailSysAcqListFlg_SysAcq(VOS_FALSE);

        /* ����ͬ������ͬ��availableϵͳ */
        NAS_MEM_SET_S(&astDstChan[0],
                      sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                      0x00,
                      sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

        CNAS_XSD_GetNextAvoidSysAcqList_SysAcq(&usDstChanNum, &astDstChan[0]);

        if (0 != usDstChanNum)
        {
            CNAS_XSD_SetAvoidSysAcqListFlg_SysAcq(VOS_TRUE);

            /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
            if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
            {
                /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
                CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
                CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
                CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                    CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
            }
            else
            {
                CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
            }

            return;
        }
    }

    /***********************************************************************************************
     * 3. available system list��û����Ҫͬ����Ƶ�㣬�˳�L2״̬��
     **********************************************************************************************/
    /* �˳�״̬��ʱ���available list */
    CNAS_XSD_InitAvailSysList(CNAS_XSD_INIT_CTX_STARTUP, CNAS_XSD_GetAvailSysFreqListAddr());

    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_FAILURE,
                           CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                           CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                           CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();

}


VOS_VOID CNAS_XSD_ContinueAvoidSysListSrch(VOS_VOID)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usDstChanNum;

    usDstChanNum = 0;

    NAS_MEM_SET_S(&astDstChan[0],
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM,
                  0x00,
                  sizeof(CNAS_PRL_FREQENCY_CHANNEL_STRU) * CNAS_CAS_1X_MAX_FREQ_NUM);

    /***********************************************************************************************
     * 1. ����avoid listͬ��
     **********************************************************************************************/
    /* ��ȡ�´�ͬ��availableϵͳ */
    CNAS_XSD_GetNextAvoidSysAcqList_SysAcq(&usDstChanNum, &astDstChan[0]);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* ����avoid system listƵ��ͬ�� */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
        }

        return;
    }

    /***********************************************************************************************
     * 2. avoid system list��û����Ҫͬ����Ƶ�㣬�˳�L2״̬��
     **********************************************************************************************/
    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_FAILURE,
                           CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                           CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                           CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

    /* �˳����״̬�� */
    CNAS_XSD_QuitFsmSysAcq_SysAcq();
}


VOS_UINT32 CNAS_XSD_IsCurrSysMostPref1xSysInGeoSysRecordSrchList(
    CNAS_PRL_1X_SYSTEM_STRU                                *pstCurSysInfo,
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList
)
{
    VOS_UINT16                          i;
    VOS_UINT8                           ucIsAllSysRoam;

    ucIsAllSysRoam                      = VOS_TRUE;

    /***********************************************************************************************
     * ע: �ú������治��Ҫ���ж��Ƿ���negativeϵͳ�ˣ���Ϊ�ڵ��øú���ǰ���Ѿ��ж���ϵͳ�Ƿ���
     *     negativeϵͳ��
     *     ���øú���ǰ��������ǰcheckϵͳ�Ƿ���negativeϵͳ������negativeϵͳ���ܻᱻѡ��!!!
     **********************************************************************************************/

#if 0
    /* ��ǰϵͳ��SID����SID�������У�����Ϊ�����ȵ�ϵͳ */
    if (VOS_FALSE == CNAS_XSD_IsSidInOperLockWhiteSidList(pstCurSysInfo->usSid))
    {
        return VOS_FALSE;
    }
#endif

    /* �����ǰͬ������ϵͳ�뵱ǰlevel�е�ϵͳƥ�䣬�����פ�� */
    for (i = 0; i < pstGeoSysRecSrchList->usTotalNum; i++ )
    {
        /* �����뵱ǰͬ����level��ͬ��ϵͳ */
        if (pstGeoSysRecSrchList->usCurSrchLevel != pstGeoSysRecSrchList->pstAcqSysInfo[i].usLevel)
        {
            continue;
        }

        /* ����ͬ�������ڵ�Ƶ�� */
        if (CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST == pstGeoSysRecSrchList->pstAcqSysInfo[i].enSysStatus)
        {
            continue;
        }

        if (CNAS_PRL_SYS_ROAMING_STATUS_OFF == pstGeoSysRecSrchList->pstAcqSysInfo[i].enRoamingInd)
        {
            ucIsAllSysRoam = VOS_FALSE;
        }

        /* ������ǰƵ����ϵͳSID/NID��ƥ���ϵͳ */
        if (VOS_FALSE == CNAS_PRL_Is1xSysMatched(pstCurSysInfo,
                                                 &(pstGeoSysRecSrchList->pstAcqSysInfo[i].stAcqSys)))
        {
            continue;
        }

#if 0
        /* ��ǰϵͳ�����ȵ�ϵͳ */
        if (CNAS_PRL_PREF_NEG_SYS_PREF == pstGeoSysRecSrchList->pstAcqSysInfo[i].enPrefNegSys)
        {
            return VOS_TRUE;
        }

        /* �����negativeϵͳ������home SID/NID�б��У���Ȼ������һ��prefϵͳ���� */
        if (VOS_TRUE == CNAS_PRL_IsCurrentSystemInHomeSidNidList(pstCurSysInfo))
        {
            return VOS_TRUE;
        }
#endif

        /* ������ROAM IND�ĳ�����ֱ����Ϊ������ϵͳ */
        if (VOS_FALSE == CNAS_XSD_GetIsConsider1xRoamIndInSysDeterminationFlg())
        {
            return VOS_TRUE;
        }

        /* ��ƥ�䵽���ε�ϵͳ���жϸ������ȼ���ϵͳ���Ƿ��з����Σ�
           �����ڣ�����ͬ��
           �������ڣ�ֱ��פ�� */
        if (CNAS_PRL_SYS_ROAMING_STATUS_OFF != pstGeoSysRecSrchList->pstAcqSysInfo[i].enRoamingInd)
        {
            if (VOS_FALSE == ucIsAllSysRoam)
            {
                return VOS_FALSE;
            }
        }

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 CNAS_XSD_IsCurSysInGeoSysRecordSrchList(
    CNAS_PRL_1X_SYSTEM_STRU                                *pstCurSysInfo,
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList
)
{
    VOS_UINT16                          i;

    for (i = 0; i < pstGeoSysRecSrchList->usTotalNum; i++ )
    {
        /* ������ǰƵ����ϵͳSID/NID��ƥ���ϵͳ */
        if (VOS_TRUE == CNAS_PRL_Is1xSysMatched(pstCurSysInfo,
                                                &(pstGeoSysRecSrchList->pstAcqSysInfo[i].stAcqSys)))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 CNAS_XSD_IsCurSysGeoListSameAsCurSyncGeoInfo(
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                    *pstGeoListInfo,
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList
)
{
    VOS_UINT32                          i;

    if (pstGeoListInfo->ucGeoNum != pstGeoSysRecSrchList->stCurSyncGeoInfo.usGeoNum)
    {
        return VOS_FALSE;
    }

    for (i = 0; i < pstGeoListInfo->ucGeoNum; i++)
    {
        if (pstGeoListInfo->astGeoInfoList[i].usGeoFirstSysRecIndex != pstGeoSysRecSrchList->stCurSyncGeoInfo.ausGeoFirstIndex[i])
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_VOID CNAS_XSD_PreformBestPreferedSystemSelectionAndNoGeoSysRecordSearchList_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU                          astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                                              usDstChanNum;
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                     stGeoListInfo = {0};
    VOS_UINT16                                              usCurScanListIndex;
    CNAS_PRL_1X_SYSTEM_STRU                                 stCurSysInfo;
    VOS_UINT32                                              ulIsNegSys;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList = VOS_NULL_PTR;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32                      enCurAcqScene;
    VOS_UINT32                                              ulIsCallAbort;
    VOS_UINT8                                               ucNextAcqIndex;

    ucNextAcqIndex                                          = 0;
    enCurAcqScene = CNAS_XSD_GetCurSysAcqScene_SysAcq();
    ulIsCallAbort = VOS_FALSE;

    /***********************************************************************************************
     * 1. ˢ��availableϵͳ�´�������������֪ͨ�����פ����ǰͬ������ϵͳ
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_GetAvailSysAcqListFlg_SysAcq())
    {
        usCurScanListIndex = CNAS_XSD_GetAvailSysFreqListNextSrchIndex();

        /* ����ʧ�ܵ�Ƶ������Լ���ǰ�ɹ���Ƶ����� */
        CNAS_XSD_SetAvailSysFeqListNextSrchIndex((VOS_UINT8)(usCurScanListIndex + pstSyncCnf->ulSyncFailFreqNum + 1));

        /* �����ϵͳ��SID�������У������ֱ��פ�� */
        if (VOS_TRUE == CNAS_XSD_IsSidInOperLockSysWhiteList(pstSyncCnf->stSyncedSysId.usSid))
        {
            /* ����ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF */
            CNAS_XSD_SndCas1xCampSyncedChannelNtf();

            /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

            /* ����������ʱ�� */
            CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);

            return;
        }
       /***********************************************************************************************
         * ��ǰ�Ƿ�GEO������ͬ���ɹ���������פ������������������һ�������˳���ǰ����
            a)�����ز������б�Ϊ�գ�����������LTE\HRPD����̬�µ�1X����
            b)LTE\HRPD��idle̬������̬��
         **********************************************************************************************/
        if (VOS_TRUE == CNAS_XSD_IsAllowedPerformAbortProcedure_SysAcq(enCurAcqScene, &ulIsCallAbort))
        {
            CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(ulIsCallAbort);

            return;
        }

        /* ����availableϵͳ���� */
        CNAS_XSD_ContinueAvailSysListSrch();

        return;
    }

    if (VOS_TRUE == CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
    {
        /* �����ϵͳSID�ǹ��ڵ�ϵͳ����ֱ���˳�����OOC״̬�������������ͬ��ʣ���AvoidƵ����Ϣ */
        if (CNAS_CCB_NETWORK_PROPERTY_NATIONAL == CNAS_CCB_IsCur1XCampOnNationalNetWork())
        {
            /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
            CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_FAILURE,
                                   CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                                   CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                                   CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

            /* �˳����״̬�� */
            CNAS_XSD_QuitFsmSysAcq_SysAcq();

            return;
        }

        usCurScanListIndex = 0;

        (VOS_VOID)CNAS_XSD_GetAvoidFreqIndexOfAvoidlist((CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncCnf->stFreq),
                                                        (VOS_UINT8 *)&usCurScanListIndex);

        ucNextAcqIndex = CNAS_XSD_GetAvoidFreqListAddr()->ucNextAcqIndex;

        usCurScanListIndex = (VOS_UINT16)CNAS_MAX(ucNextAcqIndex, usCurScanListIndex);

        /* ����ʧ�ܵ�Ƶ������Լ���ǰ�ɹ���Ƶ����� */
        CNAS_XSD_SetAvoidSysFeqListNextSrchIndex((VOS_UINT8)(usCurScanListIndex + 1));

        /***********************************************************************************************
          * ��ǰ�Ƿ�GEO������ͬ���ɹ�����������������������������һ�������˳���ǰ����
             a)�����ز������б�Ϊ�գ�����������LTE\HRPD����̬�µ�1X����
             b)LTE\HRPD��idle̬������̬��
          **********************************************************************************************/
         if (VOS_TRUE == CNAS_XSD_IsAllowedPerformAbortProcedure_SysAcq(enCurAcqScene, &ulIsCallAbort))
         {
             CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(ulIsCallAbort);

             return;
         }

        /* ����avoidϵͳ���� */
        CNAS_XSD_ContinueAvoidSysListSrch();

        return;
    }

    /***********************************************************************************************
     * 2. ��ǰϵͳ��PRL���д���
     **********************************************************************************************/
    stCurSysInfo.stFreq.enBandClass = pstSyncCnf->stFreq.usBandClass;
    stCurSysInfo.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;
    stCurSysInfo.usNid              = pstSyncCnf->stSyncedSysId.usNid;
    stCurSysInfo.usSid              = pstSyncCnf->stSyncedSysId.usSid;

    NAS_MEM_SET_S(&stGeoListInfo, sizeof(stGeoListInfo), 0, sizeof(CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU));

    CNAS_XSD_GetMostMatched1xGeoList(&stCurSysInfo, &stGeoListInfo);

    if (0 != stGeoListInfo.ucGeoNum)
    {

        ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(&stCurSysInfo, VOS_FALSE, 0);

        if (VOS_TRUE == ulIsNegSys)
        {
            CNAS_WARNING_LOG(UEPS_PID_XSD,
                             "CNAS_XSD_PreformBestPreferedSystemSelectionAndNoGeoSysRecordSearchList_SysAcq: Curr System is Neg!")
        }

        /* ʹ��һ���µ�GEO�������ŵ�ϵͳѡ�� */
        CNAS_XSD_PeformBestPreferedSystemSelectionUsingNewGeo_SysAcq(&stGeoListInfo,
                                                                     &stCurSysInfo,
                                                                     ulIsNegSys);

        return;
    }

    /***********************************************************************************************
     * 3. �����ǰϵͳ����PRL���У���ϵͳ��homeϵͳ���򲶻��ϵͳ
     **********************************************************************************************/
    /* HOME SID/NID �б�Ӱ��ϵͳפ�����ж� */
    /* ����PRL���У���ʹ��HOME SID/NID�б��У�Ҳ������ֱ��פ�� */

    /***********************************************************************************************
     * 4. ͬ������ϵͳ��availableϵͳ�������scan list�ж�ӦƵ��״̬�ٴ�ˢΪ�����ڣ�������������ͬ��
     **********************************************************************************************/
    pstScanList = CNAS_XSD_GetScanChanListAddr();

    usCurScanListIndex = pstScanList->usCurScanIndex;

    CNAS_XSD_UpdateAvailOrNegSystemChanStatusInScanChanList_SysAcq((CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncCnf->stFreq),
                                                                   pstScanList);

    /***********************************************************************************************
     * 5. �����ǰϵͳ��availableϵͳ���ϱ�������Ϣ��ͬʱˢ��available list
     **********************************************************************************************/
    if (VOS_FALSE == CNAS_PRL_GetPrefOnlyFlg())
    {
        /* check��ǰavailableϵͳ�Ƿ��ڰ��������ڰ������У�����ӵ�availableϵͳ�У��ϱ�������� */
        if (VOS_TRUE == CNAS_XSD_IsSidInOperLockSysWhiteList(pstSyncCnf->stSyncedSysId.usSid))
        {
             /* ֪ͨ����ɻ��ָʾ */
            CNAS_XSD_ReportSrvAvailableInd_SysAcq(&stCurSysInfo);

            /* ˢ��available list */
            CNAS_XSD_AddAvailSysFreqList(&(stCurSysInfo.stFreq));
        }
    }

    /***********************************************************************************************
     * 6. ��ǰ�Ƿ�GEO������ͬ���ɹ�����������������������������һ�������˳���ǰ����
        a)�����ز������б�Ϊ�գ�����������LTE\HRPD����̬�µ�1X����
        b)LTE\HRPD��idle̬������̬��
     **********************************************************************************************/
    if (CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
    {
        if (VOS_TRUE == CNAS_XSD_IsAllowedPerformAbortProcedure_SysAcq(enCurAcqScene, &ulIsCallAbort))
        {
            CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(ulIsCallAbort);

            return;
        }
    }

    /***********************************************************************************************
     * 7. ����scan listƵ���б�����
     **********************************************************************************************/
    usDstChanNum       = 0;

    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    CNAS_XSD_GetNextScanChanList_SysAcq(usCurScanListIndex, &usDstChanNum, astDstChan, pstScanList);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* ����scan listͬ������ */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, astDstChan);
        }

        return;
    }

    /*******************************************************************************************
     * 8. ����scan listͬ����ɵĺ�������
     ******************************************************************************************/
    CNAS_XSD_ProcScanListSyncComplete_SysAcq();

    return;
}


VOS_VOID CNAS_XSD_PreformBestPreferedSystemSelectionAndGeoSysRecordSearchListExist_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU                     stGeoListInfo = {0};
    CNAS_PRL_1X_SYSTEM_STRU                                 stCurSysInfo;
    VOS_UINT32                                              ulIsNegSys;
    CNAS_CAS_1X_SYNC_RSLT_INFO_STRU                         stSyncFailChan;
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;

    /***********************************************************************************************
     * 1. ˢ��GEOϵͳ��¼�����б�
     *    ע: negativeϵͳ����Ҫˢ��scan list�е�Ƶ�㣬��Ϊnegativeϵͳ�Ѿ���֮ǰ�ӵ�avoid�б����ˣ�
     *        ����һ��ͬ������ʱ���ᱻ���˵�
     **********************************************************************************************/
    stCurSysInfo.stFreq.enBandClass = pstSyncCnf->stFreq.usBandClass;
    stCurSysInfo.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;
    stCurSysInfo.usNid              = pstSyncCnf->stSyncedSysId.usNid;
    stCurSysInfo.usSid              = pstSyncCnf->stSyncedSysId.usSid;

    ulIsNegSys = CNAS_XSD_IsCurSysNotSuitableSys(&stCurSysInfo, VOS_FALSE, 0);

    if (VOS_FALSE == ulIsNegSys)
    {
        /* ˢ�µ�ǰͬ���ɹ���Ƶ��״̬��GEO��ϵͳ��¼�����б�����match��ϵͳƵ��ˢ��Ϊ�������ڣ�����Ϊ���������� */
        CNAS_XSD_UpdateSyncSuccFreqStatusInGeoSysRecordSrchList_SysAcq(&(pstSyncCnf->stFreq),
                                                                       &(pstSyncCnf->stSyncedSysId));
    }
    else
    {
        stSyncFailChan.stFreq.usBandClass = pstSyncCnf->stFreq.usBandClass;
        stSyncFailChan.stFreq.usChannel   = pstSyncCnf->stFreq.usChannel;

        /* ��ǰϵͳ��negativeϵͳ��ˢ��״̬Ϊ���������� */
        CNAS_XSD_UpdateSyncFailFreqStatusInGeoSysRecordSrchList_SysAcq(1, &stSyncFailChan);

        /* ����ϵͳ��negʱ����ˢ�¸�Ƶ��Ϊ�����ڣ������ˢ�µĻ�������ͬ����Ȼ��ͬ����Ƶ��
           ��ȡ��һ��ͬ��Ƶ��ʱ���߼�Ϊδͬ������ͬ������ */
        CNAS_XSD_UpdateAvailOrNegSystemChanStatusInScanChanList_SysAcq(&stCurSysInfo.stFreq,
                                                                       CNAS_XSD_GetScanChanListAddr());
    }

    /* ˢ�µ�ǰͬ��ʧ�ܵ�Ƶ�� */
    CNAS_XSD_UpdateSyncFailFreqStatusInGeoSysRecordSrchList_SysAcq((VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum,
                                                                   &(pstSyncCnf->astSyncRsltList[0]));

    /***********************************************************************************************
     * 2. ��ǰϵͳ��PRL���д���
     **********************************************************************************************/
    NAS_MEM_SET_S(&stGeoListInfo, sizeof(stGeoListInfo), 0, sizeof(CNAS_PRL_MATCHED_GEO_LIST_INFO_STRU));

    CNAS_XSD_GetMostMatched1xGeoList(&stCurSysInfo, &stGeoListInfo);

    if (0 != stGeoListInfo.ucGeoNum)
    {
        /***********************************************************************************************
         * 1.1. ��ǰϵͳ��GEOϵͳ�����б��д���
         **********************************************************************************************/
        pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

        if (VOS_TRUE == CNAS_XSD_IsCurSysInGeoSysRecordSrchList(&stCurSysInfo, pstGeoSysRecSrchList))
        {
            /***************************************************************************************
             * i ��ǰϵͳ����ֱ��פ������
             **************************************************************************************/
            /* ����scan list���б��е�Ƶ��״̬��ˢ��GEOϵͳ��¼�����б��е�Ƶ��״̬���´��������� */
            CNAS_XSD_UpdateGeoSysRecordListInfoAccordingToScanChanList(pstGeoSysRecSrchList);

            /* ��ǰϵͳ�Ǹ����ȼ�ϵͳ */
            if (VOS_TRUE == CNAS_XSD_IsCurrSysMostPref1xSysInGeoSysRecordSrchList(&stCurSysInfo, pstGeoSysRecSrchList))
            {
                /* ����ǰ֪ͨ�ϲ����״̬ */
                CNAS_XSD_ReportSrvAvailableInd_SysAcq(&stCurSysInfo);

                /* ����ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF */
                CNAS_XSD_SndCas1xCampSyncedChannelNtf();

                /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
                CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

                /* ����������ʱ�� */
                CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);

                return;
            }

            /***************************************************************************************
             * ii ����GEOϵͳ��¼�����б�����
             **************************************************************************************/
            CNAS_XSD_ContinueGeoSysRecordSrch();
            return;
        }

        /*******************************************************************************************
         * 1.2 ��ǰͬ������ϵͳ��GEOϵͳ�����б��еĵ����ȼ�ϵͳ, ����GEOϵͳ��¼�����б�����
         *     ע: GEOϵͳ��¼�����б��ڹ���ʱ��������˵�ǰͬ����ϵͳ�Լ���GEO�б������ȼ��ߵ�ϵͳ��
                   �����ȼ�ϵͳ��û�зŵ�GEOϵͳ�����б��С�������жϵ�ǰϵͳ�Ƿ���GEOϵͳ��¼����
                   �б��еĵ����ȼ�ϵͳ��ԭ������:
                   1. ��ǰͬ������ϵͳ����GEOϵͳ��¼�����б��У���
                   2. ��ǰͬ������ϵͳ��PRL���е�GEO�뵱ǰ����ͬ����GEO��Ϣһ��
                   ������������������Ϊ��GEO�е����ȼ�ϵͳ�����򽫼�����ǰ��GEOϵͳ��¼�����б��и�
                   ����Ƶ��ͬ��
         *******************************************************************************************/
        if (VOS_TRUE == CNAS_XSD_IsCurSysGeoListSameAsCurSyncGeoInfo(&stGeoListInfo, pstGeoSysRecSrchList))
        {
            CNAS_XSD_ContinueGeoSysRecordSrch();

            return;
        }

        /*******************************************************************************************
         * 1.3 ��ǰϵͳ���µ�GEO�����Ѿ�������,��������:
         *     i)  �����negativeϵͳ����ǰ����GEO����������
         *     ii) ֱ��פ����ǰϵͳ
         *******************************************************************************************/
        /* ������Ǹ��µ�GEO�����µ�GEO�Ѿ���������ֱ��פ����ǰϵͳ�����������ѭ�� */
        if (VOS_TRUE == CNAS_XSD_IsCurGeoListSrchedInGeoListSrchInfo(&stGeoListInfo))
        {
            if (VOS_TRUE == ulIsNegSys)
            {
                CNAS_XSD_ContinueGeoSysRecordSrch();

                return;
            }

            /* ����ǰ֪ͨ�ϲ����״̬ */
            CNAS_XSD_ReportSrvAvailableInd_SysAcq(&stCurSysInfo);

            /* ����ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF */
            CNAS_XSD_SndCas1xCampSyncedChannelNtf();

            /* Ǩ�Ƶ� CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND״̬, ������������ʱ��TI_CNAS_XSD_WAIT_CAS_OHM_IND */
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_CAS_OHM_IND);

            /* ����������ʱ�� */
            CNAS_XSD_StartTimer(TI_CNAS_XSD_WAIT_CAS_OHM_IND, TI_CNAS_XSD_WAIT_CAS_OHM_IND_LEN);

            return;
        }

        /*******************************************************************************************
         * 1.4. ����δ���������µ�GEO���ͷ��ϵ�GEOϵͳ��¼�����б������µ�GEO����ϵͳѡ��
         ******************************************************************************************/
        /* �ͷŵ�ǰ��GEOϵͳ��¼�����б� */
        PS_MEM_FREE(UEPS_PID_XSD, pstGeoSysRecSrchList->pstAcqSysInfo);
        pstGeoSysRecSrchList->pstAcqSysInfo = VOS_NULL_PTR;

        /* ʹ��һ���µ�GEO�������ŵ�ϵͳѡ�� */
        CNAS_XSD_PeformBestPreferedSystemSelectionUsingNewGeo_SysAcq(&stGeoListInfo,
                                                                     &stCurSysInfo,
                                                                     ulIsNegSys);

        return;
    }

    /***********************************************************************************************
     * 3. �����ǰϵͳ����PRL���У���ϵͳ��homeϵͳ���򲶻��ϵͳ
     **********************************************************************************************/
    /* HOME SID/NID �б�Ӱ��ϵͳפ�����ж� */
    /* ����PRL���У���ʹ��HOME SID/NID�б��У�Ҳ������ֱ��פ�� */

    /***********************************************************************************************
     * 4. ͬ������ϵͳ��availableϵͳ�������scan list�ж�ӦƵ��״̬�ٴ�ˢΪ������
     **********************************************************************************************/
    CNAS_XSD_UpdateAvailOrNegSystemChanStatusInScanChanList_SysAcq(&stCurSysInfo.stFreq,
                                                                   CNAS_XSD_GetScanChanListAddr());

    /***********************************************************************************************
     * 5. �����ǰϵͳ��availableϵͳ���ϱ�������Ϣ��ͬʱˢ��available list
     **********************************************************************************************/
    if (VOS_FALSE == CNAS_PRL_GetPrefOnlyFlg())
    {
        /* check��ǰavailableϵͳ�Ƿ��ڰ��������ڰ������У�����ӵ�availableϵͳ�У��ϱ�������� */
        if (VOS_TRUE == CNAS_XSD_IsSidInOperLockSysWhiteList(pstSyncCnf->stSyncedSysId.usSid))
        {
            /* ����ǰ֪ͨ�ϲ����״̬ */
            CNAS_XSD_ReportSrvAvailableInd_SysAcq(&stCurSysInfo);

            /* ˢ��available list */
            CNAS_XSD_AddAvailSysFreqList(&(stCurSysInfo.stFreq));
        }
    }

    /***********************************************************************************************
     * 6. ����GEOϵͳ��¼�����б�����
     **********************************************************************************************/
    CNAS_XSD_ContinueGeoSysRecordSrch();

    return;
}


VOS_VOID CNAS_XSD_ProcSyncFailAndNoGeoSysRecorSearchList_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU    *pstScanList = VOS_NULL_PTR;
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usCurScanListIndex;
    VOS_UINT16                          usDstChanNum;
    VOS_UINT16                          usSyncFailFreqNum;
    VOS_UINT8                           ucNextAcqIndex;

    ucNextAcqIndex                      = 0;

    usSyncFailFreqNum = (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum;


    /* SRLTE 1x������������Դ��SYNC CNF�����NO RF�����һ��ʧ��Ƶ���ԭ��ֵ��NO RF����һ�ּ���ͬ����Ƶ�� */
    if ((CNAS_CCB_VERSION_SRLTE                 == CNAS_CCB_IsVersionSrlte())
     && (CNAS_CAS_1X_RSLT_NO_RF                 == pstSyncCnf->enSyncRslt)
     && (0                                      != usSyncFailFreqNum))
    {
        usSyncFailFreqNum = (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum - 1;
    }


    /***********************************************************************************************
     * 1. ����availableϵͳƵ��ͬ��
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_GetAvailSysAcqListFlg_SysAcq())
    {
        /* ����availableϵͳ�´��������� */
        usCurScanListIndex = CNAS_XSD_GetAvailSysFreqListNextSrchIndex();

        /* ����ͬ������ʼ����������ʧ�ܵ�NO RF���Ǹ�Ƶ��������ʼ */
        CNAS_XSD_SetAvailSysFeqListNextSrchIndex((VOS_UINT8)(usCurScanListIndex + usSyncFailFreqNum));

        CNAS_XSD_ContinueAvailSysListSrch();

        return;
    }

    /***********************************************************************************************
     * 2. ����AvoidϵͳƵ��ͬ��
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
    {
        /* ����avoidϵͳ�´��������� */
        usCurScanListIndex = 0x00FF;

        if ((pstSyncCnf->ulSyncFailFreqNum > 0)
         && (pstSyncCnf->ulSyncFailFreqNum <= CNAS_CAS_1X_MAX_FREQ_NUM))
        {
            (VOS_VOID)CNAS_XSD_GetAvoidFreqIndexOfAvoidlist((CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncCnf->astSyncRsltList[pstSyncCnf->ulSyncFailFreqNum - 1].stFreq),
                                                            (VOS_UINT8 *)&usCurScanListIndex);
        }

        /* ����ͬ������ʼ����������ʧ�ܵ�NO RF���Ǹ�Ƶ��������ʼ */
        if ((CNAS_CCB_VERSION_SRLTE                 == CNAS_CCB_IsVersionSrlte())
         && (CNAS_CAS_1X_RSLT_NO_RF                 == pstSyncCnf->enSyncRslt))
        {
            CNAS_XSD_SetAvoidSysFeqListNextSrchIndex((VOS_UINT8)(usCurScanListIndex));
        }
        else
        {
            ucNextAcqIndex = CNAS_XSD_GetAvoidFreqListAddr()->ucNextAcqIndex;

            usCurScanListIndex = (VOS_UINT16)CNAS_MAX(ucNextAcqIndex, usCurScanListIndex);

            CNAS_XSD_SetAvoidSysFeqListNextSrchIndex((VOS_UINT8)(usCurScanListIndex + 1));
        }

        CNAS_XSD_ContinueAvoidSysListSrch();

        return;
    }

    /*******************************************************************************************
     * 3. ����scan listͬ��
     ******************************************************************************************/
    usDstChanNum       = 0;

    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    pstScanList = CNAS_XSD_GetScanChanListAddr();

    CNAS_XSD_GetNextScanChanList_SysAcq(CNAS_XSD_GetCurChannelScanIndex(),
                                        &usDstChanNum,
                                        astDstChan,
                                        pstScanList);

    if (0 != usDstChanNum)
    {
        /* ��������̬������1x���磬ÿ����sync req֮��Ӹ�����ʱ�� */
        if (VOS_FALSE == CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(VOS_FALSE))
        {
            /* ������һ�ֽ�Ҫͬ����Ƶ�㣬����ʱ��ʱ�� */
            CNAS_XSD_SaveLteOrDoConn1xSysAcqSyncDelayFreq(usDstChanNum, &astDstChan[0]);
            CNAS_XSD_SetCurrFsmState(CNAS_XSD_SYS_ACQ_STA_WAIT_SYNC_DELAY_TIMER_EXPIRED_WHEN_LTE_OR_DO_CONN);
            CNAS_XSD_StartTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER,
                                CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayTimerLen());
        }
        else
        {
            /* scan listδȫ��ͬ���꣬������ͬ������ */
            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, astDstChan);
        }

        return;
    }

    /*******************************************************************************************
     * 3. ����scan listͬ����ĺ�����������
     ******************************************************************************************/
    /* scan list��ȫ��ͬ���� */
    CNAS_XSD_ProcScanListSyncComplete_SysAcq();

    return;
}


VOS_VOID CNAS_XSD_ReportSrvAvailableInd_SysAcq(
    CNAS_PRL_1X_SYSTEM_STRU                                *pstCurSysInfo
)
{
    XSD_MSCC_SYNC_SERVICE_AVAILABLE_IND_STRU                stSrvAvailableInd;
    NAS_MSCC_PIF_SERVICE_STATUS_ENUM_UINT32                 enServiceStatus;
    CNAS_XSD_SERVICE_STATUS_ENUM_UINT32                     enXsdCurSrvStatus;
    CNAS_CCB_CARD_STATUS_ENUM_UINT8                         enCardStatus;

    /* ��ǰ����״̬����normal service������Ҫ�ϱ�����״̬ */
    if (CNAS_XSD_SERVICE_STATUS_NORMAL_SERVICE == CNAS_XSD_GetCurSrvStatus())
    {
        return;
    }

    enCardStatus = CNAS_CCB_GetCsimCardStatus();

    if ( CNAS_CCB_CARD_STATUS_ABSENT == enCardStatus )
    {
        enXsdCurSrvStatus = CNAS_XSD_SERVICE_STATUS_LIMITED_SERVICE;
        enServiceStatus   = NAS_MSCC_PIF_SERVICE_STATUS_LIMITED_SERVICE;
    }
    else
    {
        enXsdCurSrvStatus = CNAS_XSD_SERVICE_STATUS_NORMAL_SERVICE;
        enServiceStatus   = NAS_MSCC_PIF_SERVICE_STATUS_NORMAL_SERVICE;
    }

    /* ���µ�ǰ�ķ���״̬Ϊnormal service */
    CNAS_XSD_SetCurSrvStatus(enXsdCurSrvStatus);

    NAS_MEM_SET_S(&stSrvAvailableInd, sizeof(stSrvAvailableInd), 0, sizeof(XSD_MSCC_SYNC_SERVICE_AVAILABLE_IND_STRU));

    stSrvAvailableInd.usFreq          = pstCurSysInfo->stFreq.usChannel;
    stSrvAvailableInd.usBandClass     = pstCurSysInfo->stFreq.enBandClass;
    stSrvAvailableInd.usNid           = pstCurSysInfo->usNid;
    stSrvAvailableInd.usSid           = pstCurSysInfo->usSid;
    stSrvAvailableInd.enServiceStatus = enServiceStatus;
    stSrvAvailableInd.ucRoamingInd    = CNAS_XSD_GetCurrentSystemRoamingInd(pstCurSysInfo);

    CNAS_XSD_SndMsccSyncServiceAvailableInd(&stSrvAvailableInd);
    return;
}


VOS_UINT32 CNAS_XSD_IsCallRelAnyCampOnFreq_SysAcq(
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                      *pstFreq
)
{
    CNAS_XSD_CALL_REL_ANY_CAMP_ON_FREQ_STRU                *pstCallRelAnyCampOnFreq = VOS_NULL_PTR;
    VOS_UINT32                                              i;

    pstCallRelAnyCampOnFreq = CNAS_XSD_GetCallRelAnyCampOnFreqListAddr();

    for (i = 0; i < (VOS_UINT32)CNAS_MIN(pstCallRelAnyCampOnFreq->usChanNum, CNAS_XSD_CALL_REL_SYNC_MAX_FREQ_NUM); i++)
    {
        if ((pstFreq->usBandClass == pstCallRelAnyCampOnFreq->astFreq[i].enBandClass)
         && (pstFreq->usChannel   == pstCallRelAnyCampOnFreq->astFreq[i].usChannel))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT16 CNAS_XSD_UpdateSyncSuccChanStatus_SysAcq(
    VOS_UINT16                                              usBeginScanListIndex,
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstChannel,
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanChanList,
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList
)
{
    VOS_UINT16                                              usCurFreqIndex;

    /***********************************************************************************************
       ͬ���ɹ���Ƶ�㴦��
       i�� �����GEOͬ���������OOC�����µ�MRU0����Ƶ�㣬��ˢ�£�������Ҫ��scan list��ǰ��������
           ˢ�¸�Ƶ�㣻

       ii���������GEOͬ�������scan list��ǰ�����������ˢ�¸�Ƶ��
     **********************************************************************************************/

    usCurFreqIndex = usBeginScanListIndex;

    if (VOS_NULL_PTR != pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        if (VOS_TRUE == CNAS_XSD_IsMru0FreqInOocScene(pstChannel))
        {
            return usCurFreqIndex;
        }
    }

    usCurFreqIndex = CNAS_XSD_UpdateChanStatusInScanChanList(usBeginScanListIndex,
                                                             pstChannel,
                                                             CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_EXIST,
                                                             pstScanChanList);

    return usCurFreqIndex;
}


VOS_UINT16 CNAS_XSD_UpdateSyncFailChanStatus_SysAcq(
    VOS_UINT16                                              usBeginScanListIndex,
    VOS_UINT16                                              usSyncFailChanNum,
    CNAS_CAS_1X_SYNC_RSLT_INFO_STRU                        *pstSyncFailChanList,
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanChanList,
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList
)
{
    VOS_UINT16                                              i;
    VOS_UINT16                                              usCurFreqIndex;
    CNAS_PRL_1X_SYSTEM_STRU                                 stMru0SysId;
    VOS_UINT16                                              ulMru0FreqIndex;


    /***********************************************************************************************
       a. ͬ��ʧ�ܵ�Ƶ�㴦��
          i)  �����GEOͬ�����������MRU0����Ƶ�㣬����Ҫ��scan list��ǰ��������ˢ��ʧ�ܵ�Ƶ�㣻��
              ����Ҫ��scan listͷ������ǰ������������MRU0Ƶ��ˢ��Ϊͬ�������ڣ�ȷ��״̬һ���ԣ�
          ii���������GEOͬ������scan list��ǰ�����������ˢ��ʧ�ܵ�Ƶ�㣻
     **********************************************************************************************/

    usCurFreqIndex = usBeginScanListIndex;

    /* GEOͬ������ */
    if (VOS_NULL_PTR != pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        ulMru0FreqIndex = CNAS_XSD_INVAILD_SCAN_LIST_FREQ_INDEX;

        /* �������OOC��������û��MRU0Ƶ����볡�� */
        if (VOS_FALSE == CNAS_XSD_IsOocSceneAndRepeatMru0FreqSrchExist(&stMru0SysId))
        {
            ulMru0FreqIndex = CNAS_XSD_INVAILD_SCAN_LIST_FREQ_INDEX;
        }
        else
        {
            /* checkͬ��������Ƿ���MRU0Ƶ����� */
            for (i = 0; i < usSyncFailChanNum; i++)
            {
                if ((stMru0SysId.stFreq.enBandClass == pstSyncFailChanList[i].stFreq.usBandClass)
                 && (stMru0SysId.stFreq.usChannel   == pstSyncFailChanList[i].stFreq.usChannel))
                {
                    ulMru0FreqIndex = i;

                    break;
                }
            }
        }

        if (0xFFFF == ulMru0FreqIndex)
        {
            /* ����OOC�����µ�MRU0Ƶ�㣬��scan list��ǰ�����������ˢ��ʧ�ܵ�Ƶ�㣻 */
            usCurFreqIndex = CNAS_XSD_UpdateSyncFailChanStatus(usBeginScanListIndex,
                                                               usSyncFailChanNum,
                                                               pstSyncFailChanList,
                                                               pstScanChanList);

            return usCurFreqIndex;
        }

        for (i = 0; i < usSyncFailChanNum; i++)
        {
            /* ����MRU0��Ƶ��, ��ˢ��MRU0Ƶ�� */
            if (i == ulMru0FreqIndex)
            {
                continue;
            }

            usCurFreqIndex =
                CNAS_XSD_UpdateChanStatusInScanChanList(usBeginScanListIndex,
                                                        (CNAS_PRL_FREQENCY_CHANNEL_STRU *)&pstSyncFailChanList[i].stFreq,
                                                        CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                        pstScanChanList);
        }

        /* ����MRU0��Ƶ�㣬��Ҫ��scan listͷ������ǰ������������MRU0Ƶ��ˢ��Ϊͬ�������ڣ�ȷ��״̬һ���ԣ� */
        CNAS_XSD_UpdateFreqStatusBeforeCurIndexInScanChanList(CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                              usBeginScanListIndex,
                                                              &(stMru0SysId.stFreq),
                                                              pstScanChanList);

        return usCurFreqIndex;
    }

    /* ����ֻˢ��ͬ�����ڵĻ�δͬ����Ƶ��Ƶ��״̬Ϊͬ�������� */
    for (i = 0; i < usSyncFailChanNum; i++)
    {
        usCurFreqIndex =
            CNAS_XSD_UpdateNoSyncedOrSyncSuccScanChanInScanChanList(CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                                    usBeginScanListIndex,
                                                                    (CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncFailChanList[i].stFreq),
                                                                    pstScanChanList);

    }

    return usCurFreqIndex;
}


VOS_VOID CNAS_XSD_UpdateChanStatusUponRcvCasSyncCnfSucc_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList          = VOS_NULL_PTR;
    VOS_UINT16                                              usCurScanListIndex;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstEmcScanChanList   = VOS_NULL_PTR;

    pstEmcScanChanList                  = CNAS_XSD_GetEmcCallRedialScanChanListAddr();

    /***********************************************************************************************
     * 1. �����available��Avoidϵͳͬ�����򲻶�scan list��������
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_GetAvailSysAcqListFlg_SysAcq())
    {
        return;
    }

    /* Avoid��������Ҫ����ScanChanList */
    if (VOS_TRUE == CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
    {
        return;
    }

    pstScanList          = CNAS_XSD_GetScanChanListAddr();

    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    /***********************************************************************************************
     * 2. ͬ��ʧ�ܵ�Ƶ�㴦��
     **********************************************************************************************/
    if (0 != pstSyncCnf->ulSyncFailFreqNum)
    {
        /* ����ͬ��ʧ�ܵ�Ƶ��ˢ�� */
        usCurScanListIndex = CNAS_XSD_UpdateSyncFailChanStatus_SysAcq(pstScanList->usCurScanIndex,
                                                                      (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum,
                                                                      &pstSyncCnf->astSyncRsltList[0],
                                                                      pstScanList,
                                                                      pstGeoSysRecSrchList);

        /* GEO������ʱ�����µ�ǰscan list�����������ӿ�ͬ���ɹ�Ƶ���ˢ�� */
        if (VOS_NULL_PTR == pstGeoSysRecSrchList->pstAcqSysInfo)
        {
            CNAS_XSD_SetCurChannelScanIndex(usCurScanListIndex + 1);
        }

        /* ����������SD������ˢ�½�������Ƶ���б���������ظ���ͬ��ʧ�ܵ�Ƶ�� */
        if (VOS_NULL_PTR != pstEmcScanChanList->pstScanChanInfo)
        {
            if (VOS_TRUE == CNAS_XSD_IsEmcSDSysAcq_SysAcq(CNAS_XSD_GetCurSysAcqScene_SysAcq()))
            {
                (VOS_VOID)CNAS_XSD_UpdateSyncFailChanStatus(pstEmcScanChanList->usCurScanIndex,
                                                           (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum,
                                                            pstSyncCnf->astSyncRsltList,
                                                            pstEmcScanChanList);
            }
        }
    }

    /***********************************************************************************************
     * 3. ͬ���ɹ���Ƶ�㴦��
     **********************************************************************************************/
    usCurScanListIndex = CNAS_XSD_UpdateSyncSuccChanStatus_SysAcq(pstScanList->usCurScanIndex,
                                                                  (CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncCnf->stFreq),
                                                                  pstScanList,
                                                                  pstGeoSysRecSrchList);

    /***********************************************************************************************
     * 4. �������GEOͬ������ͬ���ɹ���Ƶ���ǲ����MRU0Ƶ�㣻���scan listͷ������ǰ��������ָ��scan
     *    list�б���ͬ���ɹ���MRU0��Ƶ������������������MRU0Ƶ��ˢ��Ϊͬ�����ڣ�ȷ��״̬һ���ԣ�
     **********************************************************************************************/
    if (VOS_NULL_PTR == pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        /* �ٴθ����´�scan list�������� */
        CNAS_XSD_SetCurChannelScanIndex(usCurScanListIndex + 1);


        /* ����OOC�����£�����ֲ���MRU0Ƶ�㣬Ϊ��ȷ��֮ǰ������Ƶ���뱾���ѵ���Ƶ��״̬һ�£������
         * ֮ǰ��Ƶ��Ҳˢ��ͬ���ɹ�
         */
        if (VOS_TRUE == CNAS_XSD_IsMru0FreqInOocScene((CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncCnf->stFreq)))
        {
            CNAS_XSD_UpdateFreqStatusBeforeCurIndexInScanChanList(CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_EXIST,
                                                                  usCurScanListIndex,
                                                                  (CNAS_PRL_FREQENCY_CHANNEL_STRU *)&(pstSyncCnf->stFreq),
                                                                  pstScanList);
        }

        /* log scan list����״̬ */
        CNAS_XSD_LogScanChannelList(ID_CNAS_XSD_MNTN_LOG_SCAN_CHAN_LIST_IND, pstScanList);

    }
}


VOS_VOID CNAS_XSD_UpdateChanStatusUponRcvCasSyncCnfFail_SysAcq(
    CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU   *pstSyncCnf
)
{
    CNAS_XSD_GEO_SYS_RECORD_SRCH_LIST_STRU                 *pstGeoSysRecSrchList = VOS_NULL_PTR;
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList          = VOS_NULL_PTR;
    VOS_UINT16                                              usCurScanListIndex;

    VOS_UINT16                                              usUpdateSyncFailFreqNum;

    usUpdateSyncFailFreqNum = (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum;

    /***********************************************************************************************
     * 1 �����available��Avoidϵͳͬ�����򲻶�scan list������������
     **********************************************************************************************/
    /* availableϵͳͬ�������� */
    if (VOS_TRUE == CNAS_XSD_GetAvailSysAcqListFlg_SysAcq())
    {
        return;
    }

    /* Avoid��������Ҫ����ScanChanList */
    if (VOS_TRUE == CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
    {
        return;
    }

    pstScanList          = CNAS_XSD_GetScanChanListAddr();

    pstGeoSysRecSrchList = CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq();

    usCurScanListIndex   = pstScanList->usCurScanIndex;

    /***********************************************************************************************
     * 2 �����GEOͬ�����������MRU0����Ƶ�㣬����Ҫ��scan list��ǰ��������ˢ��ʧ�ܵ�Ƶ�㣻������Ҫ��
     *   scan listͷ������ǰ������������MRU0Ƶ��ˢ��Ϊͬ�������ڣ�ȷ��״̬һ���ԣ�
    **********************************************************************************************/
    if ((CNAS_CCB_VERSION_SRLTE                 == CNAS_CCB_IsVersionSrlte())
     && (CNAS_CAS_1X_RSLT_NO_RF                 == pstSyncCnf->enSyncRslt)
     && (0                                      != usUpdateSyncFailFreqNum))
    {
        usUpdateSyncFailFreqNum = (VOS_UINT16)pstSyncCnf->ulSyncFailFreqNum - 1;
        if (0 == usUpdateSyncFailFreqNum)
        {
            /* û����Ҫˢ�µ�Ƶ�㣬ֱ�ӷ��� */
            return;
        }
    }

    usCurScanListIndex = CNAS_XSD_UpdateSyncFailChanStatus_SysAcq(pstScanList->usCurScanIndex,
                                                                  usUpdateSyncFailFreqNum,
                                                                  &pstSyncCnf->astSyncRsltList[0],
                                                                  pstScanList,
                                                                  pstGeoSysRecSrchList);

    /***********************************************************************************************
     * 3 �������GEOͬ��������Ҫ��scan list��ǰ��������ˢ��ʧ�ܵ�Ƶ�㣻
     **********************************************************************************************/
    if (VOS_NULL_PTR == pstGeoSysRecSrchList->pstAcqSysInfo)
    {
        CNAS_XSD_SetCurChannelScanIndex(usCurScanListIndex + 1);

        /* log scan list����״̬ */
        CNAS_XSD_LogScanChannelList(ID_CNAS_XSD_MNTN_LOG_SCAN_CHAN_LIST_IND, pstScanList);
    }
}


VOS_VOID CNAS_XSD_UpdateChanStatusUponRcvCasDeterminInd_SysAcq(
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstTempFreq
)
{
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList = VOS_NULL_PTR;

    /***********************************************************************************************
     * 1 �����available��Avoidϵͳͬ�����򲻶�scan list������������
     **********************************************************************************************/
    /* availableϵͳͬ�������� */
    if (VOS_TRUE == CNAS_XSD_GetAvailSysAcqListFlg_SysAcq())
    {
        return;
    }

    /* Avoid��������Ҫ����ScanChanList */
    if (VOS_TRUE == CNAS_XSD_GetAvoidSysAcqListFlg_SysAcq())
    {
        return;
    }

    pstScanList = CNAS_XSD_GetScanChanListAddr();

    /***********************************************************************************************
     * 2 �����ǰʧ�ܵ�Ƶ����MRU0��Ƶ�㣬����Ҫ��scan listͷ������ǰ������������MRU0Ƶ��ˢ��Ϊͬ������
     *   �ڣ������scan listͷ������ǰ���������ҵ�ʧ�ܵ�Ƶ�������ѭ����
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_IsMru0FreqInOocScene(pstTempFreq))
    {
        CNAS_XSD_UpdateFreqStatusBeforeCurIndexInScanChanList(CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                              pstScanList->usCurScanIndex,
                                                              pstTempFreq,
                                                              pstScanList);
    }
    else
    {

        (VOS_VOID)CNAS_XSD_UpdateChanStatusInScanChanList(0,
                                                          pstTempFreq,
                                                          CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                          pstScanList);

    }

    /* log scan list����״̬ */
    CNAS_XSD_LogScanChannelList(ID_CNAS_XSD_MNTN_LOG_SCAN_CHAN_LIST_IND, pstScanList);
}


VOS_VOID CNAS_XSD_UpdateAvailOrNegSystemChanStatusInScanChanList_SysAcq(
    CNAS_PRL_FREQENCY_CHANNEL_STRU                         *pstFreq,
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanChanList
)
{
    CNAS_XSD_SCAN_CHANNEL_LIST_STRU                        *pstScanList = VOS_NULL_PTR;

    pstScanList = CNAS_XSD_GetScanChanListAddr();

    /***********************************************************************************************
     * i) �����OOC�����µ�MRU0����Ƶ�㣬ͬ������MRU0Ƶ����negativeϵͳ��availableϵͳ����scan list��
     * �������в����MRU0��ˢ��Ϊ�����ڣ�����
     * ii) ��scan list��ǰ��ʼ���ң�ˢ�¸�Ƶ��Ϊ����������
     **********************************************************************************************/
    if (VOS_TRUE == CNAS_XSD_IsMru0FreqInOocScene(pstFreq))
    {
        CNAS_XSD_UpdateFreqStatusBeforeCurIndexInScanChanList(CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                              pstScanChanList->usTotalNum,
                                                              pstFreq,
                                                              pstScanChanList);
    }
    else
    {
        (VOS_VOID)CNAS_XSD_UpdateChanStatusInScanChanList(0,
                                                          pstFreq,
                                                          CNAS_XSD_SCAN_CHAN_STATUS_SYNCED_NO_EXIST,
                                                          pstScanList);
    }
}

VOS_UINT32 CNAS_XSD_IsAllowedPerformAbortProcedure_SysAcq(
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enCurAcqScene,
    VOS_UINT32                         *pulIsCallAbort
)
{
    VOS_UINT32                          ulHrpdConnExistFlag;
    VOS_UINT32                          ulLteConnExistFlag;
    CNAS_XSD_SCAN_CHANNEL_INFO_STRU    *pstScanChanInfo;

    ulLteConnExistFlag      = CNAS_XSD_GetLteConnExistFlag();
    ulHrpdConnExistFlag     = CNAS_XSD_GetHrpdConnExistFlag();
    *pulIsCallAbort         = VOS_FALSE;
    pstScanChanInfo         = CNAS_XSD_GetEmcCallRedialScanChanListAddr()->pstScanChanInfo;

    /* �ų���ǰ����Ҫ��ϵĳ��� */
    if ((CNAS_XSD_SYS_ACQ_SCENE_SERVICE_ACQ         == enCurAcqScene)
      ||(CNAS_XSD_SYS_ACQ_SCENE_NORMAL_CALL_REDIAL  == enCurAcqScene)
      ||(CNAS_XSD_SYS_ACQ_SCENE_EMERGENCY_CALL      == enCurAcqScene))
    {
        return VOS_FALSE;
    }

    /***********************************************************************************************
     *  ��ǰ����ʧ�ܣ�����������һ�������˳���ǰ����
        a)�����ز������б�Ϊ�գ�����������LTE\HRPD����̬�µ�1X����
        b)LTE\HRPD��idle̬������̬��
     **********************************************************************************************/
    if (CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN == enCurAcqScene)
    {
        if ((VOS_NULL_PTR != CNAS_XSD_GetCallRedialScanChanListAddr()->pstScanChanInfo)
         || (VOS_NULL_PTR != pstScanChanInfo))
        {
            *pulIsCallAbort = VOS_TRUE;
            return VOS_TRUE;
        }
    }
    else
    {
        if ((VOS_TRUE                == ulHrpdConnExistFlag)
         || ((CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
          && (VOS_TRUE               == ulLteConnExistFlag)))
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_VOID CNAS_XSD_PerformAbortCurrSysAcqScene_SysAcq(
    VOS_UINT32                          ulIsCallAbort
)
{
    if ((VOS_TRUE == ulIsCallAbort)
     && (0        != CNAS_XSD_GetCacheMsgNum()))
    {
        CNAS_XSD_PerformAbortProcedure_SysAcq();
    }
    else
    {
        CNAS_XSD_StopTimer(TI_CNAS_XSD_POWEROFF_CAMP_ON_PROTECT_TIMER);

        /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
        CNAS_XSD_SndSysAcqRslt(CNAS_XSD_SYSTEM_ACQUIRED_RESULT_FAILURE,
                               CNAS_XSD_GetCurSysAcqScene_SysAcq(),
                               CNAS_XSD_IsImmediateSysAcq_SysAcq(),
                               CNAS_XSD_GetOocTotalTimerExpiredFlg_SysAcq());

        /* �˳����״̬�� */
        CNAS_XSD_QuitFsmSysAcq_SysAcq();
    }
    return;
}


VOS_UINT32 CNAS_XSD_RcvTiLteOrDoConnSyncDelayTimerExpired_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_1X_SYS_ACQ_SYNC_DELAY_INFO_WHEN_LTE_OR_DO_CONN_STRU   *pstSyncDelayInfoAddr;
    CNAS_XSD_SCAN_CHANNEL_INFO_STRU                                *pstEmcScanChanInfo;
    CNAS_PRL_FREQENCY_CHANNEL_STRU                                  astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                                                      usDstChanNum;

    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));
    usDstChanNum         = 0;
    pstEmcScanChanInfo   = CNAS_XSD_GetEmcCallRedialScanChanListAddr()->pstScanChanInfo;
    pstSyncDelayInfoAddr = CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayInfoAddr();

    /*********************************************************************************************
     *�����ǰ��geoͬ����xsd�Ѿ��ϱ���normal service��֮ǰ����к��������Ļ��������ز����������·���
     *���Ե��ӳ�ͬ����ʱ����ʱ��ʱ���ж�һ���ز������б�Ϊ��:
     1�������ǰ��GEOͬ�������ͬ��Ƶ��32��
    **********************************************************************************************/
    if ((VOS_NULL_PTR != CNAS_XSD_GetCallRedialScanChanListAddr()->pstScanChanInfo)
     || (VOS_NULL_PTR != pstEmcScanChanInfo))
    {
        if (VOS_NULL_PTR != CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo)
        {
            CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvTiLteOrDoConnSyncDelayTimerExpired_SysAcq_WaitSyncDelayTimerExpired:xsd has report normal service");

            CNAS_XSD_GetNextSysSyncListFromGeoSysRecordSrchList(CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq(),
                                                                &usDstChanNum,
                                                                &astDstChan[0]);

            CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
            return VOS_TRUE;
        }
    }

    CNAS_XSD_ContinueSyncScanList_SysAcq(pstSyncDelayInfoAddr->us1xSysAcqSyncDelayFreqNum,
                                         pstSyncDelayInfoAddr->austSysAcqSyncDelayFreqInfo);
    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvMsccLteHrpdConnInfoInd_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ��ʱͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER);

    CNAS_XSD_ContinueSysSync_SysAcq();
    return VOS_TRUE;
}



VOS_UINT32 CNAS_XSD_RcvMsccPowerOffReq_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ��ʱͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER);

    /* ��������ȼ���Ϻ�֮ǰ��ϵĻ�����Ϣ */
    if (VOS_TRUE == CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ))
    {
        CNAS_XSD_SndMsccSrvAcqCnf(NAS_MSCC_PIF_SRV_ACQ_RESULT_FAIL);

        CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);
    }

    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    /* ����ػ����� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_OFF);

    CNAS_XSD_PerformAbortProcedure_SysAcq();

    return VOS_TRUE;
}

VOS_UINT32 CNAS_XSD_RcvMsccCallRedialSystemAcquireNtf_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usDstChanNum;

    usDstChanNum = 0;
    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    /* ֹͣ��ʱͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER);

    /* �����ǰ������̬��GEOͬ��������� */
    if (VOS_NULL_PTR != CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo)
    {
        CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvMsccCallRedialSystemAcquireNtf_SysAcq_WaitSyncDelayTimerExpired:conflict situation");

        CNAS_XSD_GetNextSysSyncListFromGeoSysRecordSrchList(CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq(),
                                                            &usDstChanNum,
                                                            &astDstChan[0]);

        CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
        return VOS_TRUE;
    }

    /* ��������ز�ָʾ��Ϣ */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_CALL_REDIAL);

    CNAS_XSD_PerformAbortProcedure_SysAcq();

    return VOS_TRUE;
}

VOS_UINT32 CNAS_XSD_RcvTiOosSysAcqCurPhaseTotalTimerExpired_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���ó�ʱ��� */
    CNAS_XSD_SetOocTotalTimerExpiredFlg_SysAcq(VOS_TRUE);

    return VOS_TRUE;
}

VOS_UINT32 CNAS_XSD_RcvMsccPowerSaveReq_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER);

    /* ��������ȼ���Ϻ�֮ǰ��ϵĻ�����Ϣ */
    if (VOS_TRUE == CNAS_XSD_ClearCacheMsgByMsgName(UEPS_PID_MSCC, ID_MSCC_XSD_SRV_ACQ_REQ))
    {
        CNAS_XSD_SndMsccSrvAcqCnf(NAS_MSCC_PIF_SRV_ACQ_RESULT_FAIL);

        CNAS_XSD_SetSrvAcqFlg(VOS_FALSE);
    }

    /* ��ջ��� */
    CNAS_XSD_InitCacheMsgQueue(CNAS_XSD_INIT_CTX_POWEROFF, &(CNAS_XSD_GetXsdCtxAddr()->stCacheMsgQueue));

    /* ����POWER SAVE���� */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_SAVE);

    CNAS_XSD_PerformAbortProcedure_SysAcq();

    return VOS_TRUE;
}

VOS_UINT32 CNAS_XSD_RcvMsccSrvAcqReq_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_PRL_FREQENCY_CHANNEL_STRU      astDstChan[CNAS_CAS_1X_MAX_FREQ_NUM];
    VOS_UINT16                          usDstChanNum;

    usDstChanNum = 0;
    NAS_MEM_SET_S(&astDstChan[0], sizeof(astDstChan), 0, sizeof(astDstChan));

    /* ֹͣ��ʱͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER);

    /* �����ǰ������̬��GEOͬ��������� */
    if (VOS_NULL_PTR != CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq()->pstAcqSysInfo)
    {
        CNAS_WARNING_LOG(UEPS_PID_XSD, "CNAS_XSD_RcvMsccSrvAcqReq_SysAcq_WaitSyncDelayTimerExpired:conflict situation");
        CNAS_XSD_SetSrvAcqFlg(VOS_TRUE);

        CNAS_XSD_GetNextSysSyncListFromGeoSysRecordSrchList(CNAS_XSD_GetGeoSysRecSrchListAddr_SysAcq(),
                                                            &usDstChanNum,
                                                            &astDstChan[0]);

        CNAS_XSD_ContinueSyncScanList_SysAcq(usDstChanNum, &astDstChan[0]);
        return VOS_TRUE;
    }

     /* ����svc acq��Ϣ */
    CNAS_XSD_SaveCacheMsg(ulEventType, pstMsg);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_SRV_ACQ);

    CNAS_XSD_PerformAbortProcedure_SysAcq();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_RcvTiPowerOffCampOnProtectTimerExpired_SysAcq_WaitSyncDelayTimerExpired(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ��ʱͬ����ʱ�� */
    CNAS_XSD_StopTimer(TI_CNAS_XSD_SYS_ACQ_WHEN_LTE_OR_DO_CONN_SYNC_DELAY_TIMER);

    CNAS_XSD_SetSystemAcquiredAbortedCause(NAS_ERR_LOG_1X_SEARCH_RSLT_ABORTED_BY_POWER_OFF);

    CNAS_XSD_PerformAbortProcedure_SysAcq();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XSD_IsAllowedSyncImmediatelyWhenLteDoConn_SysAcq(
    VOS_UINT32                          ulIsGeoSync
)
{
    VOS_UINT32                          ulHrpdConnExistFlag;
    VOS_UINT32                          ulLteConnExistFlag;
    CNAS_XSD_SCAN_CHANNEL_INFO_STRU    *pstEmcScanChanInfo;
    CNAS_XSD_SYS_ACQ_SCENE_ENUM_UINT32  enCurAcqScene;

    enCurAcqScene           = CNAS_XSD_GetCurSysAcqScene_SysAcq();
    ulLteConnExistFlag      = CNAS_XSD_GetLteConnExistFlag();
    ulHrpdConnExistFlag     = CNAS_XSD_GetHrpdConnExistFlag();
    pstEmcScanChanInfo      = CNAS_XSD_GetEmcCallRedialScanChanListAddr()->pstScanChanInfo;

    /* ������������̬1xͬ�������ӳ��·��ı�־DISABLE */
    if (VOS_FALSE == CNAS_XSD_GetLteOrDoConn1xSysAcqSyncDelayFlag())
    {
        return VOS_TRUE;
    }

    /* ��ǰ����������̬ */
    if((VOS_TRUE                == ulHrpdConnExistFlag)
    || ((CNAS_CCB_VERSION_SRLTE == CNAS_CCB_IsVersionSrlte())
     && (VOS_TRUE               == ulLteConnExistFlag)))
    {
        /* ��ǰ��GEOͬ������֮ǰû�к����ز���������,�ӳ��·�ͬ������ */
        if (VOS_TRUE == ulIsGeoSync)
        {
            if ((VOS_NULL_PTR != CNAS_XSD_GetCallRedialScanChanListAddr()->pstScanChanInfo)
             || (VOS_NULL_PTR != pstEmcScanChanInfo))
            {
                return VOS_TRUE;
            }
            else
            {
                return VOS_FALSE;
            }
        }

        /* ����������CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN���ӳ��·�ͬ������ */
        if (CNAS_XSD_SYS_ACQ_SCENE_SYSTEM_LOST_WITH_LTE_OR_DO_CONN == enCurAcqScene)
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


/*lint -restore*/

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */



