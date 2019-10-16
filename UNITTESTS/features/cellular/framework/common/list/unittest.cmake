
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  features/cellular/framework/common/util
  ../features/cellular/framework/common
)

# Source files
set(unittest-sources
)

# Test files
set(unittest-test-sources
  features/cellular/framework/common/list/listtest.cpp
)
