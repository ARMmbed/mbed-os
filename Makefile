ifeq ($(shell uname -s),Linux)
SRCS := $(wildcard linux/*)
else
SRCS := $(wildcard source/*)
endif
LIB := librand.a
EXPORT_HEADERS := mbed-client-randlib

include ../exported_rules.mk
