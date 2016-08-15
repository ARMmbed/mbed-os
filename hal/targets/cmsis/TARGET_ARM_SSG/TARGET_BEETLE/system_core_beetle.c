/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#include "CMSDK_BEETLE.h"
#include "system_core_beetle.h"

/*
 * SystemCoreConfig(): Configure the System Core
 */
void SystemCoreConfig()
{
     /* Set GPIO Alternate Functions */
     CMSDK_GPIO0->ALTFUNCSET = (1<<0); /* Sheild 0 UART 0 RXD */
     CMSDK_GPIO0->ALTFUNCSET |= (1<<1); /* Sheild 0 UART 0 TXD */
     CMSDK_GPIO0->ALTFUNCSET |= (1<<14); /* Sheild 0 I2C SDA SBCON2 */
     CMSDK_GPIO0->ALTFUNCSET |= (1<<15); /* Sheild 0 I2C SCL SBCON2 */
     CMSDK_GPIO0->ALTFUNCSET |= (1<<10); /* Sheild 0 SPI_3 nCS */
     CMSDK_GPIO0->ALTFUNCSET |= (1<<11); /* Sheild 0 SPI_3 MOSI */
     CMSDK_GPIO0->ALTFUNCSET |= (1<<12); /* Sheild 0 SPI_3 MISO */
     CMSDK_GPIO0->ALTFUNCSET |= (1<<13); /* Sheild 0 SPI_3 SCK */

     CMSDK_GPIO1->ALTFUNCSET = (1<<0); /* UART 1 RXD */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<1); /* UART 1 TXD */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<6); /* Sheild 1 I2C SDA */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<7); /* Sheild 1 I2C SCL */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<2); /* ADC SPI_2 nCS */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<3); /* ADC SPI_2 MOSI */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<4); /* ADC SPI_2 MISO */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<5); /* ADC SPI_2 SCK */

     CMSDK_GPIO1->ALTFUNCSET |= (1<<8); /* QSPI CS 2 */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<9); /* QSPI CS 1 */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<10); /* QSPI IO 0 */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<11); /* QSPI IO 1 */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<12); /* QSPI IO 2 */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<13); /* QSPI IO 3 */
     CMSDK_GPIO1->ALTFUNCSET |= (1<<14); /* QSPI SCK */

     /* Set the ARD_PWR_EN GPIO1[15] as an output */
     CMSDK_GPIO1->OUTENABLESET |= (0x1 << 15);
     /* Set on 3v3 (for ARDUINO HDR compliancy) */
     CMSDK_GPIO1->DATA |= (0x1 << 15);
}

/* POWER MANAGEMENT */

/*
 * SystemPowerConfig(): Configures the System Power Modes
 */
void SystemPowerConfig()
{
    /* Configure APB Peripheral Clock in sleep state */
    CMSDK_SYSCON->APBCLKCFG1SET = SYSTEM_CORE_TIMER0
                            | SYSTEM_CORE_TIMER1
                            | SYSTEM_CORE_DUALTIMER0
                            | SYSTEM_CORE_UART1
                            | SYSTEM_CORE_I2C0
                            | SYSTEM_CORE_QSPI
                            | SYSTEM_CORE_SPI0
                            | SYSTEM_CORE_SPI1
                            | SYSTEM_CORE_I2C1;

    /* Configure APB Peripheral Clock in deep sleep state */
    CMSDK_SYSCON->APBCLKCFG2SET = SYSTEM_CORE_TIMER0
                            | SYSTEM_CORE_TIMER1
                            | SYSTEM_CORE_DUALTIMER0
                            | SYSTEM_CORE_UART1
                            | SYSTEM_CORE_I2C0
                            | SYSTEM_CORE_QSPI
                            | SYSTEM_CORE_SPI0
                            | SYSTEM_CORE_SPI1
                            | SYSTEM_CORE_I2C1;

    /* Configure Wakeup Sources */
    CMSDK_SYSCON->PWRDNCFG1SET = SYSTEM_CORE_DUALTIMER0;
}

/*
 * SystemPowerSuspend(): Enters in System Suspend
 */
void SystemPowerSuspend(power_mode_t mode)
{
    if (mode == POWER_MODE_DEEP_SLEEP) {
        /* Enable deepsleep */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        /* Ensure effect of last store takes effect */
        __DSB();
        /* Enter sleep mode */
        __WFI();
    } else {
        /* Enter sleep mode */
        __WFI();
    }
}

/*
 * SystemPowerResume(): Returns from System Suspend
 */
void SystemPowerResume(power_mode_t mode)
{
    if (mode == POWER_MODE_DEEP_SLEEP) {
        /* Disable sleeponexit */
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
        /* Ensure effect of last store takes effect */
        __DSB();
    }
}


/*
 * System config data storage functions
 * Reserved as the data is not strictly persistent
 */

/*
 * __System_Config_GetBDAddr(): Address for the BLE device on the air.
 */
void __System_Config_GetBDAddr(uint8_t *addr, uint8_t byte_len)
{
    SystemCoreConfigData *p;
    int bank1addr = EFlash_ReturnBank1BaseAddress();

    if (byte_len > 6)
    {
        return;
    }

    if (bank1addr < 0)
    {
        memset(addr, 0xFF, byte_len);
    }
    else
    {
        /* 2x bank1 address is the top as banks have to be symmetric sizes */
        /* The data is stored at the end.*/
        p = (SystemCoreConfigData *) ((2 * bank1addr) - SYSTEM_CORE_CONFIG_DATA_SIZE);

        memcpy(addr, p->BD_ADDR, byte_len);
    }
}
