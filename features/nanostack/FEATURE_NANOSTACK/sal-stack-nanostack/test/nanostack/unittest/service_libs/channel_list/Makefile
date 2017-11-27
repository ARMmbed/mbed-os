include ../../makefile_defines.txt

COMPONENT_NAME = channel_list_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/channel_list.c \


TEST_SRC_FILES = \
	main.cpp \
        channeltest.cpp \
        test_channel_list.c \
        ../../stub/mbed_trace_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

