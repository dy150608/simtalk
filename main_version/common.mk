#!/bin/sh
#common.mk

CC=gcc
CXX=g++
MAKE=make

#system variables
BUILD?=DEBUG

#[[*----- -----*file handing function*----- -----*]]

CXX_SOURCE:=${wildcard *.cpp}
C_SOURCE:=${wildcard *.c}
SOURCES:=$(CXX_SOURCE) $(C_SOURCE)

CXX_OBJECT:=${patsubst %.cpp, %.o, $(CXX_SOURCE)}
C_OBJECT:=${patsubst %.c, %.o, $(C_SOURCE)}
OBJECTS:=$(CXX_OBJECT) $(C_OBJECT)

ifeq ($(AIM), DIR)
	MAKE_AIM=dir_role
	CLEAN_AIM=dir_role
else
	MAKE_AIM=$(TARGET)
	CLEAN_AIM=
endif

#[[*----- -----*make role*----- -----*]]

.PHONY:all
all::$(MAKE_AIM)

PHONY+=$(DEPENDS)
$(TARGET):$(OBJECTS) 
	$(CC) $^ -o $@ -lstdc++ $(EXTRA) -D$(BUILD)

%.o:%.c*
	$(CC) -c $< -o $@ -lstdc++ $(EXTRA) -D$(BUILD)

#[[*----- -----*clean role*----- -----*]]

PHONY+=clean $(DEPENDS)
clean::$(CLEAN_AIM)
	-rm -f $(TARGET) $(OBJECTS)

PHONY+=dir_role
dir_role:
	@for dir in $(DEPENDS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

.PHONY:$(PHONY)

