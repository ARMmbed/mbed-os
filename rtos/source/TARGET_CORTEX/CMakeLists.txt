# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    mbed_boot.c
    mbed_boot.h
    mbed_rtos_rtx.c
    mbed_rtx_conf.h
    mbed_rtx_handlers.c
    mbed_rtx_idle.cpp
    mbed_rtx_storage.h
)

add_subdirectory(rtx5)

if(${MBED_OS_TOOLCHAIN} STREQUAL "GCC_ARM")
target_sources(mbed-os PRIVATE TOOLCHAIN_GCC_ARM/mbed_boot_gcc_arm.c)
elseif(${MBED_OS_TOOLCHAIN} STREQUAL "ARM")
target_sources(mbed-os PRIVATE TOOLCHAIN_ARM/mbed_boot_arm_std.c)
elseif(${MBED_OS_TOOLCHAIN} STREQUAL "IAR")
target_sources(mbed-os PRIVATE TOOLCHAIN_IAR/mbed_boot_iar.c)
endif()

target_include_directories(mbed-os PUBLIC .)
target_include_directories(mbed-os PUBLIC rtx4)
