include ../../makefile_defines.txt

COMPONENT_NAME = fhss_beacon_tasklet

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/fhss_beacon_tasklet.c \


TEST_SRC_FILES = \
	main.cpp \
        fhssbeacontasklettest.cpp \
        test_fhss_beacon_tasklet.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/fhss_stub.c \
        ../../stub/fhss_beacon_stub.c \
        ../../stub/fhss_statistics_stub.c \
        ../../stub/fhss_callbacks_stub.c \
        ../../stub/fhss_platform_stub.c \
        ../../stub/event_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

