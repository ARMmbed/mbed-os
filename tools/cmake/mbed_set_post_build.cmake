# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

#
# Sets the post build operation for Mbed targets.
#
macro(mbed_set_post_build_operation)

    add_custom_target(mbed-post-build-bin-${mbed_target_name}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${APP_TARGET}.bin
    )

    # Ensures the application artefacts are created before manipulating them.
    add_dependencies(mbed-post-build-bin-${mbed_target_name} ${APP_TARGET})

    add_custom_command(
        OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/${APP_TARGET}.bin
        POST_BUILD
        COMMAND
            ${post_build_command}
        VERBATIM
    )
endmacro()
