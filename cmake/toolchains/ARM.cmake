# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER    "armasm")
set(CMAKE_C_COMPILER      "armclang")
set(CMAKE_CXX_COMPILER    "armclang")
set(CMAKE_AR              "armar")
set(ELF2BIN               "fromelf")
set(COMMON_FLAGS " \
    -c \
    --target=arm-arm-none-eabi \
    -Oz \
    -Wno-armcc-pragma-push-pop \
    -Wno-armcc-pragma-anon-unions \
    -Wno-reserved-user-defined-literal \
    -Wno-deprecated-register \
    -fdata-sections \
    -fno-exceptions \
    -MMD \
    -fshort-enums \
    -fshort-wchar \
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

add_definitions(
    -DTOOLCHAIN_ARM
)
