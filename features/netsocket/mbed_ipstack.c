/*
 * TEMPORARY FILE
 * this file will be removed once the EMAC refactoring is completed
 */

#include "platform/mbed_toolchain.h"
#include "mbed_ipstack.h"
#include "ns_trace.h"

#define TRACE_GROUP "mbed_ipstack"

MBED_WEAK void mbed_ipstack_init(void) {
    tr_error("Error, this function shouln't be called!");
}

MBED_WEAK nsapi_error_t mbed_ipstack_add_interface(emac_interface_t *emac, bool default_if) {
    tr_error("Error, this function shouln't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK void mbed_ipstack_set_stack(emac_interface_t *emac, nsapi_stack_t *stack) {
    tr_error("Error, this function shouln't be called!");
}

MBED_WEAK nsapi_error_t mbed_ipstack_bringup(emac_interface_t *emac, bool dhcp, const char *ip,
                                   const char *netmask, const char *gw) {
    tr_error("Error, this function shouln't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK nsapi_error_t mbed_ipstack_bringdown(emac_interface_t *emac) {
    tr_error("Error, this function shouln't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK char *mbed_ipstack_get_mac_address(emac_interface_t *emac) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_ip_address(emac_interface_t *emac, char *buf, nsapi_size_t buflen) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_netmask(emac_interface_t *emac, char *buf, nsapi_size_t buflen) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_gateway(emac_interface_t *emac, char *buf, nsapi_size_t buflen) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}
