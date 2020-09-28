# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-os-wifi-esp8266 OBJECT)

mbed_configure_component(mbed-os-wifi-esp8266)

add_subdirectory(esp8266-driver)

target_link_libraries(mbed-os-wifi-esp8266 mbed-os)
