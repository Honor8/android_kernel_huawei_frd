

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "NasEmmTauSerInclude.h"
#include "NasEmmAttDetInclude.h"
#include "LNasStkInterface.h"

/*lint -e767*/
#define    THIS_FILE_ID            PS_FILE_ID_NASEMMTAUREJ_C
#define    THIS_NAS_FILE_ID        NAS_FILE_ID_NASEMMTAUREJ_C
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


NAS_LMM_TAU_REJ_CAUSE_PROC_STRU  gstEmmTauRejCauseProcTbl[]=
{
    {NAS_LMM_CAUSE_ILLEGAL_UE,                              NAS_EMM_TAU_TAUReqRejectCasue367},
    {NAS_LMM_CAUSE_ILLEGAL_ME,                              NAS_EMM_TAU_TAUReqRejectCasue367},
    {NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW,                      NAS_EMM_TAU_TAUReqRejectCasue367},
    {NAS_LMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,     NAS_EMM_TAU_TAUReqRejectCasue8},
    {NAS_LMM_CAUSE_UE_ID_NOT_DERIVED,                       NAS_EMM_TAU_TAUReqRejectCasue9},
    {NAS_LMM_CAUSE_IMPLICIT_DETACHED,                       NAS_EMM_TAU_TAUReqRejectCasue10},
    {NAS_LMM_CAUSE_PLMN_NOT_ALLOW,                          NAS_EMM_TAU_TAUReqRejectCasue11},
    {NAS_LMM_CAUSE_TA_NOT_ALLOW,                            NAS_EMM_TAU_TAUReqRejectCasue12},
    {NAS_LMM_CAUSE_ROAM_NOT_ALLOW,                          NAS_EMM_TAU_TAUReqRejectCasue13},
    {NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,              NAS_EMM_TAU_TAUReqRejectCasue14},
    {NAS_LMM_CAUSE_NO_SUITABL_CELL,                         NAS_EMM_TAU_TAUReqRejectCasue15},
    {NAS_LMM_CAUSE_PROCEDURE_CONGESTION,                    NAS_EMM_TAU_TAUReqRejectCasue22},
#if(FEATURE_ON == FEATURE_CSG)
    {NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG,             NAS_EMM_TAU_TAUReqRejectCasue25},
#endif
    {NAS_LMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,NAS_EMM_TAU_TAUReqRejectCasue35},
    {NAS_LMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED,         NAS_EMM_TAU_TAUReqRejectCasue40},
    {NAS_LMM_CAUSE_SERVER_NETWORK_FAILURE,                  NAS_EMM_TAU_TAUReqRejectCasue42}
};
VOS_UINT32        gstEmmTauRejCauseProcTblLen = sizeof(gstEmmTauRejCauseProcTbl)
                                                / sizeof(NAS_LMM_TAU_REJ_CAUSE_PROC_STRU);


/*****************************************************************************
结构名称    :NAS_LMM_TAU_COLLISION_SER_RCV_CN_REJ_PROC_STRU
使用说明    :TAU打断SER冲突时收到网侧REJ不同原因值处理表
*****************************************************************************/
NAS_LMM_TAU_COLLISION_SER_RCV_CN_REJ_PROC_STRU  gstEmmTauCollisionSerRcvCnRejProcTbl[]=
{
    {NAS_LMM_CAUSE_ILLEGAL_UE,                                        NAS_EMM_TAU_TAUCollisionSerRejCasue3},
    {NAS_LMM_CAUSE_ILLEGAL_ME,                                        NAS_EMM_TAU_TAUCollisionSerRejCasue3},
    {NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW,                                NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,               NAS_EMM_TAU_TAUCollisionSerRejCasue3},
    {NAS_LMM_CAUSE_UE_ID_NOT_DERIVED,                                 NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_IMPLICIT_DETACHED,                                 NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_PLMN_NOT_ALLOW,                                    NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_TA_NOT_ALLOW,                                      NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_ROAM_NOT_ALLOW,                                    NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_NO_SUITABL_CELL,                                   NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_PROCEDURE_CONGESTION,                              NAS_EMM_TAU_TAUCollisionSerRejCasue7},
#if(FEATURE_ON == FEATURE_CSG)
    {NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG,                       NAS_EMM_TAU_TAUCollisionSerRejCasue7},
#endif
    {NAS_LMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,       NAS_EMM_NULL_PTR                    },
    {NAS_LMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED,                   NAS_EMM_TAU_TAUCollisionSerRejCasue7},
    {NAS_LMM_CAUSE_SERVER_NETWORK_FAILURE,                            NAS_EMM_TAU_TAUCollisionSerRejCasue7}
};

VOS_UINT32 gstEmmTauCollisionSerRcvCnRejProcTblLen = sizeof(gstEmmTauCollisionSerRcvCnRejProcTbl)
                                        / sizeof(NAS_LMM_TAU_COLLISION_SER_RCV_CN_REJ_PROC_STRU);

/*****************************************************************************
结构名称    :NAS_LMM_TAU_COLLISION_DETACH_RCV_CN_REJ_PROC_STRU
使用说明    :TAU打断DETACH冲突时收到网侧REJ不同原因值处理表
*****************************************************************************/
NAS_LMM_TAU_COLLISION_DETACH_RCV_CN_REJ_PROC_STRU  gstEmmTauCollisionDetachRcvCnRejProcTbl[]=
{
    {NAS_LMM_CAUSE_ILLEGAL_UE,                                        NAS_EMM_TAU_TAUCollisionDetachRejCasue3 },
    {NAS_LMM_CAUSE_ILLEGAL_ME,                                        NAS_EMM_TAU_TAUCollisionDetachRejCasue3 },
    {NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW,                                NAS_EMM_TAU_TAUCollisionDetachRejCasue3 },
    {NAS_LMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,               NAS_EMM_TAU_TAUCollisionDetachRejCasue3 },
    {NAS_LMM_CAUSE_UE_ID_NOT_DERIVED,                                 NAS_EMM_TAU_TAUCollisionDetachRejCasue9 },
    {NAS_LMM_CAUSE_IMPLICIT_DETACHED,                                 NAS_EMM_TAU_TAUCollisionDetachRejCasue9 },
    {NAS_LMM_CAUSE_PLMN_NOT_ALLOW,                                    NAS_EMM_TAU_TAUCollisionDetachRejCasue3 },
    {NAS_LMM_CAUSE_TA_NOT_ALLOW,                                      NAS_EMM_TAU_TAUCollisionDetachRejCasue3 },
    {NAS_LMM_CAUSE_ROAM_NOT_ALLOW,                                    NAS_EMM_TAU_TAUCollisionDetachRejCasue13},
    {NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_EMM_TAU_TAUCollisionDetachRejCasue3 },
    {NAS_LMM_CAUSE_NO_SUITABL_CELL,                                   NAS_EMM_TAU_TAUCollisionDetachRejCasue13},
    {NAS_LMM_CAUSE_PROCEDURE_CONGESTION,                              NAS_EMM_TAU_TAUCollisionDetachRejCasue22},
#if(FEATURE_ON == FEATURE_CSG)
    {NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG,                       NAS_EMM_TAU_TAUCollisionDetachRejCasue25},
#endif
    {NAS_LMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,       NAS_EMM_NULL_PTR                        },
    {NAS_LMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED,                   NAS_EMM_TAU_TAUCollisionDetachRejCasue9 },
    {NAS_LMM_CAUSE_SERVER_NETWORK_FAILURE,                            NAS_EMM_TAU_TAUCollisionDetachRejCasue3 }
};

VOS_UINT32 gstEmmTauCollisionDetachRcvCnRejProcTblLen = sizeof(gstEmmTauCollisionDetachRcvCnRejProcTbl)
                                        / sizeof(NAS_LMM_TAU_COLLISION_DETACH_RCV_CN_REJ_PROC_STRU);


VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue367
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_TAUReqRejectCasue367 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue3678_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置EU3 ROAMING NOT ALLOWED，并设置到NVIM中*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_DELETE_RPLMN);

    NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);

    /*USIM无效，直到关机或拔卡*/
    /*暂不处理*/

    /*删除equivalent PLMNs由MMC模块完成*/
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NO_IMSI,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    /*向LRRC发送LRRC_LMM_NAS_INFO_CHANGE_REQ携带USIM卡状态*/
    NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);

    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;
}



VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue8
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    MMC_LMM_TAU_TYPE_ENUM_UINT32        enEmmTauType;

    enEmmTauType = NAS_EMM_TAU_GetTAUtype();

    /* 联合TAU按卡无效处理 */
    if ((NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING == enEmmTauType)
      ||(NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI == enEmmTauType))
    {
        NAS_EMM_TAU_TAUReqRejectCasue367(ucTAUrejcause, pstTAURej);
        return;
    }

    /* 非联合TAU，R11按卡无效处理，R10以下按other处理 */
    if (NAS_RELEASE_R11)
    {
        NAS_EMM_TAU_TAUReqRejectCasue367(ucTAUrejcause, pstTAURej);
    }
    else
    {
        NAS_EMM_TAU_TAUReqRejectOtherCause(NAS_LMM_CAUSE_NULL, pstTAURej);
    }

    return;

}


VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue9
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    VOS_UINT32                          ulAttachFlag    = VOS_TRUE;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg       = NAS_EMM_NULL_PTR;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("Nas_Emm_Tau_TAUReqRejectCasue9 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue9_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置设置EU2 NOT UPDATED*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_NOT_UPDATED_EU2);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*删除equivalent PLMNs由MMC模块完成*/
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*转换EMM状态机MS_DEREG+SS_DEREG_NORMAL_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* 如果是打断CSFB流程，由于后续要搜网去GU，暂停ATTACH流程 */
    if ((NAS_EMM_COLLISION_SERVICE == NAS_EMM_TAU_GetEmmCollisionCtrl())
      &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure()))
    {
        ulAttachFlag = VOS_FALSE;
    }

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_RrcRelCause_TAUREJ9: CAUSE_ESM_EMC_PDN_REQ");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue9_ENUM,LNAS_FUNCTION_LABEL4);

        /* 紧急PDN发起的tau serivce被#9、#10、#40拒绝主动释放，收到系统消息发起紧急attach */
        NAS_EMM_EmcPndReqSerTAURejProc();
    }
    /*lint -e960*/
    else if(VOS_TRUE == ulAttachFlag)
    /*lint +e960*/
    {
        if (NAS_RELEASE_R11)
        {
            NAS_EMM_WaitNetworkRelInd();
            return;
        }
        else
        {
            /* 延时发送attach请求, 此处无需再压栈， 直接启动延时定时器，
               待延时定时器超时后, 直接发起ATTACH流程
            */
            NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);
        }
    }
    else
    {
        /*lint -e960*/
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
        /*lint +e960*/
    }

    /* 清空TAU流程所有资源*/
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;

}


/*lint -e960*/
VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue10
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    VOS_UINT32                          ulAttachFlag    = VOS_TRUE;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg       = NAS_EMM_NULL_PTR;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_TAUReqRejectCasue10 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue10_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /* 状态迁移Dereg.Normal_Service，发送INTRA_ATTACH_REQ消息*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* 进入DEREG态不再写统一写安全上下文，参照标杆，在TAU被#10拒绝，将安全上下文写卡 */
    NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_UPDATE);

    /* 如果是打断CSFB流程，由于后续要搜网去GU，暂停ATTACH流程 */
    if ((NAS_EMM_COLLISION_SERVICE == NAS_EMM_TAU_GetEmmCollisionCtrl())
      &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure()))
    {
        ulAttachFlag = VOS_FALSE;
    }

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_RrcRelCause_TAUREJ9: CAUSE_ESM_EMC_PDN_REQ");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue10_ENUM,LNAS_FUNCTION_LABEL3);

        /* 紧急PDN发起的tau serivce被#9、#10、#40拒绝主动释放，收到系统消息发起紧急attach */
        NAS_EMM_EmcPndReqSerTAURejProc();
    }
    else if(VOS_TRUE == ulAttachFlag)
    {
        if (NAS_RELEASE_R11)
        {
            NAS_EMM_WaitNetworkRelInd();
            return;
        }
        else
        {
            /* 延时发送attach请求, 此处无需再压栈， 直接启动延时定时器，
               待延时定时器超时后, 直接发起ATTACH流程
            */
            NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);

        }
    }
    else
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    /* 清空TAU和APP资源*/
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;
}
/*lint +e960*/

/*lint -e960*/
VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue11
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_TAUReqRejectCasue11 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue11_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置EU3 ROAMING NOT ALLOWED，并设置到NVIM中*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*删除equivalent PLMNs由MMC模块完成*/

    /*Reset the TAU attempt counter*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*为上报服务状态,转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    /*在收到MMC搜网指令后将转入DEREG_PLMN_SEARCH态*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    NAS_EMM_WaitNetworkRelInd();

    return;

}
/*lint +e960*/


/*lint -e960*/
VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue12
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    NAS_MM_TA_STRU                      stTa;

    (VOS_VOID)pstTAURej;

    /* 获取当前TA */
    NAS_EMM_GetCurrentTa(&stTa);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_TAUReqRejectCasue12 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue12_ENUM,LNAS_ENTRY);

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置EU3 ROAMING NOT ALLOWED，并设置到NVIM中*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    if (NAS_RELEASE_R11)
    {
        /* 如果是共享网络，并且TAU是在连接态发起的，不需要将被禁消息添加到被禁列表中 */
        if (NAS_EMM_YES == NAS_EMM_IsNeedAddForbTa())
        {
            /*store the current TAI in the list of
                  "forbidden tracking areas for regional provision of service"*/
            NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRposAddr());
        }
    }
    else
    {
        NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRposAddr());
    }

    /* 注册/服务状态上报上移MMC，删除注册/服务状态相关代码 */
    /*重置TAU计数器*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    NAS_EMM_WaitNetworkRelInd();
    return;

}
/*lint +e960*/

/*lint -e960*/
VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue13
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    NAS_MM_TA_STRU                      stCurrentTa;
    NAS_MM_TA_LIST_STRU                *pstTaiList;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("Nas_Emm_Tau_TAUReqRejectCasue13 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue13_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

    /*设置EU3 ROAMING NOT ALLOWED，并设置到NVIM中*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);
    if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*保存PS LOC信息*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
    /*重置TAU计数器*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

    /*Remove current TA from TAI List*/
    NAS_EMM_GetCurrentTa(&stCurrentTa);

    pstTaiList = NAS_LMM_GetEmmInfoNetInfoTaiListAddr();
    NAS_LMM_DeleteTaFromTaList(&stCurrentTa, pstTaiList, NAS_MM_MAX_TA_NUM);
#if(FEATURE_ON == FEATURE_CSG)
    if (NAS_MM_CELL_TYPE_CSG != NAS_EMM_GetCurrentCellType())
#endif
    {
        /* 需要把当前TA从临时列表中剔除 */
        NAS_LMM_UpdateGradualForbListWithTauRejCause13(&stCurrentTa);
        if (NAS_RELEASE_R11)
        {
            /* 如果是共享网络，不需要将被禁消息添加到被禁列表中 */
            if (NAS_EMM_YES == NAS_EMM_IsNeedAddForbTa())
            {
                /*store the current TAI in the list of
                            "forbidden tracking areas for roaming"*/
                NAS_EMM_AddForbTa(&stCurrentTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());
                NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);
            }
        }
        else
        {
            NAS_EMM_AddForbTa(&stCurrentTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());
            NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);
        }

    }
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUReqRejectCasue13:CAUSE_ESM_EMC_PDN_REQ");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue13_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqTauAbnormalCommProc(   NAS_EMM_MmcSendTauActionResultIndRej,
                                                (VOS_VOID*)NAS_EMM_NULL_PTR,
                                                EMM_SS_DEREG_LIMITED_SERVICE);
    }
    else
    {
        /*发送EMM_ESM_STATUS_IND(去注册)*/
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_ATTACHED);

        /*为上报服务状态,转换EMM状态机MS_DEREG+SS_REG_LIMITED_SERVICE*/
        /*在收到MMC搜网指令后将转入REG_PLMN_SEARCH态*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_LIMITED_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);

        /****检查冲突标志并做相应的处理********************************************/
        if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
        {
            /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
            NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
        }

    }

    NAS_EMM_WaitNetworkRelInd();

    return;

}
/*lint +e960*/

/*lint -e960*/
VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue14
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_TAUReqRejectCasue14 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue14_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置EU3 ROAMING NOT ALLOWED，并设置到NVIM中*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*重置TAU计数器*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*为上报服务状态,转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    /*在收到MMC搜网指令后将转入DEREG_PLMN_SEARCH态*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    NAS_EMM_WaitNetworkRelInd();

    return;

}
/*lint +e960*/

/*lint -e960*/
VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue15
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    NAS_MM_TA_STRU                      stCurrentTa;
    NAS_MM_TA_LIST_STRU                *pstTaiList;
    MMC_LMM_LTE_NO_SUBSCRIBE_PLMN_LIST_STRU  stLteNoSubsPlmnList;

    VOS_UINT32                          ulIsNeedExecLteNotAllow = NAS_EMM_YES;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_TAUReqRejectCasue15 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue15_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);
    /*设置EU3 ROAMING NOT ALLOWED*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);
    if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*保存PS LOC信息*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
    /*重置TAU计数器*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

    /*被15拒绝，当前PLMN只有一个*/
    stLteNoSubsPlmnList.ulPlmnNum = 1;
    /*拷贝当前PLMN*/
    NAS_LMM_MEM_CPY_S(&(stLteNoSubsPlmnList.astPlmnId[0]),
                     sizeof(MMC_LMM_PLMN_ID_STRU),
                     NAS_LMM_GetEmmInfoPresentPlmnAddr(),
                     sizeof(MMC_LMM_PLMN_ID_STRU));

    if((NAS_EMM_BIT_SLCT == NAS_EMM_TAU_GetEmmTauCnEmmExtRejCauseOp())
       &&(NAS_EMM_EUTRAN_NOT_ALLOWED == NAS_EMM_TAU_GetEmmTauCnEmmExtRejCause())
       &&(PS_FALSE == NAS_EMM_GetLteNoSubscribeNvConfigDiscardExtCauseFlag()))
    {
        /*被15原因值拒绝而且当前有扩展原因值，指示USIM未在LTE开户，走正常的LTE未开户流程*/
        /*NAS_EMM_SetLteNoSubscribeExtCauseFlag(NAS_EMM_EUTRAN_NOT_ALLOWED);*/
        NAS_LMM_LteNoSubscribeWithExtCauseHandle(&stLteNoSubsPlmnList);

        /* Forb TA列表维护 */
        NAS_LMM_AddTaInForbListWithTauRejCauseVal15(NAS_EMM_YES, &ulIsNeedExecLteNotAllow);
    }
    else
    {
#if(FEATURE_ON == FEATURE_CSG)
        if (NAS_MM_CELL_TYPE_CSG != NAS_EMM_GetCurrentCellType())
#endif
        {
            /* Forb TA列表维护, 并判断是否要走4G未开户流程 */
            NAS_LMM_AddTaInForbListWithTauRejCauseVal15(NAS_EMM_NO, &ulIsNeedExecLteNotAllow);

            /* 判断是否要走4G未开户流程 */
            if (NAS_EMM_YES == ulIsNeedExecLteNotAllow)
            {
                /*被15原因值拒绝但是没有带扩展原因值，走LTE未开户定制流程*/
                NAS_LMM_LteNoSubscribeWithOutExtCauseHandle(&stLteNoSubsPlmnList);
            }
        }
    }


    /*Remove current TA from TAI List*/
    NAS_EMM_GetCurrentTa(&stCurrentTa);

    pstTaiList = NAS_LMM_GetEmmInfoNetInfoTaiListAddr();
    NAS_LMM_DeleteTaFromTaList(&stCurrentTa, pstTaiList, NAS_MM_MAX_TA_NUM);

    /* 移到4G未开户判断分支里维护Forb列表 */

    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUReqRejectCasue15:CAUSE_ESM_EMC_PDN_REQ");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue15_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqTauAbnormalCommProc(   NAS_EMM_MmcSendTauActionResultIndRej,
                                                (VOS_VOID*)NAS_EMM_NULL_PTR,
                                                EMM_SS_DEREG_LIMITED_SERVICE);
    }
    else
    {
        /*发送EMM_ESM_STATUS_IND(去注册)*/
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_ATTACHED);

        /*转换EMM状态机MS_REG+SS_REG_LIMITED_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_LIMITED_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);

        /****检查冲突标志并做相应的处理********************************************/
        if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
        {
            /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
            NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
        }

    }

    NAS_EMM_WaitNetworkRelInd();

    return;
}
/*lint +e960*/



VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue22
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_TAUReqRejectCasue22 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue22_ENUM,LNAS_ENTRY);

    if (NAS_EMM_BIT_SLCT == pstTAURej->ucBitOpT3346)
    {

        /*设置EU2*/
        NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

        /*重置TAU计数器*/
        NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

        /* 停止T3346定时器 */
        NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3346);

        /* 启动T3346定时器 */
        NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_T3346);

        /* EMM注册状态没有发生变化，不需要同时ESM注册状态 */
        /*发送EMM_ESM_STATUS_IND(去注册)*/
        //NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_ATTACHED);


        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                    EMM_SS_REG_ATTEMPTING_TO_UPDATE,
                                    TI_NAS_EMM_STATE_NO_TIMER);

        /****检查冲突标志并做相应的处理********************************************/
        if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
        {
            /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
            NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
        }

        /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    }
    else
    {
        NAS_EMM_TAU_TAUReqRejectOtherCause(ucTAUrejcause, pstTAURej);
    }

    return;
}


#if(FEATURE_ON == FEATURE_CSG)

VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue25
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    /* 打印进入该函数 */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_TAUReqRejectCasue25 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue25_ENUM,LNAS_ENTRY);

    if(NAS_EMM_NO == NAS_LMM_IsCsgCampOn())
    {
        NAS_EMM_TAU_TAUReqRejectOtherCause(ucTAUrejcause, pstTAURej);
    }
    else
    {

        (VOS_VOID)pstTAURej;


        /*设置EU3 ROAMING NOT ALLOWED，并设置到NVIM中*/
        NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

        NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

        /* tau类型为联合tau,设置ps域，后续发起tau为联合tau */
        if((NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING == NAS_EMM_TAU_GetTAUtype())
            ||(NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI== NAS_EMM_TAU_GetTAUtype()))
        {
            NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);
        }

         /*删除equivalent PLMNs由MMC模块完成*/
        if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
        {
            NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUReqRejectCasu25:CAUSE_ESM_EMC_PDN_REQ");
            TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue25_ENUM,LNAS_FUNCTION_LABEL1);

            NAS_EMM_EmcPndReqTauAbnormalCommProc(   NAS_EMM_MmcSendTauActionResultIndRej,
                                                    (VOS_VOID*)NAS_EMM_NULL_PTR,
                                                    EMM_SS_DEREG_LIMITED_SERVICE);
        }
        else
        {
            /*发送EMM_ESM_STATUS_IND(去注册)*/
            NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_ATTACHED);

            /*为上报服务状态,转换EMM状态机MS_DEREG+SS_REG_LIMITED_SERVICE*/
            /*在收到MMC搜网指令后将转入REG_PLMN_SEARCH态*/
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_LIMITED_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);

            /****检查冲突标志并做相应的处理********************************************/
            if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
            {
                /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
                NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
            }

        }

        /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }
    return;
}
#endif


VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue35
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8   ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU      *pstTAURej

)

{
    /* 判断是否打开nas r10开关 */
    if (NAS_RELEASE_CTRL)
    {
        NAS_EMM_TAU_TAUReqRejectCasue11(ucTAUrejcause, pstTAURej);
    }
    else
    {
        NAS_EMM_TAU_TAUReqRejectOtherCause(ucTAUrejcause, pstTAURej);
    }
    return;
}

/*lint -e960*/

VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue40
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    VOS_UINT32                          ulAttachFlag    = VOS_TRUE;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg       = NAS_EMM_NULL_PTR;

    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_TAUReqRejectCasue40 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue40_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
    /*转换EMM状态机MS_REG+SS_REG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /* 如果是打断CSFB流程，由于后续要搜网去GU，暂停ATTACH流程 */
    if ((NAS_EMM_COLLISION_SERVICE == NAS_EMM_TAU_GetEmmCollisionCtrl())
      &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure()))
    {
        ulAttachFlag = VOS_FALSE;
    }

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_RrcRelCause_TAUREJ9: CAUSE_ESM_EMC_PDN_REQ");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue40_ENUM,LNAS_FUNCTION_LABEL4);

        /* 紧急PDN发起的tau serivce被#9、#10、#40拒绝主动释放，收到系统消息发起紧急attach */
        NAS_EMM_EmcPndReqSerTAURejProc();
    }
    else if(VOS_TRUE == ulAttachFlag)
    {
        if (NAS_RELEASE_R11)
        {
            NAS_EMM_WaitNetworkRelInd();
            return;
        }
        else
        {
            /* 延时发送attach请求, 此处无需再压栈， 直接启动延时定时器，
               待延时定时器超时后, 直接发起ATTACH流程
            */
            NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);
        }
    }
    else
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    /* 清空TAU流程所有资源*/
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;
}
/*lint +e960*/



VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue42Release11
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("Nas_Emm_Tau_TAUReqRejectCasue42 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectCasue42Release11_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置设置EU2 NOT UPDATED*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*删除equivalent PLMNs由MMC模块完成*/
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*TAU尝试次数设置为最大*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_MAX);

    /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    /* 等收到搜网指示后，再将状态转到DEREG_PLMN_SERCH */
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /****检查冲突标志并做相应的处理********************************************/
    if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
    {
        /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
        NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
    }

    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;

}

VOS_VOID NAS_EMM_TAU_TAUReqRejectCasue42
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8   ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU      *pstTAURej

)
{
    if (NAS_RELEASE_R11)
    {
        NAS_EMM_TAU_TAUReqRejectCasue42Release11(ucTAUrejcause, pstTAURej);
    }
    else
    {
        NAS_EMM_TAU_TAUReqRejectOtherCause(ucTAUrejcause, pstTAURej);
    }
}



/*lint -e960*/
VOS_VOID NAS_EMM_TAU_TAUReqRejectOtherCause
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8   ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU      *pstTAURej
)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_TAUReqRejectCasueOthers is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectOtherCause_ENUM,LNAS_ENTRY);

    (VOS_VOID)pstTAURej;

    if( (NAS_LMM_CAUSE_SEMANTICALLY_INCORRECT_MSG == ucTAUrejcause)||
        (NAS_LMM_CAUSE_INVALID_MANDATORY_INF == ucTAUrejcause)||
        (NAS_LMM_CAUSE_MSG_NONEXIST_NOTIMPLEMENTE == ucTAUrejcause)||
        (NAS_LMM_CAUSE_IE_NONEXIST_NOTIMPLEMENTED == ucTAUrejcause)||
        (NAS_LMM_CAUSE_PROTOCOL_ERROR == ucTAUrejcause)||
        (VOS_TRUE == NAS_LMM_GetRoamingRejectNoRetryFlg(ucTAUrejcause)))
    {
        NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_MAX);
    }
    else
    {
        NAS_EMM_TAU_GetEmmTAUAttemptCnt()++;
    }

    NAS_EMM_TAU_ProcAbnormal();

    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUReqRejectOtherCause:CAUSE_ESM_EMC_PDN_REQ");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUReqRejectOtherCause_ENUM,LNAS_FUNCTION_LABEL1);

        NAS_EMM_EmcPndReqTauAbnormalCommProc(   NAS_EMM_MmcSendTauActionResultIndRej,
                                                (VOS_VOID*)NAS_EMM_NULL_PTR,
                                                EMM_SS_DEREG_NORMAL_SERVICE);
    }
    else
    {
        /****检查冲突标志并做相应的处理********************************************/
        if (NAS_EMM_COLLISION_NONE == NAS_EMM_TAU_RcvTauRejCollisionProc(ucTAUrejcause))
        {
            /* 如果不存在冲突，只上报TAU结果，否则在冲突处理中会上报TAU结果 */
            NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
        }
    }

    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;

}


VOS_VOID    NAS_EMM_TAU_RcvTAURej
(
    const NAS_EMM_CN_CAUSE_ENUM_UINT8   ucTAUrejcause,
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej
)
{
    VOS_UINT32                          ulIndex = 0;
    NAS_LMM_TAU_REJ_CAUSE_PROC_FUN          pfRejCauseProcFun;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("Nas_Emm_Tau_RcvTAURej is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_RcvTAURej_ENUM,LNAS_ENTRY);

    /* 停止定时器3430*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);

    #if (FEATURE_ON == FEATURE_DSDS)
    /*发送end notify消息给RRC，通知RRC释放资源*/
    NAS_EMM_TAU_SendRrcDsdsEndNotify();
    #endif

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    NAS_EMM_SendMtcTAUEndType();
#endif

    for (ulIndex = 0; ulIndex < gstEmmTauRejCauseProcTblLen; ulIndex++)
    {
        if (ucTAUrejcause == gstEmmTauRejCauseProcTbl[ulIndex].enRejCauseVal)
        {
            pfRejCauseProcFun = gstEmmTauRejCauseProcTbl[ulIndex].pfRejCauseProcFun;

            /* 有处理函数 */
            if (VOS_NULL_PTR != pfRejCauseProcFun)
            {
                (pfRejCauseProcFun)(ucTAUrejcause, pstTAURej);
            }
            return;
        }
    }

    /*其它原因值处理*/
    NAS_EMM_TAU_TAUReqRejectOtherCause(ucTAUrejcause, pstTAURej);
    return;
}

VOS_UINT32 NAS_EMM_MsTauInitSsWaitCNCnfMsgTAURej(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru
                               )
{
    VOS_UINT32                          ulRslt      = NAS_EMM_FAIL;
    NAS_EMM_CN_TAU_REJ_STRU            *pstTAURej   = NAS_EMM_NULL_PTR;
    LNAS_STK_UPDATE_TYPE_ENUM_UINT8     uTauType = LNAS_STK_UPDATE_TYPE_BUTT;
    VOS_UINT32                          ulTimerLen = NAS_EMM_NULL;

    (VOS_VOID)ulMsgId;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_EMM_TAU_LOG_NORM( "Nas_Emm_MsTauInitSsWaitCNCnfMsgTAURej is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsTauInitSsWaitCNCnfMsgTAURej_ENUM,LNAS_ENTRY);

    /* 函数输入指针参数检查, 状态匹配检查,若不匹配,退出*/
    ulRslt = NAS_EMM_TAU_CHKFSMStateMsgp(EMM_MS_TAU_INIT,EMM_SS_TAU_WAIT_CN_TAU_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_TAU_LOG_WARN("NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        TLPS_PRINT2LAYER_WARNING(NAS_EMM_MsTauInitSsWaitCNCnfMsgTAURej_ENUM,LNAS_ERROR);
        return ulRslt;
    }
    /* 大数据:在给MMC发送TAU结果前，更新TAU CN REJ次数 */
    NAS_LMM_AddTauCounter(NAS_LMM_OM_PCE_TIMES_TYPE_CN_REJ);
    /*停止T3416，删除RAND,RES*/
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_T3416);
    NAS_EMM_SecuClearRandRes();

    /*调用消息处理函数*/
    pstTAURej = (NAS_EMM_CN_TAU_REJ_STRU*) pMsgStru;
    /*保存TAU拒绝原因*/
    NAS_EMM_TAU_GetEmmTauCnRejCause() =  pstTAURej->ucEMMCause;
    if(NAS_EMM_BIT_SLCT == pstTAURej->ucBitOpExtCause)
    {
        NAS_EMM_TAU_GetEmmTauCnEmmExtRejCauseOp() = NAS_EMM_BIT_SLCT;
        NAS_EMM_TAU_GetEmmTauCnEmmExtRejCause() = pstTAURej->ucEmmExtCause;
    }


    /* 保存T3346定时器时长 */
    if (NAS_EMM_BIT_SLCT == pstTAURej->ucBitOpT3346)
    {
        /* 此次修改仅针对GCF 9.2.3.1.20a用例修改，并未做完善，所以目前仅考虑TAU REJ是完整性保护
        的场景 */
        ulTimerLen = NAS_EMM_TimerValTransfer(pstTAURej->stT3346.ucUnit,
                                               pstTAURej->stT3346.ucTimerVal);
        NAS_LMM_ModifyPtlTimer(TI_NAS_EMM_PTL_T3346,ulTimerLen);
    }

    /* 根据当前场景考虑是否需要修改原因值 */
    NAS_LMM_AdaptRegRejCau(&NAS_EMM_TAU_GetEmmTauCnRejCause());

    #if (FEATURE_PTM == FEATURE_ON)
    NAS_EMM_TAUErrRecord(pMsgStru, EMM_OM_ERRLOG_TYPE_CN_REJ);
    #endif

    switch(NAS_EMM_TAU_GetTAUtype())
    {
        case NAS_EMM_CN_TAU_TYPE_TA_UPDATING:
                uTauType = LNAS_STK_UPDATE_TYPE_TA_UPDATEING;
                break;

        case NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING:
                uTauType = LNAS_STK_UPDATE_TYPE_COMBINED_TALA_UPDATING ;
                break;

        case NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI:
                uTauType = LNAS_STK_UPDATE_TYPE_COMBINED_TALA_UPDATING_WITH_IMSI_ATTACH;
                break;

        case NAS_EMM_CN_TAU_TYPE_PERIODIC_UPDATING:
                uTauType = LNAS_STK_UPDATE_TYPE_PERIODIC_UPDATING;
                break;

        default:
                NAS_EMM_TAU_LOG_ERR("NAS_EMM_SendRejEventToSTK: TAU Type error!");
                TLPS_PRINT2LAYER_ERROR(NAS_EMM_MsTauInitSsWaitCNCnfMsgTAURej_ENUM,LNAS_FUNCTION_LABEL1);
                break;
    }

    /*当TAU rej时，将rej事件发送给STK*/
    NAS_EMM_SendRejEventToSTK(NAS_EMM_TAU_GetEmmTauCnRejCause(), uTauType);

    NAS_EMM_TAU_RcvTAURej(NAS_EMM_TAU_GetEmmTauCnRejCause(), pstTAURej);

    return NAS_LMM_MSG_HANDLED;
}


VOS_VOID NAS_EMM_MsTauInitSsWaitCnTauCnfProcMsgAuthRej(
                                                   VOS_UINT32  ulCause)
{
    MMC_LMM_TAU_RSLT_ENUM_UINT32        enTauRslt = MMC_LMM_TAU_RSLT_AUTH_REJ;

    (VOS_VOID)ulCause;

    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_MsTauInitSsWaitCnTauCnfProcMsgAuthRej is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_MsTauInitSsWaitCnTauCnfProcMsgAuthRej_ENUM,LNAS_ENTRY);

    /*停止定时器*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_STATE_TAU_T3430);

    #if (FEATURE_ON == FEATURE_DSDS)
    /*发送end notify消息给RRC，通知RRC释放资源*/
    NAS_EMM_TAU_SendRrcDsdsEndNotify();
    #endif

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    NAS_EMM_SendMtcTAUEndType();
#endif

    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_T3416);
    NAS_EMM_SecuClearRandRes();

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    /*设置EU3 ROAMING NOT ALLOWED，并设置到NVIM中*/
    NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_ROAMING_NOT_ALLOWED_EU3);

    /*删除GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_DELETE_RPLMN);

    /*USIM无效，直到关机或拔卡*/
    /*暂不处理*/

    /*删除equivalent PLMNs由MMC模块完成*/
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        /* 记录ATTACH触发原因值 */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

#if(FEATURE_ON == FEATURE_CSG)
    if (NAS_MM_CELL_TYPE_CSG == NAS_EMM_GetCurrentCellType())
    {
            /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                    EMM_SS_DEREG_LIMITED_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
#endif
    {
        NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);
        /*转换EMM状态机MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                    EMM_SS_DEREG_NO_IMSI,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    }

    /* 上报TAU结果在冲突处理中执行 */
    /****检查冲突标志并做相应的处理********************************************/
    switch(NAS_EMM_TAU_GetEmmCollisionCtrl())
    {
        case    NAS_EMM_COLLISION_DETACH   :

                NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&enTauRslt);
                NAS_EMM_TAU_CollisionDetachProc();
                break;

        case    NAS_EMM_COLLISION_SERVICE    :

                NAS_EMM_TAU_CollisionServiceProc(NAS_EMM_MmcSendTauActionResultIndOthertype,
                                (VOS_VOID*)&enTauRslt,
                                NAS_EMM_TRANSFER_RAT_NOT_ALLOW);
                break;

        default :

                NAS_EMM_TAU_LOG_INFO( "NAS_EMM_MsTauInitSsWaitCnTauCnfProcMsgAuthRej : NO Emm Collision.");
                TLPS_PRINT2LAYER_INFO(NAS_EMM_MsTauInitSsWaitCnTauCnfProcMsgAuthRej_ENUM,LNAS_FUNCTION_LABEL1);
                NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&enTauRslt);
                break;
    }
#if(FEATURE_ON == FEATURE_CSG)
    if (NAS_MM_CELL_TYPE_CSG != NAS_EMM_GetCurrentCellType())
#endif
    {
        /*向LRRC发送LRRC_LMM_NAS_INFO_CHANGE_REQ携带USIM卡状态*/
        NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);
    }
    /*向MRRC发送NAS_EMM_MRRC_REL_REQ消息*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    return;
}




NAS_EMM_COLLISION_ENUM_UINT8 NAS_EMM_TAU_RcvTauRejCollisionProc
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_RcvTauRejCollisionProc is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_RcvTauRejCollisionProc_ENUM, LNAS_ENTRY);

    /* 如果是DETACH冲突 */
    if (NAS_EMM_COLLISION_DETACH == NAS_EMM_TAU_GetEmmCollisionCtrl())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_RcvTauRejCollisionProc: NAS_EMM_COLLISION_DETACH.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_RcvTauRejCollisionProc_ENUM, LNAS_FUNCTION_LABEL1);

        NAS_EMM_TAU_CollisionDetachRcvCnRejProc(enTauRejcause);
        return NAS_EMM_COLLISION_DETACH;
    }

    /* SERVICE冲突的处理 */
    if (NAS_EMM_COLLISION_SERVICE == NAS_EMM_TAU_GetEmmCollisionCtrl())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_RcvTauRejCollisionProc: NAS_EMM_COLLISION_SERVICE.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_RcvTauRejCollisionProc_ENUM, LNAS_FUNCTION_LABEL2);

        NAS_EMM_TAU_CollisionServiceRcvCnRejProc(enTauRejcause);
        return NAS_EMM_COLLISION_SERVICE;
    }

    /* 无冲突则无需特殊处理 */
    return NAS_EMM_COLLISION_NONE;

}



VOS_VOID  NAS_EMM_TAU_CollisionServiceRcvCnRejProc
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    VOS_UINT32                                     i;
    NAS_LMM_TAU_COLLISION_SER_RCV_CN_REJ_PROC_FUN  pfProcFun;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_CollisionServiceRcvCnRejProc is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_CollisionServiceRcvCnRejProc_ENUM, LNAS_ENTRY);

    for (i = 0; i < gstEmmTauCollisionSerRcvCnRejProcTblLen; i++)
    {
        if (enTauRejcause == gstEmmTauCollisionSerRcvCnRejProcTbl[i].enRejCauseVal)
        {
            pfProcFun = gstEmmTauCollisionSerRcvCnRejProcTbl[i].pfProcFun;

            /* 有处理函数执行 */
            if (VOS_NULL_PTR != pfProcFun)
            {
                (pfProcFun)(enTauRejcause);
            }

            /* 无处理函数无动作直接返回 */
            return;
        }
    }

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_CollisionServiceRcvCnRejProc: other cause.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_CollisionServiceRcvCnRejProc_ENUM, LNAS_FUNCTION_LABEL1);

    /*其它原因值处理*/
    NAS_EMM_TAU_TAUCollisionSerRejCasueOther(enTauRejcause);

    return;
}


VOS_VOID NAS_EMM_TAU_TAUCollisionSerRejCasue3
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    (VOS_VOID)enTauRejcause;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionSerRejCasue3 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionSerRejCasue3_ENUM, LNAS_ENTRY);

    /* ESR流程中发生TAU流程, TAU失败导致CSFB流程失败, 通知LRRC CSFB流程终止,
    LRRC收到该消息后, 若等待网侧释放定时器启动时, 停止定时器 */
    if(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        NAS_EMM_SndLrrcLmmCsfbNotify(LRRC_LNAS_CSFB_STATUS_END);
    }

    switch(NAS_EMM_SER_GetEmmSERStartCause())
    {
        case    NAS_EMM_SER_START_CAUSE_SMS_EST_REQ:

                NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionSerRejCasue3: SMS EST REQ.");
                TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionSerRejCasue3_ENUM, LNAS_FUNCTION_LABEL1);

                /* 上报TAU结果 */
                NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
                NAS_LMM_SndLmmSmsErrInd(LMM_SMS_ERR_CAUSE_OTHERS);
                NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);

                /* 通知RABM 这次SERVICE 失败 */
                NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
                break;

        case    NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ:
        case    NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ:

                NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionSerRejCasue3: MO/MT CSFB.");
                TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionSerRejCasue3_ENUM, LNAS_FUNCTION_LABEL2);

                NAS_EMM_SetCsfbProcedureFlag(PS_FALSE);
                NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_TAU_COLLISION_RSLT_ABNORMAL, NAS_LMM_CAUSE_NULL);

                /* 上报TAU结果, 顺序一定要在给MM发终止之后 */
                NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
                break;

        case    NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ:

                NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionSerRejCasue3: EMERGENCY CSFB.");
                TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionSerRejCasue3_ENUM, LNAS_FUNCTION_LABEL3);

                /* 上报TAU结果, 顺序一定要在给MMC发SER结果之前*/
                NAS_EMM_MmcSendTauActionResultIndRej(VOS_NULL_PTR);

                /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
                NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

                break;

        default:
                /* 上报TAU结果 */
                NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

                /* 通知RABM 这次SERVICE 失败 */
                NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
                break;
    }

    /* 清除冲突标志 */
    NAS_EMM_TAU_SaveEmmCollisionCtrl( NAS_EMM_COLLISION_NONE);

    NAS_EMM_TAU_ClearActiveFlagProc();

    /**清除保存数据的标志和数据区**************/
    NAS_EMM_SerClearEsmDataBuf();

    return;

}


VOS_VOID NAS_EMM_TAU_TAUCollisionSerRejCasue7
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    (VOS_VOID)enTauRejcause;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionSerRejCasue7 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionSerRejCasue7_ENUM, LNAS_ENTRY);

    /* ESR流程中发生TAU流程, TAU失败导致CSFB流程失败, 通知LRRC CSFB流程终止,
    LRRC收到该消息后, 若等待网侧释放定时器启动时, 停止定时器 */
    if(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        NAS_EMM_SndLrrcLmmCsfbNotify(LRRC_LNAS_CSFB_STATUS_END);
    }

    switch(NAS_EMM_SER_GetEmmSERStartCause())
    {
        case    NAS_EMM_SER_START_CAUSE_SMS_EST_REQ:

                NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionSerRejCasue7: SMS EST REQ.");
                TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionSerRejCasue7_ENUM, LNAS_FUNCTION_LABEL1);

                /* 上报TAU结果 */
                NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);
                NAS_LMM_SndLmmSmsErrInd(LMM_SMS_ERR_CAUSE_OTHERS);
                NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);

                /* 通知RABM 这次SERVICE 失败 */
                NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
                break;

        case    NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ:
        case    NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ:
        case    NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ:

                NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionSerRejCasue7: MO/MT/EMERGENCY CSFB.");
                TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionSerRejCasue7_ENUM, LNAS_FUNCTION_LABEL2);

                /* 上报TAU结果, 顺序一定要在给MMC发SER结果之前*/
                NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

                /*向MMC发送LMM_MMC_SERVICE_RESULT_IND消息*/
                NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

                break;

        default:
                /* 上报TAU结果 */
                NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

                /* 通知RABM 这次SERVICE 失败 */
                NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
                break;
    }

    /* 清除冲突标志 */
    NAS_EMM_TAU_SaveEmmCollisionCtrl(NAS_EMM_COLLISION_NONE);

    NAS_EMM_TAU_ClearActiveFlagProc();

    /**清除保存数据的标志和数据区**************/
    NAS_EMM_SerClearEsmDataBuf();

    return;

}


VOS_VOID NAS_EMM_TAU_TAUCollisionSerRejCasueOther
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    NAS_EMM_TAU_TAUCollisionSerRejCasue7(enTauRejcause);
    return;
}



VOS_VOID  NAS_EMM_TAU_CollisionDetachRcvCnRejProc
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    VOS_UINT32                                     i;
    NAS_LMM_TAU_COLLISION_SER_RCV_CN_REJ_PROC_FUN  pfProcFun;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_CollisionDetachRcvCnRejProc is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_CollisionDetachRcvCnRejProc_ENUM, LNAS_ENTRY);

    for (i = 0; i < gstEmmTauCollisionDetachRcvCnRejProcTblLen; i++)
    {
        if (enTauRejcause == gstEmmTauCollisionDetachRcvCnRejProcTbl[i].enRejCauseVal)
        {
            pfProcFun = gstEmmTauCollisionDetachRcvCnRejProcTbl[i].pfProcFun;

            /* 有处理函数执行 */
            if (VOS_NULL_PTR != pfProcFun)
            {
                (pfProcFun)(enTauRejcause);
            }

            /* 无处理函数无动作直接返回 */
            return;
        }
    }

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_CollisionDetachRcvCnRejProc: other cause.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_CollisionDetachRcvCnRejProc_ENUM, LNAS_FUNCTION_LABEL1);

    /*其它原因值处理*/
    NAS_EMM_TAU_TAUCollisionDetachRejCasueOther(enTauRejcause);

    return;
}


VOS_VOID NAS_EMM_TAU_TAUCollisionDetachRejCasue3
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    (VOS_VOID)enTauRejcause;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue3 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue3_ENUM, LNAS_ENTRY);

    /* 上报TAU结果 */
    NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

    NAS_EMM_TAU_CollisionDetachProc();

    if (MMC_LMM_MO_DET_CS_ONLY != NAS_EMM_GLO_AD_GetDetTypeMo())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue3: PS/CS_PS DETACH.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue3_ENUM, LNAS_FUNCTION_LABEL1);

        /* 本地DETACH*/
        NAS_LMM_DeregReleaseResource();
    }
    else
    {
        /* IMSI DETACH后释放资源 */
        NAS_LMM_ImsiDetachReleaseResource();
    }

    return;

}


VOS_VOID NAS_EMM_TAU_TAUCollisionDetachRejCasue9
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    (VOS_VOID)enTauRejcause;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue9 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue9_ENUM, LNAS_ENTRY);

    /* 上报TAU结果 */
    NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

    NAS_EMM_TAU_CollisionDetachProc();

    if (MMC_LMM_MO_DET_CS_ONLY != NAS_EMM_GLO_AD_GetDetTypeMo())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue9: PS/CS_PS DETACH.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue9_ENUM, LNAS_FUNCTION_LABEL1);

       /* 本地DETACH*/
       NAS_LMM_DeregReleaseResource();
       NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }
    else
    {
       /* IMSI DETACH后释放资源 */
       NAS_LMM_ImsiDetachReleaseResource();
    }

    return;

}


VOS_VOID NAS_EMM_TAU_TAUCollisionDetachRejCasue13
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    (VOS_VOID)enTauRejcause;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue13 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue13_ENUM, LNAS_ENTRY);

    /* 上报TAU结果 */
    NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

    if (MMC_LMM_MO_DET_CS_ONLY != NAS_EMM_GLO_AD_GetDetTypeMo())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue13: PS/CS_PS DETACH.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue13_ENUM, LNAS_FUNCTION_LABEL1);

        /*修改状态：进入主状态DEREG子状态DEREG_LIMITED_SERVICE*/
        NAS_EMM_AdStateConvert(EMM_MS_DEREG,
                               EMM_SS_DEREG_LIMITED_SERVICE,
                               TI_NAS_EMM_STATE_NO_TIMER);
        NAS_EMM_TAU_CollisionDetachProc();

        /* 本地DETACH*/
        NAS_LMM_DeregReleaseResource();
    }
    else
    {
        /* 设置注册域为PS */
        NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

        NAS_EMM_TAU_CollisionDetachProc();

        /* IMSI DETACH后释放资源 */
        NAS_LMM_ImsiDetachReleaseResource();
    }

    return;

}


VOS_VOID NAS_EMM_TAU_TAUCollisionDetachRejCasue22
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    (VOS_VOID)enTauRejcause;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue22 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue22_ENUM, LNAS_ENTRY);

    /* 上报TAU结果 */
    NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

    if (MMC_LMM_MO_DET_CS_ONLY != NAS_EMM_GLO_AD_GetDetTypeMo())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue22: PS/CS_PS DETACH.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue22_ENUM, LNAS_FUNCTION_LABEL1);

        /*修改状态：进入主状态DEREG子状态DEREG_NORMAL_SERVICE*/
        NAS_EMM_AdStateConvert(EMM_MS_DEREG,
                               EMM_SS_DEREG_NORMAL_SERVICE,
                               TI_NAS_EMM_STATE_NO_TIMER);

        /*清空TAU相关全局变量，通知ESM，APP去注册*/
        NAS_EMM_TAU_CollisionDetachProc();

        /*本地DETACH释放资源:动态内存、赋初值 */
        NAS_LMM_DeregReleaseResource();
    }
    else
    {
        /* 如果是IMSI DETACH类型，则本地CS域去注册，继续处理TAU过程中收到RRC链路释放 */

        /* 设置注册域为PS */
        NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

        /*清空TAU相关全局变量，通知ESM，APP去注册*/
        NAS_EMM_TAU_CollisionDetachProc();

        /* IMSI DETACH后释放资源 */
        NAS_LMM_ImsiDetachReleaseResource();
    }

    return;

}
#if(FEATURE_ON == FEATURE_CSG)

VOS_VOID NAS_EMM_TAU_TAUCollisionDetachRejCasue25
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    (VOS_VOID)enTauRejcause;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue25 is entered.");
    TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue25_ENUM, LNAS_ENTRY);

    /* 上报TAU结果 */
    NAS_EMM_MmcSendTauActionResultIndRej((VOS_VOID*)NAS_EMM_NULL_PTR);

    if (MMC_LMM_MO_DET_CS_ONLY != NAS_EMM_GLO_AD_GetDetTypeMo())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_TAUCollisionDetachRejCasue25: PS/CS_PS DETACH.");
        TLPS_PRINT2LAYER_INFO(NAS_EMM_TAU_TAUCollisionDetachRejCasue25_ENUM, LNAS_FUNCTION_LABEL1);

        /*修改状态：进入主状态DEREG子状态DEREG_NORMAL_SERVICE*/
        NAS_EMM_AdStateConvert(EMM_MS_DEREG,
                               EMM_SS_DEREG_NORMAL_SERVICE,
                               TI_NAS_EMM_STATE_NO_TIMER);

        /*清空TAU相关全局变量，通知ESM，APP去注册*/
        NAS_EMM_TAU_CollisionDetachProc();

        /*本地DETACH释放资源:动态内存、赋初值 */
        NAS_LMM_DeregReleaseResource();
    }
    else
    {
        /* 如果是IMSI DETACH类型，则本地CS域去注册，继续处理TAU过程中收到RRC链路释放 */

        /* 设置注册域为PS */
        NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

        /*清空TAU相关全局变量，通知ESM，APP去注册*/
        NAS_EMM_TAU_CollisionDetachProc();

        /* IMSI DETACH后释放资源 */
        NAS_LMM_ImsiDetachReleaseResource();
    }

    return;

}
#endif

VOS_VOID NAS_EMM_TAU_TAUCollisionDetachRejCasueOther
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enTauRejcause
)
{
    NAS_EMM_TAU_TAUCollisionDetachRejCasue22(enTauRejcause);
    return;
}




/*lint +e960*/



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



