/** ****************************************************************************

                    Huawei Technologies Sweden AB (C), 2001-2014

 ********************************************************************************
 * @author    Automatically generated by DAISY
 * @version
 * @date      2014-12-29
 * @file
 * @brief
 *
 * @copyright Huawei Technologies Sweden AB
 *******************************************************************************/
#ifndef HSM_HLU_PIF_H
#define HSM_HLU_PIF_H

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
 * Name        : HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16
 *
 * Description :
 *******************************************************************************/
enum HSM_HLU_PIF_MSG_TYPE_ENUM
{
    ID_HSM_HLU_1X_TO_HRPD_HANDOFF_REQ   = 0x0001, /* _H2ASN_MsgChoice HSM_HLU_1X_TO_HRPD_HANDOFF_REQ_STRU*/ /**< @sa HSM_HLU_1X_TO_HRPD_HANDOFF_REQ_STRU */
    ID_HLU_HSM_1X_TO_HRPD_HANDOFF_CNF   = 0x0002, /* _H2ASN_MsgChoice HLU_HSM_1X_TO_HRPD_HANDOFF_CNF_STRU*/ /**< @sa HLU_HSM_1X_TO_HRPD_HANDOFF_CNF_STRU */
    ID_HSM_HLU_SESSION_CHANGED_IND      = 0x0003, /* _H2ASN_MsgChoice HSM_HLU_SESSION_CHANGED_IND_STRU*/ /**< @sa HSM_HLU_SESSION_CHANGED_IND_STRU */
    ID_HLU_HSM_LOC_CHANGED_IND          = 0x0004, /* _H2ASN_MsgChoice HLU_HSM_LOC_CHANGED_IND_STRU*/ /**< @sa HLU_HSM_LOC_CHANGED_IND_STRU */
    ID_HSM_HLU_PIF_MSG_TYPE_ENUM_BUTT   = 0xFFFF
};
typedef VOS_UINT16 HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16;

/** ****************************************************************************
 * Name        : HSM_HLU_SESSION_RAT_MODE_ENUM_UINT8
 *
 * Description :
 *******************************************************************************/
enum HSM_HLU_SESSION_RAT_MODE_ENUM
{
    HSM_HLU_SESSION_RAT_MODE_EHRPD      = 0x00,
    HSM_HLU_SESSION_RAT_MODE_HRPD       = 0x01,
    HSM_HLU_SESSION_RAT_MODE_ENUM_BUTT  = 0x02
};
typedef VOS_UINT8 HSM_HLU_SESSION_RAT_MODE_ENUM_UINT8;

/** ****************************************************************************
 * Name        : CNAS_HLU_HSM_1X_TO_HRPD_HANDOFF_RSLT_ENUM_UINT32
 *
 * Description :
 *******************************************************************************/
enum CNAS_HLU_HSM_1X_TO_HRPD_HANDOFF_RSLT_ENUM
{
    CNAS_HLU_HSM_HANDOFF_SUCCESS                            = 0x00000000, /* CTTF_HRPD_SNP_RLST_SUCC */
    CNAS_HLU_HSM_HANDOFF_FAILURE                            = 0x00000001, /* CTTF_HRPD_SNP_RSLT_FAIL */
    CNAS_HLU_HSM_HANDOFF_UNSOLICIT_NOT_ALLOWED              = 0x00000002, /* Added by HLU on top of CTTF enum */
    CNAS_HLU_HSM_1X_TO_HRPD_HANDOFF_RSLT_ENUM_BUTT          = 0x00000003
};
typedef VOS_UINT32 CNAS_HLU_HSM_1X_TO_HRPD_HANDOFF_RSLT_ENUM_UINT32;
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
 * Name        : HSM_HLU_1X_TO_HRPD_HANDOFF_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /* _H2ASN_Skip */
    HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;            /* _H2ASN_Skip */
    VOS_UINT16                          usOpId;
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
    VOS_UINT8                           ucPktZoneId;
    VOS_UINT8                           aucReserved[3];
} HSM_HLU_1X_TO_HRPD_HANDOFF_REQ_STRU;

/** ****************************************************************************
 * Name        : HLU_HSM_1X_TO_HRPD_HANDOFF_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /* _H2ASN_Skip */
    HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;            /* _H2ASN_Skip */
    VOS_UINT16                          usOpId;
    VOS_UINT32                          ulCause;
} HLU_HSM_1X_TO_HRPD_HANDOFF_CNF_STRU;

/** ****************************************************************************
 * Name        : HSM_HLU_SESSION_CHANGED_IND_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16                        enMsgId;            /* _H2ASN_Skip */
    VOS_UINT16                                              usOpId;
    HSM_HLU_SESSION_RAT_MODE_ENUM_UINT8                     enRatMode;
    VOS_UINT8                                               aucReserve[3];
} HSM_HLU_SESSION_CHANGED_IND_STRU;

/** ****************************************************************************
 * Name        : HLU_HSM_LOC_CHANGED_IND_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /* _H2ASN_Skip */
    HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;            /* _H2ASN_Skip */
    VOS_UINT16                          usOpId;
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
    VOS_UINT8                           ucPktZoneId;
    VOS_UINT8                           aucReserved[3];
} HLU_HSM_LOC_CHANGED_IND_STRU;


/** ****************************************************************************
 * Name        : HSM_HLU_PIF_MSG_DATA
 * Description : H2ASN top level message structure definition
 *******************************************************************************/
typedef struct
{
    HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;     /* _H2ASN_MsgChoice_Export HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16 */
    VOS_UINT8                           aucMsgBlock[2];

    /* _H2ASN_MsgChoice_When_Comment HSM_HLU_PIF_MSG_TYPE_ENUM_UINT16 */
}HSM_HLU_PIF_MSG_DATA;

/* _H2ASN_Length UINT32*/
/** ****************************************************************************
 * Name        : HSM_HLU_PIF
 * Description : H2ASN top level message structure definition
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    HSM_HLU_PIF_MSG_DATA                stMsgData;
}HSM_HLU_PIF;

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
