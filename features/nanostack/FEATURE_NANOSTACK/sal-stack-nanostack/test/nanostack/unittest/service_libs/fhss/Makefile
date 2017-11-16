include ../../makefile_defines.txt

COMPONENT_NAME = fhss_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/fhss.c \


TEST_SRC_FILES = \
	main.cpp \
        fhsstest.cpp \
        test_fhss.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/channel_list_stub.c \
        ../../stub/fhss_config_stub.c \
        ../../stub/fhss_channel_stub.c \
        ../../stub/fhss_platform_stub.c \
        ../../stub/fhss_mac_interface_stub.c \
        ../../stub/fhss_beacon_tasklet_stub.c \
        ../../stub/fhss_beacon_stub.c \
        ../../stub/fhss_statistics_stub.c \
        ../../stub/ns_timer_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/event_stub.c \
        ../../stub/fhss_callbacks_stub.c \
        ../../stub/ns_list_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

