#!/bin/sh
#common.mk

CC=gcc
CXX=g++
AR=ar
MAKE=make

#system variables
BUILD_MODE?=DEBUG
COMM_FLAG?=-std=c++17 -lstdc++

#[[*----- -----*file handing function*----- -----*]]

CXX_SOURCE:=${wildcard *.cpp}
C_SOURCE:=${wildcard *.c}
SOURCES:=$(CXX_SOURCE) $(C_SOURCE)

CXX_OBJECT:=${patsubst %.cpp, %.o, $(CXX_SOURCE)}
C_OBJECT:=${patsubst %.c, %.o, $(C_SOURCE)}
OBJECTS:=$(CXX_OBJECT) $(C_OBJECT)

#[[*----- -----*target rule*----- -----*]]
ifeq ($(BUILD_MODE), DEBUG)
	COMM_FLAG+=-g
endif

TARGET_RULE=
OBJECT_RULE=$(CC) -c $< -o $@ -D$(BUILD_MODE) $(COMM_FLAG) $(EXTRA)

ifeq ($(AIM), DIR)
	MAKE_AIM=dir_rule
	CLEAN_AIM=dir_rule
else
	MAKE_AIM=$(TARGET)
	CLEAN_AIM=
ifeq (${suffix $(TARGET)}, .a)
	TARGET_RULE=$(AR) rcs $(TARGET) $(OBJECTS)
else
	TARGET_RULE=$(CC) $^ -o $@ -D$(BUILD_MODE) $(COMM_FLAG) $(EXTRA)
endif
endif

#[[*----- -----*make rule*----- -----*]]

.PHONY:all
all::$(MAKE_AIM)

PHONY+=$(DEPENDS)
$(TARGET):$(OBJECTS) 
	$(TARGET_RULE)

%.o:%.c*
	$(OBJECT_RULE)

#[[*----- -----*clean rule*----- -----*]]

PHONY+=clean $(DEPENDS)
clean::$(CLEAN_AIM)
	-rm -f $(TARGET) $(OBJECTS)

PHONY+=dir_rule
dir_rule:
	@for dir in $(DEPENDS); do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
	done

.PHONY:$(PHONY)

