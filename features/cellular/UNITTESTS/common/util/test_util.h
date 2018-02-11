/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_UTIL_H
#define TEST_UTIL_H

class Test_util
{
public:
    Test_util();

    virtual ~Test_util();

    void test_util_uint_to_binary_string();

    void test_util_char_str_to_hex();

    void test_util_convert_ipv6();

    void test_util_prefer_ipv6();

    void test_util_separate_ip_addresses();
};

#endif // TEST_UTIL_H

