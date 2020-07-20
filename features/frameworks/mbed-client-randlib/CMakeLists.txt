# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        source/randLIB.c
)

target_include_directories(mbed-os
    PUBLIC
        mbed-client-randlib
        mbed-client-randlib/platform
)
