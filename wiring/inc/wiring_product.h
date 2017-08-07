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
#include "system_utilities.h"


struct __ApplicationProductID {
    __ApplicationProductID(char *id) {
        system_set_product_id(id);
    }
};

struct __ApplicationProductSecret {
    __ApplicationProductSecret(char *secret) {
        system_set_product_secret(secret);
    }
};

struct __ApplicationProductSoftwareVersion {
    __ApplicationProductSoftwareVersion(char *version) {
        system_set_product_software_version(version);
    }
};

struct __ApplicationProductHardwareVersion {
    __ApplicationProductHardwareVersion(char *version) {
        system_set_product_hardware_version(version);
    }
};

struct __ApplicationProductType {
    __ApplicationProductType(product_type_t type) {
        system_set_product_type(type);
    }
};

#define PRODUCT_ID(x)                __ApplicationProductID __appProductID((char *)stringify(x));
#define PRODUCT_SECRET(x)            __ApplicationProductSecret __appProductSecret((char *)stringify(x));
#define PRODUCT_SOFTWARE_VERSION(x)  __ApplicationProductSoftwareVersion __appProductSoftwareVersion((char *)stringify(x));
#define PRODUCT_HARDWARE_VERSION(x)  __ApplicationProductHardwareVersion __appProductHardwareVersion((char *)stringify(x));
#define PRODUCT_VERSION(x)           PRODUCT_SOFTWARE_VERSION(x);
#define PRODUCT_TYPE(x)              __ApplicationProductType __appProductType(x);

#endif /* __WIRING_PRODUCT_H */
