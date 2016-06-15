/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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

/**@file
 *
 * @defgroup nrf_dfu_transport DFU transport API.
 * @{     
 *  
 * @brief DFU transport module interface.
 */
 
#ifndef DFU_TRANSPORT_H__
#define DFU_TRANSPORT_H__

#include <stdint.h>

/**@brief Function for starting the update of Device Firmware.
 *
 * @retval NRF_SUCCESS Operation success.   
 */
uint32_t dfu_transport_update_start(void);

/**@brief Function for closing the transport layer.
 *
 * @retval NRF_SUCCESS Operation success.    
 */
uint32_t dfu_transport_close(void);

#endif // DFU_TRANSPORT_H__

/**@} */
