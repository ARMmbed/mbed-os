# Table of contents

* [greentea-client](#greentea-client)
  * [Concepts](#concepts)
    * [Test suite](#test-suite)
    * [Test case](#test-case)
    * [key-value protocol](#key-value-protocol)
* [Test example](#test-examplel)

# greentea-client

`greentea-client` is a client library for [the Greentea test tool](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-greentea) when used in an [Mbed OS](https://os.mbed.com/) project.

This package implements the client side of the key-value protocol used  for communication between the device under test (DUT) and the host. The `Greentea` tool implements the protocol's host behavior. We use [utest](https://github.com/ARMmbed/mbed-os/blob/master/features/frameworks/utest/README.md) as our test harness.

```
      DUT  <--- serial port connection --->  host
    (client)         .                       (host)
                    .
[greentea-client]   .       [conn_process]               [htrun]
     =====          .      ================             =========
       |            .             |                         |
       |            .             |                         |
       |    {{ key ; value }}     |                         |
       |------------------------->| (key, value, timestamp) |
       |            .             |------------------------>|
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             | (key, value, timestamp) |
       |    {{ key ; value }}     |<------------------------|
       |<-------------------------|                         |
       |            .             |                         |
                    .
```

## Concepts

### Test suite

A test suite is a binary containing test cases we execute on hardware. The test suite has a beginning and an end (like the `main()` function would. The test suite may pass, fail or be in an error state (for example, if the test suite times out or there was a serial port connection problem).

### Test case

A test case is contained within a test suite. You can have multiple test cases within a test suite. `unity` assert macros are used to run the checks during the test case. Your test cases may pass, fail or be in an error state.

### Key-value protocol

The key-value protocol is a protocol specific to the `Greentea` test tool. It is used to send messages (events) between the DUT and the host. Each message consists of a _key_ and _value_ pair. A message is surrounded by double curly braces. The key and value are separated by a semicolon (`;`).

For example, the `{{timeout;120}}}` string is a key-value message where the key `timeout` has the value `120`. Both `greentea-client` and `Greentea` understand this format and can detect key-value messages in a data stream.

## Test example

```c++
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

void test_case_1_func() {
    // Test case #1 body
    // Here you can run your test cases and assertions
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
}

void test_case_2_func() {
    // Test case #2 body
    // Here you can run your test cases and assertions
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
}

const Case cases[] = {
    Case("Test case #1 name", test_case_1_func),
    Case("Test case #1 name", test_case_2_func)
};

status_t greentea_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

void main(int, char*[]) {
    Harness::run(specification);
}
```
