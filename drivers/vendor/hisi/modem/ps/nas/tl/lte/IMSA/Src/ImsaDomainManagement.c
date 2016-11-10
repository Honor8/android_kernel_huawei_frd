

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaDomainManagement.h"
#include "ImsaPublic.h"
#include "ImsaIntraInterface.h"
#include "ImsaServiceManagement.h"
#include "ImsaMsccInterface.h"
#include "ImsaIntraInterface.h"
#include "AtImsaInterface.h"
#include "ImsaWifiInterface.h"
#include "ImsaProcAtMsg.h"
#include "ImsaProcWifiMsg.h"
#include "ImsaProcMsccMsg.h"
#include "ImsaEntity.h"
#include "ImsaProcSpmMsg.h"
#include "ImsaImsAdaption.h"
#include "ImsaImsApi.h"
#include "ImsaProcMsccMsg.h"
#include "SpmImsaInterface.h"
#include "ImsaCallManagement.h"


/*lint -e767*/
#define    THIS_FILE_ID      PS_FILE_ID_IMSADOMAINMANAGEMENT_C
#define    THIS_NAS_FILE_ID  NAS_FILE_ID_IMSADOMAINMANAGEMENT_C
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

extern VOS_UINT32 IMSA_IsImsVoiceContidionSatisfied( VOS_VOID );
/*extern VOS_VOID IMSA_SndMsccMsgImsVoiceCapNotify
(
     MSCC_IMSA_IMS_VOICE_CAP_ENUM_UINT8   enImsVoiceCap,
     VOS_UINT32                           ucIsExistPersistentBearer
);
extern VOS_VOID IMSA_SndMsccMsgStartCnf(VOS_VOID  );*/
extern void IMSA_Domain_SaveDelaySwitchEvent( VOS_UINT32 ulPid, VOS_UINT32 ulMsgId);
extern void IMSA_Domain_StartTimer1LteDomain(VOS_VOID);
extern IMSA_IMS_DOMAIN_ENUM_UINT8 IMSA_JudgeImsDomainC21(VOS_VOID);

extern VOS_UINT32 IMSA_Domain_ProcReadIsimFileResult(VOS_VOID *pRcvMsg);

/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/

/*****************************************************************************
 Function Name  : IMSA_Domain_Init()
 Description    : Domain Manager��ʼ��
 Input          :  VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/


VOS_VOID IMSA_Domain_Init(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    IMSA_MEM_SET_S(pstDomainManager, sizeof(IMSA_DOMAIN_MANAGER_STRU), 0x0, sizeof(IMSA_DOMAIN_MANAGER_STRU));
    pstDomainManager->enImsDomainPrefer            = IMSA_IMS_DOMAIN_PREFER_CELLULAR_ONLY;
    pstDomainManager->enImsaAvailDomain            = IMSA_IMS_DOMAIN_NULL;

    pstDomainManager->enImsaTryedDomain            = IMSA_IMS_DOMAIN_NULL;
    pstDomainManager->ucLteQualityCfgFlag          = VOS_FALSE;
    pstDomainManager->enLteQuality                 = WIFI_IMSA_QUALITY_LEVEL_BUTT;
    pstDomainManager->enWifiQuality                = WIFI_IMSA_QUALITY_LEVEL_BUTT;
    pstDomainManager->enDomainFsmL2State           = IMSA_DOMAIN_FSM_L2_STATE_NULL;
    pstDomainManager->enImsaVolteCtrlVowifiStatus  = IMSA_IMS_VOLTECTRLVOWIFI_INIT;

    /* ��ʼ��������ѡ�����漰���� */
    pstDomainManager->ulCurrentSenderPid         =  0;
    pstDomainManager->ulCurrentMsgId             =  0;
    pstDomainManager->ucMsgDelayProcessFlag      =  VOS_FALSE;
    pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  VOS_FALSE;
    pstDomainManager->ucOrgPdnRlsFlag            =  VOS_TRUE;
    pstDomainManager->ucDomainSelectFlag         =  VOS_FALSE;

    /* ��ʼ���ӳ��¼�������� */
    IMSA_Domain_ClearDelaySwitchEvent();

#if 0
    /* ��ʼ����ѡ�ͷ���ʱ����ʱ����*/
    pstDomainManager->ucInitTimer1Len              = 30;
    pstDomainManager->ucMaxTimer1Len               = 60;
#endif

    /* LTE��ͷ���ʱ����ʼ�� */
    pstDomainManager->stTimer1LteDomain.phTimer    = VOS_NULL_PTR;
    pstDomainManager->stTimer1LteDomain.ucMode     = VOS_RELTIMER_NOLOOP;
    pstDomainManager->stTimer1LteDomain.usName     = TI_IMSA_DOMAIN_TIMER1_LTE;
    pstDomainManager->stTimer1LteDomain.ulTimerLen = 0;
    pstDomainManager->ucPunishTimeInLteDomain      = 1;

   /* WIFI��ͷ���ʱ����ʼ�� */
    pstDomainManager->stTimer1WifiDomain.phTimer    = VOS_NULL_PTR;
    pstDomainManager->stTimer1WifiDomain.ucMode     = VOS_RELTIMER_NOLOOP;
    pstDomainManager->stTimer1WifiDomain.usName     = TI_IMSA_DOMAIN_TIMER1_WIFI;
    pstDomainManager->stTimer1WifiDomain.ulTimerLen = 0;
    pstDomainManager->ucPunishTimeInWifiDomain      = 1;

    return ;
}


/*****************************************************************************
 Function Name  : IMSA_Domain_ClearResource()
 Description    : Domain Manager��ʼ��
 Input          :  VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/


VOS_VOID IMSA_Domain_ClearResource(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    /* ��ʼ����ѡ״̬��ر��� */
    pstDomainManager->enImsaAvailDomain            = IMSA_IMS_DOMAIN_NULL;
    pstDomainManager->enImsaTryedDomain            = IMSA_IMS_DOMAIN_NULL;

    pstDomainManager->ucLteQualityCfgFlag          = VOS_FALSE;
    pstDomainManager->enLteQuality                 = WIFI_IMSA_QUALITY_LEVEL_BUTT;
    pstDomainManager->enWifiQuality                = WIFI_IMSA_QUALITY_LEVEL_BUTT;
    pstDomainManager->enDomainFsmL2State           = IMSA_DOMAIN_FSM_L2_STATE_NULL;
    pstDomainManager->enImsaVolteCtrlVowifiStatus  = IMSA_IMS_VOLTECTRLVOWIFI_INIT;

    /* ��ʼ��������ѡ�����漰���� */
    pstDomainManager->ulCurrentSenderPid         =  0;
    pstDomainManager->ulCurrentMsgId             =  0;
    pstDomainManager->ucMsgDelayProcessFlag      =  VOS_FALSE;
    pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  VOS_FALSE;
    pstDomainManager->ucOrgPdnRlsFlag            =  VOS_TRUE;
    pstDomainManager->ucDomainSelectFlag         =  VOS_FALSE;

     /* LTE��ͷ���ʱ����ʼ�� */
     IMSA_StopTimer(&pstDomainManager->stTimer1LteDomain);
     pstDomainManager->stTimer1LteDomain.phTimer    = VOS_NULL_PTR;
     pstDomainManager->stTimer1LteDomain.ucMode     = VOS_RELTIMER_NOLOOP;
     pstDomainManager->stTimer1LteDomain.usName     = TI_IMSA_DOMAIN_TIMER1_LTE;
     pstDomainManager->stTimer1LteDomain.ulTimerLen = 0;
     pstDomainManager->ucPunishTimeInLteDomain      = 1;

    /* WIFI��ͷ���ʱ����ʼ�� */
     IMSA_StopTimer(&pstDomainManager->stTimer1WifiDomain);
     pstDomainManager->stTimer1WifiDomain.phTimer    = VOS_NULL_PTR;
     pstDomainManager->stTimer1WifiDomain.ucMode     = VOS_RELTIMER_NOLOOP;
     pstDomainManager->stTimer1WifiDomain.usName     = TI_IMSA_DOMAIN_TIMER1_WIFI;
     pstDomainManager->stTimer1WifiDomain.ulTimerLen = 0;
     pstDomainManager->ucPunishTimeInWifiDomain      = 1;


    /* ��ʼ���ӳ��¼�������� */
    IMSA_Domain_ClearDelaySwitchEvent();

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_ClearDomainVar()
 Description    : Domain Manager��ѡ������ʼ��
 Input          :  VOS_VOID
                 ucHoFailReturn2OrgPdnFlag: �л�ʧ���Ժ��Ƿ���Իص�ԭ��PDN: FALSE ���ܻص�ԭ��; TRUE ���Իص�ԭ��
                 ucOrgPdnRlsFlag: ���л��ɹ��Ժ��Ƿ���Ҫ�ͷ�ԭ���PDN: FALSE ����Ҫ�ͷţ� TRUE �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

void IMSA_Domain_InitDomainOpVar(
     VOS_UINT32 ulCurrentSenderPid,
     VOS_UINT32 ulCurrentMsgId,
     VOS_UINT8 ucHoFailReturn2OrgPdnFlag,
     VOS_UINT8 ucOrgPdnRlsFlag
)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    /* ��ʼ����ѡ��ر���,����ǰ��ѡ���� */
    pstDomainManager->ulCurrentSenderPid         =  ulCurrentSenderPid;
    pstDomainManager->ulCurrentMsgId             =  ulCurrentMsgId;
    pstDomainManager->ucMsgDelayProcessFlag      =  VOS_FALSE;
    pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  ucHoFailReturn2OrgPdnFlag;
    pstDomainManager->ucOrgPdnRlsFlag            =  ucOrgPdnRlsFlag;
    pstDomainManager->ucDomainSelectFlag         =  VOS_FALSE;

    IMSA_Domain_ClearDelaySwitchEvent();

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_InitDomainOpVarByDelayEvent()
 Description    : �����ӳ��¼�������ѡ��������
 Input          :  VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

void IMSA_Domain_InitDomainOpVarByDelayEvent()
{

    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    /* ���ݼ�¼�������ӳ��¼�������ѡ���� */
    if(IMSA_OP_TRUE == pstDomainManager->bitDomainCfgEvent)
    {
        pstDomainManager->ulCurrentSenderPid         =  UEPS_PID_MSCC;
        pstDomainManager->ulCurrentMsgId             =  ID_MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ;
        pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  VOS_TRUE;
        pstDomainManager->ucOrgPdnRlsFlag            =  VOS_TRUE;
    }
    else if(IMSA_OP_TRUE == pstDomainManager->bitLteQualityEvent)
    {
        pstDomainManager->ulCurrentSenderPid         =  PS_PID_ERRC ;
        pstDomainManager->ulCurrentMsgId             =  ID_LRRC_IMSA_STATE_IND;
        pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  VOS_TRUE;
        pstDomainManager->ucOrgPdnRlsFlag            =  VOS_TRUE;
    }
    else if(IMSA_OP_TRUE == pstDomainManager->bitServiceChangeIndEvent)
    {
        pstDomainManager->ulCurrentSenderPid         =  UEPS_PID_MSCC;
        pstDomainManager->ulCurrentMsgId             =  ID_MSCC_IMSA_SERVICE_CHANGE_IND;
        pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  VOS_TRUE;
        pstDomainManager->ucOrgPdnRlsFlag            =  VOS_TRUE;
    }
    else if(IMSA_OP_TRUE == pstDomainManager->bitWifiQualityEvent)
    {
        pstDomainManager->ulCurrentSenderPid         =  WUEPS_PID_MAPCON;
        pstDomainManager->ulCurrentMsgId             =  ID_WIFI_IMSA_STATE_IND  ;
        pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  VOS_TRUE;
        pstDomainManager->ucOrgPdnRlsFlag            =  VOS_TRUE;
    }
    else if(IMSA_OP_TRUE == pstDomainManager->bitPeriodTimerEvent)
    {
        pstDomainManager->ulCurrentSenderPid         =  VOS_PID_TIMER ;
        pstDomainManager->ulCurrentMsgId             =  000  ; /* �����޸� */
        pstDomainManager->ucHoFailReturn2OrgPdnFlag  =  VOS_TRUE;
        pstDomainManager->ucOrgPdnRlsFlag            =  VOS_TRUE;
    }
    else
    {
    }

    /* �����¼�������ӳ��¼� */
    IMSA_Domain_ClearDelaySwitchEvent();

    /* ������������ */
    pstDomainManager->ucMsgDelayProcessFlag      =  VOS_FALSE;
    pstDomainManager->ucDomainSelectFlag         =  VOS_FALSE;

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_ClearDelaySwitchEvent()
 Description    : ���������ӳ��¼�����
 Input          :  VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

void IMSA_Domain_ClearDelaySwitchEvent()
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    /* �����¼�������ӳ��¼� */
    pstDomainManager->bitDomainCfgEvent          =  IMSA_OP_FALSE;
    pstDomainManager->bitLteQualityEvent         =  IMSA_OP_FALSE;
    pstDomainManager->bitPeriodTimerEvent        =  IMSA_OP_FALSE;
    pstDomainManager->bitServiceChangeIndEvent   =  IMSA_OP_FALSE;
    pstDomainManager->bitWifiQualityEvent        =  IMSA_OP_FALSE;
    pstDomainManager->bitRsvTrigDelayEvent       =  0;

    return;
}


/*****************************************************************************
 Function Name  : IMSA_Domain_CheckSignalPrefer()
 Description    : ����IMS���ȹ�ϵ���ź�ǿ���о�ѡ����
 Input            : ��ǰ��ѡ����
 Output         : VOS_VOID
 Return Value : �����о����õ���
 History        :
      1.Zhangdongfeng 353461        2015-12-10  Draft Enact
*****************************************************************************/
void  IMSA_Domain_CheckSignalPrefer(IMSA_IMS_DOMAIN_ENUM_UINT8 *penAvailDomain)
{

    IMSA_IMS_DOMAIN_ENUM_UINT8     enImsDomainResult = *penAvailDomain;
    IMSA_DOMAIN_MANAGER_STRU      *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    IMSA_INFO_LOG("IMSA_Domain_CheckSignalPrefer is entered!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_CheckSignalPrefer_ENUM, LNAS_ENTRY);

    /* �����ź������������ȹ�ϵ����ѡ���� */
    switch( pstDomainManager->enImsDomainPrefer )
    {
        case IMSA_IMS_DOMAIN_PREFER_CELLULAR_ONLY:
            /* ���Cellular Only, LTE�ź�ΪNORMAL/GOOD��ѡ��LTE�� */
            if( (pstDomainManager->enLteQuality > IMSA_WIRELESS_QUALITY_LEVEL_BAD)
               &&(  ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                  ||( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
            }
            else
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_NULL;
            }
            break;

        case IMSA_IMS_DOMAIN_PREFER_WIFI_ONLY:
            /* ���WIFI only��WIFI�ź�ΪNORMAL/GOOD��ѡ��WIFI�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enWifiQuality )
                &&( ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                   ||( IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
            }
            else
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_NULL;
            }
            break;

        case IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER:
            /* ���Cellular Prefer, LTE�ź�ΪNORMAL/GOOD��ѡ��LTE�� */
            if( (pstDomainManager->enLteQuality > IMSA_WIRELESS_QUALITY_LEVEL_BAD)
               &&(  ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                  ||( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
            }

            /* ���Cellular Prefer��LTE�ź�ΪBAD��WIFI�ź�ΪGOOD/NORMAL��ѡ��WIFI�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enLteQuality )
                &&(IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enWifiQuality )
                &&( ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                    ||( IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
            }

            /* WIFI�ź�ΪBAD��LTE�ź�ΪBAD��ά�ַ�ע��״̬���ݲ�ѡ�� */
            if( (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enLteQuality )
                && (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enWifiQuality) )
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_NULL;
            }
#if 0

            if( (pstDomainManager->enLteQuality > IMSA_WIRELESS_QUALITY_LEVEL_NORMAL)
                  &&(  ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                     ||( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
                break;
            }
            if((IMSA_WIRELESS_QUALITY_LEVEL_NORMAL == pstDomainManager->enLteQuality)
                &&(IMSA_WIRELESS_QUALITY_LEVEL_NORMAL < pstDomainManager->enWifiQuality)
                &&( ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                   ||( IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
                break;
            }
            if( (pstDomainManager->enLteQuality > IMSA_WIRELESS_QUALITY_LEVEL_BAD)
                  &&(  ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                     ||( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
                break;
            }


            /* ���Cellular Prefer��LTE�ź�ΪBAD��WIFI�ź�ΪGOOD/NORMAL��ѡ��WIFI�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enLteQuality )
                 &&(IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enWifiQuality )
                 &&( ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                    ||( IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
                break;
            }

            /* WIFI�ź�ΪBAD��LTE�ź�ΪBAD��ά�ַ�ע��״̬���ݲ�ѡ�� */
            if( (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enLteQuality )
            && (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enWifiQuality) )
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_NULL;
                break;
            }

#endif
            break;

        case IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER:

           /* ���WIFI Prefer��WIFI�ź�ΪBAD��LTE�ź�ΪGOOD/NORMAL��ѡ��LTE�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enWifiQuality )
                &&(IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enLteQuality )
                &&(  ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                   ||( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
            }

            /* ���WIFI Prefer��WIFI�ź�ΪNORMAL/GOOD��ѡ��WIFI�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enWifiQuality )
                &&( ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                   ||( IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
            }

            /* WIFI�ź�ΪBAD��LTE�ź�ΪBAD��ά�ַ�ע��״̬���ݲ�ѡ�� */
            if((IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enLteQuality )
                &&(IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enWifiQuality))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_NULL;
            }
#if 0
            /* ���WIFI Prefer��WIFI�ź�ΪBAD��LTE�ź�ΪGOOD/NORMAL��ѡ��LTE�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enWifiQuality )
                &&(IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enLteQuality )
                &&(  ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                   ||( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
                break;
            }


            /* ���WIFI Prefer��WIFI�ź�ΪNORMAL,LTEΪGOOD��ѡ��LTE�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_NORMAL == pstDomainManager->enWifiQuality )
                &&(IMSA_WIRELESS_QUALITY_LEVEL_NORMAL < pstDomainManager->enLteQuality)
                &&( ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                   ||( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
                break;
            }

            /* ���WIFI Prefer��WIFI�ź�ΪNORMAL/GOOD��ѡ��WIFI�� */
            if(   (IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enWifiQuality )
                &&( ( IMSA_IMS_DOMAIN_LTE_WIFI == enImsDomainResult)
                   ||( IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
                break;
            }

            /* WIFI�ź�ΪBAD��LTE�ź�ΪBAD��ά�ַ�ע��״̬���ݲ�ѡ�� */
            if((IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enLteQuality )
                &&(IMSA_WIRELESS_QUALITY_LEVEL_BAD == pstDomainManager->enWifiQuality))
            {
                enImsDomainResult = IMSA_IMS_DOMAIN_NULL;
            }
#endif
            break;

        default:
            enImsDomainResult = IMSA_IMS_DOMAIN_NULL;
            IMSA_WARN_LOG("IMSA_Domain_CheckSignalPrefer: Domain Prefer ENUM Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_CheckSignalPrefer_ENUM, LNAS_PARAM_INVALID);
    }

    *penAvailDomain = enImsDomainResult;
}




VOS_UINT32 IMSA_Domain_IsWifiDomainExist(void)
{
    IMSA_CONTROL_MANAGER_STRU    *pstControlManager;

    IMSA_INFO_LOG("IMSA_Domain_IsWifiDomainExist: ENTER!");

    pstControlManager = IMSA_GetControlManagerAddress();

    if( ((IMSA_STATUS_STARTED == pstControlManager->enImsaWifiStatus)
          ||(IMSA_STATUS_STARTING == pstControlManager->enImsaWifiStatus))
       &&((IMSA_STATUS_STARTED == pstControlManager->enImsaStatus)
          ||(IMSA_STATUS_STARTING == pstControlManager->enImsaStatus))
       && (VOS_TRUE == IMSA_GetWifiImsSupportFlag()))
    {

        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}




VOS_UINT32 IMSA_Domain_IsCellularDomainExist(void)
{
    IMSA_CONTROL_MANAGER_STRU    *pstControlManager;

    IMSA_INFO_LOG("IMSA_Domain_IsCellularDomainExist: ENTER!");

    pstControlManager = IMSA_GetControlManagerAddress();

    if( ((IMSA_STATUS_STARTED == pstControlManager->enImsaCellularStatus)
          ||(IMSA_STATUS_STARTING == pstControlManager->enImsaCellularStatus))
       &&((IMSA_STATUS_STARTED == pstControlManager->enImsaStatus)
          ||(IMSA_STATUS_STARTING == pstControlManager->enImsaStatus))
       && (VOS_TRUE == IMSA_GetCellularImsSupportFlag()))
    {

        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}







VOS_UINT32 IMSA_Domain_IsDualDomainExist(void)
{
    IMSA_CONTROL_MANAGER_STRU    *pstControlManager;

    IMSA_INFO_LOG("IMSA_Domain_IsDualDomainExist: ENTER!");

    pstControlManager = IMSA_GetControlManagerAddress();

    if( ((IMSA_STATUS_STARTED == pstControlManager->enImsaWifiStatus)
          ||(IMSA_STATUS_STARTING == pstControlManager->enImsaWifiStatus))
       &&((IMSA_STATUS_STARTED == pstControlManager->enImsaStatus)
          ||(IMSA_STATUS_STARTING == pstControlManager->enImsaStatus))
       && ((IMSA_STATUS_STARTED == pstControlManager->enImsaCellularStatus)
          ||(IMSA_STATUS_STARTING == pstControlManager->enImsaCellularStatus))
       && (VOS_TRUE == IMSA_GetCellularImsSupportFlag())
       && (VOS_TRUE == IMSA_GetWifiImsSupportFlag()))
    {

        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}


VOS_UINT32 IMSA_Domain_IsNrmSrvConditonSatisfied
(
    MSCC_IMSA_SERVICE_CHANGE_IND_STRU *pRcvMsg
)
{

    IMSA_CONFIG_PARA_STRU              *pstImsaConfigPara   = IMSA_GetConfigParaAddress();


    /* ��ǰ���뼼��֧��IMS���� */
    if (MSCC_IMSA_RAT_TYPE_GSM == pRcvMsg->enRat)
    {
        IMSA_INFO_LOG("IMSA_Domain_IsNrmSrvConditonSatisfied:GSM support ims!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_IsNrmSrvConditonSatisfied_ENUM, 1);
        return IMSA_FALSE;
    }

    /* ��ǰ���뼼��֧��IMS���� */
    if ((MSCC_IMSA_RAT_TYPE_UTRAN == pRcvMsg->enRat)
        && (VOS_TRUE != pstImsaConfigPara->ucUtranImsSupportFlag))
    {
        IMSA_INFO_LOG("IMSA_Domain_IsNrmSrvConditonSatisfied:UNTRAN support ims!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_IsNrmSrvConditonSatisfied_ENUM, 2);
        return IMSA_FALSE;
    }

    if ((MSCC_IMSA_RAT_TYPE_LTE == pRcvMsg->enRat)
        && (VOS_TRUE != pstImsaConfigPara->ucLteImsSupportFlag))
    {
        IMSA_INFO_LOG("IMSA_Domain_IsNrmSrvConditonSatisfied:LTE support ims!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_IsNrmSrvConditonSatisfied_ENUM, 3);
        return IMSA_FALSE;
    }

    if (IMSA_VOICE_DOMAIN_CS_ONLY == IMSA_GetVoiceDomain())
    {
        IMSA_INFO_LOG("IMSA_Domain_IsNrmSrvConditonSatisfied:voice not support!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_IsNrmSrvConditonSatisfied_ENUM, 4);

        return IMSA_FALSE;
    }

    if (VOS_FALSE == IMSA_GetUeImsVoiceCap())
    {
        IMSA_INFO_LOG("IMSA_Domain_IsNrmSrvConditonSatisfied:voice not support!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_IsNrmSrvConditonSatisfied_ENUM, 8);

        return IMSA_FALSE;
    }

    if (IMSA_IMS_VOPS_STATUS_NOT_SUPPORT == pRcvMsg->enImsVoPsInd)
    {
        /* ������಻֧��IMS����������UE֧��IMS���ţ��Ҷ��Ŵ���ע��Ŀ���
        ��ʱ����Ҫ�����ж����������Ƿ����� */
        if ((IMSA_FALSE == IMSA_GetUeSmsCapInitRegFlag())
            || (IMSA_FALSE == IMSA_GetUeImsSmsCap()))
        {
            IMSA_INFO_LOG("IMSA_SRV_IsNrmSrvConditonSatisfied:voice not support!");
            TLPS_PRINT2LAYER_INFO(IMSA_SRV_IsNrmSrvConditonSatisfied_ENUM, 9);
            return IMSA_FALSE;
        }
    }
    #if 0
    /* IMS������֧�֣��򷵻����������� */
    if ((IMSA_VOICE_DOMAIN_CS_ONLY == IMSA_GetVoiceDomain())
            || (VOS_FALSE == IMSA_GetUeImsVoiceCap())
            || (MSCC_IMSA_IMS_VOPS_NOT_SUPPORT == pRcvMsg->enImsVoPsInd))
    {
        IMSA_INFO_LOG("IMSA_Domain_IsNrmSrvConditonSatisfied:voice not support!");
        TLPS_PRINT2LAYER_INFO(IMSA_SRV_IsNrmSrvConditonSatisfied_ENUM, 2);
        return IMSA_FALSE;
    }
    #endif

    /* PS�����״̬Ϊ�������� */
    if (MSCC_IMSA_NORMAL_SERVICE != pRcvMsg->enPsServiceStatus)
    {
        IMSA_INFO_LOG("IMSA_Domain_IsNrmSrvConditonSatisfied:Ps service not normal!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_IsNrmSrvConditonSatisfied_ENUM, 10);
        return IMSA_FALSE;
    }
    /*����ɾ�����������£�UE����Ϊ��֧�����������IMSע����о�*/


    return IMSA_TRUE;
}




void IMSA_Domain_SaveDelaySwitchEvent( VOS_UINT32 ulPid, VOS_UINT32 ulMsgId)
{
    IMSA_DOMAIN_MANAGER_STRU *pstDomainManager = VOS_NULL_PTR;

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    if( ( WUEPS_PID_MAPCON == ulPid )
        && ( ID_WIFI_IMSA_STATE_IND == ulMsgId))
    {
        pstDomainManager->bitWifiQualityEvent = IMSA_OP_TRUE;
    }
    else if( (UEPS_PID_MSCC == ulPid )
        && ( ID_MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ == ulMsgId))
    {
        pstDomainManager->bitDomainCfgEvent = IMSA_OP_TRUE;
    }
    else if( (UEPS_PID_MSCC == ulPid )
        && ( ID_MSCC_IMSA_SERVICE_CHANGE_IND == ulMsgId))
    {
        pstDomainManager->bitServiceChangeIndEvent = IMSA_OP_TRUE;
    }
    else if( (PS_PID_ERRC == ulPid )
        && ( ID_LRRC_IMSA_STATE_IND == ulMsgId))
    {
        pstDomainManager->bitLteQualityEvent = IMSA_OP_TRUE;
    }
    else if( VOS_PID_TIMER == ulPid )
    {
        pstDomainManager->bitPeriodTimerEvent = IMSA_OP_TRUE;
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_StartTimer1WifiDomain()
 Description    : ����WIFI��ĳͷ���ʱ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_Domain_StartTimer1WifiDomain(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_StartTimer1WifiDomain: ENTER!");
    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    pstDomainManager->stTimer1WifiDomain.ulTimerLen = (VOS_UINT32)(pstDomainManager->ucInitTimer1Len * pstDomainManager->ucPunishTimeInWifiDomain * 1000);
    if( pstDomainManager->stTimer1WifiDomain.ulTimerLen > (VOS_UINT32)(pstDomainManager->ucMaxTimer1Len * 60 * 1000))
    {
        pstDomainManager->stTimer1WifiDomain.ulTimerLen = (VOS_UINT32)(pstDomainManager->ucMaxTimer1Len * 60 * 1000);
    }

    /* ������ʱ�� */
    IMSA_StartTimer(&pstDomainManager->stTimer1WifiDomain);

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_StopTimer1WifiDomain()
 Description    : ֹͣWIFI��ĳͷ���ʱ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/

void IMSA_Domain_StopTimer1WifiDomain()
{
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_StopTimer1WifiDomain: ENTER!");

    TLPS_PRINT2LAYER_INFO(IMSA_Domain_StopTimer1WifiDomain_ENUM, LNAS_ENTRY);

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    pstDomainManager->ucPunishTimeInWifiDomain = 1;

    /* ֹͣ��ʱ�� */
    IMSA_StopTimer(&pstDomainManager->stTimer1WifiDomain);

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_StartTimer1LteDomain()
 Description    : ����LTE��ĳͷ���ʱ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/

void IMSA_Domain_StartTimer1LteDomain(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_StartTimer1LteDomain: ENTER!");

    TLPS_PRINT2LAYER_INFO(IMSA_Domain_StartTimer1LteDomain_ENUM, LNAS_ENTRY);

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    pstDomainManager->stTimer1LteDomain.ulTimerLen = (VOS_UINT32)( pstDomainManager->ucInitTimer1Len * pstDomainManager->ucPunishTimeInLteDomain * 1000 );
    if( pstDomainManager->stTimer1LteDomain.ulTimerLen >(VOS_UINT32) (pstDomainManager->ucMaxTimer1Len * 60* 1000))
    {
        pstDomainManager->stTimer1LteDomain.ulTimerLen =(VOS_UINT32)( pstDomainManager->ucMaxTimer1Len * 60* 1000);
    }

    /* ������ʱ�� */
    IMSA_StartTimer(&pstDomainManager->stTimer1LteDomain);

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_StopTimer1LteDomain()
 Description    : ֹͣLTE��ĳͷ���ʱ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/

void IMSA_Domain_StopTimer1LteDomain(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_StopTimer1LteDomain: ENTER!");

    TLPS_PRINT2LAYER_INFO(IMSA_Domain_StopTimer1LteDomain_ENUM, LNAS_ENTRY);

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    pstDomainManager->ucPunishTimeInLteDomain = 1;

    /* ֹͣ��ʱ�� */
    IMSA_StopTimer(&pstDomainManager->stTimer1LteDomain);

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ImsDomainC1Judge()
 Description    : �о��Ƿ���ڶ����ѡ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   :
      ���ؿ���ʹ�õĺ�ѡ��:

       ID_IMSA_IMS_DOMAIN_NULL: û�п�����
       IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
IMSA_IMS_DOMAIN_ENUM_UINT8 IMSA_JudgeImsDomainC1(VOS_VOID)
{

    IMSA_DOMAIN_MANAGER_STRU *pstDomainManager = VOS_NULL_PTR;

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();
    IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: enter");
    TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_ENTRY);

    switch(IMSA_GetDomainMode())
    {
    case IMSA_IMS_DOMAIN_MODE_NULL:
        return IMSA_IMS_DOMAIN_NULL;

    case IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:
        /* ����WIFIפ������,�򷵻�WIFI */
        if( ((IMSA_IMS_DOMAIN_WIFI == pstDomainManager->enImsaAvailDomain)
              ||(IMSA_IMS_DOMAIN_LTE_WIFI == pstDomainManager->enImsaAvailDomain))
            &&(IMSA_TRUE == IMSA_SRV_IsNrmSrvIsimParaSatisfied())
            )
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 1_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");
            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL1);
            return IMSA_IMS_DOMAIN_WIFI;
        }

        /* ��ǰL2����WIFI״̬�����Ծɷ���WIFI״̬,��ʹ��ǰ������פ������,�Ծ���WIFI��,����Ƶ������ѡ����  */
        if( (IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING == pstDomainManager->enDomainFsmL2State)
            || (IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN == pstDomainManager->enDomainFsmL2State))
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 2_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");
            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL2);
            return IMSA_IMS_DOMAIN_WIFI;
        }

        break;

    case IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY:
        /*����LTEפ������, �򷵻�LTE */
        if(((IMSA_IMS_DOMAIN_LTE == pstDomainManager->enImsaAvailDomain)||
            (IMSA_IMS_DOMAIN_LTE_WIFI == pstDomainManager->enImsaAvailDomain))&&
            (IMSA_TRUE == IMSA_SRV_IsNrmSrvIsimParaSatisfied())
            )
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 3_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");
            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL3);
            return IMSA_IMS_DOMAIN_LTE;
        }

        /* ��ǰL2����LTE״̬�����Ծɷ���LTE״̬,��ʹ��ǰ������פ������,�Ծ���LTE��,����Ƶ������ѡ���� */
        if( (IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING == pstDomainManager->enDomainFsmL2State)
            || (IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN == pstDomainManager->enDomainFsmL2State))
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 4_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");
            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL4);
            return IMSA_IMS_DOMAIN_LTE;
        }
        break;

    case IMSA_IMS_DOMAIN_MODE_DUAL:
        /* LTE&WIFI������פ������,�򷵻�˫�� */
        if((IMSA_IMS_DOMAIN_LTE_WIFI == pstDomainManager->enImsaAvailDomain)&&
            (IMSA_TRUE == IMSA_SRV_IsNrmSrvIsimParaSatisfied())
            )
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 5_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL5);
            return IMSA_IMS_DOMAIN_LTE_WIFI;
        }
        /* ����LTEפ������, �򷵻�LTE */
        else if((IMSA_IMS_DOMAIN_LTE == pstDomainManager->enImsaAvailDomain)
            &&(IMSA_TRUE == IMSA_SRV_IsNrmSrvIsimParaSatisfied()))
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 6_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");
            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL6);
            return IMSA_IMS_DOMAIN_LTE;
        }
        /* ����WIFIפ������,�򷵻�WIFI */
        else if((IMSA_IMS_DOMAIN_WIFI == pstDomainManager->enImsaAvailDomain)
            &&(IMSA_TRUE == IMSA_SRV_IsNrmSrvIsimParaSatisfied()))
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 7_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");
            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL7);
            return IMSA_IMS_DOMAIN_WIFI;
        }
        /* ��ǰL2����LTE״̬�����Ծɷ���LTE״̬,��ʹ��ǰ������פ������,�Ծ���LTE��,����Ƶ������ѡ���� */
        else if( (IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING == pstDomainManager->enDomainFsmL2State)
            || (IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN == pstDomainManager->enDomainFsmL2State))
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 8_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");

            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL8);
            return IMSA_IMS_DOMAIN_LTE;
        }
        /* ��ǰL2����WIFI״̬�����Ծɷ���WIFI״̬,��ʹ��ǰ������פ������,�Ծ���WIFI��,����Ƶ������ѡ����  */
        else if( (IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING == pstDomainManager->enDomainFsmL2State)
            || (IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN == pstDomainManager->enDomainFsmL2State))
        {
            IMSA_INFO_LOG("IMSA_JudgeImsDomainC1: 9_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");

            TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL9);
            return IMSA_IMS_DOMAIN_WIFI;
        }
        /*lint -e527 */
        else
        {
            IMSA_ERR_LOG("IMSA_JudgeImsDomainC1: No Meet CampOn Condition!");

            TLPS_PRINT2LAYER_ERROR(IMSA_JudgeImsDomainC1_ENUM, LNAS_FUNCTION_LABEL10);
            return IMSA_IMS_DOMAIN_NULL;
        }
        break;
        /*lint +e527 */

    default:
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_JudgeImsDomainC1:ERROR:Wrong Domain State!");

        TLPS_PRINT2LAYER_ERROR(IMSA_JudgeImsDomainC1_ENUM, LNAS_StateInvalid);
        return IMSA_IMS_DOMAIN_NULL;
    }

    return IMSA_IMS_DOMAIN_NULL;
}


/*****************************************************************************
 Function Name  : IMSA_ImsDomainC21Judge()
 Description    : ��ʼIMS��ѡ���о��㷨 C2_1
 Input            :
 Output         : VOS_VOID
 Return Value : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

IMSA_IMS_DOMAIN_ENUM_UINT8 IMSA_JudgeImsDomainC21(VOS_VOID)
{

    IMSA_IMS_DOMAIN_ENUM_UINT8     enImsDomainResult = IMSA_IMS_DOMAIN_LTE_WIFI;
    IMSA_DOMAIN_MANAGER_STRU      *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC21_ENUM, LNAS_ENTRY);
    /* ��ʼIMSѡ�����,�������µĲ���:
        1: ��ͷ���ʱ���Ƿ�����,�������,����ʱ��ѡ�������;
        2: �����ź������������ȹ�ϵ����ѡ����
        3: ���ѡ������Ѿ����Թ�,��������һ����û�б��ͷ�,��ѡ��������
            ����������Ѿ����Թ�,������Ա���.
        */

    /* ���WIFI/LTE �ͷ���ʱ���Ƿ�����,�������,��ѡ������� */
    if( VOS_TRUE == (IMSA_IsTimerRunning(&pstDomainManager->stTimer1WifiDomain)))
    {
        IMSA_Domain_SetWifiDisable(enImsDomainResult);
    }
    if( VOS_TRUE == (IMSA_IsTimerRunning(&pstDomainManager->stTimer1LteDomain)))
    {
        IMSA_Domain_SetLteDisable(enImsDomainResult);
    }


    /*    2: �����ź������������ȹ�ϵ����ѡ���� */
    IMSA_Domain_CheckSignalPrefer(&enImsDomainResult);


    /* �������Ѿ����Թ�,���ʼ�����Ա��� */
    if( IMSA_IMS_DOMAIN_LTE_WIFI == pstDomainManager->enImsaTryedDomain )
    {
        pstDomainManager->enImsaTryedDomain = IMSA_IMS_DOMAIN_NULL;
    }

    /* ���ѡ������Ѿ����Թ�,������һ�������㳢������,��ѡ������һ���� */
    if( pstDomainManager->enImsaTryedDomain == enImsDomainResult)
    {
        if( ( IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)
            &&( VOS_FALSE == IMSA_IsTimerRunning(&pstDomainManager->stTimer1LteDomain))
            &&( IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enLteQuality ))
        {
            enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
        }
        else if(( IMSA_IMS_DOMAIN_LTE == enImsDomainResult)
            &&( VOS_FALSE == IMSA_IsTimerRunning(&pstDomainManager->stTimer1WifiDomain))
            &&( IMSA_WIRELESS_QUALITY_LEVEL_BAD < pstDomainManager->enWifiQuality ))
        {
            enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
        }
    }
    /* else: ���ѡ�����û�г��Թ�,��ֱ�ӳ��������,����Ҫ���� */

    return enImsDomainResult;
}

/*****************************************************************************
 Function Name  : IMSA_ImsDomainC221W2LJudge()
 Description    : �л��о��㷨 C2_2_1(W2L)
 Input            :
 Output         : VOS_VOID
 Return Value : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

IMSA_IMS_HO_TYPE_ENUM_UINT8 IMSA_JudgeImsDomainC221W2L()
{
    IMSA_IMS_DOMAIN_ENUM_UINT8     enImsDomainResult = IMSA_IMS_DOMAIN_LTE_WIFI;
    IMSA_IMS_HO_TYPE_ENUM_UINT8   enDomainHoType;
    IMSA_DOMAIN_MANAGER_STRU      *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    IMSA_WARN_LOG("IMSA_JudgeImsDomainC221W2L: Enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC221W2L_ENUM, LNAS_ENTRY);

    /* LTE��ͷ���ʱ���Ƿ�����,�������,����ʱ��ѡ������� */
    /*lint -e438 */
    if( VOS_TRUE == (IMSA_IsTimerRunning(&pstDomainManager->stTimer1LteDomain)))
    {
        IMSA_WARN_LOG("IMSA_JudgeImsDomainC221W2L: 1: timer start!");
        enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */

    /*  �����ź������������ȹ�ϵ����ѡ���� */
    IMSA_Domain_CheckSignalPrefer(&enImsDomainResult);

    if( (IMSA_IMS_DOMAIN_WIFI == enImsDomainResult)
        || (IMSA_IMS_DOMAIN_NULL == enImsDomainResult))
    {
        IMSA_WARN_LOG("IMSA_JudgeImsDomainC221W2L: 2: signal weak!");
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }

    /* �Ƿ�֧��ҵ��̬VoWIFI->VoLTE NV�л�ѡ��: �绰ҵ��,��Ϣҵ��,USSDҵ����� */
    /*lint -e438 */
    if( (VOS_FALSE == IMSA_GetW2LHoInActSrvFlag() )
        &&( VOS_TRUE == IMSA_IsImsExist()))
    {
        IMSA_WARN_LOG("IMSA_JudgeImsDomainC221W2L: 3: no support HO in active service!");

        enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;

        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */

    /* �Ƿ�֧�ַ�ҵ��̬���л�:�޵绰,��Ϣ��USSD���� */
    /*lint -e438 */
    if( (VOS_FALSE == IMSA_GetHoSupportInNonActSrvFlag() )
         &&(VOS_FALSE == IMSA_IsImsExist()))
    {
        IMSA_WARN_LOG("IMSA_JudgeImsDomainC221W2L: 4: no support HO in no-active service!");
        enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */
    /*��ҵ����̬�Ƿ�֧���л�ѡ�������ж�:���ڵ绰����������,��Ϣ������,USSD������ */
    /*lint -e438 */
    if( (VOS_FALSE == IMSA_GetHoSupportInUnstableSrvFlag() )
        &&( VOS_TRUE == IMSA_IsImsTransactionExist()))
    {
        IMSA_WARN_LOG("IMSA_JudgeImsDomainC221W2L: 5: no support HO in processing service!");
        enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */

    /* �����漰����ͺ����϶࣬��ʱ��ʵ��ҵ������Ժ��������л����� */

    /* ��������������,������W2L���л� */
    /*lint -e438 */
    enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
    enDomainHoType = IMSA_IMS_HO_TYPE_WIFI2LTE;
    return enDomainHoType;
    /*lint +e438 */

}

/*****************************************************************************
 Function Name  : IMSA_ImsDomainC222L2WJudge()
 Description    : �л��о��㷨 C2_2_2(L2W)
 Input            :
 Output         : VOS_VOID
 Return Value : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

IMSA_IMS_HO_TYPE_ENUM_UINT8 IMSA_JudgeImsDomainC222L2W()
{
    IMSA_IMS_DOMAIN_ENUM_UINT8     enImsDomainResult = IMSA_IMS_DOMAIN_LTE_WIFI;
    IMSA_IMS_HO_TYPE_ENUM_UINT8   enDomainHoType;
    IMSA_DOMAIN_MANAGER_STRU      *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_JudgeImsDomainC222L2W_ENUM, LNAS_ENTRY);

    /* WIFI��ͷ���ʱ���Ƿ�����,�������,����ʱ��ѡ������� */
    /*lint -e438*/
    if( VOS_TRUE == (IMSA_IsTimerRunning(&pstDomainManager->stTimer1WifiDomain)))
    {
        enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */
    /*  �����ź������������ȹ�ϵ����ѡ���� */
    IMSA_Domain_CheckSignalPrefer(&enImsDomainResult);
    /*lint -e438 */
    if( (IMSA_IMS_DOMAIN_LTE == enImsDomainResult)
        || (IMSA_IMS_DOMAIN_NULL == enImsDomainResult))
    {
        enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */
    /* �Ƿ�֧��ҵ��̬VoWIFI->VoLTE NV�л�ѡ��: �绰ҵ��,��Ϣҵ��,USSDҵ����� */
    /*lint -e438 */
    if( (VOS_FALSE == IMSA_GetL2WHoInActSrvFlag() )
        && (VOS_TRUE == IMSA_IsImsExist()))
    {
        enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */

    /* �Ƿ�֧�ַ�ҵ��̬���л�: �绰,��Ϣ��USSD���������� */
      /*lint -e438 */
    if( (VOS_FALSE == IMSA_GetHoSupportInNonActSrvFlag() )
        && (VOS_FALSE == IMSA_IsImsExist()))
    {
        enImsDomainResult = IMSA_IMS_DOMAIN_LTE;
        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */

    /*��ҵ����̬�Ƿ�֧���л�ѡ�������ж�:���ڵ绰����������,��Ϣ������,USSD������ */
    /*lint -e438 */
    if( (VOS_FALSE == IMSA_GetHoSupportInUnstableSrvFlag() )
        && (VOS_TRUE == IMSA_IsImsTransactionExist()))
    {
        enImsDomainResult = IMSA_IMS_DOMAIN_LTE;

        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */
    /* �����漰����ͺ����϶࣬��ʱ��ʵ��ҵ������Ժ��������л����� */

    /* �ж�VoLTE���Ƿ���ڽ���PDN */
    /*lint -e438 */
    if(VOS_TRUE == IMSA_IsExistEmcCall() )
    {
        enImsDomainResult = IMSA_IMS_DOMAIN_LTE;

        enDomainHoType = IMSA_IMS_HO_TYPE_NO_HO;
        return enDomainHoType;
    }
    /*lint +e438 */


    /* ��������������,������L2W���л� */
    /*lint -e438 */
    enImsDomainResult = IMSA_IMS_DOMAIN_WIFI;
    enDomainHoType = IMSA_IMS_HO_TYPE_LTE2WIFI;
    return enDomainHoType;
    /*lint +e438 */


}

/*****************************************************************************
 Function Name  : IMSA_ProcessNoDomainSelect()
 Description    : û�������ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessNoDomainSelect(VOS_UINT8 ucOrignalPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessNoDomainSelect_ENUM, LNAS_ENTRY);

    switch(pstImsaDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            // no process, return
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE, ucOrignalPdnRlsFlag);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI, ucOrignalPdnRlsFlag);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelect: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelect_ENUM, LNAS_StateInvalid);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            //�޴���ֱ�ӷ���
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelect: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelect_ENUM, LNAS_StateInvalid);
    }

    return;
}


/*****************************************************************************
 Function Name  : IMSA_ProcessNoDomainSelectInMsccImsDomainCfgSetReq()
 Description    : û�������ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessNoDomainSelectInMsccImsDomainCfgSetReq(VOS_UINT8 ucOrignalPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessNoDomainSelectInMsccImsDomainCfgSetReq_ENUM, LNAS_ENTRY);

    switch(pstImsaDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            // no process, return
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE, ucOrignalPdnRlsFlag);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI, ucOrignalPdnRlsFlag);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE_WIFI, ucOrignalPdnRlsFlag);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            //�޴���ֱ�ӷ���
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInMsccImsDomainCfgSetReq: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInMsccImsDomainCfgSetReq_ENUM, LNAS_StateInvalid);
    }

    return;
}





/*****************************************************************************
 Function Name  : IMSA_ProcessNoDomainSelectInWifiPowerOff()
 Description    : ���յ�WIFI Power Off����û�������ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_ProcessNoDomainSelectInWifiPowerOff(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessNoDomainSelectInWifiPowerOff_ENUM, LNAS_ENTRY);

    switch(pstImsaDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            // no process, return
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInWifiPowerOff: DomainC2 FSM Status Wrong1!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInWifiPowerOff_ENUM, LNAS_FUNCTION_LABEL1);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI, VOS_FALSE);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInWifiPowerOff: DomainC2 FSM Status Wrong2!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInWifiPowerOff_ENUM, LNAS_FUNCTION_LABEL2);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            //�޴���ֱ�ӷ���
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInWifiPowerOff: DomainC2 FSM Status Wrong3!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInWifiPowerOff_ENUM, LNAS_FUNCTION_LABEL3);
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ProcessNoDomainSelectInCellularPowerOff()
 Description    : ���յ�Cellular Power Off����û�������ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_ProcessNoDomainSelectInCellularPowerOff(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessNoDomainSelectInCellularPowerOff_ENUM, LNAS_ENTRY);

    switch(pstImsaDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            // no process, return
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI, VOS_FALSE);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInCellularPowerOff: DomainC2 FSM Status Wrong1!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInCellularPowerOff_ENUM, LNAS_FUNCTION_LABEL1);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInCellularPowerOff: DomainC2 FSM Status Wrong2!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInCellularPowerOff_ENUM, LNAS_FUNCTION_LABEL2);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            //�޴���ֱ�ӷ���
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInCellularPowerOff: DomainC2 FSM Status Wrong3!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInCellularPowerOff_ENUM, LNAS_FUNCTION_LABEL3);
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ProcessNoDomainSelectInMsccServiceChangeInd()
 Description    : �յ�MSCC_Service_Changed_ind, �о�û�������ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessNoDomainSelectInMsccServiceChangeInd(VOS_UINT8 ucOrignalPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessNoDomainSelectInMsccServiceChangeInd_ENUM, LNAS_ENTRY);

    switch(pstImsaDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            // no process, return
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING;
            IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE, ucOrignalPdnRlsFlag);
            pstImsaDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* Disable Wifiֻ��WIFI Power Off ����в���,�쳣������ */
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInMsccServiceChangeInd: Abnoral Process to disable WIFI!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInMsccServiceChangeInd_ENUM, LNAS_FUNCTION_LABEL1);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* ���������ػ�,�����л������в������˫��رյ����,�쳣������ */
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInMsccServiceChangeInd: Abnoral Process to disable LTE/WIFI!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInMsccServiceChangeInd_ENUM, LNAS_FUNCTION_LABEL2);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            /* ����Disable ��, �޴���ֱ�ӷ��� */
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInMsccServiceChangeInd: Disable on-going!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInMsccServiceChangeInd_ENUM, LNAS_FUNCTION_LABEL3);

            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessNoDomainSelectInMsccServiceChangeInd: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessNoDomainSelectInMsccServiceChangeInd_ENUM, LNAS_FUNCTION_LABEL4);
            break;
    }

    return;
}


/*****************************************************************************
 Function Name  : IMSA_ProcessWifiOnlyDomainSelect()
 Description    : ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessWifiOnlyDomainSelect(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessWifiOnlyDomainSelect_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��ͣ����NULL״̬ */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if(( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
               &&(VOS_FALSE == IMSA_IsExistEmcCall()))
            {
                /* VoWIFI����, ������VoLTE->VoWIFI���л� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, VOS_FALSE, ucPdnRlsFlag);

                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��Disable LTE��: ��Ϊֻ��WIFI�����, ����WIFI���޷����� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE,ucPdnRlsFlag);

                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                //�޴���������״
            }
            else
            {
                /* VoWIFI������, ��Disable WIFI�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelect: No Switch State In Delay Event!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessWifiOnlyDomainSelect_ENUM, IMSA_STATE_ERROR);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            // ��¼�¼���������ʱ����
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelect: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessWifiOnlyDomainSelect_ENUM, LNAS_StateInvalid);
    }

    return;

}


/*****************************************************************************
 Function Name  : IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq()
 Description    : ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��ͣ����NULL״̬ */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                if(VOS_FALSE == IMSA_IsExistEmcCall())
                {
                    /* VoWIFI����, ������VoLTE->VoWIFI���л� */
                    pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                    IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, VOS_FALSE, ucPdnRlsFlag);

                    pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
                }
                else
                {
                    /* ͣ����LTE��,�ȴ�����PDN�ͷ� */
                }
            }
            else
            {
                /* VoWIFI������, ��Disable LTE��: ��Ϊֻ��WIFI�����, ����WIFI���޷����� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE,ucPdnRlsFlag);

                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                //�޴���������״
            }
            else
            {
                /* VoWIFI������, ��Disable WIFI�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, VOS_FALSE, ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��Abort LTE2WIFI���л�, disable˫�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE_WIFI,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
                IMSA_SRV_DomainSwitchAbort(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��Disable WIFI��LTE�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE_WIFI,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            // ��¼�¼���������ʱ����
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq_ENUM, LNAS_StateInvalid);
    }

    return;

}

/*****************************************************************************
 Function Name  : IMSA_ProcessWifiOnlyDomainSelectInWifiStateInd()
 Description    : ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessWifiOnlyDomainSelectInWifiStateInd
(
    VOS_UINT8 ucOrgPdnRlsFlag
)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessWifiOnlyDomainSelectInWifiStateInd_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��ͣ����NULL״̬ */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
         /* ��ǰ����LTE��,һ��WIFI�ź�ǿ��ָʾ���ᵼ��ѡ��WIFI ONLYȥ,����һ���쳣,������
            IMSA_WARN_LOG("IMSA_WifiOnlyProcess: DomainC2 FSM Status Wrong!");*/
            /* ���ʱû�п��ǵ����³���: ��ǰ����LTE��, ����������GU��,�п����ն˲��ͷ�IMS PDN(�ƶ�����Ҫ����L���ͷ�,���ǿ��Բ��ͷ�),
                          ��ʱ,һ��WIFI�ź�ǿ��ָʾ,���п��ܷ���L2_FSM��LTE��,����Ҫ��ѡ��WIFI ONLYȥ */
            if(( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
                &&(VOS_FALSE == IMSA_IsExistEmcCall()))
            {
                /* VoWIFI����, ������VoLTE->VoWIFI���л� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, VOS_FALSE, VOS_TRUE);

                /* �����������������Ϊ */
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��ͣ����LTE��: ��Ϊֻ��WIFI�����, ����WIFI���޷����� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                //�޴���������״
                IMSA_SRV_DomainContinue(IMSA_IMS_DOMAIN_WIFI);
            }
            else
            {
                /* VoWIFI������, ��Disable WIFI�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI,ucOrgPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            /* L2W �л���ǰһ����ѡ�Ľ��,����յ�WIFI State Ind��Ϣ, ���ֵ�ǰL2W�л������� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* ���ǰһ��״̬����W2L, ���ǵ�ǰ�ִ���WIFI only,�м�϶�����һ����������LTE��disable,
                         ������LTEת��GU, ���ʱ��ǰһ������Ӧ���Ѿ�Abort W2L�Ĳ������ȴ�SRV���ؽ��,
                         �����������²����� */

            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            /* ��¼�¼����ȴ�ǰһ��Disable���������ٽ��д��� */
            pstDomainManager->ucMsgDelayProcessFlag = VOS_TRUE;
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq_ENUM, LNAS_StateInvalid);
    }

    return;

}



/*****************************************************************************
 Function Name  : IMSA_ProcessWifiOnlyDomainSelectInMsccServiceChangeInd()
 Description    : MSCC_SERVICE_CHANGE_IND��Ϣ��, ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessWifiOnlyDomainSelectInMsccServiceChangeInd(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessWifiOnlyDomainSelectInMsccServiceChangeInd_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ����:
                         WIFI�����о���ԭ����,�ܿ�������LTEפ�������ı��,��������IMS�����仯,��ʱ���³��Խ��� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��ͣ����NULL״̬ */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* ��ǰ����LTE��,һ��Cellular�����������(��L->GU), ����ѡ��WIFI ONLY */
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if(( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
                &&(VOS_FALSE == IMSA_IsExistEmcCall()))
            {
                /* VoWIFI����, ������VoLTE->VoWIFI���л� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, VOS_FALSE, ucPdnRlsFlag);

                /* �����������������Ϊ */
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��ͣ����LTE��: ��Ϊֻ��WIFI�����, ����WIFI���޷����� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                //�޴���������״
            }
            else
            {
                /* Cellular����״̬�������,����VoWIFI��Ϊ������: ��Disable WIFI�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_WIFI,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            /* �ϴ���ѡ�������L2W, �����ν��յ�Service_change_ind��Ϣ����LTE������:�����л�״̬ */
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                //�޴���������״
            }
            else
            {
                /* VoWIFI������, ��Abort LTE2WIFI���л�, disable˫�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE_WIFI,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                /* �ϴ���ѡ�������W2L, ��������LTE������,������ѡ�����ͣ����WIFI��, abort�л� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
                IMSA_SRV_DomainSwitchAbort(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��Disable WIFI��LTE�� */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_DISABLING;
                IMSA_SRV_DomainDisable(IMSA_IMS_DOMAIN_LTE_WIFI,ucPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelectInMsccServiceChangeInd: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessWifiOnlyDomainSelectInMsccServiceChangeInd_ENUM, LNAS_StateInvalid);
            break;
    }

    return;

}

/*****************************************************************************
 Function Name  : IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady()
 Description    : UsimFile�ļ���ȫ�Ժ�Ĳ���: ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* �жϵ�ǰVoWIFI�Ƿ���VoLTE����, ����VoWIFI�Ƿ���� */
            if( VOS_TRUE == IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi())
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /* VoWIFI������, ��ͣ����NULL״̬ */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady: Error Status in USIM Card File Ready!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady_ENUM, IMSA_STATE_ERROR);
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady_ENUM, LNAS_StateInvalid);
            break;
    }

    return;

}



/*****************************************************************************
 Function Name  : IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff()
 Description    : CellularPOWER OFF��Ϣ�����Ժ�,ֻʣ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff()
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    IMSA_INFO_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff: Enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            IMSA_INFO_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff:1:DomainEnable!");
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
            IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:

            IMSA_INFO_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff:2:Domainswitch L2W!");
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
            IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, VOS_FALSE, VOS_FALSE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            //�޴���������״
            IMSA_INFO_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff:3: stay on wifi!");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_INFO_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff:4:DomainswitchAbort W2L!");
            IMSA_SRV_DomainSwitchAbort(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH,VOS_TRUE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            IMSA_INFO_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff:5:Domainswitch L2W change para!");
            IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, VOS_FALSE, VOS_TRUE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            IMSA_INFO_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff:6:Disabling!");
            // ��¼�¼���������ʱ����
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessWifiOnlyDomainSelectInCellularPowerOff: DomainC2 FSM Status Wrong!");
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ProcessLTEOnlyDomainSelect()
 Description    : ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessLTEOnlyDomainSelect(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessLTEOnlyDomainSelect_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
            IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_LTE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            //�޴���������״
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
            IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, VOS_FALSE, ucPdnRlsFlag);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelect: No Switch State in Delay Event!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessLTEOnlyDomainSelect_ENUM, IMSA_STATE_ERROR);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            // ��¼�¼���������ʱ����
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelect: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessLTEOnlyDomainSelect_ENUM, LNAS_StateInvalid);
    }

    return;

}


/*****************************************************************************
 Function Name  : IMSA_ProcessLTEOnlyDomainSelectInMsccImsDomainCfgSetReq()
 Description    : ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessLTEOnlyDomainSelectInMsccImsDomainCfgSetReq(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessLTEOnlyDomainSelectInMsccImsDomainCfgSetReq_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
            IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_LTE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            //�޴���������״
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
            IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, VOS_FALSE, ucPdnRlsFlag);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
            IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, VOS_FALSE, ucPdnRlsFlag);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
            IMSA_SRV_DomainSwitchAbort(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,ucPdnRlsFlag);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            // ��¼�¼���������ʱ����
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInMsccImsDomainCfgSetReq: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessLTEOnlyDomainSelectInMsccImsDomainCfgSetReq_ENUM, LNAS_StateInvalid);
    }

    return;

}

/*****************************************************************************
 Function Name  : IMSA_ProcessLTEOnlyDomainSelectInWifiPowerOff()
 Description    : WIFI POWER OFF��Ϣ�����Ժ�,ֻʣ��LTE�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_ProcessLTEOnlyDomainSelectInWifiPowerOff()
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessLTEOnlyDomainSelectInWifiPowerOff_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
            IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_LTE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            //�޴���������״
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
            IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, VOS_FALSE, VOS_FALSE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, VOS_FALSE, VOS_FALSE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            IMSA_SRV_DomainSwitchAbort(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,VOS_FALSE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            // ��¼�¼���������ʱ����
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInWifiPowerOff: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessLTEOnlyDomainSelectInWifiPowerOff_ENUM, LNAS_StateInvalid);
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd()
 Description    : ���յ�WIFI_STATE_IND��Ϣ��, ֻ��LTE�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* WIFI State_Ind��Ϣ����,��������LTE��: ��������״̬�µĴ���. */
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd: Go LTE Domain when receiving Wifi State Ind ");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* LTE��פ�����߳���פ��״̬���յ�WIFI State ind,�޴���,���ֵ�ǰ״̬ */

            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            
#if 0
            IMSA_SRV_DomainSwitch(IMSA_IMS_HO_TYPE_W2L_SWITCH, 0, ucPdnRlsFlag);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
#endif
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* �����W2L�л�,�󲿷�������WIFI Power Off���µ�,������������,���յ�WIFI State Ind ��Ӱ�쵱ǰ����,ֱ�ӷ��� */

            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            /* Wifi State Ind��Ϣ����ѡ��LTE ,ͬʱǰһ����ѡ������L2W�л�״̬, �쳣�����������κδ��� */
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd: Abnormal Switch!");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:

            /* ��¼�¼����ȴ�ǰһ��Disable���������ٽ��д��� */
            pstDomainManager->ucMsgDelayProcessFlag = VOS_TRUE;
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd_ENUM, LNAS_StateInvalid);
    }

    return;

}

/*****************************************************************************
 Function Name  : IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd()
 Description    : ���յ�MSCC_SERVICE_CHANGE_IND��Ϣ��, ֻ��LTE�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* NULL״̬���յ�Service_Change_ind, ��ѡ����LTE��,������LTE���� */
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;

            IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_LTE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* ����Continue���� */
            IMSA_SRV_DomainContinue(IMSA_IMS_DOMAIN_LTE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �յ�Service change ind��Ϣ,��һ������ת������һ������,���������쳣���� */
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd: Abnormal From WIFI2LTE");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* �������ǰ������������л�����, ��ǰ���ڵ���״̬,Ӧ����ǰһ���������µ�,
                        ��WIFI Power off,��ʱ,�����ظ��������,�ȴ�ǰһ��Ԥѡ��� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            /* ǰһ����ѡ�����L2W���л�, ���ǵ�ǰ��ѡ���ȷʵLTE����, �м����WIFI disable�����,����һ���쳣 */
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd: L2W switch in LTE only domain select");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
            TLPS_PRINT2LAYER_INFO(IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd_ENUM, 0);
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
            IMSA_SRV_DomainContinue(IMSA_IMS_DOMAIN_LTE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;

            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd_ENUM, LNAS_StateInvalid);
    }

    return;

}


/*****************************************************************************
 Function Name  : IMSA_ProcessLTEOnlyDomainSelectInUsimFileReady()
 Description    : USIM Card File Ready�µ���ѡ����: ֻ��WIFI�����ʹ�õĴ���
 Input          :
                  ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/

void IMSA_ProcessLTEOnlyDomainSelectInUsimFileReady(VOS_UINT8 ucPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessLTEOnlyDomainSelectInUsimFileReady_ENUM, LNAS_ENTRY);

    switch( pstDomainManager->enDomainFsmL2State )
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
            IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_LTE);
            pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInUsimFileReady: Error Status in USIM Card File Ready!");
            break;

        default:
            IMSA_WARN_LOG("IMSA_ProcessLTEOnlyDomainSelectInUsimFileReady: DomainC2 FSM Status Wrong!");
    }

    return;

}



/*****************************************************************************
 Function Name  : IMSA_DualDomainProcess()
 Description    : ˫��ѡ��Ĵ���
 Input          :
                      ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/
void IMSA_ProcessDualDomainSelect(VOS_UINT8 ucHoFailReturn2OrgPdnFlag, VOS_UINT8 ucOrgPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;
    IMSA_IMS_DOMAIN_ENUM_UINT8             enFinalDomain;
    IMSA_IMS_HO_TYPE_ENUM_UINT8           enHoType;


    IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessDualDomainSelect_ENUM, LNAS_ENTRY);

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    switch(pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* ��ʼIMS��ѡ���о��㷨 */
            enFinalDomain = IMSA_JudgeImsDomainC21();

            if(IMSA_IMS_DOMAIN_WIFI  == enFinalDomain)
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else if(IMSA_IMS_DOMAIN_LTE  ==  enFinalDomain)
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_LTE);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /*û���ҵ����õ�����ʱ�򣬼������ֿ���״̬���ȴ��´��о� */
            }

            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 1: enable one domain!");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
            /* ��ʼIMS��ѡ���о��㷨 */
            enFinalDomain = IMSA_JudgeImsDomainC21();

            if(IMSA_IMS_DOMAIN_WIFI  == enFinalDomain)
            {
                /*��¼�л��¼�����������LTE Domain,�ȴ�LTE����ɳ����Ժ��ٴ��о� */
                pstDomainManager->ucMsgDelayProcessFlag = VOS_TRUE;
            }
            else if(IMSA_IMS_DOMAIN_LTE  ==  enFinalDomain)
            {
                /* �������ֵ�ǰ����״̬ */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
                IMSA_SRV_DomainContinue(IMSA_IMS_DOMAIN_LTE);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /*û���ҵ����õ������ʱ�򱨸��쳣 */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: IMSA_ImsDomainC21Judge Wrong!");
                TLPS_PRINT2LAYER_WARNING(IMSA_ProcessDualDomainSelect_ENUM, IMSA_DOMAIN_STATUS_NULL);

            }

            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 2: no HO!");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* �л��о��㷨 C2_2_2: L2W */
            enHoType = IMSA_JudgeImsDomainC222L2W();

            if( IMSA_IMS_HO_TYPE_LTE2WIFI == enHoType)
            {
                 /* �ж�SRV״̬�Ƿ������л� */
                 if(VOS_TRUE == IMSA_SRV_CheckSwitchPossible())
                  {
                     /* ����L2W�л�����,ָ���л�ʧ�ܲ���,�Լ��л��ɹ�ԭ��PDN����ʽ */
                     pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                     IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, ucHoFailReturn2OrgPdnFlag, ucOrgPdnRlsFlag);
                     pstDomainManager->ucDomainSelectFlag = VOS_TRUE;

                     IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 3: L2W HO!");
                 }
                 else
                 {
                     /*��¼�л��¼����ȴ�SRV������̬�ٴ��о� */
                     pstDomainManager->ucMsgDelayProcessFlag = VOS_TRUE;

                     IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 3: wait for L2W HO!");
                 }
            }
            else if(IMSA_IMS_HO_TYPE_WIFI2LTE == enHoType)
            {
                /* �����쳣,��Ӧ�������ַ���ֵ */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: IMSA_ImsDomainC222Judge Wrong!");
            }
            else
            {
                /* û���л�,������ԭ����,�������� */

                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 3:No L2W HO!");
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
            /* ��ʼIMS��ѡ���о��㷨 */
            enFinalDomain = IMSA_JudgeImsDomainC21();

            if(IMSA_IMS_DOMAIN_LTE  == enFinalDomain)
            {
                 /* ��¼�л��¼�����������WIFI Domain,�ȴ�WIFI����ɳ����Ժ��ٴ��о� */
                 pstDomainManager->ucMsgDelayProcessFlag = VOS_TRUE;

                 IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 4: wait L2W HO!");
            }
            else if(IMSA_IMS_DOMAIN_WIFI  ==  enFinalDomain)
            {
                //�������ֵ�ǰ����״̬��
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 4: wait L2W HO!");
            }
            else
            {
               /*û���ҵ����õ������ʱ�򱨸��쳣 */
               IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: IMSA_ImsDomainC21Judge Wrong!");
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �л��о��㷨 C2_2_1:W2L */
            enHoType = IMSA_JudgeImsDomainC221W2L();

            if( IMSA_IMS_HO_TYPE_WIFI2LTE == enHoType)
            {
                /* �ж�SRV״̬�Ƿ������л� */
                if(VOS_TRUE == IMSA_SRV_CheckSwitchPossible())
                {
                    /* ����L2W�л�����,ָ���л�ʧ�ܲ���,�Լ��л��ɹ�ԭ��PDN����ʽ */
                    pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
                    IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, ucHoFailReturn2OrgPdnFlag, ucOrgPdnRlsFlag);
                    pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
                    IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 5: W2L HO!");

                }
                else
                {
                    /*��¼�л��¼����ȴ�SRV������̬�ٴ��о� */
                    pstDomainManager->ucMsgDelayProcessFlag = VOS_TRUE;

                    IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 5: wait for W2L HO!");
                }
            }
            else if(IMSA_IMS_HO_TYPE_LTE2WIFI == enHoType)
            {
                /* �����쳣,��Ӧ�������ַ���ֵ */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: IMSA_ImsDomainC221Judge Wrong!");
            }
            else
            {
                /* û���л�,������ԭ����,�������� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* ��֧���л������е���ѡ�����: �޴��� */
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 6:No HO!");
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            /* �����ǰ����Disable������,��ͣ�κ���ѡ����;
                         ��¼��ѡ�¼����ȴ�SRV������̬�ٴ��о� */
            pstDomainManager->ucMsgDelayProcessFlag = 1;
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: 7:No HO!");
            break;

        default:
            /*  �����쳣 */
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelect: DomainC2 FSM Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessDualDomainSelect_ENUM, IMSA_STATE_ERROR);
    }

    return;
}




/*****************************************************************************
 Function Name  : IMSA_ProcessDualDomainSelectInUsimFileReady()
 Description    : USIM Card File Ready�µ�˫��ѡ��Ĵ���
 Input          :
                      ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/
void IMSA_ProcessDualDomainSelectInUsimFileReady(VOS_UINT8 ucHoFailReturn2OrgPdnFlag, VOS_UINT8 ucOrgPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;
    IMSA_IMS_DOMAIN_ENUM_UINT8             enFinalDomain;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessDualDomainSelectInUsimFileReady_ENUM, LNAS_ENTRY);

    switch(pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            /* ��ʼIMS��ѡ���о��㷨 */
            enFinalDomain = IMSA_JudgeImsDomainC21();

            if(IMSA_IMS_DOMAIN_WIFI  == enFinalDomain)
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_WIFI);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else if(IMSA_IMS_DOMAIN_LTE  ==  enFinalDomain)
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
                IMSA_SRV_DomainEnable(IMSA_IMS_DOMAIN_LTE);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else
            {
                /*û���ҵ����õ�����ʱ�򣬼������ֿ���״̬���ȴ��´��о� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInUsimFileReady: Error Status in USIM Card File Ready!");
            break;

        default:
            /*  �����쳣 */
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInUsimFileReady: DomainC2 FSM Status Wrong!");
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ProcessDualDomainSelectInLrrcStateInd()
 Description    : �յ�Lrrc state ind��Ϣ��˫��ѡ��Ĵ���
 Input          :
                      ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/
void IMSA_ProcessDualDomainSelectInLrrcStateInd(VOS_UINT8 ucHoFailReturn2OrgPdnFlag, VOS_UINT8 ucOrgPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;
    /*IMSA_IMS_DOMAIN_ENUM_UINT8             enFinalDomain;*/
    IMSA_IMS_HO_TYPE_ENUM_UINT8            enHoType;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessDualDomainSelectInLrrcStateInd_ENUM, LNAS_ENTRY);

    switch(pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
            /* LRRC��Ӱ���κ������о�����������˫���л� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* �л��о��㷨 C2_2_2: L2W */
            enHoType = IMSA_JudgeImsDomainC222L2W();

            if( IMSA_IMS_HO_TYPE_LTE2WIFI == enHoType)
            {
                 /* �ж�SRV״̬�Ƿ������л� */
                 if(VOS_TRUE == IMSA_SRV_CheckSwitchPossible())
                  {
                     /* ����L2W�л�����,ָ���л�ʧ�ܲ���,�Լ��л��ɹ�ԭ��PDN����ʽ */
                     pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                     IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, ucHoFailReturn2OrgPdnFlag, ucOrgPdnRlsFlag);
                     pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
                 }
                 else
                 {
                     /*��¼�л��¼����ȴ�SRV������̬�ٴ��о� */
                     pstDomainManager->ucMsgDelayProcessFlag = 1;
                 }
            }
            else if(IMSA_IMS_HO_TYPE_WIFI2LTE == enHoType)
            {
                /* �����쳣,��Ӧ�������ַ���ֵ */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInLrrcStateInd: IMSA_ImsDomainC222Judge Wrong!");
            }
            else
            {
                /* û���л�,������ԭ����,�������� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
            /* LRRC��Ӱ���κ������о�����������˫���л� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �л��о��㷨 C2_2_1:W2L */
            enHoType = IMSA_JudgeImsDomainC221W2L();

            if( IMSA_IMS_HO_TYPE_WIFI2LTE == enHoType)
            {
                /* �ж�SRV״̬�Ƿ������л� */
                if(VOS_TRUE == IMSA_SRV_CheckSwitchPossible())
                {
                    /* ����L2W�л�����,ָ���л�ʧ�ܲ���,�Լ��л��ɹ�ԭ��PDN����ʽ */
                    pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
                    IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, ucHoFailReturn2OrgPdnFlag, ucOrgPdnRlsFlag);
                    pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
                }
                else
                {
                    /*��¼�л��¼����ȴ�SRV������̬�ٴ��о� */
                    pstDomainManager->ucMsgDelayProcessFlag = 1;
                }
            }
            else if(IMSA_IMS_HO_TYPE_LTE2WIFI == enHoType)
            {
                /* �����쳣,��Ӧ�������ַ���ֵ */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInLrrcStateInd: IMSA_ImsDomainC221Judge Wrong!");
            }
            else
            {
                /* û���л�,������ԭ����,�������� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* ��֧���л������е���ѡ�����: �޴��� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            /* �����ǰ����Disable������,��ͣ�κ���ѡ����;*/
            break;

        default:
            /*  �����쳣 */
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInLrrcStateInd: DomainC2 FSM Status Wrong!");
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ProcessDualDomainSelectInWifiErrorInd()
 Description    : �յ�Wifi_Error_Ind��Ϣ��˫��ѡ��Ĵ���
 Input          :
                      ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-07  Draft Enact
*****************************************************************************/
void IMSA_ProcessDualDomainSelectInWifiErrorInd(VOS_UINT8 ucHoFailReturn2OrgPdnFlag, VOS_UINT8 ucOrgPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;
    /*IMSA_IMS_DOMAIN_ENUM_UINT8             enFinalDomain;*/
    IMSA_IMS_HO_TYPE_ENUM_UINT8           enHoType;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessDualDomainSelectInWifiErrorInd_ENUM, LNAS_ENTRY);


    switch(pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
            /* Wifi_Error_Ind��Ӱ���κ������о�����������˫���л� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �л��о��㷨 C2_2_1:W2L */
            enHoType = IMSA_JudgeImsDomainC221W2L();

            if( IMSA_IMS_HO_TYPE_WIFI2LTE == enHoType)
            {
                /* ����L2W�л�����,ָ���л�ʧ�ܲ���,�Լ��л��ɹ�ԭ��PDN����ʽ */
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
                IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, ucHoFailReturn2OrgPdnFlag, ucOrgPdnRlsFlag);
                pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
            }
            else if(IMSA_IMS_HO_TYPE_LTE2WIFI == enHoType)
            {
                /* �����쳣,��Ӧ�������ַ���ֵ */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInWifiErrorInd: IMSA_ImsDomainC221Judge Wrong!");

                TLPS_PRINT2LAYER_WARNING(IMSA_ProcessDualDomainSelectInWifiErrorInd_ENUM, IMSA_STATE_ERROR);
            }
            else
            {
                /* û���л�,������ԭ����,�������� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* ��֧���л������е���ѡ�����: �޴��� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            /* �����ǰ����Disable������,��ͣ�κ���ѡ����;*/
            break;

        default:
            /*  �����쳣 */
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInWifiErrorInd: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_ProcessDualDomainSelectInWifiErrorInd_ENUM, LNAS_StateInvalid);
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_ProcessDualDomainSelectInBearerRelInd()
 Description    : �յ�Bearer_rel_ind��Ϣ��˫��ѡ��Ĵ���
 Input          :
                      ucOrgPdnRlsFlag: ���л��Ƿ��ͷ�ԭPDN��0 ���ͷţ�1 �ͷ�
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-25  Draft Enact
*****************************************************************************/
void IMSA_ProcessDualDomainSelectInBearerRelInd(VOS_UINT8 ucHoFailReturn2OrgPdnFlag, VOS_UINT8 ucOrgPdnRlsFlag)
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;
    /*IMSA_IMS_DOMAIN_ENUM_UINT8             enFinalDomain;*/
    IMSA_IMS_HO_TYPE_ENUM_UINT8           enHoType;

    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_ProcessDualDomainSelectInBearerRelInd_ENUM, LNAS_ENTRY);

    switch(pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
            /* �����쳣��Ӱ���κ������о�����������˫���л� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /* �л��о��㷨 C2_2_2: L2W */
            enHoType = IMSA_JudgeImsDomainC222L2W();

            if( IMSA_IMS_HO_TYPE_LTE2WIFI == enHoType)
            {
                 /* �ж�SRV״̬�Ƿ������л� */
                 if(VOS_TRUE == IMSA_SRV_CheckSwitchPossible())
                 {
                    /* LTE���Ѿ����Թ�,����ʧ��,��Ҫ������ */
                     IMSA_Domain_SetLteEnable(pstDomainManager->enImsaTryedDomain);

                     /* ����L2W�л�����,ָ���л�ʧ�ܲ���,�Լ��л��ɹ�ԭ��PDN����ʽ */
                     pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH;
                     IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH, ucHoFailReturn2OrgPdnFlag, ucOrgPdnRlsFlag);
                     pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
                 }
            }
            else if(IMSA_IMS_HO_TYPE_WIFI2LTE == enHoType)
            {
                /* �����쳣,��Ӧ�������ַ���ֵ */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInBearerRelInd: IMSA_ImsDomainC222Judge Wrong!");
            }
            else
            {
                /* û���л�,������ԭ����,�������� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
            /* �����쳣��Ӱ���κ������о�����������˫���л� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /* �л��о��㷨 C2_2_1:W2L */
            enHoType = IMSA_JudgeImsDomainC221W2L();

            if( IMSA_IMS_HO_TYPE_WIFI2LTE == enHoType)
            {
                /* �ж�SRV״̬�Ƿ������л� */
                if(VOS_TRUE == IMSA_SRV_CheckSwitchPossible())
                {
                    /* WIFI���Ѿ����Թ�,����ʧ��,��Ҫ������ */
                     IMSA_Domain_SetWifiEnable(pstDomainManager->enImsaTryedDomain);

                    /* ����L2W�л�����,ָ���л�ʧ�ܲ���,�Լ��л��ɹ�ԭ��PDN����ʽ */
                    pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH;
                    IMSA_SRV_DomainSwitch(IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH, ucHoFailReturn2OrgPdnFlag, ucOrgPdnRlsFlag);
                    pstDomainManager->ucDomainSelectFlag = VOS_TRUE;
                }
            }
            else if(IMSA_IMS_HO_TYPE_LTE2WIFI == enHoType)
            {
                /* �����쳣,��Ӧ�������ַ���ֵ */
                IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInBearerRelInd: IMSA_ImsDomainC221Judge Wrong!");
            }
            else
            {
                /* û���л�,������ԭ����,�������� */
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* ��֧���л������е���ѡ�����: �޴��� */
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            /* �����ǰ����Disable������,��ͣ�κ���ѡ����;*/
            break;

        default:
            /*  �����쳣 */
            IMSA_WARN_LOG("IMSA_ProcessDualDomainSelectInBearerRelInd: DomainC2 FSM Status Wrong!");
    }

    return;
}



/*****************************************************************************
 Function Name  : IMSA_DomainSelectResultInNoSrv()
 Description    : ����NO SERVICE��Status IND����ѡ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-15  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_DomainSelect(VOS_VOID)
{

    TLPS_PRINT2LAYER_INFO(IMSA_DomainSelect_ENUM, LNAS_ENTRY);

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:
             IMSA_ProcessNoDomainSelect(VOS_TRUE);
             break;

        case IMSA_IMS_DOMAIN_WIFI:
             IMSA_ProcessWifiOnlyDomainSelect(VOS_TRUE);
             break;

        case IMSA_IMS_DOMAIN_LTE:
             IMSA_ProcessLTEOnlyDomainSelect(VOS_TRUE);
             break;

        case IMSA_IMS_DOMAIN_LTE_WIFI:
             IMSA_ProcessDualDomainSelect(VOS_TRUE,VOS_TRUE);
             break;

        default:
            IMSA_WARN_LOG("IMSA_DomainSelect: DomainC1Judge Wrong!");
    }

}

/*****************************************************************************
 Function Name  : IMSA_Domain_DomainSelectInWifiStateInd()
 Description    : WifiStateInd����ѡ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-15  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Domain_DomainSelectInWifiStateInd()
{
    IMSA_DOMAIN_MANAGER_STRU            *pstDomainManager = VOS_NULL;

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_DomainSelectInWifiStateInd_ENUM, LNAS_ENTRY);


    /* ������ѡ��ʼ���������� */
    IMSA_Domain_InitDomainOpVar(WUEPS_PID_MAPCON,ID_WIFI_IMSA_STATE_IND,VOS_TRUE,VOS_TRUE);

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:
            IMSA_WARN_LOG("IMSA_Domain_DomainSelectInWifiStateInd: No Process in This State");
            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_DomainSelectInWifiStateInd_ENUM, IMSA_STATE_ERROR);
            break;

        case IMSA_IMS_DOMAIN_WIFI:
            IMSA_ProcessWifiOnlyDomainSelectInWifiStateInd(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_LTE:
            IMSA_ProcessLTEOnlyDomainSelectInWifiStateInd(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_LTE_WIFI:
            IMSA_ProcessDualDomainSelect(VOS_TRUE,VOS_TRUE);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_DomainSelectInWifiStateInd: DomainC1Judge Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_DomainSelectInWifiStateInd_ENUM, LNAS_StateInvalid);
    }

    /* �����Ҫ�ӳٴ�����Ϣ,��¼�¼�,�ȴ�SRV�ϱ�״̬,Ȼ������������ѡ���� */
    if(VOS_TRUE == pstDomainManager->ucMsgDelayProcessFlag)
    {
       /* ���浱ǰ��Ҫ�ӳٴ������ѡ���¼� */
       IMSA_Domain_SaveDelaySwitchEvent(WUEPS_PID_MAPCON,ID_WIFI_IMSA_STATE_IND);

       /* ��SRVģ��ע��״̬�ϱ��¼�, SRV������̬���ϱ�Ready_ind��Ϣ,�����µ���ѡ���¼� */
       IMSA_SRV_RegStatusNtfEvent();

       /* ���Delay�¼� */
       pstDomainManager->ucMsgDelayProcessFlag = VOS_FALSE;
    }
    else
    {
    }

    return;
}


/*****************************************************************************
 Function Name  : IMSA_DomainSelectResultInNoSrv()
 Description    : ����NO SERVICE��Status IND����ѡ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-15  Draft Enact
*****************************************************************************/


VOS_VOID IMSA_DomainSelectResultInNoSrv(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU            *pstDomainManager = VOS_NULL;

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_DomainSelectResultInNoSrv_ENUM, LNAS_ENTRY);

    /* ���Domain FSM L2�������ڳ���һ��IMS���״̬����ظ�������̬ */
    switch( pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            /*�������ǰ����ģʽ֧��˫�� */
            if( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_JudgeImsDomainC1())
            {
                /* LTE���Ѿ����Թ�,����ʧ��,��Ҫ������ */
                IMSA_Domain_SetLteEnable(pstDomainManager->enImsaTryedDomain);
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;

                /* ����LTE��ͷ���ʱ�� */
                /* lint -e734*/
                pstDomainManager->ucPunishTimeInLteDomain = (VOS_UINT8)(pstDomainManager->ucPunishTimeInLteDomain * 2);
                IMSA_Domain_StartTimer1LteDomain();
                /* lint +e734*/

                /* ��������򶼳��Թ�������ͣ���ԣ��ȴ��´δ��� */
                if( IMSA_IMS_DOMAIN_LTE_WIFI == pstDomainManager->enImsaTryedDomain )
                {
                    pstDomainManager->enImsaTryedDomain = IMSA_IMS_DOMAIN_NULL;

#if 0
                    /*�ϱ��������� */
                    if (IMSA_TRUE == IMSA_IsImsVoiceContidionSatisfied())
                    {
                        IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_AVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                    }
                    else
                    {
                        IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_UNAVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                    }
#endif
                }
                else
                {
                    /* ������ѡ����,���ݵ�ǰ״̬�ٴ�ѡ����ʵ��� */
                    IMSA_DomainSelect();
                }
            }
            else
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;
#if 0
                /*�ϱ��������� */
                if (IMSA_TRUE == IMSA_IsImsVoiceContidionSatisfied())
                {
                    IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_AVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                }
                else
                {
                    IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_UNAVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                }
#endif
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            /*�������ǰ����ģʽ֧��˫�򣬳ͷ���ǰ�� */
            if( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_JudgeImsDomainC1())
            {
                /* WIFI���Ѿ����Թ�,����ʧ��,��Ҫ������ */
                IMSA_Domain_SetWifiEnable(pstDomainManager->enImsaTryedDomain);
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;

                /* ����WIFI��ͷ���ʱ�� */
                /*lint -e734*/
                pstDomainManager->ucPunishTimeInWifiDomain = pstDomainManager->ucPunishTimeInWifiDomain * 2;
                IMSA_Domain_StartTimer1WifiDomain();
                /*lint +e734*/

                /* ��������򶼳��Թ�������ͣ���ԣ��ȴ��´δ��� */
                if( IMSA_IMS_DOMAIN_LTE_WIFI == pstDomainManager->enImsaTryedDomain )
                {
                    pstDomainManager->enImsaTryedDomain = IMSA_IMS_DOMAIN_NULL;
#if 0
                    /*�ϱ��������� */
                    if (IMSA_TRUE == IMSA_IsImsVoiceContidionSatisfied())
                    {
                        IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_AVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                    }
                    else
                    {
                        IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_UNAVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                    }
#endif
                }
                else
                {
                    /* ������ѡ����,���ݵ�ǰ״̬�ٴ�ѡ����ʵ��� */
                    IMSA_DomainSelect();
                }

            }
            else
            {
                pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;
#if 0
                /*�ϱ��������� */
                if (IMSA_TRUE == IMSA_IsImsVoiceContidionSatisfied())
                {
                    IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_AVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                }
                else
                {
                    IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_UNAVAILABLE,IMSA_CONN_IsExitPersistentBearer());
                }
#endif
            }
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            /* �л�ʧ��,�޷��ص�ԭ����,�ϱ�ʧ��,�޷��� */
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;
            pstDomainManager->enImsaTryedDomain = IMSA_IMS_DOMAIN_NULL;

            break;

        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            /* �л�ʧ��,�޷��ص�ԭ����,�ϱ�ʧ��,�޷��� */
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;
            pstDomainManager->enImsaTryedDomain = IMSA_IMS_DOMAIN_NULL;

            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            /* ��ѡ����Disable��ɹ� */
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;
#if 0
            /*�ϱ��������� */
            if (IMSA_TRUE == IMSA_IsImsVoiceContidionSatisfied())
            {
                IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_AVAILABLE,IMSA_CONN_IsExitPersistentBearer());
            }
            else
            {
                IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_UNAVAILABLE,IMSA_CONN_IsExitPersistentBearer());
            }
#endif
            break;

        default:
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;
            IMSA_WARN_LOG("IMSA_DomainSelectResultInNoSrv: No Srv Status is Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_DomainSelectResultInNoSrv_ENUM, LNAS_StateInvalid);
            break;
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_DomainSelectResultInNrmSrv()
 Description    : ����Normal SERVICE��Status IND����ѡ������
 Input          : IMSA_DOMAIN_SRV_STATUS_IND_STRU     *pstSrvStatusInd
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-12-15  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_DomainSelectResultInNrmSrv(IMSA_DOMAIN_SRV_STATUS_IND_STRU     *pstSrvStatusInd)
{
    IMSA_DOMAIN_MANAGER_STRU            *pstDomainManager = VOS_NULL;

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    TLPS_PRINT2LAYER_INFO(IMSA_DomainSelectResultInNrmSrv_ENUM, LNAS_ENTRY);

    /* ���Domain FSM L2�������ڳ���LTE IMS���״̬����ת��LTE Domain̬ */
    if( (IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING == pstDomainManager->enDomainFsmL2State)
        && (IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY == pstSrvStatusInd->enCurrentDomain))
    {
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN;

        /*��ʼ��LTE�ͷ���ʱ���ͷ�ʱ�� */
        IMSA_Domain_StopTimer1LteDomain();
    }
    /* ���Domain FSM L2�������ڳ���WIFI IMS���״̬����ת��WIFI Domain̬ */
    else if((IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING == pstDomainManager->enDomainFsmL2State)
        &&(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY == pstSrvStatusInd->enCurrentDomain))
    {
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN;

        /*��ʼ��WIFI�ͷ���ʱ���ͷ�ʱ�� */
        IMSA_Domain_StopTimer1WifiDomain();
    }
    /* ��� L2W�л��ɹ�,��ǰ����WIFI�� */
    else if((IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH == pstDomainManager->enDomainFsmL2State)
        &&(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY == pstSrvStatusInd->enCurrentDomain))
    {
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN;

        /*��ʼ��WIFI�ͷ���ʱ���ͷ�ʱ�� */
        IMSA_Domain_StopTimer1WifiDomain();
    }
    /* ��� L2W�л�ʧ��, ���ǳɹ�����ԭ����LTE�� */
    else if((IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH == pstDomainManager->enDomainFsmL2State)
        &&(IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY == pstSrvStatusInd->enCurrentDomain))
    {
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN;
    }
    /* ��� W2L�л��ɹ�,��ǰ����LTE�� */
    else if((IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH == pstDomainManager->enDomainFsmL2State)
        &&(IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY == pstSrvStatusInd->enCurrentDomain))
    {
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN;

        /*��ʼ��LTE�ͷ���ʱ���ͷ�ʱ�� */
        IMSA_Domain_StopTimer1LteDomain();
    }
    /* ��� W2L�л�ʧ��, ���ǳɹ�����ԭ����WIFI�� */
    else if((IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH == pstDomainManager->enDomainFsmL2State)
        &&(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY == pstSrvStatusInd->enCurrentDomain))
    {
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN;
    }
    /* L2״̬������NULL��Disabling״̬ʱ,�յ�״̬�ϱ������쳣,ת��NULL״̬,ͬʱ������ѡ���� */
    else if((IMSA_DOMAIN_FSM_L2_STATE_NULL == pstDomainManager->enDomainFsmL2State)
        || (IMSA_DOMAIN_FSM_L2_STATE_DISABLING == pstDomainManager->enDomainFsmL2State)
        || (IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING == pstDomainManager->enDomainFsmL2State)
        || (IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING == pstDomainManager->enDomainFsmL2State))
    {
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;

        /* ������ѡ����,���ݵ�ǰ״̬�ٴ�ѡ����ʵ��� */
        IMSA_DomainSelect();
    }
    else
    {
        IMSA_WARN_LOG("IMSA_DomainSelectResultInNrmSrv: Srv Status is Wrong!");
    }

#if 0
/* ע��״̬�µ�IMS VOPS�ϱ����ȴ�ȷ������ٴθ��� */
    if( ( IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN == pstDomainManager->enDomainFsmL2State)
        ||(IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN == pstDomainManager->enDomainFsmL2State))
    {
        /*�ϱ��������� */
        if (IMSA_TRUE == IMSA_IsImsVoiceContidionSatisfied())
        {
            IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_AVAILABLE,IMSA_CONN_IsExitPersistentBearer());
        }
        else
        {
            IMSA_SndMsccMsgImsVoiceCapNotify(MSCC_IMSA_IMS_VOICE_CAP_UNAVAILABLE,IMSA_CONN_IsExitPersistentBearer());
        }
    }
#endif
    return;
}



VOS_UINT32 IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi_ENUM, LNAS_ENTRY);

    /* ��ѡ�������:
        1>  ���VoWIFI_SERVICE_STATUSΪINITIAL
          ��ѡ����VoWIFI��
        1> ���VoWIFI_SERVICE_STATUSΪLTE_NORMAL
           ��ѡ����VoWIFI
        1> ���VoWIFI_SERVICE_STATUSΪLTE_LIMITED
           ��ѡ�񲻳���VoWIFI���Ѿ�פ��VoWIFI������IMSȥע�ᣬ�ͷ�VoWIFI PDN��
      */
    if( (IMSA_IMS_VOLTECTRLVOWIFI_INIT == pstImsaDomainManager->enImsaVolteCtrlVowifiStatus)
        || (IMSA_IMS_VOLTECTRLVOWIFI_NORMAL == pstImsaDomainManager->enImsaVolteCtrlVowifiStatus))
    {
        return VOS_TRUE;
    }
    else if( IMSA_IMS_VOLTECTRLVOWIFI_LIMITED == pstImsaDomainManager->enImsaVolteCtrlVowifiStatus)
    {
        return VOS_FALSE;
    }
    else
    {
        IMSA_WARN_LOG("IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi: VoLTECtrlVoWIFI ENUM Wrong!");

        TLPS_PRINT2LAYER_WARNING(IMSA_Domain_CheckWifiPossibleInVolteCtrlVowifi_ENUM, LNAS_StateInvalid);
        return VOS_TRUE;
    }
}


VOS_VOID IMSA_Domain_SetVolteCtrlVowifi
(
    const MSCC_IMSA_SERVICE_CHANGE_IND_STRU     *pstServiceChangeInd
)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_SetVolteCtrlVowifi is entered!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_SetVolteCtrlVowifi_ENUM, LNAS_ENTRY);

    /* �洢SERVICE CHANGE IND��Ϣ��Я���Ĳ��� */
    pstImsaDomainManager                    = IMSA_Domain_GetDomainManagerAddr();
    /* ����״̬��, VoWIFI��VoLTE�䲻���д������ */
    if ((VOS_TRUE == pstServiceChangeInd->bitRoamingValid)
         && (VOS_TRUE == pstServiceChangeInd->ucRoamingFlg))
    {
        pstImsaDomainManager->enImsaVolteCtrlVowifiStatus = IMSA_IMS_VOLTECTRLVOWIFI_INIT;
        return;
    }

    /* ���ѡ��"��֧��VoLTE�����粻����VoWifi"ΪTRUE, �����ָʾ����IMS�������ò��� */
    if(VOS_TRUE == IMSA_GetVolteCtrlVoWifiFlag())
    {
        if(IMSA_IMS_VOPS_STATUS_NOT_SUPPORT == IMSA_GetNwImsVoiceCap())
        {
            pstImsaDomainManager->enImsaVolteCtrlVowifiStatus = IMSA_IMS_VOLTECTRLVOWIFI_LIMITED;

            IMSA_INFO_LOG("IMSA_Domain_SetVolteCtrlVowifi:Limited!");
        }
        /* ���פ��LTEС��֧��ΪIMS */
        else
        {
            /* ����VoWIFI_SERVICE_STATUSΪLTE_NORMAL */
            pstImsaDomainManager->enImsaVolteCtrlVowifiStatus = IMSA_IMS_VOLTECTRLVOWIFI_NORMAL;

            IMSA_INFO_LOG("IMSA_Domain_SetVolteCtrlVowifi:Normal!");
        }
    }
    else
    {
        /*  ���ѡ��"��֧��VoLTE�����粻����VoWifi"ΪFALSE, ����VoWIFI_SERVICE_STATUSΪINITIAL */
        pstImsaDomainManager->enImsaVolteCtrlVowifiStatus = IMSA_IMS_VOLTECTRLVOWIFI_INIT;

        IMSA_INFO_LOG("IMSA_Domain_SetVolteCtrlVowifi: Init!");
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_SetImsDomainPreferFromSetMsg()
 Description    : ��MSCC IMS��������Ϣ��������������
 Input          : ָ������˳�� enImsDomain
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/


VOS_VOID IMSA_Domain_SetImsDomainPreferFromSetMsg( MSCC_IMSA_IMS_DOMAIN_ENUM_UINT32    enImsDomain )
{

    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_SetImsDomainPreferFromSetMsg_ENUM, LNAS_ENTRY);

    switch(enImsDomain)
    {
        case MSCC_IMSA_IMS_DOMAIN_WIFI_PREFEER:
            pstImsaDomainManager->enImsDomainPrefer = IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER;
            break;

        case MSCC_IMSA_IMS_DOMAIN_CELLULAR_PREFEER:
            pstImsaDomainManager->enImsDomainPrefer = IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER;
            break;

        case MSCC_IMSA_IMS_DOMAIN_WIFI_ONLY:
            pstImsaDomainManager->enImsDomainPrefer = IMSA_IMS_DOMAIN_PREFER_WIFI_ONLY;
            break;

        default:
            pstImsaDomainManager->enImsDomainPrefer = IMSA_IMS_DOMAIN_PREFER_CELLULAR_ONLY;
            IMSA_WARN_LOG("IMSA_Domain_SetImsDomainPreferFromSetMsg: Domain Prefer Set Para is Wrong!");
            break;
    }

    return;
}


/*****************************************************************************
 Function Name  : IMSA_Domain_SetDomainModeInWifiPowerOn()
 Description    : WIFI��������״̬Ǩ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_SetDomainModeInMsccImsDomainCfgSetReq(VOS_VOID)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstImsaDomainManager    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInMsccImsDomainCfgSetReq: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_SetDomainModeInMsccImsDomainCfgSetReq_ENUM, LNAS_ENTRY);


    pstImsaDomainManager = IMSA_Domain_GetDomainManagerAddr();

    /* ����IMS���������õ�ǰ��IMS��״̬ */
    switch(pstImsaDomainManager->enImsDomainPrefer)
    {
        case IMSA_IMS_DOMAIN_PREFER_WIFI_ONLY:
            if( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode())
            {
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY);
            }
            else if( VOS_TRUE == IMSA_Domain_IsWifiDomainExist())
            {
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY);
            }
            else
            {
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
            }
            break;

        case IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER:
        case IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER:
            if(VOS_TRUE == IMSA_Domain_IsDualDomainExist())
            {
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_DUAL);
            }
            else if(VOS_TRUE == IMSA_Domain_IsWifiDomainExist() )
            {
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY);
            }
            else if( VOS_TRUE == IMSA_Domain_IsCellularDomainExist())
            {
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY);
            }
            else
            {
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
            }
            break;

        case IMSA_IMS_DOMAIN_PREFER_CELLULAR_ONLY:
        default:
            IMSA_WARN_LOG("IMSA_Domain_SetImsDomainPreferFromSetMsg: Prefer Set Status Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_SetImsDomainPreferFromSetMsg_ENUM, LNAS_StateInvalid);
    }

    return IMSA_SUCC;
}




/*****************************************************************************
 Function Name  : IMSA_Domain_SetDomainModeInWifiPowerOn()
 Description    : WIFI��������״̬Ǩ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_SetDomainModeInWifiPowerOn(VOS_VOID)
{
    IMSA_CONTROL_MANAGER_STRU    *pstControlManager;

    IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_SetDomainModeInWifiPowerOn_ENUM, LNAS_ENTRY);

    pstControlManager = IMSA_GetControlManagerAddress();

    /* ����WIFI����״̬ */
    if( IMSA_STATUS_STARTING == pstControlManager->enImsaWifiStatus)
    {
        IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: Status is Starting!");
        return VOS_FALSE;
    }


    /* �����״̬��Ǩ�� */
    switch(IMSA_GetDomainMode())
    {
        case IMSA_IMS_DOMAIN_MODE_NULL:
            /*  ���������ģ��״̬��Ǩ��  */
            if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                  ||(IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer()))
                && (VOS_TRUE == IMSA_Domain_IsDualDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��˫��,��IMS���ȹ�ϵ֧��˫��, ����IMSA��״̬Ϊ Dual Domain */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_DUAL);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: 1_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
            }
            else if( ((IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer())
                      || (IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                      || (IMSA_IMS_DOMAIN_PREFER_WIFI_ONLY == IMSA_GetImsDomainPrefer()))
                     &&(VOS_TRUE == IMSA_Domain_IsWifiDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��WIFI��,��IMS���ȹ�ϵ֧��WIFI��,����IMSA��״̬Ϊ WIFI ONLY */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: 2_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");
            }
            else if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                       || (IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer())
                       || (IMSA_IMS_DOMAIN_PREFER_CELLULAR_ONLY == IMSA_GetImsDomainPrefer()))
                     &&(VOS_TRUE == IMSA_Domain_IsCellularDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��Cellular��,��IMS���ȹ�ϵ֧��Cellular��,����IMSA��״̬Ϊ Cellular ONLY */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: 3_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");
            }
            else
            {
                /* ����IMSA��״̬Ϊ NULL */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: 4_DomainMode is IMSA_IMS_DOMAIN_MODE_NULL!");
            }
            break;

        case IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY:
            /*  ���������ģ��״̬��Ǩ��  */
            if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                  ||(IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer()))
                && (VOS_TRUE == IMSA_Domain_IsDualDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��˫��,��IMS���ȹ�ϵ֧��˫��, ����IMSA��״̬Ϊ Dual Domain */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_DUAL);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: 5_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
            }
            break;

        case IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:
            /* Wifi����״̬�յ�Wifi������Ϣ���澯 */
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: Multi Wifi Power On Message Received!");

            /* ���˫��֧��,Ǩ�Ƶ�˫��״̬ */
            if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                  ||(IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer()))
                && (VOS_TRUE == IMSA_Domain_IsDualDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��˫��,��IMS���ȹ�ϵ֧��˫��, ����IMSA��״̬Ϊ Dual Domain */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_DUAL);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: 6_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
            }
            break;

        case IMSA_IMS_DOMAIN_MODE_DUAL:
            /* Wifi����״̬�յ�Wifi������Ϣ���澯 */
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: Multi Wifi Power On Message Received!");
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInWifiPowerOn: Status is Starting!");
            return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_SetDomainModeInWifiPowerOff()
 Description    : WIFI��������״̬Ǩ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-12-15  Draft Enact
*****************************************************************************/

void IMSA_Domain_SetDomainModeInWifiPowerOff(void)
{

    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInWifiPowerOff: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_SetDomainModeInWifiPowerOff_ENUM, LNAS_ENTRY);

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();


    /* �����״̬��Ǩ�� */
    switch(IMSA_GetDomainMode())
    {
        case IMSA_IMS_DOMAIN_MODE_NULL:
            /* NULL״̬��,�ܵ�WIFI�ػ�����,�����κδ��� */
            break;

        case IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY:
            /* Cellular״̬��,�ܵ�WIFI�ػ�����,�����κδ��� */
            break;

        case IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:
            /* ����IMSA��״̬ΪNULL */
            IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);

            /* ���֧��WIFI�� */
            IMSA_Domain_SetWifiDisable(pstDomainManager->enImsaAvailDomain);

            break;

        case IMSA_IMS_DOMAIN_MODE_DUAL:
            /* ����IMSA��״̬ΪCellular Only */
            IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY);

            /* ���֧��WIFI�� */
            IMSA_Domain_SetWifiDisable(pstDomainManager->enImsaAvailDomain);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInWifiPowerOff: Domain Status is Wrong!");

            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_SetDomainModeInWifiPowerOff_ENUM, LNAS_StateInvalid);
            break;
    }

    return;
}



/*****************************************************************************
 Function Name  : IMSA_Domain_SetDomainModeInCellularPowerOn()
 Description    : WIFI��������״̬Ǩ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_SetDomainModeInCellularPowerOn(VOS_VOID)
{
    IMSA_CONTROL_MANAGER_STRU  *pstControlManager;

    pstControlManager = IMSA_GetControlManagerAddress();


    IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_SetDomainModeInCellularPowerOn_ENUM, LNAS_ENTRY);

    /* ����WIFI����״̬ */
    if( IMSA_STATUS_STARTING == pstControlManager->enImsaCellularStatus)
    {
        IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: Status is Starting!");
        return VOS_FALSE;
    }

    switch(IMSA_GetDomainMode())
    {
        case IMSA_IMS_DOMAIN_MODE_NULL:
            /*  ���������ģ��״̬��Ǩ��  */
            if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                  ||(IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer()))
                && (VOS_TRUE == IMSA_Domain_IsDualDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��˫��,��IMS���ȹ�ϵ֧��˫��, ����IMSA��״̬Ϊ Dual Domain */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_DUAL);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 1_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
            }
            else if( ((IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer())
                      || (IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                      || (IMSA_IMS_DOMAIN_PREFER_WIFI_ONLY == IMSA_GetImsDomainPrefer()))
                     &&(VOS_TRUE == IMSA_Domain_IsWifiDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��WIFI��,��IMS���ȹ�ϵ֧��WIFI��,����IMSA��״̬Ϊ WIFI ONLY */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 2_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");
            }
            else if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                       || (IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer())
                       || (IMSA_IMS_DOMAIN_PREFER_CELLULAR_ONLY == IMSA_GetImsDomainPrefer()))
                     &&(VOS_TRUE == IMSA_Domain_IsCellularDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��Cellular��,��IMS���ȹ�ϵ֧��Cellular��,����IMSA��״̬Ϊ Cellular ONLY */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 3_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");
            }
            else
            {
                /* ����IMSA��״̬Ϊ NULL */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 4_DomainMode is IMSA_IMS_DOMAIN_MODE_NULL!");
            }
            break;

        case IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY:
            /* ����Cellular Onlyʱ���Ѿ����յ�����������Ϣ��������IMS�������� */
            /* �����ر�Cellular����Ҫ������裬��ʱ��֧����������״̬�£����յ�IMS��̬���ش򿪣���ΪCellular�����رյĳ���*/
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 5_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");
            break;

        case IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:
            /* ���յ���������������¼Cellular����״̬��ͬʱ���������Ǩ�Ƶ�DualMode״̬ */
            if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
                  ||(IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer()))
                && (VOS_TRUE == IMSA_Domain_IsDualDomainExist()))
            {
                /* ���NV�Ϳ���״̬֧��˫��,��IMS���ȹ�ϵ֧��˫��, ����IMSA��״̬Ϊ Dual Domain */
                IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_DUAL);
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 6_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
            }
            else
            {
                IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 6_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");
            }
            break;

        case IMSA_IMS_DOMAIN_MODE_DUAL:
            /* ��ʱ������ */
            IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: 7_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInCellularPowerOn: Status is Starting!");
            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_SetDomainModeInCellularPowerOn_ENUM, IMSA_PRINT_STATUS_STARTING);
            return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_SetDomainModeInWifiPowerOff()
 Description    : WIFI��������״̬Ǩ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-12-15  Draft Enact
*****************************************************************************/

void IMSA_Domain_SetDomainModeInCellularPowerOff(void)
{

    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInCellularPowerOff: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_SetDomainModeInCellularPowerOff_ENUM, LNAS_ENTRY);
    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();


    switch(IMSA_GetDomainMode())
    {
        case IMSA_IMS_DOMAIN_MODE_NULL:
            break;

        case IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY:

            IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
            /* ���֧��Cellular�� */
            IMSA_Domain_SetLteDisable(pstDomainManager->enImsaAvailDomain);
            break;

        case IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:
            break;

        case IMSA_IMS_DOMAIN_MODE_DUAL:
            /* ����IMSA��״̬ΪWIFI Only */
            IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY);

            /* ���֧��Cellular�� */
            IMSA_Domain_SetLteDisable(pstDomainManager->enImsaAvailDomain);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInCellularPowerOff: Domain Status is Wrong!");
    }

    return;
}


/*****************************************************************************
 Function Name  : IMSA_Domain_SetDomainModeInDeregReq()
 Description    : Dereg req to change Domain Cap
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2016-03-28  Draft Enact
*****************************************************************************/

void IMSA_Domain_SetDomainModeInDeregReq(void)
{
    IMSA_INFO_LOG("IMSA_Domain_SetDomainModeInDeregReq: ENTER!");

    switch(IMSA_GetDomainMode())
    {
        case IMSA_IMS_DOMAIN_MODE_NULL:
            break;

        case IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY:

            IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
            break;

        case IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:
            IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);

            break;

        case IMSA_IMS_DOMAIN_MODE_DUAL:
            IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_SetDomainModeInDeregReq: Domain Status is Wrong!");
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_InitD2ImsStack()
 Description    : D2 IMS Stack��ʼ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Domain_InitD2ImsStack(VOS_VOID)
{
    IMSA_CONTROL_MANAGER_STRU  *pstControlManager;
    static VOS_UINT8            ucD2ImsTaskStart = 0;

    pstControlManager = IMSA_GetControlManagerAddress();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_InitD2ImsStack_ENUM, LNAS_ENTRY);

    if(ucD2ImsTaskStart == 0)
    {
        /*��ʼ��D2Э��ջ*/
        (VOS_VOID)IMSA_ImsInit();
        ucD2ImsTaskStart ++;
    }


    /*�жϿ���״̬�����û�п��������ʼ��NVֵ */
    if(pstControlManager->enImsaStatus == IMSA_STATUS_NULL)
    {
        /*��ȡNV*/
        IMSA_ReadImsaNV();
    }

    return;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_StartImsStack()
 Description    : IMS��������״̬Ǩ��
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Domain_StartImsStack(VOS_VOID)
{
    IMSA_CONTROL_MANAGER_STRU    *pstControlManager;

    IMSA_INFO_LOG("IMSA_Domain_StartImsStack: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_StartImsStack_ENUM, LNAS_ENTRY);

    pstControlManager = IMSA_GetControlManagerAddress();

    /*�Ѵ��ڿ�������״̬������*/
    if(IMSA_STATUS_STARTING == pstControlManager->enImsaStatus)
    {
        IMSA_WARN_LOG("IMSA_Domain_StartImsStack: Status is Starting!");
        TLPS_PRINT2LAYER_WARNING(IMSA_ProcMsccMsgStartReq_ENUM, IMSA_PRINT_STATUS_STARTING);
        return ;
    }

    IMSA_StopTimer(&pstControlManager->stProtectTimer);
    /* ��ʼ�� */

    /*֪ͨIMS������ת״̬*/
    pstControlManager->enImsaStatus = IMSA_STATUS_STARTING;

    IMSA_SndImsMsgStartReq();

    IMSA_StartTimer(&pstControlManager->stProtectTimer);

    /* ����λ�����ȡ����Ϣ */
    if(IMSA_ISIM_STATUS_AVAILABLE == pstControlManager->enImsaIsimStatus)
    {
        IMSA_INFO_LOG("IMSA_Domain_StartImsStack: ISIM is available!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_StartImsStack_ENUM, 2);
        IMSA_ReadIsimFile();
    }

    return;

}

/*****************************************************************************
 Function Name  : IMSA_Domain_StopImsStack()
 Description    : ֹͣIMS SDK
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-11-19  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Domain_StopImsStack(VOS_VOID)
{
    IMSA_CONTROL_MANAGER_STRU *pstControlManager;

    pstControlManager = IMSA_GetControlManagerAddress();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_StopImsStack_ENUM, LNAS_ENTRY);

    /*ֹͣ���ػ�������ʱ��*/
    IMSA_StopTimer(&pstControlManager->stProtectTimer);

    /*֪ͨIMS�ػ���ת״̬*/
    pstControlManager->enImsaStatus = IMSA_STATUS_STOPING;

    /*�ػ��󣬿���λʱ����Ҫ����״̬��Ϊ��Ч����Ϊ�����п���Ч�ĳ�������Ҫ�ػ������״̬*/
    if( IMSA_ISIM_STATUS_ABSENT != pstControlManager->enImsaIsimStatus )
    {
        pstControlManager->enImsaIsimStatus = IMSA_ISIM_STATUS_AVAILABLE;
    }
    /*�������ػ�������ʱ��*/
    IMSA_StartTimer(&pstControlManager->stProtectTimer);

    /* �����ǰ״̬ʱIDLE+DEREG����ֱ��֪ͨIMS�ػ�������Ҫ������Դ */
#if (FEATURE_ON == FEATURE_DSDS)
    if (PS_RRM_RF_SHARED == IMSA_Dsds_RRM_RfResourceIsShared())
    {
        if (IMSA_TRUE == IMSA_SRV_IsNotNeedNwDereg())
        {
            IMSA_SndImsMsgStopReq();
            return;
        }
    }
#endif
    /* ��Ҫ���ݵ�ǰ��ע��״̬���ж��Ƿ���Ҫ������Դ */
    switch (IMSA_DsdsGetResourceStatus(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER))
    {
        case IMSA_RESOURCE_STATUS_ACTIVE:

            IMSA_SndImsMsgStopReq();
        #if (FEATURE_ON == FEATURE_DSDS)
            if (PS_RRM_RF_SHARED == IMSA_Dsds_RRM_RfResourceIsShared())
            {
                IMSA_DsdsModifyTaskSrcByTaskType(IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                                 RRM_PS_TASK_TYPE_LTE_IMS_REGISTER);
            }
        #endif
            break;
    #if (FEATURE_ON == FEATURE_DSDS)
        case IMSA_RESOURCE_STATUS_APPLY_NOT_NEED_WAIT_CNF:

            IMSA_DsdsProcResourceApplyReq(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER,
                                          IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                          VOS_FALSE,
                                          VOS_FALSE);
            IMSA_SndImsMsgStopReq();
            break;

        case  IMSA_RESOURCE_STATUS_NULL:

            IMSA_DsdsProcResourceApplyReq(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER,
                                          IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                          VOS_FALSE,
                                          VOS_FALSE);

            //IMSA_DsdsSaveMoSmsMsg(pstRpDataReq);
            break;

        case IMSA_RESOURCE_STATUS_APPLYING:
            /* �����ǰ״̬Ϊpending����ȡ�������ע�ᣬȻ��֪ͨIMS����ȥע�ᣬȻ��ֱ�ӹػ� */
            IMSA_DsdsModifyTaskSrcByTaskType(IMSA_RF_RES_APPLY_TRIGER_SRC_POWER_OFF,
                                             RRM_PS_TASK_TYPE_LTE_IMS_REGISTER);
            break;

        case IMSA_RESOURCE_STATUS_WAIT_STATUS_IND:

            IMSA_DsdsProcResourceRelease(RRM_PS_TASK_TYPE_LTE_IMS_REGISTER);

            IMSA_SRV_ProcStopMsgWhenNoRf();

            IMSA_SndImsMsgStopReq();
            break;
    #endif
        default:
            break;
    }

//    IMSA_SndImsMsgStopReq();


    return;
}


/*****************************************************************************
 Function Name  : IMSA_Domain_ProcWifiPOwerOnReq()
 Description    : WIFI������Ϣ������
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcWifiPowerOnReq( const VOS_VOID                     *pRcvMsg  )
{
    IMSA_CONTROL_MANAGER_STRU    *pstControlManager;
    IMSA_DOMAIN_MANAGER_STRU     *pstDomainManager;


    IMSA_INFO_LOG("IMSA_Domain_ProcWifiPowerOnReq: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcWifiPowerOnReq_ENUM, LNAS_ENTRY);

    pstControlManager = IMSA_GetControlManagerAddress();
    pstDomainManager    = IMSA_Domain_GetDomainManagerAddr();

    /* ��ʼ��D2Э��ջ��IMS NVֵ */
    IMSA_Domain_InitD2ImsStack();

    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq begin with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq begin with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq begin with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq begin with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq begin with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq begin with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq begin with state: enImsaStatus = ",pstControlManager->enImsaStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq: enImsaStatus = ",pstControlManager->enImsaStatus);

    /*�жϿ���״̬������ѿ�������ظ������ɹ�*/
    if(IMSA_STATUS_STARTED == pstControlManager->enImsaWifiStatus)
    {
        /* ָʾSPM WIFI�����ɹ� */
        IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_ON);

        /* �ظ�WIFI�����ɹ� */
        IMSA_SndWifiPowerOnCnf();

        /* ָʾ������״̬Ǩ�� */
        (VOS_VOID)IMSA_Domain_SetDomainModeInWifiPowerOn();

        /* ֪ͨERRC�����ϱ�LTE�ź����� */
        if( (VOS_FALSE == pstDomainManager->ucLteQualityCfgFlag)
            && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
        {
            IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
            pstDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
        }

        return VOS_FALSE;
    }


    if((IMSA_STATUS_STARTING == pstControlManager->enImsaStatus)
        || (IMSA_STATUS_NULL == pstControlManager->enImsaStatus))
    {
        /* ����WIFI����״̬Ϊ������ */
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_STARTING;

        /* ����IMSЭ��ջ */
        IMSA_Domain_StartImsStack();

        /* IMS�����ڲ���Ϊ,��������IMS,�������������־ */
        if( VOS_TRUE == pstControlManager->ucInternalImsStackStartFlag)
        {
            /* IMSA�ڲ�������Ϊ,����Ҫ�ظ������ɹ���Ϣ */
            pstControlManager->ucInternalImsStackStartFlag = VOS_FALSE;
        }
    }
    else if(IMSA_STATUS_STARTED == pstControlManager->enImsaStatus)
    {
        /* ����WIFI����״̬Ϊ�Ѿ����� */
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_STARTED;

        /* ָʾSPM WIFI�����ɹ� */
        IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_ON);

        /* �ظ�WIFI�����ɹ� */
        IMSA_SndWifiPowerOnCnf();
    }

    /* ָʾ������״̬Ǩ�� */
    (VOS_VOID)IMSA_Domain_SetDomainModeInWifiPowerOn();

    /* ֪ͨERRC�����ϱ�LTE�ź����� */
    if( (VOS_FALSE == pstDomainManager->ucLteQualityCfgFlag)
        && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
    {
        IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
        pstDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
    }

    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq end with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq end with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq end with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq end with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq end with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq end with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq end with state: enImsaStatus = ",pstControlManager->enImsaStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOnReq: enImsaStatus = ",pstControlManager->enImsaStatus);


    return VOS_FALSE;
}





/*****************************************************************************
 Function Name  : IMSA_Domain_ProcWifiPowerOffReq()
 Description    : WIFI�ػ���Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcWifiPowerOffReq
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_CONTROL_MANAGER_STRU          *pstControlManager;
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager = VOS_NULL;

    pstControlManager   = IMSA_GetControlManagerAddress();
    pstDomainManager    = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcWifiPowerOffReq_ENUM, LNAS_ENTRY);

    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq begin with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq begin with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq begin with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq begin with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq begin with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq begin with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq begin with state: enImsaStatus = ",pstControlManager->enImsaStatus);

    /* ������Ǩ�� */
    IMSA_Domain_SetDomainModeInWifiPowerOff();

    /* ���WIFIû�п�����ֱ�ӻظ�Wifi�ػ��ɹ� */
    if(IMSA_STATUS_NULL == pstControlManager->enImsaWifiStatus)
    {
        /* ����WIFI�ػ�״̬ */
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_NULL;

        /* �ظ�WIFI�ػ��ɹ� */
        IMSA_SndWifiPowerOffCnf();
        return VOS_FALSE;
    }

    /* ��������������ػ�״̬,��ֱ�ӷ���WIFI�ػ��ɹ� */
    if(IMSA_STATUS_STOPING == pstControlManager->enImsaStatus)
    {
        return VOS_FALSE;
    }

    /* ������ѡ��ʼ���������� */
    IMSA_Domain_InitDomainOpVar(WUEPS_PID_MAPCON,ID_WIFI_IMSA_POWEROFF_REQ,VOS_FALSE,VOS_FALSE);

    /* ������ѡ����, ���ݵ�ǰ״̬ѡ����ʵĺ�ѡ�� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:

#ifdef VOLTE_VOWIFI_CTRL_ONLY
            if(IMSA_IMS_DOMAIN_NULL == IMSA_GetDomainMode())
            {
                /*ֻ����WIFI, �ر��Ժ���ҪͣIMS SDK */
                IMSA_Domain_StopImsStack();
                /* ����WIFI�ػ���״̬ */
                pstControlManager->enImsaWifiStatus = IMSA_STATUS_STOPING;
                return VOS_FALSE;
            }
#endif

            IMSA_ProcessNoDomainSelectInWifiPowerOff();
            break;

        case IMSA_IMS_DOMAIN_WIFI:
            IMSA_WARN_LOG("IMSA_Domain_ProcWifiPowerOffReq: DomainC1Judge Wrong1!");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcWifiPowerOffReq_ENUM, LNAS_FUNCTION_LABEL1);
            break;

        case IMSA_IMS_DOMAIN_LTE:
            IMSA_ProcessLTEOnlyDomainSelectInWifiPowerOff();
            break;

        case IMSA_IMS_DOMAIN_LTE_WIFI:
            IMSA_WARN_LOG("IMSA_Domain_ProcWifiPowerOffReq: DomainC1Judge Wrong2!");

            TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcWifiPowerOffReq_ENUM, LNAS_FUNCTION_LABEL2);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_ProcWifiPowerOffReq: DomainC1Judge Wrong3!");

            TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcWifiPowerOffReq_ENUM, LNAS_FUNCTION_LABEL3);
            break;
    }


    /* WIFI�ػ�������ӳٴ���,�����ӳ�,��澯 */
    if( VOS_TRUE == pstDomainManager->ucMsgDelayProcessFlag)
    {
        // �澯�������κδ���
        IMSA_WARN_LOG("IMSA_Domain_ProcWifiPowerOffReq: Poweroff Message can't delay by SRV!");
    }
    else
    {
    }

    /* ֪ͨERRCֹͣ�ϱ�LTE�ź����� */
    if( VOS_TRUE == pstDomainManager->ucLteQualityCfgFlag)
    {
        IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_STOP);
        pstDomainManager->ucLteQualityCfgFlag = VOS_FALSE;
    }
    else
    {
    }

    /* WIFI�ػ�������ѡ����,����Ҫ�ȴ���ѡ��ɲŻظ��ػ�ȷ�� */
    if( VOS_TRUE == pstDomainManager->ucDomainSelectFlag)
    {
        /* ����WIFI�ػ���״̬ */
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_STOPING;
        return VOS_FALSE;
    }
    else
    {
    }

    /* ����WIFI�ػ�״̬ */
    pstControlManager->enImsaWifiStatus = IMSA_STATUS_NULL;

    /* ָʾSPM WIFI�ػ��ɹ� */
//    IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_OFF);

    /* �ظ�WIFI�ػ��ɹ� */
    IMSA_SndWifiPowerOffCnf();

    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq end with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq end with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq end with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq end with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq end with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq end with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiPowerOffReq end with state: enImsaStatus = ",pstControlManager->enImsaStatus);

    return VOS_FALSE;
}





/*****************************************************************************
 Function Name  : IMSA_Domain_ProcWifiStateInd()
 Description    : Wifi State Ind��Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcWifiStateInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;
    AT_IMSA_IMS_CTRL_MSG_STRU          *pAtWifiImsaCtrlMsg  = VOS_NULL_PTR;
    WIFI_IMSA_STATE_IND_STRU           *pWifiStateInd       = VOS_NULL_PTR;
    IMSA_CONTROL_MANAGER_STRU          *pstControlManager   = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_ProcWifiStateInd: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcWifiStateInd_ENUM, LNAS_ENTRY);

    pAtWifiImsaCtrlMsg = (AT_IMSA_IMS_CTRL_MSG_STRU *)pRcvMsg;
    pWifiStateInd = (WIFI_IMSA_STATE_IND_STRU *) (pAtWifiImsaCtrlMsg->aucWifiMsg);
    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();
    pstControlManager = IMSA_GetControlManagerAddress();

    /* ���õ�ǰWIFI����� */
    IMSA_Domain_SetWifiEnable(pstDomainManager->enImsaAvailDomain);

    /* ����WIFI�ź�ǿ�� */
    if(WIFI_IMSA_QUALITY_LEVEL_BUTT > pWifiStateInd->enWifiQuality)
    {
        pstDomainManager->enWifiQuality = pWifiStateInd->enWifiQuality;
    }
    else
    {
        pstDomainManager->enWifiQuality = IMSA_WIRELESS_QUALITY_LEVEL_BAD;
    }

    /* ����AP MAC��ַ */
    IMSA_SRV_SaveWifiStateInd(pWifiStateInd);

    /* NULL״̬�£��յ�State_ind��Ϣ,����������Ϣ���д���  */
    if( IMSA_IMS_DOMAIN_MODE_NULL == IMSA_GetDomainMode() )
    {
        /* ��ʼ��D2Э��ջ��IMS NVֵ */
        IMSA_Domain_InitD2ImsStack();

        /* ����IMSЭ��ջ */
        if(IMSA_STATUS_NULL == pstControlManager->enImsaStatus)
        {
            IMSA_Domain_StartImsStack();

            /* ����IMS Stack�����Ƕ������� */
            pstControlManager->ucInternalImsStackStartFlag = VOS_TRUE;
        }

        if( IMSA_STATUS_NULL == pstControlManager->enImsaWifiStatus)
        {
            /* ����WIFI�Ѿ����� */
            pstControlManager->enImsaWifiStatus = IMSA_STATUS_STARTED;
        }

        /* IMS��״̬ת�� */
        (VOS_VOID)IMSA_Domain_SetDomainModeInWifiPowerOn();

        /* ֪ͨERRC�����ϱ�LTE�ź����� */
        if( (VOS_FALSE == pstDomainManager->ucLteQualityCfgFlag)
            && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
        {
            IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
            pstDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
        }

        /* ָʾSPM WIFI�����ɹ� */
        IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_ON);
    }
    else if( IMSA_STATUS_NULL == pstControlManager->enImsaWifiStatus)
    {
        /* ����WIFI�Ѿ����� */
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_STARTED;

        /* IMS��״̬ת�� */
        (VOS_VOID)IMSA_Domain_SetDomainModeInWifiPowerOn();

        /* ֪ͨERRC�����ϱ�LTE�ź����� */
        if( (VOS_FALSE == pstDomainManager->ucLteQualityCfgFlag)
            && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
        {
            IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
            pstDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
        }

        /* ָʾSPM WIFI�����ɹ� */
        IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_ON);

        /* ������ѡ�� */
        IMSA_Domain_DomainSelectInWifiStateInd();
    }
    else
    {
        /* ������ѡ�� */
        IMSA_Domain_DomainSelectInWifiStateInd();
    }


    IMSA_INFO_LOG1("IMSA_Domain_ProcWifiStateInd: domain = ",IMSA_GetDomainMode());
    return VOS_FALSE;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_ProcMsccStartReq()
 Description    : MSCC������Ϣ������
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_UINT32  VOS_TRUE/VOS_FALSE

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcMsccStartReq( const VOS_VOID                     *pRcvMsg )
{
    IMSA_CONTROL_MANAGER_STRU   *pstControlManager;
    IMSA_DOMAIN_MANAGER_STRU    *pstImsaDomainManager = VOS_NULL_PTR;

    pstControlManager = IMSA_GetControlManagerAddress();
    pstImsaDomainManager = IMSA_Domain_GetDomainManagerAddr();

    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccStartReq_ENUM, LNAS_ENTRY);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq start with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq start with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq start with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq start with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq start with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq start with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq start with state: enImsaStatus = ",pstControlManager->enImsaStatus);

    /* ��ʼ��D2Э��ջ��IMS NVֵ */
    IMSA_Domain_InitD2ImsStack();

    /* �жϿ���״̬������ѿ�������ظ������ɹ� */
    if(IMSA_STATUS_STARTED == pstControlManager->enImsaStatus)
    {
        /* ����Cellular����״̬ */
        pstControlManager->enImsaCellularStatus = IMSA_STATUS_STARTED;

        /* IMS��״̬ת�� */
        (VOS_VOID)IMSA_Domain_SetDomainModeInCellularPowerOn();

        /* ֪ͨERRC�����ϱ�LTE�ź����� */
        if( (VOS_FALSE == pstImsaDomainManager->ucLteQualityCfgFlag)
            && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
        {
            IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
            pstImsaDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
        }

        /* �ظ������ɹ� */
        IMSA_SndMsccMsgStartCnf();
        return VOS_FALSE;
    }

    /* ����Cellular����״̬ */
    if(  IMSA_STATUS_NULL == pstControlManager->enImsaCellularStatus)
    {
        pstControlManager->enImsaCellularStatus = IMSA_STATUS_STARTING;
    }

    /* IMS�����ڲ���Ϊ,��������IMS,�������������־ */
    if( VOS_TRUE == pstControlManager->ucInternalImsStackStartFlag)
    {
        /* IMSA�ڲ�������Ϊ,����Ҫ�ظ������ɹ���Ϣ */
        pstControlManager->ucInternalImsStackStartFlag = VOS_FALSE;
    }

    /* ����IMSЭ��ջ */
    IMSA_Domain_StartImsStack();

    /* IMS��״̬ת�� */
    (void)IMSA_Domain_SetDomainModeInCellularPowerOn();

    /* ֪ͨERRC�����ϱ�LTE�ź����� */
    if( (VOS_FALSE == pstImsaDomainManager->ucLteQualityCfgFlag)
        && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
    {
        IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
        pstImsaDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
    }

    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq end with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq end with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq end with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq end with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq end with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq end with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStartReq end with state: enImsaStatus = ",pstControlManager->enImsaStatus);

    return VOS_FALSE;
}


VOS_UINT32 IMSA_Domain_ProcMsccStopReq
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_CONTROL_MANAGER_STRU *pstControlManager;

    IMSA_INFO_LOG("IMSA_ProcMsccMsgStopReq: enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccStopReq_ENUM, LNAS_ENTRY);

    pstControlManager = IMSA_GetControlManagerAddress();

    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq begin with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq begin with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq begin with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq begin with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq begin with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq begin with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq begin with state: enImsaStatus = ",pstControlManager->enImsaStatus);

    /* SRVCC�쳣(����ػ���DEREG REQ��״̬Ǩ��CONN+REG)���建�� */
    IMSA_SrvccAbormalClearBuff(IMSA_SRVCC_ABNORMAL_STOP_REQ);

    /* ����������ػ�����¼Cellular�ػ�״̬ */
    /* WIFI�����ڹػ�״̬, ����Ҫ���WIFI��־,ͬʱΪ�����ͻ,����CellularΪ�ػ��� */
    if( IMSA_STATUS_STOPING == pstControlManager->enImsaWifiStatus)
    {
        pstControlManager->enImsaCellularStatus = IMSA_STATUS_STOPING;
    }
    else
    {
        pstControlManager->enImsaCellularStatus = IMSA_STATUS_NULL;
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_NULL;
    }


    /*�ж�״̬������ѹػ�����ظ��ػ��ɹ�*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_NULL)
    {
        IMSA_SndMsccMsgStopCnf();
        return VOS_FALSE;
    }

    /*�Ѵ��ڹػ�״̬������*/
    if(IMSA_STATUS_STOPING == pstControlManager->enImsaStatus)
    {
        IMSA_WARN_LOG("IMSA_Domain_ProcMsccStopReq: Status is Stoping!");
        TLPS_PRINT2LAYER_WARNING(IMSA_Domain_ProcMsccStopReq_ENUM, IMSA_PRINT_STATUS_STOPING);
        return VOS_FALSE;
    }


    /* ����������ػ�����ֱ��ֹͣIMS SDK */

    IMSA_Domain_StopImsStack();

    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq end with state: DomainMode = ",IMSA_GetDomainMode());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq end with state: enDomainPrefer = ",IMSA_GetImsDomainPrefer());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq end with state: cellular support = ",IMSA_GetCellularImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq end with state: wifi support = ",IMSA_GetWifiImsSupportFlag());
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq end with state: enImsaCellularStatus = ",pstControlManager->enImsaCellularStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq end with state: enImsawifiStatus = ",pstControlManager->enImsaWifiStatus);
    IMSA_INFO_LOG1("IMSA_Domain_ProcMsccStopReq end with state: enImsaStatus = ",pstControlManager->enImsaStatus);
    return VOS_FALSE;

}


/*****************************************************************************
 Function Name  : IMSA_Domain_ProcMsccImsDomainCfgSetReq()
 Description    : MSCC IMS������������Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcMsccImsDomainCfgSetReq
(
    const VOS_VOID                     *pRcvMsg
)
{
    MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ_STRU   *pstMsccImsaImsDomainCfgSetReq = VOS_NULL_PTR;
    IMSA_DOMAIN_MANAGER_STRU                *pstImsaDomainManager = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_ProcMsccImsDomainCfgSetReq:enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccImsDomainCfgSetReq_ENUM, LNAS_ENTRY);

    pstMsccImsaImsDomainCfgSetReq = (MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ_STRU*) pRcvMsg;
    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();

    /* ����Domain������������������ */
    IMSA_Domain_SetImsDomainPreferFromSetMsg(pstMsccImsaImsDomainCfgSetReq->enImsDomain);

    /* ����Domain��������Ǩ */
    (VOS_VOID)IMSA_Domain_SetDomainModeInMsccImsDomainCfgSetReq();

    /* ֪ͨERRC�����ϱ�LTE�ź����� */
    if( (VOS_FALSE == pstImsaDomainManager->ucLteQualityCfgFlag)
        && ( IMSA_IMS_DOMAIN_MODE_DUAL == IMSA_GetDomainMode()))
    {
        IMSA_SendRrcQualityConfigReq(IMSA_LRRC_CFG_QUALITY_START);
        pstImsaDomainManager->ucLteQualityCfgFlag = VOS_TRUE;
    }

    /* �ظ����óɹ���Ϣ */
    IMSA_SndMsccImsDomainCfgSetCnf();

    /* ������ѡ��ʼ���������� */
    IMSA_Domain_InitDomainOpVar(UEPS_PID_MSCC,ID_MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ,VOS_TRUE,VOS_TRUE);

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:
            IMSA_ProcessNoDomainSelectInMsccImsDomainCfgSetReq(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_WIFI:
            IMSA_ProcessWifiOnlyDomainSelectInMsccImsDomainCfgSetReq(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_LTE:
            IMSA_ProcessLTEOnlyDomainSelectInMsccImsDomainCfgSetReq(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_LTE_WIFI:
            IMSA_ProcessDualDomainSelect(VOS_TRUE,VOS_TRUE);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_ProcMsccImsDomainCfgSetReq: DomainC1Judge Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_ProcMsccImsDomainCfgSetReq_ENUM, LNAS_StateInvalid);
    }

    /* �����Ҫ�ӳٴ�����Ϣ,��¼�¼�,�ȴ�SRV�ϱ�״̬,Ȼ������������ѡ���� */
    if(VOS_TRUE == pstImsaDomainManager->ucMsgDelayProcessFlag)
    {
       /* ���浱ǰ��Ҫ�ӳٴ������ѡ���¼� */
       IMSA_Domain_SaveDelaySwitchEvent(UEPS_PID_MSCC,ID_MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ);

       /* ��SRVģ��ע��״̬�ϱ��¼�, SRV������̬���ϱ�Ready_ind��Ϣ,�����µ���ѡ���¼� */
       IMSA_SRV_RegStatusNtfEvent();

       /* ���Delay�¼� */
       pstImsaDomainManager->ucMsgDelayProcessFlag = VOS_FALSE;
    }

    return VOS_FALSE;
}



/*****************************************************************************
 Function Name  : IMSA_ProcMsccServiceChangeIndNoRsp()
 Description    : MSCC Service Change Ind��Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcMsccServiceChangeInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    MSCC_IMSA_SERVICE_CHANGE_IND_STRU  *pstMsccServiceChangeInd = VOS_NULL_PTR;
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager = VOS_NULL_PTR;
    IMSA_CONTROL_MANAGER_STRU          *pstControlManager = VOS_NULL_PTR;
    MSCC_IMSA_IMS_VOICE_CAP_ENUM_UINT8  enImsVoiceCap = MSCC_IMSA_IMS_VOICE_CAP_BUTT;

    pstDomainManager    = IMSA_Domain_GetDomainManagerAddr();
    pstControlManager   = IMSA_GetControlManagerAddress();
    pstMsccServiceChangeInd = (MSCC_IMSA_SERVICE_CHANGE_IND_STRU*)pRcvMsg;
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccServiceChangeInd_ENUM, LNAS_ENTRY);


    /* ����ǰ����ΪLTE����PS״̬������������֧��VOPS ҵ�� ��ȷ��֧��LTE */
    if( VOS_TRUE == IMSA_Domain_IsNrmSrvConditonSatisfied(pstMsccServiceChangeInd))
    {
        /* ����֧��LTE�� */
        IMSA_Domain_SetLteEnable(pstDomainManager->enImsaAvailDomain);
    }
    else
    {
        /* �������֧��LTE�� */
        IMSA_Domain_SetLteDisable(pstDomainManager->enImsaAvailDomain);
    }

    /* ����SRV����Wifi State Ind�е�������� */
    IMSA_SRV_SaveServiceChangeInd(pstMsccServiceChangeInd);

    /* ����VoLTE����VoWIFI���� */
    IMSA_Domain_SetVolteCtrlVowifi(pstMsccServiceChangeInd);

    /*����ɾ��wifi only״̬�£��յ�service change ind��Ϣ���ϱ����������Ĵ���
       �ĵ��ڸú������ͳһ���д���*/

    /* ������ѡ��ʼ���������� */
    IMSA_Domain_InitDomainOpVar(UEPS_PID_MSCC,ID_MSCC_IMSA_SERVICE_CHANGE_IND,VOS_TRUE,VOS_TRUE);

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:
            IMSA_ProcessNoDomainSelectInMsccServiceChangeInd(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_WIFI:
            IMSA_ProcessWifiOnlyDomainSelectInMsccServiceChangeInd(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_LTE:
            IMSA_ProcessLTEOnlyDomainSelectInMsccServiceChangeInd(VOS_TRUE);
            break;

        case IMSA_IMS_DOMAIN_LTE_WIFI:
            IMSA_ProcessDualDomainSelect(VOS_TRUE,VOS_TRUE);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_ProcMsccServiceChangeInd: DomainC1Judge Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_ProcMsccServiceChangeInd_ENUM, LNAS_StateInvalid);
    }

    /* �����Ҫ�ӳٴ�����Ϣ,��¼�¼�,�ȴ�SRV�ϱ�״̬,Ȼ������������ѡ���� */
    if(VOS_TRUE == pstDomainManager->ucMsgDelayProcessFlag)
    {
       /* ���浱ǰ��Ҫ�ӳٴ������ѡ���¼� */
       IMSA_Domain_SaveDelaySwitchEvent(UEPS_PID_MSCC,ID_MSCC_IMSA_SERVICE_CHANGE_IND);

       /* ��SRVģ��ע��״̬�ϱ��¼�, SRV������̬���ϱ�Ready_ind��Ϣ,�����µ���ѡ���¼� */
       IMSA_SRV_RegStatusNtfEvent();

       /* ���Delay�¼� */
       pstDomainManager->ucMsgDelayProcessFlag = VOS_FALSE;
    }

    /*IMSA���յ�service change ind��Ϣ�����û�з�����ѡ����Ҫ֪ͨMSCC��������*/
    if ((IMSA_TRUE == IMSA_SRV_GetImsVoiceCapability(&enImsVoiceCap))&& (VOS_FALSE == pstDomainManager->ucDomainSelectFlag))
    {
        IMSA_SndMsccMsgImsVoiceCapNotify(enImsVoiceCap, IMSA_CONN_IsExitPersistentBearer());
    }

    /*�ں�����������IMSA�յ�����service change ind��Ϣ�Ժ󱾵ر����LTE��ע�����ͺ���Ч��־λ*/
    pstControlManager->bitRegSuccProcTypeValid = IMSA_OP_FALSE;
    pstControlManager->enRegSuccProcType       = IMSA_MSCC_REG_PROC_BUTT;


    return VOS_FALSE;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_ProcLrrcStateInd()
 Description    : Lrrc State Ind��Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcLrrcStateInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;
    LRRC_IMSA_STATE_IND_STRU           *pLrrcStateInd       = VOS_NULL_PTR;
    PS_MSG_HEADER_STRU                 *pHeader             = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_ProcLrrcStateInd: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcLrrcStateInd_ENUM, LNAS_ENTRY);

    pLrrcStateInd = (LRRC_IMSA_STATE_IND_STRU *) pRcvMsg;
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;
    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    /* ����LTE�ź�ǿ�� */
    if( (pLrrcStateInd->enLteQuality < LTE_IMSA_QUALITY_LEVEL_BUTT)
        &&( VOS_TRUE == pstDomainManager->ucLteQualityCfgFlag))
    {
        pstDomainManager->enLteQuality = pLrrcStateInd->enLteQuality;
    }
    else
    {
        pstDomainManager->enLteQuality = IMSA_WIRELESS_QUALITY_LEVEL_BAD;
    }

    /* ��ǰ������֧��˫�򣬲�����LRRC�ź�ǿ����Ϣ��ֱ�ӷ��� */
    if( IMSA_IMS_DOMAIN_MODE_DUAL != IMSA_GetDomainMode())
    {
        IMSA_WARN_LOG("IMSA_Domain_ProcLrrcStateInd: No Dual Domain Enter, no process!");
        return VOS_FALSE;
    }

    /* ������ѡ��ʼ���������� */
    IMSA_Domain_InitDomainOpVar(PS_PID_ERRC,ID_LRRC_IMSA_STATE_IND,VOS_TRUE,VOS_TRUE);

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:
        case IMSA_IMS_DOMAIN_WIFI:
        case IMSA_IMS_DOMAIN_LTE:
            IMSA_WARN_LOG("IMSA_Domain_ProcLrrcStateInd: No Process in This State");
            return VOS_FALSE;

        case IMSA_IMS_DOMAIN_LTE_WIFI:
            IMSA_ProcessDualDomainSelectInLrrcStateInd(VOS_TRUE,VOS_TRUE);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_ProcLrrcStateInd: DomainC1Judge Wrong!");
            return VOS_FALSE;
    }

    /* �����Ҫ�ӳٴ�����Ϣ,��¼�¼�,�ȴ�SRV�ϱ�״̬,Ȼ������������ѡ���� */
    if(VOS_TRUE == pstDomainManager->ucMsgDelayProcessFlag)
    {
       /* ���浱ǰ��Ҫ�ӳٴ������ѡ���¼� */
       IMSA_Domain_SaveDelaySwitchEvent(pHeader->ulSenderPid,pHeader->ulMsgName);

       /* ��SRVģ��ע��״̬�ϱ��¼�, SRV������̬���ϱ�Ready_ind��Ϣ,�����µ���ѡ���¼� */
       IMSA_SRV_RegStatusNtfEvent();

       /* ���Delay�¼� */
       pstDomainManager->ucMsgDelayProcessFlag = VOS_FALSE;
    }

    IMSA_INFO_LOG1("IMSA_Domain_ProcLrrcStateInd: domain = ",IMSA_GetDomainMode());
    return VOS_FALSE;
}

/*****************************************************************************
 Function Name  : IMSA_Domain_ProcLrrcQualityConfigCnf()
 Description    : Lrrc QualityConfigCnf������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_Domain_ProcLrrcQualityConfigCnf
(
    const VOS_VOID                     *pRcvMsg
)
{
     IMSA_INFO_LOG("IMSA_Domain_ProcLrrcQualityConfigCnf: ENTER!");

     return VOS_FALSE;
}


/*****************************************************************************
 Function Name  : IMSA_Domain_ProcWifiErrorInd()
 Description    : Lrrc Error Ind��Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2016-01-14  Draft Enact
*****************************************************************************/

VOS_UINT32 IMSA_Domain_ProcWifiErrorInd(const VOS_VOID                *pRcvMsg)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstDomainManager    = VOS_NULL_PTR;
    AT_IMSA_IMS_CTRL_MSG_STRU          *pAtWifiImsaCtrlMsg  = VOS_NULL_PTR;
    WIFI_IMSA_ERROR_IND_STRU           *pWifiErrorInd       = VOS_NULL_PTR;
    IMSA_WIFI_NETWORK_INFO_STRU        *pstWifiNetInfo = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_Domain_ProcWifiErrorInd: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcWifiErrorInd_ENUM, LNAS_ENTRY);

    pAtWifiImsaCtrlMsg = (AT_IMSA_IMS_CTRL_MSG_STRU *)pRcvMsg;
    pWifiErrorInd = (WIFI_IMSA_ERROR_IND_STRU *) (pAtWifiImsaCtrlMsg->aucWifiMsg);
    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();
    pstWifiNetInfo                          = IMSA_GetWifiNetInfoAddress();

    /* ����PsServiceΪNO_SERVICE�������ٷ���ҵ�� */
    if( WIFI_IMSA_ERROR_CAUSE_LINK_LOST == pWifiErrorInd->enCause)
    {
        pstWifiNetInfo->enImsaPsServiceStatus = IMSA_PS_SERVICE_STATUS_NO_SERVICE;
        pstDomainManager->enWifiQuality = IMSA_WIRELESS_QUALITY_LEVEL_BAD;

    }


    /* ��ǰ������֧��˫�򣬲�����ERROR_IND��Ϣ��ֱ�ӷ��� */
    if( IMSA_IMS_DOMAIN_MODE_DUAL != IMSA_GetDomainMode())
    {
        IMSA_WARN_LOG("IMSA_Domain_ProcWifiErrorInd: No Dual Domain Enter, no process!");
        return VOS_TRUE;
    }

    if( IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN != pstDomainManager->enDomainFsmL2State)
    {
        IMSA_WARN_LOG("IMSA_Domain_ProcWifiErrorInd: It is no WIFI Domain, no process!");
        return VOS_TRUE;
    }

    if(((WIFI_IMSA_ERROR_CAUSE_LINK_LOST == pWifiErrorInd->enCause)
        || (WIFI_IMSA_ERROR_CAUSE_TUNNEL_EXCEPTION == pWifiErrorInd->enCause)
        || (WIFI_IMSA_ERROR_CAUSE_NW_RELEASE_IND == pWifiErrorInd->enCause))
         &&(VOS_FALSE == IMSA_IsImsExist()))
    {
       /*���ԭ��ֵ����·��ʧ������쳣�������ͷţ��ҵ�ǰû��IMSҵ����ڣ���Domainģ�鲻���д���ֱ��ȥconn
         ģ�鴦��*/
       IMSA_WARN_LOG("IMSA_Domain_ProcWifiErrorInd: no WIFI->LTE HO!");
       return VOS_TRUE;
    }


    /* WIFI LINK/TUNNEL Abnormal, and set WIFI Signal to BAD */
     /*lint -e539*/
     pstDomainManager->enWifiQuality = IMSA_WIRELESS_QUALITY_LEVEL_BAD;

    /* ������ѡ��ʼ���������� */
    IMSA_Domain_InitDomainOpVar(WUEPS_PID_MAPCON,ID_WIFI_IMSA_STATE_IND,VOS_TRUE,VOS_TRUE);
    /*lint +e539*/

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:
        case IMSA_IMS_DOMAIN_WIFI:
        case IMSA_IMS_DOMAIN_LTE:
            IMSA_WARN_LOG("IMSA_Domain_ProcLrrcStateInd: No Process in This State");
            return VOS_TRUE;

        case IMSA_IMS_DOMAIN_LTE_WIFI:
            IMSA_ProcessDualDomainSelectInWifiErrorInd(VOS_FALSE,VOS_TRUE);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_ProcLrrcStateInd: DomainC1Judge Wrong!");
            return VOS_TRUE;
    }


    /* �����Ҫ����ģ���������, �򷵻�TRUE */
    if(VOS_TRUE == pstDomainManager->ucDomainSelectFlag)
    {
        /* ����ҪCONN�������� */
        return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}

/*****************************************************************************
     Function Name  : IMSA_Domain_ProcUsimReadFileCnf()
     Description    : USIM ReadFile Cnf��Ϣ������
     Input          : VOS_VOID
     Output         : VOS_VOID
     Return Value   : VOS_VOID

     History        :
          1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

VOS_UINT32 IMSA_Domain_ProcUsimReadFileCnf(const VOS_VOID                *pRcvMsg)
{
    IMSA_CONTROL_MANAGER_STRU       *pstControlManager;

    pstControlManager   = IMSA_GetControlManagerAddress();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcUsimReadFileCnf_ENUM, LNAS_ENTRY);

    /*���ú����Զ����ļ����д��� */
    (VOS_VOID)IMSA_Domain_ProcReadIsimFileResult((VOS_VOID *)pRcvMsg);

    if((IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.stImsaUeId.bitOpImpi)
        && (IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.stImsaUeId.bitOpTImpu)
        && (IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.bitOpHomeNetDomainName))
    {
        /* ֹͣ�����ļ��ȴ���ʱ�� */
        IMSA_StopTimer(&pstControlManager->stWaitReadfileCnfTimer);

        /* ������ѡ��ʼ���������� */
        IMSA_Domain_InitDomainOpVar(WUEPS_PID_USIM,USIMM_READFILE_CNF,VOS_TRUE,VOS_TRUE);

        /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
        switch(IMSA_JudgeImsDomainC1())
        {
            case IMSA_IMS_DOMAIN_NULL:
                IMSA_WARN_LOG("IMSA_Domain_ProcUsimReadFileCnf: No Process in This State");
                break;

            case IMSA_IMS_DOMAIN_WIFI:
                IMSA_ProcessWifiOnlyDomainSelectInUsimFileReady(VOS_TRUE);
                break;

            case IMSA_IMS_DOMAIN_LTE:
                IMSA_ProcessLTEOnlyDomainSelectInUsimFileReady(VOS_TRUE);
                break;

            case IMSA_IMS_DOMAIN_LTE_WIFI:
                IMSA_ProcessDualDomainSelectInUsimFileReady(VOS_TRUE,VOS_TRUE);
                break;

            default:
                IMSA_WARN_LOG("IMSA_Domain_ProcUsimReadFileCnf: DomainC1Judge Wrong!");
                TLPS_PRINT2LAYER_WARNING(IMSA_Domain_ProcUsimReadFileCnf_ENUM, LNAS_StateInvalid);
        }
    }
    /* ��Ҫ����Ϣ������ģ��������� */
    return VOS_TRUE;
}


/*****************************************************************************
     Function Name  : IMSA_Domain_ProcIntraMsgSrvStatusInd()
     Description    : SRV Status Ind��Ϣ������
     Input          : VOS_VOID
     Output         : VOS_VOID
     Return Value   : VOS_VOID

     History        :
          1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_Domain_ProcIntraMsgSrvStatusInd(const VOS_VOID                *pRcvMsg)
{
    IMSA_CONTROL_MANAGER_STRU           *pstControlManager = VOS_NULL;
    IMSA_DOMAIN_SRV_STATUS_IND_STRU     *pstSrvStatusInd = VOS_NULL;

    pstSrvStatusInd   = (IMSA_DOMAIN_SRV_STATUS_IND_STRU     *)pRcvMsg;
    pstControlManager = IMSA_GetControlManagerAddress();

    IMSA_INFO_LOG("IMSA_Domain_ProcIntraMsgSrvStatusInd: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcIntraMsgSrvStatusInd_ENUM, LNAS_ENTRY);

    /* �������WIFI�ػ��У��ظ��ػ��ɹ���ת״̬���ػ�״̬ */
    if(IMSA_STATUS_STOPING == pstControlManager->enImsaWifiStatus)
    {
        /* ָʾSPM WIFI�ػ��ɹ� */
//        IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_OFF);

        /* �ظ�WIFI�ػ��ɹ� */
        IMSA_SndWifiPowerOffCnf();
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_NULL;
    }

    /* �������Cellular�ػ��У��ظ��ػ��ɹ���ת״̬���ػ�״̬ */
    if(IMSA_STATUS_STOPING == pstControlManager->enImsaCellularStatus)
    {
        /* �ظ�Cellular�ػ��ɹ� */
        IMSA_SndMsccMsgStopCnf();
        pstControlManager->enImsaCellularStatus = IMSA_STATUS_NULL;
    }

    if(IMSA_DOMAIN_SERVICE_STATUS_NO_SERVICE == pstSrvStatusInd->enDomainSrvStatus)
    {
        /* No Service�¿�����ѡ�����״̬���� */
        IMSA_DomainSelectResultInNoSrv();
    }
    else if(IMSA_DOMAIN_SERVICE_STATUS_NORMAL_SERVICE == pstSrvStatusInd->enDomainSrvStatus)
    {
        /* Normal Service״̬����ѡ�����״̬���� */
        IMSA_DomainSelectResultInNrmSrv(pstSrvStatusInd);
    }
    else
    {
        IMSA_WARN_LOG("IMSA_Domain_ProcIntraMsgSrvStatusInd: Srv Status is Wrong!");
    }

}

/*****************************************************************************
     Function Name  : IMSA_Domain_ProcIntraMsgSrvReadyInd()
     Description    : SRV Ready Ind��Ϣ������
     Input          : VOS_VOID
     Output         : VOS_VOID
     Return Value   : VOS_VOID

     History        :
          1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_Domain_ProcIntraMsgSrvReadyInd(const VOS_VOID *pRcvMsg)
{

    IMSA_DOMAIN_MANAGER_STRU *pstDomainManager = VOS_NULL;

    PS_MSG_HEADER_STRU       *pHeader = VOS_NULL_PTR;

    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;
    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcIntraMsgSrvReadyInd_ENUM, LNAS_ENTRY);


    /* �����ӳٵ���Ϣ, ������ѡ��ʼ���������� */
    IMSA_Domain_InitDomainOpVarByDelayEvent();

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    IMSA_DomainSelect();

    /* �����Ҫ�ӳٴ�����Ϣ,��¼�¼�,�ȴ�SRV�ϱ�״̬,Ȼ������������ѡ���� */
    if( VOS_TRUE == pstDomainManager->ucMsgDelayProcessFlag)
    {
       /* ���浱ǰ��Ҫ�ӳٴ������ѡ���¼� */
       IMSA_Domain_SaveDelaySwitchEvent(pHeader->ulSenderPid,pHeader->ulMsgName);

       /* ��SRVģ��ע��״̬�ϱ��¼�, SRV������̬���ϱ�Ready_ind��Ϣ,�����µ���ѡ���¼� */
       IMSA_SRV_RegStatusNtfEvent();
    }
    else
    {
        /* ���������Ҫ�ӳٴ������ѡ���¼� */
        IMSA_Domain_ClearDelaySwitchEvent();
    }

    /* �������SRVû�н����κδ���,Ϊ������SRV��ǰ�Ĵ���,����Continue���� */
    if( VOS_FALSE == pstDomainManager->ucDomainSelectFlag)
    {
        /* coverity[unchecked_value] */
        IMSA_SRV_DomainContinue(IMSA_GetCurrentDomain());
    }

    return;
}


/*****************************************************************************
     Function Name  : IMSA_Domain_ProcIntraMsgSrvReadyInd()
     Description    : IMSA_DOMAIN_SWITCH_RESULT_IND��Ϣ������
     Input          : VOS_VOID
     Output         : VOS_VOID
     Return Value   : VOS_VOID

     History        :
          1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_Domain_ProcIntraMsgSwitchResultInd(const VOS_VOID *pRcvMsg)
{

    IMSA_DOMAIN_MANAGER_STRU *pstDomainManager = VOS_NULL;
    IMSA_DOMAIN_SWITCH_RESULT_IND_STRU *pSwitchResultInd;
    IMSA_CONTROL_MANAGER_STRU           *pstControlManager = VOS_NULL;

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();
    pSwitchResultInd = (IMSA_DOMAIN_SWITCH_RESULT_IND_STRU *)pRcvMsg;
    pstControlManager = IMSA_GetControlManagerAddress();

    IMSA_INFO_LOG("IMSA_Domain_ProcIntraMsgSwitchResultInd: ENTER!");
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcIntraMsgSwitchResultInd_ENUM, LNAS_ENTRY);

    /* �ػ����µ��л�, �عػ��ظ� */

    /* �������WIFI�ػ��У��ظ��ػ��ɹ���ת״̬���ػ�״̬ */
    if(IMSA_STATUS_STOPING == pstControlManager->enImsaWifiStatus)
    {
        /* ָʾSPM WIFI�ػ��ɹ� */
//        IMSA_SndSpmImsSwitchStateInd(IMSA_SPM_IMS_SWITCH_STATE_OFF);

        /* �ظ�WIFI�ػ��ɹ� */
        IMSA_SndWifiPowerOffCnf();
        pstControlManager->enImsaWifiStatus = IMSA_STATUS_NULL;
    }

    /* �������Cellular�ػ��У��ظ��ػ��ɹ���ת״̬���ػ�״̬ */
    if(IMSA_STATUS_STOPING == pstControlManager->enImsaCellularStatus)
    {
        /* �ظ�Cellular�ػ��ɹ� */
        IMSA_SndMsccMsgStopCnf();
        pstControlManager->enImsaCellularStatus = IMSA_STATUS_NULL;
    }

    /*�������ǰ����L2W�л�״̬�������ϱ��л������Ϣһ�£��������л�״̬ */
    if( (IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH == pstDomainManager->enDomainFsmL2State)
        && ( IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH == pSwitchResultInd->enSwitchType))
    {
        IMSA_INFO_LOG("IMSA_Domain_ProcIntraMsgSwitchResultInd: 1 L2W");
        if( IMSA_DOMAIN_SWITCH_RESULT_SUCCESS == pSwitchResultInd->enSwitchResult)
        {
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;
        }
        else
        {
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;

            /* ����WIFI��ͷ���ʱ�� */
            /*lint -e734*/
            pstDomainManager->ucPunishTimeInWifiDomain = pstDomainManager->ucPunishTimeInWifiDomain * 2;
            IMSA_Domain_StartTimer1WifiDomain();
            /*lint +e734*/
        }
    }
    /*�������ǰ����W2L�л�״̬�������ϱ��л������Ϣһ�£��������л�״̬ */
    else if( (IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH == pstDomainManager->enDomainFsmL2State)
        && ( IMSA_DOMAIN_SWITCH_TYPE_W2L_SWITCH == pSwitchResultInd->enSwitchType))
    {
        IMSA_INFO_LOG("IMSA_Domain_ProcIntraMsgSwitchResultInd: 2 W2L");
        if( IMSA_DOMAIN_SWITCH_RESULT_SUCCESS == pSwitchResultInd->enSwitchResult)
        {
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING;
        }
        else
        {
            pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING;

            /* ����LTE��ͷ���ʱ�� */
            pstDomainManager->ucPunishTimeInLteDomain = (VOS_UINT8)(pstDomainManager->ucPunishTimeInLteDomain * 2);
            IMSA_Domain_StartTimer1LteDomain();
        }
    }
    else
    {
        IMSA_INFO_LOG("IMSA_Domain_ProcIntraMsgSwitchResultInd: 3 Error");
        /*  L2 FSM״̬����Ϣ���ݲ�һ�£��򷵻ؿ���״̬��*/
        pstDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_NULL;
    }

    return;
}

/*****************************************************************************
     Function Name  : IMSA_Domain_ProcIntraMsgBearerRelInd()
     Description    : IMSA_DOMAIN_BEARER_REL_IND��Ϣ������
     Input          : VOS_VOID
     Output         : VOS_VOID
     Return Value   : VOS_VOID

     History        :
          1.Zhangdongfeng 353461        2015-12-25  Draft Enact
*****************************************************************************/

VOS_VOID IMSA_Domain_ProcIntraMsgBearerRelInd(const VOS_VOID *pRcvMsg)
{

    TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcIntraMsgBearerRelInd_ENUM, LNAS_ENTRY);
    /* ��ǰ������֧��˫�򣬲���������쳣��Ϣ����SRV�������ڶ�ʱ������ֱ�ӷ��� */
    if( IMSA_IMS_DOMAIN_MODE_DUAL != IMSA_GetDomainMode())
    {
        IMSA_WARN_LOG("IMSA_Domain_ProcIntraMsgBearerRelInd: No Dual Domain Enter, no process!");
        return ;
    }

    /* ������ѡ����,���ݵ�ǰ״̬ѡ����ʵ��� */
    switch(IMSA_JudgeImsDomainC1())
    {
        case IMSA_IMS_DOMAIN_NULL:
        case IMSA_IMS_DOMAIN_WIFI:
        case IMSA_IMS_DOMAIN_LTE:
            IMSA_WARN_LOG("IMSA_Domain_ProcIntraMsgBearerRelInd: No Process in This State");
            return ;
        case IMSA_IMS_DOMAIN_LTE_WIFI:
            IMSA_ProcessDualDomainSelectInBearerRelInd(VOS_TRUE,VOS_TRUE);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_ProcIntraMsgBearerRelInd: DomainC1Judge Wrong!");
            return ;
    }

    return ;

}


VOS_UINT32 IMSA_Domain_ProcMsccMsgRegReq
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_CONTROL_MANAGER_STRU          *pstControlManager;

    (VOS_VOID)pRcvMsg;

    /* ���ȥע��ԭ��ֵ */
    IMSA_SetDeregCause(IMSA_SRV_DEREG_CAUSE_BUTT);

    pstControlManager = IMSA_GetControlManagerAddress();

    /* ֹͣȥע��ȴ���ʱ�� */
    IMSA_StopTimer(&pstControlManager->stWaitDeregTimer);

    /* NV��GUNAS�޸ģ�IMSAֻ��Ҫ��NV��Ӧ��ȫ�ֱ����޸��˼��ɡ� */
    pstControlManager->stImsaConfigPara.ucWifiImsSupportFlag = VOS_TRUE;
    pstControlManager->stImsaConfigPara.ucLteImsSupportFlag  = VOS_TRUE;

    /* ��GUNAS��ͨ��IMS��̬������, ���յ�AP���IMSSWITCH=1ʱ, �������޸�Voice Domain,
       �������յ�AP���޸�Voice Domain AT����ʱ�Ż��޸�, ���:��IMS����ʱ, ����IMSA��ǰ
       �޸���Voice Domain, ���ͬʱMSCC�·�Service Change Ind, �Ӷ�������ǰ����IMS�������
       ����, ����������LMM��TAU���̶Գ� */

    /*  ���������ģ��״̬��Ǩ��  */
    if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
          ||(IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer()))
        && (VOS_TRUE == IMSA_Domain_IsDualDomainExist()))
    {
        /* ���NV�Ϳ���״̬֧��˫��,��IMS���ȹ�ϵ֧��˫��, ����IMSA��״̬Ϊ Dual Domain */
        IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_DUAL);
        IMSA_INFO_LOG("IMSA_Domain_ProcMsccMsgRegReq: 1_DomainMode is IMSA_IMS_DOMAIN_MODE_DUAL!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccMsgRegReq_ENUM, LNAS_FUNCTION_LABEL1);
    }
    else if( ((IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer())
              || (IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
              || (IMSA_IMS_DOMAIN_PREFER_WIFI_ONLY == IMSA_GetImsDomainPrefer()))
             &&(VOS_TRUE == IMSA_Domain_IsWifiDomainExist()))
    {
        /* ���NV�Ϳ���״̬֧��WIFI��,��IMS���ȹ�ϵ֧��WIFI��,����IMSA��״̬Ϊ WIFI ONLY */
        IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_WIFI_ONLY);
        IMSA_INFO_LOG("IMSA_Domain_ProcMsccMsgRegReq: 2_DomainMode is IMSA_IMS_DOMAIN_MODE_WIFI_ONLY!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccMsgRegReq_ENUM, LNAS_FUNCTION_LABEL2);
    }
    else if( ((IMSA_IMS_DOMAIN_PREFER_CELLULAR_PREFER == IMSA_GetImsDomainPrefer())
               || (IMSA_IMS_DOMAIN_PREFER_WIFI_PREFER == IMSA_GetImsDomainPrefer())
               || (IMSA_IMS_DOMAIN_PREFER_CELLULAR_ONLY == IMSA_GetImsDomainPrefer()))
             &&(VOS_TRUE == IMSA_Domain_IsCellularDomainExist()))
    {
        /* ���NV�Ϳ���״̬֧��Cellular��,��IMS���ȹ�ϵ֧��Cellular��,����IMSA��״̬Ϊ Cellular ONLY */
        IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY);
        IMSA_INFO_LOG("IMSA_Domain_ProcMsccMsgRegReq: 3_DomainMode is IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccMsgRegReq_ENUM, LNAS_FUNCTION_LABEL3);
    }
    else
    {
        /* ����IMSA��״̬Ϊ NULL */
        IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);
        IMSA_INFO_LOG("IMSA_Domain_ProcMsccMsgRegReq: 4_DomainMode is IMSA_IMS_DOMAIN_MODE_NULL!");
        TLPS_PRINT2LAYER_INFO(IMSA_Domain_ProcMsccMsgRegReq_ENUM, LNAS_FUNCTION_LABEL4);
    }

    /*AT����*/
    IMSA_SndMsccMsgRegCnf();
    return VOS_FALSE;
}


VOS_UINT32 IMSA_Domain_ProcMsccMsgDeregReq
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_DOMAIN_MANAGER_STRU           *pstImsaDomainManager = VOS_NULL_PTR;
    IMSA_CONTROL_MANAGER_STRU          *pstControlManager;

    (VOS_VOID)pRcvMsg;

    pstControlManager = IMSA_GetControlManagerAddress();

    /* NV��GUNAS�޸ģ�IMSAֻ��Ҫ��NV��Ӧ��ȫ�ֱ����޸��˼��ɡ� */
    pstControlManager->stImsaConfigPara.ucWifiImsSupportFlag = VOS_FALSE;
    pstControlManager->stImsaConfigPara.ucLteImsSupportFlag  = VOS_FALSE;

    /* ��GUNAS��ͨ�ر�IMS��̬����ʱ, ���յ�AP���IMSSWITCH=0ʱ, �������޸�Voice Domain,
       �������յ�AP���޸�Voice Domain AT����ʱ�Ż��޸�, ���:��IMS����ʱ, ����IMSA��ǰ
       �޸���Voice Domain, ���ͬʱMSCC�·�Service Change Ind, �Ӷ�������ǰ����IMS�������
       ����, ����������LMM��TAU���̶Գ� */

    /* SRVCC�쳣(����ػ���DEREG REQ��״̬Ǩ��CONN+REG)���建�� */
    IMSA_SrvccAbormalClearBuff(IMSA_SRVCC_ABNORMAL_DEREG_REQ);

    /* ����ȥע��ԭ��ֵ */
    IMSA_SetDeregCause(IMSA_SRV_DEREG_CAUSE_MSCC_DEREG_REQ);

    IMSA_SetDomainMode(IMSA_IMS_DOMAIN_MODE_NULL);

    pstImsaDomainManager =  IMSA_Domain_GetDomainManagerAddr();

    switch(pstImsaDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            IMSA_SRV_ProcDeregReq(VOS_FALSE);
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            IMSA_SRV_ProcDeregReq(VOS_FALSE);
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            IMSA_SRV_ProcDeregReq(VOS_FALSE);
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_SRV_ProcDeregReq(VOS_TRUE);
            pstImsaDomainManager->enDomainFsmL2State = IMSA_DOMAIN_FSM_L2_STATE_DISABLING;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            IMSA_SRV_ProcDeregReq(VOS_FALSE);
            break;

        default:
            IMSA_WARN_LOG("IMSA_Domain_ProcMsccMsgDeregReq: DomainC2 FSM Status Wrong!");
            TLPS_PRINT2LAYER_WARNING(IMSA_Domain_ProcMsccMsgDeregReq_ENUM, LNAS_StateInvalid);
    }

    return VOS_FALSE;
}

/*****************************************************************************
 Function Name  : IMSA_GetCurrentDomain()
 Description    : ��õ�ǰѡ���IMS Domain
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   :
      ���ص�ǰ�Ѿ�ѡ�����:
               IMSA_IMS_DOMAIN_NULL: û�п�����
               IMSA_IMS_DOMAIN_WIFI: ��ǰѡ����WIFI,
               IMSA_IMS_DOMAIN_LTE :��ǰѡ����LTE
 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
IMSA_IMS_DOMAIN_ENUM_UINT8 IMSA_GetCurrentDomain(void)
{
    IMSA_DOMAIN_MANAGER_STRU            *pstDomainManager = VOS_NULL_PTR;
    IMSA_IMS_DOMAIN_ENUM_UINT8           enCurrentDomain = IMSA_IMS_DOMAIN_NULL;

    pstDomainManager = IMSA_Domain_GetDomainManagerAddr();

    TLPS_PRINT2LAYER_INFO(IMSA_GetCurrentDomain_ENUM, LNAS_ENTRY);

    switch(pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            enCurrentDomain = IMSA_IMS_DOMAIN_NULL;
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
            enCurrentDomain = IMSA_IMS_DOMAIN_LTE;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            enCurrentDomain = IMSA_IMS_DOMAIN_WIFI;
            break;

        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
            /* �л������У��о�SRVģ��״̬ȷ��״̬ */
            enCurrentDomain = IMSA_SRV_GetCurrentDomainInHo();
            break;

        default:
            break;
    }

    /* ��ǰֻ��LTE��֧�ֽ����������ֻ���ڽ���SRV״̬ʱ���ϱ�LTE�� */
    if( (IMSA_SRV_STATUS_IDLE_DEREG == IMSA_SRV_GetNormalSrvStatus())
        &&( IMSA_SRV_STATUS_IDLE_DEREG != IMSA_SRV_GetEmcSrvStatus()))
    {
        enCurrentDomain = IMSA_IMS_DOMAIN_LTE;
    }

    return enCurrentDomain;
}

/*****************************************************************************
 Function Name  : IMSA_IsImsDomainCap()
 Description    : �жϵ�ǰ�����Ƿ�֧��IMS Domain
 Input          :
                   IMSA_IMS_DOMAIN_WIFI: ��ǰѡ����WIFI,
                   IMSA_IMS_DOMAIN_LTE :��ǰѡ����LTE
 Output         : VOS_VOID
 Return Value   :
      ����ѡ������Ƿ����: VOS_TRUE:���� VOS_FALSE:������
 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/

VOS_UINT32 IMSA_IsImsDomainCap(IMSA_IMS_DOMAIN_ENUM_UINT8 enImsaDomain)
{
    if( IMSA_IMS_DOMAIN_NULL == enImsaDomain)
    {
        return VOS_TRUE;
    }
    if(IMSA_GetDomainMode()>=IMSA_IMS_DOMAIN_BUTT)
    {
        return VOS_FALSE;
    }

    if(IMSA_GetDomainMode() == enImsaDomain)
    {
        return VOS_TRUE;
    }
    else if( (IMSA_IMS_DOMAIN_LTE_WIFI == IMSA_GetDomainMode())
             &&(  (IMSA_IMS_DOMAIN_WIFI == enImsaDomain)
                ||(IMSA_IMS_DOMAIN_LTE == enImsaDomain)))
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}

/*****************************************************************************
 Function Name  : IMSA_SndSpmImsSwitchStateInd()
 Description    : ���͵�ǰWIFI����״̬��SPM
 Input          :  enSwitchState: IMSA_SPM_IMS_SWITCH_STATE_OFF/ON
 Output         : VOS_VOID
 Return Value   :
               ��
 History        :
      1.Zhangdongfeng 353461        2015-11-17  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndSpmImsSwitchStateInd( IMSA_SPM_IMS_SWITCH_STATE_ENUM_UINT8 enSwitchState)
{

    IMSA_SPM_IMS_SWITCH_STATE_IND_STRU           *pstImsaSpmImsSwitchStateInd;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaSpmImsSwitchStateInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_IMS_SWITCH_STATE_IND_STRU));
    TLPS_PRINT2LAYER_INFO(IMSA_SndSpmImsSwitchStateInd_ENUM, LNAS_ENTRY);

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstImsaSpmImsSwitchStateInd)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndSpmImsSwitchStateInd:ERROR:Alloc Msg fail!");
        return;
    }

    /*���*/
    IMSA_MEM_SET_S( IMSA_GET_MSG_ENTITY(pstImsaSpmImsSwitchStateInd),IMSA_GET_MSG_LENGTH(pstImsaSpmImsSwitchStateInd), 0, IMSA_GET_MSG_LENGTH(pstImsaSpmImsSwitchStateInd));

    /*��д��Ϣͷ*/
    IMSA_WRITE_SPM_MSG_HEAD(pstImsaSpmImsSwitchStateInd, ID_IMSA_SPM_IMS_SWITCH_STATE_IND);

    /* ��д��Ϣ�� */
    pstImsaSpmImsSwitchStateInd->enImsSwitch = enSwitchState;

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstImsaSpmImsSwitchStateInd);
}


/*****************************************************************************
 Function Name   : IMSA_Domain_PrintDomainFsmState
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_INT32

 History         :
    1.zhangdongfeng    2016-03-02  Draft Enact

*****************************************************************************/
VOS_VOID IMSA_Domain_PrintDomainFsmState( VOS_VOID )
{
    IMSA_DOMAIN_MANAGER_STRU              *pstDomainManager   = VOS_NULL_PTR;
    /* ��ʼ���������� */
    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();
    TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, LNAS_ENTRY);

    switch(IMSA_GetDomainMode())
    {
        case IMSA_IMS_DOMAIN_MODE_NULL:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain Cap: NULL");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_CAP_NULL);
            break;
        case IMSA_IMS_DOMAIN_MODE_WIFI_ONLY:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain Cap: WIFI_ONLY");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_CAP_WIFIONLY);

            break;
        case IMSA_IMS_DOMAIN_MODE_CELLULAR_ONLY:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain Cap: CELLULAR_ONLY");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_CAP_CELLULARONLY);

            break;
        case IMSA_IMS_DOMAIN_MODE_DUAL:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain Cap: DUAL_DOMAIN");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_CAP_DUAL);
            break;
        default:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain Cap: UNKNOW");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_CAP_UNKNOWN);
            break;
    }

    switch(pstDomainManager->enDomainFsmL2State)
    {
        case IMSA_DOMAIN_FSM_L2_STATE_NULL:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: NULL");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_NULL);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: LTE_TRYING");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_LTE);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: LTE");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_LTE);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN_TRYING:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: WIFI_TRYING");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_WIFI);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DOMAIN:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: WIFI");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_WIFI);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_L2W_SWITCH:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: L2W_SWITCH");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_L2W);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_W2L_SWITCH:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: W2L_SWITCH");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_W2L);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_DISABLING:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: DISABLING");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_DISABLING);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_LTE_DISABLING:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: LTE_DISABLING");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_DISABLING);
            break;
        case IMSA_DOMAIN_FSM_L2_STATE_WIFI_DISABLING:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: WIFI_DISABLING");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_DISABLING);
            break;
        default:
            IMSA_INFO_LOG("IMSA_Domain_PrintDomainFsmState Current Domain State: UNKNOWN");
            TLPS_PRINT2LAYER_INFO(IMSA_Domain_PrintDomainFsmState_ENUM, IMSA_DOMAIN_STATUS_UNKNOWN);
            break;
    }
}


VOS_UINT32 IMSA_Domain_ProcAreaaLostInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_DOMAIN_MANAGER_STRU    *pstDomainManager = VOS_NULL_PTR;

    pstDomainManager      = IMSA_Domain_GetDomainManagerAddr();

    /* �����WIFI���ʱ�򣬶���ǰ���绰���л���WIFI�£�����Ҫ��������ֻ��L
    ���ʱ�򣬲Ŵ������Ϣ */
    if (IMSA_IMS_DOMAIN_LTE == IMSA_JudgeImsDomainC1())
    {
        return IMSA_TRUE;
    }
    else if (IMSA_IMS_DOMAIN_LTE_WIFI == IMSA_JudgeImsDomainC1())
    {
        if ((IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN == pstDomainManager->enDomainFsmL2State) ||
            (IMSA_DOMAIN_FSM_L2_STATE_LTE_DOMAIN_TRYING == pstDomainManager->enDomainFsmL2State))
        {
            return IMSA_TRUE;
        }
    }
    else
    {
        return IMSA_FALSE;
    }
    return IMSA_FALSE;
}

#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcApsMsg.c*/

