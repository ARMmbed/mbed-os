/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTL8195A_PMU_TASK_H__
#define __RTL8195A_PMU_TASK_H__
#ifdef CONFIG_LITTLE_WIFI_MCU_FUNCTION_THREAD
//BitMAPDefine
#define RATEADAPTIVE        BIT0
#define H2CEVENT            BIT1
#define C2HEVENT            BIT2
#define RATRYDONE           BIT3
#define REMOTEWAKEEVENT     BIT4
#define APOFFLOADEVENT      BIT5
#define MAILBOXEVENT        BIT6
#define SWTIMEREVENT        BIT7

#define BBNHMEVENT          BIT8
#define DBGPKTEVENT         BIT9
#define SIDEBANDWoWLAN      BIT10

#if 0
#ifdef CONFIG_POWER_SAVING
#define BCNEARLY			BIT11
#define MTIBCNIVLEAR		BIT12
#define BCNRX				BIT13
#define RXBMD1				BIT14
#define RXBMD0				BIT15
#define RXUMD1				BIT16
#define RXUMD0				BIT17
#define TXPKTIN				BIT18
#define GTIMER6TO			BIT19
#define GTIMER7TO			BIT20
#endif //#ifdef CONFIG_POWER_SAVING
#endif

//BT mailbox
#define SETDATA BIT2
#define SETACK  BIT1
#define GETDATA BIT0

/*--------------------------Define -------------------------------------------*/
#ifdef CONFIG_POWER_SAVING
#define MACID_CLIENT        0
#endif //#ifdef CONFIG_POWER_SAVING

/*------------------------------Define Enum-----------------------------------*/
#ifdef CONFIG_POWER_SAVING

//REGDUMP_FW_ERR0
typedef enum _FW_ERR0_STATUS_
{
    FES0_H2C_CMDID              = BIT0,
    FES0_H2C_PTR                = BIT1,
    FES0_BB_RW                  = BIT2,
    FES0_TXPKT_TXPAUSE          = BIT3,
    FES0_TSF_STABLE             = BIT4,
    FES0_TXSM_STABLE            = BIT5,
    FES0_RPWM_STABLE            = BIT6,
    FES0_C2H_TIMEOUT_ERR        = BIT7,
    
}FW_ERR0_STATUS, *PFW_ERR0_STATUS;


//TxPauseReasonCode
typedef enum _TRPC_ {
	TPRC_ISSUENULLDATA_1			= 0x26,
	TPRC_ISSUENULLDATA_2			= 0x27,
	TPRC_PSS2TS3                    = 0x2B,
	TPRC_PSS0TS1					= 0x2C,
	TPRC_PSS2TS4                    = 0x2D,
	TPRC_PSS2TS5                    = 0x2E,
    TPRC_PSS0TS6                    = 0x2F,
} TRPC, *PTRPC;


typedef enum _PS_MODE_SETTING_SELECTION_
{
    MODE_SETTING_ACTIVE     = 0,
    MODE_SETTING_LEGACY     = 1,
    MODE_SETTING_WMMPS      = 2,
    #ifdef TDMA_POWER_SAVING
    MODE_SETTING_TDMA       = 3
    #endif //#ifdef TDMA_POWER_SAVING    
}PS_MODE_SETTING_SELECTION, *PPS_MODE_SETTING_SELECTION;

typedef enum _RxListenBeaconMode_
{
    RLBM_MIN                = 0,
    RLBM_MAX				= 1,
    RLBM_SELF_DEFINED		= 2

}RxListenBeaconMode, *PRxListenBeaconMode;

typedef enum _SMART_PS_MODE_FOR_LEGACY_
{
    SMART_PS_MODE_LEGACY_PWR1           = 0,        // TRX all use PS_POLL
    SMART_PS_MODE_TX_PWR0               = 1,        // TX: pwr bit = 0, RX: PS_POLL
    SMART_PS_MODE_TRX_PWR0              = 2         // TX: pwr bit = 0, RX: NULL(0)
}SMART_PS_MODE_FOR_LEGACY, *PSMART_PS_MODE_FOR_LEGACY;

#endif //#ifdef CONFIG_POWER_SAVING

/*--------------------------Define MACRO--------------------------------------*/

#define HAL_WL_READ32(addr)     \
                HAL_READ32(WIFI_REG_BASE, addr)
#define HAL_WL_WRITE32(addr, value)     \
                HAL_WRITE32(WIFI_REG_BASE, addr, value)
#define HAL_WL_READ16(addr)     \
                HAL_READ16(WIFI_REG_BASE, addr)
#define HAL_WL_WRITE16(addr, value)     \
                HAL_WRITE16(WIFI_REG_BASE, addr, value)
#define HAL_WL_READ8(addr)     \
                HAL_READ8(WIFI_REG_BASE, addr)
#define HAL_WL_WRITE8(addr, value)     \
                HAL_WRITE8(WIFI_REG_BASE, addr, value)

#ifdef CONFIG_POWER_SAVING
#define mtou(x) ((x)<<10) //ms->us

#define WAIT_TSF_STABLE_BREAK_CNT       5000
#define WAIT_TSF_STABLE_CNT             50
#define WAIT_TSF_STABLE_ONCE_TIME       20
#define TSFIS32K                        1
#define TSFIS40M                        0
#define GET_TSF_STATE() (((HAL_WL_READ16(0xF0) & BIT8) && (HAL_WL_READ16(0xF0) & BIT9)) ? TSFIS32K : TSFIS40M)


#define REG_ARFR5_8723B				0x04A4
#define WAIT_TXSM_STABLE_CNT		1000
#define WAIT_TXSM_STABLE_ONCE_TIME	50

#define MODE_TIMER                  1
#define MODE_COUNTER                0


#define GTIMER6						6
#define GTIMER7						7

#define TIMER_BCNTO					GTIMER6 //6
#define TIMER_DTIM					GTIMER6 //6
#define TIMER_CHECKSTATE            GTIMER6 //6
#define TIMER_PSTRX					GTIMER7 //7
#ifdef TDMA_POWER_SAVING
#define TIMER_TDMA					GTIMER7 //7
#endif //#ifdef TDMA_POWER_SAVING

#define RTY_LMT_NULLDATA            8
#define RTY_LMT_PSPOLL				24
#define RTY_LMT_MORE_NULLDATA       24


/*
    PS_RX_INFO[7:0]: Power Save RX Information Register
    initial value: 0x00
    REG III.220 (Offset 0x 0692h) PS_RX_INFO Register Definition    
*/
#define RXDATAIN0               BIT0  //PSTX
#define RXDATAIN1               BIT1  //PSRX
#define RXDATAIN2               BIT2
#define RXMGTIN0                BIT3
#define RXCTRLIN0               BIT4

//CPWM Definition
#define CLK_DOWN_RDY            BIT4

//Power Save Tuning Parameter
//#if IS_CATEGORY_WOWLAN(CONFIG_CATEGORY_SEL)
//#define DEFAULT_BCN_TO_LIMIT    5 // 1
//#define DEFAULT_BCN_TO_PERIOD   8   //5
//#else
#define DEFAULT_BCN_TO_LIMIT    2  // 1
#define DEFAULT_BCN_TO_PERIOD   4   //5
//#endif

#define DEFAULT_BCN_TO_TIMES_LIMIT  2 // 20140806
#define DEFAULT_DTIM_TIMEOUT    15  // 7         // 7 ms
#define DEFAULT_PS_TIMEOUT      15  // 20       // 20 ms
#define DEFAULT_PS_DTIM_PERIOD  7
#define DEFAULT_PS_DRV_EARLY    2
#define DEFAULT_ENTER32K_TIMER  1000 //us
//#define PS_DRV_BCN_SHIFT_MAX   DEFAULT_PS_DRV_EARLY-1


#define NULL_DATA0_ALLOW        1
#define NULL_DATA0_DENY         0

#define PS_RF_OFF_8723B			0	
#define PS_GO_ON                BIT0
#define PS_TX_NULL              BIT1
#define PS_RF_ON                BIT2
#define PS_REGISTER_ACTIVE      BIT3
//#define PS_ACK                  BIT6
//#define PS_TOGGLE               BIT7


#define PS_STATE_MASK           (0x0F)
//#define PS_STATE(x)             (PS_STATE_MASK & (x))
#define PS_IS_TX_NULL(x)        ((x) & PS_TX_NULL )
//#define PS_IS_ACK(x)            ((x) & PS_ACK ) 
#define PS_IS_CLK_ON(x)         ((x) & (PS_RF_OFF_8723B |PS_ALL_ON ))
#define PS_IS_RF_OFF(x)         ((x)|PS_RF_OFF_8723B)
//#define PS_IS_RF_ON(x)          ((x) & (PS_RF_ON))
//#define PS_IS_ACTIVE(x)         ((x) & (PS_REGISTER_ACTIVE))

#define PS_STATUS_S0            (PS_REGISTER_ACTIVE | PS_RF_ON)                    //(1,1,0) all on = register active + rf on
#define PS_STATUS_S1            (PS_REGISTER_ACTIVE | PS_RF_ON | PS_TX_NULL)        //(1,1,1) all on + tx null(1)
#define PS_STATUS_S2            (PS_RF_ON)                                         //(0,1,0) register sleep + rf on
#define PS_STATUS_S3            (PS_RF_ON | PS_TX_NULL)                            //(0,1,1) register sleep + rf on + tx null(0)
#define PS_STATUS_S4            0                                                 //(0,0,0) all OFF
#define PS_STATUS_S5            (PS_TX_NULL )                                      //(0,0,1) SCAN = register sleep + rf on + scan enable
#define PS_STATUS_S6            (PS_REGISTER_ACTIVE)                               //(1,0,0) NoA off = register active + rf off


/* DATA FIN Condition Flags */
#define STA_DATA_OPEN           BIT0                                            // indicate that FW open due to TIM = 1 condition. (PS-POLL as trigger frame)
#define BC_DATA_OPEN            BIT1                                            // indicate that FW open due to DTIM = 1 condition.  (BC & MC)
#define QOS_DATA_OPEN           BIT2                                            // indicate that FW open due to UAPSD trigger condition. (QNULL)
    
#define ALL_80211_DATA_OPEN     (STA_DATA_OPEN | BC_DATA_OPEN | QOS_DATA_OPEN)
#define IS_80211_DATA_OPEN(x)   ((x) & ALL_80211_DATA_OPEN)

#define C2H_DATA_OPEN           BIT3                                            // indicate that FW open due to C2H event 
#define IS_C2H_DATA_OPEN(x)     ((x) & C2H_DATA_OPEN)

#define BCN_DATA_OPEN           BIT4
#define APP_DATA_OPEN           BIT5

#define SET_DATA_OPEN(x, type)  ((x) |= (type))
#define CLR_DATA_OPEN(x, type)  ((x) &= (~type))
#define IS_DATA_OPEN(x, type)   ((x) & (type))

//pwr state
#define PS_TYPE_32KPERMISSION       0
#define PS_TYPE_CURRENT_PS_STATE    1
#define PS_TYPE_LASTRPWM            2

#define CCXRPT_START_ADDR       0x0000
#define SW_DEFINE_NULL0         0x123
#define SW_DEFINE_NULL1         0x321
#define SW_DEFINE_OFFSET        6
#define RETRY_OVER              BIT7

#define CCXRPT_OFFSET(x)           (x << 3)

#define WLAN_ENTERCRITICAL()    __disable_irq() 
#define WLAN_EXITCRITICAL()     __enable_irq()

#endif //#ifdef CONFIG_POWER_SAVING

/*------------------------------Define Struct---------------------------------*/
#ifdef CONFIG_POWER_SAVING
typedef struct _PS_PARM_ {

    u8      Enter32KHzPermission;
    u8      bAllQueueUAPSD;   
    u8      ps_dtim_flag;             // indicate dtim of current beacon.
    u8      pstrx_rxcnt_period;
    u8      NoConnect32k;
    u8      ack_last_rpwm;
    u8      TxNull0;
    u8      TxNull1;
    
    u8      TxNull0ok;
    u8      TxNull1ok;
    u8      RfOffLicenseForBCNRx;     //filen: After we received ps_bcn_cnt beacons, we can sleep(rf off).
    u8      BCNAggEn;
    u8      IsGoingTo32K;    
	u8      bMaxTrackingBcnMode;
    u8      BcnTraceDone;
        
/*
    filen: to indicate whether it is smart power saving or not
    0: Legacy PS 
    1: Smart PS(RX use ps_poll)
    2: Smart PS (RX use null_data(0))
*/
    u8      smart_ps:4;                 //enum SMART_PS_MODE
    u8      RLBM:4;                     // RX BCN MODE (min, max, active, ...)
    u8      AwakeInterval;
    u8      ps_mode;                    // ps type (avtive, legacy, wmmps)
    u8      ClkRequestEnable;
    u8      last_rpwm;
    u8      current_ps_state;
    u8      ps_data_open;
    u8      ps_bcn_pass_time;           // fw will only report one beacon information to driver after ps_bcn_pass_time ms. Unit: 100ms

    u8      ps_dtim_period;
    u8      ps_dtim_cnt;
    u8      ps_bcn_to;                  // beacon timeout (ms).
    u8      bcn_to_cnt;                 // indicate the total number of contnuous BCN_TO we have received.
    u8      bcn_to_times_cnt;           //20140806
    u8      min_rate_in_rrsr;
//    u8      lps_control;
    u16     ps_drv_early_itv;           
//    u32     RFECtrl;
    u32     null1_ok_cnt;
    #ifdef TDMA_POWER_SAVING
    u8      SlotPeriod;
    u8      FirstOnPeriod;
    u8      SecondOnPeriod;
    u8      ThirdOnPeriod;
    u8      CurrentSlot;
    BOOLEAN TDMAOnPeriod;
    #endif // #ifdef TDMA_POWER_SAVING

#if 0
    u8      BcnAheadShift;
    u8      BcnEarlyShift;
    u8      BcnEarlyShiftMax;	
    u8      DefaultBcnEarly;
    u8      RxBcnCount;
    u8      TBTTCount;
    u8      CurrentEarly;
    u8      CurrentTimeOut;
    u8      ReachBcnLimitCount;
    u8      BcnDelayInAheadGroupOfAP;
    u8      BcnDelayInRearGroupOfAP;
    u8      BcnDelay[BCN_CALCULATION_MAX];
    u8      XtalDelay;

    u16     TSFOnTBTT;              //unit in TU
    u32     TSFOnRxBcn;
    u32     TSFOnBcnEarly;
#endif

#if 0
#if CONFIG_BCNEARLY_ADJUST
    u8      BcnEalyIndex;
    u8      BcnEarlyAdjustPosition;
    u8      BcnAdjustTogo;
    u8      RxBcnArray[BCN_ADJUST_COUNT];
#endif
#endif
}PS_PARM, *PPS_PARM;

typedef struct _LEGACY_PS_PPARM_ {
    u8      ps_mode:7;
    u8      ClkRequestEnable:1;
    u8      RLBM:4;             	//RX Listen BCN Mode
    u8      smart_ps:4;
    u8      AwakeInterval;      	//Unit: beacon interval, this field is only valid in PS Self-Defined mode
    u8      bAllQueueUAPSD:1;     	// 1: all queue are uapsd 0: not all queue are uapsd
    u8      bMaxTrackingBcnMode:1;
    u8      rsvd:6;
//#if CONFIG_FAST_CPWM
    u8      PwrState;
//#else
//    u8      permission32k:1;
//    u8      rsvd1:7;
//#endif

    u8      LowPwrRxBCN     :1;
    u8      AntAutoSwitch   :1;
    u8      PSAllowBTHighPri:1;
    u8      ProtectBCN      :1;
    u8      SilencePeriod   :1;
    u8      FastBTConnect   :1;
    u8      TwoAntennaEn    :1;
    u8      rsvd2           :1;
    u8      AdoptUserSetting:1;
    u8      DrvBcnEarlyShift :3;
    u8      DrvBcnTimeOut :4;
    #ifdef TDMA_POWER_SAVING
    u8      SlotPeriod;
    u8      FirstOnPeriod;
    u8      SecondOnPeriod;
    u8      ThirdOnPeriod;
    #endif //#ifdef TDMA_POWER_SAVING
}LEGACY_PS_PARM, *PLEGACY_PS_PARM;

//H2C Index: 0x20
typedef struct _H2CParam_SetPwrMode_parm_ {
    LEGACY_PS_PARM      PwrModeParm;
}H2CParam_PwrMode, *PH2CParam_PwrMode;

#endif //#ifdef CONFIG_POWER_SAVING

/*------------------------------Function declaration--------------------------*/

#ifdef CONFIG_POWER_SAVING
#if 0
extern void ClockDown(PADAPTER padapter);
extern void ClockUp(PADAPTER padapter);
extern void PrintBcnFunction(void);
extern void DisDbgMsg(void);
extern void EnDbgMsg(void);
extern void UpChGain(void);
extern void StartCount(PADAPTER padapter);
extern void StopCount(PADAPTER padapter);
extern void IssueNullDataTest(PADAPTER padapter);
extern void ShowPowerState(PADAPTER padapter);
#endif
#ifdef TDMA_POWER_SAVING
extern void TDMAChangeStateTask(PADAPTER padapter);
#endif //#ifdef TDMA_POWER_SAVING
extern void EnterPS(PADAPTER padapter);
extern void GTimer6Handle(VOID *Data);
extern void GTimer7Handle(VOID *Data);
extern void InitGTimer1ms(PADAPTER padapter, u8 IRQDis, u8 TimerID, u32 Period);
extern void DeInitGTimer1ms(PADAPTER padapter, u8 TimerID);
extern void ChangeTransmiteRate(u16 offset, u8 rate);
extern void PowerBitSetting(BOOLEAN bPowerBit, u16 offset);
extern void ChkandChangePS(PPS_PARM pPSParm, BOOLEAN bPowerBit);
extern u16 IssueRsvdPagePacketSetting(u8 PageNum, BOOLEAN bHwSEQEn, u8 RtyLmt);
extern void InitRsvdPgPkt(void);
extern BOOLEAN IssueNullData(PPS_PARM pPSParm, BOOLEAN bPowerBit, u8 RtyLmt);
extern void IssuePSPoll(void);
extern BOOLEAN WaitTxStateMachineOk(void);
extern void WriteTxPause(u8 value, u8 rcode);
extern void PsCloseRF(void);
extern void PsOpenRF(void);
extern void SetPwrStateReg(PPS_PARM pPSParm, u8 PwrStateType, u8 value);
extern BOOLEAN ChkTxQueueIsEmpty(void);
extern void InitPS(PADAPTER padapter);
extern void ResetPSParm(PADAPTER padapter);
extern void Legacy_PS_Setting(PADAPTER padapter);
extern void PSModeSetting(PADAPTER padapter, u8 on);
extern void ConfigListenBeaconPeriod(PPS_PARM pPSParm, u8 RLBM, u8 AwakeInterval);
extern void PSSetMode(PADAPTER padapter, PLEGACY_PS_PARM pparm);
extern BOOLEAN PS_S2_Condition_Match(PPS_PARM pPSParm);
extern BOOLEAN PS_S4_Condition_Match(PADAPTER padapter);
extern BOOLEAN PS_32K_Condition_Match(void);
extern void PS_S2ToS3ToS0State(PADAPTER padapter, u8 nulldata0Allow);
extern void PS_S2ToS0State(PPS_PARM pPSParm);
extern void PS_S3ToS2orS0State(PPS_PARM pPSParm);
extern void PS_S0ToS1ToS2State(PADAPTER padapter);
extern void PS_S1ToS0orS2State(PPS_PARM pPSParm);
extern void PS_S2ToS4State(PADAPTER padapter);
extern void PS_S2ToS5State(PPS_PARM pPSParm);
extern void PS_S4ToS2State(PPS_PARM pPSParm, u8 ReleaseTxPause);
extern void PS_S5ToS2State(PPS_PARM pPSParm);
extern void PS_S0ToS6State(PADAPTER padapter);
extern void PS_S6ToS0State(PPS_PARM pPSParm);
extern void CheckTSFIsStable(u8 ReqState);
extern void WaitHWStateReady(void);
extern void SysClkDown(PPS_PARM	pPSParm);
extern void SysClkUp(PPS_PARM pPSParm);
extern void SleepTo32K(PPS_PARM pPSParm);
extern void Change_PS_State(PADAPTER padapter, u8 request_ps_state, u8 nulldata0Allow);
extern void ChangePSStateByRPWM(PADAPTER padapter);
extern void SetSmartPSTimer(PADAPTER padapter);
extern void SmartPS2InitTimerAndToGetRxPkt(PADAPTER padapter);
extern void PS_OnBeacon(PADAPTER padapter);
extern void PSBcnEarlyProcess(PADAPTER padapter);
extern void PSMtiBcnEarlyProcess(PADAPTER padapter);
extern void PSRxBcnProcess(PADAPTER padapter);
extern void TxPktInPSOn(PADAPTER padapter);
extern void PsBcnToProcess(PADAPTER padapter);
extern BOOL RPWMProcess(PADAPTER padapter, BOOLEAN benter32k);
extern void ISR_MtiBcnEarly(PADAPTER padapter);
extern void ISR_BcnEarly(PADAPTER padapter);
extern void ISR_RxBcn(PADAPTER padapter);
extern void ISR_RxBCMD1(PADAPTER padapter);
extern void ISR_RxBCMD0(PADAPTER padapter);
extern void ISR_RxUCMD1(PADAPTER padapter);
extern void ISR_RxUCMD0(PADAPTER padapter);
extern void ISR_TxPktIn(PADAPTER padapter);
extern void ISR_TXCCX(PADAPTER padapter);
extern void H2CHDL_SetPwrMode(PADAPTER padapter, u8* pCmdBuffer);
extern void CheckInReqStateTask(PADAPTER padapter);
extern void HalSetRPWM(PADAPTER padapter, BOOLEAN benter32k);
extern u32  HalGetNullTxRpt(PADAPTER padapter);
//extern thread_return HalEnter32KThreadRtl8195a(thread_context context);
#endif //#ifdef CONFIG_POWER_SAVING

extern void ISR_TBTT(PADAPTER padapter);
extern void H2CHDL_BcnIgnoreEDCCA(PADAPTER padapter, u8* pCmdBuffer);

void PMUTask(PADAPTER padapter);
void PMUInitial(PADAPTER padapter);

#endif  //CONFIG_LITTLE_WIFI_MCU_FUNCTION_THREAD
#endif  //__RTL8195A_PMU_TASK_H__

