#! /bin/bash

#下载工具
ESP_TOOL=esptool
ST_FLASH=st-flash

#esptool下载参数
UPLOAD_SPEED=230400
UPLOAD_PORT=/dev/cu.usbmodem141131
UPLOAD_VERB=-v
UPLOAD_RESET=nodemcu

#esp8266 flash参数
FLASH_SIZE=4M
FLASH_MODE=qio
FLASH_SPEED=40

echo ""
echo "start download..."
echo ""

if [ "$1" == "esp8266" ];then
  if [ "$2" == "all" ];then
    $ESP_TOOL $UPLOAD_VERB -cb $UPLOAD_SPEED -cd $UPLOAD_RESET -cp $UPLOAD_PORT -ca 0x00000 -cf neutron-net-boot.bin -ca 0x3FC000 -cf esp_init_data_default.bin -ca 0x3FE000 -cf blank.bin -ca 0x08000 -cf neutron-net.bin -ca 0xBC000 -cf default-neutron.bin -ca 0x100000 -cf neutron-boot.bin
  else
    $ESP_TOOL $UPLOAD_VERB -cb $UPLOAD_SPEED -cd $UPLOAD_RESET -cp $UPLOAD_PORT -ca 0x00000 -cf neutron-net-boot.bin -ca 0x08000 -cf neutron-net.bin -ca 0xBC000 -cf default-neutron.bin -ca 0x100000 -cf neutron-boot.bin
  fi
elif [ "$1" == "st" ];then
	  ST-FLASH write neutron-boot.bin 0x8000000
    sleep 1
	  ST-FLASH --reset write default-neutron.bin 0x08020000
fi

echo ""
echo "end download..."
echo ""


