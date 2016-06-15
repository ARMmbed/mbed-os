/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
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

#include "app_error.h"

#ifdef DEBUG
#include "bsp.h"
#endif
 
/*lint -save -e14 */

/**
 * Function is implemented as weak so that it can be overwritten by custom application error handler
 * when needed.
 */
__WEAK void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    // On assert, the system can only recover with a reset.
#ifndef DEBUG
    NVIC_SystemReset();
#else

#ifdef BSP_DEFINES_ONLY
    LEDS_ON(LEDS_MASK);
#else
    UNUSED_VARIABLE(bsp_indication_set(BSP_INDICATE_FATAL_ERROR));
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Uncomment the line below to use.
    //ble_debug_assert_handler(error_code, line_num, p_file_name);
#endif // BSP_DEFINES_ONLY

    app_error_save_and_stop(id, pc, info);

#endif // DEBUG
}

/*lint -restore */


