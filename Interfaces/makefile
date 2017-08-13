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


# All Include directory paths
INCDIR_PATHS=$(CURDIR)/inc $(ROOTDIR_PATH)/Framework/inc
INC_PARAM=$(INCDIR_PATHS:%=-I%)

# "foreach" is more readable, but substitution method is shorter
#INC_PARAM=$(foreach d, $(INCDIR_PATHS), -I$(INCDIR_PATHS))

SRCDIR_PATH=$(CURDIR)/src
OBJDIR_PATH=$(CURDIR)/obj
LIBDIR_PATH=$(ROOTDIR_PATH)/Framework/lib/

# Notice $$ in awk.  
LIB_NAMES=$(shell find $(LIBDIR_PATH) -name 'lib*.a' | awk -F/ '{print $$NF}')
LIB_PARAM=$(LIB_NAMES:lib%.a=-l%)

EXEDIR_PATH= $(CURDIR)
EXE=interface.out

CC_PATH= 
CC=$(CC_PATH)gcc
CC_FLAGS=-Wall -Wextra $(INC_PARAM) 


ifeq ($(DEBUG), 1)
	CC_FLAGS:= $(CC_FLAGS) -g
endif


ifeq ($(BIT), 32)
	CC_FLAGS:= $(CC_FLAGS) -m32
endif


VPATH:= $(SRCDIR_PATH)

SRCS=main.c cliparser.c	i2cdriver.c
OBJS=$(patsubst %.c, $(OBJDIR_PATH)/%.o, $(SRCS))

.PHONY: all clean build

all : clean makdir build
makdir:
	mkdir -p $(OBJDIR_PATH)

build: $(OBJS)
	$(CC) $(CC_FLAGS) $(OBJS) -o $(EXEDIR_PATH)/$(EXE) -L$(LIBDIR_PATH) $(LIB_PARAM)

# Automatic Variables
# $@ - Contains target file name
# $< - Contains 1st dependency file name
$(OBJDIR_PATH)/%.o: $(SRCDIR_PATH)/%.c		#Generic Rule for *.o
		$(CC) -c $(CC_FLAGS)  $< -o $@

		
clean :
	rm -f $(OBJS) $(EXE) $(LIBS)
