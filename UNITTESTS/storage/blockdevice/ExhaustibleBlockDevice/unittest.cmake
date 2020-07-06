
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/ExhaustibleBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  storage/blockdevice/ExhaustibleBlockDevice/test_ExhaustibleBlockDevice.cpp
)
