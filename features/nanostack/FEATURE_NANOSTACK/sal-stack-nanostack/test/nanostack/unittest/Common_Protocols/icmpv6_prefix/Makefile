include ../../makefile_defines.txt

COMPONENT_NAME = icmpv6_prefix_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Common_Protocols/icmpv6_prefix.c

TEST_SRC_FILES = \
	main.cpp \
        icmpv6_prefixtest.cpp \
        test_icmpv6_prefix.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

