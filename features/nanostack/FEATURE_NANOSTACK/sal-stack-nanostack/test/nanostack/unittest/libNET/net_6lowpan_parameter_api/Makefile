include ../../makefile_defines.txt

COMPONENT_NAME = net_6lowpan_parameter_api_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libNET/src/net_6lowpan_parameter_api.c

TEST_SRC_FILES = \
	main.cpp \
        net_6lowpan_parameter_apitest.cpp \
        test_net_6lowpan_parameter_api.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/nd_router_object_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_6LOWPAN_ND

