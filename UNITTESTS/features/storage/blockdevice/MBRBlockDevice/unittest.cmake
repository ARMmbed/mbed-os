
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../features/storage/blockdevice/MBRBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  features/storage/blockdevice/MBRBlockDevice/test_MBRBlockDevice.cpp
)
