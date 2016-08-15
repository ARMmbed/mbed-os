if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2minterfacefactorytest
        "main.cpp"
        "m2minterfacefactorytest.cpp"
        "test_m2minterfacefactory.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mresource_stub.cpp"
        "../stub/m2mresourceinstance_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mobjectinstance_stub.cpp"
        "../stub/m2mdevice_stub.cpp"
        "../stub/m2mfirmware_stub.cpp"
        "../stub/m2msecurity_stub.cpp"
        "../stub/m2mserver_stub.cpp"
        "../stub/m2minterfaceimpl_stub.cpp"
        "../../../../source/m2minterfacefactory.cpp"
)
target_link_libraries(m2minterfacefactorytest
    CppUTest
    CppUTestExt
)
set_target_properties(m2minterfacefactorytest
PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2minterfacefactorytest")
add_test(m2minterfacefactorytest ${binary})

endif()
