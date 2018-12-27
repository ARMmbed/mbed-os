/* emac_lwip_l2_bridge.h */
/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef EMAC_LWIP_L2_BRIDGE_H_
#define EMAC_LWIP_L2_BRIDGE_H_

#include "lwip/netif.h"
#include "EMACMemoryManager.h"

#ifndef EMAC_LWIP_L2B_MAX_BRIDGE_ENTRIES
#define EMAC_LWIP_L2B_MAX_BRIDGE_ENTRIES    (10)
#endif
#ifndef EMAC_LWIP_L2B_MAX_NETIFS
#define EMAC_LWIP_L2B_MAX_NETIFS            (2)
#endif
#ifndef EMAC_LWIP_L2B_TIMER_INTERVAL
#define EMAC_LWIP_L2B_TIMER_INTERVAL        (1000) //~timer interval in ms
#endif
#ifndef EMAC_LWIP_L2B_ENTRY_TIMEOUT
#define EMAC_LWIP_L2B_ENTRY_TIMEOUT         (120)  //timer ticks before removing inactive L2B entry
#endif

err_t emac_lwip_l2b_register_interface(struct netif *netif);

err_t emac_lwip_l2b_output(struct netif *netif, emac_mem_buf_t *buf);

err_t emac_lwip_l2b_input(struct netif *netif, emac_mem_buf_t *buf);


#endif // #ifndef EMAC_LWIP_L2_BRIDGE_H_

