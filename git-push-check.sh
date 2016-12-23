#! /bin/bash
# Author: zhang baoxian[baoxianzhang] <baoxianzhang.github.io>

if [ "$1" == "-s" ];then
VERBOSE=$1
else
VERBOSE=""
fi

export INTOROBOT_DEVELOP=1

echo "github push check begin!!!"
echo "------------------------创客核心板-------------------------"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!neutron!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=neutron clean all COMPILE_LTO=n APP=default-neutron

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!nut!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=nut clean all COMPILE_LTO=n APP=default-nut

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!atom!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=atom clean all COMPILE_LTO=n APP=default-atom

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!fig!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#make $VERBOSE PLATFORM=fig clean all COMPILE_LTO=n APP=default-atom

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!lora!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=lora clean all COMPILE_LTO=n APP=default-lora

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!bee!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#make $VERBOSE PLATFORM=bee clean all COMPILE_LTO=n APP=default-gprs

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!gprs!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=gprs clean all COMPILE_LTO=n APP=default-gprs
echo "------------------------------------------------------------"

echo "------------------------商业模块-------------------------"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!w67!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=w67 clean all COMPILE_LTO=n APP=default-w67

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!w323!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#make $VERBOSE PLATFORM=w323 clean all COMPILE_LTO=n APP=default-w323

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!l6!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=l6 clean all COMPILE_LTO=n APP=default-l6
echo "------------------------------------------------------------"

echo "------------------------其他-------------------------"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!gcc!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#make $VERBOSE PLATFORM=gcc clean all COMPILE_LTO=n

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!neutron-net!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=neutron-net clean all COMPILE_LTO=n

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!anytest!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
make $VERBOSE PLATFORM=anytest clean all COMPILE_LTO=n APP=default-anytest

echo "------------------------------------------------------------"
echo "github push check success!!!"

exit 0
