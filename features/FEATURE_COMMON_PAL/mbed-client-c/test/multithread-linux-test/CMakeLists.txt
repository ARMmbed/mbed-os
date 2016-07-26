# NB external pthreads dependency!!
find_package (Threads)
add_executable(multithread-linux-test
        "main.c"
        "mbed_client.c"
)
target_link_libraries(multithread-linux-test
    mbed-client-c
)

target_link_libraries(multithread-linux-test
    ${CMAKE_THREAD_LIBS_INIT}
)


