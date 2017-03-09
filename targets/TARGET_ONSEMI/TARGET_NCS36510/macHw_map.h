/**
******************************************************************************
* @file macHw_map.h
* @brief MACHW hw module register map
* @internal
* @author ON Semiconductor
* $Rev: 3390 $
* $Date: 2015-05-13 17:21:05 +0530 (Wed, 13 May 2015) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup macHw
*
* @details
*/

#ifndef MACHW_MAP_H_
#define MACHW_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "architecture.h"

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

/** macHw register map (phy, mac and agc parts) */
typedef struct {
    __O uint32_t SEQUENCER;                                 /**< 0x40014000 */
    union {
        struct {
            __IO uint32_t MODE:2;
            __IO uint32_t NOACK:1;
            __IO uint32_t FT:1;
            __IO uint32_t PAD0:3;
            __IO uint32_t AUTO:1;
            __IO uint32_t PAD1:1;
            __IO uint32_t NOW:1;
            __IO uint32_t PAD2:1;
            __IO uint32_t PRM:1;
            __IO uint32_t NFCS:1;
            __IO uint32_t PAN:1;
            __IO uint32_t RSTT:1;
            __IO uint32_t RSTR:1;
            __IO uint32_t ACK_ENABLE:1;
            __IO uint32_t BEA_ENABLE:1;
            __IO uint32_t CMD_ENABLE:1;
            __IO uint32_t DATA_ENABLE:1;
            __IO uint32_t RES_ENABLE:1;
        } BITS;
        __IO uint32_t WORD;
    } SEQ_OPTIONS;                                          /**< 0x40014004 */
    union {
        struct {
            __IO uint32_t SRST:1;
            __IO uint32_t ON:1;
            __IO uint32_t CLKDIV:1;
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;                                              /**< 0x40014008 */
    __O uint32_t PAD0;                                      /**< 0x4001400C */
    union {
        struct {
            __I uint32_t CODE:4;
            __I uint32_t PAD0:8;
            __I uint32_t MSO:1;
            __I uint32_t CB:1;
            __I uint32_t PAD1:1;
            __I uint32_t MST:1;
        } BITS;
        __I uint32_t WORD;
    } STATUS;                                               /**< 0x40014010 */
    union {
        struct {
            __IO uint32_t TFP:1;
            __IO uint32_t SDC:1;
            __IO uint32_t IC:1;
            __IO uint32_t SDB:1;
            __IO uint32_t SSP:1;
            __IO uint32_t TFPO:1;
        } BITS;
        __IO uint32_t WORD;
    } OPTIONS;                                              /**< 0x40014014 */
    __IO uint32_t PANID;                                    /**< 0x40014018 */
    __IO uint32_t SHORT_ADDRESS;                            /**< 0x4001401C */
    __IO uint32_t LONG_ADDRESS_HIGH;                        /**< 0x40014020 */
    __IO uint32_t LONG_ADDRESS_LOW;                         /**< 0x40014024 */
    union {
        struct {
            __IO uint32_t BIT_CLOCK_DIVIDER:8;
            __IO uint32_t SYSTEM_CLOCK_DIVIDER:8;
            __IO uint32_t CHIP_CLOCK_DIVIDER:8;
        } BITS;
        __IO uint32_t WORD;
    } DIVIDER;                                              /**< 0x40014028 */
    union {
        struct {
            __IO uint32_t RECEIVE_WARMPUP:12;
            __IO uint32_t PAD0:4;
            __IO uint32_t TRANSMIT_WARMPUP:12;
        } BITS;
        __IO uint32_t WORD;
    } RX_TX_WARMPUPS;                                       /**< 0x4001402c */
    union {
        struct {
            __O uint32_t EC:1;
            __O uint32_t ES:1;
            __O uint32_t DATA:1;
            __O uint32_t FS:1;
            __O uint32_t FP:1;
            __O uint32_t FMD:1;
            __I uint32_t PC:1;
        } BITS;
        __O uint32_t WORD;
    } CLEAR_IRQ;                                            /**< 0x40014030 */
    union {
        struct {
            __IO uint32_t EC:1;
            __IO uint32_t ES:1;
            __IO uint32_t DATA:1;
            __IO uint32_t FS:1;
            __IO uint32_t FP:1;
            __IO uint32_t FM:1;
            __I uint32_t PC:1;
        } BITS;
        __IO uint32_t WORD;
    } MASK_IRQ;                                             /**< 0x40014034 */
    union {
        struct {
            __I uint32_t EC:1;
            __I uint32_t ES:1;
            __I uint32_t DATA:1;
            __I uint32_t FS:1;
            __I uint32_t FP:1;
            __I uint32_t FM:1;
            __I uint32_t PC:1;
        } BITS;
        __I uint32_t WORD;
    } IRQ_STATUS;                                           /**< 0x40014038 */
    __O uint32_t PAD1;                                      /**< 0x4001403C */
    union {
        struct {
            __IO uint32_t START:1;
            __IO uint32_t STOP:1;
        } BITS;
        __IO uint32_t WORD;
    } TIMER_ENABLE;                                         /**< 0x40014040 */
    union {
        struct {
            __IO uint32_t START:1;
            __IO uint32_t STOP:1;
        } BITS;
        __IO uint32_t WORD;
    } TIMER_DISABLE;                                        /**< 0x40014044 */
    __IO uint32_t TIMER;                                    /**< 0x40014048 */
    __IO uint32_t START_TIME;                               /**< 0x4001404C */
    __IO uint32_t STOP_TIME;                                /**< 0x40014050 */
    union {
        struct {
            __I uint32_t START:1;
            __I uint32_t STOP:1;
        } BITS;
        __I uint32_t WORD;
    } TIMER_STATUS;                                         /**< 0x40014054 */
    __I uint32_t PROTOCOL_TIMER;                            /**< 0x40014058 */
    __O uint32_t PAD4;                                      /**< 0x4001405C */
    __I uint32_t FINISH_TIME;                               /**< 0x40014060 */
    union {
        struct {
            __IO uint32_t TX_SLOT_OFFSET:12;
            __IO uint32_t PAD0:4;
            __IO uint32_t RX_SLOT_OFFSET:12;
        } BITS;
        __IO uint32_t WORD;
    } SLOT_OFFSET;                                          /**< 0x40014064 */
    __I uint32_t TIME_STAMP;                                /**< 0x40014068 */
    union {
        struct {
            __IO uint32_t CRD_SHORT_ADDRESS:16;
            __IO uint32_t PAD0:13;
            __IO uint32_t ASSOC_PAN_COORD:1;
            __IO uint32_t PAN_COORD_ADDR_L:1;
            __IO uint32_t PAN_COORD_ADDR_S:1;
        } BITS;
        __IO uint32_t WORD;
} CRD_SHORT_ADDR;                                       /**< 0x4001406C */
__IO uint32_t CRD_LONG_ADDR_HI;                         /**< 0x40014070 */
__IO uint32_t CRD_LONG_ADDR_LO;                         /**< 0x40014074 */
__O uint32_t PAD5;                                      /**< 0x40014078 */
__O uint32_t PAD9;                                      /**< 0x4001407C */
__O uint32_t PAD10;                                     /**< 0x40014080 */
__O uint32_t PAD11;                                     /**< 0x40014084 */
__IO uint32_t RX_LENGTH;                                /**< 0x40014088 */
union {
    struct {
        __IO uint32_t TXLENGTH:7;
        __O uint32_t PAD0:1;
        __IO uint32_t TX_PRE_CHIPS:4;
    } BITS;
    __IO uint32_t WORD;
} TX_LENGTH;                                            /**< 0x4001408C */
__IO uint32_t TX_SEQ_NUMBER;                            /**< 0x40014090 */
__IO uint32_t TX_ACK_DELAY;                             /**< 0x40014094 */
union {
    struct {
        __IO uint32_t RXACKDELAY:12;
        __IO uint32_t PAD0:4;
        __IO uint32_t RXAUTODELAY:12;
    } BITS;
    __IO uint32_t WORD;
} RX_ACK_DELAY;                                         /**< 0x40014098 */
__IO uint32_t TX_FLUSH;                                 /**< 0x4001409C */
union {
    struct {
        __IO uint32_t CCA_DELAY:12;
        __IO uint32_t PAD0:4;
        __IO uint32_t CCA_LENGTH:12;
    } BITS;
    __IO uint32_t WORD;
} CCA;                                                  /**< 0x400140A0 */
union {
    struct {
        __IO uint32_t RXACK_END:12;
        __IO uint32_t PAD0:4;
        __IO uint32_t RXSLOTTED_END:12;
    } BITS;
    __IO uint32_t WORD;
} ACK_STOP;                                             /**< 0x400140A4 */
__IO uint32_t TXCCA;                                    /**< 0x400140A8 */
__IO uint32_t ADDR_L_LOC;                               /**< 0x400140AC */
__IO uint32_t ADDR_S_LOC;                               /**< 0x400140B0 */
__IO uint32_t FRAME_MATCH_RESULT;                       /**< 0x400140B4 */
__IO uint32_t FRAME_MATCH_ADDR_L;                       /**< 0x400140B8 */
__IO uint32_t FRAME_MATCH_ADDR_S;                       /**< 0x400140BC */
union {
    struct {
        __IO uint32_t AA:1;
        __IO uint32_t AFA:1;
        __IO uint32_t PRE:1;
        __IO uint32_t PAD0:25;
        __IO uint32_t GAIN_START:4;
    } BITS;
    __IO uint32_t WORD;
} AGC_CONTROL;                                          /**< 0x400140C0 */
union {
    struct {
        __IO uint32_t SETTLE_DELAY:8;
        __IO uint32_t MEASURE_DELAY:8;
        __IO uint32_t DIVIDER:8;
        __IO uint32_t HIGH_THRESHOLD:4;
        __IO uint32_t LOW_THRESHOLD:4;
    } BITS;
    __IO uint32_t WORD;
} AGC_SETTINGS;                                         /**< 0x400140C4 */
union {
    struct {
        __IO uint32_t GC1:3;
        __IO uint32_t GC2:3;
        __IO uint32_t GC3:1;
        __IO uint32_t PAD:1;
        __IO uint32_t AGC_STATE:4;
    } BITS;
    __IO uint32_t WORD;
} AGC_STATUS;                                           /**< 0x400140C8 */
union {
    struct {
        __IO uint32_t GAIN3:7;
        __IO uint32_t PAD0:1;
        __IO uint32_t GAIN2:7;
        __IO uint32_t PAD1:1;
        __IO uint32_t GAIN1:7;
        __IO uint32_t PAD2:1;
        __IO uint32_t GAIN0:7;
        __IO uint32_t PAD3:1;
    } BITS;
    __IO uint32_t WORD;
} AGC_GAIN_TABLE0;                                      /**< 0x400140CC */
union {
    struct {
        __IO uint32_t GAIN7:7;
        __IO uint32_t PAD0:1;
        __IO uint32_t GAIN6:7;
        __IO uint32_t PAD1:1;
        __IO uint32_t GAIN5:7;
        __IO uint32_t PAD2:1;
        __IO uint32_t GAIN4:7;
        __IO uint32_t PAD3:1;
    } BITS;
    __IO uint32_t WORD;
} AGC_GAIN_TABLE1;                                      /**< 0x400140D0 */
union {
    struct {
        __IO uint32_t GAIN11:7;
        __IO uint32_t PAD0:1;
        __IO uint32_t GAIN10:7;
        __IO uint32_t PAD1:1;
        __IO uint32_t GAIN9:7;
        __IO uint32_t PAD2:1;
        __IO uint32_t GAIN8:7;
        __IO uint32_t PAD3:1;
    } BITS;
    __IO uint32_t WORD;
} AGC_GAIN_TABLE2;                                      /**< 0x400140D4 */
union {
    struct {
        __IO uint32_t GAIN15:7;
        __IO uint32_t PAD0:1;
        __IO uint32_t GAIN14:7;
        __IO uint32_t PAD1:1;
        __IO uint32_t GAIN13:7;
        __IO uint32_t PAD2:1;
        __IO uint32_t GAIN12:7;
        __IO uint32_t PAD3:1;
    } BITS;
    __IO uint32_t WORD;
} AGC_GAIN_TABLE3;                                      /**< 0x400140D8 */
} MacHwReg_t, *MacHwReg_pt;

/** macHw register map (demodulator part) */
typedef struct {
    union {
        struct {
            __IO uint32_t DRC:1;                            /**< Reserved */
            __IO uint32_t SWIQ:1;                           /**< Compensation for quadrature polarity. (set to 1 for RevB) */
            __IO uint32_t LIF:1;                            /**< Allows the receiver to use a low-IF frequency of +1.23 MHz (0) or -1.23 MHz (1). */
            __IO uint32_t PM:1;                             /**< Preamble Mode: Mode 0 (high sensitivity) – Preamble detection is based on observation of a regular pattern of correlation peaks over a span of 5 consecutive symbol periods.  Each symbol period produces a time index and frequency index corresponding to the largest correlation peak.  If 4 out of 5 symbol periods produce time/frequency index values that meet a set of similarity criteria, then preamble detection is declared.  This mode improves preamble detection rate by tolerating one corrupt correlation result in the span of 5 symbols.  However, the relaxed detection rule allows a higher rate of false preamble detection when no signal is present. Mode 1 (low false detection) – Preamble detection is based on a span of 4 consecutive symbol periods.  Each symbol period produces a time index and frequency index corresponding to the largest correlation peak.  If all four symbol periods produce time/frequency index values that meet a set of similarity criteria, then preamble detection is declared.  This mode enforces a more strict detection rule and therefore offers lower rate of false preamble detection at the expense of higher missed detection. */
            __IO uint32_t ASM:1;                            /**< This bit determines whether antenna selection is automatic (1) or manual (0).  For applications that do not use antenna diversity, this bit should be set to 0. */
            __IO uint32_t AS:1;                             /**< If automatic antenna selection mode is used, this bit determines the initial antenna selection.  If manual antenna selection mode is used, this bit determines the antenna selection, 0 or 1. */
            __IO uint32_t DTC:1;                            /**< Sets the decay time constant used in the RSSI calculation and Digital Gain Control functions. 0: Time constant set to 1 symbol period.  This produces a slower response time but more stable RSSI values.  Not recommended for use with antenna diversity.  1: Time constant set to 1/4th of a symbol period.  This produces a faster response with slightly more variance in the RSSI calculation.  Recommended for most cases.  */
            __IO uint32_t PAD1:9;
            __IO uint32_t DFR:16;                           /**<Selectively enables individual frequency offsets used during preamble search.  Each of the 15 bits in this field corresponds to one of 15 different frequency offsets.  A bit value of 0 removes a specific frequency offset from the search, while a bit value of 1 includes the frequency offset in the search. */
        } BITS;
        __IO uint32_t WORD;
    } DMD_CONTROL0;                                         /**< 0x40014100 */
    union {
        struct {
            __IO uint32_t DST:4;                            /**< This value specifies the SFD search period in symbols.  After preamble detection, the demodulator begins symbol recovery and searches for the start-of-frame delimiter (SFD).  If the SFD is not found within the number of symbols specified, the preamble detection flag is cleared and a new preamble search is initiated.  The default value of 8 symbols should be sufficient for 802.15.4 compliant applications. Default 8 */
            __IO uint32_t PAD0:4;
            __IO uint32_t DPT:6;                            /**< The similarity criteria used for preamble detection includes a rule that all time index values must occupy a span equal to or less than this value.  The default span of 0011 means that the correlation peaks must span a range of 3Ts, where Ts is the sample period = 0.25 µs.  This value is recommended for typical multipath conditions.  Very long-range applications may benefit from a higher value. Default 3 */
            __IO uint32_t PAD1:2;
            __IO uint32_t DPF:4;                            /**< The similarity criteria used for preamble detection includes a rule that all frequency index values must occupy a span equal to or less than this value.  The default span of 0001 means that the difference between largest frequency index and smallest frequency index must be less than or equal to 1. Default 1 */
            __IO uint32_t PAD2:4;
            __IO uint32_t DCT:4;                            /**< In order for preamble detection to be declared, the correlation peaks must exceed a threshold.  The threshold is computed dynamically and includes a programmable scale factor:  1 + bit[27]/2 + bit[26]/4 + bit[25]/8 + bit[24]/16  The default value of 1.5 is recommended for manual-antenna selection, while a value of 1.75 is recommended for automatic antenna selection. */

        } BITS;
        __IO uint32_t WORD;
    } DMD_CONTROL1;                                         /**< 0x40014104 */
    union {
        struct {
            __IO uint32_t RSSI_THRESHOLD:8;                 /**< Threshold value used to determine clear channel assessment (CCA) result.  The channel is declared busy if RSSI > threshold. Default 0xFF */
            __IO uint32_t RSSI_OFFSET:6;                    /**< Calibration constant added to the RSSI calculation.  The 6-bit field is treated as a signed value in two’s complement format with values from -32 to +31 dB. */
        } BITS;
        __IO uint32_t WORD;
    } DMD_CONTROL2;                                         /**< 0x40014108 */
    union {
        struct {
            __I uint32_t RSSI_VALUE:8;                      /**< The value is captured at the end of packet reception or at the end of ED/CCA measurements and is interpreted in dBm as follows: 00000000 -> 0127dBm (or below) ... 01111111 -> 0dBm (or above) */
            __I uint32_t FREQUENCY_OFFSET:4;                /**< Frequency correction applied to the received packet.  The value is captured at the end of packet reception or at the end of ED/CCA measurements. */
            __I uint32_t ANT:1;                             /**< Antenna used for reception.  The value is captured at the end of packet reception or at the end of ED/CCA measurements. */
            __I uint32_t PAD0:3;
            __I uint32_t RSSI_COMPONENT:4;                  /**< Magnitude of the baseband digital signal (units are dB relative to A/D saturation).  The value is updated until AGC is frozen.  The value is captured at the end of packet reception or at the end of ED/CCA measurements. */
        } BITS;
        __I uint32_t WORD;
    } DMD_STATUS;                                           /**< 0x4001410C */
} DmdReg_t, *DmdReg_pt;

#endif /* MACHW_MAP_H_ */
