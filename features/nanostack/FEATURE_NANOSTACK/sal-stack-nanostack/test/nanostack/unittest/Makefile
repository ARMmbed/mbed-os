#scan for folders having "Makefile" in them and remove 'this' to prevent loop
ifeq ($(OS),Windows_NT)
all:
clean:
else
DIRS := $(filter-out ./, $(sort $(dir $(shell find . -name 'Makefile'))))

all: $(DIRS)

.PHONY: $(DIRS)
$(DIRS):
	make -C $@ vtest

CLEAN-DIRS := $(foreach dir,$(DIRS),clean-$(dir))
clean: $(CLEAN-DIRS)
	rm -rf stub/*gcov stub/*gcda stubs/*o
	rm -rf results/*
	rm -rf coverages/*
	rm -rf results
	rm -rf coverages

.PHONY: $(CLEAN-DIRS)
$(CLEAN-DIRS):
	make -C $(subst clean-,,$@) clean

endif

