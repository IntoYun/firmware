#! /bin/bash
# Author: zhang baoxian[baoxianzhang] <baoxianzhang.github.io>

if [ "$1" == "-s" ];then
VERBOSE=$1
else
VERBOSE=""
fi

echo "github push check begin!!!"

export INTOROBOT_DEVELOP=1
echo "check atom bootloader and app!!!"
make $VERBOSE PLATFORM=atom clean all COMPILE_LTO=n APP=default-atom
echo "check neutron bootloader and app!!!"
make $VERBOSE PLATFORM=neutron clean all COMPILE_LTO=n APP=default-neutron
echo "check nut bootloader and app!!!"
make $VERBOSE PLATFORM=nut clean all COMPILE_LTO=n APP=default-nut
echo "check lora bootloader and app!!!"
make $VERBOSE PLATFORM=lora clean all COMPILE_LTO=n APP=default-lora
echo "check gprs bootloader and app!!!"
make $VERBOSE PLATFORM=gprs clean all COMPILE_LTO=n APP=default-gprs
echo "check neutron-net bootloader and app!!!"
make $VERBOSE PLATFORM=neutron-net clean all COMPILE_LTO=n
echo "check fig !!!"
cd main && make $VERBOSE PLATFORM=fig clean all COMPILE_LTO=n
echo "check w67 !!!"
cd main && make $VERBOSE PLATFORM=w67 clean all COMPILE_LTO=n



echo "github push check success!!!"

exit 0
