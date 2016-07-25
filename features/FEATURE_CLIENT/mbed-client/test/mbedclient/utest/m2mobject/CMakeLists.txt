if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mobject
        "main.cpp"
        "m2mobjecttest.cpp"
        "test_m2mobject.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2mtlvserializer_stub.cpp"
        "../stub/m2mtlvdeserializer_stub.cpp"
        "../stub/m2mreporthandler_stub.cpp"
        "../stub/common_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mtlvserializer_stub.cpp"
        "../../../../source/m2mobject.cpp"
)
target_link_libraries(m2mobject
    CppUTest
    CppUTestExt
)
set_target_properties(m2mobject
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")

set(binary "m2mobject")
add_test(m2mobject ${binary})

endif()
