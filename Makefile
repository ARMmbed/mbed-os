SRCS := $(wildcard source/*.c)
LIB := libCmdline.a
EXPORT_HEADERS := mbed-client-cli

include ../exported_rules.mk
