include ../../makefile_defines.txt

COMPONENT_NAME = blacklist_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/blacklist/blacklist.c \

TEST_SRC_FILES = \
	main.cpp \
	blacklisttest.cpp \
	test_blacklist.c \
	../../stub/mbed_trace_stub.c \
	../../stub/nsdynmemLIB_stub.c \
	../../stub/common_functions_stub.c \
	../../stub/ns_list_stub.c \
	../../stub/randLIB_stub.c \
	../../stub/address_stub.c \
	../../stub/protocol_core_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DEXTRA_CONSISTENCY_CHECKS
