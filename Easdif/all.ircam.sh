#! /bin/bash

./autogen.sh

if [ `uname` = "Darwin" -o `uname` =  "Linux" ];then
flags=$flags
else
flags="$flags CXXFLAGS=-mno-cygwin CFLAGS=-mno-cygwin LDFLAGS=-mno-cygwin"
fi

if [ "$1" = "" ]; then
./configure $flags --prefix=/u/formes/share --libdir=/u/formes/share/lib/$SYS --bindir=/u/formes/share/bin/$SYS
else
./configure $flags --prefix="$1" --libdir="$1"/lib/$SYS --bindir="$1"/bin/$SYS
fi
make
make install
