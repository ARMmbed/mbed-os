/*
 * TEMPORARY FILE
 * this file will be removed once the EMAC refactoring is completed
 */

#include "platform/mbed_toolchain.h"
#include "mbed_ipstack.h"

#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "mbed_ipstack"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)


MBED_WEAK void mbed_ipstack_init(void) {
    tr_error("Error, this function shouln't be called!");
}

MBED_WEAK nsapi_error_t mbed_ipstack_add_ethernet_interface(const emac_interface_ops_t *emac_ops, void *hw, bool default_if, mbed_ipstack_interface_t **interface_out) {
    tr_error("Error, this function shouln't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK nsapi_error_t mbed_ipstack_bringup(mbed_ipstack_interface_t *interface, bool dhcp, const char *ip,
                                   const char *netmask, const char *gw) {
    tr_error("Error, this function shouln't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK nsapi_error_t mbed_ipstack_bringdown(mbed_ipstack_interface_t *interface) {
    tr_error("Error, this function shouln't be called!");
    return NSAPI_ERROR_UNSUPPORTED;
}

MBED_WEAK char *mbed_ipstack_get_mac_address(mbed_ipstack_interface_t *interface) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_ip_address(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_netmask(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}

MBED_WEAK char *mbed_ipstack_get_gateway(mbed_ipstack_interface_t *interface, char *buf, nsapi_size_t buflen) {
    tr_error("Error, this function shouln't be called!");
    return NULL;
}
