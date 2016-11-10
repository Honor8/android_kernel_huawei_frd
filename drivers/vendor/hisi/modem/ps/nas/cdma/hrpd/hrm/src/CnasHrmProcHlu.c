

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CnasHrmProcHlu.h"
#include "CnasHrmSndCas.h"
#include "CnasHrmSndTtf.h"
#include "CnasHrmCtx.h"
#include "CnasMntn.h"
#include "hlu_hrm_pif.h"
#include "Nas_Mem.h"
#include "CnasHrmCommFunc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define THIS_FILE_ID                    PS_FILE_ID_CNAS_HRM_PROC_HLU_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


VOS_VOID  CNAS_HRM_RcvHluDataReq(
    HLU_HRM_DATA_REQ_STRU                                  *pstDataReq
)
{
    CNAS_HRM_DATA_REQ_BUFF_STRU                            *pstLuReqBuff = VOS_NULL_PTR;
    VOS_UINT16                                              usOpId;
    VOS_UINT32                                              ulMsgLen;
    VOS_UINT8                                               ucIndex;

    ucIndex      = CNAS_HRM_GetMsgBuffIndexByDataReqType(pstDataReq->enDataReqType);

    pstLuReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

    /* ��Ӧ�Ļ��������ʹ����Ϊ�쳣 */
    if (VOS_TRUE == pstLuReqBuff->stReqMsg.ulIsReqMsgExisted)
    {
        /* ���汻ʹ�ã��쳣 */
        CNAS_WARNING_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHluDataReq: buff msg is existed");

        CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
    }

    if ((CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstLuReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstLuReqBuff->stResInfo.enResApplyState))
    {
        /* ��Դδ�ͷţ����ظ����� */
        CNAS_WARNING_LOG(UEPS_PID_HRM, "CNAS_HRM_RcvHluDataReq: res isn't released");
    }
    else
    {
        CNAS_HRM_SndCasHrpdResApplyReq(RFUSER_CAS_HRPD_TASK_TYPE_LU);

        /* �ж��Ƿ���Ҫ����Դ���������������Ҫ�Ƚ�������󷢸�SNP��������յ�CNF֮���� */
        if (VOS_FALSE == CNAS_HRM_IsNeedWaitResApplied())
        {
            CNAS_HRM_UpdateLuResApplyState(CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC);
        }
        else
        {
            CNAS_HRM_UpdateLuResApplyState(CNAS_HRM_RES_APPLY_STATE_APPLYING);
        }
    }


    /* ��ʹ�ñ�־ */
    pstLuReqBuff->stReqMsg.ulIsReqMsgExisted  = VOS_TRUE;

    if (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstLuReqBuff->stResInfo.enResApplyState)
    {
        usOpId = CNAS_HRM_GetOpId();

        /* ���������SNP */

        ulMsgLen =  pstDataReq->ulLength - sizeof(pstDataReq->enDataReqType);

        CNAS_HRM_SndSnpDataReq(usOpId,
                               ulMsgLen,
                               &(pstDataReq->stDataReqCommContent));

        pstLuReqBuff->stReqMsg.usHrmOpId = usOpId;

    }

    pstLuReqBuff->stReqMsg.usReqOpId = pstDataReq->usOpId;

    pstLuReqBuff->stReqMsg.pucMsg = (VOS_UINT8 *)PS_MEM_ALLOC(UEPS_PID_HRM, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH);

    NAS_MEM_CPY_S(pstLuReqBuff->stReqMsg.pucMsg, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH,
                  pstDataReq, pstDataReq->ulLength + VOS_MSG_HEAD_LENGTH);

}


VOS_VOID  CNAS_HRM_RcvHluDataCancelNtf(VOS_VOID)
{
    VOS_UINT8                                               ucIndex;
    CNAS_HRM_DATA_REQ_BUFF_STRU                            *pstDataReqBuff = VOS_NULL_PTR;

    pstDataReqBuff = CNAS_HRM_GetDataReqBuffAddr(CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_NTF);

    /* LU�����Ϣ��Դ�������������ͬ�ģ����Ը���Ϣ����Դ����״̬Ҳ��һ���ģ�
        ����ֻ��Ҫ������һ����Ϣ����Դ����״̬�жϼ��ɡ�
        ��Դ������ɹ�����������ʱ�����ͷ���Դ ֻ��Ҫ�ͷ�һ�� */
    if ((CNAS_HRM_RES_APPLY_STATE_APPLYING   == pstDataReqBuff->stResInfo.enResApplyState)
     || (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstDataReqBuff->stResInfo.enResApplyState))
    {
        CNAS_HRM_SndCasHrpdResRelNtf(RFUSER_CAS_HRPD_TASK_TYPE_LU);
    }

    for (ucIndex = CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_NTF; ucIndex <= CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_CMPL; ucIndex++)
    {
        pstDataReqBuff = CNAS_HRM_GetDataReqBuffAddr(ucIndex);

        if (VOS_TRUE == pstDataReqBuff->stReqMsg.ulIsReqMsgExisted)
        {
            /* ����ռ��Դ����ɹ���������Դ�������룬��ȡ��SNP���ݷ��� */
            if (((CNAS_HRM_RES_OCCUPY_TYPE_NON == pstDataReqBuff->stResInfo.enResOccupyType)
               && (CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC == pstDataReqBuff->stResInfo.enResApplyState))
             || ((CNAS_HRM_RES_OCCUPY_TYPE_SHORT == pstDataReqBuff->stResInfo.enResOccupyType)
               && (CNAS_HRM_RES_APPLY_STATE_APPLYING == pstDataReqBuff->stResInfo.enResApplyState)))
            {
                CNAS_HRM_SndSnpDataCancelReq(pstDataReqBuff->stReqMsg.usHrmOpId);
            }

            /* �����Ӧ�Ļ�����Ϣ */
            CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(ucIndex);
        }

        CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(ucIndex);
    }

}


VOS_VOID  CNAS_HRM_RcvHluMsg(
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT16                          usMsgName;

    usMsgName = *((VOS_UINT16 *)(pstMsg->aucValue));

    switch (usMsgName)
    {

        case ID_HLU_HRM_DATA_REQ:
            CNAS_HRM_RcvHluDataReq((HLU_HRM_DATA_REQ_STRU*)pstMsg);
            break;

        case ID_HLU_HRM_DATA_CANCEL_NTF:
            CNAS_HRM_RcvHluDataCancelNtf();
            break;

        default:
            break;
     }

     return;

}

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*lint -save -e958*/


/*lint -restore*/

#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


