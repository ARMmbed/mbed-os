# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(LINKER_FILE ../TOOLCHAIN_GCC_ARM/memmap_default.ld)

add_library(mbed-raspberrypi-pico INTERFACE)

target_sources(mbed-raspberrypi-pico
    INTERFACE
        board.c
        bs2_default_padded_checksummed.S
)

target_include_directories(mbed-raspberrypi-pico
    INTERFACE
        .
)

mbed_set_linker_script(mbed-raspberrypi-pico ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-raspberrypi-pico INTERFACE mbed-rp2040)
