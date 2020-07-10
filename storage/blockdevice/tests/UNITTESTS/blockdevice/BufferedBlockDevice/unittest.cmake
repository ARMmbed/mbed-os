
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/source/BufferedBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)
set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/test_BufferedBlockDevice.cpp
  stubs/BlockDevice_mock.h
)
