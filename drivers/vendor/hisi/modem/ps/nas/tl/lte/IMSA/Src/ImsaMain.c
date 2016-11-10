/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaMain.c
  Description     : ��C�ļ�������IMSAģ���ʼ������Ϣ������ڵ�ʵ��
  History           :
     1.sunbing 49683      2013-04-23  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "PsTypeDef.h"
#include "ImsaMain.h"
#include "ImsaEntity.h"
#include "ImsaPublic.h"
#include "ImsaImsAdaption.h"
#include "UsimPsInterface.h"
#include "ImsaProcTimerMsg.h"
#include "ImsaProcCdsMsg.h"
#include "ImsaServiceManagement.h"
#include "ImsaProcMsccMsg.h"
#include "ImsaConnManagement.h"
#include "ImsaServiceManagement.h"
#include "ImsaRegManagement.h"
#include "ImsaCallManagement.h"
#include "VcImsaInterface.h"
#include "ImsaDsDsPublic.h"
#include "ImsaDomainManagement.h"
#if (VOS_OS_VER != VOS_WIN32)
#include "siapppih.h"
#endif
#include "ImsaProcApsMsg.h"
#include "ImsaProcSmsMsg.h"
#include "CallImsaInterface.h"
#include "ImsaProcAtMsg.h"
#include "ImsaProcSpmMsg.h"
#include "ImsaProcUssdMsg.h"

#if (FEATURE_ON == FEATURE_PTM)
#include "ImsaProcOmMsg.h"
#endif

#include "ImsaMtcInterface.h"

#include "ImsaLmmInterface.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_IMSAMAIN_C
#define    THIS_NAS_FILE_ID    NAS_FILE_ID_IMSAMAIN_C
/*lint +e767*/



/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#if (FEATURE_ON == FEATURE_IMS)


/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
IMSA_ENTITY_STRU                        g_stImsaEntity;
IMSA_ENTITY_STRU                       *pgstImsaEntity = &g_stImsaEntity;

/* NULL State */
const IMSA_DOMAIN_MSG_FUNC_TBL_STRU    g_astImsaDomainMsgFuncTbl[] =
{
   { WUEPS_PID_MAPCON,  ID_WIFI_IMSA_POWERON_REQ,               IMSA_Domain_ProcWifiPowerOnReq},
   { WUEPS_PID_MAPCON,  ID_WIFI_IMSA_POWEROFF_REQ,              IMSA_Domain_ProcWifiPowerOffReq},
   { WUEPS_PID_MAPCON,  ID_WIFI_IMSA_STATE_IND,                 IMSA_Domain_ProcWifiStateInd},
   { UEPS_PID_MSCC,     ID_MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ,    IMSA_Domain_ProcMsccImsDomainCfgSetReq},
   { UEPS_PID_MSCC,     ID_MSCC_IMSA_START_REQ,                 IMSA_Domain_ProcMsccStartReq},
   { UEPS_PID_MSCC,     ID_MSCC_IMSA_STOP_REQ,                  IMSA_Domain_ProcMsccStopReq},
   { UEPS_PID_MSCC,     ID_MSCC_IMSA_SERVICE_CHANGE_IND,        IMSA_Domain_ProcMsccServiceChangeInd},
   { UEPS_PID_MSCC,     ID_MSCC_IMSA_REG_REQ,                   IMSA_Domain_ProcMsccMsgRegReq},
   { UEPS_PID_MSCC,     ID_MSCC_IMSA_DEREG_REQ,                 IMSA_Domain_ProcMsccMsgDeregReq},
   { WUEPS_PID_USIM,    USIMM_READFILE_CNF,                     IMSA_Domain_ProcUsimReadFileCnf},
   { PS_PID_ERRC,       ID_LRRC_IMSA_STATE_IND,                 IMSA_Domain_ProcLrrcStateInd},
   { PS_PID_ERRC,       ID_LRRC_IMSA_QUALITY_CFG_CNF,           IMSA_Domain_ProcLrrcQualityConfigCnf},
   { WUEPS_PID_MAPCON,  ID_WIFI_IMSA_ERROR_IND,                 IMSA_Domain_ProcWifiErrorInd},
   { UEPS_PID_MSCC,     ID_MSCC_IMSA_AREA_LOST_IND,             IMSA_Domain_ProcAreaaLostInd}
};

const VOS_UINT32 g_astImsaDomainMsgFuncTblSize  = sizeof(g_astImsaDomainMsgFuncTbl) / sizeof(g_astImsaDomainMsgFuncTbl[0]);



/*****************************************************************************
  3 Function
*****************************************************************************/
extern VOS_VOID IMSA_MsgHandle(const VOS_VOID *pRcvMsg );

extern VOS_VOID IMSA_MsgHandleCommon(const VOS_VOID *pRcvMsg );
extern VOS_UINT32 IMSA_DomainMsgDistr( const VOS_VOID *pRcvMsg,VOS_UINT32 ulSenderPid, VOS_UINT32 ulRcvMsgId);

extern VOS_VOID IMSA_IntraMsgProcess(VOS_VOID);

extern VOS_VOID IMSA_TafMsgDistr(const VOS_VOID *pRcvMsg );

extern VOS_VOID IMSA_IntraMsgDistr(const VOS_VOID *pRcvMsg );

extern VOS_VOID IMSA_ImsMsgDistr(const VOS_VOID *pRcvMsg );

extern VOS_VOID IMSA_UsimMsgDistr(const VOS_VOID *pRcvMsg );

extern VOS_VOID IMSA_PihMsgDistr(const VOS_VOID *pRcvMsg );

extern VOS_VOID IMSA_VcMsgDistr(const VOS_VOID *pRcvMsg);

extern VOS_VOID IMSA_AtMsgDistr(const VOS_VOID *pRcvMsg);
#if (FEATURE_ON == FEATURE_PTM)
extern VOS_VOID IMSA_OmMsgDistr(const VOS_VOID *pRcvMsg);
#endif

extern VOS_VOID IMSA_MtcMsgDistr(const VOS_VOID *pRcvMsg);


extern VOS_VOID IMSA_Init(VOS_VOID);
extern VOS_VOID IMSA_PrintState( VOS_VOID );
extern VOS_UINT8 * IMSA_GetNextIntraMsg(VOS_VOID);
extern VOS_UINT32 IMSA_PidInit ( enum VOS_INIT_PHASE_DEFINE ip );
extern VOS_UINT32 IMSVA_PidInit ( enum VOS_INIT_PHASE_DEFINE ip );
extern VOS_VOID IMSA_TaskEntry(MsgBlock * pMsg);
extern VOS_VOID IMSVA_TaskEntry(MsgBlock * pMsg);

extern VOS_VOID IMSA_LmmMsgDistr(const VOS_VOID * pMsg);


extern VOS_VOID IMSA_GetPidMsgName(const VOS_VOID *pRcvMsg, VOS_UINT32 *pulSenderPid, VOS_UINT32 *pulRcvMsgId);
/*****************************************************************************
  3 Function
*****************************************************************************/




VOS_UINT32 IMSA_DomainMsgDistr( const VOS_VOID *pRcvMsg,VOS_UINT32 ulSenderPid, VOS_UINT32 ulRcvMsgId)
{
    VOS_UINT32             i  = IMSA_NULL;
    VOS_UINT32             ulContinueFlag  = VOS_TRUE;
    IMSA_DOMAIN_MSG_FUNC   pDomainMsgFunc  = VOS_NULL_PTR;


    /* ���¼�������в��Ҵ����� */
    for ( i = 0; i < g_astImsaDomainMsgFuncTblSize; i++ )
    {
        if (( ulSenderPid == g_astImsaDomainMsgFuncTbl[i].ulSenderPid ) &&
            ( ulRcvMsgId == g_astImsaDomainMsgFuncTbl[i].ulMsgId))
        {
            /* �¼�IDƥ�� */
            pDomainMsgFunc = g_astImsaDomainMsgFuncTbl[i].pDomainMsgFunc;
            break;
        }
    }

    /* ������������������ */
    if ( VOS_NULL_PTR != pDomainMsgFunc )
    {
        ulContinueFlag = pDomainMsgFunc(pRcvMsg);
    }
    else
    {
        ulContinueFlag = VOS_TRUE;
    }

    return ulContinueFlag;
}

/*****************************************************************************
 Function Name  : IMSA_Init()
 Description    : IMSA��ʼ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1. sunbing 49683      2013-04-23  Draft Enact

*****************************************************************************/
VOS_VOID IMSA_Init(VOS_VOID)
{
    /*��ģ��ĳ�ʼ��*/
    IMSA_CONN_Init();

    IMSA_CONTROL_Init();

    IMSA_RegMgrInit();

    (VOS_VOID)IMSA_CallCtxInit();

    IMSA_SMS_Init();

    IMSA_ResetImsOpId();

    IMSA_ResetUsimOpId();

    IMSA_USSD_Init();

    #if (FEATURE_ON == FEATURE_DSDS)
    IMSA_DsdsInit();
    #endif
    #if (FEATURE_ON == FEATURE_PTM)
    IMSA_InitErrLogInfo();
    #endif
    IMSA_ImsAdaption_Init();

    IMSA_Domain_Init();
    IMSA_ImsAudioListInit();
}


VOS_VOID IMSA_ClearResource( VOS_VOID )
{
    IMSA_CONN_ClearResource();

    IMSA_CONTROL_ClearResource();

    IMSA_RegMgrDeinit();

    (VOS_VOID)IMSA_CallCtxDeinit();

    IMSA_SMS_ClearResource();

    IMSA_USSD_ClearResource();

    IMSA_ResetImsOpId();

    IMSA_ResetUsimOpId();

    IMSA_Domain_ClearResource();

    #if (FEATURE_ON == FEATURE_DSDS)
    IMSA_DsdsClearResource();
    #endif

    IMSA_ImsAdaption_ClearResource();
}


VOS_VOID IMSA_PrintState( VOS_VOID )
{
    IMSA_Domain_PrintDomainFsmState();
    IMSA_SRV_PrintNrmSrvState();
    IMSA_CONN_PrintNrmConnState();
    IMSA_CONN_PrintWifiNrmConnState();
    IMSA_RegPrintNrmRegState();
    IMSA_SRV_PrintEmcSrvState();
    IMSA_CONN_PrintEmcConnState();
    IMSA_RegPrintEmcRegState();
}


VOS_VOID IMSA_MsgHandle(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;
    VOS_UINT32                  ulSenderPid = VOS_TRUE;
    VOS_UINT32                  ulRcvMsgId = VOS_TRUE;
    VOS_UINT32                  ulContinueFlg = VOS_TRUE;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    IMSA_PrintImsaRevMsg(pHeader,IMSA_GET_IMSA_PRINT_BUF());
    IMSA_PrintState();


    IMSA_GetPidMsgName(pRcvMsg, &ulSenderPid,&ulRcvMsgId);

    IMSA_INFO_LOG("IMSA_MsgHandle:enter");

    ulContinueFlg = IMSA_DomainMsgDistr(pRcvMsg,ulSenderPid,ulRcvMsgId);

    /* �����Ϣû����״̬������������Ȼ�Ѿ���������Ȼ��Ҫ������������ù�����Ϣ������ */
    if(VOS_TRUE == ulContinueFlg)
    {
        IMSA_MsgHandleCommon(pRcvMsg);
    }
}



VOS_VOID IMSA_GetPidMsgName(const VOS_VOID *pRcvMsg, VOS_UINT32 *pulSenderPid, VOS_UINT32 *pulRcvMsgId)
{

/* ������Ϣͷָ��*/
   PS_MSG_HEADER_STRU        *pHeader = VOS_NULL_PTR;
   AT_IMSA_IMS_CTRL_MSG_STRU *pAtWifiImsaCtrlMsg = VOS_NULL_PTR;
   WIFI_IMSA_MSG_STRU        *pWifiImsaMsg = VOS_NULL_PTR;


 /* ��ȡ��Ϣͷָ��*/
   pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;
   *pulSenderPid = pHeader->ulSenderPid;
   *pulRcvMsgId = pHeader->ulMsgName;

   switch(pHeader->ulSenderPid)
   {

       case WUEPS_PID_AT:
           if(ID_AT_IMSA_IMS_CTRL_MSG==pHeader->ulMsgName)
            {
                pAtWifiImsaCtrlMsg = (AT_IMSA_IMS_CTRL_MSG_STRU *)pRcvMsg;
                pWifiImsaMsg = (WIFI_IMSA_MSG_STRU *) (pAtWifiImsaCtrlMsg->aucWifiMsg);
                *pulSenderPid = WUEPS_PID_MAPCON;
                *pulRcvMsgId = (VOS_UINT32)(pWifiImsaMsg->lMsgId);

                IMSA_INFO_LOG1("IMSA_GetPidMsgName:*pulRcvMsgId =!",*pulRcvMsgId);
            }
           break;
/* Internal Review to close current unused PID&MsgId*/
#if 0
       case WUEPS_PID_TAF:
           if(MN_CALLBACK_PS_CALL == pHeader->ulMsgName)
           {
                pTafPsEvt = (TAF_PS_EVT_STRU *) pRcvMsg;
                *pulSenderPid = WUEPS_PID_TAF;
                *pulRcvMsgId = pTafPsEvt->ulEvtId;
            }
             break;

        case VOS_PID_TIMER:
            *pulSenderPid = VOS_PID_TIMER;
             *pulRcvMsgId = (VOS_UINT16)PS_GET_REL_TIMER_NAME(pRcvMsg);
            break;

        case PS_PID_IMSA:
            *pulSenderPid = PS_PID_IMSA;
            *pulRcvMsgId = pHeader->ulMsgName;
#endif
        default:
            break;
   }
}


VOS_VOID IMSA_MsgHandleCommon(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU  *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    //IMSA_PrintImsaRevMsg(pHeader,IMSA_GET_IMSA_PRINT_BUF());
    //IMSA_PrintState();

    switch(pHeader->ulSenderPid)
    {
        /*TAF�ж��ģ�飬��Ҫ��һ���ַ�*/
        case WUEPS_PID_TAF:
            IMSA_TafMsgDistr(pRcvMsg);
            break;

        case UEPS_PID_MSCC:
            IMSA_ProcMsccMsg(pRcvMsg);
            break;

        case VOS_PID_TIMER:
            IMSA_TimerMsgDistr((REL_TIMER_MSG *)pRcvMsg);
            break;

        case PS_PID_IMSA:
            IMSA_IntraMsgDistr(pRcvMsg);
            break;

        case PS_PID_IMSVA:
            IMSA_ImsMsgDistr(pRcvMsg);
            break;

        /*USIM��Ϣ����*/
        case WUEPS_PID_USIM:
            IMSA_UsimMsgDistr(pRcvMsg);
            break;

        /*��USIMע��PID��USIM��ͨ�����PID֪ͨIMSA*/
        case MAPS_PIH_PID:
            IMSA_PihMsgDistr(pRcvMsg);
            break;

        case UEPS_PID_CDS:
            IMSA_CdsMsgDistr(pRcvMsg);
            break;

        case WUEPS_PID_VC:
            IMSA_VcMsgDistr(pRcvMsg);
            break;
        case WUEPS_PID_AT:
            IMSA_AtMsgDistr(pRcvMsg);
            break;
        #if (FEATURE_PTM == FEATURE_ON)
        case MSP_PID_DIAG_APP_AGENT:
            IMSA_OmMsgDistr(pRcvMsg);
            break;
        #endif

        case UEPS_PID_MTC:
            IMSA_MtcMsgDistr(pRcvMsg);
            break;

        #if (FEATURE_ON == FEATURE_DSDS)
        case UEPS_PID_RRM:
            IMSA_RrmMsgDistr(pRcvMsg);
            break;
        #endif

        case PS_PID_MM:
            IMSA_LmmMsgDistr(pRcvMsg);
            break;

        /*���Ͷ�����󣬱���*/
        default:
            IMSA_WARN_LOG("IMSA_MsgHandle:NORM: Error SenderPid!");
            TLPS_PRINT2LAYER_WARNING(IMSA_MsgHandle_ENUM, LNAS_MSG_INVALID);
            break;

    }


    IMSA_WhenImsSrvConnStatusChangeNotifyMtc();
}

/*****************************************************************************
 Function Name  : IMSA_GetIntraMsgBuffAddr
 Discription    : ��ȡ�����ڲ���Ϣ�ĵ�ַ
                  �����ڲ���Ϣ�Ĺ���Ϊ:
                    1.�����ڲ���Ϣ��ַ,
                    2.����ڲ���Ϣ����
                    3.�����ڲ���Ϣ
 Input          : ��Ϣ����:�ó��Ⱥ���ͬDOPRA������Ϣʱ�ĳ��ȣ���������
                    ��Ϣͷ VOS_MSG_HEADER�ĳ���
 Output         : None
 Return         : ��Ϣ��ַ
 History:
       1.sunbing 49683      2013-06-25  Draft Enact
*****************************************************************************/
VOS_VOID *IMSA_GetIntraMsgBuffAddr( VOS_UINT32 ulBuffSize )
{
    IMSA_INTRA_MSG_QUEUE_STRU          *pstIntraMsgQueue;
    VOS_UINT8                          *pucMsgBuff;
    IMSA_INTRA_MSG                     *pstIntraMsgHead;

    if ( (0 == ulBuffSize) || (IMSA_INTRA_MSG_MAX_SIZE < ulBuffSize ) )
    {
        /* ��ӡ������Ϣ */
        /*lint -e961*/
        IMSA_ERR_LOG1("IMSA_GetIntraMsgBuffAddr: Size error, ", ulBuffSize);
        TLPS_PRINT2LAYER_ERROR1(IMSA_GetIntraMsgBuffAddr_ENUM, 1, ulBuffSize);
        /*lint +e961*/

        return VOS_NULL_PTR;
    }

    /* ��ȡ���е�ַ */
    pstIntraMsgQueue = IMSA_GetIntraMsgQueueAddress();

    /* ��֤�Ƿ������������һ����Ϣ��ʹ�ã��������ڴ������Ϣ�ռ䱻�����ȥ */
    if ( ((pstIntraMsgQueue->usTail + 1 + 1) % IMSA_INTRA_MSG_MAX_NUM) != pstIntraMsgQueue->usHeader )
    {
        pucMsgBuff = pstIntraMsgQueue->aucMsgBuff[pstIntraMsgQueue->usTail];

        /* ������Ļ�������0 */
        IMSA_MEM_SET_S(pucMsgBuff, IMSA_INTRA_MSG_MAX_SIZE, 0, IMSA_INTRA_MSG_MAX_SIZE);

        /*lint -e826*/
        pstIntraMsgHead                = (IMSA_INTRA_MSG *)pucMsgBuff;
        /*lint -e826*/

        /* ��д�ڲ���Ϣ��Ϣͷ�еĳ�������Ҫ��ȥOSA��Ϣͷ���� */
        pstIntraMsgHead->ulLength      = ulBuffSize - 20;

        return (VOS_VOID *)pucMsgBuff;
    }
    else
    {
        /* ��ӡ������Ϣ */
        IMSA_ERR_LOG("IMSA_GetIntraMsgBuffAddr: Queue full!");
        TLPS_PRINT2LAYER_ERROR(IMSA_GetIntraMsgBuffAddr_ENUM, 2);

        return VOS_NULL_PTR;
    }

}

/*****************************************************************************
 Function Name  : IMSA_SndIntraMsg()
 Description    : IMSA�ڲ���Ϣ���ͺ���
 Input          : VOS_VOID* pIntraMsg
 Output         : VOS_VOID
 Return Value   : VOS_UINT32

 History        :
      1.sunbing 49683      2013-06-25  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_SndIntraMsg(VOS_VOID* pIntraMsg)
{
    IMSA_INTRA_MSG_QUEUE_STRU     *pstIntraMsgQueue;
    IMSA_INTRA_MSG                *pstIntraMsgHead;

    if ( VOS_NULL_PTR == pIntraMsg )
    {
        /* ��ӡ������Ϣ */
        IMSA_ERR_LOG("IMSA_SndIntraMsg: IntraMsg Null Pointer.");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndIntraMsg_ENUM, LNAS_NULL_PTR);

        return  VOS_ERR;
    }

    pstIntraMsgQueue = IMSA_GetIntraMsgQueueAddress();

    /*�ж���Ϣ�����Ƿ�Ϸ�������Ҫ����OSA��Ϣͷ����*/
    pstIntraMsgHead = (IMSA_INTRA_MSG*)pIntraMsg;
    if((pstIntraMsgHead->ulLength + 20)> IMSA_INTRA_MSG_MAX_SIZE)
    {
        /* ��ӡ������Ϣ */
        IMSA_ERR_LOG("IMSA_SndIntraMsg: IntraMsg Length is too long.");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndIntraMsg_ENUM, 1);

        return  VOS_ERR;
    }

    /*�жϷ�����Ϣ������β*/
    if(pIntraMsg != pstIntraMsgQueue->aucMsgBuff[pstIntraMsgQueue->usTail])
    {
        /* ��ӡ������Ϣ */
        IMSA_ERR_LOG("IMSA_SndIntraMsg: IntraMsg Pointer is err.");
        TLPS_PRINT2LAYER_ERROR(IMSA_SndIntraMsg_ENUM, 2);

        /*�������߼������ش���*/
        return  VOS_ERR;
    }

    /* ѭ�����еĶ�βָ����һ��λ�� */
    pstIntraMsgQueue->usTail = (pstIntraMsgQueue->usTail + 1) % IMSA_INTRA_MSG_MAX_NUM;

    return VOS_OK;
}

/*****************************************************************************
 Function Name  : IMSA_GetNextIntraMsg()
 Description    : ��ȡһ���ڲ���Ϣ
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT8 *

 History        :
      1.sunbing 49683      2013-06-20  Draft Enact
*****************************************************************************/
VOS_UINT8 * IMSA_GetNextIntraMsg(VOS_VOID)
{
    IMSA_INTRA_MSG_QUEUE_STRU           *pstIntraMsgQueue;
    VOS_UINT8                           *pucMsg;

    /* ��ȡ���е�ַ */
    pstIntraMsgQueue = IMSA_GetIntraMsgQueueAddress();

    /*δ����β*/
    if(pstIntraMsgQueue->usHeader != pstIntraMsgQueue->usTail)
    {
        pucMsg = pstIntraMsgQueue->aucMsgBuff[pstIntraMsgQueue->usHeader];

        /* ȡ��һ����Ϣ,ͷָ�����ƫ�� */
        pstIntraMsgQueue->usHeader = (pstIntraMsgQueue->usHeader+1)%IMSA_INTRA_MSG_MAX_NUM;

        return pucMsg;
    }

    return VOS_NULL_PTR;
}

/*****************************************************************************
 Function Name  : IMSA_IntraMsgProcess()
 Description    : IMSA�ڲ���Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-20  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_IntraMsgProcess(VOS_VOID)
{
    VOS_UINT8                          *pucMsg              = VOS_NULL_PTR;

    /* ��ȡһ���ڲ���Ϣ */
    pucMsg = IMSA_GetNextIntraMsg();

    while(VOS_NULL_PTR != pucMsg)
    {
        /*��ȡ��Ϣ������HSO��ʾ*/
        (VOS_VOID)LTE_MsgHook(pucMsg);

        /*��Ϣ����*/
        IMSA_MsgHandle((VOS_VOID *)pucMsg);

        /* Ѱ����һ���ڲ���Ϣ */
        pucMsg = IMSA_GetNextIntraMsg();
    }

}

/*****************************************************************************
 Function Name  : IMSA_TafMsgDistr()
 Description    : TAF��Ϣ�ַ�����
                  TAF���PID���ж��ģ��MMA/SPM/APS����Ҫʵ�ֶ����ģ����Ϣ�ķַ�
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-20  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_TafMsgDistr(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    /*APS��Ϣ*/
    if(MN_CALLBACK_PS_CALL == pHeader->ulMsgName)
    {
        IMSA_CONN_ProcTafPsEvt((TAF_PS_EVT_STRU*)pRcvMsg);

        return ;
    }

    /*CALL ��Ϣ*/
    if((pHeader->ulMsgName >= ID_CALL_IMSA_SRVCC_STATUS_NOTIFY)
        &&(pHeader->ulMsgName <= ID_CALL_IMSA_MSG_TYPE_BUTT))
    {
        IMSA_ProcTafCallMsg(pRcvMsg);
        return;
    }


    /*SPM��Ϣ*/
    if ((pHeader->ulMsgName >= ID_SPM_IMSA_CALL_ORIG_REQ)
        && (pHeader->ulMsgName <= ID_SPM_IMSA_MSG_TYPE_BUTT))
    {
        IMSA_ProcSpmCallMsg(pRcvMsg);
        return;
    }

    /*������Ϣ*/
    if((pHeader->ulMsgName >= ID_MSG_IMSA_RP_DATA_REQ )
       &&(pHeader->ulMsgName <= ID_IMSA_MSG_TYPE_BUTT))
    {
        IMSA_ProcSmsMsg(pRcvMsg);
        return;
    }

}

/*****************************************************************************
 Function Name  : IMSA_IntraMsgDistr()
 Description    : IMSA�Լ������Լ�����Ϣ�ַ�����
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-20  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_IntraMsgDistr(const VOS_VOID *pRcvMsg )
{
    PS_MSG_HEADER_STRU                 *pstImsaIntraMsg = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    IMSA_INFO_LOG("IMSA_IntraMsgDistr is entered.");
    TLPS_PRINT2LAYER_INFO(IMSA_IntraMsgDistr_ENUM, LNAS_ENTRY);

    pstImsaIntraMsg = (PS_MSG_HEADER_STRU*)pRcvMsg;

    switch (pstImsaIntraMsg->ulMsgName)
    {
        /* CONNģ�鴦��������Ϣ */
        case ID_IMSA_CONN_SETUP_REQ:
            IMSA_CONN_ProcConnSetupReq((IMSA_CONN_SETUP_REQ_STRU*)pRcvMsg);
            break;
        case ID_IMSA_CONN_REL_REQ:
            IMSA_CONN_ProcConnRelReq((IMSA_CONN_REL_REQ_STRU*)pRcvMsg);
            break;
        case ID_IMSA_CONN_HO_REQ:
            IMSA_CONN_ProcConnHoReq((IMSA_CONN_HO_REQ_STRU *)pRcvMsg);
            break;

        /* SERVICESģ�鴦��������Ϣ */
        case ID_IMSA_REG_REG_IND:
            IMSA_SRV_ProcRegRegIndMsg((IMSA_REG_REG_IND_STRU *)pRcvMsg);
            break;
        case ID_IMSA_REG_DEREG_IND:
            IMSA_SRV_ProcRegDeregIndMsg((IMSA_REG_DEREG_IND_STRU *)pRcvMsg);
            break;
        case ID_IMSA_CONN_SETUP_IND:
            IMSA_SRV_ProcConnSetupIndMsg((IMSA_CONN_SETUP_IND_STRU *)pRcvMsg);
            break;
        case ID_IMSA_CONN_HO_CNF:
            IMSA_SRV_ProcConnHoCnfMsg((IMSA_CONN_HO_CNF_STRU *)pRcvMsg);
            break;

        case ID_IMSA_CONN_REL_IND:
            IMSA_SRV_ProcConnRelIndMsg((IMSA_CONN_REL_IND_STRU *)pRcvMsg);
            break;
        case ID_IMSA_CONN_MODIFY_IND:
            IMSA_SRV_ProcConnModifyIndMsg((IMSA_CONN_MODIFY_IND_STRU *)pRcvMsg);
            break;
        case ID_IMSA_CALL_RESULT_ACTION_IND:
            IMSA_SRV_ProcCallRsltActionIndMsg((IMSA_CALL_RESULT_ACTION_IND_STRU *)pRcvMsg);
            break;
        case ID_IMSA_CALL_SRVCC_SUCC_IND:
            IMSA_SRV_ProcCallSrvccSuccIndMsg((IMSA_CALL_SRVCC_SUCC_IND_STRU *)pRcvMsg);
            break;

        /* ע��ģ�鴦��������Ϣ */
        case ID_IMSA_REG_REG_REQ:
            (VOS_VOID)IMSA_RegProcUserRegReqMsg(pRcvMsg);
            break;
        case ID_IMSA_REG_DEREG_REQ:
            (VOS_VOID)IMSA_RegProcUserDeregReqMsg(pRcvMsg);
            break;
        case ID_IMSA_REG_REREG_REQ:
            (VOS_VOID)IMSA_RegProcUserReregReqMsg(pRcvMsg);
            break;

        /* ����ģ�鴦��������Ϣ */
        case ID_IMSA_NRM_CALL_SRV_STATUS_IND:
            (VOS_VOID)IMSA_CallProcIntraMsgNrmCallSrvStatus(pRcvMsg);
            break;
        case ID_IMSA_EMC_CALL_SRV_STATUS_IND:
            (VOS_VOID)IMSA_CallProcIntraMsgEmcCallSrvStatus(pRcvMsg);
            break;

        /* Domainģ�鴦��������Ϣ */
        case ID_IMSA_DOMAIN_SRV_STATUS_IND:
            IMSA_Domain_ProcIntraMsgSrvStatusInd(pRcvMsg);
            break;
        case ID_IMSA_DOMAIN_SRV_READY_IND:
            IMSA_Domain_ProcIntraMsgSrvReadyInd(pRcvMsg);
            break;

        case ID_IMSA_DOMAIN_SWITCH_RESULT_IND:
            IMSA_Domain_ProcIntraMsgSwitchResultInd(pRcvMsg);
            break;

        case ID_IMSA_DOMAIN_BEARER_REL_IND:
            IMSA_Domain_ProcIntraMsgBearerRelInd(pRcvMsg);
            break;

        default:
            IMSA_WARN_LOG("IMSA_IntraMsgDistr:Illegal Msg id!");
            TLPS_PRINT2LAYER_WARNING(IMSA_IntraMsgDistr_ENUM, LNAS_MSG_INVALID);
            break;
    }
}



/*****************************************************************************
 Function Name  : IMSA_UsimMsgDistr()
 Description    : USIM��Ϣ�ַ�����
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-20  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_UsimMsgDistr(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        /*���ļ����*/
        case USIMM_READFILE_CNF:
            (VOS_VOID)IMSA_ProcReadIsimFileResult((VOS_VOID*)pRcvMsg);
            break;

        /*��Ȩ���*/
        case USIMM_AUTHENTICATION_CNF:
            IMSA_ProcIsimAuthRsp(pRcvMsg);
            break;

        default:
            IMSA_ERR_LOG("IMSA_UsimMsgDistr: Msg id is err.");
            TLPS_PRINT2LAYER_ERROR(IMSA_UsimMsgDistr_ENUM, LNAS_MSG_INVALID);
            break;

    }
}

/*****************************************************************************
 Function Name  : IMSA_PihMsgDistr()
 Description    : ��USIMע�����Ӧ�����ϱ���Ϣ�ķַ�����
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-08-28  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_PihMsgDistr(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        /*��״̬�仯ָʾ*/
        case USIMM_CARDSTATUS_IND:
            IMSA_ProcIsimStatusInd(pRcvMsg);
            break;

        /*�ļ�ˢ��*/
        case USIMM_STKREFRESH_IND:
            IMSA_ProcIsimRefreshInd(pRcvMsg);
            break;

        default:
            IMSA_ERR_LOG("IMSA_PihMsgDistr: Msg id is err.");
            TLPS_PRINT2LAYER_ERROR(IMSA_PihMsgDistr_ENUM, LNAS_MSG_INVALID);
            break;

    }
}

/*****************************************************************************
 Function Name  : IMSA_VcMsgDistr()
 Description    : HIFI��Ϣ������
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-22  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_VcMsgDistr(const VOS_VOID *pRcvMsg)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        /*HIFI�쳣*/
        case ID_VC_IMSA_HIFI_EXCEPTION_NTF:
            IMSA_ProcHifiExceptionNtf((VOS_VOID *)pRcvMsg);
            break;

        default:
            break;
    }
}


VOS_VOID IMSA_AtMsgDistr(const VOS_VOID *pRcvMsg)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        /**/
        case ID_AT_IMSA_CIREG_SET_REQ:
            IMSA_ProcMsgCiregSetReq((VOS_VOID *)pRcvMsg);
            break;
        case ID_AT_IMSA_CIREG_QRY_REQ:
            IMSA_ProcMsgCiregQryReq((VOS_VOID *)pRcvMsg);
            break;
        case ID_AT_IMSA_CIREP_SET_REQ:
            IMSA_ProcMsgCirepSetReq((VOS_VOID *)pRcvMsg);
            break;
        case ID_AT_IMSA_CIREP_QRY_REQ:
            IMSA_ProcMsgCirepQryReq((VOS_VOID *)pRcvMsg);
            break;
        case ID_AT_IMSA_VOLTEIMPU_QRY_REQ:
            IMSA_ProcMsgVolteImpuQryReq((VOS_VOID *)pRcvMsg);
            break;


        case ID_AT_IMSA_IMS_CTRL_MSG:
            IMSA_ProcMsgAtCtrlInd((VOS_VOID *)pRcvMsg);
            break;

        case ID_AT_IMSA_IMS_REG_DOMAIN_QRY_REQ:
            IMSA_ProcMsgAtDomainQryReq((VOS_VOID *)pRcvMsg);
            break;
        case ID_AT_IMSA_CALL_ENCRYPT_SET_REQ:
            IMSA_ProcMsgEncryptSetReq((VOS_VOID *)pRcvMsg);
            break;
        case ID_AT_IMSA_ROAMING_IMS_QRY_REQ:
            IMSA_ProcMsgRoamingImsQryReq((VOS_VOID *)pRcvMsg);
        default:
            break;
    }
}

#if (FEATURE_ON == FEATURE_PTM)

VOS_VOID IMSA_OmMsgDistr(const VOS_VOID *pRcvMsg)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        /**/
        case ID_OM_ERR_LOG_CTRL_IND:
            IMSA_RcvAcpuOmErrLogCtrlInd((VOS_VOID *)pRcvMsg);
            break;
        case ID_OM_ERR_LOG_REPORT_REQ:
            IMSA_RcvAcpuOmErrLogRptReq((VOS_VOID *)pRcvMsg);
            break;

        default:
            break;
    }
}
#endif

VOS_VOID IMSA_MtcMsgDistr(const VOS_VOID *pRcvMsg)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        case ID_MTC_IMSA_MODEM1_INFO_IND:
            IMSA_ProcMtcMsgModem1InfoInd(pRcvMsg);
            break;
        default:
            break;
    }
}



/*****************************************************************************
 Function Name  : IMSA_PidInit
 Discription    : IMSA��PID��ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1. sunbing 49683      2013-04-23  Draft Enact

*****************************************************************************/
VOS_UINT32 IMSA_PidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    switch( ip )
    {
        case    VOS_IP_LOAD_CONFIG:
                {
                    IMSA_Init();

                    /*��ģ��ע��״̬����PID*/
                    (VOS_VOID)PIH_RegUsimCardStatusIndMsg(PS_PID_IMSA);
                }
                break;

        case    VOS_IP_FARMALLOC:
        case    VOS_IP_INITIAL:
        case    VOS_IP_ENROLLMENT:
        case    VOS_IP_LOAD_DATA:
        case    VOS_IP_FETCH_DATA:
        case    VOS_IP_STARTUP:
        case    VOS_IP_RIVAL:
        case    VOS_IP_KICKOFF:
        case    VOS_IP_STANDBY:
        case    VOS_IP_BROADCAST_STATE:
        case    VOS_IP_RESTART:
        case    VOS_IP_BUTT:
                break;

        default:
                break;
    }

    return VOS_OK;
}

/*****************************************************************************
 Function Name  : IMSVA_PidInit
 Discription    : IMSVA��PID��ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1. sunbing 49683      2013-07-15  Draft Enact

*****************************************************************************/
VOS_UINT32 IMSVA_PidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    (VOS_VOID)ip;
    return VOS_OK;
}

/*****************************************************************************
 Function Name  : IMSA_TaskEntry()
 Description    : IMSAģ��TASK��ڴ���
 Input          : struct MsgCB* pMsg-----------��Ϣָ��
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1. sunbing 49683      2013-04-23  Draft Enact

 *****************************************************************************/
VOS_VOID IMSA_TaskEntry(MsgBlock * pMsg)/*lint -e818 -e830*/
{
    /* �ж���ڲ����Ƿ�Ϸ�*/
    if( VOS_NULL_PTR == pMsg)
    {
        return;
    }

    /* ��Ϣ���� */
    IMSA_MsgHandle((const VOS_VOID *)pMsg);

    /* �ڲ���Ϣ���� */
    IMSA_IntraMsgProcess();
    /* IMSA��Ϣ���������ʹ�ӡ */
    TLPS_SendPrintBufferInfo();

}

/*****************************************************************************
 Function Name  : IMSA_TaskEntry()
 Description    : IMSVAģ��TASK��ڴ���
 Input          : struct MsgCB* pMsg-----------��Ϣָ��
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1. sunbing 49683      2013-07-12  Draft Enact

 *****************************************************************************/
VOS_VOID IMSVA_TaskEntry(MsgBlock * pMsg)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* �ж���ڲ����Ƿ�Ϸ�*/
    if( VOS_NULL_PTR == pMsg)
    {
        return;
    }

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pMsg;

    switch(pHeader->ulSenderPid)
    {
        case DSP_PID_VOICE:
            IMSVA_ProcHifiMsg(pMsg);
            break;

        case DSP_PID_VOICE_RT:
            IMSVA_ProcHifiRtMsg(pMsg);
            break;

        case VOS_PID_TIMER:
            IMSVA_TimerMsgDistr((REL_TIMER_MSG *)pMsg);
            break;

        default:
            IMSA_WARN_LOG("IMSVA_TaskEntry:NORM: Error SenderPid!");
            TLPS_PRINT2LAYER_WARNING(IMSVA_TaskEntry_ENUM, LNAS_MSG_INVALID);
            break;
    }

}


VOS_VOID IMSA_LmmMsgDistr(const VOS_VOID * pMsg)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* �ж���ڲ����Ƿ�Ϸ�*/
    if( VOS_NULL_PTR == pMsg)
    {
        return;
    }

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pMsg;

    switch(pHeader->ulMsgName)
    {
        case ID_LMM_IMSA_ATTACH_BEGIN_IND:
            IMSA_ProcLmmAttachBeginInd(pMsg);
            break;

        case ID_LMM_IMSA_EMC_EST_FAIL_IND:
            IMSA_ProcLmmEmcEstFailInd(pMsg);
            break;
        default:
            break;
    }
}

#endif

/*****************************************************************************
 Function Name  : IMSA_FidInit
 Discription    : IMSA��FID��ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1. sunbing 49683      2013-07-15  Draft Enact

*****************************************************************************/
VOS_UINT32 IMSA_FidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    VOS_UINT32                              ulRet = 0;

    (VOS_VOID)ulRet;

    switch( ip )
    {
        case    VOS_IP_LOAD_CONFIG:

                #if (FEATURE_ON == FEATURE_IMS)
                ulRet = VOS_RegisterPIDInfo(PS_PID_IMSA,
                                            IMSA_PidInit,
                                            IMSA_TaskEntry);
                if( VOS_OK != ulRet )
                {
                    return VOS_ERR;
                }


                ulRet = VOS_RegisterMsgTaskPrio(PS_FID_IMSA, VOS_PRIORITY_M5);
                if( PS_SUCC != ulRet )
                {
                    return ulRet;
                }
                #endif

                break;

        case    VOS_IP_FARMALLOC:
        case    VOS_IP_INITIAL:
        case    VOS_IP_ENROLLMENT:
        case    VOS_IP_LOAD_DATA:
        case    VOS_IP_FETCH_DATA:
        case    VOS_IP_STARTUP:
        case    VOS_IP_RIVAL:
        case    VOS_IP_KICKOFF:
        case    VOS_IP_STANDBY:
        case    VOS_IP_BROADCAST_STATE:
        case    VOS_IP_RESTART:
        case    VOS_IP_BUTT:
                break;
    }

    return VOS_OK;
}

/*****************************************************************************
 Function Name  : IMSVA_FidInit
 Discription    : IMSVA��FID��ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1. sunbing 49683      2013-07-15  Draft Enact

*****************************************************************************/
VOS_UINT32 IMSVA_FidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    VOS_UINT32                              ulRet = 0;

    (VOS_VOID)ulRet;

    switch( ip )
    {
        case    VOS_IP_LOAD_CONFIG:

                #if (FEATURE_ON == FEATURE_IMS)
                ulRet = VOS_RegisterPIDInfo(PS_PID_IMSVA,
                                            IMSVA_PidInit,
                                            IMSVA_TaskEntry);
                if( VOS_OK != ulRet )
                {
                    return VOS_ERR;
                }

                /*���ȼ���Ҫ��MAC����(123)�ͣ���MAC����(VOS_PRIORITY_P4 126)��*/
                #if(VOS_OS_VER != VOS_WIN32)/*������ֱ��ָ����Ӧ�����ȼ���PC��dorpraû�ж�ӦAPI*/
                #if (VOS_RTOSCK == VOS_OS_VER)
                ulRet = VOS_RegisterTaskPrio(PS_FID_IMSVA, 31);
                #else
                ulRet = VOS_RegisterTaskPrio(PS_FID_IMSVA, 125);
                #endif
                #else
                ulRet = VOS_RegisterMsgTaskPrio(PS_FID_IMSVA, VOS_PRIORITY_P4);
                #endif
                if( PS_SUCC != ulRet )
                {
                    return ulRet;
                }
                #endif

                break;

        case    VOS_IP_FARMALLOC:
        case    VOS_IP_INITIAL:
        case    VOS_IP_ENROLLMENT:
        case    VOS_IP_LOAD_DATA:
        case    VOS_IP_FETCH_DATA:
        case    VOS_IP_STARTUP:
        case    VOS_IP_RIVAL:
        case    VOS_IP_KICKOFF:
        case    VOS_IP_STANDBY:
        case    VOS_IP_BROADCAST_STATE:
        case    VOS_IP_RESTART:
        case    VOS_IP_BUTT:
                break;
    }

    return VOS_OK;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaMain.c */



