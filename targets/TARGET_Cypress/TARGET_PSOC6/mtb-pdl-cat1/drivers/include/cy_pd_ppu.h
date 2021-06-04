/***************************************************************************//**
* \file cy_pd_ppu.h
* \version 1.0
*
* This file provides the header for ARM PPU Platform PD specific driver
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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
* \addtogroup group_pd_ppu
* \{
* PD PPU driver is a platform dependant driver on top of ARM PPU Driver
*
* The functions and other declarations used in this driver are in cy_pd_ppu.h. 
* 
* You can use this driver to implement the platform dependant code to 
* control power domainusing PPU
*
* \section group_pd_ppu_more_information More Information
*
* For more information on the PD PPU , refer to 
* the technical reference manual (TRM).
*
* \section group_pd_ppu_MISRA MISRA-C Compliance
* The PD PPU driver does not have any specific deviations.
*
* \section group_pd_ppu_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_pd_ppu_macros Macros
* \defgroup group_pd_ppu_functions Functions
* \defgroup group_pd_ppu_data_structures Data Structures
* \defgroup group_pd_ppu_enums Enumerated Types
*/
/** \} group_pd_ppu */

#ifndef CY_PD_PPU_H
#define CY_PD_PPU_H

#include <stdint.h>
#include "ppu_v1.h"
#include "cy_pdl.h"


/*******************************************************************************
*       Enumerated Types
*******************************************************************************/

/**
* \addtogroup group_pd_ppu_enums
* \{
*/
/** Defines general-purpose function return values */
typedef enum
{
    CY_PD_PPU_SUCCESS       = 0x00UL, /**< Command completed with no errors */
    CY_PD_PPU_BAD_PARAM     = 0x01UL, /**< Invalid function input parameter */
    CY_PD_PPU_TIMEOUT       = 0x02UL, /**< Timeout occurred */
    CY_PD_PPU_INVALID_STATE = 0x03UL,  /**< Clock is in an invalid state */
    CY_PD_PPU_ERROR         = 0x04UL  /**< Error */
} cy_pd_ppu_status_t;
/** \} group_pd_ppu_enums */

/**
* \addtogroup group_pd_ppu_functions
* \{
*/
cy_pd_ppu_status_t cy_pd_ppu_init(struct ppu_v1_reg *ppu);
cy_pd_ppu_status_t cy_pd_ppu_get_power_mode(struct ppu_v1_reg *ppu, uint32_t *mode);
cy_pd_ppu_status_t cy_pd_ppu_set_power_mode(struct ppu_v1_reg *ppu, uint32_t mode);
cy_pd_ppu_status_t cy_pd_ppu_reset(struct ppu_v1_reg *ppu);
/** \} group_pd_ppu_functions */

#endif /* CY_PD_PPU_H */
