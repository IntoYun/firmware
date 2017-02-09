#! /bin/bash
# Author: chenkaiyao <chenkaiyao@molmc.com>

./release-package.sh all
./release-online-program.sh all
./release-publish.sh test
exit 0
