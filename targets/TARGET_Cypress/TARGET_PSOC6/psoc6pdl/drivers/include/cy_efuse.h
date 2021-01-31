/***************************************************************************//**
* \file cy_efuse.h
* \version 1.10.4
*
* Provides the API declarations of the eFuse driver.
*
********************************************************************************
* \copyright
* Copyright 2017-2020 Cypress Semiconductor Corporation
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

#if !defined(CY_EFUSE_H)
#define CY_EFUSE_H

/**
* \addtogroup group_efuse
* \{
*
* Electronic Fuses (eFuses) are non-volatile memory where each bit is one-time
* programmable (OTP).
*
* The functions and other declarations used in this driver are in cy_efuse.h.
* You can include cy_pdl.h to get access to all functions
* and declarations in the PDL.
*
* The eFuse driver enables reading the state of any bit. The eFuse driver does
* not support writing to eFuse memory. Writing an eFuse bit is typically done by
* a production programmer.
*
* One eFuse macro consists of 256 bits (32 * 8). PSoC devices have up to 16
* eFuse macros; consult the device-specific datasheet to determine how many
* macros a device has. These are implemented as a regular Advanced
* High-performance Bus (AHB) peripheral with the following characteristics:
* - eFuses are used to control the device life-cycle stage (NORMAL, SECURE,
*   and SECURE_WITH_DEBUG) and the protection settings;
* - eFuse memory can be programmed (eFuse bit value changed from '0' to '1')
*   only once; if an eFuse bit is blown, it cannot be cleared again;
* - fuses are programmed via the PSoC Programmer tool that parses the hex file
*   and extracts the necessary information; the fuse data must be located at the
*   dedicated section in the hex file. For more details see
*   [PSoC 6 Programming Specifications]
*   (http://www.cypress.com/documentation/programming-specifications/psoc-6-programming-specifications)
*
* \section group_efuse_configuration Configuration Considerations
*
* The eFuse driver provides the simplest way to read eFuse memory.
* No configuration is needed.
*
* \section group_efuse_more_information More Information
*
* Refer to the technical reference manual (TRM) and the device datasheet.
*
* \section group_efuse_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.10.4</td>
*     <td>Minor documentation updates.</td>
*     <td>Removed MISRA 2004 compliance details and verified MISRA 2012 complaince.</td>
*   </tr>
*   <tr>
*     <td>1.10.3</td>
*     <td>Minor documentation updates.</td>
*     <td>Documentation enhancement.</td>
*   </tr>
*   <tr>
*     <td>1.10.2</td>
*     <td>Fix driver header path.</td>
*     <td>Folder structure changed.</td>
*   </tr>
*   <tr>
*     <td>1.10.1</td>
*     <td>Added header guard CY_IP_MXEFUSE.</td>
*     <td>To enable the PDL compilation with wounded out IP blocks.</td>
*   </tr>
*   <tr>
*     <td>1.10</td>
*     <td>Flattened the organization of the driver source code into the single
*         source directory and the single include directory.
*     </td>
*     <td>Driver library directory-structure simplification.</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_efuse_macros Macros
* \defgroup group_efuse_functions Functions
* \defgroup group_efuse_data_structures Data Structures
* \defgroup group_efuse_enumerated_types Enumerated Types
*/

#include "cy_device_headers.h"
#include "cy_syslib.h"

#ifdef CY_IP_MXEFUSE

/***************************************
* Macro Definitions
***************************************/
/**
* \addtogroup group_efuse_macros
* \{
*/

/** The driver major version */
#define CY_EFUSE_DRV_VERSION_MAJOR          1
/** The driver minor version */
#define CY_EFUSE_DRV_VERSION_MINOR          10
/** The eFuse driver identifier */
#define CY_EFUSE_ID                         (CY_PDL_DRV_ID(0x1AUL))
/** The number of bits in the byte */
#define CY_EFUSE_BITS_PER_BYTE              (8UL)
/** \} group_efuse_macros */

/***************************************
* Enumerated Types
***************************************/
/**
* \addtogroup group_efuse_enumerated_types
* \{
*/
/** This enum has the return values of the eFuse driver */
typedef enum
{
    CY_EFUSE_SUCCESS               = 0x00UL,  /**< Success */
    CY_EFUSE_INVALID_PROTECTION    = CY_EFUSE_ID | CY_PDL_STATUS_ERROR | 0x01UL, /**< Invalid access in the current protection state */
    CY_EFUSE_INVALID_FUSE_ADDR     = CY_EFUSE_ID | CY_PDL_STATUS_ERROR | 0x02UL, /**< Invalid eFuse address */
    CY_EFUSE_BAD_PARAM             = CY_EFUSE_ID | CY_PDL_STATUS_ERROR | 0x03UL, /**< One or more invalid parameters */
    CY_EFUSE_IPC_BUSY              = CY_EFUSE_ID | CY_PDL_STATUS_ERROR | 0x04UL, /**< The IPC structure is already locked by another process */
    CY_EFUSE_ERR_UNC               = CY_EFUSE_ID | CY_PDL_STATUS_ERROR | 0xFFUL  /**< Unknown error code. See Cy_EFUSE_GetExternalStatus() */
} cy_en_efuse_status_t;

/** \} group_efuse_data_structure */

#if defined(__cplusplus)
extern "C" {
#endif
/***************************************
* Function Prototypes
***************************************/

/**
* \addtogroup group_efuse_functions
* \{
*/
cy_en_efuse_status_t Cy_EFUSE_GetEfuseBit(uint32_t bitNum, bool *bitVal);
cy_en_efuse_status_t Cy_EFUSE_GetEfuseByte(uint32_t offset, uint8_t *byteVal);
uint32_t Cy_EFUSE_GetExternalStatus(void);
/** \} group_efuse_functions */

#if defined(__cplusplus)
}
#endif

#endif /* #ifdef CY_IP_MXEFUSE */

#endif /* #if !defined(CY_EFUSE_H) */

/** \} group_efuse */


/* [] END OF FILE */
