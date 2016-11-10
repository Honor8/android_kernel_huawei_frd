
#ifndef _NAS_MSCC_FSM_SYS_ACQ_TBL_H_
#define _NAS_MSCC_FSM_SYS_ACQ_TBL_H_

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#include  "vos.h"
#include  "PsTypeDef.h"
#include  "NasFsm.h"
#include  "ImsaMsccInterface.h"
#include  "MsccMmcInterface.h"
#include  "MmaMsccInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  1 全局变量定义
*****************************************************************************/

extern NAS_STA_STRU                            g_astNasMsccSysAcqStaTbl[];


/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define NAS_MSCC_GetSysAcqStaTbl()                   (g_astNasMsccSysAcqStaTbl)


/*****************************************************************************
  3 枚举定义
*****************************************************************************/


enum NAS_MSCC_SYSACQ_STA_ENUM
{
    NAS_MSCC_SYSACQ_STA_INIT,

    NAS_MSCC_SYSACQ_STA_WAIT_HSD_ACQUIRE_CNF,

    NAS_MSCC_SYSACQ_STA_WAIT_HSD_POWER_SAVE_CNF,

    NAS_MSCC_SYSACQ_STA_WAIT_MMC_PLMN_SEARCH_CNF,

    NAS_MSCC_SYSACQ_STA_WAIT_MMC_RF_AVAILABLE_IND,

    NAS_MSCC_SYSACQ_STA_WAIT_MMC_POWER_SAVE_CNF,

    NAS_MSCC_SYSACQ_STA_WAIT_INIT_LOC_INFO_IND,

    NAS_MSCC_SYSACQ_STA_BUTT
};
typedef VOS_UINT32  NAS_MSCC_SYSACQ_STA_ENUM_UINT32;

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
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
VOS_UINT32 NAS_MSCC_GetSysAcqStaTblSize(VOS_VOID);

NAS_FSM_DESC_STRU * NAS_MSCC_GetSysAcqFsmDescAddr(VOS_VOID);
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

#endif /* end of NasMsccFsmSwitchOnTbl.h */



