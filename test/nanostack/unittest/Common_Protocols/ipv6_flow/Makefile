include ../../makefile_defines.txt

COMPONENT_NAME = ipv6_flow_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Common_Protocols/ipv6_flow.c

TEST_SRC_FILES = \
	main.cpp \
        ipv6_flowtest.cpp \
        test_ipv6_flow.c \
        ../../stub/randLIB_stub.c \
        ../../stub/fnv_hash_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

