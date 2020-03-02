/*******************************************************************************
* File Name: cycfg_system.c
*
* Description:
* System configuration
* This file was automatically generated and should not be modified.
* Device Configurator: 2.0.0.1483
* Device Support Library (../../../psoc6pdl): 1.3.1.1499
*
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
********************************************************************************/

#include "cycfg_system.h"

#define CY_CFG_PWR_ENABLED 1
#define CY_CFG_PWR_INIT 1
#define CY_CFG_PWR_USING_PMIC 0
#define CY_CFG_PWR_VBACKUP_USING_VDDD 0
#define CY_CFG_PWR_LDO_VOLTAGE CY_SYSPM_LDO_VOLTAGE_LP
#define CY_CFG_PWR_USING_ULP 0

__STATIC_INLINE void init_cycfg_power(void)
{
     /* Reset the Backup domain on POR, XRES, BOD only if Backup domain is supplied by VDDD */
     #if (CY_CFG_PWR_VBACKUP_USING_VDDD)
     if (0u == Cy_SysLib_GetResetReason() /* POR, XRES, or BOD */)
     {
         Cy_SysLib_ResetBackupDomain();
         Cy_SysClk_IloDisable();
         Cy_SysClk_IloInit();
     }
     #else /* Dedicated Supply */
     Cy_SysPm_BackupSetSupply(CY_SYSPM_VDDBACKUP_VBACKUP);
     #endif /* CY_CFG_PWR_VBACKUP_USING_VDDD */

     /* Configure core regulator */
     #if CY_CFG_PWR_USING_LDO
     Cy_SysPm_LdoSetVoltage(CY_SYSPM_LDO_VOLTAGE_LP);
     Cy_SysPm_LdoSetMode(CY_SYSPM_LDO_MODE_NORMAL);
     #else
     Cy_SysPm_BuckEnable(CY_SYSPM_BUCK_OUT1_VOLTAGE_LP);
     #endif /* CY_CFG_PWR_USING_LDO */
     /* Configure PMIC */
     Cy_SysPm_UnlockPmic();
     #if CY_CFG_PWR_USING_PMIC
     Cy_SysPm_PmicEnableOutput();
     #else
     Cy_SysPm_PmicDisableOutput();
     #endif /* CY_CFG_PWR_USING_PMIC */
}

