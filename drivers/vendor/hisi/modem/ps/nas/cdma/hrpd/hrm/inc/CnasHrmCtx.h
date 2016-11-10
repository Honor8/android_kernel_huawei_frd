

#ifndef __CNAS_HRM_CTX_H__
#define __CNAS_HRM_CTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include  "PsCommonDef.h"
#include  "PsLogdef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)


/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_REQ                                   (0)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_UATI_CMPL                                  (1)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_HARDWARE_ID                                (2)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_MO_KEEP_ALIVE                              (3)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_MT_KEEP_ALVIE                              (4)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_SESSION_CLOSE                              (5)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_NTF                                    (6)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_HANDOFF_LOC_NTF                            (7)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_LOC_CMPL                                   (8)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_NTF                                   (9)
#define CNAS_HRM_DATA_REQ_BUFF_INDEX_BLOB_CMPL                                  (10)

#define CNAS_HRM_DATA_REQ_BUFF_INDEX_MAX                                        (15)

#define CNAS_HRM_BYTE_LEN                                                       (8)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


enum CNAS_HRM_RES_APPLY_STATE_ENUM
{
    CNAS_HRM_RES_APPLY_STATE_NOT_APPLY,                     /* ��Դδ���� */
    CNAS_HRM_RES_APPLY_STATE_APPLYING,                      /* ��Դ�������� */
    CNAS_HRM_RES_APPLY_STATE_APPLY_SUCC,                    /* ��Դ����ɹ� */
    CNAS_HRM_RES_APPLY_STATE_APPLY_FAIL,                    /* ��Դ����ʧ�� */

    CNAS_HRM_RES_APPLY_STATE_BUTT
};
typedef VOS_UINT8 CNAS_HRM_RES_APPLY_STATE_ENUM_UINT8;


enum CNAS_HRM_RES_OCCUPY_TYPE_ENUM
{
    CNAS_HRM_RES_OCCUPY_TYPE_NON,                           /* ��Դû�б���ռ */
    CNAS_HRM_RES_OCCUPY_TYPE_SHORT,                         /* ��Դ������ */
    CNAS_HRM_RES_OCCUPY_TYPE_LONG,                          /* ��Դ������ */

    CNAS_HRM_RES_OCCUPY_TYPE_BUTT
};
typedef VOS_UINT8 CNAS_HRM_RES_OCCUPY_TYPE_ENUM_UINT8;


enum CNAS_HRM_DSDS_TYPE_ENUM
{
    CNAS_HRM_DSDS_TYPE_1    = 0x01,                         /* DSDS1.0 �������� */
    CNAS_HRM_DSDS_TYPE_2    = 0x02,                         /* DSDS2.0 ����˫�� */
    CNAS_HRM_DSDS_TYPE_BUTT
};
typedef VOS_UINT8 CNAS_HRM_DSDS_TYPE_ENUM_UINT8;

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
    VOS_UINT32                                              ulIsReqMsgExisted;  /* �յ�HSM����HLU����Ϣʱ��Ϊtrue */
    VOS_UINT16                                              usReqOpId;          /* HSM����HLU�����д���OpId */
    VOS_UINT16                                              usHrmOpId;          /* HRM��HSM����HLU���󷢸�SNPʱ����OpId */
    VOS_UINT8                                              *pucMsg;             /* ������Ϣ��̬���� */
}CNAS_HRM_DATA_REQ_BUFF_MSG_STRU;


typedef struct
{
    CNAS_HRM_RES_APPLY_STATE_ENUM_UINT8                     enResApplyState;    /* ��ǰ��Դ�����״̬ */
    CNAS_HRM_RES_OCCUPY_TYPE_ENUM_UINT8                     enResOccupyType;    /* ��ǰ��Դ����ռ������ */
    VOS_UINT8                                               aucRsv[2];
}CNAS_HRM_DATA_REQ_BUFF_RES_INFO_STRU;


typedef struct
{
    CNAS_HRM_DATA_REQ_BUFF_MSG_STRU                         stReqMsg;
    CNAS_HRM_DATA_REQ_BUFF_RES_INFO_STRU                    stResInfo;
}CNAS_HRM_DATA_REQ_BUFF_STRU;


typedef struct
{
    VOS_UINT8                           ucIsNeedWaitResApplied;                 /* ��¼�Ƿ���Ҫ����Դ������ */
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;                             /* DSDS�汾��*/
    VOS_UINT16                          usCurOpId;                              /* ��SNP�����OP ID */
    CNAS_HRM_DATA_REQ_BUFF_STRU         astDataReqBuff[CNAS_HRM_DATA_REQ_BUFF_INDEX_MAX];
}CNAS_HRM_CTX_STRU;


/*****************************************************************************
 Structure      : CNAS_HRM_PC_REPLAY_FIXED_CONTEXT_MSG_STRU
 Description    : PC�طŹ��̣�����HRM��صĹ̶�ȫ�ֱ���ͨ����Ϣ�ṹ����
 Message origin :
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucIsNeedWaitResApplied;                 /* ��¼�Ƿ���Ҫ����Դ������ */
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType;                             /* DSDS�汾��*/
    VOS_UINT8                           aucRsv[2];
}CNAS_HRM_PC_REPLAY_FIXED_CONTEXT_MSG_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/




/*****************************************************************************
  10 ��������
*****************************************************************************/
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
CNAS_HRM_CTX_STRU* CNAS_HRM_GetCtxAddr(VOS_VOID);

CNAS_HRM_DATA_REQ_BUFF_MSG_STRU* CNAS_HRM_GetDataReqBuffMsgAddr(VOS_UINT8 ucIndex);

CNAS_HRM_DATA_REQ_BUFF_RES_INFO_STRU* CNAS_HRM_GetDataReqBuffResInfoAddr(VOS_UINT8 ucIndex);

CNAS_HRM_DATA_REQ_BUFF_STRU* CNAS_HRM_GetDataReqBuffAddr(VOS_UINT8 ucIndex);

VOS_VOID CNAS_HRM_ClearMsgInSpecifiedDataReqBuff(
    VOS_UINT8                           ucIndex
);

VOS_VOID CNAS_HRM_ClearResInfoInSpecifiedDataReqBuff(
    VOS_UINT8                           ucIndex
);

VOS_VOID CNAS_HRM_InitCtx(VOS_VOID);

VOS_UINT16 CNAS_HRM_GetOpId(VOS_VOID);

CNAS_HRM_DSDS_TYPE_ENUM_UINT8 CNAS_HRM_GetDsdsType(VOS_VOID);

VOS_UINT8 CNAS_HRM_IsNeedWaitResApplied(VOS_VOID);

VOS_VOID CNAS_HRM_ReadNvHrpdRfAllocSwitchMask(VOS_VOID);

VOS_VOID CNAS_HRM_ReadNvFemChanProfile(VOS_VOID);

VOS_VOID CNAS_HRM_ReadNvimInfo(VOS_VOID);

#if defined(DMT)
VOS_VOID CNAS_HRM_SetDsdsType_DMT(
    CNAS_HRM_DSDS_TYPE_ENUM_UINT8       enDsdsType
);

VOS_VOID CNAS_HRM_SetIsNeedWaitResApplied_DMT(
    VOS_UINT8                           ucIsNeedWaitResApplied
);

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

#endif /* end of CnasHrmCtx.h */

