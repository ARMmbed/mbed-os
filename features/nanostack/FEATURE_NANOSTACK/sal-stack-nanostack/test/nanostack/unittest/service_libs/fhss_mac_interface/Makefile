include ../../makefile_defines.txt

COMPONENT_NAME = fhss_mac_if_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/fhss_mac_interface.c \


TEST_SRC_FILES = \
	main.cpp \
        fhssmaciftest.cpp \
        test_fhss_mac_interface.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/fhss_stub.c \
        ../../stub/fhss_channel_stub.c \
        ../../stub/fhss_platform_stub.c \
        ../../stub/fhss_callbacks_stub.c \
        ../../stub/fhss_beacon_tasklet_stub.c \
        ../../stub/randLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

