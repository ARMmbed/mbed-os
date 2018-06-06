if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    add_library( mbed-trace
        mbed_trace.c
    )
    add_definitions("-g -O0 -fprofile-arcs -ftest-coverage")
    target_link_libraries(mbed-trace gcov nanostack-libservice)
elseif(DEFINED TARGET_LIKE_X86_OSX_NATIVE)
    add_library( mbed-trace
        mbed_trace.c
    )
    add_definitions("-g -O0")
    target_link_libraries(mbed-trace nanostack-libservice)
else()
    add_library( mbed-trace
        mbed_trace.c
    )
    target_link_libraries(mbed-trace nanostack-libservice)
endif()
