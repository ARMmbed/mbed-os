include ../../makefile_defines.txt

COMPONENT_NAME = thread_management_client_unit

#This must be changed manually
SRC_FILES =         ../../../../../source/6LoWPAN/Thread/thread_management_client.c

TEST_SRC_FILES = \
	main.cpp \
        thread_management_clienttest.cpp \
        test_thread_management_client.c \
        ../../stub/coap_service_api_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/socket_api_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/thread_meshcop_lib_stub.c \
        ../../stub/thread_bootstrap_stub.c \
        ../../stub/thread_management_if_stub.c \
        ../../stub/thread_joiner_application_stub.c \
        ../../stub/thread_network_data_lib_stub.c \


include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD=1

