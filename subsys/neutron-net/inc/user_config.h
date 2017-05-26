#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "eboot_command.h"
#include "at_ex_command.h"
#include "upgrade.h"

//#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define DEBUG os_printf
#else
#define DEBUG
#endif

extern struct eboot_command cmd_info;

#endif
