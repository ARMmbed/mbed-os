# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("Apollo3" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_Apollo3)
endif()

target_include_directories(mbed-core
    INTERFACE
        sdk/devices
        sdk/utils
)

target_sources(mbed-core
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
