# SPDX-License-Identifier: Apache-2.0

# TODO : this logic is for any TARGET, FEATURE etc,
#  thus this should be done genericaly
# Add TARGET_ directory to the CMake tree

# PREFIX - TARGET or similar (can be empty string)
# KEYWORD_LIST - list of labels that are used to include directories
function(mbed_add_cmake_directory PREFIX KEYWORDLIST)
    foreach(key ${KEYWORDLIST})
        if(NOT "${PREFIX}" STREQUAL "")
            string(PREPEND key ${PREFIX} "_")
        endif()
        # assumption: relative path, no need to check for absolute here
        set(path ${CMAKE_CURRENT_SOURCE_DIR}/${key})
        if (EXISTS ${path})
            add_subdirectory(${path})
        endif()
    endforeach()
endfunction()

# Include other required CMake files
include(${MBED_OS_ROOT}/cmake/gcc.cmake)
include(${MBED_OS_ROOT}/cmake/target.cmake)
include(${MBED_OS_ROOT}/cmake/env.cmake)

# Compiler setup
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Project setup
enable_language(C CXX ASM)

# if the environment does not specify build type, set to Debug
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "RelWithDebInfo"
        CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

# Create application executable
add_executable(app)

# Include Mbed OS main cmake
add_subdirectory(mbed-os)

# Include project configuration
add_subdirectory(gen_config)

# add syslibs dependencies to create the correct linker order
target_link_libraries(app mbed-os gen_config -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys)

add_custom_command(TARGET app PRE_LINK
                COMMAND "arm-none-eabi-cpp" -E -P -Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_BOOT_STACK_SIZE=1024 -DXIP_ENABLE=0 ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device/TOOLCHAIN_GCC_ARM/MK64FN1M0xxx12.ld -o ${CMAKE_CURRENT_BINARY_DIR}/mbed-os-example-blinky_pp.link_script.ld
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/mbed-os-example-blinky_pp.link_script.ld"
                )

add_custom_command(TARGET app POST_BUILD
                COMMAND ${ELF2BIN} -O binary $<TARGET_FILE:app> $<TARGET_FILE:app>.bin
                COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:app>.bin"
                )