# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/coap-service
        ${CMAKE_CURRENT_SOURCE_DIR}/source/include
)

target_sources(mbed-os
    PRIVATE
        source/coap_connection_handler.c
        source/coap_message_handler.c
        source/coap_security_handler.c
        source/coap_service_api.c
)
