# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-os-nanostack
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop
        ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_sources(mbed-os-nanostack
    PRIVATE
        source/event.c
        source/ns_timeout.c
        source/ns_timer.c
        source/system_timer.c
)
