include ../../makefile_defines.txt

COMPONENT_NAME = rf_driver_storage_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/rf_driver_storage.c

TEST_SRC_FILES = \
	main.cpp \
	rf_driver_storagetest.cpp \
	test_rf_driver_storage.c \
	../../stub/mac_helper_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/protocol_tun_driver_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/randLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_LOCAL_SOCKET

