/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */

#include "config.h"

#include "thread_beacon.h"
#include "thread_config.h"
#include "common_functions.h"
#include "thread_meshcop_lib.h"
#include <string.h>
#include "ns_trace.h"
/*service lib includes */
#include "Service_Libs/utils/crc.h"
#define TRACE_GROUP "Tbcn"

#define THREAD_BEACON_VERSION_SHIFT                 4
#define THREAD_BEACON_JOINING_PERMITTED_BIT         0x01
#define THREAD_BEACON_NATIVE_COMMISSIONER_BIT       0x08

int thread_beacon_get_protocol(uint8_t *data_ptr, uint8_t data_len)
{
	return 0;
}
int thread_beacon_get_protocol_version(uint8_t *data_ptr, uint8_t data_len)
{
	return 0;
}
uint16_t thread_beacon_get_joiner_port(uint8_t *data_ptr, uint8_t data_len)
{
	return 0;
}

uint16_t thread_beacon_get_commissioning_port(uint8_t *data_ptr, uint8_t data_len)
{
	return 0;
}
bool thread_beacon_native_commissioning_allowed(uint8_t *data_ptr, uint8_t data_len)
{
	return false;
}
bool thread_beacon_joining_permitted(uint8_t *data_ptr, uint8_t data_len)
{
	return false;
}

int thread_beacon_get_name(uint8_t *data_ptr, uint8_t data_len, uint8_t *result_ptr)
{
	return 16;
}
int thread_beacon_get_xpanid(uint8_t *data_ptr, uint8_t data_len, uint8_t *result_ptr)
{
	return 8;
}
uint8_t *thread_beacon_get_steering_data(uint8_t *data_ptr, uint8_t data_len,uint8_t *result_length)
{
	return NULL;
}


uint8_t *thread_beacon_calculate_bloom_filter(uint8_t *bloom_filter_ptr,uint8_t bloom_filter_length, uint8_t *joiner_id_ptr, int joiner_id_length)
{
	return NULL;
}

