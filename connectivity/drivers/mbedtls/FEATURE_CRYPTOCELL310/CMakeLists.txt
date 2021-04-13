# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("MCU_NRF52840" IN_LIST MBED_TARGET_LABELS)
    add_subdirectory(TARGET_MCU_NRF52840)
endif()

add_subdirectory(binaries)

target_include_directories(mbed-mbedtls
    INTERFACE
        .
        ./include
        ./include/cryptocell310
        ./include/cryptocell310/internal
)

target_sources(mbed-mbedtls
    INTERFACE
        source/aes_alt.c
        source/cc_internal.c
        source/ccm_alt.c
        source/cmac_alt.c
        source/ecdh_alt.c
        source/ecdsa_alt.c
        source/sha1_alt.c
        source/sha256_alt.c
        source/sha512_alt.c
        source/trng.c
)

target_compile_definitions(mbed-mbedtls
    INTERFACE
        MBED_CONF_CRYPTOCELL310_PRESENT=1
)
