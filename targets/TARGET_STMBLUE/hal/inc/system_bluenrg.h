/*
 * Copyright (c) 2019 STMicroelectronics. All Rights Reserved.
 *
 * The information contained herein is property of STMicroelectronics.
 * Terms and conditions of usage are described in detail in SOFTWARE
 * LICENSE AGREEMENT:
 *
 * (SLA0068 - PRODUCTION LIMITED LICENSE AGREEMENT FOR ST MATERIALS)
 *
 * You may obtain a copy of the License here:
 * LICENSE-SLA0068.txt and at STMicroelectronics website.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSTEM_BLUENRG_H
#define SYSTEM_BLUENRG_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup system_bluenrg system bluenrg
  * @{
  */  

/** @addtogroup system_bluenrg_device_configuration Device Configuration Constants
  * @{
  */

/**
  * @brief High Speed crystal 32 MHz
  */ 
#define HS_SPEED_XTAL_32MHZ       1
   
/**
  * @brief High Speed crystal 16 MHz
  */ 
#define HS_SPEED_XTAL_16MHZ       2

/**
  * @brief High Speed Internal RO
  * Not useful when radio operation are needed
  * or in any case when accurate ref clock is needed.
  */ 
#define HS_SPEED_XTAL_INTERNAL_RO 3
   

/**
  * @brief Low Speed Internal RO
  */
#define LS_SOURCE_INTERNAL_RO     1

/**
 * @brief Low Speed External 32 KHz
 */
#define LS_SOURCE_EXTERNAL_32KHZ  2

/**
 * @brief SMPS Inductor 10 uH
 */
#define SMPS_INDUCTOR_10uH        1

/**
 * @brief SMPS Inductor 4.7 uH
 */
#define SMPS_INDUCTOR_4_7uH       2

/**
 * @brief SMPS Inductor None
 */
#define SMPS_INDUCTOR_NONE        3

/**
 * @brief BOR OFF
 */
#define BOR_OFF                   1

/**
 * @brief BOR ON
 */
#define BOR_ON                    2

/*Device version to apply the BOR/BLD settings*/
#define WA_DEVICE_VERSION 0x12
   
/**
 * @brief Word to unlock the flash operation
 */
#define FLASH_UNLOCK_WORD         0xFCECBCCC

/**
 * @brief Word to lock the flash operation
 */
#define FLASH_LOCK_WORD           0
   
 /**
  * @brief High Speed Crystal default configuration 
  */
#ifndef HS_SPEED_XTAL
#define HS_SPEED_XTAL HS_SPEED_XTAL_16MHZ
#endif

/**
 * @brief  Low Speed Crystal default source 
 */
#ifndef LS_SOURCE
#define LS_SOURCE LS_SOURCE_EXTERNAL_32KHZ
#endif

/** 
 * @brief SMPS default configuration 
 */
#ifndef SMPS_INDUCTOR
#define SMPS_INDUCTOR SMPS_INDUCTOR_10uH
#endif
   
/** 
 * @brief BOR default configuration 
 */
#ifndef BOR_CONFIG
#define BOR_CONFIG BOR_ON
#endif

/**
  * @}
  */


/** @addtogroup system_bluenrg_Exported_Macros Exported Macros
  * @{
  */



#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))


/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */
  
  
/**
  * @}
  */
  
/** @addtogroup system_bluenrg_Exported_Typedefs Exported Typedefs
  * @{
  */
  
typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;


typedef uint16_t BlueTransBlueStrctPtr_t;
typedef uint16_t BlueTransBlueDataPtr_t;

typedef struct
{
  volatile uint16_t Config;            // 1:0
  volatile uint16_t period_slow;       // 3:2
  volatile uint8_t unused;             // 4
  volatile uint8_t TxDelay;            // 5
  volatile uint8_t RcvDelay;           // 6
  volatile uint8_t TxDelay1;           // 7
  volatile uint8_t RcvDelay1;          // 8
  volatile uint8_t wakeup_time_offset; // 9
  volatile BlueTransBlueDataPtr_t RadioConfigPtr;    // 10:11
} BlueGlob;

typedef struct
{
  volatile uint8_t Pack_count_Tx[5];            // 4:0
  volatile uint8_t Pack_count_Rcv[5];           // 9:5        
  volatile uint8_t stat_config;                 // 10
  /*volatile uint8_t rcv_fail_count;*/
  volatile uint8_t unmapped_chan;               // 11        
  volatile BlueTransBlueStrctPtr_t txpoint_prev;     // 13:12
  volatile BlueTransBlueStrctPtr_t rcvpoint_prev;    // 15:14
  volatile BlueTransBlueStrctPtr_t txpoint;          // 17:16
  volatile BlueTransBlueStrctPtr_t rcvpoint;         // 19:18
  volatile BlueTransBlueStrctPtr_t txpoint_next;     // 21:20
  volatile uint8_t remap_chan;          // 22
  volatile uint8_t config;              // 23
  volatile uint32_t access_address;     // 27:24
  volatile uint32_t crc_init;           // 31:28
  /* volatile  uint8_t rcv_fail_trig;*/
  volatile uint8_t hop_incr;            // 32
  volatile uint8_t chan_remap[5];       // 37:33
  volatile uint8_t enc_iv[8];           // 45:38
  volatile uint8_t enc_key[16];         // 61:46
  volatile uint8_t conf_byte5;          // 62
  volatile uint8_t unused;              // 63
} BlueGlobPerMaster;


typedef struct
{
  BlueGlob BlueGlobVar;
  BlueGlobPerMaster BlueGlobPerMasterVar[8];
  
} __blue_RAM_struct;


/**
  * @}
  */


/** @addtogroup system_bluenrg_Exported_Variables Exported Variables
  * @{
  */


extern __blue_RAM_struct __blue_RAM;
extern const intvec_elem __vector_table[];
extern uint32_t savedICSR;
extern uint32_t savedSHCSR;
extern uint32_t savedNVIC_ISPR;
extern volatile  uint8_t wakeupFromSleepFlag;
extern volatile uint32_t ota_sw_activation;
extern volatile uint32_t flash_sw_lock;

/**
  * @}
  */

   
/** @addtogroup system_bluenrg_Exported_Functions Exported Functions
  * @{
  */
  
/* Important note: The __low_level_init_CS() function is critical for waking up from 
   deep sleep and it should not use more that 10 stack positions
   otherwise a stack corruption will occur when waking up from deep sleep.
   For this reason we are saving and restoring the first 10 words of the stack that 
   will be corrupted during the wake-up procedure from deep sleep.
   If the __low_level_init_CS() will be modified, this define shall be modifed according
   the new function implementation
*/
#define CSTACK_PREAMBLE_NUMBER 10


void SystemInit(void);
void DeviceConfiguration(BOOL coldStart, BOOL waitLS_Ready);
void BOR_ConfigSave(uint8_t clock_low_eng_conf, uint8_t pmu_ana_user_conf);
void SET_BORconfigStatus(uint8_t enabled);
void Set_RF_FrontEnd(void);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_BLUENRG_H */

/**
  * @}
  */
  
/**
  * @}
  */
