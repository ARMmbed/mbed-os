/*
 * (c) (2019-2020), Cypress Semiconductor Corporation. All rights reserved.
 *
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress' integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress' product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 */

/** @file
 *  Provides SCL Functionality to communicate with WiFi Chip
 */

#include "scl_common.h"
#include "cy_device.h"
#include "cy_sysint.h"
#include "cy_ipc_drv.h"
#include "scl_wifi_api.h"
#include "ip4_addr.h"

#ifndef INCLUDED_SCL_IPC_H
#define INCLUDED_SCL_IPC_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/
#define REG_IPC_STRUCT_DATA0(base)             (((IPC_STRUCT_V2_Type*)(base))->DATA0)
#define TIMER_DEFAULT_VALUE                    (100)
#define WIFI_ON_TIMEOUT                        (5000)
#define NW_CONNECT_TIMEOUT                     (30)
#define NW_DISCONNECT_TIMEOUT                  (60000)
#define NW_DELAY_TIME                          (3000000)
/******************************************************
*               Variables
******************************************************/
extern struct network_params_t network_params;
/******************************************************
*             Function Declarations
******************************************************/

/** @addtogroup communication SCL communication API
 *  APIs for communicating with NP
 *  @{
 */

/**
 * Initializes the SCL thread and necessary artifacts
 *
 *  @return SCL_SUCCESS on successful intialization
 *          SCL_ERROR otherwise
 */
extern scl_result_t scl_init(void);

/**
 * Sends the SCL data and respective command to NP
 *  @param index           Index of the command.
 *  @param buffer          Data to be sent.
 *	@param timeout(in ms)  Time to wait for IPC channel to be released.
 *
 *  @return SCL_SUCCESS on successful communication within given time
 *          SCL_ERROR on Timeout
 */
extern scl_result_t scl_send_data(int index, char *buffer, uint32_t timeout);

/**
 * Terminates the SCL thread and disables the interrupts
 *
 *  @return SCL_SUCCESS on successful communication within given time
 *          SCL_ERROR on Timeout
 */
extern scl_result_t scl_end(void);

/**
 * Gets the network parameters like IP Address, Netmask, and Gateway from NP
 *  @param network_params_t structure pointer
 *  @return SCL_SUCCESS on successful communication within given time
 *          SCL_ERROR on Timeout
 */
extern scl_result_t scl_get_nw_parameters(struct network_params_t *nw_param);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_IPC_H */

