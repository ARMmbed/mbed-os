/* Copyright (c) 2006 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#include "nrf_assert.h"
#include "app_error.h"
#include "nordic_common.h"

#if defined(DEBUG_NRF)
void assert_nrf_callback(uint16_t line_num, const uint8_t * file_name)
{
    assert_info_t assert_info =
    {
        .line_num    = line_num,
        .p_file_name = file_name,
    };
    app_error_fault_handler(NRF_FAULT_ID_SDK_ASSERT, 0, (uint32_t)(&assert_info));

    UNUSED_VARIABLE(assert_info);
}
#endif /* DEBUG_NRF */
