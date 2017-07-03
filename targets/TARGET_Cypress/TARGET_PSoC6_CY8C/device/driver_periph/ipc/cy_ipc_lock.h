/***************************************************************************//**
* \file
* \version 1.0
*
* \brief
* Header file for IPC LOCK functions
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CY_IPC_LOCK_H_
#define _CY_IPC_LOCK_H_

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "cy_ipc_drv.h"

/**
* \addtogroup group_ipc_macro
* \{
*/

#define CY_IPC_LOCK_RTN        0x0100ul   /**< Software PDL driver ID for IPC lock functions */
#define CY_IPC_LOCK_ID_INFO    (uint32_t)( CY_IPC_ID_INFO    | CY_IPC_LOCK_RTN) /**< Return prefix for IPC lock function status codes */
#define CY_IPC_LOCK_ID_WARNING (uint32_t)( CY_IPC_ID_WARNING | CY_IPC_LOCK_RTN) /**< Return prefix for IPC lock function warning return values */
#define CY_IPC_LOCK_ID_ERROR   (uint32_t)( CY_IPC_ID_ERROR   | CY_IPC_LOCK_RTN) /**< Return prefix for IPC lock function error return values */

/**< Return constants for IPC lock functions. */
typedef enum
{
    CY_IPC_LOCK_SUCCESS            = (uint32_t)0x00u,                        /**< No error has occured */
    CY_IPC_LOCK_ERROR_LOCKED       = (uint32_t)(CY_IPC_LOCK_ID_ERROR | 1ul), /**< Lock has already been locked */
    CY_IPC_LOCK_ERROR_UNLOCKED     = (uint32_t)(CY_IPC_LOCK_ID_ERROR | 2ul), /**< Lock is unlocked */
    CY_IPC_LOCK_INIT_ERROR         = (uint32_t)(CY_IPC_LOCK_ID_ERROR | 3ul), /**< Lock API return when the init function has failed */
    CY_IPC_LOCK_OUT_OF_RANGE       = (uint32_t)(CY_IPC_LOCK_ID_ERROR | 4ul), /**< Lock API return when lock is out of the range of valid locks */

    CY_IPC_LOCK_NOT_ACQUIRED       = (uint32_t)(CY_IPC_LOCK_ID_INFO  | 2ul), /**< Lock API return when lock was not acquired */
    CY_IPC_LOCK_LOCKED             = (uint32_t)(CY_IPC_LOCK_ID_INFO  | 3ul), /**< Lock API return status when lock was already locked */
    CY_IPC_LOCK_STATUS_LOCKED      = (uint32_t)(CY_IPC_LOCK_ID_INFO  | 1ul), /**< Lock status return that the channel is locked */
    CY_IPC_LOCK_STATUS_UNLOCKED    = (uint32_t)(CY_IPC_LOCK_ID_INFO  | 0ul)  /**< Lock status return that the channel is unlocked */
} cy_en_ipclock_status_t;

#define CY_IPC_LOCKS_PER_WORD                (uint32_t)32u /**< 32 locks per word */



/** \} group_ipc_macro */



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
cy_en_ipclock_status_t  Cy_IPC_LOCK_Init(uint32_t count, uint32_t mem_p[]);
cy_en_ipclock_status_t  Cy_IPC_LOCK_Acquire(uint32_t lockNumber);
cy_en_ipclock_status_t  Cy_IPC_LOCK_Release(uint32_t lockNumber);
cy_en_ipclock_status_t  Cy_IPC_LOCK_Status(uint32_t lockNumber);
uint32_t Cy_IPC_LOCK_GetMaxLocks(void);
#ifdef __cplusplus
}
#endif

/** \} group_ipc_functions */

#endif /* _CY_IPC_LOCK_H_  */

/* [] END OF FILE */
