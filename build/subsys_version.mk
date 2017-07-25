# 模组系统版本号

################创客核心板####################
#neutron
#neutorn 子系统版本号放在esp8266上，stm32通过At指令获取子系统版本号
#子系统的版本号，主要用于管理esp8266程序、默认应用程序、stm32的bootloader程序。
#版本号组成:  总共4位，其中第四位:为stm32 bootloader版本号，必须与升级包中的bootloader版本一致
#SUBSYS_VERSION_STRING_NEUTRON = 1.0.2.1  #初始版本
#SUBSYS_VERSION_STRING_NEUTRON = 2.0.0.1  #esp8266 sdk库升级至v2.0版本
#SUBSYS_VERSION_STRING_NEUTRON = 2.0.0.2  #bootloader 版本更新至v2  1.修正flash烧写失败处理,保证烧写成功  2. esp8266升级模式，波特率可以通过usb设置
#====2.0.1.3 版本说明====
# 1. 默认程序更新 * 添加激活流程  *简化配置模式
# 2. bootloade3 版本更新至v3  *.简化工作模式 *.简化灯的变化,开始统一为白灯
SUBSYS_VERSION_STRING_NEUTRON = 2.0.1.3

#nut
#nut子系统的版本号，主要用于管理默认应用程序和bootloader程序
#版本号组成:  VERSION: 为固定字符串。主要用于识别版本号。 *.*.*为默认程序版本号, 第四位为bootloader的版本号。
#SUBSYS_VERSION_STRING_NUT = 1.0.0.1  #初始版本
#====1.0.1.2 版本说明====
# 1. 默认程序更新
# 2. bootloade3 版本更新至v2  *.修改开机三色灯高亮问题
#SUBSYS_VERSION_STRING_NUT = 1.0.1.2
#====1.0.2.3 版本说明====
# 1. 默认程序更新 * 添加激活流程  *简化配置模式
# 2. bootloade3 版本更新至v3  *.简化工作模式 *.简化灯的变化,开始统一为白灯
SUBSYS_VERSION_STRING_NUT = 1.0.2.3


#atom with bootloader
SUBSYS_VERSION_STRING_ATOM = 3.6.6.0


#fig
#fig子系统的版本号，主要用于管理默认应用程序和bootloader程序
#版本号组成:  VERSION: 为固定字符串。主要用于识别版本号。 *.*.*为默认程序版本号, 第四位为bootloader的版本号。
#SUBSYS_VERSION_STRING_FIG = 1.0.0.1 #初始版本
#====1.0.1.2 版本说明====
# 1. 默认程序更新 * 添加激活流程  *简化配置模式
# 2. bootloade3 版本更新至v3  *.简化工作模式 *.简化灯的变化,开始统一为白灯
SUBSYS_VERSION_STRING_FIG = 1.0.1.2


#ant
#ant子系统的版本号，主要用于管理bootloader程序
#版本号组成:  前面3位固定为1.0.0, 第四位为bootloader的版本号。
SUBSYS_VERSION_STRING_ANT = 1.0.0.1


#fox
#fox子系统的版本号，主要用于管理默认程序和bootloader程序
#版本号组成:  *.*.*为默认程序版本号, 第四位为bootloader的版本号。
SUBSYS_VERSION_STRING_FOX = 1.0.0.1


#add 此处添加创客核心板

################商业模块####################
#w67
#w67子系统的版本号，主要用于管理bootloader程序
#版本号组成:  前面3位固定为1.0.0, 第四位为bootloader的版本号。
#SUBSYS_VERSION_STRING_W67 = 1.0.0.1
SUBSYS_VERSION_STRING_W67 = 1.0.1.2


#w323
#w323子系统的版本号，主要用于管理bootloader程序
#版本号组成:  前面3位固定为1.0.0, 第四位为bootloader的版本号。
#SUBSYS_VERSION_STRING_W323 = 1.0.0.1
SUBSYS_VERSION_STRING_W323 = 1.0.1.2


#l6
#l6子系统的版本号，主要用于管理bootloader程序
#版本号组成:  前面3位固定为1.0.0, 第四位为bootloader的版本号。
SUBSYS_VERSION_STRING_L6 = 1.0.0.1


#add  此处添加模块


################其他####################
#gcc
SUBSYS_VERSION_STRING_GCC = 1.0.0.1

#anytest
SUBSYS_VERSION_STRING_ANYTEST = 1.0.0.1

#gl2000
SUBSYS_VERSION_STRING_GL2000 = 1.0.0.1

#add  此处添加模块

