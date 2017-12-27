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


#define SPI_FLASH_SEC_SIZE      0x1000     //Flash 扇区大小
#define LIMIT_ERASE_SIZE        0x10000    //Flash 擦除扇区大小限制


/*
 *  4k    空闲
 *  28k   bootloader区
 *  4     partition tables区
 *  20k   nvs区
 *  8k    空闲
 *  4k    mac暂存区
 *  4k    bootloader参数区
 *  16k   用户系统参数区
 *  32k   eeprom模拟区
 *  8k    空闲
 *  1024k 应用程序区
 *  996k  默认应用程序区
 *  28k   bootloader备份区
 *  1024k 应用下载缓冲区
 *  896k  空闲
 **
 */

#define BOOT_SEC_START                   1    //BOOT占用总扇区
#define MAC_SEC_START                    16   //mac地址暂存扇区
#define BOOT_PARAMS_SEC_START            17   //boot参数区
#define SYSTEM_PARAMS_SEC_START          18   //用户系统参数区
#define EEPROM_SEC_START                 22   //eeprom模拟区
#define APP_SEC_START                    32   //应用程序运行起始扇区
#define DEFAULT_APP_SEC_START            288  //默认应用程序存放起始扇区

#define BOOT_SEC_NUM                     7    //BOOT占用总扇区
#define MAC_SEC_NUM                      1    //mac 地址暂存扇区个数
#define BOOT_PARAMS_SEC_NUM              1    //boot参数区扇区总个数
#define SYSTEM_PARAMS_SEC_NUM            4    //用户系统参数区扇区总个数
#define APP_SEC_NUM                      256  //应用程序扇区总个数
#define DEFAULT_APP_SEC_NUM              249  //默认应用程序扇区总个数

#define CACHE_BOOT_SEC_START             537  //boot存放起始扇区
#define CACHE_ONLINE_APP_SEC_START       544  //在线编程 缓冲起始扇区
#define CACHE_DEFAULT_APP_SEC_START      544  //默认固件 缓冲起始扇区

#define CACHE_BOOT_SEC_NUM               7    //boot程序扇区总个数
#define CACHE_ONLINE_APP_SEC_NUM         256  //在线应用程序  缓冲扇区总个数
#define CACHE_DEFAULT_APP_SEC_NUM        256  //默认应用程序 缓冲扇区总个数

#define FLASH_MAC_START_ADDR             MAC_SEC_START * SPI_FLASH_SEC_SIZE      //boot参数起始地址
#define FLASH_MAC_END_ADDR               ((MAC_SEC_START + MAC_SEC_NUM) * SPI_FLASH_SEC_SIZE - 1)         //boot参数结束地址
#define BOOT_PARAMS_START_ADDR           BOOT_PARAMS_SEC_START * SPI_FLASH_SEC_SIZE                                       //boot参数起始地址
#define BOOT_PARAMS_END_ADDR             ((BOOT_PARAMS_SEC_START + BOOT_PARAMS_SEC_NUM) * SPI_FLASH_SEC_SIZE - 1)         //boot参数结束地址
#define SYSTEM_PARAMS_START_ADDR         SYSTEM_PARAMS_SEC_START * SPI_FLASH_SEC_SIZE                                     //应用参数起始地址
#define SYSTEM_PARAMS_END_ADDR           ((SYSTEM_PARAMS_SEC_START + SYSTEM_PARAMS_SEC_NUM) * SPI_FLASH_SEC_SIZE - 1)     //应用参数结束地址

#define BOOT_ADDR                        BOOT_SEC_START * SPI_FLASH_SEC_SIZE              //应用程序存放地址
#define APP_ADDR                         APP_SEC_START * SPI_FLASH_SEC_SIZE               //应用程序存放地址
#define DEFAULT_APP_ADDR                 DEFAULT_APP_SEC_START * SPI_FLASH_SEC_SIZE       //默认程序 存放地址
#define CACHE_BOOT_ADDR                  CACHE_BOOT_SEC_START * SPI_FLASH_SEC_SIZE        //bootloader下载存放位置
#define CACHE_ONLINE_APP_ADDR            CACHE_ONLINE_APP_SEC_START * SPI_FLASH_SEC_SIZE  //在线编程程序下载存放地址
#define CACHE_DEFAULT_APP_ADDR           CACHE_DEFAULT_APP_SEC_START * SPI_FLASH_SEC_SIZE //默认程序下载存放地址
#define SUBSYS_VERSION_ADDR              DEFAULT_APP_ADDR+0x10                            //子系统程序版本号

#endif /*FLASH_MAP_H_*/
