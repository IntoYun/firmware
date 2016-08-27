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

#ifndef FLASH_MAP_H_
#define FLASH_MAP_H_


#define SPI_FLASH_SEC_SIZE      4096       //Flash 扇区大小
#define LIMIT_ERASE_SIZE        0x10000    //Flash 擦除扇区大小限制

#define SUBSYS_VERSION_ADDR     0x20010    //subsys version address

/*
 *  28k  bootloader
 *  4k   指令保存区
 *  720k esp8266应用程序
 *  256k 默认stm32应用程序
 *  16k  用户参数区
 *
 *  32k  备份区
 *  720k esp8266应用程序 stm32在线编程下载缓冲区
 *  256k 默认stm32应用程序 下载缓冲区
 **
 */

#define BOOT_SEC_START                   0    //BOOT占用16个扇区
#define BOOT_PARAMS_SEC_START            16   //boot参数区
#define SYSTEM_PARAMS_SEC_START          17   //系统参数区
#define COMMAND_INFO_SEC_START           23   //指令属性区
#define EEPROM_SEC_START                 24   //eeprom模拟区
#define APP_SEC_START                    32   //应用程序运行起始扇区
#define DEFAULT_APP_SEC_START            219  //默认应用程序存放起始扇区

#define BOOT_SEC_NUM                     16   //BOOT占用16个扇区
#define BOOT_PARAMS_SEC_NUM              1    //boot参数区扇区总个数
#define SYSTEM_PARAMS_SEC_NUM            7    //系统参数区扇区总个数
#define APP_SEC_NUM                      187  //应用程序扇区总个数
#define DEFAULT_APP_SEC_NUM              89   //默认应用程序扇区总个数

#define CACHE_BOOT_SEC_START             308  //boot存放起始扇区
#define CACHE_ONLINE_APP_SEC_START       324  //在线编程 缓冲起始扇区
#define CACHE_DEFAULT_APP_SEC_START      324  //stm32默认固件 缓冲起始扇区

#define CACHE_BOOT_SEC_NUM               16   //boot程序扇区总个数
#define CACHE_ONLINE_APP_SEC_NUM         187  //在线应用程序  缓冲扇区总个数
#define CACHE_DEFAULT_APP_SEC_NUM        187  //默认应用程序 缓冲扇区总个数

#define BOOT_PARAMS_ADDR                 BOOT_PARAMS_SEC_START * SPI_FLASH_SEC_SIZE               //应用程序存放地址
#define SYSTEM_PARAMS_ADDR               SYSTEM_PARAMS_SEC_START * SPI_FLASH_SEC_SIZE               //应用程序存放地址

#define APP_ADDR                         APP_SEC_START * SPI_FLASH_SEC_SIZE               //应用程序存放地址
#define DEFAULT_APP_ADDR                 DEFAULT_APP_SEC_START * SPI_FLASH_SEC_SIZE       //默认程序 存放地址
#define CACHE_BOOT_ADDR                  CACHE_BOOT_SEC_START * SPI_FLASH_SEC_SIZE        //bootloader下载存放位置
#define CACHE_ONLINE_APP_ADDR            CACHE_ONLINE_APP_SEC_START * SPI_FLASH_SEC_SIZE  //在线编程程序下载存放地址
#define CACHE_DEFAULT_APP_ADDR           CACHE_DEFAULT_APP_SEC_START * SPI_FLASH_SEC_SIZE //默认程序下载存放地址
#define COMMAND_INFO_ADDR                COMMAND_INFO_SEC_START * SPI_FLASH_SEC_SIZE      //参数存储位置


#endif /*FLASH_MAP_H_*/


