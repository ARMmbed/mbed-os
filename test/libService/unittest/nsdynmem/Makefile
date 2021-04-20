include ../makefile_defines.txt

COMPONENT_NAME = dynmem_unit
SRC_FILES = \
        ../../../../source/nsdynmemLIB/nsdynmemLIB.c

TEST_SRC_FILES = \
	main.cpp \
    dynmemtest.cpp \
    error_callback.c \
    ../stubs/platform_critical.c \
    ../stubs/ns_list_stub.c

#        ../../../../source/libBits/common_functions.c
#	../stubs/some_stub.c \

# XXX: without this, the CppUTest complains for memory leak even without one.
# The funny thing is that the CppUTest does not find the memory leak on
# this app when there actually is one.
CPPUTEST_USE_MEM_LEAK_DETECTION = Y

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

