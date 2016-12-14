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

#ifndef SYSTEM_PRODUCT_H_
#define SYSTEM_PRODUCT_H_

#include <stdint.h>
#include <string.h>
#include "intorobot_macros.h"

#ifndef PRODUCT_ID
#define PRODUCT_ID 888000
#endif

#ifndef PRODUCT_SECRET
#define PRODUCT_SECRET 888000
#endif

#ifndef PRODUCT_FIRMWARE_VERSION
#define PRODUCT_FIRMWARE_VERSION (0xffff)
#endif

#ifndef PLATFORM_ID
#define PLATFORM_ID 888000
#endif

#ifndef PLATFORM_NAME
#define PLATFORM_NAME intorobot
#endif

typedef struct {
    uint16_t product_firmware_version;     //产品程序版本号
    char product_id[36];                   //产品ID
    char product_secret[36];               //产品密钥
    char platform_id[16];                  //模组编号
    char platform_name[36];                //模组名称
    bool product_button_pressed;           //产品按键是否按下
    uint16_t product_button_pin;           //产品按键GPIO口
    uint16_t product_button_pressed_time;  //产品按下时间
    uint16_t product_indicator_pin;        //产品指示灯GPIO口
    uint16_t product_indicator_period;     //产品指示灯周期
} product_details_t;


class IntoRobotProduct
{
public:
    IntoRobotProduct();
    ~IntoRobotProduct();

    void set_product_id(char *product_id)
    {
        uint16_t templen;

        if (product_id!=NULL) {
            templen = MIN(sizeof(this->product_id)-1, strlen(product_id));
            memset(this->product_id, 0, sizeof(this->product_id));
            memcpy(this->product_id, product_id, templen);
        }
    }

    void set_product_secret(char *product_secret)
    {
        uint16_t templen;

        if (product_secret!=NULL) {
            templen = MIN(sizeof(this->product_secret)-1, strlen(product_secret));
            memset(this->product_secret, 0, sizeof(this->product_secret));
            memcpy(this->product_secret, product_secret, templen);
        }
    }

    void set_product_firmware_version(uint16_t product_firmware_version)
    {
        this->product_firmware_version = product_firmware_version;
    }

    void set_platform_id(char *platform_id)
    {
        uint16_t templen;

        if (platform_id!=NULL) {
            templen = MIN(sizeof(this->platform_id)-1, strlen(platform_id));
            memset(this->platform_id, 0, sizeof(this->platform_id));
            memcpy(this->platform_id, platform_id, templen);
        }
    }

    void set_platform_name(char *platform_name)
    {
        uint16_t templen;

        if (platform_name!=NULL) {
            templen = MIN(sizeof(this->platform_name)-1, strlen(platform_name));
            memset(this->platform_name, 0, sizeof(this->platform_name));
            memcpy(this->platform_name, platform_name, templen);
        }
    }

    void set_product_button(uint16_t pin)
    {
        this->product_button_pin = pin;
    }

    void set_product_indicator(uint16_t pin, uint16_t period)
    {
        this->product_indicator_pin = pin;
        this->product_indicator_period = period;
    }

    void get_product_details(product_details_t& details)
    {
        details.product_firmware_version = this->product_firmware_version;
        strcpy(details.product_id, this->product_id);
        strcpy(details.product_secret, this->product_secret);

        strcpy(details.platform_id, this->platform_id);
        strcpy(details.platform_name, this->platform_name);

        details.product_button_pressed = this->product_button_pressed;
        details.product_button_pin = this->product_button_pin;
        details.product_button_pressed_time = this->product_button_pressed_time;

        details.product_indicator_pin = this->product_indicator_pin;
        details.product_indicator_period = this->product_indicator_period;
    }

private:
    uint16_t product_firmware_version;     //产品程序版本号
    char product_id[36];                   //产品ID
    char product_secret[36];               //产品密钥

    char platform_id[16];                  //模组编号
    char platform_name[36];                //模组名称

    bool product_button_pressed;           //产品按键是否按下
    uint16_t product_button_pin;           //产品按键GPIO口
    uint16_t product_button_pressed_time;  //产品按下时间

    uint16_t product_indicator_pin;        //产品指示灯GPIO口
    uint16_t product_indicator_period;     //产品指示灯周期

};

extern IntoRobotProduct &system_product_instance(void);

#endif

