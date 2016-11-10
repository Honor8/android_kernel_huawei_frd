/******************************************************************************

        @(#)Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
    File name   : NasCommPulic.c
    Description : NAS����ģ��
    History     :
     1.���� 00307272        2016-04-13  Draft Enact

******************************************************************************/


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasCommPublic.h"
#include    "LPsOm.h"

/*lint -e767*/
#define    THIS_FILE_ID            PS_FILE_ID_NASCOMMPUBLIC_C
#define    THIS_NAS_FILE_ID        NAS_FILE_ID_NASCOMMPUBLIC_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/


/*****************************************************************************
  3 Function
*****************************************************************************/


VOS_INT32 NAS_COMM_SecuMemCpy
(
    VOS_VOID            * pDst,
    VOS_UINT32          ulMaxBuffer,
    const VOS_VOID      * pSrc,
    VOS_UINT32          ulLength,
    VOS_UINT32          ulLineNO,
    VOS_UINT32          ulFileID
)
{
    /*���Դ��ַ��Ŀ�ĵ�ַ�ص��������TLPS_MEM_MOVE_S������ȫ����ʧ�� */
    if(((pSrc > pDst) && ((VOS_UINT8 *)pSrc <= ((VOS_UINT8 *)pDst + ulMaxBuffer)))
        ||((pDst> pSrc) && ((VOS_UINT8 *)pDst <= ((VOS_UINT8 *)pSrc + ulLength)))
        || (pSrc == pDst))
    {
        return TLPS_MEM_MOVE_S((pDst), (ulMaxBuffer), (pSrc), (ulLength), (ulLineNO), (ulFileID));
    }
    else
    {
        return TLPS_MEM_CPY_S((pDst), (ulMaxBuffer), (pSrc), (ulLength), (ulLineNO), (ulFileID));
    }
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of NasCommBuffer.c */

