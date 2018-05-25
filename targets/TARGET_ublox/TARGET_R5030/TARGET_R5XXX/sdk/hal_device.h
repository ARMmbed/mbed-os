#ifndef INCLUDED_HAL_DEVICE
#define INCLUDED_HAL_DEVICE
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** HWSeq Flag bits connected to the MDM M7 CPU - start index */
#define HWSEQ_FLAGS_MDM_CPU_OFFSET 0
/** HWSeq Flag bits connected to the MDM M7 CPU - bit width */
#define HWSEQ_FLAGS_MDM_CPU_WIDTH 8
/** HWSeq Flag bits connected to the PHY M3 CPU - start index */
#define HWSEQ_FLAGS_PHY_CPU_OFFSET 8
/** HWSeq Flag bits connected to the PHY M3 CPU - bit width */
#define HWSEQ_FLAGS_PHY_CPU_WIDTH 8
/** HWSeq Flag bits connected to the VSP DE0 - start index */
#define HWSEQ_FLAGS_VSP_DE0_OFFSET 24
/** HWSeq Flag bits connected to the VSP DE0 - bit width */
#define HWSEQ_FLAGS_VSP_DE0_WIDTH 8
/** HWSeq Flag bits connected to the VSP DE1 - start index */
#define HWSEQ_FLAGS_VSP_DE1_OFFSET 16
/** HWSeq Flag bits connected to the VSP DE1 - bit width */
#define HWSEQ_FLAGS_VSP_DE1_WIDTH 8
/** Modem CPU Type */
#define APPS_CPU CORTEX_M7
/** Number of receive channels per RF datapath */
#define HAL_NR_RX 1
/** Number of SDMA channels */
#define HAL_NR_SDMA_CHANNELS 8
/** Number of System Memory Banks */
#define HAL_NR_SYSMEM_BANKS 0
/** Number of transmit channels per RF datapath */
#define HAL_NR_TX 1
/** Number of VDMA channels per VDMA engine */
#define HAL_NR_VDMA_CHANNELS 1
/** Number of VSP Subsystems available in the platform */
#define HAL_NR_VSP 1
/** Number of DSP cores available in VSP Subsystem 0 */
#define HAL_NR_VSP0_AR1DE 1
/** Continuation Code used for the Klein Matterhorn Chip (0x7 -> u-blox, 0x4 -> ARM) */
#define KM_CONTINUATION_CODE 0x7
/** Identification Code used for the Klein Matterhorn Chip (0x7 -> u-blox, 0x4 -> ARM) */
#define KM_IDENTIFICATION_CODE 0x0D
/** Part number used for the Klein Matterhorn Chip */
#define KM_PART_NUMBER 0xAA6
/** Target ID for the Klein Matterhorn Chip (field descriptions covered in ARM Coresight SoC TRM) */
#define KM_TARGETID (KM_PART_NUMBER << 16) | (KM_CONTINUATION_CODE << 8) | (KM_IDENTIFICATION_CODE << 1) | 0x1
/** Modem CPU Type */
#define MDM_CPU CORTEX_M7
/** Modem CPU Type */
#define PHY_CPU CORTEX_M3
/** Modem CPU Type */
#define RF_CPU CORTEX_M3
/** Number of RF System Memory RAM Banks */
#define HAL_RF_NR_SYSRAM_BANKS 2
/** RF Transceiver CPU Type */
#define RF_CPU CORTEX_M3

/** APP CPU IRQ Connectivity Configuration */
enum APP_SS_IRQ_CFG {
   /** App M7 Int CTI 0 */
   APP_SS_IRQ_CFG_APP_IRQ_CTIIRQ0_INT = 0,
   /** App M7 Int CTI 1 */
   APP_SS_IRQ_CFG_APP_IRQ_CTIIRQ1_INT = 1,
   /** App M7 Int I2C 1 */
   APP_SS_IRQ_CFG_APP_IRQ_I2C1_INT = 2,
   /** App M7 Int I2C 2 */
   APP_SS_IRQ_CFG_APP_IRQ_I2C2_INT = 3,
   /** App M7 Int I2C 3 */
   APP_SS_IRQ_CFG_APP_IRQ_I2C3_INT = 4,
   /** App M7 Int I2S */
   APP_SS_IRQ_CFG_APP_IRQ_I2S_INT = 5,
   /** App M7 Int UART 1 */
   APP_SS_IRQ_CFG_APP_IRQ_UART1_INT = 6,
   /** App M7 Int UART 2 */
   APP_SS_IRQ_CFG_APP_IRQ_UART2_INT = 7,
   /** App M7 Int SPI 1 */
   APP_SS_IRQ_CFG_APP_IRQ_SPI1_INT = 8,
   /** App M7 Int SPI 2 */
   APP_SS_IRQ_CFG_APP_IRQ_SPI2_INT = 9,
   /** App M7 Int TIMER 1 */
   APP_SS_IRQ_CFG_APP_IRQ_TIMER1_INT = 10,
   /** App M7 Int TIMER 2 */
   APP_SS_IRQ_CFG_APP_IRQ_TIMER2_INT = 11,
   /** App M7 Int WatchDog MDM - also connects to MDM M7 NMI */
   APP_SS_IRQ_CFG_APP_IRQ_MDM_WDG_INT = 12,
   /** App M7 Int WatchDog PHY - also connects to PHY M3 NMI */
   APP_SS_IRQ_CFG_APP_IRQ_PHY_WDG_INT = 13,
   /** App M7 Int WatchDog RF - also connects to RF M3 NMI - located in km_rf_dig_ss */
   APP_SS_IRQ_CFG_APP_IRQ_RF_WDG_INT = 14,
   /** App M7 Int SysCtrl Error */
   APP_SS_IRQ_CFG_APP_IRQ_SYSCTRL_ERR_INT = 15,
   /** App M7 Int SysCtrl Interrupt */
   APP_SS_IRQ_CFG_APP_IRQ_SYSCTRL_INT = 16,
   /** App M7 Int DMAC Error */
   APP_SS_IRQ_CFG_APP_IRQ_DMAC_ERR_INT = 17,
   /** App M7 Int DMAC TC */
   APP_SS_IRQ_CFG_APP_IRQ_DMAC_TC_INT = 18,
   /** App M7 Int DMAC Comb */
   APP_SS_IRQ_CFG_APP_IRQ_DMAC_COMB_INT = 19,
   /** App M7 Int Sdio Interrupt */
   APP_SS_IRQ_CFG_APP_IRQ_SDIO_INT = 20,
   /** App M7 Int Ospi Error */
   APP_SS_IRQ_CFG_APP_IRQ_OSPI_INT = 21,
   /** App M7 Int Psram TC */
   APP_SS_IRQ_CFG_APP_IRQ_PSRAM_INT = 22,
   /** App M7 Int ARM Crypto Comb */
   APP_SS_IRQ_CFG_APP_IRQ_CRYPTO_INT = 23,
   /** App M7 Int DVS GLB */
   APP_SS_IRQ_CFG_APP_IRQ_DVS_GLB_INT = 24,
   /** App M7 Int DVS Core */
   APP_SS_IRQ_CFG_APP_IRQ_DVS_CORE_INT = 25,
   /** App M7 Int USB */
   APP_SS_IRQ_CFG_APP_IRQ_USB_INT = 26,
   /** App M7 Int USB DMA */
   APP_SS_IRQ_CFG_APP_IRQ_USB_DMA_INT = 27,
   /** App M7 Int PIO 1 */
   APP_SS_IRQ_CFG_APP_IRQ_PIO_1_INT = 28,
   /** App M7 Int PIO 2 */
   APP_SS_IRQ_CFG_APP_IRQ_PIO_2_INT = 29,
   /** App M7 Int PIO Wakeup */
   APP_SS_IRQ_CFG_APP_IRQ_PIO_WAKEUP_INT = 30,
   /** App M7 Int Power controller Error */
   APP_SS_IRQ_CFG_APP_IRQ_PWRCTRL_ERR_INT = 31,
   /** App M7 Int Power controller */
   APP_SS_IRQ_CFG_APP_IRQ_PWRCTRL_INT = 32,
   /** App M7 Int Baseband Mailbox TX 0 */
   APP_SS_IRQ_CFG_APP_IRQ_BB_IPC_MBX_TX0 = 33,
   /** App M7 Int Baseband Mailbox TX 1 */
   APP_SS_IRQ_CFG_APP_IRQ_BB_IPC_MBX_TX1 = 34,
   /** App M7 Int Baseband Mailbox TX 2 */
   APP_SS_IRQ_CFG_APP_IRQ_BB_IPC_MBX_TX2 = 35,
   /** App M7 Int Baseband Mailbox RX 0 */
   APP_SS_IRQ_CFG_APP_IRQ_BB_IPC_MBX_RX0 = 36,
   /** App M7 Int Baseband Mailbox RX 1 */
   APP_SS_IRQ_CFG_APP_IRQ_BB_IPC_MBX_RX1 = 37,
   /** App M7 Int Baseband Mailbox RX 2 */
   APP_SS_IRQ_CFG_APP_IRQ_BB_IPC_MBX_RX2 = 38,
   /** App M7 Int GNSS and App Timing */
   APP_SS_IRQ_CFG_APP_IRQ_GNSS_TIM = 39,
   /** App M7 Int OTP Fault */
   APP_SS_IRQ_CFG_APP_IRQ_OTP_FAULT = 40,
   /** App M7 Int Onfi Pad Calibration */
   APP_SS_IRQ_CFG_APP_IRQ_ONFI_CAL = 41,
   /** App M7 Int Power controller wakeup */
   APP_SS_IRQ_CFG_APP_IRQ_PWRCTRL_WAKEUP = 42,
   /** App M7 Int ETR Limiter Threshold */
   APP_SS_IRQ_CFG_APP_IRQ_ETR_LIMITER_THRESHOLD = 43,
   /** App M7 Int USB core wakeup request */
   APP_SS_IRQ_CFG_APP_IRQ_USB_WAKEUP_INT = 44,
   /** App M7 Int Analog Companion Power Alert */
   APP_SS_IRQ_CFG_APP_IRQ_AC_PWR_ALERT = 45
};

/** APP CPU Non-Maskable IRQ Connectivity Configuration */
enum APP_SS_NMI_CFG {
   /** App Watchdog Interrupt - Non-maskable Interrupt Source */
   APP_SS_NMI_CFG_APP_WDG_INT = 2
};

/** Memory Instance Numbers - to be used for functional MBIST */
enum KM_MEM_INSTANCES {
   /** Memory Instance debug etr */
   KM_MEM_INSTANCES_MEM_ID_DEBUG_ETR = 0,
   /** Memory Instance bootrom */
   KM_MEM_INSTANCES_MEM_ID_BOOTROM = 1,
   /** Memory Instance app m7 icache data */
   KM_MEM_INSTANCES_MEM_ID_APP_M7_ICACHE_DATA = 2,
   /** Memory Instance app m7 icache tag */
   KM_MEM_INSTANCES_MEM_ID_APP_M7_ICACHE_TAG = 3,
   /** Memory Instance app m7 dcache data */
   KM_MEM_INSTANCES_MEM_ID_APP_M7_DCACHE_DATA = 4,
   /** Memory Instance app m7 dcache tag */
   KM_MEM_INSTANCES_MEM_ID_APP_M7_DCACHE_TAG = 5,
   /** Memory Instance app m7 itcm */
   KM_MEM_INSTANCES_MEM_ID_APP_M7_ITCM = 6,
   /** Memory Instance app m7 dtcm */
   KM_MEM_INSTANCES_MEM_ID_APP_M7_DTCM = 7,
   /** Memory Instance app periph */
   KM_MEM_INSTANCES_MEM_ID_APP_PERIPH = 8,
   /** Memory Instance security */
   KM_MEM_INSTANCES_MEM_ID_SECURITY = 9,
   /** Memory Instance sdio */
   KM_MEM_INSTANCES_MEM_ID_SDIO = 10,
   /** Memory Instance ospi */
   KM_MEM_INSTANCES_MEM_ID_OSPI = 11,
   /** Memory Instance crypto pka and rng */
   KM_MEM_INSTANCES_MEM_ID_CRYPTO_PKA_RNG = 12,
   /** Memory Instance dlbpe a */
   KM_MEM_INSTANCES_MEM_ID_DLBPE_A = 13,
   /** Memory Instance dlbpe b */
   KM_MEM_INSTANCES_MEM_ID_DLBPE_B = 14,
   /** Memory Instance dlbpe c */
   KM_MEM_INSTANCES_MEM_ID_DLBPE_C = 15,
   /** Memory Instance dlbpe d */
   KM_MEM_INSTANCES_MEM_ID_DLBPE_D = 16,
   /** Memory Instance dlbpe hc */
   KM_MEM_INSTANCES_MEM_ID_DLBPE_HC = 17,
   /** Memory Instance ulbpe */
   KM_MEM_INSTANCES_MEM_ID_ULBPE = 18,
   /** Memory Instance ofdm demux */
   KM_MEM_INSTANCES_MEM_ID_OFDM_DEMUX = 19,
   /** Memory Instance circ buffer */
   KM_MEM_INSTANCES_MEM_ID_CIRC_BUFFER = 20,
   /** Memory Instance pss */
   KM_MEM_INSTANCES_MEM_ID_PSS = 21,
   /** Memory Instance pss scratch */
   KM_MEM_INSTANCES_MEM_ID_PSS_SCRATCH = 22,
   /** Memory Instance sss */
   KM_MEM_INSTANCES_MEM_ID_SSS = 23,
   /** Memory Instance sss fft */
   KM_MEM_INSTANCES_MEM_ID_SSS_FFT = 24,
   /** Memory Instance mdm m7 icache data */
   KM_MEM_INSTANCES_MEM_ID_MDM_M7_ICACHE_DATA = 25,
   /** Memory Instance mdm m7 icache tag */
   KM_MEM_INSTANCES_MEM_ID_MDM_M7_ICACHE_TAG = 26,
   /** Memory Instance mdm m7 dcache data */
   KM_MEM_INSTANCES_MEM_ID_MDM_M7_DCACHE_DATA = 27,
   /** Memory Instance mdm m7 dcache tag */
   KM_MEM_INSTANCES_MEM_ID_MDM_M7_DCACHE_TAG = 28,
   /** Memory Instance mdm m7 itcm */
   KM_MEM_INSTANCES_MEM_ID_MDM_M7_ITCM = 29,
   /** Memory Instance mdm m7 dtcm */
   KM_MEM_INSTANCES_MEM_ID_MDM_M7_DTCM = 30,
   /** Memory Instance vsp itcm */
   KM_MEM_INSTANCES_MEM_ID_VSP_ITCM = 31,
   /** Memory Instance vsp dtcm */
   KM_MEM_INSTANCES_MEM_ID_VSP_DTCM = 32,
   /** Memory Instance vsp dmem0 */
   KM_MEM_INSTANCES_MEM_ID_VSP_DMEM0 = 33,
   /** Memory Instance vsp dmem1 */
   KM_MEM_INSTANCES_MEM_ID_VSP_DMEM1 = 34,
   /** Memory Instance vsp dmem2 */
   KM_MEM_INSTANCES_MEM_ID_VSP_DMEM2 = 35,
   /** Memory Instance vsp dmem3 */
   KM_MEM_INSTANCES_MEM_ID_VSP_DMEM3 = 36,
   /** Memory Instance phy m3 tcm */
   KM_MEM_INSTANCES_MEM_ID_PHY_M3_TCM = 37,
   /** Memory Instance rfss m3 tcm */
   KM_MEM_INSTANCES_MEM_ID_RFSS_M3_TCM = 38,
   /** Memory Instance rfss sysram */
   KM_MEM_INSTANCES_MEM_ID_RFSS_SYSRAM = 39,
   /** Memory Instance rfss rxscratch */
   KM_MEM_INSTANCES_MEM_ID_RFSS_RXSCRATCH = 40,
   /** Memory Instance rfss txscratch */
   KM_MEM_INSTANCES_MEM_ID_RFSS_TXSCRATCH = 41,
   /** Memory Instance usb buf */
   KM_MEM_INSTANCES_MEM_ID_USB_BUF = 42,
   /** Memory Instance usb dma */
   KM_MEM_INSTANCES_MEM_ID_USB_DMA = 43
};

/** OSPI ONFI Pad General Purpose Control Register Bit Assignments */
enum OSPI_ONFI_GP_PAD_CONFIG {
   /** OSPI ONFI Pad Control - Data Pad LNB Pull Up Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_LNB_D_PU = 0,
   /** OSPI ONFI Pad Control - Data Pad LNB Pull Down Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_LNB_D_PD = 1,
   /** OSPI ONFI Pad Control - Data Pad UNB Pull Up Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_UNB_D_PU = 2,
   /** OSPI ONFI Pad Control - Data Pad UNB Pull Down Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_UNB_D_PD = 3,
   /** OSPI ONFI Pad Control - Data Pad Bus Keeper Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_D_BK = 4,
   /** OSPI ONFI Pad Control - Data Pad ODT Select */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_D_ODT_SEL = 5,
   /** OSPI ONFI Pad Control - Data Pad DQS Pull Up Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_DQS_PU = 6,
   /** OSPI ONFI Pad Control - Data Pad DQS Down Up Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_DQS_PD = 7,
   /** OSPI ONFI Pad Control - Ctrl Pad Pull Up Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CTRL_PU = 8,
   /** OSPI ONFI Pad Control - Ctrl Pad Pull Down Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CTRL_PD = 9,
   /** OSPI ONFI Pad Control - Ctrl Pad Drive Strength */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CTRL_DRV0 = 10,
   /** OSPI ONFI Pad Control - Ctrl Pad Drive Strength */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CTRL_DRV1 = 11,
   /** OSPI ONFI Pad Control - Ctrl Pad Drive Strength */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CTRL_DRV2 = 12,
   /** OSPI ONFI Pad Control - Ctrl Pad Bus Keeper */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CTRL_BK = 13,
   /** OSPI ONFI Pad Control - Ctrl Pad ODT Select */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CTRL_ODT_SEL = 14,
   /** OSPI ONFI Pad Control - Clock Pad Pull Up Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CLK_PU = 15,
   /** OSPI ONFI Pad Control - Clock Pad Pull Down Enable */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CLK_PD = 16,
   /** OSPI ONFI Pad Control - Clock Pad Drive Strength */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CLK_DRV0 = 17,
   /** OSPI ONFI Pad Control - Clock Pad Drive Strength */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CLK_DRV1 = 18,
   /** OSPI ONFI Pad Control - Clock Pad Drive Strength */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CLK_DRV2 = 19,
   /** OSPI ONFI Pad Control - Clock Pad Bus Keeper */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CLK_BK = 20,
   /** OSPI ONFI Pad Control - Clock Pad ODT Select */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CLK_ODT_SEL = 21,
   /** OSPI ONFI Pad Control - Pad Power Down */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_PWRDN = 22,
   /** OSPI ONFI Pad Control - Cal Power Down */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CAL_PWRDN = 23,
   /** OSPI ONFI Pad Control - VREF Select */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_VREF_SEL = 24,
   /** OSPI Write Protect Pad Value */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_WP_VAL = 25,
   /** OSPI Reset Override */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_OVERRIDE_RESET = 26,
   /** OSPI Reset Override Value */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_RESET_VAL = 27,
   /** OSPI CS Override */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_OVERRIDE_CS = 28,
   /** OSPI CS Override Value */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_OSPI_CS_VAL = 29,
   /** OSPI Pad SEL18 */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_SEL18 = 30,
   /** OSPI Unused */
   OSPI_ONFI_GP_PAD_CONFIG_ONFI_GP_CONFIG_ID_UNUSED1 = 31
};

/** PIO General Purpose Control Register Bit Assignments */
enum PIO_GP_CONFIG {
   /** PIO GP Assignment - SEL18_DISP */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_SEL18_DISP = 0,
   /** PIO GP Assignment - UNUSED */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_UNUSED = 1,
   /** PIO GP Assignment - SEL18_USIM */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_SEL18_USIM = 2,
   /** PIO GP Assignment - SEL18_SDIO */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_SEL18_SDIO = 3,
   /** PIO GP Assignment - SGP_USIM */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_SGP_USIM = 4,
   /** PIO GP Assignment - SD_WP_POL */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_SD_WP_POL = 5,
   /** PIO GP Assignment - SD_CDETECT_POL */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_SD_CDETECT_POL = 6,
   /** PIO GP Assignment - USIM_DET_POL */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_USIM_DET_POL = 7,
   /** PIO GP Assignment - USIM_DET_DEFAULT */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_USIM_DET_DEFAULT = 8,
   /** PIO GP Assignment - USIM_DET_SRC */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_USIM_DET_SRC = 9,
   /** PIO GP Assignment - PWRSCL_PU */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSCL_PU = 10,
   /** PIO GP Assignment - PWRSCL_MDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSCL_MDRV = 11,
   /** PIO GP Assignment - PWRSCL_LDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSCL_LDRV = 12,
   /** PIO GP Assignment - PWRSCL_SR */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSCL_SR = 13,
   /** PIO GP Assignment - PWRSCL_ST */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSCL_ST = 14,
   /** PIO GP Assignment - PWRSDA_PU */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSDA_PU = 15,
   /** PIO GP Assignment - PWRSDA_MDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSDA_MDRV = 16,
   /** PIO GP Assignment - PWRSDA_LDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRSDA_LDRV = 17,
   /** PIO GP Assignment - LVDSCONTROL_MDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_LVDSCONTROL_MDRV = 18,
   /** PIO GP Assignment - LVDSCONTROL_LDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_LVDSCONTROL_LDRV = 19,
   /** PIO GP Assignment - LVDSCONTROL_SR */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_LVDSCONTROL_SR = 20,
   /** PIO GP Assignment - REFCLKOUT_MDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_REFCLKOUT_MDRV = 21,
   /** PIO GP Assignment - REFCLKOUT_LDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_REFCLKOUT_LDRV = 22,
   /** PIO GP Assignment - REFCLKOUT_SR */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_REFCLKOUT_SR = 23,
   /** PIO GP Assignment - PWRIDLE_MDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRIDLE_MDRV = 24,
   /** PIO GP Assignment - PWRIDLE_LDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRIDLE_LDRV = 25,
   /** PIO GP Assignment - PWRIDLE_SR */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_PWRIDLE_SR = 26,
   /** PIO GP Assignment - TMS_SRATE */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_TMS_SRATE = 27,
   /** PIO GP Assignment - TMS_MDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_TMS_MDRV = 28,
   /** PIO GP Assignment - TMS_LDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_TMS_LDRV = 29,
   /** PIO GP Assignment - TDO_MDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_TDO_MDRV = 30,
   /** PIO GP Assignment - TDO_LDRV */
   PIO_GP_CONFIG_PIO_GP_CONFIG_ID_TDO_LDRV = 31
};

/** PSRAM ONFI Pad General Purpose Control Register Bit Assignments */
enum PSRAM_ONFI_GP_PAD_CONFIG {
   /** PSRAM ONFI Pad Control - Address/Data Pad Pull Up Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_AD_PU = 0,
   /** PSRAM ONFI Pad Control - Address/Data Pad Pull Down Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_AD_PD = 1,
   /** PSRAM ONFI Pad Control - Address/Data Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_AD_DRV0 = 2,
   /** PSRAM ONFI Pad Control - Address/Data Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_AD_DRV1 = 3,
   /** PSRAM ONFI Pad Control - Address/Data Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_AD_DRV2 = 4,
   /** PSRAM ONFI Pad Control - Address/Data Pad Bus Keeper Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_AD_BK = 5,
   /** PSRAM ONFI Pad Control - Address/Data Pad ODT Select */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_AD_ODT_SEL = 6,
   /** PSRAM ONFI Pad Control - Ctrl Pad Pull Up Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CTRL_PU = 7,
   /** PSRAM ONFI Pad Control - Ctrl Pad Pull Down Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CTRL_PD = 8,
   /** PSRAM ONFI Pad Control - Ctrl Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CTRL_DRV0 = 9,
   /** PSRAM ONFI Pad Control - Ctrl Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CTRL_DRV1 = 10,
   /** PSRAM ONFI Pad Control - Ctrl Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CTRL_DRV2 = 11,
   /** PSRAM ONFI Pad Control - Ctrl Pad Bus Keeper Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CTRL_BK = 12,
   /** PSRAM ONFI Pad Control - Ctrl Pad ODT Select */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CTRL_ODT_SE = 13,
   /** PSRAM ONFI Pad Control - Clock Pad Pull Up Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CLK_PU = 14,
   /** PSRAM ONFI Pad Control - Clock Pad Pull Down Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CLK_PD = 15,
   /** PSRAM ONFI Pad Control - Clock Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CLK_DRV0 = 16,
   /** PSRAM ONFI Pad Control - Clock Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CLK_DRV1 = 17,
   /** PSRAM ONFI Pad Control - Clock Pad Drive Strength */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CLK_DRV2 = 18,
   /** PSRAM ONFI Pad Control - Clock Pad Bus Keeper Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CLK_BK = 19,
   /** PSRAM ONFI Pad Control - Clock Pad ODT Select */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CLK_ODT_SEL = 20,
   /** PSRAM ONFI Pad Control - Pad Power Down */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_PWRDN = 21,
   /** PSRAM ONFI Pad Control - Cal Power Down */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CAL_PWRDN = 22,
   /** PSRAM ONFI Pad Control - VREF Select */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_VREF_SEL = 23,
   /** PSRAM ONFI Pad Control - Wait Pad Pull Up Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_WAIT_PU = 24,
   /** PSRAM ONFI Pad Control - Wait Pad Pull Down Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_WAIT_PD = 25,
   /** PSRAM CS Override */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_OVERRIDE_CS = 26,
   /** PSRAM CS Override Value */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_ID_PSRAM_CS_VAL = 27,
   /** PSRAM ONFI Pad Control - DQS Pad Pull Up Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_PSRAM_DQS_PU = 28,
   /** PSRAM ONFI Pad Control - DQS Pad Pull Down Enable */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_PSRAM_DQS_PD = 29,
   /** PSRAM Unused */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_UNUSED2 = 30,
   /** PSRAM Unused */
   PSRAM_ONFI_GP_PAD_CONFIG_PSRAM_GP_CONFIG_UNUSED3 = 31
};

/** SDMA Handshake Configuration */
enum SDMA_HS_CFG {
   /** SDMA handshake for I2S TX */
   SDMA_HS_CFG_I2S_TX = 0,
   /** SDMA handshake for I2S RX */
   SDMA_HS_CFG_I2S_RX = 1,
   /** SDMA handshake for OSPI Read */
   SDMA_HS_CFG_OSPI_RD = 2,
   /** SDMA handshake for OSPI Write */
   SDMA_HS_CFG_OSPI_WR = 3,
   /** SDMA handshake for TCU0 */
   SDMA_HS_CFG_TCU0 = 4,
   /** SDMA handshake for TCU1 */
   SDMA_HS_CFG_TCU1 = 5,
   /** SDMA handshake for STM */
   SDMA_HS_CFG_STM = 6,
   /** SDMA handshake for Display interface */
   SDMA_HS_CFG_DISP = 7
};

/** STM HWEVENTS Configuration */
enum STM_CFG {
   /** RFDMA TX0 Start. */
   STM_CFG_RFDMA_TX0_START = 0,
   /** RFDMA TX0 End. */
   STM_CFG_RFDMA_TX0_END = 1,
   /** RFDMA RX0 Start. */
   STM_CFG_RFDMA_RX0_START = 2,
   /** RFDMA RX0 End. */
   STM_CFG_RFDMA_RX0_END = 3,
   /** Uplink BPE Start. */
   STM_CFG_ULBPE_START = 4,
   /** Uplink BPE End. */
   STM_CFG_ULBPE_END = 5,
   /** Ofdm Demux End. */
   STM_CFG_OFDMDEMUX0_END = 6,
   /** Ofdm Demux Start. */
   STM_CFG_OFDMDEMUX0_START = 7,
   /** Ofdm Demux End. */
   STM_CFG_OFDMDEMUX1_END = 8,
   /** Ofdm Demux Start. */
   STM_CFG_OFDMDEMUX1_START = 9,
   /** TCU0 FEVENT */
   STM_CFG_TCU0_FEVENT = 10,
   /** TCU1 FEVENT */
   STM_CFG_TCU1_FEVENT = 11,
   /** VSP0 VDMA Queue0 Start. */
   STM_CFG_VSP0_VDMAC_QUEUE0_START = 12,
   /** VSP0 VDMA Queue0 End. */
   STM_CFG_VSP0_VDMAC_QUEUE0_END = 13,
   /** VSP0 VDMA Queue1 Start. */
   STM_CFG_VSP0_VDMAC_QUEUE1_START = 14,
   /** VSP0 VDMA Queue1 End. */
   STM_CFG_VSP0_VDMAC_QUEUE1_END = 15,
   /** VSP0 DE0 Task Start. */
   STM_CFG_VSP0_DE0_TASK_START = 16,
   /** VSP0 DE0 Task End. */
   STM_CFG_VSP0_DE0_TASK_END = 17,
   /** VSP0 DE1 Task Start. */
   STM_CFG_VSP0_DE1_TASK_START = 18,
   /** VSP0 DE1 Task End. */
   STM_CFG_VSP0_DE1_TASK_END = 19,
   /** Downlink BPE Start */
   STM_CFG_DLBPE_START = 20,
   /** Downlink BPE End */
   STM_CFG_DLBPE_END = 21,
   /** FBUF Start */
   STM_CFG_FBUF_START = 22,
   /** PSS Start */
   STM_CFG_PSS_START = 23,
   /** PSS End */
   STM_CFG_PSS_END = 24,
   /** SSS Start */
   STM_CFG_SSS_START = 25,
   /** SSS End */
   STM_CFG_SSS_END = 26
};

/** TCU0 FEVENT Configuration */
enum TCU0_FEVENT_CFG {
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_0,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_0,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_0 */
   TCU0_FEVENT_CFG_MDM_0 = 0,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_1,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_1,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_1 */
   TCU0_FEVENT_CFG_MDM_1 = 1,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_2,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_2,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_2 */
   TCU0_FEVENT_CFG_MDM_2 = 2,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_3,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_3,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_3 */
   TCU0_FEVENT_CFG_MDM_3 = 3,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_4,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_4,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_4 */
   TCU0_FEVENT_CFG_MDM_4 = 4,
   /** Connects to STM_CFG_TCU0_FEVENT */
   TCU0_FEVENT_CFG_STM = 5,
   /**  */
   TCU0_FEVENT_CFG_PHY_SAMPLE_RATE = 6,
   /** TCU0_FEVENT[7] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_7 = 7,
   /** TCU0_FEVENT[8] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_8 = 8,
   /** TCU0_FEVENT[9] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_9 = 9,
   /** TCU0_FEVENT[10] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_10 = 10,
   /** TCU0_FEVENT[11] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_11 = 11,
   /** TCU0_FEVENT[12] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_12 = 12,
   /** Connects to RF TAS[0] */
   TCU0_FEVENT_CFG_RF_TAS_0 = 13,
   /** Connects to RF TAS[1] */
   TCU0_FEVENT_CFG_RF_TAS_1 = 14,
   /** Connects to GNSS Timing Unit */
   TCU0_FEVENT_CFG_GNSS_TIM_0 = 15,
   /** TCU0_FEVENT[16] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_16 = 16,
   /** TCU0_FEVENT[17] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_17 = 17,
   /** TCU0_FEVENT[18] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_18 = 18,
   /** TCU0_FEVENT[19] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_19 = 19,
   /** TCU0_FEVENT[20] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_20 = 20,
   /** TCU0_FEVENT[21] Output left Open - Not Connected Inside the System. */
   TCU0_FEVENT_CFG_UNCONNECTED_21 = 21,
   /** SCU sleep end */
   TCU0_FEVENT_CFG_SCU_SLEEP_END = 22,
   /** SCU calibrate end */
   TCU0_FEVENT_CFG_SCU_CALIBRATE_END = 23
};

/** TCU1 FEVENT Configuration */
enum TCU1_FEVENT_CFG {
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_0,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_0,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_0 */
   TCU1_FEVENT_CFG_MDM_0 = 0,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_1,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_1,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_1 */
   TCU1_FEVENT_CFG_MDM_1 = 1,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_2,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_2,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_2 */
   TCU1_FEVENT_CFG_MDM_2 = 2,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_3,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_3,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_3 */
   TCU1_FEVENT_CFG_MDM_3 = 3,
   /** Connects to HW_SEQ_CFG_TCU_TCU0_FEVENT_4,MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_4,PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_4 */
   TCU1_FEVENT_CFG_MDM_4 = 4,
   /** Connects to STM_CFG_TCU1_FEVENT */
   TCU1_FEVENT_CFG_STM = 5,
   /** TCU1_FEVENT[6] Output left Open - Not Connected Inside the System. */
   TCU1_FEVENT_CFG_UNCONNECTED_6 = 6,
   /** TCU1_FEVENT[7] Output left Open - Not Connected Inside the System. */
   TCU1_FEVENT_CFG_UNCONNECTED_7 = 7,
   /** TCU1_FEVENT[8] Output left Open - Not Connected Inside the System. */
   TCU1_FEVENT_CFG_UNCONNECTED_8 = 8,
   /** TCU1_FEVENT[9] Output left Open - Not Connected Inside the System. */
   TCU1_FEVENT_CFG_UNCONNECTED_9 = 9,
   /** TCU1_FEVENT[10] Output left Open - Not Connected Inside the System. */
   TCU1_FEVENT_CFG_UNCONNECTED_10 = 10,
   /** TCU1_FEVENT[11] Output left Open - Not Connected Inside the System. */
   TCU1_FEVENT_CFG_UNCONNECTED_11 = 11,
   /** TCU1_FEVENT[12] Output left Open - Not Connected Inside the System. */
   TCU1_FEVENT_CFG_UNCONNECTED_12 = 12,
   /** Connects to RF TAS[0] */
   TCU1_FEVENT_CFG_RF_TAS_2 = 13,
   /** Connects to RF TAS[1] */
   TCU1_FEVENT_CFG_RF_TAS_3 = 14,
   /** Connects to GNSS Timing Unit */
   TCU1_FEVENT_CFG_GNSS_TIM_1 = 15
};

/** Test Data Register IR Decode Values - Apps TDR Instance */
enum TEST_DATA_REGS_APPS_IR_VALUES {
   /** TDR IR Value for access to the SCAN_LBIST registers */
   TEST_DATA_REGS_APPS_IR_VALUES_TDR_APPS_IR_SCAN_LBIST = 10,
   /** TDR IR Value for access to the CLK registers */
   TEST_DATA_REGS_APPS_IR_VALUES_TDR_APPS_IR_CLK = 14,
   /** TDR IR Value for access to the RF_DBG registers */
   TEST_DATA_REGS_APPS_IR_VALUES_TDR_APPS_IR_RF_DBG = 19
};

/** Test Data Register IR Decode Values - Core TDR Instance */
enum TEST_DATA_REGS_CORE_IR_VALUES {
   /** TDR IR Value for access to the PIOPAD registers */
   TEST_DATA_REGS_CORE_IR_VALUES_TDR_CORE_IR_PIOPAD = 3,
   /** TDR IR Value for access to the PWR registers */
   TEST_DATA_REGS_CORE_IR_VALUES_TDR_CORE_IR_PWR = 4,
   /** TDR IR Value for access to the RST registers */
   TEST_DATA_REGS_CORE_IR_VALUES_TDR_CORE_IR_RST = 5,
   /** TDR IR Value for access to the CLK_AO registers */
   TEST_DATA_REGS_CORE_IR_VALUES_TDR_CORE_IR_CLK_AO = 6,
   /** TDR IR Value for access to the FASTPAD registers */
   TEST_DATA_REGS_CORE_IR_VALUES_TDR_CORE_IR_FASTPAD = 16,
   /** TDR IR Value for access to the ENABLES registers */
   TEST_DATA_REGS_CORE_IR_VALUES_TDR_CORE_IR_ENABLES = 17,
   /** TDR IR Value for access to the FASTPAD_CALIB registers */
   TEST_DATA_REGS_CORE_IR_VALUES_TDR_CORE_IR_FASTPAD_CALIB = 21
};

/** HW Sequencer Connectivity Configuration */
enum HW_SEQ_CFG {
   /** VSP DE0 Task End */
   HW_SEQ_CFG_VSP_DE0_TASK_END = 0,
   /** VSP DE1 Task End */
   HW_SEQ_CFG_VSP_DE1_TASK_END = 1,
   /** VSP Vector DMA0 Transfer End */
   HW_SEQ_CFG_VSP_VDMAC0_TRANSFER_END = 2,
   /** VSP Vector DMA1 Transfer End */
   HW_SEQ_CFG_VSP_VDMAC1_TRANSFER_END = 3,
   /** HW Accelerator RF DMA TX0 Task End */
   HW_SEQ_CFG_HWACC_RFDMA_TX0_END = 4,
   /** HW Accelerator RF DMA RX0 Task End */
   HW_SEQ_CFG_HWACC_RFDMA_RX0_END = 5,
   /** HW Accelerator UL BPE Task End */
   HW_SEQ_CFG_HWACC_UL_BPE_END = 6,
   /** HW Accelerator DL BPE Task End */
   HW_SEQ_CFG_HWACC_DL_BPE_END = 7,
   /** HW Accelerator OFDM DEMUX IRQ Pulse */
   HW_SEQ_CFG_HWACC_OFDM_DEMUX_IRQ_PULSE = 8,
   /** HW Accelerator PSS IRQ Pulse */
   HW_SEQ_CFG_HWACC_PSS_IRQ_PULSE = 9,
   /** HW Accelerator SSS IRQ Pulse */
   HW_SEQ_CFG_HWACC_SSS_IRQ_PULSE = 10,
   /** TSCU Timing Control Unit 0 Event bit 0 */
   HW_SEQ_CFG_TCU_TCU0_FEVENT_0 = 11,
   /** TSCU Timing Control Unit 0 Event bit 1 */
   HW_SEQ_CFG_TCU_TCU0_FEVENT_1 = 12,
   /** TSCU Timing Control Unit 0 Event bit 2 */
   HW_SEQ_CFG_TCU_TCU0_FEVENT_2 = 13,
   /** TSCU Timing Control Unit 0 Event bit 3 */
   HW_SEQ_CFG_TCU_TCU0_FEVENT_3 = 14,
   /** TSCU Timing Control Unit 0 Event bit 4 */
   HW_SEQ_CFG_TCU_TCU0_FEVENT_4 = 15,
   /** TSCU Timing Control Unit 1 Event bit 0 */
   HW_SEQ_CFG_TCU_TCU1_FEVENT_0 = 16,
   /** TSCU Timing Control Unit 1 Event bit 1 */
   HW_SEQ_CFG_TCU_TCU1_FEVENT_1 = 17,
   /** TSCU Timing Control Unit 1 Event bit 2 */
   HW_SEQ_CFG_TCU_TCU1_FEVENT_2 = 18,
   /** TSCU Timing Control Unit 1 Event bit 3 */
   HW_SEQ_CFG_TCU_TCU1_FEVENT_3 = 19,
   /** TSCU Timing Control Unit 1 Event bit 4 */
   HW_SEQ_CFG_TCU_TCU1_FEVENT_4 = 20
};

/** MDM CPU IRQ Connectivity Configuration */
enum MDM_CPU_IRQ_CFG {
   /** CPU CTI Interrupt bit 0 */
   MDM_CPU_IRQ_CFG_CTICPU_IRQ_0 = 0,
   /** CPU CTI Interrupt bit 1 */
   MDM_CPU_IRQ_CFG_CTICPU_IRQ_1 = 1,
   /** BB IPC MailBox TX0 Interrupt */
   MDM_CPU_IRQ_CFG_BB_IPC_MBX_TX0 = 2,
   /** BB IPC MailBox TX1 Interrupt */
   MDM_CPU_IRQ_CFG_BB_IPC_MBX_TX1 = 3,
   /** BB IPC MailBox TX2 Interrupt */
   MDM_CPU_IRQ_CFG_BB_IPC_MBX_TX2 = 4,
   /** BB IPC MailBox RX0 Interrupt */
   MDM_CPU_IRQ_CFG_BB_IPC_MBX_RX0 = 5,
   /** BB IPC MailBox RX1 Interrupt */
   MDM_CPU_IRQ_CFG_BB_IPC_MBX_RX1 = 6,
   /** BB IPC MailBox RX2 Interrupt */
   MDM_CPU_IRQ_CFG_BB_IPC_MBX_RX2 = 7,
   /** RF IPC MailBox TX0 Interrupt */
   MDM_CPU_IRQ_CFG_RF_IPC_MBX_TX0 = 8,
   /** RF IPC MailBox TX1 Interrupt */
   MDM_CPU_IRQ_CFG_RF_IPC_MBX_TX1 = 9,
   /** RF IPC MailBox RX0 Interrupt */
   MDM_CPU_IRQ_CFG_RF_IPC_MBX_RX0 = 10,
   /** RF IPC MailBox RX1 Interrupt */
   MDM_CPU_IRQ_CFG_RF_IPC_MBX_RX1 = 11,
   /** TSCU Slow Clocking Unit sleepstate on */
   MDM_CPU_IRQ_CFG_SCU_FCLK_SLEEPSTATE_ON = 12,
   /** TSCU Slow Clocking Unit Fault to Timing Control Unit 0 */
   MDM_CPU_IRQ_CFG_SCU_TCU0_FAULT = 13,
   /** TSCU Slow Clocking Unit Fault to Timing Control Unit 1 */
   MDM_CPU_IRQ_CFG_SCU_TCU1_FAULT = 14,
   /** TSCU Slow Clocking Unit Calibrate End */
   MDM_CPU_IRQ_CFG_SCU_CALIBRATE_END = 15,
   /** TSCU Slow Clocking Unit Sleep End */
   MDM_CPU_IRQ_CFG_SCU_SLEEP_END = 16,
   /** TSCU Timing Control Unit 0 Sequence FIFO Near Full */
   MDM_CPU_IRQ_CFG_TCU_TCU0_SFIFO_NEAR_FULL = 17,
   /** TSCU Timing Control Unit 0 Sequence FIFO Near Empty */
   MDM_CPU_IRQ_CFG_TCU_TCU0_SFIFO_NEAR_EMPTY = 18,
   /** TSCU Timing Control Unit 1 Sequence FIFO Near Full */
   MDM_CPU_IRQ_CFG_TCU_TCU1_SFIFO_NEAR_FULL = 19,
   /** TSCU Timing Control Unit 1 Sequence FIFO Near Empty */
   MDM_CPU_IRQ_CFG_TCU_TCU1_SFIFO_NEAR_EMPTY = 20,
   /** TSCU Timing Control Unit 0 Event bit 0 */
   MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_0 = 21,
   /** TSCU Timing Control Unit 0 Event bit 1 */
   MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_1 = 22,
   /** TSCU Timing Control Unit 0 Event bit 2 */
   MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_2 = 23,
   /** TSCU Timing Control Unit 0 Event bit 3 */
   MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_3 = 24,
   /** TSCU Timing Control Unit 0 Event bit 4 */
   MDM_CPU_IRQ_CFG_TCU_TCU0_FEVENT_4 = 25,
   /** TSCU Timing Control Unit 1 Event bit 0 */
   MDM_CPU_IRQ_CFG_TCU_TCU1_FEVENT_0 = 26,
   /** TSCU Timing Control Unit 1 Event bit 1 */
   MDM_CPU_IRQ_CFG_TCU_TCU1_FEVENT_1 = 27,
   /** TSCU Timing Control Unit 1 Event bit 2 */
   MDM_CPU_IRQ_CFG_TCU_TCU1_FEVENT_2 = 28,
   /** TSCU Timing Control Unit 1 Event bit 3 */
   MDM_CPU_IRQ_CFG_TCU_TCU1_FEVENT_3 = 29,
   /** TSCU Timing Control Unit 1 Event bit 4 */
   MDM_CPU_IRQ_CFG_TCU_TCU1_FEVENT_4 = 30,
   /** USIM Card Reader interrupt */
   MDM_CPU_IRQ_CFG_INTERRUPT = 31,
   /** Hardware Sequencer Flag OR-reduced */
   MDM_CPU_IRQ_CFG_HWSEQ_FLAG = 32,
   /** Hardware Sequencer Job Event IRQ */
   MDM_CPU_IRQ_CFG_HWSEQ_JOB_EVENT_IRQ = 33,
   /** Hardware Sequencer Job FIFO IRQ */
   MDM_CPU_IRQ_CFG_HWSEQ_JOB_FIFO_IRQ = 34,
   /** VSP DE0 Task Start */
   MDM_CPU_IRQ_CFG_VSP_DE0_TASK_START = 35,
   /** VSP DE1 Task Start */
   MDM_CPU_IRQ_CFG_VSP_DE1_TASK_START = 36,
   /** VSP Vector DMA0 Transfer Start */
   MDM_CPU_IRQ_CFG_VSP_VDMAC0_TRANSFER_START = 37,
   /** VSP Vector DMA1 Transfer Start */
   MDM_CPU_IRQ_CFG_VSP_VDMAC1_TRANSFER_START = 38,
   /** HW Accelerator RFDMA TX0 Task Start */
   MDM_CPU_IRQ_CFG_HWACC_RFDMA_TX0_START = 39,
   /** HW Accelerator RFDMA RX0 Task Start */
   MDM_CPU_IRQ_CFG_HWACC_RFDMA_RX0_START = 40,
   /** HW Accelerator UL BPE Task Start */
   MDM_CPU_IRQ_CFG_HWACC_UL_BPE_START = 41,
   /** HW Accelerator DL BPE Task Start */
   MDM_CPU_IRQ_CFG_HWACC_DL_BPE_START = 42,
   /** VSP DE0 Task End */
   MDM_CPU_IRQ_CFG_VSP_DE0_TASK_END = 43,
   /** VSP DE1 Task End */
   MDM_CPU_IRQ_CFG_VSP_DE1_TASK_END = 44,
   /** VSP Vector DMA0 Channel 0 Transfer End */
   MDM_CPU_IRQ_CFG_VSP_VDMAC0_TRANSFER_END = 45,
   /** VSP Vector DMA1 Channel 0 Transfer End */
   MDM_CPU_IRQ_CFG_VSP_VDMAC1_TRANSFER_END = 46,
   /** HW Accelerator RFDMA TX0 Task End */
   MDM_CPU_IRQ_CFG_HWACC_RFDMA_TX0_END = 47,
   /** HW Accelerator RFDMA RX0 Task End */
   MDM_CPU_IRQ_CFG_HWACC_RFDMA_RX0_END = 48,
   /** HW Accelerator UL BPE Task End */
   MDM_CPU_IRQ_CFG_HWACC_UL_BPE_END = 49,
   /** HW Accelerator DL BPE Task End */
   MDM_CPU_IRQ_CFG_HWACC_DL_BPE_END = 50,
   /** VSP DE0/DE1 Fault */
   MDM_CPU_IRQ_CFG_VSP_FAULT = 51,
   /** VSP PMU Fault */
   MDM_CPU_IRQ_CFG_VSP_PMU_FAULT = 52,
   /** HW Accelerator RF DMA Engine Fault */
   MDM_CPU_IRQ_CFG_HWACC_RFDMA_FAULT = 53,
   /** HW Accelerator UpLink BitProcessing Engine Fault */
   MDM_CPU_IRQ_CFG_HWACC_UL_BPE_FAULT = 54,
   /** HW Accelerator DownLink BitProcessing Engine Fault */
   MDM_CPU_IRQ_CFG_HWACC_DL_BPE_FAULT = 55,
   /** HW Accelerator OFDM Demux Fault */
   MDM_CPU_IRQ_CFG_HWACC_OFDMDEMUX_FAULT = 56,
   /** HW Accelerator Primary Sink Fault */
   MDM_CPU_IRQ_CFG_HWACC_PSS_FAULT = 57,
   /** HW Accelerator Secondary Sink Fault */
   MDM_CPU_IRQ_CFG_HWACC_SSS_FAULT = 58,
   /** Hardware Sequencer Fault */
   MDM_CPU_IRQ_CFG_HWACC_HWSEQ_FAULT = 59,
   /** System controller IRQ */
   MDM_CPU_IRQ_CFG_SYS_CTRL_MDM_CPU_IRQ = 60,
   /** RF Rx Scratchpad Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX_SCR_INT = 61,
   /** RF Timestamp Manager Interrupt */
   MDM_CPU_IRQ_CFG_RF_TS_MGR_INT = 62,
   /** RF Scheduler Interrupt */
   MDM_CPU_IRQ_CFG_RF_SCH_INT = 63,
   /** RF RFFE Interrupt */
   MDM_CPU_IRQ_CFG_RF_RFFE_INT = 64,
   /** RF Rx1 Data Fault I Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_DATA_I_FAULT_INT = 65,
   /** RF Rx1 Conv Fault I Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_CONV_I_FAULT_INT = 66,
   /** RF Rx1 Data Fault Q Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_DATA_Q_FAULT_INT = 67,
   /** RF RF Rx1 Conv Fault Q Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_CONV_Q_FAULT_INT = 68,
   /** RF Rx1 ADC Cal Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_ADC_CAL_DONE_INT = 69,
   /** RF Rx1 DCOC Count Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_DCOC_COUNT_DONE_INT = 70,
   /** RF Rx1 Corr Count Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_CORR_COUNT_DONE_INT = 71,
   /** RF Rx1 Histogram Overflow Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_HIST_OVERFLOW_INT = 72,
   /** RF Rx1 Histogram Count Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_HIST_COUNT_DONE_INT = 73,
   /** RF Rx1 DCOC Count NB Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_DCOC_COUNT_NB_DONE_INT = 74,
   /** RF Rx1 Corr Count NB Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_RX1_CORR_COUNT_NB_DONE_INT = 75,
   /** RF Aux Data Fault Interrupt */
   MDM_CPU_IRQ_CFG_RF_AUX_DATA_FAULT_INT = 76,
   /** RF Aux Conv Fault Interrupt */
   MDM_CPU_IRQ_CFG_RF_AUX_CONV_FAULT_INT = 77,
   /** RF Aux ADC Cal Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_AUX_ADC_CAL_DONE_INT = 78,
   /** RF Aux DCOC Count Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_AUX_DCOC_COUNT_DONE_INT = 79,
   /** RF Auc Power Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_AUX_PWR_DONE_INT = 80,
   /** RFAux DCOC Count NB Done Interrupt */
   MDM_CPU_IRQ_CFG_RF_AUX_DCOC_COUNT_NB_DONE_INT = 81,
   /** RF SMURF Fault Interrupt */
   MDM_CPU_IRQ_CFG_RF_SMURF_FAULT_INT = 82,
   /** RF Clock Control Unit Error Interrupt */
   MDM_CPU_IRQ_CFG_RF_CCU_INT = 85,
   /** RF SYN1 PLL Lock Entry */
   MDM_CPU_IRQ_CFG_RF_SYN1_PLL_LOCK_ENTRY = 86,
   /** RF SYN1 PLL Lock Exit */
   MDM_CPU_IRQ_CFG_RF_SYN1_PLL_LOCK_EXIT = 87,
   /** RF SYN2 PLL Lock Entry */
   MDM_CPU_IRQ_CFG_RF_SYN2_PLL_LOCK_ENTRY = 88,
   /** RF SYN2 PLL Lock Exit */
   MDM_CPU_IRQ_CFG_RF_SYN2_PLL_LOCK_EXIT = 89,
   /** RF SYN1 PLL Calibration Done */
   MDM_CPU_IRQ_CFG_RF_SYN1_PLL_CAL_DONE = 90,
   /** RF SYN2 PLL Calibration Done */
   MDM_CPU_IRQ_CFG_RF_SYN2_PLL_CAL_DONE = 91,
   /** FBUF IRQ */
   MDM_CPU_IRQ_CFG_HWACC_FBUF_IRQ = 93,
   /** OFDM Demux IRQ 0 (To Be Defined) */
   MDM_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_0 = 94,
   /** OFDM Demux IRQ 1 (To Be Defined) */
   MDM_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_1 = 95,
   /** OFDM Demux IRQ 2 (To Be Defined) */
   MDM_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_2 = 96,
   /** OFDM Demux IRQ 3 (To Be Defined) */
   MDM_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_3 = 97,
   /** PSS IRQ */
   MDM_CPU_IRQ_CFG_HWACC_PSS_IRQ = 98,
   /** SSS IRQ */
   MDM_CPU_IRQ_CFG_HWACC_SSS_IRQ = 99
};

/** MDM CPU Non-Maskable Interrupt Connectivity Configuration */
enum MDM_CPU_NMI_CFG {
   /** Watchdog - Non-maskable Interrupt Source */
   MDM_CPU_NMI_CFG_WDG_NMI = 2
};

/** PHY CPU IRQ Connectivity Configuration */
enum PHY_CPU_IRQ_CFG {
   /** CPU CTI Interrupt bit 0 */
   PHY_CPU_IRQ_CFG_CTICPU_IRQ_0 = 0,
   /** CPU CTI Interrupt bit 1 */
   PHY_CPU_IRQ_CFG_CTICPU_IRQ_1 = 1,
   /** BB IPC MailBox TX0 Interrupt */
   PHY_CPU_IRQ_CFG_BB_IPC_MBX_TX0 = 2,
   /** BB IPC MailBox TX1 Interrupt */
   PHY_CPU_IRQ_CFG_BB_IPC_MBX_TX1 = 3,
   /** BB IPC MailBox TX2 Interrupt */
   PHY_CPU_IRQ_CFG_BB_IPC_MBX_TX2 = 4,
   /** BB IPC MailBox RX0 Interrupt */
   PHY_CPU_IRQ_CFG_BB_IPC_MBX_RX0 = 5,
   /** BB IPC MailBox RX1 Interrupt */
   PHY_CPU_IRQ_CFG_BB_IPC_MBX_RX1 = 6,
   /** BB IPC MailBox RX2 Interrupt */
   PHY_CPU_IRQ_CFG_BB_IPC_MBX_RX2 = 7,
   /** RF IPC MailBox TX0 Interrupt */
   PHY_CPU_IRQ_CFG_RF_IPC_MBX_TX0 = 8,
   /** RF IPC MailBox TX1 Interrupt */
   PHY_CPU_IRQ_CFG_RF_IPC_MBX_TX1 = 9,
   /** RF IPC MailBox RX0 Interrupt */
   PHY_CPU_IRQ_CFG_RF_IPC_MBX_RX0 = 10,
   /** RF IPC MailBox RX1 Interrupt */
   PHY_CPU_IRQ_CFG_RF_IPC_MBX_RX1 = 11,
   /** TSCU Slow Clocking Unit sleepstate on */
   PHY_CPU_IRQ_CFG_SCU_FCLK_SLEEPSTATE_ON = 12,
   /** TSCU Slow Clocking Unit Fault to Timing Control Unit 0 */
   PHY_CPU_IRQ_CFG_SCU_TCU0_FAULT = 13,
   /** TSCU Slow Clocking Unit Fault to Timing Control Unit 1 */
   PHY_CPU_IRQ_CFG_SCU_TCU1_FAULT = 14,
   /** TSCU Slow Clocking Unit Calibrate End */
   PHY_CPU_IRQ_CFG_SCU_CALIBRATE_END = 15,
   /** TSCU Slow Clocking Unit Sleep End */
   PHY_CPU_IRQ_CFG_SCU_SLEEP_END = 16,
   /** TSCU Timing Control Unit 0 Sequence FIFO Near Full */
   PHY_CPU_IRQ_CFG_TCU_TCU0_SFIFO_NEAR_FULL = 17,
   /** TSCU Timing Control Unit 0 Sequence FIFO Near Empty */
   PHY_CPU_IRQ_CFG_TCU_TCU0_SFIFO_NEAR_EMPTY = 18,
   /** TSCU Timing Control Unit 1 Sequence FIFO Near Full */
   PHY_CPU_IRQ_CFG_TCU_TCU1_SFIFO_NEAR_FULL = 19,
   /** TSCU Timing Control Unit 1 Sequence FIFO Near Empty */
   PHY_CPU_IRQ_CFG_TCU_TCU1_SFIFO_NEAR_EMPTY = 20,
   /** TSCU Timing Control Unit 0 Event bit 0 */
   PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_0 = 21,
   /** TSCU Timing Control Unit 0 Event bit 1 */
   PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_1 = 22,
   /** TSCU Timing Control Unit 0 Event bit 2 */
   PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_2 = 23,
   /** TSCU Timing Control Unit 0 Event bit 3 */
   PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_3 = 24,
   /** TSCU Timing Control Unit 0 Event bit 4 */
   PHY_CPU_IRQ_CFG_TCU_TCU0_FEVENT_4 = 25,
   /** TSCU Timing Control Unit 1 Event bit 0 */
   PHY_CPU_IRQ_CFG_TCU_TCU1_FEVENT_0 = 26,
   /** TSCU Timing Control Unit 1 Event bit 1 */
   PHY_CPU_IRQ_CFG_TCU_TCU1_FEVENT_1 = 27,
   /** TSCU Timing Control Unit 1 Event bit 2 */
   PHY_CPU_IRQ_CFG_TCU_TCU1_FEVENT_2 = 28,
   /** TSCU Timing Control Unit 1 Event bit 3 */
   PHY_CPU_IRQ_CFG_TCU_TCU1_FEVENT_3 = 29,
   /** TSCU Timing Control Unit 1 Event bit 4 */
   PHY_CPU_IRQ_CFG_TCU_TCU1_FEVENT_4 = 30,
   /** USIM Card Reader interrupt */
   PHY_CPU_IRQ_CFG_INTERRUPT = 31,
   /** Hardware Sequencer Flag OR-reduced */
   PHY_CPU_IRQ_CFG_HWSEQ_FLAG = 32,
   /** Hardware Sequencer Job Event IRQ */
   PHY_CPU_IRQ_CFG_HWSEQ_JOB_EVENT_IRQ = 33,
   /** Hardware Sequencer Job FIFO IRQ */
   PHY_CPU_IRQ_CFG_HWSEQ_JOB_FIFO_IRQ = 34,
   /** VSP DE0 Task Start */
   PHY_CPU_IRQ_CFG_VSP_DE0_TASK_START = 35,
   /** VSP DE1 Task Start */
   PHY_CPU_IRQ_CFG_VSP_DE1_TASK_START = 36,
   /** VSP Vector DMA0 Transfer Start */
   PHY_CPU_IRQ_CFG_VSP_VDMAC0_TRANSFER_START = 37,
   /** VSP Vector DMA1 Transfer Start */
   PHY_CPU_IRQ_CFG_VSP_VDMAC1_TRANSFER_START = 38,
   /** HW Accelerator RFDMA TX0 Task Start */
   PHY_CPU_IRQ_CFG_HWACC_RFDMA_TX0_START = 39,
   /** HW Accelerator RFDMA RX0 Task Start */
   PHY_CPU_IRQ_CFG_HWACC_RFDMA_RX0_START = 40,
   /** HW Accelerator UL BPE Task Start */
   PHY_CPU_IRQ_CFG_HWACC_UL_BPE_START = 41,
   /** HW Accelerator DL BPE Task Start */
   PHY_CPU_IRQ_CFG_HWACC_DL_BPE_START = 42,
   /** VSP DE0 Task End */
   PHY_CPU_IRQ_CFG_VSP_DE0_TASK_END = 43,
   /** VSP DE1 Task End */
   PHY_CPU_IRQ_CFG_VSP_DE1_TASK_END = 44,
   /** VSP Vector DMA0 Channel 0 Transfer End */
   PHY_CPU_IRQ_CFG_VSP_VDMAC0_TRANSFER_END = 45,
   /** VSP Vector DMA1 Channel 0 Transfer End */
   PHY_CPU_IRQ_CFG_VSP_VDMAC1_TRANSFER_END = 46,
   /** HW Accelerator RFDMA TX0 Task End */
   PHY_CPU_IRQ_CFG_HWACC_RFDMA_TX0_END = 47,
   /** HW Accelerator RFDMA RX0 Task End */
   PHY_CPU_IRQ_CFG_HWACC_RFDMA_RX0_END = 48,
   /** HW Accelerator UL BPE Task End */
   PHY_CPU_IRQ_CFG_HWACC_UL_BPE_END = 49,
   /** HW Accelerator DL BPE Task End */
   PHY_CPU_IRQ_CFG_HWACC_DL_BPE_END = 50,
   /** VSP DE0/DE1 Fault */
   PHY_CPU_IRQ_CFG_VSP_FAULT = 51,
   /** VSP PMU Fault */
   PHY_CPU_IRQ_CFG_VSP_PMU_FAULT = 52,
   /** HW Accelerator RF DMA Engine Fault */
   PHY_CPU_IRQ_CFG_HWACC_RFDMA_FAULT = 53,
   /** HW Accelerator UpLink BitProcessing Engine Fault */
   PHY_CPU_IRQ_CFG_HWACC_UL_BPE_FAULT = 54,
   /** HW Accelerator DownLink BitProcessing Engine Fault */
   PHY_CPU_IRQ_CFG_HWACC_DL_BPE_FAULT = 55,
   /** HW Accelerator OFDM Demux Fault */
   PHY_CPU_IRQ_CFG_HWACC_OFDMDEMUX_FAULT = 56,
   /** HW Accelerator Primary Sink Fault */
   PHY_CPU_IRQ_CFG_HWACC_PSS_FAULT = 57,
   /** HW Accelerator Secondary Sink Fault */
   PHY_CPU_IRQ_CFG_HWACC_SSS_FAULT = 58,
   /** Hardware Sequencer Fault */
   PHY_CPU_IRQ_CFG_HWACC_HWSEQ_FAULT = 59,
   /** System controller IRQ */
   PHY_CPU_IRQ_CFG_SYS_CTRL_MDM_CPU_IRQ = 60,
   /** RF Rx Scratchpad Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX_SCR_INT = 61,
   /** RF Timestamp Manager Interrupt */
   PHY_CPU_IRQ_CFG_RF_TS_MGR_INT = 62,
   /** RF Scheduler Interrupt */
   PHY_CPU_IRQ_CFG_RF_SCH_INT = 63,
   /** RF RFFE Interrupt */
   PHY_CPU_IRQ_CFG_RF_RFFE_INT = 64,
   /** RF Rx1 Data Fault I Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_DATA_I_FAULT_INT = 65,
   /** RF Rx1 Conv Fault I Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_CONV_I_FAULT_INT = 66,
   /** RF Rx1 Data Fault Q Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_DATA_Q_FAULT_INT = 67,
   /** RF RF Rx1 Conv Fault Q Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_CONV_Q_FAULT_INT = 68,
   /** RF Rx1 ADC Cal Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_ADC_CAL_DONE_INT = 69,
   /** RF Rx1 DCOC Count Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_DCOC_COUNT_DONE_INT = 70,
   /** RF Rx1 Corr Count Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_CORR_COUNT_DONE_INT = 71,
   /** RF Rx1 Histogram Overflow Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_HIST_OVERFLOW_INT = 72,
   /** RF Rx1 Histogram Count Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_HIST_COUNT_DONE_INT = 73,
   /** RF Rx1 DCOC Count NB Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_DCOC_COUNT_NB_DONE_INT = 74,
   /** RF Rx1 Corr Count NB Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_RX1_CORR_COUNT_NB_DONE_INT = 75,
   /** RF Aux Data Fault Interrupt */
   PHY_CPU_IRQ_CFG_RF_AUX_DATA_FAULT_INT = 76,
   /** RF Aux Conv Fault Interrupt */
   PHY_CPU_IRQ_CFG_RF_AUX_CONV_FAULT_INT = 77,
   /** RF Aux ADC Cal Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_AUX_ADC_CAL_DONE_INT = 78,
   /** RF Aux DCOC Count Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_AUX_DCOC_COUNT_DONE_INT = 79,
   /** RF Auc Power Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_AUX_PWR_DONE_INT = 80,
   /** RFAux DCOC Count NB Done Interrupt */
   PHY_CPU_IRQ_CFG_RF_AUX_DCOC_COUNT_NB_DONE_INT = 81,
   /** RF SMURF Fault Interrupt */
   PHY_CPU_IRQ_CFG_RF_SMURF_FAULT_INT = 82,
   /** RF Clock Control Unit Error Interrupt */
   PHY_CPU_IRQ_CFG_RF_CCU_INT = 85,
   /** RF SYN1 PLL Lock Entry */
   PHY_CPU_IRQ_CFG_RF_SYN1_PLL_LOCK_ENTRY = 86,
   /** RF SYN1 PLL Lock Exit */
   PHY_CPU_IRQ_CFG_RF_SYN1_PLL_LOCK_EXIT = 87,
   /** RF SYN2 PLL Lock Entry */
   PHY_CPU_IRQ_CFG_RF_SYN2_PLL_LOCK_ENTRY = 88,
   /** RF SYN2 PLL Lock Exit */
   PHY_CPU_IRQ_CFG_RF_SYN2_PLL_LOCK_EXIT = 89,
   /** RF SYN1 PLL Calibration Done */
   PHY_CPU_IRQ_CFG_RF_SYN1_PLL_CAL_DONE = 90,
   /** RF SYN2 PLL Calibration Done */
   PHY_CPU_IRQ_CFG_RF_SYN2_PLL_CAL_DONE = 91,
   /** FBUF IRQ */
   PHY_CPU_IRQ_CFG_HWACC_FBUF_IRQ = 93,
   /** OFDM Demux IRQ 0 (To Be Defined) */
   PHY_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_0 = 94,
   /** OFDM Demux IRQ 1 (To Be Defined) */
   PHY_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_1 = 95,
   /** OFDM Demux IRQ 2 (To Be Defined) */
   PHY_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_2 = 96,
   /** OFDM Demux IRQ 3 (To Be Defined) */
   PHY_CPU_IRQ_CFG_HWACC_OFDMDEMUX_IRQ_3 = 97,
   /** PSS IRQ */
   PHY_CPU_IRQ_CFG_HWACC_PSS_IRQ = 98,
   /** SSS IRQ */
   PHY_CPU_IRQ_CFG_HWACC_SSS_IRQ = 99
};

/** PHY CPU Non-Maskable Interrupt Connectivity Configuration */
enum PHY_CPU_NMI_CFG {
   /** Watchdog - Non-maskable Interrupt Source */
   PHY_CPU_NMI_CFG_WDG_NMI = 2
};

/** Processor and Bus Master IDs */
enum HAL_PROC_ID {
   /** Modem CPU ID */
   HAL_PROC_ID_MDM_CPU = 0,
   /** RF CPU ID */
   HAL_PROC_ID_RF_CPU = 1,
   /** Modem CPU ID */
   HAL_PROC_ID_APPS_CPU = 2,
   /** Modem CPU ID */
   HAL_PROC_ID_PHY_CPU = 3,
   /** System DMA ID */
   HAL_PROC_ID_SDMA = 4,
   /** VSP0 DE0 ID */
   HAL_PROC_ID_VSP0_DE0 = 5,
   /** VSP0 Vector DMA ID */
   HAL_PROC_ID_VSP0_VDMA = 6,
   /** ARM Cryptocell-310 */
   HAL_PROC_ID_CRYPTOCELL310 = 7,
   /** Hardware Sequencer */
   HAL_PROC_ID_HW_SEQUENCER = 8,
   /** MODEM SS LTE Accelerators DMA */
   HAL_PROC_ID_LTE_ACCEL_DMA = 9,
   /** USB SS DMA */
   HAL_PROC_ID_USB_DMA = 10,
   /** SDIO Interface DMA */
   HAL_PROC_ID_SDIO_DMA = 11
};

/** PIO mapping */
enum PIO_CH {
   /** PIO channel number for GPIO0 */
   PIO_CH_GPIO0 = 0,
   /** PIO channel number for GPIO1 */
   PIO_CH_GPIO1 = 1,
   /** PIO channel number for GPIO2 */
   PIO_CH_GPIO2 = 2,
   /** PIO channel number for GPIO3 */
   PIO_CH_GPIO3 = 3,
   /** PIO channel number for GPIO4 */
   PIO_CH_GPIO4 = 4,
   /** PIO channel number for GPIO5 */
   PIO_CH_GPIO5 = 5,
   /** PIO channel number for GPIO6 */
   PIO_CH_GPIO6 = 6,
   /** PIO channel number for PWRGPIO0 */
   PIO_CH_PWRGPIO0 = 7,
   /** PIO channel number for PWRGPIO1 */
   PIO_CH_PWRGPIO1 = 8,
   /** PIO channel number for HWTESTMODE */
   PIO_CH_HWTESTMODE = 9,
   /** PIO channel number for TRCCLK */
   PIO_CH_TRCCLK = 10,
   /** PIO channel number for TRCCTL */
   PIO_CH_TRCCTL = 11,
   /** PIO channel number for TRCDATA0 */
   PIO_CH_TRCDATA0 = 12,
   /** PIO channel number for TRCDATA1 */
   PIO_CH_TRCDATA1 = 13,
   /** PIO channel number for TRCDATA2 */
   PIO_CH_TRCDATA2 = 14,
   /** PIO channel number for TRCDATA3 */
   PIO_CH_TRCDATA3 = 15,
   /** PIO channel number for SDDAT0 */
   PIO_CH_SDDAT0 = 16,
   /** PIO channel number for SDDAT1 */
   PIO_CH_SDDAT1 = 17,
   /** PIO channel number for SDDAT2 */
   PIO_CH_SDDAT2 = 18,
   /** PIO channel number for SDDAT3 */
   PIO_CH_SDDAT3 = 19,
   /** PIO channel number for SDCLK */
   PIO_CH_SDCLK = 20,
   /** PIO channel number for SDCMD */
   PIO_CH_SDCMD = 21,
   /** PIO channel number for DISPIO0 */
   PIO_CH_DISPIO0 = 22,
   /** PIO channel number for DISPIO1 */
   PIO_CH_DISPIO1 = 23,
   /** PIO channel number for DISPIO2 */
   PIO_CH_DISPIO2 = 24,
   /** PIO channel number for DISPIO3 */
   PIO_CH_DISPIO3 = 25,
   /** PIO channel number for DISPIO4 */
   PIO_CH_DISPIO4 = 26,
   /** PIO channel number for DISPIO5 */
   PIO_CH_DISPIO5 = 27,
   /** PIO channel number for DISPIO6 */
   PIO_CH_DISPIO6 = 28,
   /** PIO channel number for DISPIO7 */
   PIO_CH_DISPIO7 = 29,
   /** PIO channel number for DISPGPIO0 */
   PIO_CH_DISPGPIO0 = 30,
   /** PIO channel number for DISPGPIO1 */
   PIO_CH_DISPGPIO1 = 31,
   /** PIO channel number for DISPGPIO2 */
   PIO_CH_DISPGPIO2 = 32,
   /** PIO channel number for DISPGPIO3 */
   PIO_CH_DISPGPIO3 = 33,
   /** PIO channel number for DISPGPIO4 */
   PIO_CH_DISPGPIO4 = 34,
   /** PIO channel number for I2SSDI */
   PIO_CH_I2SSDI = 35,
   /** PIO channel number for I2SSCK */
   PIO_CH_I2SSCK = 36,
   /** PIO channel number for I2SSDO */
   PIO_CH_I2SSDO = 37,
   /** PIO channel number for I2SWS */
   PIO_CH_I2SWS = 38,
   /** PIO channel number for USIMRSTB */
   PIO_CH_USIMRSTB = 39,
   /** PIO channel number for USIMDAT */
   PIO_CH_USIMDAT = 40,
   /** PIO channel number for USIMCLK */
   PIO_CH_USIMCLK = 41,
   /** PIO channel number for UART1RXD */
   PIO_CH_UART1RXD = 42,
   /** PIO channel number for UART1TXD */
   PIO_CH_UART1TXD = 43,
   /** PIO channel number for UART1CTS */
   PIO_CH_UART1CTS = 44,
   /** PIO channel number for UART1RTS */
   PIO_CH_UART1RTS = 45,
   /** PIO channel number for UART2RXD */
   PIO_CH_UART2RXD = 46,
   /** PIO channel number for UART2TXD */
   PIO_CH_UART2TXD = 47,
   /** PIO channel number for UART2CTS */
   PIO_CH_UART2CTS = 48,
   /** PIO channel number for UART2RTS */
   PIO_CH_UART2RTS = 49,
   /** PIO channel number for I2C1SCL */
   PIO_CH_I2C1SCL = 50,
   /** PIO channel number for I2C1SDA */
   PIO_CH_I2C1SDA = 51,
   /** PIO channel number for I2C2SCL */
   PIO_CH_I2C2SCL = 52,
   /** PIO channel number for I2C2SDA */
   PIO_CH_I2C2SDA = 53,
   /** PIO channel number for VBUSVALID */
   PIO_CH_VBUSVALID = 54,
   /** PIO channel number for ACTIMEBASESYNC */
   PIO_CH_ACTIMEBASESYNC = 55,
   /** PIO channel number for RFFECLK */
   PIO_CH_RFFECLK = 56,
   /** PIO channel number for RFFEDATA */
   PIO_CH_RFFEDATA = 57,
   /** PIO channel number for RFGPIO2 */
   PIO_CH_RFGPIO2 = 58,
   /** PIO channel number for RFGPIO3 */
   PIO_CH_RFGPIO3 = 59
};

/** RF CPU IRQ Connectivity Configuration */
enum RF_CPU_IRQ_CFG {
   /** RF Rx Scratchpad Interrupt */
   RF_CPU_IRQ_CFG_RX_SCR_INT = 0,
   /** RF Timestamp Manager Interrupt */
   RF_CPU_IRQ_CFG_TS_MGR_INT = 1,
   /** RF Scheduler Interrupt */
   RF_CPU_IRQ_CFG_SCH_INT = 2,
   /** RF RFFE Interrupt */
   RF_CPU_IRQ_CFG_RFFE_INT = 3,
   /** RF Rx1 Data Fault I Interrupt */
   RF_CPU_IRQ_CFG_RX1_DATA_I_FAULT_INT = 4,
   /** RF Rx1 Conv Fault I Interrupt */
   RF_CPU_IRQ_CFG_RX1_CONV_I_FAULT_INT = 5,
   /** RF Rx1 Data Fault Q Interrupt */
   RF_CPU_IRQ_CFG_RX1_DATA_Q_FAULT_INT = 6,
   /** RF RF Rx1 Conv Fault Q Interrupt */
   RF_CPU_IRQ_CFG_RX1_CONV_Q_FAULT_INT = 7,
   /** RF Rx1 ADC Cal Done Interrupt */
   RF_CPU_IRQ_CFG_RX1_ADC_CAL_DONE_INT = 8,
   /** RF Rx1 DCOC Count Done Interrupt */
   RF_CPU_IRQ_CFG_RX1_DCOC_COUNT_DONE_INT = 9,
   /** RF Rx1 Corr Count Done Interrupt */
   RF_CPU_IRQ_CFG_RX1_CORR_COUNT_DONE_INT = 10,
   /** RF Rx1 Histogram Overflow Interrupt */
   RF_CPU_IRQ_CFG_RX1_HIST_OVERFLOW_INT = 11,
   /** RF Rx1 Histogram Count Done Interrupt */
   RF_CPU_IRQ_CFG_RX1_HIST_COUNT_DONE_INT = 12,
   /** RF Rx1 DCOC Count NB Done Interrupt */
   RF_CPU_IRQ_CFG_RX1_DCOC_COUNT_NB_DONE_INT = 13,
   /** RF Rx1 Corr Count NB Done Interrupt */
   RF_CPU_IRQ_CFG_RX1_CORR_COUNT_NB_DONE_INT = 14,
   /** RF Aux Data Fault Interrupt */
   RF_CPU_IRQ_CFG_AUX_DATA_FAULT_INT = 15,
   /** RF Aux Conv Fault Interrupt */
   RF_CPU_IRQ_CFG_AUX_CONV_FAULT_INT = 16,
   /** RF Aux ADC Cal Done Interrupt */
   RF_CPU_IRQ_CFG_AUX_ADC_CAL_DONE_INT = 17,
   /** RF Aux DCOC Count Done Interrupt */
   RF_CPU_IRQ_CFG_AUX_DCOC_COUNT_DONE_INT = 18,
   /** RF Auc Power Done Interrupt */
   RF_CPU_IRQ_CFG_AUX_PWR_DONE_INT = 19,
   /** RFAux DCOC Count NB Done Interrupt */
   RF_CPU_IRQ_CFG_AUX_DCOC_COUNT_NB_DONE_INT = 20,
   /** RF SMURF Fault Interrupt */
   RF_CPU_IRQ_CFG_SMURF_FAULT_INT = 21,
   /** RF Clock Control Unit Error Interrupt */
   RF_CPU_IRQ_CFG_CCU_INT = 24,
   /** PLL1 DigIF PLL Lock Entry */
   RF_CPU_IRQ_CFG_SYN1_PLL_LOCK_INT = 25,
   /** PLL1 DigIF PLL Unlocked */
   RF_CPU_IRQ_CFG_SYN1_PLL_UNLOCK_INT = 26,
   /** PLL2 DigIF PLL Locked */
   RF_CPU_IRQ_CFG_SYN2_PLL_LOCK_INT = 27,
   /** PLL2 DigIF PLL Unlocked */
   RF_CPU_IRQ_CFG_SYN2_PLL_UNLOCK_INT = 28,
   /** PLL2 DigIF Cal Done */
   RF_CPU_IRQ_CFG_SYN2_PLL_CAL_DONE_INT = 29,
   /** PLL1 DigIF Cal Done */
   RF_CPU_IRQ_CFG_SYN1_PLL_CAL_DONE_INT = 30,
   /** RF Timer 1 Interrupt */
   RF_CPU_IRQ_CFG_TIMER1_INT = 31,
   /** RF Timer 1 Overflow Interrupt */
   RF_CPU_IRQ_CFG_TIMER1_OVF_INT = 32,
   /** RF Timer 2 Interrupt */
   RF_CPU_IRQ_CFG_TIMER2_INT = 33,
   /** RF Timer 2 Overflow Interrupt */
   RF_CPU_IRQ_CFG_TIMER2_OVF_INT = 34,
   /** RF Rx Mailbox 0 Interrupt */
   RF_CPU_IRQ_CFG_RX_MBX0_INT = 35,
   /** RF Rx Mailbox 1 Interrupt */
   RF_CPU_IRQ_CFG_RX_MBX1_INT = 36,
   /** RF Tx Mailbox 0 Interrupt */
   RF_CPU_IRQ_CFG_TX_MBX0_INT = 37,
   /** RF Tx Mailbox 1 Interrupt */
   RF_CPU_IRQ_CFG_TX_MBX1_INT = 38,
   /** RF Cross Trigger 1 Interface Interrupt */
   RF_CPU_IRQ_CFG_CTI1_INT = 39,
   /** RF Cross Trigger 2 Interface Interrupt */
   RF_CPU_IRQ_CFG_CTI2_INT = 40,
   /** Uart Interrupt */
   RF_CPU_IRQ_CFG_UART_INT = 41
};

/** RF CPU Non-Maskable Interrupt Connectivity Configuration */
enum RF_CPU_NMI_CFG {
   /** Watchdog Interrupt - Non-maskable Interrupt Source */
   RF_CPU_NMI_CFG_WDG_INT = 2
};

/** RF Scheduler Queue IDs */
enum RF_DIG_SCH_Q_DEPTH {
   /** RF Scheduler Q5 depth - PLL2 RX SYN */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_PLL2 = 1,
   /** RF Scheduler Q4 depth - PLL1 TX SYN */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_PLL1 = 1,
   /** RF Scheduler Q1 depth - TX2 (GO Only) */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_TX2 = 4,
   /** RF Scheduler Q7 depth - Master Load */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_MSTR = 4,
   /** RF Scheduler Q0 depth - TX1 */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_TX1 = 4,
   /** RF Scheduler Q6 depth - AUX */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_AUX = 4,
   /** RF Scheduler Q3 depth - RX2 (GO Only) */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_RX2 = 8,
   /** RF Scheduler Q2 depth - RX1 */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_RX1 = 8,
   /** RF Scheduler Q15 depth - CPU Interrupt */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_CPU = 16,
   /** RF Scheduler Q14 depth - RFFE */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_RFFE = 16,
   /** RF Scheduler Q13 depth - Clock */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_CLK = 16,
   /** RF Scheduler Q14 depth - GPIO */
   RF_DIG_SCH_Q_DEPTH_RF_SCH_Q_DEPTH_GPIO = 16
};

/** RF Scheduler Queue IDs */
enum RF_DIG_SCH_Q_ID {
   /** RF Scheduler Q0 - TX1 */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_TX1 = 0,
   /** RF Scheduler Q1 - TX2 (GO Only) */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_TX2 = 1,
   /** RF Scheduler Q2 - RX1 */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_RX1 = 2,
   /** RF Scheduler Q3 - RX2 (GO Only) */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_RX2 = 3,
   /** RF Scheduler Q4 - PLL1 TX SYN */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_PLL1 = 4,
   /** RF Scheduler Q5 - PLL2 RX SYN */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_PLL2 = 5,
   /** RF Scheduler Q6 - AUX */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_AUX = 6,
   /** RF Scheduler Q7 - Master Load */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_MSTR = 7,
   /** RF Scheduler Q13 - Clock */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_CLK = 13,
   /** RF Scheduler Q14 - GPIO */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_GPIO = 14,
   /** RF Scheduler Q14 - RFFE */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_RFFE = 14,
   /** RF Scheduler Q15 - CPU Interrupt */
   RF_DIG_SCH_Q_ID_RF_SCH_Q_CPU = 15
};

/** RF Scheduler Queue 6 Bit Positions for AUX */
enum RF_DIG_SCH_Q_POS_AUX {
   /** AUX DFE Load */
   RF_DIG_SCH_Q_POS_AUX_Q6_AUXLD = 0,
   /** AUX DFE Start */
   RF_DIG_SCH_Q_POS_AUX_Q6_AUXSTART = 1,
   /** AUX DigIF Load */
   RF_DIG_SCH_Q_POS_AUX_Q6_AUXDIGIFLD = 2
};

/** RF Scheduler Queue 13 Bit Positions for the Clock Queue */
enum RF_DIG_SCH_Q_POS_CLK {
   /** UNUSED */
   RF_DIG_SCH_Q_POS_CLK_Q13_UNUSED0 = 0,
   /** UNUSED */
   RF_DIG_SCH_Q_POS_CLK_Q13_UNUSED1 = 1,
   /** UNUSED */
   RF_DIG_SCH_Q_POS_CLK_Q13_UNUSED2 = 2,
   /** UNUSED */
   RF_DIG_SCH_Q_POS_CLK_Q13_UNUSED3 = 3,
   /** Timestamp Manager Trigger */
   RF_DIG_SCH_Q_POS_CLK_Q13_TS_MGR = 4
};

/** RF Scheduler Queue 14 Bit Positions for the GPIO and RFFE Queue */
enum RF_DIG_SCH_Q_POS_GPIO_RFFE {
   /** RFFE Trigger */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_RFFE_TRIG = 0,
   /** GPIO[0] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO0 = 16,
   /** GPIO[1] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO1 = 17,
   /** GPIO[2] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO2 = 18,
   /** GPIO[3] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO3 = 19,
   /** GPIO[4] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO4 = 20,
   /** GPIO[5] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO5 = 21,
   /** GPIO[6] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO6 = 22,
   /** GPIO[7] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO7 = 23,
   /** GPIO[8] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO8 = 24,
   /** GPIO[9] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO9 = 25,
   /** GPIO[10] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO10 = 26,
   /** GPIO[11] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO11 = 27,
   /** GPIO[12] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO12 = 28,
   /** GPIO[13] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO13 = 29,
   /** GPIO[14] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO14 = 30,
   /** GPIO[15] */
   RF_DIG_SCH_Q_POS_GPIO_RFFE_Q14_GPIO15 = 31
};

/** RF Scheduler Queue 7 Bit Positions for Master Load */
enum RF_DIG_SCH_Q_POS_MSTR {
   /** Master Load - used for Config Regs */
   RF_DIG_SCH_Q_POS_MSTR_Q7_MASLD = 0,
   /** Tx Scratchpad Trigger */
   RF_DIG_SCH_Q_POS_MSTR_Q7_TXSCRTRIG = 1,
   /** Rx Scratchpad Trigger */
   RF_DIG_SCH_Q_POS_MSTR_Q7_RXSCRTRIG = 2
};

/** RF Scheduler Queue 4 Bit Positions for PLL1 Tx Synthesiser */
enum RF_DIG_SCH_Q_POS_PLL1 {
   /** PLL1 DFE Load - Tx Synthesiser */
   RF_DIG_SCH_Q_POS_PLL1_Q4_PLL1LD = 0
};

/** RF Scheduler Queue 5 Bit Positions for PLL1 Rx Synthesiser */
enum RF_DIG_SCH_Q_POS_PLL2 {
   /** PLL2 DFE Load - Rx Synthesiser */
   RF_DIG_SCH_Q_POS_PLL2_Q5_PLL2LD = 0
};

/** RF Scheduler Queue 2 Bit Positions for Rx1 */
enum RF_DIG_SCH_Q_POS_RX1 {
   /** RX1 DFE Load */
   RF_DIG_SCH_Q_POS_RX1_Q2_RX1DFELD = 0,
   /** RX1 DFE Start */
   RF_DIG_SCH_Q_POS_RX1_Q2_RX1DFESTART = 1,
   /** RX1 Baseband Load */
   RF_DIG_SCH_Q_POS_RX1_Q2_RX1BBLD = 2,
   /** RX1 RF Load */
   RF_DIG_SCH_Q_POS_RX1_Q2_RX1RFLD = 3,
   /** RX1 ADC Load */
   RF_DIG_SCH_Q_POS_RX1_Q2_RX1ADCLD = 4
};

/** RF Scheduler Queue 3 Bit Positions for Rx2 */
enum RF_DIG_SCH_Q_POS_RX2 {
   /** RX2 DFE Load */
   RF_DIG_SCH_Q_POS_RX2_Q3_RX2DFELD = 0,
   /** RX2 DFE Start */
   RF_DIG_SCH_Q_POS_RX2_Q3_RX2DFESTART = 1,
   /** RX2 Baseband Load */
   RF_DIG_SCH_Q_POS_RX2_Q3_RX2BBLD = 2,
   /** RX2 RF Load */
   RF_DIG_SCH_Q_POS_RX2_Q3_RX2RFLD = 3,
   /** RX2 ADC Load */
   RF_DIG_SCH_Q_POS_RX2_Q3_RX2ADCLD = 4
};

/** RF Scheduler Queue 0 Bit Positions for Tx1 */
enum RF_DIG_SCH_Q_POS_TX1 {
   /** TX1 DFE Load */
   RF_DIG_SCH_Q_POS_TX1_Q0_TX1DFELD = 0,
   /** TX1 DFE Start */
   RF_DIG_SCH_Q_POS_TX1_Q0_TX1DFESTART = 1,
   /** TX1 Baseband Load */
   RF_DIG_SCH_Q_POS_TX1_Q0_TX1BBLD = 2,
   /** TX1 RF Load */
   RF_DIG_SCH_Q_POS_TX1_Q0_TX1RFLD = 3
};

/** RF Scheduler Queue 1 Bit Positions for Tx2 */
enum RF_DIG_SCH_Q_POS_TX2 {
   /** TX2 DFE Load */
   RF_DIG_SCH_Q_POS_TX2_Q1_TX2DFELD = 0,
   /** TX2 DFE Start */
   RF_DIG_SCH_Q_POS_TX2_Q1_TX2DFESTART = 1,
   /** TX2 Baseband Load */
   RF_DIG_SCH_Q_POS_TX2_Q1_TX2BBLD = 2,
   /** TX2 RF Load */
   RF_DIG_SCH_Q_POS_TX2_Q1_TX2RFLD = 3
};

/** RF CPU Memory Numbers */
enum RF_DIG_SS_MEM_ID {
   /** RF CPU Tightly Coupled Memory Identification */
   RF_DIG_SS_MEM_ID_RF_TCM_ID = 0,
   /** RF System RAM */
   RF_DIG_SS_MEM_ID_RF_SYSRAM_ID = 1,
   /** TX RF Scratchpad */
   RF_DIG_SS_MEM_ID_TX_SCRATCHPAD_ID = 2,
   /** RX RF Scratchpad */
   RF_DIG_SS_MEM_ID_RX_SCRATCHPAD_ID = 3
};

/** Processor/Master IDs */
enum RF_HAL_PROC_ID {
   /** RF CPU ID */
   RF_HAL_PROC_ID_RF_CPU = 0,
   /** Base-band Control Interface ID */
   RF_HAL_PROC_ID_BB_CTRL_IF = 1
};

/** PwrCtrl Wakeup Event Mapping */
enum WAKEUP_CH {
   /** PwrCtrl Wakeup Event Mapping  0 */
   WAKEUP_CH_WKUP_USB = 0,
   /** PwrCtrl Wakeup Event Mapping  1 */
   WAKEUP_CH_WKUP_PWRCTRL_WDG = 1,
   /** PwrCtrl Wakeup Event Mapping  2 */
   WAKEUP_CH_WKUP_SCU = 2,
   /** PwrCtrl Wakeup Event Mapping  3 */
   WAKEUP_CH_WKUP_PWR_ALERT = 3,
   /** PwrCtrl Wakeup Event Mapping  4 */
   WAKEUP_CH_WKUP_APP_CPU_IRQ = 4,
   /** PwrCtrl Wakeup Event Mapping  5 */
   WAKEUP_CH_WKUP_SDIO = 5,
   /** PwrCtrl Wakeup Event Mapping  6 */
   WAKEUP_CH_WKUP_UART2_TXD = 6,
   /** PwrCtrl Wakeup Event Mapping  7 */
   WAKEUP_CH_WKUP_UART2_RTS = 7,
   /** PwrCtrl Wakeup Event Mapping  8 */
   WAKEUP_CH_WKUP_SPI1_SEL = 8,
   /** PwrCtrl Wakeup Event Mapping  9 */
   WAKEUP_CH_WKUP_SPI2_SEL = 9,
   /** PwrCtrl Wakeup Event Mapping 10 */
   WAKEUP_CH_WKUP_GNSS_TIME_SYNC = 10,
   /** PwrCtrl Wakeup Event Mapping 11 */
   WAKEUP_CH_WKUP_TIMER1 = 11,
   /** PwrCtrl Wakeup Event Mapping 12 */
   WAKEUP_CH_WKUP_TIMER2 = 12,
   /** PwrCtrl Wakeup Event Mapping 13 */
   WAKEUP_CH_WKUP_USIM_DETECT = 13,
   /** PwrCtrl Wakeup Event Mapping 14 */
   WAKEUP_CH_WKUP_SDCARD_DETECT = 14,
   /** PwrCtrl Wakeup Event Mapping 15 */
   WAKEUP_CH_WKUP_GPIO0 = 15,
   /** PwrCtrl Wakeup Event Mapping 16 */
   WAKEUP_CH_WKUP_GPIO1 = 16,
   /** PwrCtrl Wakeup Event Mapping 17 */
   WAKEUP_CH_WKUP_GPIO2 = 17,
   /** PwrCtrl Wakeup Event Mapping 18 */
   WAKEUP_CH_WKUP_GPIO3 = 18,
   /** PwrCtrl Wakeup Event Mapping 19 */
   WAKEUP_CH_WKUP_GPIO4 = 19,
   /** PwrCtrl Wakeup Event Mapping 20 */
   WAKEUP_CH_WKUP_GPIO5 = 20,
   /** PwrCtrl Wakeup Event Mapping 21 */
   WAKEUP_CH_WKUP_GPIO6 = 21,
   /** PwrCtrl Wakeup Event Mapping 22 */
   WAKEUP_CH_WKUP_UART1_TXD = 22,
   /** PwrCtrl Wakeup Event Mapping 23 */
   WAKEUP_CH_WKUP_UART1_RTS = 23
};
struct kmgo_sysctrl_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *System Control Module->
 *System Controller Module
*/
static volatile struct kmgo_sysctrl_s* const app_ss_sys_ctrl = (struct kmgo_sysctrl_s*)(+0x49000000);
#ifdef __cplusplus
}
#endif

struct km_pwrctrl_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Power Control Module->
 *Power Controller Module
*/
static volatile struct km_pwrctrl_s* const app_ss_pwr_ctrl = (struct km_pwrctrl_s*)(+0x49001000);
#ifdef __cplusplus
}
#endif

struct security_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Security Module->
 *
*/
static volatile struct security_s* const app_ss_security = (struct security_s*)(+0x49002000);
#ifdef __cplusplus
}
#endif

struct km_leti_cvp_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Dynamic Voltage Scaling Controller ->
 *DVS CVP
*/
static volatile struct km_leti_cvp_s* const app_ss_leti_cvp = (struct km_leti_cvp_s*)(+0x49003000);
#ifdef __cplusplus
}
#endif

struct usbhs_cfgs_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *KM USB configuration Registers->
 *usbhs_cfgs
*/
static volatile struct usbhs_cfgs_s* const app_ss_usb_cfgs = (struct usbhs_cfgs_s*)(+0x49004000);
#ifdef __cplusplus
}
#endif

struct km_sf_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Special Function Registers in always ON domain->
 *Klein Matterhorn Special Function Registers
*/
static volatile struct km_sf_s* const app_ss_km_sf = (struct km_sf_s*)(+0x49005000);
#ifdef __cplusplus
}
#endif

struct ospi_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *OSPI Control Interface->
 * ospi_flash_apb_regs
*/
static volatile struct ospi_s* const app_ss_ospi_ctrl = (struct ospi_s*)(+0x49006000);
#ifdef __cplusplus
}
#endif

struct wdg_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *App Processor Watchdog->
 *Watchdog IP block has the following features : 
- APB slave interface
- 16 bit down counters with 15 bit prescale (7 settings for prescale possible)
- Interrupt output generation on timeout
- Reset output generation on timeout with possibility of delayed reset
- Access keys to protect registers from being altered by software unintentionally
- Stall feature supported via external stall input
*/
static volatile struct wdg_s* const app_ss_app_wdg = (struct wdg_s*)(+0x49007000);
#ifdef __cplusplus
}
#endif

struct timer_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Dual Timer->
 *TIMER IP.  
This module has the following features : 
- APB slave interface
- 32 bit down counters with 3 different settings of 8-bit prescale counter 
- Timers programmable as 32 or 16 bit counters
- Timers can run continuously or for single-shot 
- Can be programmed in free running or periodic mode
- Stall feature supported via external stall input
*/
static volatile struct timer_s* const app_ss_app_timer = (struct timer_s*)(+0x49008000);
#ifdef __cplusplus
}
#endif

struct i2c_ss;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *I2C Interface Instance 3 - AC Control->
 *Here comes the decription of this IP component
*/
static volatile struct i2c_ss* const app_ss_i2c3 = (struct i2c_ss*)(+0x49009000);
#ifdef __cplusplus
}
#endif

struct pio_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Peripheral IO Control and ONFI Pad Calibration->
 *Peripheral IO Controller Module
*/
static volatile struct pio_s* const app_ss_pio_onfi_wrap_maps_pio = (struct pio_s*)(+0x4900A000);
#ifdef __cplusplus
}
#endif

struct pio_onfi_cal_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Peripheral IO Control and ONFI Pad Calibration->
 *Control for the Aragio ONFI Pad Calibration Cell and additional Aragio ONFI pad control
*/
static volatile struct pio_onfi_cal_s* const app_ss_pio_onfi_wrap_maps_pio_onfi_cal_1 = (struct pio_onfi_cal_s*)(+0x4900A800);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Peripheral IO Control and ONFI Pad Calibration->
 *Control for the Aragio ONFI Pad Calibration Cell and additional Aragio ONFI pad control
*/
static volatile struct pio_onfi_cal_s* const app_ss_pio_onfi_wrap_maps_pio_onfi_cal_2 = (struct pio_onfi_cal_s*)(+0x4900AC00);
#ifdef __cplusplus
}
#endif

struct otp_ctrl_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *One-Time-Programmable memory control Interface->
 *Here comes the decription of this IP component
*/
static volatile struct otp_ctrl_s* const app_ss_otp_ctrl = (struct otp_ctrl_s*)(+0x4900B000);
#ifdef __cplusplus
}
#endif

struct km_gnss_apps_tim_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *GNSS and Applications Timing module->
 *Klein Matterhorn GNSS and Applications Timing Assistance Module
*/
static volatile struct km_gnss_apps_tim_s* const app_ss_km_gnss_apps_tim = (struct km_gnss_apps_tim_s*)(+0x4900C000);
#ifdef __cplusplus
}
#endif

struct tdr_access_unit_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Test Data Register Acccess Gateway->
 *TDR Access Unit register block
*/
static volatile struct tdr_access_unit_s* const app_ss_tdr_access_unit = (struct tdr_access_unit_s*)(+0x4900D000);
#ifdef __cplusplus
}
#endif

struct uart_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Uart Interface 1->
 *Uart Module
*/
static volatile struct uart_s* const app_ss_app_uart1 = (struct uart_s*)(+0x49100000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Uart Interface 2->
 *Uart Module
*/
static volatile struct uart_s* const app_ss_app_uart2 = (struct uart_s*)(+0x49101000);
#ifdef __cplusplus
}
#endif

struct spi_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *SPI Interface 1->
 *SPI Control Registers
*/
static volatile struct spi_s* const app_ss_app_spi1 = (struct spi_s*)(+0x49102000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *SPI Interface 2->
 *SPI Control Registers
*/
static volatile struct spi_s* const app_ss_app_spi2 = (struct spi_s*)(+0x49103000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *I2C Interface Instance 1->
 *Here comes the decription of this IP component
*/
static volatile struct i2c_ss* const app_ss_i2c1 = (struct i2c_ss*)(+0x49105000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *I2C Interface Instance 2->
 *Here comes the decription of this IP component
*/
static volatile struct i2c_ss* const app_ss_i2c2 = (struct i2c_ss*)(+0x49106000);
#ifdef __cplusplus
}
#endif

struct i2s_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *I2S Interface->
 * cdn_i2ssc_addr_map
*/
static volatile struct i2s_s* const app_ss_i2s = (struct i2s_s*)(+0x49107000);
#ifdef __cplusplus
}
#endif

struct display_top_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Serial and Parallel Display Interface->
 *This is the Display IF register file.  This configurable engine can convey data to an external display using either a serial interface or i8080 parallel interface.
*/
static volatile struct display_top_s* const app_ss_disp_if = (struct display_top_s*)(+0x49108000);
#ifdef __cplusplus
}
#endif

struct pwm_top_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *PWM Interface->
 *This is the PWM Top register file description
*/
static volatile struct pwm_top_s* const app_ss_pwm = (struct pwm_top_s*)(+0x49109000);
#ifdef __cplusplus
}
#endif

struct tscu_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *Slow Clock Calibration Interface->
 *Time Control Unit 0 and 1 Control Interface
*/
static volatile struct tscu_s* const app_ss_scu = (struct tscu_s*)(+0x49200000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *MODEM MDM Processors Watchdog->
 *Watchdog IP block has the following features : 
- APB slave interface
- 16 bit down counters with 15 bit prescale (7 settings for prescale possible)
- Interrupt output generation on timeout
- Reset output generation on timeout with possibility of delayed reset
- Access keys to protect registers from being altered by software unintentionally
- Stall feature supported via external stall input
*/
static volatile struct wdg_s* const app_ss_mdm_wdg = (struct wdg_s*)(+0x49201000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *MODEM PHY Processors Watchdog->
 *Watchdog IP block has the following features : 
- APB slave interface
- 16 bit down counters with 15 bit prescale (7 settings for prescale possible)
- Interrupt output generation on timeout
- Reset output generation on timeout with possibility of delayed reset
- Access keys to protect registers from being altered by software unintentionally
- Stall feature supported via external stall input
*/
static volatile struct wdg_s* const app_ss_phy_wdg = (struct wdg_s*)(+0x49202000);
#ifdef __cplusplus
}
#endif

struct dmac_pl081_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *System DMA ARM PL081->
 *ARM PL081 Dma Controller
*/
static volatile struct dmac_pl081_s* const app_ss_sdma_pl081 = (struct dmac_pl081_s*)(+0x49400000);
#ifdef __cplusplus
}
#endif

struct usbhs_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *USB Core Interface->
 *USB Module
*/
static volatile struct usbhs_s* const app_ss_usb_ctrl = (struct usbhs_s*)(+0x49401000);
#ifdef __cplusplus
}
#endif

struct psram_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *PSRAM Control Interface->
 *PSRAM IP.  
This module implements an AXI3-compatible PSRAM controller for Winbond and ISSI PSRAM devices
*/
static volatile struct psram_s* const app_ss_psram_ctrl = (struct psram_s*)(+0x49600000);
#ifdef __cplusplus
}
#endif

struct arm_crypto310_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *ARM CryptoCell 310 Control Interface->
 * ARM CryptoCell 310 Register.  Taken from ARM TrustZone CryptoCell-310 Hardware Technical Reference Manual v1.2, dx_crys_kernel.h and dx_rng.h.  Please note that the 0x1000 offset seen in the documentation shall be added as part of the base address.
*/
static volatile struct arm_crypto310_s* const app_ss_cryptocell_310_ctrl = (struct arm_crypto310_s*)(+0x49601000);
#ifdef __cplusplus
}
#endif

struct sdio_host_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *SDIO Control Interface->
 * __ALL__
*/
static volatile struct sdio_host_s* const app_ss_sdio_ctrl = (struct sdio_host_s*)(+0x49602000);
#ifdef __cplusplus
}
#endif

struct nic400_app_ic_s;
#ifdef __cplusplus
namespace device {
#endif
/** Applications Subsystem Control->
 *NIC GPV control registers
*/
static volatile struct nic400_app_ic_s* const app_ss_app_ic_ctrl = (struct nic400_app_ic_s*)(+0x49900000);
#ifdef __cplusplus
}
#endif

struct vcu_cr_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *VSP0 Control Registers->
 *VSP->
 *VSP Control Unit
*/
static volatile struct vcu_cr_s* const bb_modem_ss_vsp0_vcu = (struct vcu_cr_s*)(+0x40000000);
#ifdef __cplusplus
}
#endif

struct vdma_cr_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *VSP0 Control Registers->
 *VDMA Control Registers->
 *VDMA Control Registers.
The Vector DMA Controller (VDMA) is used for memory to memory data transfers within the VSP memory system. The VDMA can transfer data to/from any memory, either private or shared, in the VSP memory system. Example use cases for the VDMA are:
   - The instruction code for AR1DE tasks might be located in the shared memory. In this case, the instruction code must be copied to the AR1DEs private instruction memory before the task can be executed.
   - Data copy and move within the VSP memory system, e.g. from the shared memory to a private data memory or between the private data memory of one AR1DE to the private data memory of the other AR1DE.

*/
static volatile struct vdma_cr_s* const bb_modem_ss_vsp0_vdma0 = (struct vdma_cr_s*)(+0x40002000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *VSP0 Control Registers->
 *VDMA Control Registers->
 *VDMA Control Registers.
The Vector DMA Controller (VDMA) is used for memory to memory data transfers within the VSP memory system. The VDMA can transfer data to/from any memory, either private or shared, in the VSP memory system. Example use cases for the VDMA are:
   - The instruction code for AR1DE tasks might be located in the shared memory. In this case, the instruction code must be copied to the AR1DEs private instruction memory before the task can be executed.
   - Data copy and move within the VSP memory system, e.g. from the shared memory to a private data memory or between the private data memory of one AR1DE to the private data memory of the other AR1DE.

*/
static volatile struct vdma_cr_s* const bb_modem_ss_vsp0_vdma1 = (struct vdma_cr_s*)(+0x40003000);
#ifdef __cplusplus
}
#endif

struct ar1de_cr_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *VSP0 Control Registers->
 *DE0->
 *Control Interface.
\par The register map consists of the following regions:
   - Component configuration: These registers enable the identification of system components by software and are standard Primecell Peripheral Component Identification Registers.
   - Event Fault control: These registers control events and exception faults behavior in the AR1DE
   - Task descriptor fifo queue configuration: These registers configure and start tasks on the OptimoDE data engine.
	
\par The control interface register map can be divided into two classes of registers based upon the access methodology provided.
   - Task dependent configuration registers.  These are implemented using a fifo queue of depth 2.  The top head element of the queue is allocated to the AHB-lite control interface, whereas the bottom tail element of the queue is allocated to the AR1DPU (and can only be observed, but not written, from the AHB-lite control interface).  This ensures that the next task to be run on the AR1DE can already be setup when the current task is still executing.
   - Task independent configuration registers.  These are implemented as a single register inside the AR1DE.  Writing to these registers will have an immediate effect.
\par Task Control
Starting a task on the data engine is done by writing a task descriptor into the task buffer. Note that only the PENDENTRY field is mandatory.
   - PENDENTRY is a 32-bit field containing the start address of the task in instruction memory. This offset is a byte offset. However because AR1DE cannot access instruction memory on a byte address resolution, the lower bits are ignored by AR1DE.
   - PENDGP0, PENDGP1, PENDGP2 and PENDGP3 are 32-bit general purpose registers. It is up to the programmer to use them to pass arguments to the task to be run or ignore them. They are typically used to pass stack pointers, frame pointers, constant table pointers etc.

When the AR1DE is idle and a task descriptor is committed to the task buffer by writing the PENDENTRY register:
   - The task will be started
   - The content of the pending task general purpose registers is shifted into the running task general purpose registers to make them available to the AR1DE.

The pending task buffer now becomes empty and thus a next task can be uploaded while the previous task is still running.
\par Starting a task
First a task descriptor has to be loaded into the task buffer. A task descriptor is loaded into the task buffer when a value is written in the PENDENTRY register. Loading a task descriptor sets the task pending flag in the PENDSTATUS register. If the AR1DPU is not busy, the task will be started. This clears the task pending flag of the PENDSTATUS register. The task buffer is again available for loading a next task descriptor. Note that there is no mechanism that prevents from overriding the top element of the queue.
An ARM CPU can use Store Multiple instructions (STM, STMIA)bursts to load a task into the task buffer. It should first write the general purpose fields (if present) and end with the PENDENTRY field - since a write to this register sets the task pending flag in the PENDSTATUS register.
\par Clearing the pending task
It is possible the clear the task buffer by writing to the clear pending task flag in the PENDCTRL register. This clears the task pending flag in the PENDSTATUS register. It is strongly advised to first suspend the queue by writing to the suspend pending task flag in the PENDCTRL register. As such you can first check if the task queue contains a pending task descriptor without the data engine consuming it in the meantime. Please note that you don't have to clear the task valid flag in the PENDSTATUS register when you want to replace the top element of the queue. The fastest way to replace the top element of the queue is as follows:
   - Disable the task buffer by writing the suspend pending task flag in the PENDCTRL register
   - Check if the top task buffer contains an element by reading the task pending flag in the PENDSTATUS register.
   - Load a new task descriptor.
   - Enable the task buffer by clearing the suspend pending task flag in the PENDCTRL register.
\par Aborting a running task
Writing to the running task abort flag in the TASKCTRL register will reset the AR1DE. This might take a variable number of cycles, due to the fact that outstanding transactions (shared memory, AXI fabric) may need to be completed first.  The task abort busy flag in the TASKSTATUS register indicates if the reset has completed.  Note that aborting a task does not implicitly remove the pending task from the queue. If there is a pending task after abort is done, it will be automatically started.

*/
static volatile struct ar1de_cr_s* const bb_modem_ss_vsp0_de0_cr = (struct ar1de_cr_s*)(+0x40004000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *VSP0 Control Registers->
 *DE1->
 *Control Interface.
\par The register map consists of the following regions:
   - Component configuration: These registers enable the identification of system components by software and are standard Primecell Peripheral Component Identification Registers.
   - Event Fault control: These registers control events and exception faults behavior in the AR1DE
   - Task descriptor fifo queue configuration: These registers configure and start tasks on the OptimoDE data engine.
	
\par The control interface register map can be divided into two classes of registers based upon the access methodology provided.
   - Task dependent configuration registers.  These are implemented using a fifo queue of depth 2.  The top head element of the queue is allocated to the AHB-lite control interface, whereas the bottom tail element of the queue is allocated to the AR1DPU (and can only be observed, but not written, from the AHB-lite control interface).  This ensures that the next task to be run on the AR1DE can already be setup when the current task is still executing.
   - Task independent configuration registers.  These are implemented as a single register inside the AR1DE.  Writing to these registers will have an immediate effect.
\par Task Control
Starting a task on the data engine is done by writing a task descriptor into the task buffer. Note that only the PENDENTRY field is mandatory.
   - PENDENTRY is a 32-bit field containing the start address of the task in instruction memory. This offset is a byte offset. However because AR1DE cannot access instruction memory on a byte address resolution, the lower bits are ignored by AR1DE.
   - PENDGP0, PENDGP1, PENDGP2 and PENDGP3 are 32-bit general purpose registers. It is up to the programmer to use them to pass arguments to the task to be run or ignore them. They are typically used to pass stack pointers, frame pointers, constant table pointers etc.

When the AR1DE is idle and a task descriptor is committed to the task buffer by writing the PENDENTRY register:
   - The task will be started
   - The content of the pending task general purpose registers is shifted into the running task general purpose registers to make them available to the AR1DE.

The pending task buffer now becomes empty and thus a next task can be uploaded while the previous task is still running.
\par Starting a task
First a task descriptor has to be loaded into the task buffer. A task descriptor is loaded into the task buffer when a value is written in the PENDENTRY register. Loading a task descriptor sets the task pending flag in the PENDSTATUS register. If the AR1DPU is not busy, the task will be started. This clears the task pending flag of the PENDSTATUS register. The task buffer is again available for loading a next task descriptor. Note that there is no mechanism that prevents from overriding the top element of the queue.
An ARM CPU can use Store Multiple instructions (STM, STMIA)bursts to load a task into the task buffer. It should first write the general purpose fields (if present) and end with the PENDENTRY field - since a write to this register sets the task pending flag in the PENDSTATUS register.
\par Clearing the pending task
It is possible the clear the task buffer by writing to the clear pending task flag in the PENDCTRL register. This clears the task pending flag in the PENDSTATUS register. It is strongly advised to first suspend the queue by writing to the suspend pending task flag in the PENDCTRL register. As such you can first check if the task queue contains a pending task descriptor without the data engine consuming it in the meantime. Please note that you don't have to clear the task valid flag in the PENDSTATUS register when you want to replace the top element of the queue. The fastest way to replace the top element of the queue is as follows:
   - Disable the task buffer by writing the suspend pending task flag in the PENDCTRL register
   - Check if the top task buffer contains an element by reading the task pending flag in the PENDSTATUS register.
   - Load a new task descriptor.
   - Enable the task buffer by clearing the suspend pending task flag in the PENDCTRL register.
\par Aborting a running task
Writing to the running task abort flag in the TASKCTRL register will reset the AR1DE. This might take a variable number of cycles, due to the fact that outstanding transactions (shared memory, AXI fabric) may need to be completed first.  The task abort busy flag in the TASKSTATUS register indicates if the reset has completed.  Note that aborting a task does not implicitly remove the pending task from the queue. If there is a pending task after abort is done, it will be automatically started.

*/
static volatile struct ar1de_cr_s* const bb_modem_ss_vsp0_de1_cr = (struct ar1de_cr_s*)(+0x40005000);
#ifdef __cplusplus
}
#endif

struct rfdma_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *RFDMA control registers->
 *RFDMA control registers
*/
static volatile struct rfdma_s* const bb_modem_ss_mdm_hwacc_rfdma = (struct rfdma_s*)(+0x40008000);
#ifdef __cplusplus
}
#endif

struct smurf_if_hwacc_config_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *BB-RF Phy control registers + Hwacc config extra registers->
 *Simple MUltichannel RF interface (SMURF).  The SMURF is the physical interface between the baseband and the transceiver and consists of 2 separate components that need to be used in pairs. This HAL contains extra registers specific for KM Modem HWAcc.
*/
static volatile struct smurf_if_hwacc_config_s* const bb_modem_ss_mdm_hwacc_bbphy = (struct smurf_if_hwacc_config_s*)(+0x40009000);
#ifdef __cplusplus
}
#endif

struct dl_bpe_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *DownLink BitProcessing control registers->
 *Downlink bit processing engine control registers.
*/
static volatile struct dl_bpe_s* const bb_modem_ss_mdm_hwacc_dlbpe = (struct dl_bpe_s*)(+0x4000A000);
#ifdef __cplusplus
}
#endif

struct km_ul_bpe_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *UpLink BitProcessing control registers->
 *This is the KM UL BPE register module
*/
static volatile struct km_ul_bpe_s* const bb_modem_ss_mdm_hwacc_ulbpe = (struct km_ul_bpe_s*)(+0x4000B000);
#ifdef __cplusplus
}
#endif

struct km_ofdm_demux_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *OFDM DeMux control registers->
 *This is the OFDM register module
*/
static volatile struct km_ofdm_demux_s* const bb_modem_ss_mdm_hwacc_ofdmdm = (struct km_ofdm_demux_s*)(+0x4000C000);
#ifdef __cplusplus
}
#endif

struct km_fbuf_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *Circular Buffer control registers->
 *Register module for the Klein Matterhorn LTE Subsystem Front End Buffer (Fbuf)
*/
static volatile struct km_fbuf_s* const bb_modem_ss_mdm_hwacc_cirbuf = (struct km_fbuf_s*)(+0x4000D000);
#ifdef __cplusplus
}
#endif

struct km_pss_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *PSS control registers->
 *This is the PSS register module
*/
static volatile struct km_pss_s* const bb_modem_ss_mdm_hwacc_pss = (struct km_pss_s*)(+0x4000E000);
#ifdef __cplusplus
}
#endif

struct km_sss_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *SSS control registers->
 *This is the SSS register module
*/
static volatile struct km_sss_s* const bb_modem_ss_mdm_hwacc_sss = (struct km_sss_s*)(+0x4000F000);
#ifdef __cplusplus
}
#endif

struct usim_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *USIM->
 *Smart Card Reader control registers.
*/
static volatile struct usim_s* const bb_modem_ss_usim = (struct usim_s*)(+0x40010000);
#ifdef __cplusplus
}
#endif

struct ipc_mbx_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *Mailbox->
 *IPC Mailbox
*/
static volatile struct ipc_mbx_s* const bb_modem_ss_ipc_mbx = (struct ipc_mbx_s*)(+0x41001000);
#ifdef __cplusplus
}
#endif

struct hwsequencer_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *Hardware Sequencer->
 *Hardware Sequencer
*/
static volatile struct hwsequencer_s* const bb_modem_ss_hw_seq = (struct hwsequencer_s*)(+0x48000000);
#ifdef __cplusplus
}
#endif

struct tcu_seqfifo_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *TCU Sequence Fifos->
 *Timping Control Unit Sequence FIFOs
*/
static volatile struct tcu_seqfifo_s* const bb_modem_ss_tcuseqfifos = (struct tcu_seqfifo_s*)(+0x48010000);
#ifdef __cplusplus
}
#endif

struct nic400_modem_ic_s;
#ifdef __cplusplus
namespace device {
#endif
/** Baseband MODEM Subsystem Control->
 *Modem Subsystem NIC GPV control registers
*/
static volatile struct nic400_modem_ic_s* const bb_modem_ss_modem_ic_ctrl = (struct nic400_modem_ic_s*)(+0x49A00000);
#ifdef __cplusplus
}
#endif

struct rx_apb_mmap_rxadc_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS Rx1 DigIF control Registers->
 *This is the digital receiver path control register description
*/
static volatile struct rx_apb_mmap_rxadc_s* const rf_ana_ss_rf_rx1_digif_rx_apb_mmap_rxadc_maps = (struct rx_apb_mmap_rxadc_s*)(+0x44003400);
#ifdef __cplusplus
}
#endif

struct rx_apb_mmap_rxrf_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS Rx1 DigIF control Registers->
 *This is the digital receiver path control register description
*/
static volatile struct rx_apb_mmap_rxrf_s* const rf_ana_ss_rf_rx1_digif_rx_apb_mmap_rxrf_maps = (struct rx_apb_mmap_rxrf_s*)(+0x44003800);
#ifdef __cplusplus
}
#endif

struct rx_apb_mmap_rxbb_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS Rx1 DigIF control Registers->
 *This is the digital receiver path control register description
*/
static volatile struct rx_apb_mmap_rxbb_s* const rf_ana_ss_rf_rx1_digif_rx_apb_mmap_rxbb_maps = (struct rx_apb_mmap_rxbb_s*)(+0x44003C00);
#ifdef __cplusplus
}
#endif

struct tx_apb_mmap_txrf_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS Tx1 DigIF control Registers->
 *This is the tx rf line-up register description
*/
static volatile struct tx_apb_mmap_txrf_s* const rf_ana_ss_rf_tx1_digif_tx_apb_mmap_txrf_maps = (struct tx_apb_mmap_txrf_s*)(+0x44007400);
#ifdef __cplusplus
}
#endif

struct tx_apb_mmap_txbb_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS Tx1 DigIF control Registers->
 *This is the tx base band line-up register description
*/
static volatile struct tx_apb_mmap_txbb_s* const rf_ana_ss_rf_tx1_digif_tx_apb_mmap_txbb_maps = (struct tx_apb_mmap_txbb_s*)(+0x44007600);
#ifdef __cplusplus
}
#endif

struct pll_dig_if_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS PLL1 control Registers->
 *This is the synthesizer register description
*/
static volatile struct pll_dig_if_s* const rf_ana_ss_rf_pll1 = (struct pll_dig_if_s*)(+0x4400A000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS PLL2 control Registers->
 *This is the synthesizer register description
*/
static volatile struct pll_dig_if_s* const rf_ana_ss_rf_pll2 = (struct pll_dig_if_s*)(+0x4400B000);
#ifdef __cplusplus
}
#endif

struct aux_digif_apb_mmap_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Analog Subsystem Control->
 *RF SS Auxiliary DigIF control Registers->
 *This is the Aux ADC control register description
*/
static volatile struct aux_digif_apb_mmap_s* const rf_ana_ss_rf_aux_digif = (struct aux_digif_apb_mmap_s*)(+0x4400D000);
#ifdef __cplusplus
}
#endif

struct km_go_config_regs_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RF Configuration registers->
 *KM GO RF Digital Configuration Registers
*/
static volatile struct km_go_config_regs_s* const rf_dig_ss_km_go_config_regs = (struct km_go_config_regs_s*)(+0x44000000);
#ifdef __cplusplus
}
#endif

struct kmgo_rf_dig_ccu_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RF Clock control Unit (CCU) Interface->
 *RF Digital Clock Control Unit for Gottardo and Klein Matterhorn
*/
static volatile struct kmgo_rf_dig_ccu_s* const rf_dig_ss_kmgo_rf_dig_ccu = (struct kmgo_rf_dig_ccu_s*)(+0x44001000);
#ifdef __cplusplus
}
#endif

struct rx_dig_top_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RX Digital Front-End (DFE)->
 *This is the digital receiver path control register description
*/
static volatile struct rx_dig_top_s* const rf_dig_ss_rx_dig_top = (struct rx_dig_top_s*)(+0x44002000);
#ifdef __cplusplus
}
#endif

struct tx_dig_top_txdfe1_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *TX Digital Front-End (DFE)->
 *This is the tx dfe part 1 line-up register description
*/
static volatile struct tx_dig_top_txdfe1_s* const rf_dig_ss_tx_dig_top_tx_dig_top_txdfe1 = (struct tx_dig_top_txdfe1_s*)(+0x44006000);
#ifdef __cplusplus
}
#endif

struct tx_dig_top_txdfe2_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *TX Digital Front-End (DFE)->
 *This is the tx dfe part 2 line-up register description
*/
static volatile struct tx_dig_top_txdfe2_s* const rf_dig_ss_tx_dig_top_tx_dig_top_txdfe2 = (struct tx_dig_top_txdfe2_s*)(+0x44006200);
#ifdef __cplusplus
}
#endif

struct aux_dig_top_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *AUX Digital Front-End (DFE)->
 *This is the measurement receiver path control register description
*/
static volatile struct aux_dig_top_s* const rf_dig_ss_aux_dig_top = (struct aux_dig_top_s*)(+0x4400C000);
#ifdef __cplusplus
}
#endif

struct rffe_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RF Front-End (RFFE)->
 *MIPI RFFE v1.10a Interface
*/
static volatile struct rffe_s* const rf_dig_ss_rf_rffe = (struct rffe_s*)(+0x4400E000);
#ifdef __cplusplus
}
#endif

struct ic_rx_scratch_ram_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RF RX scratchpad->
 *Rx ScratchPad Control Registers
*/
static volatile struct ic_rx_scratch_ram_s* const rf_dig_ss_rx_scratchpad = (struct ic_rx_scratch_ram_s*)(+0x44010000);
#ifdef __cplusplus
}
#endif

struct ic_tx_scratch_ram_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RF TX scratchpad->
 *Tx ScratchPad Control Registers
*/
static volatile struct ic_tx_scratch_ram_s* const rf_dig_ss_tx_scratchpad = (struct ic_tx_scratch_ram_s*)(+0x44011000);
#ifdef __cplusplus
}
#endif

struct sch_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RF Scheduler registers->
 *This is the Breithorn Scheduler module register interface
*/
static volatile struct sch_s* const rf_dig_ss_rf_sch = (struct sch_s*)(+0x44012000);
#ifdef __cplusplus
}
#endif

struct ts_mgr_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *RF Timestamp Manager->
 *RTC and Timestamp Manager
*/
static volatile struct ts_mgr_s* const rf_dig_ss_rf_ts_mgr = (struct ts_mgr_s*)(+0x44013000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *Watchdog Timer->
 *Watchdog IP block has the following features : 
- APB slave interface
- 16 bit down counters with 15 bit prescale (7 settings for prescale possible)
- Interrupt output generation on timeout
- Reset output generation on timeout with possibility of delayed reset
- Access keys to protect registers from being altered by software unintentionally
- Stall feature supported via external stall input
*/
static volatile struct wdg_s* const rf_dig_ss_rf_wdg = (struct wdg_s*)(+0x44014000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *Dual Timer->
 *TIMER IP.  
This module has the following features : 
- APB slave interface
- 32 bit down counters with 3 different settings of 8-bit prescale counter 
- Timers programmable as 32 or 16 bit counters
- Timers can run continuously or for single-shot 
- Can be programmed in free running or periodic mode
- Stall feature supported via external stall input
*/
static volatile struct timer_s* const rf_dig_ss_rf_timer = (struct timer_s*)(+0x44015000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *Uart Control Interface->
 *Uart Module
*/
static volatile struct uart_s* const rf_dig_ss_rf_uart = (struct uart_s*)(+0x44016000);
#ifdef __cplusplus
}
#endif

struct smurf_if_1ch_s;
#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *BB-RF Data Interface->
 *Simple MUltichannel RF interface (SMURF).  The SMURF is the physical interface between the baseband and the transceiver and consists of 2 separate components that need to be used in pairs.
*/
static volatile struct smurf_if_1ch_s* const rf_dig_ss_rf_smurf = (struct smurf_if_1ch_s*)(+0x44018000);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace device {
#endif
/** RF Digital Subsystem Control->
 *Inter-process communication mailbox->
 *IPC Mailbox
*/
static volatile struct ipc_mbx_s* const rf_dig_ss_rf_ipc_mbx = (struct ipc_mbx_s*)(+0x44100000);
#ifdef __cplusplus
}
#endif

/* EOF hal_device.h */
#endif
