# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

function(_mbed_get_lib_file_uno_91h)
    if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
        set(LIB_FILE TOOLCHAIN_GCC_ARM/libwifi_sta_ap.a)
    elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
        set(LIB_FILE TOOLCHAIN_ARM_STD/libwifi_sta_ap.ar)
    endif()
    target_sources(mbed-emac PRIVATE ${LIB_FILE})
endfunction()

_mbed_get_lib_file_uno_91h()
