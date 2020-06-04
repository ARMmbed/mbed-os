# SPDX-License-Identifier: Apache-2.0

# force cmake compilers
set(CMAKE_ASM_COMPILER    "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER      "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER    "arm-none-eabi-g++")
set(ELF2BIN               "arm-none-eabi-objcopy")

set(MBED_OS_TOOLCHAIN "GCC ARM" CACHE INTERNAL "")

set(LD_SYS_LIBS "-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group")

set(CMAKE_C_FLAGS "-std=gnu11 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Og -g3 -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -include mbed_config.h")
set(CMAKE_CXX_FLAGS "-std=gnu++14 -fno-rtti -Wvla -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Og -g3 -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000  -include mbed_config.h")
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Og -g3 -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp  -include mbed_config.h")
set(CMAKE_CXX_LINK_FLAGS "-Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,__malloc_r -Wl,--wrap,__free_r -Wl,--wrap,__realloc_r -Wl,--wrap,__memalign_r -Wl,--wrap,__calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -DMBED_BOOT_STACK_SIZE=1024 -DXIP_ENABLE=0 ")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LD_SYS_LIBS} -T ${CMAKE_BINARY_DIR}/mbed-os-example-blinky_pp.link_script.ld")
