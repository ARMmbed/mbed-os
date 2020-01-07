####################
# UNIT TESTS
####################

# Source files
set(unittest-sources
  ../platform/source/ATCmdParser.cpp
  ../platform/source/minimal-printf/mbed_printf.c
  ../platform/source/minimal-printf/mbed_printf_implementation.c
)

# Test files
set(unittest-test-sources
  platform/ATCmdParser/test_ATCmdParser.cpp
  stubs/FileHandle_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/mbed_poll_stub.cpp
)
