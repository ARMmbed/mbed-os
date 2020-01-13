/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "s5js100_pmip.h"
#include "s5js100_pmusfr.h"
#include "s5js100_type.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
static int pmip_fused;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static void s5js100_pmip_trim_value(void)
{
    unsigned int pmip_option[4];
    unsigned int value, temp1, temp2, temp3, temp4, temp5, temp6;

    // read pmip option
    pmip_option[0] = getreg32(0x82010504);
    pmip_option[1] = getreg32(0x82010508);
    pmip_option[2] = getreg32(0x8201050C);
    pmip_option[3] = getreg32(0x82010510);

    if (pmip_option[0] != 0) {
        pmip_fused = 1;

        /*
         * Buck1
         */
        // V18TRIMB : PMIP_OPTION[11:10]
        temp1 = (pmip_option[0] >> 10) & 0x3;

        // VOTRIM : PMIP_OPTION[9:7]
        temp2 = (pmip_option[0] >> 7) & 0x7;

        // VSLTRIMB : PMIP_OPTION[13:12]
        temp3 = (pmip_option[0] >> 12) & 0x3;

        value = (2 << 25) + (0 << 23) + (3 << 21) + (3 << 19) + (1 << 17) + (temp1 << 13) + (temp2 << 10) + (8 << 6) + (temp3 << 4) + (4 << 1);
        putreg32(value, 0x81000500);

        value = (1 << 3) + (1 << 0);
        putreg32(value, 0x81000504);

        /*
         * Buck0
         */
        // V18TRIMB : PMIP_OPTION[4:3]
        temp1 = (pmip_option[0] >> 3) & 0x3;

        // VOTRIM : PMIP_OPTION[2:0]
        temp2 = (pmip_option[0] >> 0) & 0x7;

        // VSLTRIMB : PMIP_OPTION[6:5]
        temp3 = (pmip_option[0] >> 6) & 0x3;

        value = (1 << 25) + (0 << 23) + (3 << 21) + (3 << 19) + (1 << 17) + (temp1 << 13) + (temp2 << 10) + (8 << 6) + (temp3 << 4) + (4 << 1);
        putreg32(value, 0x82020600);

        value = (1 << 3) + (1 << 0);
        putreg32(value, 0x82020604);

        /*
         * BuckBoost
         */
        // BTVOTM : PMIP_OPTION[28:26]
        temp1 = (pmip_option[0] >> 26) & 0x7;

        // BKVOTM : PMIP_OPTION[22:20]
        temp2 = (pmip_option[0] >> 20) & 0x7;

        // BBVOTM : PMIP_OPTION[25:23]
        temp3 = (pmip_option[0] >> 23) & 0x7;

        // TSLTRIM : PMIP_OPTION[19:18]
        temp4 = (pmip_option[0] >> 18) & 0x3;

        // KSLTRIM : PMIP_OPTION[17:16]
        temp5 = (pmip_option[0] >> 16) & 0x3;

        // V18TRIM : PMIP_OPTION[15:14]
        temp6 = (pmip_option[0] >> 14) & 0x3;

        value = (12 << 27) + (3 << 24) + (temp1 << 21) + (temp2 << 18) + (temp3 << 15) + (temp4 << 13) + (temp5 << 11) + (temp6 << 9) + (3 << 7) + (1 << 5) + (3 << 3) + (3 << 1);
        putreg32(value, 0x85040400);

        value = (1 << 18) + (1 << 17) + (0x1c << 5) + (0x1c << 0);
        putreg32(value, 0x85040404);

        /*
         * LDO0
         */
        // VOTRIM : PMIP_OPTION[31:29]
        temp1 = (pmip_option[0] >> 29) & 0x7;
        value = (2 << 7) + (1 << 5) + (temp1 << 2) + (1 << 1);
        putreg32(value, 0x81000508);

        /*
         * LDO1
         */
        // VOTRIM : PMIP_OPTION[34:32]
        temp1 = (pmip_option[1] >> 0) & 0x7;
        value = (2 << 7) + (1 << 6) + (1 << 4) + (temp1 << 1);
        putreg32(value, 0x82020608);

        /*
         * LDO2
         */
        // VOTRIM : PMIP_OPTION[37:35]
        temp1 = (pmip_option[1] >> 3) & 0x7;
        value = (1 << 10) + (2 << 7) + (1 << 6) + (1 << 4) + (temp1 << 1);
        putreg32(value, 0x8202060c);

        /*
         * LDO3
         */
        // VOTRIM : PMIP_OPTION[40:38]
        temp1 = (pmip_option[1] >> 6) & 0x7;
        value = (2 << 7) + (2 << 4) + (temp1 << 1);
        putreg32(value, 0x82020618);

        /*
         * LDO4
         */
        // VOTRIM : PMIP_OPTION[43:41]
        temp1 = (pmip_option[1] >> 9) & 0x7;
        value = (2 << 7) + (2 << 4) + (temp1 << 1);
        putreg32(value, 0x82020610);

        /*
         * LDO5
         */
        // VOTRIM : PMIP_OPTION[46:44]
        temp1 = (pmip_option[1] >> 12) & 0x7;
        value = (2 << 7) + (1 << 4) + (temp1 << 1);
        putreg32(value, 0x82020610);

        /*
         * MPBGR
         */
        putreg32(0x7, 0x81000510);

        /*
         * DCXO LDO
         */
        // VOTRIM_ANALOG : PMIP_OPTION[56:53]
        // VOTRIM_DIGITAL : PMIP_OPTION[60:57]
        temp1 = (pmip_option[1] >> 21) & 0x7;
        temp2 = (pmip_option[1] >> 25) & 0x7;

        if (temp1 | temp2) {
            value = (1 << 14) + (temp1 << 8) + (temp2 << 2) + (1 << 1) + (1 << 0);
        } else {
            value = (1 << 14) + (0 << 8) + (4 << 2) + (1 << 1) + (1 << 0);
        }

        putreg32(value, 0x8100050c);
    }
}

/****************************************************************************
 * Name: s5js100_pmip_initialize
 *
 * Description:
 *   Initialize the PMIP.
 *
 * Returned Value:
 *   0 : success
 *   <0 : fail
 *
 ****************************************************************************/
int s5js100_pmip_initialize(void)
{
    // Apply pmip trim value
    s5js100_pmip_trim_value();

    // set LDO6 to 1.8V
    s5js100_pmip_control_Ldo6_Tcxo(1);

    if (!pmip_fused) {
        // set LDO2 to 1.2V
        modifyreg32(PMU_SYS_PMIP_ANARF12_LDO, (0x1 << 10), (0x1 << 10));

        // set MP_CTR to 0x7
        modifyreg32(PMU_ALIVE_PMIP_MPBGR_LDO, 0x3f, 0x7);

        // LDO6
        if (is_evt0()) {
            putreg32((1 << 8) + (1 << 5) + (1 << 4) + (1 << 1) + (1 << 0), PMU_ALIVE_PMIP_TCXO_LDO);
        }

        // DCXO LDO
        putreg32((1 << 14) + (0 << 8) + (4 << 2) + (1 << 1) + (1 << 0), PMU_ALIVE_PMIP_DCXO_LDO);
    }

    return 0;
}

/****************************************************************************
 * Name: s5js100_pmip_enable_BuckBooster
 *
 * Description:
 *   control BuckBooster. 3.3V output or off
 *
 * Returned Value:
 *   a NULL on failure
 *
 ****************************************************************************/
int s5js100_pmip_control_BuckBooster(int enable)
{
    if (pmip_fused) {
        if (enable) {
            modifyreg32(SYS_CFG_PM_BST_CFG0, 1, 1);
        } else {
            modifyreg32(SYS_CFG_PM_BST_CFG0, 1, 0);
        }
    } else {
        if (enable) {
            putreg32(0xE36DACB5, SYS_CFG_PM_BST_CFG0);
            putreg32(0x000C738C, SYS_CFG_PM_BST_CFG1);
        } else {
            putreg32(0, SYS_CFG_PM_BST_CFG0);
            putreg32(0, SYS_CFG_PM_BST_CFG1);
        }
    }

    return 1;
}

/****************************************************************************
 * Name: s5js100_pmip_control_Ldo5_Efuse
 *
 * Description:
 *   control LDO5. 1.8V output or off
 *
 * Returned Value:
 *   a NULL on failure
 *
 ****************************************************************************/
int s5js100_pmip_control_Ldo5_Efuse(int enable)
{
    if (enable) {
        modifyreg32(PMU_SYS_PMIP_EFUSE18_LDO, 0x1, 0x1);
    } else {
        modifyreg32(PMU_SYS_PMIP_EFUSE18_LDO, 0x1, 0x0);
    }

    return 1;
}

/****************************************************************************
 * Name: s5js100_pmip_control_Ldo6_Tcxo
 *
 * Description:
 *   control TCXO LDO output or off
 *
 * Returned Value:
 *   a NULL on failure
 *
 ****************************************************************************/
int s5js100_pmip_control_Ldo6_Tcxo(int enable)
{
    // VOTRIM should be set as 001b in EVT0 (1.8V)
    if (is_evt0()) {
        modifyreg32(PMU_ALIVE_PMIP_TCXO_LDO, (0x7 << 1), (0x1 << 1));
    }

    if (enable) {
        modifyreg32(PMU_ALIVE_PMIP_TCXO_LDO, 0x80000001, 0x80000001);
    } else {
        modifyreg32(PMU_ALIVE_PMIP_TCXO_LDO, 0x80000001, 0x80000000);
    }

    return 1;
}

int s5js100_pmip_control_powersave(int enable)
{
    if (enable) {
        /* Buck1 and LDO0 set to minimum voltage */
        modifyreg32(PMU_ALIVE_PMIP_BUCK1_CFG0, 0x7 << 10, 0);
        modifyreg32(PMU_ALIVE_PMIP_DBB10_LDO, 0x7 << 2, 0);

        /* Buck0 and LDO3 set to minimum voltage */
        modifyreg32(PMU_SYS_PMIP_BUCK0_CFG0, 0x7 << 10, 0);
        modifyreg32(PMU_SYS_PMIP_IO18_LDO, 0x7 << 1, 0);
    } else {
        s5js100_pmip_trim_value();
    }

    return 1;
}

