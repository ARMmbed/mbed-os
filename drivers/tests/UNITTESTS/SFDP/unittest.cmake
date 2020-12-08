####################
# UNIT TESTS
####################
set(TEST_SUITE_NAME "SFDP")

# Source files
set(unittest-sources ../drivers/source/SFDP.cpp)

# Test files
set(unittest-test-sources ../drivers/tests/UNITTESTS/SFDP/test_sfdp.cpp
                          stubs/mbed_assert_stub.cpp
)

set(unittest-test-flags -DDEVICE_SPI)
