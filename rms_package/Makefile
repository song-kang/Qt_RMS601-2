CC = g++

CC_INCLUDES = -I./sbase -I./sbase/comm

CC_FLAGS    = $(CC_INCLUDES) -pipe -w -O3 -pipe -D_WITHOUT_AGENT_CHANNEL_
CC_FLAGS_D  = $(CC_INCLUDES) -pipe -Wall -O2 -g -pipe -D_WITHOUT_AGENT_CHANNEL_
CC_COMPLIE  = $(CC_INCLUDES) $(CC_FLAGS) $<
CC_COMPLIE_D= $(CC_INCLUDES) $(CC_FLAGS_D) $<
CC_LINK     = -lpq -lpthread -lm -lrt 

CC_TARGET   = rms_package
INSTALL_PATH= ../bin

RM = rm -rf
LN = ln
CP = cp -f

SRCS =  main.cpp \
		PcapFile.cpp \
		PackageApplication.cpp \
		./sbase/*.cpp \
		./sbase/comm/*.cpp
OBJS =  *.o 

all:
		$(CC) -c $(SRCS) $(CC_COMPLIE)
		$(CC) -o $(CC_TARGET) -g $(OBJS) $(CC_LINK)

debug:
		$(CC) -c $(SRCS) $(CC_COMPLIE_D)
		$(CC) -o $(CC_TARGET) -g $(OBJS) $(CC_LINK)

clean:
		$(RM) *.o
		$(RM) *.d
		$(RM) $(CC_TARGET)

install:
		$(CP) $(CC_TARGET) $(INSTALL_PATH)/$(CC_TARGET)
