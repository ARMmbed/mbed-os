if(MBED_TOOLCHAIN strequal "GCC_ARM")

set(COMMON_FLAGS
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -fmessage-length=0
    -fno-exceptions
    -ffunction-sections
    -fdata-sections
    -funsigned-char
    -MMD
    -fomit-frame-pointer
    -Os
    -g
)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    ${COMMON_FLAGS}
)


elseif(MBED_TOOLCHAIN strequal "ARMC6")

set(COMMON_FLAGS
    -c
    --target=arm-arm-none-eabi
    -mthumb
    -Oz
    -Wno-armcc-pragma-push-pop
    -Wno-armcc-pragma-anon-unions
    -Wno-reserved-user-defined-literal
    -Wno-deprecated-register
    -fdata-sections
    -fno-exceptions
    -MMD
    -fshort-enums
    -fshort-wchar
)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    ${COMMON_FLAGS}
)


elseif(MBED_TOOLCHAIN strequal "ARM")

set(COMMON_FLAGS
    -c
    --gnu
    -Ospace
    --split_sections
    --apcs=interwork
    --brief_diagnostics
    --restrict
    --multibyte_chars
    -O3
)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    ${COMMON_FLAGS}
)


elseif(MBED_TOOLCHAIN strequal "uARM")

set(COMMON_FLAGS
    -c
    --gnu
    -Ospace
    --split_sections
    --apcs=interwork
    --brief_diagnostics
    --restrict
    --multibyte_chars
    -O3
    --library_type=microlib
)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    ${COMMON_FLAGS}
)


elseif(MBED_TOOLCHAIN strequal "IAR")

set(COMMON_FLAGS
    --no_wrap_diagnostics
    -e
    --diag_suppress=Pa050,Pa084,Pa093,Pa082,Pe540
    -Ohz
    --enable_restrict
)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    ${COMMON_FLAGS}
)
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    ${COMMON_FLAGS}
)


endif()
