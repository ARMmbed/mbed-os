# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE mbed_boot.c)
target_sources(mbed-os PRIVATE mbed_boot.h)
target_sources(mbed-os PRIVATE mbed_rtos_rtx.c)
target_sources(mbed-os PRIVATE mbed_rtx_conf.h)
target_sources(mbed-os PRIVATE mbed_rtx_handlers.c)
target_sources(mbed-os PRIVATE mbed_rtx_idle.cpp)
target_sources(mbed-os PRIVATE mbed_rtx_storage.h)

add_subdirectory(rtx5)

# TODO: toolchain selection
target_sources(mbed-os PRIVATE TOOLCHAIN_GCC_ARM/mbed_boot_gcc_arm.c)

target_include_directories(mbed-os PUBLIC .)
target_include_directories(mbed-os PUBLIC rtx4)
