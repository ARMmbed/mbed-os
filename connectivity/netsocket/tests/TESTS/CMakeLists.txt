
# Only build the netsocket tests if this target has at least one network interface
string(FIND "${MBED_CONFIG_DEFINITIONS}" MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE DEFAULT_IFC_IDX)
if(NOT DEFAULT_IFC_IDX EQUAL -1)
    add_subdirectory(netsocket)
endif()