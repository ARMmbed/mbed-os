if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mtlv
        "../../../../source/m2mtlvdeserializer.cpp"
        "../../../../source/m2mtlvserializer.cpp"
        "main.cpp"
        "m2mtlvdeserializertest.cpp"
        "test_m2mtlvdeserializer.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2mobject_stub.cpp"
        "test_m2mtlvserializer.cpp"
)

target_link_libraries(m2mtlv
    CppUTest
    CppUTestExt
)
set_target_properties(m2mtlv
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")

set(binary "m2mtlv")
add_test(m2mtlv ${binary})

endif()
