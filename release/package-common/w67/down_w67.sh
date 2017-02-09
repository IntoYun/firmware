#! /bin/bash

#下载工具
ESP_TOOL=esptool
sysType=`uname -s`

#esptool下载参数
UPLOAD_SPEED=921600
if [ $sysType = "Linux" ]; then
    UPLOAD_PORT=/dev/ttyUSB0
    ADDSUDO=sudo
elif [ $sysType = "Darwin" ]; then
    UPLOAD_PORT=/dev/cu.SLAB_USBtoUART
    ADDSUDO=""
fi

UPLOAD_VERB=-v
UPLOAD_RESET=nodemcu

#esp8266 flash参数
FLASH_SIZE=16M
FLASH_MODE=qio
FLASH_SPEED=40

echo ""
echo "start download..."
echo ""

if [ "$1" == "all" ];then
    $ADDSUDO $ESP_TOOL $UPLOAD_VERB -cb $UPLOAD_SPEED -cd $UPLOAD_RESET -cp $UPLOAD_PORT -ca 0x00000 -cf w67-boot.bin -ca 0x14000 -cf default-w67.bin -ca 0x114000 -cf default-w67.bin -ca 0x194000 -cf w67-boot.bin -ca 0x3FC000 -cf esp_init_data_default.bin -ca 0x3FE000 -cf blank.bin
else
    $ADDSUDO $ESP_TOOL $UPLOAD_VERB -cb $UPLOAD_SPEED -cd $UPLOAD_RESET -cp $UPLOAD_PORT -ca 0x00000 -cf w67-boot.bin -ca 0x14000 -cf default-w67.bin -ca 0x114000 -cf default-w67.bin -ca 0x194000 -cf w67-boot.bin
fi

echo ""
echo "end download..."
echo ""


