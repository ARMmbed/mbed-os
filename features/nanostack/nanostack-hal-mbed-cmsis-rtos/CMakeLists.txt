# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        arm_hal_fhss_timer.cpp
        arm_hal_interrupt.c
        arm_hal_interrupt_private.h
        arm_hal_random.c
        arm_hal_timer.cpp
        ns_event_loop.c
        ns_event_loop.h
        ns_event_loop_mbed.cpp
        ns_event_loop_mutex.c
        ns_event_loop_mutex.h
        ns_hal_init.c
        ns_hal_init.h
        nvm/nvm_ram.c
)
