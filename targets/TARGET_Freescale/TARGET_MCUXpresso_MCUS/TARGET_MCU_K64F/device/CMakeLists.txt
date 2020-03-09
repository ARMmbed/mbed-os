# SPDX-License-Identifier: Apache-2.0

mbed_os_add_cmake_directory("TOOLCHAIN"
"${MBED_OS_TOOLCHAIN}")

target_sources(mbed_os PRIVATE cmsis.h)
target_sources(mbed_os PRIVATE cmsis_nvic.h)
target_sources(mbed_os PRIVATE fsl_device_registers.h)
target_sources(mbed_os PRIVATE MK64F12.h)
target_sources(mbed_os PRIVATE MK64F12_features.h)
target_sources(mbed_os PRIVATE system_MK64F12.c)
target_sources(mbed_os PRIVATE system_MK64F12.h)

target_sources(mbed_os PRIVATE cmsis.h)
target_sources(mbed_os PRIVATE cmsis_nvic.h)
target_sources(mbed_os PRIVATE fsl_device_registers.h)
target_sources(mbed_os PRIVATE MK64F12.h)
target_sources(mbed_os PRIVATE MK64F12_features.h)
target_sources(mbed_os PRIVATE system_MK64F12.c)
target_sources(mbed_os PRIVATE system_MK64F12.h)

target_include_directories(mbed_os PUBLIC .)
