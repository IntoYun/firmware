/**
 ******************************************************************************
 * @file     : panic.c
 * @author   : robin
 * @version  : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
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

#include <stdint.h>
#include "macros.h"
#include "panic.h"
#include "debug.h"
#include "hw_config.h"

#define LOOPSPERMSEC 5483

typedef struct
{
    uint32_t  led;
    uint16_t  count;
} flash_codes_t;


#define def_panic_codes(_class,led,code) {led, code},
static const flash_codes_t flash_codes[] = {
                 {0,0}, //NotUsedPanicCode
#include "panic_codes.h"
};
#undef def_panic_codes

/*********************************************************************************
  *Function		:
  *Description	:
  *Input		:
  *Output		:
  *Return		:
  *author		:
  *date			:
  *Others		:
**********************************************************************************/
void panic_(ePanicCode code)
{
	__disable_irq();
	// Flush any serial message to help the poor bugger debug this;
	flash_codes_t pcd = flash_codes[code];
	uint16_t c;
	int loops = 2;
	if (debug_output_)(debug_output_("!"));
	while(loops) {
		// preamble
		KICK_WDT();
	    // add led indicate
#ifdef RELEASE_BUILD
		if (--loops == 0) NVIC_SystemReset();
#endif
	}

}
