# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_set_post_build)

#
# Patch an LPC target vector table in the binary file.
#
macro(mbed_post_build_lpc_patch_vtable nxp_lpc_target)
    if("${nxp_lpc_target}" STREQUAL "${MBED_TARGET}")
        function(mbed_post_build_function target)
            find_package(Python3)
            add_custom_command(
                TARGET
                    ${target}
                POST_BUILD
                COMMAND
                    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/LPC.py
                    $<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_BASE_NAME:${target}>.bin
            )
        endfunction()
    endif()
endmacro()
