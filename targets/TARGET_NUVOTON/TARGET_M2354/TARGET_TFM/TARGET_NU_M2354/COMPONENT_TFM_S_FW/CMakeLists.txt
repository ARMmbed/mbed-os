# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build_nuvoton)

target_link_libraries(mbed-m2354-tfm
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/s_veneers.o
)

# Must align with TF-M import assets
set(bl2 true)
set(mcuboot_image_number 2)
set(region_defs_h_path "${CMAKE_CURRENT_SOURCE_DIR}/partition/region_defs.h")
set(partition_mcu_h_path "${CMAKE_CURRENT_SOURCE_DIR}/partition/partition_M2354_im.h")
set(update_stage_sdh true)
set(update_stage_flash false)

if(mcuboot_image_number EQUAL 2)
    mbed_post_build_nuvoton_tfm_sign_image(
        NU_M2354
        ${CMAKE_CURRENT_SOURCE_DIR}
        nuvoton_m2354-root-rsa-3072.pem
        nuvoton_m2354-root-rsa-3072_1.pem
    )
else()
    mbed_post_build_nuvoton_tfm_sign_image(
        NU_M2354
        ${CMAKE_CURRENT_SOURCE_DIR}
        nuvoton_m2354-root-rsa-3072.pem
        NOTFOUND
    )
endif()

target_compile_definitions(mbed-m2354-tfm
    INTERFACE
        NU_TFM_S_BL2=$<IF:$<BOOL:${bl2}>,1,0>
        NU_TFM_S_MCUBOOT_IMAGE_NUMBER=${mcuboot_image_number}
        NU_TFM_S_REGION_DEFS_H_PATH="${region_defs_h_path}"
        NU_TFM_S_PARTITION_MCU_H_PATH="${partition_mcu_h_path}"
        NU_TFM_S_UPDATE_STAGE_SDH=$<IF:$<BOOL:${update_stage_sdh}>,1,0>
        NU_TFM_S_UPDATE_STAGE_FLASH=$<IF:$<BOOL:${update_stage_flash}>,1,0>
)
