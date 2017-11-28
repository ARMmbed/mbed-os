include ../../makefile_defines.txt

COMPONENT_NAME = sha256_unit

SRC_FILES = \
        ../../../../../source/Service_Libs/SHA256_Lib/ns_sha256.c

# use GCC by default, but allow it to be overridden
ifndef $(CC)
   CC = gcc
endif

TEST_SRC_FILES = \
        main.cpp \
        shalibtest.cpp \
        test_shalib.c \

include ../../MakefileWorker.mk

LD_LIBRARIES += \
		../../../../../../libService/$(addsuffix _$(CC), libservice).a
