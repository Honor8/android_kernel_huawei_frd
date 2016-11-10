
#ifndef __TL_PHY_INTERFACE_LOWER_H__
#define __TL_PHY_INTERFACE_LOWER_H__

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

#define DUAL_DSP_FOR_DUAL_4G    //LPHY_FOR_EMU_DBG
#define COMPILE_BBE16_CORE0     //LPHY_FOR_EMU_DBG

#if (defined(DUAL_DSP_FOR_DUAL_4G))
    #define LPHY_GET_COREIDX_FROM_MODEMID(m)           (m)   //modemID m(0��1)��Ӧ��DSP CORE indexҲ��0��1
    #define LPHY_GET_MODEMIDX_FROM_MODEMID(m)          (0)   //modemID m(0��1)��Ӧ��modemIndex����0

    #if (defined(COMPILE_BBE16_CORE0))
        #define LPHY_GET_MODEMID_FROM_MODEMIDX(m)      (0)   //modemIndex m(0)��Ӧ��modemID��0
    #else
        #define LPHY_GET_MODEMID_FROM_MODEMIDX(m)      (1)   //modemIndex m(0)��Ӧ��modemID��1
    #endif
#else
    #define LPHY_GET_COREIDX_FROM_MODEMID(m)           (0)   //modemID m(0��1)��Ӧ��DSP CORE index����0
    #define LPHY_GET_MODEMIDX_FROM_MODEMID(m)          (m)   //modemID m(0��1)��Ӧ��modemIndexҲ��0��1
    #define LPHY_GET_MODEMID_FROM_MODEMIDX(m)          (m)   //modemindex m(0��1)��Ӧ��modemID��0��1
#endif
/*-------------------------------------------------------------------------*
 *                  Hi6960[Balong760] ASIC[/SFT] board configuration       *
 *-------------------------------------------------------------------------*/
    #define MASTER_MODEM0_TCM_ADDR_OFFSET           (0x00000000UL)  //LPHY_FOR_EMU_DBG, ��Ҫ����boston��ַƫ��
    #define MASTER_MODEM1_TCM_ADDR_OFFSET           (0x00000000UL)  //LPHY_FOR_EMU_DBG, ��Ҫ����boston��ַƫ��
    #define MASTER_TCM_ADDR_OFFSET                  (0x00000000UL)  //LPHY_FOR_EMU_DBG, ��Ҫ����boston��ַƫ��
//    #define SLAVE_TCM_ADDR_OFFSET                   (MASTER_TCM_ADDR_OFFSET)
    #define LPHY_DSP_ARM_RAM_BASE_OFFSET            (0x00000000UL - MASTER_TCM_ADDR_OFFSET)
    //#define TLPHY_BBP_BASE_ADDR                     (0x81000000UL)

    #define LPHY_CHIP_BB_6960CS
    #define LPHY_RFIC_HI6363
    #define LPHY_ASIC_V760
    #define LPHY_V760
    #define LPHY_ASIC                                "V700R600"
    #define PHY_VER_SEG                             __attribute__((section(".lphy.version")))
    #define SYS_DBG_SEG                             __attribute__((section(".sys.dbg.seg")))

/*-------------------------------------------------------------------------*
 *      Difference between [FPGA/ASIC] and pc-based verification platform  *
 *-------------------------------------------------------------------------*/
#if defined(MS_VC6_PLATFORM)
    #define LPHY_LMAC_HARQ_BUFF_BASE_ADDR           (LPHY_SIM_LMAC_HARQ_BUFF_BASE_ADDR)
    #define LPHY_SHARE_MEM_BASE_ADDR_FOR_DSP        (LPHY_SIM_SHARE_MEM_BASE_ADDR_FOR_DSP)
    #define LPHY_SHARE_MEM_BASE_ADDR_FOR_SOC        (LPHY_SIM_SHARE_MEM_BASE_ADDR_FOR_DSP)

    #undef  PHY_VER_SEG
    #undef  SYS_DBG_SEG
    #define PHY_VER_SEG
    #define SYS_DBG_SEG
#else
    #define LPHY_LMAC_HARQ_BUFF_BASE_ADDR                         (TLPHY_BBP_BASE_ADDR + 0x680000UL)
    #define LPHY_SHARE_MEM_BASE_ADDR_FOR_DSP                      (LPHY_PUB_DTCM_BASE)

    #define LPHY_MODEM0_SHARE_MEM_BASE_ADDR_FOR_SOC               (LPHY_PUB_DTCM_BASE + MASTER_MODEM0_TCM_ADDR_OFFSET) //��BOSTON���� LPHY_FOR_EMU_DBG
    #define LPHY_MODEM1_SHARE_MEM_BASE_ADDR_FOR_SOC               (LPHY_PUB_DTCM_BASE + MASTER_MODEM1_TCM_ADDR_OFFSET) //��BOSTON���� LPHY_FOR_EMU_DBG
#endif

    #define LPHY_LMAC_TIMER_LOCK_ADDR                             (TLPHY_BBP_BASE_ADDR + 0x70E094UL)
    #define LPHY_LMAC_FRAME_ADDR                                  (TLPHY_BBP_BASE_ADDR + 0x70E014UL)
    #define LPHY_LMAC_SUB_FRAME_ADDR                              (TLPHY_BBP_BASE_ADDR + 0x70E018UL)
    #define LPHY_LMAC_HARQ_BUFF_ADDR_BY_HARQID(ucHarqId)          (LPHY_LMAC_HARQ_BUFF_BASE_ADDR + (ucHarqId*0x4000))

/*�������ַ����*/
#if (defined(TENSILICA_PLATFORM) || defined(MS_VC6_PLATFORM))    /*DSP access mailbox base address*/
    #define LPHY_MODEM0_MAILBOX_BASE_ADDR                         (LPHY_SHARE_MEM_BASE_ADDR_FOR_DSP)
    #define LPHY_MODEM1_MAILBOX_BASE_ADDR                         (LPHY_SHARE_MEM_BASE_ADDR_FOR_DSP)
#else
    #define LPHY_MODEM0_MAILBOX_BASE_ADDR                         (LPHY_MODEM0_SHARE_MEM_BASE_ADDR_FOR_SOC)
    #define LPHY_MODEM1_MAILBOX_BASE_ADDR                         (LPHY_MODEM1_SHARE_MEM_BASE_ADDR_FOR_SOC)
#endif


/************************************************************
                             ���ݽṹ����
************************************************************/



/*****************************************************************************
 ENUM NAME: LPHY_WORKING_MODE_ENUM defined for LPHY_MAILBOX_LPHY_WORKING_MODE_ADDR
 INTENTION: TLPHY SYSTEM WORKING MODE DEFINE
*****************************************************************************/
typedef enum __LPHY_WORKING_MODE_ENUM__
{
    LPHY_WORKING_MODE_RTT = 0,
    LPHY_WORKING_MODE_SINGLE_MODE  = 1,
    LPHY_WORKING_MODE_MULTI_MODE   = 2,
    LPHY_WORKING_MODE_DUO_SIM_MODE = 3,
}LPHY_WORKING_MODE_ENUM;

/*****************************************************************************
 ENUM NAME: LPHY_RTT_LPC_MODE_ENUM defined for power-saving control mode
 INTENTION: TLDSP POWER SAVING MODE
*****************************************************************************/
typedef enum __LPHY_RTT_LPC_MODE_ENUM__
{
    POWER_SAVING_NULL,
    POWER_SAVING_SNOOZE,
    POWER_SAVING_DEEP_SLEEP
}LPHY_RTT_LPC_MODE_ENUM;

typedef unsigned short LPHY_RTT_LPC_MODE_ENUM_UINT16;
/*****************************************************************************
 STRU NAME: MULTIMODE_BB_STARTUP_MODE_IND_STRU defined for LPHY_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR
 INTENTION: Multimode startup indicator
*****************************************************************************/
typedef struct __MULTIMODE_BB_STARTUP_MODE_IND_STRU__
{
    unsigned int bLteStartInd    :4;    //0:Inactive,1:Active
    unsigned int bTdsStartInd    :4;    //0:Inactive,1:Active
    unsigned int bReserved       :24;
}MULTIMODE_BB_STARTUP_MODE_IND_STRU;

/*****************************************************************************
 STRU NAME: TLPHY_DDR_ADDR_INFO_STRU defined TLPHY_DDR_ADDR_INFO
 INTENTION: ps pre-powerup initialized ddr segment base address list for dsp
*****************************************************************************/
typedef struct __OUTER_MEM_IND_STRU__
{
    unsigned long ulBase;
    unsigned long ulSize;
}OUTER_MEM_IND_STRU;

typedef struct __TLPHY_OUTER_MEM_INFO_STRU__
{
    OUTER_MEM_IND_STRU stPhyImagBuff;      //Image buffer
    OUTER_MEM_IND_STRU stSdrDataBuff;      //Sdr buff for csu & emu
    OUTER_MEM_IND_STRU stLcsDataBuff;      //Otdoa carculate buffer
    OUTER_MEM_IND_STRU stLBandNvBuff;      //Lte Band related buffer

    OUTER_MEM_IND_STRU stSRamLpcBuff;      //SRam Low power contol buffer
    OUTER_MEM_IND_STRU stSRamMbxProtect;   //mbx protect flag
    OUTER_MEM_IND_STRU stSRamMntnInfo;     //mbx overflow flag
    OUTER_MEM_IND_STRU stHkADCInfoBuff;    //Get temperature info

    OUTER_MEM_IND_STRU stTimeStampLog;     //timestamp log buffer
    OUTER_MEM_IND_STRU stAfcTraceInfo;     //afc trace information

    OUTER_MEM_IND_STRU stHifiMbxULHead;
    OUTER_MEM_IND_STRU stHifiMbxULBody;
    OUTER_MEM_IND_STRU stHifiMbxDLHead;
    OUTER_MEM_IND_STRU stHifiMbxDLBody;

    OUTER_MEM_IND_STRU stDsdsConfigInfo;    //dsds configure descriptor
    OUTER_MEM_IND_STRU stRsvdBuffInfo;      //reserved ddr buffer descriptor
}TLPHY_OUTER_MEM_INFO_STRU;



/************************************************************
                               �궨��
************************************************************/


/*****************************************************************************
 MARC NAME: MAILBOX DEFINE
 INTENTION: MAILBOX defined List
*****************************************************************************/
#if (defined(DUAL_DSP_FOR_DUAL_4G))
    #define LPHY_MODEM0_MAILBOX_SHARE_MEM_SIZE                 (0x00000400UL)  /*1k�ֽڣ�TBD*/
    #define LPHY_MODEM0_MAILBOX_DEBUG_SEG_SIZE                 (0x00000C00UL)  /*3k�ֽڣ�TBD*/
    #define LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_SIZE          (0x00002400UL)  /*9K�ֽ�, TBD*/
    #define LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_SIZE           (0x00001000UL)  /*9K�ֽ�, TBD*/
    #define LPHY_MODEM0_MAILBOX_LHPA_UP_SIZE                   (0x00002800UL)  /*10K�ֽڣ�PS���������С*/
    #define LPHY_MODEM0_MAILBOX_LHPA_DOWN_SIZE                 (0x00002800UL)  /*10k�ֽڣ�PS���������С*/
    #define LPHY_MODEM0_MAILBOX_OM_UP_SIZE                     (0x00000400UL)  /*1k�ֽڣ�OM���������С*/
    #define LPHY_MODEM0_MAILBOX_OM_DOWN_SIZE                   (0x00000800UL)  /*2k�ֽڣ�OM���������С*/
    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_DLTB_QUEUE_SIZE                (0x00000400UL)  /*1k�ֽڣ�MACPHY����TB QUEUE�����С*/
    #define LPHY_MODEM0_MAILBOX_LPP_DOWN_SIZE                  (0x00000A00UL)  /*2.5�ֽڣ�LPP ��λ������Ϣ�����С*/
    #define LPHY_MODEM0_MAILBOX_TOA_SIZE                       (0x00000600UL)  /*1.5k�ֽڣ�TOA�����С*/
    #define LPHY_MODEM0_LPHY_NV_CFG_SIZE                       (0x00002800UL)  /*10k�ֽڣ�LTE NV����������С*/
    #define LPHY_MODEM0_TPHY_NV_CFG_SIZE                       (0x00001c00UL)  /*07k�ֽڣ�TDS NV����������С*/
    #define LPHY_MODEM0_LTPHY_NV_COMM_CFG_SIZE                 (0x00000800UL)  /*02k�ֽڣ�LT PHY ����NV ��С*/
    #define LPHY_MODEM0_BAND_NV_DDR_IDX_SIZE                   (0x00000800UL)  /*02k�ֽڣ�TDS NV����������С*/

    #define LPHY_MODEM1_MAILBOX_SHARE_MEM_SIZE                 (0x00000400UL)  /*1k�ֽڣ�TBD*/
    #define LPHY_MODEM1_MAILBOX_DEBUG_SEG_SIZE                 (0x00000C00UL)  /*3k�ֽڣ�TBD*/
    #define LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_SIZE          (0x00002400UL)  /*9K�ֽ�, TBD*/
    #define LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_SIZE           (0x00001000UL)  /*9K�ֽ�, TBD*/
    #define LPHY_MODEM1_MAILBOX_LHPA_UP_SIZE                   (0x00002800UL)  /*10K�ֽڣ�PS���������С*/
    #define LPHY_MODEM1_MAILBOX_LHPA_DOWN_SIZE                 (0x00002800UL)  /*10k�ֽڣ�PS���������С*/
    #define LPHY_MODEM1_MAILBOX_OM_UP_SIZE                     (0x00000400UL)  /*1k�ֽڣ�OM���������С*/
    #define LPHY_MODEM1_MAILBOX_OM_DOWN_SIZE                   (0x00000800UL)  /*2k�ֽڣ�OM���������С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_DLTB_QUEUE_SIZE                (0x00000400UL)  /*1k�ֽڣ�MACPHY����TB QUEUE�����С*/
    #define LPHY_MODEM1_MAILBOX_LPP_DOWN_SIZE                  (0x00000A00UL)  /*2.5�ֽڣ�LPP ��λ������Ϣ�����С*/
    #define LPHY_MODEM1_MAILBOX_TOA_SIZE                       (0x00000600UL)  /*1.5k�ֽڣ�TOA�����С*/
    #define LPHY_MODEM1_LPHY_NV_CFG_SIZE                       (0x00002800UL)  /*10k�ֽڣ�LTE NV����������С*/
    #define LPHY_MODEM1_TPHY_NV_CFG_SIZE                       (0x00001c00UL)  /*07k�ֽڣ�TDS NV����������С*/
    #define LPHY_MODEM1_LTPHY_NV_COMM_CFG_SIZE                 (0x00000800UL)  /*02k�ֽڣ�LT PHY ����NV ��С*/
    #define LPHY_MODEM1_BAND_NV_DDR_IDX_SIZE                   (0x00000800UL)  /*02k�ֽڣ�TDS NV����������С*/

    #define LPHY_TOTAL_RSVD_MEM_SIZE                           (128*1024)      /*128K*/
#else   //Two LTE Share One DSP
    #define LPHY_MODEM0_MAILBOX_SHARE_MEM_SIZE                 (0x00000400UL)  /*1k�ֽڣ�TBD*/
    #define LPHY_MODEM0_MAILBOX_DEBUG_SEG_SIZE                 (0x00000C00UL)  /*3k�ֽڣ�TBD*/
    #define LPHY_MODEM1_MAILBOX_SHARE_MEM_SIZE                 (0x00000400UL)  /*1k�ֽڣ�TBD*/
    #define LPHY_MODEM1_MAILBOX_DEBUG_SEG_SIZE                 (0x00000C00UL)  /*3k�ֽڣ�TBD*/
    #define LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_SIZE          (0x00002400UL)  /*9K�ֽ�, TBD*/
    #define LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_SIZE           (0x00001000UL)  /*9K�ֽ�, TBD*/
    #define LPHY_MODEM0_MAILBOX_LHPA_UP_SIZE                   (0x00002800UL)  /*10K�ֽڣ�PS���������С*/
    #define LPHY_MODEM0_MAILBOX_LHPA_DOWN_SIZE                 (0x00002800UL)  /*10k�ֽڣ�PS���������С*/
    #define LPHY_MODEM0_MAILBOX_OM_UP_SIZE                     (0x00000400UL)  /*1k�ֽڣ�OM���������С*/
    #define LPHY_MODEM0_MAILBOX_OM_DOWN_SIZE                   (0x00000800UL)  /*2k�ֽڣ�OM���������С*/
    #define LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_SIZE          (0x00000000UL)  /*LPHY_MODEM1��LPHY_MODEM0����*/
    #define LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_SIZE           (0x00000000UL)  /*LPHY_MODEM1��LPHY_MODEM0����*/
    #define LPHY_MODEM1_MAILBOX_LHPA_UP_SIZE                   (0x00000000UL)  /*LPHY_MODEM1��LPHY_MODEM0����*/
    #define LPHY_MODEM1_MAILBOX_LHPA_DOWN_SIZE                 (0x00000000UL)  /*LPHY_MODEM1��LPHY_MODEM0����*/
    #define LPHY_MODEM1_MAILBOX_OM_UP_SIZE                     (0x00000000UL)  /*LPHY_MODEM1��LPHY_MODEM0����*/
    #define LPHY_MODEM1_MAILBOX_OM_DOWN_SIZE                   (0x00000000UL)  /*LPHY_MODEM1��LPHY_MODEM0����*/
    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE               (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_SIZE             (0x00000400UL)  /*1k�ֽڣ�MACר������1��CG��С*/
    #define LPHY_MODEM0_MAILBOX_DLTB_QUEUE_SIZE                (0x00000400UL)  /*1k�ֽڣ�MACPHY����TB QUEUE�����С*/
    #define LPHY_MODEM1_MAILBOX_DLTB_QUEUE_SIZE                (0x00000400UL)  /*1k�ֽڣ�MACPHY����TB QUEUE�����С*/
    #define LPHY_MODEM0_MAILBOX_LPP_DOWN_SIZE                  (0x00000A00UL)  /*2.5�ֽڣ�LPP ��λ������Ϣ�����С*/
    #define LPHY_MODEM1_MAILBOX_LPP_DOWN_SIZE                  (0x00000A00UL)  /*2.5�ֽڣ�LPP ��λ������Ϣ�����С*/
    #define LPHY_MODEM0_MAILBOX_TOA_SIZE                       (0x00000600UL)  /*1.5k�ֽڣ�TOA�����С*/
    #define LPHY_MODEM1_MAILBOX_TOA_SIZE                       (0x00000600UL)  /*1.5k�ֽڣ�TOA�����С*/
    #define LPHY_MODEM0_LPHY_NV_CFG_SIZE                       (0x00002800UL)  /*10k�ֽڣ�LTE NV����������С*/
    #define LPHY_MODEM0_TPHY_NV_CFG_SIZE                       (0x00001c00UL)  /*07k�ֽڣ�TDS NV����������С*/
    #define LPHY_MODEM0_LTPHY_NV_COMM_CFG_SIZE                 (0x00000800UL)  /*02k�ֽڣ�LT PHY ����NV ��С*/
    #define LPHY_MODEM0_BAND_NV_DDR_IDX_SIZE                   (0x00000800UL)  /*02k�ֽڣ�TDS NV����������С*/
    #define LPHY_MODEM1_LPHY_NV_CFG_SIZE                       (0x00002800UL)  /*10k�ֽڣ�LTE NV����������С*/
    #define LPHY_MODEM1_TPHY_NV_CFG_SIZE                       (0x00001c00UL)  /*07k�ֽڣ�TDS NV����������С*/
    #define LPHY_MODEM1_LTPHY_NV_COMM_CFG_SIZE                 (0x00000800UL)  /*02k�ֽڣ�LT PHY ����NV ��С*/
    #define LPHY_MODEM1_BAND_NV_DDR_IDX_SIZE                   (0x00000800UL)  /*02k�ֽڣ�TDS NV����������С*/

    #define LPHY_TOTAL_RSVD_MEM_SIZE                           (128*1024)      /*128K*/
#endif
/*****************************************************************************
 MARC NAME: TOTAL MAILBOX SIZE
 INTENTION: TOTAL MAILBOX SIZE
*****************************************************************************/
/*�����ܴ�С*/
#if (defined(DUAL_DSP_FOR_DUAL_4G))
    #if (defined(COMPILE_BBE16_CORE0))
        #define LPHY_TOTAL_USED_MEM_SIZE (              \
              LPHY_MODEM0_MAILBOX_SHARE_MEM_SIZE        \
            + LPHY_MODEM0_MAILBOX_DEBUG_SEG_SIZE        \
            + LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_SIZE \
            + LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_SIZE  \
            + LPHY_MODEM0_MAILBOX_LHPA_UP_SIZE          \
            + LPHY_MODEM0_MAILBOX_LHPA_DOWN_SIZE        \
            + LPHY_MODEM0_MAILBOX_OM_UP_SIZE            \
            + LPHY_MODEM0_MAILBOX_OM_DOWN_SIZE          \
            + LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE      \
            + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE    \
            + LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE      \
            + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_SIZE    \
            + LPHY_MODEM0_MAILBOX_DLTB_QUEUE_SIZE       \
            + LPHY_MODEM0_MAILBOX_LPP_DOWN_SIZE         \
            + LPHY_MODEM0_MAILBOX_TOA_SIZE              \
            + LPHY_MODEM0_LPHY_NV_CFG_SIZE              \
            + LPHY_MODEM0_TPHY_NV_CFG_SIZE              \
            + LPHY_MODEM0_LTPHY_NV_COMM_CFG_SIZE        \
            + LPHY_MODEM0_BAND_NV_DDR_IDX_SIZE)
    #else
        #define LPHY_TOTAL_USED_MEM_SIZE (              \
              LPHY_MODEM1_MAILBOX_SHARE_MEM_SIZE        \
            + LPHY_MODEM1_MAILBOX_DEBUG_SEG_SIZE        \
            + LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_SIZE \
            + LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_SIZE  \
            + LPHY_MODEM1_MAILBOX_LHPA_UP_SIZE          \
            + LPHY_MODEM1_MAILBOX_LHPA_DOWN_SIZE        \
            + LPHY_MODEM1_MAILBOX_OM_UP_SIZE            \
            + LPHY_MODEM1_MAILBOX_OM_DOWN_SIZE          \
            + LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE      \
            + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE    \
            + LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE      \
            + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_SIZE    \
            + LPHY_MODEM1_MAILBOX_DLTB_QUEUE_SIZE       \
            + LPHY_MODEM1_MAILBOX_LPP_DOWN_SIZE         \
            + LPHY_MODEM1_MAILBOX_TOA_SIZE              \
            + LPHY_MODEM1_LPHY_NV_CFG_SIZE              \
            + LPHY_MODEM1_TPHY_NV_CFG_SIZE              \
            + LPHY_MODEM1_LTPHY_NV_COMM_CFG_SIZE        \
            + LPHY_MODEM1_BAND_NV_DDR_IDX_SIZE)
    #endif
#else
    #define LPHY_TOTAL_USED_MEM_SIZE (              \
          LPHY_MODEM0_MAILBOX_SHARE_MEM_SIZE        \
        + LPHY_MODEM0_MAILBOX_DEBUG_SEG_SIZE        \
        + LPHY_MODEM1_MAILBOX_SHARE_MEM_SIZE        \
        + LPHY_MODEM1_MAILBOX_DEBUG_SEG_SIZE        \
        + LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_SIZE \
        + LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_SIZE  \
        + LPHY_MODEM0_MAILBOX_LHPA_UP_SIZE          \
        + LPHY_MODEM0_MAILBOX_LHPA_DOWN_SIZE        \
        + LPHY_MODEM0_MAILBOX_OM_UP_SIZE            \
        + LPHY_MODEM0_MAILBOX_OM_DOWN_SIZE          \
        + LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_SIZE \
        + LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_SIZE  \
        + LPHY_MODEM1_MAILBOX_LHPA_UP_SIZE          \
        + LPHY_MODEM1_MAILBOX_LHPA_DOWN_SIZE        \
        + LPHY_MODEM1_MAILBOX_OM_UP_SIZE            \
        + LPHY_MODEM1_MAILBOX_OM_DOWN_SIZE          \
        + LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE      \
        + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE    \
        + LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE      \
        + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_SIZE    \
        + LPHY_MODEM0_MAILBOX_DLTB_QUEUE_SIZE       \
        + LPHY_MODEM0_MAILBOX_LPP_DOWN_SIZE         \
        + LPHY_MODEM0_MAILBOX_TOA_SIZE              \
        + LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE      \
        + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE    \
        + LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE      \
        + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_SIZE    \
        + LPHY_MODEM1_MAILBOX_DLTB_QUEUE_SIZE       \
        + LPHY_MODEM1_MAILBOX_LPP_DOWN_SIZE         \
        + LPHY_MODEM1_MAILBOX_TOA_SIZE              \
        + LPHY_MODEM0_LPHY_NV_CFG_SIZE              \
        + LPHY_MODEM0_TPHY_NV_CFG_SIZE              \
        + LPHY_MODEM0_LTPHY_NV_COMM_CFG_SIZE        \
        + LPHY_MODEM0_BAND_NV_DDR_IDX_SIZE          \
        + LPHY_MODEM1_LPHY_NV_CFG_SIZE              \
        + LPHY_MODEM1_TPHY_NV_CFG_SIZE              \
        + LPHY_MODEM1_LTPHY_NV_COMM_CFG_SIZE        \
        + LPHY_MODEM1_BAND_NV_DDR_IDX_SIZE)
#endif


/*****************************************************************************
 MARC NAME: Reserved shared memory size check & protection
 INTENTION: Reserved shared memory size check & protection
*****************************************************************************/
#if (LPHY_TOTAL_USED_MEM_SIZE > LPHY_TOTAL_RSVD_MEM_SIZE)
    #error "sorry, reserved memory exhausted!"
#endif

/*****************************************************************************
 MARC NAME: detail mailbox segment base address
 INTENTION: detail mailbox segment base address
*****************************************************************************/
#if (defined(DUAL_DSP_FOR_DUAL_4G))

    /*MODEM0��ַ����*/
    #define LPHY_MODEM0_MAILBOX_VERSION_ADDR                   (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0200)   /*DSP�汾��ѯ��ַ*/
    #define LPHY_MODEM0_MAILBOX_LOAD_MEMORY_ADDR               (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x020C)   /*������ʼ�����ֵ�ַ*/
    #define LPHY_MODEM0_MAILBOX_LPHY_WORKING_MODE_ADDR         (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0210)   /*LTE PHY WorkModeInd*/
    #define LPHY_MODEM0_MAILBOX_LPHY_CTU_REG_BASE_ADDR         (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0214)   /*LTE ����ģʽ*/
    #define LPHY_MODEM0_MAILBOX_LPHY_DSP_NV_PARA_SIZE_ADDR     (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0218)   /*LTE PHY DSP ����ʱ NV Para_Size*/
    #define LPHY_MODEM0_MAILBOX_LPHY_ARM_NV_PARA_SIZE_ADDR     (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x021C)   /*LTE PHY ARM�·���  NV Para_Size*/
    #define LPHY_MODEM0_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR    (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0220)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM0_MAILBOX_IMAG_SIZE_INFO_ADDR            (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0230)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM0_MAILBOX_OUTER_MEM_INFO_ADDR            (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0240)   /*TL PHY ��DDR��ַ������IMAGE SDR LCS BANDNV��64B*/

    #define LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET                 (LPHY_MODEM0_MAILBOX_SHARE_MEM_SIZE             + LPHY_MODEM0_MAILBOX_DEBUG_SEG_SIZE)
    #define LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR     (LPHY_MODEM0_MAILBOX_BASE_ADDR                  + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET)
    #define LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR      (LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR + LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_SIZE)
    #define LPHY_MODEM0_MAILBOX_LHPA_UP_BASE_ADDR              (LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR  + LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_SIZE)
    #define LPHY_MODEM0_MAILBOX_LHPA_DOWN_BASE_ADDR            (LPHY_MODEM0_MAILBOX_LHPA_UP_BASE_ADDR          + LPHY_MODEM0_MAILBOX_LHPA_UP_SIZE)
    #define LPHY_MODEM0_MAILBOX_OM_UP_BASE_ADDR                (LPHY_MODEM0_MAILBOX_LHPA_DOWN_BASE_ADDR        + LPHY_MODEM0_MAILBOX_LHPA_DOWN_SIZE)
    #define LPHY_MODEM0_MAILBOX_OM_DOWN_BASE_ADDR              (LPHY_MODEM0_MAILBOX_OM_UP_BASE_ADDR            + LPHY_MODEM0_MAILBOX_OM_UP_SIZE)

    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_BASE_ADDR          (LPHY_MODEM0_MAILBOX_OM_DOWN_BASE_ADDR          + LPHY_MODEM0_MAILBOX_OM_DOWN_SIZE)
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR        (LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_BASE_ADDR      + LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE)
    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_BASE_ADDR          (LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR    + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE)
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR        (LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_BASE_ADDR      + LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE)
    #define LPHY_MODEM0_MAILBOX_DL_TB_QUEUE_BASE_ADDR          (LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR    + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_SIZE)
    #define LPHY_MODEM0_MAILBOX_LPP_DOWN_BASE_ADDR             (LPHY_MODEM0_MAILBOX_DL_TB_QUEUE_BASE_ADDR      + LPHY_MODEM0_MAILBOX_DLTB_QUEUE_SIZE)
    #define LPHY_MODEM0_MAILBOX_TOA_BASE_ADDR                  (LPHY_MODEM0_MAILBOX_LPP_DOWN_BASE_ADDR         + LPHY_MODEM0_MAILBOX_LPP_DOWN_SIZE)

    #define LPHY_MODEM0_MAILBOX_LPHY_NV_CFG_BASE_ADDR          (LPHY_MODEM0_MAILBOX_TOA_BASE_ADDR              + LPHY_MODEM0_MAILBOX_TOA_SIZE)
    #define LPHY_MODEM0_MAILBOX_TPHY_NV_CFG_BASE_ADDR          (LPHY_MODEM0_MAILBOX_LPHY_NV_CFG_BASE_ADDR      + LPHY_MODEM0_LPHY_NV_CFG_SIZE)
    #define LPHY_MODEM0_MAILBOX_COMM_NV_CFG_BASE_ADDR          (LPHY_MODEM0_MAILBOX_TPHY_NV_CFG_BASE_ADDR      + LPHY_MODEM0_TPHY_NV_CFG_SIZE)
    #define LPHY_MODEM0_MAILBOX_BAND_NV_IDX_BASE_ADDR          (LPHY_MODEM0_MAILBOX_COMM_NV_CFG_BASE_ADDR      + LPHY_MODEM0_LTPHY_NV_COMM_CFG_SIZE)


    /*MODEM1��ַ����*/
    #define LPHY_MODEM1_MAILBOX_VERSION_ADDR                   (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x0200)   /*DSP�汾��ѯ��ַ*/
    #define LPHY_MODEM1_MAILBOX_LOAD_MEMORY_ADDR               (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x020C)   /*������ʼ�����ֵ�ַ*/
    #define LPHY_MODEM1_MAILBOX_LPHY_WORKING_MODE_ADDR         (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x0210)   /*LTE PHY WorkModeInd*/
    #define LPHY_MODEM1_MAILBOX_LPHY_CTU_REG_BASE_ADDR         (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x0214)   /*LTE ����ģʽ*/
    #define LPHY_MODEM1_MAILBOX_LPHY_DSP_NV_PARA_SIZE_ADDR     (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x0218)   /*LTE PHY DSP ����ʱ NV Para_Size*/
    #define LPHY_MODEM1_MAILBOX_LPHY_ARM_NV_PARA_SIZE_ADDR     (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x021C)   /*LTE PHY ARM�·���  NV Para_Size*/
    #define LPHY_MODEM1_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR    (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x0220)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM1_MAILBOX_IMAG_SIZE_INFO_ADDR            (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x0230)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM1_MAILBOX_OUTER_MEM_INFO_ADDR            (LPHY_MODEM1_MAILBOX_BASE_ADDR + 0x0240)   /*TL PHY ��DDR��ַ������IMAGE SDR LCS BANDNV��64B*/

    #define LPHY_MODEM1_MAILBOX_SEGMENT_OFFSET                 (LPHY_MODEM1_MAILBOX_SHARE_MEM_SIZE             + LPHY_MODEM1_MAILBOX_DEBUG_SEG_SIZE)
    #define LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR     (LPHY_MODEM1_MAILBOX_BASE_ADDR                  + LPHY_MODEM1_MAILBOX_SEGMENT_OFFSET)
    #define LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR      (LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR + LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_SIZE)
    #define LPHY_MODEM1_MAILBOX_LHPA_UP_BASE_ADDR              (LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR  + LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_SIZE)
    #define LPHY_MODEM1_MAILBOX_LHPA_DOWN_BASE_ADDR            (LPHY_MODEM1_MAILBOX_LHPA_UP_BASE_ADDR          + LPHY_MODEM1_MAILBOX_LHPA_UP_SIZE)
    #define LPHY_MODEM1_MAILBOX_OM_UP_BASE_ADDR                (LPHY_MODEM1_MAILBOX_LHPA_DOWN_BASE_ADDR        + LPHY_MODEM1_MAILBOX_LHPA_DOWN_SIZE)
    #define LPHY_MODEM1_MAILBOX_OM_DOWN_BASE_ADDR              (LPHY_MODEM1_MAILBOX_OM_UP_BASE_ADDR            + LPHY_MODEM1_MAILBOX_OM_UP_SIZE)

    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_BASE_ADDR          (LPHY_MODEM1_MAILBOX_OM_DOWN_BASE_ADDR          + LPHY_MODEM1_MAILBOX_OM_DOWN_SIZE)
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR        (LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_BASE_ADDR      + LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE)
    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_BASE_ADDR          (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR    + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE)
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR        (LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_BASE_ADDR      + LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE)
    #define LPHY_MODEM1_MAILBOX_DL_TB_QUEUE_BASE_ADDR          (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR    + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_SIZE)
    #define LPHY_MODEM1_MAILBOX_LPP_DOWN_BASE_ADDR             (LPHY_MODEM1_MAILBOX_DL_TB_QUEUE_BASE_ADDR      + LPHY_MODEM1_MAILBOX_DLTB_QUEUE_SIZE)
    #define LPHY_MODEM1_MAILBOX_TOA_BASE_ADDR                  (LPHY_MODEM1_MAILBOX_LPP_DOWN_BASE_ADDR         + LPHY_MODEM1_MAILBOX_LPP_DOWN_SIZE)

    #define LPHY_MODEM1_MAILBOX_LPHY_NV_CFG_BASE_ADDR          (LPHY_MODEM1_MAILBOX_TOA_BASE_ADDR              + LPHY_MODEM1_MAILBOX_TOA_SIZE)
    #define LPHY_MODEM1_MAILBOX_TPHY_NV_CFG_BASE_ADDR          (LPHY_MODEM1_MAILBOX_LPHY_NV_CFG_BASE_ADDR      + LPHY_MODEM1_LPHY_NV_CFG_SIZE)
    #define LPHY_MODEM1_MAILBOX_COMM_NV_CFG_BASE_ADDR          (LPHY_MODEM1_MAILBOX_TPHY_NV_CFG_BASE_ADDR      + LPHY_MODEM1_TPHY_NV_CFG_SIZE)
    #define LPHY_MODEM1_MAILBOX_BAND_NV_IDX_BASE_ADDR          (LPHY_MODEM1_MAILBOX_COMM_NV_CFG_BASE_ADDR      + LPHY_MODEM1_LTPHY_NV_COMM_CFG_SIZE)

#else
    /*˫LTE��һ��DSP*/
    #define LPHY_MODEM0_MAILBOX_VERSION_ADDR                     (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0200)   /*DSP�汾��ѯ��ַ*/
    #define LPHY_MODEM0_MAILBOX_LOAD_MEMORY_ADDR                 (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x020C)   /*������ʼ�����ֵ�ַ*/
    #define LPHY_MODEM0_MAILBOX_LPHY_WORKING_MODE_ADDR           (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0210)   /*LTE PHY WorkModeInd*/
    #define LPHY_MODEM0_MAILBOX_LPHY_CTU_REG_BASE_ADDR           (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0214)   /*LTE ����ģʽ*/
    #define LPHY_MODEM0_MAILBOX_LPHY_DSP_NV_PARA_SIZE_ADDR       (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0218)   /*LTE PHY DSP ����ʱ NV Para_Size*/
    #define LPHY_MODEM0_MAILBOX_LPHY_ARM_NV_PARA_SIZE_ADDR       (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x021C)   /*LTE PHY ARM�·���  NV Para_Size*/
    #define LPHY_MODEM0_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR      (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0220)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM0_MAILBOX_IMAG_SIZE_INFO_ADDR              (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0230)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM0_MAILBOX_OUTER_MEM_INFO_ADDR              (LPHY_MODEM0_MAILBOX_BASE_ADDR + 0x0240)   /*TL PHY ��DDR��ַ������IMAGE SDR LCS BANDNV��64B*/

    #define LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET                   (LPHY_MODEM0_MAILBOX_SHARE_MEM_SIZE + LPHY_MODEM0_MAILBOX_DEBUG_SEG_SIZE)
    #define LPHY_MODEM1_MAILBOX_VERSION_ADDR                     (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x0200)   /*DSP�汾��ѯ��ַ*/
    #define LPHY_MODEM1_MAILBOX_LOAD_MEMORY_ADDR                 (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x020C)   /*������ʼ�����ֵ�ַ*/
    #define LPHY_MODEM1_MAILBOX_LPHY_WORKING_MODE_ADDR           (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x0210)   /*LTE PHY WorkModeInd*/
    #define LPHY_MODEM1_MAILBOX_LPHY_CTU_REG_BASE_ADDR           (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x0214)   /*LTE ����ģʽ*/
    #define LPHY_MODEM1_MAILBOX_LPHY_DSP_NV_PARA_SIZE_ADDR       (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x0218)   /*LTE PHY DSP ����ʱ NV Para_Size*/
    #define LPHY_MODEM1_MAILBOX_LPHY_ARM_NV_PARA_SIZE_ADDR       (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x021C)   /*LTE PHY ARM�·���  NV Para_Size*/
    #define LPHY_MODEM1_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR      (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x0220)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM1_MAILBOX_IMAG_SIZE_INFO_ADDR              (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x0230)   /*��ģ��������ģʽָʾ*/
    #define LPHY_MODEM1_MAILBOX_OUTER_MEM_INFO_ADDR              (LPHY_MODEM0_MAILBOX_BASE_ADDR      + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + 0x0240)   /*TL PHY ��DDR��ַ������IMAGE SDR LCS BANDNV��64B*/

    #define LPHY_MODEM1_MAILBOX_SEGMENT_OFFSET                   (LPHY_MODEM1_MAILBOX_SHARE_MEM_SIZE             + LPHY_MODEM1_MAILBOX_DEBUG_SEG_SIZE)
    #define LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR       (LPHY_MODEM0_MAILBOX_BASE_ADDR                  + LPHY_MODEM0_MAILBOX_SEGMENT_OFFSET + LPHY_MODEM1_MAILBOX_SEGMENT_OFFSET)
    #define LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR        (LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR + LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_SIZE)
    #define LPHY_MODEM0_MAILBOX_LHPA_UP_BASE_ADDR                (LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR  + LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_SIZE)
    #define LPHY_MODEM0_MAILBOX_LHPA_DOWN_BASE_ADDR              (LPHY_MODEM0_MAILBOX_LHPA_UP_BASE_ADDR          + LPHY_MODEM0_MAILBOX_LHPA_UP_SIZE)
    #define LPHY_MODEM0_MAILBOX_OM_UP_BASE_ADDR                  (LPHY_MODEM0_MAILBOX_LHPA_DOWN_BASE_ADDR        + LPHY_MODEM0_MAILBOX_LHPA_DOWN_SIZE)
    #define LPHY_MODEM0_MAILBOX_OM_DOWN_BASE_ADDR                (LPHY_MODEM0_MAILBOX_OM_UP_BASE_ADDR            + LPHY_MODEM0_MAILBOX_OM_UP_SIZE)
    #define LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR       (LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR)
    #define LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR        (LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR)
    #define LPHY_MODEM1_MAILBOX_LHPA_UP_BASE_ADDR                (LPHY_MODEM0_MAILBOX_LHPA_UP_BASE_ADDR)
    #define LPHY_MODEM1_MAILBOX_LHPA_DOWN_BASE_ADDR              (LPHY_MODEM0_MAILBOX_LHPA_DOWN_BASE_ADDR)
    #define LPHY_MODEM1_MAILBOX_OM_UP_BASE_ADDR                  (LPHY_MODEM0_MAILBOX_OM_UP_BASE_ADDR)
    #define LPHY_MODEM1_MAILBOX_OM_DOWN_BASE_ADDR                (LPHY_MODEM0_MAILBOX_OM_DOWN_BASE_ADDR)

    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_BASE_ADDR            (LPHY_MODEM0_MAILBOX_OM_DOWN_BASE_ADDR         + LPHY_MODEM0_MAILBOX_OM_DOWN_SIZE)
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR          (LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_BASE_ADDR     + LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE)
    #define LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_BASE_ADDR            (LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR   + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE)
    #define LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR          (LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_BASE_ADDR     + LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE)
    #define LPHY_MODEM0_MAILBOX_DL_TB_QUEUE_BASE_ADDR            (LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR   + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_SIZE)
    #define LPHY_MODEM0_MAILBOX_LPP_DOWN_BASE_ADDR               (LPHY_MODEM0_MAILBOX_DL_TB_QUEUE_BASE_ADDR     + LPHY_MODEM0_MAILBOX_DLTB_QUEUE_SIZE)
    #define LPHY_MODEM0_MAILBOX_TOA_BASE_ADDR                    (LPHY_MODEM0_MAILBOX_LPP_DOWN_BASE_ADDR        + LPHY_MODEM0_MAILBOX_LPP_DOWN_SIZE)

    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_BASE_ADDR            (LPHY_MODEM0_MAILBOX_TOA_BASE_ADDR             + LPHY_MODEM0_MAILBOX_TOA_SIZE)
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR          (LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_BASE_ADDR     + LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE)
    #define LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_BASE_ADDR            (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR   + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE)
    #define LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR          (LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_BASE_ADDR     + LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE)
    #define LPHY_MODEM1_MAILBOX_DL_TB_QUEUE_BASE_ADDR            (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR   + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_SIZE)
    #define LPHY_MODEM1_MAILBOX_LPP_DOWN_BASE_ADDR               (LPHY_MODEM1_MAILBOX_DL_TB_QUEUE_BASE_ADDR     + LPHY_MODEM1_MAILBOX_DLTB_QUEUE_SIZE)
    #define LPHY_MODEM1_MAILBOX_TOA_BASE_ADDR                    (LPHY_MODEM1_MAILBOX_LPP_DOWN_BASE_ADDR        + LPHY_MODEM1_MAILBOX_LPP_DOWN_SIZE)

    #define LPHY_MODEM0_MAILBOX_LPHY_NV_CFG_BASE_ADDR            (LPHY_MODEM1_MAILBOX_TOA_BASE_ADDR             + LPHY_MODEM1_MAILBOX_TOA_SIZE)
    #define LPHY_MODEM0_MAILBOX_TPHY_NV_CFG_BASE_ADDR            (LPHY_MODEM0_MAILBOX_LPHY_NV_CFG_BASE_ADDR     + LPHY_MODEM0_LPHY_NV_CFG_SIZE)
    #define LPHY_MODEM0_MAILBOX_COMM_NV_CFG_BASE_ADDR            (LPHY_MODEM0_MAILBOX_TPHY_NV_CFG_BASE_ADDR     + LPHY_MODEM0_TPHY_NV_CFG_SIZE)
    #define LPHY_MODEM0_MAILBOX_BAND_NV_IDX_BASE_ADDR            (LPHY_MODEM0_MAILBOX_COMM_NV_CFG_BASE_ADDR     + LPHY_MODEM0_LTPHY_NV_COMM_CFG_SIZE)
    #define LPHY_MODEM1_MAILBOX_LPHY_NV_CFG_BASE_ADDR            (LPHY_MODEM0_MAILBOX_BAND_NV_IDX_BASE_ADDR     + LPHY_MODEM0_BAND_NV_DDR_IDX_SIZE)
    #define LPHY_MODEM1_MAILBOX_TPHY_NV_CFG_BASE_ADDR            (LPHY_MODEM1_MAILBOX_LPHY_NV_CFG_BASE_ADDR     + LPHY_MODEM1_LPHY_NV_CFG_SIZE)
    #define LPHY_MODEM1_MAILBOX_COMM_NV_CFG_BASE_ADDR            (LPHY_MODEM1_MAILBOX_TPHY_NV_CFG_BASE_ADDR     + LPHY_MODEM1_TPHY_NV_CFG_SIZE)
    #define LPHY_MODEM1_MAILBOX_BAND_NV_IDX_BASE_ADDR            (LPHY_MODEM1_MAILBOX_COMM_NV_CFG_BASE_ADDR     + LPHY_MODEM1_LTPHY_NV_COMM_CFG_SIZE)
#endif

/*****************************************************************************
 �����ͳһʹ�����¶����ȡmailbox��С�͵�ַ�����в���m����modemID:
 0:ModemID0; 1:ModemID1
*****************************************************************************/
#define LPHY_MBX_SHARE_MEM_SIZE(m)                       ( 0 == m )? (LPHY_MODEM0_MAILBOX_SHARE_MEM_SIZE       ) : (LPHY_MODEM1_MAILBOX_SHARE_MEM_SIZE       )
#define LPHY_MBX_DEBUG_SEG_SIZE(m)                       ( 0 == m )? (LPHY_MODEM0_MAILBOX_DEBUG_SEG_SIZE       ) : (LPHY_MODEM1_MAILBOX_DEBUG_SEG_SIZE       )
#define LPHY_MBX_HDLC_HIGHPRO_BUF_SIZE(m)                ( 0 == m )? (LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_SIZE) : (LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_SIZE)
#define LPHY_MBX_HDLC_LOWPRO_BUF_SIZE(m)                 ( 0 == m )? (LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_SIZE ) : (LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_SIZE )
#define LPHY_MBX_LHPA_UP_SIZE(m)                         ( 0 == m )? (LPHY_MODEM0_MAILBOX_LHPA_UP_SIZE         ) : (LPHY_MODEM1_MAILBOX_LHPA_UP_SIZE         )
#define LPHY_MBX_LHPA_DOWN_SIZE(m)                       ( 0 == m )? (LPHY_MODEM0_MAILBOX_LHPA_DOWN_SIZE       ) : (LPHY_MODEM1_MAILBOX_LHPA_DOWN_SIZE       )
#define LPHY_MBX_OM_UP_SIZE(m)                           ( 0 == m )? (LPHY_MODEM0_MAILBOX_OM_UP_SIZE           ) : (LPHY_MODEM1_MAILBOX_OM_UP_SIZE           )
#define LPHY_MBX_OM_DOWN_SIZE(m)                         ( 0 == m )? (LPHY_MODEM0_MAILBOX_OM_DOWN_SIZE         ) : (LPHY_MODEM1_MAILBOX_OM_DOWN_SIZE         )
#define LPHY_MBX_LMAC_UP_CG0_SIZE(m)                     ( 0 == m )? (LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE     ) : (LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE     )
#define LPHY_MBX_LMAC_DOWN_CG0_SIZE(m)                   ( 0 == m )? (LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE   ) : (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE   )
#define LPHY_MBX_LMAC_UP_CG1_SIZE(m)                     ( 0 == m )? (LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE     ) : (LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE     )
#define LPHY_MBX_LMAC_DOWN_CG1_SIZE(m)                   ( 0 == m )? (LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_SIZE   ) : (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_SIZE   )
#define LPHY_MBX_DLTB_QUEUE_SIZE(m)                      ( 0 == m )? (LPHY_MODEM0_MAILBOX_DLTB_QUEUE_SIZE      ) : (LPHY_MODEM1_MAILBOX_DLTB_QUEUE_SIZE      )
#define LPHY_MBX_LPP_DOWN_SIZE(m)                        ( 0 == m )? (LPHY_MODEM0_MAILBOX_LPP_DOWN_SIZE        ) : (LPHY_MODEM1_MAILBOX_LPP_DOWN_SIZE        )
#define LPHY_MBX_TOA_SIZE(m)                             ( 0 == m )? (LPHY_MODEM0_MAILBOX_TOA_SIZE             ) : (LPHY_MODEM1_MAILBOX_TOA_SIZE             )

#define LPHY_MBX_LPHY_NV_CFG_SIZE(m)                     ( 0 == m )? (LPHY_MODEM0_LPHY_NV_CFG_SIZE             ) : (LPHY_MODEM1_LPHY_NV_CFG_SIZE             )
#define LPHY_MBX_TPHY_NV_CFG_SIZE(m)                     ( 0 == m )? (LPHY_MODEM0_TPHY_NV_CFG_SIZE             ) : (LPHY_MODEM1_TPHY_NV_CFG_SIZE             )
#define LPHY_MBX_LTPHY_NV_COMM_CFG_SIZE(m)               ( 0 == m )? (LPHY_MODEM0_LTPHY_NV_COMM_CFG_SIZE       ) : (LPHY_MODEM1_LTPHY_NV_COMM_CFG_SIZE       )
#define LPHY_MBX_BAND_NV_DDR_IDX_SIZE(m)                 ( 0 == m )? (LPHY_MODEM0_BAND_NV_DDR_IDX_SIZE         ) : (LPHY_MODEM1_BAND_NV_DDR_IDX_SIZE         )

#define LPHY_MBX_BASE_ADDR(m)                            ( 0 == m )?(LPHY_MODEM0_MAILBOX_BASE_ADDR)              : (LPHY_MODEM1_MAILBOX_BASE_ADDR)

/*�����������ַ����*/
#define LPHY_MBX_HDLC_HIGHPRO_BUF_BASE_ADDR(m)           ( 0 == m )?(LPHY_MODEM0_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR) : (LPHY_MODEM1_MAILBOX_HDLC_HIGHPRO_BUF_BASE_ADDR)
#define LPHY_MBX_HDLC_LOWPRO_BUF_BASE_ADDR(m)            ( 0 == m )?(LPHY_MODEM0_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR)  : (LPHY_MODEM1_MAILBOX_HDLC_LOWPRO_BUF_BASE_ADDR)
#define LPHY_MBX_LHPA_UP_BASE_ADDR(m)                    ( 0 == m )?(LPHY_MODEM0_MAILBOX_LHPA_UP_BASE_ADDR)          : (LPHY_MODEM1_MAILBOX_LHPA_UP_BASE_ADDR)
#define LPHY_MBX_LHPA_DOWN_BASE_ADDR(m)                  ( 0 == m )?(LPHY_MODEM0_MAILBOX_LHPA_DOWN_BASE_ADDR)        : (LPHY_MODEM1_MAILBOX_LHPA_DOWN_BASE_ADDR)
#define LPHY_MBX_OM_UP_BASE_ADDR(m)                      ( 0 == m )?(LPHY_MODEM0_MAILBOX_OM_UP_BASE_ADDR)            : (LPHY_MODEM1_MAILBOX_OM_UP_BASE_ADDR)
#define LPHY_MBX_OM_DOWN_BASE_ADDR(m)                    ( 0 == m )?(LPHY_MODEM0_MAILBOX_OM_DOWN_BASE_ADDR)          : (LPHY_MODEM1_MAILBOX_OM_DOWN_BASE_ADDR)

/*ר�������ַ����*/
#define LPHY_MBX_LMAC_UP_CG0_BASE_ADDR(m)                ( 0 == m )?(LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_BASE_ADDR)      : (LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_BASE_ADDR)
#define LPHY_MBX_LMAC_DOWN_CG0_BASE_ADDR(m)              ( 0 == m )?(LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR)    : (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_BASE_ADDR)
#define LPHY_MBX_LMAC_UP_CG1_BASE_ADDR(m)                ( 0 == m )?(LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_BASE_ADDR)      : (LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_BASE_ADDR)
#define LPHY_MBX_LMAC_DOWN_CG1_BASE_ADDR(m)              ( 0 == m )?(LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR)    : (LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG1_BASE_ADDR)
#define LPHY_MBX_DL_TB_QUEUE_BASE_ADDR(m)                ( 0 == m )?(LPHY_MODEM0_MAILBOX_DL_TB_QUEUE_BASE_ADDR)      : (LPHY_MODEM1_MAILBOX_DL_TB_QUEUE_BASE_ADDR)
#define LPHY_MBX_LPP_DOWN_BASE_ADDR(m)                   ( 0 == m )?(LPHY_MODEM0_MAILBOX_LPP_DOWN_BASE_ADDR)         : (LPHY_MODEM1_MAILBOX_LPP_DOWN_BASE_ADDR)

/*�����ַ����*/
#define LPHY_MBX_VERSION_ADDR(m)                         ( 0 == m )?(LPHY_MODEM0_MAILBOX_VERSION_ADDR)                : (LPHY_MODEM1_MAILBOX_VERSION_ADDR)
#define LPHY_MBX_LOAD_MEMORY_ADDR(m)                     ( 0 == m )?(LPHY_MODEM0_MAILBOX_LOAD_MEMORY_ADDR)            : (LPHY_MODEM1_MAILBOX_LOAD_MEMORY_ADDR)
#define LPHY_MBX_LPHY_WORKING_MODE_ADDR(m)               ( 0 == m )?(LPHY_MODEM0_MAILBOX_LPHY_WORKING_MODE_ADDR)      : (LPHY_MODEM1_MAILBOX_LPHY_WORKING_MODE_ADDR)
#define LPHY_MBX_LPHY_CTU_REG_BASE_ADDR(m)               ( 0 == m )?(LPHY_MODEM0_MAILBOX_LPHY_CTU_REG_BASE_ADDR)      : (LPHY_MODEM1_MAILBOX_LPHY_CTU_REG_BASE_ADDR)
#define LPHY_MBX_LPHY_DSP_NV_PARA_SIZE_ADDR(m)           ( 0 == m )?(LPHY_MODEM0_MAILBOX_LPHY_DSP_NV_PARA_SIZE_ADDR)  : (LPHY_MODEM1_MAILBOX_LPHY_DSP_NV_PARA_SIZE_ADDR)
#define LPHY_MBX_LPHY_ARM_NV_PARA_SIZE_ADDR(m)           ( 0 == m )?(LPHY_MODEM0_MAILBOX_LPHY_ARM_NV_PARA_SIZE_ADDR)  : (LPHY_MODEM1_MAILBOX_LPHY_ARM_NV_PARA_SIZE_ADDR)
#define LPHY_MBX_MULTIMODE_STARTUP_MODE_ADDR(m)          ( 0 == m )?(LPHY_MODEM0_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR) : (LPHY_MODEM1_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR)
#define LPHY_MBX_IMAG_SIZE_INFO_ADDR(m)                  ( 0 == m )?(LPHY_MODEM0_MAILBOX_IMAG_SIZE_INFO_ADDR)         : (LPHY_MODEM1_MAILBOX_IMAG_SIZE_INFO_ADDR)
#define LPHY_MBX_OUTER_MEM_INFO_ADDR(m)                  ( 0 == m )?(LPHY_MODEM0_MAILBOX_OUTER_MEM_INFO_ADDR)         : (LPHY_MODEM1_MAILBOX_OUTER_MEM_INFO_ADDR)
#define LPHY_MBX_TOA_BASE_ADDR(m)                        ( 0 == m )?(LPHY_MODEM0_MAILBOX_TOA_BASE_ADDR)               : (LPHY_MODEM1_MAILBOX_TOA_BASE_ADDR)

/*NV����ַ����*/
#define LPHY_MBX_LPHY_NV_CFG_BASE_ADDR(m)                ( 0 == m )?(LPHY_MODEM0_MAILBOX_LPHY_NV_CFG_BASE_ADDR) : (LPHY_MODEM1_MAILBOX_LPHY_NV_CFG_BASE_ADDR)
#define LPHY_MBX_TPHY_NV_CFG_BASE_ADDR(m)                ( 0 == m )?(LPHY_MODEM0_MAILBOX_TPHY_NV_CFG_BASE_ADDR) : (LPHY_MODEM1_MAILBOX_TPHY_NV_CFG_BASE_ADDR)
#define LPHY_MBX_COMM_NV_CFG_BASE_ADDR(m)                ( 0 == m )?(LPHY_MODEM0_MAILBOX_COMM_NV_CFG_BASE_ADDR) : (LPHY_MODEM1_MAILBOX_COMM_NV_CFG_BASE_ADDR)
#define LPHY_MBX_BAND_NV_IDX_BASE_ADDR(m)                ( 0 == m )?(LPHY_MODEM0_MAILBOX_BAND_NV_IDX_BASE_ADDR) : (LPHY_MODEM1_MAILBOX_BAND_NV_IDX_BASE_ADDR)


/*LTEר�����䶨�����*/
/*****************************************************************************
 ר���������PHY �ϱ���MAC�ĸ�����Ϣ��ַ���ܴ�СԤ��Ϊ512Byte
 ˵��:Ϊ��ʹ�÷��㣬�Լ��������ܣ�һЩFLAG_ADDR��ʱ������ʹ��
        ֱ�Ӷ���ص�ַ��ȡ��ɣ������ر�˵����FLAG ADDRĬ����ʹ�õġ�
*****************************************************************************/
#define LPHY_DEDIMBX_UL_BASE_ADDR(m)                         (LPHY_MBX_LMAC_UP_CG0_BASE_ADDR(m))
#define LPHY_DEDIMBX_UL_CG_BUFF_OFFSET(m,c)                  ( 0 == m )? ((LPHY_MODEM0_MAILBOX_LMAC_UP_CG0_SIZE + LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE) * c):((LPHY_MODEM1_MAILBOX_LMAC_UP_CG0_SIZE + LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE) * c)

#define LPHY_DEDIMBX_UL_GRANT_HARQ_FLAG_ADDR(m,c)            ((LPHY_DEDIMBX_UL_BASE_ADDR(m))                        + (LPHY_DEDIMBX_UL_CG_BUFF_OFFSET(m,c)))
#define LPHY_DEDIMBX_UL_GRANT_HARQ_ADDR(m,c)                 ((LPHY_DEDIMBX_UL_GRANT_HARQ_FLAG_ADDR(m,c))           + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_DL_ASSIGNMENT_FLAG_ADDR(m,c)            ((LPHY_DEDIMBX_UL_GRANT_HARQ_ADDR(m,c))                + sizeof(LMAC_LPHY_UL_GRANT_HARQ_IND_STRU))
#define LPHY_DEDIMBX_DL_ASSIGNMENT_ADDR(m,c)                 ((LPHY_DEDIMBX_DL_ASSIGNMENT_FLAG_ADDR(m,c))           + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_PUCCH_IND_FLAG_ADDR(m,c)                ((LPHY_DEDIMBX_DL_ASSIGNMENT_ADDR(m,c))                + sizeof(LMAC_LPHY_DL_ASSIGNMENT_IND_STRU))
#define LPHY_DEDIMBX_PUCCH_IND_ADDR(m,c)                     ((LPHY_DEDIMBX_PUCCH_IND_FLAG_ADDR(m,c))               + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_PDCCH_ORDER_IND_FLAG_ADDR(m,c)          ((LPHY_DEDIMBX_PUCCH_IND_ADDR(m,c))                    + sizeof(LMAC_LPHY_PUCCH_IND_STRU)        )
#define LPHY_DEDIMBX_PDCCH_ORDER_IND_ADDR(m,c)               ((LPHY_DEDIMBX_PDCCH_ORDER_IND_FLAG_ADDR(m,c))         + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_PDCCH_ORDER_SUCC_FLAG_ADDR(m,c)         ((LPHY_DEDIMBX_PDCCH_ORDER_IND_ADDR(m,c))              + sizeof(LMAC_LPHY_PDCCH_ORDER_IND_STRU)  )
#define LPHY_DEDIMBX_PDCCH_ORDER_SUCC_ADDR(m,c)              ((LPHY_DEDIMBX_PDCCH_ORDER_SUCC_FLAG_ADDR(m,c))        + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_RAR_TIMEOUT_IND_FLAG_ADDR(m,c)          ((LPHY_DEDIMBX_PDCCH_ORDER_SUCC_ADDR(m,c))             + sizeof(LMAC_LPHY_PDCCH_ORDER_SUCC_IND_STRU))
#define LPHY_DEDIMBX_RAR_TIMEOUT_IND_ADDR(m,c)               ((LPHY_DEDIMBX_RAR_TIMEOUT_IND_FLAG_ADDR(m,c))         + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_ACCESS_CNF_FLAG_ADDR(m,c)               ((LPHY_DEDIMBX_RAR_TIMEOUT_IND_ADDR(m,c))              + sizeof(LMAC_LPHY_RAR_TIMEOUT_IND_STRU)  )
#define LPHY_DEDIMBX_ACCESS_CNF_ADDR(m,c)                    ((LPHY_DEDIMBX_ACCESS_CNF_FLAG_ADDR(m,c))              + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_SR_DATA_CNF_FLAG_ADDR(m,c)              ((LPHY_DEDIMBX_ACCESS_CNF_ADDR(m,c))                   + sizeof(LMAC_LPHY_ACCESS_CNF_STRU)       )
#define LPHY_DEDIMBX_SR_DATA_CNF_ADDR(m,c)                   ((LPHY_DEDIMBX_SR_DATA_CNF_FLAG_ADDR(m,c))             + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_PL_IND_FLAG_ADDR(m,c)                   ((LPHY_DEDIMBX_SR_DATA_CNF_ADDR(m,c))                  + sizeof(LMAC_LPHY_SR_DATA_CNF_STRU)      )
#define LPHY_DEDIMBX_PL_IND_ADDR(m,c)                        ((LPHY_DEDIMBX_PL_IND_FLAG_ADDR(m,c))                  + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_PB_IND_FLAG_ADDR(m,c)                   ((LPHY_DEDIMBX_PL_IND_ADDR(m,c))                       + sizeof(LMAC_LPHY_PL_IND_STRU)           )
#define LPHY_DEDIMBX_PB_IND_ADDR(m,c)                        ((LPHY_DEDIMBX_PB_IND_FLAG_ADDR(m,c))                  + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_PDCCH_RELEASE_ULSPS_IND_FLAG_ADDR(m,c)  ((LPHY_DEDIMBX_PB_IND_ADDR(m,c))                       + sizeof(LMAC_LPHY_PB_IND_STRU)           )
#define LPHY_DEDIMBX_PDCCH_RELEASE_ULSPS_IND_ADDR(m,c)       ((LPHY_DEDIMBX_PDCCH_RELEASE_ULSPS_IND_FLAG_ADDR(m,c)) + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_MSG3_HARQ_IND_FLAG_ADDR(m,c)            ((LPHY_DEDIMBX_PDCCH_RELEASE_ULSPS_IND_ADDR(m,c))      + sizeof(LMAC_LPHY_PDCCH_RELEASE_ULSPS_IND_STRU))
#define LPHY_DEDIMBX_MSG3_HARQ_IND_ADDR(m,c)                 ((LPHY_DEDIMBX_MSG3_HARQ_IND_FLAG_ADDR(m,c))           + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_RAR_CONFIG_CNF_FLAG_ADDR(m,c)           ((LPHY_DEDIMBX_MSG3_HARQ_IND_ADDR(m,c))                + sizeof(LMAC_LPHY_MSG3_HARQ_IND_STRU)    )
#define LPHY_DEDIMBX_RAR_CONFIG_CNF_ADDR(m,c)                ((LPHY_DEDIMBX_RAR_CONFIG_CNF_FLAG_ADDR(m,c))          + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_MAC_PHY_ERR_IND_FLAG_ADDR(m,c)          ((LPHY_DEDIMBX_RAR_CONFIG_CNF_ADDR(m,c))               + sizeof(LMAC_LPHY_RAR_CONFIG_CNF_STRU)   )
#define LPHY_DEDIMBX_MAC_PHY_ERR_IND_ADDR(m,c)               ((LPHY_DEDIMBX_MAC_PHY_ERR_IND_FLAG_ADDR(m,c))         + DEDI_MAILBOX_FLAG                       )
#define LPHY_DEDIMBX_MAC_PDU_INVALID_FLAG(m,c)               ((LPHY_DEDIMBX_MAC_PHY_ERR_IND_ADDR(m,c))              + sizeof(LMAC_LPHY_ERR_IND_STRU)          )
#define LPHY_DEDIMBX_DRX_PAGING_PROC_ADDR(m,c)               ((LPHY_DEDIMBX_MAC_PDU_INVALID_FLAG(m,c))              + sizeof(VOS_UINT32)                      )
#define LPHY_DEDIMBX_UL_END_ADDR(m,c)                        ((LPHY_DEDIMBX_DRX_PAGING_PROC_ADDR(m,c))              + sizeof(LMAC_LPHY_DATA_IND_HEAD_STRU) + DRX_PAGING_LEN)
#define LPHY_DEDIMBX_UL_SIZE(m,c)                            ((LPHY_DEDIMBX_UL_END_ADDR(m,c))                       - (LPHY_DEDIMBX_UL_GRANT_HARQ_FLAG_ADDR(m,c)))

/*****************************************************************************
 ר���������MAC�·���PHY�ĸ�����Ϣ��ַ�ܴ�СԤ��Ϊ256Byte
 ˵��:Ϊ��ʹ�÷��㣬�Լ��������ܣ�һЩFLAG_ADDR��ʱ������ʹ��
        ֱ�Ӷ���ص�ַ��ȡ��ɣ������ر�˵����FLAG ADDRĬ����ʹ�õġ�
*****************************************************************************/
#define LPHY_DEDIMBX_DL_START_ADDR(m)                       (LPHY_MBX_LMAC_DOWN_CG0_BASE_ADDR(m))
#define LPHY_DEDIMBX_DL_CG_BUFF_OFFSET(m,c)                 ( 0 == m )? ((LPHY_MODEM0_MAILBOX_LMAC_DOWN_CG0_SIZE + LPHY_MODEM0_MAILBOX_LMAC_UP_CG1_SIZE) * c):((LPHY_MODEM1_MAILBOX_LMAC_DOWN_CG0_SIZE + LPHY_MODEM1_MAILBOX_LMAC_UP_CG1_SIZE) * c)

#define LPHY_DEDIMBX_ACCESS_REQ_FLAG_ADDR(m,c)              ((LPHY_DEDIMBX_DL_START_ADDR(m))             + (LPHY_DEDIMBX_DL_CG_BUFF_OFFSET(m,c)))
#define LPHY_DEDIMBX_ACCESS_REQ_ADDR(m,c)                   ((LPHY_DEDIMBX_ACCESS_REQ_FLAG_ADDR(m,c))        + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_SR_DATA_REQ_FLAG_ADDR(m,c)             ((LPHY_DEDIMBX_ACCESS_REQ_ADDR(m,c))             + sizeof(LMAC_LPHY_ACCESS_REQ_STRU))
#define LPHY_DEDIMBX_SR_DATA_REQ_ADDR(m,c)                  ((LPHY_DEDIMBX_SR_DATA_REQ_FLAG_ADDR(m,c))       + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_RA_ACK_REQ_FLAG_ADDR(m,c)              ((LPHY_DEDIMBX_SR_DATA_REQ_ADDR(m,c))            + sizeof(LMAC_LPHY_SR_DATA_REQ_STRU))
#define LPHY_DEDIMBX_RA_ACK_REQ_ADDR(m,c)                   ((LPHY_DEDIMBX_RA_ACK_REQ_FLAG_ADDR(m,c))        + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_FORCE_ACTIVE_REQ_FLAG_ADDR(m,c)        ((LPHY_DEDIMBX_RA_ACK_REQ_ADDR(m,c))             + sizeof(LMAC_LPHY_RA_ACK_REQ_STRU))
#define LPHY_DEDIMBX_FORCE_ACTIVE_REQ_ADDR(m,c)             ((LPHY_DEDIMBX_FORCE_ACTIVE_REQ_FLAG_ADDR(m,c))  + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_HO_CQI_SRS_ACTIVE_FLAG_ADDR(m,c)       ((LPHY_DEDIMBX_FORCE_ACTIVE_REQ_ADDR(m,c))       + sizeof(LMAC_LPHY_FORCE_ACTIVE_REQ_STRU))
#define LPHY_DEDIMBX_HO_CQI_SRS_ACTIVE_ADDR(m,c)            ((LPHY_DEDIMBX_HO_CQI_SRS_ACTIVE_FLAG_ADDR(m,c)) + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_RAR_CONFIG_REQ_FLAG_ADDR(m,c)          ((LPHY_DEDIMBX_HO_CQI_SRS_ACTIVE_ADDR(m,c))      + sizeof(LMAC_LPHY_HO_CQI_SRS_ACTIVE_STRU))
#define LPHY_DEDIMBX_RAR_CONFIG_REQ_ADDR(m,c)               ((LPHY_DEDIMBX_RAR_CONFIG_REQ_FLAG_ADDR(m,c))    + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_MAC_PHY_ONLY_RF_SLEEP_FLAG(m,c)        ((LPHY_DEDIMBX_RAR_CONFIG_REQ_ADDR(m,c))         + sizeof(LMAC_LPHY_RAR_CONFIG_REQ_STRU))
#define LPHY_DEDIMBX_PHR_CONFIG_REQ_FLAG_ADDR(m,c)          ((LPHY_DEDIMBX_MAC_PHY_ONLY_RF_SLEEP_FLAG(m,c))  + sizeof(VOS_UINT32))
#define LPHY_DEDIMBX_PHR_CONFIG_REQ_ADDR(m,c)               ((LPHY_DEDIMBX_PHR_CONFIG_REQ_FLAG_ADDR(m,c))    + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_GAP_CONFIG_REQ_FLAG_ADDR(m,c)          ((LPHY_DEDIMBX_PHR_CONFIG_REQ_ADDR(m,c))         + sizeof(LMAC_LPHY_PHR_CONFIG_REQ_STRU))
#define LPHY_DEDIMBX_GAP_CONFIG_REQ_ADDR(m,c)               ((LPHY_DEDIMBX_GAP_CONFIG_REQ_FLAG_ADDR(m,c))    + DEDI_MAILBOX_FLAG)
#define LPHY_DEDIMBX_DL_END_ADDR(m,c)                       ((LPHY_DEDIMBX_GAP_CONFIG_REQ_ADDR(m,c))         + sizeof(LMAC_LPHY_CAP_CONFIG_REQ_STRU))
#define LPHY_DEDIMBX_DL_SIZE(m,c)                           ((LPHY_DEDIMBX_DL_END_ADDR(m,c))                 - (LPHY_DEDIMBX_ACCESS_REQ_FLAG_ADDR(m,c)))
/*****************************************************************************
 ����TB���մ�����У����������У��ܴ�СԤ��Ϊ1kByte
 ˵��:
*****************************************************************************/
#define LPHY_DEDIMBX_LTE_MASTER_DL_TB_MEM_POOL_QUE_ADDR(m)   (LPHY_MBX_DL_TB_QUEUE_BASE_ADDR(m))
#define LPHY_DEDIMBX_LTE_SLAVE_DL_TB_MEM_POOL_QUE_ADDR(m)    ((LPHY_DEDIMBX_LTE_MASTER_DL_TB_MEM_POOL_QUE_ADDR(m)) + LMAC_LPHY_DLTB_LTE_MASTER_QUEUE_SIZE)
#define LPHY_DEDIMBX_DL_TB_DATA_IND_QUE_ADDR(m)              ((LPHY_DEDIMBX_LTE_SLAVE_DL_TB_MEM_POOL_QUE_ADDR(m))  + LMAC_LPHY_DLTB_LTE_SLAVE_QUEUE_SIZE)
#define LPHY_DEDIMBX_DLTB_QUEUE_END_ADDR(m)                  ((LPHY_DEDIMBX_DL_TB_DATA_IND_QUE_ADDR(m))            + LMAC_LPHY_DLTB_DATA_IND_QUEUE_SIZE)
#define LPHY_DEDIMBX_DLTB_QUEUE_SIZE(m)                      ((LPHY_DEDIMBX_DLTB_QUEUE_END_ADDR(m))                - (LPHY_MBX_DL_TB_QUEUE_BASE_ADDR(m)))


//LPHY_FOR_EMU_DBG begin: ����Ϊ��ʱ���ɴ�׮��
//#define LPHY_MAILBOX_BASE_ADDR                           (LPHY_MBX_BASE_ADDR(0))
//#define LPHY_MAILBOX_SHARE_MEM_SIZE                      (LPHY_MBX_SHARE_MEM_SIZE(0))
//#define LPHY_MAILBOX_DEBUG_SEG_SIZE                      (LPHY_MBX_DEBUG_SEG_SIZE(0))
//#define LPHY_MAILBOX_HDLC_BUF_SIZE                       (LPHY_MBX_HDLC_HIGHPRO_BUF_SIZE(0))
#define LPHY_MAILBOX_LHPA_UP_SIZE                        (LPHY_MBX_LHPA_UP_SIZE(0))
#define LPHY_MAILBOX_LHPA_DOWN_SIZE                      (LPHY_MBX_LHPA_DOWN_SIZE(0))
//#define LPHY_MAILBOX_OM_UP_SIZE                          (LPHY_MBX_OM_UP_SIZE(0))
//#define LPHY_MAILBOX_OM_DOWN_SIZE                        (LPHY_MBX_OM_DOWN_SIZE(0))
//#define LPHY_MAILBOX_LMAC_UP_SIZE                        (LPHY_MBX_LMAC_UP_CG0_SIZE(0))
//#define LPHY_MAILBOX_LMAC_DOWN_SIZE                      (LPHY_MBX_LMAC_DOWN_CG0_SIZE(0))
//#define LPHY_MAILBOX_DLTB_QUEUE_SIZE                     (LPHY_MBX_DLTB_QUEUE_SIZE(0))
//#define LPHY_MAILBOX_LPP_DOWN_SIZE                       (LPHY_MBX_LPP_DOWN_SIZE(0))
//#define LPHY_MAILBOX_TOA_SIZE                            (LPHY_MBX_TOA_SIZE(0))

//#define LTPHY_NV_COMM_CFG_SIZE                           (LPHY_MBX_LTPHY_NV_COMM_CFG_SIZE(0))
//#define LPHY_NV_CFG_SIZE                                 (LPHY_MBX_LPHY_NV_CFG_SIZE(0))
//#define TPHY_NV_CFG_SIZE                                 (LPHY_MBX_TPHY_NV_CFG_SIZE(0))
//#define BAND_NV_DDR_IDX_SIZE                             (LPHY_MBX_BAND_NV_DDR_IDX_SIZE(0))

//#define LPHY_MAILBOX_VERSION_ADDR                        (LPHY_MBX_VERSION_ADDR(0))
//#define LPHY_MAILBOX_LOAD_MEMORY_ADDR                    (LPHY_MBX_LOAD_MEMORY_ADDR(0))
//#define LPHY_MAILBOX_LPHY_WORKING_MODE_ADDR              (LPHY_MBX_LPHY_WORKING_MODE_ADDR(0))
//#define LPHY_MAILBOX_LPHY_CTU_REG_BASE_ADDR              (LPHY_MBX_LPHY_CTU_REG_BASE_ADDR(0))
#define LPHY_MAILBOX_LPHY_DSP_NV_PARA_SIZE_ADDR          (LPHY_MBX_LPHY_DSP_NV_PARA_SIZE_ADDR(0))
#define LPHY_MAILBOX_LPHY_ARM_NV_PARA_SIZE_ADDR          (LPHY_MBX_LPHY_ARM_NV_PARA_SIZE_ADDR(0))
//#define LPHY_MAILBOX_MULTIMODE_STARTUP_MODE_ADDR         (LPHY_MBX_MULTIMODE_STARTUP_MODE_ADDR(0))
//#define LPHY_MAILBOX_IMAG_SIZE_INFO_ADDR                 (LPHY_MBX_IMAG_SIZE_INFO_ADDR(0))
//#define LPHY_MAILBOX_OUTER_MEM_INFO_ADDR                 (LPHY_MBX_OUTER_MEM_INFO_ADDR(0))

//#define LPHY_MAILBOX_HDLC_BUF_BASE_ADDR                  (LPHY_MBX_HDLC_HIGHPRO_BUF_BASE_ADDR(0))
//#define LPHY_MAILBOX_LHPA_UP_BASE_ADDR                   (LPHY_MBX_LHPA_UP_BASE_ADDR(0))
//#define LPHY_MAILBOX_LHPA_DOWN_BASE_ADDR                 (LPHY_MBX_LHPA_DOWN_BASE_ADDR(0))
//#define LPHY_MAILBOX_OM_UP_BASE_ADDR                     (LPHY_MBX_OM_UP_BASE_ADDR(0))
//#define LPHY_MAILBOX_OM_DOWN_BASE_ADDR                   (LPHY_MBX_OM_DOWN_BASE_ADDR(0))
#define LPHY_MAILBOX_LMAC_UP_BASE_ADDR                   (LPHY_MBX_LMAC_UP_CG0_BASE_ADDR(0))
#define LPHY_MAILBOX_LMAC_DOWN_BASE_ADDR                 (LPHY_MBX_LMAC_DOWN_CG0_BASE_ADDR(0))
#define LPHY_MAILBOX_DL_TB_QUEUE_BASE_ADDR               (LPHY_MBX_DL_TB_QUEUE_BASE_ADDR(0))
#define LPHY_MAILBOX_LPP_DOWN_BASE_ADDR                  (LPHY_MBX_LPP_DOWN_BASE_ADDR(0))
#define LPHY_MAILBOX_TOA_BASE_ADDR                       (LPHY_MBX_TOA_BASE_ADDR(0))

#define LPHY_MAILBOX_NV_CFG_BASE_ADDR                    (LPHY_MBX_LPHY_NV_CFG_BASE_ADDR(0))
#define TPHY_MAILBOX_NV_CFG_BASE_ADDR                    (LPHY_MBX_TPHY_NV_CFG_BASE_ADDR(0))
#define COMM_MAILBOX_NV_CFG_BASE_ADDR                    (LPHY_MBX_COMM_NV_CFG_BASE_ADDR(0))
#define BAND_MAILBOX_NV_IDX_BASE_ADDR                    (LPHY_MBX_BAND_NV_IDX_BASE_ADDR(0))

//LPHY_FOR_EMU_DBG end.

#define LPHY_MAILBOX_TDS_BANKA_MEM_SIZE                  (0x00008000UL)  /*32K�ֽ�, ����  pliot�汾lcs����ʹ��*/ //LPHY_FOR_EMU_DBG
//#define LPHY_MAILBOX_CSU_BANKA_MEM_SIZE                (0x00000000UL)
//#define LPHY_MAILBOX_LCS_BANKA_MEM_SIZE                (0x00000000UL)

//#define LPHY_MAILBOX_RSV_BANKA_MEM_SIZE                (0x00007800UL)  /*31K rsvd segment 1*/
#define LPHY_MAILBOX_TDS_BANKA_BASE_ADDR                 ((LPHY_MBX_BAND_NV_IDX_BASE_ADDR(0))      + (LPHY_MBX_BAND_NV_DDR_IDX_SIZE(0))) //LPHY_FOR_EMU_DBG
//#define LPHY_MAILBOX_CSU_BANKA_BASE_ADDR               (LPHY_MAILBOX_TDS_BANKA_BASE_ADDR   + LPHY_MAILBOX_TDS_BANKA_MEM_SIZE)
//#define LPHY_MAILBOX_LCS_BANKA_BASE_ADDR               (LPHY_MAILBOX_CSU_BANKA_BASE_ADDR   + LPHY_MAILBOX_CSU_BANKA_MEM_SIZE)
//#define LPHY_MAILBOX_RSV_BANKA_BASE_ADDR               (LPHY_MAILBOX_LCS_BANKA_BASE_ADDR   + LPHY_MAILBOX_LCS_BANKA_MEM_SIZE)
//#define LPHY_MAILBOX_LPHY_NV_BASE_ADDR                 (LPHY_MAILBOX_TOA_BASE_ADDR)  //TBD_Onetrack

/*----------------------------------------------------------------------------------
 * redefine the marco to keep the compatible with old code.
-------------------------------------------------------------------------------------*/
#define TPHY_RSVD_32K_BUFFER_BASE_ADDR                  (LPHY_MAILBOX_TDS_BANKA_BASE_ADDR)
#define LTPHY_MAILBOX_NV_COMM_CFG_BASE_ADDR             (LPHY_MBX_COMM_NV_CFG_BASE_ADDR(0))
#define TLPHY_DDR_ADDR_INFO                             (LPHY_MBX_OUTER_MEM_INFO_ADDR(0))

#define LPHY_MAILBOX_DDR_NV_ADDR                        (LPHY_MBX_BAND_NV_IDX_BASE_ADDR(0))

#define LPHY_MAILBOX_SLAVE_AWAKE_ADDR                   (0)  //Reseved for CPHY
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* __MEM_MAP_BASE_H__ */

