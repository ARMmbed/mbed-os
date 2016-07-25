if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mnsdlinterface
        "main.cpp"
        "m2mnsdlinterfacetest.cpp"
        "test_m2mnsdlinterface.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mserver_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2msecurity_stub.cpp"
        "../stub/m2mconnectionsecurity_stub.cpp"
        "../stub/m2mconnectionhandler_stub.cpp"
        "../stub/m2mtimer_stub.cpp"
        "../stub/common_stub.cpp"
        "../stub/nsdlaccesshelper_stub.cpp"
        "../stub/m2mtlvserializer_stub.cpp"
        "../stub/m2mtlvdeserializer_stub.cpp"
        "../../../../source/m2mnsdlinterface.cpp"
)
target_link_libraries(m2mnsdlinterface
    CppUTest
    CppUTestExt
)
set_target_properties(m2mnsdlinterface
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mnsdlinterface")
add_test(m2mnsdlinterface ${binary})

endif()
