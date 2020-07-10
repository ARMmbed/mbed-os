# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Supported GCC ARM and ARM currently
# TODO: @mbed-os-tools - this file should be auto generated

if(MBED_OS_TOOLCHAIN STREQUAL "GCC_ARM")

# force cmake compilers
set(CMAKE_ASM_COMPILER    "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER      "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER    "arm-none-eabi-g++")
set(ELF2BIN               "arm-none-eabi-objcopy")

set(LD_SYS_LIBS "-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group")

# TODO: @mbed-os-tools get flags from config, target and profile
set(CMAKE_C_FLAGS "-std=gnu11 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Og -g3 -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -include mbed_config.h")
set(CMAKE_CXX_FLAGS "-std=gnu++14 -fno-rtti -Wvla -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Og -g3 -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000  -include mbed_config.h")
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Og -g3 -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp  -include mbed_config.h")
set(CMAKE_CXX_LINK_FLAGS "-Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,__malloc_r -Wl,--wrap,__free_r -Wl,--wrap,__realloc_r -Wl,--wrap,__memalign_r -Wl,--wrap,__calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -DMBED_BOOT_STACK_SIZE=1024 -DXIP_ENABLE=0 ")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LD_SYS_LIBS} -T ${CMAKE_BINARY_DIR}/app.link_script.ld")

elseif(MBED_OS_TOOLCHAIN STREQUAL "ARM")

# TODO: @mbed tools to pass the processor type
set(CMAKE_SYSTEM_PROCESSOR    cortex-m4)

set(CMAKE_ASM_COMPILER    "armasm")
set(CMAKE_C_COMPILER      "armclang")
set(CMAKE_CXX_COMPILER    "armclang")
set(CMAKE_AR              "armar")
set(ELF2BIN               "fromelf")

# TODO: @mbed-os-tools get flags from mbed-os/tools/profiles/,
#       mbed-os/tools/toolchains/arm.py, and target config in mbed-os/targets/targets.json
set(CMAKE_C_FLAGS
    "--target=arm-arm-none-eabi -mthumb -g -O1 -Wno-armcc-pragma-push-pop -Wno-armcc-pragma-anon-unions -Wno-reserved-user-defined-literal -Wno-deprecated-register -DMULADDC_CANNOT_USE_R7 -fdata-sections -fno-exceptions -fshort-enums -fshort-wchar -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -D__ASSERT_MSG -std=gnu11 -mfpu=none -mcpu=cortex-m4 -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -include mbed_config.h"
)
set(CMAKE_CXX_FLAGS
    "--target=arm-arm-none-eabi -mthumb -g -O1 -Wno-armcc-pragma-push-pop -Wno-armcc-pragma-anon-unions -Wno-reserved-user-defined-literal -Wno-deprecated-register -DMULADDC_CANNOT_USE_R7 -fdata-sections -fno-exceptions -fshort-enums -fshort-wchar -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -fno-rtti -fno-c++-static-destructors -std=gnu++14 -mfpu=none -mcpu=cortex-m4 -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000  -include mbed_config.h"
)
set(CMAKE_ASM_FLAGS
    "--cpu=Cortex-M4 --cpreproc --cpreproc_opts=--target=arm-arm-none-eabi,-D,__FPU_PRESENT"
)
set(CMAKE_CXX_LINK_FLAGS
    "--verbose --remove --show_full_path --legacyalign --any_contingency --keep=os_cb_sections --predefine=-DMBED_ROM_START=0x0 --predefine=-DMBED_ROM_SIZE=0x100000 --predefine=-DMBED_RAM1_START=0x1fff0000 --predefine=-DMBED_RAM1_SIZE=0x10000 --predefine=-DMBED_RAM_START=0x20000000 --predefine=-DMBED_RAM_SIZE=0x30000 --predefine=-DMBED_BOOT_STACK_SIZE=1024 --predefine=-DXIP_ENABLE=0"
)

endif()
