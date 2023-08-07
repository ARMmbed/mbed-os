# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-raspberry-pi-pico INTERFACE)

target_include_directories(mbed-raspberry-pi-pico
    INTERFACE
        .
) 

target_sources(mbed-raspberry-pi-pico
    INTERFACE
        bs2_default_padded_checksummed.S
)

target_link_libraries(mbed-raspberry-pi-pico INTERFACE mbed-rp2040)

# swd target is an alias of raspberry-pi-pico, just with the USB serial port disabled
add_library(mbed-raspberry-pi-pico-swd ALIAS mbed-raspberry-pi-pico)