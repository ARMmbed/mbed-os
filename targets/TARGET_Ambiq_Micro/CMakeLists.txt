# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_Apollo3 EXCLUDE_FROM_ALL)

add_library(mbed-ambiq-micro INTERFACE)

target_include_directories(mbed-ambiq-micro
    INTERFACE
        sdk/devices
        sdk/utils
)

target_sources(mbed-ambiq-micro
    INTERFACE
        sdk/devices/am_devices_button.c
        sdk/devices/am_devices_led.c

        sdk/utils/am_util_ble.c
        sdk/utils/am_util_debug.c
        sdk/utils/am_util_delay.c
        sdk/utils/am_util_id.c
        sdk/utils/am_util_stdio.c
        sdk/utils/am_util_string.c
        sdk/utils/am_util_time.c
)

target_link_libraries(mbed-ambiq-micro INTERFACE mbed-cmsis-cortex-m)
