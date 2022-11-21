#!/bin/bash

sudo apt-get update
sudo apt-get install g++ cmake valgrind libgtest-dev

cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp -v lib/*.a /usr/lib
