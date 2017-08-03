# SELECT TARGET OPERATING SYSTEM 
UNAME_S:=$(shell uname -s)
ifeq ($(UNAME_S), Linux)
	OS=Lin
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M), x86_64)
		ARCH=64
	else
		ARCH=32
	endif
endif

DEBUG=0

ROOTDIR_PATH= $(CURDIR)
FRAMEWORKDIR_PATH=$(ROOTDIR_PATH)/Framework
APPDIR_PATH=$(ROOTDIR_PATH)/Interfaces

SUBDIRS: Framework Interfaces

CC_PATH= 
CC= $(CC_PATH)gcc
CC_FLAGS= -Wall -Wextra -I$(INC_DIR)


ifeq ($(DEBUG), 1)
	CC_FLAGS := $(CC_FLAGS) -g
	BUILDDIR_PATH := debug
else
	BUILDDIR_PATH := release
endif


ifeq ($(BIT), 32)
	CC_FLAGS := $(CC_FLAGS) -m32
endif


export

.PHONY: all subdirs $(SUBDIRS) frmwrk applcatn 

subdirs: $(SUBDIRS)


all: frmwrk applcatn

$(SUBDIRS):
	$(MAKE) -C $@

frmwrk:
	$(MAKE) -C Framework	


applcatn:
	$(MAKE) -C Interfaces	
	


