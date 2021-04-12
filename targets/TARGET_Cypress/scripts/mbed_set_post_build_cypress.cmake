# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build)

set(MBED_POST_BUILD_CYPRESS_DIR "${CMAKE_CURRENT_LIST_DIR}")

#
# Merge Cortex-M4 HEX and a Cortex-M0 HEX.
#
function(mbed_post_build_psoc6_merge_hex mbed_target_name)
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
            COMMAND ${Python3_EXECUTABLE} ${MBED_POST_BUILD_CYPRESS_DIR}/PSOC6.py
                merge
                --elf ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.elf
                --m4hex ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.hex
                --m0hex ${cortex_m0_hex}
        )
    else()
        set(post_build_command
            COMMAND ${Python3_EXECUTABLE} ${MBED_POST_BUILD_CYPRESS_DIR}/PSOC6.py
                merge
                --elf ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.elf
                --m4hex ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.hex
        )
    endif()

    mbed_set_post_build_operation()
endfunction()


#
# Sign a Cortex-M4 HEX with Cortex-M0 HEX.
#
function(mbed_post_build_psoc6_sign_image
    m0hex_filename
    mbed_target_name
    policy_file_name
    boot_scheme
    cm0_img_id
    cm4_img_id
    cortex_m0_hex
)
    find_package(Python3)

    set(post_build_command
        COMMAND ${Python3_EXECUTABLE} ${MBED_POST_BUILD_CYPRESS_DIR}/PSOC6.py
            sign
            --build-dir ${CMAKE_BINARY_DIR}
            --m0hex-filename ${m0hex_filename}
            --target-name ${mbed_target_name}
            --policy-file-name ${policy_file_name}
            --boot-scheme ${boot_scheme}
            --cm0-img-id ${cm0_img_id}
            --cm4-img-id ${cm4_img_id}
            --elf ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.elf
            --m4hex ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.hex
            --m0hex ${cortex_m0_hex}
    )

    mbed_set_post_build_operation()
endfunction()
