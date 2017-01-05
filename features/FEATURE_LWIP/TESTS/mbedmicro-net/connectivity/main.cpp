#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
#if DEVICE_EMAC
    #error [NOT_SUPPORTED] Not supported for WiFi targets
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "EthernetInterface.h"

using namespace utest::v1;


// Bringing the network up and down
template <int COUNT>
void test_bring_up_down() {
    EthernetInterface eth;

    for (int i = 0; i < COUNT; i++) {
        int err = eth.connect();
        TEST_ASSERT_EQUAL(0, err);

        printf("MBED: IP Address %s\r\n", eth.get_ip_address());
        printf("MBED: Netmask %s\r\n", eth.get_netmask());
        printf("MBED: Gateway %s\r\n", eth.get_gateway());
        TEST_ASSERT(eth.get_ip_address());
        TEST_ASSERT(eth.get_netmask());
        TEST_ASSERT(eth.get_gateway());

        UDPSocket udp;
        err = udp.open(&eth);
        TEST_ASSERT_EQUAL(0, err);
        err = udp.close();
        TEST_ASSERT_EQUAL(0, err);

        TCPSocket tcp;
        err = tcp.open(&eth);
        TEST_ASSERT_EQUAL(0, err);
        err = tcp.close();
        TEST_ASSERT_EQUAL(0, err);

        err = eth.disconnect();
        TEST_ASSERT_EQUAL(0, err);
    }
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing bringing the network up and down", test_bring_up_down<1>),
    Case("Testing bringing the network up and down twice", test_bring_up_down<2>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
