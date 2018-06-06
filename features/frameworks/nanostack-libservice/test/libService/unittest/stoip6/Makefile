include ../makefile_defines.txt

COMPONENT_NAME = stoip6_unit
SRC_FILES = \
        ../../../../source/libip6string/stoip6.c

TEST_SRC_FILES = \
	main.cpp \
        stoip6test.cpp \
        ../../../../source/libBits/common_functions.c 
#	../stubs/some_stub.c \

# XXX: without this, the CppUTest complains for memory leak even without one.
# The funny thing is that the CppUTest does not find the memory leak on 
# this app when there actually is one.
CPPUTEST_USE_MEM_LEAK_DETECTION = N

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

