
####################
# UNIT TESTS
####################

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  connectivity/cellular/framework/common/util
  ../connectivity/cellular/framework/common
)

# Source files
set(unittest-sources
)

# Test files
set(unittest-test-sources
  connectivity/cellular/framework/common/list/listtest.cpp
)
