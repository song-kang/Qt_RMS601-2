CC = g++

CC_INCLUDES = -I/usr/include -I/usr/local/include

CC_FLAGS    = $(CC_INCLUDES) -pipe -w -O3 -pipe 
CC_FLAGS_D  = $(CC_INCLUDES) -pipe -Wall -O2 -g -pipe
CC_COMPLIE  = $(CC_INCLUDES) $(CC_FLAGS) $<
CC_COMPLIE_D= $(CC_INCLUDES) $(CC_FLAGS_D) $<
CC_LINK     = -L/usr/lib/ -L/usr/local/lib/ -lpq -lpthread -lm -lrt

CC_TARGET   = ysbase.so
INSTALL_PATH= /usr/ysr/

RM = rm -rf
LN = ln
CP = cp -f

SRCS =  *.cpp yscomm/*.cpp
OBJS =  *.o 

all:
		$(CC) -c $(SRCS) $(CC_COMPLIE)

debug:
		$(CC) -c $(SRCS) $(CC_COMPLIE_D)

clean:
		$(RM) *.o
		$(RM) *.d

install:
		$(CP) $(CC_TARGET) $(INSTALL_PATH)/$(CC_TARGET)
