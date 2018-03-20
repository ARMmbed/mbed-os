/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
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
* @brief Main including header for the FNET TCP/IP stack.
*
***************************************************************************/

#ifndef _FNET_STACK_H_

#define _FNET_STACK_H_

#include "fnet_assert.h"
#include "fnet_error.h"
#include "fnet_stdlib.h"
#include "fnet_socket.h"
#include "fnet_inet.h"
#include "fnet_ip.h"
#include "fnet_ip6.h"
#include "fnet_netif.h"
#include "fnet_timer.h"
#include "fnet_debug.h"
#include "fnet_eth.h"
#include "fnet_isr.h"
#include "fnet_netbuf.h"
#include "fnet_arp.h"

/*! @addtogroup fnet_stack_init
* - The @ref fnet.h file includes all the other header files needed to use the FNET TCP/IP stack
*   user interface. This means that it is the only file the application developer needs to include
*   in the source code using the FNET stack API.
* - The @ref fnet_init() function must be called
*   in order to initialize the FNET TCP/IP stack.
*   The return value from @ref fnet_init() must be verified to indicate the success before
*   calling any other TCP/IP functions.
* - After @ref fnet_init() returns the @ref FNET_OK value, the FNET TCP/IP stack is ready
*   for use.
* - Initialize required networking interfaces using @ref fnet_netif_init().
*
* For example:
* @code
* ...
*    static fnet_uint8_t stack_heap[FAPP_CFG_HEAP_SIZE];
*    struct fnet_init_params init_params;
*
*    // Input parameters for FNET stack initialization.
*    init_params.netheap_ptr = stack_heap;
*    init_params.netheap_size = sizeof(stack_heap);
*
*    // Init FNET stack.
*    if(fnet_init(&init_params) != FNET_ERR)
*    {
*        // Initialize Network Interfaces.
	     ,,,
*        // Place your code here.
*    }
*    else
*        fnet_printf("ERROR: FNET stack initialization is failed!\n");
* ...
* @endcode
*/
/*! @{ */

#if FNET_CFG_MULTITHREADING || defined(__DOXYGEN__)

/**************************************************************************/ /*!
 * @brief Mutex type.
 * @see FNET_CFG_MULTITHREADING,
 ******************************************************************************/
typedef void *fnet_mutex_t;

/**************************************************************************/ /*!
 * @brief Mutex API.
 * It should be defined by application if @ref FNET_CFG_MULTITHREADING is enabled.
 * @see FNET_CFG_MULTITHREADING, fnet_init()
 ******************************************************************************/
typedef struct
{
    fnet_return_t (*mutex_init)( fnet_mutex_t * ); /**< @brief Create a new mutex. Parameter is pointer to the mutex to create. */
    void (*mutex_free)( fnet_mutex_t * ); /**< @brief  Delete a mutex. Parameter is pointer to the mutex to delete. */
    void (*mutex_lock)( fnet_mutex_t * ); /**< @brief  Lock a mutex. Parameter is the mutex to lock. */
    void (*mutex_unlock)( fnet_mutex_t * ); /**< @brief Unlock a mutex. Parameter is the mutex to unlock.  */
} fnet_mutex_api_t;
#endif /* FNET_CFG_MULTITHREADING */


/**************************************************************************/ /*!
 * @brief Input parameters structure for @ref fnet_init()
 ******************************************************************************/
struct fnet_init_params
{
    void                        *netheap_ptr;   /**< @brief Pointer to the FNET heap buffer. @n
                                                * @n
                                                * The FNET uses this heap buffer for the internal
                                                * dynamic data allocation as:
                                                *  - Ethernet Tx/Rx frame buffers.
                                                *  - Sockets Input/Output buffers.
                                                *  - Protocol headers and service information.
                                                *  - Various control structures.
                                                *  - Temporary data.@n
                                                * @n
                                                * An application can allocate this buffer statically,
                                                * dynamically, or use a special memory region (for example SRAM).*/
    fnet_size_t                 netheap_size;   /**< @brief Size of the FNET heap buffer. */
#if FNET_CFG_MULTITHREADING || defined(__DOXYGEN__)
    const fnet_mutex_api_t      *mutex_api;    /**< @brief Mutex API. It is optional and availble only when FNET_CFG_MULTITHREADING is set.*/
#endif
};

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************************************************/ /*!
 *
 * @brief    Initializes the FNET TCP/IP stack.
 *
 * @param init_params    Pointer to the initialization parameter structure.
 *
 * @return   This function returns:
 *   - @c FNET_OK  = Stack initialization is successful.
 *   - @c FNET_ERR = Stack initialization has failed.
 *
 *
 * @see fnet_release()
 *
 ******************************************************************************
 *
 * This function executes the initialization of the FNET TCP/IP stack.@n
 * Only after a succesful initialization, the application may use other FNET API
 * functions and services.
 *
 ******************************************************************************/
fnet_return_t fnet_init( struct fnet_init_params *init_params );

/***************************************************************************/ /*!
 *
 * @brief    Releases the FNET TCP/IP stack.
 *
 * @see fnet_init()
 *
 ******************************************************************************
 *
 * This function releases all resources occupied by the FNET TCP/IP stack.
 * But it does not release resources occupied by FNET services.
 *
 ******************************************************************************/
void fnet_release(void);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _FNET_STACK_H_ */

