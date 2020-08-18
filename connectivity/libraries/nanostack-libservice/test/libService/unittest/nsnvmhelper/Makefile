include ../makefile_defines.txt

COMPONENT_NAME = ns_nvm_helper_unit

SRC_FILES = ../../../../source/nvmHelper/ns_nvm_helper.c

TEST_SRC_FILES = main.cpp \
	nsnvmhelpertest.cpp \
	test_ns_nvm_helper.c \
	../stubs/platform_nvm_stub.c \
	../stubs/nsdynmemLIB_stub.c \
	../stubs/mbed_trace_stub.c \
	../stubs/ns_list_stub.c

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

