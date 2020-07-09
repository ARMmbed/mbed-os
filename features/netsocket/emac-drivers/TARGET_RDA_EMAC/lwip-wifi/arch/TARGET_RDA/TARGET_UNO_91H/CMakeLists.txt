# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(lib)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_sources(mbed-os
    PRIVATE
        src/maclib_task.c
        src/rda5991h_wland.c
        src/rda_sys_wrapper.c
        src/wland_flash.c
        src/wland_flash_wp.c
        src/wland_ota.c
)
