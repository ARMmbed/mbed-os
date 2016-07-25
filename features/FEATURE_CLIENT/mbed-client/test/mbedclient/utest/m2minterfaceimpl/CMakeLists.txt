if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2minterfaceimpl
        "main.cpp"
        "m2minterfaceimpltest.cpp"
        "test_m2minterfaceimpl.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2msecurity_stub.cpp"
        "../stub/m2mtimer_stub.cpp"
        "../stub/m2mnsdlinterface_stub.cpp"
        "../stub/m2mconnectionhandler_stub.cpp"
        "../stub/m2mconnectionsecurity_stub.cpp"
        "../stub/nsdlaccesshelper_stub.cpp"
        "../../../../source/m2minterfaceimpl.cpp"
)
target_link_libraries(m2minterfaceimpl
    CppUTest
    CppUTestExt
)
set_target_properties(m2minterfaceimpl
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2minterfaceimpl")
add_test(m2minterfaceimpl ${binary})

endif()
