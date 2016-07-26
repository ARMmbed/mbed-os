/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_sn_coap_parser.h"

TEST_GROUP(sn_coap_parser)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(sn_coap_parser, test_sn_coap_parser)
{
    CHECK(test_sn_coap_parser());
}

TEST(sn_coap_parser, test_sn_coap_parser_options_parsing)
{
    CHECK(test_sn_coap_parser_options_parsing());
}

TEST(sn_coap_parser, test_sn_coap_parser_options_parsing_switches)
{
    CHECK(test_sn_coap_parser_options_parsing_switches());
}

TEST(sn_coap_parser, test_sn_coap_parser_options_count_needed_memory_multiple_option)
{
    CHECK(test_sn_coap_parser_options_count_needed_memory_multiple_option());
}

TEST(sn_coap_parser, test_sn_coap_parser_options_parse_multiple_options)
{
    CHECK(test_sn_coap_parser_options_parse_multiple_options());
}

TEST(sn_coap_parser, test_sn_coap_parser_parsing)
{
    CHECK(test_sn_coap_parser_parsing());
}

TEST(sn_coap_parser, test_sn_coap_parser_release_allocated_coap_msg_mem)
{
    CHECK(test_sn_coap_parser_release_allocated_coap_msg_mem());
}
