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

#ifndef SYSTEM_UTILITIES_H_
#define SYSTEM_UTILITIES_H_

#include <stdint.h>
#include "params_hal.h"
#include "system_product.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SYSTEM_PARAMS_PRODUCT_BOARD_ID = 0,
    SYSTEM_PARAMS_PRODUCT_BOARD_NAME,
    SYSTEM_PARAMS_SECURITY_MODE,
    SYSTEM_PARAMS_PRODUCT_ID,
    SYSTEM_PARAMS_PRODUCT_SECRET,
    SYSTEM_PARAMS_PRODUCT_SOFTWARE_VERSION,
    SYSTEM_PARAMS_PRODUCT_HARDWARE_VERSION
} system_params_t;

uint16_t system_get_firmlib_version(char* buffer, uint16_t len);
product_type_t system_get_product_type(void);
int system_set_product_type(product_type_t type);
uint16_t system_get_product_software_version(char *buffer, uint16_t len);
int system_set_product_software_version(char *buffer);
uint16_t system_get_product_hardware_version(char *buffer, uint16_t len);
int system_set_product_hardware_version(char *buffer);
uint16_t system_get_product_id(char *buffer, uint16_t len);
int system_set_product_id(char *buffer);
uint16_t system_get_product_secret(char *buffer, uint16_t len);
int system_set_product_secret(char *buffer);
uint16_t system_get_board_id(char *buffer, uint16_t len);
int system_set_board_id(char *buffer);
uint16_t system_get_board_name(char *buffer, uint16_t len);
int system_set_board_name(char *buffer);

int system_get_params_int(system_params_t params_type, int &value);
int system_set_params_int(system_params_t params_type, int value);
int system_get_params_double(system_params_t params_type, double &value);
int system_set_params_double(system_params_t params_type, double value);
uint16_t system_get_params_array(system_params_t params_type, char *buffer, uint16_t len);
int system_set_params_array(system_params_t params_type, char *buffer, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_UTILITIES_H_ */

