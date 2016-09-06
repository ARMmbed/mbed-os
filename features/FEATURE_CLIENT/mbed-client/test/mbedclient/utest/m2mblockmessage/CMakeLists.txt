if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mblockmessage
        "main.cpp"
        "m2mblockmessagetest.cpp"
        "test_m2mblockmessage.cpp"
        "../../../../source/m2mblockmessage.cpp"
)

target_link_libraries(m2mblockmessage
    CppUTest
    CppUTestExt
)
set_target_properties(m2mblockmessage
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mblockmessage")
add_test(m2mblockmessage ${binary})

endif()
