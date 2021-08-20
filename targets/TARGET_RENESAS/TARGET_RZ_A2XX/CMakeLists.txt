# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_GR_MANGO EXCLUDE_FROM_ALL)

add_library(mbed-rz-a2xx INTERFACE)

target_include_directories(mbed-rz-a2xx
    INTERFACE
        .
        common
        common/r_cache/inc
        common/r_octabus/inc
        r_can/inc
)

target_sources(mbed-rz-a2xx
    INTERFACE
        analogin_api.c
        can_api.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        pinmap.c
        port_api.c
        pwmout_api.c
        rtc_api.c
        serial_api.c
        sleep.c
        spi_api.c
        us_ticker.c
        USBPhy_RZ_A2.cpp

        common/r_cache/src/lld/r_cache_lld_rza2m.c
        common/r_octabus/src/lld/r_octabus_lld_rza2m_api.c
        common/r_octabus/src/lld/r_octabus_memclk_setup.c
        common/rza_io_regrw.c
        common/target_override_console.cpp        

        r_can/src/r_can_rz.c
)

target_link_libraries(mbed-rz-a2xx INTERFACE mbed-renesas)
