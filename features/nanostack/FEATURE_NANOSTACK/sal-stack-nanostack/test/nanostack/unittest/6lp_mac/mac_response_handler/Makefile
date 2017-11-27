include ../../makefile_defines.txt

COMPONENT_NAME = mac_response_handler_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/MAC/mac_response_handler.c

TEST_SRC_FILES = \
	main.cpp \
        mac_response_handlertest.cpp \
        test_mac_response_handler.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_timer_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/thread_management_if_stub.c \
        ../../stub/adaptation_interface_stub.c \
        

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD

