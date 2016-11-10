/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaProcApsMsg.c
  Description     : ��C�ļ�ʵ��APS��Ϣ�����APS��Ϣ����
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaConnManagement.h"
#include "ImsaProcApsMsg.h"
#include "TafApsApi.h"
#include "ImsaPublic.h"
#include "ImsaIntraInterface.h"
#include "ImsaProcAtMsg.h"
#include "ImsaCallManagement.h"
#include "ImsaServiceManagement.h"
#include "ImsaRegManagement.h"
#include "ImsaImsAdaption.h"
#include "ImsaProcWifiMsg.h"
#include "ImsaRnicInterface.h"
#include "ImsaProcRnicMsg.h"
#include "ImsaProcMsccMsg.h"
/*lint -e767*/
#define    THIS_FILE_ID      PS_FILE_ID_IMSAPROCWIFIMSG_C
#define    THIS_NAS_FILE_ID  NAS_FILE_ID_IMSAPROCWIFIMSG_C
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


/*****************************************************************************
 Function Name  : IMSA_SndWifiPowerOffCnf()
 Description    : WIFI power off cnf������Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndWifiPowerOffCnf(VOS_VOID )
{

    IMSA_WIFI_POWEROFF_CNF_STRU        *pImsaWifiPowerOffCnf;


    /*����ռ䲢��������Ƿ�ɹ�*/
    pImsaWifiPowerOffCnf = (VOS_VOID*)IMSA_MEM_ALLOC(sizeof(IMSA_WIFI_POWEROFF_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pImsaWifiPowerOffCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndWifiPowerOffCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET_S( pImsaWifiPowerOffCnf, sizeof(IMSA_WIFI_POWEROFF_CNF_STRU), 0, sizeof(IMSA_WIFI_POWEROFF_CNF_STRU));

    /*��д��Ϣ����*/
    pImsaWifiPowerOffCnf->lMsgId = ID_IMSA_WIFI_POWEROFF_CNF;

    /*������Ϣ���ͺ��� */
    IMSA_SndMsgAtImsCtrlMsg(pImsaWifiPowerOffCnf, sizeof(IMSA_WIFI_POWEROFF_CNF_STRU));

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pImsaWifiPowerOffCnf);

}





/*****************************************************************************
 Function Name  : IMSA_SndWifiPowerOnCnf()
 Description    : WIFI power on cnf������Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.Zhangdongfeng 353461        2015-10-19  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndWifiPowerOnCnf(VOS_VOID )
{

    IMSA_WIFI_POWERON_CNF_STRU        *pImsaWifiPowerOnCnf;


    /* ����ռ䲢��������Ƿ�ɹ� */
    pImsaWifiPowerOnCnf = (VOS_VOID*)IMSA_MEM_ALLOC(sizeof(IMSA_WIFI_POWERON_CNF_STRU));

    /* ����Ƿ����ɹ� */
    if (VOS_NULL_PTR == pImsaWifiPowerOnCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndWifiPowerOnCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET_S(pImsaWifiPowerOnCnf,sizeof(IMSA_WIFI_POWERON_CNF_STRU), 0, sizeof(IMSA_WIFI_POWERON_CNF_STRU));

    /*��д��Ϣ����*/
    pImsaWifiPowerOnCnf->lMsgId = ID_IMSA_WIFI_POWERON_CNF;

    /*������Ϣ���ͺ��� */
    IMSA_SndMsgAtImsCtrlMsg(pImsaWifiPowerOnCnf, sizeof(IMSA_WIFI_POWERON_CNF_STRU));

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pImsaWifiPowerOnCnf);

}


unsigned int Imsa_SndWifiAudioInfoInd( IMSA_IMS_AUDIO_INFO_IND_STRU  *pstImsAudioInfoList)
{

    IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU   *pstAudioInfoInd;
    int                                 i;
    int                                 j;
    VOS_INT8                            cAudioInfoCnt=0;


    /*����ռ䲢��������Ƿ�ɹ�*/
    pstAudioInfoInd = IMSA_MEM_ALLOC(sizeof(IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU));
    if ( VOS_NULL_PTR == pstAudioInfoInd )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("Imsa_SndWifiAudioInfoInd:ERROR:Alloc Mem fail!");
        return VOS_ERR;
    }
    /*���*/
    IMSA_MEM_SET_S(pstAudioInfoInd,sizeof(IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU), 0, sizeof(IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU));

    pstAudioInfoInd->lMsgId    = ID_IMSA_WIFI_IMS_AUDIO_INFO_IND;

    /*     ����IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU���ݽṹ�����Ϣ�� ��ѯImsaAudioInfo */
    for(i=0; i<IMSA_IMS_AUDIO_LIST_NUM;  i++)
    {
        /*   ���stAudioInfo[i]����ʹ��,  ��Audio��Ӧ���ݿ�����Ϣ�ṹ��  */
        if(IMSA_PORT_STATUS_IN_USE == pstImsAudioInfoList->stAudioInfo[i].enUseFlag)
        {
            pstAudioInfoInd->stMediaInfo[cAudioInfoCnt].ulChannelId        = pstImsAudioInfoList->stAudioInfo[i].usChannelId;
            pstAudioInfoInd->stMediaInfo[cAudioInfoCnt].usLocalRTPPort     = pstImsAudioInfoList->stAudioInfo[i].usLocalRTPPort;
            pstAudioInfoInd->stMediaInfo[cAudioInfoCnt].usLocalRTCPPort    = pstImsAudioInfoList->stAudioInfo[i].usLocalRTCPPort;
            pstAudioInfoInd->stMediaInfo[cAudioInfoCnt].usRemoteRTPPort    = pstImsAudioInfoList->stAudioInfo[i].usRemoteRTPPort;
            pstAudioInfoInd->stMediaInfo[cAudioInfoCnt].usRemoteRTCPPort   = pstImsAudioInfoList->stAudioInfo[i].usRemoteRTCPPort;
            IMSA_MEM_CPY_S(  &pstAudioInfoInd->stRemoteAddr,
                             sizeof(IMSA_IP_ADDRESS_STRU),
                             &pstImsAudioInfoList->stAudioInfo[i].stRemoteAddr,
                             sizeof(IMSA_IP_ADDRESS_STRU));
            cAudioInfoCnt++;

            IMSA_INFO_LOG("the audio info that IMSA SEND is as below");
            for(j=0;j<4;j++)
            {
                IMSA_INFO_LOG1("pstAudioInfoInd->stRemoteAddr.aucIPV4addr[] = ",pstAudioInfoInd->stRemoteAddr.aucIpV4Addr[j]);
            }
            for(j=0;j<16;j++)
            {
                IMSA_INFO_LOG1("pstAudioInfoInd->stRemoteAddr.aucIPV6addr[] = ",pstAudioInfoInd->stRemoteAddr.aucIpV6Addr[j]);
            }
        }
    }
    pstAudioInfoInd->cAudioInfoCnt = cAudioInfoCnt;

    /*  ��Ϣ�е�cAudioInfoCnt��audio�����е�ucAudioInfoCnt��һ��, �����쳣 */
    if( cAudioInfoCnt != pstImsAudioInfoList->ucAudioInfoCnt)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("Imsa_SndWifiAudioInfoInd:ERROR:Alloc Mem fail!");
    }

    IMSA_SndMsgAtImsCtrlMsg(pstAudioInfoInd, sizeof(IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU));

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstAudioInfoInd);

    return VOS_OK;
}


VOS_VOID IMSA_CONN_RlsWifiPdn(VOS_VOID)
{
    IMSA_WIFI_NORMAL_CONN_STRU         *pstNormalConn   = VOS_NULL_PTR;


    IMSA_INFO_LOG("IMSA_CONN_RlsWifiPdn enter!");
    TLPS_PRINT2LAYER_INFO(IMSA_CONN_RlsWifiPdn_ENUM, LNAS_ENTRY);

    pstNormalConn                   = IMSA_CONN_GetWifiNormalConnAddr();

#if 0
    /* ֪ͨA��ɾ���������� */
    /* ��������wifi�³���û�г���ID�����Ժͷ���Լ������Ϊ0 */
    IMSA_SndMsgRnicPdnDeactInd((VOS_UINT8)0, IMSA_RNIC_IMS_RAT_TYPE_WIFI);
    IMSA_SndMsgAtPdpDeactInd((TAF_PDP_TYPE_ENUM_UINT8)pstNormalConn->stSipSignalPdp.enPdnType, IMSA_AT_IMS_RAT_TYPE_WIFI);
#endif

    /* ɾ�����������Ϣ */
    IMSA_CONN_DeleteWifiPdpInfo2Reg(IMSA_CONN_TYPE_NORMAL, &pstNormalConn->stSipSignalPdp);

    /* ����wifi�ͷų��� */
    IMSA_CONN_SndWifiMsgPdnDeactiveReq(IMSA_WIFI_PDN_RELEASE_TYPE_NW_CMD);

    /* �����ȴ�PDP_DEACTIVE_CNF��ʱ�� */
    IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_DEACTIVE_CNF);

    /* ����ģ��״̬Ǩ��IDLE */
    IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_RELEASING);

    return;

}




VOS_VOID IMSA_CONN_SndWifiMsgPdnActivateReq
(
    VOS_UINT32                           bitOpApn,
    IMSA_WIFI_PDP_APN_STRU              *pstApnInfo,
    IMSA_WIFI_IP_TYPE_ENUM_INT8          enIpType,
    VOS_INT8                             enHoFlag
)
{
    IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU   *pstPdnActReq;
    IMSA_PDP_CNTXT_INFO_STRU              *pstLtePdpInfo       = VOS_NULL_PTR;
    IMSA_NORMAL_CONN_STRU                 *pstNormalConn       = VOS_NULL_PTR;
    /*IMSA_EMC_CONN_STRU                    *pstEmcConn          = VOS_NULL_PTR;*/
    VOS_UINT8                              i;
    VOS_UINT8                              aucTemp[IMSA_IPV6_PREFIX_LEN]   = {0};
    VOS_UINT8                              ucIpv4TypeFlag = 0;
    VOS_UINT8                              ucIpv6TypeFlag = 0;

    /* �����κ����� */
    if( ((VOS_TRUE != enHoFlag)&&(VOS_FALSE != enHoFlag))
        || ((enIpType>IMSA_WIFI_IP_IPV4V6)||(enIpType<IMSA_WIFI_IP_IPV4)))
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndWifiMsgPdnActivateReq:ERROR:Par is invalid!");
        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_SndWifiMsgPdnActivateReq_ENUM,LNAS_PARAM_INVALID);
        return;
    }


    /*����ռ䲢��������Ƿ�ɹ�*/
    pstPdnActReq = IMSA_MEM_ALLOC(sizeof(IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU));
    pstNormalConn                   = IMSA_CONN_GetNormalConnAddr();

    if ( VOS_NULL_PTR == pstPdnActReq )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndWifiMsgPdnActivateReq:ERROR:Alloc Mem fail!");
        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_SndWifiMsgPdnActivateReq_ENUM, LNAS_NULL_PTR);
        return ;
    }

    /*���*/
    IMSA_MEM_SET_S(pstPdnActReq, sizeof(IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU),0, sizeof(IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU));

    pstPdnActReq->lMsgId        = ID_IMSA_WIFI_IMS_PDN_ACTIVATE_REQ;

    /* ��wifiԼ��������ϵͳ������д�������� */
    pstPdnActReq->enIpType      = enIpType;

    /* �����л������������� */
    pstPdnActReq->cIsHandover   = enHoFlag;

    if(VOS_TRUE == enHoFlag)
    {
        for(i=0;i<pstNormalConn->ulSipSignalPdpNum;i++)
        {
            pstLtePdpInfo  = &pstNormalConn->astSipSignalPdpArray[i];
            if((IMSA_IP_TYPE_IPV4 == pstLtePdpInfo->stPdpAddr.enIpType)
               || (IMSA_IP_TYPE_IPV4V6 == pstLtePdpInfo->stPdpAddr.enIpType))
            {
                ucIpv4TypeFlag = 1;
                /* ����IPV4 IP ADDR */
                IMSA_MEM_CPY_S( pstPdnActReq->stPdpAddr.aucIpV4Addr,
                                 IMSA_IPV4_ADDR_LEN,
                               pstLtePdpInfo->stPdpAddr.aucIpV4Addr,
                               IMSA_IPV4_ADDR_LEN);
                /* ����IPV4 DNS��ַ */
                pstPdnActReq->stDns.cOpPriDns = pstLtePdpInfo->stPdpIpv4Dns.bitOpPriDns;
                IMSA_MEM_CPY_S(   pstPdnActReq->stDns.aucPriDns,
                                IMSA_IPV4_ADDR_LEN,
                                pstLtePdpInfo->stPdpIpv4Dns.aucPriDns,
                                IMSA_IPV4_ADDR_LEN);

                pstPdnActReq->stDns.cOpSecDns = pstLtePdpInfo->stPdpIpv4Dns.bitOpSecDns;
                IMSA_MEM_CPY_S(   pstPdnActReq->stDns.aucSecDns,
                               IMSA_IPV4_ADDR_LEN,
                                pstLtePdpInfo->stPdpIpv4Dns.aucSecDns,
                                IMSA_IPV4_ADDR_LEN);

                /* ����IPV4 PCSCF��ַ */
                pstPdnActReq->stPcscf.cOpPrimPcscfAddr = pstLtePdpInfo->stPdpIpv4Pcscf.bitOpPrimPcscfAddr;
                IMSA_MEM_CPY_S(   pstPdnActReq->stPcscf.aucPrimPcscfAddr,
                               IMSA_IPV4_ADDR_LEN,
                                pstLtePdpInfo->stPdpIpv4Pcscf.aucPrimPcscfAddr,
                                IMSA_IPV4_ADDR_LEN);


                pstPdnActReq->stPcscf.cOpSecPcscfAddr = pstLtePdpInfo->stPdpIpv4Pcscf.bitOpSecPcscfAddr;
                IMSA_MEM_CPY_S(   pstPdnActReq->stPcscf.aucSecPcscfAddr,
                               IMSA_IPV4_ADDR_LEN,
                                pstLtePdpInfo->stPdpIpv4Pcscf.aucSecPcscfAddr,
                                IMSA_IPV4_ADDR_LEN);


                pstPdnActReq->stPcscf.cOpThiPcscfAddr = pstLtePdpInfo->stPdpIpv4Pcscf.bitOpThiPcscfAddr;
                IMSA_MEM_CPY_S(   pstPdnActReq->stPcscf.aucThiPcscfAddr,
                              IMSA_IPV4_ADDR_LEN,
                                pstLtePdpInfo->stPdpIpv4Pcscf.aucThiPcscfAddr,
                                IMSA_IPV4_ADDR_LEN);

            }
            if((IMSA_IP_TYPE_IPV6 == pstLtePdpInfo->stPdpAddr.enIpType)
               || (IMSA_IP_TYPE_IPV4V6 == pstLtePdpInfo->stPdpAddr.enIpType))
            {

                if (0 != IMSA_MEM_CMP(aucTemp, pstLtePdpInfo->stPdpAddr.aucIpV6Addr, IMSA_IPV6_PREFIX_LEN))
                {
                    ucIpv6TypeFlag = 1;
                    /* ����IPV6 IP��ַ */
                    IMSA_MEM_CPY_S( pstPdnActReq->stPdpAddr.aucIpV6Addr,IMSA_IPV6_ADDR_LEN,
                               pstLtePdpInfo->stPdpAddr.aucIpV6Addr,IMSA_IPV6_ADDR_LEN);

                    /* ����IPV6 DNS��ַ */
                    pstPdnActReq->stIPv6Dns.cOpPriDns = pstLtePdpInfo->stPdpIpv6Dns.bitOpPriDns;
                    IMSA_MEM_CPY_S(   pstPdnActReq->stIPv6Dns.aucPriDns,
                                     IMSA_IPV6_ADDR_LEN,
                                    pstLtePdpInfo->stPdpIpv6Dns.aucPriDns,
                                    IMSA_IPV6_ADDR_LEN);

                    pstPdnActReq->stIPv6Dns.cOpSecDns = pstLtePdpInfo->stPdpIpv6Dns.bitOpSecDns;
                    IMSA_MEM_CPY_S(   pstPdnActReq->stIPv6Dns.aucSecDns,
                                    IMSA_IPV6_ADDR_LEN,
                                    pstLtePdpInfo->stPdpIpv6Dns.aucSecDns,
                                    IMSA_IPV6_ADDR_LEN);

                    /* ����IPV4 PCSCF��ַ */
                    pstPdnActReq->stIPv6Pcscf.cOpPrimPcscfAddr = pstLtePdpInfo->stPdpIpv6Pcscf.bitOpPrimPcscfAddr;
                    IMSA_MEM_CPY_S(   pstPdnActReq->stIPv6Pcscf.aucPrimPcscfAddr,
                                     IMSA_IPV6_ADDR_LEN,
                                    pstLtePdpInfo->stPdpIpv6Pcscf.aucPrimPcscfAddr,
                                    IMSA_IPV6_ADDR_LEN);


                    pstPdnActReq->stIPv6Pcscf.cOpSecPcscfAddr = pstLtePdpInfo->stPdpIpv6Pcscf.bitOpSecPcscfAddr;
                    IMSA_MEM_CPY_S(   pstPdnActReq->stIPv6Pcscf.aucSecPcscfAddr,
                                    IMSA_IPV6_ADDR_LEN,
                                    pstLtePdpInfo->stPdpIpv6Pcscf.aucSecPcscfAddr,
                                    IMSA_IPV6_ADDR_LEN);


                    pstPdnActReq->stIPv6Pcscf.cOpThiPcscfAddr = pstLtePdpInfo->stPdpIpv6Pcscf.bitOpThiPcscfAddr;
                    IMSA_MEM_CPY_S(   pstPdnActReq->stIPv6Pcscf.aucThiPcscfAddr,
                                     IMSA_IPV6_ADDR_LEN,
                                    pstLtePdpInfo->stPdpIpv6Pcscf.aucThiPcscfAddr,
                                    IMSA_IPV6_ADDR_LEN);
                }
                else
                {
                    if( IMSA_IP_TYPE_IPV4V6 == pstPdnActReq->stPdpAddr.enIpType)
                    {
                        pstPdnActReq->stPdpAddr.enIpType = IMSA_IP_TYPE_IPV4;
                    }
                }
            }
        }
        if( (1 == ucIpv6TypeFlag)&&(1 ==ucIpv4TypeFlag))
        {
            pstPdnActReq->enIpType      = IMSA_IP_TYPE_IPV4V6;
            pstPdnActReq->stPdpAddr.enIpType = IMSA_IP_TYPE_IPV4V6;
        }
        else if(1 ==ucIpv6TypeFlag)
        {
            pstPdnActReq->enIpType      = IMSA_IP_TYPE_IPV6;
            pstPdnActReq->stPdpAddr.enIpType = IMSA_IP_TYPE_IPV6;
        }
        else
        {
            pstPdnActReq->enIpType      = IMSA_IP_TYPE_IPV4;
            pstPdnActReq->stPdpAddr.enIpType = IMSA_IP_TYPE_IPV4;
        }
    }
    else
    {
       /* �������л�֮�У���������*/
    }


    if (IMSA_TRUE == bitOpApn)
    {
        /* ����APN����û�г����ֶΣ�����Ϊ���ַ�����ʽ�����ڽ�β����'\0' */
        IMSA_MEM_CPY_S( &pstPdnActReq->stApn,
                      sizeof(IMSA_WIFI_PDP_APN_STRU),
                      pstApnInfo,
                      sizeof(IMSA_WIFI_PDP_APN_STRU));
    }

    IMSA_SndMsgAtImsCtrlMsg(pstPdnActReq, sizeof(IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU));

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstPdnActReq);

}


/*lint -e18*/
VOS_VOID IMSA_CONN_SndWifiMsgPdnDeactiveReq(IMSA_WIFI_PDN_RELEASE_TYPE_ENUM_INT8 enRlsType)
{
    IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU   *pstPdnActReq;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstPdnActReq = IMSA_MEM_ALLOC(sizeof(IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU));

    if ( VOS_NULL_PTR == pstPdnActReq )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_CONN_SndWifiMsgPdnDeactiveReq:ERROR:Alloc Mem fail!");
        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_SndWifiMsgPdnDeactiveReq_ENUM, LNAS_NULL_PTR);
        return ;
    }

    /*���*/
    IMSA_MEM_SET_S(pstPdnActReq, sizeof(IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU), 0, sizeof(IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU));

    pstPdnActReq->lMsgId    = ID_IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ;
    pstPdnActReq->cIsLocal  = enRlsType;

    /* ��дAPN */
    /* todo */

    IMSA_SndMsgAtImsCtrlMsg(pstPdnActReq, sizeof(IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU));

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstPdnActReq);

}
/*lint +e18*/



void IMSA_CONN_ProcHoWifiMsgPdnActiveCnf
(
    const WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU* pstPdnActCnf
)
{
    IMSA_WIFI_NORMAL_CONN_STRU              *pstWifiNormalConn   = VOS_NULL_PTR;
    IMSA_WIFI_PDP_CNTXT_INFO_STRU           *pstPdpInfo          = VOS_NULL_PTR;
    IMSA_CONN_HO_EVENT_STRU                 *pstImsaHoEvent  = VOS_NULL_PTR;


    pstWifiNormalConn   = IMSA_CONN_GetWifiNormalConnAddr();

    pstImsaHoEvent      = IMSA_CONN_GetHoEventAddr();

    /* ����CONNING״̬����ֱ���˳� */
    if( (IMSA_CONN_STATUS_CONNING != IMSA_CONN_GetWifiNormalConnStatus()))
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcHoWifiMsgPdnActiveCnf: no CONNING State!");
        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcHoWifiMsgPdnActiveCnf_ENUM,IMSA_STATUS_NOT_CONNING);
        return;
    }

    if(IMSA_CONN_PDN_HO_STATE_HOING != pstImsaHoEvent->enPdnHoState)
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcHoWifiMsgPdnActiveCnf: not HOING state.");
        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcHoWifiMsgPdnActiveCnf_ENUM,IMSA_STATUS_NOT_HOING);
        return;
    }

    if (IMSA_FALSE == pstPdnActCnf->cIsEmergency)
    {
        if (WIFI_IMSA_PDN_ACT_RESULT_SUCCESS == pstPdnActCnf->enCause)
        {
            /*����A���������� */
            pstPdpInfo = &(pstWifiNormalConn->stSipSignalPdp);

            /* �洢WIFI����Я������Ϣ */
            IMSA_CONN_SaveWifiPdpInfo(pstPdpInfo, pstPdnActCnf);

            /* ֹͣIMS���Ŷ�ʱ�� */
            IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_ACTIVE_CNF);

#if 0
            /* ��Ҫ�ȸ�A�������������ó�����Ϣ��WIFI�ŻὫREADY IND��Ϣ�����������ԣ���ǰ��
            A�����ã�C���������������ñ������д��� */
            /* ��A�������������ó�����Ϣ */
            IMSA_SndMsgRnicWifiPdnActInd(pstPdpInfo);
            IMSA_SndMsgAtWifiPdpActInd(pstPdpInfo);
#endif
            /* �����ȴ�STATE IND��ʱ�� */
            IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_READY_IND);

        }
        else  /* ���ؽ���ʧ�� */
        {
            if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType)
            {
                /* ����ʧ�ܵĳ��� */
                /* ֹͣIMS���Ŷ�ʱ�� */
                IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_ACTIVE_CNF);

                /* ֪ͨSERVICEģ�� */
                /* ֪ͨCONNģ���л�����ʧ�� */
                IMSA_CONN_HoConnFail(IMSA_CONN_TYPE_NORMAL,
                                     IMSA_CONN_RESULT_FAIL_HO_NEW_PDN_SETUP_FAIL,
                                     IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,
                                     IMSA_IMS_DOMAIN_WIFI);
            }
            else
            {
                /* W2L */
                /* �����л��¼��е�ԭ��pdn�ͷű�־ */
                pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;

                /* ����ʧ�ܵĳ��� */
                /* ֹͣIMS���Ŷ�ʱ�� */
                IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_ACTIVE_CNF);

                /* ����ģ��״̬Ǩ��IDLE */
                IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

            }
        }

    }
    else
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcHoWifiMsgPdnActiveCnf:Emergency is not support at present.");

        TLPS_PRINT2LAYER_WARNING(IMSA_CONN_ProcHoWifiMsgPdnActiveCnf_ENUM,IMSA_EMS_NOT_SUPPORT_IN_VOWIFI_NOW);
    }

}


void IMSA_CONN_ProcNoHoWifiMsgPdnActiveCnf
(
    const WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU* pstPdnActCnf
)
{
    IMSA_WIFI_NORMAL_CONN_STRU              *pstWifiNormalConn   = VOS_NULL_PTR;
    IMSA_WIFI_PDP_CNTXT_INFO_STRU           *pstPdpInfo          = VOS_NULL_PTR;

    pstWifiNormalConn                   = IMSA_CONN_GetWifiNormalConnAddr();

    /* ����CONNING״̬����ֱ���˳� */
    if( (IMSA_CONN_STATUS_CONNING != IMSA_CONN_GetWifiNormalConnStatus()))
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcNoHoWifiMsgPdnActiveCnf: no CONNING State!");

        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcNoHoWifiMsgPdnActiveCnf_ENUM,IMSA_STATUS_NOT_CONNING);
        return;
    }

    if (IMSA_FALSE == pstPdnActCnf->cIsEmergency)
    {
        if (WIFI_IMSA_PDN_ACT_RESULT_SUCCESS == pstPdnActCnf->enCause)
        {
            pstPdpInfo = &(pstWifiNormalConn->stSipSignalPdp);

            /* �洢WIFI����Я������Ϣ */
            IMSA_CONN_SaveWifiPdpInfo(pstPdpInfo, pstPdnActCnf);

            /* ֹͣIMS���Ŷ�ʱ�� */
            IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_ACTIVE_CNF);

#if 0
            /* ��Ҫ�ȸ�A�������������ó�����Ϣ��WIFI�ŻὫREADY IND��Ϣ�����������ԣ���ǰ��
            A�����ã�C���������������ñ������д��� */
            /* ��A�������������ó�����Ϣ */
            IMSA_SndMsgRnicWifiPdnActInd(pstPdpInfo);
            IMSA_SndMsgAtWifiPdpActInd(pstPdpInfo);
#endif

            /* �����ȴ�STATE IND��ʱ�� */
            IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_READY_IND);
        }
        else
        {
            /* ����ʧ�ܵĳ��� */
            /* ֹͣIMS���Ŷ�ʱ�� */
            IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_ACTIVE_CNF);

            /* ֪ͨSERVICEģ�� */
            /* ֪ͨCONNģ�齨��ʧ�� */
            IMSA_CONN_SetupConnFail(IMSA_CONN_TYPE_NORMAL,
                                    IMSA_CONN_RESULT_FAIL_TIMER_EXP,
                                    (TAF_PS_CAUSE_ENUM_UINT32)TAF_PS_CAUSE_BUTT,
                                    IMSA_IMS_DOMAIN_WIFI);
        }
    }
    else
    {
        /* wifi���ݲ�֧�ֽ������У����� */
    }


}



VOS_VOID IMSA_CONN_ProcWifiMsgPdnActivateCnf(WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU* pstPdnActCnf)
{
    IMSA_CONN_HO_EVENT_STRU                 *pstImsaHoEvent          = VOS_NULL_PTR;

    pstImsaHoEvent                      = IMSA_CONN_GetHoEventAddr();

    if(IMSA_CONN_PDN_HO_STATE_IDLE != pstImsaHoEvent->enPdnHoState)
    {
        IMSA_CONN_ProcHoWifiMsgPdnActiveCnf(pstPdnActCnf);
    }
    else
    {
        IMSA_CONN_ProcNoHoWifiMsgPdnActiveCnf(pstPdnActCnf);
    }
}


VOS_VOID IMSA_CONN_ProcHoWifiMsgPdnReadyInd
(
    const  WIFI_IMSA_IMS_PDN_READY_IND_STRU*  pstPdnReadyInd
)
{
    IMSA_WIFI_NORMAL_CONN_STRU              *pstWifiNormalConn   = VOS_NULL_PTR;
    IMSA_CONN_HO_EVENT_STRU                 *pstImsaHoEvent      = VOS_NULL_PTR;
    IMSA_CONN_STATUS_ENUM_UINT8              enLteConnStatus;
    IMSA_CONN_STATUS_ENUM_UINT8              enWifiConnStatus;


    pstWifiNormalConn   = IMSA_CONN_GetWifiNormalConnAddr();
    /*pstNormalConn       = IMSA_CONN_GetNormalConnAddr();*/
    pstImsaHoEvent       = IMSA_CONN_GetHoEventAddr();

    enLteConnStatus  = IMSA_CONN_GetNormalConnStatus();
    enWifiConnStatus = IMSA_CONN_GetWifiNormalConnStatus();

    /* ����CONNING״̬����ֱ���˳� */
    if( (IMSA_CONN_STATUS_CONNING != IMSA_CONN_GetWifiNormalConnStatus()))
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcHoWifiMsgPdnReadyInd: no CONNING State!");

        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcHoWifiMsgPdnReadyInd_ENUM,IMSA_STATUS_NOT_CONNING);
        return;
    }

    /* ֹͣ�ȴ�READY IND��ʱ�� */
    IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_READY_IND);

    IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_CONN);

    if(IMSA_CONN_PDN_HO_STATE_HOING != pstImsaHoEvent->enPdnHoState)
    {
        return;
    }

    /*HoState:HOING */
    /* L-->W */
    if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType)
    {
        /* �����л��¼�����pdn����ָʾ״̬ */
        pstImsaHoEvent->ucNewSignalPdnSetupEvent =  VOS_TRUE;

        /* ����������ص�IP��ַ��PCSCF���ǵ�ǰע��ʹ�õ�IP/PCSCF,ͬʱ���ں���ͨ��,������ͨ���л� */
        if( (VOS_TRUE == IMSA_CONN_IsWifiPdpInfoIncludeRegPara(IMSA_CONN_TYPE_NORMAL,&pstWifiNormalConn->stSipSignalPdp))
            && ( VOS_TRUE == IMSA_IsImsAudioChannelExist())
            && (VOS_FALSE == pstImsaHoEvent->ucOldPdnRelIndEvent))
        {

            IMSA_WARN_LOG("IMSA_CONN_ProcHoWifiMsgPdnReadyInd:VC Control HO event in voice call!");

            TLPS_PRINT2LAYER_WARNING(IMSA_CONN_ProcHoWifiMsgPdnReadyInd_ENUM,IMSA_VC_CTRL_HO_IN_VOICECALL);
            /* ֪ͨVC�����л� */
            IMSA_NtfVcHoEvent(IMSA_VC_MODEM_WIFI_IMS);
        }

        /*û���յ��ɵ�pdn�ͷŵ���Ϣ */
        if(VOS_FALSE == pstImsaHoEvent->ucOldPdnRelIndEvent)
        {
            /* ��־֧����������ͷžɵ�pdn */
            if(VOS_TRUE == pstImsaHoEvent->ucOriginalDomainPdnRlsFlag)
            {
                /* IDLE̬*/
                if(IMSA_CONN_STATUS_IDLE == enLteConnStatus)
                {
                    /* �л��ɹ� */
                    pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;
                }
                else if(IMSA_CONN_STATUS_RELEASING == enLteConnStatus)
                {
                    /*Conn releasing :abnormal,discard */
                    IMSA_WARN_LOG("IMSA_CONN_ProcHoWifiMsgPdnReadyInd:releasing");

                    TLPS_PRINT2LAYER_WARNING(IMSA_CONN_ProcHoWifiMsgPdnReadyInd_ENUM,IMSA_STATUS_RELEASING);
                    return;
                }
                else
                {
                    /* ֹͣIMS���Ŷ�ʱ�� */
                    IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_SIP_SIGAL_PDP_ORIG);

                    /* ֹͣ�ȴ�IPV6������ʱ�� */
                    IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_IPV6_INFO);

                    /* ����IMSȥ���Ŷ�ʱ�� */
                    IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_SIP_SIGAL_PDP_END);

                    /* ����APS�ͷ����� */
                    IMSA_CONN_RequestApsRelConn(IMSA_CONN_TYPE_NORMAL);

                    /*�����л��¼���ԭ��pdn�ͷ�״̬Ϊ�ɹ�����*/
                    pstImsaHoEvent->enPdnHoState = IMSA_CONN_PDN_HO_STATE_AFTER_HOSUCC;

                    return;

                 }
            }
            else
            {
                /* VoLTE�²����ڱ���ȥ����������ص����  ,ֱ�Ӹ���״̬,�л��ɹ� */
                pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;
            }

        }
        else
        {
           /* ���Ѿ��յ������pdn�ͷŵ���Ϣ ��ֱ�ӽ�����һ������ */
        }

        IMSA_CONN_HoConnSucc(IMSA_CONN_TYPE_NORMAL,IMSA_IMS_HO_TYPE_LTE2WIFI);
    }
    else
    {
       /* �����ǰ�л�Ϊw2L,�յ�wifi�µ�ready ind ��Ϣ����Ҫ�����л��¼�״̬��ͣ���ȴ���ʱ�� */

       if(IMSA_CONN_STATUS_CONNING == enWifiConnStatus )
       {
           /* ֹͣ�ȴ�READY IND��ʱ�� */
           IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_READY_IND);

           /* �����л��¼�ԭ��pdn״̬Ϊ�ȴ�ԭ��pdn�ͷ�ָʾ*/
           pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_FALSE;
       }
       else
       {
          /* ��������Ϣ���������� */
       }
    }
}


VOS_VOID IMSA_CONN_ProcNoHoWifiMsgPdnReadyInd
(
    const  WIFI_IMSA_IMS_PDN_READY_IND_STRU*  pstPdnReadyInd
)
{
    IMSA_WIFI_NORMAL_CONN_STRU         *pstWifiNormalConn       = VOS_NULL_PTR;

    pstWifiNormalConn         = IMSA_CONN_GetWifiNormalConnAddr();

    /* ֹͣ�ȴ�READY IND��ʱ�� */
    IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_READY_IND);

    if( (IMSA_CONN_STATUS_CONNING != IMSA_CONN_GetWifiNormalConnStatus()))
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcNoHoWifiMsgPdnReadyInd: no CONNING State!");

        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcHoWifiMsgPdnReadyInd_ENUM,IMSA_STATUS_NOT_CONNING);
        return;
    }

    /* ��REGģ�����õ�ַ�� */
    IMSA_CONN_ConfigWifiPdpInfo2Reg(IMSA_CONN_TYPE_NORMAL, &pstWifiNormalConn->stSipSignalPdp);

    /* ��REGģ������AP MAC��ַ */
    IMSA_CONN_ConfigApMacInfo2Reg(&pstWifiNormalConn->stSipSignalPdp);

    /* ��CDS�������й��˳��� */
    IMSA_CONN_SndCdsSetImsBearerReq(IMSA_CDS_IMS_BEARER_RAT_TYPE_IEEE_802_11);

    /* ע��״̬Ǩ����CONN״̬ */
    /* ֪ͨSERVICEģ�齨���ɹ� */
    IMSA_CONN_SetupConnSucc(IMSA_CONN_TYPE_NORMAL, IMSA_IMS_DOMAIN_WIFI);

}


VOS_VOID IMSA_CONN_ProcWifiMsgPdnReadyInd(WIFI_IMSA_IMS_PDN_READY_IND_STRU* pstPdnReadyInd)
{
    IMSA_CONN_HO_EVENT_STRU                 *pstImsaHoEvent          = VOS_NULL_PTR;

    pstImsaHoEvent       = IMSA_CONN_GetHoEventAddr();

    if(IMSA_CONN_PDN_HO_STATE_IDLE != pstImsaHoEvent->enPdnHoState)
    {
        IMSA_CONN_ProcHoWifiMsgPdnReadyInd(pstPdnReadyInd);
    }
    else
    {
        IMSA_CONN_ProcNoHoWifiMsgPdnReadyInd(pstPdnReadyInd);
    }
}


VOS_VOID IMSA_CONN_ProcWifiMsgPdnDeactiveCnf
(
    WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF_STRU* pstPdnDeactCnf
)
{
    IMSA_WIFI_NORMAL_CONN_STRU              *pstWifiNormalConn       = VOS_NULL_PTR;
    IMSA_CONN_HO_EVENT_STRU                 *pstImsaHoEvent          = VOS_NULL_PTR;

    pstWifiNormalConn                   = IMSA_CONN_GetWifiNormalConnAddr();
    pstImsaHoEvent                           = IMSA_CONN_GetHoEventAddr();

    IMSA_INFO_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf is entered!");

    TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,LNAS_ENTRY);

    /* ֹͣ�ȴ�DEACTIVE_CNF��ʱ�� */
    IMSA_CONN_StopTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_DEACTIVE_CNF);

    if(IMSA_CONN_STATUS_RELEASING != IMSA_CONN_GetWifiNormalConnStatus())
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf: no CONNING State!");

        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,IMSA_STATUS_NOT_CONNING);
        return;
    }


    if(IMSA_CONN_PDN_HO_STATE_HOING == pstImsaHoEvent->enPdnHoState)
    {

        IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

        /* ����ģ��״̬Ǩ��IDLE */
        IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

        if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType )
        {

            IMSA_INFO_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf:1 L2W HO fail!");

            TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,LNAS_FUNCTION_LABEL1);
            /* ֪ͨSERVICEģ���л�����ʧ�� */
            IMSA_CONN_HoConnFail(IMSA_CONN_TYPE_NORMAL,
                                 IMSA_CONN_RESULT_FAIL_TIMER_EXP,
                                 IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,
                                 IMSA_IMS_DOMAIN_WIFI);
            return;
        }
        else
        {
            IMSA_INFO_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf:2 W2L HO !");

            TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,LNAS_FUNCTION_LABEL2);
            /* W2L���յ�wifi deactive cnf��Ϣ�������л��¼���ԭ��pdn״̬Ϊ�Ѿ��յ�ԭ��pdn�ͷ�ָʾ */
            pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;
        }
    }
    else if(IMSA_CONN_PDN_HO_STATE_AFTER_HOSUCC == pstImsaHoEvent->enPdnHoState)
    {

        IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

        /* ����ģ��״̬Ǩ��IDLE */
        IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

        if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType)
        {

            IMSA_INFO_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf:3 L2W HO fail!");

            TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,LNAS_FUNCTION_LABEL3);
            /* ֪ͨSERVICEģ���л�����ʧ�� */
            IMSA_CONN_HoConnFail(IMSA_CONN_TYPE_NORMAL,
                                 IMSA_CONN_RESULT_FAIL_TIMER_EXP,
                                 IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,
                                 IMSA_IMS_DOMAIN_WIFI);
            return;
        }
        else
        {
            IMSA_INFO_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf:4 W2L HO success!");

            TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,LNAS_FUNCTION_LABEL4);
            /* WIFI2LTE���յ�PDN_DEACT_CNF,HO�ɹ� */
            IMSA_CONN_HoConnSucc(IMSA_CONN_TYPE_NORMAL,IMSA_IMS_HO_TYPE_WIFI2LTE);
            return;
        }
    }
    else if(IMSA_CONN_PDN_HO_STATE_AFTER_HOFAIL == pstImsaHoEvent->enPdnHoState)
    {

        IMSA_INFO_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf:5 HO fail!");

        TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,LNAS_FUNCTION_LABEL5);
        IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

        /* ����ģ��״̬Ǩ��IDLE */
        IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

        if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType)
        {
            /* ֪ͨSERVICEģ�� */
            /* ֪ͨCONNģ���л�����ʧ�� */
            IMSA_CONN_HoConnFail(IMSA_CONN_TYPE_NORMAL,
                                 IMSA_CONN_RESULT_FAIL_HO_NEW_PDN_SETUP_FAIL,
                                 IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,
                                 IMSA_IMS_DOMAIN_WIFI);
            return;
        }
        else
        {
           /* WIFI2LTE, abnormal case */
           return;
        }
    }
    else
    {
        /* NO HO process */

        IMSA_INFO_LOG("IMSA_CONN_ProcWifiMsgPdnDeactiveCnf:6 no Ho release!");

        TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcWifiMsgPdnDeactiveCnf_ENUM,LNAS_FUNCTION_LABEL6);
         /* ɾ��������Ϣǰ�������ͷų��ص����ͺ�CID */
        if (IMSA_SRV_STATUS_CONN_REG == IMSA_SRV_GetEmcSrvStatus() ||
            IMSA_SRV_STATUS_CONN_REG == IMSA_SRV_GetNormalSrvStatus())
        {
            IMSA_RegSaveRegedPara(IMSA_REG_TYPE_NORMAL,
                                IMSA_CONN_GetWifiNormalConnSelSdfParaAddr()->ucCid,
                                IMSA_CONN_GetWifiNormalConnSelSdfParaAddr()->enPdnType);
        }

        /* ɾ�����������Ϣ */
        IMSA_CONN_DeleteWifiPdpInfo2Reg(IMSA_CONN_TYPE_NORMAL, &pstWifiNormalConn->stSipSignalPdp);

    IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

    /* ɾ��CDS�ĳ��� */
    IMSA_CONN_SndCdsSetImsBearerReq(IMSA_CDS_IMS_BEARER_RAT_TYPE_IEEE_802_11);

    /* ����ģ��״̬Ǩ��IDLE */
    IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

        /* ֪ͨSERVICEģ�������ͷ� */
        /* ������� */
        IMSA_CONN_SndConnRelInd(    IMSA_CONN_TYPE_NORMAL,
                                    IMSA_CONN_SIP_PDP_TYPE_SIGNAL,
                                    IMSA_CONN_MEDIA_PDP_TYPE_BUTT,
                                    IMSA_IMS_DOMAIN_WIFI);
    }
}

VOS_VOID IMSA_CONN_ProcHoWifiMsgErrorInd
(
    const WIFI_IMSA_ERROR_IND_STRU* pstErrorInd
)
{
    IMSA_CONN_HO_EVENT_STRU                 *pstImsaHoEvent          = VOS_NULL_PTR;
    IMSA_CONN_STATUS_ENUM_UINT8              enConnStatus;


    enConnStatus                             = IMSA_CONN_GetWifiNormalConnStatus();
    pstImsaHoEvent                           = IMSA_CONN_GetHoEventAddr();

    /* ��ǰ���л���ΪLTE */
    if(IMSA_IMS_HO_TYPE_WIFI2LTE == pstImsaHoEvent->enPdnHoType)
    {
        /*û���յ��ɵ�pdn�ͷŵ���Ϣ*/
        if(VOS_FALSE == pstImsaHoEvent->ucOldPdnRelIndEvent)
        {
            /* ��־֧��ͨ��������ͷžɵ�pdn */
            if(VOS_TRUE == pstImsaHoEvent->ucOriginalDomainPdnRlsFlag)
            {
                if (IMSA_CONN_STATUS_IDLE == enConnStatus)
                {
                     /* �����¼�״̬*/
                    pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;

                }
                else if(IMSA_CONN_STATUS_RELEASING == enConnStatus)
                {
                    /* WIFI PDN�ͷŹ�����,�����κδ��� */
                }
                else
                {
                    /* CONN||CONNING, �����ͷŹ��� */
                    IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_DEACTIVE_CNF);

                    /* ����WIFI�ͷ����� */
                    IMSA_CONN_SndWifiMsgPdnDeactiveReq(IMSA_WIFI_PDN_RELEASE_TYPE_LOCAL);

                    IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_RELEASING);


                }
            }
            else
            {
                /* VoLTE�²����ڱ���ȥ����������ص����  ,ֱ�Ӹ���״̬*/
                pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;

                /* ��������״̬ΪIDLE */
                IMSA_CONN_SetConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE, IMSA_IMS_DOMAIN_WIFI);

                /* �������ʵ����Ϣ */
                IMSA_CONN_ClearRatResource(IMSA_CONN_TYPE_NORMAL,IMSA_IMS_DOMAIN_WIFI);
            }
        }
        else
        {
            pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;

            /* ��������״̬ΪIDLE */
            IMSA_CONN_SetConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE, IMSA_IMS_DOMAIN_WIFI);

            /* �������ʵ����Ϣ */
            IMSA_CONN_ClearRatResource(IMSA_CONN_TYPE_NORMAL,IMSA_IMS_DOMAIN_WIFI);
        }
    }
    else
    {
         /* L2W���յ�wifi��error ind��Ϣ����Ҫ�����л��¼����л�״̬ ��������һ������*/
         pstImsaHoEvent->enPdnHoState = IMSA_CONN_PDN_HO_STATE_AFTER_HOFAIL;


        IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_DEACTIVE_CNF);

         /* ����WIFI�ͷ����� */
        IMSA_CONN_SndWifiMsgPdnDeactiveReq(IMSA_WIFI_PDN_RELEASE_TYPE_LOCAL);

         IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_RELEASING);
    }
}


VOS_VOID IMSA_CONN_ProcNoHoWifiMsgErrorInd
(
    const WIFI_IMSA_ERROR_IND_STRU* pstErrorInd
)
{
    IMSA_WIFI_NORMAL_CONN_STRU              *pstWifiNormalConn       = VOS_NULL_PTR;

    pstWifiNormalConn                        = IMSA_CONN_GetWifiNormalConnAddr();

    if( (IMSA_CONN_STATUS_IDLE == pstWifiNormalConn->enImsaConnStatus)
        || ( IMSA_CONN_STATUS_RELEASING == pstWifiNormalConn->enImsaConnStatus))
    {
        /* Connect Manager is in idle and releasing status, no process and return directly */
        return;
    }

    IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_DEACTIVE_CNF);

    /* ����WIFI�ͷ����� */
    IMSA_CONN_SndWifiMsgPdnDeactiveReq(IMSA_WIFI_PDN_RELEASE_TYPE_LOCAL);

    IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_RELEASING);

}



VOS_VOID IMSA_CONN_ProcWifiMsgErrorInd
(
    WIFI_IMSA_ERROR_IND_STRU* pstErrorInd
)
{
    IMSA_CONN_HO_EVENT_STRU                 *pstImsaHoEvent  = VOS_NULL_PTR;

    pstImsaHoEvent                     = IMSA_CONN_GetHoEventAddr();

    if(IMSA_CONN_PDN_HO_STATE_IDLE == pstImsaHoEvent->enPdnHoState)
    {
         IMSA_CONN_ProcNoHoWifiMsgErrorInd(pstErrorInd);
    }
    else
    {
         IMSA_CONN_ProcHoWifiMsgErrorInd(pstErrorInd);
    }

}


VOS_VOID IMSA_CONN_ProcTimerOutActiveCnf
(
    const VOS_VOID *pTimerMsg
)
{
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType       = IMSA_CONN_TYPE_BUTT;
    IMSA_CONN_HO_EVENT_STRU             *pstImsaHoEvent  = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_ProcTimerOutActiveCnf is entered!");
    TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcTimerOutActiveCnf_ENUM, LNAS_ENTRY);

    pstImsaHoEvent       = IMSA_CONN_GetHoEventAddr();
    enConnType           = PS_GET_REL_TIMER_PARA(pTimerMsg);

    if (enConnType >= IMSA_CONN_TYPE_BUTT)
    {
        IMSA_ERR_LOG("IMSA_ProcTimerOutActiveCnf:Illegal Para!");

        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcTimerOutActiveCnf_ENUM, LNAS_PARAM_INVALID);
        return ;
    }

    /* ����CONNING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING, IMSA_IMS_DOMAIN_WIFI))
    {
        IMSA_WARN_LOG("IMSA_ProcTimerOutActiveCnf:Not CONNING state!");
        TLPS_PRINT2LAYER_WARNING(IMSA_CONN_ProcTimerOutActiveCnf_ENUM, IMSA_STATE_ERROR);
        return ;
    }

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_WAIT_WIFI_PDP_ACTIVE_CNF);

    /* ����wifi�ͷų��� */
    IMSA_CONN_SndWifiMsgPdnDeactiveReq(IMSA_WIFI_PDN_RELEASE_TYPE_NW_CMD);

    /* �����ȴ�PDP_DEACTIVE_CNF��ʱ�� */
    IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_WIFI_PDP_DEACTIVE_CNF);

    /* ��������״̬ΪRELEASING*/
    IMSA_CONN_SetConnStatus(enConnType, IMSA_CONN_STATUS_RELEASING, IMSA_IMS_DOMAIN_WIFI);

    /*������ڽ����л�,���л���¼��״̬���ظ�servģ�齨��ʧ�� */
    if(IMSA_CONN_PDN_HO_STATE_HOING == pstImsaHoEvent->enPdnHoState)
    {
        if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType)
        {
            /* L2W���յ�active cnf��ʱ��Ϣ�Ժ� ,���л�״̬��ʶ��Ϊʧ�ܺ����ȴ�wifi��deactive cnf��Ϣ�Ժ��ٴ���*/
            pstImsaHoEvent->enPdnHoState = IMSA_CONN_PDN_HO_STATE_AFTER_HOFAIL;
        }
        else
        {
            /* W2L���յ�act_cnf��ʱ��Ϣ�Ժ�, �ȴ��ͷŽ�� */
        }
    }
    else if(IMSA_CONN_PDN_HO_STATE_IDLE == pstImsaHoEvent->enPdnHoState)
    {
        /* ֪ͨCONNģ�齨��ʧ�� */
        IMSA_CONN_SetupConnFail(enConnType,
                                IMSA_CONN_RESULT_FAIL_TIMER_EXP,
                                (TAF_PS_CAUSE_ENUM_UINT32)TAF_PS_CAUSE_BUTT,
                                IMSA_IMS_DOMAIN_WIFI);

        #if (FEATURE_ON == FEATURE_PTM)
        IMSA_PdnConnFailErrLogRecord(   enConnType,
                                        IMSA_ERR_LOG_NORM_PDNREJ_CAUSE_ORIG_TIMER_EXP,
                                        IMSA_ERR_LOG_PDN_CONN_NW_REJ_CAUSE_NONE,
                                        IMSA_ERR_LOG_EMC_REG_FAIL_REASON_SIP_SIGAL_PDP_ORIG_TIMER_EXP);
        #endif
    }
    else
    {
       /* �����л�״̬��,��������ʧ��,�������κδ��� */
    }
    return;
}


VOS_VOID IMSA_CONN_ProcTimerOutReadyInd
(
    const VOS_VOID *pTimerMsg
)
{
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType      = IMSA_CONN_TYPE_BUTT;
    IMSA_CONN_HO_EVENT_STRU             *pstImsaHoEvent  = VOS_NULL_PTR;


    IMSA_INFO_LOG("IMSA_ProcTimerOutActiveCnf is entered!");
    TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcTimerOutReadyInd_ENUM, LNAS_ENTRY);

    enConnType = PS_GET_REL_TIMER_PARA(pTimerMsg);
    pstImsaHoEvent       = IMSA_CONN_GetHoEventAddr();

    if (enConnType >= IMSA_CONN_TYPE_BUTT)
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcTimerOutReadyInd:Illegal Para!");

        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcTimerOutReadyInd_ENUM, LNAS_PARAM_INVALID);
        return ;
    }

    /* ����CONNING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING, IMSA_IMS_DOMAIN_WIFI))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTimerOutReadyInd:Not CONNING state!");
        TLPS_PRINT2LAYER_WARNING(IMSA_CONN_ProcTimerOutReadyInd_ENUM, IMSA_STATE_ERROR);
        return ;
    }

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_WAIT_WIFI_PDP_READY_IND);

    /* �ͷ�WIFI PDN���� */
    IMSA_CONN_RlsWifiPdn();

    /*������ڽ����л�,���л���¼��״̬���ظ�servģ�齨��ʧ�� */
    if(IMSA_CONN_PDN_HO_STATE_HOING == pstImsaHoEvent->enPdnHoState)
    {
        if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType)
        {
            /* L2W���յ�ready ind��ʱ��Ϣ�Ժ� ,���л�״̬��ʶ��Ϊʧ�ܺ����ȴ�wifi��deactive cnf��Ϣ�Ժ��ٴ���*/
            pstImsaHoEvent->enPdnHoState = IMSA_CONN_PDN_HO_STATE_AFTER_HOFAIL;
        }
        else
        {
            /* W2L�±�������Ҫ�ͷ�WIFI PDN,�������������� */
        }
    }
    else if(IMSA_CONN_PDN_HO_STATE_IDLE == pstImsaHoEvent->enPdnHoState)
    {
        /*�ȴ���·�ͷ� */
    }
    else
    {
        /* �����л�״̬��,��������ʧ��,�������κδ��� */
    }
    return;
}


VOS_VOID IMSA_CONN_ProcTimerOutDeactcnf
(
    const VOS_VOID *pTimerMsg
)
{
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType              = IMSA_CONN_TYPE_BUTT;
    IMSA_WIFI_NORMAL_CONN_STRU         *pstWifiNormalConn       = VOS_NULL_PTR;
    IMSA_CONN_HO_EVENT_STRU             *pstImsaHoEvent  = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CONN_ProcTimerOutDeactcnf is entered!");
    TLPS_PRINT2LAYER_INFO(IMSA_CONN_ProcTimerOutDeactcnf_ENUM, LNAS_ENTRY);

    enConnType = PS_GET_REL_TIMER_PARA(pTimerMsg);
    pstWifiNormalConn = IMSA_CONN_GetWifiNormalConnAddr();
    pstImsaHoEvent       = IMSA_CONN_GetHoEventAddr();

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_WAIT_WIFI_PDP_DEACTIVE_CNF);

    if (enConnType != IMSA_CONN_TYPE_NORMAL)
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcTimerOutDeactcnf:Illegal Para!");
        TLPS_PRINT2LAYER_ERROR(IMSA_CONN_ProcTimerOutDeactcnf_ENUM, LNAS_PARAM_INVALID);

        return ;
    }

    /* ����RELEASING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_RELEASING, IMSA_IMS_DOMAIN_WIFI))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTimerOutDeactcnf:Not releasing state!");
        TLPS_PRINT2LAYER_WARNING(IMSA_CONN_ProcTimerOutDeactcnf_ENUM, IMSA_STATE_ERROR);
        return ;
    }

    if(IMSA_CONN_PDN_HO_STATE_HOING == pstImsaHoEvent->enPdnHoState)
    {
        if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType )
        {
            /* ֪ͨSERVICEģ�� */
            /* ֪ͨCONNģ���л�����ʧ�� */
            IMSA_CONN_HoConnFail(IMSA_CONN_TYPE_NORMAL,
                                 IMSA_CONN_RESULT_FAIL_TIMER_EXP,
                                 IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,
                                 IMSA_IMS_DOMAIN_WIFI);

        }
        else
        {
            /* W2L���յ�wifi deactive cnf��Ϣ�������л��¼���ԭ��pdn״̬Ϊ�Ѿ��յ�ԭ��pdn�ͷ�ָʾ */
            pstImsaHoEvent->ucOldPdnRelIndEvent = VOS_TRUE;

            IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

            /* ����ģ��״̬Ǩ��IDLE */
            IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

        }
    }

    else if(IMSA_CONN_PDN_HO_STATE_AFTER_HOSUCC == pstImsaHoEvent->enPdnHoState)
    {
        if(IMSA_IMS_HO_TYPE_WIFI2LTE == pstImsaHoEvent->enPdnHoType)
        {
            /* WIFI2LTE���յ�PDN_DEACT_CNF,HO�ɹ� */
            IMSA_CONN_HoConnSucc(IMSA_CONN_TYPE_NORMAL,IMSA_IMS_HO_TYPE_WIFI2LTE);

            IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

            /* ����ģ��״̬Ǩ��IDLE */
            IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

            return;
        }
        else
        {
            /* ֪ͨSERVICEģ���л�����ʧ�� */
            IMSA_CONN_HoConnFail(IMSA_CONN_TYPE_NORMAL,
                                 IMSA_CONN_RESULT_FAIL_TIMER_EXP,
                                 IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,
                                 IMSA_IMS_DOMAIN_WIFI);
            /* �쳣 */
            IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

            /* ����ģ��״̬Ǩ��IDLE */
            IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);
            return;
        }
    }
    else if(IMSA_CONN_PDN_HO_STATE_AFTER_HOFAIL == pstImsaHoEvent->enPdnHoState)
    {
        if(IMSA_IMS_HO_TYPE_LTE2WIFI == pstImsaHoEvent->enPdnHoType )
        {
            /* ֪ͨSERVICEģ�� */
            /* ֪ͨCONNģ���л�����ʧ�� */
            IMSA_CONN_HoConnFail(IMSA_CONN_TYPE_NORMAL,
                                 IMSA_CONN_RESULT_FAIL_WIFI_READY_IND_TIMEOUT,
                                 IMSA_DOMAIN_SWITCH_TYPE_L2W_SWITCH,
                                 IMSA_IMS_DOMAIN_WIFI);

            IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

            /* ����ģ��״̬Ǩ��IDLE */
            IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);
        }
        else
        {
           IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

           /* ����ģ��״̬Ǩ��IDLE */
           IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);
           return;
        }
    }
    else
    {
        /* �л�״̬Ϊinactive ,ֱ�ӽ�������Ĵ���*/

        /* ɾ�����������Ϣ */
        IMSA_CONN_DeleteWifiPdpInfo2Reg(IMSA_CONN_TYPE_NORMAL, &pstWifiNormalConn->stSipSignalPdp);

        IMSA_CONN_ClearWifiConnResource(IMSA_CONN_TYPE_NORMAL);

        /* ֪ͨCDS���й��˳��� */
        IMSA_CONN_SndCdsSetImsBearerReq(IMSA_CDS_IMS_BEARER_RAT_TYPE_IEEE_802_11);

        IMSA_CONN_SetWifiConnStatus(IMSA_CONN_TYPE_NORMAL,IMSA_CONN_STATUS_IDLE);

        /* ֪ͨSERVICEģ�������ͷ� */
        /* ������� */
        IMSA_CONN_SndConnRelInd(    IMSA_CONN_TYPE_NORMAL,
                                    IMSA_CONN_SIP_PDP_TYPE_SIGNAL,
                                    IMSA_CONN_MEDIA_PDP_TYPE_BUTT,
                                    IMSA_IMS_DOMAIN_WIFI);
    }
}


#endif


#ifdef __cplusplus
    #if __cplusplus
            }
    #endif
#endif
    /* end of ImsaProcApsMsg.c*/


