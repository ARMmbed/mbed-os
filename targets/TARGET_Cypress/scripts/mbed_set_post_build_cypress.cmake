# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build)

#
# Merge Cortex-M4 HEX and a Cortex-M0 HEX.
#
macro(mbed_post_build_psoc6_merge_hex cypress_psoc6_target)
    if("${cypress_psoc6_target}" STREQUAL "${MBED_TARGET}")
        function(mbed_post_build_function target)
            find_package(Python3)

            # Copy ${ARGN} to a variable first as it cannot be used directly with
            # the list() command
            set (extra_macro_args ${ARGN})

            # Get the number of arguments past the last expected argument
            list(LENGTH extra_macro_args num_extra_args)

            if(${num_extra_args} GREATER 0)
                # Get extra argument as `cortex_m0_hex`
                list(GET extra_macro_args 0 cortex_m0_hex)
                set(post_build_command
                    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/PSOC6.py
                    merge
                    --elf $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.elf
                    --m4hex $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.hex
                    --m0hex ${cortex_m0_hex}
                )
            else()
                set(post_build_command
                    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/PSOC6.py
                    merge
                    --elf $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.elf
                    --m4hex $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.hex
                )
            endif()

            add_custom_command(
                TARGET
                    ${target}
                POST_BUILD
                COMMAND
                    ${post_build_command}
            )
        endfunction()
    endif()
endmacro()


#
# Sign a Cortex-M4 HEX with Cortex-M0 HEX.
#
macro(mbed_post_build_psoc6_sign_image
    m0hex_filename
    cypress_psoc6_target
    policy_file_name
    boot_scheme
    cm0_img_id
    cm4_img_id
    cortex_m0_hex
)
    if("${cypress_psoc6_target}" STREQUAL "${MBED_TARGET}")
        function(mbed_post_build_function target)
            find_package(Python3)

            set(post_build_command
                ${Python3_EXECUTABLE} ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/PSOC6.py
                sign
                --build-dir ${CMAKE_BINARY_DIR}
                --m0hex-filename ${m0hex_filename}
                --target-name ${cypress_psoc6_target}
                --policy-file-name ${policy_file_name}
                --boot-scheme ${boot_scheme}
                --cm0-img-id ${cm0_img_id}
                --cm4-img-id ${cm4_img_id}
                --elf $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.elf
                --m4hex $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.hex
                --m0hex ${cortex_m0_hex}
            )

            add_custom_command(
                TARGET
                    ${target}
                POST_BUILD
                COMMAND
                    ${post_build_command}
            )
        endfunction()
    endif()
endmacro()
