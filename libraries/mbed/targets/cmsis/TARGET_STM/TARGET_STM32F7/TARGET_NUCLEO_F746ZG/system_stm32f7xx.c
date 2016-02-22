/**
  ******************************************************************************
  * @file    system_stm32f7xx.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    25-June-2015
  * @brief   CMSIS Cortex-M7 Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from 
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f7xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  * This file configures the system clock as follows:
  *-----------------------------------------------------------------------------
  * System clock source       | [1] PLL_HSE_XTAL      | [2] PLL_HSI if [1] fails
  *                           | (external 25MHz xtal) | (internal 16MHz clock)
  *-----------------------------------------------------------------------------
  * SYSCLK(MHz)               | 216                   | 216
  *-----------------------------------------------------------------------------
  * AHBCLK (MHz)              | 216                   | 216
  *-----------------------------------------------------------------------------
  * APB1CLK (MHz)             |  54                   |  54
  *-----------------------------------------------------------------------------
  * APB2CLK (MHz)             | 108                   | 108
  *-----------------------------------------------------------------------------
  * USB capable               | YES                   |  NO
  * with 48 MHz precise clock |                       |
  *-----------------------------------------------------------------------------  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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

/** @addtogroup stm32f7xx_system
  * @{
  */  
  
/** @addtogroup STM32F7xx_System_Private_Includes
  * @{
  */

#include "stm32f7xx.h"
#include "hal_tick.h"

HAL_StatusTypeDef HAL_Init(void);

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Default value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Private_Defines
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

/** @addtogroup STM32F7xx_System_Private_Macros
  * @{
  */

/* Select the clock sources (other than HSI) to start with (0=OFF, 1=ON) */
#define USE_PLL_HSE_EXTC (1) /* Use external clock */ 
#define USE_PLL_HSE_XTAL (1) /* Use external xtal */

/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Private_Variables
  * @{
  */

  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
uint32_t SystemCoreClock = HSI_VALUE;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Private_FunctionPrototypes
  * @{
  */
#if (USE_PLL_HSE_XTAL != 0) || (USE_PLL_HSE_EXTC != 0)
uint8_t SetSysClock_PLL_HSE(uint8_t bypass);
#endif

uint8_t SetSysClock_PLL_HSI(void);

/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemFrequency variable.
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
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM1_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif

  /* Configure the Cube driver */
  SystemCoreClock = HSI_VALUE; // At this stage the HSI is used as system clock
  HAL_Init();

  // Enable CPU L1-Cache
  SCB_EnableICache();
  SCB_EnableDCache();

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
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32f7xx.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32f7xx.h file (default value
  *              25 MHz), user has to ensure that HSE_VALUE is same as the real
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
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
  
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */    
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
      if (pllsource != 0)
      {
        /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);      
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK frequency */
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
      /* 3- If fail start with HSI clock */
      if (SetSysClock_PLL_HSI() == 0)
      {
        while(1)
        {
          // [TODO] Put something here to tell the user that a problem occured...
        }
      }
    }
  }
  
  // Output clock on MCO2 pin(PC9) for debugging purpose
  // Can be visualized on CN8 connector pin 4
  //HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_4); // 216 MHz / 4 = 54 MHz
}

#if (USE_PLL_HSE_XTAL != 0) || (USE_PLL_HSE_EXTC != 0)
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  // Enable power clock  
  __PWR_CLK_ENABLE();
  
  // Enable HSE oscillator and activate PLL with HSE as source
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
  if (bypass == 0)
  {
    RCC_OscInitStruct.HSEState          = RCC_HSE_ON; /* External xtal on OSC_IN/OSC_OUT */
  }
  else
  {
    RCC_OscInitStruct.HSEState          = RCC_HSE_BYPASS; /* External clock on OSC_IN */
  }
  // Warning: this configuration is for a 8 MHz xtal clock only
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM            = 8;             // VCO input clock = 1 MHz (8 MHz / 8)
  RCC_OscInitStruct.PLL.PLLN            = 432;           // VCO output clock = 432 MHz (1 MHz * 432)
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; // PLLCLK = 216 MHz (432 MHz / 2)
  RCC_OscInitStruct.PLL.PLLQ            = 9;             // USB clock = 48 MHz (432 MHz / 9) --> OK for USB
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 0; // FAIL
  }

  // Activate the OverDrive to reach the 216 MHz Frequency
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    return 0; // FAIL
  }
  
  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 216 MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 216 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           //  54 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;           // 108 MHz
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    return 0; // FAIL
  }

  return 1; // OK
}
#endif

/******************************************************************************/
/*            PLL (clocked by HSI) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSI(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  // Enable CPU L1-Cache
  SCB_EnableICache();
  SCB_EnableDCache();

  // Enable power clock  
  __PWR_CLK_ENABLE();
 
  // Enable HSI oscillator and activate PLL with HSI as source
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;   
  RCC_OscInitStruct.PLL.PLLM            = 16;            // VCO input clock = 1 MHz (16 MHz / 16)
  RCC_OscInitStruct.PLL.PLLN            = 432;           // VCO output clock = 432 MHz (1 MHz * 432)
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; // PLLCLK = 216 MHz (432 MHz / 2)
  RCC_OscInitStruct.PLL.PLLQ            = 9;             // USB clock = 48 MHz (432 MHz / 9) --> OK for USB
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 0; // FAIL
  }
 
  // Activate the OverDrive to reach the 216 MHz Frequency
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    return 0; // FAIL
  }
  
  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK; // 216 MHz
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;         // 216 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           //  54 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;           // 108 MHz

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    return 0; // FAIL
  }

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
