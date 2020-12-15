
####################
# UNIT TESTS
####################
set(TEST_SUITE_NAME "SFDP")

# Source files
set(unittest-sources
  ../storage/blockdevice/source/SFDP.cpp
)

# Test files
set(unittest-test-sources
  ../storage/blockdevice/tests/UNITTESTS/SFDP/test_sfdp.cpp
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-flags
  -DDEVICE_SPI
)
