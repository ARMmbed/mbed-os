# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nanostack-coap_service STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-nanostack-coap_service
    PUBLIC
        .
        ./coap-service
        ./source/include
)

target_sources(mbed-nanostack-coap_service
    PRIVATE
        source/coap_connection_handler.c
        source/coap_message_handler.c
        source/coap_security_handler.c
        source/coap_service_api.c
)

target_link_libraries(mbed-nanostack-coap_service
    PUBLIC
        mbed-nanostack-libservice
        mbed-nanostack-sal_stack
        mbed-coap
        mbed-nanostack-sal_stack-event_loop
        mbed-randlib
        mbed-mbedtls
)
