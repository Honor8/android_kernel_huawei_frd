

#ifndef __NASMMCTX_H__
#define __NASMMCTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "MM_Global.h"
#include "Nasrrcinterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define NAS_MM_CM_TI_INVALID            16

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


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
    VOS_UINT32                          ulTi;
}NAS_MM_LCS_EST_REQ_STRU;


typedef struct
{
    VOS_UINT32                          ulTi;
}NAS_MM_LCS_REL_REQ_STRU;


typedef struct
{
    NAS_MM_LCS_EST_REQ_STRU             stLcsEstReq;                            /* MMģ��LCS���������¼ */
    NAS_MM_LCS_REL_REQ_STRU             stLcsRelReq;                            /* MMģ��LCS�ͷ���·���� */
}NAS_MM_LCS_ENTITY_CTX_STRU;


typedef struct
{
    NAS_MM_LCS_ENTITY_CTX_STRU          stLcsEntityCtx;                         /* CM��LCSҵ��ʵ�������� */
}NAS_MM_CM_ENTITY_CTX_STRU;

typedef struct
{
    RRMM_LCS_STATE_ENUM_UINT8           enLcsState;                             /* �����RRLP��RRC LCS����״̬ */
    VOS_UINT8                           aucReserved[3];
}NAS_MM_RR_INFO_CTX_STRU;


typedef struct
{
    NAS_MM_CM_ENTITY_CTX_STRU           stCmEntityCtx;                          /* MMģ���ŵ�CM��ʵ�������� */
    NAS_MM_RR_INFO_CTX_STRU             stRrInfoCtx;                            /* MMģ���Ž������Ϣ������ */
}NAS_MM_CONTEXT_STRU;



/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern MM_CONN_CTRL_STRU *NAS_MM_GetConnCtrlInfo(
           NAS_MM_CONN_CTRL_ENUM_UINT8         enMmConnCtrl
       );
extern NAS_MM_CONTEXT_STRU* NAS_MM_GetCtx(VOS_VOID);
extern VOS_UINT32 NAS_MM_GetLcsEstReqTi(VOS_VOID);
extern VOS_UINT32 NAS_MM_GetLcsRelReqTi(VOS_VOID);
extern NAS_MM_STATE_ENUM_UINT8 NAS_MM_GetMmPreState(VOS_VOID);
extern MM_SERVICE_STATUS_ENUM_UINT8 NAS_MM_GetMmServiceState(VOS_VOID);
extern NAS_MM_STATE_ENUM_UINT8 NAS_MM_GetMmState(VOS_VOID);
extern RRMM_LCS_STATE_ENUM_UINT8 NAS_MM_GetRrLcsState(VOS_VOID);
extern NAS_MM_STATE_ENUM_UINT8 NAS_MM_GetStaOfRcvXXEstReq(VOS_VOID);
extern VOS_VOID NAS_MM_InitCtx(VOS_VOID);
extern VOS_VOID NAS_MM_InitLcsEntityCtx(VOS_VOID);
extern VOS_VOID NAS_MM_InitRrInfoCtx(VOS_VOID);
extern VOS_VOID NAS_MM_SetLcsEstReqTi(VOS_UINT32 ulTi);
extern VOS_VOID NAS_MM_SetLcsRelReqTi(VOS_UINT32 ulTi);
extern VOS_VOID NAS_MM_SetMMConnEstingPD(
           VOS_UINT8                           ucMmPD
       );
extern VOS_VOID NAS_MM_SetMmPreState(
           NAS_MM_STATE_ENUM_UINT8             enState
       );
extern VOS_VOID NAS_MM_SetMmServiceState(
           MM_SERVICE_STATUS_ENUM_UINT8        enMmServiceState
       );
extern VOS_VOID NAS_MM_SetRrLcsState(
           RRMM_LCS_STATE_ENUM_UINT8           enLcsState
       );
extern VOS_VOID NAS_MM_SetStaAfterWaitForNwkCmd(
           NAS_MM_STATE_ENUM_UINT8             enState
       );
extern VOS_VOID NAS_MM_SetStaOfRcvXXEstReq(
           NAS_MM_STATE_ENUM_UINT8             enState
       );







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

#endif /* end of NasMmCtx.h */

