# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
TARGET_BASE=test_libCoap
TARGET_EXTENSION=.out
TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)

SRC_FILES=../../../unittest/src/unity.c ../../source/libCoap/src/sn_coap_protocol.c ../../source/libCoap/src/sn_coap_parser.c \
../../source/libCoap/src/sn_coap_header_check.c ../../source/libCoap/src/sn_coap_builder.c ../../../libService/source/libList/ns_list.c  \
Test.c  Test_Runner.c

INC_DIRS=-I../../../unittest/src -I../../nsdl-c/ -I../../source/libCoap/src/include -I../../../libService/libService

SYMBOLS=-DTEST -std=gnu99 -DUNITY_EXCLUDE_STDINT_H

CLEANUP = rm -f build/*.o ; rm -f $(TARGET)

all: clean default

default:
	ruby ../../../unittest/auto/generate_test_runner.rb Test.c  Test_Runner.c
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
	
