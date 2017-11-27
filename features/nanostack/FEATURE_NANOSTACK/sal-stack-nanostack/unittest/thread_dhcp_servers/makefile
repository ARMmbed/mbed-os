C_COMPILER=gcc
TARGET_BASE=test_thread_dhcp_servers
TARGET_EXTENSION=.exe
TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)
UNIT_TEST_BASE = ../../../unittest/src

SRC_FILES = $(UNIT_TEST_BASE)/unity.c Test_Runner.c ../unity_improvement/unity_improved.c 
## Test files and simulated interfaces
SRC_FILES += Test.c
SRC_FILES += ../simulations/sim_nanostack.c ../simulations/sim_address.c ../simulations/sim_socket_api.c
## libservice dependencies
SRC_FILES += ../../../libService/source/libList/ns_list.c
SRC_FILES += ../../../libService/source/libTrace/ns_trace.c
SRC_FILES += ../../../libService/source/libip6string/ip6tos.c
SRC_FILES += ../../../libService/source/libBits/common_functions.c
## nanostack files
TEST_SRC_FILES += ../../source/6LoWPAN/Thread/thread_routerid_server.c 
TEST_SRC_FILES += ../../source/libDHCPv6/dhcp_service_api.c 
TEST_SRC_FILES += ../../source/libDHCPv6/libDHCPv6.c 

SRC_FILES += $(TEST_SRC_FILES)
INC_DIRS = -I$(UNIT_TEST_BASE) -I../../source/6LoWPAN/Thread/ -I../../nanostack -I../../source -I../../../event-loop/nanostack-event-loop -I../../../libService/libService -I../simulations -I../unity_improvement

SYMBOLS=-DTEST -std=gnu99 -pedantic -D'MEM_ALLOC=malloc' -D'MEM_FREE=free' -fprofile-arcs -ftest-coverage -O0

CLEANUP=rm -f Test_Runner.c&&rm -f *.gcno&&rm -f *.gcda&&rm -f *.gcov&&rm -f $(TARGET)

all: clean default

run: clean default execute

cov: clean default execute coverage

default:
	ruby $(UNIT_TEST_BASE)/../auto/generate_test_runner.rb Test.c Test_Runner.c
	$(C_COMPILER) -g -O0 -DFEA_TRACE_SUPPORT $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)

execute:
	./$(TARGET)

TEST_FILES      :=  $(notdir $(TEST_SRC_FILES))
coverage:
	gcov -r $(TEST_FILES)&&rm -f *.gcno&&rm -f *.gcda

clean:
	$(CLEANUP)
