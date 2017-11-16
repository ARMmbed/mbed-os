include ../../makefile_defines.txt

COMPONENT_NAME = thread_commissioning_api_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Thread/thread_commissioning_api.c

TEST_SRC_FILES = \
	main.cpp \
        thread_commissioning_apitest.cpp \
        test_thread_commissioning_api.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/mbed_trace_stub.c \
	../../stub/thread_meshcop_lib_stub.c \
	../../stub/coap_service_api_stub.c \
	../../stub/thread_management_if_stub.c \
	../../stub/common_functions_stub.c \
	../../stub/thread_management_api_stub.c \
	../../stub/ns_sha256_stub.c \
	../../stub/thread_beacon_stub.c \
	../../stub/protocol_core_stub.c \
	../../stub/protocol_6lowpan_stub.c \
	../../stub/thread_common_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD=1

