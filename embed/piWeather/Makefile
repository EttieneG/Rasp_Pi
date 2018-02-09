# DHT temperature sensor makefile
# This makefile can be used to compile the example code for wiringPi
# by specifying the MAINFILE at the top

HOME_FLDR = /home/ettiene
#MAINFILE=main
#MAINFILE=main_python
MAINFILE=main_php

#TARGET=$(MAINFILE).exe
TARGET=./exec/pi_weather.exe
#TARGET=pi_weather.so

CMP_INFO_TEXT  = ============================== Compiler Info ===========================
MSG_INFO_END   = ========================================================================
CPP_CHECK_TEXT = ============================== CPP Check Info ==========================

#==============================================================================
# Common software paths
#==============================================================================
CMN_DEV_PATH = ./../common/device
CMN_PRT_PATH = ./../common/protocol
CMN_INC_PATH = ./../common/include

#==============================================================================
# Application software paths
#==============================================================================
APP_DEV_PATH = ./device
APP_PRT_PATH = ./protocol
APP_INC_PATH = ./include

#==============================================================================
# Programs to generate the executables
# 1 -> cross tools from emdebian
# 2 -> Linaro cross tools from Derek Molloy git 
# 3 -> Native g++ (Use when compiling on embeded system)
#==============================================================================
COMPILER = 2
LINARO_PATH=$(HOME_FLDR)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
ifeq ($(COMPILER),1)
CC=arm-linux-gnueabihf-g++
endif
ifeq ($(COMPILER),2)
CC=$(LINARO_PATH)/arm-linux-gnueabihf-g++
endif
ifeq ($(COMPILER),3)
CC=g++
endif

#==============================================================================
# Compilation and linkage flags
#==============================================================================
CPPFLAGS  = -c
CPPFLAGS += -o3
CPPFLAGS += -Wall
CPPFLAGS += -Wextra
CPPFLAGS += -pedantic
#CPPFLAGS += -fPIC
#CPPFLAGS += -static
CPPFLAGS += -std=c++11
CPPFLAGS += -DLINUX
DEBUG = -g
LDFLAGS = -pthread
# LDFLAGS += -shared
# DEBUG==DNDEBUG -O2

#==============================================================================
# Library paths (In addition to /usr/lib)
#==============================================================================
LIB_PATHS = -L/usr/local/lib
#LIB_PATHS += -L/home/prjlib
#LIB_PATHS += -L../lib

#==============================================================================
# Libraries to link into the executable
#==============================================================================
LIBS  = -lm
LIBS += -lwiringPi
LIBS += -lpthread
#LIBS += -lpython2.7
#LIBS += -lboost_python
#LIBS += -lc
#LIBS += -lmylib

#==============================================================================
# Include paths
#==============================================================================
INCLUDES  = -I$(CMN_DEV_PATH)
INCLUDES += -I$(CMN_PRT_PATH)
INCLUDES += -I$(CMN_INC_PATH)
INCLUDES += -I$(APP_INC_PATH)
INCLUDES += -I$(APP_DEV_PATH)
INCLUDES += -I/usr/local/include
#INCLUDES += -I/usr/include/python2.7/
#INCLUDES += -I/usr/include/
#INCLUDES += -I/usr/include/boost/
#INCLUDES += -I/usr/include/boost/python/

#==============================================================================
# Source Files
#==============================================================================
SRC  = $(MAINFILE).cpp
SRC += $(CMN_DEV_PATH)/lcd_driver.cpp
SRC += $(APP_DEV_PATH)/dht.cpp

#==============================================================================
# Objects List
#==============================================================================
OBJS = $(SRC:.cpp=.o) # Replace all .cpp with .o in the SRC macro

#==============================================================================
# Make all target
#==============================================================================
all: clean app info 

#==============================================================================
# Clean all
#==============================================================================
clean:
	rm -f *.o ./exec/*.exe *~
	rm -f $(APP_DEV_PATH)/*.o
	rm -f $(CMN_DEV_PATH)/*.o
	rm -f $(APP_PRT_PATH)/*.o

#==============================================================================
# Compile application
#==============================================================================
app: $(TARGET)
	@echo
	@echo $(MSG_INFO_END)
	@echo
	@echo Done compiling $(TARGET)!!!
  
#==============================================================================
# Make target (Link everything)
#==============================================================================
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LIB_PATHS) $(LIBS)
#$(TARGET): $(OBJS)
#	$(CC) $(LDFLAGS) $(INCLUDES) $(OBJS) $(LIB_PATHS) $(LIBS) -o $(TARGET)
   
.PHONY: all clean app info

#==============================================================================
# Compile objects for each source file
#==============================================================================
%.o: %.cpp
	echo $<
	$(CC) $(CPPFLAGS) $< -o $@ $(INCLUDES)

check:
	@echo 
	@echo $(CPP_CHECK_TEXT)
	cppcheck --enable=all *.cpp *.h*
	@echo $(MSG_INFO_END)
  
info:
	@echo 
	@echo $(CMP_INFO_TEXT)
	@echo $(OS)
	@$(CC) --version
	@echo $(MSG_INFO_END)

	