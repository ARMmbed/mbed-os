if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(nsdlaccesshelper
        "../../../../source/nsdlaccesshelper.cpp"
        "main.cpp"
        "nsdlaccesshelpertest.cpp"
        "test_nsdlaccesshelper.cpp"
        "../stub/common_stub.cpp"
        "../stub/m2mnsdlinterface_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mconnectionhandler_stub.cpp"
        "../stub/m2mconnectionsecurity_stub.cpp"
)

target_link_libraries(nsdlaccesshelper
    CppUTest
    CppUTestExt
)
set_target_properties(nsdlaccesshelper
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")

set(binary "nsdlaccesshelper")
add_test(nsdlaccesshelper ${binary})

endif()
