/*******************************************************************************
* File Name: cycfg_system.h
*
* Description:
* System configuration
* This file was automatically generated and should not be modified.
* Tools Package 2.2.0.2790
* latest-v2.X 2.0.0.6211
* personalities 3.0.0.0
* udd 3.0.0.562
*
********************************************************************************
* Copyright 2020 Cypress Semiconductor Corporation
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

#if !defined(CYCFG_SYSTEM_H)
#define CYCFG_SYSTEM_H

#include "cycfg_notices.h"
#include "cy_sysclk.h"
#include "cy_pra.h"
#include "cy_pra_cfg.h"
#include "cy_systick.h"
#if defined (CY_USING_HAL)
	#include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)

#if defined(__cplusplus)
extern "C" {
#endif

#define cpuss_0_dap_0_ENABLED 1U
#define srss_0_clock_0_ENABLED 1U
#define srss_0_clock_0_altsystickclk_0_ENABLED 1U
#define srss_0_clock_0_bakclk_0_ENABLED 1U
#define srss_0_clock_0_fastclk_0_ENABLED 1U
#define srss_0_clock_0_hfclk_0_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF0 0UL
#define CY_CFG_SYSCLK_CLKHF0_CLKPATH_NUM 1UL
#define srss_0_clock_0_hfclk_2_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF2 2UL
#define CY_CFG_SYSCLK_CLKHF2_CLKPATH_NUM 1UL
#define srss_0_clock_0_hfclk_4_ENABLED 1U
#define CY_CFG_SYSCLK_CLKHF4 4UL
#define CY_CFG_SYSCLK_CLKHF4_CLKPATH_NUM 1UL
#define srss_0_clock_0_ilo_0_ENABLED 1U
#define srss_0_clock_0_imo_0_ENABLED 1U
#define srss_0_clock_0_lfclk_0_ENABLED 1U
#define CY_CFG_SYSCLK_CLKLF_FREQ_HZ 32768
#define CY_CFG_SYSCLK_CLKLF_SOURCE CY_SYSCLK_CLKLF_IN_ILO
#define srss_0_clock_0_pathmux_0_ENABLED 1U
#define srss_0_clock_0_pathmux_1_ENABLED 1U
#define srss_0_clock_0_pathmux_2_ENABLED 1U
#define srss_0_clock_0_pathmux_3_ENABLED 1U
#define srss_0_clock_0_pathmux_4_ENABLED 1U
#define srss_0_clock_0_pathmux_5_ENABLED 1U
#define srss_0_clock_0_periclk_0_ENABLED 1U
#define srss_0_clock_0_pll_0_ENABLED 1U
#define srss_0_clock_0_slowclk_0_ENABLED 1U
#define srss_0_clock_0_timerclk_0_ENABLED 1U

#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_0_obj;
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_1_obj;
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_2_obj;
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_3_obj;
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_4_obj;
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t srss_0_clock_0_pathmux_5_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_system(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_SYSTEM_H */
