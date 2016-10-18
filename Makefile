SRCS := $(wildcard source/*)
LIB := librand.a
EXPORT_HEADERS := mbed-client-randlib

include ../exported_rules.mk
