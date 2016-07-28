#include "syshealth_hal.h"
#include "core_hal.h"


eSystemHealth HAL_Get_Sys_Health() {
    return  HAL_Core_Read_Backup_Register(BKP_DR_01);
    //return 0;
}

void HAL_Set_Sys_Health(eSystemHealth health) {
#if 0
    static eSystemHealth cache;
    if (health>cache) {
        cache = health;
        HAL_Core_Write_Backup_Register(BKP_DR_10, health);
    }
#endif
}

