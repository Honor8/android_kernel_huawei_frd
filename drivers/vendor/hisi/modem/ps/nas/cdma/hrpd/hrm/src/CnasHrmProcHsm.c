

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CnasHrmProcHsm.h"
#include "CnasHrmSndCas.h"
#include "CnasHrmSndTtf.h"
#include "CnasHrmSndHsm.h"
#include "CnasHrmCtx.h"
#include "CnasHrmCommFunc.h"
#include "CnasMntn.h"

#include "hsm_hrm_pif.h"
#include "Nas_Mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define THIS_FILE_ID                    PS_FILE_ID_CNAS_HRM_PROC_HSM_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*lint -save -e958*/


VOS_VOID CNAS_HRM_ProcHsmDataReqCommProcess(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstReqBuff = VOS_NULL_PTR;
    VOS_UINT16                          usOpId;
    VOS_UINT32                          ulMsgLen;
    VOS_UINT8                           ucIndex;

    ulMsgLen        =  pstDataReq->ulLength - sizeof(pstDataReq->enDataReqType);
    ucIndex         = CNAS_HRM_GetMsgBuffIndexByDataReqType(pstDataReq->enDataReqType);

    pstReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

    /* �յ�CNAS_HRM_DATA_REQ_UATI_REQUEST(DSDS2.0)
            CNAS_HRM_DATA_REQ_UATI_CMPL(DSDS2.0)
            CNAS_HRM_DATA_REQ_HARDWARE_ID
            CNAS_HRM_DATA_REQ_MO_KEEP_ALIVE(DSDS2.0)
            CNAS_HRM_DATA_REQ_MT_KEEP_ALIVE
            CNAS_HRM_DATA_REQ_SESSION_CLOSEʱ��
        ��Ӧ�Ļ��������ʹ�û�����Դδ�ͷ���Ϊ�쳣 */
    if (VOS_TRUE == pstReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        /* ���汻ʹ�ã��쳣 */
        CNAS_WARNING_LOG(UEPS_PID_HRM, "CNAS_HRM_ProcHsmDataReqCommProcess: buff msg is existed");

        CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
    }

    if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstReqBuff->stResInfo.enResApplyState))
    {
        /* ��Դδ�ͷţ��쳣�������ظ����� */
        CNAS_WARNING_LOG(UEPS_PID_HRM, "CNAS_HRM_ProcHsmDataReqCommProcess: res isn't released");
    }
    else
    {
        CNAS_HRM_SndCasHrpdResApplyReq(CNAS_HRM_GetTaskTypeByDataReqType(pstDataReq->enDataReqType));

        /* �ж��Ƿ���Ҫ����Դ���������������Ҫ�Ƚ�������󷢸�SNP��������յ�CNF֮���� */
        if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
        {
            pstReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        }
        else
        {
            pstReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
        }
    }


    /* ��ʹ�ñ�־ */
    pstReqBuff->stReqMsg.ulIsReqMsgExisted  = VOS_TRUE;

    if (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstReqBuff->stResInfo.enResApplyState)
    {
        usOpId = CNAS_HRM_GetOpId();

        /* ���������SNP */
        CNAS_HRM_SndSnpDataReq(usOpId, ulMsgLen, &(pstDataReq->stDataReqCommContent));

        /* �����UATI CMPL����Ҫ������HSM�ظ�UATI CMPL��Ϣ�ѷ��͸�SNP */
        if (CNAS_HRM_DATA_REQ_UATI_CMPL == pstDataReq->enDataReqType)
        {
            CNAS_HRM_SndHsmUatiCmplSndInd(pstDataReq->usOpId);
        }

        pstReqBuff->stReqMsg.usHrmOpId = usOpId;
    }

    pstReqBuff->stReqMsg.usReqOpId = pstDataReq->usOpId;

    pstReqBuff->stReqMsg.pucMsg = (VOS_UINT8 *)PS_MEM_ALLOC(UEPS_PID_HRM, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH);

    NAS_MEM_CPY_S(pstReqBuff->stReqMsg.pucMsg, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH,
                  pstDataReq, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH);

}


VOS_VOID CNAS_HRM_ProcHsmDataReqCommProcessDsds1(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstDataReqBuff = VOS_NULL_PTR;
    VOS_UINT16                          usOpId;
    VOS_UINT32                          ulMsgLen;
    VOS_UINT8                           ucBuffIndex;

    ulMsgLen       = pstDataReq->ulLength - sizeof(pstDataReq->enDataReqType);
    ucBuffIndex    = CNAS_HRM_GetMsgBuffIndexByDataReqType(pstDataReq->enDataReqType);

    pstDataReqBuff     = CNAS_HRM_GetDataReqBuffAddr(ucBuffIndex);

    /* UATI:
        DSDS1.0���յ�ID_HSM_HRM_SESSION_BEGIN_NTFʱ�Ѿ�������Դ��
        �������Ҫ��������������ʹ�ú���Դ�����־��
        �����Ҫ�������������յ�ID_CAS_RFUSER_HRPD_RESOURCE_APPLY_CNFʱ��ʹ�ú���Դ�����־
        ���������ATΪ��UATI����ʱ�·�UATI ASSIGN������UATI REQ��UATI CMPL������ͬ
        UATI CMPL���ͺ���Ҫ������HSM�ظ�UATI CMPL SND IND

        MO KEEP ALVIE:
        DSDS1.0���յ�ID_HSM_HRM_SESSION_BEGIN_NTFʱ�Ѿ�������Դ��
        �������Ҫ��������������ʹ�ú���Դ�����־��
        �����Ҫ�������������յ�ID_CAS_RFUSER_HRPD_RESOURCE_APPLY_CNFʱ��ʹ�ú���Դ�����־
     */

    /* �ж��Ƿ���Ҫ����Դ������ */
    if (VOS_TRUE == CNAS_HRM_IsNeedWaitResApplied())
    {
        /* �����Դ�����뵽 */
        if (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstDataReqBuff->stResInfo.enResApplyState)
        {
            usOpId = CNAS_HRM_GetOpId();

            /* ���������SNP */
            CNAS_HRM_SndSnpDataReq(usOpId, ulMsgLen, &(pstDataReq->stDataReqCommContent));

            /* �����UATI CMPL����Ҫ������HSM�ظ�UATI CMPL��Ϣ�ѷ��͸�SNP */
            if (CNAS_HRM_DATA_REQ_UATI_CMPL == pstDataReq->enDataReqType)
            {
                CNAS_HRM_SndHsmUatiCmplSndInd(pstDataReq->usOpId);
            }

            /* �軺����Ϣ������SNP�ظ����� */
            pstDataReqBuff->stReqMsg.usHrmOpId = usOpId;
        }

        /* �����ǰ��Դ�Ǳ���ռ��
             (���ܳ���:
              UATI REQ����ռ������SNP��data cnf�ǳɹ��ģ���ʱ����UATI CMPL) */
        if (CNAS_HRM_RES_OCCUPY_TYPE_NON != pstDataReqBuff->stResInfo.enResOccupyType)
        {
            if (CNAS_HRM_RES_APPLY_STATE_APPLYING != pstDataReqBuff->stResInfo.enResApplyState)
            {
                CNAS_HRM_SndCasHrpdResApplyReq(CNAS_HRM_GetTaskTypeByDataReqType(pstDataReq->enDataReqType));

                pstDataReqBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
            }

            pstDataReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
        }

        /* �����Դ����ʧ�� */
        if (CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL == pstDataReqBuff->stResInfo.enResApplyState)
        {
            /* NO RFΪHRM���ڲ�ԭ��ֵ����SNP�ظ�ԭ��ֵ��reason��butt */
            CNAS_HRM_SndHsmDataCnf(pstDataReq->usOpId, HRM_HSM_DATA_CNF_RSLT_NO_RF, CTTF_HRPD_SNP_DATA_FAIL_REASON_ENUM_BUTT);

            return;
        }

        /* ����״̬��ֻ��������� */
    }
    else
    {
        /* ����Ҫ������Դ�����δ����Դ�����־���쳣 */
        if (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC != pstDataReqBuff->stResInfo.enResApplyState)
        {
            CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_ProcHsmUatiCommProcessDsds1: res isn't applied");
        }

        usOpId = CNAS_HRM_GetOpId();

        /* ���������SNP */
        CNAS_HRM_SndSnpDataReq(usOpId, ulMsgLen, &(pstDataReq->stDataReqCommContent));

        /* �����UATI CMPL����Ҫ������HSM�ظ�UATI CMPL��Ϣ�ѷ��͸�SNP */
        if (CNAS_HRM_DATA_REQ_UATI_CMPL == pstDataReq->enDataReqType)
        {
            CNAS_HRM_SndHsmUatiCmplSndInd(pstDataReq->usOpId);
        }

        /* �軺����Ϣ */
        pstDataReqBuff->stReqMsg.usHrmOpId = usOpId;
    }

    /* ������Ϣ */
    pstDataReqBuff->stReqMsg.ulIsReqMsgExisted = VOS_TRUE;

    pstDataReqBuff->stReqMsg.usReqOpId         = pstDataReq->usOpId;

    if (VOS_NULL_PTR != pstDataReqBuff->stReqMsg.pucMsg)
    {
        CNAS_WARNING_LOG(UEPS_PID_HRM, "CNAS_HRM_ProcHsmDataReqCommProcessDsds1: buff msg is existed");

        PS_MEM_FREE(UEPS_PID_HRM, pstDataReqBuff->stReqMsg.pucMsg);

        pstDataReqBuff->stReqMsg.pucMsg = VOS_NULL_PTR;

        pstDataReqBuff->stReqMsg.usHrmOpId = 0;
    }

    pstDataReqBuff->stReqMsg.pucMsg = (VOS_UINT8 *)PS_MEM_ALLOC(UEPS_PID_HRM, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH);

    NAS_MEM_CPY_S(pstDataReqBuff->stReqMsg.pucMsg, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH,
                  pstDataReq, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH);

}



VOS_VOID CNAS_HRM_RcvHsmUatiBeginNtf(VOS_VOID)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiReqBuff = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiCmplBuff = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType      = CNAS_HRM_GetDsdsType();

    pstUatiReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);

    pstUatiCmplBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

    /* �����DSDS1.0��UATI�����������̣���Ҫ����������Դ��������
        �����DSDS2.0, ֻ����SNP_DATA_REQ��CNF,���Դ˴��������� */
    if (CNAS_HRM_DSDS_TYPE_1 != enDsdsType)
    {
        return;
    }

    /* �յ�ID_HSM_HRM_SESSION_BEGIN_NTFʱ����Ӧ�Ļ��������ʹ�û�����Դδ�ͷ���Ϊ�쳣 */
    if ((VOS_TRUE == pstUatiReqBuff->stReqMsg.ulIsReqMsgExisted)
     || (VOS_TRUE == pstUatiCmplBuff->stReqMsg.ulIsReqMsgExisted))
    {
        /* ���汻ʹ�ã��쳣 */
        CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHsmUatiBeginNtf: buff msg is existed");
    }

    if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstUatiReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstUatiCmplBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstUatiReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstUatiCmplBuff->stResInfo.enResApplyState))
    {
        /* ��Դδ�ͷţ��쳣��ӡ�����ظ�������Դ */
        CNAS_WARNING_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHsmUatiBeginNtf: res isn't released");

        return;
    }

    CNAS_HRM_SndCasHrpdResApplyReq(RFUSER_CAS_HRPD_TASK_TYPE_UATI);

    /* �ж��Ƿ���Ҫ����Դ���������������Ҫ�Ƚ���ñ�־��������յ�CNF֮���� */
    if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
    {
        pstUatiReqBuff->stResInfo.enResApplyState  = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
        pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
    }
    else
    {
        pstUatiReqBuff->stResInfo.enResApplyState  = CNAS_HRM_RES_APPLY_STATE_APPLYING;
        pstUatiCmplBuff->stResInfo.enResApplyState = CNAS_HRM_RES_APPLY_STATE_APPLYING;
    }

    return;
}


VOS_VOID CNAS_HRM_RcvHsmMoKeepAliveBeginNtf(VOS_VOID)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstMoKeepAliveReqBuff = VOS_NULL_PTR;
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType      = CNAS_HRM_GetDsdsType();

    pstMoKeepAliveReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE);

    /* �����DSDS1.0��MO KEEP ALVIE �����������̣���Ҫ����������Դ��������
        �����DSDS2.0, ֻ����SNP_DATA_REQ��CNF,���Դ˴��������� */
    if (CNAS_HRM_DSDS_TYPE_1 != enDsdsType)
    {
        return;
    }

    /* �յ�ID_HSM_HRM_SESSION_BEGIN_NTFʱ����Ӧ�Ļ��������ʹ�û�����Դδ�ͷ���Ϊ�쳣 */
    if (VOS_TRUE == pstMoKeepAliveReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        /* ���汻ʹ�ã��쳣 */
        CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHsmMoKeepAliveBeginNtf: buff msg is existed");
    }

    if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstMoKeepAliveReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstMoKeepAliveReqBuff->stResInfo.enResApplyState))
    {
        /* ��Դδ�ͷţ��쳣��ӡ�����ظ�������Դ */
        CNAS_WARNING_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHsmMoKeepAliveBeginNtf: res isn't released");

        return;
    }

    CNAS_HRM_SndCasHrpdResApplyReq(RFUSER_CAS_HRPD_TASK_TYPE_MO_KEEP_ALIVE);

    /* �ж��Ƿ���Ҫ����Դ���������������Ҫ�Ƚ���ñ�־��������յ�CNF֮���� */
    if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
    {
        pstMoKeepAliveReqBuff->stResInfo.enResApplyState  = CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC;
    }
    else
    {
        pstMoKeepAliveReqBuff->stResInfo.enResApplyState  = CNAS_HRM_RES_APPLY_STATE_APPLYING;
    }

    return;

}


VOS_VOID CNAS_HRM_RcvHsmSessionBeginNtf(
    HSM_HRM_SESSION_BEGIN_NTF_STRU     *pstSessionBeginNtf
)
{
    switch(pstSessionBeginNtf->enSessionType)
    {
        case HSM_HRM_SESSION_TYPE_UATI:
            CNAS_HRM_RcvHsmUatiBeginNtf();
            break;

        case HSM_HRM_SESSION_TYPE_MO_KEEP_ALIVE:
            CNAS_HRM_RcvHsmMoKeepAliveBeginNtf();
            break;

        default:
            CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHsmSessionBeginNtf: invalid session type ");
            break;
    }
}


VOS_VOID CNAS_HRM_RcvHsmUatiEndNtf(VOS_VOID)
{
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiReqBuff = VOS_NULL_PTR;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstUatiCmplBuff = VOS_NULL_PTR;

    enDsdsType = CNAS_HRM_GetDsdsType();

    pstUatiReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);

    pstUatiCmplBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

    /* �����DSDS1.0��UATI�����������̣��ͷ���Դʱ������� */
    if (CNAS_HRM_DSDS_TYPE_1 != enDsdsType)
    {
        return;
    }

    if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstUatiReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstUatiCmplBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstUatiReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstUatiCmplBuff->stResInfo.enResApplyState))
    {
        /* ��Դ���ظ��ͷ� */
        CNAS_HRM_SndCasHrpdResRelNtf(RFUSER_CAS_HRPD_TASK_TYPE_UATI);
    }

    CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ);
    CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL);

    return;
}


VOS_VOID CNAS_HRM_RcvHsmMoKeepAliveEndNtf(VOS_VOID)
{
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstMoKeepAliveReqBuff = VOS_NULL_PTR;

    enDsdsType = CNAS_HRM_GetDsdsType();

    pstMoKeepAliveReqBuff  = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE);

    /* �����DSDS1.0��UATI�����������̣��ͷ���Դʱ������� */
    if (CNAS_HRM_DSDS_TYPE_1 != enDsdsType)
    {
        return;
    }

    if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstMoKeepAliveReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstMoKeepAliveReqBuff->stResInfo.enResApplyState))
    {
        /* ��Դ���ظ��ͷ� */
        CNAS_HRM_SndCasHrpdResRelNtf(RFUSER_CAS_HRPD_TASK_TYPE_MO_KEEP_ALIVE);
    }

    CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE);

    return;

}


VOS_VOID CNAS_HRM_RcvHsmSessionEndNtf(
    HSM_HRM_SESSION_END_NTF_STRU       *pstSessionEndNtf
)
{
    switch(pstSessionEndNtf->enSessionType)
    {
        case HSM_HRM_SESSION_TYPE_UATI:
            CNAS_HRM_RcvHsmUatiEndNtf();
            break;

        case HSM_HRM_SESSION_TYPE_MO_KEEP_ALIVE:
            CNAS_HRM_RcvHsmMoKeepAliveEndNtf();
            break;

        default:
            CNAS_ERROR_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHsmSessionEndNtf: invalid session type ");
            break;
    }
}



VOS_VOID CNAS_HRM_ProcHsmUatiReqDsds1(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcessDsds1(pstDataReq);

}


VOS_VOID CNAS_HRM_ProcHsmUatiReqDsds2(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcess(pstDataReq);
}



VOS_VOID CNAS_HRM_RcvHsmUatiReq(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();

    if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
    {
        CNAS_HRM_ProcHsmUatiReqDsds1(pstDataReq);
        return;
    }

    if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
    {
        CNAS_HRM_ProcHsmUatiReqDsds2(pstDataReq);
        return;
    }
}


VOS_VOID CNAS_HRM_ProcHsmUatiCmplDsds1(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcessDsds1(pstDataReq);
}


VOS_VOID CNAS_HRM_ProcHsmUatiCmplDsds2(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcess(pstDataReq);
}



VOS_VOID CNAS_HRM_RcvHsmUatiCmpl(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();

    if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
    {
        CNAS_HRM_ProcHsmUatiCmplDsds1(pstDataReq);
        return;
    }

    if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
    {
        CNAS_HRM_ProcHsmUatiCmplDsds2(pstDataReq);
        return;
    }
}


VOS_VOID CNAS_HRM_RcvHsmHarewareId(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcess(pstDataReq);
}


VOS_VOID CNAS_HRM_ProcHsmMoKeepAliveReqDsds1(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcessDsds1(pstDataReq);
}


VOS_VOID CNAS_HRM_ProcHsmMoKeepAliveReqDsds2(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcess(pstDataReq);
}



VOS_VOID CNAS_HRM_RcvHsmMoKeepAlive(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;

    enDsdsType = CNAS_HRM_GetDsdsType();

    if (CNAS_HRM_DSDS_TYPE_1 == enDsdsType)
    {
        CNAS_HRM_ProcHsmMoKeepAliveReqDsds1(pstDataReq);
        return;
    }

    if (CNAS_HRM_DSDS_TYPE_2 == enDsdsType)
    {
        CNAS_HRM_ProcHsmMoKeepAliveReqDsds2(pstDataReq);
        return;
    }
}


VOS_VOID CNAS_HRM_RcvHsmMtKeepAlive(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcess(pstDataReq);
}


VOS_VOID CNAS_HRM_RcvHsmSessionClose(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    CNAS_HRM_ProcHsmDataReqCommProcess(pstDataReq);
}



VOS_VOID CNAS_HRM_RcvHsmDataReq(
    HSM_HRM_DATA_REQ_STRU              *pstDataReq
)
{
    switch (pstDataReq->enDataReqType)
    {
        case CNAS_HRM_DATA_REQ_UATI_REQUEST:
            CNAS_HRM_RcvHsmUatiReq(pstDataReq);
            break;

        case CNAS_HRM_DATA_REQ_UATI_CMPL:
            CNAS_HRM_RcvHsmUatiCmpl(pstDataReq);
            break;

        case CNAS_HRM_DATA_REQ_HARDWARE_ID:
            CNAS_HRM_RcvHsmHarewareId(pstDataReq);
            break;

        case CNAS_HRM_DATA_REQ_MO_KEEP_ALIVE:
            CNAS_HRM_RcvHsmMoKeepAlive(pstDataReq);
            break;

        case CNAS_HRM_DATA_REQ_MT_KEEP_ALIVE:
            CNAS_HRM_RcvHsmMtKeepAlive(pstDataReq);
            break;

        case CNAS_HRM_DATA_REQ_SESSION_CLOSE:
            CNAS_HRM_RcvHsmSessionClose(pstDataReq);
            break;

        default:
            break;
     }

}


VOS_VOID CNAS_HRM_RcvHsmDataCancelNtf(
    HSM_HRM_DATA_CANCEL_NTF_STRU       *pstDataCancelNtf
)
{
    VOS_UINT8                           ucBuffIndex;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstReqBuff = VOS_NULL_PTR;
    VOS_UINT8                           ucIsNeedRelRes;

    ucIsNeedRelRes    = VOS_TRUE;
    ucBuffIndex       = CNAS_HRM_GetMsgBuffIndexByDataReqType(pstDataCancelNtf->enDataReqType);
    pstReqBuff        = CNAS_HRM_GetDataReqBuffAddr(ucBuffIndex);


    /* UATI��MO KEEP ALIVEֻ����DSDS2.0ʱ�����ͷ���Դ��DSDS1.0���յ�HSM��ID_HSM_HRM_SESSION_END_NTFʱ�ͷ���Դ
        �������ͣ��ͷ���Դ */
    if (VOS_TRUE == CNAS_HRM_IsDsds1FullFlowResProtect(ucBuffIndex))
    {
        ucIsNeedRelRes = VOS_FALSE;
    }

    /* �޶�Ӧ�ĵĻ�����Ϣ */
    if (VOS_TRUE != pstReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        return;
    }

    /* ����ռ��Դ����ɹ�����������������Դ����ȡ��SNP���ݷ��� */
    if (((CNAS_HRM_RES_OCCUPY_TYPE_NON == pstReqBuff->stResInfo.enResOccupyType)
       && (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstReqBuff->stResInfo.enResApplyState))
     || ((CNAS_HRM_RES_OCCUPY_TYPE_SHORT == pstReqBuff->stResInfo.enResOccupyType)
       && (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstReqBuff->stResInfo.enResApplyState)))
    {
        CNAS_HRM_SndSnpDataCancelReq(pstReqBuff->stReqMsg.usHrmOpId);
    }

    if (VOS_TRUE == ucIsNeedRelRes)
    {
        /* �ͷ���Դ */
        if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstReqBuff->stResInfo.enResApplyState)
         || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstReqBuff->stResInfo.enResApplyState))
        {
            CNAS_HRM_SndCasHrpdResRelNtf(CNAS_HRM_GetTaskTypeByDataReqType(pstDataCancelNtf->enDataReqType));
        }

        /* �建�� */
        CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucBuffIndex);
        CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(ucBuffIndex);
    }
    else
    {
        CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucBuffIndex);
    }
}


VOS_VOID CNAS_HRM_RcvHsmMsg(
    struct MsgCB                       *pstMsg
)
{

    VOS_UINT16                          usMsgName;

    usMsgName = *((VOS_UINT16 *)(pstMsg->aucValue));

    switch (usMsgName)
    {
        case ID_HSM_HRM_SESSION_BEGIN_NTF:
            CNAS_HRM_RcvHsmSessionBeginNtf((HSM_HRM_SESSION_BEGIN_NTF_STRU*)pstMsg);
            break;

        case ID_HSM_HRM_SESSION_END_NTF:
            CNAS_HRM_RcvHsmSessionEndNtf((HSM_HRM_SESSION_END_NTF_STRU*)pstMsg);
            break;

        case ID_HSM_HRM_DATA_REQ:
            CNAS_HRM_RcvHsmDataReq((HSM_HRM_DATA_REQ_STRU*)pstMsg);
            break;

        case ID_HSM_HRM_DATA_CANCEL_NTF:
            CNAS_HRM_RcvHsmDataCancelNtf((HSM_HRM_DATA_CANCEL_NTF_STRU*)pstMsg);
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



