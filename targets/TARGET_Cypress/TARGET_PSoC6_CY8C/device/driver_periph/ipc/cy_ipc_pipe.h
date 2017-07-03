/***************************************************************************//**
* \file
* \version 1.0
*
*  Description:
*   IPC Pipe Driver - This header file contains all the function prototypes,
*   structure definitions, pipe constants, and pipe endpoint address definitions.
*
*   Warning: The API for the pipe level functions are preliminary and may change
*   prior to release.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#ifndef CY_IPC_PIPE_H
#define CY_IPC_PIPE_H

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "syslib/cy_syslib.h"
#include "cy_ipc_drv.h"


/*
 * This section defines the system level constants required to define
 * callback arrays for the Cypress pipe and the user pipe.  These defines
 * are used for both the max callback count and maximum clients.
*/

typedef void (* cy_ipc_pipe_callback_ptr_t)(uint32_t * msgPtr);   /**< Typedef for pipe callback function pointer */
typedef void (* cy_ipc_pipe_relcallback_ptr_t)(void);             /**< Typedef for a pipe release callback function pointer */
typedef void (** cy_ipc_pipe_callback_array_ptr_t)(uint32_t * msgPtr); /**< Typedef for array of callback function pointers */


/**
* \addtogroup group_ipc_macro
* \{
*/

/*
 * The System pipe address is what is used to send a message to one of the
 * endpoints of a pipe.  Currently the Cypress pipe and the User pipe
 * are supported.  For parts with extra IPC channels users may create
 * their own custom pipes and create their own pipe addresses.
 *
 * The format of the endPoint configuration
 *    Bits[31:16] Interrupt Mask
 *    Bits[15:8 ] IPC interrupt
 *    Bits[ 7:0 ] IPC channel
 */
#define CY_IPC_PIPE_CFG_IMASK_SHIFT    (16UL)         /**< Interrupt mask shift value for Pipe functions */
#define CY_IPC_PIPE_CFG_IMASK_MASK     (0xFFFF0000UL) /**< Interrupt mask for endpoint address */
#define CY_IPC_PIPE_CFG_INTR_SHIFT     (8UL)          /**< Interrupt shift Pipe functions */
#define CY_IPC_PIPE_CFG_INTR_MASK      (0x0000FF00UL) /**< Interrupt mask for endpoint address */
#define CY_IPC_PIPE_CFG_CHAN_SHIFT     (0UL)          /**< Interrupt shift Pipe functions */
#define CY_IPC_PIPE_CFG_CHAN_MASK      (0x000000FFUL) /**< Pipe channel address for endpoint address */



#define CY_IPC_PIPE_MSG_CLIENT_MASK  (0x000000FFul)    /**< Client mask for first word of Pipe message */
#define CY_IPC_PIPE_MSG_USR_MASK     (0x0000FF00ul)    /**< User data mask for first word of Pipe message */
#define CY_IPC_PIPE_MSG_RELEASE_MASK (0xFFFF0000ul)    /**< Mask for message release mask */
#define CY_IPC_PIPE_MSG_RELEASE_SHIFT (16UL)           /**< Shift require to line up mask to LSb */

#define CY_IPC_PIPE_ENDPOINT_BUSY     (1UL)       /**< Use to set the busy flag when waiting for a release interrupt */
#define CY_IPC_PIPE_ENDPOINT_NOTBUSY  (0UL)       /**< Denotes that a release interrupt is not pending */

/** \} group_ipc_macro */

/**
* \addtogroup group_ipc_data_structures
* \{
*/

/*
 * This is the definition of a pipe endpoint.  There is one endpoint structure for each CPU in
 * a pipe.  It contains all the information to process a message sent from one of the other
 * CPUs in the pipe.
 */
/**
* This is the definition of a pipe endpoint.  There is one endpoint structure for each CPU in
* a pipe.  It contains all the information to process a message send from one of the other
* CPUs in the pipe.
*/
typedef struct
{
    uint32_t         ipcChan;         /**< IPC channel number used for this endpoint to receive messages             */
    uint32_t         intrChan;        /**< IPC interrupt channel number used for this endpoint to receive interrupts */
    uint32_t         pipeIntMask;     /**< Release/Notify interrupt mask that includes all endpoints on pipe         */
    IPC_STRUCT_Type *   ipcPtr;          /**< Pointer to receive IPC channel ( If ptr == NULL, cannot receive )         */
    IPC_INTR_STRUCT_Type * ipcIntrPtr;  /**< Pointer to IPC interrupt, needed to clear the interrupt                   */
    uint32_t         busy;            /**< Endpoint busy flag.  If sent no messages can be sent from this endpoint   */
    uint32_t         clientCount;     /**< Client count and size of MsgCallback array                                */
    cy_ipc_pipe_callback_array_ptr_t callbackArray; /**< Pointer to array of callback functions, one for each Client  */
    cy_ipc_pipe_relcallback_ptr_t releaseCallbackPtr;  /**< Pointer to release callback function                    */
    cy_ipc_pipe_relcallback_ptr_t defaultReleaseCbPtr; /**< Pointer to default release callback function            */
} cy_stc_ipc_pipe_ep_t;

/** \} goup_ipc_data_structures */

/**
* \addtogroup group_ipc_macro
* \{
*/
/* Status and error types */
#define CY_IPC_PIPE_RTN        0x0200ul  /**< Software PDL driver ID for IPC pipe functions */
#define CY_IPC_PIPE_ID_INFO    (uint32_t)( CY_IPC_ID_INFO    | CY_IPC_PIPE_RTN) /**< Return prefix for IPC pipe function status codes */
#define CY_IPC_PIPE_ID_WARNING (uint32_t)( CY_IPC_ID_WARNING | CY_IPC_PIPE_RTN) /**< Return prefix for IPC pipe function warning return values */
#define CY_IPC_PIPE_ID_ERROR   (uint32_t)( CY_IPC_ID_ERROR   | CY_IPC_PIPE_RTN) /**< Return prefix for IPC pipe function error return values */


/**< Return constants for IPC pipe functions. */
typedef enum
{
    CY_IPC_PIPE_SUCCESS            =(uint32_t)0x00u,                        /**<  Pipe API return for no error */
    CY_IPC_PIPE_ERROR_NO_IPC       =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 1ul), /**<  Pipe API return for no valid IPC channel */
    CY_IPC_PIPE_ERROR_NO_INTR      =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 2ul), /**<  Pipe API return for no valid interrupt */
    CY_IPC_PIPE_ERROR_BAD_PRIORITY =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 3ul), /**<  Pipe API return for bad priority parameter */
    CY_IPC_PIPE_ERROR_BAD_HANDLE   =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 4ul), /**<  Pipe API return for bad pipe handle */
    CY_IPC_PIPE_ERROR_BAD_ID       =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 5ul), /**<  Pipe API return for bad pipe ID */
    CY_IPC_PIPE_ERROR_DIR_ERROR    =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 6ul), /**<  Pipe API return for invalid direction (Not used at this time) */
    CY_IPC_PIPE_ERROR_SEND_BUSY    =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 7ul), /**<  Pipe API return for pipe is currently busy */
    CY_IPC_PIPE_ERROR_NO_MESSAGE   =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 8ul), /**<  Pipe API return for no message indicated   */
    CY_IPC_PIPE_ERROR_BAD_CPU      =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 9ul), /**<  Pipe API return for invalid CPU value      */
    CY_IPC_PIPE_ERROR_BAD_CLIENT   =(uint32_t)(CY_IPC_PIPE_ID_ERROR | 10ul) /**< Pipe API return for client out of range    */
} cy_en_ipc_pipe_status_t;

/** \} group_ipc_macro */



/**
* \addtogroup group_ipc_data_structures
* \{
*/



/** \cond
*   NOTE: This doxygen comment must be placed before some code entity, or else
*         it will belong to a random entity that follows it, e.g. group_ipc_functions
*
* Client identifier for a message.
* For a given pipe, traffic across the pipe can be multiplexed with multiple
* senders on one end and multiple receivers on the other end.
*
* The first 32-bit word of the message is used to identify the client that owns
* the message.
*
* The upper 16 bits are the client ID.
*
* The lower 16 bits are for use by the client in any way desired.
*
* The lower 16 bits are preserved (not modified) and not interpreted in any way.
* \endcond
*/

/** \} group_ipc_data_structures */

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

/**
* \addtogroup group_ipc_functions
* \{
*/

#ifdef __cplusplus
extern "C" {
#endif
/* The API for the pipes level functions are preliminary and may change prior to release */
void                     Cy_IPC_PIPE_Init(uint32_t epAddr, cy_ipc_pipe_callback_array_ptr_t cbArray, uint32_t cbCnt, uint32_t epConfig);
cy_en_ipc_pipe_status_t  Cy_IPC_PIPE_SendMessage(uint32_t toAddr, uint32_t fromAddr, void *msg_ptr, cy_ipc_pipe_relcallback_ptr_t callBack_ptr);
cy_en_ipc_pipe_status_t  Cy_IPC_PIPE_RegisterCallback(uint32_t epAddr, cy_ipc_pipe_callback_ptr_t callBack_ptr,  uint32_t clientID);
void                     Cy_IPC_PIPE_ExecCallback(cy_stc_ipc_pipe_ep_t * endPoint);
void                     Cy_IPC_PIPE_RegisterCallbackRel(uint32_t epAddr, cy_ipc_pipe_relcallback_ptr_t callBack_ptr);
void                     Cy_IPC_PIPE_Config(cy_stc_ipc_pipe_ep_t * theEpArray);
#ifdef __cplusplus
}
#endif

/** \} group_ipc_functions */

#endif /* _CY_IPC_PIPE_H_  */

/* [] END OF FILE */
