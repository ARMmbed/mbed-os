#ifndef CMD_IFCONFIG_H
#define CMD_IFCONFIG_H

#include "NetworkInterface.h"
#include "NetworkStack.h"

/** Get a pointer to a network interface instance
 *
 * Allowed interface types (depend on application configurations):
 * cell0, wlan0, eth0, mesh0
 *
 *  @return         pointer to the network interface, or NULL if unrecognized or ambiguous
 */
NetworkInterface* get_interface(void);

void cmd_ifconfig_init(void);
const char* networkstack_error_to_str(int errorcode);

#endif
