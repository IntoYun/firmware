#! /bin/bash

#下载工具
ESP32_TOOL=./tools/esp32/esptool

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

#fig flash参数
FLASH_SIZE=4MB
FLASH_MODE=qio
FLASH_SPEED=40m

echo ""
echo "start download..."
echo ""

$ADDSUDO $ESP32_TOOL --chip esp32 --port $UPLOAD_PORT --baud $UPLOAD_SPEED --before default_reset --after hard_reset write_flash -z --flash_freq $FLASH_SPEED --flash_mode $FLASH_MODE --flash_size $FLASH_SIZE 0x1000 fig-boot.bin 0x8000 partitions.bin 0x20000 default-fig.bin 0x120000 default-fig.bin 0x219000 fig-boot.bin

echo ""
echo "end download..."
echo ""


