#!/bin/bash

apt install build-essential dh-autoreconf autoconf libevent-dev libboost-all-dev -y

./configure
make
cp -fr ./bitcointool /usr/bin/
