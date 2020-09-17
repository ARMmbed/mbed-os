# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(GCC_ELF2BIN "arm-none-eabi-objcopy")
set_property(GLOBAL PROPERTY ELF2BIN ${GCC_ELF2BIN})


# Sets toolchain options
function(mbed_set_toolchain_options target)
    list(APPEND link_options
        "-Wl,--start-group"
            "-lstdc++"
            "-lsupc++"
            "-lm"
            "-lc"
            "-lgcc"
            "-lnosys"
        "-Wl,--end-group"
        "-specs=nosys.specs"
        "-T" "${CMAKE_BINARY_DIR}/${APP_TARGET}.link_script.ld"
    )

    # Add linking time preprocessor macro for TFM targets
    if("TFM" IN_LIST MBED_TARGET_LABELS)
        list(APPEND link_options
            "-DDOMAIN_NS=1"
        )
    endif()

    list(APPEND common_options
        "-Wall"
        "-Wextra"
        "-Wno-unused-parameter"
        "-Wno-missing-field-initializers"
        "-fmessage-length=0"
        "-fno-exceptions"
        "-ffunction-sections"
        "-fdata-sections"
        "-funsigned-char"
        "-MMD"
        "-fomit-frame-pointer"
        "-g3"
    )

    target_compile_options(${target}
        PUBLIC
            ${common_options}
    )

    target_compile_definitions(${target}
        PUBLIC
            TOOLCHAIN_GCC_ARM
            TOOLCHAIN_GCC
    )


    target_link_options(${target}
        PUBLIC
            ${common_options}
            ${link_options}
    )
endfunction()

# GCC ARM requires preprecessing linker script, execute generators to get definitions needed for
# this step - linker options and compile definitions 
function(mbed_generate_gcc_options_for_linker target definitions_file linker_options_file)
    set(_compile_definitions 
        "$<TARGET_PROPERTY:${target},COMPILE_DEFINITIONS>"
    )

    set(_linker_options
        "$<TARGET_PROPERTY:${target},LINK_OPTIONS>"
    )

    set(_compile_definitions 
        "$<$<BOOL:${_compile_definitions}>:-D$<JOIN:${_compile_definitions}, -D>>"
    )
    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/compile_time_defs.txt" CONTENT "${_compile_definitions}\n")
    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/linker_options.txt" CONTENT "${_linker_options}\n")
    set(definitions_file @${CMAKE_BINARY_DIR}/compile_time_defs.txt)
    set(linker_options_file @${CMAKE_BINARY_DIR}/linker_options.txt)
endfunction()
