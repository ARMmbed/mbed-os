# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        sources/port/baremetal/wsf_assert.c
        sources/port/baremetal/wsf_buf.c
        sources/port/baremetal/wsf_bufio.c
        sources/port/baremetal/wsf_detoken.c
        sources/port/baremetal/wsf_efs.c
        sources/port/baremetal/wsf_heap.c
        sources/port/baremetal/wsf_msg.c
        sources/port/baremetal/wsf_nvm.c
        sources/port/baremetal/wsf_queue.c
        sources/port/baremetal/wsf_timer.c
        sources/port/baremetal/wsf_trace.c

        sources/util/bda.c
        sources/util/bstream.c
        sources/util/calc128.c
        sources/util/crc32.c
        sources/util/fcs.c
        sources/util/prand.c
        sources/util/print.c
        sources/util/terminal.c
        sources/util/wstr.c
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/util
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/port/baremetal
)
