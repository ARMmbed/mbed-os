# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-emac-uno-91h)

add_subdirectory(lib)

target_include_directories(mbed-emac-uno-91h
    PUBLIC
        .
        ./inc
)

target_sources(mbed-emac-uno-91h
    PRIVATE
        src/maclib_task.c
        src/rda5991h_wland.c
        src/rda_sys_wrapper.c
        src/wland_flash.c
        src/wland_flash_wp.c
        src/wland_ota.c
)

target_link_libraries(mbed-emac-uno-91h INTERFACE mbed-emac-rda)
