#!/bin/bash

source utils.sh --source-specific set_build_type

BUILD_T=$(set_build_type "$1" Release Debug Debug)

cmake -G "Unix Makefiles" -S ../build/ -B ../build/out/ -DCMAKE_BUILD_TYPE=${BUILD_T}
