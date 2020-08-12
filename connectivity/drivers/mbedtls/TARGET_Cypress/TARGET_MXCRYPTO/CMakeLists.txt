# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_add_cmake_directory_if_labels("TARGET")

target_include_directories(mbed-os
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(mbed-os
    PRIVATE
        aes_alt.c
        crypto_common.c
        ecdsa_alt.c
        ecp_alt.c
        ecp_curves_alt.c
        sha1_alt.c
        sha256_alt.c
        sha512_alt.c
)
