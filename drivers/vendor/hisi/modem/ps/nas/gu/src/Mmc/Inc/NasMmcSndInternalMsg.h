
#ifndef _NAS_MMC_SND_INTERNAL_MSG_H_
#define _NAS_MMC_SND_INTERNAL_MSG_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include  "NasMmcCtx.h"
#include  "NasCommDef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


enum NAS_MMC_INTERNAL_MSG_ID_ENUM
{
    /* MMC�յ����ڲ���ϢID,MMC�ڲ��յ�����ϢID��0x000��0x199 */
    MMCMMC_ABORT_FSM_REQ                       = 0,
    MMCMMC_INTER_PLMN_SEARCH_REQ               = 1,                                  /* �ڲ��������� ,���³�ʼ���������������� */
    MMCMMC_INTER_ANYCELL_SEARCH_REQ            = 2,                                  /* �ڲ��������� ,���³�ʼ���������������� */
    MMCMMC_INTER_PLMN_LIST_REQ                 = 3,

    MMCMMC_SWITCH_ON_RSLT_CNF                  = 4,
    MMCMMC_POWER_OFF_RSLT_CNF                  = 5,
    MMCMMC_SUSPEND_RSLT_CNF                    = 6,
    MMCMMC_PLMN_LIST_RSLT_CNF                  = 7,

    MMCMMC_PLMN_SELECTION_RSLT_CNF             = 8,

    MMCMMC_ANYCELL_SEARCH_RSLT_CNF             = 9,

    MMCMMC_SYSCFG_RSLT_CNF                     = 10,

    MMCMMC_BG_PLMN_SEARCH_RSLT_CNF             = 11,

    MMCMMC_INTER_NVIM_OPLMN_REFRESH_IND        = 12,

    MMCMMC_INTER_SKIP_SEARCH_W_IND             = 13,

    MMCMMC_INTER_SKIP_SEARCH_TDS_IND           = 14,
    MMCMMC_INTER_ABORT_UTRAN_CTRL_PLMN_SEARCH_REQ = 15,
    MMCMMC_INTER_ABORT_UTRAN_CTRL_PLMN_SEARCH_CNF = 16,

    MMCMMC_GET_GEO_RSLT_CNF                    = 17,
    MMCMMC_INTER_DPLMN_SET_REQ                 = 18,

    MMCMMC_INTER_UTRAN_CTRL_PLMN_SELECTION_ABORT_FINISH_IND = 19,

    MMCMMC_INTER_BG_SEARCH_REQ                              = 20,

    MMCMMC_INTERNAL_MSG_ID_BUTT
};
typedef VOS_UINT32 NAS_MMC_INTERNAL_MSG_ID_ENUM_U32;



enum NAS_MMC_ANYCELL_SEARCH_RESULT_ENUM
{
    NAS_MMC_ANYCELL_SEARCH_SUCC         = 0,
    NAS_MMC_ANYCELL_SEARCH_FAIL         = 1,
    NAS_MMC_ANYCELL_SEARCH_ABORTED      = 2,
    NAS_MMC_ANYCELL_SEARCH_REJECT       = 3,
    NAS_MMC_PLMN_SEARCH_BUTT
};
typedef VOS_UINT32 NAS_MMC_ANYCELL_SEARCH_RESULT_ENUM_UINT32;


enum NAS_MMC_PLMN_SELECTION_RESULT_ENUM
{
    NAS_MMC_PLMN_SELECTION_SUCC            = 0,                                    /*����������פ��*/
    NAS_MMC_PLMN_SELECTION_FAIL            = 1,                                    /*�������ע�ᵼ�¿���Ч*/
    NAS_MMC_PLMN_SELECTION_ABORTED         = 2,                                    /* PlmnSelection״̬�������*/
    NAS_MMC_PLMN_SELECTION_INTER_SYS_HRPD  = 3,                                 /* ������������ѡ��HRPD */
    NAS_MMC_PLMN_SELECTION_BUTT
};
typedef VOS_UINT32 NAS_MMC_PLMN_SELECTION_RESULT_ENUM_UINT32;



enum NAS_MMC_SUSPEND_RSLT_ENUM
{
    NAS_MMC_SUSPEND_SUCCESS            = 0,
    NAS_MMC_SUSPEND_FAIL               = 1,
    NAS_MMC_SUSPEND_ABORTED            = 2,
    NAS_MMC_SUSPEND_BUTT
};
typedef VOS_UINT32 NAS_MMC_SUSPEND_RSLT_ENUM_UINT32;



enum NAS_MMC_GET_GEO_RSLT_ENUM
{
    NAS_MMC_GET_GEO_SUCCESS             = 0,
    NAS_MMC_GET_GEO_FAIL                = 1,
    NAS_MMC_GET_GEO_ABORTED             = 2,
    NAS_MMC_GET_GEO_ABNORMAL            = 3,                /* ״̬�����쳣��ֹ */
    NAS_MMC_GET_GEO_NO_RF               = 4,                /* GET GEO���ؽ��ΪNO_RF */
    NAS_MMC_GET_GEO_BUTT
};
typedef VOS_UINT32 NAS_MMC_GET_GEO_RSLT_ENUM_UINT32;


enum NAS_MMC_SUSPEND_FAIL_CAUSE_ENUM
{
    NAS_MMC_SUSPEND_FAIL_CAUSE_AREA_LOST        = 0,
    NAS_MMC_SUSPEND_FAIL_CAUSE_BUTT
};
typedef VOS_UINT8 NAS_MMC_SUSPEND_FAIL_CAUSE_ENUM_UINT8;


enum NAS_MMC_PLMN_LIST_RESULT_ENUM
{
    NAS_MMC_PLMN_LIST_SUCC         = 0,
    NAS_MMC_PLMN_LIST_FAIL         = 1,
    NAS_MMC_PLMN_LIST_ABORTED      = 2,

    NAS_MMC_PLMN_LIST_NO_RF_FAIL    = 3,

    NAS_MMC_PLMN_LIST_BUTT
};
typedef VOS_UINT32 NAS_MMC_PLMN_LIST_RESULT_ENUM_UINT32;


enum NAS_MMC_BG_PLMN_SEARCH_RSLT_ENUM
{
    NAS_MMC_BG_PLMN_SEARCH_SUCC         = 0,                                    /* BG�����ɹ���� */
    NAS_MMC_BG_PLMN_SEARCH_FAIL         = 1,                                    /* BG����δ��� */
    NAS_MMC_BG_PLMN_SEARCH_ABORTED      = 2,                                    /* BG������Abort */

    NAS_MMC_BG_PLMN_SEARCH_INTER_SYS_HRPD  = 3,                                 /* BG������������ѡ��HRPD */

    NAS_MMC_BG_PLMN_SEARCH_BUTT
};
typedef VOS_UINT32 NAS_MMC_BG_PLMN_SEARCH_RSLT_ENUM_UINT32;



enum NAS_MMC_HIGH_PRIO_PLMN_SEARCH_RSLT_ENUM
{
    NAS_MMC_HIGH_PRIO_PLMN_SEARCH_SUCC         = 0,                             /* �����ȼ������ɹ���� */
    NAS_MMC_HIGH_PRIO_PLMN_SEARCH_FAIL         = 1,                             /* �����ȼ�����δ��� */
    NAS_MMC_HIGH_PRIO_PLMN_SEARCH_ABORTED      = 2,                             /* �����ȼ�������Abort */
    NAS_MMC_HIGH_PRIO_PLMN_SEARCH_BUTT
};
typedef VOS_UINT32 NAS_MMC_HIGH_PRIO_PLMN_SEARCH_RSLT_ENUM_UINT32;



enum NAS_MMC_SEARCHED_LIST_INFO_TYPE_ENUM
{
    NAS_MMC_SEARCHED_LIST_INFO_TYPE_NOT_CSG_LIST = 0,  /* list�����������ͨ�û��б��������ڲ��б��������� */
    NAS_MMC_SEARCHED_LIST_INFO_TYPE_CSG_LIST     = 1,  /* list���������CSG LIST�������� */
    NAS_MMC_SEARCHED_LIST_INFO_TYPE_BUTT
};
typedef VOS_UINT8 NAS_MMC_SEARCHED_LIST_INFO_TYPE_ENUM_UINT8;


enum NAS_MMC_POWEROFF_RSLT_ENUM
{
    NAS_MMC_POWEROFF_RSLT_POWER_OFF            = 0,
    NAS_MMC_POWEROFF_RSLT_POWER_SAVE           = 1,
    NAS_MMC_POWEROFF_RSLT_BUTT
};
typedef VOS_UINT32 NAS_MMC_POWEROFF_RSLT_ENUM_UINT32;

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
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgID;                                /* ��ϢID */

    VOS_UINT32                          ulEventType;                            /* ����¼�ID */

    NAS_MMC_ABORT_FSM_TYPE_UINT8        enAbortFsmType;
    VOS_UINT8                           aucReserved[3];
}NAS_MMC_ABORT_FSM_STRU;


typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;
    NAS_MML_PLMN_RAT_PRIO_STRU          stAnycellSrchRatList;                   /* ��ǰҪ�����Ľ��뼼���б� */

    NAS_MMC_ANYCELL_SEARCH_SCENE_ENUM_UINT8  enAnycellSearchScene;
    VOS_UINT8                            aucReserved[3];
}NAS_MMC_INTER_ANYCELL_SEARCH_REQ_STRU;



typedef struct
{
    MSG_HEADER_STRU                         stMsgHeader;            /* ��Ϣͷ */
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32   enPlmnSearchScene;      /* �������� */
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU    astInterPlmnSearchInfo[NAS_MML_MAX_RAT_NUM]; /* Я��������Ϣ */
}NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU;


typedef struct
{
    MSG_HEADER_STRU                                         MsgHeader;                                      /* ��Ϣͷ */
    NAS_MMC_PLMN_LIST_SEARCH_SCENE_ENUM_UINT8               enListPlmnSearchScene;                          /* �ڲ��б��������� */
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astInterPlmnSearchInfo[NAS_MML_MAX_RAT_NUM];    /* Я��������Ϣ */
}NAS_MMC_INTER_PLMN_LIST_REQ_STRU;


typedef struct
{
    MSG_HEADER_STRU                                         MsgHeader;          /* ��Ϣͷ */
    NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM_UINT8                 enBgPlmnSearchScene;  /* �������� */
    VOS_UINT8                                               aucReserve[3];
}NAS_MMC_INTER_BG_SEARCH_REQ_STRU;


typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                                        /* ��Ϣͷ */
    VOS_UINT8                           aucVersionId[NAS_MSCC_PIF_INFO_VERSION_LEN];        /* �汾�ţ��̶�Ϊxx.xx.xxx */
    VOS_UINT8                           ucEhPlmnNum;                                        /* EHPLMN�ĸ���*/
    VOS_UINT8                           aucReserve[2];
    NAS_MML_PLMN_ID_STRU                astEhPlmnInfo[NAS_MML_MAX_EHPLMN_NUM + 1];          /* EHPLMN ID�б� */
}NAS_MMC_INTER_DPLMN_SET_REQ_STRU;


typedef struct
{
    MSG_HEADER_STRU                               stMsgHeader;  /* ��Ϣͷ */

    NAS_MMC_GET_GEO_RSLT_ENUM_UINT32              enGetGeoRslt; /* ��ȡ������Ϣ��� */
    NAS_MML_PLMN_ID_STRU                          stGetGeoPlmn; /* ��ȡ������Ϣ�ɹ�ʱ��Ч����ȡ����PLMN */
}NAS_MMCMMC_GET_GEO_RSLT_STRU;


typedef struct
{
    MSG_HEADER_STRU                            MsgHeader;
    VOS_UINT32                                 ulResult;
} NAS_MMC_SWITCHON_RLST_STRU;



typedef struct
{
    MSG_HEADER_STRU                             MsgHeader;
    NAS_MMC_ANYCELL_SEARCH_RESULT_ENUM_UINT32   enResult;
} NAS_MMCMMC_ANYCELL_SEARCH_RSLT_STRU;


typedef struct
{
    MSG_HEADER_STRU                             MsgHeader;                              /*��Ϣͷ*/
    NAS_MMC_PLMN_SELECTION_RESULT_ENUM_UINT32   enResult;                               /*ѡ�����*/
    VOS_UINT32                                  ulCampFlg;                              /*��ǰ�Ƿ�פ����־,VOS_TRUE:��פ��,VOS_FALSE:δפ��*/
    VOS_UINT32                                  ulRatNum;                               /*�и��ǵĽ��뼼������*/
    NAS_MMC_RAT_SEARCH_INFO_STRU                astSearchRatInfo[NAS_MML_MAX_RAT_NUM];  /*���治ͬ���뼼����������Ϣ*/
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32       ulPlmnSearchScene;                      /* �������� */
} NAS_MMCMMC_PLMN_SELECTION_RSLT_STRU;


typedef struct
{
    MSG_HEADER_STRU                             MsgHeader;
    VOS_UINT8                                   aucReserved[4];
} NAS_MMCMMC_INTER_SKIP_SEARCH_W_IND_STRU;


typedef struct
{
    MSG_HEADER_STRU                             MsgHeader;
    VOS_UINT8                                   aucReserved[4];
} NAS_MMCMMC_INTER_SKIP_SEARCH_TDS_IND_STRU;


typedef struct
{
    MSG_HEADER_STRU                             MsgHeader;
    VOS_UINT8                                   aucReserved[4];
} NAS_MMCMMC_INTER_ABORT_UTRAN_CTRL_PLMN_SEARCH_REQ_STRU;


typedef struct
{
    MSG_HEADER_STRU                             MsgHeader;
    VOS_UINT8                                   aucReserved[4];
} NAS_MMCMMC_INTER_ABORT_UTRAN_CTRL_PLMN_SEARCH_CNF_STRU;




typedef struct
{
    MSG_HEADER_STRU                             MsgHeader;
    VOS_UINT8                                   aucReserved[4];
} NAS_MMCMMC_INTER_UTRAN_CTRL_PLMN_SELECTION_ABORT_FINISH_IND_STRU;



typedef struct
{
    MSG_HEADER_STRU                                         stMsgHeader;

    /* �������� */
    MMC_SUSPEND_CAUSE_ENUM_UINT8                            enSuspendCause;

    /* ״̬��ʧ��ԭ�� */
    NAS_MMC_SUSPEND_FAIL_CAUSE_ENUM_UINT8                   enFailCause;

    MMC_SUSPEND_DESTINATION_UINT8                           enSuspendDestination;

    VOS_UINT8                                               ucReserve;

    /*״̬����ɵĽ�� */
    NAS_MMC_SUSPEND_RSLT_ENUM_UINT32                        enRslt;


    /* ������ϱ��������б� */
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnSearchInfo[NAS_MML_MAX_RAT_NUM];
}NAS_MMCMMC_SUSPEND_RSLT_STRU;



typedef struct
{
    MSG_HEADER_STRU                                         stMsgHeader;

    /* ״̬����ɵĽ�� */
    NAS_MMC_PLMN_LIST_RESULT_ENUM_UINT32                    enRslt;

    /* �Ƿ���Ҫ���� */
    VOS_UINT8                                               ucNeedPlmnSearch;

    /* ��ǰ����CS��ע�����ĸ��Ӵ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsRegAdditionalAction;

    /* ��ǰ����PS��ע�����ĸ��Ӵ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsRegAdditionalAction;

    VOS_UINT8                                               aucReserve[1];

    /* plmn list����״̬���Ľ�� */
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnSearchInfo[NAS_MML_MAX_RAT_NUM];

    NAS_MMC_PLMN_LIST_SEARCH_SCENE_ENUM_UINT8               enPlmnListSearchScene;            /* Я����������*/
    VOS_UINT8                                               aucReserved[3];

}NAS_MMCMMC_PLMN_LIST_RSLT_STRU;


typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;

    NAS_MMC_POWEROFF_RSLT_ENUM_UINT32   enRslt;
}NAS_MMCMMC_POWEROFF_RSLT_STRU;



typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;        /* ��Ϣͷ */
    VOS_UINT32                          ulRslt;             /* SYSCFG״̬����ɽ��:VOS_TRUE:SYSCFG���óɹ���VOS_FALSE:SYSCFG����ʧ�� */
    VOS_UINT32                          ulNeedPlmnSrch;     /* SYSCFG״̬������Ƿ���Ҫ����:VOS_TRUE:SYSCFG���óɹ�����Ҫ������VOS_FALSE:YSCFG���óɹ�����Ҫ���� */

    VOS_UINT32                          ulIsNeedAttachPs;

    NAS_MML_PLMN_RAT_PRIO_STRU          stPrePrioRatList;   /* ��syscfg״̬��ǰ�Ľ��뼼�������ȼ� */


} NAS_MMCMMC_SysCfg_RSLT_STRU;


typedef struct
{
    MSG_HEADER_STRU                             stMsgHeader;                    /* ��Ϣͷ */
    NAS_MMC_BG_PLMN_SEARCH_RSLT_ENUM_UINT32     enRslt;                         /* BG������� */
    VOS_UINT32                                  ulNeedPlmnSearch;               /* �Ƿ���Ҫ������־,VOS_TRUE:��Ҫ����,VOS_FALSE:����Ҫ���� */
    VOS_UINT32                                  ulSearchCompleted;              /* �Ƿ���ɸ����ȼ�������־,VOS_TRUE::���ָ����ȼ�����������VOS_FALSE:���ָ����ȼ�����δ���� */
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU        astSearchedPlmnListInfo[NAS_MML_MAX_RAT_NUM];         /* Я��������Ϣ */
    NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM_UINT8     enBgPlmnSearchScene;            /* Я����������*/

    NAS_MMC_BG_SEARCH_TYPE_ENUM_UINT8           enFsmBgSearchType;              /* Я������״̬�����еı����������� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8        enCsRegAdditionalAction;          /* ��ǰ����CS��ע�����ĸ��Ӵ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8        enPsRegAdditionalAction;          /* ��ǰ����PS��ע�����ĸ��Ӵ��� */

} NAS_MMCMMC_BG_PLMN_SEARCH_RSLT_STRU;

typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;                        /* ��Ϣͷ */
}NAS_MMC_INTER_NVIM_OPLMN_REFRESH_IND_STRU;


typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;                        /* ��Ϣͷ */
}NAS_MMC_INTER_UPDATE_UPLMN_NTF_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID  NAS_MMC_SndAbortFsmMsg(
    VOS_UINT32                          ulEventType,
    NAS_MMC_ABORT_FSM_TYPE_UINT8        enAbortFsmType
);
VOS_VOID NAS_MMC_SndSwitchOnRlst(VOS_UINT32 ulResult);

VOS_VOID  NAS_MMC_SndSuspendRsltMsg(
    MMC_SUSPEND_CAUSE_ENUM_UINT8                            enSuspendCause,
    NAS_MMC_SUSPEND_RSLT_ENUM_UINT32                        enSuspendRslt,
    NAS_MMC_SUSPEND_FAIL_CAUSE_ENUM_UINT8                   enSuspendFailCause,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstListInfo
);


VOS_VOID  NAS_MMC_SndPlmnSelectionRslt(
    NAS_MMC_PLMN_SELECTION_RESULT_ENUM_UINT32               enPlmnSelectionRslt,
    VOS_UINT32                                              ulCampFlg,
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo,
    VOS_UINT32                                              ulRatNum,
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   ulPlmnSearchScene
);

VOS_VOID  NAS_MMC_SndInterSkipSearchWIndMsg(VOS_VOID);

VOS_VOID  NAS_MMC_SndInterSkipSearchTdsIndMsg(VOS_VOID);

VOS_VOID  NAS_MMC_SndInterAbortUtranCtrlPlmnSearchCnfMsg(VOS_VOID);

VOS_VOID  NAS_MMC_SndInterAbortUtranCtrlPlmnSearchReqMsg(VOS_VOID);



VOS_VOID NAS_MMC_SndAnycellSearchRslt(
    NAS_MMC_ANYCELL_SEARCH_RESULT_ENUM_UINT32               enResult
);

VOS_VOID  NAS_MMC_SndBgPlmnSearchRslt(
    NAS_MMCMMC_BG_PLMN_SEARCH_RSLT_STRU                    *pstBgRsltCnf
);


VOS_VOID  NAS_MMC_SndSysCfgRslt(
    VOS_UINT32                          ulSysCfgRslt,
    VOS_UINT32                          ulNeedPlmnSrch,
    VOS_UINT32                          ulIsNeedAttachPs,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrePrioRatList
);

VOS_VOID NAS_MMC_SndInterPlmnSearchReq(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32    enPlmnSearchScene,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU    *pstIntraPlmnSrchInfo,
    VOS_UINT8                                ucRatNum
);

VOS_VOID NAS_MMC_SndInterPlmnListReq(
    NAS_MMC_PLMN_LIST_SEARCH_SCENE_ENUM_UINT8               enListPlmnSearchScene,
    VOS_UINT32                                              ucRatNum,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
);

VOS_VOID NAS_MMC_SndInterBgSearchReq(
    NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM_UINT8                 enBgPlmnSearchScene
);

VOS_VOID NAS_MMC_SndInterAnyCellSearchReq(
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstAnycellSrchRatList,
    NAS_MMC_ANYCELL_SEARCH_SCENE_ENUM_UINT8                 enAnycellSearchScene
);


VOS_VOID NAS_MMC_BulidInterPlmnSearchReqMsg(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstIntraPlmnSrchInfo,
    VOS_UINT8                                               ucRatNum,
    NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU                     *pstMsg
);

VOS_VOID NAS_MMC_BuildInterDplmnSetReqMsg(
    VOS_UINT8                                              *pucVersion,
    VOS_UINT8                                               ucEhPlmnNum,
    NAS_MSCC_PIF_PLMN_ID_STRU                              *pstEhPlmnInfo,
    NAS_MMC_INTER_DPLMN_SET_REQ_STRU                       *pstMsg
);
VOS_VOID NAS_MMC_SndPowerOffRslt(
    NAS_MMC_POWEROFF_RSLT_ENUM_UINT32   enRslt
);

VOS_VOID NAS_MMC_SndPlmnListRslt(
    NAS_MMC_PLMN_LIST_RESULT_ENUM_UINT32                    enRslt,
    VOS_UINT8                                               ulNeedPlmnSearch,
    NAS_MMC_PLMN_LIST_SEARCH_SCENE_ENUM_UINT8               enPlmnListSearchScene,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    *pstListInfo,
    NAS_MMC_PLMN_LIST_REG_RSLT_INFO_STRU                    *pstRegRsltInfo
);

VOS_VOID NAS_MMC_SndInterNvimOPlmnRefreshInd(VOS_VOID);

VOS_VOID  NAS_MMC_SndGetGeoRsltMsg(
    NAS_MMC_GET_GEO_RSLT_ENUM_UINT32    enGetGeoRslt,
    NAS_MML_PLMN_ID_STRU               *pstGetGeoPlmn
);

VOS_VOID NAS_MMC_SndInterUpdateUplmnNtf(VOS_VOID);

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

#endif
