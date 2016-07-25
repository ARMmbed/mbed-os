if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mfirmware 
        "main.cpp"
        "m2mfirmwaretest.cpp"
        "test_m2mfirmware.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../../../../source/m2mfirmware.cpp"
)
target_link_libraries(m2mfirmware
    CppUTest
    CppUTestExt
)
set_target_properties(m2mfirmware
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mfirmware")
add_test(m2mfirmware ${binary})

endif()
