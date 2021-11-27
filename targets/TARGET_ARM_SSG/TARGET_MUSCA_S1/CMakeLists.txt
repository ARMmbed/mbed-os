# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("TFM" IN_LIST MBED_TARGET_LABELS)
    include(mbed_set_post_build_tfm)

    mbed_post_build_tfm_sign_image(
        ARM_MUSCA_S1
        musca_s1
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_s.bin
    )
endif()

add_library(mbed-arm-musca-s1 INTERFACE)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARMC6/musca_ns.sct)
    set(STARTUP_FILE device/TOOLCHAIN_ARMC6/startup_cmsdk_musca_ns.S)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/musca_ns.ld)
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_cmsdk_musca_ns.S)
endif()

mbed_set_linker_script(mbed-arm-musca-s1 ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_include_directories(mbed-arm-musca-s1
    INTERFACE
        .
        device
        device/drivers
        Libraries
        partition
)

target_sources(mbed-arm-musca-s1
    INTERFACE
        flash_api_qspi.c
        gpio_api_ns.c
        gpio_irq_api.c
        i2c_api.c
        lp_ticker.c
        mbed_serial_platform.c
        pinmap.c
        serial_api.c
        sleep_api.c
        tfm_ioctl_ns_api.c
        tfm_ns_interface.c
        us_ticker.c

        device/device_definition.c
        device/system_core_init.c

        device/drivers/cache_drv.c
        device/drivers/gpio_cmsdk_drv.c
        device/drivers/i2c_ip6510_drv.c
        device/drivers/musca_s1_scc_drv.c
        device/drivers/qspi_ip6514e_drv.c
        device/drivers/timer_cmsdk_drv.c
        device/drivers/timer_gp_drv.c
        device/drivers/uart_pl011_drv.c

        Libraries/mt25ql_flash_lib.c

        ${STARTUP_FILE}
)

target_link_libraries(mbed-arm-musca-s1
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/s_veneers.o
        mbed-arm-ssg
)
