#! /bin/bash

./autogen.sh
./configure --prefix=/u/formes/share
make
make install
