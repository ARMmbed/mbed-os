#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

using namespace utest::v1;


// IP parsing verification
void test_ip_accept(const char *string, nsapi_addr_t addr) {
    SocketAddress address;
    TEST_ASSERT(address.set_ip_address(string));
    TEST_ASSERT(address == SocketAddress(addr));
}

template <const char *string>
void test_ip_reject() {
    SocketAddress address;
    TEST_ASSERT(!address.set_ip_address(string));
    TEST_ASSERT(!address);
}

#define TEST_IP_ACCEPT(name, string, ...)           \
void name() {                                       \
    nsapi_addr_t addr = __VA_ARGS__;                \
    test_ip_accept(string, addr);                   \
}

#define TEST_IP_REJECT(name, string)                \
void name() {                                       \
    test_ip_reject(string);                         \
}


// Test cases
TEST_IP_ACCEPT(test_simple_ipv4_address,
    "12.34.56.78",
    {NSAPI_IPv4,{12,34,56,78}})
TEST_IP_ACCEPT(test_left_weighted_ipv4_address,
    "255.0.0.0",
    {NSAPI_IPv4,{255,0,0,0}})
TEST_IP_ACCEPT(test_right_weighted_ipv4_address,
    "0.0.0.255",
    {NSAPI_IPv4,{0,0,0,255}})
TEST_IP_ACCEPT(test_null_ipv4_address,
    "0.0.0.0",
    {NSAPI_IPv4,{0,0,0,0}})

TEST_IP_ACCEPT(test_simple_ipv6_address,
    "1234:5678:9abc:def0:1234:5678:9abc:def0",
    {NSAPI_IPv6,{0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,
                 0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0}})
TEST_IP_ACCEPT(test_left_weighted_ipv6_address,
    "1234:5678::",
    {NSAPI_IPv6,{0x12,0x34,0x56,0x78,0x00,0x00,0x00,0x00,
                 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}})
TEST_IP_ACCEPT(test_right_weighted_ipv6_address,
    "::1234:5678",
    {NSAPI_IPv6,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0x00,0x00,0x00,0x12,0x34,0x56,0x78}})
TEST_IP_ACCEPT(test_hollowed_ipv6_address,
    "1234:5678::9abc:def8",
    {NSAPI_IPv6,{0x12,0x34,0x56,0x78,0x00,0x00,0x00,0x00,
                 0x00,0x00,0x00,0x00,0x9a,0xbc,0xde,0xf8}})
TEST_IP_ACCEPT(test_null_ipv6_address,
    "::",
    {NSAPI_IPv6,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}})


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Simple IPv4 address", test_simple_ipv4_address),
    Case("Left-weighted IPv4 address", test_left_weighted_ipv4_address),
    Case("Right-weighted IPv4 address", test_right_weighted_ipv4_address),
    Case("Null IPv4 address", test_null_ipv4_address),

    Case("Simple IPv6 address", test_simple_ipv6_address),
    Case("Left-weighted IPv6 address", test_left_weighted_ipv6_address),
    Case("Right-weighted IPv6 address", test_right_weighted_ipv6_address),
    Case("Hollowed IPv6 address", test_hollowed_ipv6_address),
    Case("Null IPv6 address", test_null_ipv6_address),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
