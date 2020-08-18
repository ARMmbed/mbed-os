/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************/
/*!
* @brief FNET Services polling mechanism API.
*
***************************************************************************/

#ifndef _FNET_POLL_H_

#define _FNET_POLL_H_

/*! @addtogroup fnet_polling
* The polling mechanism enables the execution of registered services
* (DHCP client, TFTP client, Shell, Telnet server or HTTP server) in
* "background" - during the application
* idle time. Typically, the polling callback is registered during the service
* initialization (for example the @ref fnet_http_init() or other service initialization
* function).
* In order to make the polling mechanism work, the user application should
* call the @ref fnet_poll_service() API function periodically, during the idle time.@n
* @n
* Configuration parameters:
* - @ref FNET_CFG_POLL_MAX
*/
/*! @{ */

/**************************************************************************/ /*!
 * @brief Descriptor of a registered service.
 ******************************************************************************/
typedef fnet_uint32_t fnet_poll_desc_t;

/**************************************************************************/ /*!
 * @brief Service callback function prototype.
 *
 * @param service_param   This parameter is assigned during
 *                        a service registration by the @ref
 *                        fnet_poll_service_register().
 *
 ******************************************************************************/
typedef void (* fnet_poll_service_t)(void *service_param);

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************************/ /*!
 *
 * @brief    Main polling function.
 *
 * @see fnet_poll_service_register()
 *
 ******************************************************************************
 *
 * This function calls all registered service routines.@n
 * The user application should call this function periodically, after any service
 * initialization.
 *
 ******************************************************************************/
void fnet_poll_service(void);

/***************************************************************************/ /*!
 *
 * @brief    Unregisters all registered service routines.
 *
 * @see fnet_poll_service_register(), fnet_poll_service_unregister()
 *
 ******************************************************************************
 *
 * This function unregisters all registered service routines from
 * the polling list.
 *
 ******************************************************************************/
void fnet_poll_service_release(void);

/***************************************************************************/ /*!
 *
 * @brief    Registers the service routine in the polling list.
 *
 * @param service        Pointer to the service-polling routine.
 *
 * @param service_param  Service-polling-routine-specific parameter.
 *
 * @return This function returns:
 *   - Service descriptor, if no error occurs.
 *   - @c 0 if an error occurs.
 *
 * @see fnet_poll_service_unregister()
 *
 ******************************************************************************
 *
 * This function adds the service routine into the polling list.@n
 * This function is usually called during a service initialization.
 * User application should not call this function directly.
 *
 ******************************************************************************/
fnet_poll_desc_t fnet_poll_service_register( fnet_poll_service_t service, void *service_param );

/***************************************************************************/ /*!
 *
 * @brief    Unregisters the service routine.
 *
 * @param desc       Service descriptor to be unregistered.
 *
 * @see fnet_poll_service_register()
 *
 ******************************************************************************
 *
 * This function unregisters the service routine assigned to the @c desc
 * descriptor.@n
 * This function is usually called during a service release.
 * User application should not call this function directly.
 *
 ******************************************************************************/
void fnet_poll_service_unregister( fnet_poll_desc_t desc );

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif
