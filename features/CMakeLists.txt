# SPDX-License-Identifier: Apache-2.0

add_subdirectory(frameworks/mbed-trace)

target_link_libraries(mbedTrace PRIVATE gen_config)
target_link_libraries(mbed-os PUBLIC mbedTrace)

add_subdirectory(lwipstack)
add_subdirectory(mbedtls)
add_subdirectory(frameworks/mbed-client-randlib)
add_subdirectory(frameworks/nanostack-libservice)
add_subdirectory(netsocket)
add_subdirectory(nanostack)

target_include_directories(mbed-os PUBLIC .)
