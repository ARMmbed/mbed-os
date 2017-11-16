include ../../makefile_defines.txt

COMPONENT_NAME = fhss_config_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Service_Libs/fhss/fhss_configuration_interface.c \


TEST_SRC_FILES = \
	main.cpp \
        fhssconfigtest.cpp \
        test_fhss_config.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/fhss_stub.c \
        ../../stub/fhss_mac_interface_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

