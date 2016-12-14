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

#ifndef WIRING_PRODUCT_H_
#define WIRING_PRODUCT_H_

#include "system_product.h"


struct __ApplicationProductID {
    __ApplicationProductID(char *id) {
        system_product_instance().set_product_id(id);
    }
};

struct __ApplicationProductSecret {
    __ApplicationProductSecret(char *secret) {
        system_product_instance().set_product_secret(secret);
    }
};

struct __ApplicationProductVersion {
    __ApplicationProductVersion(uint16_t version) {
        system_product_instance().set_product_firmware_version(version);
    }
};

struct __ApplicationPlatformID {
    __ApplicationPlatformID(char *id) {
        system_product_instance().set_platform_id(id);
    }
};

struct __ApplicationPlatformName {
    __ApplicationPlatformName(char *name) {
        system_product_instance().set_platform_name(name);
    }
};

struct __ApplicationProductButton {
    __ApplicationProductButton(uint16_t pin) {
        system_product_instance().set_product_button(pin);
    }
};

struct __ApplicationProductIndicator {
    __ApplicationProductIndicator(uint16_t pin, uint16_t period) {
        system_product_instance().set_product_indicator(pin, period);
    }
};
/*
#ifdef PRODUCT_ID
#undef PRODUCT_ID
#endif
*/

#define PLATFORM_ID(x)             __ApplicationPlatformID __appPlatformID(stringify(x));
#define PLATFORM_NAME(x)           __ApplicationPlatformName __appPlatformName(stringify(x));
#define PRODUCT_ID(x)              __ApplicationProductID __appProductID(stringify(x));
#define PRODUCT_SECRET(x)          __ApplicationProductSecret __appProductSecret(stringify(x));
#define PRODUCT_VERSION(x)         __ApplicationProductVersion __appProductVersion(x);
#define PRODUCT_BUTTON(x)          __ApplicationProductButton __appProductButton(x);
#define PRODUCT_INDICATOR(x,y)       __ApplicationProductIndicator __appProductIndicator(x,y);

#endif	/* __WIRING_PRODUCT_H */
