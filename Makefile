
# Check if we are compiling for Linux using GCC
ifneq (,$(findstring gcc,$(CC)))
TARGET_IS_LINUX := $(shell $(CC) -dM -E - < /dev/null | grep __linux >/dev/null && echo "yes" || echo "no")
endif

ifeq (yes,$(TARGET_IS_LINUX))
SRCS := $(wildcard linux/*)
else
SRCS := $(wildcard source/*)
endif

LIB := librand.a
EXPORT_HEADERS := mbed-client-randlib

include ../exported_rules.mk
