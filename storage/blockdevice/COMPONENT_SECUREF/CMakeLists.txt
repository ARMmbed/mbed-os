# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-storage-securef
    INTERFACE
        source/SecureFBlockDevice.cpp
    PRIVATE
        platform/plat_secure_flash.cpp
        spi_nor_flash/spi_nor.c
)

target_include_directories(mbed-storage-securef
    INTERFACE
        include
        include/SECUREF
    PRIVATE
        platform/include/
        spi_nor_flash/
)
add_subdirectory(JEDEC_security_HAL)
