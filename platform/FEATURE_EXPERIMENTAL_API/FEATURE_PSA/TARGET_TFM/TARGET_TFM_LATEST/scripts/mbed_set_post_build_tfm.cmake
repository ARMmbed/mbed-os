# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build)

set(MBED_POST_BUILD_TFM_DIR "${CMAKE_CURRENT_LIST_DIR}")

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
            ${MBED_POST_BUILD_TFM_DIR}/generate_mbed_image.py
            --tfm-target ${tfm_target}
            --target-path ${target_path}
            --secure-bin ${secure_bin}
            --non-secure-bin ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.bin
    )

    mbed_set_post_build_operation()
endfunction()
