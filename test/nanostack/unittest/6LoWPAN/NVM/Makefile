include ../../makefile_defines.txt

COMPONENT_NAME = nwk_nvm_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/NVM/nwk_nvm.c

TEST_SRC_FILES = \
	main.cpp \
        nwk_nvmtest.cpp \
        test_nwk_nvm.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/address_stub.c \
        ../../stub/etx_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/mesh_stub.c \
        ../../stub/iphc_decompress_stub.c \
        ../../stub/rpl_data_stub.c \
        

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

