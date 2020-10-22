# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(lib)

target_include_directories(mbed-emac
    PUBLIC
        .
        ./inc
)

target_sources(mbed-emac
    PRIVATE
        src/maclib_task.c
        src/rda5991h_wland.c
        src/rda_sys_wrapper.c
        src/wland_flash.c
        src/wland_flash_wp.c
        src/wland_ota.c
)
