

#ifndef __IMSAPROCRNICMSG_H__
#define __IMSAPROCRNICMSG_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/

#include    "vos.h"
#include    "ImsaRnicInterface.h"
#include    "ImsaIntraInterface.h"
#include    "ImsaEntity.h"
/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
#pragma pack(*)    设置字节对齐方式
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/


/*****************************************************************************
  3 Massage Declare
*****************************************************************************/



/*****************************************************************************
  4 Enum
*****************************************************************************/




/*****************************************************************************
  5 STRUCT
*****************************************************************************/


/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/

/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/
extern VOS_VOID IMSA_SndMsgRnicPdnActInd(const IMSA_PDP_CNTXT_INFO_STRU     *pstPdpInfo);
extern VOS_VOID IMSA_SndMsgRnicPdnDeactInd
(
    VOS_UINT8                           ucPdpId,
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType

);
extern VOS_VOID IMSA_SndMsgRnicPdnModifyInd
(
    const IMSA_PDP_CNTXT_INFO_STRU     *pstPdpInfo
);

extern VOS_VOID IMSA_SndMsgRnicWifiPdnModifyInd(const IMSA_WIFI_PDP_CNTXT_INFO_STRU     *pstPdpInfo);

extern VOS_VOID IMSA_SndMsgRnicWifiPdnActInd(const IMSA_WIFI_PDP_CNTXT_INFO_STRU     *pstPdpInfo);
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

#endif /* end of ImsaProcRnicMsg.h */



