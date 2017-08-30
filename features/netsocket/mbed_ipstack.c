/*
 * TEMPORARY FILE
 * this file will be removed once the EMAC refactoring is completed
 */

#include "platform/mbed_toolchain.h"
#include "mbed_ipstack.h"


MBED_WEAK void mbed_ipstack_init(void);

MBED_WEAK nsapi_error_t mbed_ipstack_add_interface(emac_interface_t *emac, bool default_if);

MBED_WEAK void mbed_ipstack_set_stack(emac_interface_t *emac, nsapi_stack_t *stack);

MBED_WEAK nsapi_error_t mbed_ipstack_bringup(emac_interface_t *emac, bool dhcp, const char *ip,
                                   const char *netmask, const char *gw);

MBED_WEAK nsapi_error_t mbed_ipstack_bringdown(emac_interface_t *emac);

MBED_WEAK char *mbed_ipstack_get_mac_address(emac_interface_t *emac);

MBED_WEAK char *mbed_ipstack_get_ip_address(emac_interface_t *emac, char *buf, nsapi_size_t buflen);

MBED_WEAK char *mbed_ipstack_get_netmask(emac_interface_t *emac, char *buf, nsapi_size_t buflen);

MBED_WEAK char *mbed_ipstack_get_gateway(emac_interface_t *emac, char *buf, nsapi_size_t buflen);
