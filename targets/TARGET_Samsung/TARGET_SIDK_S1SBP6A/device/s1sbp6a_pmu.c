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

#include <stdbool.h>
#include "s1sbp6a.h"
#include "s1sbp6a_type.h"
#include "s1sbp6a_pmu.h"

void PMU_SetSysconControl(pmu_syscon_ctrl_t field, bool enable)
{
    modifyreg32(&BP_SYSCON->PMU_SYSCON_CTRL, 0x01 << field, enable << field);
}

bool PMU_GetSysconControl(pmu_syscon_ctrl_t field)
{
    return !!((getreg32(&BP_SYSCON->PMU_SYSCON_CTRL) >> field) & 0x01);
}

void PMU_SetDelayControl(pmu_delay_ctrl_t field, uint32_t val)
{

    uint32_t reg = (uint32_t)(&BP_SYSCON->PMU_SYSCON_DLY_CTRL);
    uint32_t mask = 0x0F;

    if (field > PMU_DLEAY_MCU_RESET_POWN_PD) {
        reg = (uint32_t)(&BP_SYSCON->PMU_MCU_DLY_CTRL);
        mask = 0xFF;
    }

    modifyreg32(reg,  mask << field, (val & mask) << field);
}

uint32_t PMU_GetDelayControl(pmu_delay_ctrl_t field)
{

    uint32_t reg = (uint32_t)(&BP_SYSCON->PMU_SYSCON_DLY_CTRL);
    uint32_t mask = 0x0F;

    if (field > PMU_DLEAY_MCU_RESET_POWN_PD) {
        reg = (uint32_t)(&BP_SYSCON->PMU_MCU_DLY_CTRL);
        mask = 0xFF;
    }

    return ((getreg32(reg) >> field) & mask);
}

void PMU_SetSysconDelayEnable(bool val)
{
    putreg32(&BP_SYSCON->PMU_SYSCON_DLY_EN, (uint32_t)val);
}


bool PMU_GetSysconDelayEnable(void)
{
    return !!(getreg32(&BP_SYSCON->PMU_SYSCON_DLY_EN));
}


void PMU_SetSysconFastBoot(bool val)
{
    putreg32(&BP_SYSCON->PMU_SYSCON_FAST_BOOT, (uint32_t)val);
}

bool PMU_GetSysconFastBoot(void)
{
    return !!(getreg32(&BP_SYSCON->PMU_SYSCON_FAST_BOOT));
}


void PMU_SetPowerControl(pmu_power_ctrl_t field, bool enable)
{
    modifyreg32(&BP_SYSCON->PMU_SYSCON_PD_CTRL, 0x01 << field, enable << field);
}


bool PMU_GetPowerControl(pmu_power_ctrl_t field)
{
    return !!((getreg32(&BP_SYSCON->PMU_SYSCON_PD_CTRL) >> field) & 0x01);
}


bool PMU_GetPowerStatus(pmu_power_ctrl_t field)
{
    return !!((getreg32(&BP_SYSCON->PMU_SYSCON_PD_CTRL) >> field) & 0x01);
}

void PMU_SetRetentionControl(pmu_retention_ctrl_t field, bool enable)
{
    modifyreg32(&BP_SYSCON->PMU_SYSCON_RET_CTRL, 0x1 << field, enable << field);
}

bool PMU_GetRetentionControl(pmu_retention_ctrl_t field, bool *pEnable)
{
    return !!((getreg32(&BP_SYSCON->PMU_SYSCON_RET_CTRL) >> field) & 0x01);
}


void bp6a_power_mode(pmu_LPM_t mode)
{
    switch (mode) {
        case PMU_LPM0:
            putreg32(&BP_SYSCTRL->PMUCTRL, 0x1);
            PMU_SetRetentionControl(PMU_RETENTION_SYSTEM_POWER, true);
            PMU_SetSysconControl(PMU_RTC_MODE_READY, true);

            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __DSB();
            __WFI();
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

            break;
        case PMU_LPM1:
            putreg32(&BP_SYSCTRL->PMUCTRL, 0x1);
            PMU_SetRetentionControl(PMU_RETENTION_SYSTEM_POWER, true);
            modifyreg32(&BP_SYSCON->PMU_SYSCON_CTRL,
                        PMU_SYSCON_CTRL_RTCMODEREADY_MASK |
                        PMU_SYSCON_CTRL_STANBYTEADY_MASK |
                        PMU_SYSCON_CTRL_CORE_RETENTION_MASK |
                        PMU_SYSCON_CTRL_SYSTEM_RETENTION_MASK |
                        PMU_SYSCON_CTRL_HOSCEN_CTRL_MASK |
                        PMU_SYSCON_CTRL_LOSCEN_CTRL_MASK |
                        PMU_SYSCON_CTRL_PLLEN_CTRL_MASK |
                        PMU_SYSCON_CTRL_HOSCEN_SEQ_MASK |
                        PMU_SYSCON_CTRL_LOSCEN_SEQ_MASK |
                        PMU_SYSCON_CTRL_PLLEN_SEQ_MASK |
                        PMU_SYSCON_CTRL_MANUAL_OSC_MASK,
                        PMU_SYSCON_CTRL_RTCMODEREADY(0) |
                        PMU_SYSCON_CTRL_STANBYTEADY(0) |
                        PMU_SYSCON_CTRL_CORE_RETENTION(1) |
                        PMU_SYSCON_CTRL_SYSTEM_RETENTION(1) |
                        PMU_SYSCON_CTRL_HOSCEN_CTRL(1) |
                        PMU_SYSCON_CTRL_LOSCEN_CTRL(1) |
                        PMU_SYSCON_CTRL_PLLEN_CTRL(1) |
                        PMU_SYSCON_CTRL_HOSCEN_SEQ(1) |
                        PMU_SYSCON_CTRL_LOSCEN_SEQ(1) |
                        PMU_SYSCON_CTRL_PLLEN_SEQ(0) |
                        PMU_SYSCON_CTRL_MANUAL_OSC(1));
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            PMU_SetSysconControl(PMU_STANDBY_MODE_READY, true);
            __DSB();
            __WFI();
            PMU_SetSysconControl(PMU_STANDBY_MODE_READY, false);
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

            PMU_SetRetentionControl(PMU_RETENTION_SYSTEM_POWER, false);

            break;
        case PMU_LPM2:
            putreg32(&BP_SYSCTRL->PMUCTRL, 0x1);
            PMU_SetRetentionControl(PMU_RETENTION_SYSTEM_POWER, true);

            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __DSB();
            __WFI();
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

            PMU_SetRetentionControl(PMU_RETENTION_SYSTEM_POWER, false);

            break;
        case PMU_LPM3:
            putreg32(&BP_SYSCTRL->PMUCTRL, 0x1);
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            __DSB();
            __WFI();
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
            break;
        case PMU_LPM4:
            putreg32(&BP_SYSCTRL->PMUCTRL, 0x1);

            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
            __DSB();
            __WFI();
            break;

    }
}

void bp6a_pum_init(void)
{
    modifyreg32(&(BP_SYSCON->PMU_SYSCON_CTRL),
                PMU_SYSCON_CTRL_RTCMODEREADY_MASK |
                PMU_SYSCON_CTRL_STANBYTEADY_MASK |
                PMU_SYSCON_CTRL_HOSCEN_CTRL_MASK |
                PMU_SYSCON_CTRL_LOSCEN_CTRL_MASK |
                PMU_SYSCON_CTRL_PLLEN_CTRL_MASK  |
                PMU_SYSCON_CTRL_HOSCEN_SEQ_MASK  |
                PMU_SYSCON_CTRL_LOSCEN_SEQ_MASK  |
                PMU_SYSCON_CTRL_PLLEN_SEQ_MASK |
                PMU_SYSCON_CTRL_MANUAL_OSC_MASK,
                PMU_SYSCON_CTRL_RTCMODEREADY(1) |
                PMU_SYSCON_CTRL_STANBYTEADY(0) |
                PMU_SYSCON_CTRL_HOSCEN_CTRL(1) |
                PMU_SYSCON_CTRL_LOSCEN_CTRL(1) |
                PMU_SYSCON_CTRL_PLLEN_CTRL(0)  |
                PMU_SYSCON_CTRL_HOSCEN_SEQ(1)  |
                PMU_SYSCON_CTRL_LOSCEN_SEQ(1)  |
                PMU_SYSCON_CTRL_PLLEN_SEQ(0)   |
                PMU_SYSCON_CTRL_MANUAL_OSC(1));

    modifyreg32(&(BP_SYSCON->PMU_SYSCON_PD_CTRL),
                PMU_SYSCON_PD_DSP_TMEM_MASK |
                PMU_SYSCON_PD_DSP_DMEM_MASK |
                PMU_SYSCON_PD_DECIM_MASK |
                PMU_SYSCON_PD_DSP_MASK,
                PMU_SYSCON_PD_DSP_TMEM(1) |
                PMU_SYSCON_PD_DSP_DMEM(0x0F) |
                PMU_SYSCON_PD_DSP(1) |
                PMU_SYSCON_PD_DECIM(1));

    modifyreg32(&(BP_SYSCON->PMU_SYSCON_PD_CTRL),
                PMU_SYSCON_CHG_PWR_STATE_MASK,
                PMU_SYSCON_CHG_PWR_STATE(1));
    modifyreg32(&(BP_SYSCON->PMU_SYSCON_DLY_EN),
                PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING_MASK,
                PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING(0));
}
