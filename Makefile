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
# OR (ArmCC/Keil)
# make CC=ArmCC AR=ArmAR
#
# OR (IAR-ARM)
# make CC=iccarm

#
# External sources from libService
#
SERVLIB_DIR := ../libService
override CFLAGS += -I$(SERVLIB_DIR)/libService/
#override CFLAGS += -I$(SERVLIB_DIR)/libService/platform/

NSDLC_DIR := ../nsdl-c
override CFLAGS += -I$(NSDLC_DIR)/nsdl-c

EVENTLOOP_DIR := ../event-loop
override CFLAGS += -I$(EVENTLOOP_DIR)/nanostack-event-loop/
#override CFLAGS += -I$(EVENTLOOP_DIR)/event-loop/

#override CFLAGS += -I../libService/libService/
#override CFLAGS += -I../nsdl-c/nsdl-c/
#override CFLAGS += -I../event-loop/nanostack-event-loop/

LIB = libcoap-service.a

SRCS := \
	source/coap_service.c \
	source/coap_server.c \
	coap-service-nanomesh/coap_server_impl.c \

override CFLAGS += -DVERSION='"$(VERSION)"'



COAPSERVICE_DIR := ../coap-service
override CFLAGS += -I$(COAPSERVICE_DIR)/coap-service/
override CFLAGS += -I$(COAPSERVICE_DIR)/source/include/

#override CFLAGS += -Isource/coap-service/source/include/
#override CFLAGS += -Icoap-service-nanomesh/include/


include ../libService/toolchain_rules.mk

$(eval $(call generate_rules,$(LIB),$(SRCS)))

.PHONY: release
release:
	7z a coap-service_$(VERSION).zip *.a *.lib include

.PHONY: deploy_to
deploy_to: all
	tar --transform 's,^,coap-service/,' --append -f $(TO) *.a
