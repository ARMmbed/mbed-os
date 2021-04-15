# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Sets profile options
function(mbed_set_profile_options target mbed_toolchain)
    set(profile_link_options "")

    if(${mbed_toolchain} STREQUAL "GCC_ARM")
        list(APPEND profile_c_compile_options
            "-c"
            "-Og"
        )
        target_compile_options(${target}
            INTERFACE
                $<$<COMPILE_LANGUAGE:C>:${profile_c_compile_options}>
        )

        list(APPEND profile_cxx_compile_options
            "-c"
            "-fno-rtti"
            "-Wvla"
            "-Og"
        )
        target_compile_options(${target}
            INTERFACE
                $<$<COMPILE_LANGUAGE:CXX>:${profile_cxx_compile_options}>
        )

        list(APPEND profile_asm_compile_options
            "-c"
            "-x" "assembler-with-cpp"
        )
        target_compile_options(${target}
            INTERFACE
                $<$<COMPILE_LANGUAGE:ASM>:${profile_asm_compile_options}>
        )

        list(APPEND profile_link_options
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
    elseif(${mbed_toolchain} STREQUAL "ARM")
        list(APPEND profile_c_compile_options
            "-O1"
        )
        target_compile_options(${target}
            INTERFACE
                $<$<COMPILE_LANGUAGE:C>:${profile_c_compile_options}>
        )

        list(APPEND profile_cxx_compile_options
            "-fno-rtti"
            "-fno-c++-static-destructors"
            "-O1"
        )
        target_compile_options(${target}
            INTERFACE
                $<$<COMPILE_LANGUAGE:CXX>:${profile_cxx_compile_options}>
        )

        list(APPEND link_options
            "--verbose"
            "--remove"
            "--show_full_path"
            "--legacyalign"
            "--any_contingency"
            "--keep=os_cb_sections"
        )

        target_compile_definitions(${target}
            INTERFACE
                __ASSERT_MSG
        )
    endif()

    target_compile_definitions(${target}
        INTERFACE
            MBED_DEBUG
            MBED_TRAP_ERRORS_ENABLED=1
    )

    target_link_options(${target}
        INTERFACE
            ${profile_link_options}
    )
endfunction()
