include ../../makefile_defines.txt

COMPONENT_NAME = fhss_channel

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/fhss_channel.c \


TEST_SRC_FILES = \
	main.cpp \
        fhsschanneltest.cpp \
        test_fhss_channel.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/channel_list_stub.c \
        ../../stub/fhss_beacon_stub.c \
        ../../stub/fhss_stub.c \
        ../../stub/fhss_callbacks_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

