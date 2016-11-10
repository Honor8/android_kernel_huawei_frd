

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CnasHrmProcTtf.h"
#include "CnasHrmSndHlu.h"
#include "CnasHrmSndHsm.h"
#include "CnasHrmSndCas.h"
#include "CnasHrmCtx.h"
#include "CnasHrmCommFunc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define THIS_FILE_ID                    PS_FILE_ID_CNAS_HRM_PROC_TTF_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*lint -save -e958*/


VOS_VOID  CNAS_HRM_RcvFsigSnpDataIndMsg(
    CTTF_CNAS_HRPD_SNP_DATA_IND_STRU   *pstSnpDataInd
)
{
    CTTF_HRPD_PROTOCOL_TYPE_ENUM_UINT16                     enProtocolType;

    enProtocolType = pstSnpDataInd->enProtocolType;

    /* ����protocaltype�ַ���HSM����HLU */
    if ((CTTF_HRPD_SNP_SESSION_MANAGEMENT_PROTOCOL == enProtocolType)
     || (CTTF_HRPD_SNP_ADDRESS_MANAGEMENT_PROTOCOL == enProtocolType))
    {
        CNAS_HRM_SndHsmDataInd(pstSnpDataInd->enProtocolType,
                               pstSnpDataInd->enInstanceType,
                               pstSnpDataInd->usOpId,
                               &(pstSnpDataInd->stMsgData));
    }
    else if(CTTF_HRPD_SNP_STREAM_2_APPLICATION_PROTOCOL == enProtocolType)
    {
        CNAS_HRM_SndHluDataInd(pstSnpDataInd->enProtocolType,
                               pstSnpDataInd->enInstanceType,
                               pstSnpDataInd->usOpId,
                               &(pstSnpDataInd->stMsgData));
    }
    else
    {

    }
}


VOS_VOID  CNAS_HRM_ProcSnpDataCnfMsgAboutHsm(
    VOS_UINT8                           ucIndex,
    CTTF_CNAS_HRPD_SNP_DATA_CNF_STRU   *pstMsg
)
{
    HRM_HSM_DATA_CNF_RSLT_ENUM_UINT32                       enRslt;
    VOS_UINT16                                              usOpId;
    CNAS_HRM_DATA_REQ_BUFF_STRU                            *pstReqBuff = VOS_NULL_PTR;

    pstReqBuff        = CNAS_HRM_GetDataReqBuffAddr(ucIndex);
    usOpId            = pstReqBuff->stReqMsg.usReqOpId;

    if (CTTF_HRPD_SNP_RLST_SUCC == pstMsg->enRslt)
    {
        enRslt = HRM_HSM_DATA_CNF_RSLT_SUCC;
    }
    else
    {
        /* ������Դ����ʧ�ܺ��յ�SNP��DATA CNF����Ҫ�ظ�NO RF */
        if (CNAS_HRM_RES_OCCUPY_TYPE_NON != pstReqBuff->stResInfo.enResOccupyType)
        {
            enRslt = HRM_HSM_DATA_CNF_RSLT_NO_RF;
        }
        else
        {
            /* ���յ���Դ��ռ��Ϣ֮ǰ���յ�SNP��DATA CNF��ԭ��ֵΪ����ռ */
            if (CTTF_HRPD_SNP_DATA_FAIL_REASON_FORCE_CLEAR_TX_AC_CHAN_DATA == pstMsg->enFailReason)
            {
                enRslt = HRM_HLU_DATA_CNF_RSLT_NO_RF;
            }
            else
            {
                enRslt = HRM_HLU_DATA_CNF_RSLT_FAIL;
            }
        }
    }

    CNAS_HRM_SndHsmDataCnf(usOpId, enRslt, pstMsg->enFailReason);

    /* UATI��MO KEEP ALIVEֻ����DSDS2.0ʱ�����ͷ���Դ��DSDS1.0���յ�HSM��ID_HSM_HRM_SESSION_END_NTFʱ�ͷ���Դ
        �������ͣ��ͷ���Դ */
    if (VOS_TRUE == CNAS_HRM_IsDsds1FullFlowResProtect(ucIndex))
    {
        CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);

        return;
    }
    else
    {
        if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstReqBuff->stResInfo.enResApplyState)
         || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstReqBuff->stResInfo.enResApplyState))
        {
            CNAS_HRM_SndCasHrpdResRelNtf(CNAS_HRM_GetTaskTypeByDataReqType(CNAS_HRM_GetDataReqTypeByMsgBuffIndex(ucIndex)));
        }
    }

    /* ������� */
    CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
    CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(ucIndex);
}


VOS_VOID  CNAS_HRM_ProcSnpDataCnfMsgAboutHlu(
    VOS_UINT8                           ucIndex,
    CTTF_CNAS_HRPD_SNP_DATA_CNF_STRU   *pstMsg
)
{
    HRM_HLU_DATA_CNF_RSLT_ENUM_UINT32   enRslt;
    CNAS_HRM_DATA_REQ_BUFF_STRU        *pstReqBuff = VOS_NULL_PTR;

    pstReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

    /* ��HLU �ظ�DataCnf*/
    if (CTTF_HRPD_SNP_RLST_SUCC == pstMsg->enRslt)
    {
        enRslt = HRM_HLU_DATA_CNF_RSLT_SUCC;
    }
    else
    {
        /* �������ռ���ڣ���HLU�ظ� NO RF */
        if (CNAS_HRM_RES_OCCUPY_TYPE_NON != pstReqBuff->stResInfo.enResOccupyType)
        {
            enRslt = HRM_HLU_DATA_CNF_RSLT_NO_RF;
        }
        else
        {
            /* ���յ���Դ��ռ��Ϣ֮ǰ���յ�SNP��DATA CNF��ԭ��ֵΪ����ռ */
            if (CTTF_HRPD_SNP_DATA_FAIL_REASON_FORCE_CLEAR_TX_AC_CHAN_DATA == pstMsg->enFailReason)
            {
                enRslt = HRM_HLU_DATA_CNF_RSLT_NO_RF;
            }
            else
            {
                enRslt = HRM_HLU_DATA_CNF_RSLT_FAIL;
            }
        }
    }

    CNAS_HRM_SndHluDataCnf(pstReqBuff->stReqMsg.usReqOpId, enRslt, pstMsg->enFailReason);

    /* ���������Ϣ  */
    CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);

    /* û��LU�����Ϣ����ʱ�����ͷ���Դ  */
    if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstReqBuff->stResInfo.enResApplyState))
    {
        if (VOS_FALSE == CNAS_HRM_IsLuDataReqBuffExisted())
        {
            CNAS_HRM_SndCasHrpdResRelNtf(RFUSER_CAS_HRPD_TASK_TYPE_LU);

            CNAS_HRM_UpdateLuResApplyState(CNAS_HRM_RES_APPLY_STATE_NOT_APPLY);
        }
    }

    pstReqBuff->stResInfo.enResOccupyType = CNAS_HRM_RES_OCCUPY_TYPE_NON;
}


VOS_VOID  CNAS_HRM_RcvFsigSnpDataCnfMsg(
    CTTF_CNAS_HRPD_SNP_DATA_CNF_STRU   *pstMsg
)
{
    VOS_UINT8                           ucIndex;

    ucIndex   = CNAS_HRM_GetMsgBuffIndexByHrmOpId(pstMsg->usOpId);

    switch (ucIndex)
    {
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_HARDWARE_ID:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_MT_KEEP_ALVIE:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_SESSION_CLOSE:
            CNAS_HRM_ProcSnpDataCnfMsgAboutHsm(ucIndex, pstMsg);
            break;

        case CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_NTF:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_HANDOFF_LOC_NTF:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_CMPL:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_NTF:
        case CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_CMPL:
            CNAS_HRM_ProcSnpDataCnfMsgAboutHlu(ucIndex, pstMsg);
            break;

        default:
            break;

    }

}



VOS_VOID  CNAS_HRM_RcvFsigMsg(
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT16                          usMsgName;

    usMsgName = *((VOS_UINT16 *)(pstMsg->aucValue));

    switch (usMsgName)
    {
        case ID_CTTF_CNAS_HRPD_SNP_DATA_IND:
            CNAS_HRM_RcvFsigSnpDataIndMsg((CTTF_CNAS_HRPD_SNP_DATA_IND_STRU*)pstMsg);
            break;

        case ID_CTTF_CNAS_HRPD_SNP_DATA_CNF:
            CNAS_HRM_RcvFsigSnpDataCnfMsg((CTTF_CNAS_HRPD_SNP_DATA_CNF_STRU*)pstMsg);
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


