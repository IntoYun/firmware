## 添加新产品流程

1. 进入build/platform-id.mk 添加新platform和新产品描述
2. 进入build/linker 创建产品链接文件。
3. 进入build/startup 创建产品启动文件。
4. 进入bootloader 创建一个新产品分支。
5. 进入hal/inc/variants创建一个新产品分支。进入hal/src/创建一个新产品分支。
6. 进入platform/shared/inc/platforms.h添加一个产品宏定义。
7. 进入platform/MCU添加一个平台驱动库或者SDK库。如果已经存在，使用现成。
8. 进入wiring_ex/添加一个产品扩展功能分支。

