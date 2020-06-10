# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0


target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/atomic
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/atomic_fifo
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/balloc
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/delay
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/experimental_log
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/experimental_log/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/experimental_memobj
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/experimental_section_vars
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/fds
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/fstorage
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/queue
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/spi_mngr
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/strerror
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/twi_mngr
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/util
)

target_sources(mbed-os
    PRIVATE
        libraries/atomic/nrf_atomic.c

        libraries/atomic_fifo/nrf_atfifo.c

        libraries/balloc/nrf_balloc.c

        libraries/experimental_log/src/nrf_log_backend_flash.c
        libraries/experimental_log/src/nrf_log_backend_rtt.c
        libraries/experimental_log/src/nrf_log_backend_serial.c
        libraries/experimental_log/src/nrf_log_backend_uart.c
        libraries/experimental_log/src/nrf_log_default_backends.c
        libraries/experimental_log/src/nrf_log_frontend.c
        libraries/experimental_log/src/nrf_log_str_formatter.c

        libraries/experimental_memobj/nrf_memobj.c

        libraries/experimental_section_vars/nrf_section_iter.c

        libraries/fds/fds.c

        libraries/fstorage/nrf_fstorage.c

        libraries/queue/nrf_queue.c

        libraries/spi_mngr/nrf_spi_mngr.c

        libraries/strerror/nrf_strerror.c

        libraries/twi_mngr/nrf_twi_mngr.c

        libraries/util/app_error.c
        libraries/util/app_error_handler_gcc.c
        libraries/util/app_error_handler_iar.c
        libraries/util/app_error_handler_keil.c
        libraries/util/app_error_weak.c
        libraries/util/app_util_platform.c
        libraries/util/nrf_assert.c
        libraries/util/sdk_mapped_flags.c
)