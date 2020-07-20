# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        arm_hal_fhss_timer.cpp
        arm_hal_interrupt.c
        arm_hal_random.c
        arm_hal_timer.cpp
        mbed_lib.json
        ns_event_loop.c
        ns_event_loop_mbed.cpp
        ns_event_loop_mutex.c
        ns_file_system_api.cpp
        ns_hal_init.c

        nvm/nvm_ram.c
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
