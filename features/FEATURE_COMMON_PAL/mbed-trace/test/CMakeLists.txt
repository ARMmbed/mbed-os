# only build tests on targets that declare they are like posix
if(DEFINED TARGET_LIKE_X86_WINDOWS_NATIVE OR DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    
    # describe the test executable
    add_executable(mbed_trace_test EXCLUDE_FROM_ALL Test.cpp stubs/ip6tos_stub.c)
    
    include_directories("../yotta_modules/cpputest" "./stubs")
    
    # describe what the test executable needs to link with
    target_link_libraries(mbed_trace_test "mbed-trace" cpputest)
    
    # describe what is actual test binary
    if(DEFINED TARGET_LIKE_X86_WINDOWS_NATIVE)
        add_test(mbed_trace_test "build/x86-windows-native/test/mbed_trace_test")
        add_dependencies(all_tests mbed_trace_test)
    endif()
    if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
        SET(TEST_EXECUTABLE "../../../build/x86-linux-native/test/mbed_trace_test")
        add_test(mbed_trace_test ${TEST_EXECUTABLE})
        add_dependencies(all_tests mbed_trace_test)
    endif()
endif()
