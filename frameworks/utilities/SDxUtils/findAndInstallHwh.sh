#!/bin/sh

# This script copies the hwh file to a specific location given by arg2.
# This assumes there is only one valid block diagram and generated hwh
# file for the target platform.
# arg1 is search directory where _sds is found
# arg2 is directory and file name where the new file should go
set -x
cp -f $1/_sds/p0/vivado/prj/prj.srcs/sources_1/bd/*/hw_handoff/*.hwh $2
