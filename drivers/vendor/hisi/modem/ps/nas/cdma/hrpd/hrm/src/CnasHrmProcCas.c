

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CnasHrmProcCas.h"
#include "CnasHrmSndTtf.h"
#include "CnasHrmSndHsm.h"
#include "CnasHrmSndHlu.h"
#include "CnasHrmSndCas.h"
#include "CnasHrmCtx.h"
#include "CnasHrmCommFunc.h"
#include "CnasMntn.h"

#include "hrm_pif.h"
#include "hsm_hrm_pif.h"
#include "hsm_hlu_pif.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define THIS_FILE_ID                    PS_FILE_ID_CNAS_HRM_PROC_CAS_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*lint -save -e958*/


VOS_UINT8 CNAS_HRM_IsHrpdResReApplyCnf(
    VOS_UINT8                           ucBuffIndex
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstDataReqBuff = VOS_NULL_PTR;

    pstDataReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucBuffIndex);

    if ((VOS_TRUE == pstDataReqBuff->stReqMsg.ulIsReqMsgExisted)
     && (CNAS_HRM_RES_OCCUPY_TYPE_SHORT == pstDataReqBuff->stResInfo.enResOccupyType)
     && (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstDataReqBuff->stResInfo.enResApplyState))
    {

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID CNAS_HRM_ProcCasHrpdResReApplyCnf(
    VOS_UINT8                           ucDataReqBuffIndex,
    PS_BOOL_ENUM_UINT8                  enApplyRFSucc
)
{
    /* ��������ɹ����ȴ�SNP��data cnf�������SNP����cancel */
    if (PS_TRUE == enApplyRFSucc)
    {
        CNAS_HRM_GetDataReqBuffAddr(ucDataReqBuffIndex)->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        CNAS_HRM_GetDataReqBuffAddr(ucDataReqBuffIndex)->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;

        /* DSDS1.0 uati req��Դ����ռʱˢ��uati cmpl����Դ״̬Ϊnot apply, not occupy,
           * ��uati req����������Դ�ɹ�ʱˢ��uati cmpl����Դ״̬Ϊsucc,
           * ����ˢ��, �������Ա�֤���uati req���ͳɹ�, �յ�uati assign����uati cmpl��ʱ�������������Դ */
        if ((CNAS_HRM_DSDS_TYPE_1 == CNAS_HRM_GetDsdsType())
         && (CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ == ucDataReqBuffIndex))
        {
            CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL)->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
            CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL)->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
        }
    }
    else
    {
        CNAS_HRM_SndSnpDataCancelReq(CNAS_HRM_GetDataReqBuffAddr(ucDataReqBuffIndex)->stReqMsg.usHrmOpId);

        CNAS_HRM_GetDataReqBuffAddr(ucDataReqBuffIndex)->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;

        if ((CNAS_HRM_DSDS_TYPE_1 == CNAS_HRM_GetDsdsType())
         && (CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ == ucDataReqBuffIndex))
        {
            CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL)->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
        }
    }

}


VOS_VOID CNAS_HRM_ProcCasHrpdUatiResApplySucc(
    CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU                *pstHrpdResApplyCnf
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiReqBuff = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiCmplBuff = VOS_NULL_PTR;
    HSM_HRM_DATA_REQ_STRU              *pstHsmUatiReq = VOS_NULL_PTR;
    HSM_HRM_DATA_REQ_STRU              *pstHsmUatiCmpl = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgLen;
    VOS_UINT16                          usOpId;

    pstUatiReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
    pstUatiCmplBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

    /* DSDS1.0 UATI REQ��CMPL����applying״̬ */
    if (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstUatiReqBuff->stResInfo.enResApplyState)
    {
        pstUatiReqBuff->stResInfo.enResApplyState  = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        pstUatiReqBuff->stResInfo.enResOccupyType  = CNAS_HRM_RES_OCCUPY_TYPE_NON;
    }

    if (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstUatiCmplBuff->stResInfo.enResApplyState)
    {
        pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        pstUatiCmplBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
    }

    if (VOS_TRUE == pstUatiReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        usOpId        = CNAS_HRM_GetOpId();
        pstHsmUatiReq = (HSM_HRM_DATA_REQ_STRU *)(pstUatiReqBuff->stReqMsg.pucMsg);
        ulMsgLen      =  pstHsmUatiReq->ulLength - sizeof(pstHsmUatiReq->enDataReqType);

        /* ���������SNP */
        CNAS_HRM_SndSnpDataReq(usOpId, ulMsgLen, &(pstHsmUatiReq->stDataReqCommContent));

        pstUatiReqBuff->stReqMsg.usHrmOpId     = usOpId;
    }

    if (VOS_TRUE == pstUatiCmplBuff->stReqMsg.ulIsReqMsgExisted)
    {
        usOpId         = CNAS_HRM_GetOpId();
        pstHsmUatiCmpl = (HSM_HRM_DATA_REQ_STRU *)(pstUatiCmplBuff->stReqMsg.pucMsg);
        ulMsgLen       =  pstHsmUatiCmpl->ulLength - sizeof(pstHsmUatiCmpl->enDataReqType);

        /* ���������SNP */
        CNAS_HRM_SndSnpDataReq(usOpId, ulMsgLen, &(pstHsmUatiCmpl->stDataReqCommContent));

        /* ��Ҫ������HSM�ظ�UATI CMPL��Ϣ�ѷ��͸�SNP */
        CNAS_HRM_SndHsmUatiCmplSndInd(pstUatiCmplBuff->stReqMsg.usReqOpId);

        pstUatiCmplBuff->stReqMsg.usHrmOpId     = usOpId;
    }

}


VOS_VOID CNAS_HRM_ProcCasHrpdUatiResApplyFail(
    CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU                *pstHrpdResApplyCnf
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiReqBuff = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiCmplBuff = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();
    pstUatiReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
    pstUatiCmplBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

    if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
    {
        pstUatiReqBuff->stResInfo.enResApplyState  = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
        pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;

        /* DSDS1.0 ����Ѿ��յ�DATA_REQ����HSM�ظ�NO RF */
        if (VOS_TRUE == pstUatiReqBuff->stReqMsg.ulIsReqMsgExisted)
        {
            /* NO RFΪHRM���ڲ�ԭ��ֵ����SNP�ظ�ԭ��ֵ��reason��butt */
            CNAS_HRM_SndHsmDataCnf(pstUatiReqBuff->stReqMsg.usReqOpId, HRM_HSM_DATA_CNF_RSLT_NO_RF, CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

            /* ֻ����Ϣ */
            CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
        }

        if (VOS_TRUE == pstUatiCmplBuff->stReqMsg.ulIsReqMsgExisted)
        {
            /* NO RFΪHRM���ڲ�ԭ��ֵ����SNP�ظ�ԭ��ֵ��reason��butt */
            CNAS_HRM_SndHsmDataCnf(pstUatiCmplBuff->stReqMsg.usReqOpId, HRM_HSM_DATA_CNF_RSLT_NO_RF, CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

            /* ֻ����Ϣ */
            CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);
        }
    }

    if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
    {
        if (VOS_TRUE == pstUatiReqBuff->stReqMsg.ulIsReqMsgExisted)
        {
            /* NO RFΪHRM���ڲ�ԭ��ֵ����SNP�ظ�ԭ��ֵ��reason��butt */
            CNAS_HRM_SndHsmDataCnf(pstUatiReqBuff->stReqMsg.usReqOpId, HRM_HSM_DATA_CNF_RSLT_NO_RF, CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

            /* �建�� */
            CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);

            CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
        }

        if (VOS_TRUE == pstUatiCmplBuff->stReqMsg.ulIsReqMsgExisted)
        {
            /* NO RFΪHRM���ڲ�ԭ��ֵ����SNP�ظ�ԭ��ֵ��reason��butt */
            CNAS_HRM_SndHsmDataCnf(pstUatiCmplBuff->stReqMsg.usReqOpId, HRM_HSM_DATA_CNF_RSLT_NO_RF, CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

            /* �建�� */
            CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

            CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);
        }
    }
}


VOS_VOID CNAS_HRM_ProcCasHrpdUatiResApplyCnf(
    CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU                *pstHrpdResApplyCnf
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiReqBuff = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiCmplBuff = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    pstUatiReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
    pstUatiCmplBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);
    enDsdsType      = CNAS_HRM_GetDsdsType();

    /* ����Ƕ�������������Դ�ɹ������ط�ֻ���±�־���ȴ�SNP��data cnf */
    if (VOS_TRUE == CNAS_HRM_IsHrpdResReApplyCnf(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ))
    {
        CNAS_HRM_ProcCasHrpdResReApplyCnf(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ, pstHrpdResApplyCnf->enApplyRFSucc);

        if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
        {
            pstUatiCmplBuff->stResInfo.enResApplyState = pstUatiReqBuff->stResInfo.enResApplyState;
        }

        return;
    }

    if (VOS_TRUE == CNAS_HRM_IsHrpdResReApplyCnf(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL))
    {
        CNAS_HRM_ProcCasHrpdResReApplyCnf(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL, pstHrpdResApplyCnf->enApplyRFSucc);

        if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
        {
            pstUatiReqBuff->stResInfo.enResApplyState = pstUatiCmplBuff->stResInfo.enResApplyState;
        }

        return;
    }

    /* û����ռ����µ���Դ���������� */
    if (PS_TRUE == pstHrpdResApplyCnf->enApplyRFSucc)
    {
        CNAS_HRM_ProcCasHrpdUatiResApplySucc(pstHrpdResApplyCnf);
    }
    else
    {
        CNAS_HRM_ProcCasHrpdUatiResApplyFail(pstHrpdResApplyCnf);
    }
}


VOS_VOID  CNAS_HRM_ProcCasHrpdMoKeepAliveResApplyCnf(
    CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU                *pstHrpdResApplyCnf
)
{
    VOS_UINT16                          usOpId;
    VOS_UINT32                          ulMsgLen;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstReqBuff = VOS_NULL_PTR;
    HSM_HRM_DATA_REQ_STRU              *pstDataReq = VOS_NULL_PTR;
    VOS_UINT8                           ucIndex;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType      = CNAS_HRM_GetDsdsType();

    ucIndex = CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE;

    pstReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

    /* ����Ƕ�������������Դ�ɹ������ط�ֻ���±�־���ȴ�SNP��data cnf */
    if (VOS_TRUE == CNAS_HRM_IsHrpdResReApplyCnf(ucIndex))
    {
        CNAS_HRM_ProcCasHrpdResReApplyCnf(ucIndex, pstHrpdResApplyCnf->enApplyRFSucc);

        return;
    }

    /* �����Դ����ɹ������л�����Ϣ�����ͻ������Ϣ��
       �����Դ����ʧ�ܣ����л�����Ϣ�����ϲ�ظ�ʧ�ܣ�DSDS2.0������棬��������Դ����ʧ�ܱ�־ */
    if (PS_TRUE == pstHrpdResApplyCnf->enApplyRFSucc)
    {
        pstReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        pstReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;

        if (VOS_TRUE == pstReqBuff->stReqMsg.ulIsReqMsgExisted)
        {
            usOpId     = CNAS_HRM_GetOpId();
            pstDataReq = (HSM_HRM_DATA_REQ_STRU*)pstReqBuff->stReqMsg.pucMsg;
            ulMsgLen   =  pstDataReq->ulLength - sizeof(pstDataReq->enDataReqType);

            /* ���������SNP */
            CNAS_HRM_SndSnpDataReq(usOpId,
                                   ulMsgLen,
                                   &(pstDataReq->stDataReqCommContent));

            pstReqBuff->stReqMsg.usHrmOpId     = usOpId;
        }

    }
    else
    {
        if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
        {
            pstReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;

            if (VOS_TRUE == pstReqBuff->stReqMsg.ulIsReqMsgExisted)
            {
                CNAS_HRM_SndHsmDataCnf(pstReqBuff->stReqMsg.usReqOpId,
                                       HRM_HLU_DATA_CNF_RSLT_NO_RF,
                                       CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

                CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
            }

        }

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            if (VOS_TRUE == pstReqBuff->stReqMsg.ulIsReqMsgExisted)
            {
                CNAS_HRM_SndHsmDataCnf(pstReqBuff->stReqMsg.usReqOpId,
                                       HRM_HLU_DATA_CNF_RSLT_NO_RF,
                                       CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

                CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);

                CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(ucIndex);
            }
        }

    }
}


VOS_VOID  CNAS_HRM_ProcCasHrpdHsmResApplyCnfCommProcess(
    CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU                *pstHrpdResApplyCnf
)
{
    VOS_UINT16                          usOpId;
    VOS_UINT32                          ulMsgLen;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstReqBuff = VOS_NULL_PTR;
    HSM_HRM_DATA_REQ_STRU              *pstDataReq = VOS_NULL_PTR;
    VOS_UINT8                           ucIndex;

    /* ֻ����HARDWARE_ID, MT_KEEP_ALIVE, SESSION_CLOSE */
    ucIndex = CNAS_HRM_GetHsmMsgBuffIndexByTaskTypeExceptUati(pstHrpdResApplyCnf->enTaskType);

    pstReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

    /* ����޻�����Ϣֱ���˳� */
    if (VOS_TRUE != pstReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_ProcCasHrpdHsmResApplyCnfCommProcess: no buff msg");
        return;
    }

    /* ����Ƕ�������������Դ�ɹ������ط�ֻ���±�־���ȴ�SNP��data cnf */
    if (VOS_TRUE == CNAS_HRM_IsHrpdResReApplyCnf(ucIndex))
    {
        CNAS_HRM_ProcCasHrpdResReApplyCnf(ucIndex, pstHrpdResApplyCnf->enApplyRFSucc);

        return;
    }

    /* �����Դ����ɹ������ͻ������Ϣ�������Դ����ʧ�ܣ����ϲ�ظ�ʧ�ܣ���������� */
    if (PS_TRUE == pstHrpdResApplyCnf->enApplyRFSucc)
    {
        usOpId     = CNAS_HRM_GetOpId();
        pstDataReq = (HSM_HRM_DATA_REQ_STRU*)pstReqBuff->stReqMsg.pucMsg;
        ulMsgLen   =  pstDataReq->ulLength - sizeof(pstDataReq->enDataReqType);

        /* ���������SNP */
        CNAS_HRM_SndSnpDataReq(usOpId,
                               ulMsgLen,
                               &(pstDataReq->stDataReqCommContent));

        pstReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        pstReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
        pstReqBuff->stReqMsg.usHrmOpId     = usOpId;
    }
    else
    {
        CNAS_HRM_SndHsmDataCnf(pstReqBuff->stReqMsg.usReqOpId,
                               HRM_HLU_DATA_CNF_RSLT_NO_RF,
                               CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

        CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
        CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(ucIndex);
    }
}


VOS_VOID  CNAS_HRM_ProcCasHrpdLuResApplyCnf(
    CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU                *pstHrpdResApplyCnf
)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT16                          usOpId;
    VOS_UINT32                          ulMsgLen;
    HLU_HRM_DATA_REQ_STRU              *pstDataReq     = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstDataReqBuff = VOS_NULL_PTR;
    CNAS_HRM_RES_APPLY_STATE_ENUM_UINT8 enResApplyState;

    enResApplyState   = CNAS_HRM_RES_APPLY_STATE_NOT_APPLY;

    for (ucIndex = CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_NTF; ucIndex <= CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_CMPL; ucIndex++)
    {
        pstDataReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

        if (VOS_TRUE == pstDataReqBuff->stReqMsg.ulIsReqMsgExisted)
        {
            pstDataReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

            /* ����Ƕ�������������Դ�ɹ������ط�ֻ���±�־���ȴ�SNP��data cnf */
            if (VOS_TRUE == CNAS_HRM_IsHrpdResReApplyCnf(ucIndex))
            {
                CNAS_HRM_ProcCasHrpdResReApplyCnf(ucIndex, pstHrpdResApplyCnf->enApplyRFSucc);

                enResApplyState = pstDataReqBuff->stResInfo.enResApplyState;

                continue;
            }

            /* �����Դ����ɹ������ͻ������Ϣ�������Դ����ʧ�ܣ��� HLU �ظ�ʧ�ܣ���������� */
            if (PS_TRUE == pstHrpdResApplyCnf->enApplyRFSucc)
            {
                usOpId     = CNAS_HRM_GetOpId();
                pstDataReq = (HLU_HRM_DATA_REQ_STRU*)pstDataReqBuff->stReqMsg.pucMsg;
                ulMsgLen   =  pstDataReq->ulLength - sizeof(pstDataReq->enDataReqType);

                /* ���������SNP */
                CNAS_HRM_SndSnpDataReq(usOpId,
                                       ulMsgLen,
                                       &(pstDataReq->stDataReqCommContent));

                pstDataReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
                pstDataReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
                pstDataReqBuff->stReqMsg.usHrmOpId       = usOpId;

            }
            else
            {
                CNAS_HRM_SndHluDataCnf(pstDataReqBuff->stReqMsg.usReqOpId,
                                       HRM_HLU_DATA_CNF_RSLT_NO_RF,
                                       CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

                CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
                CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(ucIndex);
            }

            enResApplyState = pstDataReqBuff->stResInfo.enResApplyState;
        }

    }

    CNAS_HRM_UpdateLuResApplyState(enResApplyState);
}


VOS_VOID CNAS_HRM_RcvCasHrpdResApplyCnf(
    CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU                *pstHrpdResApplyCnf
)
{
    /* �ж�task type��Ч�� */
    if (VOS_TRUE != CNAS_HRM_IsValidTaskType(pstHrpdResApplyCnf->enTaskType))
    {
        CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvCasHrpdResApplyCnf: invalid task type");
        return;
    }

    /* �ж��Ƿ���Ҫ����Դ������ */
    if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
    {
        CNAS_INFO_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvCasHrpdResApplyCnf: not wait res apply rslt");
        return;
    }

    switch (pstHrpdResApplyCnf->enTaskType)
    {
        case RFUSER_CAS_HRPD_TASK_TYPE_UATI:
            CNAS_HRM_ProcCasHrpdUatiResApplyCnf(pstHrpdResApplyCnf);
            break;

        case RFUSER_CAS_HRPD_TASK_TYPE_MO_KEEP_ALIVE:
            CNAS_HRM_ProcCasHrpdMoKeepAliveResApplyCnf(pstHrpdResApplyCnf);
            break;

        case RFUSER_CAS_HRPD_TASK_TYPE_HARD_ID:
        case RFUSER_CAS_HRPD_TASK_TYPE_MT_KEEP_ALIVE:
        case RFUSER_CAS_HRPD_TASK_TYPE_SESSION_CLOSE:
            CNAS_HRM_ProcCasHrpdHsmResApplyCnfCommProcess(pstHrpdResApplyCnf);
            break;

        case RFUSER_CAS_HRPD_TASK_TYPE_LU:
            CNAS_HRM_ProcCasHrpdLuResApplyCnf(pstHrpdResApplyCnf);
            break;

        default:
            CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvCasHrpdResApplyCnf: invalid task type");
            break;
    }

}


VOS_VOID  CNAS_HRM_ProcCasHrpdResLongOccupy(
    VOS_UINT8                           ucBuffIndex
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstMsgBuff = VOS_NULL_PTR;

    pstMsgBuff                = CNAS_HRM_GetDataReqBuffAddr(ucBuffIndex);

    /* ������ռ���ͺ���Դ����״̬ */
    pstMsgBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;
    pstMsgBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;

    /* ȡ��SNP���ݷ��� */
    CNAS_HRM_SndSnpDataCancelReq(pstMsgBuff->stReqMsg.usHrmOpId);
}


VOS_VOID  CNAS_HRM_ProcCasHrpdResShortOccupy(
    VOS_UINT8                                               ucBuffIndex
)
{
    CNAS_HRM_DATA_REQ_TYPE_ENUM_UINT32                      enDataReqType;
    RFUSER_CAS_HRPD_TASK_TYPE_ENUM_UINT16                   enTaskType;
    CNAS_HRM_DATA_REQ_BUFF_STRU                            *pstDataReqBuff = VOS_NULL_PTR;

    /* ��¼��ռ���� */
    pstDataReqBuff                  = CNAS_HRM_GetDataReqBuffAddr(ucBuffIndex);

    pstDataReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;

    enDataReqType             = CNAS_HRM_GetDataReqTypeByMsgBuffIndex(ucBuffIndex);
    enTaskType                = CNAS_HRM_GetTaskTypeByDataReqType(enDataReqType);

    /* ��������ʱ��������CAS������Դ */
    CNAS_HRM_SndCasHrpdResApplyReq(enTaskType);

    if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
    {
        pstDataReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        pstDataReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
    }
    else
    {
        pstDataReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
    }

}


VOS_VOID  CNAS_HRM_ProcCasHrpdResOccupy(
    CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND_STRU               *pstHrpdResOccupyInd,
    VOS_UINT8                                               ucBuffIndex
)
{
    switch (pstHrpdResOccupyInd->enOccupyType)
    {
        case RFUSER_CAS_OCCUPY_TYPE_LONG_URGENT:
        case RFUSER_CAS_OCCUPY_TYPE_LONG_NOT_URGENT:
            CNAS_HRM_ProcCasHrpdResLongOccupy(ucBuffIndex);
            break;

        case RFUSER_CAS_OCCUPY_TYPE_SHORT:
            CNAS_HRM_ProcCasHrpdResShortOccupy(ucBuffIndex);
            break;

        default:
            break;
    }
}

VOS_VOID  CNAS_HRM_ProcCasHrpdHsmResOccupyIndCommProcess(
    CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND_STRU               *pstHrpdResOccupyInd
)
{
    VOS_UINT8                           ucBuffIndex;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstReqBuff = VOS_NULL_PTR;

    ucBuffIndex = CNAS_HRM_GetHsmMsgBuffIndexByTaskTypeExceptUati(pstHrpdResOccupyInd->enTaskType);

    pstReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucBuffIndex);

    /* �޻�����Ϣ���˳� */
    if (VOS_TRUE != pstReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_ProcCasHrpdResOccupyExceptUati: no buff msg");

        return;
    }

    CNAS_HRM_ProcCasHrpdResOccupy(pstHrpdResOccupyInd, ucBuffIndex);
}


VOS_VOID  CNAS_HRM_ProcCasHrpdUatiResLongOccupy(VOS_VOID)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiReqBuff  = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiCmplBuff = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();

    pstUatiReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
    pstUatiCmplBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

    /* ������ռ���ͺ���Դ����״̬ */
    if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
    {
        pstUatiReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;
        pstUatiReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;

        pstUatiCmplBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;
        pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
    }

    if (VOS_TRUE == pstUatiReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        /* ȡ��SNP���ݷ��� */
        CNAS_HRM_SndSnpDataCancelReq(pstUatiReqBuff->stReqMsg.usHrmOpId);

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            pstUatiReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;
            pstUatiReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
        }
    }

    if (VOS_TRUE == pstUatiCmplBuff->stReqMsg.ulIsReqMsgExisted)
    {
        /* ȡ��SNP���ݷ��� */
        CNAS_HRM_SndSnpDataCancelReq(pstUatiCmplBuff->stReqMsg.usHrmOpId);

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            pstUatiCmplBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;
            pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
        }
    }

}


VOS_VOID  CNAS_HRM_ProcCasHrpdUatiResShortOccupy(VOS_VOID)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiReqBuff  = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiCmplBuff = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();

    pstUatiReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
    pstUatiCmplBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

    /* ��������ʱ��������CAS������Դ */
    CNAS_HRM_SndCasHrpdResApplyReq(RFUSER_CAS_HRPD_TASK_TYPE_UATI);

    if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
    {
        if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
        {
            pstUatiReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
            pstUatiReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;

            pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
            pstUatiCmplBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
        }

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            if (VOS_TRUE == pstUatiReqBuff->stReqMsg.ulIsReqMsgExisted)
            {
                pstUatiReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
                pstUatiReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
            }

            if (VOS_TRUE == pstUatiCmplBuff->stReqMsg.ulIsReqMsgExisted)
            {
                pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
                pstUatiCmplBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
            }
        }
    }
    else
    {
        if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
        {
            pstUatiReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
            pstUatiReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;

            pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
            pstUatiCmplBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;
        }

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            if (VOS_TRUE == pstUatiReqBuff->stReqMsg.ulIsReqMsgExisted)
            {
                pstUatiReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
                pstUatiReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;
            }

            if (VOS_TRUE == pstUatiCmplBuff->stReqMsg.ulIsReqMsgExisted)
            {
                pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
                pstUatiCmplBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;
            }
        }
    }

}



VOS_VOID  CNAS_HRM_ProcCasHrpdUatiResOccupyInd(
    CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND_STRU               *pstHrpdResOccupyInd
)
{

    if ((RFUSER_CAS_OCCUPY_TYPE_LONG_URGENT == pstHrpdResOccupyInd->enOccupyType)
     || (RFUSER_CAS_OCCUPY_TYPE_LONG_NOT_URGENT == pstHrpdResOccupyInd->enOccupyType))
    {
        CNAS_HRM_ProcCasHrpdUatiResLongOccupy();
    }

    if (RFUSER_CAS_OCCUPY_TYPE_SHORT == pstHrpdResOccupyInd->enOccupyType)
    {
        CNAS_HRM_ProcCasHrpdUatiResShortOccupy();
    }

}


VOS_VOID  CNAS_HRM_ProcCasHrpdMoKeepAliveResLongOccupy(VOS_VOID)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstMoKeepAlvieReqBuff  = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();

    pstMoKeepAlvieReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE);

    /* ������ռ���ͺ���Դ����״̬ */
    if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
    {
        pstMoKeepAlvieReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;
        pstMoKeepAlvieReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
    }

    if (VOS_TRUE == pstMoKeepAlvieReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        /* ȡ��SNP���ݷ��� */
        CNAS_HRM_SndSnpDataCancelReq(pstMoKeepAlvieReqBuff->stReqMsg.usHrmOpId);

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            pstMoKeepAlvieReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;
            pstMoKeepAlvieReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
        }
    }

}


VOS_VOID  CNAS_HRM_ProcCasHrpdMoKeepAliveResShortOccupy(VOS_VOID)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstMoKeepAlvieReqBuff  = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();

    pstMoKeepAlvieReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE);

    /* ��������ʱ��������CAS������Դ */
    CNAS_HRM_SndCasHrpdResApplyReq(RFUSER_CAS_HRPD_TASK_TYPE_MO_KEEP_ALIVE);

    if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
    {
        if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
        {
            pstMoKeepAlvieReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
            pstMoKeepAlvieReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
        }

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            if (VOS_TRUE == pstMoKeepAlvieReqBuff->stReqMsg.ulIsReqMsgExisted)
            {
                pstMoKeepAlvieReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
                pstMoKeepAlvieReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
            }
        }
    }
    else
    {
        if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
        {
            pstMoKeepAlvieReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
            pstMoKeepAlvieReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;
        }

        if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
        {
            if (VOS_TRUE == pstMoKeepAlvieReqBuff->stReqMsg.ulIsReqMsgExisted)
            {
                pstMoKeepAlvieReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
                pstMoKeepAlvieReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;
            }
        }
    }

}


VOS_VOID  CNAS_HRM_ProcCasHrpdMoKeepAliveResOccupyInd(
    CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND_STRU               *pstHrpdResOccupyInd
)
{

    if ((RFUSER_CAS_OCCUPY_TYPE_LONG_URGENT == pstHrpdResOccupyInd->enOccupyType)
     || (RFUSER_CAS_OCCUPY_TYPE_LONG_NOT_URGENT == pstHrpdResOccupyInd->enOccupyType))
    {
        CNAS_HRM_ProcCasHrpdMoKeepAliveResLongOccupy();
    }

    if (RFUSER_CAS_OCCUPY_TYPE_SHORT == pstHrpdResOccupyInd->enOccupyType)
    {
        CNAS_HRM_ProcCasHrpdMoKeepAliveResShortOccupy();
    }
}



VOS_VOID  CNAS_HRM_ProcCasHrpdLuResOccupyInd(
    CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND_STRU               *pstHrpdResOccupyInd
)
{
    VOS_UINT8                                               ucIndex;
    VOS_UINT8                                               ucAlreadyApplyRes;
    CNAS_HRM_RES_APPLY_STATE_ENUM_UINT8                     enResApplyState;
    CNAS_HRM_DATA_REQ_BUFF_STRU                            *pstDataReqBuff = VOS_NULL_PTR;

    ucAlreadyApplyRes = VOS_FALSE;
    enResApplyState   = CNAS_HRM_RES_APPLY_STATE_NOT_APPLY;

    for (ucIndex = CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_NTF; ucIndex <= CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_CMPL; ucIndex++)
    {
        pstDataReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

        if (VOS_FALSE == pstDataReqBuff->stReqMsg.ulIsReqMsgExisted)
        {
             continue;
        }

        if (RFUSER_CAS_OCCUPY_TYPE_SHORT == pstHrpdResOccupyInd->enOccupyType)
        {
            /* ��������ʱ��������CAS������Դ */
            if (VOS_FALSE == ucAlreadyApplyRes)
            {
                CNAS_HRM_SndCasHrpdResApplyReq(RFUSER_CAS_HRPD_TASK_TYPE_LU);
                ucAlreadyApplyRes = VOS_TRUE;
            }

            if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
            {
                enResApplyState                           = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
                pstDataReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
            }
            else
            {
                enResApplyState                           = CNAS_HRM_RES_APPLY_STATE_APPLYING;
                pstDataReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_SHORT;
            }
        }
        else
        {
            /* ��������ʱ��ȡ��SNP���ݷ��� */
            enResApplyState                           = CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL;
            pstDataReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_LONG;

            /* ȡ��SNP���ݷ��� */
            CNAS_HRM_SndSnpDataCancelReq(pstDataReqBuff->stReqMsg.usHrmOpId);
        }
    }

    CNAS_HRM_UpdateLuResApplyState(enResApplyState);

}


VOS_VOID CNAS_HRM_RcvCasHrpdResOccupyInd(
    CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND_STRU               *pstHrpdResOccupyInd
)
{
    if ((RFUSER_CAS_OCCUPY_TYPE_SHORT           != pstHrpdResOccupyInd->enOccupyType)
     && (RFUSER_CAS_OCCUPY_TYPE_LONG_URGENT     != pstHrpdResOccupyInd->enOccupyType)
     && (RFUSER_CAS_OCCUPY_TYPE_LONG_NOT_URGENT != pstHrpdResOccupyInd->enOccupyType))
    {
        return;
    }

    switch (pstHrpdResOccupyInd->enTaskType)
    {
        case RFUSER_CAS_HRPD_TASK_TYPE_UATI:
            CNAS_HRM_ProcCasHrpdUatiResOccupyInd(pstHrpdResOccupyInd);
            break;

        case RFUSER_CAS_HRPD_TASK_TYPE_MO_KEEP_ALIVE:
            CNAS_HRM_ProcCasHrpdMoKeepAliveResOccupyInd(pstHrpdResOccupyInd);
            break;

        case RFUSER_CAS_HRPD_TASK_TYPE_HARD_ID:
        case RFUSER_CAS_HRPD_TASK_TYPE_MT_KEEP_ALIVE:
        case RFUSER_CAS_HRPD_TASK_TYPE_SESSION_CLOSE:
            CNAS_HRM_ProcCasHrpdHsmResOccupyIndCommProcess(pstHrpdResOccupyInd);
            break;

        case RFUSER_CAS_HRPD_TASK_TYPE_LU:
            CNAS_HRM_ProcCasHrpdLuResOccupyInd(pstHrpdResOccupyInd);
            break;

        default:
            CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvCasHrpdResOccupyInd: invalid task type");
            break;
    }

}


VOS_VOID CNAS_HRM_RcvCasHrpdResResetInd(VOS_VOID)
{
    VOS_UINT8                           ucIndex;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstReqBuff = VOS_NULL_PTR;

    for (ucIndex = 0; ucIndex < CNAS_HRM_DATA_REQ_BUFF_INDEX_MAX; ucIndex++)
    {
        pstReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

        /* ������Ϣ���ڣ��ظ�DataCnf  NO RF */
        if (VOS_TRUE == pstReqBuff->stReqMsg.ulIsReqMsgExisted)
        {
            if (CNAS_HRM_DATA_REQ_BUFF_INDEX_SESSION_CLOSE >= ucIndex)
            {
                CNAS_HRM_SndHsmDataCnf(pstReqBuff->stReqMsg.usReqOpId, HRM_HSM_DATA_CNF_RSLT_NO_RF, CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);
            }
            else if ((CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_NTF <= ucIndex)
                  && (CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_CMPL >= ucIndex))
            {
                CNAS_HRM_SndHluDataCnf(pstReqBuff->stReqMsg.usReqOpId, HRM_HLU_DATA_CNF_RSLT_NO_RF, CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);
            }
            else
            {
            }

            CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
        }

        /* �����Դ״̬ */
        CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(ucIndex);
    }

}


VOS_VOID CNAS_HRM_RcvCasMsg(
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT16                          usMsgName;

    usMsgName = *((VOS_UINT16 *)(pstMsg->aucValue));

    switch (usMsgName)
    {

        case ID_CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF:
            CNAS_HRM_RcvCasHrpdResApplyCnf((CAS_RFUSER_HRPD_RESOURCE_APPLY_CNF_STRU*)pstMsg);
            break;

        case ID_CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND:
            CNAS_HRM_RcvCasHrpdResOccupyInd((CAS_RFUSER_HRPD_RESOURCE_OCCUPY_IND_STRU*)pstMsg);
            break;

        case ID_CAS_RFUSER_HRPD_RESOURCE_RESET_IND:
            CNAS_HRM_RcvCasHrpdResResetInd();
            break;

        default:
            break;
     }

     return;

}

/*lint -restore*/

#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


