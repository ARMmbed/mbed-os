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
include ../makefile_defines.txt

COMPONENT_NAME = coap_connection_handler_unit

#This must be changed manually
SRC_FILES = \
	../../../../source/coap_connection_handler.c

TEST_SRC_FILES = \
	main.cpp \
	coap_connection_handlertest.cpp \
	test_coap_connection_handler.c \
	../stub/mbed_trace_stub.c \
	../stub/ns_list_stub.c \
	../stub/ns_timer_stub.c \
	../stub/timeout_stub.c \
	../stub/nsdynmemLIB_stub.c \
	../stub/socket_api_stub.c \
	../stub/coap_security_handler_stub.c \
	../stub/coap_service_api_stub.c \
	../stub/coap_message_handler_stub.c \

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD

