# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-coap
    INTERFACE
        .
        ./mbed-coap
        ./source/include
)

target_sources(mbed-coap
    INTERFACE
        source/sn_coap_builder.c
        source/sn_coap_header_check.c
        source/sn_coap_parser.c
        source/sn_coap_protocol.c
)

target_compile_definitions(mbed-coap
    INTERFACE
        MBED_CONF_COAP_PRESENT=1
)
