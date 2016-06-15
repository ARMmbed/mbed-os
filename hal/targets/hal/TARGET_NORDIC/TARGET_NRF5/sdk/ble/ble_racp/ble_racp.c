/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */
 
#include "ble_racp.h"
#include <stdlib.h>


void ble_racp_decode(uint8_t data_len, uint8_t * p_data, ble_racp_value_t * p_racp_val)
{
    p_racp_val->opcode      = 0xFF;
    p_racp_val->operator    = 0xFF;
    p_racp_val->operand_len = 0;
    p_racp_val->p_operand   = NULL;
    
    if (data_len > 0)
    {
        p_racp_val->opcode = p_data[0];
    }
    if (data_len > 1)
    {
        p_racp_val->operator = p_data[1];      //lint !e415
    }
    if (data_len > 2)
    {
        p_racp_val->operand_len = data_len - 2;
        p_racp_val->p_operand   = &p_data[2];  //lint !e416
    }
}


uint8_t ble_racp_encode(const ble_racp_value_t * p_racp_val, uint8_t * p_data)
{
    uint8_t len = 0;
    int     i;

    if (p_data != NULL)
    {
        p_data[len++] = p_racp_val->opcode;
        p_data[len++] = p_racp_val->operator;

        for (i = 0; i < p_racp_val->operand_len; i++)
        {
            p_data[len++] = p_racp_val->p_operand[i];
        }
    }

    return len;
}
