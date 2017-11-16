include ../../makefile_defines.txt

COMPONENT_NAME = pana_relay_table_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Security/PANA/pana_relay_table.c

TEST_SRC_FILES = \
	main.cpp \
        pana_relay_tabletest.cpp \
        test_pana_relay_table.c \
        ../../stub/nsdynmemLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DPANA

