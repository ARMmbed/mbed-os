if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mserver
        "main.cpp"
        "m2mservertest.cpp"
        "test_m2mserver.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../../../../source/m2mserver.cpp"
)

target_link_libraries(m2mserver
    CppUTest
    CppUTestExt
)
set_target_properties(m2mserver
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mserver")
add_test(m2mserver ${binary})

endif()
