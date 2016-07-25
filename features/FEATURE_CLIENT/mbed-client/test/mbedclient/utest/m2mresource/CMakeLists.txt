if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mresource
        "../../../../source/m2mresource.cpp"
	"main.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mtimer_stub.cpp"
        "../stub/m2mreporthandler_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mtlvdeserializer_stub.cpp"
        "../stub/m2mtlvserializer_stub.cpp"
        "../stub/common_stub.cpp"
	"m2mresourcetest.cpp"
        "test_m2mresource.cpp"
)
target_link_libraries(m2mresource
    CppUTest
    CppUTestExt
)
set_target_properties(m2mresource
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mresource")
add_test(m2mresource ${binary})

endif()
