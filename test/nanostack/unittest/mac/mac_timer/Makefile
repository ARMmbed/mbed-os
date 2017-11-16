include ../../makefile_defines.txt

COMPONENT_NAME = mac_timer_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_timer.c

TEST_SRC_FILES = \
	main.cpp \
        mac_timertest.cpp \
        test_mac_timer.c \
        ../../stub/event_stub.c \
        ../../stub/ns_timer_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/sw_mac_stub.c \
        ../../stub/mac_pd_sap_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

