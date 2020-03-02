/*
 * Copyright (c) 2016-2019 Arm Limited
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

#ifndef __MUSCA_B1_PLATFORM_REGS_H__
#define __MUSCA_B1_PLATFORM_REGS_H__

#include <stdint.h>
#include "platform_base_address.h"

/* System info memory mapped register access structure */
struct sysinfo_t {
    volatile uint32_t sysversion;       /* (R/ ) System version */
    volatile uint32_t sysconfig;        /* (R/ ) System configuration */
    volatile uint32_t reserved0[1008];
    volatile uint32_t pidr4;            /* (R/ ) Peripheral ID 4 */
    volatile uint32_t reserved1[3];
    volatile uint32_t pidr0;            /* (R/ ) Peripheral ID 0 */
    volatile uint32_t pidr1;            /* (R/ ) Peripheral ID 1 */
    volatile uint32_t pidr2;            /* (R/ ) Peripheral ID 2 */
    volatile uint32_t pidr3;            /* (R/ ) Peripheral ID 3 */
    volatile uint32_t cidr0;            /* (R/ ) Component ID 0 */
    volatile uint32_t cidr1;            /* (R/ ) Component ID 1 */
    volatile uint32_t cidr2;            /* (R/ ) Component ID 2 */
    volatile uint32_t cidr3;            /* (R/ ) Component ID 3 */
};

/* Secure System Control (SYSCTRL) alias */
#define CMSDK_SYSCTRL_BASE_S  MUSCA_B1_SYSTEM_CTRL_S_BASE

/* System control memory mapped register access structure */
struct sysctrl_t {
    volatile uint32_t secdbgstat;             /* (R/ ) Secure Debug Configuration
                                               *       Status Register*/
    volatile uint32_t secdbgset;              /* ( /W) Secure Debug Configuration
                                               *       Set Register */
    volatile uint32_t secdbgclr;              /* ( /W) Secure Debug Configuration
                                               *       Clear Register */
    volatile uint32_t scsecctrl;              /* (R/W) System Control Security
                                               *       Control Register */
    volatile uint32_t fclk_div;               /* (R/W) Fast Clock Divider
                                               *       Configuration Register */
    volatile uint32_t sysclk_div;             /* (R/W) System Clock Divider
                                               *       Configuration Register */
    volatile uint32_t clockforce;             /* (R/W) Clock Forces */
    volatile uint32_t reserved1[57];
    volatile uint32_t resetsyndrome;          /* (R/W) Reset syndrome */
    volatile uint32_t resetmask;              /* (R/W) Reset MASK */
    volatile uint32_t swreset;                /* ( /W) Software Reset */
    volatile uint32_t gretreg;                /* (R/W) General Purpose Retention
                                               *       Register */
    volatile uint32_t initsvtor0;             /* (R/W) Initial Secure Reset Vector
                                               *       Register For CPU 0 */
    volatile uint32_t initsvtor1;             /* (R/W) Initial Secure Reset
                                               *       Vector Register For CPU 1*/
    volatile uint32_t cpuwait;                /* (R/W) CPU Boot wait control
                                               *       after reset */
    volatile uint32_t nmi_enable;             /* (R/W) NMI Enable Register.*/
    volatile uint32_t wicctrl;                /* (R/W) CPU WIC Request and
                                               *       Acknowledgement */
    volatile uint32_t ewctrl;                 /* (R/W) External Wakeup Control */
    volatile uint32_t reserved2[54];
    volatile uint32_t pdcm_pd_sys_sense;      /* (R/W) Power Control Dependency
                                               *       Matrix PD_SYS
                                               *       Power Domain Sensitivity.*/
    volatile uint32_t reserved3[2];
    volatile uint32_t pdcm_pd_sram0_sense;    /* (R/W) Power Control Dependency
                                               *       Matrix PD_SRAM0 Power
                                               *       Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sram1_sense;    /* (R/W) Power Control Dependency
                                               *       Matrix PD_SRAM1 Power
                                               *       Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sram2_sense;    /* (R/W) Power Control Dependency
                                               *       Matrix PD_SRAM2 Power
                                               *       Domain Sensitivity.*/
    volatile uint32_t pdcm_pd_sram3_sense;    /* (R/W) Power Control Dependency
                                               *       Matrix PD_SRAM3 Power
                                               *       Domain Sensitivity.*/
    volatile uint32_t reserved4[877];
    volatile uint32_t pidr4;                  /* (R/ ) Peripheral ID 4 */
    volatile uint32_t reserved5[3];
    volatile uint32_t pidr0;                  /* (R/ ) Peripheral ID 0 */
    volatile uint32_t pidr1;                  /* (R/ ) Peripheral ID 1 */
    volatile uint32_t pidr2;                  /* (R/ ) Peripheral ID 2 */
    volatile uint32_t pidr3;                  /* (R/ ) Peripheral ID 3 */
    volatile uint32_t cidr0;                  /* (R/ ) Component ID 0 */
    volatile uint32_t cidr1;                  /* (R/ ) Component ID 1 */
    volatile uint32_t cidr2;                  /* (R/ ) Component ID 2 */
    volatile uint32_t cidr3;                  /* (R/ ) Component ID 3 */
};

/* Secure Privilege Control */
#define CMSDK_SPCTRL  ((struct spctrl_def*)MUSCA_B1_SPCTRL_S_BASE)

/* SPCTRL memory mapped register access structure */
struct spctrl_def {
    volatile uint32_t spcsecctrl;             /* (R/W) Secure Configuration Control
                                                       Register */
    volatile uint32_t buswait;                /* (R/W) Bus Access wait control after reset.*/
    volatile uint32_t reserved[2];
    volatile uint32_t secrespcfg;             /* (R/W) Security Violation Response
                                               *       Configuration register.*/
    volatile uint32_t nsccfg;                 /* (R/W) Non Secure Callable Configuration
                                               *       for IDAU. */
    volatile uint32_t reserved2;
    volatile uint32_t secmpcintstat;          /* (R/W) Secure MPC Interrupt Status. */
    volatile uint32_t secppcintstat;          /* (R/W) Secure PPC Interrupt Status. */
    volatile uint32_t secppcintclr;           /* (R/W) Secure PPC Interrupt Clear. */
    volatile uint32_t secppcinten;            /* (R/W) Secure PPC Interrupt Enable. */
    volatile uint32_t reserved3;
    volatile uint32_t secmscintstat;          /* (R/W) Secure MSC Interrupt Status. */
    volatile uint32_t secmscintclr;           /* (R/W) Secure MSC Interrupt Clear. */
    volatile uint32_t secmscinten;            /* (R/W) Secure MSC Interrupt Enable. */
    volatile uint32_t reserved4;
    volatile uint32_t brgintstat;             /* (R/W) Bridge Buffer Error Interrupt Status. */
    volatile uint32_t brgintclr;              /* (R/W) Bridge Buffer Error Interrupt Clear. */
    volatile uint32_t brginten;               /* (R/W) Bridge Buffer Error Interrupt Enable. */
    volatile uint32_t reserved5;
    volatile uint32_t ahbnsppc0;              /* (R/W) Non-Secure Access AHB slave Peripheral
                                               *       Protection Control #0 */
    volatile uint32_t reserved6[3];
    volatile uint32_t ahbnsppcexp0;           /* (R/W) Expansion 0 Non_Secure Access AHB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t ahbnsppcexp1;           /* (R/W) Expansion 1 Non_Secure Access AHB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t ahbnsppcexp2;           /* (R/W) Expansion 2 Non_Secure Access AHB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t ahbnsppcexp3;           /* (R/W) Expansion 3 Non_Secure Access AHB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t apbnsppc0;              /* (R/W) Non-Secure Access APB slave Peripheral
                                               *       Protection Control #0 */
    volatile uint32_t apbnsppc1;              /* (R/W) Non-Secure Access APB slave Peripheral
                                               *       Protection Control #1 */
    volatile uint32_t reserved7[2];
    volatile uint32_t apbnsppcexp0;           /* (R/W) Expansion 0 Non_Secure Access APB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t apbnsppcexp1;           /* (R/W) Expansion 1 Non_Secure Access APB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t apbnsppcexp2;           /* (R/W) Expansion 2 Non_Secure Access APB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t apbnsppcexp3;           /* (R/W) Expansion 3 Non_Secure Access APB slave
                                               *       Peripheral Protection Control */
    volatile uint32_t ahbspppc0;              /* (R/W) Secure Unprivileged Access AHB slave
                                               *       Peripheral Protection Control #0. */
    volatile uint32_t reserved8[3];
    volatile uint32_t ahbspppcexp0;           /* (R/W) Expansion 0 Secure Unprivileged Access
                                               *       AHB slave Peripheral Protection Control. */
    volatile uint32_t ahbspppcexp1;           /* (R/W) Expansion 1 Secure Unprivileged Access
                                               *       AHB slave Peripheral Protection Control. */
    volatile uint32_t ahbspppcexp2;           /* (R/W) Expansion 2 Secure Unprivileged Access
                                               *       AHB slave Peripheral Protection Control. */
    volatile uint32_t ahbspppcexp3;           /* (R/W) Expansion 3 Secure Unprivileged Access
                                               *       AHB slave Peripheral Protection Control. */
    volatile uint32_t apbspppc0;              /* (R/W) Secure Unprivileged Access APB slave
                                               *       Peripheral Protection Control #0 */
    volatile uint32_t apbspppc1;              /* (R/W) Secure Unprivileged Access APB slave
                                               *       Peripheral Protection Control #1 */
    volatile uint32_t reserved9[2];
    volatile uint32_t apbspppcexp0;           /* (R/W) Expansion 0 Secure Unprivileged Access
                                               *       APB slave Peripheral Protection Control */
    volatile uint32_t apbspppcexp1;           /* (R/W) Expansion 1 Secure Unprivileged Access
                                               *       APB slave Peripheral Protection Control */
    volatile uint32_t apbspppcexp2;           /* (R/W) Expansion 2 Secure Unprivileged Access
                                               *       APB slave Peripheral Protection Control */
    volatile uint32_t apbspppcexp3;           /* (R/W) Expansion 3 Secure Unprivileged Access
                                               *       APB slave Peripheral Protection Control */
    volatile uint32_t nsmscexp;               /* (R/W) Expansion MSC Non-Secure Configuration */
    volatile uint32_t reserved10[959];
    volatile uint32_t pidr4;                  /* (R/ ) Peripheral ID 4 */
    volatile uint32_t reserved11[3];
    volatile uint32_t pidr0;                  /* (R/ ) Peripheral ID 0 */
    volatile uint32_t pidr1;                  /* (R/ ) Peripheral ID 1 */
    volatile uint32_t pidr2;                  /* (R/ ) Peripheral ID 2 */
    volatile uint32_t pidr3;                  /* (R/ ) Peripheral ID 3 */
    volatile uint32_t cidr0;                  /* (R/ ) Component ID 0 */
    volatile uint32_t cidr1;                  /* (R/ ) Component ID 1 */
    volatile uint32_t cidr2;                  /* (R/ ) Component ID 2 */
    volatile uint32_t cidr3;                  /* (R/ ) Component ID 3 */

};

/* PPC interrupt position mask */
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
#define CMSDK_NSPCTRL  ((struct nspctrl_def*)MUSCA_B1_NSPCTRL_NS_BASE)

/* NSPCTRL memory mapped register access structure */
struct nspctrl_def {
    volatile uint32_t reserved[36];
    volatile uint32_t ahbnspppc0;              /* (R/W) Non-Secure Unprivileged Access AHB slave
                                                *       Peripheral Protection Control #0 */
    volatile uint32_t reserved3[3];
    volatile uint32_t ahbnspppcexp0;           /* (R/W) Expansion 0 Non-Secure Unprivileged Access
                                                *       AHB slave Peripheral Protection Control */
    volatile uint32_t ahbnspppcexp1;           /* (R/W) Expansion 1 Non-Secure Unprivileged Access
                                                *       AHB slave Peripheral Protection Control */
    volatile uint32_t ahbnspppcexp2;           /* (R/W) Expansion 2 Non-Secure Unprivileged Access
                                                *       AHB slave Peripheral Protection Control */
    volatile uint32_t ahbnspppcexp3;           /* (R/W) Expansion 3 Non-Secure Unprivileged Access
                                                *       AHB slave Peripheral Protection Control */
    volatile uint32_t apbnspppc0;              /* (R/W) Non-Secure Unprivileged Access APB slave
                                                *       Peripheral Protection Control #0 */
    volatile uint32_t apbnspppc1;              /* (R/W) Non-Secure Unprivileged Access APB slave
                                                *       Peripheral Protection Control #1 */
    volatile uint32_t reserved4[2];
    volatile uint32_t apbnspppcexp0;           /* (R/W) Expansion 0 Non-Secure Unprivileged Access
                                                *       APB slave Peripheral Protection Control */
    volatile uint32_t apbnspppcexp1;           /* (R/W) Expansion 1 Non-Secure Unprivileged Access
                                                *       APB slave Peripheral Protection Control */
    volatile uint32_t apbnspppcexp2;           /* (R/W) Expansion 2 Non-Secure Unprivileged Access
                                                *       APB slave Peripheral Protection Control */
    volatile uint32_t apbnspppcexp3;           /* (R/W) Expansion 3 Non-Secure Unprivileged Access
                                                *       APB slave Peripheral Protection Control */
    volatile uint32_t reserved5[960];
    volatile uint32_t pidr4;                   /* (R/W) Peripheral ID 3 */
    volatile uint32_t reserved7[3];
    volatile uint32_t pidr0;                   /* (R/W) Peripheral ID 0 */
    volatile uint32_t pidr1;                   /* (R/W) Peripheral ID 1 */
    volatile uint32_t pidr2;                   /* (R/W) Peripheral ID 2 */
    volatile uint32_t pidr3;                   /* (R/W) Peripheral ID 3 */
    volatile uint32_t cidr0;                   /* (R/W) Component ID 0 */
    volatile uint32_t cidr1;                   /* (R/W) Component ID 1 */
    volatile uint32_t cidr2;                   /* (R/W) Component ID 2 */
    volatile uint32_t cidr3;                   /* (R/W) Component ID 3 */
};

/* SSE-200 specific PPC bit definitions */

/* ARM APB PPC0 peripherals definition */
#define CMSDK_TIMER0_APB_PPC_POS             0U
#define CMSDK_TIMER1_APB_PPC_POS             1U
#define CMSDK_DTIMER_APB_PPC_POS             2U
#define CMSDK_MHU0_APB_PPC_POS               3U
#define CMSDK_MHU1_APB_PPC_POS               4U
/* The bits 31:5 are reserved */
/* End ARM APB PPC0 peripherals definition */

/* ARM APB PPC1 peripherals definition */
#define CMSDK_S32K_TIMER_PPC_POS             0U
/* The bits 31:1 are reserved */
/* End ARM APB PPC1 peripherals definition */

/* ARM AHB PPC0 peripherals definition */
/* The bits 31:0 are reserved */
/* End of ARM AHB PPC0 peripherals definition */

/* Musca B1 specific PPC bit definitions */

/* ARM AHB PPCEXP0 peripherals definition */
/* Bit 0 is reserved */
#define MUSCA_B1_GPIO_AHB_PPC_POS            1U
/* The bits 31:2 are reserved */
/* End of ARM AHB PPCEXP0 peripherals definition */

/* ARM AHB PPCEXP1 peripherals definition */
/* The bits 31:0 are reserved */
/* End of ARM AHB PPCEXP1 peripherals definition */

/* ARM AHB PPCEXP2 peripherals definition */
/* The bits 31:0 are reserved */
/* End of ARM AHB PPCEXP2 peripherals definition */

/* ARM AHB PPCEXP3 peripherals definition */
/* The bits 31:0 are reserved */
/* End of ARM AHB PPCEXP3 peripherals definition */

/* ARM APB PPCEXP0 peripherals definition */
#define MUSCA_B1_EFLASH0_CTRL_APB_PPC_POS    0U
#define MUSCA_B1_EFLASH1_CTRL_APB_PPC_POS    1U
#define MUSCA_B1_QSPI_APB_PPC_POS            2U
#define MUSCA_B1_EFLASH0_MPC_APB_PPC_POS     3U
#define MUSCA_B1_EFLASH1_MPC_APB_PPC_POS     4U
#define MUSCA_B1_SRAM_MPC_APB_PPC_POS        5U
#define MUSCA_B1_QSPI_MPC_APB_PPC_POS        6U
#define MUSCA_B1_CI_MHU0_S_APB_PPC_POS       7U
#define MUSCA_B1_CI_MHU0_R_APB_PPC_POS       8U
/* The bits 13:9 are reserved. */
#define MUSCA_B1_CI_MPC_APB_PPC_POS         14U
/* The bits 31:15 are reserved */
/* End of ARM APB PPCEXP0 peripherals definition */

/* ARM APB PPCEXP1 peripherals definition */
/* Bit 0 is reserved. */
#define MUSCA_B1_PWM0_APB_PPC_POS            1U
#define MUSCA_B1_PWM1_APB_PPC_POS            2U
#define MUSCA_B1_PWM2_APB_PPC_POS            3U
#define MUSCA_B1_I2S_APB_PPC_POS             4U
#define MUSCA_B1_UART0_APB_PPC_POS           5U
#define MUSCA_B1_UART1_APB_PPC_POS           6U
/* Bit 7 is reserved. */
#define MUSCA_B1_I2C0_APB_PPC_POS            8U
#define MUSCA_B1_I2C1_APB_PPC_POS            9U
#define MUSCA_B1_SPI_APB_PPC_POS            10U
#define MUSCA_B1_SCC_APB_PPC_POS            11U
#define MUSCA_B1_GPTIMER_APB_PPC_POS        12U
#define MUSCA_B1_RTC_APB_PPC_POS            13U
#define MUSCA_B1_PVT_APB_PPC_POS            14U
#define MUSCA_B1_SDIO_APB_PPC_POS           15U
/* The bits 31:16 are reserved */
/* End of ARM APB PPCEXP1 peripherals definition */

#endif /* __MUSCA_B1_PLATFORM_REGS_H__ */
