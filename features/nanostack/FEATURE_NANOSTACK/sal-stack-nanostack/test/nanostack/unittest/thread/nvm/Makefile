include ../../makefile_defines.txt

COMPONENT_NAME = thread_nvm_store_unit

#This must be changed manually
SRC_FILES = \
    ../../../../../source/6LoWPAN/Thread/thread_nvm_store.c \
	../../../../../source/Service_Libs/utils/ns_file_system.c \
	../../stub/nsdynmemLIB_stub.c \

TEST_SRC_FILES = \
    ../../stub/mbed_trace_stub.c \
    main.cpp \
    thread_nvm_storetest.cpp \
    test_thread_nvm_store.c \

include ../../MakefileWorker.mk

CLEAN-DIRS := $(foreach dir,$(DIRS),clean-$(dir))
clean: $(CLEAN-DIRS)
	rm -rf stub/*gcov stub/*gcda stubs/*o
	rm -rf results/*
	rm -rf coverages/*
	rm -rf results
	rm -rf coverages
	rm -rf lib/*
	rm -rf objs/*
	rm -f /tmp/fd.txt

.PHONY: $(CLEAN-DIRS)
$(CLEAN-DIRS):
	make -C $(subst clean-,,$@) clean	

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD=1
