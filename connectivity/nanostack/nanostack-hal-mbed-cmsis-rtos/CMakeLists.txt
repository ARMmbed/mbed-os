# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nanostack-hal_mbed_cmsis_rtos STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-nanostack-hal_mbed_cmsis_rtos
    PUBLIC
        .
)

target_sources(mbed-nanostack-hal_mbed_cmsis_rtos
    PRIVATE
        arm_hal_fhss_timer.cpp
        arm_hal_interrupt.c
        arm_hal_timer.cpp
        ns_event_loop.c
        ns_event_loop_mbed.cpp
        ns_event_loop_mutex.c
        ns_file_system_api.cpp
        ns_hal_init.c

        nvm/nvm_ram.c
)

target_link_libraries(mbed-nanostack-hal_mbed_cmsis_rtos
    PUBLIC
        mbed-core-flags
        mbed-randlib
        mbed-nanostack-sal_stack-event_loop
        mbed-nanostack-libservice
        mbed-nanostack-sal_stack
    PRIVATE
        mbed-rtos-flags
)