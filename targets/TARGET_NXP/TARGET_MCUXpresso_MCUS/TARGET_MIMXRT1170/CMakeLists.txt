# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_EVK EXCLUDE_FROM_ALL)

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MIMXRT1176_cm7.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MIMXRT1176xxxxx.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MIMXRT1176_cm7.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MIMXRT1170xxxxx.ld)
endif()

add_library(mbed-mimxrt1170-evk INTERFACE)

target_include_directories(mbed-mimxrt1170-evk
    INTERFACE
        .
        device
        drivers
)

target_sources(mbed-mimxrt1170-evk
    INTERFACE
     
        device/system_MIMXRT1176_cm7.c

        drivers/fsl_anatop_ai.c
        drivers/fsl_cache.c
        drivers/fsl_clock.c
        drivers/fsl_common.c
        drivers/fsl_dcdc.c
        drivers/fsl_flexspi_nor_boot.c
        drivers/fsl_flexspi.c
        drivers/fsl_gpio.c
        drivers/fsl_gpt.c
        drivers/fsl_lpadc.c
        drivers/fsl_lpi2c.c
        drivers/fsl_lpspi.c
        drivers/fsl_lpuart.c
        drivers/fsl_pit.c
        drivers/fsl_pmu.c
        drivers/fsl_pwm.c
        drivers/fsl_xbara.c
        
        ${STARTUP_FILE}
)

target_link_libraries(mbed-mimxrt1170-evk
    INTERFACE
        mbed-mcuxpresso-mcus
        mbed-rt1170-evk
        mbed-imx
)

mbed_set_linker_script(mbed-mimxrt1170-evk ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})
