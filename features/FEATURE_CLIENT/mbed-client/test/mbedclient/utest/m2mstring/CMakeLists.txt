if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mstring
        "main.cpp"
        "test_m2mstring.cpp"
        "m2mstringtest.cpp"
        "../../../../source/m2mstring.cpp"
)

target_link_libraries(m2mstring
    CppUTest
    CppUTestExt
)
set_target_properties(m2mstring
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")

set(binary "m2mstring")
add_test(m2mstring ${binary})

endif()
