/* LWIP common helpers
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LWIP_TOOLS_H
#define LWIP_TOOLS_H

#include <stdbool.h>

#include "lwip/netif.h"
#include "lwip/ip.h"

struct mbed_lwip_socket {
    bool in_use;

    struct netconn *conn;
    struct netbuf *buf;
    u16_t offset;

    void (*cb)(void *);
    void *data;
};

extern struct mbed_lwip_socket mbed_lwip_arena[MEMP_NUM_NETCONN];

const ip_addr_t *mbed_lwip_get_ip_addr(bool any_addr, const struct netif *netif);
void mbed_lwip_arena_init(void);
struct mbed_lwip_socket *mbed_lwip_arena_alloc(void);
void mbed_lwip_arena_dealloc(struct mbed_lwip_socket *s);

#endif /* LWIP_TOOLS_H */
