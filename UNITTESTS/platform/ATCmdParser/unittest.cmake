####################
# UNIT TESTS
####################

# Source files
set(unittest-sources
  ../platform/source/ATCmdParser.cpp
)

# Test files
set(unittest-test-sources
  platform/ATCmdParser/test_ATCmdParser.cpp
  stubs/FileHandle_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/mbed_poll_stub.cpp
)
