# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

C_COMPILER=gcc
TARGET_BASE=test_libNsdl
TARGET_EXTENSION=.out

TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)

SRC_FILES=../../../unittest/src/unity.c ../../source/libNsdl/src/sn_grs.c ../../source/libNsdl/src/sn_nsdl.c \
../../../libService/source/libList/ns_list.c Test.c  Test_Runner.c

INC_DIRS=-I../../../unittest/src -I../../nsdl-c/ -I../../source/libNsdl/src/include -I../../../libService/libService

SYMBOLS= -std=c99 -DTEST -DUNITY_EXCLUDE_STDINT_H

LDFLAGS = -L../../ -lnsdl_gcc 

CLEANUP = rm -f build/*.o ; rm -f $(TARGET)

all: clean default

default:
	ruby ../../../unittest/auto/generate_test_runner.rb Test.c  Test_Runner.c
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)
	./$(TARGET)

clean:
	$(CLEANUP)
	
