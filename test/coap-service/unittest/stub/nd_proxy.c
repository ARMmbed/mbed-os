/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include "ns_types.h"

int nd_proxy_downstream_interface_unregister(int8_t interface_id)
{
    return 0;
}

bool nd_proxy_enabled_for_upstream(int8_t interface_id)
{
    return false;
}

bool nd_proxy_upstream_route_onlink(int8_t downstream_id, uint8_t *address)
{
    return false;
}
