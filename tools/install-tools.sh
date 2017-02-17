#! /bin/bash

# Color-echo.
# arg $1 = message
# arg $2 = Color
cecho() {
  echo -e "${2}${1}"
  tput sgr0
  # Reset # Reset to normal.
  return
}
# Set the colours you can use
red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'

rm -rf gcc-arm-none-eabi
rm -rf xtensa-lx106-elf
rm -rf xtensa-esp32-elf
rm -rf stlink
rm -rf esptool

mkdir dist
cd dist

sysType=`uname -s`
cecho "Your system is $sysType " $blue
if [ $sysType = "Linux" ]; then
    cecho "--->install lib tools " $blue
    sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0
    sudo apt-get install git wget make libncurses-dev flex bison gperf python python-serial
    cecho "--->install success " $blue

    cecho "--->install arm gnu toolchain " $blue
    wget -nc https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/+download/gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
    tar -jxf gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
    mv gcc-arm-none-eabi-4_9-2015q3 ../gcc-arm-none-eabi
    cecho "--->install success " $blue

    if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
        cecho "--->install xtensa lx106 gnu toolchain " $blue
        wget -nc http://arduino.esp8266.com/linux64-xtensa-lx106-elf-gb404fb9.tar.gz
        tar -zxf linux64-xtensa-lx106-elf-gb404fb9.tar.gz
        mv  xtensa-lx106-elf ../xtensa-lx106-elf
        cecho "--->install success " $blue

        cecho "--->install xtensa esp32 toolchain " $blue
        wget -nc https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-61-gab8375a-5.2.0.tar.gz
        tar -zxf xtensa-esp32-elf-linux64-1.22.0-61-gab8375a-5.2.0.tar.gz
        mv  xtensa-esp32-elf ../xtensa-esp32-elf
        cecho "--->install success " $blue

        cecho "--->install st-flash " $blue
        wget -nc https://github.com/IntoRobot/stlink/releases/download/1.2.0/stlink-1.2.0-linux64.tar.gz
        tar -zxf stlink-1.2.0-linux64.tar.gz
        mv stlink-1.2.0-linux64 ../stlink
        cecho "--->install success " $blue

        cecho "--->install esptool " $blue
        wget -nc https://github.com/igrr/esptool-ck/releases/download/0.4.11/esptool-0.4.11-linux64.tar.gz
        tar -zxf esptool-0.4.11-linux64.tar.gz
        mv  esptool-0.4.11-linux64 ../esptool
        cecho "--->install success " $blue

    else
        cecho "--->install xtensa lx106 gnu toolchain " $blue
        wget -nc http://arduino.esp8266.com/linux32-xtensa-lx106-elf.tar.gz
        tar -zxf linux32-xtensa-lx106-elf.tar.gz
        mv  xtensa-lx106-elf ../xtensa-lx106-elf
        cecho "--->install success " $blue

        cecho "--->install xtensa esp32 toolchain " $blue
        wget -nc https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-61-gab8375a-5.2.0.tar.gz
        tar -zxf xtensa-esp32-elf-linux32-1.22.0-61-gab8375a-5.2.0.tar.gz
        mv  xtensa-esp32-elf ../xtensa-esp32-elf
        cecho "--->install success " $blue

        cecho "--->install st-flash " $blue
        wget -nc https://github.com/IntoRobot/stlink/releases/download/1.2.0/stlink-1.2.0-linux32.tar.gz
        tar -zxf stlink-1.2.0-linux32.tar.gz
        mv stlink-1.2.0-linux32 ../stlink
        cecho "--->install success " $blue

        cecho "--->install esptool " $blue
        wget -nc https://github.com/igrr/esptool-ck/releases/download/0.4.11/esptool-0.4.11-linux64.tar.gz
        tar -zxf esptool-0.4.11-linux64.tar.gz
        mv  esptool-0.4.11-linux64 ../esptool
        cecho "--->install success " $blue

    fi
    cecho "--->install dfu-util " $blue
    sudo apt-get install dfu-util
    cecho "--->install success " $blue

elif [ $sysType = "Darwin" ]; then
    cecho "--->install lib tools " $blue
    sudo easy_install pip
    sudo pip install pyserial
    cecho "--->install success " $blue

    cecho "--->install arm gnu toolchain " $blue
    wget -nc -c -t 10 https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q2-update/+download/gcc-arm-none-eabi-4_9-2015q2-20150609-mac.tar.bz2
    tar -jxf  gcc-arm-none-eabi-4_9-2015q2-20150609-mac.tar.bz2
    mv gcc-arm-none-eabi-4_9-2015q2 ../gcc-arm-none-eabi
    rm -rf gcc-arm-none-eabi-4_9-2015q2
    cecho "--->install success " $blue

    cecho "--->install xtensa lx106 gnu toolchain " $blue
    wget -nc -c -t 10 http://arduino.esp8266.com/osx-xtensa-lx106-elf-gb404fb9-2.tar.gz
    tar -zxf osx-xtensa-lx106-elf-gb404fb9-2.tar.gz
    mv xtensa-lx106-elf ../xtensa-lx106-elf
    cecho "--->install success " $blue

    cecho "--->install xtensa esp32 toolchain " $blue
    wget -nc https://dl.espressif.com/dl/xtensa-esp32-elf-osx-1.22.0-61-gab8375a-5.2.0.tar.gz
    tar -zxf xtensa-esp32-elf-osx-1.22.0-61-gab8375a-5.2.0.tar.gz
    mv  xtensa-esp32-elf ../xtensa-esp32-elf
    cecho "--->install success " $blue

    cecho "--->install st-flash " $blue
    wget -nc -c -t 10 https://github.com/IntoRobot/stlink/releases/download/1.2.0/stlink-1.2.0-osx.tar.gz
    tar -zxf stlink-1.2.0-osx.tar.gz
    mv stlink-1.2.0-osx ../stlink
    cecho "--->install success " $blue

    cecho "--->install esptool " $blue
    wget -nc -c -t 10 https://github.com/igrr/esptool-ck/releases/download/0.4.11/esptool-0.4.11-osx.tar.gz
    tar -zxf esptool-0.4.11-osx.tar.gz
    mv esptool-0.4.11-osx ../esptool
    rm -rf esptool-0.4.11-osx
    cecho "--->install success " $blue

    cecho "--->install dfu-util " $blue
    brew install dfu-util
    cecho "--->install success " $blue

else
    cecho "unsupported system, exit " $red
fi

cd ..

cecho "---------end  install  tools-------------" $blue

