# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nu-pfm-m2351-npsa-ns INTERFACE)
target_link_libraries(mbed-nu-pfm-m2351-npsa-ns
    INTERFACE
        TARGET_NU_PFM_M2351_NPSA_NS/TARGET_NU_PREBUILD_SECURE/cmse_lib.o
)

add_library(mbed-nu-pfm-m2351-ns INTERFACE)
target_link_libraries(mbed-nu-pfm-m2351-ns
    INTERFACE
        TARGET_NU_PFM_M2351_NS/TARGET_NU_PREBUILD_SECURE/cmse_lib.o
)

add_library(mbed-nu-m23-ns-tfm INTERFACE)
target_include_directories(mbed-nu-m23-ns-tfm
    INTERFACE
        device/TARGET_TFM
)
target_sources(mbed-nu-m23-ns-tfm
    INTERFACE
        TARGET_TFM/tfm_ns_lock_rtx.c
        device/TARGET_TFM/cmsis_nvic_virtual.c
)


add_library(mbed-m23-ns INTERFACE)

target_sources(mbed-m23-ns
    INTERFACE
        wait_ns.c
)

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/M2351.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(LINKER_FILE device/TOOLCHAIN_ARMC6/M2351.sct)
endif()

mbed_set_linker_script(mbed-m23-ns ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-m23-ns INTERFACE mbed-2351)
