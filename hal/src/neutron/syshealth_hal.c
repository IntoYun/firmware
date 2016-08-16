#include "syshealth_hal.h"
#include "core_hal.h"


eSystemHealth HAL_Get_Sys_Health() {
    return  (eSystemHealth)HAL_Core_Read_Backup_Register(BKP_DR_02);
}

void HAL_Set_Sys_Health(eSystemHealth health) {
    static eSystemHealth cache;
    if (health>cache) {
        cache = health;
        HAL_Core_Write_Backup_Register(BKP_DR_02, health);
    }
}

