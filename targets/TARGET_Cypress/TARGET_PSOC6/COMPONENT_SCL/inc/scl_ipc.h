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
 *  Provides SCL functionality to communicate with Network Processor
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
/**
 * Hardware address of IPC_DATA0 register
 */
#define REG_IPC_STRUCT_DATA0(base)             (((IPC_STRUCT_V2_Type*)(base))->DATA0)
/**
 * Default timeout value (in ms) for SCL operations
 */
#define TIMER_DEFAULT_VALUE                    (100)
/**
 * Default timeout value (in ms) for Wi-Fi on
 */
#define WIFI_ON_TIMEOUT                        (5000)
/**
 * Default timeout value (in seconds) for Wi-Fi connection
 */
#define NW_CONNECT_TIMEOUT                     (30)
/**
 * Default interval (in micro seconds) for polling the Network Processor
 */
#define NW_DELAY_TIME_US                       (3000000)
/**
 * Default parameter length
 */
#define PARAM_LEN                              (20)

/******************************************************
*               Variables
******************************************************/
/**
 * Network parameters structure.
 */
typedef struct network_params {
    char ip_address[PARAM_LEN]; /**< IP address */
    char netmask[PARAM_LEN]; /**< Netmask */
    char gateway[PARAM_LEN]; /**< Gateway */
    int  connection_status; /**< Connection status */
}network_params_t;

/******************************************************
*             Function Declarations
******************************************************/

/** @addtogroup communication SCL communication API
 *  APIs for communicating with Network Processor
 *  @{
 */

/** Initializes the SCL thread and necessary artifacts
 *
 *  @return SCL_SUCCESS on successful initialization or SCL_ERROR otherwise
 */
extern scl_result_t scl_init(void);

/** Sends the SCL data and respective command to Network Processor
 *
 *  @param index           Index of the command.
 *  @param buffer          Data to be sent.
 *	@param timeout         The maximum time (in ms) to wait for the Network Processor to release IPC channel.
 *
 *  @return SCL_SUCCESS on successful communication within SCL timeout duration or SCL_ERROR
 */
extern scl_result_t scl_send_data(int index, char *buffer, uint32_t timeout);

/** Terminates the SCL thread and disables the interrupts
 *
 *  @return SCL_SUCCESS on successful termination of SCL thread and disabling of interrupts or SCL_ERROR on timeout
 */
extern scl_result_t scl_end(void);

/** Gets the network parameters like IP Address, Netmask, and Gateway from Network Processor
 *
 *  @param  nw_param      structure pointer of type @a network_params_t
 *
 *  @return SCL_SUCCESS on successful communication or SCL_ERROR
 */
extern scl_result_t scl_get_nw_parameters(network_params_t *nw_param);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_IPC_H */