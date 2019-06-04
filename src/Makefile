# Name of application
APPLICATION = pcbmaker

# Used Board
BOARD ?= bluepill

# Terminal port
PORT ?= COM1

# RIOT base directory
RIOTBASE ?= $(CURDIR)/../lib/RIOT

# Bin directory
BINDIRBASE = $(CURDIR)/../build

# Custom CPU and Board
#RIOTCPU ?= $(CURDIR)/../../../thirdparty_cpu
#RIOTBOARD ?= $(CURDIR)/../../../thirdparty_boards

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Used Features
FEATURES_REQUIRED += cpp
FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_timer

# Used Modules
USEMODULE += xtimer
USEMODULE += ds18
USEMODULE += servo
USEMODULE += hd44780

# C Flags
# CFLAGS += -DSCHEDSTATISTICS
# CFLAGS += -DDEVELHELP

# C++ extra Flags
CXXEXFLAGS += -std=gnu++11
CXXEXFLAGS += -fno-rtti
CXXEXFLAGS += -fno-exceptions

# Linker Flags
LINKFLAGS += -Wl,--print-memory-usage

include $(RIOTBASE)/Makefile.include