# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build)

set(MBED_POST_BUILD_NXP_DIR "${CMAKE_CURRENT_LIST_DIR}")

#
# Patch an LPC target vector table in the binary file.
#
function(mbed_post_build_lpc_patch_vtable mbed_target_name)
    find_package(Python3)

    set(post_build_command
        COMMAND ${Python3_EXECUTABLE} ${MBED_POST_BUILD_NXP_DIR}/LPC.py
        ${CMAKE_BINARY_DIR}/$<TARGET_PROPERTY:mbed-post-build-bin-${mbed_target_name},application>.bin
    )

    mbed_set_post_build_operation()
endfunction()
