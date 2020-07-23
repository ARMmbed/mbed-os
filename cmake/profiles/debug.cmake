# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND link_options)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
    list(APPEND c_compile_options
        "-c"
        "-std=gnu11"
        "-g3"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${c_compile_options}>
    )

    list(APPEND cxx_compile_options
        "-c"
        "-g3"
        "-std=gnu++14"
        "-fno-rtti"
        "-Wvla"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:CXX>:${cxx_compile_options}>
    )

    list(APPEND asm_compile_options
        "-c"
        "-g3"
        "-x" "assembler-with-cpp"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:ASM>:${asm_compile_options}>
    )

    list(APPEND link_options
        "-Wl,--gc-sections"
        "-Wl,--wrap,main"
        "-Wl,--wrap,_malloc_r"
        "-Wl,--wrap,_free_r"
        "-Wl,--wrap,_realloc_r"
        "-Wl,--wrap,_memalign_r"
        "-Wl,--wrap,_calloc_r"
        "-Wl,--wrap,exit"
        "-Wl,--wrap,atexit"
        "-Wl,-n"
    )
elseif(MBED_TOOLCHAIN STREQUAL "ARM")
    list(APPEND c_compile_options
        "-std=gnu11"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${c_compile_options}>
    )

    list(APPEND cxx_compile_options
        "-std=gnu++14"
        "-fno-rtti"
        "-fno-c++-static-destructors"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:CXX>:${cxx_compile_options}>
    )

    list(APPEND link_options
        "--verbose"
        "--remove"
        "--show_full_path"
        "--legacyalign"
        "--any_contingency"
        "--keep=os_cb_sections"
    )

    target_compile_definitions(mbed-os
        PUBLIC
            __ASSERT_MSG
            MULADDC_CANNOT_USE_R7
    )
endif()

target_compile_definitions(mbed-os
    PUBLIC
        MBED_DEBUG
        MBED_TRAP_ERRORS_ENABLED=1
)

target_link_options(mbed-os
    PUBLIC
        ${link_options}
)
