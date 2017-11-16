include ../../makefile_defines.txt

COMPONENT_NAME = mdns_unit

#This must be changed manually
SRC_FILES = \
    ../../../../../source/Service_Libs/mdns/ns_mdns_api.c \
    ../../../../../source/Service_Libs/mdns/ns_fnet_events.c \

TEST_SRC_FILES = \
    main.cpp \
    mdns_tests.cpp \
    test_ns_mdns_api.c \
    ../../stub/fnet_mdns_stub.c \
    ../../stub/fnet_poll_stub.c \
    ../../stub/nsdynmemLIB_stub.c \
    ../../stub/ns_list_stub.c \
    ../../stub/mbed_trace_stub.c \
    ../../stub/event_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

