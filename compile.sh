#!/bin/bash
rm -r compiled
mkdir compiled
cmake ./src
make
mv SK compiled/serwerRun.exec
chmod +x compiled/serwerRun.exec
rm Makefile
rm CMakeCache.txt
rm cmake_install.cmake
echo Skompilowano serwer