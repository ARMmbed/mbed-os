if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mobjectinstance
        "main.cpp"
        "m2mobjectinstancetest.cpp"
        "test_m2mobjectinstance.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/common_stub.cpp"
        "../stub/m2mtlvserializer_stub.cpp"
        "../stub/m2mtlvdeserializer_stub.cpp"
        "../stub/m2mreporthandler_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../../../../source/m2mobjectinstance.cpp"
)
target_link_libraries(m2mobjectinstance
    CppUTest
    CppUTestExt
)
set_target_properties(m2mobjectinstance
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mobjectinstance")
add_test(m2mobjectinstance ${binary})

endif()
