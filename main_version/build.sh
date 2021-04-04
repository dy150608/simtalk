#!/bin/bash

build_debug()
{
	source devenv
	export BUILD="DEBUG"
	make -f Makefile
}

build_release()
{
	source devenv
	export BUILD="RELEASE"
	make -f Makefile
}

build_clean()
{
	source devenv
	make -f Makefile clean
}

use_case()
{
echo "./build.sh options
options:
	debug: build for debug mode
	release: build for release mode
	others: cannot build"
}

#build.sh begin

case $1 in
	"prepare")
		source devenv
	;;
	"debug")
		build_debug
	;;
	"release")
		build_release
	;;
	"clean")
		build_clean
	;;
	*)
		use_case
esac

#build.sh end
