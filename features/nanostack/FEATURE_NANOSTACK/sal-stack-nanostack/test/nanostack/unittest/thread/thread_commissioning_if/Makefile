include ../../makefile_defines.txt

COMPONENT_NAME = thread_thread_commissioning_if_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Thread/thread_commissioning_if.c \

TEST_SRC_FILES = \
	main.cpp \
        thread_commissioning_iftest.cpp \
        test_thread_commissioning_if.c \
	../../stub/mbed_trace_stub.c \
    ../../stub/common_functions_stub.c \
	../../stub/thread_joiner_application_stub.c \
	../../stub/protocol_core_stub.c \
	../../stub/thread_management_if_stub.c \
	../../stub/thread_bootstrap_stub.c \
	../../stub/thread_network_data_storage_stub.c \
	../../stub/nsdynmemLIB_stub.c \
	../../stub/thread_common_stub.c \
	../../stub/randLIB_stub.c \
	../../stub/mac_pairwise_key_stub.c \
	../../stub/mac_security_interface_stub.c \
	../../stub/thread_management_server_stub.c \
	../../stub/timeout_stub.c \
	../../stub/mac_helper_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD

