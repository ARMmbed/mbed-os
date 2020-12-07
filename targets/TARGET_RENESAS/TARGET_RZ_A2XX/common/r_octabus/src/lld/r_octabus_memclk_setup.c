/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/* Copyright (c) 2020 Renesas Electronics Corporation.
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
 */
/**********************************************************************************************************************
 * File Name   : r_octabus_memclk_setup.c
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "iobitmask.h"
#include "rza_io_regrw.h"

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/


/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Imported global variables and functions (from other files)
 *********************************************************************************************************************/


/**********************************************************************************************************************
 Exported global variables and functions (to be accessed by other files)
 *********************************************************************************************************************/


/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/
static void octabus_io_regwrite_16(volatile uint16_t *ioreg, uint16_t write_value, uint16_t shift, uint32_t mask);

/**********************************************************************************************************************
 * Function Name: octabus_io_regwrite_16
 * Description  : IO register 16-bit write
 * Arguments    : volatile uint16_t * ioreg : IO register for writing
 *              :                           : Use register definition name of the
 *              :                           : iodefine.h
 *              : uint16_t write_value      : Write value for the IO register
 *              : uint16_t shift            : The number of left shifts to the
 *              :                          : target bit
 *              : uint32_t mask             : Mask value for the IO register
 *              :                          : (Target bit : "1")
 * Return Value : None
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
static void octabus_io_regwrite_16(volatile uint16_t *ioreg, uint16_t write_value, uint16_t shift, uint32_t mask)
{
    uint16_t reg_value;

    if (IOREG_NONMASK_ACCESS != mask) {
        reg_value = *ioreg;                                         /* Read from register */
        reg_value = (uint16_t)((reg_value & (~mask)) | (unsigned)(write_value << shift)); /* Modify value       */
    } else {
        reg_value = write_value;
    }
    *ioreg    = reg_value;                                      /* Write to register  */
}
/**********************************************************************************************************************
 * End of function octabus_io_regwrite_16
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_octabus_memclk_setup
 * Description  : Initialize OctaBus controller of RZ/A2M
 * Arguments    : p_cfg       : Pointer to the configuration table
 * Return Value : DRV_SUCCESS  Successful operation (always)
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
void r_octabus_memclk_setup(void)
{
    /* Set OCTCR to G-phy */
    octabus_io_regwrite_16(&CPG.SCLKSEL.WORD, (uint16_t)3, CPG_SCLKSEL_OCTCR_SHIFT, CPG_SCLKSEL_OCTCR);

}
/**********************************************************************************************************************
 * End of function r_octabus_memclk_setup
 *********************************************************************************************************************/

/* End of File */
