include ../../makefile_defines.txt

COMPONENT_NAME = etx_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/etx/etx.c \

TEST_SRC_FILES = \
	main.cpp \
        etxtest.cpp \
        test_etx.c \
	../../stub/mle_stub.c \
	../../stub/address_stub.c \
	../../stub/protocol_stats_stub.c \
	../../stub/protocol_core_stub.c \
	../../stub/isqrt_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DEXTRA_CONSISTENCY_CHECKS

