/***************************************************************************//**
* \file cy_sysanalog.c
* \version 1.10
*
* Provides the public functions for the API for the SAR driver.
*
********************************************************************************
* \copyright
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
*******************************************************************************/
#include "cy_sysanalog.h"

#ifdef CY_IP_MXS40PASS

#if defined(__cplusplus)
extern "C" {
#endif

/* Configure the AREF to use the local Vref and local IZTAT. Can be used with \ref Cy_SysAnalog_Init. */
const cy_stc_sysanalog_config_t Cy_SysAnalog_Fast_Local =
{
    /*.startup    */ CY_SYSANALOG_STARTUP_FAST,
    /*.iztat      */ CY_SYSANALOG_IZTAT_SOURCE_LOCAL,
    /*.vref       */ CY_SYSANALOG_VREF_SOURCE_LOCAL_1_2V,
    /*.deepSleep  */ CY_SYSANALOG_DEEPSLEEP_DISABLE,
};

/* Configure the AREF to use the SRSS Vref and SRSS IZTAT. Can be used with \ref Cy_SysAnalog_Init. */
const cy_stc_sysanalog_config_t Cy_SysAnalog_Fast_SRSS =
{
    /*.startup    */ CY_SYSANALOG_STARTUP_FAST,
    /*.iztat      */ CY_SYSANALOG_IZTAT_SOURCE_SRSS,
    /*.vref       */ CY_SYSANALOG_VREF_SOURCE_SRSS,
    /*.deepSleep  */ CY_SYSANALOG_DEEPSLEEP_DISABLE,
};

/* Configure the AREF to use the external Vref and local IZTAT. Can be used with \ref Cy_SysAnalog_Init. */
const cy_stc_sysanalog_config_t Cy_SysAnalog_Fast_External =
{
    /*.startup    */ CY_SYSANALOG_STARTUP_FAST,
    /*.iztat      */ CY_SYSANALOG_IZTAT_SOURCE_LOCAL,
    /*.vref       */ CY_SYSANALOG_VREF_SOURCE_EXTERNAL,
    /*.deepSleep  */ CY_SYSANALOG_DEEPSLEEP_DISABLE,
};

/*******************************************************************************
* Function Name: Cy_SysAnalog_Init
****************************************************************************//**
*
* Initialize the AREF block.
*
* \param config
* Pointer to structure containing configuration data. See \ref cy_stc_sysanalog_config_t
*
* \return
* - \ref CY_SYSANALOG_SUCCESS : initialization complete
* - \ref CY_SYSANALOG_BAD_PARAM : input pointers are null, initialization incomplete
*
* \funcusage
*
* \snippet sysanalog/snippet/main.c SYSANA_SNIPPET_INIT
*
*******************************************************************************/
cy_en_sysanalog_status_t Cy_SysAnalog_Init(const cy_stc_sysanalog_config_t *config)
{
    CY_ASSERT_L1(NULL != config);

    cy_en_sysanalog_status_t result;
    uint32_t ctrlReg = CY_SYSANALOG_DEINIT;

    if (NULL == config)
    {
        result = CY_SYSANALOG_BAD_PARAM;
    }
    else
    {
        CY_ASSERT_L3(CY_SYSANALOG_STARTUP(config->startup));
        CY_ASSERT_L3(CY_SYSANALOG_DEEPSLEEP(config->deepSleep));
        CY_ASSERT_L3(CY_SYSANALOG_VREF(config->vref));
        CY_ASSERT_L3(CY_SYSANALOG_IZTAT(config->iztat));

        ctrlReg = (uint32_t) config->startup \
                  | CY_SYSANALOG_DEFAULT_BIAS_SCALE \
                  | (uint32_t) config->iztat \
                  | (uint32_t) config->vref \
                  | (uint32_t) config->deepSleep;

        PASS_AREF_AREF_CTRL = ctrlReg;

        result = CY_SYSANALOG_SUCCESS;
    }

    return result;
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXS40PASS */

/* [] END OF FILE */

