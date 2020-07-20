# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(ELF2BIN "arm-none-eabi-objcopy")
set(LD_SYS_LIBS " \
    -Wl,--start-group \
    -lstdc++ \
    -lsupc++ \
    -lm \
    -lc \
    -lgcc \
    -lnosys \
    -Wl,--end-group \
")
set(COMMON_FLAGS " \
    -Wall \
    -Wextra \
    -Wno-unused-parameter \
    -Wno-missing-field-initializers \
    -fmessage-length=0 \
    -fno-exceptions \
    -ffunction-sections \
    -fdata-sections \
    -funsigned-char \
    -MMD \
    -fomit-frame-pointer \
    -Os \
    -g \
")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    ${COMMON_FLAGS} \
")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    ${COMMON_FLAGS} \
")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} \
    ${COMMON_FLAGS} \
")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} \
    ${COMMON_FLAGS} \
")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} \
    ${LD_SYS_LIBS} \
    -T ${CMAKE_BINARY_DIR}/app.link_script.ld \
")

add_definitions(
    -DTOOLCHAIN_GCC_ARM
    -DTOOLCHAIN_GCC
)
