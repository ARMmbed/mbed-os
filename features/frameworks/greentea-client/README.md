# Table of Contents

* [greentea-client](#greentea-client)
  * [mbed-drivers dependencies](#mbed-drivers-dependencies)
  * [Greentea test tools](#greentea-test-tools)
  * [Compatibility](#compatibility)
    * [Greentea support](#greentea-support)
    * [utest support](#utest-support)
    * [greentea-client support in your module](#greentea-client-support-in-your-module)
  * [Terms](#terms)
    * [Test suite](#test-suite)
    * [Test case](#test-case)
    * [key-value protocol](#key-value-protocol)
  * [Where can I use it?](#where-can-i-use-it)
* [Test suite model](#test-suite-model)
  * [utest support template](#utest-support-template)
  * [No utest support template](#no-utest-support-template)

# greentea-client

**Tip:** If you are unsure about the terms used in this document, please see the [Terms](#Terms) section.

```greentea-client``` is a C++ client library for [the Greentea test tool](https://github.com/ARMmbed/greentea) when used in an [mbed](www.mbed.com) project.
This package implements the slave side of the simple key-value protocol used  for communication between the device under test (DUT) and the host. ```Greentea``` on the host side implements the protocol's master behavior. ```greentea-client``` is released through the [yotta registry](https://yotta.mbed.com/#/module/greentea-client/0.1.8).


```
      DUT  <--- serial port connection --->   host
    (slave)         .                       (master)
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

```greentea-client``` is a [yotta module](http://yottadocs.mbed.com/reference/module.html). You can easily include it in your yotta project as ```dependency```/```testDependency```.

## mbed-drivers dependencies

This package was introduced as a future replacement for the [mbed-drivers/test_env.h](https://github.com/ARMmbed/mbed-drivers/blob/master/mbed-drivers/test_env.h) test framework. ```mbed-drivers/test_env.h``` is no longer considered the correct way to write tests for mbed modules.

Examples of test cases ported from the old ```mbed-drivers/test_env``` to the ```greentea-client``` model can be found here:
* [core-util](https://github.com/ARMmbed/core-util/pull/95)
* [ualloc](https://github.com/ARMmbed/ualloc/pull/24) 
* [minar](https://github.com/ARMmbed/minar/pull/36)
* [mbed-drivers](https://github.com/ARMmbed/mbed-drivers/pull/165)
* [sal](https://github.com/ARMmbed/sal/pull/46)
* [sockets](https://github.com/ARMmbed/sockets/pull/64)
* [utest](https://github.com/ARMmbed/utest/pull/6)

## Greentea test tools

```Greentea``` is a test automation tool written in Python and designed to automate test execution for [mbed](www.mbed.com) projects encapsulated as [yotta](http://yottadocs.mbed.com) modules. Its key features include: integration with yotta module, test automation for yotta module's tests and reporting.

## Compatibility

```greentea-client``` is compatible with:
* [Greentea](https://github.com/ARMmbed/greentea) v0.2.x onwards
* [htrun](https://github.com/ARMmbed/htrun) v0.2.x onwards
* [utest](https://github.com/ARMmbed/utest) v1.10.0 onwards
* [unity](https://github.com/ARMmbed/utest) v2.0.1 onwards

### Greentea support

If you wish to use ```greentea-client``` please make sure you are using latest ```Greentea``` tools from [PyPI](https://pypi.python.org/pypi/mbed-greentea) (you will need Python 2.7):
```
$ pip install mbed-greentea --upgrade
```

*Note*: If you previously used ```mbed-drivers/test_env.h``` features to write your test cases please downgrade ```Greentea``` to a version lower than ```v0.2.0``` to stay compatible:
```
$ pip install "mbed-greentea<0.2.0" --upgrade
```

*Note*: [Greentea v0.1.x](https://github.com/ARMmbed/greentea/tree/master_v0_1_x) is still developed on a ```master_v0_1_x``` branch. We will only apply critical patches to version 0.1.x, no feature development is planned.

### utest support

```utest``` is our preferred test harness which allows you to execute a series of (asynchronous) C++ test cases.  ```greentea-client``` includes as test dependency ```utest``` yotta module. See the ```greentea-client``` [module.json](https://github.com/ARMmbed/greentea-client/blob/master/module.json) ```testDependency``` section:
```json
{
  "testDependencies": {
    "utest": "^1.10.0"
  }
}
```

### greentea-client support in your module

Currently ```greentea-client``` is in on version ```1.0.x```. Please use a ```^1.0.0``` dependency version in your ```module.json``` file.

Example of ```module.json``` file with ```greentea-client``` as a test dependency:
```json
{
  "testDependencies": {
    "greentea-client": "^1.0.0",
    "utest" : "^1.10.0",
    "unity" : "^2.1.0"
  }
}
```

## Terms

### Test suite
A test suite is a binary containing test cases we execute on hardware. The test suite has a beginning and an end (like your ```main()``` function would. The test suite may pass, fail or be in an error state (for example if test suite times out or there was a serial port connection problem).

### Test case

Preferably you will use ```utest``` to define test cases . test case has the beginning and the end. During test case execution you will use ``ùnity``` assert macros, schedule MINAR callbacks, check for timeouts in your code. Your test cases may pass, fail or be in an error state which means something went wrong and we were unable to determine exactly what that was (you may have to check the logs).

### key-value protocol

The key-value protocol (also called ```KiVi```) is a simple protocol introduced to the ```Greentea``` test tools. It is used to send simple text messages (events) between the DUT and the host. Each message consists of a _key_ and corresponding _value_ pair.
A ```KiVi``` message is defined as a string encapsulated between double curly braces. The key and value are separated by a semicolon (```;```).
For example: the ```{{timeout;120}}}``` string is a simple key-value message where the key "_timeout_" is associated with the value "_120_". Both ```greentea-client``` and ```Greentea``` understand this format and can detect key-value messages in a data stream. Both _key_ and _value_ are ASCII strings.
This protocol is a master-slave protocol. The host has the role of _master_ and the DUT is the _slave_.

```greentea-client``` implements the key-value protocol tokenizer and parser.

## Where can I use it?
It is possible to write test cases that use ```greentea-client``` and at the same time support ```mbed``` features such as [MINAR scheduler](https://github.com/ARMmbed/minar). It is also possible to mix ```greentea-client``` with other test tools we use at ```mbed``` such as [utest](https://github.com/ARMmbed/utest) and [unity](https://github.com/ARMmbed/unity).

You can also find references to ```greentea-client``` in many ```mbed``` packages. For example:
  * [mbed-drivers](https://github.com/ARMmbed/mbed-drivers) - check its [module.json](https://github.com/ARMmbed/mbed-drivers/blob/master/module.json)
  * [core-util](https://github.com/ARMmbed/core-util) - check its [module.json](https://github.com/ARMmbed/core-util/blob/master/module.json)

# Test suite model
## utest support template
```c++
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

void app_start(int, char*[]) {
    Harness::run(specification);
}
```
## No utest support template
```c++
#include "greentea-client/test_env.h"
#include "unity/unity.h"

void app_start(int, char*[]) {
    bool result = true;
    GREENTEA_SETUP(15, "default_auto");

    // Test suite body
    // Here you can run your test cases and or assertions
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);

    GREENTEA_TESTSUITE_RESULT(result);
}
```
