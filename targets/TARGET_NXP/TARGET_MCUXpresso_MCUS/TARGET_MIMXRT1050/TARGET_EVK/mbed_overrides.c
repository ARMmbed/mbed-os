/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "pinmap.h"

#include "fsl_clock_config.h"
#include "fsl_clock.h"

#define LPSPI_CLOCK_SOURCE_DIVIDER (7U)
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
uint8_t mbed_otp_mac_address(char *mac);
void mbed_default_mac_address(char *mac);

/* MPU configuration. */
void BOARD_ConfigMPU(void)
{
    /* Disable I cache and D cache */
    SCB_DisableICache();
    SCB_DisableDCache();

    /* Disable MPU */
    ARM_MPU_Disable();

    /* Region 0 setting */
    MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 1 setting */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 2 setting */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 3 setting */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 5 setting */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 6 setting */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

#if defined(SDRAM_MPU_INIT)
    /* Region 7 setting */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);

    /* Region 8 setting */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x81E00000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);
#endif

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

// called before main
void mbed_sdk_init()
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
}

void spi_setup_clock()
{
    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, 1U);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);
}

uint32_t spi_get_clock(void)
{
    return (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (LPSPI_CLOCK_SOURCE_DIVIDER + 1U));
}

void us_ticker_setup_clock()
{
    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
}

uint32_t us_ticker_get_clock()
{
    return CLOCK_GetFreq(kCLOCK_OscClk);
}

void serial_setup_clock(void)
{
    /* We assume default PLL and divider settings */
}

uint32_t serial_get_clock(void)
{
    uint32_t clock_freq;

    /* We assume default PLL and divider settings, and the only variable
     * from application is use PLL3 source or OSC source
     */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */ {
        clock_freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    } else {
        clock_freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return clock_freq;
}

void i2c_setup_clock()
{
    /* Select USB1 PLL (480 MHz) as master lpi2c clock source */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, 0U);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
}

uint32_t i2c_get_clock()
{
    return ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U));
}

void pwm_setup_clock()
{
    /* Use default settings */
}

uint32_t pwm_get_clock()
{
    return CLOCK_GetFreq(kCLOCK_IpgClk);
}

// Change the NMI pin to an input. This allows NMI pin to
//  be used as a low power mode wakeup.  The application will
//  need to change the pin back to NMI_b or wakeup only occurs once!
void NMI_Handler(void)
{
    pin_function(WAKEUP, 7);
    pin_mode(WAKEUP, PullDefault);
}

void mbed_mac_address(char *mac) {
    if (mbed_otp_mac_address(mac)) {
        return;
    } else {
        mbed_default_mac_address(mac);
    }
    return;
}

uint8_t mbed_otp_mac_address(char *mac) {
    /* Check if a valid MAC address is programmed to the fuse bank */
    if ((OCOTP->MAC0 != 0) &&
        (OCOTP->MAC1 != 0) &&
        (OCOTP->GP3 != 0)) {
        uint16_t MAC[3];  // 3 16 bits words for the MAC

        // Read the MAC address from the OCOTP MAC registers
        MAC[0] = (uint16_t)OCOTP->MAC0;  // most significant half-word
        MAC[1] = (uint16_t)OCOTP->MAC1;
        MAC[2] = (uint16_t)OCOTP->GP3;  // least significant half word

        // The network stack expects an array of 6 bytes
        // so we copy, and shift and copy from the half-word array to the byte array
        mac[0] = MAC[0] >> 8;
        mac[1] = MAC[0];
        mac[2] = MAC[1] >> 8;
        mac[3] = MAC[1];
        mac[4] = MAC[2] >> 8;
        mac[5] = MAC[2];
    } else {
        return 0;
    }
    return 1;
}

void mbed_default_mac_address(char *mac) {
    mac[0] = 0x00;
    mac[1] = 0x02;
    mac[2] = 0xF7;
    mac[3] = 0xF0;
    mac[4] = 0x00;
    mac[5] = 0x00;

    return;
}
