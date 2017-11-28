include ../../makefile_defines.txt

COMPONENT_NAME = rpl_objective_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/RPL/rpl_objective.c

TEST_SRC_FILES = \
	main.cpp \
        rpl_objectivetest.cpp \
        test_rpl_objective.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/rpl_downward_stub.c \
        ../../stub/rpl_upward_stub.c \
        ../../stub/rpl_policy_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

