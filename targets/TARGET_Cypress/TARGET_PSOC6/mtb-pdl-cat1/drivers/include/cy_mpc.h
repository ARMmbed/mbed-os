/***************************************************************************//**
* \file cy_mpc.h
* \version 1.0
*
* \brief
* The header file of the MPC driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_mpc
* \{
* MPC is the Memory Protection Controller which helps in configuring the memory as secure or non-secure.
*
* The functions and other declarations used in this driver are in cy_mpc.h.
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions
* and declarations in the PDL.
*
* You can use this driver to configre memory regions as secure/non-secure.
*
* \section group_mpc_more_information More Information
*
* For more information on the MPC , refer to
* the technical reference manual (TRM).
*
* \section group_mpc_MISRA MISRA-C Compliance
* The mpc driver does not have any specific deviations.
*
* \section group_mpc_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_mpc_macros Macros
* \defgroup group_mpc_functions Functions
* \defgroup group_mpc_data_structures Data Structures
* \defgroup group_mpc_enums Enumerated Types
*/
/** \} group_mpc */


#if !defined (CY_MPC_H)
#define CY_MPC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cy_syslib.h"
#include "cy_device_headers.h"
#include "cy_device.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_mpc_macros
* \{
*/
/** The MPC driver identifier */
/** MPC driver ID */
#define CY_MPC_ID                      (CY_PDL_DRV_ID(0x60U))

/** Driver major version */
#define CY_MPC_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_MPC_DRV_VERSION_MINOR       0
/** \} group_mpc_macros */

/**
* Prot Driver error codes
*/

/**
* \addtogroup group_mpc_enums
* \{
*/
/** MPC API return status */
typedef enum 
{
    CY_MPC_SUCCESS       = 0x00U,                                   /**< Returned successful */
    CY_MPC_BAD_PARAM     = CY_MPC_ID | CY_PDL_STATUS_ERROR | 0x01U, /**< Bad parameter was passed */
    CY_MPC_INVALID_STATE = CY_MPC_ID | CY_PDL_STATUS_ERROR | 0x02U, /**< The operation is not setup */
    CY_MPC_FAILURE       = CY_MPC_ID | CY_PDL_STATUS_ERROR | 0x03U, /**< The resource is locked */
    CY_MPC_UNAVAILABLE   = CY_MPC_ID | CY_PDL_STATUS_ERROR | 0x04U  /**< The resource is unavailable */
} cy_en_mpc_status_t;

/** Access permission */
typedef enum 
{
    CY_MPC_ACCESS_DISABLED = 0x00U, /**< Read and Write disabled */
    CY_MPC_ACCESS_R        = 0x01U, /**< Read enabled */
    CY_MPC_ACCESS_W        = 0x02U, /**< Write enabled */
    CY_MPC_ACCESS_RW       = 0x03U  /**< Read and Write enabled */
}cy_en_mpc_access_attr_t;

/** Security permission */
typedef enum 
{
    CY_MPC_SECURE     = 0x0U, /**< Secure */
    CY_MPC_NON_SECURE = 0x1U  /**< Non-secure */
}cy_en_mpc_sec_attr_t;

/** MPC block size */
typedef enum 
{
    CY_MPC_SIZE_32B   = 0U,  /**< 32 bytes */
    CY_MPC_SIZE_64B   = 1U,  /**< 64 bytes */
    CY_MPC_SIZE_128B  = 2U,  /**< 128 bytes */
    CY_MPC_SIZE_256B  = 3U,  /**< 256 bytes */
    CY_MPC_SIZE_512B  = 4U,  /**< 512 bytes */
    CY_MPC_SIZE_1KB   = 5U,  /**< 1 Kilobyte */
    CY_MPC_SIZE_2KB   = 6U,  /**< 2 Kilobytes */
    CY_MPC_SIZE_4KB   = 7U,  /**< 4 Kilobytes */
    CY_MPC_SIZE_8KB   = 8U,  /**< 8 Kilobytes */
    CY_MPC_SIZE_16KB  = 9U,  /**< 16 Kilobytes */
    CY_MPC_SIZE_32KB  = 10U, /**< 32 Kilobytes */
    CY_MPC_SIZE_64KB  = 11U, /**< 64 Kilobytes */
    CY_MPC_SIZE_128KB = 12U, /**< 128 Kilobytes */
    CY_MPC_SIZE_256KB = 13U, /**< 256 Kilobytes */
    CY_MPC_SIZE_512KB = 14U, /**< 512 Kilobytes */
    CY_MPC_SIZE_1MB   = 15U  /**< 1 Megabyte */
}cy_en_mpc_size_t;

/** MPC protection context */
typedef enum 
{
    CY_MPC_PC_0  = 0U, /**< PC 0 */
    CY_MPC_PC_1  = 1U, /**< PC 1 */
    CY_MPC_PC_2  = 2U  /**< PC 2 */
}cy_en_mpc_prot_context_t;

/** \} group_mpc_enums */

/**
* \addtogroup group_mpc_data_structures
* \{
*/
/** The OTP Control configuration structure. */
/** Configuration structure for ROT MPC Struct initialization */
typedef struct 
{
    uint32_t                 startAddress; /**< Sarting memory address */
    uint32_t                 size;         /**< Size of the memory */
    cy_en_mpc_size_t         regionSize;   /**< Size of the memory region */
    cy_en_mpc_prot_context_t pc;           /**< Protection Context */
    cy_en_mpc_sec_attr_t     secure;       /**< Security permissions for the region */
    cy_en_mpc_access_attr_t  access;       /**< Access permissions for the region */
} cy_stc_mpc_rot_cfg_t;

/** Configuration structure for MPC Struct initialization */
typedef struct 
{
    uint32_t                 startAddress; /**< Sarting memory address */
    uint32_t                 size;         /**< Size of the memory */
    cy_en_mpc_size_t         regionSize;   /**< Size of the memory region */
    cy_en_mpc_sec_attr_t     secure;       /**< Security permissions for the region */
} cy_stc_mpc_cfg_t;

/** \} group_mpc_data_structures */


/**
* \addtogroup group_mpc_functions
* \{
*/

cy_en_mpc_status_t Cy_Mpc_ConfigRotMpcStruct(RAMC_MPC_Type* base, const cy_stc_mpc_rot_cfg_t* rotConfig);
cy_en_mpc_status_t Cy_Mpc_ConfigMpcStruct(RAMC_MPC_Type* base, const cy_stc_mpc_cfg_t* config);
void Cy_Mpc_Lock(RAMC_MPC_Type* base);

/** \} group_mpc_functions */

#if defined(__cplusplus)
}
#endif

#endif /* #if !defined (CY_MPC_H) */

/* [] END OF FILE */