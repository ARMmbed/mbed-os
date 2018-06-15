include ../../makefile_defines.txt

COMPONENT_NAME = util_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/common/CellularUtil.cpp

TEST_SRC_FILES = \
	main.cpp \
        utiltest.cpp \
        test_util.cpp \
        ../../stubs/randLIB_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

