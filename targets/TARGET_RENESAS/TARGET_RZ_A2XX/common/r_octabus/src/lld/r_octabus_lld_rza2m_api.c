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
 * File Name   : r_octabus_lld_rza2m_api.c
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "core_ca.h"
#include "r_typedefs.h"
#include "iodefine.h"
#include "iobitmask.h"
#include "rza_io_regrw.h"
#include "dev_drv.h"

#include "r_octabus_lld_rza2m_api.h"
#include "r_octabus_drv_sc_cfg.h"
/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/


/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define R_PRV_OCTARAM_MAP_OFFSET (0x60000000uL) /**< base address of OctaRAM */
#define R_PRV_OCTABUS_RESULT_OK (1)
#define R_PRV_OCTABUS_RESULT_NG (0)


/**********************************************************************************************************************
 Imported global variables and functions (from other files)
 *********************************************************************************************************************/


/**********************************************************************************************************************
 Exported global variables and functions (to be accessed by other files)
 *********************************************************************************************************************/


/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

static void octabus_io_regwrite_32(volatile uint32_t *ioreg, uint32_t write_value, uint32_t shift, uint32_t mask);
static uint32_t octabus_io_regread_32(volatile uint32_t *ioreg, uint32_t shift, uint32_t mask);
static int_t get_ok_area_start(uint8_t *p_s_ok_ng_result);
static int_t get_ok_area_count(uint8_t *p_s_ok_ng_result, int_t index_start);
static int_t octaram_manualcalib(const st_octabus_cfg_t *p_cfg);
static int_t octaram_init(const st_octabus_cfg_t *p_cfg);
static int_t octaram_autocalib(const st_octabus_cfg_t *p_cfg);

static const uint32_t s_preamble_data[4] = {
    0xFFFF0000uL,
    0x0800FF00uL,
    0xFF0000F7uL,
    0x00F708F7uL
};
static uint8_t s_ok_ng_result[16];

/**********************************************************************************************************************
 * Function Name: octabus_io_regwrite_32
 * Description  : IO register 32-bit write
 * Arguments    : volatile uint32_t * ioreg : IO register for writing
 *              :                           : Use register definition name of the
 *              :                           : iodefine.h
 *              : uint32_t write_value      : Write value for the IO register
 *              : uint32_t shift            : The number of left shifts to the
 *              :                           : target bit
 *              : uint32_t mask             : Mask value for the IO register
 *              :                           : (Target bit : "1")
 * Return Value : None
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
static void octabus_io_regwrite_32(volatile uint32_t *ioreg, uint32_t write_value, uint32_t shift, uint32_t mask)
{
    uint32_t reg_value;

    if (IOREG_NONMASK_ACCESS != mask) {
        reg_value = *ioreg;                                         /** Read from register */
        reg_value = (reg_value & (~mask)) | (unsigned)(write_value << shift); /** Modify value       */
    } else {
        reg_value = write_value;
    }
    *ioreg    = reg_value;                                      /** Write to register  */
}
/**********************************************************************************************************************
 * End of function octabus_io_regwrite_32
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: octabus_io_regread_32
 * Description  : IO register 32-bit read
 * Arguments    : volatile uint32_t * ioreg : IO register for reading
 *              :                           : Use register definition name of the
 *              :                           : iodefine.h
 *              : uint32_t shift            : The number of right shifts to the
 *              :                           : target bit
 *              : uint32_t mask             : Mask bit for the IO register
 *              :                           : (Target bit: "1")
 * Return Value : uint32_t : Value of the obtained target bit
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
static uint32_t octabus_io_regread_32(volatile uint32_t *ioreg, uint32_t shift, uint32_t mask)
{
    uint32_t reg_value;

    reg_value = *ioreg;                         /* Read from register            */
    if (IOREG_NONMASK_ACCESS != mask) {
        reg_value = (reg_value & mask) >> shift;    /* Clear other bit and Bit shift */
    }

    return reg_value;
}
/**********************************************************************************************************************
 * End of function octabus_io_regread_32
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: octaram_init
 * Description  : Initialize OctaBus controller of RZ/A2M
 * Arguments    : p_cfg       : Pointer to the configuration table
 * Return Value : DEVDRV_SUCCESS  Successful operation (always)
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
static int_t octaram_init(const st_octabus_cfg_t *p_cfg)
{
    uint32_t dummy_32;

    /*****************************************************************/

    /** Make write data to DSR1  */
    dummy_32 = (uint32_t)(0x01 << OCTA_DSR1_DV1TYP_SHIFT)
               | (p_cfg->devsize1 << OCTA_DSR1_DV1SZ_SHIFT);

    /** Write to DSR1 */
    octabus_io_regwrite_32(&OCTA.DSR1.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Read from CDSR */
    dummy_32 = octabus_io_regread_32(&OCTA.CDSR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /** Clear DV1PC & DV1TTYP */
    dummy_32 &= (~(uint32_t)(OCTA_CDSR_DV1PC | OCTA_CDSR_DV1TTYP));

    /** Make write data to CDSR  */
    dummy_32 |= (uint32_t)((0x1 << OCTA_CDSR_DLFT_SHIFT)
                           | (p_cfg->precycle1 << OCTA_CDSR_DV1PC_SHIFT)        /* Pre-cycle */
                           | (p_cfg->ttype1 << OCTA_CDSR_DV1TTYP_SHIFT));       /* Transfer mode */

    /** Write to CDSR */
    octabus_io_regwrite_32(&OCTA.CDSR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Read from MDLR */
    dummy_32 = octabus_io_regread_32(&OCTA.MDLR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /** Clear DV1WDL & DV1RDL */
    dummy_32 &= (~(uint32_t)(OCTA_MDLR_DV1WDL | OCTA_MDLR_DV1RDL));

    /** Make write data to MDLR  */
    dummy_32 |= (uint32_t)((p_cfg->w_dmy_len1 << OCTA_MDLR_DV1WDL_SHIFT)   /* Write Dummy cycle */
                           | (p_cfg->r_dmy_len1 << OCTA_MDLR_DV1RDL_SHIFT));          /* Read Dummy cycle */

    /** Write to MDLR */
    octabus_io_regwrite_32(&OCTA.MDLR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Read from MDTR */
    dummy_32 = octabus_io_regread_32(&OCTA.MDTR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /** Clear DQSERAM */
    dummy_32 &= (~(uint32_t)(OCTA_MDTR_DQSERAM));

    /** Make write data to MDTR  */
    dummy_32 |= (uint32_t)(p_cfg->dqs_ena_cnt1 << OCTA_MDTR_DQSERAM_SHIFT);     /* DQS enable counter(RAM) */

    /** Write to MDTR */
    octabus_io_regwrite_32(&OCTA.MDTR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Read from DRCSTR */
    dummy_32 = octabus_io_regread_32(&OCTA.DRCSTR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /** Clear DVRDLO1 & DVRDHI1& DVRDCMD1 */
    dummy_32 &= (~(uint32_t)(OCTA_DRCSTR_DVRDLO1 | OCTA_DRCSTR_DVRDHI1 | OCTA_DRCSTR_DVRDCMD1));

    /** Make write data to DRCSTR  */
    dummy_32 |= (uint32_t)((p_cfg->dvrdlo1 << OCTA_DRCSTR_DVRDLO1_SHIFT)  /* DVRDLO1 */
                           | (p_cfg->dvrdhi1 << OCTA_DRCSTR_DVRDHI1_SHIFT)           /* DVRDHI1 */
                           | (p_cfg->dvrdcmd1 << OCTA_DRCSTR_DVRDCMD1_SHIFT));       /* DVRDCMD1 */

    /** Write to DRCSTR */
    octabus_io_regwrite_32(&OCTA.DRCSTR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Read from DWCSTR */
    dummy_32 = octabus_io_regread_32(&OCTA.DWCSTR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /** Clear DVWLO1 & DVWHI1 & DVWCMD1 */
    dummy_32 &= (~(uint32_t)(OCTA_DWCSTR_DVWLO1 | OCTA_DWCSTR_DVWHI1 | OCTA_DWCSTR_DVWCMD1));

    /** Make write data to DWCSTR  */
    dummy_32 |= (uint32_t)((p_cfg->dvwlo1 << OCTA_DWCSTR_DVWLO1_SHIFT)  /* DVWLO1   */
                           | (p_cfg->dvwhi1 << OCTA_DWCSTR_DVWHI1_SHIFT)           /* DVWHI1   */
                           | (p_cfg->dvwcmd1 << OCTA_DWCSTR_DVWCMD1_SHIFT));       /* DVWCMD1 */

    /** Write to DWCSTR */
    octabus_io_regwrite_32(&OCTA.DWCSTR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Make write data to MRWCR1  */
    dummy_32 = (uint32_t)((p_cfg->d1mwcmd1 << OCTA_MRWCR1_D1MWCMD1_SHIFT)   /* D1MWCMD1(1st CMD for Write) */
                          | (p_cfg->d1mwcmd0 << OCTA_MRWCR1_D1MWCMD0_SHIFT)          /* D1MWCMD0(2nd CMD for Write) */
                          | (p_cfg->d1mrcmd1 << OCTA_MRWCR1_D1MRCMD1_SHIFT)          /* D1MRCMD1(1st CMD for Read) */
                          | (p_cfg->d1mrcmd0 << OCTA_MRWCR1_D1MRCMD0_SHIFT));         /* D1MRCMD0(2nd CMD for Read) */

    /** Write to MRWCR1 */
    octabus_io_regwrite_32(&OCTA.MRWCR1.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Read from MRWCSR */
    dummy_32 = octabus_io_regread_32(&OCTA.MRWCSR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /** Clear MWO1 & MWCL1 & MWAL1 & MRO1 & MRCL1 & MRAL1 */
    dummy_32 &= (~(uint32_t)(OCTA_MRWCSR_MWO1 | OCTA_MRWCSR_MWCL1 | OCTA_MRWCSR_MWAL1
                             | OCTA_MRWCSR_MRO1 | OCTA_MRWCSR_MRCL1 | OCTA_MRWCSR_MRAL1));

    /** Make write data to MRWCSR  */
    dummy_32 |= (uint32_t)((p_cfg->mwo1 << OCTA_MRWCSR_MWO1_SHIFT)  /* MWO1  */
                           | (p_cfg->mwcl1 << OCTA_MRWCSR_MWCL1_SHIFT)         /* MWCL1 : Write Command Length */
                           | (p_cfg->mwal1 << OCTA_MRWCSR_MWAL1_SHIFT)         /* MWAL1 : Write Address Length */
                           | (p_cfg->mro1 << OCTA_MRWCSR_MRO1_SHIFT)           /* MRO1  */
                           | (p_cfg->mrcl1 << OCTA_MRWCSR_MRCL1_SHIFT)         /* MRCL1 : Read Command Length  */
                           | (p_cfg->mral1 << OCTA_MRWCSR_MRAL1_SHIFT));       /* MRAL1 : Read Address Lengthe */

    /** Write to MRWCSR */
    octabus_io_regwrite_32(&OCTA.MRWCSR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /*****************************************************************/

    /** Select OctaBus controller */
    octabus_io_regwrite_32(&GPIO.PHMOM0.LONG, (uint32_t)1, GPIO_PHMOM0_HOSEL_SHIFT, GPIO_PHMOM0_HOSEL);

    return DEVDRV_SUCCESS;
}
/**********************************************************************************************************************
 * End of function octaram_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: get_ok_area_start
 * Description  : find start index of ok area
 * Arguments    : p_s_ok_ng_result : Pointer of s_ok_ng_result
 * Return Value : start index of ok area
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
static int_t get_ok_area_start(uint8_t *p_s_ok_ng_result)
{
    int_t index;             /**< loop counter */
    int_t index_start = -1;


    /* detect ok area index start*/
    for (index = 0; index < 16; index++) {
        if (R_PRV_OCTABUS_RESULT_OK == p_s_ok_ng_result[index]) {
            index_start = index;
            break;

        }
    }
    return index_start;
}
/**********************************************************************************************************************
 * End of function get_ok_area_start
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: get_ok_area_count
 * Description  : find count of ok area
 * Arguments    : p_s_ok_ng_result : Pointer of s_ok_ng_result
 *              : index_start    : start index of ok area
 * Return Value : count of ok area
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
static int_t get_ok_area_count(uint8_t *p_s_ok_ng_result, int_t index_start)
{
    int_t index;             /**< loop counter */
    int_t ok_cnt = 0;

    /* count number of ok */
    for (index = index_start; index < 16; index++) {
        if (R_PRV_OCTABUS_RESULT_OK == p_s_ok_ng_result[index]) {
            ok_cnt++;
        } else {
            break;
        }

    }
    return ok_cnt;
}
/**********************************************************************************************************************
 * End of function get_ok_area_count
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: octaram_manualcalib
 * Description  : Initialize OctaBus controller of RZ/A2M
 * Arguments    : p_cfg       : Pointer to the configuration table
 * Return Value : DEVDRV_SUCCESS  Successful operation (always)
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *              : Writing preamble of this function data must be performed in the cache invalid area.
 *********************************************************************************************************************/
static int_t octaram_manualcalib(const st_octabus_cfg_t *p_cfg)
{
    volatile uint32_t *p_preamble;
    uint32_t dummy_32;
    int_t    judge_result;
    int_t    i;
    int_t index;             /**< loop counter */
    int_t index_start = -1;
    int_t ok_cnt = 0;

    /* write preamble pattern to defined address in p_cfg->acar1 */
    p_preamble = (uint32_t *)(p_cfg->acar1 + R_PRV_OCTARAM_MAP_OFFSET);
    p_preamble[0] = s_preamble_data[0];
    p_preamble[1] = s_preamble_data[1];
    p_preamble[2] = s_preamble_data[2];
    p_preamble[3] = s_preamble_data[3];

    /* detect ok/ng area */
    for (index = 0; index < 16; index++) {
        /* Read from MDTR */
        dummy_32 = octabus_io_regread_32(&OCTA.MDTR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

        /* Make write data to MDTR  */
        dummy_32 &= (~(uint32_t)OCTA_MDTR_DV1DEL); /* Clear DV1DEL */

        /* Set DQS delay value */
        dummy_32 |= (uint32_t)((index << 4) << OCTA_MDTR_DV1DEL_SHIFT);

        /** Write to MDTR */
        octabus_io_regwrite_32(&OCTA.MDTR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

        /* Read preamble data and test */
        judge_result = 0;

        for (i = 0; i < 4; i++) {
            if (s_preamble_data[i] != p_preamble[i]) {
                judge_result = 1;
                break;
            }
        }

        /* Store the judgement result in the table */
        if (0 == judge_result) {
            s_ok_ng_result[index] = R_PRV_OCTABUS_RESULT_OK;
        } else { /* Continuous OK region */
            s_ok_ng_result[index] = R_PRV_OCTABUS_RESULT_NG;
        }
    }

    /* Search for the start index of the OK area */
    index_start = get_ok_area_start(s_ok_ng_result);
    if ((-1) == index_start) {
        return DEVDRV_ERROR; /* OK area not found */
    }

    /* Find the number of OK areas */
    ok_cnt = get_ok_area_count(s_ok_ng_result, index_start);

    if (3 < ok_cnt) {
        /** Read from MDTR */
        dummy_32 = octabus_io_regread_32(&OCTA.MDTR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

        /** Make write data to MDTR  */
        dummy_32 &= (~(uint32_t)OCTA_MDTR_DV1DEL); /* Clear DV1DEL */

        dummy_32 |= (uint32_t)(((index_start + (ok_cnt >> 1)) << 4) << OCTA_MDTR_DV1DEL_SHIFT);  /* Set Delay value */

        /** Write to MDTR */
        octabus_io_regwrite_32(&OCTA.MDTR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

        return DEVDRV_SUCCESS;
    } else {
        return DEVDRV_ERROR;
    }

}
/**********************************************************************************************************************
 * End of function octaram_manualcalib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: octaram_autocalib
 * Description  : set octabus#1 to enable auto calibration
 * Arguments    : p_cfg       : Pointer to the configuration table
 * Return Value : DEVDRV_SUCCESS  Successful operation (always)
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
static int_t octaram_autocalib(const st_octabus_cfg_t *p_cfg)
{
    volatile uint32_t *p_acar1;
    uint32_t dummy_32;

    /* Write preamble pattern match with configured transfer order */
    p_acar1 = (uint32_t *)(p_cfg->acar1 + R_PRV_OCTARAM_MAP_OFFSET);
    if (OCTABUS_BYTE_ORDER_B1B0B3B2 == p_cfg->mro1) {
        p_acar1[3] = 0xF700F708uL;
        p_acar1[2] = 0x00FFF700uL;
        p_acar1[1] = 0x000800FFuL;
        p_acar1[0] = 0xFFFF0000uL;
    } else {
        p_acar1[3] = 0x00F708F7uL;
        p_acar1[2] = 0xFF0000F7uL;
        p_acar1[1] = 0x0800FF00uL;
        p_acar1[0] = 0xFFFF0000uL;
    }

    /* write ACTR by the config */
    octabus_io_regwrite_32(&OCTA.ACTR.LONG, p_cfg->actr, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* write ACAR1 by the config */
    octabus_io_regwrite_32(&OCTA.ACAR1.LONG, p_cfg->acar1, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /** CDSR register must do read-modify-write sequence */
    dummy_32 = octabus_io_regread_32(&OCTA.CDSR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    dummy_32 |= (uint32_t)((1 << OCTA_CDSR_ACMODE_SHIFT)  /** enable automatic calibration mode */
                           | (2 << OCTA_CDSR_ACMEME_SHIFT));         /** enable automatic calibration for OctaRAM */
    octabus_io_regwrite_32(&OCTA.CDSR.LONG, dummy_32, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);


    return DEVDRV_SUCCESS;
}
/**********************************************************************************************************************
 * End of function octaram_autocalib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OCTABUS_Setup
 * Description  : Initialize OctaBus controller and connected device via user
 *                code
 * Arguments    : none
 * Return Value : none
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
void R_OCTABUS_Setup(void)
{
    int_t ret;

    /** Octabus clock start */
    volatile uint8_t dummy8;

    /* ---- STBCR9 Setting ---- */
    CPG.STBCR9.BIT.MSTP92 = 0;           // Octa Memory Controller runs
    dummy8 = CPG.STBCR9.BYTE;
    (void)dummy8;

    /** initialise controller using smart configurator settings */
    ret = R_OCTABUS_Init(&OCTABUS_SC_TABLE[0]);
    if (DEVDRV_SUCCESS != ret) {
        while (1) {
            /** spin here forever */
            __NOP();
        }
    }

    /** checking if user function is avail */
    if (NULL != OctaBus_UserConfig) {
        OctaBus_UserConfig(&OCTABUS_SC_TABLE[0]);
    }

    /** Manual calibration */
    ret = R_OCTABUS_ManualCalib(&OCTABUS_SC_TABLE[0]);
    if (DEVDRV_SUCCESS != ret) {
        while (1) {
            /** spin here forever */
            __NOP();
        }
    }

    /** Set auto calibration */
    R_OCTABUS_AutoCalib(&OCTABUS_SC_TABLE[0]);

}
/**********************************************************************************************************************
 * End of function R_OCTABUS_Setup
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OCTABUS_Init
 * Description  : Initialize OctaBus controller of RZ/A2M
 * Arguments    : p_cfg       : Pointer to the configuration table
 * Return Value : DEVDRV_SUCCESS  Successful operation (always)
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
int_t R_OCTABUS_Init(const st_octabus_cfg_t *p_cfg)
{
    int_t ret;

    ret = octaram_init(p_cfg);

    return ret;
}
/**********************************************************************************************************************
 * End of function R_OCTABUS_Init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OCTABUS_ManualCalib
 * Description  : Manual calibration OctaBus controller of RZ/A2M
 * Arguments    : p_cfg       : Pointer to the configuration table
 * Return Value : DEVDRV_SUCCESS  Successful operation
 *              : DEVDRV_ERROR    Unsuccessful operation
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
int_t R_OCTABUS_ManualCalib(const st_octabus_cfg_t *p_cfg)
{
    int_t ret;

    ret = octaram_manualcalib(p_cfg);

    return ret;
}
/**********************************************************************************************************************
 * End of function R_OCTABUS_ManualCalib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OCTABUS_AutoCalib
 * Description  : Auto calibration OctaBus controller of RZ/A2M
 * Arguments    : p_cfg       : Pointer to the configuration table
 * Return Value : DEVDRV_SUCCESS  Successful operation (always)
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
int_t R_OCTABUS_AutoCalib(const st_octabus_cfg_t *p_cfg)
{
    int_t ret;

    ret = octaram_autocalib(p_cfg);

    return ret;
}
/**********************************************************************************************************************
 * End of function R_OCTABUS_AutoCalib
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: R_OCTABUS_WriteConfigMode
 * Description  : Write data in configuration mode
 * Arguments    : p_config  : pointer to register setting data
 *              : write_value : write data
 * Return Value : none
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
void R_OCTABUS_WriteConfigMode(st_octabus_configmode_t *p_config, uint32_t write_value)
{
    /** Write Configuration Register */
    octabus_io_regwrite_32(&OCTA.DCR.LONG,   p_config->dcr_value,   IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    octabus_io_regwrite_32(&OCTA.DAR.LONG,   p_config->dar_value,   IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    octabus_io_regwrite_32(&OCTA.DCSR.LONG,  p_config->dcsr_value,  IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    octabus_io_regwrite_32(&OCTA.DCSTR.LONG, p_config->dcstr_value, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    if (0 != (p_config->dcsr_value & 0xff)) {
        /** Write to CWDR */
        octabus_io_regwrite_32(&OCTA.CWDR.LONG, write_value, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    } else {
        /** Wrige to CWNDR */
        octabus_io_regwrite_32(&OCTA.CWNDR, (uint32_t)0, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    }

}
/**********************************************************************************************************************
 * End of function R_OCTABUS_WriteConfigMode
 *********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: R_OCTABUS_ReadConfigMode
 * Description  : Read data in configuration mode
 * Arguments    : p_config  : pointer to register setting data
 * Return Value : read value
 * Precautions  : This function cannot be assigned to execute from OctaFlash or OctaRAM.
 *              : This function must be assigned to an area other than OctaFlash or OctaRAM.
 *********************************************************************************************************************/
uint32_t R_OCTABUS_ReadConfigMode(st_octabus_configmode_t *p_config)
{
    volatile uint32_t read_value;

    /** Read Configuration Register */
    octabus_io_regwrite_32(&OCTA.DCR.LONG,   p_config->dcr_value,   IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    octabus_io_regwrite_32(&OCTA.DAR.LONG,   p_config->dar_value,   IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    octabus_io_regwrite_32(&OCTA.DCSR.LONG,  p_config->dcsr_value,  IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    octabus_io_regwrite_32(&OCTA.DCSTR.LONG, p_config->dcstr_value, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    read_value = octabus_io_regread_32(&OCTA.CRR.LONG, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    return read_value;
}
/**********************************************************************************************************************
 * End of function R_OCTABUS_ReadConfigMode
 *********************************************************************************************************************/

/* End of File */
