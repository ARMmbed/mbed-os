include ../../makefile_defines.txt

COMPONENT_NAME = mle_tlv_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MLE/mle_tlv.c

TEST_SRC_FILES = \
	main.cpp \
        mle_tlvtest.cpp \
        test_mle_tlv.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/mbed_trace_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

