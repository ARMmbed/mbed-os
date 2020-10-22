# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-hal_mbed_cmsis_rtos
    INTERFACE
        .
)

target_sources(mbed-nanostack-hal_mbed_cmsis_rtos
    INTERFACE
        arm_hal_fhss_timer.cpp
        arm_hal_interrupt.c
        arm_hal_random.c
        arm_hal_timer.cpp
        ns_event_loop.c
        ns_event_loop_mbed.cpp
        ns_event_loop_mutex.c
        ns_file_system_api.cpp
        ns_hal_init.c

        nvm/nvm_ram.c
)
