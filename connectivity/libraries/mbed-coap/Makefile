#
# Makefile for COAP library
#

# Define compiler toolchain with CC or PLATFORM variables
# Example (GCC toolchains, default $CC and $AR are used)
# make
#
# OR (Cross-compile GCC toolchain)
# make PLATFORM=arm-linux-gnueabi-
#
# OR (armcc/Keil)
# make CC=armcc AR=ArmAR
#
# OR (IAR-ARM)
# make CC=iccarm

LIB = libmbedcoap.a
SRCS := \
	source/sn_coap_protocol.c \
	source/sn_coap_parser.c \
	source/sn_coap_header_check.c \
	source/sn_coap_builder.c \

override CFLAGS += -DVERSION='"$(VERSION)"'

override CFLAGS += -Isource/include/
SERVLIB_DIR := ../libService
override CFLAGS += -I$(SERVLIB_DIR)/libService
override CFLAGS += -I.

include ../libService/toolchain_rules.mk

$(eval $(call generate_rules,$(LIB),$(SRCS)))

.PHONY: release
release:
	7z a nsdl-c_$(VERSION).zip *.a *.lib include

.PHONY: deploy_to
deploy_to: all
	tar --transform 's,^,libcoap/,' --append -f $(TO) *.a libcoap
