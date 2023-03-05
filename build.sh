#!/bin/bash

export PICO_SDK_PATH=../../pico-sdk

mkdir -p build
cd build
# rm -r *

echo "-------------------------"
echo "-        CMAKE          -"
echo "-------------------------"
cmake ..
[ $? -ne 0 ] && exit 1

echo ""
sleep 1

echo "-------------------------"
echo "-         MAKE          -"
echo "-------------------------"
make
