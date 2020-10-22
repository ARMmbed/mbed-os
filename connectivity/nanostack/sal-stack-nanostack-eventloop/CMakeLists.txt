# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-nanostack-sal_stack-event_loop
    INTERFACE
        .
        ./nanostack-event-loop
        ./nanostack-event-loop/platform
        ./source
)

target_sources(mbed-nanostack-sal_stack-event_loop
    INTERFACE
        source/event.c
        source/ns_timeout.c
        source/ns_timer.c
        source/system_timer.c
)
