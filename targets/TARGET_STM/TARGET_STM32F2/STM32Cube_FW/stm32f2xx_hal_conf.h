/**
  ******************************************************************************
  * @file    stm32f2xx_hal_conf_template.h
  * @author  MCD Application Team
  * @brief   HAL configuration template file. 
  *          This file should be copied to the application folder and renamed
  *          to stm32f2xx_hal_conf.h.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F2xx_HAL_CONF_H
#define __STM32F2xx_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver 
  */
#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
/* #define HAL_CAN_MODULE_ENABLED */
#define HAL_CAN_LEGACY_MODULE_ENABLED  // MBED
#define HAL_CRC_MODULE_ENABLED
#define HAL_CRYP_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
#define HAL_DCMI_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_ETH_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_NAND_MODULE_ENABLED
#define HAL_NOR_MODULE_ENABLED
#define HAL_PCCARD_MODULE_ENABLED
#define HAL_SRAM_MODULE_ENABLED
#define HAL_HASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#define HAL_IWDG_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_RNG_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_SD_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_USART_MODULE_ENABLED
#define HAL_IRDA_MODULE_ENABLED
#define HAL_SMARTCARD_MODULE_ENABLED
#define HAL_WWDG_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED
#define HAL_HCD_MODULE_ENABLED
#define HAL_MMC_MODULE_ENABLED

/* ########################## HSE/HSI Values adaptation ##################### */
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).  
  */
#if !defined  (HSE_VALUE) 
  #define HSE_VALUE                     8000000U       /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT               100U       /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL). 
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE                    16000000U       /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE) 
 #define LSI_VALUE                        32000U       /*!< LSI Typical Value in Hz*/
#endif /* LSI_VALUE */                                 /*!< Value of the Internal Low Speed oscillator in Hz
                                                            The real value may vary depending on the variations
                                                            in voltage and temperature.*/
/**
  * @brief External Low Speed oscillator (LSE) value.
  */
#if !defined  (LSE_VALUE)
 #define LSE_VALUE                        32768U       /*!< Value of the External Low Speed oscillator in Hz */
#endif /* LSE_VALUE */

#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT              5000U       /*!< Time out for LSE start up, in ms */
#endif /* LSE_STARTUP_TIMEOUT */

/**
  * @brief External clock source for I2S peripheral
  *        This value is used by the I2S HAL module to compute the I2S clock source 
  *        frequency, this source is inserted directly through I2S_CKIN pad. 
  */
#if !defined  (EXTERNAL_CLOCK_VALUE)
  #define EXTERNAL_CLOCK_VALUE       12288000U        /*!< Value of the Internal oscillator in Hz*/
#endif /* EXTERNAL_CLOCK_VALUE */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */     
#define  VDD_VALUE                      3300U /*!< Value of VDD in mv */
#define  TICK_INT_PRIORITY              0x0FU /*!< tick interrupt priority */
#define  USE_RTOS                          0U
#define  PREFETCH_ENABLE                   1U
#define  INSTRUCTION_CACHE_ENABLE          1U
#define  DATA_CACHE_ENABLE                 1U

#if !defined  (USE_HAL_ADC_REGISTER_CALLBACKS)
  #define  USE_HAL_ADC_REGISTER_CALLBACKS         0U /* ADC register callback disabled       */
#endif

#if !defined  (USE_HAL_CAN_REGISTER_CALLBACKS)
  #define  USE_HAL_CAN_REGISTER_CALLBACKS         0U /* CAN register callback disabled       */
#endif

#if !defined  (USE_HAL_CRYP_REGISTER_CALLBACKS)
  #define  USE_HAL_CRYP_REGISTER_CALLBACKS        0U /* CRYP register callback disabled      */
#endif

#if !defined  (USE_HAL_DAC_REGISTER_CALLBACKS)
  #define  USE_HAL_DAC_REGISTER_CALLBACKS         0U /* DAC register callback disabled       */
#endif

#if !defined  (USE_HAL_DCMI_REGISTER_CALLBACKS)
  #define  USE_HAL_DCMI_REGISTER_CALLBACKS        0U /* DCMI register callback disabled      */
#endif

#if !defined  (USE_HAL_ETH_REGISTER_CALLBACKS)
  #define  USE_HAL_ETH_REGISTER_CALLBACKS         0U /* ETH register callback disabled       */
#endif

#if !defined  (USE_HAL_HASH_REGISTER_CALLBACKS)
  #define  USE_HAL_HASH_REGISTER_CALLBACKS        0U /* HASH register callback disabled      */
#endif

#if !defined  (USE_HAL_HCD_REGISTER_CALLBACKS)
  #define  USE_HAL_HCD_REGISTER_CALLBACKS         0U /* HCD register callback disabled       */
#endif

#if !defined  (USE_HAL_I2C_REGISTER_CALLBACKS)
  #define  USE_HAL_I2C_REGISTER_CALLBACKS         0U /* I2C register callback disabled       */
#endif

#if !defined  (USE_HAL_I2S_REGISTER_CALLBACKS)
  #define  USE_HAL_I2S_REGISTER_CALLBACKS         0U /* I2S register callback disabled       */
#endif

#if !defined  (USE_HAL_MMC_REGISTER_CALLBACKS)
  #define  USE_HAL_MMC_REGISTER_CALLBACKS         0U /* MMC register callback disabled       */
#endif

#if !defined  (USE_HAL_NAND_REGISTER_CALLBACKS)
  #define  USE_HAL_NAND_REGISTER_CALLBACKS        0U /* NAND register callback disabled      */
#endif

#if !defined  (USE_HAL_NOR_REGISTER_CALLBACKS)
  #define  USE_HAL_NOR_REGISTER_CALLBACKS         0U /* NOR register callback disabled       */
#endif

#if !defined  (USE_HAL_PCCARD_REGISTER_CALLBACKS)
  #define  USE_HAL_PCCARD_REGISTER_CALLBACKS      0U /* PCCARD register callback disabled    */
#endif

#if !defined  (USE_HAL_PCD_REGISTER_CALLBACKS)
  #define  USE_HAL_PCD_REGISTER_CALLBACKS         0U /* PCD register callback disabled       */
#endif

#if !defined  (USE_HAL_RTC_REGISTER_CALLBACKS)
  #define  USE_HAL_RTC_REGISTER_CALLBACKS         0U /* RTC register callback disabled       */
#endif

#if !defined  (USE_HAL_RNG_REGISTER_CALLBACKS)
  #define  USE_HAL_RNG_REGISTER_CALLBACKS         0U /* RNG register callback disabled       */
#endif

#if !defined  (USE_HAL_SD_REGISTER_CALLBACKS)
  #define  USE_HAL_SD_REGISTER_CALLBACKS          0U /* SD register callback disabled        */
#endif

#if !defined  (USE_HAL_SMARTCARD_REGISTER_CALLBACKS)
  #define  USE_HAL_SMARTCARD_REGISTER_CALLBACKS   0U /* SMARTCARD register callback disabled */
#endif

#if !defined  (USE_HAL_IRDA_REGISTER_CALLBACKS)
  #define  USE_HAL_IRDA_REGISTER_CALLBACKS        0U /* IRDA register callback disabled      */
#endif

#if !defined  (USE_HAL_SRAM_REGISTER_CALLBACKS)
  #define  USE_HAL_SRAM_REGISTER_CALLBACKS        0U /* SRAM register callback disabled      */
#endif

#if !defined  (USE_HAL_SPI_REGISTER_CALLBACKS)
  #define  USE_HAL_SPI_REGISTER_CALLBACKS         0U /* SPI register callback disabled       */
#endif

#if !defined  (USE_HAL_TIM_REGISTER_CALLBACKS)
  #define  USE_HAL_TIM_REGISTER_CALLBACKS         0U /* TIM register callback disabled       */
#endif

#if !defined  (USE_HAL_UART_REGISTER_CALLBACKS)
  #define  USE_HAL_UART_REGISTER_CALLBACKS        0U /* UART register callback disabled      */
#endif

#if !defined  (USE_HAL_USART_REGISTER_CALLBACKS)
  #define  USE_HAL_USART_REGISTER_CALLBACKS       0U /* USART register callback disabled     */
#endif

#if !defined  (USE_HAL_WWDG_REGISTER_CALLBACKS)
  #define  USE_HAL_WWDG_REGISTER_CALLBACKS        0U /* WWDG register callback disabled      */
#endif

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* ################## Ethernet peripheral configuration ##################### */

/* Definition of the Ethernet driver buffers size and count */
#define ETH_RX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for receive               */
#define ETH_TX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for transmit              */

#ifdef MBED_CONF_STM32_EMAC_ETH_RXBUFNB
/* default value in connectivity/drivers/emac/TARGET_STM/mbed_lib.json */
#define ETH_RXBUFNB                    MBED_CONF_STM32_EMAC_ETH_RXBUFNB  /* Rx buffers of size ETH_RX_BUF_SIZE  */
#define ETH_TXBUFNB                    MBED_CONF_STM32_EMAC_ETH_TXBUFNB  /* Tx buffers of size ETH_TX_BUF_SIZE  */
#else
/* ex: bare metal profile */
#define ETH_RXBUFNB                    0  /* Rx buffers of size ETH_RX_BUF_SIZE  */
#define ETH_TXBUFNB                    0  /* Tx buffers of size ETH_TX_BUF_SIZE  */
#endif

/* Section 2: PHY configuration section */

/* PHY delay */
#ifdef MBED_CONF_STM32_EMAC_ETH_PHY_RESET_DELAY
#define PHY_RESET_DELAY                 MBED_CONF_STM32_EMAC_ETH_PHY_RESET_DELAY
#else
#define PHY_RESET_DELAY                 0
#endif
#define PHY_CONFIG_DELAY                0x00000FFFU
#define PHY_READ_TO                     0x0000FFFFU
#define PHY_WRITE_TO                    0x0000FFFFU

/* Section 3: Common PHY Registers */

#define PHY_BCR                         ((uint16_t)0x00U)    /*!< Transceiver Basic Control Register   */
#define PHY_BSR                         ((uint16_t)0x01U)    /*!< Transceiver Basic Status Register    */
 
#define PHY_RESET                       ((uint16_t)0x8000U)  /*!< PHY Reset */
#define PHY_AUTONEGOTIATION             ((uint16_t)0x1000U)  /*!< Enable auto-negotiation function     */
#define PHY_AUTONEGO_COMPLETE           ((uint16_t)0x0020U)  /*!< Auto-Negotiation process completed   */
#define PHY_LINKED_STATUS               ((uint16_t)0x0004U)  /*!< Valid link established               */
  
/* Section 4: Extended PHY Registers */
#ifdef MBED_CONF_STM32_EMAC_ETH_PHY_STATUS_REGISTER
#define PHY_SR                          MBED_CONF_STM32_EMAC_ETH_PHY_STATUS_REGISTER  /*!< PHY status register Offset                      */
#define PHY_SPEED_STATUS                MBED_CONF_STM32_EMAC_ETH_PHY_SPEED_STATUS     /*!< PHY Speed mask                                  */
#define PHY_DUPLEX_STATUS               MBED_CONF_STM32_EMAC_ETH_PHY_DUPLEX_STATUS    /*!< PHY Duplex mask                                 */
#else
#define PHY_SR                          0
#define PHY_SPEED_STATUS                0
#define PHY_DUPLEX_STATUS               0
#endif

/* ################## SPI peripheral configuration ########################## */

/* CRC FEATURE: Use to activate CRC feature inside HAL SPI Driver
* Activated: CRC code is present inside driver
* Deactivated: CRC code cleaned from driver
*/

#define USE_SPI_CRC                     1U

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file 
  */

#ifdef HAL_RCC_MODULE_ENABLED
  #include "stm32f2xx_hal_rcc.h"
#endif /* HAL_RCC_MODULE_ENABLED */

#ifdef HAL_GPIO_MODULE_ENABLED
  #include "stm32f2xx_hal_gpio.h"
#endif /* HAL_GPIO_MODULE_ENABLED */

#ifdef HAL_EXTI_MODULE_ENABLED
  #include "stm32f2xx_hal_exti.h"
#endif /* HAL_EXTI_MODULE_ENABLED */

#ifdef HAL_DMA_MODULE_ENABLED
  #include "stm32f2xx_hal_dma.h"
#endif /* HAL_DMA_MODULE_ENABLED */
   
#ifdef HAL_CORTEX_MODULE_ENABLED
  #include "stm32f2xx_hal_cortex.h"
#endif /* HAL_CORTEX_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
  #include "stm32f2xx_hal_adc.h"
#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_CAN_MODULE_ENABLED
  #include "stm32f2xx_hal_can.h"
#endif /* HAL_CAN_MODULE_ENABLED */

#ifdef HAL_CAN_LEGACY_MODULE_ENABLED
  #include "stm32f2xx_hal_can_legacy.h"
#endif /* HAL_CAN_LEGACY_MODULE_ENABLED */

#ifdef HAL_CRC_MODULE_ENABLED
  #include "stm32f2xx_hal_crc.h"
#endif /* HAL_CRC_MODULE_ENABLED */

#ifdef HAL_CRYP_MODULE_ENABLED
  #include "stm32f2xx_hal_cryp.h" 
#endif /* HAL_CRYP_MODULE_ENABLED */

#ifdef HAL_DAC_MODULE_ENABLED
  #include "stm32f2xx_hal_dac.h"
#endif /* HAL_DAC_MODULE_ENABLED */

#ifdef HAL_DCMI_MODULE_ENABLED
  #include "stm32f2xx_hal_dcmi.h"
#endif /* HAL_DCMI_MODULE_ENABLED */

#ifdef HAL_ETH_MODULE_ENABLED
  #include "stm32f2xx_hal_eth.h"
#endif /* HAL_ETH_MODULE_ENABLED */

#ifdef HAL_FLASH_MODULE_ENABLED
  #include "stm32f2xx_hal_flash.h"
#endif /* HAL_FLASH_MODULE_ENABLED */
 
#ifdef HAL_SRAM_MODULE_ENABLED
  #include "stm32f2xx_hal_sram.h"
#endif /* HAL_SRAM_MODULE_ENABLED */

#ifdef HAL_NOR_MODULE_ENABLED
  #include "stm32f2xx_hal_nor.h"
#endif /* HAL_NOR_MODULE_ENABLED */

#ifdef HAL_NAND_MODULE_ENABLED
  #include "stm32f2xx_hal_nand.h"
#endif /* HAL_NAND_MODULE_ENABLED */

#ifdef HAL_PCCARD_MODULE_ENABLED
  #include "stm32f2xx_hal_pccard.h"
#endif /* HAL_PCCARD_MODULE_ENABLED */ 

#ifdef HAL_HASH_MODULE_ENABLED
 #include "stm32f2xx_hal_hash.h"
#endif /* HAL_HASH_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
 #include "stm32f2xx_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_I2S_MODULE_ENABLED
 #include "stm32f2xx_hal_i2s.h"
#endif /* HAL_I2S_MODULE_ENABLED */

#ifdef HAL_IWDG_MODULE_ENABLED
 #include "stm32f2xx_hal_iwdg.h"
#endif /* HAL_IWDG_MODULE_ENABLED */

#ifdef HAL_PWR_MODULE_ENABLED
 #include "stm32f2xx_hal_pwr.h"
#endif /* HAL_PWR_MODULE_ENABLED */

#ifdef HAL_RNG_MODULE_ENABLED
 #include "stm32f2xx_hal_rng.h"
#endif /* HAL_RNG_MODULE_ENABLED */

#ifdef HAL_RTC_MODULE_ENABLED
 #include "stm32f2xx_hal_rtc.h"
#endif /* HAL_RTC_MODULE_ENABLED */

#ifdef HAL_SD_MODULE_ENABLED
 #include "stm32f2xx_hal_sd.h"
#endif /* HAL_SD_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
 #include "stm32f2xx_hal_spi.h"
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_TIM_MODULE_ENABLED
 #include "stm32f2xx_hal_tim.h"
#endif /* HAL_TIM_MODULE_ENABLED */

#ifdef HAL_UART_MODULE_ENABLED
 #include "stm32f2xx_hal_uart.h"
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_USART_MODULE_ENABLED
 #include "stm32f2xx_hal_usart.h"
#endif /* HAL_USART_MODULE_ENABLED */

#ifdef HAL_IRDA_MODULE_ENABLED
 #include "stm32f2xx_hal_irda.h"
#endif /* HAL_IRDA_MODULE_ENABLED */

#ifdef HAL_SMARTCARD_MODULE_ENABLED
 #include "stm32f2xx_hal_smartcard.h"
#endif /* HAL_SMARTCARD_MODULE_ENABLED */

#ifdef HAL_WWDG_MODULE_ENABLED
 #include "stm32f2xx_hal_wwdg.h"
#endif /* HAL_WWDG_MODULE_ENABLED */

#ifdef HAL_PCD_MODULE_ENABLED
 #include "stm32f2xx_hal_pcd.h"
#endif /* HAL_PCD_MODULE_ENABLED */

#ifdef HAL_HCD_MODULE_ENABLED
 #include "stm32f2xx_hal_hcd.h"
#endif /* HAL_HCD_MODULE_ENABLED */

#ifdef HAL_MMC_MODULE_ENABLED
 #include "stm32f2xx_hal_mmc.h"
#endif /* HAL_MMC_MODULE_ENABLED */
/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
#include "stm32_assert.h" // MBED patch
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F2xx_HAL_CONF_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
