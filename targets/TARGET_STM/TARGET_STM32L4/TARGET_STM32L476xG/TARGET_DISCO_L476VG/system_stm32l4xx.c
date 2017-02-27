/**
  ******************************************************************************
  * @file    system_stm32l4xx.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    29-April-2016
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *   After each device reset the MSI (4 MHz) is used as system clock source.
  *   Then SystemInit() function is called, in "startup_stm32l4xx.s" file, to
  *   configure the system clock before to branch to main program.
  *
  *   This file configures the system clock as follows:
  *=============================================================================
  * System clock source                | PLL_HSE                     | PLL_HSI           | PLL_MSI
  *                                    | (external 4 to 48 MHz xtal) | (internal 16 MHz) | (internal 100kHz to 48 MHz)
  *---------------------------------------------------------------------------------------------
  * SYSCLK(MHz)                        | 48                          | 80                | 80
  *---------------------------------------------------------------------------------------------
  * AHBCLK (MHz)                       | 48                          | 80                | 80
  *---------------------------------------------------------------------------------------------
  * APB1CLK (MHz)                      | 48                          | 80                | 80
  *---------------------------------------------------------------------------------------------
  * APB2CLK (MHz)                      | 48                          | 80                | 80
  *---------------------------------------------------------------------------------------------
  * USB capable (48 MHz precise clock) | YES                         | NO                | YES
  *---------------------------------------------------------------------------------------------
  *=============================================================================
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
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
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l4xx_system
  * @{
  */

/** @addtogroup STM32L4xx_System_Private_Includes
  * @{
  */

#include "stm32l4xx.h"
#include "hal_tick.h"

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
  #define MSI_VALUE    ((uint32_t)4000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */
/******************************************************************************/
/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Macros
  * @{
  */

// Select the clock sources (default is PLL_MSI) to start with (0=OFF, 1=ON)
#define USE_PLL_HSE_EXTC (0) // Use external clock
#define USE_PLL_HSE_XTAL (0) // Use external xtal
#define USE_PLL_HSI      (0) // Use HSI/MSI internal clock (0=MSI, 1=HSI)
#define DEBUG_MCO        (0) // Output the MCO on PA8 for debugging (0=OFF, 1=SYSCLK, 2=HSE, 3=HSI, 4=MSI)
/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Variables
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
  uint32_t SystemCoreClock = 4000000;

  const uint8_t  AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
  const uint8_t  APBPrescTable[8] =  {0, 0, 0, 0, 1, 2, 3, 4};
  const uint32_t MSIRangeTable[12] = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                      4000000, 8000000, 16000000, 24000000, 32000000, 48000000};
/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_FunctionPrototypes
  * @{
  */

#if (USE_PLL_HSE_XTAL != 0) || (USE_PLL_HSE_EXTC != 0)
uint8_t SetSysClock_PLL_HSE(uint8_t bypass);
#endif

#if (USE_PLL_HSI != 0)
uint8_t SetSysClock_PLL_HSI(void);
#endif

uint8_t SetSysClock_PLL_MSI(void);

/**
  * @}
  */

/** @addtogroup STM32L4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  * @param  None
  * @retval None
  */

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC->CR |= RCC_CR_MSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , HSION, and PLLON bits */
  RCC->CR &= (uint32_t)0xEAF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00001000;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif

  /* Configure the Cube driver */
  SystemCoreClock = MSI_VALUE; // At this stage the MSI is used as system clock
  HAL_Init();

  /* Configure the System clock source, PLL Multiplier and Divider factors,
     AHB/APBx prescalers and Flash settings */
  SetSysClock();
  
  /* Reset the timer to avoid issues after the RAM initialization */
  TIM_MST_RESET_ON;
  TIM_MST_RESET_OFF;
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI_VALUE(*)
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***)
  *
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(***)
  *             or HSI_VALUE(*) or MSI_VALUE(*) multiplied/divided by the PLL factors.
  *
  *         (*) MSI_VALUE is a constant defined in stm32l4xx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSI_VALUE is a constant defined in stm32l4xx_hal.h file (default value
  *              16 MHz) but the real value may vary depending on the variations
  *              in voltage and temperature.
  *
  *         (***) HSE_VALUE is a constant defined in stm32l4xx_hal.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, msirange = 0, pllvco = 0, pllr = 2, pllsource = 0, pllm = 2;

  /* Get MSI Range frequency--------------------------------------------------*/
  if((RCC->CR & RCC_CR_MSIRGSEL) == RESET)
  { /* MSISRANGE from RCC_CSR applies */
    msirange = (RCC->CSR & RCC_CSR_MSISRANGE) >> 8;
  }
  else
  { /* MSIRANGE from RCC_CR applies */
    msirange = (RCC->CR & RCC_CR_MSIRANGE) >> 4;
  }
  /*MSI frequency range in HZ*/
  msirange = MSIRangeTable[msirange];

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
    case 0x00:  /* MSI used as system clock source */
      SystemCoreClock = msirange;
      break;

    case 0x04:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;

    case 0x08:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;

    case 0x0C:  /* PLL used as system clock  source */
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE/ PLLM) * PLLN
         SYSCLK = PLL_VCO / PLLR
         */
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
      pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> 4) + 1 ;

      switch (pllsource)
      {
        case 0x02:  /* HSI used as PLL clock source */
          pllvco = (HSI_VALUE / pllm);
          break;

        case 0x03:  /* HSE used as PLL clock source */
          pllvco = (HSE_VALUE / pllm);
          break;

        default:    /* MSI used as PLL clock source */
          pllvco = (msirange / pllm);
          break;
      }
      pllvco = pllvco * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8);
      pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 25) + 1) * 2;
      SystemCoreClock = pllvco/pllr;
      break;

    default:
      SystemCoreClock = msirange;
      break;
  }
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors,
  *               AHB/APBx prescalers and Flash settings
  * @note   This function should be called only once the RCC clock configuration  
  *         is reset to the default reset state (done in SystemInit() function).             
  * @param  None
  * @retval None
  */
void SetSysClock(void)
{
  /* 1- Try to start with HSE and external clock */
#if USE_PLL_HSE_EXTC != 0
  if (SetSysClock_PLL_HSE(1) == 0)
#endif
  {
    /* 2- If fail try to start with HSE and external xtal */
#if USE_PLL_HSE_XTAL != 0
    if (SetSysClock_PLL_HSE(0) == 0)
#endif
    {
      /* 3- If fail start with HSI or MSI clock */
#if (USE_PLL_HSI != 0)
      if (SetSysClock_PLL_HSI() == 0)
#else
      if (SetSysClock_PLL_MSI() == 0)
#endif
      {
        while(1)
        {
          // [TODO] Put something here to tell the user that a problem occured...
        }
      }
    }
  }
  
  // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 1
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
#endif
}

#if (USE_PLL_HSE_XTAL != 0) || (USE_PLL_HSE_EXTC != 0)
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  // Used to gain time after DeepSleep in case HSI is used
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET)
  {
    return 0;
  }

  // Select MSI as system clock source to allow modification of the PLL configuration
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
    
  // Enable HSE oscillator and activate PLL with HSE as source
  RCC_OscInitStruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
  if (bypass == 0)
  {
    RCC_OscInitStruct.HSEState            = RCC_HSE_ON; // External 8 MHz xtal on OSC_IN/OSC_OUT
  }
  else
  {
    RCC_OscInitStruct.HSEState            = RCC_HSE_BYPASS; // External 8 MHz clock on OSC_IN
  }
  RCC_OscInitStruct.HSIState              = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE; // 8 MHz
  RCC_OscInitStruct.PLL.PLLState          = RCC_PLL_ON;
  
  // Non-USB configuration : sysclock = 80MHz
  //RCC_OscInitStruct.PLL.PLLM              = 1; // VCO input clock = 8 MHz (8 MHz / 1)
  //RCC_OscInitStruct.PLL.PLLN              = 20; // VCO output clock = 160 MHz (8 MHz * 20)
  //RCC_OscInitStruct.PLL.PLLP              = 7; // PLLSAI3 clock = 22 MHz (160 MHz / 7)
  //RCC_OscInitStruct.PLL.PLLQ              = 4; // USB clock (PLL48M1) = 40 MHz (160 MHz / 4) --> Not good for USB
  //RCC_OscInitStruct.PLL.PLLR              = 2; // PLL clock = 80 MHz (160 MHz / 2)

  // USB configuration : sysclock = 48 MHz
  RCC_OscInitStruct.PLL.PLLM              = 1; // VCO input clock = 8 MHz (8 MHz / 1)
  RCC_OscInitStruct.PLL.PLLN              = 24; // VCO output clock = 192 MHz (8 MHz * 24)
  RCC_OscInitStruct.PLL.PLLP              = 7; // PLLSAI3 clock = 27.4 MHz (192 MHz / 7)
  RCC_OscInitStruct.PLL.PLLQ              = 4; // USB clock (PLL48M1) = 48 MHz (192 MHz / 4) --> OK for USB
  RCC_OscInitStruct.PLL.PLLR              = 4; // PLL clock = 48 MHz (192 MHz / 4)
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 0; // FAIL
  }

  // Select PLL clock as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 80 MHz or 48 MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 80 MHz or 48 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 80 MHz or 48 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 80 MHz or 48 MHz
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    return 0; // FAIL
  }

  // Disable MSI Oscillator
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState       = RCC_MSI_OFF;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // No PLL update
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 2
  if (bypass == 0)
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_2); // 4 MHz
  else
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1); // 8 MHz
#endif
  
  return 1; // OK
}
#endif

#if (USE_PLL_HSI != 0)
/******************************************************************************/
/*            PLL (clocked by HSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSI(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
 
  // Select MSI as system clock source to allow modification of the PLL configuration
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
  
  // Enable HSI oscillator and activate PLL with HSI as source
  RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState             = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState             = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource        = RCC_PLLSOURCE_HSI; // 16 MHz
  RCC_OscInitStruct.PLL.PLLM             = 2; // VCO input clock = 8 MHz (16 MHz / 2)
  RCC_OscInitStruct.PLL.PLLN             = 20; // VCO output clock = 160 MHz (8 MHz * 20)
  RCC_OscInitStruct.PLL.PLLP             = 7; // PLLSAI3 clock = 22 MHz (160 MHz / 7)
  RCC_OscInitStruct.PLL.PLLQ             = 4; // USB clock (PLL48M1) = 40 MHz (160 MHz / 4) --> Not good for USB
  RCC_OscInitStruct.PLL.PLLR             = 2; // PLL clock = 80 MHz (160 MHz / 2)
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 0; // FAIL
  }
  
  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 80 MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 80 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           // 80 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // 80 MHz
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    return 0; // FAIL
  }

  // Disable MSI Oscillator
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState       = RCC_MSI_OFF;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // No PLL update
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 3
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1); // 16 MHz
#endif
  
  return 1; // OK
}
#endif

/******************************************************************************/
/*            PLL (clocked by MSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_MSI(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  // Enable LSE Oscillator to automatically calibrate the MSI clock
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // No PLL update
  RCC_OscInitStruct.LSEState       = RCC_LSE_ON; // External 32.768 kHz clock on OSC_IN/OSC_OUT
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK) {
    RCC->CR |= RCC_CR_MSIPLLEN; // Enable MSI PLL-mode
  }

  HAL_RCCEx_DisableLSECSS();
   /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.MSIState             = RCC_MSI_ON;
  RCC_OscInitStruct.HSEState             = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState             = RCC_HSI_OFF;

  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_11; /* 48 MHz */
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM            = 6;    /* 8 MHz */
  RCC_OscInitStruct.PLL.PLLN            = 40;   /* 320 MHz */
  RCC_OscInitStruct.PLL.PLLP            = 7;    /* 45 MHz */
  RCC_OscInitStruct.PLL.PLLQ            = 4;    /* 80 MHz */
  RCC_OscInitStruct.PLL.PLLR            = 4;    /* 80 MHz */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 0; // FAIL
  }
   /* Enable MSI Auto-calibration through LSE */
  HAL_RCCEx_EnableMSIPLLMode();
  /* Select MSI output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_MSI; /* 48 MHz */
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; /* 80 MHz */
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         /* 80 MHz */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;           /* 80 MHz */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           /* 80 MHz */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    return 0; // FAIL
  }
  
  // Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 4
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_MSI, RCC_MCODIV_2); // 2 MHz
#endif
  
  return 1; // OK
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
