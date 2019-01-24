/**
******************************************************************************
* @file    BlueNRG1_radio.c
* @author  RF Application Team
* @version V1.2.0
* @date    3-April-2018
* @brief   This file provides all the BlueNRG-1,2 Radio Driver APIs.
*          This driver is based on undocumented hardware features and 
*          it is strongly recommend to do not change it.
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

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/BlueNRG1_radio.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
* @{
*/

/** @addtogroup Radio_Peripheral  Radio Peripheral
* @{
*/


/** @defgroup Radio_Exported_Types Exported Types
* @{
*/

 

/**
* @}
*/


/** @defgroup Radio_Exported_Constants Exported Constants
* @{
*/ 

/** @defgroup Radio_TXRX_Delay_Constants TXRX Delay Constants
* @{
*/
  
#define TX_DELAY     0x09
#define TX_DELAY1    0x09
#define RX_DELAY     0x05
#define RX_DELAY1    0x05

#define ANALOG_OFFSET 0x00
/**
* @}
*/

#define LS_CRYSTAL_PERIOD   (0X00190000) // Default period for 32768 Hz Crystal
#define LS_CRYSTAL_FREQ     (0x0028F5C2) // Default frequency for 32768 Hz Crystal

/* timeout register offsets and masks */
#define OFF_R_DESTINATION    (24)
#define OFF_R_SLEEP          (28)
#define MASK_R_TIMEOUT       (0X00ffffff)

/*#define Blue_BIT_PLLTRIG 1
#define Blue_BIT_NEXT_TX_MODE 2
#define Blue_BIT_SLEEP (1<<2)
#define Blue_BIT_NS_EN (1<<3)
#define Blue_BIT_INC_CHAN (1<<4)
#define Blue_SHIFT_NEXT_MAS (5)*/

#define Blue_INT_DONE (1)
#define Blue_INT_RCV_OK (2)
#define Blue_INT_CRC_ERR (1<<2)
#define Blue_INT_RCV_TRIG (1<<3)
#define Blue_INT_CMD (1<<4)
#define Blue_INT_RCVMD (1<<5)
#define Blue_INT_TIMEOUT (1<<6)
#define Blue_INT_TX_OK (1<<7)

#define WAKEUP_TIMER_TRIGGER     0x1000000 
#define TIMER1_TIMER_TRIGGER     0x2000000 
#define TIMER2_TIMER_TRIGGER     0x3000000

#define WAKEUP_TIMER_MASK     0x1
#define TIMER1_TIMER_MASK     0x2
#define TIMER2_TIMER_MASK     0x3

#define BLUE_SLEEP_ENABLE        0x10000000
#define BLUE_STANDBY_ENABLE      0x30000000
#define TIMER_OFF 0
#define SLEEP_TIME_MIN   (250)
#define NO_DEEP_SLEEP                       (0x97)


#define IRQSPI                  (SPI_IRQn)
#define IRQBLUE                 (BLUE_CTRL_IRQn)
#define IRQNVM                  (NVM_IRQn)

#define BLE_STATUS_SUCCESS            		    (0x00)

/**
* @}
*/

/** @defgroup Radio_Exported_Macros Exported Macros
* @{
*/
#define DISABLE_INTERRUPT(irqNum)            {NVIC_DisableIRQ(irqNum);}
#define ENABLE_INTERRUPT(irqNum)             {NVIC_EnableIRQ(irqNum);}
#define BLUE_STRUCT_PTR_CAST(x) (((BlueTransBlueStrctPtr_t)(int)(x)) & 0xffff)
#define BLUE_DATA_PTR_CAST(x) (((BlueTransBlueDataPtr_t)(int)(x)) & 0xffff)
#define TIME24_DIFF(a, b)       (((int32_t)((a - b) << 8)) >> 8)
#define IS_STATE_VALID(STATEMACHINE_NO) (STATEMACHINE_NO < STATEMACHINE_COUNT)
#define IS_POWERLEVEL_VALID(POWER) (POWER < 16)
#define IS_RFCHANNEL_VALID(RF_CHANNEL) (RF_CHANNEL <40)
#define GET_CUR_TIME            (MASK_R_TIMEOUT & BLUE_CTRL->CURRENT_TIME)
#define IFR (&(globalParameters.hardware_config) )

#define IS_DELAY_VALID(TIME) ( ((us_time_translate(TIME) - (IFR->hs_startup_time + (IFR->hs_startup_time>>2)-(IFR->hs_startup_time>>4)) - ANALOG_HW_OFFSET)>10) && \
                            ((us_time_translate(TIME) - (IFR->hs_startup_time + (IFR->hs_startup_time>>2)-(IFR->hs_startup_time>>4)) - ANALOG_HW_OFFSET)<(1<<24)))

  
#define MASK_INTERRUPTS() __disable_irq(); 
#define UNMASK_INTERRUPTS()  __enable_irq();


/**
* @}
*/ 

/** @defgroup Radio_Private_Variables Private Variables
* @{
*/

static volatile BlueGlob* const blueglob = &__blue_RAM.BlueGlobVar; // (BlueGlob*) BLUEGLOB_BASE;
static volatile BlueGlobPerMaster* const bluedata = __blue_RAM.BlueGlobPerMasterVar;// (BlueGlobPerMaster*) (BLUEGLOB_BASE+12);

/* volatile uint8_t *testP;*/

/* glabal variables */
RadioGlobalParameters_t globalParameters;

/*ActionPacket dummy_action_packet;
uint8_t dummy_data[50];*/


/**
* @}
*/ 




/** @defgroup Radio_Private_Functions Private Functions 
* @{
*/

/**
* @brief  Weak definition of RADIO_TimerOverrunCallback
* @param  void
* @retval void
*/
WEAK_FUNCTION(void RADIO_TimerOverrunCallback(void));;

/**
* @brief  Weak definition of RADIO_HostTimerCallback
* @param  void
* @retval void
*/
WEAK_FUNCTION(void RADIO_HostTimerCallback(void));

/**
* @brief  This function translates the incoming time in system time units to machine time units.
* @param  time: Time in system time units (625/256 us).
* @retval int32_t: time in machine time units (period slow clock/16).
*/
static int32_t machine_time_translate(int32_t time)
{
  int32_t freq_global = globalParameters.freq_global_debug ;
  int32_t time1 = (time >> 15) ;
  int32_t time2 = time & 0x7fff ;
  int32_t freq1 = freq_global >> 7 ;
  int32_t result = freq_global*time1 + ((time2 * freq1) >> 8) ;
  
  return((result + 32) >> 6) ;
}


/**
* @brief  This function translates the incoming time in 512 kHz time units to us.
* @param  time: Time in system in 512 kHz unit.
* @retval uint32_t: time in micro second.
*/
static uint32_t us_time_translate(int32_t time)
{
  
    //0.68DB8BAC710CB29
  uint32_t t1 = time * 0x68;
  uint32_t t2 = time * 0xDB;

  return machine_time_translate( (t1 >> 8) + (t2 >> 16) );//(time*256)/625);
}


/**
* @brief  This function gets frequency and period and sets approprite registers of BLE.
* @param  a: frequency.
*         b: period.
* @retval None
*/
static void set_freq(int32_t a, int32_t b)
{
  int32_t freq_global;
  int32_t period_global;
  
  if(IFR->ls_source != 0) {
    int32_t mult = 0x753 ;
    int32_t a1 = a >> 6 ;
    int32_t a2 = a1 * mult ;
    
    int32_t mul1 = 0x8BCF6 ;
    int32_t b1 = b >> 8 ;
    int32_t b2 = b & 0xff ;
    period_global = ((mul1 * b1) + ((b2 * mul1)>>8) + 16 )>>5 ;
    
    freq_global = (a2 + 128) >> 8 ;      
    
    globalParameters.period_slow_patch = ((b * 1365) >> 16); /* Divide by 48 */
  }
  else {
    freq_global = LS_CRYSTAL_FREQ ;
    period_global = LS_CRYSTAL_PERIOD;
    /* [EM:] Fix bug #1343
    *  save the period_slow value to restore in the ISR
    */
    globalParameters.period_slow_patch = ((LS_CRYSTAL_PERIOD>>6)*1250) >> 16;
  }
  
  blueglob->period_slow = globalParameters.period_slow_patch;    
  globalParameters.freq_global_debug = freq_global;   
  globalParameters.period_global_debug = period_global; /* not used in current implementation */ 
  int32_t time1 = machine_time_translate(IFR->hs_startup_time) ;
  if(time1 >= 1024)
    time1 = 1023 ;
  if (time1 < 8)
    time1 = 8;
  blueglob->wakeup_time_offset = time1 >> 3 ;
}


/**
* @brief  BlueNRG Start Crystal Measurement.
* @param  None
* @retval None
*/
static void BlueNRG_Start_Crystal_Measurement(void)
{
  CKGEN_BLE->CLK32K_PERIOD = 0;
  globalParameters.Clk32Context.calibr.started_flag = 1;    
}


/**
* @brief  Read RSSI
* @param  None
* @retval int32_t: RSSI in dBm
*/
static int32_t read_RSSI(void)
{
  int i = 0 ;
  int rsi_dbm;
  
  while(i < 100 && (BLUE_CTRL->RADIO_CONFIG & 0x10000) != 0)
  {
    //        dump_it = i++;
    rsi_dbm = i++; // use rsi_dbm instead of dump_it which is in RAM2
  }
  
  int rssi_int16 = ((globalParameters.rssiLevel[0] >> 16) & 0xff00) | (globalParameters.rssiLevel[1] & 0xff) ;
  int reg_agc = (globalParameters.rssiLevel[0] >> 16) & 0xff ;
  
  if(rssi_int16 == 0 || reg_agc > 0xb)
  {
    rsi_dbm = 127 ;
  }
  else
  {
    rsi_dbm = reg_agc * 6 -127 ;
    while(rssi_int16 > 30)
    {
      rsi_dbm = rsi_dbm + 6 ;
      rssi_int16 = rssi_int16 >> 1 ;
    }
    rsi_dbm = rsi_dbm + ((417*rssi_int16 + 18080)>>10) ;
  }
  
  return rsi_dbm ;
}




#if 0
/**
* @brief  Select which timer should be employed.
*         If wakeupTime is less than 1.2 ms Timer1 will be employed,
*         otherwise wakeupTime will be employed.
* @param  wakeupTime
* @retval uint8_t: BLE_STATUS_SUCCESS or NO_DEEP_SLEEP
*/
static uint8_t timerSelect(uint32_t wakeupTime)
{
  int32_t time2 = BLUE_CTRL->CURRENT_TIME ;
  int32_t timediff = (wakeupTime - time2) & 0xffffff ;
  
  /* more than 1.2 ms in the future */
  if(timediff > (blueglob->wakeup_time_offset << 3) + machine_time_translate(SLEEP_TIME_MIN) && timediff < 0x800000) {
    return BLE_STATUS_SUCCESS;
  }
  else {
    return NO_DEEP_SLEEP;
  }
}
#endif



/**
* @brief  write_radio_config.
* @param  ptr
* @retval None
*/
static void write_radio_config(char *ptr)
{
  /* rc_config[0] = 0x005b3402 ; // turn on X-tal */
  int var11 = ((int)ptr & 0xffff) | 0x10000 ;
  BLUE_CTRL->RADIO_CONFIG = var11 ;
  int i = 0 ;
  while((BLUE_CTRL->RADIO_CONFIG & 0x10000) != 0)
  {
    i = i+1 ;
  }
}

/**
* @}
*/



/** @defgroup Radio_Exported_Functions Exported Functions
* @{
*/


/**
* @brief  Radio ISR. Completed event is handled here.
*         Besides, next packet is scheduled here.
* @param  None
* @retval None
*/
void RADIO_IRQHandler(void)
{
  uint32_t int_value = BLUE_CTRL->INTERRUPT;
  
  if ( (int_value & IRQ_DONE) != 0) {
    /* Indicates that there is no scheduling for wakeup Timer */
    globalParameters.wakeupTime = 0;
    
    /* Restore period_slow since it is rewritten by hardware */
    blueglob->period_slow = globalParameters.period_slow_patch;
    
    /* if (!Is_Flash_Write_On_Going())
    {
      BLUE_CTRL->RADIO_CONFIG = ((int)GVAR(rssi_level) & 0xffff) | 0x10000;
    } */
    BLUE_CTRL->RADIO_CONFIG = ((int)(globalParameters.rssiLevel) & 0xffff) | 0x10000;
    
    
    ActionPacket* next, *actionPacketBackup;
    BlueTransStruct *p;
    
    /* Copy status in order for callback to access it. */ 
    globalParameters.current_action_packet->status = int_value;
    
    if( ( globalParameters.current_action_packet->condRoutine(globalParameters.current_action_packet) ) == TRUE) {
      next = globalParameters.current_action_packet->next_true;
    }
    else {
      next = globalParameters.current_action_packet->next_false;
    }
    
    if(next == NULL_0) {
      /* timer2 off */
      BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE;
      blueglob->Config = 0;
    }
    else {
      blueglob->Config = (blueglob->Config & 0xFFF0) | (next->StateMachineNo | 0x48);
      p= &next->trans_packet;
      (bluedata + next->StateMachineNo)->stat_config  = next->trans_config;
      (bluedata + next->StateMachineNo)->txpoint  = BLUE_STRUCT_PTR_CAST(p);
      (bluedata + next->StateMachineNo)->rcvpoint  = BLUE_STRUCT_PTR_CAST(p);
      
#ifdef BLUENRG2_DEVICE
    (bluedata + next->StateMachineNo)->remap_chan = (bluedata+next->StateMachineNo)->remap_chan | 0x80;
#endif

      /* Packet will execute on time specified by WakeupTime */
      if( (next->ActionTag & TIMER_WAKEUP ) !=0 ) {
        /* timer2 off */
        BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE;
        
        /* program timer at next->wakeuptime */
        if((next->ActionTag & RELATIVE ) !=0) {
          globalParameters.wakeupTime = BLUE_CTRL->CURRENT_TIME;
          globalParameters.wakeupTime = globalParameters.wakeupTime + us_time_translate(next->WakeupTime);
          /* Adding the correction factor 1.1875*HS_STARTUP_TIME + HW_OFFSET for all packet except first packet for XC oscillator */
          globalParameters.wakeupTime = globalParameters.wakeupTime -((IFR->hs_startup_time) + (IFR->hs_startup_time>>2) - (IFR->hs_startup_time>>4)) - ANALOG_HW_OFFSET - TIMING_ERROR_CORRECTION;

          globalParameters.wakeupTime =(globalParameters.wakeupTime & 0x00FFFFFF);
        }
        else {
          globalParameters.wakeupTime = next->WakeupTime;
          globalParameters.wakeupTime =(globalParameters.wakeupTime & 0x00FFFFFF);
        }
        
        BLUE_CTRL->TIMEOUT = (globalParameters.wakeupTime | WAKEUP_TIMER_TRIGGER | BLUE_SLEEP_ENABLE);
        
     /*   if(timerSelect(next->WakeupTime) == BLE_STATUS_SUCCESS) {
          blue->Timeout = (globalParameters.wakeupTime | WAKEUP_TIMER_TRIGGER | BLUE_SLEEP_ENABLE);
        }
        else {
          blue->Timeout = globalParameters.wakeupTime | TIMER1_TIMER_TRIGGER;
        }*/
      }
      /* back to back */
      else {
      }
    }  
    
    /* Accept the packet even with CRC Error */
    if(   ( (int_value & IRQ_RCV_OK) != 0) || ( (int_value & IRQ_CRC_ERR) != 0) ) {
      /* read RSSI */
      globalParameters.current_action_packet->rssi =  read_RSSI();
      
      /* bluedata->config = bluedata->config ^ 0x80 ;  toggle NESN bit
      bluedata->config = bluedata->config & 0x7F ;  //reset NESN bit */
      
      /* read time stamp */
      globalParameters.current_action_packet->timestamp_receive = BLUE_CTRL->TIMER_CAPTURE;
    }
    
    actionPacketBackup = globalParameters.current_action_packet;
    globalParameters.current_action_packet = next;
    actionPacketBackup->dataRoutine(actionPacketBackup, next);
  }
  
  if ( (int_value & IRQ_WAKEUP_2) != 0) {
    RADIO_HostTimerCallback();
  }
  if (int_value & BIT_TIME_OVERRUN) {
    // Timer overrun!!!
    RADIO_TimerOverrunCallback();
  }
  
  /* clear interrupts */
  BLUE_CTRL->INTERRUPT = int_value;
  
  return ;
}

/**
* @brief  This routine force the radio to be stopped. After calling this function,
*         the ISR is not triggered unless, "MakeActionPacketPending" API called again.
* @param  None
* @retval TRUE
*/
uint8_t RADIO_StopActivity(void)
{
  MASK_INTERRUPTS();
  blueglob->Config = 0; 
  BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE;
  globalParameters.current_action_packet = 0;
  globalParameters.forceRadiotoStop = TRUE;
  
  UNMASK_INTERRUPTS();
  return TRUE;     
}


/**
* @brief  This routine sets the 40-bit receive and transmit packet count, to be used in
*         encryption calculation.
*         Both are pointers to char, and set the 39-bit counter + 1-bit MSB as defined in the
*         Bluetooth Low Energy spec. One is for transmit packets, the other is for receive packets.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  count_tx: 40-bit transmit packet count, to be used in encryption nounce calculation.
* @param  count_rcv: 40-bit receive packet count, to be used in encryption nounce calculation.
* @retval None
*/
void RADIO_SetEncryptionCount(uint8_t StateMachineNo, uint8_t *count_tx, uint8_t *count_rcv) 
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));
  
  for(uint8_t i = 0; i < 5; i++) {
    (bluedata + StateMachineNo)->Pack_count_Rcv[i] = count_rcv[i];
    (bluedata + StateMachineNo)->Pack_count_Tx[i] = count_tx[i];
  }
  return; 
}


/**
* @brief  This routines sets the 8-byte encryption initial vector, and the 16-byte encryption key.
*         The definition of the encryption key and its initial value is according the Bluetooth Low Energy spec.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  enc_iv: 8-byte encryption initial vector.
* @param  enc_key: 16-byte encryption key.
* @retval None
*/
void RADIO_SetEncryptionAttributes(uint8_t StateMachineNo, uint8_t *enc_iv, uint8_t *enc_key) 
{   
  uint8_t i = 0;
  
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));
  
  for(i = 0; i < 8; i++) {
    (bluedata + StateMachineNo)->enc_iv[i] = enc_iv[i];
    (bluedata + StateMachineNo)->enc_key[i] = enc_key[i];
  }
  for(i = 8; i < 16; i++) {
    (bluedata + StateMachineNo)->enc_key[i] = enc_key[i];
  }
  
  return ; 
}


/**
* @brief  This routine sets the channel map. If channel map is not in use, do not use it. The
*         chan_remap constant is a 37-bit vector, one bit per channel from 0 to 36, with the
*         LSB corresponding to the channel 0. If the corresponding bit is set, the channel is in use; if it is cleared,
*         there is an automatic remap to another channel, as defined in the Bluetooth Low
*         Energy specification. 
*         Setting all bits of the array to one, disables the channel remapping, and this is the mode
*         that should be used if the Bluetooth channel remap is not in use.
*
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  chan_remap: a 37-bit vector, one bit per channel from 0 to 36, with the LSB corresponding to the channel 0 
* @retval None
*/
void RADIO_SetChannelMap(uint8_t StateMachineNo,uint8_t *chan_remap)
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));  
  
  for(uint8_t i = 0; i < 5; i++) {
    (bluedata + StateMachineNo)->chan_remap[i] = chan_remap[i];
  }     
  return ; 
}


/**
* @brief  This routine sets the channel and the channel increment. Channel and channel
*         increment follows channel mapping corresponding to Bluetooth specification.
*         RF channel 0:  2402 MHz -> channel 37
*         RF channel 1:  2404 MHz -> channel 0
*         RF channel 2:  2406 MHz -> channel 1
*         ...
*         RF channel 11: 2424 MHz -> channel 10
*         RF channel 12: 2426 MHz -> channel 38
*         RF channel 13: 2428 MHz -> channel 11
*         RF channel 14: 2430 MHz -> channel 12
*         ...
*         RF channel 38: 2478 MHz -> channel 36
*         RF channel 39: 2480 MHz -> channel 39
*
*         For the channel 37, 38 and 39 (RF channel 0, 12, 39):
*          - The crc_init value should always be 0x555555.
*          - There is no encryption.
*          - The channel increment is dedicated, the channel sequence is:
*            37 -> 38 -> 39 -> 37 -> ... while for the other channels is:
*            0 -> 1 -> ... -> 36 -> 0 -> ...
*            There is no channel map for these three channels.
*            Valid values for packet length for these channels are from 6 to 37.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  channel: frequency channel. From 0 to 39.
* @param  channel_increment: determines the hoping value.
* @retval None
*/
void RADIO_SetChannel(uint8_t StateMachineNo, uint8_t channel, uint8_t channel_increment) 
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo)); 
  assert_param(IS_RFCHANNEL_VALID(channel)); 
  
  (bluedata + StateMachineNo)->unmapped_chan = channel;  
  (bluedata + StateMachineNo)->hop_incr = channel_increment;
  
  return ; 
}


/**
* @brief  This routine sets the NetworkID, the CRC init, and the SCA.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  NetworkID: The NetworkID is the ID of the device.
*         The user shall ensure that the NetworkID meets the following requirements:
*           - It shall have no more than six consecutive zeros or ones.
*           - It shall not have all four octets equal.
*           - It shall have no more than 24 transitions.
*           - It shall have a minimum of two transitions in the most significant six bits.   
* @param  crc_init: CRC initilization value.
*         This value must be 0x555555 when channel frequency is one of these values: 37, 38, 39.
* @param  sca: slow clock accuracy. Period duration of the 32 kHz clock (slow clock).
*              "period_slow" defines the number of 16 MHz clock cycle in a 32 kHz period. 
*              Note: the number 16 MHz clock cycle in a 32 kHz period can be measured by the clockgen block. 
*              Note: If the application uses a CLK32K crystal then the value is a constant provided by the firmware. (488 = 16,000,000 / 32768) 
*              Note: If the application uses the 32 kHz RC clock, then its value is measured in the clock_gen. 
*         Note: not used here. This value should be set automatically in RADIO_CrystalCheck routine. 
* @retval None
*/
void RADIO_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID, uint32_t crc_init, uint32_t sca)
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo)); 
  
  (bluedata + StateMachineNo)->access_address = NetworkID;
  (bluedata + StateMachineNo)->crc_init = crc_init;      
  
  /* blueglob->period_slow = sca; */
  return; 
}



/**
* @brief  Initializes the time between back-to-back radio transmissions.
* @param  back_to_back_time: time between two packets if wakeupTimer is not used. Resolution is 1 us.
* @retval None
*/
void RADIO_SetBackToBackTime(uint32_t back_to_back_time)
{
  globalParameters.back2backTime = back_to_back_time;
  return;
}


/**
* @brief  Configures the transmit power level.
* @param  PowerLevel: power level which should set to this value:
*          - 0 = -18 dBm,
*          - 1 = -15 dBm,
*          - 2 = -12 dBm,
*          - 3 = -9 dBm,
*          - 4 = -6 dBm,
*          - 5 = -2 dBm,
*          - 6 =  0 dBm,
*          - 7 =  5 dBm. 
*
*          - 8 = -14 dBm,
*          - 9 = -11 dBm,
*          - 10 = -8 dBm,
*          - 11 = -5 dBm,
*          - 12 = -2 dBm,
*          - 13 =  2 dBm,
*          - 14 =  4 dBm,
*          - 15 =  8 dBm.
* @retval None
*/
void RADIO_SetTxPower(uint8_t PowerLevel)
{
  /* Check the parameters */
  assert_param(IS_POWERLEVEL_VALID(PowerLevel)); 
  
  PowerLevel <<= 3;
  for(int n = 0; n <STATEMACHINE_COUNT ; n++) {
    (bluedata+n)->conf_byte5 = PowerLevel;
  }
  return;
}

  
/**
* @brief  This routine turns encrypt ON and OFF. When the encryption is enabled, the hardware will add 4 bytes
*         at the end of the packet as MIC (Message Authentication Code). So the user need to add 4 byte to the
*         length of the packet. The parameters enable the encryption for both. There are two separated parameters
*         for this function just compliance with previous version of the driver.
* @param  StateMachineNo: state machine number in multi state. From 0 to 7.
* @param  EncryptFlagTx: DISABLE: encryption is turned OFF for both TX and RX operations.
*              ENABLE: encryption is turned OFF for both TX and RX operations.
*         This parameter can be: ENABLE or DISABLE.
* @param  EncryptFlagRcv: encryption is turned OFF for both TX and RX operations.
*              ENABLE: encryption is turned OFF for both TX and RX operations.
*          This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void RADIO_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlagTx, FunctionalState EncryptFlagRcv)
{
  /* Check the parameters */
  assert_param(IS_STATE_VALID(StateMachineNo));
  assert_param(IS_FUNCTIONAL_STATE(EncryptFlagTx));
  assert_param(IS_FUNCTIONAL_STATE(EncryptFlagRcv));
  
  if(EncryptFlagTx==ENABLE || EncryptFlagRcv==ENABLE) {
    (bluedata + StateMachineNo)->config = (bluedata + StateMachineNo)->config | 0x0A;
  }
  else {
    (bluedata + StateMachineNo)->config = (bluedata + StateMachineNo)->config & 0xF5;
  }
  return;
}


/**
* @brief  This routine should be called after writing/modifying an action packet, and before it
*         is executed via either the API mechanism, or the next mechanism.
* @param  p: pointer to action packet.
* @retval None
*/
void RADIO_SetReservedArea(ActionPacket *p)
{
  //p->WakeupTime = (uint32_t) (p->WakeupTime * 0.525); /* convert us to appropriated value.  1000 * 1/512000 = 1953.125 */
  
  /* Check the parameters */
  assert_param(IS_DELAY_VALID(p->WakeupTime));
   
  p->trans_packet.datptr = BLUE_DATA_PTR_CAST(p->data);
  
  /* rcvlen[17:0] */
  if(p->ReceiveWindowLength < 0x40000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength)       & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 8)  & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 16) & 0x00000003 ;
  }
  else if(p->ReceiveWindowLength < 0x100000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength >> 2)  & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 10) & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 18) & 0x00000003 ;
    
    p->trans_packet.rcvlen[2] |= 0x04;
  }
  else if(p->ReceiveWindowLength < 0x400000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength >> 4)  & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 12) & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 20) & 0x00000003 ;
    
    p->trans_packet.rcvlen[2] |= 0x08;  
  }
  else if(p->ReceiveWindowLength < 0x1000000) {
    p->trans_packet.rcvlen[0] =   (p->ReceiveWindowLength >> 6)  & 0x000000FF ;
    p->trans_packet.rcvlen[1] =   (p->ReceiveWindowLength >> 14) & 0x000000FF ;
    p->trans_packet.rcvlen[2] =   (p->ReceiveWindowLength >> 22) & 0x00000003 ;
    
    p->trans_packet.rcvlen[2] |= 0x0C; 
  }
  else {
    /* error */
  }
  
  if((p->ActionTag & TIMESTAMP_POSITION) == 0) {
    p->trans_packet.rcvlen[2] |= TIMESTAMP_POSITION_LASTBIT;
  }
  else {
    p->trans_packet.rcvlen[2] |= TIMESTAMP_POSITION_ACCESSADDRESS;
  }
  
  /* p->trans_packet.rcvlen[2] = p->trans_packet.rcvlen[2] | 0x30 | 0x40 | TIMESTAMP_POSITION; */
  p->trans_packet.rcvlen[2] = p->trans_packet.rcvlen[2] | 0x30;
  
  p->trans_packet.timeout[0] = (globalParameters.back2backTime >> 0)  & 0x000000FF;
  p->trans_packet.timeout[1] = (globalParameters.back2backTime >> 8)  & 0x000000FF;
  p->trans_packet.timeout[2] = (globalParameters.back2backTime >> 16) & 0x0000000F; /* only 20 bits */
  
  p->trans_packet.byte10 =0;
  p->trans_packet.byte10 = (p->ActionTag & INC_CHAN) | (p->ActionTag & PLL_TRIG) | (p->ActionTag & TXRX);
  
  /* next state is zero */
  p->trans_packet.byte10 = p->trans_packet.byte10 & 0x1F;
  
  /* sleep . do not go to sleep*/
  p->trans_packet.byte10 = p->trans_packet.byte10 & 0xFB;
  
  /* for multi-state automatically increment the channel */
  p->trans_packet.byte10 = p->trans_packet.byte10 | ((p->StateMachineNo)<<5);

  /* enable all interrupts */
  p->trans_packet.byte11 = 0xFF;
  
  p->trans_config =0;
  if( (p->ActionTag & TXRX) != 0) {
    p->trans_config = 0x80;
  }
  
  return ;
}


/**
* @brief  This routine should be called for the first actionPacket that needs to be scheduled
*         on the radio. For it to work OK, the WakeupTime should be valid, and the TIMER_WAKEUP flag set.
*         Subsequent packets can be dispatched using the same process, or by providing non-NULL pointers
*         to the next_true and next_false pointers of the ActionPacket.
*
* @param  p: pointer to action packet.
* @retval uint8_t with following values:
*          - 0x00 : Success.
*          - 0xC4 : Radio is busy, action packet has not been executed.
*/
uint8_t RADIO_MakeActionPacketPending(ActionPacket *p)
{
  uint8_t returnValue = SUCCESS_0;
  uint32_t dummyTime ;
  
  if(RADIO_GetStatus(&dummyTime) == BLUE_IDLE_0)
  {
    globalParameters.forceRadiotoStop = FALSE;
    uint16_t  statemachineNo;
    BlueTransStruct *p1 ; 
    
    /* timer2 off */
    BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE;
    globalParameters.rssiLevel[0] = 0x2e86 ;
    globalParameters.rssiLevel[1] = 0 ;
    
    int32_t* temp ;
    temp = globalParameters.hot_ana_config_table_a;
    blueglob->RadioConfigPtr = BLUE_DATA_PTR_CAST(temp) ; /* to be discussed */
    
    /* Set_Radio_Config_Data(); */   /* to be discussed */
    
    /* tell the schedular which table in SRAM should be applied and activate it */
    statemachineNo = 0x0007 & p->StateMachineNo;
    
    blueglob->Config = (blueglob->Config & 0xFFF0) | (statemachineNo | 0x48);
    
    (bluedata+statemachineNo)->txpoint_prev = 0;
    (bluedata+statemachineNo)->rcvpoint_prev = 0;
    (bluedata+statemachineNo)->txpoint_next = 0;

    p1= &p->trans_packet;
    (bluedata+statemachineNo)->txpoint =  BLUE_STRUCT_PTR_CAST(p1);
    (bluedata+statemachineNo)->rcvpoint = BLUE_STRUCT_PTR_CAST(p1);
    
#ifdef BLUENRG2_DEVICE
    (bluedata+statemachineNo)->remap_chan = (bluedata+statemachineNo)->remap_chan | 0x80;
#endif

    globalParameters.current_action_packet = p;
    (bluedata+statemachineNo)->stat_config = p->trans_config; /* Transmision or reception for first packet */
    
    /* program timer at next->wakeuptime */
    MASK_INTERRUPTS();
    if((p->ActionTag & RELATIVE ) !=0) {
      globalParameters.wakeupTime = BLUE_CTRL->CURRENT_TIME;
      /* Adding the correction factor 1.1875*HS_STARTUP_TIME + ANALOG_HW_OFFSET for the first packet */
      globalParameters.wakeupTime = globalParameters.wakeupTime + us_time_translate(p->WakeupTime) - (IFR->hs_startup_time + (IFR->hs_startup_time>>2)-(IFR->hs_startup_time>>4)) - ANALOG_HW_OFFSET;
      globalParameters.wakeupTime =(globalParameters.wakeupTime & 0x00FFFFFF);
    }
    else {
      globalParameters.wakeupTime = p->WakeupTime;
      globalParameters.wakeupTime =(globalParameters.wakeupTime & 0x00FFFFFF);
    }
    BLUE_CTRL->TIMEOUT = (globalParameters.wakeupTime | WAKEUP_TIMER_TRIGGER | BLUE_SLEEP_ENABLE);    
    UNMASK_INTERRUPTS();
  }
  else {
    returnValue = RADIO_BUSY_C4;
  }
  
  return returnValue;
}


/**
* @brief  This function performs the automatic calibration procedure on the BLE period_slow hardware register when
*         the value of hardware_config.ls_source variable is non-zero. In this case, also
*         the internal BLE stack timing reference is tuned accordingly.
*         When the hardware_config.ls_source is zero, the period_slow register value is set on the
*         basis of the LS_CRYSTAL_PERIOD macro. In this case, the internal BLE stack timing reference is
*         set according to the LS_CRYSTAL_FREQ and LS_CRYSTAL_PERIOD macros.
*         In any cases, the resulting internal BLE stack timing reference is used to set the BLE wakeup_time_offset
*         hardware register according to the hardware_config.hs_startup_time content.
* @param  None
* @retval None
*/
void RADIO_CrystalCheck(void)
{
  Clk32Context_t *Clk32Context_p = &(globalParameters.Clk32Context);
  BOOL new_computation_flag = Clk32Context_p->period == -1;    
  if(IFR->ls_source != 0) /* using internal RO */
  {
    if (new_computation_flag)
    {
      int32_t freq1 = 0 ;
      int32_t var1 = 0;
      int32_t freq2;
      do{
        freq2 = freq1 ;
        freq1 = CKGEN_BLE->CLK32K_FREQ; /*reading frequency from hardware */
        var1++;
      }while((freq1 != freq2 || freq1 == 0) && var1 < 5000); /* repeat until two consecutive reading are equal */
      Clk32Context_p->freq = freq1;
      Clk32Context_p->period = CKGEN_BLE->CLK32K_PERIOD;
      Clk32Context_p->calibr.Time_mT = GET_CUR_TIME;
      BlueNRG_Start_Crystal_Measurement();
    }
    else{
      uint32_t current_time_mT = GET_CUR_TIME;
      if ((Clk32Context_p->calibr.started_flag != 0) && (CKGEN_BLE->CLK32K_IT != 0) )
      {
        Clk32Context_p->freq = CKGEN_BLE->CLK32K_FREQ;
        Clk32Context_p->period = CKGEN_BLE->CLK32K_PERIOD;
        Clk32Context_p->calibr.started_flag = 0;
        Clk32Context_p->calibr.Time_mT = current_time_mT;
        new_computation_flag = TRUE;
      }
      else{
        if ((Clk32Context_p->calibr.started_flag == 0) && (((uint32_t)TIME24_DIFF(current_time_mT, Clk32Context_p->calibr.Time_mT)) > (1000<<9)))/* [RG:] about 1sec +/-20% */
        {
          BlueNRG_Start_Crystal_Measurement();
        }
      }
    }
  }
  else {  /* using external Crystal */    
    Clk32Context_p->period = 0; 
  }
  
  if (new_computation_flag == TRUE )
  {
    set_freq(Clk32Context_p->freq,Clk32Context_p->period) ;
  }
  
}

/**
* @brief  Initializes the radio.
* @param  hs_startup_time: start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us).
* @param  low_speed_osc: source for the 32 kHz slow speed clock: 1: internal RO; 0: external crystal.
* @param  hot_table: reserved for future use, set to NULL.
* @param  whitening: DISABLE: disable whitening for TX and RX. ENABLE: enable whitening for TX and RX.
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void RADIO_Init(uint16_t hs_startup_time, uint8_t low_speed_osc, uint32_t* hot_table, FunctionalState whitening)
{     
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(whitening));
    
  DISABLE_INTERRUPT(IRQBLUE);
  
  blueglob->TxDelay = TX_DELAY;
  blueglob->TxDelay1 = TX_DELAY1;        
  blueglob->RcvDelay = RX_DELAY;
  blueglob->RcvDelay1 = RX_DELAY1;
  
  if(whitening == DISABLE) {
    /* disable Transmision Whitening */
    blueglob->TxDelay |= 0x80;
    blueglob->TxDelay1 |= 0x80; 
    
    /* disable Receiving Whitening */
    blueglob->RcvDelay |= 0x80;
    blueglob->RcvDelay1 |= 0x80; 
  }
  
  globalParameters.hardware_config.hs_startup_time = hs_startup_time;
  globalParameters.hardware_config.ls_source = low_speed_osc;
  globalParameters.hardware_config.hot_ana_config_table = hot_table;
  
  blueglob->Config = 0;
  BLUE_CTRL->TIMEOUT = TIMER_OFF | BLUE_SLEEP_ENABLE ;
  BLUE_CTRL->HOST_WKUP_TIMER = TIMER_OFF  | (1<<OFF_R_SLEEP);
  int int_value = BLUE_CTRL->INTERRUPT ;
  BLUE_CTRL->INTERRUPT = int_value ; 
  
  blueglob->period_slow = SCA_DEFAULTVALUE;
  
#if BLUENRG_START_CRYSTAL_MEASUREMENT_CALLED_EXTERNALLY != 0   
  if (Clk32Context.calibr.started_flag != 1){ 
    CKGEN_BLE->CLK32K_IT = 1;    
    BlueNRG_Start_Crystal_Measurement();
  }
#else    
  CKGEN_BLE->CLK32K_COUNT = 23 ;  
#endif
  
  globalParameters.back2backTime = 150; /* 150 micro second */
  globalParameters.forceRadiotoStop = FALSE;
  globalParameters.powerUpfirstPacket = 1;
  globalParameters.Clk32Context.period = -1;
  
  RADIO_CrystalCheck();
  
  RADIO_SetTxPower(11);
  RADIO_SetEncryptFlags(0, DISABLE, DISABLE);
  
  //blueglob->wakeup_time_offset= 0x07;  /* 30.51us */
  
  ENABLE_INTERRUPT(IRQBLUE);
  ENABLE_INTERRUPT(IRQNVM);
  
  return;
}


/**
* @brief  This routine returns the status of the Radio whether it is busy or not.
*         If it is busy, the time argument will be filled with absolute time value of next radio activity.
*         User can use this value (in comparison with current time) to check that 
*         how long it is far to force the device to sleep or not.
*         Note: when Timer2 is on schedule, time value is not absolute time.
* @param  time: pointer to int value which is going to have time value.
* @retval uint8_t with following values:
*          - 0 : BLUE_IDLE_0. Radio is not busy.
*          - 1 : BLUE_BUSY_NOWAKEUP_T1. Radio is busy, but there is no wakeup timer on schedule but Timer1 is.
*          - 2 : BLUE_BUSY_NOWAKEUP_T2. Radio is busy, but there is no wakeup timer on schedule but Timer2 is.
*          - 3 : BLUE_BUSY_WAKEUP. Radio is busy and wakeup timer is on the schedule.
*/
uint8_t RADIO_GetStatus(uint32_t *time)
{
  uint8_t retValue =  BLUE_IDLE_0; 
  uint32_t timeOut;
  if((blueglob->Config & 0x8) != 0)
  {
    timeOut = BLUE_CTRL->TIMEOUT;
    *time = timeOut;
    timeOut = (timeOut >> 24) & 0x07;
    if(timeOut == TIMER2_TIMER_MASK)
    {
      retValue =  BLUE_BUSY_NOWAKEUP_T2;
    }
    else if(timeOut == TIMER1_TIMER_MASK)
    {
      retValue =  BLUE_BUSY_NOWAKEUP_T1;
    }
    else if(timeOut == WAKEUP_TIMER_MASK)
    {
      retValue =  BLUE_BUSY_WAKEUP;
    }
    
    /* *time =globalParameters.wakeupTime; */     
  }       
  
  return retValue;
}


/**
* @brief  Starts tone transmission on selected channel.
*         This API is dedicated for test and destroys context and multistate.
*         So, after calling this function the radio should be re-initialized.
* @param  RF_Channel: radio frequency channel number from 0 to 39.
* @param  PowerLevel: power level which should set to this value:
*          - 0 = -18 dBm,
*          - 1 = -15 dBm,
*          - 2 = -12 dBm,
*          - 3 = -9 dBm,
*          - 4 = -6 dBm,
*          - 5 = -2 dBm,
*          - 6 =  0 dBm,
*          - 7 =  5 dBm. 
*
*          - 8 = -14 dBm,
*          - 9 = -11 dBm,
*          - 10 = -8 dBm,
*          - 11 = -5 dBm,
*          - 12 = -2 dBm,
*          - 13 =  2 dBm,
*          - 14 =  4 dBm,
*          - 15 =  8 dBm.
* @retval None
*/
void RADIO_StartTone(uint8_t RF_Channel, uint8_t powerLevel)
{
  /* Check the parameters */
  assert_param(IS_RFCHANNEL_VALID(RF_Channel)); 
  assert_param(IS_POWERLEVEL_VALID(powerLevel));
  
  uint8_t channel;
  uint8_t value1;
  uint8_t value2;
  uint32_t dummy;
  char data[24];
  
  if (RADIO_GetStatus(&dummy) == BLUE_IDLE_0) {
    /* [EM:] Calculate the synt divide factor for 16 MHz quarts and +250 kHz offset wrt the channel center frequency
    *  Algorithmically MAK = F_rf*(2^20)/16 = F_rf*(2^16)
    *  With F_rf = (2402+2*RF_Channel)+0.25 MHz
    *  K corresponds to b19-b0  of MAK
    *  A corresponds to b22-b20 of MAK
    *  M corresponds to b27-b23 of MAK
    */
    uint32_t kHz_250_scaled = 16384;    /* = 0.250*2^20/16 */
    uint32_t MAK = ((2402+2*RF_Channel)<<16) - kHz_250_scaled;
    uint8_t M  = (MAK>>23)&0x1F;                                             
    uint8_t A  = (MAK>>20)&0x07;
    uint32_t K = (MAK&0x000FFFFF)+1;
    uint8_t MOD3_DIG_TEST = (M<<3) | (A & 0x7);
    uint8_t MOD2_DIG_TEST = ((K>>12) & 0xFF);
    uint8_t MOD1_DIG_TEST = ((K>> 4) & 0xFF);
    uint8_t MOD0_DIG_TEST = ((K&0x0F)<<4) | 0x09;
    
    value1  =  0x7E & 0x87;        
    
    value1 |= (powerLevel     << 3);        
    value2  =  0x3F & 0xF1; 
    powerLevel &= 0x07;        
    value2 |= (powerLevel     << 1);
    
    channel = RF_Channel;
    channel <<= 1;
    data[0]  = 0x03;
    data[1]  = 0x04;
    data[2]  = channel;         // addr 0x4, value channel
    data[3]  = value1;          // addr 0x5, value TX power
    data[4]  = 0x02;
    data[5]  = 0x1C;            // addr 0x1C, value 0x73
    data[6]  = 0x73;
    data[7]  = 0x03;
    data[8]  = 0x24;
    data[9]  = value2;          // addr 0x24, value 0x3F
    data[10] = 0x3F;            // addr 0x25, value 0x3F
    data[11] = 0x02;
    data[12] = 0x23;
    data[13] = 0x03;            // addr 0x23, value 0x03
    data[14] = 0x02;
    data[15] = 0x23;
    data[16] = 0x07;            // addr 0x23, value 0x07�
    /*[EM:]	Fix 204: force MAK values through MODx test registers */
    data[17] = 0x05; 
    data[18] = 0x28;
    data[19] = MOD3_DIG_TEST;
    data[20] = MOD2_DIG_TEST;
    data[21] = MOD1_DIG_TEST;
    data[22] = MOD0_DIG_TEST;
    
    data[23] = 0x0;
    
    write_radio_config(data);
  }
}


/**
* @brief  This routine starts tone transmission.
*         This API is dedicated for test and destroys context and multistate.
*         So, after calling this function the radio should be re-initialized.
* @param  None
* @retval None
*/
void RADIO_StopTone()
{
  char data[9];
  
  data[0] = 0x04;
  data[1] = 0x23;
  data[2] = 0x00;             // addr 0x23, value 0x00
  data[3] = 0x0E;             // addr 0x24, value 0x0E
  data[4] = 0x00;             // addr 0x25, value 0x00
  data[5] = 0x02;	      //[EM:]	Fix 204
  data[6] = 0x2B;             // Reg MOD0_DIG_TEST@0x2B
  data[7] = 0x08;             // Reset to default value (disable mod test mode)
  data[8] = 0x00;
  write_radio_config(data);
}


/**
* @brief  Encrypts plain text data using AES encryption with given input key and
*         128 bit block size
* @param  Key: encryption key to be used.
* @param  plainData: text data to be encrypted.
* @param  cypherData: contains the encrypted data.
* @retval None
*/
void RADIO_EncryptPlainData(uint8_t *Key, 
                            uint8_t *plainData,
                            uint8_t *cypherData)
{   
  uint32_t key[4] ;
  uint32_t cleartext[4] ;
  uint32_t ciphertext[4] ;
  volatile uint32_t ii ;
  
  for(uint8_t i = 0; i < 4; i++) {
    key[i] = ((uint32_t)Key[i*4]) | ((uint32_t)Key[i*4+1])<<8 | ((uint32_t)Key[i*4+2])<<16 | ((uint32_t)Key[i*4+3])<<24;
    cleartext[i] = ((uint32_t)plainData[i*4]) | ((uint32_t)plainData[i*4+1])<<8 | ((uint32_t)plainData[i*4+2])<<16 | ((uint32_t)plainData[i*4+3])<<24;
  }
  
  BLUE_CTRL->AES_KEY0 = key[3];
  BLUE_CTRL->AES_KEY1 = key[2];
  BLUE_CTRL->AES_KEY2 = key[1];
  BLUE_CTRL->AES_KEY3 = key[0];
  
  BLUE_CTRL->CLEAR_TEXT0 = cleartext[3];
  BLUE_CTRL->CLEAR_TEXT1 = cleartext[2];
  BLUE_CTRL->CLEAR_TEXT2 = cleartext[1];
  BLUE_CTRL->CLEAR_TEXT3 = cleartext[0];
  
  ii = 0 ;
  while( ( BLUE_CTRL->STATUS & BIT_AES_READY) == 0 && ii < 100) { 
    ii++ ;}
  
  ciphertext[0] = BLUE_CTRL->AES_CYPHERTEXT3;
  ciphertext[1] = BLUE_CTRL->AES_CYPHERTEXT2;
  ciphertext[2] = BLUE_CTRL->AES_CYPHERTEXT1;
  ciphertext[3] = BLUE_CTRL->AES_CYPHERTEXT0;

  for(uint8_t i = 0; i < 4; i++) {
    cypherData[i*4] = ((uint8_t)ciphertext[i]);
    cypherData[i*4+1] = (uint8_t)(ciphertext[i]>>8);
    cypherData[i*4+2] = (uint8_t)(ciphertext[i]>>16);
    cypherData[i*4+3] = (uint8_t)(ciphertext[i]>>24);
  }
  
  return ;
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

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
