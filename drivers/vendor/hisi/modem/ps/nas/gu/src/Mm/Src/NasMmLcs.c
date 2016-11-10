

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "MM_Ext.h"
#include "NasMmlLib.h"
#include "MmSuspend.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NAS_MM_LCS_C
/*lint +e767*/


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 NAS_MM_RcvLcsEstReq_PreProc(VOS_VOID* pMsg)
{
    LCS_MM_EST_REQ_STRU                *pstEstReqMsg  = VOS_NULL_PTR;

    pstEstReqMsg    = (LCS_MM_EST_REQ_STRU *)pMsg;

    /* ��Ϣ�Ϸ��Լ�� */
    /* Ti Value����Ϊ'111' */
    if (ID_LCS_MM_EST_REQ != pstEstReqMsg->ulMsgId)
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsEstReq_PreProc: Message legitimacy check failed!");
        return VOS_FALSE;
    }

    /* Ti�Ƿ����� */
    if ( (pstEstReqMsg->ulTi < MM_CONST_NUM_8)
      || (pstEstReqMsg->ulTi > MM_CONST_NUM_14) )
    {
        /* ֪ͨLCS����ʧ�� */
        NAS_MM_SndLcsRelInd(pstEstReqMsg->ulTi, NAS_MMCM_REL_CAUSE_MM_INTER_ERR_TI_INVALID);
        return VOS_FALSE;
    }

    /* ���ɹ�����¼Ti */
    NAS_MM_SetLcsEstReqTi(pstEstReqMsg->ulTi);

    /* ����LCSҵ����״̬ */
    NAS_MML_SetMoLcsStatus(NAS_MML_MO_LCS_SETUP_START);

    return VOS_TRUE;
}


VOS_VOID NAS_MM_RcvLcsEstReq_CSFB(VOS_VOID)
{
    /* �Ѿ�����CSFB���̣���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsCsfbServiceStatusExist())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CSFB_ALREADY_EXISTS);
        return;
    }

    /* ���潨������ */
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

    /* ����CSFB״̬ */
    NAS_MM_ProcCsfbServiceStatusChange(NAS_MML_GetCsfbServiceStatus(), NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST);

    /* ��¼CSFBҵ������ʼ */
    NAS_MML_SetMoLcsStatus(NAS_MML_CSFB_MO_LCS_SETUP_START);

    /* ��L����CSFB���̿�ʼ����CSFB�Ƿ���Է�����ж���LMM���� */
    /* �����������,L��ظ�LMM_MM_CSFB_SERVICE_END_IND */
    NAS_MM_SndLmmCsfbServiceStartNotify();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_Null(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* CS�������DETACH״̬���ظ�LCS����ʧ�� */
    NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_DETACH);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdleNoCellAvaliable(VOS_VOID* pMsg)
{
    VOS_UINT8                           ucSimCsRegStatus;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ���뼼��ΪLTE only�����LCS�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY);
        return;
    }

    /* �����ǰפ��LTE���򴥷�CSFB���� */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        NAS_MM_RcvLcsEstReq_CSFB();
        return;
    }
#endif

    /* �����ǰNO RF����Ҫ����ҵ������֪ͨMMC�������� */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ( (VOS_FALSE == NAS_MML_GetRfAvailFlg())
      && (VOS_TRUE == ucSimCsRegStatus) )
    {
        NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);
        Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);
        return;
    }

    /* �������֪ͨLCS����ʧ�� */
    NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_OUT_OF_COVERAGE);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdlePlmnSearch(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ���뼼��ΪLTE only�����LCS�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY);
        return;
    }
#endif

    /* �������֪ͨLCS����ʧ�� */
    NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_SEARCHING_NETWORK);

    return;

}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdleNormalService(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ���뼼��ΪLTE only�����LCS�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY);
        return;
    }

    /* �����ǰפ��LTE���򴥷�LCS CSFB���� */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        NAS_MM_RcvLcsEstReq_CSFB();
        return;
    }
#endif

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* ��¼�յ���������ʱ��״̬������MM�������� */
    NAS_MM_SetStaOfRcvXXEstReq(NAS_MM_GetMmState());
    NAS_MM_EstMmConnection(MM_CONN_CTRL_LCS);

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdleLimitedService(VOS_VOID* pMsg)
{
    NAS_MML_MISCELLANEOUS_CFG_INFO_STRU    *pstMiscellaneousCfgInfo = VOS_NULL_PTR;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* CS����Ч����ظ�LCS����ʧ�� */
    if (VOS_FALSE  == NAS_MML_GetSimCsRegStatus())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_SIM_INVALID);
        return;
    }

    /* �����ǰPS ONLYʱ֧��CSҵ��NVδ������PS ONLY��ظ�LCS����ʧ�� */
    pstMiscellaneousCfgInfo = NAS_MML_GetMiscellaneousCfgInfo();
    if ( (NAS_MML_MS_MODE_PS_ONLY != NAS_MML_GetMsMode())
      || (NAS_MMC_NV_ITEM_DEACTIVE == pstMiscellaneousCfgInfo->ucPsOnlyCsServiceSupportFlg) )
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_DETACH);
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ���뼼��ΪLTE only�����LCS�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY);
        return;
    }

    /* �����ǰפ��LTE���򴥷�LCS��CSFB���� */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        NAS_MM_RcvLcsEstReq_CSFB();
        return;
    }
#endif

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* ����LCSҵ�������󣬴���CSע������ */
    g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);
    NAS_MML_SetCsAttachAllowFlg(VOS_TRUE);

    if (VOS_TRUE == NAS_MM_IsClassAAndNmoIMode(g_MmGlobalInfo.ucNtMod))
    {
        /* ����ģʽI��֪ͨGMM����ATTACH���� */
        NAS_MM_SndGmmRegisterInit();
    }
    else
    {
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsEstReq_MmIdleLimitedService: NOT A+I trigger lau");

        /* ����LU���ͣ�����LAU���� */
        g_MmGlobalInfo.LuInfo.ucLuType      = MM_IE_LUT_NORMAL_LU;
        g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = 0;
        Mm_ComLuOnly();
    }

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdleAttempToUpdate(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* �����ǰפ��LTE���򴥷�LCS��CSFB���� */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        NAS_MM_RcvLcsEstReq_CSFB();
        return;
    }
#endif

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    if (VOS_TRUE == NAS_MML_IsPlmnSupportDam(NAS_MML_GetCurrCampPlmnId()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_UE_INVALID_STATE);
        return;
    }

    /* ����LCSҵ�������󣬴���CSע������ */
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

    if (VOS_TRUE == NAS_MM_IsClassAAndNmoIMode(g_MmGlobalInfo.ucNtMod))
    {
        /* ����ģʽI��֪ͨGMM����ATTACH���� */
        NAS_MM_SndGmmRegisterInit();
    }
    else
    {
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsEstReq_MmIdleAttempToUpdate: NOT A+I trigger lau");

        /* ����LU���ͣ�����LAU���� */
        g_MmGlobalInfo.LuInfo.ucLuType      = MM_IE_LUT_NORMAL_LU;
        g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = 0;
        Mm_ComLuOnly();
    }

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdleLauNeeded(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ���뼼��ΪLTE only�����LCS�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY);
        return;
    }

    /* �����ǰפ��LTE���򴥷�LCS CSFB���� */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        NAS_MM_RcvLcsEstReq_CSFB();
        return;
    }
#endif

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* ��ǰCS�����״̬����������ظ�LCS����ʧ�� */
    if (MM_COM_SRVST_NORMAL_SERVICE != NAS_MM_GetLauUptNeededCsSrvStatus())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* ��¼�յ���������ʱ��״̬������MM�������� */
    g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
    NAS_MM_EstMmConnection(MM_CONN_CTRL_LCS);

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdleNoImsi(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* SIM����Ч���ظ�LCS����ʧ�� */
    NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_SIM_INVALID);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForOutgoingMmConn(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* ����ҵ���ͻ�Ƿ����� */
    if (VOS_FALSE == Mm_IsMultiSrvCollisionAllow(MM_CONN_CTRL_LCS))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_SERVICE_CONGESTION);
        return;
    }

    /* �л����LCS������������������tiֵ��ͬ,��ظ�LCS����ʧ�� */
    if ( (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].RcvXXEstReq.ucFlg)
      && (NAS_MM_GetLcsEstReqTi() != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].RcvXXEstReq.ucFlg) )
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(),
                       NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CC_CONN_REQ_EXIST);
        return;
    }

    /* ����LCSҵ�������� */
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmConnectionActive(VOS_VOID* pMsg)
{
    MM_MSG_CM_SVC_REQ_STRU              stCmSvcReq;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* ��������ҵ������ʱһ������LCS���� */
    /* ����Ti����·�Ѿ���������ֱ�Ӹ�LCS�ظ������ɹ� */
    if (VOS_TRUE == NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, NAS_MM_GetLcsEstReqTi()))
    {
        NAS_MM_SndLcsEstCnf(NAS_MM_GetLcsEstReqTi());
    }
    else
    {
        /* ����û�н����򴥷�MM���� */
        NAS_MM_SetStaOfRcvXXEstReq(NAS_MM_GetMmState());

        /* ������Ϣ */
        NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

        /* ���CM SERVICE REQUEST�ṹ�壬�����͵������ */
        /* ����LCS֧��LMU��λ����ʱ��Ҫ�ж��޸�CMSvcTypeΪMM_IE_CM_SVC_TYPE_LCS */
        Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_SS_ACT, MM_FALSE, &stCmSvcReq);
        Mm_ComMsgCmSvcReqSnd(&stCmSvcReq);

        /* ��¼���ڽ�����MM���ӵ�PD */
        NAS_MM_SetMMConnEstingPD(MM_IE_PD_LOCATION_SRVC);

        /* Ǩ�Ƶ�WAIT FOR ADDITIONAL OUTGING MM CONNECTION״̬������T3230��ʱ�� */
        Mm_ComSetMmState(WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION);
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsEstReq_MmConnectionActive: MM STATUS is WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION");
        Mm_TimerStart(MM_TIMER_T3230);
    }

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForNetworkCmd(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR OUTGOING MM CONNECTION״̬ */
    NAS_MM_RcvLcsEstReq_WaitForOutgoingMmConn(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForRrConn(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR OUTGOING MM CONNECTION״̬ */
    NAS_MM_RcvLcsEstReq_WaitForOutgoingMmConn(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForReestReq(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR OUTGOING MM CONNECTION״̬ */
    NAS_MM_RcvLcsEstReq_WaitForOutgoingMmConn(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForReestCnf(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR OUTGOING MM CONNECTION״̬ */
    NAS_MM_RcvLcsEstReq_WaitForOutgoingMmConn(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForAdditionalMmConn(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR OUTGOING MM CONNECTION״̬ */
    NAS_MM_RcvLcsEstReq_WaitForOutgoingMmConn(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_LauPending(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ���뼼��ΪLTE only�����LCS�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY);
        return;
    }
#endif

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* �л����LCS������������������tiֵ��ͬ,��ظ�LCS����ʧ�� */
    if ( (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].RcvXXEstReq.ucFlg)
      && (NAS_MM_GetLcsEstReqTi() != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].RcvXXEstReq.ucFlg) )
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(),
                       NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CC_CONN_REQ_EXIST);
        return;
    }

    /* ����LCSҵ�������� */
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_ImsiDetachPending(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ���뼼��ΪLTE only�����LCS�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_NOT_SUPPORT_CS_CALL_S1_MODE_ONLY);
        return;
    }

    /* �����ǰפ��LTE���򴥷�LCS��CSFB���� */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        NAS_MM_RcvLcsEstReq_CSFB();
        return;
    }
#endif

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* �л����LCS������������������tiֵ��ͬ,��ظ�LCS����ʧ�� */
    if ( (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].RcvXXEstReq.ucFlg)
      && (NAS_MM_GetLcsEstReqTi() != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].RcvXXEstReq.ucFlg) )
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(),
                       NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CC_CONN_REQ_EXIST);
        return;
    }

    /* ����LCSҵ�������� */
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForAttach(VOS_VOID* pMsg)
{
    NAS_MML_MISCELLANEOUS_CFG_INFO_STRU    *pstMiscellaneousCfgInfo = VOS_NULL_PTR;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* �����ǰPS ONLYʱ֧��CSҵ��NVδ�����ظ�LCS����ʧ�� */
    pstMiscellaneousCfgInfo = NAS_MML_GetMiscellaneousCfgInfo();
    if ( (NAS_MML_MS_MODE_PS_ONLY == NAS_MML_GetMsMode())
      && (NAS_MMC_NV_ITEM_DEACTIVE == pstMiscellaneousCfgInfo->ucPsOnlyCsServiceSupportFlg) )
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_DETACH);
        return;
    }

    /* ����LCSҵ�������󣬴���CSע������ */
    g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);
    NAS_MML_SetCsAttachAllowFlg(VOS_TRUE);

    if (VOS_TRUE == NAS_MM_IsClassAAndNmoIMode(g_MmGlobalInfo.ucNtMod))
    {
        /* ����ģʽI��֪ͨGMM����ATTACH���� */
        NAS_MM_SndGmmRegisterInit();
    }
    else
    {
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsEstReq_WaitForAttach: NOT A+I trigger lau");

        /* ����LU���ͣ�����LAU���� */
        g_MmGlobalInfo.LuInfo.ucLuType      = MM_IE_LUT_NORMAL_LU;
        g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = 0;
        Mm_ComLuOnly();
    }

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForRrConnLau(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* ����LCSҵ�������� */
    g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_LauInitiated(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR RR CONNECTION (LOCATION UPDATING)״̬ */
    NAS_MM_RcvLcsEstReq_WaitForRrConnLau(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_LauRejected(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR RR CONNECTION (LOCATION UPDATING)״̬ */
    NAS_MM_RcvLcsEstReq_WaitForRrConnLau(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_WaitForRrConnImsiDetach(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬNULL״̬ */
    NAS_MM_RcvLcsEstReq_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_ImsiDetachInitiated(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬNULL״̬ */
    NAS_MM_RcvLcsEstReq_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_ProcCmServicePrompt(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬWAIT FOR RR CONNECTION (LOCATION UPDATING)״̬ */
    NAS_MM_RcvLcsEstReq_WaitForRrConnLau(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_RrConnRelNotAllow(VOS_VOID* pMsg)
{
    MM_MSG_CM_SVC_REQ_STRU              stCmSvcReq;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* ��ǰС��CS������ҵ�����ޣ���ظ�LCS����ʧ�� */
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_CS_ACCESS_BAR);
        return;
    }

    /* ��ǰ�������޷���״̬����ظ�LCS����ʧ�� */
    if (MM_TRUE == Mm_ComJudgeLimitSvc())
    {
        NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_UE_INVALID_STATE);
        return;
    }

    /* ֹͣT3241��ʱ�� */
    Mm_TimerStop(MM_TIMER_T3241);

    /* ����MM���� */

    /* ������Ϣ */
    NAS_MM_UpdateConnCtrlInfo(MM_CONN_CTRL_LCS);

    /* ���CM SERVICE REQUEST�ṹ�壬�����͵������ */
    /* ����LCS֧��LMU��λ����ʱ��Ҫ�ж��޸�CMSvcTypeΪMM_IE_CM_SVC_TYPE_LCS */
    Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_SS_ACT, MM_FALSE, &stCmSvcReq);
    Mm_ComMsgCmSvcReqSnd(&stCmSvcReq);

    /* ��¼���ڽ�����MM���ӵ�PD */
    NAS_MM_SetMMConnEstingPD(MM_IE_PD_LOCATION_SRVC);

    /* Ǩ�Ƶ�WAIT FOR OUTGING MM CONNECTION״̬������T3230��ʱ�� */
    Mm_ComSetMmState(WAIT_FOR_OUTGOING_MM_CONNECTION);
    NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsEstReq_MmConnectionActive: MM STATUS is WAIT_FOR_OUTGOING_MM_CONNECTION");
    Mm_TimerStart(MM_TIMER_T3230);

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_TestContrlActive(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬNULL״̬ */
    NAS_MM_RcvLcsEstReq_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmInterRatChange(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* �ڲ����뼼���л��У��ظ�LCS����ʧ�� */
    NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_INTER_RAT_SYSTEM_CHANGE);

    return;
}


VOS_VOID NAS_MM_RcvLcsEstReq_MmIdleECallInactive(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsEstReq_PreProc(pMsg))
    {
        return;
    }

    /* eCall Inactiveʱ�ܾ�LCS MO-LRҵ�񣬻ظ�LCS����ʧ�� */
    NAS_MM_SndLcsRelInd(NAS_MM_GetLcsEstReqTi(), NAS_MMCM_REL_CAUSE_MM_INTER_ERR_ECALL_INACTIVE);

    return;
}


VOS_UINT32 NAS_MM_RcvLcsDataReq_PreProc(VOS_VOID* pMsg)
{
    LCS_MM_DATA_REQ_STRU               *pstDataReqMsg   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;

    pstDataReqMsg   = (LCS_MM_DATA_REQ_STRU *)pMsg;
    ulResult        = VOS_TRUE;

    /* ��Ϣ�Ϸ��Լ�� */
    if ( (ID_LCS_MM_DATA_REQ != pstDataReqMsg->ulMsgId)
      || (MM_CONST_NUM_15 < pstDataReqMsg->ulTi) )
    {
        ulResult = VOS_FALSE;
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsDataReq_PreProc: Message legitimacy check failed!");
    }

    /* �ж�Ti��Ӧ��MM�����Ƿ���ڣ���������֪ͨLCS�ͷ����� */
    if (VOS_TRUE != NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, pstDataReqMsg->ulTi))
    {
        ulResult = VOS_FALSE;
        NAS_MM_SndLcsRelInd(pstDataReqMsg->ulTi, NAS_MMCM_REL_CAUSE_MM_INTER_ERR_TI_INVALID);
    }

    NAS_SS_ReportM2NOtaMsg((SS_MSG_STRU *)&pstDataReqMsg->stLcsMsg);

    return ulResult;
}


VOS_VOID NAS_MM_RcvLcsDataReq_MmConnectionActive(VOS_VOID* pMsg)
{
    LCS_MM_DATA_REQ_STRU               *pstLcsDataReq = VOS_NULL_PTR;

    pstLcsDataReq = (LCS_MM_DATA_REQ_STRU *)pMsg;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsDataReq_PreProc(pMsg))
    {
        return;
    }

    /* ͨ��RRMM_DATA_REQ��Ϣ��LCS�տ����������� */
    Mm_SndRrDataReq(pstLcsDataReq->stLcsMsg.ulLcsMsgSize,
        &pstLcsDataReq->stLcsMsg.aucLcsMsg[0],
        RRC_NAS_MSG_PRIORTY_HIGH);

    return;
}


VOS_VOID NAS_MM_RcvLcsDataReq_WaitForAdditionalMmConn(VOS_VOID* pMsg)
{
    LCS_MM_DATA_REQ_STRU               *pstLcsDataReq = VOS_NULL_PTR;

    pstLcsDataReq = (LCS_MM_DATA_REQ_STRU *)pMsg;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsDataReq_PreProc(pMsg))
    {
        return;
    }

    /* ͨ��RRMM_DATA_REQ��Ϣ��LCS�տ����������� */
    Mm_SndRrDataReq(pstLcsDataReq->stLcsMsg.ulLcsMsgSize,
        &pstLcsDataReq->stLcsMsg.aucLcsMsg[0],
        RRC_NAS_MSG_PRIORTY_HIGH);

    return;
}


VOS_VOID NAS_MM_RcvLcsDataReq_MmInterRatChange(VOS_VOID* pMsg)
{
    /* ��Ϣͷ�ṹ�������ж���Ϣ���� */
    MSG_HEADER_STRU                    *pstMsgHeader;

    /* ��ȡ��Ϣͷ */
    pstMsgHeader = (MSG_HEADER_STRU *)pMsg;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsDataReq_PreProc(pMsg))
    {
        return;
    }

    /* ���ú��� MM_SaveMsg ���л��洦�� */
    MM_SaveMsg(pMsg, g_MmSubLyrShare.MmShare.MmSuspendEvent, pstMsgHeader->ulLength + VOS_MSG_HEAD_LENGTH);

    return;
}


VOS_UINT32 NAS_MM_RcvLcsRelReq_PreProc(VOS_VOID* pMsg)
{
    LCS_MM_REL_REQ_STRU                *pstRelReqMsg   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;

    pstRelReqMsg    = (LCS_MM_REL_REQ_STRU *)pMsg;
    ulResult        = VOS_FALSE;

    /* ��Ϣ�Ϸ��Լ�� */
    /* Ti Value����Ϊ'111' */
    if ( (ID_LCS_MM_REL_REQ != pstRelReqMsg->ulMsgId)
      || (MM_CONST_NUM_14 < pstRelReqMsg->ulTi)
      || (MM_CONST_NUM_7 == pstRelReqMsg->ulTi) )
    {
        ulResult = VOS_FALSE;
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_PreProc: Message legitimacy check failed!");
    }
    else
    {
        /* ��¼LCS�ͷ���·�����Ti */
        ulResult = VOS_TRUE;
        NAS_MM_SetLcsRelReqTi(pstRelReqMsg->ulTi);
    }

    return ulResult;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsRelReq_PreProc(pMsg))
    {
        return;
    }

    /* ��ǰ��û�л����Ti�Ľ���������ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_IsCsServiceBuffered(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi()))
    {
        NAS_WARNING_LOG1(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable: Establish request not buffered! Ti:", NAS_MM_GetLcsRelReqTi());
        return;
    }

    /* ���LCS�������󻺴��־��MM������־ */
    NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ����LCSҵ���CSFB���̣������CSFB���ڱ�־ */
    if (NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST == NAS_MML_GetCsfbServiceStatus())
    {
        NAS_MM_ProcCsfbServiceStatusChange(NAS_MML_GetCsfbServiceStatus(), NAS_MML_CSFB_SERVICE_STATUS_NOT_EXIST);

        if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            /* ��ǰ���뼼��ΪLTE��֪ͨLMM��ֹCSFB���� */
            NAS_MM_SndLmmCsfbServiceAbortNotify();
        }
    }
#endif

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmIdlePlmnSearch(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsRelReq_PreProc(pMsg))
    {
        return;
    }

    /* ��ǰ��û�л����Ti�Ľ���������ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_IsCsServiceBuffered(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi()))
    {
        NAS_WARNING_LOG1(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable: Establish request not buffered! Ti:", NAS_MM_GetLcsRelReqTi());
        return;
    }

    /* ���LCS�������󻺴��־��MM������־ */
    NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ����LCSҵ���CSFB���̣������CSFB���ڱ�־ */
    if (NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST == NAS_MML_GetCsfbServiceStatus())
    {
        NAS_MM_ProcCsfbServiceStatusChange(NAS_MML_GetCsfbServiceStatus(), NAS_MML_CSFB_SERVICE_STATUS_NOT_EXIST);

        if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            /* ��ǰ���뼼��ΪLTE��֪ͨLMM��ֹCSFB���� */
            NAS_MM_SndLmmCsfbServiceAbortNotify();
        }

        /* ֪ͨMMC��ֹCSFB���� */
        NAS_MM_SndMmcPlmnSearchInd(MM_MMC_PLMN_SEARCH_TYPE_CSFB_END);
    }
#endif

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmIdleNormalService(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmIdleLimitedService(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmIdleAttempToUpdate(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmIdleLauNeeded(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmIdleNoImsi(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForOutgoingMmConn(VOS_VOID* pMsg)
{
    VOS_UINT8                           ucMmConnFlg;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsRelReq_PreProc(pMsg))
    {
        return;
    }

    if (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].ucMMConnEstingTI == NAS_MM_GetLcsEstReqTi())
    {
        /* �����Ti���ڽ���MM�������� */
        /* ���MM������Ǻ�������� */
        NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);

        /* ͣT3230��ʱ�� */
        Mm_TimerStop(MM_TIMER_T3230);

        /* �����緢��CM_SRV_ABORT���� */
        Mm_ComMsgCmSvcAbortSnd();

        /* ��¼���ص����յ�����MM��������ʱ��״̬ */
        NAS_MM_SetStaAfterWaitForNwkCmd(NAS_MM_GetStaOfRcvXXEstReq());

        /* �洢�������̼�ԭ��ֵ */
        Mm_ComSaveProcAndCauseVal( MM_MM_CONN_PROC, NAS_MML_REG_FAIL_CAUSE_NULL );

        /* ��¼Ǩ��֮ǰ��״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());

        ucMmConnFlg = Mm_ComMmConnExist();
        if ( (RRMM_LCS_STATE_START == NAS_MM_GetRrLcsState())
          && (MM_TRUE != ucMmConnFlg) )
        {
            /* Ǩ�Ƶ�RR CONNECTION RELEASE NOT ALLOWED״̬������T3241��ʱ�� */
            Mm_ComSetMmState(MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForOutgoingMmConn: MM STATUS is MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED.");
            Mm_TimerStart(MM_TIMER_T3241);
        }
        else
        {
            /* Ǩ�Ƶ�WAIT FOR NETWORK COMMAND״̬������T3240��ʱ�� */
            Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForOutgoingMmConn: MM STATUS is WAIT_FOR_NETWORK_COMMAND.");
            Mm_TimerStart(MM_TIMER_T3240);
        }
    }
    else if (VOS_TRUE == NAS_MM_IsCsServiceBuffered(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi()))
    {
        /* �����TiΪ����Ľ��������Ti����������� */
        NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForOutgoingMmConn: Invalid Transaction Id!");
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ����LCSҵ���CSFB���̣������CSFB���ڱ�־ */
    if (NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST == NAS_MML_GetCsfbServiceStatus())
    {
        NAS_MM_ProcCsfbServiceStatusChange(NAS_MML_GetCsfbServiceStatus(), NAS_MML_CSFB_SERVICE_STATUS_NOT_EXIST);
    }
#endif

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmConnectionActive(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsRelReq_PreProc(pMsg))
    {
        return;
    }

    /* ����Ti��MM�����Ƿ���ڣ������������Ti��MM���ӱ�־ */
    if (VOS_TRUE == NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi()))
    {
        NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi(), NAS_MM_CONN_NOT_EXIST);
    }

    /* ��ǰMM���Ӷ��Ѿ��ͷţ���Ǩ��MM״̬ΪWAIT FOR NETWORK COMMAND */
    if (MM_TRUE != Mm_ComMmConnExist())
    {
        /* ��¼���ص����յ�����MM��������ʱ��״̬ */
        NAS_MM_SetStaAfterWaitForNwkCmd(NAS_MM_GetStaOfRcvXXEstReq());

        /* �洢�������̼�ԭ��ֵ */
        Mm_ComSaveProcAndCauseVal(MM_MM_CONN_PROC, NAS_MML_REG_FAIL_CAUSE_NULL);

        /* ��¼Ǩ��֮ǰ��״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());

        if (RRMM_LCS_STATE_START == NAS_MM_GetRrLcsState())
        {
            /* Ǩ�Ƶ�RR CONNECTION RELEASE NOT ALLOWED״̬������T3241��ʱ�� */
            Mm_ComSetMmState(MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_MmConnectionActive: MM STATUS is MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED.");
            Mm_TimerStart(MM_TIMER_T3241);
        }
        else
        {
            /* Ǩ�Ƶ�WAIT FOR NETWORK COMMAND״̬������T3240��ʱ�� */
            Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_MmConnectionActive: MM STATUS is WAIT_FOR_NETWORK_COMMAND.");
            Mm_TimerStart(MM_TIMER_T3240);
        }
    }

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForNetworkCmd(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForRrConn(VOS_VOID* pMsg)
{
    VOS_UINT8                           ucMmConnFlg;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsRelReq_PreProc(pMsg))
    {
        return;
    }

    if (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].ucMMConnEstingTI == NAS_MM_GetLcsEstReqTi())
    {
        /* ͣ�������ʱ�� */
        Mm_TimerStop( MM_TIMER_PROTECT_SIGNALLING );

        /* ��¼Ǩ��֮ǰ��״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());

        if (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_LCS].ucRat == NAS_MML_GetCurrNetRatType())
        {
            /* ��������Ľ��뼼��δ�����仯 */
            /* ֪ͨ������ͷ���· */
            Mm_SndRrRelReq(RRC_CELL_UNBARRED);

            /* ��¼���ص����յ�����MM��������ʱ��״̬ */
            NAS_MM_SetStaAfterWaitForNwkCmd(NAS_MM_GetStaOfRcvXXEstReq());

            /* �洢�������̼�ԭ��ֵ */
            Mm_ComSaveProcAndCauseVal(MM_MM_CONN_PROC, NAS_MML_REG_FAIL_CAUSE_NULL);

            ucMmConnFlg = Mm_ComMmConnExist();
            if ( (RRMM_LCS_STATE_START == NAS_MM_GetRrLcsState())
              && (MM_TRUE != ucMmConnFlg) )
            {
                /* Ǩ�Ƶ�RR CONNECTION RELEASE NOT ALLOWED״̬������T3241��ʱ�� */
                Mm_ComSetMmState(MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED);
                NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForRrConn: MM STATUS is MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED.");
                Mm_TimerStart(MM_TIMER_T3241);
            }
            else
            {
                /* Ǩ�Ƶ�WAIT FOR NETWORK COMMAND״̬������T3240��ʱ�� */
                Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
                NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForRrConn: MM STATUS is WAIT_FOR_NETWORK_COMMAND.");
                Mm_TimerStart(MM_TIMER_T3240);
            }
        }
        else
        {
            /* ��������Ľ��뼼�������˱仯����ֱ��Ǩ�Ƶ��յ���������ǰ��״̬ */
            Mm_ComSetMmState(NAS_MM_GetStaOfRcvXXEstReq());
        }

        /* �����Ti���ڽ���MM�������� */
        /* ���MM������Ǽ�LCS�������󻺴��־ */
        NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);
    }
    else if (VOS_TRUE == NAS_MM_IsCsServiceBuffered(MM_CONN_CTRL_LCS, NAS_MM_GetLcsEstReqTi()))
    {
        /* �����Ti���ڻ����У�������LCS�������󻺴��־ */
        NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForRrConn: Invalid Transaction Id!");
        return;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ����LCSҵ���CSFB���̣������CSFB���ڱ�־ */
    if (NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST == NAS_MML_GetCsfbServiceStatus())
    {
        NAS_MM_ProcCsfbServiceStatusChange(NAS_MML_GetCsfbServiceStatus(), NAS_MML_CSFB_SERVICE_STATUS_NOT_EXIST);
    }
#endif

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForReestReq(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForReestCnf(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForAdditionalMmConn(VOS_VOID* pMsg)
{
    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsRelReq_PreProc(pMsg))
    {
        return;
    }

    /* ����Ti��MM�����Ƿ���ڣ������������Ti��MM���ӱ�־ */
    if (VOS_TRUE == NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi()))
    {
        NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi(), NAS_MM_CONN_NOT_EXIST);
    }

    if (MM_TRUE != Mm_ComMmConnExist())
    {
        /* ��ǰû������MM���� */
        /* ��¼Ǩ��֮ǰ��״̬����Ǩ�Ƶ�WAIT FOR OUTGOING MM CONNECTION״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());
        Mm_ComSetMmState(WAIT_FOR_OUTGOING_MM_CONNECT);
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForAdditionalMmConn: MM STATUS is WAIT_FOR_OUTGOING_MM_CONNECT.");
    }
    else
    {
        /* ��ǰ��������MM���� */
        if (NAS_MM_GetConnCtrlInfo(MM_CONN_CTRL_LCS)->ucMMConnEstingTI == NAS_MM_GetLcsEstReqTi())
        {
            /* �����Ti���ڽ���MM�������� */
            /* ���MM������Ǻ�������� */
            NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);

            /* ͣT3230��ʱ�� */
            Mm_TimerStop(MM_TIMER_T3230);

            /* ��¼Ǩ��֮ǰ��״̬����Ǩ�Ƶ�MM CONNECTION ACTIVE״̬ */
            NAS_MM_SetMmPreState(NAS_MM_GetMmState());
            Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForAdditionalMmConn: MM STATUS is MM_CONNECTION_ACTIVE.");
        }
        else if (VOS_TRUE == NAS_MM_IsCsServiceBuffered(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi()))
        {
            /* �����TiΪ����Ľ��������Ti����������� */
            NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvLcsRelReq_WaitForAdditionalMmConn: Invalid Transaction Id!");
            return;
        }
    }

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_LauPending(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_ImsiDetachPending(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForAttach(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForRrConnLau(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_LauInitiated(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_LauRejected(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_WaitForRrConnImsiDetach(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_ImsiDetachInitiated(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_ProcCmServicePrompt(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_RrConnRelNotAllow(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM IDLE NO CELL AVAILABLE״̬ */
    NAS_MM_RcvLcsRelReq_MmIdleNoCellAvaliable(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvLcsRelReq_MmInterRatChange(VOS_VOID* pMsg)
{
    /* ��Ϣͷ�ṹ�������ж���Ϣ���� */
    MSG_HEADER_STRU                    *pstMsgHeader    = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷ */
    pstMsgHeader = (MSG_HEADER_STRU* )pMsg;

    /* ��Ϣ���ʧ�ܣ�ֱ�ӷ��� */
    if (VOS_TRUE != NAS_MM_RcvLcsRelReq_PreProc(pMsg))
    {
        return;
    }

    if (WAIT_FOR_OUTGOING_MM_CONNECTION == g_MmGlobalInfo.ucSuspendPreState)
    {
        /* ���ú��� MM_SaveMsg ���д��� */
        MM_SaveMsg(pMsg, g_MmSubLyrShare.MmShare.MmSuspendEvent, pstMsgHeader->ulLength + VOS_MSG_HEAD_LENGTH);
        return;
    }

    if (VOS_TRUE == NAS_MM_IsCsServiceBuffered(MM_CONN_CTRL_LCS, NAS_MM_GetLcsRelReqTi()))
    {
        /* �����TiΪ����Ľ��������Ti����������� */
        NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);


        NAS_MM_SndRrMmServiceAbortNotify(WUEPS_PID_WRR);
        NAS_MM_SndRrMmServiceAbortNotify(UEPS_PID_GAS);

#if (FEATURE_ON == FEATURE_LTE)
        /* ��ǰ����LCSҵ���CSFB���̣������CSFB���ڱ�־ */
        if (NAS_MML_CSFB_SERVICE_STATUS_MO_NORMAL_LCS_EXIST == NAS_MML_GetCsfbServiceStatus())
        {
            NAS_MM_ProcCsfbServiceStatusChange(NAS_MML_GetCsfbServiceStatus(), NAS_MML_CSFB_SERVICE_STATUS_NOT_EXIST);

            if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
            {
                /* ��ǰ���뼼��ΪLTE��֪ͨLMM��ֹCSFB���� */
                NAS_MM_SndLmmCsfbServiceAbortNotify();
            }
        }
#endif
    }

    /* ����CS���������״̬������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID NAS_MM_DealBufferedLcsEstReq(VOS_VOID)
{
    LCS_MM_EST_REQ_STRU                 stLcsEstReq;

    PS_MEM_SET(&stLcsEstReq, 0, sizeof(stLcsEstReq));

    NAS_MM_BuildLcsEstReqFromBuffer(&stLcsEstReq);
    NAS_MM_ClearConnCtrlInfoWithBuffer(MM_CONN_CTRL_LCS);

    NAS_INFO_LOG1(WUEPS_PID_MM, "NAS_MM_DealBufferedLcsEstReq: Buffered Lcs Est Request Ti:", stLcsEstReq.ulTi);

    /*lint -e961*/
    gaMmStateTable[MM_EVENT_ID_LCS_EST_REQ - 1][g_MmGlobalInfo.ucState](&stLcsEstReq);
    /*lint +e961*/

    return;
}


VOS_VOID NAS_MM_RcvRrDataIndLcs_WaitForOutgoingMmConn(VOS_VOID* pMsg)
{
    RRMM_DATA_IND_STRU      *pRrDataInd     = VOS_NULL_PTR;
    VOS_UINT8               *pucCmMsg       = VOS_NULL_PTR;
    VOS_UINT8                ucTi;

    pRrDataInd  = (RRMM_DATA_IND_STRU *)pMsg;
    pucCmMsg    = pRrDataInd->RcvNasMsg.aucNasMsg;

    /* ͣT3212��ʱ�� */
    Mm_TimerStop( MM_TIMER_T3212 );

    /* ��NAS�տ���Ϣ�л�ȡLCSҵ��TI */
    ucTi = (VOS_UINT8)((pucCmMsg[0] & 0xf0) >> 4);
    if ( MM_CONST_NUM_7 > ucTi )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* ���ཨ����MM���� */
        if (VOS_TRUE != NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, ucTi))
        {
            /* ����֪ͨ�ĵ�һ��LCS�տ���Ϣ������MM���ӱ�ʶ */
            NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

            /* ��¼Ǩ��֮ǰ��״̬ */
            NAS_MM_SetMmPreState(NAS_MM_GetMmState());

            /* Ǩ��״̬��WAIT FOR ADDITIONAL OUTGOING MM CONNECTION */
            Mm_ComSetMmState(WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForOutgoingMmConn: MM STATUS is WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION");
        }
    }
    else if ( ( MM_CONST_NUM_7 < ucTi ) && ( MM_CONST_NUM_15 > ucTi ) )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* LCS������MM���� */
        if (VOS_TRUE != NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, ucTi))
        {
            /* ����ظ�LCS�ĵ�һ����Ϣ������MM���ӱ�ʶ */
            NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

            /* ͣT3230 */
            Mm_TimerStop(MM_TIMER_T3230);

            /* ��¼Ǩ��֮ǰ��״̬ */
            NAS_MM_SetMmPreState(NAS_MM_GetMmState());

            /* Ǩ��״̬��MM CONNECTION ACTIVE */
            Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForOutgoingMmConn: MM STATUS is MM_CONNECTION_ACTIVE");

            /* ������Ľ������� */
            Mm_ComCheckDelayMmConn(MM_FALSE);
        }
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForOutgoingMmConn: Invalid Network TransactionId!");
    }

    return;
}


VOS_VOID NAS_MM_RcvRrDataIndLcs_MmConnectionActive(VOS_VOID* pMsg)
{
    RRMM_DATA_IND_STRU      *pRrDataInd     = VOS_NULL_PTR;
    VOS_UINT8               *pucCmMsg       = VOS_NULL_PTR;
    VOS_UINT8                ucTi;

    pRrDataInd  = (RRMM_DATA_IND_STRU *)pMsg;
    pucCmMsg    = pRrDataInd->RcvNasMsg.aucNasMsg;

    /* ��NAS�տ���Ϣ�л�ȡLCSҵ��TI */
    ucTi = (VOS_UINT8)((pucCmMsg[0] & 0xf0) >> 4);

    if ( MM_CONST_NUM_7 > ucTi )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* ���ཨ����MM���� */
        if (VOS_TRUE != NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, ucTi))
        {
            /* ����֪ͨ�ĵ�һ��LCS�տ���Ϣ������MM���ӱ�ʶ */
            NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);
        }
    }
    else if ( ( MM_CONST_NUM_7 < ucTi ) && ( MM_CONST_NUM_15 > ucTi ) )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* LCS������MM���� */
        if (VOS_TRUE != NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, ucTi))
        {
            /* ����ظ�LCS�ĵ�һ����Ϣ������MM���ӱ�ʶ */
            NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

            /* ������Ľ������� */
            Mm_ComCheckDelayMmConn(MM_FALSE);
        }
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_MmConnectionActive: Invalid Network TransactionId!");
    }

    /* ����CS���������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvRrDataIndLcs_WaitForNetworkCmd(VOS_VOID* pMsg)
{
    RRMM_DATA_IND_STRU      *pRrDataInd     = VOS_NULL_PTR;
    VOS_UINT8               *pucCmMsg       = VOS_NULL_PTR;
    VOS_UINT8                ucTi;

    pRrDataInd  = (RRMM_DATA_IND_STRU *)pMsg;
    pucCmMsg    = pRrDataInd->RcvNasMsg.aucNasMsg;

    /* ��NAS�տ���Ϣ�л�ȡLCSҵ��TI */
    ucTi = (VOS_UINT8)((pucCmMsg[0] & 0xf0) >> 4);

    if ( MM_CONST_NUM_7 > ucTi )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* ����֪ͨ�ĵ�һ��LCS�տ���Ϣ������MM���ӱ�ʶ */
        NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

        /* ͣT3240��ʱ�� */
        Mm_TimerStop(MM_TIMER_T3240);
        
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForNetworkCmd: Call NAS_MM_ResetCsPagingFlgAndT3240Len()");
        NAS_MM_ResetCsPagingFlgAndT3240Len();

        /* ��¼Ǩ��֮ǰ��״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());

        /* Ǩ��״̬��MM CONNECTION ACTIVE */
        Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForNetworkCmd: MM STATUS is MM_CONNECTION_ACTIVE");

        /* ������Ľ������� */
        Mm_ComCheckDelayMmConn(MM_FALSE);
    }

    else if ( ( MM_CONST_NUM_7 < ucTi ) && ( MM_CONST_NUM_15 > ucTi ) )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* ����ظ�LCS�ĵ�һ����Ϣ������MM���ӱ�ʶ */
        NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

        /* ͣT3240��ʱ�� */
        Mm_TimerStop(MM_TIMER_T3240);

        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForNetworkCmd: Call NAS_MM_ResetCsPagingFlgAndT3240Len()");
        NAS_MM_ResetCsPagingFlgAndT3240Len();

        /* ��¼Ǩ��֮ǰ��״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());

        /* Ǩ��״̬��MM CONNECTION ACTIVE */
        Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForNetworkCmd: MM STATUS is MM_CONNECTION_ACTIVE");

        /* ������Ľ������� */
        Mm_ComCheckDelayMmConn(MM_FALSE);
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForNetworkCmd: Invalid Network TransactionId!");
    }

    /* ����CS���������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvRrDataIndLcs_WaitForAdditionalMmConn(VOS_VOID* pMsg)
{
    RRMM_DATA_IND_STRU      *pRrDataInd     = VOS_NULL_PTR;
    VOS_UINT8               *pucCmMsg       = VOS_NULL_PTR;
    VOS_UINT8                ucTi;

    pRrDataInd  = (RRMM_DATA_IND_STRU *)pMsg;
    pucCmMsg    = pRrDataInd->RcvNasMsg.aucNasMsg;

    /* ��NAS�տ���Ϣ�л�ȡLCSҵ��TI */
    ucTi = (VOS_UINT8)((pucCmMsg[0] & 0xf0) >> 4);
    if ( MM_CONST_NUM_7 > ucTi )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* ���ཨ����MM���� */
        if (VOS_TRUE != NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, ucTi))
        {
            /* ����֪ͨ�ĵ�һ��LCS�տ���Ϣ������MM���ӱ�ʶ */
            NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);
        }
    }
    else if ( ( MM_CONST_NUM_7 < ucTi ) && ( MM_CONST_NUM_15 > ucTi ) )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* LCS������MM���� */
        if (VOS_TRUE != NAS_MM_IsMmConnExist(MM_CONN_CTRL_LCS, ucTi))
        {
            /* ����ظ�LCS�ĵ�һ����Ϣ������MM���ӱ�ʶ */
            NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

            /* ͣT3230 */
            Mm_TimerStop(MM_TIMER_T3230);

            /* ��¼Ǩ��֮ǰ��״̬ */
            NAS_MM_SetMmPreState(NAS_MM_GetMmState());

            /* Ǩ��״̬��MM CONNECTION ACTIVE */
            Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
            NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForAdditionalMmConn: MM STATUS is MM_CONNECTION_ACTIVE");

            /* ������Ľ������� */
            Mm_ComCheckDelayMmConn(MM_FALSE);
        }
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_WaitForAdditionalMmConn: Invalid Network TransactionId!");
    }

    return;
}


VOS_VOID NAS_MM_RcvRrDataIndLcs_RrConnRelNotAllow(VOS_VOID* pMsg)
{
    RRMM_DATA_IND_STRU      *pstRrDataInd   = VOS_NULL_PTR;
    VOS_UINT8               *pucCmMsg       = VOS_NULL_PTR;
    VOS_UINT8                ucTi;

    pstRrDataInd    = (RRMM_DATA_IND_STRU *)pMsg;
    pucCmMsg        = pstRrDataInd->RcvNasMsg.aucNasMsg;

    /* ��NAS�տ���Ϣ�л�ȡLCSҵ��TI */
    ucTi = (VOS_UINT8)((pucCmMsg[0] & 0xf0) >> 4);

    if ( MM_CONST_NUM_7 > ucTi )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pstRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* ����֪ͨ�ĵ�һ��LCS�տ���Ϣ������MM���ӱ�ʶ */
        NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

        /* ͣT3240��ʱ�� */
        Mm_TimerStop(MM_TIMER_T3241);

        /* ��¼Ǩ��֮ǰ��״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());

        /* Ǩ��״̬��MM CONNECTION ACTIVE */
        Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_RrConnRelNotAllow: MM STATUS is MM_CONNECTION_ACTIVE");

        /* ������Ľ������� */
        Mm_ComCheckDelayMmConn(MM_FALSE);
    }

    else if ( ( MM_CONST_NUM_7 < ucTi ) && ( MM_CONST_NUM_15 > ucTi ) )
    {
        /* ��LCS������ϢID_MM_LCS_DATA_IND */
        NAS_MM_SndLcsDataInd(ucTi, pucCmMsg, pstRrDataInd->RcvNasMsg.ulNasMsgSize);

        /* ����ظ�LCS�ĵ�һ����Ϣ������MM���ӱ�ʶ */
        NAS_MM_UpdateMmConnByTi(MM_CONN_CTRL_LCS, ucTi, NAS_MM_CONN_EXIST);

        /* ͣT3240��ʱ�� */
        Mm_TimerStop(MM_TIMER_T3241);

        /* ��¼Ǩ��֮ǰ��״̬ */
        NAS_MM_SetMmPreState(NAS_MM_GetMmState());

        /* Ǩ��״̬��MM CONNECTION ACTIVE */
        Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_RrConnRelNotAllow: MM STATUS is MM_CONNECTION_ACTIVE");

        /* ������Ľ������� */
        Mm_ComCheckDelayMmConn(MM_FALSE);
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrDataIndLcs_RrConnRelNotAllow: Invalid Network TransactionId!");
    }

    /* ����CS���������״̬ */
    NAS_MM_UpdateCsServiceConnStatusFlg();

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_Null(VOS_VOID* pMsg)
{
    RRMM_LCS_STATE_IND_STRU            *pstLcsStateInd = VOS_NULL_PTR;
    RRMM_LCS_STATE_ENUM_UINT8           enLcsNewState;

    pstLcsStateInd = (RRMM_LCS_STATE_IND_STRU *)pMsg;
    enLcsNewState  = pstLcsStateInd->enState;

    if (enLcsNewState >= RRMM_LCS_STATE_BUTT)
    {
        enLcsNewState = RRMM_LCS_STATE_FINISH;
    }

    if  (RRMM_LCS_STATE_FINISH != enLcsNewState)
    {
        NAS_WARNING_LOG1(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_Null: RR Lcs State:", NAS_MM_GetRrLcsState());
        enLcsNewState = RRMM_LCS_STATE_FINISH;
    }

    NAS_MM_SetRrLcsState(enLcsNewState);
    NAS_NORMAL_LOG1(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_Null: RR Lcs State:", enLcsNewState);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdleNoCellAvaliable(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdlePlmnSearch(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdleNormalService(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdleLimitedService(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdleAttempToUpdate(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdleLauNeeded(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdleNoImsi(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForOutgoingMmConn(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmConnectionActive(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForNetworkCmd(VOS_VOID* pMsg)
{
    RRMM_LCS_STATE_IND_STRU            *pstLcsStateInd = VOS_NULL_PTR;
    RRMM_LCS_STATE_ENUM_UINT8           enLcsNewState;

    pstLcsStateInd  = (RRMM_LCS_STATE_IND_STRU *)pMsg;
    enLcsNewState   = pstLcsStateInd->enState;

    /* �쳣���� */
    if (enLcsNewState >= RRMM_LCS_STATE_BUTT)
    {
        /* �����LCS״̬�쳣 */
        NAS_WARNING_LOG1(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_WaitForNetworkCmd: RR LCS State is Abnormal.", enLcsNewState);

        enLcsNewState = RRMM_LCS_STATE_FINISH;
    }

    /* ����ȫ�ֱ��� */
    NAS_MM_SetRrLcsState(enLcsNewState);

    /* LCS����״̬ΪFINISH����ֱ�ӷ��� */
    if (RRMM_LCS_STATE_FINISH == enLcsNewState)
    {
        return;
    }

    /* ����MM���ӣ���ֱ�ӷ��� */
    if (MM_TRUE == Mm_ComMmConnExist())
    {
        return;
    }

    /* �����LCS״̬��ΪSTART�����л�MM״̬ΪRR CONNECTION RELEASE NOT ALLOWED״̬ */
    NAS_NORMAL_LOG1(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_WaitForNetworkCmd: RR Lcs State:", RRMM_LCS_STATE_START);

    /* ֹͣT3240��ʱ�� */
    Mm_TimerStop(MM_TIMER_T3240);

    /* ��¼Ǩ��֮ǰ��״̬ */
    NAS_MM_SetMmPreState(NAS_MM_GetMmState());

    /* Ǩ��״̬��RR CONNECTION RELEASE NOT ALLOWED */
    Mm_ComSetMmState(MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED);
    NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_WaitForNetworkCmd: MM STATUS is MM_STATE_RR_CONNECTION_RELEASE_NOT_ALLOWED");

    /* ����T3241��ʱ�� */
    Mm_TimerStart(MM_TIMER_T3241);

    /* ������Ľ������� */
    Mm_DealWithBuffMsg();

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForRrConn(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForReestReq(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForReestCnf(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForAdditionalMmConn(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_LauPending(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_ImsiDetachPending(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForAttach(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForRrConnLau(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_LauInitiated(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_LauRejected(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_WaitForRrConnImsiDetach(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_ImsiDetachInitiated(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_ProcCmServicePrompt(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_RrConnRelNotAllow(VOS_VOID* pMsg)
{
    RRMM_LCS_STATE_IND_STRU            *pstLcsStateInd = VOS_NULL_PTR;
    RRMM_LCS_STATE_ENUM_UINT8           enLcsNewState;

    pstLcsStateInd  = (RRMM_LCS_STATE_IND_STRU *)pMsg;
    enLcsNewState   = pstLcsStateInd->enState;

    /* �쳣���� */
    if (enLcsNewState >= RRMM_LCS_STATE_BUTT)
    {
        /* �����LCS״̬�쳣 */
        NAS_WARNING_LOG1(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_RrConnRelNotAllow: RR LCS State is Abnormal.", enLcsNewState);

        enLcsNewState = RRMM_LCS_STATE_FINISH;
    }

    /* LCS����״̬ΪSTART����ֱ�ӷ��� */
    if (RRMM_LCS_STATE_START == enLcsNewState)
    {
        return;
    }

    /* �����LCS״̬��ΪΪSTART�����л�MM״̬ΪRR CONNECTION RELEASE NOT ALLOWED״̬ */
    NAS_MM_SetRrLcsState(RRMM_LCS_STATE_FINISH);
    NAS_NORMAL_LOG1(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_RrConnRelNotAllow: RR Lcs State:", RRMM_LCS_STATE_FINISH);

    /* ֹͣT3241��ʱ�� */
    Mm_TimerStop(MM_TIMER_T3241);

    /* ��¼Ǩ��֮ǰ��״̬ */
    NAS_MM_SetMmPreState(NAS_MM_GetMmState());

    /* Ǩ��״̬��WAIT FOR NETWORK COMMAND */
    Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
    NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvRrLcsStateInd_WaitForNetworkCmd: MM STATUS is WAIT_FOR_NETWORK_COMMAND");

    /* ����T3240��ʱ�� */
    Mm_TimerStart(MM_TIMER_T3240);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_TestContrlActive(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmInterRatChange(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvRrLcsStateInd_MmIdleECallInactive(VOS_VOID* pMsg)
{
    /* ��״̬�´���ͬMM NULL״̬ */
    NAS_MM_RcvRrLcsStateInd_Null(pMsg);

    return;
}


VOS_VOID NAS_MM_RcvT3241Expired_PreProc(VOS_VOID)
{
    /* ֹͣT3241��ʱ�� */
    Mm_TimerStop(MM_TIMER_T3241);
    return;
}


VOS_VOID NAS_MM_RcvT3241Expired_RrConnRelNotAllow(VOS_VOID* pMsg)
{
    /* ���½����LCS����״̬ΪFINISH */
    NAS_MM_SetRrLcsState(RRMM_LCS_STATE_FINISH);

    /* ֪ͨ������ͷ�RR���� */
    Mm_SndRrRelReq(RRC_CELL_UNBARRED);

    /* ��¼Ǩ��֮ǰ��״̬ */
    NAS_MM_SetMmPreState(NAS_MM_GetMmState());

    /* Ǩ��״̬��WAIT FOR NETWORK COMMAND */
    Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
    NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_RcvT3241Expired_RrConnRelNotAllow: MM STATUS is WAIT_FOR_NETWORK_COMMAND");

    /* ����T3240��ʱ�� */
    Mm_TimerStart(MM_TIMER_T3240);

    return;
}


VOS_VOID NAS_MM_RcvLcsStatusNty(VOS_VOID* pMsg)
{
    LCS_MM_STATUS_NTY_STRU             *pstLcsStatusNty = VOS_NULL_PTR;
    RRMM_TRANSACTION_STATUS_ENUM_UINT8  enTransStatus;
    NAS_MML_MO_LCS_STATUS_ENUM_UINT8    enOldLcsMoStatus;

    pstLcsStatusNty     = (LCS_MM_STATUS_NTY_STRU*)pMsg;
    enOldLcsMoStatus    = NAS_MML_GetMoLcsStatus();
    enTransStatus       = RRMM_TRANSACTION_FAIL;

    /* ��Ϣ��Ч�Լ�� */
    if ( (NAS_LCS_STATUS_SETUP_SUCC != pstLcsStatusNty->enLcsStatus)
      && (NAS_LCS_STATUS_SETUP_FAIL != pstLcsStatusNty->enLcsStatus) )
    {
        return;
    }

    /* ��ҵ������־ʱ��֪ͨAS�������ͷ�ʱ����FR */
    if (VOS_TRUE == NAS_MML_GetMoLcsSetupFlg())
    {
        enTransStatus = NAS_MM_ConvertLcsStatusToTransactionStatus(pstLcsStatusNty->enLcsStatus);
        NAS_MM_SndTransactionStatusNotify(enTransStatus);
    }

    /* ���MMC��������ʱҵ�������֪ͨMMCֹͣ��GU�����´�LTE��ʼ����;���MMC����ON PLMN�������ȼ����뼼��BG�Ѷ�ʱ�� */
    if ((NAS_MML_CSFB_MO_LCS_SETUP_START == enOldLcsMoStatus)
     && (NAS_LCS_STATUS_SETUP_FAIL == pstLcsStatusNty->enLcsStatus))
    {
        NAS_MM_SndMmcPlmnSearchInd(MM_MMC_PLMN_SEARCH_TYPE_CSFB_END);
    }

    /* ���±�־ */
    NAS_MM_UpdateLcsStatus(pstLcsStatusNty->enLcsStatus);

    return;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

