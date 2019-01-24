/**
  ******************************************************************************
  * @file    BlueNRG1_radio.h
  * @author  RF Application Team
  * @version V1.2.0
  * @date    03-April-2018
  * @brief   This file contains all the functions prototypes for the radio firmware 
  *          library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  ******************************************************************************
  */
  
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_RADIO_H
#define BLUENRG1_RADIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"


/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */


/** @addtogroup Radio_Peripheral  Radio Peripheral
  * @{
  */

/** @defgroup Radio_Exported_Constants Exported Constants
* @{
  */

#define HEADER_LENGTH           2
#ifdef BLUENRG2_DEVICE
#define MAX_PACKET_LENGTH       (255+HEADER_LENGTH) // 255 (-4 if encryption)
#else
#define MAX_PACKET_LENGTH       (38+HEADER_LENGTH)  // 31 in data channel, 38 in adv. channel (-4 if encryption)
#endif
#define MIC_FIELD_LENGTH        4
#define RECEIVE_BUFFER_LENGTH   45
#define MAX_OUTPUT_RF_POWER     15
#define SUCCESS_0               0
#define INVALID_PARAMETER_C0    0xC0
#define WAKEUP_NOTSET_C2        0xC2
#define RADIO_BUSY_C4           0xC4
#define COMMAND_DISALLOWED      0xC5
#define NULL_0                  0
#define BLUE_IDLE_0             0  
#define BLUE_BUSY_NOWAKEUP_T1   1 
#define BLUE_BUSY_NOWAKEUP_T2   2 
#define BLUE_BUSY_WAKEUP        3 
#define TIMESTAMP_POSITION_ACCESSADDRESS    0x40 
#define TIMESTAMP_POSITION_LASTBIT          0x80 /*C0*/  
#define HOT_ANA_CONFIG_TABLE_LENGTH         64
#define CONFIG_ERROR_EN                     0x10
#define SCA_DEFAULTVALUE                    488

#define STATEMACHINE_COUNT   8

#define ANALOG_HW_OFFSET        (86)  /* subtrack 2*86 in the 1st packet for XC oscillator */  
#define TIMING_ERROR_CORRECTION (69)  /* subtrack 2*69 in all packet except 1st for XC oscillator */

  
/** @defgroup Radio_Interrupt_Status_Bits  Interrupt Status bits
  * @{
  */

#define IRQ_RCV_OK              (1UL<<31)       /* The packet is received, and the CRC is valid. */
#define IRQ_CRC_ERR             (1UL<<30)       /* The packet is received with CRC error or timeout error. */
#define IRQ_RCV_TRIG            (1UL<<29)
#define IRQ_CMD                 (1UL<<28)
#define IRQ_MD                  (1UL<<27)
#define IRQ_TIMEOUT             (1UL<<26)       /* No packet received within the defined RX time window. */
#define IRQ_RCV_FAIL            (1UL<<25)
#define IRQ_DONE                (1UL<<24)       /* Requested action (TX or RX) has been executed. */
#define IRQ_ERR_ENC             (1UL<<23)       /* A packet is received, but there is an error in the MIC field. */
#define IRQ_TX_OK               (1UL<<22)       /* The packet has been sent successfully. */
#define BIT_TX_SKIP             (1UL<<20)
#define IRQ_CONFIG_ERR          (1UL<<19)
#define BIT_TX_MODE             (1UL<<18)       /* The packet has been sent successfully. */
#define BIT_TIME_OVERRUN        (1UL<<17)
#define BIT_ACT2_ERROR          (1UL<<16)
#define IRQ_WAKEUP_2            (1UL<<15)
#define BIT_AES_READY           (1UL<<12)

/**
  * @}
  */ 

  
/** @defgroup Radio_ActionTag_BitMask ActionTag BitMask
* @{
*/

/* This bit activates the radio frequency PLL calibration.
 * 0: Radio frequency calibration disabled.
 * 1: Radio frequency calibration enabled.
 * User should set this bit only if TIMER_WAKEUP is set to 1.
*/
#define PLL_TRIG                    0x01
    
/* This bit determines if the action is an RX action or a TX action.
 * 1: TX
 * 0: RX
*/
#define TXRX                        0x02
    
/* The bit determines if the action (RX or TX) is going to be executed based on the back-to-back time or based on the WakeupTime.
 * 0: Based on the back-to-back time (default 150 µs).
 * 1: Based on the WakeupTime.
*/
#define TIMER_WAKEUP                0x04
    
/* This bit activates automatic channel increment. The API RADIO_SetChannel sets the value of the increment.
 * 0: No increment
 * 1: Automatic increment
*/
#define INC_CHAN                    0x10
    
/* It determines if the WakeupTime field of the ActionPacket is considered as absolute time or relative time to the current.
 * 0: Absolute
 * 1: Relative
*/
#define RELATIVE                    0x20
    
/* This bit sets where the position of time stamp is taken, the beginning of the packet or the end of it. RX only.
 * 0: End of the Packet
 * 1: Beginning of the packet
 */
#define TIMESTAMP_POSITION          0x80 

/**
  * @}
  */


/**
* @}
*/

/** @defgroup Radio_Exported_Types Exported Types
* @{
*/

typedef enum {
  STATE_MACHINE_0 = 0,
  STATE_MACHINE_1,
  STATE_MACHINE_2,
  STATE_MACHINE_3,
  STATE_MACHINE_4,
  STATE_MACHINE_5,
  STATE_MACHINE_6,
  STATE_MACHINE_7,
} StateMachine_t; 

typedef struct
{
  volatile uint16_t next;
  volatile uint16_t datptr;
  volatile uint8_t  rcvlen[3];
  volatile uint8_t  timeout[3];
  volatile uint8_t  byte10;
  volatile uint8_t  byte11;
} BlueTransStruct;


typedef struct ActionPacket ActionPacket;

typedef struct {
  uint32_t *hot_ana_config_table;   /**< Set to NULL */
  uint8_t ls_source;                /**< Source for the 32 kHz slow speed clock: 1: internal RO; 0: external crystal */
  uint16_t hs_startup_time ;        /**< Start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us)*/
} config_table_t;


typedef struct {
    int32_t freq;    
    int32_t period;    
    struct {
      uint32_t  Time_mT        : 24;
      uint32_t  started_flag   :  8;
    } calibr;    
} Clk32Context_t;

typedef struct {
    uint32_t back2backTime;
    uint8_t forceRadiotoStop;
    uint32_t rssiLevel[2];
    int32_t wakeupTime;
    uint16_t period_slow_patch;
    int32_t freq_global_debug;
    int32_t period_global_debug;
    int32_t hot_ana_config_table_a[HOT_ANA_CONFIG_TABLE_LENGTH>>2];
    Clk32Context_t Clk32Context;
    config_table_t hardware_config;
   // uint8_t tone_start_stop_flag;
    ActionPacket* current_action_packet;    
    uint8_t powerUpfirstPacket;

}RadioGlobalParameters_t;

extern RadioGlobalParameters_t globalParameters;


struct ActionPacket
{
  uint8_t StateMachineNo ;      /* This parameter indicates the state machine number for this action. From 0 to 7. */
  uint8_t ActionTag;            /* The configuration of the current action. 
                                 * Action Tag: PLL_TRIG, TXRX, TIMER_WAKEUP, INC_CHAN, TIMESTAMP_POSITION, RELATIVE */
  uint32_t WakeupTime;          /* Contains the wakeup time in microsecond if it is relative.
                                 * It should not be more than 24 bits if it is absolute. 
                                 * It only applies if TIMER_WAKEUP flag is set in ActionTag. */
  uint32_t ReceiveWindowLength; /* Sets RX window size in microsecond. Applicable only for RX actions. */
  uint8_t *data;                /* Pointer to the array with the data to send (header, length and data field), for TX.
                                 * Pointer to the array where the data received are copied, for RX.
                                 * In case of RX, the array must have the max size MAX_PACKET_LENGTH. */
  uint32_t status;              /* The Status Register with the information on the action. */
  uint32_t timestamp_receive;   /* This field contains the timestamp when a packet is received.
                                 * Intended to be used in the dataRoutine() callback routine. RX only. */
  int32_t rssi;                 /* The rssi of the packet was received with. RX only. */
  BlueTransStruct trans_packet; /* This is a linked list, which is going to be used by Hardware.
                                  * User does not need to do anything. */
  ActionPacket *next_true;      /* Pointer to next ActionPacket if condRoutine() returns TRUE */
  ActionPacket *next_false;     /* Pointer to next ActionPacket if condRoutine() returns FALSE */
  uint8_t (*condRoutine)(ActionPacket*);        /* User callback that decide the next ActionPacket to use.
                                                 * It is time critical. Routine must end within 45 us. */
  uint8_t (*dataRoutine)(ActionPacket*, ActionPacket*); /* User callback for managing data. */
  uint8_t trans_config;                                 /* This is for configuring the device for TX or RX. User does not need to do anything. */
};

/**
* @}
*/


/** @defgroup Radio_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup Radio_Private_Functions Private Functions 
* @{
*/
  
/**
  * @}
  */


/** @defgroup Radio_Exported_Functions Exported Functions
  * @{
  */

void RADIO_Init(uint16_t hs_startup_time, uint8_t low_speed_osc, uint32_t* hot_table, FunctionalState whitening);
uint8_t RADIO_GetStatus(uint32_t *time);
void RADIO_SetChannelMap(uint8_t StateMachineNo,uint8_t *chan_remap);
void RADIO_SetChannel(uint8_t StateMachineNo, uint8_t channel,uint8_t channel_increment); 
void RADIO_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID, uint32_t crc_init, uint32_t sca);
void RADIO_SetBackToBackTime(uint32_t back_to_back_time);  
void RADIO_SetTxPower(uint8_t PowerLevel);    
void RADIO_IRQHandler(void);
uint8_t RADIO_StopActivity(void);
void RADIO_SetReservedArea(ActionPacket *p); 
uint8_t RADIO_MakeActionPacketPending(ActionPacket *p);
void RADIO_CrystalCheck(void);
void RADIO_StartTone(uint8_t RF_channel, uint8_t powerLevel);
void RADIO_StopTone(void);
void RADIO_SetEncryptionCount(uint8_t StateMachineNo, uint8_t *count_tx, uint8_t *count_rcv);    
void RADIO_SetEncryptionAttributes(uint8_t StateMachineNo, uint8_t *enc_iv, uint8_t *enc_key); 
void RADIO_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlagTx, FunctionalState EncryptFlagRcv);
void RADIO_EncryptPlainData(uint8_t *Key, uint8_t *plainData, uint8_t *cypherData);

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /*BLUENRG1_RADIO_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
