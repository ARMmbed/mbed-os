include ../makefile_defines.txt

COMPONENT_NAME = sn_grs_unit

#This must be changed manually
SRC_FILES = \
        ../../../../source/libNsdl/src/sn_grs.c

TEST_SRC_FILES = \
	main.cpp \
        sn_grstest.cpp \
        test_sn_grs.c \
        ../stubs/sn_coap_protocol_stub.c \
        ../stubs/sn_coap_parser_stub.c \
        ../stubs/sn_coap_builder_stub.c \
        ../stubs/sn_nsdl_stub.c \
        ../stubs/ns_list_stub.c \

include ../MakefileWorker.mk

