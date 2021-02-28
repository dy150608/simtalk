#!/bin/bash
ROOT="/simtalk"
CURR_ROOT=$(pwd)

build_debug()
{
	export BUILD="debug"
	make -f Makefile
}

build_release()
{
	export BUILD="release"
	make -f Makefile
}

use_case()
{
echo "./build.sh options
options:
	debug: build for debug mode
	release: build for release mode
	others: cannot build"
}

environment()
{
if [ -e $ROOT ]; then
	mkdir $ROOT; cd $ROOT
fi
#gcc include path
C_INCLUDE_PATH=$(C_INCLUDE_PATH):$(CURR_ROOT)
export C_INCLUDE_PATH

#g++ include path
CPLUS_INCLUDE_PATH=$(CPLUS_INCLUDE_PATH):$(CURR_ROOT)
export CPLUS_INCLUDE_PATH

#dynamic link library path
LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(CURR_ROOT)
export LD_LIBRARY_PATH

#static library path
LIBRARY_PATH=$(LIBRARY_PATH):$(CURR_ROOT)
export LIBRARY_PATH
}

#build begin

environment

case $1 in
	"debug")
		build_debug
	;;
	"release")
		build_release
	;;
	*)
		use_case
esac

#build end
