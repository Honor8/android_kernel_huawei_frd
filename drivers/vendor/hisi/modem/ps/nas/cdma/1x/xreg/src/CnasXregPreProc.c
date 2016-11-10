

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "PsTypeDef.h"
#include  "xsd_xreg_pif.h"
#include  "CnasXregSndInternalMsg.h"
#include  "CnasXregProcess.h"
#include  "CnasTimerMgmt.h"
#include  "CnasXregTimer.h"
#include  "CnasXregListProc.h"

#include  "CnasCcb.h"

#include  "CnasXregSndCas.h"
#include  "CnasXregCtx.h"
#include  "CnasXregFsmSwitchOn.h"
#include  "CnasXregMntn.h"
#include  "CnasXregPreProcTbl.h"
#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_CNAS_XREG_PRE_PROC_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ��������
*****************************************************************************/
/*lint -save -e958*/


VOS_UINT32 CNAS_XREG_RcvSysInfo_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*
    1. If REG_PRDs is equal to zero, the mobile station shall set COUNTER_ENABLEDs to NO.
    2. If REG_PRDs is not equal to zero, the mobile station shall set REG_COUNT_MAXs as specified in 2.6.5.1.3.
    3. The mobile station shall update its roaming status and set REG_ENABLEDs as specified in 2.6.5.3.
    4. If ZONE_LISTs contains more than TOTAL_ZONESs entries, the mobile station shall
       delete the excess entries according to the rules specified in 2.6.5.1.5.
    5. If MULT_SIDSs is equal to '0' and SID_NID_LIST contains entries with different
       SIDs, delete the excess entries according to the rules specified in 2.6.5.1.5.
    6. If MULT_NIDSs is equal to '0' and SID_NID_LIST contains more than one entry for
       any SID, delete the excess entries according to the rules specified in 2.6.5.1.5.
    */

    XSD_XREG_SYS_INFO_IND_STRU                 *pstSysMsg;
    CNAS_XREG_SYS_MSG_CONTENT_STRU             *pstSysMsgCont;
    CNAS_XREG_REGSTATE_STRU                    *pstRegState;
    VOS_UINT32                                  ulDistValue;

    pstSysMsg = (XSD_XREG_SYS_INFO_IND_STRU*)pstMsg;

    pstSysMsgCont               = CNAS_XREG_GetSysMsgAddr();

    /* ��TCH̬�л���ϵͳ�����жϣ��Ƿ��˻�IDLE̬�����ע�� */
    CNAS_XREG_TchHandoffProc(pstSysMsg);

    /* ֮ǰ�б����ϵͳʱ�Ž����ж� */
    if (CNAS_XREG_SYSMSG_STATE_BUTT != pstSysMsgCont->enAvailable)
    {
        CNAS_XREG_BlkSysChange(&(pstSysMsg->stSysInfo), &(pstSysMsgCont->stSysInfo));
    }

    /* ���������²Ŵ������Ϣ */
    if (XSD_XREG_SERVICE_STATUS_NORMAL_SERVICE == pstSysMsg->enServiceStatus)
    {
        pstSysMsgCont->enAvailable  = CNAS_XREG_SYSMSG_CURRENT;
    }
    else
    {
        pstSysMsgCont->enAvailable  = CNAS_XREG_SYSMSG_NOT_CURRENT;
    }


    /* ����ǰϵͳ��Ϣ���ݱ��浽ȫ�ֱ����� */
    VOS_MemCpy(&(pstSysMsgCont->stSysInfo), &(pstSysMsg->stSysInfo), sizeof(XSD_XREG_SYS_INFO_STRU));

    if (PS_TRUE == pstSysMsg->stSysInfo.enRegInfoIncl)
    {
        /* ����ϵͳ��Ϣ�е�REG_PRDs����ע�ᶨʱ�� */
        CNAS_XREG_RegTimerRefresh(pstSysMsg->stSysInfo.stRegInfo.ucRegPeriod);

        /* ��������״̬������ע��ʹ�ܱ�� */
        CNAS_XREG_SetRegEnabled(pstSysMsg->stSysInfo.usSid,
                                pstSysMsg->stSysInfo.usNid,
                                pstSysMsg->stSysInfo.stRegInfo.ucHomeReg,
                                pstSysMsg->stSysInfo.stRegInfo.ucSidRoamReg,
                                pstSysMsg->stSysInfo.stRegInfo.ucNidRoamReg);

        /* ɾ����ʱ�ڵ� */
        CNAS_XREG_RefreshLists();

        /* ����ϵͳ��Ϣ�е�ucRegZoneNum����ZONE_LIST�еĶ�����ɾ�� */
        CNAS_XREG_TotalZones(pstSysMsg->stSysInfo.stRegInfo.ucRegZoneNum);

        /* ����ucMultiSidFlgΪ0 */
        if (0 == pstSysMsg->stSysInfo.stRegInfo.ucMultiSidFlg)
        {
            CNAS_XREG_DelMultSids();
        }

        /* ����ucMultiNidFlgΪ0 */
        if (0 == pstSysMsg->stSysInfo.stRegInfo.ucMultiNidFlg)
        {
            CNAS_XREG_DelMultNids();
        }

        pstRegState = CNAS_XREG_GetRegStateAddr();

        /* �ж��Ƿ�������ھ���ע�� */
        if ((0 != pstSysMsg->stSysInfo.stRegInfo.usRegDistance)
            && (CNAS_XREG_VAR_YES == pstRegState->enRegEnabledFlag))
        {
            ulDistValue = CNAS_XREG_CalcDistance(pstRegState->stDistInfo.lBaseLast,
                                                 pstRegState->stDistInfo.lBaseLong,
                                                 pstSysMsg->stSysInfo.stBaseStationInfo.lBaseLatitude,
                                                 pstSysMsg->stSysInfo.stBaseStationInfo.lBaseLongitude);

            if (ulDistValue > pstRegState->stDistInfo.ulDistThrd)
            {
                pstRegState->enDistRegFlag = CNAS_XREG_VAR_YES;
            }
            else
            {
                pstRegState->enDistRegFlag = CNAS_XREG_VAR_NO;
            }
        }

        /* ������ʾע�������ϱ� */
        CNAS_XREG_SndOmRegPRDReportInd(pstSysMsg->stSysInfo.stRegInfo.ucRegPeriod);
    }

    /* ����SCI��ֵ */
    CNAS_XREG_CalcSciValue();

    /* ͨ�������ڲ���Ϣ����ע����� */
    CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_REGISTRATION_TYPE_BUTT);

    /* ����ǰ��SIDNIDLIST��ZONELIST��ά�ɲ���� */
    CNAS_XREG_ZoneListMntn();
    CNAS_XREG_SidNidListMntn();
    CNAS_XREG_SndOmRegListReportInd();

    return VOS_TRUE;

}


VOS_UINT32 CNAS_XREG_RcvPowerOffReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ػ�������ʱ�� */
    CNAS_XREG_StartTime(TI_CNAS_XREG_TIMER_PWROFF_ESTCNF_PT, TI_CNAS_XREG_TIMER_PWROFF_ESTCNF_PT_LEN);

    /* �رն�ʱ������ */
    CNAS_StopTimer(UEPS_PID_XREG, TI_CNAS_XREG_TIMER_T57M, 0);
    CNAS_StopTimer(UEPS_PID_XREG, TI_CNAS_XREG_TIMER_PERIOD_REG, 0);
    CNAS_StopTimer(UEPS_PID_XREG, TI_CNAS_XREG_TIMER_ZONELIST_PT, 0);

    /* ����ǰ��ZONE_LIST���SID_NID_LIST��浽NV���� */
    CNAS_XREG_SaveList();

    /* ͨ�������ڲ���Ϣ����ע����� */
    CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_POWER_DOWN_REGISTRATION);

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XREG_RcvXsdDeregisterInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    XSD_XREG_DEREGISTER_IND_STRU       *pstDeregInd = VOS_NULL_PTR;

    pstDeregInd = (XSD_XREG_DEREGISTER_IND_STRU *)pstMsg;

    if (XSD_XREG_DEREGISTER_REASON_POWEROFF == pstDeregInd->enCause)
    {
        CNAS_XREG_SetPowerOffDeregFlg(VOS_TRUE);
    }

    return VOS_TRUE;
}



VOS_UINT32 CNAS_XREG_RcvXsdRedirInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XREG_ClearList();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvNotCurrInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XREG_GetSysMsgAddr()->enAvailable = CNAS_XREG_SYSMSG_NOT_CURRENT;

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvXsdStateInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    XSD_XREG_UE_STATE_IND_STRU                 *pstStateMsg;
    CNAS_XREG_REGSTATE_STRU                    *pstRegState;
    VOS_UINT32                                  ulState;

    pstStateMsg = (XSD_XREG_UE_STATE_IND_STRU*)pstMsg;

    pstRegState = CNAS_XREG_GetRegStateAddr();

    /* ���浱ǰUE״̬ */
    pstRegState->enCasState = pstStateMsg->enCasState;

    CNAS_CCB_SetCasState((CNAS_CCB_1X_CAS_STATE_ENUM_UINT8)pstStateMsg->enCasState);

    CNAS_CCB_SetCasSubState((CNAS_CCB_1X_CAS_SUB_STATE_ENUM_UINT8)pstStateMsg->enCasSubState);

    ulState = CNAS_XREG_GetFsmTopState();

    /* ��ǰΪNULL̬���ô������Ϣ */
    if ((CNAS_XREG_FSM_L1_MAIN == CNAS_XREG_GetCurrFsmId())
     && (CNAS_XREG_L1_STA_NULL == ulState))
    {
        return VOS_TRUE;
    }

    if (CAS_CNAS_1X_CAS_INIT_STATE == pstStateMsg->enCasState)   /*Init*/
    {
        CNAS_XREG_GetSysMsgAddr()->enAvailable = CNAS_XREG_SYSMSG_NOT_CURRENT;

        return VOS_TRUE;
    }

    if (CAS_CNAS_1X_CAS_IDLE_STATE == pstStateMsg->enCasState)  /*Idle*/
    {

        /* ����SCI��ֵ */
        CNAS_XREG_CalcSciValue();

        /*�ж϶�ʱ��δ����*/
        if (CNAS_XREG_TIMER_NULL == pstRegState->enT57MState)
        {
            /*����T57��ʱ��*/
            CNAS_XREG_StartTime(TI_CNAS_XREG_TIMER_T57M, CNAS_XREG_T57M_LEN);

            pstRegState->enT57MState = CNAS_XREG_TIMER_RUNNING;

            return VOS_TRUE;
        }

        /*ͨ���ڲ���Ϣ����ע������*/
        CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_REGISTRATION_TYPE_BUTT);

        return VOS_TRUE;
    }

    if (CAS_CNAS_1X_CAS_TCH_STATE == pstStateMsg->enCasState)  /*Traffic*/
    {
        /* ����Traffic Channel̬����Ҫֹͣ��ʱע�Ṧ�� */
        CNAS_XREG_RegTimerStop();

        return VOS_TRUE;
    }

    return VOS_TRUE;   /*����״̬*/
}


VOS_UINT32 CNAS_XREG_RcvOrderRegDataInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_1X_CSCH_DATA_IND_STRU     *pstCschData;

    pstCschData = (CAS_CNAS_1X_CSCH_DATA_IND_STRU*)pstMsg;

    /* ���������ж�,ֻ����ע����� */
    if (CAS_CNAS_1X_RX_COMMON_ORDER_MSG != pstCschData->enCschMsgType)
    {
        return VOS_TRUE;
    }

    /* ��Ϣ���봦�� */
    if (VOS_OK == CNAS_XREG_CSCHDataTrans(pstCschData))
    {
        CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_ORDERED_REGISTRATION);   /*ͨ���ڲ���Ϣ����ע��*/
    }

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvT57TimeOut_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XREG_GetRegStateAddr()->enT57MState = CNAS_XREG_TIMER_EXPIRED;

    /* ͨ�������ڲ���Ϣ����ע����� */
    CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_REGISTRATION_TYPE_BUTT);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvPrdRegTimeout_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XREG_REGSTATE_STRU                    *pstSysState;

    pstSysState = CNAS_XREG_GetRegStateAddr();

    if (0 < pstSysState->ulRemainderTimerLen)
    {
        if (CNAS_XREG_TIMER_MAX_LEN < pstSysState->ulRemainderTimerLen)
        {
            /* ����ע�ᶨʱ�� */
            CNAS_XREG_StartTime(TI_CNAS_XREG_TIMER_PERIOD_REG, CNAS_XREG_TIMER_MAX_LEN);

            pstSysState->ulRemainderTimerLen -= CNAS_XREG_TIMER_MAX_LEN;
        }
        else
        {
            /* ����ע�ᶨʱ�� */
            CNAS_XREG_StartTime(TI_CNAS_XREG_TIMER_PERIOD_REG, pstSysState->ulRemainderTimerLen);

            pstSysState->ulRemainderTimerLen = 0;
        }

        return VOS_TRUE;
    }

    pstSysState->enRegTimerState = CNAS_XREG_TIMER_EXPIRED;

    /* ͨ�������ڲ���Ϣ����ע����� */
    CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_REGISTRATION_TYPE_BUTT);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvTMSITimeOut_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XREG_ClearList();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvZoneListTimeout_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XREG_RefreshLists();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvProVerInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CAS_CNAS_1X_P_VER_IND_STRU         *pstPVerMsg;

    pstPVerMsg = (CAS_CNAS_1X_P_VER_IND_STRU*)pstMsg;

    CNAS_CCB_SetHighProRev((CNAS_CCB_1X_CAS_P_REV_ENUM_UINT8)pstPVerMsg->enProtocolRev);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvStatusInd_L1Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    CNAS_XREG_REGSTATE_STRU            *pstRegState;

    /* ȥע����� */
    CNAS_XREG_SndRrmDeRegisterInd(RRM_PS_TASK_TYPE_1X_REGISTER);

    pstRegState = CNAS_XREG_GetRegStateAddr();

    /* ͨ�������ڲ���Ϣ����ע����� */
    CNAS_XREG_SndRegReqMsg(pstRegState->enCurRegType);

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvOmRegReportReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    OM_NAS_CDMA_REG_INFO_REPORT_REQ_STRU       *pstOmReqMsg;
    NAS_OM_RESULT_ENUM_UINT32                   enResult;
    NAS_OM_CDMA_REG_TYPE_ENUM_UINT32            enRegTypeMntn;

    pstOmReqMsg = (OM_NAS_CDMA_REG_INFO_REPORT_REQ_STRU*)pstMsg;

    /* ���浱ǰOM�ϱ�״̬ */
    if (NAS_OM_REPORT_BUTT > pstOmReqMsg->enReportType)
    {
        CNAS_XREG_SetOmRegReportFlag(pstOmReqMsg->enReportType);

        enResult = NAS_OM_RESULT_NO_ERROR;
    }
    else
    {
        enResult = NAS_OM_RESULT_INCORRECT_PARAMETERS;
    }

    CNAS_XREG_SndOmRegReportCnf(enResult, pstOmReqMsg);

    /* �����ϱ���ά�ɲ���Ϣ */
    enRegTypeMntn = CNAS_XREG_GetRegStateAddr()->enRegTypeMntn;

    if (NAS_OM_CDMA_REG_MODE_BUTT != enRegTypeMntn)
    {
        CNAS_XREG_SndOmRegTypeReportInd(enRegTypeMntn);
    }

    if (CNAS_XREG_SYSMSG_CURRENT == CNAS_XREG_GetSysMsgAddr()->enAvailable)
    {
        if (PS_TRUE == CNAS_XREG_GetSysMsgAddr()->stSysInfo.enRegInfoIncl)
        {
            CNAS_XREG_SndOmRegPRDReportInd(CNAS_XREG_GetSysMsgAddr()->stSysInfo.stRegInfo.ucRegPeriod);
        }
    }

    CNAS_XREG_SndOmRegListReportInd();

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvUsimRefreshInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    USIMM_STKREFRESH_IND_STRU          *pstRefreshInd;
    CNAS_CCB_CARD_STATUS_ENUM_UINT8     enCsimCardStatus;
    VOS_UINT32                          ulLoop;
    VOS_UINT16                          usEfNum;        /* ���µ��ļ��ĸ��� */
    USIMM_CUIM_FILEID_ENUM_UINT16       enFileTermId;
    USIMM_CUIM_FILEID_ENUM_UINT16       enFileSsciId;

    enCsimCardStatus = CNAS_CCB_GetCsimCardStatus();

    if (CNAS_CCB_CARD_STATUS_ABSENT == enCsimCardStatus)
    {
        /* ������λ */
        return VOS_TRUE;
    }

    pstRefreshInd = (USIMM_STKREFRESH_IND_STRU *)pstMsg;

    /* ���������������ػ��ٿ������� */
    if (USIMM_REFRESH_FILE_LIST != pstRefreshInd->enRefreshType)
    {
        return VOS_TRUE;
    }

    /* ��Refresh���ȫ�ֱ��� */
    CNAS_XREG_ClearRefreshCardFilesCtx();

    if (CNAS_CCB_CARD_STATUS_CSIM_PRESENT == enCsimCardStatus)
    {
        enFileTermId  = USIMM_CSIM_EFTERM_ID;
        enFileSsciId  = USIMM_CSIM_EFSSCI_ID;
    }
    else
    {
        enFileTermId  = USIMM_CDMA_EFTERM_ID;
        enFileSsciId  = USIMM_CDMA_EFSSCI_ID;
    }

    usEfNum = pstRefreshInd->usEfNum;

    for (ulLoop = 0; ulLoop < usEfNum; ulLoop++)
    {
        if (enFileTermId == pstRefreshInd->astEfId[ulLoop].usFileId)
        {
            CNAS_XREG_SetReadCardFilesIndFlg(CNAS_XREG_REFRESH_CARD_FILE_TERM_FLG);

            /* ��ȡ Term file */
            CNAS_XREG_SndCardReadFileReq(enFileTermId);
        }

        if (enFileSsciId == pstRefreshInd->astEfId[ulLoop].usFileId)
        {
            CNAS_XREG_SetReadCardFilesIndFlg(CNAS_XREG_REFRESH_CARD_FILE_SSCI_FLG);

            /* ��ȡ Ssci file */
            CNAS_XREG_SndCardReadFileReq(enFileSsciId);
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvUsimReadFileCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    USIMM_READFILE_CNF_STRU            *pstFile;
    USIMM_DEF_FILEID_ENUM_UINT32        enEfId;

    /* SwitchOn �����ı��λ��Ϊ�գ�����Ϊ��SwitchOn�����ļ� */
    if (CNAS_XREG_READ_CARD_FILE_FLG_NULL != CNAS_XREG_GetWaitCardFilesCnfFlg_SwitchOn())
    {
        return VOS_FALSE;
    }

    pstFile = (USIMM_READFILE_CNF_STRU*)pstMsg;

    if (VOS_OK != USIMM_ChangePathToDefFileID(USIMM_CDMA_APP, pstFile->stFilePath.ulPathLen, pstFile->stFilePath.acPath, &enEfId))
    {
        return VOS_TRUE;
    }

    CNAS_XREG_ClearReadCardFilesIndFlg(enEfId);

    /* �ļ������Ҫ����ע����� */
    CNAS_XREG_ProcCardReadFileCnf(pstMsg);

    /* �ļ���ȡ��� */
    if (CNAS_XREG_REFRESH_CARD_FILE_FLG_NULL != CNAS_XREG_GetReadCardFilesIndFlg())
    {
        /* �ļ�δ��ȡ��ϼ����ȴ� */
        return VOS_TRUE;
    }

    /* �ļ������б䶯������ע����� */
    if (CNAS_XREG_CARD_FILE_CHANGE_FLG_NULL != CNAS_XREG_GetCardFilesChangeFlg())
    {
        /* ����CASû�ж��ļ�������д�������XREG���㷢����ע�ᣬ��������Ҳ�ϵ�����
            ����������δʵ��ע����� */
        //CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_REGISTRATION_TYPE_BUTT);
    }

    return VOS_TRUE;
}


VOS_UINT32 CNAS_XREG_RcvUeSrvState_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    XSD_XREG_NORMAL_SERVICE_IND_STRU   *pstSrvInd;

    pstSrvInd = (XSD_XREG_NORMAL_SERVICE_IND_STRU*)pstMsg;

    /* �ж��Ƿ���Կ���ע�� */
    if (VOS_TRUE == CNAS_XREG_IsPowerupReg(pstSrvInd->ulRatMode, pstSrvInd->ulMcc))
    {
        /* ���統ǰ���ڵȴ�������ʱ����ʱ���ճ�ʱ���� */
        if (CNAS_XREG_TIMER_RUNNING == CNAS_XREG_GetRegStateAddr()->enT57MState)
        {
            CNAS_StopTimer(UEPS_PID_XREG, TI_CNAS_XREG_TIMER_T57M, 0);

            CNAS_XREG_GetRegStateAddr()->enT57MState = CNAS_XREG_TIMER_EXPIRED;

            /* ͨ�������ڲ���Ϣ����ע����� */
            CNAS_XREG_SndRegReqMsg(CAS_CNAS_1X_REGISTRATION_TYPE_BUTT);
        }
    }

    /* �ж��Ƿ�ȥ�����Ҫ���ע����Ϣ */
    if (VOS_TRUE == CNAS_XREG_IsRoamReg(pstSrvInd->ulRatMode, pstSrvInd->ulMcc))
    {
        CNAS_XREG_GetRegStateAddr()->enRegisterFlag = CNAS_XREG_VAR_NO;

        CNAS_XREG_ClearList();
    }

    return VOS_TRUE;
}

#if (FEATURE_PTM == FEATURE_ON)


VOS_UINT32 CNAS_XREG_IsErrLogNeedReport(
    VOS_UINT16                          usLevel
)
{
    VOS_UINT8                           ucReportAlarmLevel;

    ucReportAlarmLevel = (VOS_UINT8)usLevel;

    /* �����ϱ��ܿؿ��� */
    if (VOS_FALSE == NAS_ERRLOG_CCB_GetXregMainActiveReportFlag())
    {
        return VOS_FALSE;
    }

    if (VOS_TRUE == NAS_ERRLOG_CCB_GetCdmaXregResltReportFlag())
    {
        if ((NAS_ERRLOG_CCB_GetXregMainActiveReportLevel() < ucReportAlarmLevel)
         || (NAS_ERR_LOG_CTRL_LEVEL_NULL == ucReportAlarmLevel))
        {
            /* ģ��log����usLevel�����û����õ�log�ϱ������usLevel��Ч������Ҫ�ϱ� */
            return VOS_FALSE;
        }
    }
    else
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID CNAS_XREG_ErrLogRpt(
    NAS_ERR_LOG_XREG_REGISTRATION_TYPE_ENUM_UINT8  enRegType,
    NAS_ERR_LOG_XREG_FAIL_REASON_ENUM_UINT8        enRegFailReason
)
{
    VOS_UINT16                               usLevel;
    VOS_UINT32                               ulIsLogRecord;

    /* ��ѯ��ӦAlarm Id�Ƿ���Ҫ��¼�쳣��Ϣ */
    usLevel       = NAS_GetErrLogAlmLevel(NAS_ERR_LOG_ALM_XREG_FAIL_INFO);
    ulIsLogRecord = CNAS_XREG_IsErrLogNeedReport(usLevel);

    /* Errlog����Ҫ�ϱ� */
    if (VOS_FALSE == ulIsLogRecord)
    {
        return;
    }

    NAS_ERRLOG_CCB_SetXregFailType(enRegType);

    NAS_ERRLOG_CCB_SetXregFailReason(enRegFailReason);

    CNAS_XREG_ErrLogRptAP();
}


VOS_VOID CNAS_XREG_ErrLogRptAP(VOS_VOID)
{
    VOS_UINT32                                ulLength;
    NAS_ERR_LOG_XREG_RESULT_EVENT_STRU       *pstErrLogXregFail;

    pstErrLogXregFail =  (NAS_ERR_LOG_XREG_RESULT_EVENT_STRU *)PS_MEM_ALLOC(UEPS_PID_XREG,
                                                    sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU));
    if (VOS_NULL_PTR == pstErrLogXregFail)
    {
        CNAS_ERROR_LOG(UEPS_PID_XREG, "CNAS_XREG_ErrLogRptAP:ERROR:Alloc Mem Fail.");
        return;
    }

    NAS_MEM_SET_S(pstErrLogXregFail,
                  sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU),
                  0,
                  sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU));

    NAS_MEM_CPY_S(&(pstErrLogXregFail->stXregErrLogCtx),
                  sizeof(NAS_ERR_LOG_XREG_CTX_STRU),
                  NAS_ERRLOG_CCB_GetXregFailCtxAddr(),
                  sizeof(NAS_ERRLOG_CCB_XREG_FAIL_INFO_STRU));

    ulLength = sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU);

    /*  ���CHR��Ϣ��ͷ  */
    CNAS_COMM_BULID_ERRLOG_HEADER_INFO(&(pstErrLogXregFail->stHeader),
                                       CNAS_CCB_GetCdmaModeModemId(),
                                       NAS_ERR_LOG_ALM_XREG_FAIL_INFO,
                                       NAS_GetErrLogAlmLevel(NAS_ERR_LOG_ALM_XREG_FAIL_INFO),
                                       VOS_GetSlice(),
                                       (ulLength - sizeof(OM_ERR_LOG_HEADER_STRU)));
    /* OM��Ϣ���� */
    NAS_ERRLOG_CCB_SndAcpuOmFaultErrLogInd(pstErrLogXregFail,
                                           ulLength,
                                           FAULT_ID_1X_REG_FAIL,
                                           (VOS_UINT8)NAS_ERR_LOG_ALM_XREG_FAIL_INFO,
                                           UEPS_PID_XREG);

    PS_MEM_FREE(UEPS_PID_XREG, pstErrLogXregFail);

    /* ���Errlog��¼ȫ�ֱ��� */
    CNAS_XREG_InitErrLogXregFailInfo();

    return;
}


VOS_VOID CNAS_XREG_InitErrLogXregFailInfo(VOS_VOID)
{
    NAS_ERRLOG_CCB_InitXregFailCtx();

    return;
}


VOS_VOID CNAS_XREG_ErrLogRptCnf(MsgBlock *pstMsg)
{
    VOS_UINT32                               ulLength;
    OM_ERR_LOG_REPORT_REQ_STRU              *pstErrLogReq;
    NAS_ERR_LOG_XREG_RESULT_EVENT_STRU      *pstErrLogXregFail;

    pstErrLogReq = (OM_ERR_LOG_REPORT_REQ_STRU*)pstMsg;

    pstErrLogXregFail =  (NAS_ERR_LOG_XREG_RESULT_EVENT_STRU *)PS_MEM_ALLOC(UEPS_PID_XREG,
                                                    sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU));
    if (VOS_NULL_PTR == pstErrLogXregFail)
    {
        CNAS_ERROR_LOG(UEPS_PID_XREG, "CNAS_XREG_ErrLogRptCnf:ERROR:Alloc Mem Fail.");
        return;
    }

    NAS_MEM_SET_S(pstErrLogXregFail, sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU), 0, sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU));

    NAS_MEM_CPY_S(&(pstErrLogXregFail->stXregErrLogCtx),
                  sizeof(NAS_ERR_LOG_XREG_CTX_STRU),
                  NAS_ERRLOG_CCB_GetXregFailCtxAddr(),
                  sizeof(NAS_ERRLOG_CCB_XREG_FAIL_INFO_STRU));

    ulLength = sizeof(NAS_ERR_LOG_XREG_RESULT_EVENT_STRU);

    /*  ���CHR��Ϣ��ͷ  */
    CNAS_COMM_BULID_ERRLOG_HEADER_INFO(&(pstErrLogXregFail->stHeader),
                                       CNAS_CCB_GetCdmaModeModemId(),
                                       NAS_ERR_LOG_ALM_XREG_FAIL_INFO,
                                       NAS_GetErrLogAlmLevel(NAS_ERR_LOG_ALM_XREG_FAIL_INFO),
                                       VOS_GetSlice(),
                                       (ulLength - sizeof(OM_ERR_LOG_HEADER_STRU)));
    /* OM��Ϣ���� */
    NAS_ERRLOG_CCB_SndAcpuOmFaultErrLogCnf(pstErrLogXregFail,
                                           ulLength,
                                           pstErrLogReq->ucFaultID,
                                           (VOS_UINT8)NAS_ERR_LOG_ALM_XREG_FAIL_INFO,
                                           UEPS_PID_XREG);

    PS_MEM_FREE(UEPS_PID_XREG, pstErrLogXregFail);

    /* ���Errlog��¼ȫ�ֱ��� */
    CNAS_XREG_InitErrLogXregFailInfo();

    return;

}


VOS_VOID CNAS_XREG_ErrLogRptProc(MsgBlock *pstMsg)
{
    OM_ERR_LOG_REPORT_REQ_STRU         *pstErrLogReq = VOS_NULL_PTR;

    pstErrLogReq = (OM_ERR_LOG_REPORT_REQ_STRU*)pstMsg;

    switch (pstErrLogReq->ucFaultID)
    {
        case FAULT_ID_1X_REG_FAIL:

            (VOS_VOID)CNAS_XREG_ErrLogRptCnf(pstMsg);

            break;

        default:
            CNAS_ERROR_LOG(UEPS_PID_XREG, "CNAS_XREG_ErrLogRptProc: Fault id err");
            break;
    }

    return;
}


VOS_UINT32 CNAS_XREG_RcvErrLogMsg_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    OM_ERR_LOG_REPORT_REQ_STRU  *pstErrLogReq = VOS_NULL_PTR;

    pstErrLogReq = (OM_ERR_LOG_REPORT_REQ_STRU*)pstMsg;

    switch (pstErrLogReq->ulMsgName)
    {
        case ID_OM_ERR_LOG_REPORT_REQ:

            CNAS_XREG_ErrLogRptProc(pstMsg);

            break;

        default:
            CNAS_ERROR_LOG(UEPS_PID_XREG, "CNAS_XREG_RcvErrLogMsg_PreProc: msg id err");
            break;
    }

    return VOS_TRUE;
}
#endif
/*lint -restore*/
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


