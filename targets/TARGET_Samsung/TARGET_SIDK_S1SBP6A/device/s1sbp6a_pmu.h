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

#ifndef __S1SBP6A_PMU_H
#define __S1SBP6A_PMU_H

#define PMU_SYSCON_CTRL_OFFSET                            (0x0000)
#define PMU_SYSCON_DLY_CTRL_OFFSET                        (0x0004)
#define PMU_SYSCON_MCU_DLY_CTRL_OFFSET                    (0x0008)
#define PMU_SYSCON_PD_CTRL_OFFSET                         (0x0200)
#define PMU_SYSCON_PD_STATUS_OFFSET                       (0x0204)
#define PMU_SYSCON_PMU_RET_CTRL_OFFSET                    (0x0208)
#define PMU_SYSCON_PMU_FLASH_MODE_OFFSET                  (0x020C)

/* PMU_SYSCON_CTRL */
#define PMU_SYSCON_CTRL_RTCMODEREADY_SHIFT                (0)
#define PMU_SYSCON_CTRL_STANBYTEADY_SHIFT                 (1)
#define PMU_SYSCON_CTRL_CORE_RETENTION_SHIFT              (2)
#define PMU_SYSCON_CTRL_SYSTEM_RETENTION_SHIFT            (3)
#define PMU_SYSCON_CTRL_HOSCEN_CTRL_SHIFT                 (4)
#define PMU_SYSCON_CTRL_LOSCEN_CTRL_SHIFT                 (5)
#define PMU_SYSCON_CTRL_PLLEN_CTRL_SHIFT                  (6)
#define PMU_SYSCON_CTRL_HOSCEN_SEQ_SHIFT                  (7)
#define PMU_SYSCON_CTRL_LOSCEN_SEQ_SHIFT                  (8)
#define PMU_SYSCON_CTRL_PLLEN_SEQ_SHIFT                   (9)
#define PMU_SYSCON_CTRL_MANUAL_OSC_SHIFT                  (10)

#define PMU_SYSCON_CTRL_RTCMODEREADY_MASK                 (0x01 << PMU_SYSCON_CTRL_RTCMODEREADY_SHIFT)
#define PMU_SYSCON_CTRL_STANBYTEADY_MASK                  (0x01 << PMU_SYSCON_CTRL_STANBYTEADY_SHIFT)
#define PMU_SYSCON_CTRL_CORE_RETENTION_MASK               (0x01 << PMU_SYSCON_CTRL_CORE_RETENTION_SHIFT)
#define PMU_SYSCON_CTRL_SYSTEM_RETENTION_MASK             (0x01 << PMU_SYSCON_CTRL_SYSTEM_RETENTION_SHIFT)
#define PMU_SYSCON_CTRL_HOSCEN_CTRL_MASK                  (0x01 << PMU_SYSCON_CTRL_HOSCEN_CTRL_SHIFT)
#define PMU_SYSCON_CTRL_LOSCEN_CTRL_MASK                  (0x01 << PMU_SYSCON_CTRL_LOSCEN_CTRL_SHIFT)
#define PMU_SYSCON_CTRL_PLLEN_CTRL_MASK                   (0x01 << PMU_SYSCON_CTRL_PLLEN_CTRL_SHIFT)
#define PMU_SYSCON_CTRL_HOSCEN_SEQ_MASK                   (0x01 << PMU_SYSCON_CTRL_HOSCEN_SEQ_SHIFT)
#define PMU_SYSCON_CTRL_LOSCEN_SEQ_MASK                   (0x01 << PMU_SYSCON_CTRL_LOSCEN_SEQ_SHIFT)
#define PMU_SYSCON_CTRL_PLLEN_SEQ_MASK                    (0x01 << PMU_SYSCON_CTRL_PLLEN_SEQ_SHIFT)
#define PMU_SYSCON_CTRL_MANUAL_OSC_MASK                   (0x01 << PMU_SYSCON_CTRL_MANUAL_OSC_SHIFT)

#define PMU_SYSCON_CTRL_RTCMODEREADY(c)                   ((c) << PMU_SYSCON_CTRL_RTCMODEREADY_SHIFT)
#define PMU_SYSCON_CTRL_STANBYTEADY(c)                    ((c) <<  PMU_SYSCON_CTRL_STANBYTEADY_SHIFT)
#define PMU_SYSCON_CTRL_CORE_RETENTION(c)                 ((c) << PMU_SYSCON_CTRL_CORE_RETENTION_SHIFT)
#define PMU_SYSCON_CTRL_SYSTEM_RETENTION(c)               ((c) << PMU_SYSCON_CTRL_SYSTEM_RETENTION_SHIFT)
#define PMU_SYSCON_CTRL_HOSCEN_CTRL(c)                    ((c) <<  PMU_SYSCON_CTRL_HOSCEN_CTRL_SHIFT)
#define PMU_SYSCON_CTRL_LOSCEN_CTRL(c)                    ((c) << PMU_SYSCON_CTRL_LOSCEN_CTRL_SHIFT)
#define PMU_SYSCON_CTRL_PLLEN_CTRL(c)                     ((c) << PMU_SYSCON_CTRL_PLLEN_CTRL_SHIFT)
#define PMU_SYSCON_CTRL_HOSCEN_SEQ(c)                     ((c) << PMU_SYSCON_CTRL_HOSCEN_SEQ_SHIFT)
#define PMU_SYSCON_CTRL_LOSCEN_SEQ(c)                     ((c) << PMU_SYSCON_CTRL_LOSCEN_SEQ_SHIFT)
#define PMU_SYSCON_CTRL_PLLEN_SEQ(c)                      ((c) << PMU_SYSCON_CTRL_PLLEN_SEQ_SHIFT)
#define PMU_SYSCON_CTRL_MANUAL_OSC(c)                     ((c) << PMU_SYSCON_CTRL_MANUAL_OSC_SHIFT)

/* PMU_SYSCON_DLY_CTRL_ */

#define PMU_SYSCON_DLY_CTRL_CLK_PU_SHIFT                   (0)
#define PMU_SYSCON_DLY_CTRL_MCURSTN_PU_SHIFT               (4)
#define PMU_SYSCON_DLY_CTRL_FLASH_PU_SHIFT                 (8)
#define PMU_SYSCON_DLY_CTRL_FLASH_PD_SHIFT                 (12)
#define PMU_SYSCON_DLY_CTRL_MCURSTN_PD_SHIFT               (16)
/* PMU_SYSCON_MCU_DLY_CTRL */
#define PMU_SYSCON_MCU_DLY_CTRL_ISO_SHIFT                  (0)
#define PMU_SYSCON_MCU_DLY_CTRL_RET_SHIFT                  (8)
#define PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING_SHIFT           (16)
#define PMU_SYSCON_MCU_DLY_CTRL_RSTN_SHIFT                 (24)

#define PMU_SYSCON_MCU_DLY_CTRL_ISO_MASK                   (0xff << PMU_SYSCON_MCU_DLY_CTRL_ISO_SHIFT)
#define PMU_SYSCON_MCU_DLY_CTRL_RET_MASK                   (0xff << PMU_SYSCON_MCU_DLY_CTRL_RET_SHIFT)
#define PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING_MASK            (0xff << PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING_SHIFT)
#define PMU_SYSCON_MCU_DLY_CTRL_RSTN_MASK                  (0xff << PMU_SYSCON_MCU_DLY_CTRL_RSTN_SHIFT)

#define PMU_SYSCON_MCU_DLY_CTRL_ISO(c)                     ((c) << PMU_SYSCON_MCU_DLY_CTRL_ISO_SHIFT)
#define PMU_SYSCON_MCU_DLY_CTRL_RET(c)                     ((c) << PMU_SYSCON_MCU_DLY_CTRL_RET_SHIFT)
#define PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING(c)              ((c) << PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING_SHIFT)
#define PMU_SYSCON_MCU_DLY_CTRL_RSTN(c)                    ((c) << PMU_SYSCON_MCU_DLY_CTRL_RSTN_SHIFT)


/* PMU_SYSCON_CTRL */
#define PMU_SYSCON_PD_MCU_SRAM_SHIFT                      (0)
#define PMU_SYSCON_PD_DSP_TMEM_SHIFT                      (8)
#define PMU_SYSCON_PD_DSP_DMEM_SHIFT                      (9)
#define PMU_SYSCON_PD_DSP_SHIFT                           (13)
#define PMU_SYSCON_PD_FCACHE_SHIFT                        (15)
#define PMU_SYSCON_PD_DECIM_SHIFT                         (16)
#define PMU_SYSCON_PD_FLASH_SHIFT                         (17)
#define PMU_SYSCON_CHG_PWR_STATE_SHIFT                    (19)

#define PMU_SYSCON_PD_MCU_SRAM_MASK                       (0xFF << PMU_SYSCON_PD_MCU_SRAM_SHIFT)
#define PMU_SYSCON_PD_DSP_TMEM_MASK                       (0x01 << PMU_SYSCON_PD_DSP_TMEM_SHIFT)
#define PMU_SYSCON_PD_DSP_DMEM_MASK                       (0x0F << PMU_SYSCON_PD_DSP_DMEM_SHIFT)
#define PMU_SYSCON_PD_DSP_MASK                            (0x01 << PMU_SYSCON_PD_DSP_SHIFT)
#define PMU_SYSCON_PD_FCACHE_MASK                         (0x01 << PMU_SYSCON_PD_FCACHE_SHIFT)
#define PMU_SYSCON_PD_DECIM_MASK                          (0x01 << PMU_SYSCON_PD_DECIM_SHIFT)
#define PMU_SYSCON_PD_FLASH_MASK                          (0x01 << PMU_SYSCON_PD_FLASH_SHIFT)
#define PMU_SYSCON_CHG_PWR_STATE_MASK                     (0x01 << PMU_SYSCON_CHG_PWR_STATE_SHIFT)

#define PMU_SYSCON_PD_MCU_SRAM(c)                         ((c) << PMU_SYSCON_PD_MCU_SRAM_SHIFT)
#define PMU_SYSCON_PD_DSP_TMEM(c)                         ((c) << PMU_SYSCON_PD_DSP_TMEM_SHIFT)
#define PMU_SYSCON_PD_DSP_DMEM(c)                         ((c) << PMU_SYSCON_PD_DSP_DMEM_SHIFT)
#define PMU_SYSCON_PD_DSP(c)                              ((c) << PMU_SYSCON_PD_DSP_SHIFT)
#define PMU_SYSCON_PD_FCACHE(c)                           ((c) << PMU_SYSCON_PD_FCACHE_SHIFT)
#define PMU_SYSCON_PD_DECIM(c)                            ((c) << PMU_SYSCON_PD_DECIM_SHIFT)
#define PMU_SYSCON_PD_FLASH(c)                            ((c) << PMU_SYSCON_PD_FLASH_SHIFT)
#define PMU_SYSCON_CHG_PWR_STATE(c)                       ((c) << PMU_SYSCON_CHG_PWR_STATE_SHIFT)

#define PMU_SYSCON_PD_MCU_SRAM_STATUS_SHIFT               (0)
#define PMU_SYSCON_PD_DSP_TMEM_STATAUS_SHIFT              (8)
#define PMU_SYSCON_PD_DSP_DMEM_STATUS_SHIFT               (9)
#define PMU_SYSCON_PD_DSP_STATUS_SHIFT                    (13)
#define PMU_SYSCON_PD_FCACHE_STATUS_SHIFT                 (15)
#define PMU_SYSCON_PD_DECIM_STATUS_SHIFT                  (16)

#define PMU_SYSCON_PD_MCU_SRAM_STATUS_MASK                (0xFF << PMU_SYSCON_PD_MCU_SRAM_STATUS_SHIFT)
#define PMU_SYSCON_PD_DSP_TMEM_STAUS_MASK                 (0x01 << PMU_SYSCON_PD_DSP_TMEM_STATAUS_SHIFT)
#define PMU_SYSCON_PD_DSP_DMEM_STATUS_MASK                (0x0F << PMU_SYSCON_PD_DSP_DMEM_STATUS_SHIFT)
#define PMU_SYSCON_PD_DSP_STATUS_MASK                     (0x01 << PMU_SYSCON_PD_DSP_STATUS_SHIFT)
#define PMU_SYSCON_PD_FCACHE_STATUS_MASK                  (0x01 << PMU_SYSCON_PD_FCACHE_STATUS_SHIFT)
#define PMU_SYSCON_PD_DECIM_STATUS_MASK                   (0x01 << PMU_SYSCON_PD_DECIM_STATUS_SHIFT)

#define PMU_SYSCON_PD_MCU_SRAM_STATUS(c)                  ((c) << PMU_SYSCON_PD_MCU_SRAM_STATUS_SHIFT)
#define PMU_SYSCON_PD_DSP_TMEM_STAUS(c)                   ((c) << PMU_SYSCON_PD_DSP_TMEM_STATAUS_SHIFT)
#define PMU_SYSCON_PD_DSP_DMEM_STATUS(c)                  ((c) << PMU_SYSCON_PD_DSP_DMEM_STATUS_SHIFT)
#define PMU_SYSCON_PD_DSP_STATUS(c)                       ((c) << PMU_SYSCON_PD_DSP_STATUS_SHIFT)
#define PMU_SYSCON_PD_FCACHE_STATUS(c)                    ((c) << PMU_SYSCON_PD_FCACHE_STATUS_SHIFT)
#define PMU_SYSCON_PD_DECIM_STATUS(c)                     ((c) << PMU_SYSCON_PD_DECIM_STATUS_SHIFT)

#define PMU_SYSCON_PMU_RET_CTRL_MCU_SRAM_SHIFT            (0)
#define PMU_SYSCON_PMU_RET_CTRL_DSP_TMEM_SHIFT            (8)
#define PMU_SYSCON_PMU_RET_CTRL_DSP_DMEM_SHIFT            (9)
#define PMU_SYSCON_PMU_RET_CTRL_FCHCHE_SHIFT              (15)
#define PMU_SYSCON_PMU_RET_CTRL_DECIM_SHIFT               (16)
#define PMU_SYSCON_PMU_RET_CTRL_SYSPWR_CTRL_SHIFT         (19)
#define PMU_SYSCON_PMU_RET_CTRL_MCUMEM_PD_M_SHIFT         (20)
#define PMU_SYSCON_PMU_RET_CTRL_DSP_RESET_SHIFT           (21)

#define PMU_SYSCON_PMU_RET_CTRL_MCU_SRAM(c)               ((c) <<  PMU_SYSCON_PMU_RET_CTRL_MCU_SRAM_SHIFT)
#define PMU_SYSCON_PMU_RET_CTRL_DSP_TMEM(c)               ((c) <<  PMU_SYSCON_PMU_RET_CTRL_DSP_TMEM_SHIFT)
#define PMU_SYSCON_PMU_RET_CTRL_DSP_DMEM(c)               ((c) <<  PMU_SYSCON_PMU_RET_CTRL_DSP_DMEM_SHIFT)
#define PMU_SYSCON_PMU_RET_CTRL_FCHCHE(c)                 ((c) <<  PMU_SYSCON_PMU_RET_CTRL_FCHCHE_SHIFT)
#define PMU_SYSCON_PMU_RET_CTRL_DECIM(c)                  ((c) <<  PMU_SYSCON_PMU_RET_CTRL_DECIM_SHIFT)
#define PMU_SYSCON_PMU_RET_CTRL_SYSPWR_CTRL(c)            ((c) <<  PMU_SYSCON_PMU_RET_CTRL_SYSPWR_CTRL_SHIFT)
#define PMU_SYSCON_PMU_RET_CTRL_MCUMEM_PD_M(c)            ((c) <<  PMU_SYSCON_PMU_RET_CTRL_MCUMEM_PD_M_SHIFT)
#define PMU_SYSCON_PMU_RET_CTRL_DSP_RESET(c)              ((c) <<  PMU_SYSCON_PMU_RET_CTRL_DSP_RESET_SHIFT)


#define PMU_SYSCON_PMU_FLASH_MODE_OFFSET                  (0x020C)


typedef enum _pmu_retention_ctrl_t {
    PMU_RETENTION_MCU_SRAM0 = PMU_SYSCON_PMU_RET_CTRL_MCU_SRAM_SHIFT,
    PMU_RETENTION_MCU_SRAM1,
    PMU_RETENTION_MCU_SRAM2,
    PMU_RETENTION_MCU_SRAM3,
    PMU_RETENTION_MCU_SRAM4,
    PMU_RETENTION_MCU_SRAM5,
    PMU_RETENTION_MCU_SRAM6,
    PMU_RETENTION_MCU_SRAM7,
    PMU_RETENTION_DSP_TMEM,
    PMU_RETENTION_DSP_DMEM0,
    PMU_RETENTION_DSP_DMEM1,
    PMU_RETENTION_DSP_DMEM2,
    PMU_RETENTION_DSP_DMEM3,
    PMU_RETENTION_FCACHE = PMU_SYSCON_PMU_RET_CTRL_FCHCHE_SHIFT,
    PMU_RETENTION_DECIM,
    PMU_RETENTION_SYSTEM_POWER = PMU_SYSCON_PMU_RET_CTRL_SYSPWR_CTRL_SHIFT,
    PMU_SRP_RESET_CTRL,
} pmu_retention_ctrl_t;

typedef enum {
    PMU_POWER_DOWN_MCU_SRAM0 = PMU_SYSCON_PD_MCU_SRAM_SHIFT,
    PMU_POWER_DOWN_MCU_SRAM1,
    PMU_POWER_DOWN_MCU_SRAM2,
    PMU_POWER_DOWN_MCU_SRAM3,
    PMU_POWER_DOWN_MCU_SRAM4,
    PMU_POWER_DOWN_MCU_SRAM5,
    PMU_POWER_DOWN_MCU_SRAM6,
    PMU_POWER_DOWN_MCU_SRAM7,
    PMU_POWER_DOWN_DSP_TMEM = PMU_SYSCON_PD_DSP_TMEM_SHIFT,
    PMU_POWER_DOWN_DSP_DMEM0,
    PMU_POWER_DOWN_DSP_DMEM1,
    PMU_POWER_DOWN_DSP_DMEM2,
    PMU_POWER_DOWN_DSP_DMEM3,
    PMU_POWER_DOWN_DSP = PMU_SYSCON_PD_DSP_SHIFT,
    PMU_POWER_DOWN_FCACHE = PMU_SYSCON_PD_FCACHE_SHIFT,
    PMU_POWER_DOWN_DECIM = PMU_SYSCON_PD_DECIM_SHIFT,
    PMU_POWER_DOWN_FLASH = PMU_SYSCON_PD_FLASH_SHIFT,
    PMU_CHANGE_POWER_STATE = PMU_SYSCON_CHG_PWR_STATE_SHIFT,
} pmu_power_ctrl_t;

typedef enum {
    PMU_RTC_MODE_READY = PMU_SYSCON_CTRL_RTCMODEREADY_SHIFT,
    PMU_STANDBY_MODE_READY = PMU_SYSCON_CTRL_STANBYTEADY_SHIFT,
    PMU_CORE_RETENTION = PMU_SYSCON_CTRL_CORE_RETENTION_SHIFT,
    PMU_SYSTEM_RETENTION = PMU_SYSCON_CTRL_SYSTEM_RETENTION_SHIFT,
    PMU_HOSC_ENABLE_PD = PMU_SYSCON_CTRL_HOSCEN_CTRL_SHIFT,
    PMU_LOSC_ENABLE_PD = PMU_SYSCON_CTRL_LOSCEN_CTRL_SHIFT,
    PMU_PLL_ENABLE_PD = PMU_SYSCON_CTRL_PLLEN_CTRL_SHIFT,
    PMU_HOSC_ENABLE_PU = PMU_SYSCON_CTRL_HOSCEN_SEQ_SHIFT,
    PMU_LOSC_ENABLE_PU = PMU_SYSCON_CTRL_LOSCEN_SEQ_SHIFT,
    PMU_PLL_ENABLE_PU = PMU_SYSCON_CTRL_PLLEN_SEQ_SHIFT,
    PMU_MANUAL_OSC_CTRL = PMU_SYSCON_CTRL_MANUAL_OSC_SHIFT,
} pmu_syscon_ctrl_t;

typedef enum {
    PMU_DELAY_CLOCK_ENABLE_PU = PMU_SYSCON_DLY_CTRL_CLK_PU_SHIFT,
    PMU_DELAY_MCU_RESET_RELEASE_PU = PMU_SYSCON_DLY_CTRL_MCURSTN_PU_SHIFT,
    PMU_DELAY_FLASH_POWER_DOWN_PU = PMU_SYSCON_DLY_CTRL_FLASH_PU_SHIFT,
    PMU_DELAY_FLASH_POWER_DOWN_PD = PMU_SYSCON_DLY_CTRL_FLASH_PD_SHIFT,
    PMU_DLEAY_MCU_RESET_POWN_PD = PMU_SYSCON_DLY_CTRL_MCURSTN_PD_SHIFT,

    PMU_DELAY_ISOLATION_STATE_PUPD = PMU_SYSCON_MCU_DLY_CTRL_ISO_SHIFT,
    PMU_DELAY_RETENTION_STATE_PUPD = PMU_SYSCON_MCU_DLY_CTRL_RET_SHIFT,
    PMU_DELAY_POWER_GATING = PMU_SYSCON_MCU_DLY_CTRL_PWR_GATING_SHIFT,
    PMU_DELAY_RESET_STATE_PUPD = PMU_SYSCON_MCU_DLY_CTRL_RSTN_SHIFT,
} pmu_delay_ctrl_t;


typedef enum {
    PMU_LPM0,
    PMU_LPM1,
    PMU_LPM2,
    PMU_LPM3,
    PMU_LPM4,
} pmu_LPM_t;

void bp6a_pum_init(void);
void bp6a_power_mode(pmu_LPM_t mode);

#endif /* __S1SBP6A_PMU_H*/