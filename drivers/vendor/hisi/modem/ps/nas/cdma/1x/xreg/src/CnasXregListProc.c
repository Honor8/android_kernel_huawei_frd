

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "CnasXregListProc.h"
#include  "CnasMntn.h"
#include  "CnasXregTimer.h"
#include  "CnasXregMntn.h"

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_CNAS_XREG_LIST_PROC_C

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ���ZONE_LIST & SIDNID_LIST�ڵ���ڴ����� */
CNAS_XREG_ZONE_NODE_STRU                g_astCnasXregZoneNode[CNAS_XREG_ZONE_NODE_MAX_NUM];
CNAS_XREG_SIDNID_NODE_STRU              g_astCnasXregSidNidNode[CNAX_XREG_SIDNID_NODE_MAX_NUM];

/* ָ��ǰZONE_LIST & SIDNID_LIST�Ľڵ� */
CNAS_XREG_ZONE_NODE_STRU               *g_pstCnasXregCurrZoneNode   = VOS_NULL_PTR;
CNAS_XREG_SIDNID_NODE_STRU             *g_pstCnasXregCurrSidNidNode = VOS_NULL_PTR;

/* ά��ZONE_LIST & SIDNID_LIST������ͷ */
CNAS_XREG_LIST_HEADER_STRU              g_astCnasXregListHeader[CNAS_XREG_LIST_BUTT];

/* ��ŵ�ǰ��ʱ����Ϣ */
CNAS_XREG_TIME_INFO_STRU                g_stCnasXregTimeInfo;

/* ZONE_TIMER���������ο�Э��: Table 3.7.2.3.2.1-1. */
VOS_UINT16                              g_ausCnasXregZoneTimerMap[] = {60, 120, 300, 600,
                                                                       1200, 1800, 2700, 3600};

/*****************************************************************************
  3 ��������
*****************************************************************************/
/*lint -save -e958*/


VOS_UINT8 CNAS_XREG_GetNodeCount(CNAS_XREG_LIST_ID_ENUM_UINT8 enListId)
{
    return (VOS_UINT8)g_astCnasXregListHeader[enListId].ulNodeCount;
}


VOS_VOID CNAS_XREG_SetNodeCount(CNAS_XREG_LIST_ID_ENUM_UINT8 enListId, VOS_UINT32 ulNodeCount)
{
    g_astCnasXregListHeader[enListId].ulNodeCount = ulNodeCount;

    return;
}


VOS_VOID CNAS_XREG_IncNodeCount(CNAS_XREG_LIST_ID_ENUM_UINT8 enListId)
{
    g_astCnasXregListHeader[enListId].ulNodeCount++;

    return;
}


VOS_VOID CNAS_XREG_DecNodeCount(CNAS_XREG_LIST_ID_ENUM_UINT8 enListId)
{
    g_astCnasXregListHeader[enListId].ulNodeCount--;

    return;
}


HI_LIST_S* CNAS_XREG_GetListHeader(CNAS_XREG_LIST_ID_ENUM_UINT8 enListId)
{
    return &(g_astCnasXregListHeader[enListId].stListHeader);
}


HI_LIST_S* CNAS_XREG_AllocListNode(CNAS_XREG_LIST_ID_ENUM_UINT8 enListId)
{
    HI_LIST_S                           *pstIndexNode;

    if (0 == CNAS_XREG_GetNodeCount(enListId))
    {
        return VOS_NULL_PTR;
    }

    pstIndexNode = CNAS_XREG_GetListHeader(enListId)->next;

    msp_list_del(pstIndexNode);

    CNAS_XREG_DecNodeCount(enListId);

    return pstIndexNode;
}


VOS_VOID CNAS_XREG_RelCurrZoneNode(VOS_VOID)
{
    msp_list_add_tail(&(g_pstCnasXregCurrZoneNode->stListNode), CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_FREE_LIST_ID));

    CNAS_XREG_IncNodeCount(CNAS_XREG_ZONE_FREE_LIST_ID);

    g_pstCnasXregCurrZoneNode = VOS_NULL_PTR;

    return;
}


VOS_VOID CNAS_XREG_RelZoneListNode(HI_LIST_S *pstZoneNode)
{
    msp_list_del(pstZoneNode);

    CNAS_XREG_DecNodeCount(CNAS_XREG_ZONE_USED_LIST_ID);

    msp_list_add_tail(pstZoneNode, CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_FREE_LIST_ID));

    CNAS_XREG_IncNodeCount(CNAS_XREG_ZONE_FREE_LIST_ID);

    return;
}


VOS_VOID CNAS_XREG_RelCurrSidNidNode(VOS_VOID)
{
    msp_list_add_tail(&(g_pstCnasXregCurrSidNidNode->stListNode), CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_FREE_LIST_ID));

    CNAS_XREG_IncNodeCount(CNAS_XREG_SIDNID_FREE_LIST_ID);

    g_pstCnasXregCurrSidNidNode = VOS_NULL_PTR;

    return;
}


VOS_VOID CNAS_XREG_RelSidNidListNode(HI_LIST_S *pstSidNidNode)
{
    msp_list_del(pstSidNidNode);

    CNAS_XREG_DecNodeCount(CNAS_XREG_SIDNID_USED_LIST_ID);

    msp_list_add_tail(pstSidNidNode, CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_FREE_LIST_ID));

    CNAS_XREG_IncNodeCount(CNAS_XREG_SIDNID_FREE_LIST_ID);
}


VOS_VOID CNAS_XREG_InitList(VOS_VOID)
{
    VOS_UINT8                           ucIndex;
    HI_LIST_S                          *pstListHeader;

    /* ��ʼ������ͷ */
    for (ucIndex = 0; ucIndex < CNAS_XREG_LIST_BUTT; ucIndex++)
    {
        CNAS_XREG_SetNodeCount(ucIndex, 0);

        pstListHeader = CNAS_XREG_GetListHeader(ucIndex);

        /*lint -e717*/
        HI_INIT_LIST_HEAD(pstListHeader);
        /*lint +e717*/
    }

    /* ��ʼ��ZoneList */
    NAS_MEM_SET_S(g_astCnasXregZoneNode, sizeof(g_astCnasXregZoneNode), 0, sizeof(g_astCnasXregZoneNode));

    for (ucIndex = 0; ucIndex < CNAS_XREG_ZONE_NODE_MAX_NUM; ucIndex++)
    {
        msp_list_add_tail(&g_astCnasXregZoneNode[ucIndex].stListNode,
                            CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_FREE_LIST_ID));
    }

    CNAS_XREG_SetNodeCount(CNAS_XREG_ZONE_FREE_LIST_ID, CNAS_XREG_ZONE_NODE_MAX_NUM);

    /* ��ʼ��SidNidList */
    NAS_MEM_SET_S(g_astCnasXregSidNidNode, sizeof(g_astCnasXregSidNidNode), 0, sizeof(g_astCnasXregSidNidNode));

    for (ucIndex = 0; ucIndex < CNAX_XREG_SIDNID_NODE_MAX_NUM; ucIndex++)
    {
        msp_list_add_tail(&g_astCnasXregSidNidNode[ucIndex].stListNode,
                            CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_FREE_LIST_ID));
    }

    CNAS_XREG_SetNodeCount(CNAS_XREG_SIDNID_FREE_LIST_ID, CNAX_XREG_SIDNID_NODE_MAX_NUM);

    return;
}


VOS_VOID CNAS_XREG_ClearList(VOS_VOID)
{
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    HI_LIST_S                          *pstListHeader;

    /* ���ZONE_LIST�������� */
    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        CNAS_XREG_RelZoneListNode(pstIndexNode);
    }

    if (VOS_NULL_PTR != g_pstCnasXregCurrZoneNode)
    {
        CNAS_XREG_RelCurrZoneNode();
    }

    /* ���SIDNID_LIST�������� */
    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        CNAS_XREG_RelSidNidListNode(pstIndexNode);
    }

    if (VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
    {
        CNAS_XREG_RelCurrSidNidNode();
    }

    return;
}


VOS_VOID CNAS_XREG_RestoreList(VOS_VOID)
{
    NAS_MEM_SET_S(&g_stCnasXregTimeInfo, sizeof(g_stCnasXregTimeInfo), 0, sizeof(g_stCnasXregTimeInfo));

    CNAS_XREG_ClearList();

    /* ����NV���д���ZONE��Ԫ��ֵ����ǰZONE�� */
    ;

    /* ����NV���д���ZONE��Ԫ��ֵ����ǰZONE�� */
    ;

    return;
}


VOS_VOID CNAS_XREG_SaveList(VOS_VOID)
{
    /* ����NV���д���ZONE��Ԫ��ֵ����ǰZONE�� */
    ;


    /* ����NV���д���ZONE��Ԫ��ֵ����ǰZONE�� */
    ;

    return;
}


VOS_VOID CNAS_XREG_AddZoneNode(CNAS_XREG_ZONE_NODE_STRU *pstZoneNode)
{
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_ZONE_NODE_STRU           *pstIndexZoneNode;

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexZoneNode = msp_list_entry(pstIndexNode, CNAS_XREG_ZONE_NODE_STRU, stListNode);

        /* �ҵ������в���λ�� */
        if (pstIndexZoneNode->ulSecNum > pstZoneNode->ulSecNum)
        {
            break;
        }
    }

    __list_add_msp(&(pstZoneNode->stListNode), pstIndexNode->prev, pstIndexNode);

    CNAS_XREG_IncNodeCount(CNAS_XREG_ZONE_USED_LIST_ID);

    return;
}


VOS_VOID CNAS_XREG_AddCurrZoneNode(VOS_UINT16 usSid, VOS_UINT16 usNid,
                                        VOS_UINT16 usRegZone, VOS_UINT8 ucZoneTimer)
{
    if (VOS_NULL_PTR == g_pstCnasXregCurrZoneNode)
    {
        return;
    }

    /* ������� */
    if (ucZoneTimer >= (sizeof(g_ausCnasXregZoneTimerMap)/sizeof(VOS_UINT16)))
    {
        CNAS_ERROR_LOG1(UEPS_PID_XREG, "CNAS_XREG_AddCurrZoneNode: ucZoneTimer is not correct.", ucZoneTimer);

        return;
    }

    if ((usSid != g_pstCnasXregCurrZoneNode->usSid)
        || ((usNid != g_pstCnasXregCurrZoneNode->usNid)
            && (usNid != CNAS_XREG_HOME_NID))
        || (usRegZone != g_pstCnasXregCurrZoneNode->usRegZone))
    {
        g_pstCnasXregCurrZoneNode->ulSecNum = g_stCnasXregTimeInfo.ulSecNum + g_ausCnasXregZoneTimerMap[ucZoneTimer];

        CNAS_XREG_AddZoneNode(g_pstCnasXregCurrZoneNode);

        g_pstCnasXregCurrZoneNode = VOS_NULL_PTR;
    }

    return;
}


VOS_VOID CNAS_XREG_AddSidNidNode(CNAS_XREG_SIDNID_NODE_STRU *pstSidNidNode)
{
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_SIDNID_NODE_STRU         *pstIndexSidNidNode;

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        /* �ҵ������в���λ�� */
        if (pstIndexSidNidNode->ulSecNum > pstSidNidNode->ulSecNum)
        {
            break;
        }
    }

    __list_add_msp(&(pstSidNidNode->stListNode), pstIndexNode->prev, pstIndexNode);

    CNAS_XREG_IncNodeCount(CNAS_XREG_SIDNID_USED_LIST_ID);

    return;
}


VOS_VOID CNAS_XREG_AddCurrSidNidNode(VOS_UINT16 usSid, VOS_UINT16 usNid,
                                                    VOS_UINT8 ucZoneTimer)
{
    if (VOS_NULL_PTR == g_pstCnasXregCurrSidNidNode)
    {
        return;
    }

    /* ������� */
    if (ucZoneTimer >= (sizeof(g_ausCnasXregZoneTimerMap)/sizeof(VOS_UINT16)))
    {
        CNAS_ERROR_LOG1(UEPS_PID_XREG, "CNAS_XREG_AddCurrSidNidNode: ucZoneTimer is not correct.", ucZoneTimer);

        return;
    }

    if ((usSid != g_pstCnasXregCurrSidNidNode->usSid)
        || ((usNid != g_pstCnasXregCurrSidNidNode->usNid)
            && (usNid != CNAS_XREG_HOME_NID)))
    {
        g_pstCnasXregCurrSidNidNode->ulSecNum = g_stCnasXregTimeInfo.ulSecNum + g_ausCnasXregZoneTimerMap[ucZoneTimer];

        CNAS_XREG_AddSidNidNode(g_pstCnasXregCurrSidNidNode);

        g_pstCnasXregCurrSidNidNode = VOS_NULL_PTR;
    }

    return;
}


VOS_VOID CNAS_XREG_RefreshLists(VOS_VOID)
{
    VOS_UINT32                          ulCurrSlice;
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_ZONE_NODE_STRU           *pstIndexZoneNode;
    CNAS_XREG_SIDNID_NODE_STRU         *pstIndexSidNidNode;

    /* ����ǰ��SIDNIDLIST��ZONELIST��ά�ɲ���� */
    CNAS_XREG_ZoneListMntn();
    CNAS_XREG_SidNidListMntn();
    CNAS_XREG_SndOmRegListReportInd();

    ulCurrSlice = CNAS_XREG_GetSlice();

    /* ��¼��ǰ�ϵ���ʱ�䣬��λΪ�� */
    g_stCnasXregTimeInfo.ulSecNum   += (ulCurrSlice - g_stCnasXregTimeInfo.ulPreSlice)/VOS_GetSliceUnit();

    g_stCnasXregTimeInfo.ulPreSlice = ulCurrSlice;

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexZoneNode = msp_list_entry(pstIndexNode, CNAS_XREG_ZONE_NODE_STRU, stListNode);

        /* �жϵ�ǰ�ڵ��Ƿ��Ѿ���ʱ */
        if (pstIndexZoneNode->ulSecNum <= g_stCnasXregTimeInfo.ulSecNum)
        {
            CNAS_XREG_RelZoneListNode(pstIndexNode);
        }
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        /* �жϵ�ǰ�ڵ��Ƿ��Ѿ���ʱ */
        if (pstIndexSidNidNode->ulSecNum <= g_stCnasXregTimeInfo.ulSecNum)
        {
            CNAS_XREG_RelSidNidListNode(pstIndexNode);
        }
    }

    /* ����LIST������ʱ����ȷ��1Сʱ��LIST��ˢ�� */
    CNAS_XREG_StartTime(TI_CNAS_XREG_TIMER_ZONELIST_PT, TI_CNAS_XREG_TIMER_ZONELIST_PT_LEN);

    return;
}


VOS_VOID CNAS_XREG_TotalZones(VOS_UINT8 ucRegZoneNum)
{
    /*
    1��If TOTAL_ZONESs is equal to zero, the mobile station shall delete all entries.
    2��If TOTAL_ZONESs is not equal to zero, the mobile station shall delete those entries
       having active zone list entry timers, starting with the oldest entry, as determined by
       the timer values, and continuing in order of decreasing age until no more than TOTAL_ZONESs entries remain.
    */

    VOS_UINT8                           ucDelNum = 0;
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;

    /* ������Ҫɾ���ĸ��� */
    if (ucRegZoneNum <= CNAS_XREG_GetNodeCount(CNAS_XREG_ZONE_USED_LIST_ID))
    {
        ucDelNum = CNAS_XREG_GetNodeCount(CNAS_XREG_ZONE_USED_LIST_ID) - ucRegZoneNum;

        if (VOS_NULL_PTR != g_pstCnasXregCurrZoneNode)
        {
            ucDelNum++;
        }
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_USED_LIST_ID);

    /* �������ZONELISTɾ�� */
    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        if (0 == ucDelNum)
        {
            break;
        }

        CNAS_XREG_RelZoneListNode(pstIndexNode);

        ucDelNum--;
    }

    /* ���绹ʣ��һ������ѵ�ǰ��ZONEɾ�� */
    if (1 == ucDelNum)
    {
        CNAS_XREG_RelCurrZoneNode();
    }

    return;
}


VOS_VOID CNAS_XREG_DelNodesByBandBlkSys(XSD_XREG_SYS_INFO_STRU *pstSysInfo)
{
    VOS_UINT16                          usBandClass;
    CNAS_XREG_BLKSYS_ENUM_UINT8         enBlkSys;
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_ZONE_NODE_STRU           *pstIndexZoneNode;
    CNAS_XREG_SIDNID_NODE_STRU         *pstIndexSidNidNode;

    usBandClass = pstSysInfo->usBandClass;
    enBlkSys    = CNAS_XREG_BandFreqToBlkSys(pstSysInfo->usBandClass, pstSysInfo->usFreq);

    /* ZONE_LIST */
    if ((VOS_NULL_PTR != g_pstCnasXregCurrZoneNode)
        && ((usBandClass != g_pstCnasXregCurrZoneNode->usBandClass)
            || (enBlkSys != g_pstCnasXregCurrZoneNode->enBlkSys)))
    {
        CNAS_XREG_RelCurrZoneNode();
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexZoneNode = msp_list_entry(pstIndexNode, CNAS_XREG_ZONE_NODE_STRU, stListNode);

        if ((pstIndexZoneNode->usBandClass != usBandClass)
            ||(pstIndexZoneNode->enBlkSys != enBlkSys))
        {
            CNAS_XREG_RelZoneListNode(pstIndexNode);
        }
    }

    /* SIDNID_LIST */
    if ((VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
        && ((usBandClass != g_pstCnasXregCurrSidNidNode->usBandClass)
            || (enBlkSys != g_pstCnasXregCurrSidNidNode->enBlkSys)))
    {
        CNAS_XREG_RelCurrSidNidNode();
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        if ((pstIndexSidNidNode->usBandClass != usBandClass)
            ||(pstIndexSidNidNode->enBlkSys != enBlkSys))
        {
            CNAS_XREG_RelSidNidListNode(pstIndexNode);
        }
    }

    return;
}


CNAS_XREG_ZONE_NODE_STRU *CNAS_XREG_FindZoneNode(VOS_UINT16 usSid, VOS_UINT16 usNid, VOS_UINT16 usRegZone)
{
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_ZONE_NODE_STRU           *pstIndexZoneNode;

    if (VOS_NULL_PTR != g_pstCnasXregCurrZoneNode)
    {
        if ((usSid == g_pstCnasXregCurrZoneNode->usSid)
            && ((CNAS_XREG_HOME_NID == usNid)
                || (usNid == g_pstCnasXregCurrZoneNode->usNid))
            && (usRegZone == g_pstCnasXregCurrZoneNode->usRegZone))
        {
            return g_pstCnasXregCurrZoneNode;
        }
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexZoneNode = msp_list_entry(pstIndexNode, CNAS_XREG_ZONE_NODE_STRU, stListNode);

        if ((usSid == pstIndexZoneNode->usSid)
            && ((CNAS_XREG_HOME_NID == usNid)
                || (usNid == pstIndexZoneNode->usNid))
            && (usRegZone == pstIndexZoneNode->usRegZone))
        {
            return pstIndexZoneNode;
        }
    }

    return VOS_NULL_PTR;
}


CNAS_XREG_SIDNID_NODE_STRU *CNAS_XREG_FindSidNidNode(VOS_UINT16 usSid, VOS_UINT16 usNid)
{
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_SIDNID_NODE_STRU         *pstIndexSidNidNode;

    if (VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
    {
        if ((usSid == g_pstCnasXregCurrSidNidNode->usSid)
            && ((CNAS_XREG_HOME_NID == usNid)
                || (usNid == g_pstCnasXregCurrSidNidNode->usNid)))
        {
            return g_pstCnasXregCurrSidNidNode;
        }
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        if ((usSid == pstIndexSidNidNode->usSid)
            && ((CNAS_XREG_HOME_NID == usNid)
                || (usNid == pstIndexSidNidNode->usNid)))
        {
            return pstIndexSidNidNode;
        }
    }

    return VOS_NULL_PTR;
}


VOS_VOID CNAS_XREG_UpdateZoneList(XSD_XREG_SYS_INFO_STRU *pstSysInfo, VOS_UINT16 usSid, VOS_UINT16 usNid,
                                    VOS_UINT16 usRegZone, VOS_UINT8 ucZoneTimer)
{
    CNAS_XREG_ZONE_NODE_STRU           *pstZoneNode;
    HI_LIST_S                          *pstIndexNode;

    /* ������� */
    if (ucZoneTimer >= (sizeof(g_ausCnasXregZoneTimerMap)/sizeof(VOS_UINT16)))
    {
        CNAS_ERROR_LOG1(UEPS_PID_XREG, "CNAS_XREG_UpdateZoneList: ucZoneTimer is not correct.", ucZoneTimer);

        return;
    }

    pstZoneNode = CNAS_XREG_FindZoneNode(usSid, usNid, usRegZone);

    /* ����û�ҵ���Ӧ��ZONE_LIST */
    if (VOS_NULL_PTR == pstZoneNode)
    {
        if (VOS_NULL_PTR != g_pstCnasXregCurrZoneNode)
        {
            g_pstCnasXregCurrZoneNode->ulSecNum = g_stCnasXregTimeInfo.ulSecNum + g_ausCnasXregZoneTimerMap[ucZoneTimer];

            CNAS_XREG_AddZoneNode(g_pstCnasXregCurrZoneNode);
        }

        pstIndexNode = CNAS_XREG_AllocListNode(CNAS_XREG_ZONE_FREE_LIST_ID);

        if (VOS_NULL_PTR == pstIndexNode)
        {
            /* ������ʣ��ڵ㣬���ɾ�����Ͻڵ� */
            pstIndexNode = CNAS_XREG_AllocListNode(CNAS_XREG_ZONE_USED_LIST_ID);
        }

        g_pstCnasXregCurrZoneNode = msp_list_entry(pstIndexNode, CNAS_XREG_ZONE_NODE_STRU, stListNode);

        g_pstCnasXregCurrZoneNode->ulSecNum     = 0;
        g_pstCnasXregCurrZoneNode->usSid = usSid;
        g_pstCnasXregCurrZoneNode->usNid = usNid;
        g_pstCnasXregCurrZoneNode->usRegZone    = usRegZone;
        g_pstCnasXregCurrZoneNode->usBandClass  = pstSysInfo->usBandClass;
        g_pstCnasXregCurrZoneNode->enBlkSys     = CNAS_XREG_BandFreqToBlkSys(pstSysInfo->usBandClass, pstSysInfo->usFreq);;

        return;
    }

    /* �ҵ���Ӧ��ZONE_LIST�����Һ͵�ǰ��ZONE_LIST��ͬ */
    if (pstZoneNode != g_pstCnasXregCurrZoneNode)
    {
        /* ɾ��ԭ�нڵ� */
        msp_list_del(&(pstZoneNode->stListNode));

        CNAS_XREG_DecNodeCount(CNAS_XREG_ZONE_USED_LIST_ID);

        /* ����ǰZONE���뵽ZONE_LIST�� */
        if (VOS_NULL_PTR != g_pstCnasXregCurrZoneNode)
        {
            g_pstCnasXregCurrZoneNode->ulSecNum = g_stCnasXregTimeInfo.ulSecNum + g_ausCnasXregZoneTimerMap[ucZoneTimer];

            CNAS_XREG_AddZoneNode(g_pstCnasXregCurrZoneNode);
        }

        g_pstCnasXregCurrZoneNode = pstZoneNode;

        g_pstCnasXregCurrZoneNode->ulSecNum     = 0;
        g_pstCnasXregCurrZoneNode->usSid = usSid;
        g_pstCnasXregCurrZoneNode->usNid = usNid;
        g_pstCnasXregCurrZoneNode->usRegZone    = usRegZone;
        g_pstCnasXregCurrZoneNode->usBandClass  = pstSysInfo->usBandClass;
        g_pstCnasXregCurrZoneNode->enBlkSys     = CNAS_XREG_BandFreqToBlkSys(pstSysInfo->usBandClass, pstSysInfo->usFreq);;
    }

    return;
}


VOS_VOID CNAS_XREG_UpdateSidNidList(XSD_XREG_SYS_INFO_STRU *pstSysInfo, VOS_UINT16 usSid,
                                                VOS_UINT16 usNid, VOS_UINT8 ucZoneTimer)
{
    CNAS_XREG_SIDNID_NODE_STRU         *pstSidNidNode;
    HI_LIST_S                          *pstIndexNode;

    /* ������� */
    if (ucZoneTimer >= (sizeof(g_ausCnasXregZoneTimerMap)/sizeof(VOS_UINT16)))
    {
        CNAS_ERROR_LOG1(UEPS_PID_XREG, "CNAS_XREG_UpdateSidNidList: ucZoneTimer is not correct.", ucZoneTimer);

        return;
    }

    pstSidNidNode = CNAS_XREG_FindSidNidNode(usSid, usNid);

    /* ����û�ҵ���Ӧ��SIDNID_LIST */
    if (VOS_NULL_PTR == pstSidNidNode)
    {
        if (VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
        {
            g_pstCnasXregCurrSidNidNode->ulSecNum = g_stCnasXregTimeInfo.ulSecNum + g_ausCnasXregZoneTimerMap[ucZoneTimer];

            CNAS_XREG_AddSidNidNode(g_pstCnasXregCurrSidNidNode);
        }

        pstIndexNode = CNAS_XREG_AllocListNode(CNAS_XREG_SIDNID_FREE_LIST_ID);

        if (VOS_NULL_PTR == pstIndexNode)
        {
            /* ������ʣ��ڵ㣬���ɾ�����Ͻڵ� */
            pstIndexNode = CNAS_XREG_AllocListNode(CNAS_XREG_SIDNID_USED_LIST_ID);
        }

        g_pstCnasXregCurrSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        g_pstCnasXregCurrSidNidNode->ulSecNum     = 0;
        g_pstCnasXregCurrSidNidNode->usSid = usSid;
        g_pstCnasXregCurrSidNidNode->usNid = usNid;
        g_pstCnasXregCurrSidNidNode->usBandClass = pstSysInfo->usBandClass;
        g_pstCnasXregCurrSidNidNode->enBlkSys    = CNAS_XREG_BandFreqToBlkSys(pstSysInfo->usBandClass, pstSysInfo->usFreq);;

        return;
    }

    /* �ҵ���Ӧ��SIDNID_LIST�����Һ͵�ǰ��SIDNID_LIST��ͬ */
    if (pstSidNidNode != g_pstCnasXregCurrSidNidNode)
    {
        /* ɾ��ԭ�нڵ� */
        msp_list_del(&(pstSidNidNode->stListNode));

        CNAS_XREG_DecNodeCount(CNAS_XREG_SIDNID_USED_LIST_ID);

        /* ����ǰZONE���뵽ZONE_LIST�� */
        if (VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
        {
            g_pstCnasXregCurrSidNidNode->ulSecNum = g_stCnasXregTimeInfo.ulSecNum + g_ausCnasXregZoneTimerMap[ucZoneTimer];

            CNAS_XREG_AddSidNidNode(g_pstCnasXregCurrSidNidNode);
        }

        g_pstCnasXregCurrSidNidNode = pstSidNidNode;

        g_pstCnasXregCurrSidNidNode->ulSecNum     = 0;
        g_pstCnasXregCurrSidNidNode->usSid = usSid;
        g_pstCnasXregCurrSidNidNode->usNid = usNid;
        g_pstCnasXregCurrSidNidNode->usBandClass    = pstSysInfo->usBandClass;
        g_pstCnasXregCurrSidNidNode->enBlkSys       = CNAS_XREG_BandFreqToBlkSys(pstSysInfo->usBandClass, pstSysInfo->usFreq);;
    }

    return;
}


VOS_VOID CNAS_XREG_DelMultSids(VOS_VOID)
{
    /*
    Whenever MULT_SIDSs is equal to '0' and SID_NID_LIST contains entries with different
    SIDs, the mobile station shall delete the excess entries according to the following rules:
      If the SID/NID entry timer for any entry is disabled, the mobile station shall delete
       all entries not having the same SID as the entry whose timer is disabled;
      Otherwise, the mobile station shall delete all entries not having the same SID as the
       newest entry in SID_NID_LIST, as determined by the timer values.
    */
    VOS_UINT16                          usSid;
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_SIDNID_NODE_STRU         *pstSidNidNode;

    if (VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
    {
        usSid = g_pstCnasXregCurrSidNidNode->usSid;
    }
    else
    {
        if (0 == CNAS_XREG_GetNodeCount(CNAS_XREG_SIDNID_USED_LIST_ID))
        {
            return;
        }

        /* ȡ���½ڵ��SID��Ϊ�Ƚ�ֵ */
        pstIndexNode  = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID)->prev;
        pstSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        usSid         = pstSidNidNode->usSid;
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

    /* ����SIDNID_LIST��������ͬSID�ڵ�ɾ�� */
    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        if (usSid != pstSidNidNode->usSid)
        {
            CNAS_XREG_RelSidNidListNode(pstIndexNode);
        }
    }

    return;
}


VOS_VOID CNAS_XREG_DelMultNids(VOS_VOID)
{
/*
    Whenever MULT_NIDSs is equal to '0' and SID_NID_LIST contains more than one entry for
    any SID, the mobile station shall delete the excess entries for each SID according to the
    following rules:
    If the SID/NID entry timer for any entry is disabled, the mobile station shall delete
    all entries for that SID except the entry whose timer is disabled;
    For all other SIDs, the mobile station shall delete all entries for each SID except the
    newest entry, as determined by the timer values.
*/
    VOS_UINT16                          usSid;
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    HI_LIST_S                          *pstTailNode;
    CNAS_XREG_SIDNID_NODE_STRU         *pstSidNidNode;

    if (VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
    {
        usSid = g_pstCnasXregCurrSidNidNode->usSid;

        pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

        /* ����SIDNID_LIST��������ͬSID�ڵ�ɾ�� */
        msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
        {
            pstSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

            if (usSid == pstSidNidNode->usSid)
            {
                CNAS_XREG_RelSidNidListNode(pstIndexNode);
            }
        }
    }

    /* ָ����β�Ľڵ� */
    pstTailNode = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID)->prev;

    while (pstTailNode != CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID))
    {
        pstSidNidNode = msp_list_entry(pstTailNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);
        usSid         = pstSidNidNode->usSid;

        pstIndexNode = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID)->next;
        pstTempNode  = pstIndexNode->next;

        for (; pstIndexNode != pstTailNode; pstIndexNode = pstTempNode, pstTempNode = pstIndexNode->next)
        {
            pstSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

            if (usSid == pstSidNidNode->usSid)
            {
                CNAS_XREG_RelSidNidListNode(pstIndexNode);
            }
        }

        /* ���һ�ֱ�������Ҫ��ǰ��һ�� */
        pstTailNode = pstTailNode->prev;
    }

    return;
}


VOS_UINT32 CNAS_XREG_SidNidListIsEmpty(VOS_VOID)
{
    if (VOS_NULL_PTR == g_pstCnasXregCurrSidNidNode)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 CNAS_XREG_GetSlice(VOS_VOID)
{
    return VOS_GetSlice();
}


VOS_VOID CNAS_XREG_SidNidListMntn(VOS_VOID)
{
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_SIDNID_NODE_STRU         *pstIndexSidNidNode;
    CNAS_XREG_SIDNID_LIST_MNTN_STRU    *pstSidNidListMntnMsg;
    VOS_UINT32                          ulIndex = 0;

    pstSidNidListMntnMsg = (CNAS_XREG_SIDNID_LIST_MNTN_STRU*)VOS_AllocMsg(UEPS_PID_XREG,
                                                                          sizeof(CNAS_XREG_SIDNID_LIST_MNTN_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstSidNidListMntnMsg)
    {
        return;
    }

    pstSidNidListMntnMsg->ulReceiverPid   = UEPS_PID_XREG;
    pstSidNidListMntnMsg->enMsgId         = CNAS_XREG_MNTN_SIDNID_LIST;

    /* ���ʱ����Ϣ */
    NAS_MEM_CPY_S(&(pstSidNidListMntnMsg->stTimeInfo),
                  sizeof(CNAS_XREG_TIME_INFO_STRU),
                  &g_stCnasXregTimeInfo,
                  sizeof(CNAS_XREG_TIME_INFO_STRU));

    NAS_MEM_SET_S(pstSidNidListMntnMsg->astSidNidNode,
                  sizeof(pstSidNidListMntnMsg->astSidNidNode),
                  0,
                  sizeof(pstSidNidListMntnMsg->astSidNidNode));

    if (VOS_NULL_PTR != g_pstCnasXregCurrSidNidNode)
    {
        NAS_MEM_CPY_S(&(pstSidNidListMntnMsg->astSidNidNode[ulIndex]),
                      sizeof(CNAS_XREG_SIDNID_NODE_STRU),
                      g_pstCnasXregCurrSidNidNode,
                      sizeof(CNAS_XREG_SIDNID_NODE_STRU));
        ulIndex++;
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_SIDNID_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexSidNidNode = msp_list_entry(pstIndexNode, CNAS_XREG_SIDNID_NODE_STRU, stListNode);

        NAS_MEM_CPY_S(&(pstSidNidListMntnMsg->astSidNidNode[ulIndex]),
                      sizeof(CNAS_XREG_SIDNID_NODE_STRU),
                      pstIndexSidNidNode,
                      sizeof(CNAS_XREG_SIDNID_NODE_STRU));
    }

    DIAG_TraceReport((VOS_VOID *)(pstSidNidListMntnMsg));

    VOS_FreeMsg(UEPS_PID_XREG, pstSidNidListMntnMsg);

    return;
}


VOS_VOID CNAS_XREG_ZoneListMntn(VOS_VOID)
{
    HI_LIST_S                          *pstListHeader;
    HI_LIST_S                          *pstIndexNode;
    HI_LIST_S                          *pstTempNode;
    CNAS_XREG_ZONE_NODE_STRU           *pstIndexZoneNode;
    CNAS_XREG_ZONE_LIST_MNTN_STRU      *pstZoneListMntnMsg;
    VOS_UINT32                          ulIndex = 0;

    pstZoneListMntnMsg = (CNAS_XREG_ZONE_LIST_MNTN_STRU*)VOS_AllocMsg(UEPS_PID_XREG,
                                                                      sizeof(CNAS_XREG_ZONE_LIST_MNTN_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstZoneListMntnMsg)
    {
        return;
    }

    pstZoneListMntnMsg->ulReceiverPid   = UEPS_PID_XREG;
    pstZoneListMntnMsg->enMsgId         = CNAS_XREG_MNTN_ZONE_LIST;

    pstZoneListMntnMsg->ulCurSlice = VOS_GetSlice();
    NAS_MEM_SET_S(pstZoneListMntnMsg->astZoneNode,
                  sizeof(pstZoneListMntnMsg->astZoneNode),
                  0,
                  sizeof(pstZoneListMntnMsg->astZoneNode));

    if (VOS_NULL_PTR != g_pstCnasXregCurrZoneNode)
    {
        NAS_MEM_CPY_S(&(pstZoneListMntnMsg->astZoneNode[ulIndex]),
                      sizeof(CNAS_XREG_ZONE_NODE_STRU),
                      g_pstCnasXregCurrZoneNode,
                      sizeof(CNAS_XREG_ZONE_NODE_STRU));
        ulIndex++;
    }

    pstListHeader = CNAS_XREG_GetListHeader(CNAS_XREG_ZONE_USED_LIST_ID);

    msp_list_for_each_safe(pstIndexNode, pstTempNode, pstListHeader)
    {
        pstIndexZoneNode = msp_list_entry(pstIndexNode, CNAS_XREG_ZONE_NODE_STRU, stListNode);

        NAS_MEM_CPY_S(&(pstZoneListMntnMsg->astZoneNode[ulIndex]),
                      sizeof(CNAS_XREG_ZONE_NODE_STRU),
                      pstIndexZoneNode,
                      sizeof(CNAS_XREG_ZONE_NODE_STRU));
        ulIndex++;
    }

    DIAG_TraceReport((VOS_VOID *)(pstZoneListMntnMsg));

    VOS_FreeMsg(UEPS_PID_XREG, pstZoneListMntnMsg);

    return;
}

/*lint -restore*/

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */



