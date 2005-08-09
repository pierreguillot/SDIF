#! /bin/bash

./autogen.sh

if [ `uname` = "Darwin" -o `uname` =  "Linux" ];then
flags=$flags
else
flags="$flags CXXFLAGS=-mno-cygwin CFLAGS=-mno-cygwin LDFLAGS=-mno-cygwin"
fi

if [ "$1" = "" ]; then
./configure $flags --prefix=/u/formes/share
else
./configure $flags --prefix="$1"
fi
make
make install
