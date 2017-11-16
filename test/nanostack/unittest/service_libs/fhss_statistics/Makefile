include ../../makefile_defines.txt

COMPONENT_NAME = fhss_statistics_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/fhss_statistics.c \


TEST_SRC_FILES = \
	main.cpp \
        fhssstatisticstest.cpp \
        test_fhss_statistics.c \


include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

