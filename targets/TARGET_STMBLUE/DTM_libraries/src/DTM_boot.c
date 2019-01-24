/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : DTM_boot.c
* Author             : AMS - VMA RF Application Team
* Version            : V1.0.0
* Date               : 25-December-2015
* Description        : BlueNRG-1 DTM boot routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "BlueNRG1_conf.h"
#include "bluenrg1_stack.h"
#include "DTM_config.h"
#include "miscutil.h"
#include "DTM_boot.h"

/* Private typedef -----------------------------------------------------------*/
typedef  PACKED(struct) devConfigS  {
  uint8_t  HS_crystal;
  uint8_t  LS_source;
  uint8_t  SMPS_management;
  uint8_t  Reserved;
  uint16_t HS_startup_time;
  uint16_t SlaveSCA;
  uint8_t  MasterSCA;
  uint32_t max_conn_event_length;
  uint8_t  Test_mode;
} devConfig_t;

/* Private define ------------------------------------------------------------*/

/**
 * @name Device Configuration Registers
 *@{
 */

/**
 *@brief Analog Test Bus 0 register settings
 */
#define ATB0_ANA_ENG_REG    0x3F
/**
 *@brief Analog Test Bus 1 register settings
 */
#define ATB1_ANA_ENG_REG    0x3E
/**
 *@brief Rate Multiplier 1 register settings 
 */
#define RM1_DIG_ENG_REG     0x3C
/**
 *@brief Low Frequency Clock and SMPS register settings
 */
#define CLOCK_LOW_ENG_REG   0x3B
/**
 *@brief High Frequency Clock register settings
 */
#define CLOCK_HIGH_ENG_REG  0x3A
/**
 *@brief Power Management register settings
 */
#define PMU_ANA_ENG_REG     0x39
/**
 *@brief System Clock register settings
 */
#define CLOCK_ANA_USER_REG  0x34
/**
 *@brief System Power Management register settings
 */
#define PMU_ANA_USER_REG    0x35
//@} \\Device Configuration Registers

/**
 * @name Device Configuration values
 *@{
 */

/**
 * @brief Enable the low frequency RO
 */
#define LOW_FREQ_RO                 0x1B
/**
 * @brief Enable the external low frequency XO
 */
#define LOW_FREQ_XO                 0x5B
/**
 * @brief Enable the high frequency 16 MHz
 */
#define HIGH_FREQ_16M               0x40
/**
 * @brief Enable the high frequrency 32 MHz
 */
#define HIGH_FREQ_32M               0x44
/**
 * @brief Enable the SMPS
 */
#define SMPS_ON                     0x4C
/**
 * @brief Disable the SMPS
 */
#define SMPS_OFF                    0x6C
/**
 * @brief SMPS clock frequency value for 4.7 uH inductor
 */
#define SMPS_4_7uH_RM1              0x40
/**
 * @brief Power management configuration for 4.7 uH inductor
 */
#define SMPS_4_7uH_PMU              0xBE
/**
 * @brief SMPS clock frequency value for 10 uH inductor
 */
#define SMPS_10uH_RM1               0x20
/**
 * @brief Power management configuration for 10 uH inductor
 */
#define SMPS_10uH_PMU               0xB2
/**
 * @brief RCO32 trimming default values
 */
#define PMU_ANA_USER_RESET_VALUE    0x0B
/**
 * @brief Analog test bus 0 settings for 
 * normal application mode
 */
#define USER_MODE_ATB0              0x00
/**
 * @brief Analog test bus 1 settings for 
 * normal application mode
 */
#define USER_MODE_ATB1              0x30
/**
 * @brief Analog test bus 0 settings for 
 * low speed crystal measurement
 */
#define LS_XTAL_MEAS_ATB0           0x37
/**
 * @brief Analog test bus 1 settings for 
 * low speed crystal measurement
 */
#define LS_XTAL_MEAS_ATB1           0x34
/**
 * @brief Analog test bus 0 settings for 
 * high speed crystal startup time measurement
 */
#define HS_STARTUP_TIME_MEAS_ATB0   0x04
/**
 * @brief Analog test bus 1 settings for 
 * high speed crystal startup time measurement
 */
#define HS_STARTUP_TIME_MEAS_ATB1   0x34
/**
 * @brief Analog test bus 0 settings for 
 * Tx/Rx start stop signal measurement
 */
#define TX_RX_START_STOP_MEAS_ATB0  0x38
/**
 * @brief Analog test bus 1 settings for 
 * Tx/Rx start stop signal measurement
 */
#define TX_RX_START_STOP_MEAS_ATB1  0x34
//@} \\Device Configuration values

/**
 *@brief Number of configuration bytes to send over Blue SPI
 */
#define NUMBER_CONFIG_BYTE  0x02
/**
 *@brief End Configuration Tag
 */
#define END_CONFIG          0x00


/* Cold Start Configuration Table */
#define COLD_START_CONFIGURATION                                     \
{                                                                    \
  NUMBER_CONFIG_BYTE, ATB0_ANA_ENG_REG,   USER_MODE_ATB0,            \
  NUMBER_CONFIG_BYTE, ATB1_ANA_ENG_REG,   USER_MODE_ATB1,            \
  NUMBER_CONFIG_BYTE, RM1_DIG_ENG_REG,    SMPS_10uH_RM1,             \
  NUMBER_CONFIG_BYTE, CLOCK_LOW_ENG_REG,  SMPS_ON,                   \
  NUMBER_CONFIG_BYTE, CLOCK_HIGH_ENG_REG, HIGH_FREQ_16M,             \
  NUMBER_CONFIG_BYTE, PMU_ANA_ENG_REG,    SMPS_10uH_PMU,             \
  NUMBER_CONFIG_BYTE, CLOCK_ANA_USER_REG, LOW_FREQ_XO,               \
  NUMBER_CONFIG_BYTE, PMU_ANA_USER_REG,   PMU_ANA_USER_RESET_VALUE,  \
  END_CONFIG                                                         \
}

/* Device Configuration value for struct deviceConfigS */
#define HS_CRYSTAL_16MHZ 0xFF
#define HS_CRYSTAL_32MHZ 0x00

#define LS_SOURCE_RO    0xFF
#define LS_SOURCE_32KHZ 0x00

#define SMSP_10uH     0xFF
#define SMSPS_4_7uH   0x00
#define SMPS_IND_OFF  0x01

#define USER_MODE               0xFF
#define LS_CRYSTAL_MEASURE      0x01
#define HS_STARTUP_TIME_MEASURE 0x02

/* Device Configuration value for struct deviceConfigS */
#define TX_RX_START_STOP_MEASURE 0x03

/**
 * @brief RCO32K trimming value flash location
 */
#define RCO32K_TRIMMING_FLASH_ADDR 0x100007E8
/**
 * @brief LDO1V2 trimming value flash location
 */
#define LDO1V2_TRIMMING_FLASH_ADDR 0x100007E4
/**
 * @brief Battery Level Detector Threshold trimming value flash location
 */
#define SET_BLD_LVL_TRIMMING_FLASH_ADDR 0x100007B0
/**
 * @brief Check bytes tag
 */
#define CHECK_BYTES                0xAA55

/**
 * @brief Max HS startup time expressed in system time (1953 us / 2.4414 us)
 */
#define MAX_HS_STARTUP_TIME 0x320


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

devConfig_t deviceConfig;
volatile uint8_t cold_start_config[] = COLD_START_CONFIGURATION;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void coldStartConfiguration(void)
{
  uint8_t pmu_ana_user, clock_low_eng;
  uint32_t Trimm_config;
  PartInfoType Device_partInfo;

  /* Get partInfo */
  HAL_GetPartInfo(&Device_partInfo);

  /* High Speed Crystal Configuration */
  if (deviceConfig.HS_crystal == HS_CRYSTAL_32MHZ) {
    /* 32 MHz should be set in some way. This info is retrieved from flash info */
    cold_start_config[14] = HIGH_FREQ_32M;
  } else {
    cold_start_config[14] = HIGH_FREQ_16M;
  }

  /* Low Speed Crystal source */
  if (deviceConfig.LS_source == LS_SOURCE_32KHZ) {
    cold_start_config[20] = LOW_FREQ_XO;
  } else {
    cold_start_config[20] = LOW_FREQ_RO;
  }
  
  /* SMPS configuration */
  switch (deviceConfig.SMPS_management) {
  case SMSPS_4_7uH:
    {
      cold_start_config[11] = SMPS_ON;
      cold_start_config[8]  = SMPS_4_7uH_RM1;
      cold_start_config[17] = SMPS_4_7uH_PMU;
    }
    break;
  case SMPS_IND_OFF:
    {
      cold_start_config[11] = SMPS_OFF;
    }
    break;
  default:
    {
      cold_start_config[11] = SMPS_ON;
      cold_start_config[8]  = SMPS_10uH_RM1;
      cold_start_config[17] = SMPS_10uH_PMU;
    }
    break;
  }

  /* Test Mode */
  switch (deviceConfig.Test_mode) {
  case LS_CRYSTAL_MEASURE:
    {
      cold_start_config[2] = LS_XTAL_MEAS_ATB0;
      cold_start_config[5] = LS_XTAL_MEAS_ATB1;
    }
    break;
  case HS_STARTUP_TIME_MEASURE:
    {
      cold_start_config[2] = HS_STARTUP_TIME_MEAS_ATB0;
      cold_start_config[5] = HS_STARTUP_TIME_MEAS_ATB1;
    }
    break;
  case TX_RX_START_STOP_MEASURE:
    {
      cold_start_config[2] = TX_RX_START_STOP_MEAS_ATB0;
      cold_start_config[5] = TX_RX_START_STOP_MEAS_ATB1;
    }
    break;
  default:
    {
      cold_start_config[2] = USER_MODE_ATB0;
      cold_start_config[5] = USER_MODE_ATB1;
    }
    break;
  }

  /* Setup RCO32K trimming value in PMU_ANA_USER_REG  */
  Trimm_config = *(volatile uint32_t*)RCO32K_TRIMMING_FLASH_ADDR;
  if ((Trimm_config >> 16) == CHECK_BYTES) {
    cold_start_config[23] &= ~0x70;                  // Clear the register content bit 4-5-6
    cold_start_config[23] |= (Trimm_config&0x7)<<4;  // Store the RCO32K trimming value in bit 4-5-6
  }

  /* Setup LDO1V2 trimming value in ATB1_ANA_ENG_REG  */
  Trimm_config = *(volatile uint32_t*)LDO1V2_TRIMMING_FLASH_ADDR;
  if ((Trimm_config >> 16) == CHECK_BYTES) {
    cold_start_config[5] &= ~0x30;                 // Clear the register content of bit 4 and 5
    cold_start_config[5] |= (Trimm_config&0x3)<<4; // Store the LDO1V2 trimming value in bit 4 and 5
  }

  /* Setup Battery Level Detector Threshold value in PMU_ANA_USER_REG  */
  Trimm_config = *(volatile uint32_t*)SET_BLD_LVL_TRIMMING_FLASH_ADDR;
  if ((Trimm_config >> 16) == CHECK_BYTES) {
    cold_start_config[23] &= ~0x03;              // Clear the register content of bit 0 and 1
    cold_start_config[23] |= (Trimm_config&0x3); // Store the BLD_LVL trimming value in bit 0 and 1
  }

  /* BOR configuration. Note: this setup shall be executed after the SMPS configuration*/
#if (BOR_CONFIG == BOR_ON)
  /* Clear the 3 bit of the CLOCK_LOW_ENG_REG register */
  cold_start_config[11] &= ~(1<<2);
  if (((Device_partInfo.die_major<<4)|(Device_partInfo.die_cut)) >= WA_DEVICE_VERSION) {
    /* Set the 3 bit of the PMU_ANA_USER register */
    cold_start_config[23] |= (1<<2);    
    clock_low_eng = cold_start_config[11];
    pmu_ana_user = cold_start_config[23];
    BOR_ConfigSave(clock_low_eng, pmu_ana_user);
  }
#elif (BOR_CONFIG == BOR_OFF)
  /* Set the 3 bit of the CLOCK_LOW_ENG_REG register */
  cold_start_config[11] |= (1<<2);
  if (((Device_partInfo.die_major<<4)|(Device_partInfo.die_cut)) >= WA_DEVICE_VERSION) {
    /* Clear the 3 bit of the PMU_ANA_USER register */
    cold_start_config[23] &= ~(1<<2);
    clock_low_eng = cold_start_config[11];
    pmu_ana_user = cold_start_config[23];
    BOR_ConfigSave(clock_low_eng, pmu_ana_user);
  }
#else
#error "No definition for BOR Configuration"
#endif

}

#define FLASH_ADDRESS_STACK_DEV_ID_DATA   (FLASH_END - N_BYTES_PAGE + 1)
void DTM_SystemInit(void)
{
  uint32_t current_time;
  uint8_t *dev_config_addr;
  PartInfoType Device_partInfo;

  /* Remap the vector table */
  //FLASH->CONFIG = FLASH_PREMAP_MAIN;

  /* Load device configuration from FLASH memory */
  /* The first 256 bytes are for stacklib_stored_device_id_data */
  dev_config_addr = (uint8_t *) (FLASH_ADDRESS_STACK_DEV_ID_DATA + 0x100);
  memcpy(&deviceConfig, dev_config_addr, sizeof(deviceConfig));

#ifdef UART_INTERFACE
#if (HS_SPEED_XTAL==HS_SPEED_XTAL_16MHZ)
  if(deviceConfig.HS_crystal == HS_CRYSTAL_32MHZ) {
    deviceConfig.HS_crystal = 0xFF; // Set HS_CRYSTAL_16MHZ
    FLASH_ErasePage((FLASH_ADDRESS_STACK_DEV_ID_DATA-FLASH_START)/N_BYTES_PAGE);
    uint32_t tmp32[4] = {0,0,0,0};
    tmp32[0] = (((uint32_t)deviceConfig.Reserved)<<24) | (((uint32_t)deviceConfig.SMPS_management)<<16) | (((uint32_t)deviceConfig.LS_source)<<8) |  (((uint32_t)0xFF)<<0);
    tmp32[1] = (((uint32_t)deviceConfig.SlaveSCA)<<16) | (((uint32_t)deviceConfig.HS_startup_time)<<0);
    tmp32[2] = ((((uint32_t)deviceConfig.max_conn_event_length)&0x00FFFFFF)<<8) | (((uint32_t)deviceConfig.MasterSCA)<<0);
    tmp32[3] = (((uint32_t)0xFF)<<24) | (((uint32_t)0xFF)<<16) | (((uint32_t)deviceConfig.Test_mode)<<8) | ((((uint32_t)deviceConfig.max_conn_event_length)&0xFF000000)>>24);    
    FLASH_ProgramWordBurst(FLASH_ADDRESS_STACK_DEV_ID_DATA + 0x100, tmp32);
  }
#elif (HS_SPEED_XTAL==HS_SPEED_XTAL_32MHZ)
  if(deviceConfig.HS_crystal == HS_CRYSTAL_16MHZ) {
    uint32_t tmp32 = (((uint32_t)deviceConfig.Reserved)<<24) | (((uint32_t)deviceConfig.SMPS_management)<<16) | (((uint32_t)deviceConfig.LS_source)<<8) |  (((uint32_t)0x00)<<0);
    deviceConfig.HS_crystal = 0x00; // Set HS_CRYSTAL_32MHZ
    FLASH_ProgramWord(FLASH_ADDRESS_STACK_DEV_ID_DATA + 0x100, tmp32);
  }
#endif
#else
#if (HS_SPEED_XTAL==HS_SPEED_XTAL_16MHZ)
  if(deviceConfig.HS_crystal == HS_CRYSTAL_32MHZ) {
    deviceConfig.HS_crystal = 0xFF; // Set HS_CRYSTAL_16MHZ
  }
#elif (HS_SPEED_XTAL==HS_SPEED_XTAL_32MHZ)
  if(deviceConfig.HS_crystal == HS_CRYSTAL_16MHZ) {
    deviceConfig.HS_crystal = 0x00; // Set HS_CRYSTAL_32MHZ
  }
#endif 
#endif
  
  coldStartConfiguration();

  if (deviceConfig.HS_crystal == HS_CRYSTAL_32MHZ) {
    /* Set 32MHz_SEL bit in the System controller register */
    SYSTEM_CTRL->CTRL_b.MHZ32_SEL = 1;
  }
  
  /* Cold start configuration device */
  BLUE_CTRL->RADIO_CONFIG = 0x10000U | (uint16_t)((uint32_t)cold_start_config & 0x0000FFFFU);
  while ((BLUE_CTRL->RADIO_CONFIG & 0x10000) != 0);

  /* Configure RF FrontEnd */
  HAL_GetPartInfo(&Device_partInfo);
  if (((Device_partInfo.die_major<<4)|(Device_partInfo.die_cut)) >= WA_DEVICE_VERSION) {
    Set_RF_FrontEnd();
  }

  /* Wait until HS is ready. The slow clock period 
  * measurement is done automatically each time the
  * device enters in active2 state and the HS is ready.
  * The interrupt signals that a measurement is done.
  */
  while(CKGEN_BLE->CLK32K_IT == 0);
  CKGEN_BLE->CLK32K_IT = 1;
  CKGEN_BLE->CLK32K_COUNT = 23; //Restore the window length for slow clock measurement.
  CKGEN_BLE->CLK32K_PERIOD = 0;

  /* Wait until the RO or 32KHz is ready */
  current_time = *(volatile uint32_t *)0x48000010;
  while(((*(volatile uint32_t *)0x48000010)&0x10) == (current_time&0x10));
  
  if (deviceConfig.HS_crystal == HS_CRYSTAL_32MHZ) {
    /* AHB up converter command register write*/
    AHBUPCONV->COMMAND = 0x15;
  }

  /* Unlock the Flash */
  flash_sw_lock = FLASH_UNLOCK_WORD;
  
  /* Disable all the peripherals clock except NVM, SYSCTR, PKA and RNG */
  CKGEN_SOC->CLOCK_EN = 0xE0066;
  
  /* Enable the IRQs */
  __enable_irq();
}

void DTM_StackInit(void)
{
  uint8_t ret;

  if (deviceConfig.max_conn_event_length > 1639344) {
    BlueNRG_Stack_Init_params.hardware_config.max_conn_event_length = 0xFFFFFFFF;
  } else {
    BlueNRG_Stack_Init_params.hardware_config.max_conn_event_length = deviceConfig.max_conn_event_length;
  }
  if (deviceConfig.SlaveSCA == 0xFFFF) {
    BlueNRG_Stack_Init_params.hardware_config.slave_sca = 500;
  } else {
    BlueNRG_Stack_Init_params.hardware_config.slave_sca = deviceConfig.SlaveSCA;
  }
  if (deviceConfig.MasterSCA == 0xFF) {
    BlueNRG_Stack_Init_params.hardware_config.master_sca = 0;
  } else {
    BlueNRG_Stack_Init_params.hardware_config.master_sca = deviceConfig.MasterSCA;
  }
  BlueNRG_Stack_Init_params.hardware_config.ls_source = deviceConfig.LS_source; 
  if (deviceConfig.HS_startup_time > MAX_HS_STARTUP_TIME) {
    BlueNRG_Stack_Init_params.hardware_config.hs_startup_time = MAX_HS_STARTUP_TIME;
  } else {
    BlueNRG_Stack_Init_params.hardware_config.hs_startup_time = deviceConfig.HS_startup_time;
  }
 
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != 0) {
    while(1);
  }
}

