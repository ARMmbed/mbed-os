# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

#
# Sets the post build operation for Mbed targets.
#
macro(mbed_set_post_build_operation)

    add_custom_target(mbed-post-build-bin-${mbed_target_name})

    add_custom_command(
        TARGET
            mbed-post-build-bin-${mbed_target_name}
        POST_BUILD
        COMMAND
            ${post_build_command}
        VERBATIM
    )
endmacro()
