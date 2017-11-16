include ../../makefile_defines.txt

COMPONENT_NAME = socket_api_unit
SRC_FILES = \
		../../../../../source/libNET/src/socket_api.c

TEST_SRC_FILES = \
	main.cpp \
	socket_api_test.cpp \
	../../stub/socket_stub.c \
	../../stub/tcp_stub.c \
	../../stub/mbed_trace_stub.c \
	../../stub/buffer_dyn_stub.c \
	../../stub/common_functions_stub.c \
	../../stub/protocol_core_stub.c \
	../../stub/security_lib_stub.c \
	../../stub/address_stub.c \
	../../stub/ipv6_flow_stub.c \
	../../stub/sockbuf_stub.c \
	../../stub/ns_list_stub.c \


include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

