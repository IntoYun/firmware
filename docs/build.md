### 编译与烧录

在firmware目录下，可以进行各个产品的编译。编译默认固件的命令主要如下：

```
make PLATFORM=neutron clean all APP=smartLight-default
make PLATFORM=nut clean all APP=smartLight-default
make PLATFORM=atom clean all APP=smartLight-default
make PLATFORM=fig clean all APP=smartLight-default
make PLATFORM=ant clean all APP=blink
make PLATFORM=fox clean all APP=smartLight-default

make PLATFORM=w67 clean all APP=smartLight-w67
make PLATFORM=w323 clean all APP=blink
make PLATFORM=l6 clean all APP=blink
```


其中，*PLATFORM=product_name*也可以替换成*PLATFORM_ID=product_id*.产品的名称和ID的关系如下表（详情请参见build/platform-id.mk）：

| 开发板       | PLATFORM_ID |
|--------------|:-----------:|
| neutron      | 888002      |
| nut          | 888003      |
| atom         | 888004      |
| fig          | 888005      |
| ant          | 868009      |
| fox          | 878008      |

| 模组         | PLATFORM_ID |
|--------------|:-----------:|
| w67          | 888006      |
| w323         | 888007      |
| l6           | 868010      |

| 网关         | PLATFORM_ID |
|--------------|:-----------:|
| gl1000       | 188001      |
| gl2000       | 188002      |
| gl2100       | 178003      |

| 其他产品     | PLATFORM_ID |
|--------------|:-----------:|
| gcc          | 208001      |
| neutron-net  | 208002      |
| anytest      | 208003      |

进入到main目录下，可以选择更多的编译选项，还可以进行烧录。
以下常用的编译及烧录命令：

```
make PLATFORM=atom clean all st-flash
make PLATFORM=atom clean all program-dfu

make PLATFORM=neutron clean all st-flash
make PLATFORM=neutron clean all program-dfu

make PLATFORM=ant clean all DEBUG_BUILD=y USE_SWD=y st-flash
make PLATFORM=ant clean all DEBUG_BUILD=y USE_SWD=y program-dfu

make PLATFORM=fox clean all DEBUG_BUILD=y USE_SWD=y st-flash
make PLATFORM=fox clean all DEBUG_BUILD=y USE_SWD=y program-dfu

make PLATFORM=nut clean all DEBUG_BUILD=y USE_SWD=y esptool

make PLATFORM=fig clean all DEBUG_BUILD=y USE_SWD=y esptool-py

make PLATFORM=w67 clean all DEBUG_BUILD=y USE_SWD=y esptool

make PLATFORM=w323 clean all DEBUG_BUILD=y USE_SWD=y esptool-py

make PLATFORM=l6 clean all DEBUG_BUILD=y USE_SWD=y st-flash
make PLATFORM=l6 clean all DEBUG_BUILD=y USE_SWD=y program-dfu

```

指定工程目录编译:
工程目录结构：
1. 工程目录/src  放用户代码
2. 工程目录/lib  放用户库

```
make PLATFORM=w67 all DEBUG_BUILD=y USE_SWD=y APPDIR=../project/loragateway

```

DEBUG_BUILD=y打开调试， st-flash program-dfu esptool分别选择相应的烧录工具。
