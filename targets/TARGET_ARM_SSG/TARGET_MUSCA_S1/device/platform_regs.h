/*
 * Copyright (c) 2016-2020 Arm Limited. All rights reserved.
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

#ifndef __MUSCA_S1_PLATFORM_REGS__
#define __MUSCA_S1_PLATFORM_REGS__

#include <stdint.h>
#include "platform_base_address.h"

/* sysinfo memory mapped register access structure */
struct sysinfo_t {
    volatile uint32_t sysversion;       /* (R/ ) 0x000 System version */
    volatile uint32_t sysconfig;        /* (R/ ) 0x004 System configuration */
    volatile uint32_t reserved0[1010];  /*       0x010-0xFCC Reserved */
    volatile uint32_t pidr4;            /* (R/ ) 0xFD0 Peripheral ID 4 */
    volatile uint32_t reserved1[3];     /*       0xFD4-0xFDC Reserved */
    volatile uint32_t pidr0;            /* (R/ ) 0xFE0 Peripheral ID 0 */
    volatile uint32_t pidr1;            /* (R/ ) 0xFE4 Peripheral ID 1 */
    volatile uint32_t pidr2;            /* (R/ ) 0xFE8 Peripheral ID 2 */
    volatile uint32_t pidr3;            /* (R/ ) 0xFEC Peripheral ID 3 */
    volatile uint32_t cidr0;            /* (R/ ) 0xFF0 Component ID 0 */
    volatile uint32_t cidr1;            /* (R/ ) 0xFF4 Component ID 1 */
    volatile uint32_t cidr2;            /* (R/ ) 0xFF8 Component ID 2 */
    volatile uint32_t cidr3;            /* (R/ ) 0xFFC Component ID 3 */
};

/* Secure System Control (SYSCTRL) Alias */
#define CMSDK_SYSCTRL_BASE_S    MUSCA_S1_SYSTEM_CTRL_S_BASE

/* sysctrl memory mapped register access structure */
struct sysctrl_t {
    /* (R/ ) 0x000 Secure Debug Configuration Status Register*/
    volatile uint32_t secdbgstat;
    /* ( /W) 0x004 Secure Debug Configuration Set Register */
    volatile uint32_t secdbgset;
    /* ( /W) 0x008 Secure Debug Configuration Clear Register */
    volatile uint32_t secdbgclr;
    /* (R/W) 0x00C System Control Security Control Register */
    volatile uint32_t scsecctrl;
    /* (R/W) 0x010 Fast Clock Divider Configuration Register */
    volatile uint32_t fclk_div;
    /* (R/W) 0x014 System Clock Divider Configuration Register */
    volatile uint32_t sysclk_div;
    /* (R/W) 0x018 Clock Forces */
    volatile uint32_t clockforce;
    /* 0x01C-0x0FC Reserved */
    volatile uint32_t reserved0[57];
    /* (R/W) 0x100 Reset syndrome */
    volatile uint32_t resetsyndrome;
    /* (R/W) 0x104 Reset MASK */
    volatile uint32_t resetmask;
    /* ( /W) 0x108 Software Reset */
    volatile uint32_t swreset;
    /* (R/W) 0x10C General Purpose Retention Register */
    volatile uint32_t gretreg;
    /* (R/W) 0x110 Initial Secure Reset Vector Register For CPU 0 */
    volatile uint32_t initsvtor0;
    /* (R/W) 0x114 Initial Secure Reset Vector Register For CPU 1*/
    volatile uint32_t initsvtor1;
    /* (R/W) 0x118 CPU Boot wait control after reset */
    volatile uint32_t cpuwait;
    /* (R/W) 0x11C NMI Enable */
    volatile uint32_t nmi_enable;
    /* (R/W) 0x120 CPU WIC Request and Acknowledgement */
    volatile uint32_t wicctrl;
    /* (R/W) 0x124 External Wakeup Control */
    volatile uint32_t ewctrl;
    /* 0x128-0x1FC Reserved */
    volatile uint32_t reserved2[54];
    /* (R/W) 0x200 Power Control Dependency Matrix
     * PD_SYS Power Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sys_sense;
    /* 0x204-0x208 Reserved */
    volatile uint32_t reserved3[2];
    /* (R/W) 0x20C Power Control Dependency Matrix
     * PD_SRAM0 Power Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sram0_sense;
    /* (R/W) 0x210 Power Control Dependency Matrix
     * PD_SRAM1 Power Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sram1_sense;
    /* 0x214(R/W) Power Control Dependency Matrix
     * PD_SRAM2 Power Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sram2_sense;
    /* (R/W) 0x218 Power Control Dependency Matrix
     * PD_SRAM3 Power Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sram3_sense;
    /* 0x21C-0xFCC Reserved */
    volatile uint32_t reserved4[877];
    /* (R/ ) 0xFD0 Peripheral ID 4 */
    volatile uint32_t pidr4;
    /* 0xFD4-0xFDC Reserved */
    volatile uint32_t reserved5[3];
    /* (R/ ) 0xFE0 Peripheral ID 0 */
    volatile uint32_t pidr0;
    /* (R/ ) 0xFE4 Peripheral ID 1 */
    volatile uint32_t pidr1;
    /* (R/ ) 0xFE8 Peripheral ID 2 */
    volatile uint32_t pidr2;
    /* (R/ ) 0xFEC Peripheral ID 3 */
    volatile uint32_t pidr3;
    /* (R/ ) 0xFF0 Component ID 0 */
    volatile uint32_t cidr0;
    /* (R/ ) 0xFF4 Component ID 1 */
    volatile uint32_t cidr1;
    /* (R/ ) 0xFF8 Component ID 2 */
    volatile uint32_t cidr2;
    /* (R/ ) 0xFFC Component ID 3 */
    volatile uint32_t cidr3;
};

/* Secure Privilege Control */
#define CMSDK_SPCTRL  ((struct spctrl_def*)MUSCA_S1_SPCTRL_S_BASE)

/* SPCTRL memory mapped register access structure */
struct spctrl_def {
    /* (R/W) 0x000 Secure Configuration Control Register */
    volatile uint32_t spcsecctrl;
    /* (R/W) 0x004 Bus Access wait control after reset.*/
    volatile uint32_t buswait;
    /* 0x008-0x00C Reserved */
    volatile uint32_t reserved1[2];
    /* (R/W) 0x010Security Violation Response Configuration register.*/
    volatile uint32_t secrespcfg;
    /* (R/W) 0x014 Non Secure Callable Configuration for IDAU. */
    volatile uint32_t nsccfg;
    /* 0x018 Reserved */
    volatile uint32_t reserved2;
    /* (R/W) 0x01C Secure MPC Interrupt Status. */
    volatile uint32_t secmpcintstat;
    /* (R/W) 0x020 Secure PPC Interrupt Status. */
    volatile uint32_t secppcintstat;
    /* (R/W) 0x024 Secure PPC Interrupt Clear. */
    volatile uint32_t secppcintclr;
    /* (R/W) 0x28 Secure PPC Interrupt Enable. */
    volatile uint32_t secppcinten;
    /* 0x02C-0x03C Reserved */
    volatile uint32_t reserved3[5];
    /* (R/W) 0x040 Bridge Buffer Error Interrupt Status. */
    volatile uint32_t brgintstat;
    /* (R/W) 0x044 Bridge Buffer Error Interrupt Clear. */
    volatile uint32_t brgintclr;
    /* (R/W) 0x048 Bridge Buffer Error Interrupt Enable. */
    volatile uint32_t brginten;
    /* 0x04C-0x05C Reserved */
    volatile uint32_t reserved4[5];
    /* (R/W) 0x060 Expansion 0 Non_Secure Access
     * AHB slave Peripheral Protection Control */
    volatile uint32_t ahbnsppcexp0;
    /* 0x064-0x06C Reserved */
    volatile uint32_t reserved5[3];
    /* (R/W) 0x070 Non-Secure Access
     * APB slave Peripheral Protection Control #0 */
    volatile uint32_t apbnsppc0;
    /* (R/W) 0x074 Non-Secure Access
     * APB slave Peripheral Protection Control #1 */
    volatile uint32_t apbnsppc1;
    /* 0x078-0x07C Reserved */
    volatile uint32_t reserved6[2];
    /* (R/W) 0x080 Expansion 0 Non_Secure Access
     * APB slave Peripheral Protection Control */
    volatile uint32_t apbnsppcexp0;
    /* (R/W) 0x084 Expansion 1 Non_Secure Access
     * APB slave Peripheral Protection Control */
    volatile uint32_t apbnsppcexp1;
    /* 0x088-0x08C Reserved */
    volatile uint32_t reserved7[2];
    /* (R/W) 0x090 Secure Unprivileged Access
     * AHB slave Peripheral Protection Control #0. */
    volatile uint32_t ahbspppc0;
    /* 0x094-0x09C Reserved */
    volatile uint32_t reserved8[3];
    /* (R/W) 0x0A0 Expansion 0 Secure Unprivileged Access
     * AHB slave Peripheral Protection Control. */
    volatile uint32_t ahbspppcexp0;
    /* 0x0A4-0x0AC Reserved */
    volatile uint32_t reserved9[3];
    /* (R/W) 0x0B0 Secure Unprivileged Access
     * APB slave Peripheral Protection Control #0 */
    volatile uint32_t apbspppc0;
    /* (R/W) 0x0B4 Secure Unprivileged Access
     * APB slave Peripheral Protection Control #1 */
    volatile uint32_t apbspppc1;
    /* 0x0B8-0x0BC Reserved */
    volatile uint32_t reserved10[2];
    /* (R/W) 0x0C0 Expansion 0 Secure Unprivileged Access
     * APB slave Peripheral Protection Control */
    volatile uint32_t apbspppcexp0;
    /* (R/W) 0x0C4 Expansion 1 Secure Unprivileged Access
     * APB slave Peripheral Protection Control */
    volatile uint32_t apbspppcexp1;
    /* 0x0C8-0xFCC Reserved */
    volatile uint32_t reserved11[962];
    /* (R/W) 0xFD0 Peripheral ID 4 */
    volatile uint32_t pid4;
    /* 0xFD4-0xFDC Reserved */
    volatile uint32_t reserved12[3];
    /* (R/W) 0xFE0 Peripheral ID 0 */
    volatile uint32_t pid0;
    /* (R/W) 0xFE4 Peripheral ID 1 */
    volatile uint32_t pid1;
    /* (R/W) 0xFE8 Peripheral ID 2 */
    volatile uint32_t pid2;
    /* (R/W) 0xFEC Peripheral ID 3 */
    volatile uint32_t pid3;
    /* (R/W) 0xFF0 Component ID 0 */
    volatile uint32_t cid0;
    /* (R/W) 0xFF4 Component ID 1 */
    volatile uint32_t cid1;
    /* (R/W) 0xFF8 Component ID 2 */
    volatile uint32_t cid2;
    /* (R/W) 0xFFC Component ID 3 */
    volatile uint32_t cid3;
};

/* Secure PPC interrupt position mask */
#define CMSDK_APB_PPC0_INT_POS_MASK     (1UL << 0)
#define CMSDK_APB_PPC1_INT_POS_MASK     (1UL << 1)
#define CMSDK_APB_PPCEXP0_INT_POS_MASK  (1UL << 4)
#define CMSDK_APB_PPCEXP1_INT_POS_MASK  (1UL << 5)
#define CMSDK_APB_PPCEXP2_INT_POS_MASK  (1UL << 6)
#define CMSDK_APB_PPCEXP3_INT_POS_MASK  (1UL << 7)
#define CMSDK_AHB_PPCEXP0_INT_POS_MASK  (1UL << 20)
#define CMSDK_AHB_PPCEXP1_INT_POS_MASK  (1UL << 21)
#define CMSDK_AHB_PPCEXP2_INT_POS_MASK  (1UL << 22)
#define CMSDK_AHB_PPCEXP3_INT_POS_MASK  (1UL << 23)

/* Non-Secure Privilege Control */
#define CMSDK_NSPCTRL  ((struct nspctrl_def*)MUSCA_S1_NSPCTRL_NS_BASE)

/* NSPCTRL memory mapped register access structure */
struct nspctrl_def {
    /* 0x000-0x09C Reserved */
    volatile uint32_t reserved1[40];
    /* (R/W) 0x0A0 Expansion 0 Non-Secure Unprivileged Access
     * AHB slave Peripheral Protection Control */
    volatile uint32_t ahbnspppcexp0;
    /* 0x0A4-0x0AC Reserved */
    volatile uint32_t reserved2[3];
    /* (R/W) 0x0B0 Non-Secure Unprivileged Access
     * APB slave Peripheral Protection Control #0 */
    volatile uint32_t apbnspppc0;
    /* (R/W) 0x0B4 Non-Secure Unprivileged Access
     * APB slave Peripheral Protection Control #1 */
    volatile uint32_t apbnspppc1;
    /* 0x0B8-0x0BC Reserved */
    volatile uint32_t reserved3[2];
    /* (R/W) 0x0C0 Expansion 0 Non-Secure Unprivileged Access
     * APB slave Peripheral Protection Control */
    volatile uint32_t apbnspppcexp0;
    /* (R/W) 0x0C4 Expansion 1 Non-Secure Unprivileged Access
     * APB slave Peripheral Protection Control */
    volatile uint32_t apbnspppcexp1;
    /* 0x0C8-0xFCC Reserved */
    volatile uint32_t reserved4[962];
    /* (R/W) 0xFD0 Peripheral ID 4 */
    volatile uint32_t pidr4;
    /* 0xFD4-0xFDC Reserved */
    volatile uint32_t reserved5[3];
    /* (R/W) 0xFE0 Peripheral ID 0 */
    volatile uint32_t pidr0;
    /* (R/W) 0xFE4 Peripheral ID 1 */
    volatile uint32_t pidr1;
    /* (R/W) 0xFE8 Peripheral ID 2 */
    volatile uint32_t pidr2;
    /* (R/W) 0xFEC Peripheral ID 3 */
    volatile uint32_t pidr3;
    /* (R/W) 0xFF0 Component ID 0 */
    volatile uint32_t cidr0;
    /* (R/W) 0xFF4 Component ID 1 */
    volatile uint32_t cidr1;
    /* (R/W) 0xFF8 Component ID 2 */
    volatile uint32_t cidr2;
    /* (R/W) 0xFFC Component ID 3 */
    volatile uint32_t cidr3;
};

/* ARM APB PPC0 peripherals definition */
#define CMSDK_TIMER0_APB_PPC_POS       0U
#define CMSDK_TIMER1_APB_PPC_POS       1U
#define CMSDK_DTIMER_APB_PPC_POS       2U
#define CMSDK_MHU0_APB_PPC_POS         3U
#define CMSDK_MHU1_APB_PPC_POS         4U
/* The bits 31:5 are reserved */

/* ARM APB PPC1 peripherals definition */
#define CMSDK_S32K_TIMER_PPC_POS       0U
/* The bits 31:1 are reserved */

/* ARM AHB PPC0 peripherals definition */
/* The bits 31:0 are reserved */

/* ARM AHB PPCEXP0 peripherals definition */
#define MUSCA_S1_GPIO_AHB_PPC_POS      1U
/* The bits 31:1 are reserved */

/* ARM AHB PPCEXP1 peripherals definition */
/* The bits 31:0 are reserved */

/* ARM AHB PPCEXP2 peripherals definition */
/* The bits 31:0 are reserved */

/* ARM AHB PPCEXP3 peripherals definition */
/* The bits 31:0 are reserved */

/* ARM APB PPCEXP0 peripherals definition */
#define MUSCA_S1_QSPI_MPC_APB_PPC_POS  0U
#define MUSCA_S1_SRAM_MPC_APB_PPC_POS  1U
#define MUSCA_S1_MRAM_MPC_APB_PPC_POS  2U

/* ARM APB PPCEXP1 peripherals definition */
#define MUSCA_S1_UART0_APB_PPC_POS     0U
#define MUSCA_S1_UART1_APB_PPC_POS     1U
#define MUSCA_S1_SPI_APB_PPC_POS       2U
#define MUSCA_S1_I2C0_APB_PPC_POS      3U
#define MUSCA_S1_I2C1_APB_PPC_POS      4U
#define MUSCA_S1_I2S_APB_PPC_POS       5U
#define MUSCA_S1_PWM0_APB_PPC_POS      6U
#define MUSCA_S1_RTC_APB_PPC_POS       7U
#define MUSCA_S1_PVT_APB_PPC_POS       8U
#define MUSCA_S1_QSPI_APB_PPC_POS      9U
#define MUSCA_S1_GPTIMER_APB_PPC_POS   10U
#define MUSCA_S1_SCC_APB_PPC_POS       11U
#define MUSCA_S1_PWM1_APB_PPC_POS      12U
#define MUSCA_S1_PWM2_APB_PPC_POS      13U

#endif /* __MUSCA_S1_PLATFORM_REGS__ */
