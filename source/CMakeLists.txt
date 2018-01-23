if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    add_library( mbed-client-cli
        ns_cmdline.c
        ns_list_internal/ns_list.c
    )
    add_definitions("-g -O0 -fprofile-arcs -ftest-coverage")
    target_link_libraries(mbed-client-cli gcov)
else()
    add_library( mbed-client-cli
        ns_cmdline.c
        ns_list_internal/ns_list.c
    )
    target_link_libraries(mbed-client-cli)
endif()
