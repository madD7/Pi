# SELECT TARGET OPERATING SYSTEM 
UNAME_S:= $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	OS=Lin
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M), x86_64)
		ARCH=64
	else
		ARCH=32
	endif
else 
	$(shell exit -1)
endif	

DEBUG=0

SRCDIR_PATH= $(CURDIR)/src
INCDIR_PATH= $(CURDIR)/inc
OBJDIR_PATH= $(CURDIR)/obj
LIBDIR_PATH= $(CURDIR)/lib


CC_PATH= 
CC=$(CC_PATH)gcc
CC_FLAGS= -Wall -Wextra -I$(INCDIR_PATH)


ifeq ($(DEBUG), 1)
	CC_FLAGS := $(CC_FLAGS) -g
endif


ifeq ($(BIT), 32)
	CC_FLAGS := $(CC_FLAGS) -m32
endif


VPATH:= $(SRCDIR_PATH)

SRCS=log.c
OBJS=$(patsubst %.c, $(OBJDIR_PATH)/%.o, $(SRCS))
LIBS=$(patsubst %.c, $(LIBDIR_PATH)/lib%.a, $(SRCS))

.PHONY: all clean build

all : clean makdir build
makdir:
	mkdir -p $(OBJDIR_PATH)
	mkdir -p $(LIBDIR_PATH)

build: $(LIBS)

$(LIBDIR_PATH)/lib%.a: $(OBJDIR_PATH)/%.o
	ar rcs $@ $< 
	
#$(CC_FLAGS) += "-l$(@:lib%=%.a)"

# Automatic Variables
# $@ - Contains target file name
# $< - Contains 1st dependency file name
$(OBJDIR_PATH)/%.o: $(SRCDIR_PATH)/%.c		#Generic Rule for *.o
		$(CC) -c $(CC_FLAGS) $< -o $@ 

		
clean :
	rm -f $(OBJS) $(EXE) $(LIBS)
