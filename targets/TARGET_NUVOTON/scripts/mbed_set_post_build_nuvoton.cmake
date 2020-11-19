# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/mbed_set_post_build.cmake)

#
# Sign TF-M secure and non-secure images and combine them with the bootloader
#
function(mbed_post_build_nuvoton_tfm_sign_image_tgt
    mbed_target
    tfm_import_path
    signing_key
)
    find_package(Python3)

    set(mbed_target_name ${mbed_target})
    set(post_build_command
        COMMAND ${Python3_EXECUTABLE}
            ${MBED_PATH}/targets/TARGET_NUVOTON/scripts/NUVOTON.py
            tfm_sign_image_tgt
            --tfm-import-path ${tfm_import_path}
            --signing_key ${signing_key}
            --non-secure-bin ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.bin
    )

    mbed_set_post_build_operation()
endfunction()
