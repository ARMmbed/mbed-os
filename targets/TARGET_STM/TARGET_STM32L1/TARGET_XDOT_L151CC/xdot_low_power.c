/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, MultiTech Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of MultiTech nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include "xdot_low_power.h"
#include "stdio.h"
#include "mbed_debug.h"

static uint32_t portA[6];
static uint32_t portB[6];
static uint32_t portC[6];
static uint32_t portH[6];

void xdot_disable_systick_int() {
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void xdot_enable_systick_int() {
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void xdot_save_gpio_state() {
    portA[0] = GPIOA->MODER;
    portA[1] = GPIOA->OTYPER;
    portA[2] = GPIOA->OSPEEDR;
    portA[3] = GPIOA->PUPDR;
    portA[4] = GPIOA->AFR[0];
    portA[5] = GPIOA->AFR[1];

    portB[0] = GPIOB->MODER;
    portB[1] = GPIOB->OTYPER;
    portB[2] = GPIOB->OSPEEDR;
    portB[3] = GPIOB->PUPDR;
    portB[4] = GPIOB->AFR[0];
    portB[5] = GPIOB->AFR[1];

    portC[0] = GPIOC->MODER;
    portC[1] = GPIOC->OTYPER;
    portC[2] = GPIOC->OSPEEDR;
    portC[3] = GPIOC->PUPDR;
    portC[4] = GPIOC->AFR[0];
    portC[5] = GPIOC->AFR[1];

    portH[0] = GPIOH->MODER;
    portH[1] = GPIOH->OTYPER;
    portH[2] = GPIOH->OSPEEDR;
    portH[3] = GPIOH->PUPDR;
    portH[4] = GPIOH->AFR[0];
    portH[5] = GPIOH->AFR[1];
}

void xdot_restore_gpio_state() {
    GPIOA->MODER = portA[0];
    GPIOA->OTYPER = portA[1];
    GPIOA->OSPEEDR = portA[2];
    GPIOA->PUPDR = portA[3];
    GPIOA->AFR[0] = portA[4];
    GPIOA->AFR[1] = portA[5];

    GPIOB->MODER = portB[0];
    GPIOB->OTYPER = portB[1];
    GPIOB->OSPEEDR = portB[2];
    GPIOB->PUPDR = portB[3];
    GPIOB->AFR[0] = portB[4];
    GPIOB->AFR[1] = portB[5];

    GPIOC->MODER = portC[0];
    GPIOC->OTYPER = portC[1];
    GPIOC->OSPEEDR = portC[2];
    GPIOC->PUPDR = portC[3];
    GPIOC->AFR[0] = portC[4];
    GPIOC->AFR[1] = portC[5];

    GPIOH->MODER = portH[0];
    GPIOH->OTYPER = portH[1];
    GPIOH->OSPEEDR = portH[2];
    GPIOH->PUPDR = portH[3];
    GPIOH->AFR[0] = portH[4];
    GPIOH->AFR[1] = portH[5];
}

void xdot_enter_stop_mode() {
    GPIO_InitTypeDef GPIO_InitStruct;

    // disable ADC and DAC - they can consume power in stop mode
    ADC1->CR2 &= ~ADC_CR2_ADON;
    ADC->CCR &= ~ADC_CCR_TSVREFE;
    DAC->CR &= ~DAC_CR_EN1;
    DAC->CR &= ~DAC_CR_EN2;

    // enable ULP and enable fast wakeup
    HAL_PWREx_EnableUltraLowPower();
    HAL_PWREx_EnableFastWakeUp();

    // disable HSI, MSI, and LSI if they are running
    if (RCC->CR & RCC_CR_HSION) {
        RCC->CR &= ~RCC_CR_HSION;
    }
    if (RCC->CR & RCC_CR_MSION) {
        RCC->CR &= ~RCC_CR_MSION;
    }
    if (RCC->CSR & RCC_CSR_LSION) {
        RCC->CSR &= ~RCC_CSR_LSION;
    }

    // configure USBTX & USBRX, LORA SPI, LORA_DIO, LORA_RESET, Secure Element, crystal pins, and SWD pins to analog nopull
    // the application must do the same with WAKE, GPIO*, UART1_*, I2C_*, and SPI_*

    // GPIO Ports Clock Enable
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();

    // USBTX & USBRX to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LORA_RESET to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LORA_MISO to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // LORA_SCK & LORA_MOSI to input pulldown - additional current draw if left floating
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // LORA_NSS to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LORA_DIO0 - LORA_DIO2 to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LORA_DIO3 - LORA_DIO4 to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // SE_CTRL, SE_IO, & SE_CLK to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // SE_RESET to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // SWDIO & SWCLK to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // OSC32_IN & OSC32_OUT to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // OSC_IN & OSC_OUT to analog nopull
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    // done configuring pins to analog nopull

    // make sure wakeup flag is cleared
    PWR->CR |= PWR_CR_CWUF;

    // enter stop mode - don't execute past here until woken up
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef HSERCC_OscInitStruct;
    /* Enable HSE and HSI48 oscillators and activate PLL with HSE as source */
    HSERCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
    HSERCC_OscInitStruct.HSEState = RCC_HSE_ON; /* External 24 MHz xtal on OSC_IN/OSC_OUT */
    HSERCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  // SYSCLK = 32 MHz ((24 MHz * 4) / 3)
  // USBCLK = 48 MHz ((24 MHz * 4) / 2) --> USB OK
    HSERCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    HSERCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
    HSERCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL4;
    HSERCC_OscInitStruct.PLL.PLLDIV          = RCC_PLL_DIV3;
    if (HAL_RCC_OscConfig(&HSERCC_OscInitStruct) != HAL_OK) {
        debug("OSC initialization failed - initiating soft reset\r\n");
        NVIC_SystemReset();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 32 MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 32 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 32 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 32 MHz
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        debug("PLL initialization failed - initiating soft reset\r\n");
        NVIC_SystemReset();
    }

    /* Enable the HSI for ADC peripherals */
    RCC_OscInitTypeDef HSIRCC_OscInitStruct;
    HAL_RCC_GetOscConfig(&HSIRCC_OscInitStruct);
    if ( HSIRCC_OscInitStruct.HSIState != RCC_HSI_ON ) {
        HSIRCC_OscInitStruct.HSIState = RCC_HSI_ON;
        HSIRCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
        HSIRCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
        HAL_StatusTypeDef ret = HAL_RCC_OscConfig(&HSIRCC_OscInitStruct);
        if ( ret != HAL_OK ) {
            debug("HSI initialization failed - ADC will not function properly\r\n");
        }
    }

    SystemCoreClockUpdate();

    // clear wakeup flag in power control register
    PWR->CR |= PWR_CR_CWUF;

    // enable the ADC and DAC
    ADC->CCR |= ADC_CCR_TSVREFE;
    ADC1->CR2 |= ADC_CR2_ADON;
    DAC->CR |= DAC_CR_EN1;
    DAC->CR |= DAC_CR_EN2;
}

void xdot_enter_standby_mode() {
    // enable ULP and enable fast wakeup
    HAL_PWREx_EnableUltraLowPower();
    HAL_PWREx_EnableFastWakeUp();

    // disable HSI, MSI, and LSI if they are running
    if (RCC->CR & RCC_CR_HSION)
        RCC->CR &= ~RCC_CR_HSION;
    if (RCC->CR & RCC_CR_MSION)
        RCC->CR &= ~RCC_CR_MSION;
    if (RCC->CSR & RCC_CSR_LSION)
        RCC->CSR &= ~RCC_CSR_LSION;


    // make sure wakeup and standby flags are cleared
    PWR->CR |= PWR_CR_CWUF;
    PWR->CR |= PWR_CR_CSBF;

    // enter standby mode
    HAL_PWR_EnterSTANDBYMode();
}

void xdot_enable_standby_wake_pin() {
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
}

void xdot_disable_standby_wake_pin() {
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
}

