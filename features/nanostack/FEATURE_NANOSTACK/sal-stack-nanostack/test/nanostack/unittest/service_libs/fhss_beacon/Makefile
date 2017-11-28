include ../../makefile_defines.txt

COMPONENT_NAME = fhss_beacon

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/fhss_beacon.c \


TEST_SRC_FILES = \
	main.cpp \
        fhssbeacontest.cpp \
        test_fhss_beacon.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/fhss_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

