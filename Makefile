#
# Makefile for NanoStack
#
# Define compiler toolchain with CC variable
# Example (GCC toolchains, default $CC and $AR are used)
# make
#
# OR (armcc/Keil)
# make CC=armcc AR=ArmAR
#
# OR (IAR-ARM)
# make CC=iccarm

LIB = libnanostack.a

# If configuration is not specified, use generic
ifeq (,$(CONFIG))
CONFIG=generic
endif

include sources.mk
include include_dirs.mk
override CFLAGS += -DNSCONFIG=$(CONFIG)

COAP_SERVICE_LIB := ../coap-service
SERVLIB_DIR := ../libService
EVENTLOOP_DIR := ../event-loop
NSDL_DIR := ../nsdl-c
COAP_DIR := ../mbed-coap
MDNS_DIR := source/Service_Libs/mdns
MDNS_SRV_DIR := source/Service_Libs/mdns/fnet/fnet_stack/services
MBEDTLS_DIR := ../mbedtls
override CFLAGS += -I$(MDNS_DIR) -I$(MDNS_DIR)/fnet -I$(MDNS_DIR)/fnet/fnet_stack -I$(MDNS_DIR)/fnet/fnet_stack/stack
override CFLAGS += -I$(MDNS_SRV_DIR) -I$(MDNS_SRV_DIR)/poll -I$(MDNS_SRV_DIR)/mdns -I$(MDNS_SRV_DIR)/serial
override CFLAGS += -I$(SERVLIB_DIR)/libService
override CFLAGS += -I$(EVENTLOOP_DIR)/nanostack-event-loop
override CFLAGS += -I$(NSDL_DIR)/nsdl-c
override CFLAGS += -I$(COAP_DIR)
override CFLAGS += -I$(COAP_SERVICE_LIB)/coap-service
override CFLAGS += -I$(MBEDTLS_DIR)/include -I$(MBEDTLS_DIR)/crypto/include
override CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
override CFLAGS += $(addprefix -D,$(FLAGS))


#
# Define compiler toolchain
#
include ../libService/toolchain_rules.mk

$(eval $(call generate_rules,$(LIB),$(SRCS)))

.PHONY: $(LIB)

# Extend default clean rule
clean: clean-extra

TEST_FOLDER := ./test/
UNITTESTS := $(sort $(dir $(wildcard $(TEST_FOLDER)*/unittest/*)))
TESTDIRS := $(UNITTESTS:%=build-%)
CLEANTESTDIRS := $(UNITTESTS:%=clean-%)
COVERAGEFILE := ./lcov/coverage.info

$(TESTDIRS):
	@make -C $(@:build-%=%)

$(CLEANTESTDIRS):
	@make -C $(@:clean-%=%) clean
	@rm -fr results

.PHONY: release
release:
	7z a nanostack_$(VERSION).zip *.a *.lib include

.PHONY: deploy_to
deploy_to: all
	tar --transform 's,^,nanostack/,' --append -f $(TO) *.a

.PHONY: test
test: $(TESTDIRS)
	@rm -rf lcov
	@rm -rf results
	@mkdir -p lcov
	@mkdir -p results
	@./xsl_script.sh
	@cp junit_xsl.xslt results/.
	@xsltproc -o results/index.html results/junit_xsl.xslt results/index.xml
	@lcov -d test/. -c -o $(COVERAGEFILE)
	@lcov -q -r $(COVERAGEFILE) "/usr*" -o $(COVERAGEFILE)
	@lcov -q -r $(COVERAGEFILE) "/test*" -o $(COVERAGEFILE)
	@lcov -q -r $(COVERAGEFILE) "/libService*" -o $(COVERAGEFILE)
	@genhtml -q $(COVERAGEFILE) --show-details --prefix $(CURDIR:%/applications/nanostack=%) --output-directory lcov
	@echo Nanostack unit tests built, see results/index.html and lcov/index.html
	@sed -n 's/.*\(failures: [0-9]\+\).*/\1/p' results/index.html

clean-extra: $(CLEANTESTDIRS)
	@rm -rf lcov/

#
# Releasing the binary for mbed module
#
# This must be runned from fully cloned Nanomesh application repository
# with all submodules.
#
# 1. `make mbed-release-build`
#	You can also separate this step per toolchain:
#	`make mbed-release-build-GCC`
#	`make mbed-release-build-ARM`
#	`make mbed-release-build-IAR`
# 2. `make mbed-release`
# 3. TBD: importing to mbed OS

TARGET_DIR:=release
CONFIGURATIONS_TO_BUILD:=lowpan_border_router lowpan_host lowpan_router nanostack_full thread_border_router thread_router thread_end_device ethernet_host ws_border_router ws_router
TOOLCHAINS_TO_BUILD:=GCC ARM ARMC6 IAR
CORES_TO_BUILD:=Cortex-M0 Cortex-M3

CC_GCC:=arm-none-eabi-gcc
CC_ARM:=armcc
CC_ARMC6=armclang
CC_IAR:=iccarm
LIB_SUFFIX_GCC:=a
LIB_SUFFIX_ARM:=ar
LIB_SUFFIX_ARMC6:=ar
LIB_SUFFIX_IAR:=a

.PHONY: mbed-release
mbed-release: mbed-release-copy
	cd release; git add -A
	(TAG=$$(git describe --tag); cd release; git commit -m "$$TAG")
	git add release; git commit -m "Update release pointer"

.PHONY: mbed-release-copy
mbed-release-copy: mbed-release-pre-clean
	cp mbed_lib.json $(TARGET_DIR)
	cp -r nanostack $(TARGET_DIR)
	cp -r docs README.md $(TARGET_DIR)
	cp -r Doxyfile DOXYGEN_FRONTPAGE.md doxygen $(TARGET_DIR)
	cp mkdocs.yml $(TARGET_DIR)
	cp binary_release_template_lisense $(TARGET_DIR)/LICENSE
	cp binary_release_template_lisense_txt $(TARGET_DIR)/LICENSE-permissive-binary-license-1.0.txt
	for header in `find release/nanostack -name \*.h`;do awk 'BEGIN{found=0;}{if(!found){print >"tmp-head";}else{print >"tmp-tail"}}/Copyright/{found=1;}' $$header; cat tmp-head binary_release_template_header tmp-tail > tmp && mv tmp $$header; rm tmp-head tmp-tail; done

# Template for generating rules for all configuration options
# $(1) is Toolchain, GCC, ARM or IAR
# $(2) is Configuration
# $(3) is CPU variant
#
# Here be dragons... This is dangerous territory. Touch only if you have a beard.
#
define mbed-rel-build-template
mbed-release-build: $(1)-$(2)-$(3)-build
mbed-release-build-$(1): $(1)-$(2)-$(3)-build
.PHONY: $(1)-$(2)-build
$(1)-$(2)-$(3)-build: export-headers
	@echo Build $(2) on $(1) for $(3)
	make CC=$(CC_$(1)) CONFIG=$(2) CPU=$(3) APPEND_LIB_NAME=1 CFLAGS="-DNS_USE_EXTERNAL_MBED_TLS -I../mbedtls/include/ -I../crypto/include/"

# Generate target directory name
# Like: FEATURE_NANOSTACK/FEATURE_LOWPAN_ROUTER/TOOLCHAIN_ARM/TARGET_CORTEX_M0P
$(1)-$(2)-$(3)-target-dir:=$(TARGET_DIR)/FEATURE_$(shell echo $(2) | tr '[:lower:]' '[:upper:]')/TOOLCHAIN_$(1)/TARGET_$(shell echo $(3) | tr '[:lower:]' '[:upper:]' | tr - _ | sed 's/CORTEX_//')
# Cortex-M3 is also used for M4...M7, there is alias for them
ifeq (Cortex-M3,$(3))
$(1)-$(2)-$(3)-alt-target-dir:=$(TARGET_DIR)/FEATURE_$(shell echo $(2) | tr '[:lower:]' '[:upper:]')/TOOLCHAIN_$(1)/TARGET_RTOS_M4_M7
endif
# M0 is used as M0 and M0+. There is alias for them
ifeq (Cortex-M0,$(3))
$(1)-$(2)-$(3)-target-dir:=$(TARGET_DIR)/FEATURE_$(shell echo $(2) | tr '[:lower:]' '[:upper:]')/TOOLCHAIN_$(1)/TARGET_LIKE_CORTEX_M0
endif
mbed-release-copy: $(1)-$(2)-$(3)-copy
$(1)-$(2)-$(3)-copy: mbed-release-pre-clean
	mkdir -p $$($(1)-$(2)-$(3)-target-dir)
	cp libnanostack_$(CC_$(1))_$(3)_$(2).* $$($(1)-$(2)-$(3)-target-dir)/libnanostack.$(LIB_SUFFIX_$(1))
ifneq (,$$($(1)-$(2)-$(3)-alt-target-dir))
	mkdir -p $$($(1)-$(2)-$(3)-alt-target-dir)
	cp libnanostack_$(CC_$(1))_$(3)_$(2).* $$($(1)-$(2)-$(3)-alt-target-dir)/libnanostack.$(LIB_SUFFIX_$(1))
endif
endef
# Now generate rules
$(foreach tool,$(TOOLCHAINS_TO_BUILD),$(foreach cpu,$(CORES_TO_BUILD),$(foreach cfg,$(CONFIGURATIONS_TO_BUILD),$(eval $(call mbed-rel-build-template,$(tool),$(cfg),$(cpu))))))

#
# End of dangerous territory
#


# Run export-headers before build
.PHONY: export-headers
export-headers:
	make -C ../ -f build_external_libs.mk export-headers

.PHONY: mbed-release-pre-clean
mbed-release-pre-clean:
	rm -rf $(TARGET_DIR)/*

.PHONY: mbed-release-build
mbed-release-build:
	@echo Done
