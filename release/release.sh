#! /bin/bash
# Author: chenkaiyao <chenkaiyao@molmc.com>

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

if [ $# != 1 ];then
    cecho "Please select the type which you want to release(请选择你要发布的类型):" $yellow
    cecho "-------------------------------------------------------------------------------" $yellow
    cecho "type: package|online|all" $yellow
    cecho "-------------------------------------------------------------------------------" $yellow
    cecho "example: ./release.sh package" $yellow
    cecho "-------------------------------------------------------------------------------" $yellow
    exit 1
fi

case $1 in
    package )
        cecho "release package" $blue
        cecho "---------------------------------------------------------------------------" $yellow
        ./release-package.sh all
        ;;

    online )
        cecho "release online" $blue
        cecho "---------------------------------------------------------------------------" $yellow
        ./release-online-program.sh all
        ;;

    all )
        cecho "release all" $blue
        cecho "---------------------------------------------------------------------------" $yellow
        ./release-package.sh all
        ./release-online-program.sh all
        ./release-publish.sh test
        ;;
esac

cecho "release done!!!" $green
exit 0
