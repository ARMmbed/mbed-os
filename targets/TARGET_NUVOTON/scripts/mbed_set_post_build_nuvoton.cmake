# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/mbed_set_post_build.cmake)

#
# Sign TF-M secure and non-secure images and combine them with the bootloader
#
macro(mbed_post_build_nuvoton_tfm_sign_image_tgt
    nuvoton_target
    tfm_import_path
    signing_key
)
    if("${nuvoton_target}" STREQUAL "${MBED_TARGET}")
        function(mbed_post_build_function target)
            find_package(Python3)

            add_custom_command(
                TARGET
                    ${target}
                POST_BUILD
                COMMAND
                    ${Python3_EXECUTABLE}
                    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/NUVOTON.py
                    tfm_sign_image_tgt
                    --tfm-import-path ${tfm_import_path}
                    --signing_key ${signing_key}
                    --non-secure-bin $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.bin
            )
        endfunction()
    endif()
endmacro()
