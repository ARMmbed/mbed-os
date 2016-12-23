include ../makefile_defines.txt

MBED_CLIENT_USER_CONFIG_FILE ?= $(CURDIR)/test_config.h
COMPONENT_NAME = sn_coap_protocol_unit
SRC_FILES = \
        ../../../../source/libCoap/src/sn_coap_protocol.c 

TEST_SRC_FILES = \
	main.cpp \
        libCoap_protocol_test.cpp \
        ../stubs/sn_coap_builder_stub.c \
        ../stubs/sn_coap_parser_stub.c \
        ../stubs/sn_coap_header_check_stub.c \
        ../stubs/ns_list_stub.c \
        ../stubs/randLIB_stub.cpp \

include ../MakefileWorker.mk

# the config is needed for client application compilation too
override CFLAGS += -DMBED_CLIENT_USER_CONFIG_FILE='<$(MBED_CLIENT_USER_CONFIG_FILE)>'
override CXXFLAGS += -DMBED_CLIENT_USER_CONFIG_FILE='<$(MBED_CLIENT_USER_CONFIG_FILE)>'

CPPUTESTFLAGS += -DYOTTA_CFG_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE=16 -DENABLE_RESENDINGS=1 -DSN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE=65535
