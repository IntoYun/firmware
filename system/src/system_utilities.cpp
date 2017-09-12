/**
 ******************************************************************************
 Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#include "system_task.h"
#include <algorithm>
#include "system_utilities.h"
#include "static_assert.h"
#include "intorobot_macros.h"
#include "params_hal.h"

using std::min;

/**
 * Series 1s (5 times), 2s (5 times), 4s (5 times), 8s (5 times) then to 16s thereafter.
 * @param connection_attempts
 * @return The number of milliseconds to backoff.
 */
unsigned backoff_period(unsigned connection_attempts)
{
    if (!connection_attempts)
        return 0;
    unsigned exponent = min(4u, (connection_attempts-1)/5);
    return 1000*(1<<exponent);
}

uint16_t system_get_firmlib_version(char* buffer, uint16_t len)
{
    if(NULL != buffer) {
        strcpy(buffer, stringify(FIRMLIB_VERSION_STRING));
        return strlen(buffer);
    }
    return 0;
}

product_type_t system_get_product_type(void) {
    return system_product_instance().get_product_type();
}

int system_set_product_type(product_type_t type) {
    return system_product_instance().set_product_type(type);
}

uint16_t system_get_product_software_version(char *buffer, uint16_t len) {
    return system_product_instance().get_product_param(SYSTEM_PRODUCT_PARAM_SOFTWARE_VERSION, buffer, len);
}

int system_set_product_software_version(char *buffer) {
    return system_product_instance().set_product_param(SYSTEM_PRODUCT_PARAM_SOFTWARE_VERSION, buffer);
}

uint16_t system_get_product_hardware_version(char *buffer, uint16_t len) {
    return system_product_instance().get_product_param(SYSTEM_PRODUCT_PARAM_HARDWARE_VERSION, buffer, len);
}

int system_set_product_hardware_version(char *buffer) {
    return system_product_instance().set_product_param(SYSTEM_PRODUCT_PARAM_HARDWARE_VERSION, buffer);
}

uint16_t system_get_product_id(char *buffer, uint16_t len) {
    return system_product_instance().get_product_param(SYSTEM_PRODUCT_PARAM_ID, buffer, len);
}

int system_set_product_id(char *buffer) {
    return system_product_instance().set_product_param(SYSTEM_PRODUCT_PARAM_ID, buffer);
}

uint16_t system_get_product_secret(char *buffer, uint16_t len) {
    return system_product_instance().get_product_param(SYSTEM_PRODUCT_PARAM_SECRET, buffer, len);
}

int system_set_product_secret(char *buffer) {
    return system_product_instance().set_product_param(SYSTEM_PRODUCT_PARAM_SECRET, buffer);
}

uint16_t system_get_board_id(char *buffer, uint16_t len) {
    return system_product_instance().get_product_param(SYSTEM_PRODUCT_PARAM_BOARD_ID, buffer, len);
}

int system_set_board_id(char *buffer) {
    return system_product_instance().set_product_param(SYSTEM_PRODUCT_PARAM_BOARD_ID, buffer);
}

uint16_t system_get_board_name(char *buffer, uint16_t len) {
    return system_product_instance().get_product_param(SYSTEM_PRODUCT_PARAM_BOARD_NAME, buffer, len);
}

int system_set_board_name(char *buffer) {
    return system_product_instance().set_product_param(SYSTEM_PRODUCT_PARAM_BOARD_NAME, buffer);
}

int system_get_params_int(system_params_t params_type, int &value) {
    switch(params_type) {
        case SYSTEM_PARAMS_SECURITY_MODE:
            value = HAL_PARAMS_Get_System_at_mode();
            return 0;
            break;
        default:
            break;
    }
    return -1;
}

int system_set_params_int(system_params_t params_type, int value) {
    switch(params_type) {
        case SYSTEM_PARAMS_SECURITY_MODE:
            HAL_PARAMS_Set_System_at_mode((AT_MODE_FLAG_TypeDef)value);
            HAL_PARAMS_Save_Params();
            return 0;
            break;
        default:
            break;
    }
    return -1;
}

int system_get_params_double(system_params_t params_type, double &value) {
    return -1;
}

int system_set_params_double(system_params_t params_type, double value) {
    return -1;
}

uint16_t system_get_params_array(system_params_t params_type, char *buffer, uint16_t len) {
    switch(params_type) {
        case SYSTEM_PARAMS_PRODUCT_ID:
            return system_get_product_id(buffer, len);
            break;
        case SYSTEM_PARAMS_PRODUCT_SECRET:
            return system_get_product_secret(buffer, len);
            break;
        case SYSTEM_PARAMS_PRODUCT_SOFTWARE_VERSION:
            return system_get_product_software_version(buffer, len);
            break;
        case SYSTEM_PARAMS_PRODUCT_HARDWARE_VERSION:
            return system_get_product_hardware_version(buffer, len);
            break;
        case SYSTEM_PARAMS_PRODUCT_BOARD_ID:
            return system_get_board_id(buffer, len);
            break;
        case SYSTEM_PARAMS_PRODUCT_BOARD_NAME:
            return system_get_board_name(buffer, len);
            break;
        default:
            break;
    }
    return 0;
}

int system_set_params_array(system_params_t params_type, char *buffer, uint16_t len) {
    switch(params_type) {
        case SYSTEM_PARAMS_PRODUCT_ID:
            return system_set_product_id(buffer);
            break;
        case SYSTEM_PARAMS_PRODUCT_SECRET:
            return system_set_product_secret(buffer);
            break;
        case SYSTEM_PARAMS_PRODUCT_SOFTWARE_VERSION:
            return system_set_product_software_version(buffer);
            break;
        case SYSTEM_PARAMS_PRODUCT_HARDWARE_VERSION:
            return system_set_product_hardware_version(buffer);
            break;
        case SYSTEM_PARAMS_PRODUCT_BOARD_ID:
            return system_set_board_id(buffer);
            break;
        case SYSTEM_PARAMS_PRODUCT_BOARD_NAME:
            return system_set_board_name(buffer);
            break;
        default:
            break;
    }
    return -1;
}

