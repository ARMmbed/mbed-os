/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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

#ifndef INCLUDED_WHD_CHIP_REG_H_
#define INCLUDED_WHD_CHIP_REG_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*             Architecture Constants
******************************************************/

/* D11 registers and SHM */
#define D11_BASE_ADDR           0x18001000
#define D11_AXI_BASE_ADDR       0xE8000000
#define D11_SHM_BASE_ADDR       (D11_AXI_BASE_ADDR + 0x4000)

#define D11REG_ADDR(offset)     (D11_BASE_ADDR + offset)
#define D11IHR_ADDR(offset)     (D11_AXI_BASE_ADDR + 0x400 + (2 * offset) )
#define D11SHM_ADDR(offset)     (D11_SHM_BASE_ADDR + offset)

#define M_DS1_CTRL_STATUS               (0xe0b * 2)

/* RMC operational modes */
enum ds1_ctrl_status
{
    DS1_SLEEP               = 0, /* DS1_SLEEP  */
    MAC_ON                  = 1, /* MAC_ON */
    RADIO_PHY_ON            = 2, /* RADIO_PHY_ON  */
    DS1_EXIT                = 3  /* DS1_EXIT.*/
};

#define M_DS1_CTRL_SDIO                 (0xe0c * 2)

#define C_DS1_CTRL_SDIO_DS1_SLEEP       (1 << 0)
#define C_DS1_CTRL_SDIO_MAC_ON          (1 << 1)
#define C_DS1_CTRL_SDIO_RADIO_PHY_ON    (1 << 2)
#define C_DS1_CTRL_SDIO_DS1_EXIT        (1 << 3)
#define C_DS1_CTRL_PROC_DONE            (1 << 8)
#define C_DS1_CTRL_REQ_VALID            (1 << 9)
/* MacControl register */
#define D11_MACCONTROL_REG          D11REG_ADDR(0x120)
#define D11_MACCONTROL_REG_SIZE     4
#define D11_MACCONTROL_REG_WAKE     (1 << 26)
#define D11_MACCONTROL_CLEAR_WAKE     (0xFBFFFFFF)
#define PMU_MINRESMASK              (PMU_BASE_ADDRESS + 0x618)
#define DEFAULT_43012_MIN_RES_MASK   0x0f8bfe77

/* Backplane architecture */
#define CHIPCOMMON_BASE_ADDRESS  0x18000000   /* Chipcommon core register region   */
#define I2S0_BASE_ADDRESS        0x18001000   /* I2S0 core register region     */
#define I2S1_BASE_ADDRESS        0x18002000   /* I2S1 core register region     */
#define APPS_ARMCR4_BASE_ADDRESS 0x18003000   /* Apps Cortex-R4 core register region     */
#define DMA_BASE_ADDRESS         0x18004000   /* DMA core register region     */
#define GMAC_BASE_ADDRESS        0x18005000   /* GMAC core register region     */
#define USB20H0_BASE_ADDRESS     0x18006000   /* USB20H0 core register region     */
#define USB20D_BASE_ADDRESS      0x18007000   /* USB20D core register region     */
#define SDIOH_BASE_ADDRESS       0x18008000   /* SDIOH Device core register region */
#define DOT11MAC_BASE_ADDRESS    0x18001000

#define BACKPLANE_ADDRESS_MASK   0x7FFF
#define BACKPLANE_WINDOW_SIZE    (BACKPLANE_ADDRESS_MASK + 1)

#define CHIP_STA_INTERFACE   0
#define CHIP_AP_INTERFACE    1
#define CHIP_P2P_INTERFACE   2

/* Chipcommon registers */
#define CHIPCOMMON_CORE_CAPEXT_ADDR  ( (uint32_t)(CHIPCOMMON_BASE_ADDRESS + 0x64c) )
#define CHIPCOMMON_CORE_CAPEXT_SR_SUPPORTED   ( (uint32_t)(1 << 1) )
#define CHIPCOMMON_CORE_RCTL_LOGIC_DISABLE   ( (uint32_t)(1 << 27) )
#define CHIPCOMMON_CORE_RCTL_MACPHY_DISABLE   ( (uint32_t)(1 << 26) )
#define CHIPCOMMON_CORE_RETENTION_CTL      ( (uint32_t)(CHIPCOMMON_BASE_ADDRESS + 0x670) )

#define CHIPCOMMON_GPIO_CONTROL ( (uint32_t)(CHIPCOMMON_BASE_ADDRESS + 0x6C) )
#define CHIPCOMMON_SR_CONTROL0        ( (uint32_t)(CHIPCOMMON_BASE_ADDRESS + 0x504) )
#define CHIPCOMMON_SR_CONTROL1      ( (uint32_t)(CHIPCOMMON_BASE_ADDRESS + 0x508) )

/* SOCSRAM core registers */
#define SOCSRAM_BANKX_INDEX(wd)   ( (uint32_t)(GET_C_VAR(wd, SOCSRAM_BASE_ADDRESS) + 0x10) )
#define SOCSRAM_BANKX_PDA(wd)    ( (uint32_t)(GET_C_VAR(wd, SOCSRAM_BASE_ADDRESS) + 0x44) )

/* PMU core registers */
#define RETENTION_CTL(wd)       ( (uint32_t)(GET_C_VAR(wd, PMU_BASE_ADDRESS) + 0x670) )
#define RCTL_MACPHY_DISABLE     ( (uint32_t)(1 << 26) )
#define RCTL_LOGIC_DISABLE      ( (uint32_t)(1 << 27) )

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_CHIP_REG_H_ */

