# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

#include(${MBED_OS_BASE}/util.cmake)

target_sources(mbed-os
    PRIVATE
        fsl_common.c

        api/analogin_api.c
        api/analogout_api.c
        api/dma_api.c
        api/dma_api_hal.h
        api/flash_api.c
        api/gpio_api.c
        api/gpio_irq_api.c
        api/gpio_object.h
        api/i2c_api.c
        api/lp_ticker.c
        api/objects.h
        api/PeripheralPins.h
        api/pinmap.c
        api/PortNames.h
        api/port_api.c
        api/qspi_api.c
        api/rtc_api.c
        api/sleep.c
)

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os
    PUBLIC
        api
)
