/*
 * Copyright (c) 2018-2020 Arm Limited. All rights reserved.
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

#include "target_cfg.h"
#include "Driver_MPC.h"
#include "Driver_PPC.h"
#include "platform_description.h"
#include "device_definition.h"
#include "region_defs.h"
#include "tfm_secure_api.h"
#include "mbed_assert.h"
#include "platform_srv_impl.h"


/* Macros to pick linker symbols */
#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

/* The section names come from the scatter file */
REGION_DECLARE(Load$$LR$$, LR_NS_PARTITION, $$Base);
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

/* Import MPC driver */
extern ARM_DRIVER_MPC Driver_CODE_SRAM_MPC, Driver_EFLASH0_MPC;
extern ARM_DRIVER_MPC Driver_ISRAM0_MPC, Driver_ISRAM1_MPC;
extern ARM_DRIVER_MPC Driver_ISRAM2_MPC, Driver_ISRAM3_MPC;

/* Import PPC driver */
extern ARM_DRIVER_PPC Driver_APB_PPC0, Driver_APB_PPC1;
extern ARM_DRIVER_PPC Driver_AHB_PPCEXP0;
extern ARM_DRIVER_PPC Driver_APB_PPCEXP0, Driver_APB_PPCEXP1;

/* Define Peripherals NS address range for the platform */
#define PERIPHERALS_BASE_NS_START (0x40000000)
#define PERIPHERALS_BASE_NS_END   (0x4FFFFFFF)

/* Enable system reset request for CPU 0 */
#define ENABLE_CPU0_SYSTEM_RESET_REQUEST (1U << 4U)

/* To write into AIRCR register, 0x5FA value must be write to the VECTKEY field,
 * otherwise the processor ignores the write.
 */
#define SCB_AIRCR_WRITE_MASK ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos))

/* Debug configuration MASKS */
#define DBG_CTRL_MASK_DBGEN   (0x01 << 1)
#define DBG_CTRL_MASK_NIDEN   (0x01 << 2)
#define DBG_CTRL_MASK_SPIDEN  (0x01 << 3)
#define DBG_CTRL_MASK_SPNIDEN (0x01 << 4)

#define DBG_CTRL_ADDR         0x50089E00UL

#define All_SEL_STATUS (SPNIDEN_SEL_STATUS | SPIDEN_SEL_STATUS | \
                        NIDEN_SEL_STATUS | DBGEN_SEL_STATUS)

struct tfm_spm_partition_platform_data_t tfm_peripheral_std_uart = {
        MUSCA_B1_UART1_NS_BASE,
        MUSCA_B1_UART1_NS_BASE + 0xFFF,
        PPC_SP_DO_NOT_CONFIGURE,
        -1
};

static ARM_DRIVER_PPC *const ppc_bank_drivers[] = {
    0,                      /* AHB PPC0 */
    0,                      /* Reserved */
    0,                      /* Reserved */
    0,                      /* Reserved */
    &Driver_AHB_PPCEXP0,    /* AHB PPCEXP0 */
    0,                      /* AHB PPCEXP1 */
    0,                      /* AHB PPCEXP2 */
    0,                      /* AHB PPCEXP3 */
    &Driver_APB_PPC0,       /* APB PPC0 */
    &Driver_APB_PPC1,       /* APB PPC1 */
    0,                      /* Reserved */
    0,                      /* Reserved */
    &Driver_APB_PPCEXP0,    /* APB PPCEXP0 */
    &Driver_APB_PPCEXP1,    /* APB PPCEXP1 */
};

#define PPC_BANK_COUNT \
    (sizeof(ppc_bank_drivers)/sizeof(ppc_bank_drivers[0]))

void enable_fault_handlers(void)
{
    /* Explicitly set secure fault priority to the highest */
    NVIC_SetPriority(SecureFault_IRQn, 0);

    /* Enables BUS, MEM, USG and Secure faults */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk
                  | SCB_SHCSR_BUSFAULTENA_Msk
                  | SCB_SHCSR_MEMFAULTENA_Msk
                  | SCB_SHCSR_SECUREFAULTENA_Msk;
}

void system_reset_cfg(void)
{
    struct sysctrl_t *sysctrl = (struct sysctrl_t *)CMSDK_SYSCTRL_BASE_S;
    uint32_t reg_value = SCB->AIRCR;

    /* Enable system reset request for CPU 0, to be triggered via
     * NVIC_SystemReset function.
     */
    sysctrl->resetmask |= ENABLE_CPU0_SYSTEM_RESET_REQUEST;

    /* Clear SCB_AIRCR_VECTKEY value */
    reg_value &= ~(uint32_t)(SCB_AIRCR_VECTKEY_Msk);

    /* Enable system reset request only to the secure world */
    reg_value |= (uint32_t)(SCB_AIRCR_WRITE_MASK | SCB_AIRCR_SYSRESETREQS_Msk);

    SCB->AIRCR = reg_value;
}

void tfm_spm_hal_init_debug(void)
{

    volatile uint32_t *dbg_ctrl_p = (uint32_t*)DBG_CTRL_ADDR;

#if defined(DAUTH_NONE)

    *dbg_ctrl_p &= ~(DBG_CTRL_MASK_DBGEN |
                     DBG_CTRL_MASK_NIDEN |
                     DBG_CTRL_MASK_SPIDEN |
                     DBG_CTRL_MASK_SPNIDEN);

#elif defined(DAUTH_NS_ONLY)
    *dbg_ctrl_p &= ~(DBG_CTRL_MASK_SPIDEN |
                     DBG_CTRL_MASK_SPNIDEN);
    *dbg_ctrl_p |= DBG_CTRL_MASK_DBGEN |
                   DBG_CTRL_MASK_NIDEN;

#elif defined(DAUTH_FULL)
    *dbg_ctrl_p |= DBG_CTRL_MASK_DBGEN |
                   DBG_CTRL_MASK_NIDEN |
                   DBG_CTRL_MASK_SPIDEN |
                   DBG_CTRL_MASK_SPNIDEN;
#else

#if !defined(DAUTH_CHIP_DEFAULT)
#error "No debug authentication setting is provided."
#endif
    /* No need to set any enable bits because the value depends on
     * input signals.
     */
    (void)dbg_ctrl_p;
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
    NVIC_ClearTargetState(S_MPC_COMBINED_IRQn);
    NVIC_ClearTargetState(S_PPC_COMBINED_IRQn);
}

/*----------------- NVIC interrupt enabling for S peripherals ----------------*/
void nvic_interrupt_enable()
{
    int32_t ret = ARM_DRIVER_OK;
    /* MPC interrupt enabling */
    ret |= Driver_EFLASH0_MPC.EnableInterrupt();
    ret |= Driver_CODE_SRAM_MPC.EnableInterrupt();
    if (ret != ARM_DRIVER_OK) {
        MBED_ASSERT("MPC IRQ enable failed!");
    }
    NVIC_EnableIRQ(S_MPC_COMBINED_IRQn);

    /* PPC interrupt enabling */
    /* Clear pending PPC interrupts */
    /* In the PPC configuration function, we have used the Non-Secure
     * Privilege Control Block to grant unprivilged NS access to some
     * peripherals used by NS. That triggers a PPC0 exception as that
     * register is meant for NS privileged access only. Clear it here
     */
    Driver_APB_PPC0.ClearInterrupt();

    /* Enable PPC interrupts for APB PPC */
    ret |= Driver_APB_PPC0.EnableInterrupt();
    ret |= Driver_APB_PPC1.EnableInterrupt();
    ret |= Driver_APB_PPCEXP0.EnableInterrupt();
    ret |= Driver_APB_PPCEXP1.EnableInterrupt();
    if (ret != ARM_DRIVER_OK) {
        MBED_ASSERT("PPC IRQ enable failed!");
    }
    NVIC_EnableIRQ(S_PPC_COMBINED_IRQn);
}

/*------------------- SAU/IDAU configuration functions -----------------------*/

void sau_and_idau_cfg(void)
{
    /* Enables SAU */
    TZ_SAU_Enable();

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

    /* Secondary image partition */
    SAU->RNR  = TFM_NS_SECONDARY_IMAGE_REGION;
    SAU->RBAR = (memory_regions.secondary_partition_base  & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.secondary_partition_limit & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;

    /* Allows SAU to define the code region as a NSC */
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    spctrl->nsccfg |= NSCCFG_CODENSC;
}

/*------------------- Memory configuration functions -------------------------*/

void mpc_init_cfg(void)
{
    ARM_DRIVER_MPC* mpc_data_region0 = &Driver_ISRAM0_MPC;
    ARM_DRIVER_MPC* mpc_data_region1 = &Driver_ISRAM1_MPC;
    ARM_DRIVER_MPC* mpc_data_region2 = &Driver_ISRAM2_MPC;
    ARM_DRIVER_MPC* mpc_data_region3 = &Driver_ISRAM3_MPC;
    int32_t ret = ARM_DRIVER_OK;

    ret |= Driver_EFLASH0_MPC.Initialize();
    ret |= Driver_EFLASH0_MPC.ConfigRegion(memory_regions.non_secure_partition_base,
                                    memory_regions.non_secure_partition_limit,
                                    ARM_MPC_ATTR_NONSECURE);

    /* Secondary image region */
    ret |= Driver_EFLASH0_MPC.ConfigRegion(memory_regions.secondary_partition_base,
                                    memory_regions.secondary_partition_limit,
                                    ARM_MPC_ATTR_NONSECURE);

    ret |= mpc_data_region0->Initialize();
    ret |= mpc_data_region0->ConfigRegion(MPC_ISRAM0_RANGE_BASE_S,
                                   MPC_ISRAM0_RANGE_LIMIT_S,
                                   ARM_MPC_ATTR_SECURE);

    ret |= mpc_data_region1->Initialize();
    ret |= mpc_data_region1->ConfigRegion(MPC_ISRAM1_RANGE_BASE_S,
                                   MPC_ISRAM1_RANGE_LIMIT_S,
                                   ARM_MPC_ATTR_SECURE);

    ret |= mpc_data_region2->Initialize();
    ret |= mpc_data_region2->ConfigRegion(MPC_ISRAM2_RANGE_BASE_NS,
                                   MPC_ISRAM2_RANGE_LIMIT_NS,
                                   ARM_MPC_ATTR_NONSECURE);

    ret |= mpc_data_region3->Initialize();
    ret |= mpc_data_region3->ConfigRegion(MPC_ISRAM3_RANGE_BASE_NS,
                                   MPC_ISRAM3_RANGE_LIMIT_NS,
                                   ARM_MPC_ATTR_NONSECURE);

    if (ret != ARM_DRIVER_OK) {
        MBED_ASSERT("MPC init failed!");
    }

    /* Add barriers to assure the MPC configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();
}

void mpc_revert_non_secure_to_secure_cfg(void)
{
    ARM_DRIVER_MPC* mpc_data_region2 = &Driver_ISRAM2_MPC;
    ARM_DRIVER_MPC* mpc_data_region3 = &Driver_ISRAM3_MPC;
    int32_t ret = ARM_DRIVER_OK;

    ret |= Driver_EFLASH0_MPC.ConfigRegion(MPC_EFLASH0_RANGE_BASE_S,
                                    MPC_EFLASH0_RANGE_LIMIT_S,
                                    ARM_MPC_ATTR_SECURE);

    ret |= mpc_data_region2->ConfigRegion(MPC_ISRAM2_RANGE_BASE_S,
                                   MPC_ISRAM2_RANGE_LIMIT_S,
                                   ARM_MPC_ATTR_SECURE);

    ret |= mpc_data_region3->ConfigRegion(MPC_ISRAM3_RANGE_BASE_S,
                                   MPC_ISRAM3_RANGE_LIMIT_S,
                                   ARM_MPC_ATTR_SECURE);

    if (ret != ARM_DRIVER_OK) {
        MBED_ASSERT("MPC revert config failed!");
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
    struct spctrl_def* spctrl = CMSDK_SPCTRL;
    int32_t ret = ARM_DRIVER_OK;

    /* Grant non-secure access to peripherals in the APB PPC0
     * (timer0 and 1, dualtimer, mhu 0 and 1)
     */
    ret |= Driver_APB_PPC0.Initialize();
    ret |= Driver_APB_PPC0.ConfigPeriph(CMSDK_TIMER0_APB_PPC_POS,
                                 ARM_PPC_NONSECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPC0.ConfigPeriph(CMSDK_TIMER1_APB_PPC_POS,
                                 ARM_PPC_NONSECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPC0.ConfigPeriph(CMSDK_DTIMER_APB_PPC_POS,
                                 ARM_PPC_NONSECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPC0.ConfigPeriph(CMSDK_MHU0_APB_PPC_POS,
                                 ARM_PPC_NONSECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPC0.ConfigPeriph(CMSDK_MHU1_APB_PPC_POS,
                                 ARM_PPC_NONSECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);

    /* Grant non-secure access for APB peripherals on EXP1 */
    ret |= Driver_APB_PPCEXP1.Initialize();
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_PWM0_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_PWM1_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_PWM2_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_I2S_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_UART0_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_UART1_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_AND_NONPRIV);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_I2C0_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_I2C1_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_SPI_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_GPTIMER_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_RTC_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_PVT_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);
    ret |= Driver_APB_PPCEXP1.ConfigPeriph(MUSCA_B1_SDIO_APB_PPC_POS,
                                    ARM_PPC_NONSECURE_ONLY,
                                    ARM_PPC_PRIV_ONLY);

    if (ret != ARM_DRIVER_OK) {
        MBED_ASSERT("PPC init failed!");
    }

    /* Configure the response to a security violation as a
     * bus error instead of RAZ/WI
     */
    spctrl->secrespcfg |= 1U;
}

void ppc_configure_to_non_secure(enum ppc_bank_e bank, uint16_t pos)
{
    /* Setting NS flag for peripheral to enable NS access */
    ARM_DRIVER_PPC *ppc_driver;

    if (bank >= PPC_BANK_COUNT) {
        return;
    }

    ppc_driver = ppc_bank_drivers[bank];
    if (ppc_driver) {
        ppc_driver->ConfigPeriph(pos, ARM_PPC_NONSECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);
    }
}

void ppc_configure_to_secure(enum ppc_bank_e bank, uint16_t pos)
{
    /* Clear NS flag for peripheral to prevent NS access */
    ARM_DRIVER_PPC *ppc_driver;

    if (bank >= PPC_BANK_COUNT) {
        return;
    }

    ppc_driver = ppc_bank_drivers[bank];
    if (ppc_driver) {
        ppc_driver->ConfigPeriph(pos, ARM_PPC_SECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);
    }
}

void ppc_en_secure_unpriv(enum ppc_bank_e bank, uint16_t pos)
{
    ARM_DRIVER_PPC *ppc_driver;

    if (bank >= PPC_BANK_COUNT) {
        return;
    }

    ppc_driver = ppc_bank_drivers[bank];
    if (ppc_driver) {
        ppc_driver->ConfigPeriph(pos, ARM_PPC_SECURE_ONLY,
                                 ARM_PPC_PRIV_AND_NONPRIV);
    }
}

void ppc_clr_secure_unpriv(enum ppc_bank_e bank, uint16_t pos)
{
    ARM_DRIVER_PPC *ppc_driver;

    if (bank >= PPC_BANK_COUNT) {
        return;
    }

    ppc_driver = ppc_bank_drivers[bank];
    if (ppc_driver) {
        ppc_driver->ConfigPeriph(pos, ARM_PPC_SECURE_ONLY,
                                 ARM_PPC_PRIV_ONLY);
    }
}

void ppc_clear_irq(void)
{
    Driver_AHB_PPCEXP0.ClearInterrupt();
    Driver_APB_PPC0.ClearInterrupt();
    Driver_APB_PPC1.ClearInterrupt();
    Driver_APB_PPCEXP0.ClearInterrupt();
    Driver_APB_PPCEXP1.ClearInterrupt();
}


void mbed_psa_system_reset_impl(void)
{
    __disable_irq();
    mpc_revert_non_secure_to_secure_cfg();

    NVIC->ICPR[0] = UINT32_MAX;         /* Clear all pending interrupts */
    NVIC->ICPR[1] = UINT32_MAX;         /* Clear all pending interrupts */
    NVIC->ICPR[2] = UINT32_MAX;         /* Clear all pending interrupts */
    NVIC->ICPR[3] = UINT32_MAX;         /* Clear all pending interrupts */
    NVIC->ICPR[4] = UINT32_MAX;         /* Clear all pending interrupts */
    NVIC->ICPR[5] = UINT32_MAX;         /* Clear all pending interrupts */
    NVIC->ICPR[6] = UINT32_MAX;         /* Clear all pending interrupts */
    NVIC->ICPR[7] = UINT32_MAX;         /* Clear all pending interrupts */

    NVIC_SystemReset();

}

