/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
 *
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
#include "cmsis.h"
#include "partition_M2351.h"

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#if TFM_LVL > 0

#include "tfm_secure_api.h"
#include "platform/mbed_assert.h"

/* The configuration of TDB internal storage area defined in "partition_M2351_mem.h"
 * must match "tdb_internal/mbed_lib.json", so it can pass to linker files for
 * memory layout check. */
MBED_STATIC_ASSERT(NU_TDB_INTERNAL_STORAGE_START == MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS,
    "NU_TDB_INTERNAL_STORAGE_START must be equal to MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS");
MBED_STATIC_ASSERT(NU_TDB_INTERNAL_STORAGE_SIZE == MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE,
    "NU_TDB_INTERNAL_STORAGE_SIZE must be equal to MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE");

#endif

void FMC_NSBA_Setup(void)
{
    /* Skip NSBA Setupt according config */
    if(FMC_INIT_NSBA == 0)
        return;

    /* Check if NSBA value with current active NSBA */
    if(SCU->FNSADDR != FMC_SECURE_ROM_SIZE)
    {
        /* Unlock Protected Register */
        SYS_UnlockReg();

        /* Enable ISP and config update */
        FMC->ISPCTL = FMC_ISPCTL_ISPEN_Msk | FMC_ISPCTL_CFGUEN_Msk;

        /* Config Base of NSBA */
        FMC->ISPADDR = 0x200800;

        /* Read Non-secure base address config */
        FMC->ISPCMD = FMC_ISPCMD_READ;
        FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
        while(FMC->ISPTRG);

        /* Setting NSBA when it is empty */
        if(FMC->ISPDAT == 0xfffffffful)
        {
            FMC->ISPDAT = FMC_SECURE_ROM_SIZE;
            FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
            FMC->ISPTRG = FMC_ISPTRG_ISPGO_Msk;
            while(FMC->ISPTRG);

            /* Force Chip Reset to valid new setting */
            SYS->IPRST0 = SYS_IPRST0_CHIPRST_Msk;
        }

        /* Fatal Error:
           FMC NSBA setting is different to FMC_INIT_NSBA_VAL.
           User must double confirm which one is wrong.

           If user need to change NSBA config of FMC, user must do Mess-erase by
           ISP or ICP.
        */
        while(1);
    }

}

void SCU_Setup(void)
{
    int32_t i;

    SCU->PNSSET[0] = SCU_INIT_PNSSET0_VAL;
    SCU->PNSSET[1] = SCU_INIT_PNSSET1_VAL;
    SCU->PNSSET[2] = SCU_INIT_PNSSET2_VAL;
    SCU->PNSSET[3] = SCU_INIT_PNSSET3_VAL;
    SCU->PNSSET[4] = SCU_INIT_PNSSET4_VAL;
    SCU->PNSSET[5] = SCU_INIT_PNSSET5_VAL;
    SCU->PNSSET[6] = SCU_INIT_PNSSET6_VAL;

    SCU->IONSSET = SCU_INIT_IONSSET_VAL;

    /* Set Non-secure SRAM */
    for(i = 11; i >= SCU_SECURE_SRAM_SIZE / 8192; i--)
    {
        SCU->SRAMNSSET |= (1U << i);
    }
}

void TZ_SAU_Setup(void)
{
#if defined (__SAU_PRESENT) && (__SAU_PRESENT == 1U)

#if defined (SAU_INIT_REGION0) && (SAU_INIT_REGION0 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(0);
#else
    #error("TFM doesn't support SAU_INIT_REGION0")
#endif
#endif

#if defined (SAU_INIT_REGION1) && (SAU_INIT_REGION1 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(1);
#else
    #error("TFM doesn't support SAU_INIT_REGION1")
#endif
#endif

#if defined (SAU_INIT_REGION2) && (SAU_INIT_REGION2 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(2);
#else
    #error("TFM doesn't support SAU_INIT_REGION2")
#endif
#endif

#if defined (SAU_INIT_REGION3) && (SAU_INIT_REGION3 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(3);
#else
    SAU_INIT_REGION(3, TFM_NS_REGION_VENEER);
#endif
#endif

#if defined (SAU_INIT_REGION4) && (SAU_INIT_REGION4 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(4);
#else
    SAU_INIT_REGION(4, TFM_NS_REGION_CODE);
#endif
#endif

#if defined (SAU_INIT_REGION5) && (SAU_INIT_REGION5 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(5);
#else
    #warning("TFM doesn't support SAU_INIT_REGION5")
#endif
#endif

#if defined (SAU_INIT_REGION6) && (SAU_INIT_REGION6 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(6);
#else
    SAU_INIT_REGION(6, TFM_NS_REGION_DATA);
#endif
#endif

#if defined (SAU_INIT_REGION7) && (SAU_INIT_REGION7 == 1U)
#if TFM_LVL == 0
    SAU_INIT_REGION(7);
#else
    SAU_INIT_REGION(7, TFM_NS_REGION_PERIPH_1);
#endif
#endif

    /* repeat this for all possible SAU regions */

#if defined (SAU_INIT_CTRL) && (SAU_INIT_CTRL == 1U)
    SAU->CTRL = ((SAU_INIT_CTRL_ENABLE << SAU_CTRL_ENABLE_Pos) & SAU_CTRL_ENABLE_Msk) |
                ((SAU_INIT_CTRL_ALLNS  << SAU_CTRL_ALLNS_Pos)  & SAU_CTRL_ALLNS_Msk)   ;
#endif

#endif /* defined (__SAU_PRESENT) && (__SAU_PRESENT == 1U) */
}

void SCB_Setup(void)
{
#if defined (SCB_CSR_AIRCR_INIT) && (SCB_CSR_AIRCR_INIT == 1U)
    SCB->SCR   = (SCB->SCR   & ~(SCB_SCR_SLEEPDEEPS_Msk)) |
                 ((SCB_CSR_DEEPSLEEPS_VAL     << SCB_SCR_SLEEPDEEPS_Pos)     & SCB_SCR_SLEEPDEEPS_Msk);

//    SCB->AIRCR = (SCB->AIRCR & ~(SCB_AIRCR_SYSRESETREQS_Msk | SCB_AIRCR_BFHFNMINS_Msk |  SCB_AIRCR_PRIS_Msk)) |
//                 ((SCB_AIRCR_SYSRESETREQS_VAL << SCB_AIRCR_SYSRESETREQS_Pos) & SCB_AIRCR_SYSRESETREQS_Msk) |
//                 ((SCB_AIRCR_BFHFNMINS_VAL    << SCB_AIRCR_BFHFNMINS_Pos)    & SCB_AIRCR_BFHFNMINS_Msk)    |
//                 ((SCB_AIRCR_PRIS_VAL         << SCB_AIRCR_PRIS_Pos)         & SCB_AIRCR_PRIS_Msk);

    SCB->AIRCR = (0x05FA << 16) |
                 ((SCB_AIRCR_SYSRESETREQS_VAL << SCB_AIRCR_SYSRESETREQS_Pos) & SCB_AIRCR_SYSRESETREQS_Msk) |
                 ((SCB_AIRCR_BFHFNMINS_VAL    << SCB_AIRCR_BFHFNMINS_Pos)    & SCB_AIRCR_BFHFNMINS_Msk)    |
                 ((SCB_AIRCR_PRIS_VAL         << SCB_AIRCR_PRIS_Pos)         & SCB_AIRCR_PRIS_Msk);



#endif /* defined (SCB_CSR_AIRCR_INIT) && (SCB_CSR_AIRCR_INIT == 1U) */

#if defined (SCB_ICSR_INIT) && (SCB_ICSR_INIT == 1U)
    SCB->ICSR  = (SCB->ICSR  & ~(SCB_ICSR_STTNS_Msk)) |
                 ((SCB_ICSR_STTNS_VAL         << SCB_ICSR_STTNS_Pos)         & SCB_ICSR_STTNS_Msk);
#endif /* defined (SCB_ICSR_INIT) && (SCB_ICSR_INIT == 1U) */
}

void TZ_NVIC_Setup(void)
{
#if defined (NVIC_INIT_ITNS0) && (NVIC_INIT_ITNS0 == 1U)
    NVIC->ITNS[0] = NVIC_INIT_ITNS0_VAL;
#endif

#if defined (NVIC_INIT_ITNS1) && (NVIC_INIT_ITNS1 == 1U)
    NVIC->ITNS[1] = NVIC_INIT_ITNS1_VAL;
#endif

#if defined (NVIC_INIT_ITNS2) && (NVIC_INIT_ITNS2 == 1U)
    NVIC->ITNS[2] = NVIC_INIT_ITNS2_VAL;
#endif

#if defined (NVIC_INIT_ITNS3) && (NVIC_INIT_ITNS3 == 1U)
    NVIC->ITNS[3] = NVIC_INIT_ITNS3_VAL;
#endif

    /* repeat this for all possible ITNS elements */
}

#endif  /* #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
