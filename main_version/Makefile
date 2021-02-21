#!/bin/bash
include comm/common.mk

TARGET=APP
SUBDIRS=server
CLEANSUBDIRS=$(addprefix clean, ${SUBDIRS})

.PHONY:${TARGET} ${SUBDIRS}
${TARGET}:${SUBDIRS}
${SUBDIRS}:
	@echo $@
#${call testfunc}
	${MAKE} -C $@

.PHONY:clean ${CLEANSUBDIRS}
clean:${CLEANSUBDIRS}
${CLEANSUBDIRS}:
	${MAKE} -C $@
