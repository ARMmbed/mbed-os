include ../makefile_defines.txt

COMPONENT_NAME = sn_coap_parser_unit

#This must be changed manually
SRC_FILES = \
        ../../../../source/libCoap/src/sn_coap_parser.c

TEST_SRC_FILES = \
	main.cpp \
        sn_coap_parsertest.cpp \
        test_sn_coap_parser.c \

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

