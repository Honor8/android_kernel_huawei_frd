#ifndef __MDRV_CCORE_DSPDVFS_H__
#define __MDRV_CCORE_DSPDVFS_H__
#ifdef __cplusplus
extern "C"
{
#endif

enum Dspdvs_pmu_type
{
    DSPDVS_BBP = 1,
    DSPDVS_DSP,
    DSPDVS_BUTT
};

enum Dspdfs_DSP_Core_ID
{
    TLDSP = 1,     /*tlbbe16*/
    CDSP,         /*cbbe16*/
    DSP_BUTT
};

/*****************************************************************************
* �� �� ��  : mdrv_dspdfs_qos_request
*
* ��������  : ע��dspdfs_qos
*
* �������  : u32 core_id - DSP_core ID
* �������  : u32 *req_id - ����ID
*
* �� �� ֵ  : 0���ɹ���-1��ʧ��
*
* ����˵��  :
*
*****************************************************************************/
int mdrv_dspdfs_qos_request(unsigned int core_id, unsigned int *req_id);

/*****************************************************************************
* �� �� ��  : mdrv_dspdfs_qos_update
*
* ��������  : dsp��ƵͶƱ�ӿڣ�����DFS QoS
*
* �������  : u32 req_id - ����ID
              u32 req_value - ����Ƶ��ֵ 
* �������  : ��
*
* �� �� ֵ  : 0���ɹ���-1��ʧ��
*
* ����˵��  :
*
*****************************************************************************/
int mdrv_dspdfs_qos_update(unsigned int req_id, unsigned int req_value);

/*****************************************************************************
* �� �� ��  : mdrv_dspdfs_qos_get
*
* ��������  : ��ȡ��Ӧreq_id��ͶƱƵ��
*
* �������  : req_id - ����ID
* �������  :
*
* �� �� ֵ  : �ɹ����ض�Ӧreq_id��ͶƱƵ��
*             ʧ�ܷ��� -1
*
* ����˵��  : 
*
*****************************************************************************/
int mdrv_dspdfs_qos_get(unsigned int req_id);

/*****************************************************************************
* �� �� ��  : mdrv_dspdvs_get_bbp_voltage
*
* ��������  : ��ȡTLDSP��BBP��ѹ��ͶƱֵ
*
* �������  : ��
* �������  : �� 
*
* �� �� ֵ  : �ɹ�����TLDSP����һ�ε�BBP��ѹ����
*             ʧ�ܷ��� -1
*
* ����˵��  : 
*
*****************************************************************************/
int mdrv_dspdvs_get_bbp_voltage(void);

/*****************************************************************************
* �� �� ��  : mdrv_dspdvs_get_low_voltage_profile
*
* ��������  : ��ȡDSP/BBP��͵�ѹֵ
*
* �������  : req_type: 1-BBP 2-DSP
* �������  : �� 
*
* �� �� ֵ  : �ɹ�����DSP/BBP��͵�ѹֵ
*             ʧ�ܷ��� -1
*
* ����˵��  : 
*
*****************************************************************************/
int mdrv_dspdvs_get_low_voltage_profile(unsigned int req_type);

/*****************************************************************************
* �� �� ��  : mdrv_dspdvs_voltage_qos
*
* ��������  : ���ݵ�ѹ����ֵ����dsp����BBP���е�ѹ
*
* �������  : u32 req_type: 1-BBP 2-DSP
              u32 voltage_req:��Ҫ�ĵ�ѹ
* �������  : �� 
*
* �� �� ֵ  : ��
*
* ����˵��  : 
*
*****************************************************************************/
void mdrv_dspdvs_voltage_qos(unsigned int req_type, unsigned int voltage_req);


#ifdef __cplusplus
}
#endif
#endif

