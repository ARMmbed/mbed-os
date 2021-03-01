# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_PATH}/tools/cmake/mbed_set_post_build.cmake)

#
# Patch an LPC target vector table in the binary file.
#
function(mbed_post_build_lpc_patch_vtable mbed_target_name)
    find_package(Python3)

    set(post_build_command
        COMMAND ${Python3_EXECUTABLE} ${MBED_PATH}/targets/TARGET_NXP/scripts/LPC.py
        ${CMAKE_BINARY_DIR}
    )

    mbed_set_post_build_operation()
endfunction()
