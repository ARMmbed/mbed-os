/**
  ******************************************************************************
  * @file    system_stm32wbxx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex Device Peripheral Access Layer System Source File
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32wbxx.s" file.
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
  *   Then SystemInit() function is called, in "startup_stm32wbxx.s" file, to
  *   configure the system clock before to branch to main program.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32WBxx_system
  * @{
  */

/** @addtogroup stm32WBxx_System_Private_Includes
  * @{
  */

#include "stm32wbxx.h"
#include "nvic_addr.h" // MBED patch

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)32000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
   #define MSI_VALUE    ((uint32_t)4000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#if !defined  (LSI_VALUE) 
 #define LSI_VALUE  ((uint32_t)32000)       /*!< Value of LSI in Hz*/
#endif /* LSI_VALUE */ 

#if !defined  (LSE_VALUE)
  #define LSE_VALUE    ((uint32_t)32768)    /*!< Value of LSE in Hz*/
#endif /* LSE_VALUE */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Defines
  * @{
  */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
#ifdef CORE_CM0PLUS
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET         0x0U            /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x100. */

#define VECT_TAB_BASE_ADDRESS   RAM2A_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x100. */
#else
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET         0x0U            /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */

#define VECT_TAB_BASE_ADDRESS   RAM1_BASE       /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#endif
/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Variables
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
  uint32_t SystemCoreClock  = 4000000; /*CPU1: M4 on MSI clock after startup (4MHz)*/

  const uint32_t AHBPrescTable[16] = {1, 3, 5, 1, 1, 6, 10, 32, 2, 4, 8, 16, 64, 128, 256, 512};  /* eqv. division factor used for Dory*/
                                                                                                    /* index=[0,...15]*/
  const uint32_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

  const uint32_t MSIRangeTable[12] = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                      4000000, 8000000, 16000000, 24000000, 32000000, 48000000};

  const uint32_t SmpsPrescalerTable[4][6]={{1,3,2,2,1,2}, \
                                        {2,6,4,3,2,4}, \
                                        {4,12,8,6,4,8}, \
                                        {4,12,8,6,4,8}};

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* Configure the Vector Table location add offset address ------------------*/
#ifdef CORE_CM0PLUS
#if defined(VECT_TAB_SRAM) && defined(VECT_TAB_BASE_ADDRESS)  
  /* program in SRAMx */
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAMx for CPU2 */
#else    
  /*  program in FLASH */
  SCB->VTOR = VECT_TAB_OFFSET;              /* Vector Table Relocation in Internal FLASH */
#endif  /* Program memory type */ 
#else   
#if defined(VECT_TAB_SRAM) && defined(VECT_TAB_BASE_ADDRESS)  
  /* program in SRAMx */
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET;  /* Vector Table Relocation in Internal SRAMx for CPU1 */
#else    /* program in FLASH */
  SCB->VTOR = NVIC_FLASH_VECTOR_ADDRESS; /* Vector Table Relocation in Internal FLASH - MBED patch */
#endif
#endif

  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC->CR |= RCC_CR_MSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00070000U;

  /* Reset PLLSAI1ON, PLLON, HSECSSON, HSEON, HSION, and MSIPLLON bits */
  RCC->CR &= (uint32_t)0xFAF6FEFBU;

  /*!< Reset LSI1 and LSI2 bits */
  RCC->CSR &= (uint32_t)0xFFFFFFFAU;
  
  /*!< Reset HSI48ON  bit */
  RCC->CRRCR &= (uint32_t)0xFFFFFFFEU;
    
  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x22041000U;

  /* Reset PLLSAI1CFGR register */
  RCC->PLLSAI1CFGR = 0x22041000U;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;
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
  *         (*) MSI_VALUE is a constant defined in stm32wbxx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSI_VALUE is a constant defined in stm32wbxx_hal_conf.h file (default value
  *              16 MHz) but the real value may vary depending on the variations
  *              in voltage and temperature.
  *
  *         (***) HSE_VALUE is a constant defined in stm32wbxx_hal_conf.h file (default value
  *              32 MHz), user has to ensure that HSE_VALUE is same as the real
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

   /*MSI frequency range in Hz*/
    msirange = MSIRangeTable[(RCC->CR & RCC_CR_MSIRANGE) >> RCC_CR_MSIRANGE_Pos];

  /*SystemCoreClock=HAL_RCC_GetSysClockFreq();*/
  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
    case 0x00:   /* MSI used as system clock source */
      SystemCoreClock = msirange;
      break;

    case 0x04:  /* HSI used as system clock source */
      /* HSI used as system clock source */
        SystemCoreClock = HSI_VALUE;
      break;

    case 0x08:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;

    case 0x0C: /* PLL used as system clock  source */
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE/ PLLM) * PLLN
         SYSCLK = PLL_VCO / PLLR
         */
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
      pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1 ;

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
      
      pllvco = pllvco * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos);
      pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos) + 1);
      
      SystemCoreClock = pllvco/pllr;
      break;

    default:
      SystemCoreClock = msirange;
      break;
  }
  
  /* Compute HCLK clock frequency --------------------------------------------*/
#ifdef CORE_CM0PLUS
  /* Get HCLK2 prescaler */
tmp = AHBPrescTable[((RCC->EXTCFGR & RCC_EXTCFGR_C2HPRE) >> RCC_EXTCFGR_C2HPRE_Pos)];

#else
  /* Get HCLK1 prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos)];
#endif
  /* HCLK clock frequency */
  SystemCoreClock = SystemCoreClock / tmp;

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
