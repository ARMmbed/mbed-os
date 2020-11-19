# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/targets/TARGET_NUVOTON/scripts/mbed_set_post_build_nuvoton.cmake)

target_link_libraries(mbed-m2354-tfm
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/s_veneers.o
)

mbed_post_build_nuvoton_tfm_sign_image_tgt(
    NU_M2354
    ${CMAKE_CURRENT_SOURCE_DIR}
    nuvoton_m2354-root-rsa-3072.pem
)
