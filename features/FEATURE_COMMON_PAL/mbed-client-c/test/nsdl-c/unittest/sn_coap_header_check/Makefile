include ../makefile_defines.txt

COMPONENT_NAME = sn_coap_header_check_unit
SRC_FILES = \
        ../../../../source/libCoap/src/sn_coap_header_check.c

TEST_SRC_FILES = \
	main.cpp \
        libCoap_header_test.cpp \

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

