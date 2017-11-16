include ../../makefile_defines.txt

COMPONENT_NAME = net_short_address_extension_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libNET/src/net_short_address_extension.c

TEST_SRC_FILES = \
	main.cpp \
        net_short_address_extensiontest.cpp \
        test_net_short_address_extension.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/protocol_6lowpan_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

