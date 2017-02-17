#! /bin/bash
# Author: chenkaiyao <chenkaiyao@molmc.com>
source ./release-version.sh

function askForContinue() {
    read ANS
    case $ANS in
        n|N|no|NO)
            echo 0
            ;;
            *)
            echo 1
            ;;
    esac
}

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

export INTOROBOT_DEVELOP=1

RELEASE_DIR=$(cd "$(dirname "$0")"; pwd)
PROJECT_DIR=$RELEASE_DIR/..
BUILD_DIR=$PROJECT_DIR/build

release_package() {
    case $1 in
################创客核心板####################
        neutron )
            PLATFORM_ID=888002
            ;;

        nut )
            PLATFORM_ID=888003
            ;;

        atom )
            PLATFORM_ID=888004
            ;;

        fig )
            PLATFORM_ID=888005
            ;;

        lora )
            PLATFORM_ID=888007
            ;;

        #add 此处添加创客核心板

################商业模块####################
        w67 )
            PLATFORM_ID=888101
            ;;

        w323 )
            PLATFORM_ID=888102
            ;;

        l6 )
            PLATFORM_ID=888103
            ;;

        #add  此处添加模块

################其他####################
        * )
            cecho "ERROR: Invalid selection." $red
            exit 1
            ;;
    esac

    cecho "Release $1 software-package" $yellow
    cecho "---------------------------------------------------------------------------" $yellow

    RELEASE_PACKAGE_DIR=$RELEASE_DIR/release-package/$SYSTEM_VERSION_STRING/$1-$SYSTEM_VERSION_STRING
    BUILD_BOOTLOADER_BIN_DIR=$BUILD_DIR/target/bootloader/platform-$PLATFORM_ID
    BUILD_MAIN_BIN_DIR=$BUILD_DIR/target/main/platform-$PLATFORM_ID
    RELEASE_COMMON_PACKAGE_DIR=$RELEASE_DIR/package-common/$1/

    rm -rf $RELEASE_PACKAGE_DIR
    mkdir -p $RELEASE_PACKAGE_DIR
    cd $PROJECT_DIR

    cecho "build $1-boot.bin default-$1.bin files" $yellow
    make -s PLATFORM=$1 clean all COMPILE_LTO=n APP=smartLight-default
    cp $BUILD_BOOTLOADER_BIN_DIR/$1-boot.bin $RELEASE_PACKAGE_DIR
    cp $BUILD_MAIN_BIN_DIR/default-$1.bin $RELEASE_PACKAGE_DIR

    if [ "$1"x = "neutron"x ];then
        BUILD_BOOTLOADER_BIN_DIR=$BUILD_DIR/target/bootloader/platform-888202
        BUILD_MAIN_BIN_DIR=$BUILD_DIR/target/main/platform-888202

        cecho "build neutron-net-boot.bin neutron-net.bin files" $yellow
        make -s PLATFORM=neutron-net clean all COMPILE_LTO=n
        cp $BUILD_BOOTLOADER_BIN_DIR/neutron-net-boot.bin $RELEASE_PACKAGE_DIR
        cp $BUILD_MAIN_BIN_DIR/neutron-net.bin $RELEASE_PACKAGE_DIR
    fi

    echo "$RELEASE_PACKAGE_DIR"
    cp -rf $RELEASE_COMMON_PACKAGE_DIR/* $RELEASE_PACKAGE_DIR
    tar -czf $RELEASE_PACKAGE_DIR.tar.gz $RELEASE_PACKAGE_DIR
}

if [ $# != 1 ];then
    cecho "Please enter the board to release(请选择需要发布的创客核心板或者商业模块:):" $yellow
    cecho "---------------------------------------------------------------------------" $yellow
    cecho "Maker Boards (创客核心板): neutorn     nut     atom     fig     lora" $yellow
    cecho "Business Boards(商业模组): w67         w323    l6                   " $yellow
    cecho "Release all please input : all" $yellow
    cecho "Clean all please input   : clean" $yellow
    cecho "---------------------------------------------------------------------------" $yellow
    cecho "example: ./release_package.sh all                                             " $yellow
    cecho "---------------------------------------------------------------------------" $yellow
    exit 1
fi
case $1 in
    all )
        cecho "Release all boards software-package" $yellow
        cecho "---------------------------------------------------------------------------" $yellow
        release_package neutron
        release_package nut
        #release_package atom
        #release_package fig
        #release_package lora

        #release_package w67
        #release_package w323
        #release_package l6
        ;;

    clean )
        cecho "Clean the $SYSTEM_VERSION_STRING software-package [Y/n]:" $blue
        ans=$(askForContinue)
        if [ $ans == 1 ];then
            rm $RELEASE_DIR/release-package/$SYSTEM_VERSION_STRING -rf
            cecho "Clean the $SYSTEM_VERSION_STRING software-package done!!!" $green
            exit 0
        fi
        cecho "clean the $SYSTEM_VERSION_STRING software-package failed!!!" $red
        exit 0
        ;;

    * )
        release_package $1
        ;;
esac

cecho "Release boards software-package done!!!" $green
exit 0
