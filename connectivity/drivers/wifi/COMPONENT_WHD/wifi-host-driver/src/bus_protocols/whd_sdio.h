/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INCLUDED_WHD_SDIO_H_
#define INCLUDED_WHD_SDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*             SDIO Constants
******************************************************/
/* CurrentSdiodProgGuide r23 */

/* Base registers */
#define SDIO_CORE(wd)                    ( (uint32_t)(GET_C_VAR(wd, SDIOD_CORE_BASE_ADDRESS) + 0x00) )
#define SDIO_INT_STATUS(wd)              ( (uint32_t)(GET_C_VAR(wd, SDIOD_CORE_BASE_ADDRESS) + 0x20) )
#define SDIO_TO_SB_MAILBOX(wd)           ( (uint32_t)(GET_C_VAR(wd, SDIOD_CORE_BASE_ADDRESS) + 0x40) )
#define SDIO_TO_SB_MAILBOX_DATA(wd)      ( (uint32_t)(GET_C_VAR(wd, SDIOD_CORE_BASE_ADDRESS) + 0x48) )
#define SDIO_TO_HOST_MAILBOX_DATA(wd)    ( (uint32_t)(GET_C_VAR(wd, SDIOD_CORE_BASE_ADDRESS) + 0x4C) )
#define SDIO_INT_HOST_MASK(wd)           ( (uint32_t)(GET_C_VAR(wd, SDIOD_CORE_BASE_ADDRESS) + 0x24) )
#define SDIO_FUNCTION_INT_MASK(wd)       ( (uint32_t)(GET_C_VAR(wd, SDIOD_CORE_BASE_ADDRESS) + 0x34) )

/* SDIO Function 0 (SDIO Bus) register addresses */

/* SDIO Device CCCR (Card Common Control Register) offsets */
/* CCCR accesses do not require backpane clock */
#define SDIOD_CCCR_REV             ( (uint32_t)0x00 )     /* CCCR/SDIO Revision */
#define SDIOD_CCCR_SDREV           ( (uint32_t)0x01 )     /* SD Revision */
#define SDIOD_CCCR_IOEN            ( (uint32_t)0x02 )     /* I/O Enable */
#define SDIOD_CCCR_IORDY           ( (uint32_t)0x03 )     /* I/O Ready */
#define SDIOD_CCCR_INTEN           ( (uint32_t)0x04 )     /* Interrupt Enable */
#define SDIOD_CCCR_INTPEND         ( (uint32_t)0x05 )     /* Interrupt Pending */
#define SDIOD_CCCR_IOABORT         ( (uint32_t)0x06 )     /* I/O Abort */
#define SDIOD_CCCR_BICTRL          ( (uint32_t)0x07 )     /* Bus Interface control */
#define SDIOD_CCCR_CAPABLITIES     ( (uint32_t)0x08 )     /* Card Capabilities */
#define SDIOD_CCCR_CISPTR_0        ( (uint32_t)0x09 )     /* Common CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_CISPTR_1        ( (uint32_t)0x0A )     /* Common CIS Base Address Pointer Register 1 */
#define SDIOD_CCCR_CISPTR_2        ( (uint32_t)0x0B )     /* Common CIS Base Address Pointer Register 2 (MSB - only bit 1 valid)*/
#define SDIOD_CCCR_BUSSUSP         ( (uint32_t)0x0C )     /* Bus Suspend. Valid only if SBS is set */
#define SDIOD_CCCR_FUNCSEL         ( (uint32_t)0x0D )     /* Function Select. Valid only if SBS is set */
#define SDIOD_CCCR_EXECFLAGS       ( (uint32_t)0x0E )     /* Exec Flags. Valid only if SBS is set */
#define SDIOD_CCCR_RDYFLAGS        ( (uint32_t)0x0F )     /* Ready Flags. Valid only if SBS is set */
#define SDIOD_CCCR_BLKSIZE_0       ( (uint32_t)0x10 )     /* Function 0 (Bus) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_BLKSIZE_1       ( (uint32_t)0x11 )     /* Function 0 (Bus) SDIO Block Size Register 1 (MSB) */
#define SDIOD_CCCR_POWER_CONTROL   ( (uint32_t)0x12 )     /* Power Control */
#define SDIOD_CCCR_SPEED_CONTROL   ( (uint32_t)0x13 )     /* Bus Speed Select  (control device entry into high-speed clocking mode)  */
#define SDIOD_CCCR_UHS_I           ( (uint32_t)0x14 )     /* UHS-I Support */
#define SDIOD_CCCR_DRIVE           ( (uint32_t)0x15 )     /* Drive Strength */
#define SDIOD_CCCR_INTEXT          ( (uint32_t)0x16 )     /* Interrupt Extension */
#define SDIOD_CCCR_BRCM_CARDCAP    ( (uint32_t)0xF0 )     /* Brcm Card Capability */
#define SDIOD_SEP_INT_CTL          ( (uint32_t)0xF2 )     /* Separate Interrupt Control*/
#define SDIOD_CCCR_F1INFO          ( (uint32_t)0x100 )    /* Function 1 (Backplane) Info */
#define SDIOD_CCCR_F1HP            ( (uint32_t)0x102 )    /* Function 1 (Backplane) High Power */
#define SDIOD_CCCR_F1CISPTR_0      ( (uint32_t)0x109 )    /* Function 1 (Backplane) CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_F1CISPTR_1      ( (uint32_t)0x10A )    /* Function 1 (Backplane) CIS Base Address Pointer Register 1       */
#define SDIOD_CCCR_F1CISPTR_2      ( (uint32_t)0x10B )    /* Function 1 (Backplane) CIS Base Address Pointer Register 2 (MSB - only bit 1 valid) */
#define SDIOD_CCCR_F1BLKSIZE_0     ( (uint32_t)0x110 )    /* Function 1 (Backplane) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_F1BLKSIZE_1     ( (uint32_t)0x111 )    /* Function 1 (Backplane) SDIO Block Size Register 1 (MSB) */
#define SDIOD_CCCR_F2INFO          ( (uint32_t)0x200 )    /* Function 2 (WLAN Data FIFO) Info */
#define SDIOD_CCCR_F2HP            ( (uint32_t)0x202 )    /* Function 2 (WLAN Data FIFO) High Power */
#define SDIOD_CCCR_F2CISPTR_0      ( (uint32_t)0x209 )    /* Function 2 (WLAN Data FIFO) CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_F2CISPTR_1      ( (uint32_t)0x20A )    /* Function 2 (WLAN Data FIFO) CIS Base Address Pointer Register 1       */
#define SDIOD_CCCR_F2CISPTR_2      ( (uint32_t)0x20B )    /* Function 2 (WLAN Data FIFO) CIS Base Address Pointer Register 2 (MSB - only bit 1 valid) */
#define SDIOD_CCCR_F2BLKSIZE_0     ( (uint32_t)0x210 )    /* Function 2 (WLAN Data FIFO) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_F2BLKSIZE_1     ( (uint32_t)0x211 )    /* Function 2 (WLAN Data FIFO) SDIO Block Size Register 1 (MSB) */
#define SDIOD_CCCR_F3INFO          ( (uint32_t)0x300 )    /* Function 3 (Bluetooth Data FIFO) Info */
#define SDIOD_CCCR_F3HP            ( (uint32_t)0x302 )    /* Function 3 (Bluetooth Data FIFO) High Power */
#define SDIOD_CCCR_F3CISPTR_0      ( (uint32_t)0x309 )    /* Function 3 (Bluetooth Data FIFO) CIS Base Address Pointer Register 0 (LSB) */
#define SDIOD_CCCR_F3CISPTR_1      ( (uint32_t)0x30A )    /* Function 3 (Bluetooth Data FIFO) CIS Base Address Pointer Register 1       */
#define SDIOD_CCCR_F3CISPTR_2      ( (uint32_t)0x30B )    /* Function 3 (Bluetooth Data FIFO) CIS Base Address Pointer Register 2 (MSB - only bit 1 valid) */
#define SDIOD_CCCR_F3BLKSIZE_0     ( (uint32_t)0x310 )    /* Function 3 (Bluetooth Data FIFO) SDIO Block Size Register 0 (LSB) */
#define SDIOD_CCCR_F3BLKSIZE_1     ( (uint32_t)0x311 )    /* Function 3 (Bluetooth Data FIFO) SDIO Block Size Register 1 (MSB) */


/* SDIO Function 1 (Backplane) register addresses */
/* Addresses 0x00000000 - 0x0000FFFF are directly access the backplane
 * throught the backplane window. Addresses above 0x0000FFFF are
 * registers relating to backplane access, and do not require a backpane
 * clock to access them
 */
#define SDIO_GPIO_SELECT              ( (uint32_t)0x10005 )
#define SDIO_GPIO_OUTPUT              ( (uint32_t)0x10006 )
#define SDIO_GPIO_ENABLE              ( (uint32_t)0x10007 )
#define SDIO_FUNCTION2_WATERMARK      ( (uint32_t)0x10008 )
#define SDIO_DEVICE_CONTROL           ( (uint32_t)0x10009 )
#define SDIO_BACKPLANE_ADDRESS_LOW    ( (uint32_t)0x1000A )
#define SDIO_BACKPLANE_ADDRESS_MID    ( (uint32_t)0x1000B )
#define SDIO_BACKPLANE_ADDRESS_HIGH   ( (uint32_t)0x1000C )
#define SDIO_FRAME_CONTROL            ( (uint32_t)0x1000D )
#define SDIO_CHIP_CLOCK_CSR           ( (uint32_t)0x1000E )
#define SDIO_PULL_UP                  ( (uint32_t)0x1000F )
#define SDIO_READ_FRAME_BC_LOW        ( (uint32_t)0x1001B )
#define SDIO_READ_FRAME_BC_HIGH       ( (uint32_t)0x1001C )
#define SDIO_WAKEUP_CTRL              ( (uint32_t)0x1001E )
#define SDIO_SLEEP_CSR                ( (uint32_t)0x1001F )
#define I_HMB_SW_MASK                 ( (uint32_t)0x000000F0 )
#define I_HMB_FRAME_IND               (1 << 6)
#define I_HMB_HOST_INT                (1 << 7)
#define FRAME_AVAILABLE_MASK          I_HMB_SW_MASK

/******************************************************
*             Bit Masks
******************************************************/

/* SDIOD_CCCR_REV Bits */
#define SDIO_REV_SDIOID_MASK       ( (uint32_t)0xF0 )     /* SDIO spec revision number */
#define SDIO_REV_CCCRID_MASK       ( (uint32_t)0x0F )     /* CCCR format version number */

/* SDIOD_CCCR_SDREV Bits */
#define SD_REV_PHY_MASK            ( (uint32_t)0x0F )     /* SD format version number */

/* SDIOD_CCCR_IOEN Bits */
#define SDIO_FUNC_ENABLE_1         ( (uint32_t)0x02 )     /* function 1 I/O enable */
#define SDIO_FUNC_ENABLE_2         ( (uint32_t)0x04 )     /* function 2 I/O enable */
#define SDIO_FUNC_ENABLE_3         ( (uint32_t)0x08 )     /* function 3 I/O enable */

/* SDIOD_CCCR_IORDY Bits */
#define SDIO_FUNC_READY_1          ( (uint32_t)0x02 )     /* function 1 I/O ready */
#define SDIO_FUNC_READY_2          ( (uint32_t)0x04 )     /* function 2 I/O ready */
#define SDIO_FUNC_READY_3          ( (uint32_t)0x08 )     /* function 3 I/O ready */

/* SDIOD_CCCR_INTEN Bits */
#define INTR_CTL_MASTER_EN         ( (uint32_t)0x01 )     /* interrupt enable master */
#define INTR_CTL_FUNC1_EN          ( (uint32_t)0x02 )     /* interrupt enable for function 1 */
#define INTR_CTL_FUNC2_EN          ( (uint32_t)0x04 )     /* interrupt enable for function 2 */

/* SDIOD_SEP_INT_CTL Bits */
#define SEP_INTR_CTL_MASK          ( (uint32_t)0x01 )     /* out-of-band interrupt mask */
#define SEP_INTR_CTL_EN            ( (uint32_t)0x02 )     /* out-of-band interrupt output enable */
#define SEP_INTR_CTL_POL           ( (uint32_t)0x04 )     /* out-of-band interrupt polarity */

/* SDIOD_CCCR_INTPEND Bits */
#define INTR_STATUS_FUNC1          ( (uint32_t)0x02 )     /* interrupt pending for function 1 */
#define INTR_STATUS_FUNC2          ( (uint32_t)0x04 )     /* interrupt pending for function 2 */
#define INTR_STATUS_FUNC3          ( (uint32_t)0x08 )     /* interrupt pending for function 3 */

/* SDIOD_CCCR_IOABORT Bits */
#define IO_ABORT_RESET_ALL         ( (uint32_t)0x08 )     /* I/O card reset */
#define IO_ABORT_FUNC_MASK         ( (uint32_t)0x07 )     /* abort selction: function x */

/* SDIOD_CCCR_BICTRL Bits */
#define BUS_CARD_DETECT_DIS        ( (uint32_t)0x80 )     /* Card Detect disable */
#define BUS_SPI_CONT_INTR_CAP      ( (uint32_t)0x40 )     /* support continuous SPI interrupt */
#define BUS_SPI_CONT_INTR_EN       ( (uint32_t)0x20 )     /* continuous SPI interrupt enable */
#define BUS_SD_DATA_WIDTH_MASK     ( (uint32_t)0x03 )     /* bus width mask */
#define BUS_SD_DATA_WIDTH_4BIT     ( (uint32_t)0x02 )     /* bus width 4-bit mode */
#define BUS_SD_DATA_WIDTH_1BIT     ( (uint32_t)0x00 )     /* bus width 1-bit mode */

/* SDIOD_CCCR_CAPABLITIES Bits */
#define SDIO_CAP_4BLS              ( (uint32_t)0x80 )     /* 4-bit support for low speed card */
#define SDIO_CAP_LSC               ( (uint32_t)0x40 )     /* low speed card */
#define SDIO_CAP_E4MI              ( (uint32_t)0x20 )     /* enable interrupt between block of data in 4-bit mode */
#define SDIO_CAP_S4MI              ( (uint32_t)0x10 )     /* support interrupt between block of data in 4-bit mode */
#define SDIO_CAP_SBS               ( (uint32_t)0x08 )     /* support suspend/resume */
#define SDIO_CAP_SRW               ( (uint32_t)0x04 )     /* support read wait */
#define SDIO_CAP_SMB               ( (uint32_t)0x02 )     /* support multi-block transfer */
#define SDIO_CAP_SDC               ( (uint32_t)0x01 )     /* Support Direct commands during multi-byte transfer */

/* SDIOD_CCCR_POWER_CONTROL Bits */
#define SDIO_POWER_SMPC            ( (uint32_t)0x01 )     /* supports master power control (RO) */
#define SDIO_POWER_EMPC            ( (uint32_t)0x02 )     /* enable master power control (allow > 200mA) (RW) */

/* SDIOD_CCCR_SPEED_CONTROL Bits */
#define SDIO_SPEED_SHS             ( (uint32_t)0x01 )     /* supports high-speed [clocking] mode (RO) */
#define SDIO_SPEED_EHS             ( (uint32_t)0x02 )     /* enable high-speed [clocking] mode (RW) */

/* SDIOD_CCCR_BRCM_CARDCAP Bits */
#define SDIOD_CCCR_BRCM_CARDCAP_CMD14_SUPPORT ( (uint32_t)0x02 )   /* Supports CMD14 */
#define SDIOD_CCCR_BRCM_CARDCAP_CMD14_EXT     ( (uint32_t)0x04 )   /* CMD14 is allowed in FSM command state */
#define SDIOD_CCCR_BRCM_CARDCAP_CMD_NODEC     ( (uint32_t)0x08 )   /* sdiod_aos does not decode any command */

/* SDIO_FUNCTION_INT_MASK Bits*/
#define SDIO_FUNC_MASK_F1          ( (uint32_t)0x01 )     /* interrupt mask enable for function 1 */
#define SDIO_FUNC_MASK_F2          ( (uint32_t)0x02 )     /* interrupt mask enable for function 2 */

/* SDIO_DEVICE_CONTROL Bits */
#define SDIO_DATA_PAD_ISO          ( (uint32_t)0x08 )     /* isolate internal SDIO data bus signals */

/* SDIO_CHIP_CLOCK_CSR Bits */
#define SBSDIO_FORCE_ALP           ( (uint32_t)0x01 )     /* Force ALP request to backplane */
#define SBSDIO_FORCE_HT            ( (uint32_t)0x02 )     /* Force HT request to backplane */
#define SBSDIO_FORCE_ILP           ( (uint32_t)0x04 )     /* Force ILP request to backplane */
#define SBSDIO_ALP_AVAIL_REQ       ( (uint32_t)0x08 )     /* Make ALP ready (power up xtal) */
#define SBSDIO_HT_AVAIL_REQ        ( (uint32_t)0x10 )     /* Make HT ready (power up PLL) */
#define SBSDIO_FORCE_HW_CLKREQ_OFF ( (uint32_t)0x20 )     /* Squelch clock requests from HW */
#define SBSDIO_ALP_AVAIL           ( (uint32_t)0x40 )     /* Status: ALP is ready */
#define SBSDIO_HT_AVAIL            ( (uint32_t)0x80 )     /* Status: HT is ready */
#define SBSDIO_Rev8_HT_AVAIL       ( (uint32_t)0x40 )
#define SBSDIO_Rev8_ALP_AVAIL      ( (uint32_t)0x80 )

/* SDIO_FRAME_CONTROL Bits */
#define SFC_RF_TERM                ( (uint32_t)(1 << 0) ) /* Read Frame Terminate */
#define SFC_WF_TERM                ( (uint32_t)(1 << 1) ) /* Write Frame Terminate */
#define SFC_CRC4WOOS               ( (uint32_t)(1 << 2) ) /* HW reports CRC error for write out of sync */
#define SFC_ABORTALL               ( (uint32_t)(1 << 3) ) /* Abort cancels all in-progress frames */

/* SDIO_TO_SB_MAILBOX bits corresponding to intstatus bits */
#define SMB_NAK                    ( (uint32_t)(1 << 0) ) /* To SB Mailbox Frame NAK */
#define SMB_INT_ACK                ( (uint32_t)(1 << 1) ) /* To SB Mailbox Host Interrupt ACK */
#define SMB_USE_OOB                ( (uint32_t)(1 << 2) ) /* To SB Mailbox Use OOB Wakeup */
#define SMB_DEV_INT                ( (uint32_t)(1 << 3) ) /* To SB Mailbox Miscellaneous Interrupt */

/* SDIO_WAKEUP_CTRL bits */
#define SBSDIO_WCTRL_WAKE_TILL_ALP_AVAIL     ( (uint32_t)(1 << 0) ) /* WakeTillAlpAvail bit */
#define SBSDIO_WCTRL_WAKE_TILL_HT_AVAIL      ( (uint32_t)(1 << 1) ) /* WakeTillHTAvail bit */

/* SDIO_SLEEP_CSR bits */
#define SBSDIO_SLPCSR_KEEP_SDIO_ON           ( (uint32_t)(1 << 0) ) /* KeepSdioOn bit */
#define SBSDIO_SLPCSR_DEVICE_ON              ( (uint32_t)(1 << 1) ) /* DeviceOn bit */

/* To hostmail box data */
#define I_HMB_DATA_NAKHANDLED       0x0001  /* retransmit NAK'd frame */
#define I_HMB_DATA_DEVREADY         0x0002  /* talk to host after enable */
#define I_HMB_DATA_FC               0x0004  /* per prio flowcontrol update flag */
#define I_HMB_DATA_FWREADY          0x0008  /* fw ready for protocol activity */
#define I_HMB_DATA_FWHALT           0x0010  /* firmware halted */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_SDIO_H_ */

