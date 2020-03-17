# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_assembly_k64f)
    if(${MBED_OS_TOOLCHAIN} STREQUAL "GCC ARM")
        set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_MK64F12.S)
    elseif(${MBED_OS_TOOLCHAIN} STREQUAL ARM)
        set(STARTUP_FILE TOOLCHAIN_ARM/startup_MK64F12.S)
    elseif(${MBED_OS_TOOLCHAIN} STREQUAL IAR)
        set(STARTUP_FILE TOOLCHAIN_IAR/startup_MK64F12.S)
    endif()
    target_sources(mbed-os PRIVATE ${STARTUP_FILE})
endfunction()

_mbed_get_assembly_k64f()

target_sources(mbed-os PRIVATE cmsis.h)
target_sources(mbed-os PRIVATE cmsis_nvic.h)
target_sources(mbed-os PRIVATE fsl_device_registers.h)
target_sources(mbed-os PRIVATE MK64F12.h)
target_sources(mbed-os PRIVATE MK64F12_features.h)
target_sources(mbed-os PRIVATE system_MK64F12.c)
target_sources(mbed-os PRIVATE system_MK64F12.h)

target_sources(mbed-os PRIVATE cmsis.h)
target_sources(mbed-os PRIVATE cmsis_nvic.h)
target_sources(mbed-os PRIVATE fsl_device_registers.h)
target_sources(mbed-os PRIVATE MK64F12.h)
target_sources(mbed-os PRIVATE MK64F12_features.h)
target_sources(mbed-os PRIVATE system_MK64F12.c)
target_sources(mbed-os PRIVATE system_MK64F12.h)

target_include_directories(mbed-os PUBLIC .)
