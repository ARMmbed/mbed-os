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

set(bl2 true)
set(mcuboot_image_number 1)
set(region_defs_h_path "${CMAKE_CURRENT_SOURCE_DIR}/partition/region_defs.h")

target_compile_definitions(mbed-m2354-tfm
    INTERFACE
        NU_TFM_S_BL2=$<IF:$<BOOL:${bl2}>,1,0>
        NU_TFM_S_MCUBOOT_IMAGE_NUMBER=${mcuboot_image_number}
        # TODO: Fix escape sequence in NU_TFM_S_REGION_DEFS_H_PATH
        #NU_TFM_S_REGION_DEFS_H_PATH=\"${region_defs_h_path}\"
)
