#!/bin/bash

source utils.sh --source-specific get_default_args

./build.sh -R
./compile.sh
./run.sh -m train $(get_default_args "TRAIN")
./run.sh -m eval $(get_default_args "EVAL")
