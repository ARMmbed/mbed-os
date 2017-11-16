#
# Test that all of our public headers include all of their requisities.
#

HEADER_DIR := ../../nanostack/
HEADERS:= $(shell find $(HEADER_DIR) -type f -name '*.h' | sed 's/$(subst /,\/,$(HEADER_DIR))//' )

CFLAGS += -I$(HEADER_DIR)
SERVLIB_DIR := ../../../libService
EVENTLOOP_DIR := ../../../event-loop
CFLAGS += -I$(SERVLIB_DIR)/libService
CFLAGS += -I$(EVENTLOOP_DIR)/nanostack-event-loop

.PHONY: run
run: $(HEADERS)

# Dummy compile to see if header includes all its requisities
%.h:
	@echo "#include \"$@\"" | gcc -c -o /dev/null $(CFLAGS) -xc -
