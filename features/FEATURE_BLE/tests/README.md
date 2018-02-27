# mbed BLE host tests

This folder contains tests for mbed BLE that can be run on an x86 host.

## Getting started

Run the following instructions to build the tests:

```
cd features/FEATURE_BLE/tests
mkdir build
cd build
cmake ..
make
```

The various tests applications are present at the root of the test folder;
execute them to run the tests. As an examples gatt client related tests can be
run with:

```
./gatt-client-tests
```

## Requirements

These tests requires cmake on the host and a compliant C++14 compiler.

## Resources

These tests use extensively google test (gtest) and google mock. Please refer to
the documentation of each products to get more insight:
* gtest: https://github.com/google/googletest/tree/master/googletest/docs
* gmock: https://github.com/google/googletest/tree/master/googlemock/docs

