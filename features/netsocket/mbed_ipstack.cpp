/*
 * TEMPORARY FILE
 * this file will be removed once the EMAC refactoring is completed
 */

#include "platform/mbed_toolchain.h"
#include "mbed_ipstack.h"

#include "mbed_error.h"

extern "C" {

MBED_WEAK void mbed_ipstack_init(void) {
    error("Error, this function shouldn't be called!");
}

MBED_WEAK nsapi_error_t mbed_ipstack_add_ethernet_interface(const emac_interface_ops_t *emac_ops, void *hw, bool default_if, mbed_ipstack_interface_t **interface_out) {
    error("Error, this function shouldn't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK nsapi_error_t mbed_ipstack_bringup(mbed_ipstack_interface_t *interface, bool dhcp, const char *ip,
                                   const char *netmask, const char *gw, nsapi_ip_stack_t stack) {
    error("Error, this function shouldn't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK nsapi_error_t mbed_ipstack_bringdown(mbed_ipstack_interface_t *interface) {
    error("Error, this function shouldn't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK char *mbed_ipstack_get_mac_address(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen) {
    error("Error, this function shouldn't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_ip_address(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen) {
    error("Error, this function shouldn't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_netmask(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen) {
    error("Error, this function shouldn't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_gateway(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen) {
    error("Error, this function shouldn't be called!");
    return NULL;
}

} // extern "C"

MBED_WEAK NetworkStack *mbed_ipstack_get_stack(void) {
    error("Error, this function shouldn't be called!");
    return NULL;
}
