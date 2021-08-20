# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-arm-cm3ds-mps2 INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MPS2.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MPS2.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MPS2.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MPS2.S)
endif()

mbed_set_linker_script(mbed-arm-cm3ds-mps2 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-arm-cm3ds-mps2
    INTERFACE
        .
        device
        device/drivers
        device/drivers/TZ-TRNG/host/src/tztrng_lib/include
        device/drivers/TZ-TRNG/shared/hw/include
        device/drivers/timer_cmsdk
)

target_sources(mbed-arm-cm3ds-mps2
    INTERFACE
        analogin_api.c
        cmsdk_ticker.c
        flash_api.c
        gpio_api.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        mbed_overrides.c
        pinmap.c
        port_api.c
        rtc_api.c
        serial_api.c
        sleep_api.c
        spi_api.c
        trng_api.c
        us_ticker.c

        device/platform_devices.c
        device/system_CMSDK_CM3DS.c

        device/drivers/arm_gpio_drv.c
        device/drivers/arm_mps2_io_drv.c
        device/drivers/arm_uart_drv.c
        device/drivers/dualtimer_cmsdk_drv.c
        device/drivers/rtc_pl031_drv.c
        device/drivers/smsc9220_eth_drv.c
        device/drivers/spi_pl022_drv.c

        device/drivers/TZ-TRNG/host/src/tztrng_lib/llf_rnd.c
        device/drivers/TZ-TRNG/host/src/tztrng_lib/llf_rnd_common.c
        device/drivers/TZ-TRNG/host/src/tztrng_lib/llf_rnd_cont.c
        device/drivers/TZ-TRNG/host/src/tztrng_lib/llf_rnd_trng90b.c
        device/drivers/TZ-TRNG/host/src/tztrng_lib/tztrng_driver.c
        device/drivers/TZ-TRNG/host/src/tztrng_lib/tztrng_pal.c

        device/drivers/timer_cmsdk/timer_cmsdk_drv.c
        ${STARTUP_FILE}
)

target_link_libraries(mbed-arm-cm3ds-mps2
    INTERFACE
        mbed-arm-ssg
)
