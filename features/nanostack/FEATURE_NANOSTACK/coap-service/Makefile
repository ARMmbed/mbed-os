#
# Makefile for combined CoAP Service library
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

#
# External sources from libService
#
SERVLIB_DIR := ../libService
override CFLAGS += -I$(SERVLIB_DIR)/libService/

NANOSTACK_DIR := ../nanostack
override CFLAGS += -I$(NANOSTACK_DIR)/nanostack/

COAP_DIR := ../mbed-coap
override CFLAGS += -I$(COAP_DIR)/


EVENTLOOP_DIR := ../event-loop
override CFLAGS += -I$(EVENTLOOP_DIR)/nanostack-event-loop/

COAPSERVICE_DIR := ../coap-service
override CFLAGS += -I$(COAPSERVICE_DIR)/coap-service/
override CFLAGS += -I$(COAPSERVICE_DIR)/source/include/

LIB = libcoap-service.a

SRCS := \
	source/coap_connection_handler.c \
	source/coap_message_handler.c \
	source/coap_security_handler.c \
	source/coap_service_api.c \

override CFLAGS += -DVERSION='"$(VERSION)"'

include ../libService/toolchain_rules.mk

$(eval $(call generate_rules,$(LIB),$(SRCS)))

.PHONY: release
release:
	7z a coap-service_$(VERSION).zip *.a *.lib include

.PHONY: deploy_to
deploy_to: all
	tar --transform 's,^,coap-service/,' --append -f $(TO) *.a
