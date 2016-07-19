#! /bin/bash
# Author: zhang baoxian[baoxianzhang] <baoxianzhang.github.io>

function askForContinue()
{
    read ANS
    case $ANS in
        n|N|no|NO)
            echo 0
            break;;
        *)
            echo 1
            break;;
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

release_atom_bin() {
    mkdir -p $RELEASE_DIR/intorobot-bin/$VERSION/atom/
    cd $PROJECT_DIR
    make -s PLATFORM=atom clean all COMPILE_LTO=n APP=default-atom
    cp $BUILD_DIR/target/main/platform-0/default-atom.bin $RELEASE_DIR/intorobot-bin/$VERSION/atom/

    # For now not include atom-boot.bin
}

release_neutron_bin() {
    mkdir -p $RELEASE_DIR/intorobot-bin/$VERSION/neutron/
    cd $PROJECT_DIR

    # default-neutron.bin
    make -s PLATFORM=neutron clean all COMPILE_LTO=n APP=default-neutron
    cp $BUILD_DIR/target/main/platform-1/default-neutron.bin $RELEASE_DIR/intorobot-bin/$VERSION/neutron/

    # blank.bin esp_init_data_default.bin down_neutron.sh
    cp $MAIN_DIR/neutron/* $RELEASE_DIR/intorobot-bin/$VERSION/neutron/

    # neutron-net.bin
    make -s PLATFORM=neutron-net clean all COMPILE_LTO=n
    cp $BUILD_DIR/target/main/platform-3/neutron-net.bin $RELEASE_DIR/intorobot-bin/$VERSION/neutron/

    # neutron-boot.bin
    cd $PROJECT_DIR/bootloader
    make -s PLATFORM=neutron clean all COMPILE_LTO=n
    cp $BUILD_DIR/target/bootloader/platform-1/neutron-boot.bin $RELEASE_DIR/intorobot-bin/$VERSION/neutron/

    # neutron-net-boot.bin
    cd $PROJECT_DIR/bootloader
    make -s PLATFORM=neutron-net clean all COMPILE_LTO=n
    cp $BUILD_DIR/target/bootloader/platform-3/neutron-net-boot.bin $RELEASE_DIR/intorobot-bin/$VERSION/neutron/
}

cecho "Please enter the release version (1.3.0.160625):" $yellow
read VERSION
cecho "The input version is $VERSION" $blue

# Select the board to release bin
cecho "Please select the number which board bin files to release:" $yellow
select yn in "no" "atom" "neutron" "all" "clean"; do
      case $yn in
        no )
            cecho "Not release!!" $blue
            exit 0
            break;;
        atom )
            cecho "Release atom bin files" $blue
            RELEASE_BOARD=atom
            break;;
        neutron )
            cecho "Release neutron bin files" $blue
            RELEASE_BOARD=neutron
            break;;
        all )
            cecho "Release all boards (atom & neutron) bin files" $blue
            RELEASE_BOARD=all
            break;;
        clean )
            cecho "Clean the $VERSION bin files [Y/n]:" $blue
            ans=$(askForContinue)
            if [ $ans == 1 ];then
                rm $RELEASE_DIR/intorobot-bin/$VERSION -rf
                cecho "Clean the $VERSION bin files done!!!" $green
                exit 0
            fi
            cecho "Not clean the $VERSION bin files!!!" $green
            exit 0
            break;;
        * )
            cecho "ERROR: Invalid selection." $red
            exit 1
            break;;
      esac
done

cecho "Attempting to release $RELEASE_BOARD bin files" $yellow 
if [ $RELEASE_BOARD == "atom" ]; then
    release_atom_bin
elif [ $RELEASE_BOARD == "neutron" ]; then
    release_neutron_bin
elif [ $RELEASE_BOARD == "all" ]; then
    release_atom_bin
    release_neutron_bin
fi

cecho "Rlease $RELEASE_BOARD bin files done!!!" $green
exit 0
