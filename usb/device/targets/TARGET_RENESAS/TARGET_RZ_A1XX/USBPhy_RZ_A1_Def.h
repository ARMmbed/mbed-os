/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited, All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef USBPHY_RZ_A1_DEF
#define USBPHY_RZ_A1_DEF

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* H/W function type */
#define USB_BIT0                ((uint16_t)0x0001)
#define USB_BIT1                ((uint16_t)0x0002)
#define USB_BIT2                ((uint16_t)0x0004)
#define USB_BIT3                ((uint16_t)0x0008)
#define USB_BIT4                ((uint16_t)0x0010)
#define USB_BIT5                ((uint16_t)0x0020)
#define USB_BIT6                ((uint16_t)0x0040)
#define USB_BIT7                ((uint16_t)0x0080)
#define USB_BIT8                ((uint16_t)0x0100)
#define USB_BIT9                ((uint16_t)0x0200)
#define USB_BIT10               ((uint16_t)0x0400)
#define USB_BIT11               ((uint16_t)0x0800)
#define USB_BIT12               ((uint16_t)0x1000)
#define USB_BIT13               ((uint16_t)0x2000)
#define USB_BIT14               ((uint16_t)0x4000)
#define USB_BIT15               ((uint16_t)0x8000)
#define USB_BITSET(x)           ((uint16_t)((uint16_t)1 << (x)))

/* Start Pipe No */
#define USB_MIN_PIPE_NO         (1u)

/* Pipe configuration table define */
#define USB_EPL                 (6u)        /* Pipe configuration table length */
#define USB_TYPFIELD            (0xC000u)   /* Transfer type */
#define USB_PERIODIC            (0x8000u)   /* Periodic pipe */
#define USB_TYPFIELD_ISO        (0xC000u)   /* Isochronous */
#define USB_TYPFIELD_INT        (0x8000u)   /* Interrupt */
#define USB_TYPFIELD_BULK       (0x4000u)   /* Bulk */
#define USB_NOUSE               (0x0000u)   /* Not configuration */
#define USB_BFREFIELD           (0x0400u)   /* Buffer ready interrupt mode select */
#define USB_BFREON              (0x0400u)
#define USB_BFREOFF             (0x0000u)
#define USB_DBLBFIELD           (0x0200u)   /* Double buffer mode select */
#define USB_CFG_DBLBON          (0x0200u)
#define USB_CFG_DBLBOFF         (0x0000u)
#define USB_CNTMDFIELD          (0x0100u)   /* Continuous transfer mode select */
#define USB_CFG_CNTMDON         (0x0100u)
#define USB_CFG_CNTMDOFF        (0x0000u)
#define USB_SHTNAKFIELD         (0x0080u)   /* Transfer end NAK */
#define USB_DIRFIELD            (0x0010u)   /* Transfer direction select */
#define USB_DIR_H_OUT           (0x0010u)   /* HOST OUT */
#define USB_DIR_P_IN            (0x0010u)   /* PERI IN */
#define USB_DIR_H_IN            (0x0000u)   /* HOST IN */
#define USB_DIR_P_OUT           (0x0000u)   /* PERI OUT */
#define USB_BUF2FIFO            (0x0010u)   /* Buffer --> FIFO */
#define USB_FIFO2BUF            (0x0000u)   /* FIFO --> buffer */
#define USB_EPNUMFIELD          (0x000Fu)   /* Endpoint number select */
#define USB_MAX_EP_NO           (15u)       /* EP0 EP1 ... EP15 */

#define USB_BUF_SIZE(x)         ((uint16_t)(((x) / 64u) - 1u) << 10u)
#define USB_BUF_NUMB(x)         (x)

/* FIFO read / write result */
#define USB_FIFOERROR           (0x00ffu)   /* FIFO not ready */
#define USB_WRITEEND            (0x0000u)   /* End of write (but packet may not be outputting) */
#define USB_WRITESHRT           (0x0001u)   /* End of write (send short packet) */
#define USB_WRITING             (0x0002u)   /* Write continues */
#define USB_READEND             (0x0000u)   /* End of read */
#define USB_READSHRT            (0x0001u)   /* Insufficient (receive short packet) */
#define USB_READING             (0x0002u)   /* Read continues */
#define USB_READOVER            (0x0003u)   /* Buffer size over */

/* Transfer status Type */
#define USB_CTRL_END            (0u)
#define USB_DATA_NONE           (1u)
#define USB_DATA_WAIT           (2u)
#define USB_DATA_OK             (3u)
#define USB_DATA_SHT            (4u)
#define USB_DATA_OVR            (5u)
#define USB_DATA_STALL          (6u)
#define USB_DATA_ERR            (7u)
#define USB_DATA_STOP           (8u)
#define USB_DATA_TMO            (9u)
#define USB_CTRL_READING        (17u)
#define USB_CTRL_WRITING        (18u)
#define USB_DATA_READING        (19u)
#define USB_DATA_WRITING        (20u)


/* System Configuration Control Register */
#define USB_HSE                 (0x0080u)   /* b7: Hi-speed enable */
#define USB_DCFM                (0x0040u)   /* b6: Function select */
#define USB_DRPD                (0x0020u)   /* b5: D+/D- pull down control */
#define USB_DPRPU               (0x0010u)   /* b4: D+ pull up control */
#define USB_DMRPU               (0x0008u)   /* b3: D- pull up control */ /* For low speed */
#define USB_UCKSEL              (0x0004u)   /* b2: USB clock select */
#define USB_EXTAL_12MHZ         (0x0004u)   /* EXTAL 12MHz */
#define USB_X1_48MHZ            (0x0000u)   /* USB_X1 48MHz */
#define USB_UPLLE               (0x0002u)   /* b1: USB internal PLL enable */
#define USB_USBE                (0x0001u)   /* b0: USB module enable */

/* CPU Bus Wait Register */
#define USB_BWAIT               (0x003Fu)   /* b5-0: Bus wait bit */
#define USB_BWAIT_15            (0x000Fu)   /* 15 wait (access cycle 17) */
#define USB_BWAIT_14            (0x000Eu)   /* 14 wait (access cycle 16) */
#define USB_BWAIT_13            (0x000Du)   /* 13 wait (access cycle 15) */
#define USB_BWAIT_12            (0x000Cu)   /* 12 wait (access cycle 14) */
#define USB_BWAIT_11            (0x000Bu)   /* 11 wait (access cycle 13) */
#define USB_BWAIT_10            (0x000Au)   /* 10 wait (access cycle 12) */
#define USB_BWAIT_9             (0x0009u)   /*  9 wait (access cycle 11) */
#define USB_BWAIT_8             (0x0008u)   /*  8 wait (access cycle 10) */
#define USB_BWAIT_7             (0x0007u)   /*  7 wait (access cycle  9) */
#define USB_BWAIT_6             (0x0006u)   /*  6 wait (access cycle  8) */
#define USB_BWAIT_5             (0x0005u)   /*  5 wait (access cycle  7) */
#define USB_BWAIT_4             (0x0004u)   /*  4 wait (access cycle  6) */
#define USB_BWAIT_3             (0x0003u)   /*  3 wait (access cycle  5) */
#define USB_BWAIT_2             (0x0002u)   /*  2 wait (access cycle  4) */
#define USB_BWAIT_1             (0x0001u)   /*  1 wait (access cycle  3) */
#define USB_BWAIT_0             (0x0000u)   /*  0 wait (access cycle  2) */

/* System Configuration Status Register */
#define USB_OVCMON              (0xC000u)   /* b15-14: Over-current monitor */
#define USB_OVCBIT              (0x8000u)   /* b15-14: Over-current bit */
#define USB_HTACT               (0x0040u)   /* b6: USB Host Sequencer Status Monitor */
#define USB_SOFEA               (0x0020u)   /* b5: SOF monitor */
#define USB_IDMON               (0x0004u)   /* b2: ID-pin monitor */
#define USB_LNST                (0x0003u)   /* b1-0: D+, D- line status */
#define USB_SE1                 (0x0003u)   /* SE1 */
#define USB_FS_KSTS             (0x0002u)   /* Full-Speed K State */
#define USB_FS_JSTS             (0x0001u)   /* Full-Speed J State */
#define USB_LS_JSTS             (0x0002u)   /* Low-Speed J State */
#define USB_LS_KSTS             (0x0001u)   /* Low-Speed K State */
#define USB_SE0                 (0x0000u)   /* SE0 */

/* PLL Status Register */
#define USB_PLLLOCK             (0x0001u)

/* Device State Control Register */
#define USB_HNPBTOA             (0x0800u)   /* b11: Host negotiation protocol (BtoA) */
#define USB_EXICEN              (0x0400u)   /* b10: EXICEN output terminal control */
#define USB_VBUSEN              (0x0200u)   /* b9: VBUS output terminal control */
#define USB_WKUP                (0x0100u)   /* b8: Remote wakeup */
#define USB_RWUPE               (0x0080u)   /* b7: Remote wakeup sense */
#define USB_USBRST              (0x0040u)   /* b6: USB reset enable */
#define USB_RESUME              (0x0020u)   /* b5: Resume enable */
#define USB_UACT                (0x0010u)   /* b4: USB bus enable */
#define USB_RHST                (0x0007u)   /* b2-0: Reset handshake status */
#define USB_HSPROC              (0x0004u)   /* HS handshake processing */
#define USB_HSMODE              (0x0003u)   /* Hi-Speed mode */
#define USB_FSMODE              (0x0002u)   /* Full-Speed mode */
#define USB_LSMODE              (0x0001u)   /* Low-Speed mode */
#define USB_UNDECID             (0x0000u)   /* Undecided */

/* Test Mode Register */
#define USB_UTST                (0x000Fu)   /* b3-0: Test mode */
#define USB_H_TST_F_EN          (0x000Du)   /* HOST TEST FORCE ENABLE */
#define USB_H_TST_PACKET        (0x000Cu)   /* HOST TEST Packet */
#define USB_H_TST_SE0_NAK       (0x000Bu)   /* HOST TEST SE0 NAK */
#define USB_H_TST_K             (0x000Au)   /* HOST TEST K */
#define USB_H_TST_J             (0x0009u)   /* HOST TEST J */
#define USB_H_TST_NORMAL        (0x0000u)   /* HOST Normal Mode */
#define USB_P_TST_PACKET        (0x0004u)   /* PERI TEST Packet */
#define USB_P_TST_SE0_NAK       (0x0003u)   /* PERI TEST SE0 NAK */
#define USB_P_TST_K             (0x0002u)   /* PERI TEST K */
#define USB_P_TST_J             (0x0001u)   /* PERI TEST J */
#define USB_P_TST_NORMAL        (0x0000u)   /* PERI Normal Mode */

/* CFIFO/DxFIFO Port Select Register */
#define USB_RCNT                (0x8000u)   /* b15: Read count mode */
#define USB_REW                 (0x4000u)   /* b14: Buffer rewind */
#define USB_DCLRM               (0x2000u)   /* b13: Automatic buffer clear mode */
#define USB_DREQE               (0x1000u)   /* b12: DREQ output enable */
#define USB_MBW                 (0x0C00u)   /* b10: Maximum bit width for FIFO access */
#define USB_MBW_32              (0x0800u)   /* FIFO access : 32bit */
#define USB_MBW_16              (0x0400u)   /* FIFO access : 16bit */
#define USB_MBW_8               (0x0000u)   /* FIFO access : 8bit */
#define USB_BIGEND              (0x0100u)   /* b8: Big endian mode */
#define USB_FIFO_BIG            (0x0100u)   /* Big endian */
#define USB_FIFO_LITTLE         (0x0000u)   /* Little endian */
#define USB_ISEL                (0x0020u)   /* b5: DCP FIFO port direction select */
#define USB_ISEL_WRITE          (0x0020u)   /* write */
#define USB_ISEL_READ           (0x0000u)   /* read */
#define USB_CURPIPE             (0x000Fu)   /* b2-0: PIPE select */

/* CFIFO/DxFIFO Port Control Register */
#define USB_BVAL                (0x8000u)   /* b15: Buffer valid flag */
#define USB_BCLR                (0x4000u)   /* b14: Buffer clear */
#define USB_FRDY                (0x2000u)   /* b13: FIFO ready */
#define USB_DTLN                (0x0FFFu)   /* b11-0: FIFO data length */

/* Interrupt Enable Register 0 */
#define USB_VBSE                (0x8000u)   /* b15: VBUS interrupt */
#define USB_RSME                (0x4000u)   /* b14: Resume interrupt */
#define USB_SOFE                (0x2000u)   /* b13: Frame update interrupt */
#define USB_DVSE                (0x1000u)   /* b12: Device state transition interrupt */
#define USB_CTRE                (0x0800u)   /* b11: Control transfer stage transition interrupt */
#define USB_BEMPE               (0x0400u)   /* b10: Buffer empty interrupt */
#define USB_NRDYE               (0x0200u)   /* b9: Buffer notready interrupt */
#define USB_BRDYE               (0x0100u)   /* b8: Buffer ready interrupt */

/* Interrupt Enable Register 1 */
#define USB_OVRCRE              (0x8000u)   /* b15: Over-current interrupt */
#define USB_BCHGE               (0x4000u)   /* b14: USB bus change interrupt */
#define USB_DTCHE               (0x1000u)   /* b12: Detach sense interrupt */
#define USB_ATTCHE              (0x0800u)   /* b11: Attach sense interrupt */
#define USB_L1RSMENDE           (0x0200u)   /* b9: L1 resume completion interrupt */
#define USB_LPMENDE             (0x0100u)   /* b8: LPM transaction completion interrupt */
#define USB_EOFERRE             (0x0040u)   /* b6: EOF error interrupt */
#define USB_SIGNE               (0x0020u)   /* b5: SETUP IGNORE interrupt */
#define USB_SACKE               (0x0010u)   /* b4: SETUP ACK interrupt */
#define USB_PDDETINTE           (0x0001u)   /* b0: PDDET detection interrupt */

/* BRDY Interrupt Enable/Status Register */
#define USB_BRDY9               (0x0200u)   /* b9: PIPE9 */
#define USB_BRDY8               (0x0100u)   /* b8: PIPE8 */
#define USB_BRDY7               (0x0080u)   /* b7: PIPE7 */
#define USB_BRDY6               (0x0040u)   /* b6: PIPE6 */
#define USB_BRDY5               (0x0020u)   /* b5: PIPE5 */
#define USB_BRDY4               (0x0010u)   /* b4: PIPE4 */
#define USB_BRDY3               (0x0008u)   /* b3: PIPE3 */
#define USB_BRDY2               (0x0004u)   /* b2: PIPE2 */
#define USB_BRDY1               (0x0002u)   /* b1: PIPE1 */
#define USB_BRDY0               (0x0001u)   /* b1: PIPE0 */

/* NRDY Interrupt Enable/Status Register */
#define USB_NRDY9               (0x0200u)   /* b9: PIPE9 */
#define USB_NRDY8               (0x0100u)   /* b8: PIPE8 */
#define USB_NRDY7               (0x0080u)   /* b7: PIPE7 */
#define USB_NRDY6               (0x0040u)   /* b6: PIPE6 */
#define USB_NRDY5               (0x0020u)   /* b5: PIPE5 */
#define USB_NRDY4               (0x0010u)   /* b4: PIPE4 */
#define USB_NRDY3               (0x0008u)   /* b3: PIPE3 */
#define USB_NRDY2               (0x0004u)   /* b2: PIPE2 */
#define USB_NRDY1               (0x0002u)   /* b1: PIPE1 */
#define USB_NRDY0               (0x0001u)   /* b1: PIPE0 */

/* BEMP Interrupt Enable/Status Register */
#define USB_BEMP9               (0x0200u)   /* b9: PIPE9 */
#define USB_BEMP8               (0x0100u)   /* b8: PIPE8 */
#define USB_BEMP7               (0x0080u)   /* b7: PIPE7 */
#define USB_BEMP6               (0x0040u)   /* b6: PIPE6 */
#define USB_BEMP5               (0x0020u)   /* b5: PIPE5 */
#define USB_BEMP4               (0x0010u)   /* b4: PIPE4 */
#define USB_BEMP3               (0x0008u)   /* b3: PIPE3 */
#define USB_BEMP2               (0x0004u)   /* b2: PIPE2 */
#define USB_BEMP1               (0x0002u)   /* b1: PIPE1 */
#define USB_BEMP0               (0x0001u)   /* b0: PIPE0 */

/* SOF Pin Configuration Register */
#define USB_TRNENSEL            (0x0100u)   /* b8: Select transaction enable period */
#define USB_BRDYM               (0x0040u)   /* b6: BRDY clear timing */
#define USB_INTL                (0x0020u)   /* b5: Interrupt sense select */
#define USB_EDGESTS             (0x0010u)   /* b4:  */
#define USB_SOFMODE             (0x000Cu)   /* b3-2: SOF pin select */
#define USB_SOF_125US           (0x0008u)   /* SOF 125us Frame Signal */
#define USB_SOF_1MS             (0x0004u)   /* SOF 1ms Frame Signal */
#define USB_SOF_DISABLE         (0x0000u)   /* SOF Disable */

#define USB_HSEB                (0x8000u)   /* b15: CL only mode bit */

#define USB_REPSTART            (0x0800u)   /* b11: Terminator adjustment forcible starting bit */
#define USB_REPSEL              (0x0300u)   /* b9-8: Terminator adjustment cycle setting */
#define USB_REPSEL_128          (0x0300u)   /* 128 sec */
#define USB_REPSEL_64           (0x0200u)   /* 64 sec */
#define USB_REPSEL_16           (0x0100u)   /* 16 sec */
#define USB_REPSEL_NONE         (0x0000u)   /* - */
#define USB_CLKSEL              (0x0030u)   /* b5-4: System clock setting */
#define USB_CLKSEL_24           (0x0030u)   /* 24MHz */
#define USB_CLKSEL_20           (0x0020u)   /* 20MHz */
#define USB_CLKSEL_48           (0x0010u)   /* 48MHz */
#define USB_CLKSEL_30           (0x0000u)   /* 30MHz */
#define USB_CDPEN               (0x0008u)   /* b3: Charging downstream port enable */
#define USB_PLLRESET            (0x0002u)   /* b1: PLL reset control */
#define USB_DIRPD               (0x0001u)   /* b0: Power down control */

/* Interrupt Status Register 0 */
#define USB_VBINT               (0x8000u)   /* b15: VBUS interrupt */
#define USB_RESM                (0x4000u)   /* b14: Resume interrupt */
#define USB_SOFR                (0x2000u)   /* b13: SOF update interrupt */
#define USB_DVST                (0x1000u)   /* b12: Device state transition interrupt */
#define USB_CTRT                (0x0800u)   /* b11: Control transfer stage transition interrupt */
#define USB_BEMP                (0x0400u)   /* b10: Buffer empty interrupt */
#define USB_NRDY                (0x0200u)   /* b9: Buffer notready interrupt */
#define USB_BRDY                (0x0100u)   /* b8: Buffer ready interrupt */
#define USB_VBSTS               (0x0080u)   /* b7: VBUS input port */
#define USB_DVSQ                (0x0070u)   /* b6-4: Device state */
#define USB_DS_SPD_CNFG         (0x0070u)   /* Suspend Configured */
#define USB_DS_SPD_ADDR         (0x0060u)   /* Suspend Address */
#define USB_DS_SPD_DFLT         (0x0050u)   /* Suspend Default */
#define USB_DS_SPD_POWR         (0x0040u)   /* Suspend Powered */
#define USB_DS_SUSP             (0x0040u)   /* Suspend */
#define USB_DS_CNFG             (0x0030u)   /* Configured */
#define USB_DS_ADDS             (0x0020u)   /* Address */
#define USB_DS_DFLT             (0x0010u)   /* Default */
#define USB_DS_POWR             (0x0000u)   /* Powered */
#define USB_DVSQS               (0x0030u)   /* b5-4: Device state */
#define USB_VALID               (0x0008u)   /* b3: Setup packet detect flag */
#define USB_CTSQ                (0x0007u)   /* b2-0: Control transfer stage */
#define USB_CS_SQER             (0x0006u)   /* Sequence error */
#define USB_CS_WRND             (0x0005u)   /* Ctrl write nodata status stage */
#define USB_CS_WRSS             (0x0004u)   /* Ctrl write status stage */
#define USB_CS_WRDS             (0x0003u)   /* Ctrl write data stage */
#define USB_CS_RDSS             (0x0002u)   /* Ctrl read status stage */
#define USB_CS_RDDS             (0x0001u)   /* Ctrl read data stage */
#define USB_CS_IDST             (0x0000u)   /* Idle or setup stage */

/* Interrupt Status Register 1 */
#define USB_OVRCR               (0x8000u)   /* b15: Over-current interrupt */
#define USB_BCHG                (0x4000u)   /* b14: USB bus change interrupt */
#define USB_DTCH                (0x1000u)   /* b12: Detach sense interrupt */
#define USB_ATTCH               (0x0800u)   /* b11: Attach sense interrupt */
#define USB_L1RSMEND            (0x0200u)   /* b9: L1 resume completion interrupt */
#define USB_LPMEND              (0x0100u)   /* b8: LPM transaction completion interrupt */
#define USB_EOFERR              (0x0040u)   /* b6: EOF-error interrupt */
#define USB_SIGN                (0x0020u)   /* b5: Setup ignore interrupt */
#define USB_SACK                (0x0010u)   /* b4: Setup ack interrupt */
#define USB_PDDETINT            (0x0001u)   /* b0: PDDET detection interrupt */

/* Frame Number Register */
#define USB_OVRN                (0x8000u)   /* b15: Overrun error */
#define USB_CRCE                (0x4000u)   /* b14: Received data error */
#define USB_FRNM                (0x07FFu)   /* b10-0: Frame number */

/* Device State Change Register */ /* For USB0 */
#define USB_DVCHG               (0x8000u)   /* b15: Device state change */

/* Micro Frame Number Register */ /* For USBHS */
#define USB_UFRNM               (0x0007u)   /* b2-0: Micro frame number */

/* USB Address / Low Power Status Recovery Register */
#define USB_STSRECOV            (0x0F00u)   /* b11-8: Status Recovery */
#define USB_USBADDR_MASK        (0x007Fu)   /* b6-0: USB address */

/* USB Request Type Register */
#define USB_BMREQUESTTYPE       (0x00FFu)   /* b7-0: USB_BMREQUESTTYPE */
#define USB_BMREQUESTTYPEDIR    (0x0080u)   /* b7  : Data transfer direction */
#define USB_BMREQUESTTYPETYPE   (0x0060u)   /* b6-5: Type */
#define USB_BMREQUESTTYPERECIP  (0x001Fu)   /* b4-0: Recipient */

/* USB Request Value Register */
#define USB_WVALUE              (0xFFFFu)   /* b15-0: wValue */
#define USB_DT_TYPE             (0xFF00u)
#define USB_GET_DT_TYPE(v)      (((v) & USB_DT_TYPE) >> 8)
#define USB_DT_INDEX            (0x00FFu)
#define USB_CONF_NUM            (0x00FFu)
#define USB_ALT_SET             (0x00FFu)

/* USB Request Index Register */
#define USB_WINDEX              (0xFFFFu)   /* b15-0: wIndex */
#define USB_TEST_SELECT         (0xFF00u)   /* b15-b8: Test Mode Selectors */
#define USB_TEST_J              (0x0100u)   /* Test_J */
#define USB_TEST_K              (0x0200u)   /* Test_K */
#define USB_TEST_SE0_NAK        (0x0300u)   /* Test_SE0_NAK */
#define USB_TEST_PACKET         (0x0400u)   /* Test_Packet */
#define USB_TEST_FORCE_ENABLE   (0x0500u)   /* Test_Force_Enable */
#define USB_TEST_STSelectors    (0x0600u)   /* Standard test selectors */
#define USB_TEST_RESERVED       (0x4000u)   /* Reserved */
#define USB_TEST_VSTMODES       (0xC000u)   /* VendorSpecific test modes */
#define USB_EP_DIR              (0x0080u)   /* b7: Endpoint Direction */
#define USB_EP_DIR_IN           (0x0080u)
#define USB_EP_DIR_OUT          (0x0000u)

/* USB Request Length Register */
#define USB_WLENGTH             (0xFFFFu)   /* b15-0: wLength */

#define USB_TYPE                (0xC000u)   /* b15-14: Transfer type */
#define USB_BFRE                (0x0400u)   /* b10: Buffer ready interrupt mode select */

#define USB_DEVSEL              (0xF000u)   /* b15-14: Device address select */
#define USB_MAXP                (0x007Fu)   /* b6-0: Maxpacket size of default control pipe */
#define USB_MXPS                (0x07FFu)   /* b10-0: Maxpacket size */

#define USB_BSTS                (0x8000u)   /* b15: Buffer status */
#define USB_SUREQ               (0x4000u)   /* b14: Send USB request  */
#define USB_INBUFM              (0x4000u)   /* b14: IN buffer monitor (Only for PIPE1 to 5) */
#define USB_CSCLR               (0x2000u)   /* b13: c-split status clear */
#define USB_CSSTS               (0x1000u)   /* b12: c-split status */
#define USB_SUREQCLR            (0x0800u)   /* b11: stop setup request */
#define USB_ATREPM              (0x0400u)   /* b10: Auto repeat mode */
#define USB_ACLRM               (0x0200u)   /* b9: buffer auto clear mode */
#define USB_SQCLR               (0x0100u)   /* b8: Sequence bit clear */
#define USB_SQSET               (0x0080u)   /* b7: Sequence bit set */
#define USB_SQMON               (0x0040u)   /* b6: Sequence bit monitor */
#define USB_PBUSY               (0x0020u)   /* b5: pipe busy */
#define USB_PINGE               (0x0010u)   /* b4: ping enable */
#define USB_CCPL                (0x0004u)   /* b2: Enable control transfer complete */
#define USB_PID                 (0x0003u)   /* b1-0: Response PID */
#define USB_PID_STALL2          (0x0003u)   /* STALL2 */
#define USB_PID_STALL           (0x0002u)   /* STALL */
#define USB_PID_BUF             (0x0001u)   /* BUF */
#define USB_PID_NAK             (0x0000u)   /* NAK */

#define USB_PIPENM              (0x0007u)   /* b2-0: Pipe select */

#define USB_BUFSIZE             (0x7C00u)   /* b14-10: Pipe buffer size */
#define USB_BUFNMB              (0x007Fu)   /* b6-0: Pipe buffer number */
#define USB_PIPE0BUF            (256u)
#define USB_PIPEXBUF            (64u)

#define USB_TRENB               (0x0200u)   /* b9: Transaction count enable */
#define USB_TRCLR               (0x0100u)   /* b8: Transaction count clear */
#define USB_TRNCNT              (0xFFFFu)   /* b15-0: Transaction counter */

#define USB_UPPHUB              (0x7800u)   /* b14-11: HUB register */
#define USB_HUBPORT             (0x0700u)   /* b10-8: HUB port */
#define USB_USBSPD              (0x00C0u)   /* b7-6: Device speed */

/********* USB0 Only ******************************************************************************/

/* PHY Crosspoint Adjustment Register */
/* PHYSLEW */
#define USB_SLEWF01             (0x0008u)   /* b3: Cross point adjustment bit 01 */
#define USB_SLEWF00             (0x0004u)   /* b2: Cross point adjustment bit 00 */
#define USB_SLEWR01             (0x0002u)   /* b1: Cross point adjustment bit 01 */
#define USB_SLEWR00             (0x0001u)   /* b0: Cross point adjustment bit 00 */

/* Deep Standby USB Transceiver Control/Terminal Monitor Register */
/* DPUSR0R */
#define USB_DVBSTS0             (0x00800000u)   /* b23: USB0 VBUS input */
#define USB_DOVCB0              (0x00200000u)   /* b21: USB0 OVRCURB input */
#define USB_DOVCA0              (0x00100000u)   /* b20: USB0 OVRCURA input */
#define USB_DM0                 (0x00200000u)   /* b17: USB0 D- input */
#define USB_DP0                 (0x00100000u)   /* b16: USB0 D+ input */
#define USB_FIXPHY0             (0x00000010u)   /* b4: USB0 transceiver output fixed bit  */
#define USB_DRPD0               (0x00000008u)   /* b3: D+/D- pull down resistor control bit */
#define USB_RPUE0               (0x00000002u)   /* b1: DP pull up resistor control bit */
#define USB_SRPC0               (0x00000001u)   /* b0: USB0 single end receiver control bit */

/* Deep Standby USB Suspend/Resume Interrupt Register */
/* DPUSR1R */
#define USB_DVBINT0             (0x00800000u)   /* b23: USB0 VBUS monitor bit */
#define USB_DOVRCRB0            (0x00200000u)   /* b21: USB0 OVRCURB DM monitor bit */
#define USB_DOVRCRA0            (0x00100000u)   /* b20: USB0 OVRCURA DM monitor bit */
#define USB_DMINT0              (0x00020000u)   /* b17: USB0 DM monitor bit */
#define USB_DPINT0              (0x00010000u)   /* b16: USB0 DP monitor bit */
#define USB_DVBSE0              (0x00000080u)   /* b7: USB0 VBUS interrupt enable */
#define USB_DOVRCRBE0           (0x00000020u)   /* b5: USB0 OVRCURB interrupt enable */
#define USB_DOVRCRAE0           (0x00000010u)   /* b4: USB0 OVRCURA interrupt enable */
#define USB_DMINTE0             (0x00000002u)   /* b1: USB0 DM interrupt enable */
#define USB_DPINTE0             (0x00000001u)   /* b0: USB0 DP interrupt enable */

/**************************************************************************************************/

/********* USBHS Only *****************************************************************************/

/* Low Power Control Register */
/* LPCTRL */
#define USB_HWUPM               (0x0080u)   /* b7:  */

/* Low Power Status Register */
/* LPSTS */
#define USB_SUSPENDM            (0x4000u)   /* b14: UTMI SuspendM control */

/* PHY Single Access Read Data Register */
/* SPRDAT */
#define USB_PHYRDAT             (0x00FFu)   /* PHY read data bit */

/* Battery Charging Control Register */
/* BCCTRL */
#define USB_PDDETSTS            (0x0200u)   /* b9: PDDET status */
#define USB_CHGDETSTS           (0x0100u)   /* b8: CHGDET status */
#define USB_DCPMODE             (0x0020u)   /* b5: DCP mode  control */
#define USB_VDMSRCE             (0x0010u)   /* b4: VDMSRC control */
#define USB_IDPSINKE            (0x0008u)   /* b3: IDPSINK control */
#define USB_VDPSRCE             (0x0004u)   /* b2: VDPSRC control */
#define USB_IDMSINKE            (0x0002u)   /* b1: IDMSINK control */
#define USB_IDPSRCE             (0x0001u)   /* b0: IDPSRC control */

/* Function L1 Control Register 1 */
/* PL1CTRL1 */
#define USB_L1EXTMD             (0x4000u)   /* b14: PHY control mode */
#define USB_DVSQEX              (0x0080u)   /* b7: DVSQ extension bit */
#define USB_DVSQ                (0x0070u)   /* b6-4: Mirror of DVSQ */
#define USB_L1NEGOMD            (0x0008u)   /* b3: L1 response negotiation control */
#define USB_L1RESPMD            (0x0006u)   /* b2-1: L1 response mode */
#define USB_L1RESPEN            (0x0001u)   /* b0: L1 response enable */

/* Function L1 Control Register 2 */
/* PL1CTRL2 */
#define USB_HSRDMON             (0x0F00u)   /* b12: RWE monitor bit */
#define USB_RWEMON              (0x1000u)   /* b11-8: HIRD monitor bit */

/* Host L1 Control Register 1 */
/* HL1CTRL1 */
#define USB_L1STATUS            (0x0006u)   /* b2-1: L1 request completion state */
#define USB_L1REQ               (0x0001u)   /* b0: L1 changes request bit
 */

/* Host L1 Control Register 2 */
/* HL1CTRL2 */
#define USB_BESL                (0x8000u)   /* b15: BESL & Alternate HIRD bit */
#define USB_L1RWE               (0x1000u)   /* b12: L1 RemoteWake enable for LPM tokens */
#define USB_HSRD                (0x0F00u)   /* b11-8: HIRD bit for LPM tokens */
#define USB_L1ADDR              (0x000Fu)   /* b3-0: DeviceAddress for LPM tokens */

/* PHY Timing Register 1 */
/* PHYTRIM1 */
#define USB_IMPOFFSET           (0x7000u)   /* b14-12: Terminator adjustment */
#define USB_HSIUP               (0x0F00u)   /* b11-8: Output level setup of HS  */
#define USB_PCOMPENB            (0x0080u)   /* b7: PVDD starting detection setup */
#define USB_DFALL               (0x000Cu)   /* b3-2: FS/LS falling output waveform compensation function setting */
#define USB_DRISE               (0x0003u)   /* b1-0: FS/LS standup output waveform compensation function setting */

/* PHY Timing Register 2 */
/* PHYTRIM2 */
#define USB_DIS                 (0x7000u)   /* b14-12: Disconnect disregard level setup */
#define USB_PDR                 (0x0300u)   /* b9-8: HS output compensation function setting */
#define USB_HSRXENMODE          (0x0080u)   /* b7: HS reception permission control mode setup */
#define USB_SQU                 (0x000Fu)   /* b3-0: Squelch disregard level setup */

/* Deep Standby USB Register */
/* DPUSR0R */
#define USB_DVBSTSHM            (0x00800000u)   /* b23: VBUS input */
#define USB_DOVCBHM             (0x00200000u)   /* b21: OVRCURB input */
#define USB_DOVCAHM             (0x00100000u)   /* b20: OVRCURA input */

/* DPUSR1R */
#define USB_DVBSTSH             (0x00800000u)   /* b23: Interruption factor return display of VBUS */
#define USB_DOVCBH              (0x00200000u)   /* b21: Interruption factor return display of OVRCURB  */
#define USB_DOVCAH              (0x00100000u)   /* b20: Interruption factor return display of OVRCURA  */
#define USB_DVBSTSHE            (0x00000080u)   /* b7 : VBUS interrupt enable */
#define USB_DOVCBHE             (0x00000020u)   /* b5 : OVRCURB interrupt enable */
#define USB_DOVCAHE             (0x00000010u)   /* b4 : OVRCURA interrupt enable */

/**************************************************************************************************/

/* DMAx Pin Configuration Register */
#define USB_DREQA               (0x4000u)   /* b14: Dreq active select */
#define USB_BURST               (0x2000u)   /* b13: Burst mode */
#define USB_DACKA               (0x0400u)   /* b10: Dack active select */
#define USB_DFORM               (0x0380u)   /* b9-7: DMA mode select */
#define USB_CPU_ADR_RD_WR       (0x0000u)   /* Address + RD/WR mode (CPU bus) */
#define USB_CPU_DACK_RD_WR      (0x0100u)   /* DACK + RD/WR (CPU bus) */
#define USB_CPU_DACK_ONLY       (0x0180u)   /* DACK only (CPU bus) */
#define USB_SPLIT_DACK_ONLY     (0x0200u)   /* DACK only (SPLIT bus) */
#define USB_DENDA               (0x0040u)   /* b6: Dend active select */
#define USB_PKTM                (0x0020u)   /* b5: Packet mode */
#define USB_DENDE               (0x0010u)   /* b4: Dend enable */
#define USB_OBUS                (0x0004u)   /* b2: OUTbus mode */

/* USB IO Register Reserved bit mask */
#define INTSTS1_MASK            (0x5870u)   /* INTSTS1 Reserved bit mask */
#define BRDYSTS_MASK            (0xFFFFu)   /* BRDYSTS Reserved bit mask */
#define NRDYSTS_MASK            (0xFFFFu)   /* NRDYSTS Reserved bit mask */
#define BEMPSTS_MASK            (0xFFFFu)   /* BEMPSTS Reserved bit mask */



/* USB Request Type Register */
#define USB_BREQUEST            (0xFF00u)   /* b15-8 */

/* USB Standard request */
#define USB_GET_STATUS          (0x0000u)
#define USB_CLEAR_FEATURE       (0x0100u)
#define USB_REQRESERVED         (0x0200u)
#define USB_SET_FEATURE         (0x0300u)
#define USB_REQRESERVED1        (0x0400u)
#define USB_SET_ADDRESS         (0x0500u)
#define USB_GET_DESCRIPTOR      (0x0600u)
#define USB_SET_DESCRIPTOR      (0x0700u)
#define USB_GET_CONFIGURATION   (0x0800u)
#define USB_SET_CONFIGURATION   (0x0900u)
#define USB_GET_INTERFACE       (0x0A00u)
#define USB_SET_INTERFACE       (0x0B00u)
#define USB_SYNCH_FRAME         (0x0C00u)

/* USB_BMREQUESTTYPEDIR 0x0080u(b7) */
#define USB_HOST_TO_DEV         (0x0000u)
#define USB_DEV_TO_HOST         (0x0080u)

/* USB_BMREQUESTTYPETYPE    0x0060u(b6-5) */
#define USB_STANDARD            (0x0000u)
#define USB_CLASS               (0x0020u)
#define USB_VENDOR              (0x0040u)

/* USB_BMREQUESTTYPERECIP   0x001Fu(b4-0) */
#define USB_DEVICE              (0x0000u)
#define USB_INTERFACE           (0x0001u)
#define USB_ENDPOINT            (0x0002u)
#define USB_OTHER               (0x0003u)

#define USB_NULL                (0x0u)

#define USB_IP0                 (0)         /* USB0 module */
#define USB_IP1                 (1)         /* USB1 module */

/* USB pipe number */
#define USB_PIPE0               (0x0u)
#define USB_PIPE1               (0x1u)
#define USB_PIPE2               (0x2u)
#define USB_PIPE3               (0x3u)
#define USB_PIPE4               (0x4u)
#define USB_PIPE5               (0x5u)
#define USB_PIPE6               (0x6u)
#define USB_PIPE7               (0x7u)
#define USB_PIPE8               (0x8u)
#define USB_PIPE9               (0x9u)
#define USB_PIPE10              (0xAu)
#define USB_PIPE11              (0xBu)
#define USB_PIPE12              (0xCu)
#define USB_PIPE13              (0xDu)
#define USB_PIPE14              (0xEu)
#define USB_PIPE15              (0xFu)

#define USB_EP0                 (0x0u)
#define USB_EP1                 (0x1u)
#define USB_EP2                 (0x2u)
#define USB_EP3                 (0x3u)
#define USB_EP4                 (0x4u)
#define USB_EP5                 (0x5u)
#define USB_EP6                 (0x6u)
#define USB_EP7                 (0x7u)
#define USB_EP8                 (0x8u)
#define USB_EP9                 (0x9u)
#define USB_EP10                (0xAu)
#define USB_EP11                (0xBu)
#define USB_EP12                (0xCu)
#define USB_EP13                (0xDu)
#define USB_EP14                (0xEu)
#define USB_EP15                (0xFu)

/* Descriptor type  Define */
#define USB_DT_DEVICE           (0x01u)     /* Configuration Descriptor */
#define USB_DT_CONFIGURATION    (0x02u)     /* Configuration Descriptor */
#define USB_DT_STRING           (0x03u)     /* Configuration Descriptor */
#define USB_DT_INTERFACE        (0x04u)     /* Interface Descriptor */
#define USB_DT_ENDPOINT         (0x05u)     /* Endpoint Descriptor */
#define USB_DT_DEVICE_QUALIFIER (0x06u)     /* Device Qualifier Descriptor */
#define USB_DT_OTHER_SPEED_CONF (0x07u)     /* Other Speed Configuration Descriptor */
#define USB_DT_INTERFACE_POWER  (0x08u)     /* Interface Power Descriptor */
#define USB_DT_OTGDESCRIPTOR    (0x09u)     /* OTG Descriptor */
#define USB_DT_HUBDESCRIPTOR    (0x29u)     /* HUB descriptor */

/* Interface class Define */
#define USB_IFCLS_NOT           (0x00u)     /* Un corresponding Class */
#define USB_IFCLS_AUD           (0x01u)     /* Audio Class */
#define USB_IFCLS_CDC           (0x02u)     /* CDC Class */
#define USB_IFCLS_CDCC          (0x02u)     /* CDC-Control Class */
#define USB_IFCLS_HID           (0x03u)     /* HID Class */
#define USB_IFCLS_PHY           (0x05u)     /* Physical Class */
#define USB_IFCLS_IMG           (0x06u)     /* Image Class */
#define USB_IFCLS_PRN           (0x07u)     /* Printer Class */
#define USB_IFCLS_MAS           (0x08u)     /* Mass Storage Class */
#define USB_IFCLS_HUB           (0x09u)     /* HUB Class */
#define USB_IFCLS_CDCD          (0x0Au)     /* CDC-Data Class */
#define USB_IFCLS_CHIP          (0x0Bu)     /* Chip/Smart Card Class */
#define USB_IFCLS_CNT           (0x0Cu)     /* Content-Security Class */
#define USB_IFCLS_VID           (0x0Du)     /* Video Class */
#define USB_IFCLS_DIAG          (0xDCu)     /* Diagnostic Device */
#define USB_IFCLS_WIRE          (0xE0u)     /* Wireless Controller */
#define USB_IFCLS_APL           (0xFEu)     /* Application-Specific */
#define USB_IFCLS_VEN           (0xFFu)     /* Vendor-Specific Class */

/* Endpoint Descriptor  Define */
#define USB_EP_IN               (0x80u)     /* In Endpoint */
#define USB_EP_OUT              (0x00u)     /* Out Endpoint */
#define USB_EP_ISO              (0x01u)     /* Isochronous Transfer */
#define USB_EP_BULK             (0x02u)     /* Bulk Transfer */
#define USB_EP_INT              (0x03u)     /* Interrupt Transfer */

/* Configuration descriptor bit define */
#define USB_CF_RESERVED         (0x80u)     /* Reserved(set to 1) */
#define USB_CF_SELFP            (0x40u)     /* Self Powered */
#define USB_CF_BUSP             (0x00u)     /* Bus Powered */
#define USB_CF_RWUPON           (0x20u)     /* Remote Wake up ON */
#define USB_CF_RWUPOFF          (0x00u)     /* Remote Wake up OFF */

/* Descriptor length Define */
#define USB_DD_BLENGTH          (18u)       /* Device Descriptor Length */
#define USB_CD_BLENGTH          (9u)        /* Configuration Descriptor Length */
#define USB_ID_BLENGTH          (9u)        /* Interface Descriptor Length */
#define USB_ED_BLENGTH          (7u)        /* Endpoint Descriptor Length */

#define USB_BUFSIZE_BIT         (10u)

/* other */
#define USB_FUNCTION_CFIFO_USE  (0x0000u)
#define USB_FUNCTION_D0FIFO_USE (0x1000u)
#define USB_FUNCTION_D1FIFO_USE (0x2000u)


#endif
