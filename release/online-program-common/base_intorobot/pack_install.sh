#! /bin/bash

SERVER_DIR=$1
chmod 755 $SERVER_DIR
mkdir -p $SERVER_DIR/base_intorobot
mkdir -p $SERVER_DIR/libs_public
mkdir -p $SERVER_DIR/users

sudo cp -rvf ./* $SERVER_DIR/base_intorobot
