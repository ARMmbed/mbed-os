# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-coap_service
    INTERFACE
        .
        ./coap-service
        ./source/include
)

target_sources(mbed-nanostack-coap_service
    INTERFACE
        source/coap_connection_handler.c
        source/coap_message_handler.c
        source/coap_security_handler.c
        source/coap_service_api.c
)

target_link_libraries(mbed-nanostack
    INTERFACE
        mbed-coap
)
