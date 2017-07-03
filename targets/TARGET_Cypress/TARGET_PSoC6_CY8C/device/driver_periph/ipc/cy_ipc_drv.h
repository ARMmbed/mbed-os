/***************************************************************************//**
* \file cy_ipc_drv.h
* \version 1.0
*
* Provides an API declaration of the IPC driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_IPC_DRV_H)
#define CY_IPC_DRV_H


/**
* \defgroup group_ipc Inter Process Communication (IPC)
* \{
* Contains the driver API for low level IPC interface as well as Lock and Pipe
* upper level interfaces.
*
* The IPC driver provides a safe and reliable method to transfer data between
* CPUs or processes in a single device.  Hardware locking provides a reliable
* method to ensure that only one device can acquire and transfer data at one
* time so no data is lost or over written by asynchronous processes or CPUs.
*
* The Lock functions made use of a single IPC channel to allow multiple locks
* that can be used by system or user function calls.  By default there are
* 128 locks provided, although the user may modify the default value to any
* number, limited only by SRAM.
*
* The Pipe functions provide a method to transfer one or more words of data
* between CPUs or tasks.  The data can be defined as a single 32-bit unsigned
* word, an array of data, or a user defined structure.  The only limitation is
* that the first word in the array or structure must be a 32-bit unsigned word
* in which a client ID number is passed.  The client ID dictates the callback
* function that will be called by the receiver of the message.  After the
* callback function returns by the receiver, it will invoke a release callback
* function defined by the sender of the message.
*
* A User Pipe is provided for the user to transfer data between CPUs and
* tasks.
*
* \section group_ipc_configuration Configuration Considerations
*
* All IPC channels have been defined by the system.  Users should not
* call any of the low level IPC functions.  Only the Lock and Pipe functions
* are accessible by the user.  The CyPipe is reserved for system level
* functions such as BLE and CapSense at this time.
*
* \section group_ipc_more_information More Information
*
* See TRM (Technical Reference Manual) for more information on IPC.
*
* \section group_ipc_MISRA MISRA-C Compliance
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th style="width: 50%;">Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>8.8</td>
*     <td>R</td>
*     <td>An external object or function shall be declared in one and only one
*         file.</td>
*     <td>This rule is violated in the following files:
*         cy_ipc_pipe.c, cy_ipc_lock.c, cy_ipc_config.c</td>
*   </tr>
*   <tr>
*     <td>10.1</td>
*     <td>R</td>
*     <td>The value of an expression of integer type shall not be
*         implicitly converted to a different underlying type.</td>
*     <td>Implicit conversion from signed to unsigned is present in the
*         following files: cy_ipc_pipe.c, cy_ipc_config.c</td>
*   </tr>
*   <tr>
*     <td>11.4</td>
*     <td>A</td>
*     <td>A cast should not be performed between a pointer to object type
*         and a different pointer to object type.</td>
*     <td>In file cy_ipc_pipe.c, the message pointer is set to a void pointer
*         because the message structure is defined by the user and is unknown.
*     </td>
*   </tr>
*   <tr>
*     <td>14.10</td>
*     <td>R</td>
*     <td>All if ... else if constructs shall be terminated with an else clause.
*     </td>
*     <td>In file cy_ipc_pipe.c this rule is violated intentionally and safely.
*     </td>
*   </tr>
*   <tr>
*     <td>16.7</td>
*     <td>A</td>
*     <td>A pointer parameter in a function prototype should be declared as
*         pointer to const if the pointer is not used to modify the addressed
*         object.</td>
*     <td>In file cy_ipc_drv.c this rule is violated because the API
*         implementation is expected in future to modify the parameter.</td>
*   </tr>
*   <tr>
*     <td>16.9</td>
*     <td>R</td>
*     <td>A function identifier shall only be used with either a preceding &,
*         or with a parenthesised parameter list, which may be empty.</td>
*     <td>In file cy_ipc_config.c this rule is violated intentionally.</td>
*   </tr>
*   <tr>
*     <td>16.10</td>
*     <td>R</td>
*     <td>If a function returns error information, then that error information
*         shall be tested.</td>
*     <td>In file cy_ipc_pipe.c this rule is violated intentionally.</td>
*   </tr>
* </table>
*
* \section group_ipc_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_ipc_macro Macro
* \defgroup group_ipc_functions Functions
* \defgroup group_ipc_data_structures Data Structures
* \defgroup group_ipc_enums Enumerated Types
*
*/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "syslib/cy_syslib.h"
#include "cy_device_headers.h"


/**
* \addtogroup group_ipc_data_structures
* \{
*/

/** \} group_ipc_data_structures */

/**
* \addtogroup group_ipc_macro
* \{
*/

/** Driver major version */
#define CY_IPC_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_IPC_DRV_VERSION_MINOR       0

/* defined in the device.h */


/* Constants for ACQUIRE register  */
#define CY_IPC_ACQ_NOT_ACQUIRED     (uint32_t)(0x00000000ul)  /**< Used to determine if lock was not acquired */
#define CY_IPC_NO_NOTIFIFICATION    (uint32_t)(0x00000000ul)  /**< Return value when no notifications have occurred or will be forced */

/* Error Code constants */


#define CY_IPC_ID CY_PDL_DRV_ID(0x22u)  /**< Software PDL driver ID for IPC */
#define CY_IPC_ID_INFO    (uint32_t)( CY_IPC_ID | CY_PDL_STATUS_INFO )   /**< Return prefix for IPC driver function status codes */
#define CY_IPC_ID_WARNING (uint32_t)( CY_IPC_ID | CY_PDL_STATUS_WARNING) /**< Return prefix for IPC driver function warning return values */
#define CY_IPC_ID_ERROR   (uint32_t)( CY_IPC_ID | CY_PDL_STATUS_ERROR)   /**< Return prefix for IPC driver function error return values */


/** \} group_ipc_macro */

/* end of definition in device.h */


/**
* \addtogroup group_ipc_enums
* \{
*/

/**
* This is a list of ENUMs used for function return status.
*/
typedef enum
{
    CY_IPC_DRV_SUCCESS      = 0x00u,                     /**< Function was successfully executed */
    CY_IPC_DRV_ERROR        = ( CY_IPC_ID_ERROR + 1ul),  /**< Function was not executed due to an error. Typical conditions for the error explained in the function description */
    CY_IPC_DRV_NOT_ACQUIRED = ( CY_IPC_ID_ERROR + 2ul),  /**< IPC was not previously acquired */
    CY_IPC_DRV_RELEASED     = ( CY_IPC_ID_INFO + 0ul),   /**< IPC was in a released state */
    CY_IPC_DRV_LOCKED       = ( CY_IPC_ID_INFO + 1ul)    /**< IPC was in a locked/acquired state */
} cy_en_ipcdrv_status_t;

/** \} group_ipc_enums */

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
cy_en_ipcdrv_status_t  Cy_IPC_DRV_LockAcquire (IPC_STRUCT_Type const * ipcPtr);
cy_en_ipcdrv_status_t  Cy_IPC_DRV_GetLockStatus (IPC_STRUCT_Type const * ipcPtr);
cy_en_ipcdrv_status_t  Cy_IPC_DRV_Release (IPC_STRUCT_Type* ipcPtr, uint32_t releaseEventIntr);
void                   Cy_IPC_DRV_Notify (IPC_STRUCT_Type* ipcPtr, uint32_t notifyEventIntr);
cy_en_ipcdrv_status_t  Cy_IPC_DRV_SendMsgWord (IPC_STRUCT_Type* ipcPtr, uint32_t notifyEventIntr, uint32_t message);
cy_en_ipcdrv_status_t  Cy_IPC_DRV_ReadMsgWord (IPC_STRUCT_Type* ipcPtr, uint32_t * message);
cy_en_ipcdrv_status_t  Cy_IPC_DRV_SendMsgPtr (IPC_STRUCT_Type* ipcPtr, uint32_t notifyEventIntr, void const * msgPtr);
cy_en_ipcdrv_status_t  Cy_IPC_DRV_ReadMsgPtr (IPC_STRUCT_Type* ipcPtr, void** msgPtr);

void                   Cy_IPC_DRV_SetIntrMask (IPC_INTR_STRUCT_Type* ipcIntrPtr, uint32_t ipcReleaseMask, uint32_t ipcNotifyMask);
uint32_t               Cy_IPC_DRV_GetIntrMask(IPC_INTR_STRUCT_Type const * ipcIntrPtr);
uint32_t               Cy_IPC_DRV_GetIntrStatusMasked (IPC_INTR_STRUCT_Type const * ipcIntrPtr);
uint32_t               Cy_IPC_DRV_GetIntrStatus(IPC_INTR_STRUCT_Type const * ipcIntrPtr);
void                   Cy_IPC_DRV_SetIntr(IPC_INTR_STRUCT_Type* ipcIntrPtr, uint32_t ipcReleaseMask, uint32_t ipcNotifyMask);
void                   Cy_IPC_DRV_ClearIntr(IPC_INTR_STRUCT_Type* ipcIntrPtr, uint32_t ipcReleaseMask, uint32_t ipcNotifyMask);

__STATIC_INLINE IPC_STRUCT_Type*       Cy_IPC_DRV_GetIpcBaseAddress (uint32_t ipcIndex);
__STATIC_INLINE IPC_INTR_STRUCT_Type*  Cy_IPC_DRV_GetIntrBaseAddr (uint32_t ipcIntrIndex);

/*******************************************************************************
* Function Name: Cy_IPC_DRV_GetIpcBaseAddress
****************************************************************************//**
*
* This function takes an IPC index as a parameter and returns the base address the IPC registers corresponding to the IPC channel.
* The function returns a zero value if the ipcIndex passed was invalid.
*
* \param ipcIndex
* Represents the hardware IPC index. This is converted to base address of the IPC channel registers.
*
* \return
* Returns a pointer to the base of the IPC registers. If the ipcIndex passed was invalid the function returns zero
*
*******************************************************************************/
__STATIC_INLINE IPC_STRUCT_Type* Cy_IPC_DRV_GetIpcBaseAddress (uint32_t ipcIndex)
{
    CY_ASSERT((uint32_t)CPUSS_IPC_IPC_NR > ipcIndex);
    return ( (IPC_STRUCT_Type*) ( &IPC->STRUCT[ipcIndex] ) );
}

/*******************************************************************************
* Function Name: Cy_IPC_DRV_GetIntrBaseAddr
****************************************************************************//**
*
* This function takes an IPC index and returns the base address of the IPC interrupt registers corresponding to the IPC Interrupt.
* The function returns a zero value if the ipcIntrIndex passed was invalid.
*
* \param ipcIntrIndex
* Represents the hardware IPC interrupt number. This is converted to the base address of the IPC interrupt registers.
*
* \return
* Returns a pointer to the base of the IPC interrupt registers. If the IpcIntIndex passed was invalid, the function returns zero
*
*******************************************************************************/
__STATIC_INLINE IPC_INTR_STRUCT_Type* Cy_IPC_DRV_GetIntrBaseAddr (uint32_t ipcIntrIndex)
{
    CY_ASSERT((uint32_t)CPUSS_IPC_IPC_IRQ_NR > ipcIntrIndex);
    return ( (IPC_INTR_STRUCT_Type*) ( &IPC->INTR_STRUCT[ipcIntrIndex] ) );
}

/** \} group_ipc_functions */

#ifdef __cplusplus
}
#endif

/** \} group_ipc */

#endif /* !defined(CY_IPC_DRV_H) */

/* [] END OF FILE */
