
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  ../connectivity/cellular/tests/UNITTESTS/framework/common/util
  ../connectivity/cellular/include/cellular/framework/common
)

# Source files
set(unittest-sources
)

# Test files
set(unittest-test-sources
  ${CMAKE_CURRENT_LIST_DIR}/listtest.cpp
)
