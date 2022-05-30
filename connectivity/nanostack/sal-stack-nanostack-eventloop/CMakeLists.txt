# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nanostack-sal_stack-event_loop STATIC EXCLUDE_FROM_ALL)

target_include_directories(mbed-nanostack-sal_stack-event_loop
    PUBLIC
        .
        ./nanostack-event-loop
        ./nanostack-event-loop/platform
        ./source
)

target_sources(mbed-nanostack-sal_stack-event_loop
    PRIVATE
        source/event.c
        source/ns_timeout.c
        source/ns_timer.c
        source/system_timer.c
)

target_link_libraries(mbed-nanostack-sal_stack-event_loop
    PUBLIC
        mbed-core-flags
)