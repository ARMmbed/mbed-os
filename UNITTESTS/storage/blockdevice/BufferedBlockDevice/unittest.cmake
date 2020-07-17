
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/BufferedBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  storage/blockdevice/BufferedBlockDevice/test_BufferedBlockDevice.cpp
  stubs/BlockDevice_mock.h
)
