

#ifndef __NAS_ERRLOG_CCB_CTX_H__
#define __NAS_ERRLOG_CCB_CTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include  "NasErrorLogCdma.h"
#include  "NasErrorLogGu.h"
#include  "NasMntn.h"
#include  "omnvinterface.h"
#include  "nas_errlog_cfg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define NAS_ERRLOG_CCB_INVALID_SID                          (0xFFFF)  /* SID����Чֵ */
#define NAS_ERRLOG_CCB_INVALID_NID                          (0xFFFF)  /* NID����Чֵ */
#define NAS_ERRLOG_CCB_INVALID_BASEID                       (0xFFFF)  /* BaseID����Чֵ */

#define NAS_ERRLOG_CCB_CL_OOS_MAX_SRCH_RECORD_NUM           (20)   /* CL��ģ�������̣�����¼�������� */

#define NAS_ERRLOG_CCB_1X_CS_CALL_MAX_REDIAL_NUM            (10)   /* CS���й����У��ز���Ϣ��¼���� */

#define NAS_ERRLOG_CCB_1X_OOS_MAX_SEARCH_RECORD_NUM         (20)   /* 1x�������̣�����¼�������� */

#define NAS_ERRLOG_CCB_HRPD_OOS_MAX_SEARCH_RECORD_NUM       (20)   /* Hrpd�������̣�����¼�������� */

#define NAS_ERRLOG_CCB_1X_MT_MAX_EXCEPTION_RECORD_NUM       (4)    /* 1x���У�����¼��Ŀ����cs/ps/csms/agps */

#define NAS_ERRLOG_CCB_INVALID_MCC                          (0xffffffff)
#define NAS_ERRLOG_CCB_INVALID_MNC                          (0xffffffff)

#define CNAS_COMM_BULID_ERRLOG_HEADER_INFO(pstHeader, ModemId, AlmId, AlmLevel, ulSlice, ulLength) \
{ \
    (pstHeader)->ulMsgModuleId     = OM_ERR_LOG_MOUDLE_ID_CNAS; \
    (pstHeader)->usModemId         = ModemId; \
    (pstHeader)->usAlmId           = AlmId; \
    (pstHeader)->usAlmLevel        = AlmLevel; \
    (pstHeader)->usAlmType         = NAS_ERR_LOG_ALM_TYPE_COMMUNICATION; \
    (pstHeader)->usAlmLowSlice     = ulSlice; \
    (pstHeader)->usAlmHighSlice    = 0; \
    (pstHeader)->ulAlmLength       = ulLength; \
}

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/



enum NAS_ERRLOG_CCB_SESSION_EXCEP_DEACT_CAUSE_ENUM
{
    NAS_ERRLOG_CCB_SESSION_EXCEP_DEACT_CAUSE_BY_AN                  = 0,
    NAS_ERRLOG_CCB_SESSION_EXCEP_DEACT_CAUSE_BY_KEEPALIVE_TIMEOUT   = 1,
    NAS_ERRLOG_CCB_SESSION_EXCEP_DEACT_CAUSE_BY_SESSION_NEG_FAIL    = 2,
    NAS_ERRLOG_CCB_SESSION_EXCEP_DEACT_CAUSE_BUTT
};
typedef VOS_UINT32 NAS_ERRLOG_CCB_SESSION_EXCEP_DEACT_CAUSE_ENUM_UINT32;

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/


typedef struct
{
    VOS_UINT8                           ucErrLogActiveReportAlarmFlag;
    VOS_UINT8                           ucErrLogActiveReportAlarmLevel;
    VOS_UINT8                           aucReserved[2];
}NAS_ERRLOG_CCB_ACTIVE_REPORT_CONTROL_INFO_STRU;


typedef struct
{
    NAS_ERRLOG_CCB_ACTIVE_REPORT_CONTROL_INFO_STRU           stNasErrLogCcbMmaActiveReportControl;
    NAS_ERRLOG_CCB_ACTIVE_REPORT_CONTROL_INFO_STRU           stNasErrLogCcbXccActiveReportControl;
    NAS_ERRLOG_CCB_ACTIVE_REPORT_CONTROL_INFO_STRU           stNasErrLogCcbHsmActiveReportControl;
    NAS_ERRLOG_CCB_ACTIVE_REPORT_CONTROL_INFO_STRU           stNasErrLogCcbXregActiveReportControl;
    NAS_ERRLOG_CCB_ACTIVE_REPORT_CONTROL_INFO_STRU           stNasErrLogCcbXsmsActiveReportControl;
}NAS_ERRLOG_CCB_CDMA_MAIN_ACTIVE_REPORT_CONTROL_STRU;


typedef struct
{
    VOS_UINT8                           uc1xOosReportEnable;
    VOS_UINT8                           uc1xMtServiceExceptionReportEnable;
    VOS_UINT8                           ucHrpdUatiFailReportEnable;
    VOS_UINT8                           ucHrpdSessionFailReportEnable;
    VOS_UINT8                           ucHrpdSessionExcepDeactReportEnable;
    VOS_UINT8                           ucHrpdOrLteOosReportEnable;
    VOS_UINT8                           ucXregResltReportEnable;
    VOS_UINT8                           ucXsmsReportEnable;
}NAS_ERRLOG_CCB_CDMA_SELF_ACTIVE_REPORT_CONTROL_STRU;


typedef struct
{
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
    VOS_UINT16                          usBaseId;
    VOS_UINT8                           aucReserve[2];
    VOS_INT32                           lLatitude;
    VOS_INT32                           lLongitude;
    VOS_UINT16                          usBand;
    VOS_UINT16                          usFreq;
}NAS_ERRLOG_CCB_1X_COMMON_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucColorCode;
    VOS_UINT8                           ucSubnetMask;
    VOS_UINT8                           aucReserve[2];
    VOS_UINT8                           aucSectorId[NAS_ERR_LOG_HRPD_SECTORID_LENGTH];
    VOS_UINT32                          ulSector24;
    VOS_UINT16                          usBand;
    VOS_UINT16                          usFreq;
}NAS_ERRLOG_CCB_HRPD_COMMON_INFO_STRU;


typedef struct
{
    MNTN_PLMN_ID_STRU                   stPlmn;          /* LTE��ǰ������PLMN */
    VOS_UINT16                          usLac;
    VOS_UINT16                          usRac;
    VOS_UINT32                          ulLteArfcn;      /* LTEפ��Ƶ����Ϣ */
    VOS_UINT32                          ulCellId;
}NAS_ERRLOG_CCB_CL_LTE_COMMON_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucAlreadyReportChrOosFlag;      /* �Ƿ��ϱ���CHR��ʶ���ϱ���ΪTRUE */
    VOS_UINT8                           ucAlreadyReportOosToApFlag;      /* ���ֶ����ϱ��Ƿ��û�����֪������֪ΪTRUE�����ӳ��ϱ���ʱ����ʱ��δ�ѵ��� */
    VOS_UINT8                           ucReserved[2];
}NAS_ERRLOG_CCB_1X_OOS_RECOVERY_STRU;


typedef struct
{
    VOS_UINT32                                              ulIndex;        /* ����������ڼ��������Ľ�� */
    VOS_UINT32                                              ulStartTimer;   /* ����������ʼʱ�� */
    VOS_UINT32                                              ulEndTimer;     /* ������������ʱ�� */
    NAS_ERR_LOG_1X_SEARCH_RSLT_ENUM_UINT32                  enSearchRslt;   /* ����������� */
} NAS_ERRLOG_CCB_1X_OOS_SEARCH_RECORD_STRU;


typedef struct
{
    VOS_UINT32                                              ulSearchRecordCnt;      /*�������Ĵ���*/
    NAS_ERRLOG_CCB_1X_OOS_SEARCH_RECORD_STRU                ast1xOosSearchRecord[NAS_ERRLOG_CCB_1X_OOS_MAX_SEARCH_RECORD_NUM];    /* ������¼ */
}NAS_ERRLOG_CCB_1X_OOS_SEARCH_STRU;


typedef struct
{
    VOS_UINT8                           ucAlreadyReportChrOosFlag;       /* �Ƿ��ϱ���CHR��ʶ���ϱ���ΪTRUE */
    VOS_UINT8                           ucAlreadyReportOosToApFlag;      /* ���ֶ����ϱ��Ƿ��û�����֪������֪ΪTRUE�����ӳ��ϱ���ʱ����ʱ��δ�ѵ��� */
    VOS_UINT8                           ucReserved[2];
}NAS_ERRLOG_CCB_HRPD_OR_LTE_OOS_RECOVERY_STRU;


typedef struct
{
    VOS_UINT32                                              ulIndex;        /* ����������ڼ��������Ľ�� */
    VOS_UINT32                                              ulStartTimer;   /* ����������ʼʱ�� */
    VOS_UINT32                                              ulEndTimer;     /* ������������ʱ�� */
    NAS_ERR_LOG_HRPD_SEARCH_RSLT_ENUM_UINT32                enSearchRslt;   /* ����������� */
} NAS_ERRLOG_CCB_HRPD_OOS_SEARCH_RECORD_STRU;


typedef struct
{
    VOS_UINT32                                              ulSearchRecordCnt;      /*�������Ĵ���*/
    NAS_ERRLOG_CCB_HRPD_OOS_SEARCH_RECORD_STRU              astHrpdOosSearchRecord[NAS_ERRLOG_CCB_HRPD_OOS_MAX_SEARCH_RECORD_NUM];    /* ������¼ */
}NAS_ERRLOG_CCB_HRPD_OOS_SEARCH_STRU;


typedef struct
{
    NAS_ERR_LOG_HRPD_OOS_CAUSE_ENUM_UINT32                  enOosCause;
}NAS_ERRLOG_CCB_HRPD_OOS_REPORT_STRU;


typedef struct
{
    NAS_ERR_LOG_XREG_REGISTRATION_TYPE_ENUM_UINT8           enRegType;              /* ��ǰע������ */
    NAS_ERR_LOG_XREG_FAIL_REASON_ENUM_UINT8                 enRegFailReason;        /* ע��ʧ��ԭ��ֵ */
    VOS_UINT8                                               ucRsv[2];
}NAS_ERRLOG_CCB_XREG_FAIL_INFO_STRU;


typedef struct
{
    VOS_UINT32                                      enFailState;            /* ���������ʱ���״̬��״̬ TAF_XSMS_MO_FSM_ID_ENUM_UINT32 */
    VOS_UINT32                                      enLastXccCnfRslt;       /* ���һ�ε�XCC RESULT ֵ�ο�:TAF_XCC_CAUSE_ENUM_UINT32 */
    VOS_UINT8                                       enLastCasCnfRslt;       /* ���һ�ε�CAS RESULT ֵ�ο�:CAS_CNAS_1X_DATA_CNF_RSLT_ENUM_UINT8 */
    VOS_UINT8                                       enLastTlAckCauseCode;   /* �յ�TL_ACK �����CAUSE CODE��û�д���дBUTTֵ TAF_XSMS_TL_CAUSE_CODE_ENUM8*/
    NAS_ERR_LOG_XSMS_NO_TL_ACK_REASON_ENUM_UINT8    enNoTlAckReason;        /* û���յ�TL_ACK�� ����ԭ�� */
    NAS_ERR_LOG_XSMS_FAIL_REASON_ENUM_UINT8         enSndFailReason;        /* ����ʧ��ԭ�� */
    VOS_UINT32                                      ulReSendTime;

}NAS_ERRLOG_CCB_XSMS_SND_FAIL_INFO_STRU;


typedef struct
{
    NAS_ERR_LOG_XSMS_FAIL_REASON_ENUM_UINT8         enFailReason;         /* MTʧ��ԭ��ֵ */
    VOS_UINT8                                       ucReRcvTime;          /* MTʧ�ܴ��� */
    VOS_UINT8                                       enLastCasCnfRslt;     /* ֵ�ο�:CAS_CNAS_1X_DATA_CNF_RSLT_ENUM_UINT8  */
    VOS_UINT8                                       ucRcv;
}NAS_ERRLOG_CCB_XSMS_RCV_FAIL_INFO_STRU;


typedef NAS_ERR_LOG_CDMA_PS_CALL_REDIAL_INFO_STRU  NAS_ERRLOG_CCB_CDMA_PS_CALL_REDIAL_INFO_STRU;


typedef NAS_ERR_LOG_APS_PDP_ENTITY_FSM_INFO_STRU   NAS_ERRLOG_CCB_APS_PDP_ENTITY_FSM_INFO_STRU;


typedef struct
{
    VOS_UINT32                                              ulPsCause;     /* ���һ��ʧ��ԭ�� */
    NAS_ERRLOG_CCB_APS_PDP_ENTITY_FSM_INFO_STRU             stApsFsmInfo;  /* ״̬����Ϣ */
    VOS_UINT32                                              ulRedialCnt;   /* �ز����� */
    NAS_ERRLOG_CCB_CDMA_PS_CALL_REDIAL_INFO_STRU            astRedialInfo[NAS_ERR_LOG_MAX_CDMA_PS_CALL_EXCEP_REDIAL_NUM];
}NAS_ERRLOG_CCB_CDMA_PS_CALL_EXCEPTION_STRU;


typedef struct
{
    VOS_UINT32                          ulMainFsmId;
    VOS_UINT32                          ulMainState;
    VOS_UINT32                          ulSubFsmId;
    VOS_UINT32                          ulSubState;
    VOS_UINT8                           ucPdpId;
    VOS_UINT8                           ucReserved[2];
}NAS_ERRLOG_PDP_ENTITY_FSM_CTX_STRU;


typedef NAS_ERR_LOG_CDMA_PS_DISC_RECORD_STRU  NAS_ERRLOG_CCB_CDMA_PS_DISC_RECORD_STRU;


typedef struct
{
    VOS_UINT32                                              ulCdmaPsDiscRecdCnt;   /* PS�����󣬷��������ؽ�ʧ�ܴ��� */
    NAS_ERRLOG_CCB_CDMA_PS_DISC_RECORD_STRU                 astErrLogCdmaPsDiscRecd[NAS_ERR_LOG_MAX_CDMA_PS_DISC_RECORD_NUM];    /* PS�����쳣��Ϣ */
}NAS_ERRLOG_CCB_CDMA_PS_DISC_INFO_STRU;


typedef NAS_ERR_LOG_HRPD_UATI_ASSIGN_FAIL_RECORD_STRU NAS_ERRLOG_CCB_HRPD_UATI_ASSIGN_FAIL_RECORD_STRU;


typedef struct
{
    VOS_UINT32                                              ulUatiStartTimer;    /* ����UATI����Ŀ�ʼʱ�� */
    VOS_UINT32                                              ulUatiEndTimer;      /* UATI�˳�״̬��������chr�¼���ʱ�� */
    VOS_UINT8                                               ucTriggerCause;      /* ����UATI��ԭ��,��CANS_HSM_UATI_REQ_TRIGGER_ENUM_UINT8ת���õ� */
    VOS_UINT8                                               ucUATIFailCause;     /* UATIʧ�ܵ�ԭ����CNAS_HSM_UATI_REQUEST_FAIL_REASON_ENUM_UINT8ת���õ� */
    VOS_UINT8                                               ucRecordCnt;
    VOS_UINT8                                               ucReserved;
    NAS_ERRLOG_CCB_HRPD_UATI_ASSIGN_FAIL_RECORD_STRU        astUatiAssignFailRecord[NAS_ERR_LOG_HRPD_UATI_ASSIGN_MAX_CNT];
}NAS_ERRLOG_CCB_HRPD_UATI_FAIL_STRU;


typedef struct
{
    VOS_UINT8                           ucSessionActReason;      /* ����Session�����ԭ��,��CNAS_HSM_SESSION_ACTIVE_REASON_ENUM_UINT8ת���õ� */
    VOS_UINT8                           ucSessionActType;       /* Session��������ͣ�����recovery��init��restore prio,��CNAS_HSM_HRPD_SCP_SCT_TYPE_ENUM_UINT8ת���õ� */
    VOS_UINT8                           ucSupportedEhrpdFlg;    /* UE֧�ַ���ehrpd������ */
    VOS_UINT8                           ucSessionFailCause;     /* Sessionʧ�ܵ�ԭ����CNAS_HSM_SESSION_ACT_FAIL_REASON_ENUM_UINT8ת���õ� */
    VOS_UINT32                          ulStartTimer;
    VOS_UINT32                          ulEndTimer;
}NAS_ERRLOG_CCB_HRPD_SESSION_FAIL_STRU;



typedef struct
{
    NAS_ERRLOG_CCB_HRPD_UATI_FAIL_STRU                      stHrpdUatiFail;    /* UATI����ʧ�� */
    NAS_ERRLOG_CCB_HRPD_SESSION_FAIL_STRU                   stHrpdSessionFail; /* Session����ʧ�� */
}NAS_ERRLOG_CCB_HRPD_SESSION_INFO_STRU;



typedef NAS_ERR_LOG_CL_OOS_SEARCH_RECORD_STRU NAS_ERRLOG_CCB_CL_OOS_SEARCH_RECORD_STRU;


typedef struct
{
    VOS_UINT32                                              ulIsHrpdAndLteBothSupport; /* ��ǰϵͳ�Ƿ�ͬʱ֧��HRPD��LTE, TRUE��֧�֣�FALSE����֧�� */
    VOS_UINT32                                              ulSearchRecordCnt;
    NAS_ERRLOG_CCB_CL_OOS_SEARCH_RECORD_STRU                astSearchRecord[NAS_ERRLOG_CCB_CL_OOS_MAX_SRCH_RECORD_NUM];
}NAS_ERRLOG_CCB_CL_OOS_SEARCH_INFO_STRU;


typedef struct
{
    VOS_UINT32                          ulIndex;         /* �ṹ����������*/
    VOS_UINT32                          ulStartTimer;    /* ���ν�����ʼʱ��*/
    VOS_UINT32                          ulEndTimer;      /* ���ν�������ʱ��*/
    VOS_UINT32                          ulRedialRslt;    /* ʧ��ԭ��ֵ */
}NAS_ERRLOG_CCB_1X_CS_CALL_REDIAL_INFO_STRU;


typedef struct
{
    VOS_UINT32                                              ulCcCause;           /* �������һ��ʧ��ԭ��ֵ */
    VOS_UINT32                                              ulRedialCnt;         /* �ز�����*/
    NAS_ERRLOG_CCB_1X_CS_CALL_REDIAL_INFO_STRU              astRedialInfo[NAS_ERRLOG_CCB_1X_CS_CALL_MAX_REDIAL_NUM];
}NAS_ERRLOG_CCB_1X_CS_CALL_EXCEPTION_STRU;


typedef struct
{
    NAS_ERR_LOG_1X_MT_SERVICE_EXCEPTION_CAUSE_ENUM_UINT32   enCcCause;      /* �����е�ԭ��ֵ����չ */
    VOS_UINT32                                              ulPagingTimer;  /* ���յ�Paging��Ϣ��ʱ�� */
    NAS_ERR_LOG_1X_SERVICE_TYPE_ENUM_UINT32                 enPagingType;   /* ����ҵ�����ͣ��������������ź�����*/
    VOS_UINT32                                              ulXccState;     /* ����쳣������XCCģ��ı���״̬���У�����XCC��ǰ����״̬*/
}NAS_ERRLOG_CCB_1X_MT_EXCEPTION_RECORD_STRU;


typedef struct
{
    VOS_UINT8                                               ucRecordIndex;
    VOS_UINT8                                               aucReserved[3];
    NAS_ERRLOG_CCB_1X_MT_EXCEPTION_RECORD_STRU              ast1xMtExceptionRecord[NAS_ERRLOG_CCB_1X_MT_MAX_EXCEPTION_RECORD_NUM];
}NAS_ERRLOG_CCB_1X_MT_SERVICE_EXCEPTION_STRU;


typedef struct
{
    MODEM_ID_ENUM_UINT16                enCdmaModeModemId;
    VOS_UINT8                           aucReserve[2];
}NAS_ERRLOG_CCB_MODEM_INFO_STRU;


typedef struct
{
    NAS_ERRLOG_CCB_MODEM_INFO_STRU                          stModemInfo;

    NAS_ERRLOG_CCB_CDMA_MAIN_ACTIVE_REPORT_CONTROL_STRU     stNasErrLogCcbMainActiveReportControl;  /*�ϱ��ܿ�������*/

    NAS_ERRLOG_CCB_CDMA_SELF_ACTIVE_REPORT_CONTROL_STRU     stNasErrLogCcbCdmaActiveReportControl;  /*�ϱ�CDMA��������*/

    NAS_ERRLOG_CCB_1X_COMMON_INFO_STRU                      stNasErrLogCcb1xCommonInfo;             /*1x������Ϣ*/

    NAS_ERRLOG_CCB_HRPD_COMMON_INFO_STRU                    stNasErrLogCcbHrpdCommonInfo;           /*hrpd������Ϣ*/
    NAS_ERRLOG_CCB_CL_LTE_COMMON_INFO_STRU                  stNasErrLogCcbLteCommonInfo;            /* LTE������Ϣ */
    NAS_ERRLOG_CCB_1X_OOS_RECOVERY_STRU                     stNasErrLogCcb1xOosRecovery;            /*1x�����ָ��ϱ��ṹ��*/

    NAS_ERRLOG_CCB_1X_OOS_SEARCH_STRU                       stNasErrLogCcb1xOosSearchRecord;        /*1x������¼*/

    NAS_ERRLOG_CCB_1X_CS_CALL_EXCEPTION_STRU                stNasErrLogCcb1xCsOrigCallException;    /*1x����ʧ���쳣��Ϣ�ṹ��*/

    NAS_ERRLOG_CCB_1X_CS_CALL_EXCEPTION_STRU                stNasErrLogCcb1xConversationException;  /*1x�Ự�쳣��Ϣ�ṹ��*/

    NAS_ERRLOG_CCB_1X_MT_SERVICE_EXCEPTION_STRU             stNasErrLogCcb1xMtServiceException;     /*1x����ʧ���쳣��Ϣ�ṹ��*/

    NAS_ERRLOG_CCB_CDMA_PS_CALL_EXCEPTION_STRU              stNasErrLogCcbCdmaPsCallExp;            /* PS����ʧ����Ϣ */

    NAS_ERRLOG_CCB_CDMA_PS_DISC_INFO_STRU                   stNasErrLogCcbCdmaPsDiscInfo;           /* PS�����쳣��Ϣ */

    NAS_ERRLOG_CCB_HRPD_SESSION_INFO_STRU                   stNasErrLogHrpdSessionInfo;             /* Hrpd Session��Ϣ */

    NAS_ERRLOG_CCB_CL_OOS_SEARCH_INFO_STRU                  stNasErrLogCcbCLOosSearchInfo;          /* CL��ģ������Ϣ */
    NAS_ERRLOG_CCB_HRPD_OR_LTE_OOS_RECOVERY_STRU            stNasErrLogCcbHrpdOrLteOosRecovery;     /*CLģʽ��HRPD��LTE�����ָ��ϱ��ṹ��*/
    NAS_ERRLOG_CCB_HRPD_OOS_SEARCH_STRU                     stNasErrLogCcbHrpdOosSearchRecord;      /*Hrpd������¼*/

    NAS_ERRLOG_CCB_HRPD_OOS_REPORT_STRU                     stNasErrLogCcbHrpdOosRpt;               /*Hrpd������¼*/

    NAS_ERRLOG_CCB_XREG_FAIL_INFO_STRU                      stNasErrLogCcbXregFailInfo;             /* Xregʧ���ϱ��ṹ��*/

    NAS_ERRLOG_CCB_XSMS_SND_FAIL_INFO_STRU                  stNasErrLogCcbXsmsSndFailInfo;          /* x���ŷ���ʧ���ϱ��ṹ��*/

    NAS_ERRLOG_CCB_XSMS_RCV_FAIL_INFO_STRU                  stNasErrLogCcbXsmsRcvFailInfo;          /* x���Ž���ʧ���ϱ��ṹ��*/
}NAS_ERRLOG_CCB_CDMA_CTX_STRU;



typedef struct
{
    NAS_ERRLOG_CCB_CDMA_CTX_STRU        stErrLogCcbCdmaCtx;
}NAS_ERRLOG_CCB_CTX_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

#if (FEATURE_PTM == FEATURE_ON)

extern NAS_ERRLOG_CCB_CTX_STRU* NAS_ERRLOG_CCB_GetCcbCtxAddr( VOS_VOID );

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
extern VOS_UINT32 NAS_ERRLOG_CCB_Calc1xOosSrchInfoRecordIndex(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_CalcCdmaPsDiscRecdRedialIndex(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_CalcClOosSrchInfoRecordIndex(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_CalcHrpdOosSrchInfoRecordIndex(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_CalcPsCallExcepRedialRecordIndex(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_CalcPsDiscRecdIndex(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_CdmaPsDiscExcepStartRecord(
           VOS_UINT32                          ulCause,
           NAS_ERRLOG_PDP_ENTITY_FSM_CTX_STRU *pstPdpEntityFsmCtx
       );
extern NAS_ERRLOG_CCB_1X_COMMON_INFO_STRU* NAS_ERRLOG_CCB_Get1xCommonInfoCtxAddr(VOS_VOID);
extern NAS_ERRLOG_CCB_1X_CS_CALL_EXCEPTION_STRU *NAS_ERRLOG_CCB_Get1xConversationExceptionAddr(VOS_VOID);
extern NAS_ERRLOG_CCB_1X_CS_CALL_EXCEPTION_STRU *NAS_ERRLOG_CCB_Get1xCsCallExceptionAddr(VOS_VOID);
extern NAS_ERRLOG_CCB_1X_MT_SERVICE_EXCEPTION_STRU *NAS_ERRLOG_CCB_Get1xMtServiceExceptionAddr(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_Get1xMtServiceExceptionRecordIndex(
           NAS_ERR_LOG_1X_SERVICE_TYPE_ENUM_UINT32                 enServiceType,
           VOS_UINT8                                              *pucIndex
       );
extern NAS_ERRLOG_CCB_1X_OOS_RECOVERY_STRU* NAS_ERRLOG_CCB_Get1xOosRecoveryCtxAddr(VOS_VOID);
extern NAS_ERRLOG_CCB_1X_OOS_SEARCH_STRU* NAS_ERRLOG_CCB_Get1xOosSearchCtxAddr(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_Get1xOosSrchInfoRecordCnt(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetAlreadyReportChr1xOosFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetAlreadyReportChrHrpdOrLteOosFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetAlreadyReportHrpdOrLteOosToApFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetAlreadyReportOosToApFlag(VOS_VOID);

NAS_ERRLOG_CCB_CDMA_CTX_STRU* NAS_ERRLOG_CCB_GetCdmaCcbCtxAddr(VOS_VOID);

VOS_VOID NAS_ERRLOG_CCB_SetCdmaModemId(
    MODEM_ID_ENUM_UINT16                enModeModemId
);

extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdma1xMtServiceExceptionActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdma1xOosActiveReportFlag(VOS_VOID);
extern NAS_ERRLOG_CCB_CDMA_SELF_ACTIVE_REPORT_CONTROL_STRU* NAS_ERRLOG_CCB_GetCdmaActiveReportControlCtxAddr(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdmaHrpdOrLteOosReportFlag(VOS_VOID);

extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdmaHrpdSessionExcepDeactActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdmaHrpdSessionFailActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdmaHrpdUatiFailActiveReportFlag(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetCdmaPsCallExcepRedialCnt(VOS_VOID);
extern NAS_ERRLOG_CCB_CDMA_PS_CALL_EXCEPTION_STRU* NAS_ERRLOG_CCB_GetCdmaPsCallExpCtxAddr(VOS_VOID);
extern NAS_ERRLOG_CCB_CDMA_PS_DISC_INFO_STRU* NAS_ERRLOG_CCB_GetCdmaPsDiscCtxAddr(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetCdmaPsDiscRecdCnt(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetCdmaPsDiscRecdRedialCnt(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdmaXregResltReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetCdmaXsmsReportFlag(VOS_VOID);
extern NAS_ERRLOG_CCB_CL_OOS_SEARCH_INFO_STRU* NAS_ERRLOG_CCB_GetClOosSrchInfoCtxAddr(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetClOosSrchInfoRecordCnt(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetCurCsCallRedialRecordInfoIndex(
           VOS_UINT32                          ulRedialCnt
       );
extern NAS_ERRLOG_CCB_HRPD_COMMON_INFO_STRU* NAS_ERRLOG_CCB_GetHrpdCommonInfoCtxAddr(VOS_VOID);
extern NAS_ERR_LOG_HRPD_OOS_CAUSE_ENUM_UINT32 NAS_ERRLOG_CCB_GetHrpdOosCause(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdOrLteOosMainActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdOrLteOosMainActiveReportLevel(VOS_VOID);
extern NAS_ERRLOG_CCB_HRPD_OR_LTE_OOS_RECOVERY_STRU* NAS_ERRLOG_CCB_GetHrpdOrLteOosRecoveryCtxAddr(VOS_VOID);
extern NAS_ERRLOG_CCB_HRPD_OOS_REPORT_STRU* NAS_ERRLOG_CCB_GetHrpdOosReportCtxAddr(VOS_VOID);
extern NAS_ERRLOG_CCB_HRPD_OOS_SEARCH_STRU* NAS_ERRLOG_CCB_GetHrpdOosSearchCtxAddr(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetHrpdOosSrchInfoRecordCnt(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdSessionFailActReason(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdSessionFailActType(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdSessionFailCause(VOS_VOID);
extern NAS_ERRLOG_CCB_HRPD_SESSION_FAIL_STRU* NAS_ERRLOG_CCB_GetHrpdSessionFailCtxAddr(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetHrpdSessionFailEndTime(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetHrpdSessionFailStartTime(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdSessionFailSupportEhrpdFlg(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdUatiFailCause(VOS_VOID);
extern NAS_ERRLOG_CCB_HRPD_UATI_FAIL_STRU* NAS_ERRLOG_CCB_GetHrpdUatiFailCtxAddr(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetHrpdUatiFailEndTime(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetHrpdUatiFailStartTime(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdUatiFailTriggerCause(VOS_VOID);

extern VOS_VOID NAS_ERRLOG_CCB_InCreaseHrpdUatiFailRecordCnt(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiAssignRecordStartTime(
    VOS_UINT32                          ulIndex,
    VOS_UINT32                          ulStartTime
);
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiAssignRecordEndTime(
    VOS_UINT32                          ulIndex,
    VOS_UINT32                          ulEndTime
);
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiAssignRecordRslt(
    VOS_UINT32                                              ulIndex,
    NAS_ERR_LOG_HRPD_UATI_ASSIGN_RSLT_ENUM_UINT32           enUatlAssignRlst
);
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiAssignRecordIndex(
    VOS_UINT32                          ulIndex
);
extern VOS_UINT32 NAS_ERRLOG_CCB_CalcHrpdUatiAssignFailRecordIndex(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHrpdUatiFailRecordCnt(VOS_VOID);

VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiRcvSnpDataCnfTimer(
    VOS_UINT32                          ulIndex,
    VOS_UINT32                          ulUatiReqRcvSnpDataCnfTimer
);

extern VOS_UINT8 NAS_ERRLOG_CCB_GetHsmMainActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetHsmMainActiveReportLevel(VOS_VOID);
extern NAS_ERRLOG_CCB_CDMA_MAIN_ACTIVE_REPORT_CONTROL_STRU* NAS_ERRLOG_CCB_GetMainActiveReportControlCtxAddr(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetMmaMainActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetMmaMainActiveReportLevel(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXccMainActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXccMainActiveReportLevel(VOS_VOID);
extern NAS_ERRLOG_CCB_XREG_FAIL_INFO_STRU* NAS_ERRLOG_CCB_GetXregFailCtxAddr(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXregFailType(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXregMainActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXregMainActiveReportLevel(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXreRegFailReason(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXsmsMainActiveReportFlag(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXsmsMainActiveReportLevel(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_GetXsmsRcvCasCnfRslt(VOS_VOID);
extern NAS_ERRLOG_CCB_XSMS_RCV_FAIL_INFO_STRU* NAS_ERRLOG_CCB_GetXsmsRcvCtxAddr(VOS_VOID);
extern NAS_ERR_LOG_XSMS_FAIL_REASON_ENUM_UINT8 NAS_ERRLOG_CCB_GetXsmsRcvFailReason(VOS_VOID);
extern VOS_UINT32 NAS_ERRLOG_CCB_GetXsmsReRcvTime(VOS_VOID);
extern NAS_ERRLOG_CCB_XSMS_SND_FAIL_INFO_STRU* NAS_ERRLOG_CCB_GetXsmsSndCtxAddr(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InCrease1xOosSrchInfoRecordCnt(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InCreaseCdmaPsDiscRecdRedialCnt(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InCreaseClOosSrchInfoRecordCnt(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_IncreaseCsCallRedialCnt(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_IncreaseHrpdOosSrchInfoRecordCnt(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InCreasePsCallExcepRedialCnt(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InCreasePsDiscRecdCnt(VOS_VOID);

extern NAS_ERRLOG_CCB_MODEM_INFO_STRU* NAS_ERRLOG_CCB_GetModemInfoAddr(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitModemInfo(VOS_VOID);

extern VOS_VOID NAS_ERRLOG_CCB_Init1xCommonInfoCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_Init1xConversationExceptionCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_Init1xCsCallExceptionCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_Init1xMtServiceExceptionCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_Init1xMtServiceExceptionRecord(
           NAS_ERR_LOG_1X_SERVICE_TYPE_ENUM_UINT32                 enServiceType
       );
extern VOS_VOID NAS_ERRLOG_CCB_Init1xOosRecoveryCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_Init1xOosSearchCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_Init1xOosSrchInfoRecord(
           VOS_UINT32                          ulIndex
       );


extern VOS_VOID NAS_ERRLOG_CCB_InitCdmaActiveReportControlCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitCdmaPsCallExpCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitCdmaPsDiscExcepRecd(
           VOS_UINT32                          ulRecdIndex
       );
extern VOS_VOID NAS_ERRLOG_CCB_InitCdmaPsDiscInfoCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitClOosSearchInfo(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitClOosSrchInfoRecord(
           VOS_UINT32                          ulIndex
       );
extern NAS_ERRLOG_CCB_CL_OOS_SEARCH_RECORD_STRU *NAS_ERRLOG_CCB_GetClOosSrchInfoRecord(
    VOS_UINT32                          ulIndex
);
extern VOS_VOID NAS_ERRLOG_CCB_InitHrpdCommonInfoCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitHrpdOrLteOosRecoveryCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitHrpdOosReportCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitHrpdOosSearchCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitHrpdOosSrchInfoRecord(
    VOS_UINT32                          ulIndex
);
extern VOS_VOID NAS_ERRLOG_CCB_InitHrpdSessionFailCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitHrpdUatiFailCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitMainActiveReportControlCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitXregFailCtx(VOS_VOID);
extern VOS_UINT8 NAS_ERRLOG_CCB_IsCsCallRedialCntUpdata(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_SaveCdmaPsCallFailStartRecord(
           NAS_ERR_LOG_PS_CALL_RAT_TYPE_ENUM_UINT32                enApsRatType
       );
extern VOS_VOID NAS_ERRLOG_CCB_SaveCdmaPsCreateLinkEndRecord(
           VOS_UINT32                          ulTafPsCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SaveCdmaPsDiscExcepRedialStartRecord(
           NAS_ERR_LOG_PS_CALL_RAT_TYPE_ENUM_UINT32                enApsRatType
       );
extern VOS_VOID NAS_ERRLOG_CCB_SaveConversationRsltCause(
           NAS_ERR_LOG_1X_MT_SERVICE_EXCEPTION_CAUSE_ENUM_UINT32  enErrLogCallCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SaveCsCallRsltCause(
           VOS_UINT32                          ulErrLogCallCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SaveMtServiceExceptionCause(
           NAS_ERR_LOG_1X_SERVICE_TYPE_ENUM_UINT32                 enServiceType,
           NAS_ERR_LOG_1X_MT_SERVICE_EXCEPTION_CAUSE_ENUM_UINT32   enErrLogCallCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SaveMtServiceExceptionXccState(
           NAS_ERR_LOG_1X_SERVICE_TYPE_ENUM_UINT32                 enServiceType,
           VOS_UINT32                                              ulXccState
       );
extern VOS_VOID NAS_ERRLOG_CCB_SavePagingServiceType(
           NAS_ERR_LOG_1X_SERVICE_TYPE_ENUM_UINT32                 enServiceType
       );
extern VOS_VOID NAS_ERRLOG_CCB_SaveReceivedPagingTimer(
           NAS_ERR_LOG_1X_SERVICE_TYPE_ENUM_UINT32                 enServiceType,
           VOS_UINT32                                              ulTimer
       );
extern VOS_VOID NAS_ERRLOG_CCB_Set1xOosSrchInfoEndTime(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulEndTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_Set1xOosSrchInfoRecordIndex(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulRecordIndex
       );
extern VOS_VOID NAS_ERRLOG_CCB_Set1xOosSrchInfoSearchRslt(
           VOS_UINT32                                              ulIndex,
           NAS_ERR_LOG_1X_SEARCH_RSLT_ENUM_UINT32                  enSearchRslt
       );
extern VOS_VOID NAS_ERRLOG_CCB_Set1xOosSrchInfoStartTime(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulStartTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetAlreadyReportChr1xOosFlag(
           VOS_UINT8                           ucAlreadyReportChrOosFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetAlreadyReportChrHrpdOrLteOosFlag(
           VOS_UINT8                           ucAlreadyReportChrOosFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetAlreadyReportHrpdOrLteOosToApFlag(
           VOS_UINT8                           ucAlreadyReportOosToApFlag
       );

extern VOS_VOID NAS_ERRLOG_CCB_SetAlreadyReportOosToApFlag(
           VOS_UINT8                           ucAlreadyReportOosToApFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdma1xMtServiceExceptionActiveReportFlag(
           VOS_UINT8                           uc1xMtServiceExceptionReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdma1xOosActiveReportFlag(
           VOS_UINT8                           uc1xOosReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaHrpdOrLteOosReportFlag(
           VOS_UINT8                           ucHrpdOrLteOosReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaHrpdSessionExcepDeactActiveReportFlag(
           VOS_UINT8                           ucHrpdSessionExcepDeactReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaHrpdSessionFailActiveReportFlag(
           VOS_UINT8                           ucHrpdSessionFailReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaHrpdUatiFailActiveReportFlag(
           VOS_UINT8                           ucHrpdUatiFailReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsCallExcepApsFsmInfo(
           VOS_UINT32                              ulMainFsmId,
           VOS_UINT32                              ulMainFsmState,
           VOS_UINT32                              ulSubFsmId,
           VOS_UINT32                              ulSubFsmState,
           VOS_UINT8                               ucPdpId
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsCallExcepPsCause(
           VOS_UINT32                          ulPsCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsCallExcepRedialEndTime(
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulEndTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsCallExcepRedialRatMode(
           VOS_UINT32                                              ulRedialIndex,
           NAS_ERR_LOG_PS_CALL_RAT_TYPE_ENUM_UINT32                enApsRatMode
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsCallExcepRedialRecordIndex(
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulRedialCnt
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsCallExcepRedialRslt(
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulTafPsCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsCallExcepRedialStartTime(
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulStartTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdApsFsmInfo(
           VOS_UINT32                              ulMainFsmId,
           VOS_UINT32                              ulMainFsmState,
           VOS_UINT32                              ulSubFsmId,
           VOS_UINT32                              ulSubFsmState,
           VOS_UINT8                               ucPdpId
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdIndex(
           VOS_UINT32                          ulRecdIndex,
           VOS_UINT32                          ulCdmaPsDiscRecdCnt
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdPsCause(
           VOS_UINT32                          ulRecdIndex,
           TAF_PS_CAUSE_ENUM_UINT32            enPsCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdRedialEndTime(
           VOS_UINT32                          ulRecdIndex,
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulEndTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdRedialRatMode(
           VOS_UINT32                                              ulRecdIndex,
           VOS_UINT32                                              ulRedialIndex,
           NAS_ERR_LOG_PS_CALL_RAT_TYPE_ENUM_UINT32                enRatMode
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdRedialRecordIndex(
           VOS_UINT32                          ulRecdIndex,
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulRedialCnt
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdRedialRslt(
           VOS_UINT32                          ulRecdIndex,
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulTafPsCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaPsDiscRecdRedialStartTime(
           VOS_UINT32                          ulRecdIndex,
           VOS_UINT32                          ulRedialIndex,
           VOS_UINT32                          ulStartTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaXregResltReportFlag(
           VOS_UINT8                           ucXregResltReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCdmaXsmsReportFlag(
           VOS_UINT8                           ucXsmsReportEnable
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetClOosSrchInfoEndTime(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulEndTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetClOosSrchInfoIsHrpdAndLteBothSupport(
           VOS_UINT32                          ulIsHrpdAndLteBothSupport
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetClOosSrchInfoRecordIndex(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulRecordCnt
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetClOosSrchInfoSearchRat(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulSearchRat
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetClOosSrchInfoSearchRslt(
           VOS_UINT32                                              ulIndex,
           NAS_ERR_LOG_CL_SEARCH_RSLT_ENUM_UINT32                  enSearchRslt
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetClOosSrchInfoSearchType(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulSearchType
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetClOosSrchInfoStartTime(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulStartTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetConversationEndTimer(
           VOS_UINT32                          ulEndTimer
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetConversationStartTimer(
           VOS_UINT32                          ulStartTimer
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCsCallEndTimer(
           VOS_UINT32                          ulEndTimer
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetCsCallStartTimer(
           VOS_UINT32                          ulStartTimer
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdOosCause(
           NAS_ERR_LOG_HRPD_OOS_CAUSE_ENUM_UINT32                  enOosCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdOosSrchInfoEndTime(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulEndTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdOosSrchInfoRecordIndex(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulRecordIndex
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdOosSrchInfoSearchRslt(
           VOS_UINT32                                                ulIndex,
           NAS_ERR_LOG_HRPD_SEARCH_RSLT_ENUM_UINT32                  enSearchRslt
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdOosSrchInfoStartTime(
           VOS_UINT32                          ulIndex,
           VOS_UINT32                          ulStartTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdSessionFailActReason(
           VOS_UINT8                           ucSessionActReason
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdSessionFailActType(
           VOS_UINT8                           ucSessionActType
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdSessionFailCause(
           VOS_UINT8                           ucSessionFailCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdSessionFailEndTime(
           VOS_UINT32                          ulEndTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdSessionFailStartTime(
           VOS_UINT32                          ulStartTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdSessionFailSupportEhrpdFlg(
           VOS_UINT8                           ucSupportedEhrpdFlg
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiFailCause(
           VOS_UINT8                           ucUatiFailCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiFailEndTime(
           VOS_UINT32                          ulEndTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiFailStartTime(
           VOS_UINT32                          ulStartTime
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHrpdUatiFailTriggerCause(
           VOS_UINT8                           ucTriggerCause
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHsmMainActiveReportFlag(
           VOS_UINT8                           ucErrLogActiveReportAlarmFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetHsmMainActiveReportLevel(
           VOS_UINT8                           ucErrLogActiveReportAlarmLevel
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetMmaMainActiveReportFlag(
           VOS_UINT8                           ucErrLogActiveReportAlarmFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetMmaMainActiveReportLevel(
           VOS_UINT8                           ucErrLogActiveReportAlarmLevel
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXccMainActiveReportFlag(
           VOS_UINT8                           ucErrLogActiveReportAlarmFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXccMainActiveReportLevel(
           VOS_UINT8                           ucErrLogActiveReportAlarmLevel
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXregFailReason(
           NAS_ERR_LOG_XREG_FAIL_REASON_ENUM_UINT8           enRegFailReason
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXregFailType(
           NAS_ERR_LOG_XREG_REGISTRATION_TYPE_ENUM_UINT8           enRegType
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXregMainActiveReportFlag(
           VOS_UINT8                           ucErrLogActiveReportAlarmFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXregMainActiveReportLevel(
           VOS_UINT8                           ucErrLogActiveReportAlarmLevel
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsCasCnf(VOS_UINT8 enLastCasCnfRslt);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsMainActiveReportFlag(
           VOS_UINT8                           ucErrLogActiveReportAlarmFlag
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsMainActiveReportLevel(
           VOS_UINT8                           ucErrLogActiveReportAlarmLevel
       );
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsNoTlAckCauseCode(NAS_ERR_LOG_XSMS_NO_TL_ACK_REASON_ENUM_UINT8 enNoTlAckReason);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsRcvCasCnfRslt(VOS_UINT8 enLastCasCnfRslt);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsRcvFailReason(NAS_ERR_LOG_XSMS_FAIL_REASON_ENUM_UINT8 enFailReason);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsReRcvTime(VOS_UINT8 ucReRcvTime);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsReSendTime(VOS_UINT32 ulReSendTime);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsSndFailReason(NAS_ERR_LOG_XSMS_FAIL_REASON_ENUM_UINT8 enSndFailReason);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsSndFSMState(VOS_UINT32 enFailState);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsTlAckCauseCode(VOS_UINT8 enLastTlAckCauseCode);
extern VOS_VOID NAS_ERRLOG_CCB_SetXsmsXccCnf(VOS_UINT32 enLastXccCnfRslt);
extern VOS_VOID NAS_ERRLOG_CCB_SndAcpuOmFaultErrLogCnf(
           VOS_VOID                           *pData,
           VOS_UINT32                          ulDataLen,
           VOS_UINT8                           ucFaultId,
           VOS_UINT8                           ucAlarmId,
           VOS_UINT32                          ulSenderPid
       );
extern VOS_VOID NAS_ERRLOG_CCB_SndAcpuOmFaultErrLogInd(
           VOS_VOID                           *pData,
           VOS_UINT32                          ulDataLen,
           VOS_UINT8                           ucFaultId,
           NAS_ERR_LOG_ALM_ID_ENUM_U16         usAlmId,
           VOS_UINT32                          ulSenderPid
       );

extern NAS_ERRLOG_CCB_CL_LTE_COMMON_INFO_STRU *NAS_ERRLOG_CCB_GetClLteCommonInfoCtxAddr(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitClLteCommonInfoCtx(VOS_VOID);
extern VOS_VOID NAS_ERRLOG_CCB_InitCcbCtx(VOS_VOID);
#endif
#endif
#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of NasErrlogCcbCtx.h */

