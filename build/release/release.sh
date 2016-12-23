#! /bin/bash
# Author: zhang baoxian[baoxianzhang] <baoxianzhang.github.io>


VERSION="1.4.1.161223"


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

MAIN_DIR=$(cd "$(dirname "$0")"; pwd)
BUILD_DIR=$MAIN_DIR/..
RELEASE_DIR=$BUILD_DIR/release
PROJECT_DIR=$BUILD_DIR/..

release_bin() {
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

    cecho "Release $1 bin files" $blue

    RELEASE_BIN_DIR=$RELEASE_DIR/release-bin/$VERSION/$1-$VERSION/
    BUILD_BOOTLOADER_BIN_DIR=$BUILD_DIR/target/bootloader/platform-$PLATFORM_ID/
    BUILD_MAIN_BIN_DIR=$BUILD_DIR/target/main/platform-$PLATFORM_ID/
    RELEASE_COMMON_BIN_DIR=$RELEASE_DIR/common/$1/

    mkdir -p $RELEASE_BIN_DIR
    cd $PROJECT_DIR

    cecho "build $1-boot.bin default-$1.bin files" $blue
    make -s PLATFORM=$1 clean all COMPILE_LTO=n APP=default-$1
    cp $BUILD_BOOTLOADER_BIN_DIR/$1-boot.bin $RELEASE_BIN_DIR
    cp $BUILD_MAIN_BIN_DIR/default-$1.bin $RELEASE_BIN_DIR

    if [ "$1"x = "neutron"x ];then
        BUILD_BOOTLOADER_BIN_DIR=$BUILD_DIR/target/main/platform-888202/
        BUILD_MAIN_BIN_DIR=$BUILD_DIR/target/bootloader/platform-888202/

        make -s PLATFORM=neutron-net clean all COMPILE_LTO=n
        cp $BUILD_BOOTLOADER_BIN_DIR/neutron-net-boot.bin $RELEASE_BIN_DIR
        cp $BUILD_MAIN_BIN_DIR/neutron-net.bin $RELEASE_BIN_DIR
    fi

    cp $RELEASE_COMMON_BIN_DIR/* $RELEASE_BIN_DIR
}

if [ $# != 1 ];then
    cecho "Please enter the board to release(请选择需要发布的创客核心板或者商业模块:):" $yellow
    cecho "---------------------------------------------------------------------------" $yellow
    cecho "Maker Boards (创客核心板): neutorn     nut     atom     fig     lora" $yellow
    cecho "Business Boards(商业模组): w67         w323    l6                   " $yellow
    cecho "Release all please input : all" $yellow
    cecho "Clean all please input   : clean" $yellow
    cecho "---------------------------------------------------------------------------" $yellow
exit 1
fi
case $1 in
    all )
        cecho "Release all boards bin files" $blue
        release_bin neutron
        release_bin nut
        release_bin atom
        release_bin fig
        release_bin lora

        release_bin w67
        release_bin w323
        release_bin l6
        ;;

    clean )
        cecho "Clean the $VERSION bin files [Y/n]:" $blue
        ans=$(askForContinue)
        if [ $ans == 1 ];then
            rm $RELEASE_DIR/release-bin/$VERSION -rf
            cecho "Clean the $VERSION bin files done!!!" $green
            exit 0
        fi
        cecho "Not clean the $VERSION bin files!!!" $green
        exit 0
        ;;

    * )
        release_bin $1
        ;;
esac

cecho "Release bin files done!!!" $green
exit 0
