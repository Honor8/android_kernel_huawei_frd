/** ****************************************************************************

                    Huawei Technologies Sweden AB (C), 2001-2016

 ********************************************************************************
 * @author    Automatically generated by DAISY
 * @version
 * @date      2016-02-25 09:39:22
 * @file
 * @brief
 * @copyright Huawei Technologies Sweden AB
 *******************************************************************************/
#ifndef HSD_HRM_PIF_H
#define HSD_HRM_PIF_H

/*******************************************************************************
 1. Other files included
*******************************************************************************/

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#pragma pack(4)

/*******************************************************************************
 2. Macro definitions
*******************************************************************************/


/*******************************************************************************
 3. Enumerations declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : HSD_HRM_PIF_MSG_TYPE_ENUM_UINT16
 *
 * Description :
 *******************************************************************************/
enum HSD_HRM_PIF_MSG_TYPE_ENUM
{
    ID_HSD_HRM_START_REQ                = 0x0000, /**< @sa HSD_HRM_START_REQ_STRU */
    ID_HRM_HSD_START_CNF                = 0x0001, /**< @sa HRM_HSD_START_CNF_STRU */
    ID_HSD_HRM_POWEROFF_REQ             = 0x0002, /**< @sa HSD_HRM_POWEROFF_REQ_STRU */
    ID_HRM_HSD_POWEROFF_CNF             = 0x0003, /**< @sa HRM_HSD_POWEROFF_CNF_STRU */
    ID_HSD_HRM_PIF_MSG_TYPE_ENUM_BUTT   = 0x0004
};
typedef VOS_UINT16 HSD_HRM_PIF_MSG_TYPE_ENUM_UINT16;

/** ****************************************************************************
 * Name        : HRM_HSD_START_RSLT_ENUM_UINT32
 *
 * Description :
 *******************************************************************************/
enum HRM_HSD_START_RSLT_ENUM
{
    HRM_HSD_START_RSLT_SUCC             = 0x00000000,
    HRM_HSD_START_RSLT_FAIL             = 0x00000001,
    HRM_HSD_START_RSLT_ENUM_BUTT        = 0x00000002
};
typedef VOS_UINT32 HRM_HSD_START_RSLT_ENUM_UINT32;

/** ****************************************************************************
 * Name        : HRM_HSD_POWEROFF_RSLT_ENUM_UINT32
 *
 * Description :
 *******************************************************************************/
enum HRM_HSD_POWEROFF_RSLT_ENUM
{
    HRM_HSD_POWEROFF_RSLT_SUCC          = 0x00000000,
    HRM_HSD_POWEROFF_RSLT_FAIL          = 0x00000001,
    HRM_HSD_POWEROFF_RSLT_ENUM_BUTT     = 0x00000002
};
typedef VOS_UINT32 HRM_HSD_POWEROFF_RSLT_ENUM_UINT32;

/*******************************************************************************
 4. Message Header declaration
*******************************************************************************/

/*******************************************************************************
 5. Message declaration
*******************************************************************************/

/*******************************************************************************
 6. STRUCT and UNION declaration
*******************************************************************************/

/*******************************************************************************
 7. OTHER declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : HSD_HRM_START_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    HSD_HRM_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;
    VOS_UINT16                          usOpId;
} HSD_HRM_START_REQ_STRU;

/** ****************************************************************************
 * Name        : HRM_HSD_START_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    HSD_HRM_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;
    VOS_UINT16                          usOpId;
    HRM_HSD_START_RSLT_ENUM_UINT32      enRslt;
} HRM_HSD_START_CNF_STRU;

/** ****************************************************************************
 * Name        : HSD_HRM_POWEROFF_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    HSD_HRM_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;
    VOS_UINT16                          usOpId;
} HSD_HRM_POWEROFF_REQ_STRU;

/** ****************************************************************************
 * Name        : HRM_HSD_POWEROFF_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    HSD_HRM_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;
    VOS_UINT16                          usOpId;
    HRM_HSD_POWEROFF_RSLT_ENUM_UINT32   enRslt;
} HRM_HSD_POWEROFF_CNF_STRU;

/*******************************************************************************
 8. Global  declaration
*******************************************************************************/

/*******************************************************************************
 9. Function declarations
*******************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
