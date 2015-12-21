/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

/*
 * \file thread_dynamic_reed.c
 *
 */
#include "config.h"
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "6LoWPAN/Thread/thread_dynamic_reed.h"

bool thread_dynamic_reed_upgrade(uint8_t activeRouterCnt, uint8_t routerUpgradeThreshold)
{
    return false;
}

bool thread_dynamic_reed_downgrade(uint8_t childCount, uint8_t goodLinkCount, uint8_t activeRouterCnt, bool possibleDefaultRouter, uint8_t routerDowngradeThreshold, uint8_t minNeighborsThreshold)
{
    return false;
}

void thread_dynamic_reed_initialize(thread_router_select_t *routerSelect)
{
}

uint32_t thread_dynamic_reed_timeout_calculate(thread_router_select_t *routerSelect)
{
    return 0;
}
