if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mconnectionsecurity_mbedtls
        "../../../../source/m2mconnectionsecurity.cpp"
        "main.cpp"
        "m2mconnectionsecurity_mbedtlstest.cpp"
        "test_m2mconnectionsecurity_mbedtls.cpp"
        "../stub/m2msecurity_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mtimer_stub.cpp"
        "../stub/mbedtls_stub.cpp"
        "../stub/m2mconnectionhandler_stub.cpp"
        "../stub/m2mconnectionsecuritypimpl_stub.cpp"
)
target_link_libraries(m2mconnectionsecurity_mbedtls
    CppUTest
    CppUTestExt
)
set_target_properties(m2mconnectionsecurity_mbedtls
PROPERTIES COMPILE_FLAGS "${UTEST_COMPILE_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mconnectionsecurity_mbedtls -ojunit")
add_test(m2mconnectionsecurity_mbedtls ${binary})

endif()
