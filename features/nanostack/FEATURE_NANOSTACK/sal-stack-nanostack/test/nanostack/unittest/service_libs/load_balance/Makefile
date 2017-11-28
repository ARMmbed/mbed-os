include ../../makefile_defines.txt

COMPONENT_NAME = load_balance_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/load_balance/load_balance.c \

TEST_SRC_FILES = \
	main.cpp \
        loadbalance_test.cpp \
		../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/event_stub.c \
        ../../stub/randLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DEXTRA_CONSISTENCY_CHECKS

