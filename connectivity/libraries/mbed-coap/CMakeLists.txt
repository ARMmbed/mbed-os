# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-coap STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-coap
    PUBLIC
        .
        ./mbed-coap
        ./source/include
)

target_sources(mbed-coap
    PRIVATE
        source/sn_coap_builder.c
        source/sn_coap_header_check.c
        source/sn_coap_parser.c
        source/sn_coap_protocol.c
)

target_compile_definitions(mbed-coap
    PUBLIC
        MBED_CONF_COAP_PRESENT=1
)

target_link_libraries(mbed-coap
    PUBLIC
        mbed-nanostack-libservice
        mbed-core-flags
        mbed-randlib
)
