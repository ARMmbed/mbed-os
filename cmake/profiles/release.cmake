# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

list(APPEND link_options)

if(MBED_TOOLCHAIN STREQUAL "GCC_ARM")
    list(APPEND c_compile_options
        "-c"
        "-std=gnu11"
        "-Os"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${c_compile_options}>
    )

    list(APPEND cxx_compile_options
        "-c"
        "-std=gnu++14"
        "-fno-rtti"
        "-Wvla"
        "-Os"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:CXX>:${cxx_compile_options}>
    )

    list(APPEND asm_compile_options
        "-c"
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
        "-Oz"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${c_compile_options}>
    )

    list(APPEND cxx_compile_options
        "-std=gnu++14"
        "-fno-rtti"
        "-fno-c++-static-destructors"
        "-Oz"
    )
    target_compile_options(mbed-os
        PUBLIC
            $<$<COMPILE_LANGUAGE:CXX>:${cxx_compile_options}>
    )

    list(APPEND link_options
        "--show_full_path"
        "--legacyalign"
        "--inline"
        "--any_contingency"
        "--keep=os_cb_sections"
    )

    target_compile_definitions(mbed-os
        PUBLIC
            __ASSERT_MSG
    )
endif()

target_compile_definitions(mbed-os
    PUBLIC
        NDEBUG
)

target_link_options(mbed-os
    PUBLIC
        ${link_options}
)
