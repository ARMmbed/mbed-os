/*******************************************************************************
* File Name: cycfg_peripherals.c
*
* Description:
* Peripheral Hardware Block configuration
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

#include "cycfg_peripherals.h"

const cy_stc_smif_config_t CYBSP_QSPI_config = {
    .mode = (uint32_t)CY_SMIF_NORMAL,
    .deselectDelay = CYBSP_QSPI_DESELECT_DELAY,
    .rxClockSel = (uint32_t)CY_SMIF_SEL_INV_INTERNAL_CLK,
    .blockEvent = (uint32_t)CY_SMIF_BUS_ERROR,
};
#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_QSPI_obj = {
    .type = CYHAL_RSC_SMIF,
    .block_num = 0U,
    .channel_num = 0U,
};
#endif //defined (CY_USING_HAL)


void init_cycfg_peripherals(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CYBSP_QSPI_obj);
#endif //defined (CY_USING_HAL)
}
