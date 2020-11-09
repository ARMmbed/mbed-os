# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nfc
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/acore
)

target_sources(mbed-nfc
    INTERFACE
        source/ac_buffer.c
        source/ac_buffer_builder.c
        source/ac_buffer_reader.c
        source/ac_stream.c
)
