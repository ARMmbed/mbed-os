# Copyright (c) 2015-2017, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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
override CFLAGS += -DHAVE_DEBUG

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
