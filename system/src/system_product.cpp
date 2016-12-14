/**
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include "system_product.h"

IntoRobotProduct::IntoRobotProduct()
{
    product_firmware_version = PRODUCT_FIRMWARE_VERSION;     //产品程序版本号
    strcpy(product_id, stringify(PRODUCT_ID));               //产品ID
    strcpy(product_secret, stringify(PRODUCT_SECRET));       //产品密钥

    strcpy(platform_id, stringify(PLATFORM_ID));             //模组编号
    strcpy(platform_name, stringify(PLATFORM_NAME));         //模组名称

    product_button_pressed = false;   //产品按键是否按下
    product_button_pin = 0xffff;      //产品按键GPIO口
    product_button_pressed_time = 0;  //产品按下时间

    product_indicator_pin = 0xffff;   //产品指示灯GPIO口
    product_indicator_period = 0;     //产品指示灯周期
}

IntoRobotProduct::~IntoRobotProduct()
{

}

IntoRobotProduct &system_product_instance(void)
{
    static IntoRobotProduct intorobot_product;
    return intorobot_product;
}

