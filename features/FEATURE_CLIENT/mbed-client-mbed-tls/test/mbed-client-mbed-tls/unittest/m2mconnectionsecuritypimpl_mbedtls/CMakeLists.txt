if(TARGET_LIKE_LINUX)
include("../includes.txt")
add_executable(m2mconnectionsecuritypimpl_mbedtls
        "../../../../yotta_modules/mbed-client-mbedtls/source/m2mconnectionsecuritypimpl.cpp"
        "main.cpp"
        "m2mconnectionsecuritypimpl_mbedtlstest.cpp"
        "test_m2mconnectionsecuritypimpl_mbedtls.cpp"
        "../stub/m2msecurity_stub.cpp"
        "../stub/m2mstring_stub.cpp"
        "../stub/m2mobject_stub.cpp"
        "../stub/m2mbase_stub.cpp"
        "../stub/m2mtimer_stub.cpp"
        "../stub/mbedtls_stub.cpp"
        "../stub/m2mconnectionhandler_stub.cpp"
        "../stub/m2mconnectionsecurity_stub.cpp"
)

target_link_libraries(m2mconnectionsecuritypimpl_mbedtls
    CppUTest
    CppUTestExt
)
set_target_properties(m2mconnectionsecuritypimpl_mbedtls
PROPERTIES COMPILE_FLAGS "${UTEST_COMPILE_FLAGS}"
           LINK_FLAGS "${UTEST_LINK_FLAGS}")
set(binary "m2mconnectionsecuritypimpl_mbedtls -ojunit")
add_test(m2mconnectionsecuritypimpl_mbedtls ${binary})

endif()
