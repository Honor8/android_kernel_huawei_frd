

#ifndef __IMSALRRCINTERFACE_H__
#define __IMSALRRCINTERFACE_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/

#include    "vos.h"
#include    "LPSCommon.h"
#include    "LPsNvInterface.h"
#include    "LPsOm.h"



/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
#pragma pack(*)    �����ֽڶ��뷽ʽ
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/
#define  IMSA_LRRC_MSG_HDR   (PS_MSG_ID_IMSA_TO_ERRC_BASE)


#define  LRRC_IMSA_MSG_HDR   (PS_MSG_ID_ERRC_TO_IMSA_BASE)







/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

enum    IMSA_LRRC_MSG_ID_ENUM
{
    ID_IMSA_LRRC_BEGIN_SESSION_NOTIFY     = (IMSA_LRRC_MSG_HDR + 0x00),     /* _H2ASN_MsgChoice IMSA_LRRC_BEGIN_SESSION_NOTIFY_STRU */
    ID_IMSA_LRRC_END_SESSION_NOTIFY       = (IMSA_LRRC_MSG_HDR + 0x01),     /* _H2ASN_MsgChoice IMSA_LRRC_END_SESSION_NOTIFY_STRU */

    ID_IMSA_LRRC_HIFI_SYNC_SWITCH_IND     = (IMSA_LRRC_MSG_HDR + 0x02),     /* _H2ASN_MsgChoice IMSA_LRRC_HIFI_SYNC_SWITCH_IND_STRU */

    ID_IMSA_LRRC_VOLTE_STATUS_NOTIFY      = (IMSA_LRRC_MSG_HDR + 0x03),    /* _H2ASN_MsgChoice IMSA_LRRC_VOLTE_STATUS_NOTIFY_STRU */
    ID_IMSA_LRRC_QUALITY_CFG_REQ          = (IMSA_LRRC_MSG_HDR + 0x04),     /* _H2ASN_MsgChoice IMSA_LRRC_QUALITY_CFG_REQ_STRU */

    ID_LRRC_IMSA_QUALITY_CFG_CNF          = (LRRC_IMSA_MSG_HDR + 0x00),     /* _H2ASN_MsgChoice LRRC_IMSA_QUALITY_CFG_CNF_STRU */
    ID_LRRC_IMSA_STATE_IND                = (LRRC_IMSA_MSG_HDR + 0x01),     /* _H2ASN_MsgChoice LRRC_IMSA_STATE_IND_STRU */


    ID_IMSA_LRRC_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32   IMSA_LRRC_MSG_ID_ENUM_UINT32;



/*****************************************************************************
  4 Enum
*****************************************************************************/

enum IMSA_LRRC_SESSION_TYPE_ENUM
{
    IMSA_LRRC_SESSION_TYPE_MO_NORMAL_CALL,
    IMSA_LRRC_SESSION_TYPE_MT_NORMAL_CALL,
    IMSA_LRRC_SESSION_TYPE_EMERGENCY_CALL,
    IMSA_LRRC_SESSION_TYPE_MO_SS,
    IMSA_LRRC_SESSION_TYPE_MT_SS,
    IMSA_LRRC_SESSION_TYPE_MO_SMS,
    IMSA_LRRC_SESSION_TYPE_MT_SMS,
    IMSA_LRRC_SESSION_TYPE_REGISTER,
    IMSA_LRRC_SESSION_TYPE_BUTT
};
typedef VOS_UINT8 IMSA_LRRC_SESSION_TYPE_ENUM_UINT8;

enum IMSA_LRRC_CFG_QUALITY_ENUM
{
    IMSA_LRRC_CFG_QUALITY_START      = 0,
    IMSA_LRRC_CFG_QUALITY_STOP,

    IMSA_LRRC_CFG_QUALITY_BUTT
};

typedef VOS_UINT8 IMSA_LRRC_CFG_QUALITY_ENUM_UINT8;

/*****************************************************************************
    ö����    : LTE_QUALITY_LEVEL_ENUM
    ö��˵��  : LTE�����ȼ�ö��
*****************************************************************************/
enum LTE_IMSA_QUALITY_LEVEL_ENUM
{
    LTE_IMSA_QUALITY_LEVEL_BAD         = 0x00,
    LTE_IMSA_QUALITY_LEVEL_NORMAL      = 0x01,
    LTE_IMSA_QUALITY_LEVEL_GOOD        = 0x02,

    LTE_IMSA_QUALITY_LEVEL_BUTT
};
typedef VOS_INT8 LTE_IMSA_QUALITY_LEVEL_ENUM_UINT8;

/*****************************************************************************
 ö����    : LRRC_IMSA_RESULT_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  : IMSAԭ�����������ݽṹ
*****************************************************************************/
enum LRRC_IMSA_RESULT_ENUM
{
    LRRC_IMSA_RESULT_SUCC                 = 0,                /* �����ɹ� */
    LRRC_IMSA_RESULT_FAIL ,                                   /* ����ʧ�� */
    LRRC_IMSA_RESULT_BUTT
};
typedef VOS_UINT16    LRRC_IMSA_RESULT_ENUM_UINT16;


enum IMSA_LRRC_VOLTE_STATUS_ENUM
{
    IMSA_LRRC_VOLTE_STATUS_INIT,
    IMSA_LRRC_VOLTE_STATUS_START,
    IMSA_LRRC_VOLTE_STATUS_END,

    IMSA_LRRC_VOLTE_STATUS_BUTT
};
typedef VOS_UINT8  IMSA_LRRC_VOLTE_STATUS_ENUM_UINT8;


enum IMSA_LRRC_VOLTE_CALL_TYPE_ENUM
{
    IMSA_LRRC_VOLTE_CALL_TYPE_MO,
    IMSA_LRRC_VOLTE_CALL_TYPE_MT,

    IMSA_LRRC_VOLTE_CALL_TYPE_BUTT
};
typedef VOS_UINT8  IMSA_LRRC_VOLTE_CALL_TYPE_ENUM_UINT8;

/*****************************************************************************
  5 STRUCT
*****************************************************************************/

typedef struct
{
    MSG_HEADER_STRU                                 stMsgHeader;          /* ��Ϣͷ    */   /*_H2ASN_Skip*/
    IMSA_LRRC_SESSION_TYPE_ENUM_UINT8               enSessionType;
    VOS_UINT8                                       aucReserved[3];
}IMSA_LRRC_BEGIN_SESSION_NOTIFY_STRU;


typedef struct
{
    MSG_HEADER_STRU                                 stMsgHeader;          /* ��Ϣͷ    */   /*_H2ASN_Skip*/
    IMSA_LRRC_SESSION_TYPE_ENUM_UINT8               enSessionType;
    VOS_UINT8                                       aucReserved[3];
}IMSA_LRRC_END_SESSION_NOTIFY_STRU;


typedef struct
{
    MSG_HEADER_STRU                                 stMsgHeader;          /* ��Ϣͷ    */   /*_H2ASN_Skip*/
    VOS_UINT8                                       ucHifiSyncEnabled;    /* 0��ʾ�رգ�1��ʾ�� */
    VOS_UINT8                                       ucPowerState;         /* Modem1���ػ�״̬��0��ʾ�ر�;1��ʾ�� */
    VOS_UINT8                                       aucReserved[2];
}IMSA_LRRC_HIFI_SYNC_SWITCH_IND_STRU;



typedef struct
{
    MSG_HEADER_STRU                                 stMsgHeader;          /* ��Ϣͷ    */   /*_H2ASN_Skip*/
    IMSA_LRRC_CFG_QUALITY_ENUM_UINT8                enStartOrStop;         /* ������������ֹͣ�ϱ�LTE�ź����� */
    VOS_UINT8                                       aucReserved[3];
}IMSA_LRRC_QUALITY_CFG_REQ_STRU;



typedef struct
{
    MSG_HEADER_STRU                                 stMsgHeader;          /* ��Ϣͷ    */   /*_H2ASN_Skip*/
    LTE_IMSA_QUALITY_LEVEL_ENUM_UINT8               enLteQuality;         /* LTE�ź����� */
    VOS_UINT8                                       aucReserved[3];
}LRRC_IMSA_STATE_IND_STRU;



typedef struct
{
    MSG_HEADER_STRU                                 stMsgHeader;          /* ��Ϣͷ    */   /*_H2ASN_Skip*/
    LRRC_IMSA_RESULT_ENUM_UINT16                    usResult;
    VOS_UINT8                                       aucReserved[2];
}LRRC_IMSA_QUALITY_CFG_CNF_STRU;





/*****************************************************************************
 �ṹ��    : LRRC_IMSA_VOWIFI_RPT_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : vowifi �����ϱ�
*****************************************************************************/
typedef struct
{
    IMSA_LRRC_CFG_QUALITY_ENUM_UINT8        enStartRptFlg;      /* ��ǰ�Ƿ�ʼ�ϱ� */
    LTE_IMSA_QUALITY_LEVEL_ENUM_UINT8       enLastQuality;      /* ��һ���ϱ������� */
    LTE_IMSA_QUALITY_LEVEL_ENUM_UINT8       enUnRptQuality;     /* ��ǰ�б仯��δ�ϱ������� */
    LTE_IMSA_QUALITY_LEVEL_ENUM_UINT8       enCurrQuality;      /* ��ǰ������ */
    LPS_32K_TIMESTAMP                       stLatestChangeTime; /* ���һ��״̬�ı��ʱ�� */
    LRRC_NV_VOWIFI_THRES_STRU               stVoWifiThres;      /* ���� */
    LPS_32K_TIMESTAMP                       stStableIntervalTime; /*�����ϱ������ȶ������ϴ����ϱ���ʼ��ʱʱ��*/
    VOS_INT16                               sThresWlanDeltaP;   /*����С���ϱ���������ֵ,Ĭ��5db*/
    PS_BOOL_ENUM_UINT8                      ucLastBadToNormalEvaFlg;/* ��һ��badתnormal�������λ,true:������;����,δ���� */
    PS_BOOL_ENUM_UINT8                      ucLastNormalToBadEvaFlg;/* ��һ��normalתbad�������λ,true:������;����,δ���� */

}LRRC_IMSA_VOWIFI_RPT_INFO_STRU;

/*****************************************************************************
 ��������: IMSA_LRRC_GetSsacInfo
 Э����:
 ASN.1 ����:
 ����˵��:
*****************************************************************************/
typedef struct
{
    VOS_UINT8    ucBarFactorForVoice;   /*ȡֵ��Χ:0~15ö��ֵ*/
    VOS_UINT8    ucBarTimeForVoice;     /*ȡֵ��Χ:0~7ö��ֵ,��λ:s*/
    VOS_UINT8    ucBarFactorForVideo;   /*ȡֵ��Χ:0~15ö��ֵ*/
    VOS_UINT8    ucBarTimeForVideo;     /*ȡֵ��Χ:0~7ö��ֵ,��λ:s*/
}IMSA_LRRC_SSAC_INFO_STRU;

/*****************************************************************************
 �ṹ��    : IMSA_LRRC_VOLTE_STATUS_NOTIFY_STRU
 �ṹ˵��  : IMSA֪ͨLRRC VOLTE���������Լ�����
*****************************************************************************/
typedef struct
{
    MSG_HEADER_STRU                                 stMsgHeader;          /*_H2ASN_Skip*/
    IMSA_LRRC_VOLTE_STATUS_ENUM_UINT8               enVolteStatus;
    IMSA_LRRC_VOLTE_CALL_TYPE_ENUM_UINT8            enVolteCallTye;
    VOS_UINT8                                       aucReserved[2];
}IMSA_LRRC_VOLTE_STATUS_NOTIFY_STRU;


/*****************************************************************************
 ��������: IMSA_LRRC_GetSsacInfo
 Э����:
 ASN.1 ����:
 ����˵��:LRRC�ṩ��IMSA SSAC�����Ϣ
*****************************************************************************/
extern VOS_UINT32  IMSA_LRRC_GetSSACInfo
(
    IMSA_LRRC_SSAC_INFO_STRU *pstSsacInfo
);



extern VOS_UINT32 IMSA_LRRC_JudgeCurrCellSingalQuality(VOS_VOID);

/*****************************************************************************
 ��������: IMSA_LRRC_IsAcbEnabled
 Э����:
 ASN.1 ����:
 ����˵��:LRRC�ṩ��IMSA ACB Enable�����Ϣ
*****************************************************************************/
extern VOS_UINT32 IMSA_LRRC_IsAcbEnabled(VOS_VOID);
/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/

/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/





/*****************************************************************************
  9 OTHERS
*****************************************************************************/


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
            }
    #endif
#endif

#endif /* end of ImsaLrrcInterface.h */








