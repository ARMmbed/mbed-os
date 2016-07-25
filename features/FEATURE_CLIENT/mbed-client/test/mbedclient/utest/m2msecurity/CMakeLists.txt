if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2msecurity
        "main.cpp"
        "m2msecuritytest.cpp"
        "test_m2msecurity.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../../../../source/m2msecurity.cpp"
)
target_link_libraries(m2msecurity
    CppUTest
    CppUTestExt
)
set_target_properties(m2msecurity
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")

set(binary "m2msecurity")
add_test(m2msecurity ${binary})

endif()
