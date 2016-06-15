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

#ifndef APP_ERROR_WEAK_H__
#define APP_ERROR_WEAK_H__

/** @file
 *
 * @defgroup app_error Common application error handler
 * @{
 * @ingroup app_common
 *
 * @brief Common application error handler.
 */
 
/**@brief       Callback function for asserts in the SoftDevice.
 *
 * @details     A pointer to this function will be passed to the SoftDevice. This function will be
 *              called by the SoftDevice if certain unrecoverable errors occur within the
 *              application or SoftDevice.
 *
 *              See @ref nrf_fault_handler_t for more details.
 *
 * @param[in] id    Fault identifier. See @ref NRF_FAULT_IDS.
 * @param[in] pc    The program counter of the instruction that triggered the fault, or 0 if
 *                  unavailable.
 * @param[in] info  Optional additional information regarding the fault. Refer to each fault
 *                  identifier for details.
 *
 * @remarks Function is implemented as weak so that it can be overwritten by custom application
 *          error handler when needed.
 */
#ifdef __CC_ARM
	     void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info);
#else
__WEAK void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info);
#endif


/** @} */

#endif // APP_ERROR_WEAK_H__
