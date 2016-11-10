

#ifndef __NASMMSNDLCS_H__
#define __NASMMSNDLCS_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "MmCmInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/


/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/
extern VOS_VOID NAS_MM_SndLcsDataInd(
           VOS_UINT32                          ulTransactionId,
           VOS_VOID                           *pNasMsg,
           VOS_UINT32                          ulNasMsgLen
       );
extern VOS_VOID NAS_MM_SndLcsEstCnf(
           VOS_UINT32                          ulTransactionId
       );
extern VOS_VOID NAS_MM_SndLcsRelInd(
           VOS_UINT32                          ulTransactionId,
           NAS_MMCM_REL_CAUSE_ENUM_UINT32      enRelCause
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

#endif /* end of NasMmSndLcs.h */

