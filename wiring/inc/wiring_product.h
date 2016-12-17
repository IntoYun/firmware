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

struct __ApplicationProductMode {
    __ApplicationProductMode(product_mode_t mode) {
        system_product_instance().set_product_mode(mode);
    }
};

#define PRODUCT_ID(x)              __ApplicationProductID __appProductID(stringify(x));
#define PRODUCT_SECRET(x)          __ApplicationProductSecret __appProductSecret(stringify(x));
#define PRODUCT_VERSION(x)         __ApplicationProductVersion __appProductVersion(x);
#define PRODUCT_MODE(x)              __ApplicationProductMode __appProductMode(x);

#endif /* __WIRING_PRODUCT_H */
