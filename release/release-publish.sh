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

RELEASE_DIR=$(cd "$(dirname "$0")"; pwd)
BASE_NAME=base_intorobot_$SYSTEM_VERSION_STRING
RELEASE_ONLINE_PROGRAM_DIR=$RELEASE_DIR/release-online-program

if [ $# != 1 ];then
    cecho "Please select the server which you want to publish(请选择你要发布的服务器平台):" $yellow
    cecho "-------------------------------------------------------------------------------" $yellow
    cecho "server type: test     intorobot    intoyun" $yellow
    cecho "-------------------------------------------------------------------------------" $yellow
    cecho "example: ./release_publish.sh test" $yellow
    cecho "-------------------------------------------------------------------------------" $yellow
    exit 1
fi

cd $RELEASE_ONLINE_PROGRAM_DIR
tar -czf $BASE_NAME.tar.gz $BASE_NAME

case $1 in
    test )
        cecho "Publish to the test server" $blue
        cecho "---------------------------------------------------------------------------" $yellow
        scp $BASE_NAME.tar.gz root@112.124.117.64:/tmp/
        sleep 1
        ssh root@112.124.117.64
        ;;

    intorobot )
        cecho "Publish to the intorobot server" $blue
        cecho "---------------------------------------------------------------------------" $yellow
        scp $BASE_NAME.tar.gz root@115.29.193.81:/tmp/
        cecho "Tell zhongjin upload！" $yellow
        ;;

    intoyun )
        cecho "Publish to the intorobot server" $blue
        cecho "---------------------------------------------------------------------------" $yellow
        scp $BASE_NAME.tar.gz root@115.29.193.81:/tmp/
        cecho "Tell zhongjin upload！" $yellow
        ;;
esac

cecho "publish boards online-program-package done!!!" $green
exit 0
