include ../makefile_defines.txt

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

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DSN_COAP_DUPLICATION_MAX_MSGS_COUNT=4 -DYOTTA_CFG_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE=16 -DENABLE_RESENDINGS=1 -DSN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE=65535
