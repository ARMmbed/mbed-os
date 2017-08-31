#ifndef INCLUDED_KM_GNSS_APPS_TIM
#define INCLUDED_KM_GNSS_APPS_TIM
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** km_gnss_apps_tim HAL Spreadsheet version number */
#define KM_GNSS_APPS_TIM_HAL_VERSION 1

/** Klein Matterhorn GNSS and Applications Timing Assistance Module
*/
struct km_gnss_apps_tim_s {
   /** Control Register Enable at address offset 0x000, read-write */
   uint32_t ctrls;
   /** Control Register Disable at address offset 0x004, read-write */
   uint32_t ctrlc;
   /** Control Register at address offset 0x008, read-write */
   uint32_t ctrl;
   /** Enable for the KM RTC.  If using 32K RTC mode, then the counter will wait for ACTimeBaseSyncxSI to go high before starting as per the AC  -> KM RTC Preload mechanism (unless Ctrl.IgnoreACTimeBaseSync is set).  In 26MHz RTC Mode the counter will start immediately. at address offset 0x00C, read-write */
   uint32_t rtc;
   /** RTC Counter Preload value. Set RTCPreload.Enable.  Write to LSB, then MSB register value to set RTC counter when ACTimeBaseSync is next set to 1.  Ensure  at address offset 0x010, read-write */
   uint32_t rtcpreloadmsb;
   /** RTC Counter Preload value. Set RTCPreload.Enable.  Write to LSB, then MSB register value to set RTC counter when ACTimeBaseSync is next set to 1 at address offset 0x014, read-write */
   uint32_t rtcpreloadlsb;
   /** Trigger Funcitonality Enable at address offset 0x018, read-write */
   uint32_t trig;
   /** Timestamp Trigger Control register.  Only set when Trig.Enable = 0.  Read value back before setting  Trig.Enable = 1 to ensure no false triggers from change of trigger source at address offset 0x01C, read-write */
   uint32_t trigctrl;
   /** GPO Control Register at address offset 0x020, read-write */
   uint32_t gpoctrl;
   /** Interrupt Status Mask at address offset 0x024, read-write */
   uint32_t irqmask;
   /** Interrupt Status Register at address offset 0x028, read-write */
   uint32_t irqstatus;
   /** Apps CPU Timestamp Manual Trigger at address offset 0x02C, read-write */
   uint32_t apptrig;
   /** Request to read a RTC value from the hardware to be read from the RTCMsb and RTCLsb register pair.  Set only 1 bit at a time - one-hot. Check for valid in RTCMsb before sending another request at address offset 0x030, read-write */
   uint32_t rtcvaluereq;
   /** Requested RTC Value.  Read here after writing to RTCValueReq. The total RTC value is a concatenation of the lsb and msb registers to form the full 44-bit value.  Read MSB, check Valid bit, read LSB and concatenate 44-bit value at address offset 0x034, read-only */
   uint32_t rtcmsb;
   /** KM RTC Value Bits [31:0] at address offset 0x038, read-only */
   uint32_t rtclsb;
   /** Timer Comparison Enable at address offset 0x03C, read-write */
   uint32_t timecomp;
   /** Timer comparison value for GPO and GPO Interrupt Event Triggering at address offset 0x040, read-write */
   uint32_t timecompmsb;
   /** Timer comparison value for GPO and GPO Interrupt Event Triggering at address offset 0x044, read-write */
   uint32_t timecomplsb;
   /** Timer comparison Mask. Timer event is generated when (TimerComparer AND TimerCompMask) = RTC at address offset 0x048, read-write */
   uint32_t timecompmaskmsb;
   /** Timer comparison Mask at address offset 0x04C, read-write */
   uint32_t timecompmasklsb;
   /** Edge Counter at address offset 0x050, read-write */
   uint32_t edgecount;
};

/** bit field defines for km_gnss_apps_tim_s#ctrls */
#define KM_GNSS_APPS_TIM_CTRLS_ENABLE_OFFSET 0
#define KM_GNSS_APPS_TIM_CTRLS_ENABLE_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLS_RTCMODE_OFFSET 1
#define KM_GNSS_APPS_TIM_CTRLS_RTCMODE_SIZE 1
/** RTC Runs from the 26MHz clock */
#define KM_GNSS_APPS_TIM_CTRLS_RTCMODE_CLK26MHZ_VALUE 1
/** RTC Runs from the 32kHz clock */
#define KM_GNSS_APPS_TIM_CTRLS_RTCMODE_CLK32KHZ_VALUE 0
#define KM_GNSS_APPS_TIM_CTRLS_RTCSTARTWAITAC_OFFSET 2
#define KM_GNSS_APPS_TIM_CTRLS_RTCSTARTWAITAC_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLS_EDGECNTENABLE_OFFSET 3
#define KM_GNSS_APPS_TIM_CTRLS_EDGECNTENABLE_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLS_EDGECNTCOR_OFFSET 4
#define KM_GNSS_APPS_TIM_CTRLS_EDGECNTCOR_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLS_TIMESTAMPCOR_OFFSET 5
#define KM_GNSS_APPS_TIM_CTRLS_TIMESTAMPCOR_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLS_FORCECLKGATES_OFFSET 6
#define KM_GNSS_APPS_TIM_CTRLS_FORCECLKGATES_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLS_SYNCIRQ_OFFSET 7
#define KM_GNSS_APPS_TIM_CTRLS_SYNCIRQ_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#ctrlc */
#define KM_GNSS_APPS_TIM_CTRLC_ENABLE_OFFSET 0
#define KM_GNSS_APPS_TIM_CTRLC_ENABLE_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLC_RTCMODE_OFFSET 1
#define KM_GNSS_APPS_TIM_CTRLC_RTCMODE_SIZE 1
/** RTC Runs from the 26MHz clock */
#define KM_GNSS_APPS_TIM_CTRLC_RTCMODE_CLK26MHZ_VALUE 1
/** RTC Runs from the 32kHz clock */
#define KM_GNSS_APPS_TIM_CTRLC_RTCMODE_CLK32KHZ_VALUE 0
#define KM_GNSS_APPS_TIM_CTRLC_RTCSTARTWAITAC_OFFSET 2
#define KM_GNSS_APPS_TIM_CTRLC_RTCSTARTWAITAC_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLC_EDGECNTENABLE_OFFSET 3
#define KM_GNSS_APPS_TIM_CTRLC_EDGECNTENABLE_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLC_EDGECNTCOR_OFFSET 4
#define KM_GNSS_APPS_TIM_CTRLC_EDGECNTCOR_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLC_TIMESTAMPCOR_OFFSET 5
#define KM_GNSS_APPS_TIM_CTRLC_TIMESTAMPCOR_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLC_FORCECLKGATES_OFFSET 6
#define KM_GNSS_APPS_TIM_CTRLC_FORCECLKGATES_SIZE 1
#define KM_GNSS_APPS_TIM_CTRLC_SYNCIRQ_OFFSET 7
#define KM_GNSS_APPS_TIM_CTRLC_SYNCIRQ_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#ctrl */
#define KM_GNSS_APPS_TIM_CTRL_ENABLE_OFFSET 0
#define KM_GNSS_APPS_TIM_CTRL_ENABLE_SIZE 1
#define KM_GNSS_APPS_TIM_CTRL_RTCMODE_OFFSET 1
#define KM_GNSS_APPS_TIM_CTRL_RTCMODE_SIZE 1
/** RTC Runs from the 26MHz clock */
#define KM_GNSS_APPS_TIM_CTRL_RTCMODE_CLK26MHZ_VALUE 1
/** RTC Runs from the 32kHz clock */
#define KM_GNSS_APPS_TIM_CTRL_RTCMODE_CLK32KHZ_VALUE 0
#define KM_GNSS_APPS_TIM_CTRL_RTCSTARTWAITAC_OFFSET 2
#define KM_GNSS_APPS_TIM_CTRL_RTCSTARTWAITAC_SIZE 1
#define KM_GNSS_APPS_TIM_CTRL_EDGECNTENABLE_OFFSET 3
#define KM_GNSS_APPS_TIM_CTRL_EDGECNTENABLE_SIZE 1
#define KM_GNSS_APPS_TIM_CTRL_EDGECNTCOR_OFFSET 4
#define KM_GNSS_APPS_TIM_CTRL_EDGECNTCOR_SIZE 1
#define KM_GNSS_APPS_TIM_CTRL_TIMESTAMPCOR_OFFSET 5
#define KM_GNSS_APPS_TIM_CTRL_TIMESTAMPCOR_SIZE 1
#define KM_GNSS_APPS_TIM_CTRL_FORCECLKGATES_OFFSET 6
#define KM_GNSS_APPS_TIM_CTRL_FORCECLKGATES_SIZE 1
#define KM_GNSS_APPS_TIM_CTRL_SYNCIRQ_OFFSET 7
#define KM_GNSS_APPS_TIM_CTRL_SYNCIRQ_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#rtc */
#define KM_GNSS_APPS_TIM_RTC_ENABLE_OFFSET 0
#define KM_GNSS_APPS_TIM_RTC_ENABLE_SIZE 1
#define KM_GNSS_APPS_TIM_RTC_SYNC_OFFSET 1
#define KM_GNSS_APPS_TIM_RTC_SYNC_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#rtcpreloadmsb */
#define KM_GNSS_APPS_TIM_RTCPRELOADMSB_VAL_OFFSET 0
#define KM_GNSS_APPS_TIM_RTCPRELOADMSB_VAL_SIZE 12

/** bit field defines for km_gnss_apps_tim_s#rtcpreloadlsb */
#define KM_GNSS_APPS_TIM_RTCPRELOADLSB_VAL_OFFSET 0
#define KM_GNSS_APPS_TIM_RTCPRELOADLSB_VAL_SIZE 32

/** bit field defines for km_gnss_apps_tim_s#trig */
#define KM_GNSS_APPS_TIM_TRIG_ENABLE_OFFSET 0
#define KM_GNSS_APPS_TIM_TRIG_ENABLE_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#trigctrl */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_OFFSET 0
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_SIZE 3
/** AppTrig Register Trigger Source. */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_APPTRIGA_VALUE 3
/** Trigger on the rising edge of the 32KHz clock.  Only for use in the 26MHz clocking mode. */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_CLK32KCHG_VALUE 4
/** Change in clock mode completed.  Due to the trigger enable activation delay ensure SW implemented a small delay (>8 RefClk cycles) between re-enabling Trig.Enable and setting Ctrl.RtcMode = <new setting> else the timestamp will not be correctly captured. */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_CLKMODECHG_VALUE 5
/** GPI External Input Trigger Source.  Set GpiTrigType. RisingEdgeDet or FallingEdgeDet: Min pulse width required for edge detect.  LevelDet mode also supported */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_GPI_VALUE 2
/** No Trigger */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_NONE_VALUE 0
/** TCU 0 FEvent Trigger Source. Change of level used to trigger timestamp */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCA_TCUFEVENT0_VALUE 1
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCB_OFFSET 3
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCB_SIZE 3
/** Trigger on rising edge of the AcTimeBaseSync Input from the AC */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCB_ACTIMEBASESYNC_VALUE 4
/** AppTrig Register Trigger Source. */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCB_APPTRIGB_VALUE 3
/** GPI External Input Trigger Source.  Set GpiTrigType. RisingEdgeDet or FallingEdgeDet: Min pulse width required for edge detect.  LevelDet mode also supported */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCB_GPI_VALUE 2
/** No Trigger */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCB_NONE_VALUE 0
/** TCU 1 FEvent Trigger Source. Use TrigType LevelDet mode.  Change of level used to trigger timestamp */
#define KM_GNSS_APPS_TIM_TRIGCTRL_SRCB_TCUFEVENT1_VALUE 1
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEA_OFFSET 6
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEA_SIZE 2
/** Trigger on both Rising and Falling Edges of input Signal */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEA_BOTHEDGEDET_VALUE 2
/** Trigger on Falling Edge of input signal */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEA_FALLINGEDGEDET_VALUE 1
/** Trigger on change in level */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEA_LEVELDET_VALUE 3
/** Trigger on Rising Edge of input signal */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEA_RISINGEDGEDET_VALUE 0
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEB_OFFSET 8
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEB_SIZE 2
/** Trigger on both Rising and Falling Edges of input Signal */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEB_BOTHEDGEDET_VALUE 2
/** Trigger on Falling Edge of input signal */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEB_FALLINGEDGEDET_VALUE 1
/** Trigger on change in level */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEB_LEVELDET_VALUE 3
/** Trigger on Rising Edge of input signal */
#define KM_GNSS_APPS_TIM_TRIGCTRL_GPITYPEB_RISINGEDGEDET_VALUE 0

/** bit field defines for km_gnss_apps_tim_s#gpoctrl */
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_OFFSET 0
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_SIZE 3
/** Transparent mode.  GPO is driven directly from the GPI */
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_GPI_VALUE 3
/** GPO is not changed on timer compare.  Use this mode if only the CPU interrupt is required on timer compare */
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_NONE_VALUE 0
/** Send Pulse on timer compare - pulse is 1x 32kHz tick wide in 32K Mode or 8x 26MHz ticks wide in 26MHz mode */
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_PULSE_VALUE 2
/** Transparent mode.  GPO is driven directly from TCU Fevent0 */
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_TCUFEVENT0_VALUE 4
/** Transparent mode.  GPO is driven directly from TCU Fevent1 */
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_TCUFEVENT1_VALUE 5
/** Change sense of signal on timer compare */
#define KM_GNSS_APPS_TIM_GPOCTRL_TYPE_TOGGLE_VALUE 1
#define KM_GNSS_APPS_TIM_GPOCTRL_INVERT_OFFSET 3
#define KM_GNSS_APPS_TIM_GPOCTRL_INVERT_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#irqmask */
#define KM_GNSS_APPS_TIM_IRQMASK_TRIGA_OFFSET 0
#define KM_GNSS_APPS_TIM_IRQMASK_TRIGA_SIZE 1
#define KM_GNSS_APPS_TIM_IRQMASK_TRIGB_OFFSET 1
#define KM_GNSS_APPS_TIM_IRQMASK_TRIGB_SIZE 1
#define KM_GNSS_APPS_TIM_IRQMASK_GPO_OFFSET 2
#define KM_GNSS_APPS_TIM_IRQMASK_GPO_SIZE 1
#define KM_GNSS_APPS_TIM_IRQMASK_RTCROLLOVER_OFFSET 3
#define KM_GNSS_APPS_TIM_IRQMASK_RTCROLLOVER_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#irqstatus */
#define KM_GNSS_APPS_TIM_IRQSTATUS_TRIGA_OFFSET 0
#define KM_GNSS_APPS_TIM_IRQSTATUS_TRIGA_SIZE 1
#define KM_GNSS_APPS_TIM_IRQSTATUS_TRIGB_OFFSET 1
#define KM_GNSS_APPS_TIM_IRQSTATUS_TRIGB_SIZE 1
#define KM_GNSS_APPS_TIM_IRQSTATUS_GPO_OFFSET 2
#define KM_GNSS_APPS_TIM_IRQSTATUS_GPO_SIZE 1
#define KM_GNSS_APPS_TIM_IRQSTATUS_RTCROLLOVER_OFFSET 3
#define KM_GNSS_APPS_TIM_IRQSTATUS_RTCROLLOVER_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#apptrig */
#define KM_GNSS_APPS_TIM_APPTRIG_TSA_OFFSET 0
#define KM_GNSS_APPS_TIM_APPTRIG_TSA_SIZE 1
#define KM_GNSS_APPS_TIM_APPTRIG_TSB_OFFSET 1
#define KM_GNSS_APPS_TIM_APPTRIG_TSB_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#rtcvaluereq */
#define KM_GNSS_APPS_TIM_RTCVALUEREQ_SEL_OFFSET 0
#define KM_GNSS_APPS_TIM_RTCVALUEREQ_SEL_SIZE 2
/** When set, the current RTC counter value is made available in RTCMsb and RTCLsb */
#define KM_GNSS_APPS_TIM_RTCVALUEREQ_SEL_RTCCURRENT_VALUE 3
/** When set, TimestampA and EdgeCountA value is made available in RTCMsb and RTCLsb registers.  If Ctrl.EdgeCountCOR is set, then this action will reset the EdgeCountA value to zero. */
#define KM_GNSS_APPS_TIM_RTCVALUEREQ_SEL_TIMESTAMPA_VALUE 1
/** When set, TimestampB and EdgeCountB value is made available in RTCMsb and RTCLsb registers.  If Ctrl.EdgeCountCOR is set, then this action will reset the EdgeCountB value to zero. */
#define KM_GNSS_APPS_TIM_RTCVALUEREQ_SEL_TIMESTAMPB_VALUE 2
#define KM_GNSS_APPS_TIM_RTCVALUEREQ_GET_OFFSET 2
#define KM_GNSS_APPS_TIM_RTCVALUEREQ_GET_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#rtcmsb */
#define KM_GNSS_APPS_TIM_RTCMSB_RTCVAL_OFFSET 0
#define KM_GNSS_APPS_TIM_RTCMSB_RTCVAL_SIZE 12
#define KM_GNSS_APPS_TIM_RTCMSB_EDGECOUNTVAL_OFFSET 16
#define KM_GNSS_APPS_TIM_RTCMSB_EDGECOUNTVAL_SIZE 8
#define KM_GNSS_APPS_TIM_RTCMSB_EDGECOUNTOFLOW_OFFSET 24
#define KM_GNSS_APPS_TIM_RTCMSB_EDGECOUNTOFLOW_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#rtclsb */
#define KM_GNSS_APPS_TIM_RTCLSB_RTCVAL_OFFSET 0
#define KM_GNSS_APPS_TIM_RTCLSB_RTCVAL_SIZE 32

/** bit field defines for km_gnss_apps_tim_s#timecomp */
#define KM_GNSS_APPS_TIM_TIMECOMP_ENABLE_OFFSET 0
#define KM_GNSS_APPS_TIM_TIMECOMP_ENABLE_SIZE 1

/** bit field defines for km_gnss_apps_tim_s#timecompmsb */
#define KM_GNSS_APPS_TIM_TIMECOMPMSB_RTCVAL_OFFSET 0
#define KM_GNSS_APPS_TIM_TIMECOMPMSB_RTCVAL_SIZE 12

/** bit field defines for km_gnss_apps_tim_s#timecomplsb */
#define KM_GNSS_APPS_TIM_TIMECOMPLSB_RTCVAL_OFFSET 0
#define KM_GNSS_APPS_TIM_TIMECOMPLSB_RTCVAL_SIZE 32

/** bit field defines for km_gnss_apps_tim_s#timecompmaskmsb */
#define KM_GNSS_APPS_TIM_TIMECOMPMASKMSB_RTCVAL_OFFSET 0
#define KM_GNSS_APPS_TIM_TIMECOMPMASKMSB_RTCVAL_SIZE 12

/** bit field defines for km_gnss_apps_tim_s#timecompmasklsb */
#define KM_GNSS_APPS_TIM_TIMECOMPMASKLSB_RTCVAL_OFFSET 0
#define KM_GNSS_APPS_TIM_TIMECOMPMASKLSB_RTCVAL_SIZE 32

/** bit field defines for km_gnss_apps_tim_s#edgecount */
#define KM_GNSS_APPS_TIM_EDGECOUNT_RESETA_OFFSET 0
#define KM_GNSS_APPS_TIM_EDGECOUNT_RESETA_SIZE 1
#define KM_GNSS_APPS_TIM_EDGECOUNT_RESETB_OFFSET 1
#define KM_GNSS_APPS_TIM_EDGECOUNT_RESETB_SIZE 1

/* EOF km_gnss_apps_tim.h */
#endif
