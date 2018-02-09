# LEDss makefile
#

HOME_FLDR = /home/ettiene
MAINFILE=ERPiModule
TARGET=$(MAINFILE).so


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
# Programs to generate the executables
# 1 -> cross tools from emdebian
# 2 -> Linaro cross tools from Derek Molloy git 
#==============================================================================
COMPILER = 2
LINARO_PATH=$(HOME_FLDR)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
ifeq ($(COMPILER),1)
CC=arm-linux-gnueabihf-g++
else
CC=$(LINARO_PATH)/arm-linux-gnueabihf-g++
endif

#==============================================================================
# Compilation and linkage flags
#==============================================================================
CPPFLAGS  = -c
CPPFLAGS += -o3
CPPFLAGS += -Wall
CPPFLAGS += -Wextra
CPPFLAGS += -shared
#CPPFLAGS += -static
CPPFLAGS += -std=c++11
DEBUG = -g
LDFLAGS = -lpython3.5m
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
#LIBS += -lc
#LIBS += -lmylib

#==============================================================================
# Include paths
#==============================================================================
INCLUDES  = -I$(CMN_DEV_PATH)/gpio
INCLUDES += -I$(CMN_PRT_PATH)
INCLUDES += -I$(CMN_INC_PATH)
INCLUDES += -I/usr/local/include
INCLUDES += -I/usr/bin

#==============================================================================
# Source Files
#==============================================================================
SRC  = $(MAINFILE).cpp
#SRC += $(CMN_DEV_PATH)/gpio/GPIO.cpp

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
	rm -f *.o *.exe *~
	rm -f $(CMN_DEV_PATH)/gpio/*.o

#==============================================================================
# Compile application
#==============================================================================
app: $(TARGET) 
	@echo Done!!!
  
#==============================================================================
# Make target (Link everything)
#==============================================================================
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LIB_PATHS) $(LIBS)

   
.PHONY: all clean app

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
  
info :
	@echo 
	@echo $(CMP_INFO_TEXT)
	@echo $(OS)
	@$(CC) --version
	@echo $(MSG_INFO_END)

	