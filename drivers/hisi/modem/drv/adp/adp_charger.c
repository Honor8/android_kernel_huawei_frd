

#include <osl_types.h>
#include "mdrv_chg.h"

/*lint --e{715,818}*/

/*************************CHG模块START********************************/

/*****************************************************************************
 函 数 名  : mdrv_misc_get_cbc_state
 功能描述  : 返回电池状态和电量
 输入参数  :pusBcs 0:电池在供电 1:与电池连接电池未供电 2:没有与电池连接
                          pucBcl  0:电量不足或没有与电池连接
 输出参数  : pusBcs 0:电池在供电 1:与电池连接电池未供电 2:没有与电池连接
                          pucBcl  0:电量不足或没有与电池连接
 返回值：   0 操作成功
                         -1操作失败

*****************************************************************************/
int mdrv_misc_get_cbc_state(unsigned char *pusBcs,unsigned char *pucBcl)
{
    return -1;
}


/*****************************************************************************
 函 数 名  : mdrv_misc_get_battery_state
 功能描述  :获取底层电池状态信息
 输入参数  :battery_state 电量信息
 输出参数  :battery_state 电量信息
 返回值：   0 操作成功
                         -1操作失败

*****************************************************************************/
int mdrv_misc_get_battery_state(BATT_STATE_S *battery_state)
{
    return -1;
}

/*****************************************************************************
 函 数 名  : mdrv_misc_get_charging_status
 功能描述  :查询目前是否正在充电中
 输入参数  :无
 输出参数  :无
 返回值：   0 未充电
                          1 充电中
*****************************************************************************/
int mdrv_misc_get_charging_status(void)
{
    return -1;
}

/*****************************************************************************
 函 数 名  : mdrv_misc_set_charge_state
 功能描述  :使能或者禁止充电
 输入参数  :ulState      0:禁止充电
 						1:使能充电
 输出参数  :无
 返回值：    无
*****************************************************************************/
void mdrv_misc_set_charge_state(unsigned long ulState)
{

}

/*****************************************************************************
 函 数 名  : mdrv_misc_get_charge_state
 功能描述  :查询是否需要上电
 输入参数  :
 输出参数  :无
 返回值：    无
*****************************************************************************/
int mdrv_misc_get_charge_state(void)
{
    return -1;
}

/*****************************************************************************
 函 数 名  : BSP_CHG_Sply
 功能描述  :查询
 输入参数  :
 输出参数  :无
 返回值：    无
*****************************************************************************/
int mdrv_misc_sply_battery(void)
{
    return -1;
}

/*************************CHG模块 END*********************************/


