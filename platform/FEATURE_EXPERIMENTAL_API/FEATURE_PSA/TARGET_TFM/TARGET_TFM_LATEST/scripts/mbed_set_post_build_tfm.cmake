# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/mbed_set_post_build.cmake)

#
# Sign TF-M secure and non-secure images and combine them with the bootloader
#
function(mbed_post_build_tfm_sign_image
    mbed_target
    tfm_target
    target_path
    secure_bin
)
    find_package(Python3)

    set(mbed_target_name ${mbed_target})
    set(post_build_command
        COMMAND ${Python3_EXECUTABLE}
            ${MBED_PATH}/platform/FEATURE_EXPERIMENTAL_API/FEATURE_PSA/TARGET_TFM/TARGET_TFM_LATEST/scripts/generate_mbed_image.py
            --tfm-target ${tfm_target}
            --target-path ${target_path}
            --secure-bin ${secure_bin}
            --non-secure-bin ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.bin
    )

    mbed_set_post_build_operation()
endfunction()
