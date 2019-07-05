/*
 * Copyright (c) 2018 Arm Limited
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
 *
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "cmsis.h"
#include "target_cfg.h"
#include "region_defs.h"
#include "tfm_secure_api.h"
#include <stdio.h>

/* Macros to pick linker symbols */
#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

/* The section names come from the scatter file */
REGION_DECLARE(Load$$LR$$, LR_NS_PARTITION, $$Base);
REGION_DECLARE(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base);

REGION_DECLARE(Image$$, ER_CODE_CMSE_VENEER, $$Base);
REGION_DECLARE(Image$$, ER_CODE_CMSE_VENEER, $$Limit);


const struct memory_region_limits memory_regions = {
    .non_secure_code_start =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        BL2_HEADER_SIZE,

    .non_secure_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base),

    .non_secure_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        NS_PARTITION_SIZE - 1,

    .veneer_base =
        (uint32_t)&REGION_NAME(Image$$, ER_CODE_CMSE_VENEER, $$Base),

    .veneer_limit =
        (uint32_t)&REGION_NAME(Image$$, ER_CODE_CMSE_VENEER, $$Limit),
};

/* Allows software, via SAU, to define the code region as a NSC */
#define NSCCFG_CODENSC  1

/* Define Peripherals NS address range for the platform */
#define PERIPHERALS_BASE_NS_START (0x40000000)
#define PERIPHERALS_BASE_NS_END   (0x4010FFFF)

/* Enable system reset request for CPU 0 */
#define ENABLE_CPU0_SYSTEM_RESET_REQUEST (1U << 4U)

/* To write into AIRCR register, 0x5FA value must be write to the VECTKEY field,
 * otherwise the processor ignores the write.
 */
#define SCB_AIRCR_WRITE_MASK ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos))

/* Debug configuration flags */
#define SPNIDEN_SEL_STATUS (0x01u << 7)
#define SPNIDEN_STATUS     (0x01u << 6)
#define SPIDEN_SEL_STATUS  (0x01u << 5)
#define SPIDEN_STATUS      (0x01u << 4)
#define NIDEN_SEL_STATUS   (0x01u << 3)
#define NIDEN_STATUS       (0x01u << 2)
#define DBGEN_SEL_STATUS   (0x01u << 1)
#define DBGEN_STATUS       (0x01u << 0)

#define All_SEL_STATUS (SPNIDEN_SEL_STATUS | SPIDEN_SEL_STATUS | \
                        NIDEN_SEL_STATUS | DBGEN_SEL_STATUS)

void enable_fault_handlers(void)
{
    /* Enables BUS, MEM, USG and Secure faults */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk
                  | SCB_SHCSR_BUSFAULTENA_Msk
                  | SCB_SHCSR_MEMFAULTENA_Msk
                  | SCB_SHCSR_SECUREFAULTENA_Msk;
}

void system_reset_cfg(void)
{
    uint32_t reg_value = SCB->AIRCR;

    /* Clear SCB_AIRCR_VECTKEY value */
    reg_value &= ~(uint32_t)(SCB_AIRCR_VECTKEY_Msk);

    /* Enable system reset request only to the secure world */
    reg_value |= (uint32_t)(SCB_AIRCR_WRITE_MASK | SCB_AIRCR_SYSRESETREQS_Msk);

    SCB->AIRCR = reg_value;
}

void tfm_spm_hal_init_debug(void)
{
    volatile struct sysctrl_t *sys_ctrl =
                                       (struct sysctrl_t *)CMSDK_SYSCTRL_BASE_S;

#if defined(DAUTH_NONE)
    /* Set all the debug enable selector bits to 1 */
    sys_ctrl->secdbgset = All_SEL_STATUS;
    /* Set all the debug enable bits to 0 */
    sys_ctrl->secdbgclr =
                   DBGEN_STATUS | NIDEN_STATUS | SPIDEN_STATUS | SPNIDEN_STATUS;
#elif defined(DAUTH_NS_ONLY)
    /* Set all the debug enable selector bits to 1 */
    sys_ctrl->secdbgset = All_SEL_STATUS;
    /* Set the debug enable bits to 1 for NS, and 0 for S mode */
    sys_ctrl->secdbgset = DBGEN_STATUS | NIDEN_STATUS;
    sys_ctrl->secdbgclr = SPIDEN_STATUS | SPNIDEN_STATUS;
#elif defined(DAUTH_FULL)
    /* Set all the debug enable selector bits to 1 */
    sys_ctrl->secdbgset = All_SEL_STATUS;
    /* Set all the debug enable bits to 1 */
    sys_ctrl->secdbgset =
                   DBGEN_STATUS | NIDEN_STATUS | SPIDEN_STATUS | SPNIDEN_STATUS;
#else

#if !defined(DAUTH_CHIP_DEFAULT)
#error "No debug authentication setting is provided."
#endif

    /* Set all the debug enable selector bits to 0 */
    sys_ctrl->secdbgclr = All_SEL_STATUS;

    /* No need to set any enable bits because the value depends on
     * input signals.
     */
#endif
}

/*----------------- NVIC interrupt target state to NS configuration ----------*/
void nvic_interrupt_target_state_cfg()
{
    /* Target every interrupt to NS; unimplemented interrupts will be WI */
    for (uint8_t i=0; i<sizeof(NVIC->ITNS)/sizeof(NVIC->ITNS[0]); i++) {
        NVIC->ITNS[i] = 0xFFFFFFFF;
    }

    /* Make sure that MPC and PPC are targeted to S state */
    NVIC_ClearTargetState(SEC_VIO_IRQn);
}

/*----------------- NVIC interrupt enabling for S peripherals ----------------*/
void nvic_interrupt_enable()
{
    NVIC_EnableIRQ(SEC_VIO_IRQn);
}


/*------------------- SAU/IDAU configuration functions -----------------------*/

void sau_and_idau_cfg(void)
{
    /* Disable SAU */
    TZ_SAU_Disable();

    /* Configures SAU regions to be non-secure */
    SAU->RNR  = TFM_NS_REGION_CODE;
    SAU->RBAR = (memory_regions.non_secure_partition_base
                & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.non_secure_partition_limit
                & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;

    SAU->RNR  = TFM_NS_REGION_DATA;
    SAU->RBAR = (NS_DATA_START & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NS_DATA_LIMIT & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    /* Configures veneers region to be non-secure callable */
    SAU->RNR  = TFM_NS_REGION_VENEER;
    SAU->RBAR = (memory_regions.veneer_base  & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.veneer_limit & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk
                | SAU_RLAR_NSC_Msk;

    /* Configure the peripherals space */
    SAU->RNR  = TFM_NS_REGION_PERIPH_1;
    SAU->RBAR = (PERIPHERALS_BASE_NS_START & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (PERIPHERALS_BASE_NS_END & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;

#ifdef BL2
    /* Secondary image partition */
    SAU->RNR  = TFM_NS_SECONDARY_IMAGE_REGION;
    SAU->RBAR = (memory_regions.secondary_partition_base  & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.secondary_partition_limit & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;
#endif /* BL2 */

    /* Force memory writes before continuing */
    __DSB();
    /* Flush and refill pipeline with updated permissions */
    __ISB();

    /* Enable SAU */
    TZ_SAU_Enable();

}

/*------------------- Memory configuration functions -------------------------*/

void mpc_init_cfg(void)
{
    uint32_t mpcIdx = 0;
    uint32_t startRegionIdx = 0;
    uint32_t endRegionIdx = 0;

    /*
     * Starts changing actual configuration so issue DMB to ensure every
     * transaction has completed by now
     */
    __DMB();

    //FLASH memory configuration (all regions set to secure privilidged)
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[0] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[1] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[2] = 0x33333333U;

    //FLASH memory configuration (set according to region_defs.h and flash_layout.h)
    //The regions have to be alligned to 32 kB to cover the AHB Flash Region
    if (((NS_PARTITION_START % FLASH_AREA_IMAGE_SECTOR_SIZE) == 0)  && \
        (((NS_PARTITION_START + NS_PARTITION_SIZE) % FLASH_AREA_IMAGE_SECTOR_SIZE) == 0)) {
        startRegionIdx = NS_PARTITION_START / FLASH_AREA_IMAGE_SECTOR_SIZE;
        endRegionIdx = (NS_PARTITION_START + NS_PARTITION_SIZE) / FLASH_AREA_IMAGE_SECTOR_SIZE;

        for(mpcIdx = startRegionIdx; mpcIdx < endRegionIdx; mpcIdx++) {
            if(mpcIdx < 8) {
                //Set regions the ABH controller for flash memory 0x0000_0000 - 0x0004_0000
                AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[0] &= ~(0xF << (mpcIdx * 4));
            }
            if((mpcIdx >= 8) && (mpcIdx < 16)) {
                //Set regions in the ABH controller for flash memory 0x0004_0000 - 0x0008_0000
                AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[1] &= ~(0xF << ((mpcIdx - 8) * 4));
            }
            if((mpcIdx >= 16) && (mpcIdx < 24)) {
                //Set regions the ABH controller for flash memory 0x0008_0000 - 0x0009_8000
                AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[2] &= ~(0xF << ((mpcIdx - 16) * 4));
            }
        }
    } else {
        ERROR_MSG("Failed to set up initial MPC configuration! NS Flash regions are not alligned to 32kB. Halting.");
        while (1) {
            ;
        }
    }

    //ROM memory configuration (all secure)
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[0] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[1] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[2] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[3] = 0x33333333U;

    //SRAM memory configuration (all secure)
    AHB_SECURE_CTRL->SEC_CTRL_RAMX[0].MEM_RULE[0]= 0x33333333U;

    //RAM memory configuration
    AHB_SECURE_CTRL->SEC_CTRL_RAM0[0].MEM_RULE[0]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM0[0].MEM_RULE[1]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM1[0].MEM_RULE[0]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM1[0].MEM_RULE[1]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[0]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[1]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM3[0].MEM_RULE[0]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM3[0].MEM_RULE[1]= 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM4[0].MEM_RULE[0]= 0x33333333U;

    //RAM memory configuration (set according to region_defs.h and flash_layout.h)
    //The regions have to be alligned to 4 kB to cover the AHB RAM Region
    if (((S_DATA_SIZE % DATA_SUBREGION_SIZE) == 0)  && \
        (((S_DATA_SIZE + NS_DATA_SIZE) % DATA_SUBREGION_SIZE) == 0)) {
        startRegionIdx = S_DATA_SIZE / DATA_SUBREGION_SIZE;
        endRegionIdx = (S_DATA_SIZE + NS_DATA_SIZE) / DATA_SUBREGION_SIZE;

        for(mpcIdx = startRegionIdx; mpcIdx < endRegionIdx; mpcIdx++) {
            //Set regions the ABH controller for ram memory 0x0000_0000 - 0x0000_8000
            if(mpcIdx < 8) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM0[0].MEM_RULE[0] &= ~(0xF << (mpcIdx*4));
            }
            //Set regions the ABH controller for ram memory 0x0000_8000 - 0x0001_0000
            if((mpcIdx >= 8) && (mpcIdx < 16)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM0[0].MEM_RULE[1] &= ~(0xF << ((mpcIdx-8)*4));
            }
            //Set regions the ABH controller for ram memory 0x0001_0000 - 0x0001_8000
            if((mpcIdx >= 16) && (mpcIdx < 24)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM1[0].MEM_RULE[0] &= ~(0xF << ((mpcIdx-16)*4));
            }
            //Set regions the ABH controller for ram memory 0x0001_8000 - 0x0002_0000
            if((mpcIdx >= 24) && (mpcIdx < 32)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM1[0].MEM_RULE[1] &= ~(0xF << ((mpcIdx-24)*4));
            }
            //Set regions the ABH controller for ram memory 0x0002_0000 - 0x0002_8000
            if((mpcIdx >= 32) && (mpcIdx < 40)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[0] &= ~(0xF << ((mpcIdx-32)*4));
            }
            //Set regions the ABH controller for ram memory 0x0002_8000 - 0x0003_0000
            if((mpcIdx >= 40) && (mpcIdx < 48)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[1] &= ~(0xF << ((mpcIdx-40)*4));
            }
            //Set regions the ABH controller for ram memory 0x0003_0000 - 0x0003_8000
            if((mpcIdx >= 48) && (mpcIdx < 56)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM3[0].MEM_RULE[0] &= ~(0xF << ((mpcIdx-48)*4));
            }
            //Set regions the ABH controller for ram memory 0x0003_8000 - 0x0004_0000
            if((mpcIdx >= 56) && (mpcIdx < 64)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM3[0].MEM_RULE[1] &= ~(0xF << ((mpcIdx-56)*4));
            }
            //Set regions the ABH controller for ram memory 0x0004_0000 - 0x0004_4000
            if((mpcIdx >= 64) && (mpcIdx < 72)) {
                AHB_SECURE_CTRL->SEC_CTRL_RAM4[0].MEM_RULE[0] &= ~(0xF << ((mpcIdx-64)*4));
            }
        }
    } else {
        ERROR_MSG("Failed to set up initial MPC configuration! NS RAM regions are not alligned to 4kB. Halting.");
        while (1) {
            ;
        }
    }

    /* Add barriers to assure the MPC configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();
}

/*---------------------- PPC configuration functions -------------------------*/

void ppc_init_cfg(void)
{
    /* Secure access to Flash controller in the PPC */

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL2 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL2_FLASH_CTRL_RULE(0x3U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL2_PRINCE_RULE(0x0U);

    /* Grant non-secure access to peripherals in the PPC */

    /* APB settings for Bridge 0 */
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_SYSCON_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_IOCON_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_GINT0_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_GINT1_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_PINT_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_SEC_PINT_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_PMUX_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_CTIMER0_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_CTIMER1_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_WWDT_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_MRT_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_UTICK_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL2 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL2_ANACTRL_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL2_EFUSE_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL3 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL3_CAPTOUCH_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE0_MEM_CTRL3_EZH_RULE(0x0U);

    /* APB settings for Bridge 1 */
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL0 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL0_PMC_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL0_PVT_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL0_SYSCTRL_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_CTIMER2_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_CTIMER3_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_CTIMER4_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_RTC_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_OSEVENT_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3 =
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_USBHPHY_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_RNG_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_PUFF_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_PLU_RULE(0x0U) |
     AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_ROMPC_RULE(0x0U);

    /* AHB settings*/
    AHB_SECURE_CTRL->SEC_CTRL_AHB0_0_SLAVE_RULE =
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_0_SLAVE_RULE_DMA0_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_0_SLAVE_RULE_FS_USB_DEV_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_0_SLAVE_RULE_SCT_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_0_SLAVE_RULE_FLEXCOMM0_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_0_SLAVE_RULE_FLEXCOMM1_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_AHB0_1_SLAVE_RULE =
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_1_SLAVE_RULE_FLEXCOMM2_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_1_SLAVE_RULE_FLEXCOMM3_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_1_SLAVE_RULE_FLEXCOMM4_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_1_SLAVE_RULE_MAILBOX_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB0_1_SLAVE_RULE_GPIO0_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_AHB1_0_SLAVE_RULE =
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_0_SLAVE_RULE_USB_HS_DEV_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_0_SLAVE_RULE_CRC_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_0_SLAVE_RULE_FLEXCOMM5_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_0_SLAVE_RULE_FLEXCOMM6_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_AHB1_1_SLAVE_RULE =
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_1_SLAVE_RULE_FLEXCOMM7_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_1_SLAVE_RULE_SDIO_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_1_SLAVE_RULE_DBG_MAILBOX_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB1_1_SLAVE_RULE_HS_LSPI_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_AHB2[0].SEC_CTRL_AHB2_0_SLAVE_RULE =
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_0_SLAVE_RULE_ADC_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_0_SLAVE_RULE_USB_FS_HOST_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_0_SLAVE_RULE_USB_HS_HOST_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_0_SLAVE_RULE_HASH_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_0_SLAVE_RULE_CASPER_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_0_SLAVE_RULE_PQ_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_0_SLAVE_RULE_DMA1_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_AHB2[0].SEC_CTRL_AHB2_1_SLAVE_RULE =
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_1_SLAVE_RULE_GPIO1_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_1_SLAVE_RULE_AHB_SEC_CTRL_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_AHB2[0].SEC_CTRL_AHB2_0_MEM_RULE[0] =
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_MEM_RULE_SEC_CTRL_AHB2_0_MEM_RULE_AHB_SEC_CTRL_SECT_0_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_MEM_RULE_SEC_CTRL_AHB2_0_MEM_RULE_AHB_SEC_CTRL_SECT_1_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_MEM_RULE_SEC_CTRL_AHB2_0_MEM_RULE_AHB_SEC_CTRL_SECT_2_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_AHB2_SEC_CTRL_AHB2_MEM_RULE_SEC_CTRL_AHB2_0_MEM_RULE_AHB_SEC_CTRL_SECT_3_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_USB_HS[0].SLAVE_RULE =
       AHB_SECURE_CTRL_SEC_CTRL_USB_HS_SLAVE_RULE_RAM_USB_HS_RULE(0x0U);

    AHB_SECURE_CTRL->SEC_CTRL_USB_HS[0].MEM_RULE[0] =
       AHB_SECURE_CTRL_SEC_CTRL_USB_HS_SEC_CTRL_USB_HS_MEM_RULE_MEM_RULE_SRAM_SECT_0_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_USB_HS_SEC_CTRL_USB_HS_MEM_RULE_MEM_RULE_SRAM_SECT_1_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_USB_HS_SEC_CTRL_USB_HS_MEM_RULE_MEM_RULE_SRAM_SECT_2_RULE(0x0U) |
       AHB_SECURE_CTRL_SEC_CTRL_USB_HS_SEC_CTRL_USB_HS_MEM_RULE_MEM_RULE_SRAM_SECT_3_RULE(0x0U);

    /* Enable AHB secure controller check and lock all rule registers */
    AHB_SECURE_CTRL->MISC_CTRL_REG =
      (AHB_SECURE_CTRL->MISC_CTRL_REG & ~(AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS_MASK |
       AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_MASK)) |
       AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS(0x1U) |
       AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING(0x1U);

    AHB_SECURE_CTRL->MISC_CTRL_DP_REG =
      (AHB_SECURE_CTRL->MISC_CTRL_DP_REG & ~(AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS_MASK |
       AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_MASK |
       AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK_MASK)) |
       AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS(0x1U) |
       AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING(0x1U) |
       AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK(0x1U);

}
