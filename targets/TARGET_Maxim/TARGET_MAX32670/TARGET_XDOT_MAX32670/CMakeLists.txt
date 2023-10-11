add_library(mbed-xdot-max32670 INTERFACE)

target_include_directories(mbed-xdot-max32670
    INTERFACE
        .
)

target_link_libraries(mbed-xdot-max32670
    INTERFACE 
        mbed-max32670
)

target_sources(mbed-xdot-max32670
    INTERFACE
        SystemInit.c
)
