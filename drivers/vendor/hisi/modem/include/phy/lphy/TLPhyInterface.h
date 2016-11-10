
#ifndef __TL_PHY_INTERFACE_H__
#define __TL_PHY_INTERFACE_H__

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
#include "product_config.h"

/************************************************************
                               �궨��
************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


#if (defined(TL_PHY_ASIC_V760))
    #include "comm/hi6960/TLPhyInterfaceLower.h"
#else
    #include "comm/hi6950/TLPhyInterfaceLower.h"
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* __MEM_MAP_BASE_H__ */
