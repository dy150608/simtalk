#!/bin/bash
ROOT="/simtalk"

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
