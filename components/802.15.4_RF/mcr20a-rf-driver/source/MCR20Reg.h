/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
*
* \file MCR20reg.h
* MCR20 Registers
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __MCR20_REG_H__
#define __MCR20_REG_H__
/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 * Note that it is not a good practice to include header files into header   *
 * files, so use this section only if there is no other better solution.     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/****************************************************************************/
/* Transceiver SPI Registers */
/****************************************************************************/

#define TransceiverSPI_IARIndexReg                  (0x3E)

#define TransceiverSPI_ReadSelect                   (1<<7)
#define TransceiverSPI_WriteSelect                  (0<<7)
#define TransceiverSPI_RegisterAccessSelect         (0<<6)
#define TransceiverSPI_PacketBuffAccessSelect       (1<<6)
#define TransceiverSPI_PacketBuffBurstModeSelect    (0<<5)
#define TransceiverSPI_PacketBuffByteModeSelect     (1<<5)

#define TransceiverSPI_DirectRegisterAddressMask    (0x3F)

#define IRQSTS1             0x00
#define IRQSTS2             0x01
#define IRQSTS3             0x02
#define PHY_CTRL1           0x03
#define PHY_CTRL2           0x04
#define PHY_CTRL3           0x05
#define RX_FRM_LEN          0x06
#define PHY_CTRL4           0x07
#define SRC_CTRL            0x08
#define SRC_ADDRS_SUM_LSB   0x09
#define SRC_ADDRS_SUM_MSB   0x0A
#define CCA1_ED_FNL         0x0B
#define EVENT_TMR_LSB       0x0C
#define EVENT_TMR_MSB       0x0D
#define EVENT_TMR_USB       0x0E
#define TIMESTAMP_LSB       0x0F
#define TIMESTAMP_MSB       0x10
#define TIMESTAMP_USB       0x11
#define T3CMP_LSB           0x12
#define T3CMP_MSB           0x13
#define T3CMP_USB           0x14
#define T2PRIMECMP_LSB      0x15
#define T2PRIMECMP_MSB      0x16
#define T1CMP_LSB           0x17
#define T1CMP_MSB           0x18
#define T1CMP_USB           0x19
#define T2CMP_LSB           0x1A
#define T2CMP_MSB           0x1B
#define T2CMP_USB           0x1C
#define T4CMP_LSB           0x1D
#define T4CMP_MSB           0x1E
#define T4CMP_USB           0x1F
#define PLL_INT0            0x20
#define PLL_FRAC0_LSB       0x21
#define PLL_FRAC0_MSB       0x22
#define PA_PWR              0x23
#define SEQ_STATE           0x24
#define LQI_VALUE           0x25
#define RSSI_CCA_CONT       0x26
//--------------            0x27
#define ASM_CTRL1           0x28
#define ASM_CTRL2           0x29
#define ASM_DATA_0          0x2A
#define ASM_DATA_1          0x2B
#define ASM_DATA_2          0x2C
#define ASM_DATA_3          0x2D
#define ASM_DATA_4          0x2E
#define ASM_DATA_5          0x2F
#define ASM_DATA_6          0x30
#define ASM_DATA_7          0x31
#define ASM_DATA_8          0x32
#define ASM_DATA_9          0x33
#define ASM_DATA_A          0x34
#define ASM_DATA_B          0x35
#define ASM_DATA_C          0x36
#define ASM_DATA_D          0x37
#define ASM_DATA_E          0x38
#define ASM_DATA_F          0x39
//-------------------       0x3A
#define OVERWRITE_VER       0x3B
#define CLK_OUT_CTRL        0x3C
#define PWR_MODES           0x3D
#define IAR_INDEX           0x3E
#define IAR_DATA            0x3F


#define PART_ID             0x00
#define XTAL_TRIM           0x01
#define PMC_LP_TRIM         0x02
#define MACPANID0_LSB       0x03
#define MACPANID0_MSB       0x04
#define MACSHORTADDRS0_LSB  0x05
#define MACSHORTADDRS0_MSB  0x06
#define MACLONGADDRS0_0     0x07
#define MACLONGADDRS0_8     0x08
#define MACLONGADDRS0_16    0x09
#define MACLONGADDRS0_24    0x0A
#define MACLONGADDRS0_32    0x0B
#define MACLONGADDRS0_40    0x0C
#define MACLONGADDRS0_48    0x0D
#define MACLONGADDRS0_56    0x0E
#define RX_FRAME_FILTER     0x0F
#define PLL_INT1            0x10
#define PLL_FRAC1_LSB       0x11
#define PLL_FRAC1_MSB       0x12
#define MACPANID1_LSB       0x13
#define MACPANID1_MSB       0x14
#define MACSHORTADDRS1_LSB  0x15
#define MACSHORTADDRS1_MSB  0x16
#define MACLONGADDRS1_0     0x17
#define MACLONGADDRS1_8     0x18
#define MACLONGADDRS1_16    0x19
#define MACLONGADDRS1_24    0x1A
#define MACLONGADDRS1_32    0x1B
#define MACLONGADDRS1_40    0x1C
#define MACLONGADDRS1_48    0x1D
#define MACLONGADDRS1_56    0x1E
#define DUAL_PAN_CTRL       0x1F
#define DUAL_PAN_DWELL      0x20
#define DUAL_PAN_STS        0x21
#define CCA1_THRESH         0x22
#define CCA1_ED_OFFSET_COMP 0x23
#define LQI_OFFSET_COMP     0x24
#define CCA_CTRL            0x25
#define CCA2_CORR_PEAKS     0x26
#define CCA2_CORR_THRESH    0x27
#define TMR_PRESCALE        0x28
//----------------          0x29
#define GPIO_DATA           0x2A
#define GPIO_DIR            0x2B
#define GPIO_PUL_EN         0x2C
#define GPIO_PUL_SEL        0x2D
#define GPIO_DS             0x2E
//--------------            0x2F
#define ANT_PAD_CTRL        0x30
#define MISC_PAD_CTRL       0x31
#define BSM_CTRL            0x32
//---------------           0x33
#define _RNG                0x34
#define RX_BYTE_COUNT       0x35
#define RX_WTR_MARK         0x36
#define SOFT_RESET          0x37
#define TXDELAY             0x38
#define ACKDELAY            0x39
#define SEQ_MGR_CTRL        0x3A
#define SEQ_MGR_STS         0x3B
#define SEQ_T_STS           0x3C
#define ABORT_STS           0x3D
#define CCCA_BUSY_CNT       0x3E
#define SRC_ADDR_CHECKSUM1  0x3F
#define SRC_ADDR_CHECKSUM2  0x40
#define SRC_TBL_VALID1      0x41
#define SRC_TBL_VALID2      0x42
#define FILTERFAIL_CODE1    0x43
#define FILTERFAIL_CODE2    0x44
#define SLOT_PRELOAD        0x45
//----------------          0x46
#define CORR_VT             0x47
#define SYNC_CTRL           0x48
#define PN_LSB_0            0x49
#define PN_LSB_1            0x4A
#define PN_MSB_0            0x4B
#define PN_MSB_1            0x4C
#define CORR_NVAL           0x4D
#define TX_MODE_CTRL        0x4E
#define SNF_THR             0x4F
#define FAD_THR             0x50
#define ANT_AGC_CTRL        0x51
#define AGC_THR1            0x52
#define AGC_THR2            0x53
#define AGC_HYS             0x54
#define AFC                 0x55
//---------------           0x56
//---------------           0x57
#define PHY_STS             0x58
#define RX_MAX_CORR         0x59
#define RX_MAX_PREAMBLE     0x5A
#define RSSI                0x5B
//---------------           0x5C
//---------------           0x5D
#define PLL_DIG_CTRL        0x5E
#define VCO_CAL             0x5F
#define VCO_BEST_DIFF       0x60
#define VCO_BIAS            0x61
#define KMOD_CTRL           0x62
#define KMOD_CAL            0x63
#define PA_CAL              0x64
#define PA_PWRCAL           0x65
#define ATT_RSSI1           0x66
#define ATT_RSSI2           0x67
#define RSSI_OFFSET         0x68
#define RSSI_SLOPE          0x69
#define RSSI_CAL1           0x6A
#define RSSI_CAL2           0x6B
//---------------           0x6C
//---------------           0x6D
#define XTAL_CTRL           0x6E
#define XTAL_COMP_MIN       0x6F
#define XTAL_COMP_MAX       0x70
#define XTAL_GM             0x71
//---------------           0x72
//---------------           0x73
#define LNA_TUNE            0x74
#define LNA_AGCGAIN         0x75
//---------------           0x76
//---------------           0x77
#define CHF_PMA_GAIN        0x78
#define CHF_IBUF            0x79
#define CHF_QBUF            0x7A
#define CHF_IRIN            0x7B
#define CHF_QRIN            0x7C
#define CHF_IL              0x7D
#define CHF_QL              0x7E
#define CHF_CC1             0x7F
#define CHF_CCL             0x80
#define CHF_CC2             0x81
#define CHF_IROUT           0x82
#define CHF_QROUT           0x83
//---------------           0x84
//---------------           0x85
#define RSSI_CTRL           0x86
//---------------           0x87
//---------------           0x88
#define PA_BIAS             0x89
#define PA_TUNING           0x8A
//---------------           0x8B
//---------------           0x8C
#define PMC_HP_TRIM         0x8D
#define VREGA_TRIM          0x8E
//---------------           0x8F
//---------------           0x90
#define VCO_CTRL1           0x91
#define VCO_CTRL2           0x92
//---------------           0x93
//---------------           0x94
#define ANA_SPARE_OUT1      0x95
#define ANA_SPARE_OUT2      0x96
#define ANA_SPARE_IN        0x97
#define MISCELLANEOUS       0x98
//---------------           0x99
#define SEQ_MGR_OVRD0       0x9A
#define SEQ_MGR_OVRD1       0x9B
#define SEQ_MGR_OVRD2       0x9C
#define SEQ_MGR_OVRD3       0x9D
#define SEQ_MGR_OVRD4       0x9E
#define SEQ_MGR_OVRD5       0x9F
#define SEQ_MGR_OVRD6       0xA0
#define SEQ_MGR_OVRD7       0xA1
//---------------           0xA2
#define TESTMODE_CTRL       0xA3
#define DTM_CTRL1           0xA4
#define DTM_CTRL2           0xA5
#define ATM_CTRL1           0xA6
#define ATM_CTRL2           0xA7
#define ATM_CTRL3           0xA8
//---------------           0xA9
#define LIM_FE_TEST_CTRL    0xAA
#define CHF_TEST_CTRL       0xAB
#define VCO_TEST_CTRL       0xAC
#define PLL_TEST_CTRL       0xAD
#define PA_TEST_CTRL        0xAE
#define PMC_TEST_CTRL       0xAF
#define SCAN_DTM_PROTECT_1  0xFE
#define SCAN_DTM_PROTECT_0  0xFF

// IRQSTS1 bits
#define cIRQSTS1_RX_FRM_PEND         (1<<7)
#define cIRQSTS1_PLL_UNLOCK_IRQ      (1<<6)
#define cIRQSTS1_FILTERFAIL_IRQ      (1<<5)
#define cIRQSTS1_RXWTRMRKIRQ         (1<<4)
#define cIRQSTS1_CCAIRQ              (1<<3)
#define cIRQSTS1_RXIRQ               (1<<2)
#define cIRQSTS1_TXIRQ               (1<<1)
#define cIRQSTS1_SEQIRQ              (1<<0)

typedef union regIRQSTS1_tag {
    uint8_t byte;
    struct {
        uint8_t SEQIRQ: 1;
        uint8_t TXIRQ: 1;
        uint8_t RXIRQ: 1;
        uint8_t CCAIRQ: 1;
        uint8_t RXWTRMRKIRQ: 1;
        uint8_t FILTERFAIL_IRQ: 1;
        uint8_t PLL_UNLOCK_IRQ: 1;
        uint8_t RX_FRM_PEND: 1;
    } bit;
} regIRQSTS1_t;

// IRQSTS2 bits
#define cIRQSTS2_CRCVALID            (1<<7)
#define cIRQSTS2_CCA                 (1<<6)
#define cIRQSTS2_SRCADDR             (1<<5)
#define cIRQSTS2_PI                  (1<<4)
#define cIRQSTS2_TMRSTATUS           (1<<3)
#define cIRQSTS2_ASM_IRQ             (1<<2)
#define cIRQSTS2_PB_ERR_IRQ          (1<<1)
#define cIRQSTS2_WAKE_IRQ            (1<<0)

typedef union regIRQSTS2_tag {
    uint8_t byte;
    struct {
        uint8_t WAKE_IRQ: 1;
        uint8_t PB_ERR_IRQ: 1;
        uint8_t ASM_IRQ: 1;
        uint8_t TMRSTATUS: 1;
        uint8_t PI_: 1;
        uint8_t SRCADDR: 1;
        uint8_t CCA: 1;
        uint8_t CRCVALID: 1;
    } bit;
} regIRQSTS2_t;

// IRQSTS3 bits
#define cIRQSTS3_TMR4MSK             (1<<7)
#define cIRQSTS3_TMR3MSK             (1<<6)
#define cIRQSTS3_TMR2MSK             (1<<5)
#define cIRQSTS3_TMR1MSK             (1<<4)
#define cIRQSTS3_TMR4IRQ             (1<<3)
#define cIRQSTS3_TMR3IRQ             (1<<2)
#define cIRQSTS3_TMR2IRQ             (1<<1)
#define cIRQSTS3_TMR1IRQ             (1<<0)

typedef union regIRQSTS3_tag {
    uint8_t byte;
    struct {
        uint8_t TMR1IRQ: 1;
        uint8_t TMR2IRQ: 1;
        uint8_t TMR3IRQ: 1;
        uint8_t TMR4IRQ: 1;
        uint8_t TMR1MSK: 1;
        uint8_t TMR2MSK: 1;
        uint8_t TMR3MSK: 1;
        uint8_t TMR4MSK: 1;
    } bit;
} regIRQSTS3_t;

// PHY_CTRL1 bits
#define cPHY_CTRL1_TMRTRIGEN           (1<<7)
#define cPHY_CTRL1_SLOTTED             (1<<6)
#define cPHY_CTRL1_CCABFRTX            (1<<5)
#define cPHY_CTRL1_RXACKRQD            (1<<4)
#define cPHY_CTRL1_AUTOACK             (1<<3)
#define cPHY_CTRL1_XCVSEQ              (7<<0)

typedef union regPHY_CTRL1_tag {
    uint8_t byte;
    struct {
        uint8_t XCVSEQ: 3;
        uint8_t AUTOACK: 1;
        uint8_t RXACKRQD: 1;
        uint8_t CCABFRTX: 1;
        uint8_t SLOTTED: 1;
        uint8_t TMRTRIGEN: 1;
    } bit;
} regPHY_CTRL1_t;

// PHY_CTRL2 bits
#define cPHY_CTRL2_CRC_MSK             (1<<7)
#define cPHY_CTRL2_PLL_UNLOCK_MSK      (1<<6)
#define cPHY_CTRL2_FILTERFAIL_MSK      (1<<5)
#define cPHY_CTRL2_RX_WMRK_MSK         (1<<4)
#define cPHY_CTRL2_CCAMSK              (1<<3)
#define cPHY_CTRL2_RXMSK               (1<<2)
#define cPHY_CTRL2_TXMSK               (1<<1)
#define cPHY_CTRL2_SEQMSK              (1<<0)

typedef union regPHY_CTRL2_tag {
    uint8_t byte;
    struct {
        uint8_t SEQMSK: 1;
        uint8_t TXMSK: 1;
        uint8_t RXMSK: 1;
        uint8_t CCAMSK: 1;
        uint8_t RX_WMRK_MSK: 1;
        uint8_t FILTERFAIL_MSK: 1;
        uint8_t PLL_UNLOCK_MSK: 1;
        uint8_t CRC_MSK: 1;
    } bit;
} regPHY_CTRL2_t;

// PHY_CTRL3 bits
#define cPHY_CTRL3_TMR4CMP_EN          (1<<7)
#define cPHY_CTRL3_TMR3CMP_EN          (1<<6)
#define cPHY_CTRL3_TMR2CMP_EN          (1<<5)
#define cPHY_CTRL3_TMR1CMP_EN          (1<<4)
#define cPHY_CTRL3_ASM_MSK             (1<<2)
#define cPHY_CTRL3_PB_ERR_MSK          (1<<1)
#define cPHY_CTRL3_WAKE_MSK            (1<<0)

typedef union regPHY_CTRL3_tag {
    uint8_t byte;
    struct {
        uint8_t WAKE_MSK: 1;
        uint8_t PB_ERR_MSK: 1;
        uint8_t ASM_MSK: 1;
        uint8_t RESERVED: 1;
        uint8_t TMR1CMP_EN: 1;
        uint8_t TMR2CMP_EN: 1;
        uint8_t TMR3CMP_EN: 1;
        uint8_t TMR4CMP_EN: 1;
    } bit;
} regPHY_CTRL3_t;

// RX_FRM_LEN bits
#define cRX_FRAME_LENGTH               (0x7F)

// PHY_CTRL4 bits
#define cPHY_CTRL4_TRCV_MSK            (1<<7)
#define cPHY_CTRL4_TC3TMOUT            (1<<6)
#define cPHY_CTRL4_PANCORDNTR0         (1<<5)
#define cPHY_CTRL4_CCATYPE             (3<<0)
#define cPHY_CTRL4_CCATYPE_Shift_c     (3)
#define cPHY_CTRL4_TMRLOAD             (1<<2)
#define cPHY_CTRL4_PROMISCUOUS         (1<<1)
#define cPHY_CTRL4_TC2PRIME_EN         (1<<0)

typedef union regPHY_CTRL4_tag {
    uint8_t byte;
    struct {
        uint8_t TC2PRIME_EN: 1;
        uint8_t PROMISCUOUS: 1;
        uint8_t TMRLOAD: 1;
        uint8_t CCATYPE: 2;
        uint8_t PANCORDNTR0: 1;
        uint8_t TC3TMOUT: 1;
        uint8_t TRCV_MSK: 1;
    } bit;
} regPHY_CTRL4_t;

// SRC_CTRL bits
#define cSRC_CTRL_INDEX               (0x0F)
#define cSRC_CTRL_INDEX_Shift_c       (4)
#define cSRC_CTRL_ACK_FRM_PND         (1<<3)
#define cSRC_CTRL_SRCADDR_EN          (1<<2)
#define cSRC_CTRL_INDEX_EN            (1<<1)
#define cSRC_CTRL_INDEX_DISABLE       (1<<0)

typedef union regSRC_CTRL_tag {
    uint8_t byte;
    struct {
        uint8_t INDEX_DISABLE: 1;
        uint8_t INDEX_EN: 1;
        uint8_t SRCADDR_EN: 1;
        uint8_t ACK_FRM_PND: 1;
        uint8_t INDEX: 4;
    } bit;
} regSRC_CTRL_t;

// ASM_CTRL1 bits
#define cASM_CTRL1_CLEAR               (1<<7)
#define cASM_CTRL1_START               (1<<6)
#define cASM_CTRL1_SELFTST             (1<<5)
#define cASM_CTRL1_CTR                 (1<<4)
#define cASM_CTRL1_CBC                 (1<<3)
#define cASM_CTRL1_AES                 (1<<2)
#define cASM_CTRL1_LOAD_MAC            (1<<1)

// ASM_CTRL2 bits
#define cASM_CTRL2_DATA_REG_TYPE_SEL          (7)
#define cASM_CTRL2_DATA_REG_TYPE_SEL_Shift_c  (5)
#define cASM_CTRL2_TSTPAS                     (1<<1)

// CLK_OUT_CTRL bits
#define cCLK_OUT_CTRL_EXTEND           (1<<7)
#define cCLK_OUT_CTRL_HIZ              (1<<6)
#define cCLK_OUT_CTRL_SR               (1<<5)
#define cCLK_OUT_CTRL_DS               (1<<4)
#define cCLK_OUT_CTRL_EN               (1<<3)
#define cCLK_OUT_CTRL_DIV              (7)

// PWR_MODES bits
#define cPWR_MODES_XTAL_READY          (1<<5)
#define cPWR_MODES_XTALEN              (1<<4)
#define cPWR_MODES_ASM_CLK_EN          (1<<3)
#define cPWR_MODES_AUTODOZE            (1<<1)
#define cPWR_MODES_PMC_MODE            (1<<0)

// RX_FRAME_FILTER bits
#define cRX_FRAME_FLT_FRM_VER             (0xC0)
#define cRX_FRAME_FLT_FRM_VER_Shift_c     (6)
#define cRX_FRAME_FLT_ACTIVE_PROMISCUOUS  (1<<5)
#define cRX_FRAME_FLT_NS_FT               (1<<4)
#define cRX_FRAME_FLT_CMD_FT              (1<<3)
#define cRX_FRAME_FLT_ACK_FT              (1<<2)
#define cRX_FRAME_FLT_DATA_FT             (1<<1)
#define cRX_FRAME_FLT_BEACON_FT           (1<<0)

typedef union regRX_FRAME_FILTER_tag {
    uint8_t byte;
    struct {
        uint8_t FRAME_FLT_BEACON_FT: 1;
        uint8_t FRAME_FLT_DATA_FT: 1;
        uint8_t FRAME_FLT_ACK_FT: 1;
        uint8_t FRAME_FLT_CMD_FT: 1;
        uint8_t FRAME_FLT_NS_FT: 1;
        uint8_t FRAME_FLT_ACTIVE_PROMISCUOUS: 1;
        uint8_t FRAME_FLT_FRM_VER: 2;
    } bit;
} regRX_FRAME_FILTER_t;

// DUAL_PAN_CTRL bits
#define cDUAL_PAN_CTRL_DUAL_PAN_SAM_LVL_MSK       (0xF0)
#define cDUAL_PAN_CTRL_DUAL_PAN_SAM_LVL_Shift_c   (4)
#define cDUAL_PAN_CTRL_CURRENT_NETWORK            (1<<3)
#define cDUAL_PAN_CTRL_PANCORDNTR1                (1<<2)
#define cDUAL_PAN_CTRL_DUAL_PAN_AUTO              (1<<1)
#define cDUAL_PAN_CTRL_ACTIVE_NETWORK             (1<<0)

// DUAL_PAN_STS bits
#define cDUAL_PAN_STS_RECD_ON_PAN1        (1<<7)
#define cDUAL_PAN_STS_RECD_ON_PAN0        (1<<6)
#define cDUAL_PAN_STS_DUAL_PAN_REMAIN     (0x3F)

// CCA_CTRL bits
#define cCCA_CTRL_AGC_FRZ_EN          (1<<6)
#define cCCA_CTRL_CONT_RSSI_EN        (1<<5)
#define cCCA_CTRL_LQI_RSSI_NOT_CORR   (1<<4)
#define cCCA_CTRL_CCA3_AND_NOT_OR     (1<<3)
#define cCCA_CTRL_POWER_COMP_EN_LQI   (1<<2)
#define cCCA_CTRL_POWER_COMP_EN_ED    (1<<1)
#define cCCA_CTRL_POWER_COMP_EN_CCA1  (1<<0)

// GPIO_DATA bits
#define cGPIO_DATA_7        (1<<7)
#define cGPIO_DATA_6        (1<<6)
#define cGPIO_DATA_5        (1<<5)
#define cGPIO_DATA_4        (1<<4)
#define cGPIO_DATA_3        (1<<3)
#define cGPIO_DATA_2        (1<<2)
#define cGPIO_DATA_1        (1<<1)
#define cGPIO_DATA_0        (1<<0)

// GPIO_DIR bits
#define cGPIO_DIR_7         (1<<7)
#define cGPIO_DIR_6         (1<<6)
#define cGPIO_DIR_5         (1<<5)
#define cGPIO_DIR_4         (1<<4)
#define cGPIO_DIR_3         (1<<3)
#define cGPIO_DIR_2         (1<<2)
#define cGPIO_DIR_1         (1<<1)
#define cGPIO_DIR_0         (1<<0)

// GPIO_PUL_EN bits
#define cGPIO_PUL_EN_7      (1<<7)
#define cGPIO_PUL_EN_6      (1<<6)
#define cGPIO_PUL_EN_5      (1<<5)
#define cGPIO_PUL_EN_4      (1<<4)
#define cGPIO_PUL_EN_3      (1<<3)
#define cGPIO_PUL_EN_2      (1<<2)
#define cGPIO_PUL_EN_1      (1<<1)
#define cGPIO_PUL_EN_0      (1<<0)

// GPIO_PUL_SEL bits
#define cGPIO_PUL_SEL_7     (1<<7)
#define cGPIO_PUL_SEL_6     (1<<6)
#define cGPIO_PUL_SEL_5     (1<<5)
#define cGPIO_PUL_SEL_4     (1<<4)
#define cGPIO_PUL_SEL_3     (1<<3)
#define cGPIO_PUL_SEL_2     (1<<2)
#define cGPIO_PUL_SEL_1     (1<<1)
#define cGPIO_PUL_SEL_0     (1<<0)

// GPIO_DS bits
#define cGPIO_DS_7          (1<<7)
#define cGPIO_DS_6          (1<<6)
#define cGPIO_DS_5          (1<<5)
#define cGPIO_DS_4          (1<<4)
#define cGPIO_DS_3          (1<<3)
#define cGPIO_DS_2          (1<<2)
#define cGPIO_DS_1          (1<<1)
#define cGPIO_DS_0          (1<<0)

// SPI_CTRL bits
//#define cSPI_CTRL_MISO_HIZ_EN        (1<<1)
//#define cSPI_CTRL_PB_PROTECT         (1<<0)

// ANT_PAD_CTRL bits
#define cANT_PAD_CTRL_ANTX_POL           (0x0F)
#define cANT_PAD_CTRL_ANTX_POL_Shift_c   (4)
#define cANT_PAD_CTRL_ANTX_CTRLMODE      (1<<3)
#define cANT_PAD_CTRL_ANTX_HZ            (1<<2)
#define cANT_PAD_CTRL_ANTX_EN            (3)

// MISC_PAD_CTRL bits
#define cMISC_PAD_CTRL_MISO_HIZ_EN        (1<<3)
#define cMISC_PAD_CTRL_IRQ_B_OD           (1<<2)
#define cMISC_PAD_CTRL_NON_GPIO_DS        (1<<1)
#define cMISC_PAD_CTRL_ANTX_CURR          (1<<0)

// ANT_AGC_CTRL bits
#define cANT_AGC_CTRL_FAD_EN_Shift_c    (0)
#define cANT_AGC_CTRL_FAD_EN_Mask_c     (1<<cANT_AGC_CTRL_FAD_EN_Shift_c)
#define cANT_AGC_CTRL_ANTX_Shift_c      (1)
#define cANT_AGC_CTRL_ANTX_Mask_c       (1<<cANT_AGC_CTRL_ANTX_Shift_c)

// BSM_CTRL bits
#define cBSM_CTRL_BSM_EN                  (1<<0)

// SOFT_RESET bits
#define cSOFT_RESET_SOG_RST            (1<<7)
#define cSOFT_RESET_REGS_RST           (1<<4)
#define cSOFT_RESET_PLL_RST            (1<<3)
#define cSOFT_RESET_TX_RST             (1<<2)
#define cSOFT_RESET_RX_RST             (1<<1)
#define cSOFT_RESET_SEQ_MGR_RST        (1<<0)

// SEQ_MGR_CTRL bits
#define cSEQ_MGR_CTRL_SEQ_STATE_CTRL          (3)
#define cSEQ_MGR_CTRL_SEQ_STATE_CTRL_Shift_c  (6)
#define cSEQ_MGR_CTRL_NO_RX_RECYCLE           (1<<5)
#define cSEQ_MGR_CTRL_LATCH_PREAMBLE          (1<<4)
#define cSEQ_MGR_CTRL_EVENT_TMR_DO_NOT_LATCH  (1<<3)
#define cSEQ_MGR_CTRL_CLR_NEW_SEQ_INHIBIT     (1<<2)
#define cSEQ_MGR_CTRL_PSM_LOCK_DIS            (1<<1)
#define cSEQ_MGR_CTRL_PLL_ABORT_OVRD          (1<<0)

// SEQ_MGR_STS bits
#define cSEQ_MGR_STS_TMR2_SEQ_TRIG_ARMED (1<<7)
#define cSEQ_MGR_STS_RX_MODE             (1<<6)
#define cSEQ_MGR_STS_RX_TIMEOUT_PENDING  (1<<5)
#define cSEQ_MGR_STS_NEW_SEQ_INHIBIT     (1<<4)
#define cSEQ_MGR_STS_SEQ_IDLE            (1<<3)
#define cSEQ_MGR_STS_XCVSEQ_ACTUAL       (7)

// ABORT_STS bits
#define cABORT_STS_PLL_ABORTED        (1<<2)
#define cABORT_STS_TC3_ABORTED        (1<<1)
#define cABORT_STS_SW_ABORTED         (1<<0)

// FILTERFAIL_CODE2 bits
#define cFILTERFAIL_CODE2_PAN_SEL  (1<<7)
#define cFILTERFAIL_CODE2_9_8      (3)

// PHY_STS bits
#define cPHY_STS_PLL_UNLOCK  (1<<7)
#define cPHY_STS_PLL_LOCK_ERR        (1<<6)
#define cPHY_STS_PLL_LOCK            (1<<5)
#define cPHY_STS_CRCVALID            (1<<3)
#define cPHY_STS_FILTERFAIL_FLAG_SEL (1<<2)
#define cPHY_STS_SFD_DET             (1<<1)
#define cPHY_STS_PREAMBLE_DET        (1<<0)

// TESTMODE_CTRL bits
#define cTEST_MODE_CTRL_HOT_ANT            (1<<4)
#define cTEST_MODE_CTRL_IDEAL_RSSI_EN      (1<<3)
#define cTEST_MODE_CTRL_IDEAL_PFC_EN       (1<<2)
#define cTEST_MODE_CTRL_CONTINUOUS_EN      (1<<1)
#define cTEST_MODE_CTRL_FPGA_EN            (1<<0)

// DTM_CTRL1 bits
#define cDTM_CTRL1_ATM_LOCKED  (1<<7)
#define cDTM_CTRL1_DTM_EN      (1<<6)
#define cDTM_CTRL1_PAGE5       (1<<5)
#define cDTM_CTRL1_PAGE4       (1<<4)
#define cDTM_CTRL1_PAGE3       (1<<3)
#define cDTM_CTRL1_PAGE2       (1<<2)
#define cDTM_CTRL1_PAGE1       (1<<1)
#define cDTM_CTRL1_PAGE0       (1<<0)

// TX_MODE_CTRL
#define cTX_MODE_CTRL_TX_INV   (1<<4)
#define cTX_MODE_CTRL_BT_EN    (1<<3)
#define cTX_MODE_CTRL_DTS2     (1<<2)
#define cTX_MODE_CTRL_DTS1     (1<<1)
#define cTX_MODE_CTRL_DTS0     (1<<0)

#define cTX_MODE_CTRL_DTS_MASK (7)

// CLK_OUT_CTRL bits
#define cCLK_OUT_EXTEND        (1<<7)
#define cCLK_OUT_HIZ           (1<<6)
#define cCLK_OUT_SR            (1<<5)
#define cCLK_OUT_DS            (1<<4)
#define cCLK_OUT_EN            (1<<3)
#define cCLK_OUT_DIV_Mask      (7<<0)

#define gCLK_OUT_FREQ_32_MHz      (0)
#define gCLK_OUT_FREQ_16_MHz      (1)
#define gCLK_OUT_FREQ_8_MHz       (2)
#define gCLK_OUT_FREQ_4_MHz       (3)
#define gCLK_OUT_FREQ_1_MHz       (4)
#define gCLK_OUT_FREQ_250_KHz     (5)
#define gCLK_OUT_FREQ_62_5_KHz    (6)
#define gCLK_OUT_FREQ_32_78_KHz   (7)
#define gCLK_OUT_FREQ_DISABLE     (8)




#endif /* __MCR20_REG_H__ */
