#! /bin/bash
rm -rf gcc-arm-none-eabi
rm -rf stlink
rm -rf xtensa-lx106-elf
rm -rf esptool

mkdir dist
cd dist

sysType=`uname -s`
echo -e "\033[40;32m Your system is $sysType \033[0m"
if [ $sysType = "Linux" ]; then
    echo -e "\033[40;32m 1. install arm gnu toolchain\033[0m"
    wget -nc https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/+download/gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
    tar -jxf gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
    mv gcc-arm-none-eabi-4_9-2015q3 ../gcc-arm-none-eabi
    sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0

    if [ $(getconf WORD_BIT) = '32' ] && [ $(getconf LONG_BIT) = '64' ] ; then
        echo -e "\033[40;32m 2. install st-flash\033[0m"
        wget -nc https://github.com/IntoRobot/stlink/releases/download/1.2.0/stlink-1.2.0-linux64.tar.gz
        tar -zxf stlink-1.2.0-linux64.tar.gz
        mv stlink-1.2.0-linux64 ../stlink

        echo -e "\033[40;32m 3. install xtensa lx106 gnu toolchain\033[0m"
        wget -nc http://arduino.esp8266.com/linux64-xtensa-lx106-elf-gb404fb9.tar.gz
        tar -zxf linux64-xtensa-lx106-elf-gb404fb9.tar.gz
        mv  xtensa-lx106-elf ../xtensa-lx106-elf

        echo -e "\033[40;32m 4. install esptool\033[0m"
        wget -nc https://github.com/igrr/esptool-ck/releases/download/0.4.9/esptool-0.4.9-linux64.tar.gz
        tar -zxf esptool-0.4.9-linux64.tar.gz
        mv  esptool-0.4.9-linux64 ../esptool

    else
        echo -e "\033[40;32m 2. install st-flash\033[0m"
        wget -nc https://github.com/IntoRobot/stlink/releases/download/1.2.0/stlink-1.2.0-linux32.tar.gz
        tar -zxf stlink-1.2.0-linux32.tar.gz
        mv stlink-1.2.0-linux32 ../stlink

        echo -e "\033[40;32m 3. install xtensa lx106 gnu toolchain\033[0m"
        wget -nc http://arduino.esp8266.com/linux32-xtensa-lx106-elf.tar.gz
        tar -zxf linux32-xtensa-lx106-elf.tar.gz
        mv  xtensa-lx106-elf ../xtensa-lx106-elf

        echo -e "\033[40;32m 4. install esptool\033[0m"
        wget -nc https://github.com/igrr/esptool-ck/releases/download/0.4.9/esptool-0.4.9-linux32.tar.gz
        tar -zxf esptool-0.4.9-linux32.tar.gz
        mv  esptool-0.4.9-linux32 ../esptool

    fi
    echo -e "\033[40;32m 5. install dfu-util\033[0m"
    sudo apt-get install dfu-util

elif [ $sysType = "Darwin" ]; then
    echo -e "\033[40;32m 1. install arm gnu toolchain\033[0m"
    wget -nc https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q2-update/+download/gcc-arm-none-eabi-4_9-2015q2-20150609-mac.tar.bz2
    tar -jxf gcc-arm-none-eabi-5_4-2016q2-20160622-mac.tar.bz2
    mv gcc-arm-none-eabi-5_4-2016q2 ../gcc-arm-none-eabi
    rm -rf gcc-arm-none-eabi-4_9-2015q2

    echo -e "\033[40;32m 2. install st-flash\033[0m"
    wget -nc https://github.com/IntoRobot/stlink/releases/download/1.2.0/stlink-1.2.0-osx.tar.gz
    tar -zxf stlink-1.2.0-osx.tar.gz
    mv stlink-1.2.0-osx ../stlink

    echo -e "\033[40;32m 3. install xtensa lx106 gnu toolchain\033[0m"
    wget -nc http://arduino.esp8266.com/osx-xtensa-lx106-elf-gb404fb9-2.tar.gz
    tar -zxf osx-xtensa-lx106-elf-gb404fb9-2.tar.gz
    mv xtensa-lx106-elf ../xtensa-lx106-elf

    echo -e "\033[40;32m 4. install esptool\033[0m"
    wget -nc https://github.com/igrr/esptool-ck/releases/download/0.4.9/esptool-0.4.9-osx.tar.gz
    tar -zxf esptool-0.4.9-osx.tar.gz
    mv esptool-0.4.9-osx ../esptool
    rm -rf esptool-0.4.9-osx

    echo -e "\033[40;32m 5. install dfu-util\033[0m"
    brew install dfu-util

else
    echo -e "\033[40;32m unsupported system, exit \033[0m"
fi

cd ..

echo "---------end  install  tools-------------"

