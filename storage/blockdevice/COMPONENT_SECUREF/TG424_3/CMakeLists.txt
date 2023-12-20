#-------------------------------------------------------------------------------
# Copyright (c) 2020-2023 Macronix International Co. LTD. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0079 NEW)

add_library(jedec_security_hal STATIC)

target_sources(jedec_security_hal
    PRIVATE
        JEDEC_security_HAL/jedec_security_hal.c
        JEDEC_security_HAL/queue.c
)

target_include_directories(jedec_security_hal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/JEDEC_security_HAL
        ${CMAKE_CURRENT_SOURCE_DIR}/JEDEC_security_HAL/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor_impl
)

add_subdirectory(vendor_impl)