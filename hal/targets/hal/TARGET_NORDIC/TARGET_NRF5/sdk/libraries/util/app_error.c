/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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

/** @file
 *
 * @defgroup app_error Common application error handler
 * @{
 * @ingroup app_common
 *
 * @brief Common application error handler.
 */

#include "nrf.h"
#include <stdio.h>
#include "app_error.h"
#include "nordic_common.h"
#include "sdk_errors.h"
#include "nrf_log.h"

#ifdef DEBUG
#include "bsp.h"
#endif



/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */

/*lint -save -e14 */
void app_error_handler(ret_code_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    error_info_t error_info =
    {
        .line_num    = line_num,
        .p_file_name = p_file_name,
        .err_code    = error_code,
    };
    app_error_fault_handler(NRF_FAULT_ID_SDK_ERROR, 0, (uint32_t)(&error_info));

    UNUSED_VARIABLE(error_info);
}

/*lint -save -e14 */
void app_error_handler_bare(ret_code_t error_code)
{
    error_info_t error_info =
    {
        .line_num    = 0,
        .p_file_name = NULL,
        .err_code    = error_code,
    };
    app_error_fault_handler(NRF_FAULT_ID_SDK_ERROR, 0, (uint32_t)(&error_info));

    UNUSED_VARIABLE(error_info);
}


void app_error_save_and_stop(uint32_t id, uint32_t pc, uint32_t info)
{
    /* static error variables - in order to prevent removal by optimizers */
    static volatile struct
    {
        uint32_t        fault_id;
        uint32_t        pc;
        uint32_t        error_info;
        assert_info_t * p_assert_info;
        error_info_t  * p_error_info;
        ret_code_t      err_code;
        uint32_t        line_num;
        const uint8_t * p_file_name;
    } m_error_data = {0};

    // The following variable helps Keil keep the call stack visible, in addition, it can be set to
    // 0 in the debugger to continue executing code after the error check.
    volatile bool loop = true;
    UNUSED_VARIABLE(loop);

    m_error_data.fault_id   = id;
    m_error_data.pc         = pc;
    m_error_data.error_info = info;

    switch (id)
    {
        case NRF_FAULT_ID_SDK_ASSERT:
            m_error_data.p_assert_info = (assert_info_t *)info;
            m_error_data.line_num      = m_error_data.p_assert_info->line_num;
            m_error_data.p_file_name   = m_error_data.p_assert_info->p_file_name;
            break;

        case NRF_FAULT_ID_SDK_ERROR:
            m_error_data.p_error_info = (error_info_t *)info;
            m_error_data.err_code     = m_error_data.p_error_info->err_code;
            m_error_data.line_num     = m_error_data.p_error_info->line_num;
            m_error_data.p_file_name  = m_error_data.p_error_info->p_file_name;
            break;
    }

    UNUSED_VARIABLE(m_error_data);

    // If printing is disrupted, remove the irq calls, or set the loop variable to 0 in the debugger.
    __disable_irq();

    while(loop);

    __enable_irq();
}

/*lint -restore */
