# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build)

#
# Sign TF-M secure and non-secure images and combine them with the bootloader
#
macro(mbed_post_build_tfm_sign_image
    mbed_tfm_target
    tfm_target
    target_path
    secure_bin
)
    if("${mbed_tfm_target}" STREQUAL "${MBED_TARGET}")
        function(mbed_post_build_function target)
            find_package(Python3)
            add_custom_command(
                TARGET
                    ${target}
                POST_BUILD
                COMMAND
                    ${Python3_EXECUTABLE}
                    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate_mbed_image.py
                    --tfm-target ${tfm_target}
                    --target-path ${target_path}
                    --secure-bin ${secure_bin}
                    --non-secure-bin $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.bin
            )
        endfunction()
    endif()
endmacro()
