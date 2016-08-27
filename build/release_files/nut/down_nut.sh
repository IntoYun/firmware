#! /bin/bash

#下载工具
ESP_TOOL=esptool

#esptool下载参数
UPLOAD_SPEED=921600
UPLOAD_PORT=/dev/cu.SLAB_USBtoUART
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
    $ESP_TOOL $UPLOAD_VERB -cb $UPLOAD_SPEED -cd $UPLOAD_RESET -cp $UPLOAD_PORT -ca 0x00000 -cf nut-boot.bin -ca 0x20000 -cf default-nut.bin -ca 0xDB000 -cf default-nut.bin -ca 0x134000 -cf nut-boot.bin -ca 0x3FC000 -cf esp_init_data_default.bin -ca 0x3FE000 -cf blank.bin
else
    $ESP_TOOL $UPLOAD_VERB -cb $UPLOAD_SPEED -cd $UPLOAD_RESET -cp $UPLOAD_PORT -ca 0x00000 -cf nut-boot.bin -ca 0x20000 -cf default-nut.bin -ca 0xDB000 -cf default-nut.bin -ca 0x134000 -cf nut-boot.bin
fi

echo ""
echo "end download..."
echo ""


