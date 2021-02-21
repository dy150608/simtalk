#!/bin/bash
#include comm/common.mk

TARGET=
SUBDIRS=module

.PHONY:subdir $(SUBDIRS)
subdir:${SUBDIRS}
	@echo subdir
	${call testfunc}
	${MAKE} -C $@

.PHONY:clean ${SUBDIRS}
clean:
	${MAKE} -C $@
