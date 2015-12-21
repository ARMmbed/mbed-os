/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

#include "thread_meshcop_lib.h"
#include "common_functions.h"
#include <string.h>

uint8_t *thread_meshcop_tlv_data_write(uint8_t *ptr, uint8_t type, uint16_t length, const uint8_t *data)
{
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint8(uint8_t *ptr, uint8_t type, uint8_t data)
{
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint16(uint8_t *ptr, uint8_t type, uint16_t data)
{
    return NULL;
}
uint8_t *thread_meshcop_tlv_data_write_uint32(uint8_t *ptr, uint8_t type, uint32_t data)
{
    return NULL;
}

uint16_t thread_meshcop_tlv_find(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t **result_ptr)
{
    return 0;

}

uint8_t thread_meshcop_tlv_data_get_uint8(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t *data_ptr)
{
    return 0;
}

uint8_t thread_meshcop_tlv_data_get_uint16(const uint8_t *ptr, uint16_t length, uint8_t type, uint16_t *data_ptr)
{
    return 0;
}

uint8_t thread_meshcop_tlv_data_get_uint32(const uint8_t *ptr, uint16_t length, uint8_t type, uint32_t *data_ptr)
{
    return 0;
}
