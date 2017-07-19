#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include MBED_CONF_APP_HEADER_FILE

using namespace utest::v1;

// Bringing the network up and down
template <int COUNT>
void test_bring_up_down() {
    NetworkInterface* net = MBED_CONF_APP_OBJECT_CONSTRUCTION;

    for (int i = 0; i < COUNT; i++) {
        int err = MBED_CONF_APP_CONNECT_STATEMENT;
        TEST_ASSERT_EQUAL(0, err);

        printf("MBED: IP Address %s\r\n", net->get_ip_address());
        TEST_ASSERT(net->get_ip_address());

        UDPSocket udp;
        err = udp.open(net);
        TEST_ASSERT_EQUAL(0, err);
        err = udp.close();
        TEST_ASSERT_EQUAL(0, err);

        TCPSocket tcp;
        err = tcp.open(net);
        TEST_ASSERT_EQUAL(0, err);
        err = tcp.close();
        TEST_ASSERT_EQUAL(0, err);

        err = net->disconnect();
        TEST_ASSERT_EQUAL(0, err);
    }
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(120, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Bringing the network up and down", test_bring_up_down<1>),
    Case("Bringing the network up and down twice", test_bring_up_down<2>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
