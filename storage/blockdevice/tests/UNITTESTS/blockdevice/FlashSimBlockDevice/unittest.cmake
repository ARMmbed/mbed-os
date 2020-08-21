
####################
# UNIT TESTS
####################

set(unittest-includes ${unittest-includes}
  .
  ..
)

set(unittest-sources
  ../storage/blockdevice/source/FlashSimBlockDevice.cpp
  stubs/mbed_atomic_stub.c
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/test_FlashSimBlockDevice.cpp
)
