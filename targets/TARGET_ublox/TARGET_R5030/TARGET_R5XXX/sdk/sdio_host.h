#ifndef INCLUDED_SDIO_HOST
#define INCLUDED_SDIO_HOST
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** sdio_host HAL Spreadsheet version number */
#define SDIO_HOST_HAL_VERSION 1

/**  __ALL__
*/
struct sdio_host_s {
   /** HRS00 - General Information Register at address offset 0x000, read-write */
   uint32_t hrs00;
   /** HRS01 - Debounce Setting Register at address offset 0x004, read-write */
   uint32_t hrs01;
   /** HRS02 - Bus Setting Register at address offset 0x008, read-write */
   uint32_t hrs02;
   /** HRS03 - AXI ERROR Responses Register These registers extend the standard set of SD-HOST interrupt statuses by information about AXI interface exceptions. The registers are divided into three groups:  - Signal Enable registers allow to enable/mask signalling the Interrupt Status registers (HRS03[3:0]) on interrupt port  - Status Enable registers allow to enable/disable interrupt source for each Interrupt Status separately  - Interrupt Status are triggered whenever the interrupt source is detected and the Status Enable register is enabled at address offset 0x00C, read-write */
   uint32_t hrs03;
   /** HRS04 - UHS-I PHY Settings The HRS04 register is an interface for register delay line control implemented in the UHS-I PHY. The purpose of the register delay line is to adjust a window of received data transferred through CMD/DAT to the fixed sampling point. Each mode has its own configuration register (except modes which use tuning, i.e. UHS-I SDR104, eMMC HS200, eMMC HS400 ). If the mode is switched, the delay line settings are updated with respective value. 
       The setting registers are mapped by UIS_ADDR as follows: 0000b - High Speed 0001b - Default Speed 0010b - UHS-I SDR12 0011b - UHS-I SDR25 0100b - UHS-I SDR50 0101b - UHS-I DDR50 0110b - MMC Legacy 0111b - MMC SDR 1000b - MMC DDR others - Reserved Through the registers, the user can read or update specific setting register. In case of write, before setting write request the address is to be set in UIS_ADDR and write data in UIS_ADDR. After that UIS_WR (write request) is set to 1, the value from the UIS_WDATA is written to the setting register pointed by UIS_ADDR. The operation is completed when UIS_ACK is set. In case of read, when UIS_RD is set to 1, the setting register addressed by UIS_ADDR is available for read in UIS_RDATA when UIS_ACK is set to 1. If the read/write operation is completed the UIS_RD and UIS_WR will be released (set to 0) and the UIS_ACK need to be 0 before new operation. at address offset 0x010, read-write */
   uint32_t hrs04;
   /** HRS05 - UHS-I PHY BIST Control/Status at address offset 0x014, read-write */
   uint32_t hrs05;
   /** HRS06 - eMMC control registers at address offset 0x018, read-write */
   uint32_t hrs06;
   /** Reserved space */
   uint8_t fill0[100];
   /** HRS32 - FSM Monitor Register at address offset 0x080, read-write */
   uint32_t hrs32;
   /** HRS33 - Tune Status 0 Register at address offset 0x084, read-only */
   uint32_t hrs33;
   /** HRS34 - Tune Status 1 Register at address offset 0x088, read-only */
   uint32_t hrs34;
   /** HRS35 - Tune Debug Register at address offset 0x08C, read-write */
   uint32_t hrs35;
   /** HRS36 - Boot Status Register at address offset 0x090, read-only */
   uint32_t hrs36;
   /** HRS37 - Read bLock gap coefficient interface mode select at address offset 0x094, read-write */
   uint32_t hrs37;
   /** HRS38 - Read bLock gap coefficient at address offset 0x098, read-write */
   uint32_t hrs38;
   /** Reserved space */
   uint8_t fill1[80];
   /** HRS59 - Generic register at address offset 0x0EC, read-write */
   uint32_t hrs59;
   /** Reserved space */
   uint8_t fill2[12];
   /** CRS63 - Host Controller Version/Slot Interrupt Status at address offset 0x0FC, read-only */
   uint32_t crs63;
   /** Reserved space */
   uint8_t fill3[256];
   /** SRS00 - SDMA System Address / Argument 2 at address offset 0x200, read-write */
   uint32_t srs00;
   /** SRS01 - Block Size / Block Count at address offset 0x204, read-write */
   uint32_t srs01;
   /** SRS02 - Argument 1 at address offset 0x208, read-write */
   uint32_t srs02;
   /** SRS03 - Command/Transfer Mode at address offset 0x20C, read-write */
   uint32_t srs03;
   /** The SRS04 - SRS07 registers store the response returned by the card. The mapping of the actual device response and the SRS04 - SRS07 contents depends on the type of response. The type of response is determined by the RTS field (Response Type) for all user-defined commands. The separate cases are the Auto-CMD12 response (called R1b in the SD Memory Specification) and Auto-CMD23 response (called R1 in the SD Memory Specification). Auto-CMD12 and Auto-CMD23 responses are handled by the core automatically and goes to the SRS07 register regardless of the RTS value. SRS04-SRS07 relation to received response field: Auto-CMD12 resp.  Response field R[39:8] - RESP3[31:0] Auto-CMD23 resp.  Response field R[39:8] - RESP3[31:0] No response  RTS=00b 136-bit  RTS=01b, Response field R[127:8] - {RESP3[23:0], RESP2[31:0], RESP1[31:0], RESP0[31:0]} 48-bit  RTS=10b, Response field R[39:8] - RESP1[31:0] 48-bit with BUSY  RTS=11b, Response field R[39:8] - RESP1[31:0] 
       In UHS-II mode, these registers contains responses to the CM-TRAN abort CCMD and the SDTRAN abort CCMD. The CM-TRAN abort CCMD response is 4 bytes wide and it is located in SRS04. The SD-TRAN abort CCMD response is 8 bytes wide and it is located in SRS07 and SRS06. In UHS-II mode, the byte order of used registers is programmable and depends on value of HRS59.U2CRE. Implementation note: Registers value are undefined after reset, and will be valid after response is received. at address offset 0x210, read-only */
   uint32_t srs04;
   /** The SRS04 - SRS07 registers store the response returned by the card. The mapping of the actual device response and the SRS04 - SRS07 contents depends on the type of response. The type of response is determined by the RTS field (Response Type) for all user-defined commands. The separate cases are the Auto-CMD12 response (called R1b in the SD Memory Specification) and Auto-CMD23 response (called R1 in the SD Memory Specification). Auto-CMD12 and Auto-CMD23 responses are handled by the core automatically and goes to the SRS07 register regardless of the RTS value. SRS04-SRS07 relation to received response field: Auto-CMD12 resp.  Response field R[39:8] - RESP3[31:0] Auto-CMD23 resp.  Response field R[39:8] - RESP3[31:0] No response  RTS=00b 136-bit  RTS=01b, Response field R[127:8] - {RESP3[23:0], RESP2[31:0], RESP1[31:0], RESP0[31:0]} 48-bit  RTS=10b, Response field R[39:8] - RESP1[31:0] 48-bit with BUSY  RTS=11b, Response field R[39:8] - RESP1[31:0] 
       In UHS-II mode, these registers contains responses to the CM-TRAN abort CCMD and the SDTRAN abort CCMD. The CM-TRAN abort CCMD response is 4 bytes wide and it is located in SRS04. The SD-TRAN abort CCMD response is 8 bytes wide and it is located in SRS07 and SRS06. In UHS-II mode, the byte order of used registers is programmable and depends on value of HRS59.U2CRE. Implementation note: Registers value are undefined after reset, and will be valid after response is received. at address offset 0x214, read-only */
   uint32_t srs05;
   /** The SRS04 - SRS07 registers store the response returned by the card. The mapping of the actual device response and the SRS04 - SRS07 contents depends on the type of response. The type of response is determined by the RTS field (Response Type) for all user-defined commands. The separate cases are the Auto-CMD12 response (called R1b in the SD Memory Specification) and Auto-CMD23 response (called R1 in the SD Memory Specification). Auto-CMD12 and Auto-CMD23 responses are handled by the core automatically and goes to the SRS07 register regardless of the RTS value. SRS04-SRS07 relation to received response field: Auto-CMD12 resp.  Response field R[39:8] - RESP3[31:0] Auto-CMD23 resp.  Response field R[39:8] - RESP3[31:0] No response  RTS=00b 136-bit  RTS=01b, Response field R[127:8] - {RESP3[23:0], RESP2[31:0], RESP1[31:0], RESP0[31:0]} 48-bit  RTS=10b, Response field R[39:8] - RESP1[31:0] 48-bit with BUSY  RTS=11b, Response field R[39:8] - RESP1[31:0] 
       In UHS-II mode, these registers contains responses to the CM-TRAN abort CCMD and the SDTRAN abort CCMD. The CM-TRAN abort CCMD response is 4 bytes wide and it is located in SRS04. The SD-TRAN abort CCMD response is 8 bytes wide and it is located in SRS07 and SRS06. In UHS-II mode, the byte order of used registers is programmable and depends on value of HRS59.U2CRE. Implementation note: Registers value are undefined after reset, and will be valid after response is received. at address offset 0x218, read-only */
   uint32_t srs06;
   /** The SRS04 - SRS07 registers store the response returned by the card. The mapping of the actual device response and the SRS04 - SRS07 contents depends on the type of response. The type of response is determined by the RTS field (Response Type) for all user-defined commands. The separate cases are the Auto-CMD12 response (called R1b in the SD Memory Specification) and Auto-CMD23 response (called R1 in the SD Memory Specification). Auto-CMD12 and Auto-CMD23 responses are handled by the core automatically and goes to the SRS07 register regardless of the RTS value. SRS04-SRS07 relation to received response field: Auto-CMD12 resp.  Response field R[39:8] - RESP3[31:0] Auto-CMD23 resp.  Response field R[39:8] - RESP3[31:0] No response  RTS=00b 136-bit  RTS=01b, Response field R[127:8] - {RESP3[23:0], RESP2[31:0], RESP1[31:0], RESP0[31:0]} 48-bit  RTS=10b, Response field R[39:8] - RESP1[31:0] 48-bit with BUSY  RTS=11b, Response field R[39:8] - RESP1[31:0] 
       In UHS-II mode, these registers contains responses to the CM-TRAN abort CCMD and the SDTRAN abort CCMD. The CM-TRAN abort CCMD response is 4 bytes wide and it is located in SRS04. The SD-TRAN abort CCMD response is 8 bytes wide and it is located in SRS07 and SRS06. In UHS-II mode, the byte order of used registers is programmable and depends on value of HRS59.U2CRE. Implementation note: Registers value are undefined after reset, and will be valid after response is received. at address offset 0x21C, read-only */
   uint32_t srs07;
   /** SRS08 - Data Buffer at address offset 0x220, read-write */
   uint32_t srs08;
   /** SRS09 - Present State Register at address offset 0x224, read-only */
   uint32_t srs09;
   /** SRS10 - Host Control 1 (General / Power / Block-Gap / Wake-Up) at address offset 0x228, read-write */
   uint32_t srs10;
   /** SRS11 - Host Control 2 (Clock, Timeout, Reset) at address offset 0x22C, read-write */
   uint32_t srs11;
   /** SRS12 - Error/Normal Interrupt Status at address offset 0x230, read-write */
   uint32_t srs12;
   /** SRS13 - Error/Normal Status Enable at address offset 0x234, read-write */
   uint32_t srs13;
   /** SRS14 - Error/Normal Signal Enable at address offset 0x238, read-write */
   uint32_t srs14;
   /** SRS15 - Host Control #2 / Auto CMD Error Status at address offset 0x23C, read-write */
   uint32_t srs15;
   /** SRS16 - Capabilities #1 HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x240, read-only */
   uint32_t srs16;
   /** SRS17 - Capabilities #2 HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x244, read-only */
   uint32_t srs17;
   /** SRS18 - Capabilities #3 HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x248, read-only */
   uint32_t srs18;
   /** SRS19 - Capabilities #4 HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x24C, read-only */
   uint32_t srs19;
   /** SRS20 - Force Event Each field of this register is related to the specific error status. Writing 1 to field will set the status error. This function is provided for SW debug purpose. at address offset 0x250, write-only */
   uint32_t srs20;
   /** SRS21 - ADMA Error Status at address offset 0x254, read-only */
   uint32_t srs21;
   /** SRS22  ADMA/SDMA System Address 1 at address offset 0x258, read-write */
   uint32_t srs22;
   /** SRS23 ADMA/SDMA System Address 2 at address offset 0x25C, read-write */
   uint32_t srs23;
   /** SRS24 - Preset Value (Default Speed) SRS24[31:16] - Default Speed if: SRS15.U2IE=0 SRS15.V18SE=0 SRS10.HSE=0 HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x260, read-only */
   uint32_t srs24;
   /** SRS25 - Preset Value (High Speed and SDR12) SRS25[15:0] - High Speed if: SRS15.U2IE=0 SRS15.V18SE=0 SRS.HSE=1 SRS25[31:16] - SDR12 if: SRS15.U2IE=0 SRS15.V18SE=1 SRS15.UMS=000b HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x264, read-only */
   uint32_t srs25;
   /** SRS26 - Preset Value (SDR25 and SDR50) SRS26[15:0] - SDR25 if: SRS15.U2IE=0 SRS15.V18SE=1 SRS15.UMS=001b SRS26[31:16] - SDR50 if: SRS15.U2IE=0 SRS15.V18SE=1 SRS15.UMS=010b HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x268, read-only */
   uint32_t srs26;
   /** SRS27 - Preset Value (SDR104 and DDR50) SRS27[15:0] - SDR104 if: SRS15.U2IE=0 SRS15.V18SE=1 SRS15.UMS=011b SRS27[31:16] - DDR50 if: SRS15.U2IE=0 SRS15.V18SE=1 SRS15.UMS=100b HWINIT Register - Note this register is hardware initialized after reset and the value read back will match the IP configuration. at address offset 0x26C, read-only */
   uint32_t srs27;
   /** Reserved space */
   uint8_t fill4[400];
   /** CQRS00 - Command Queuing Version at address offset 0x400, read-only */
   uint32_t cqrs00;
   /** CQRS01 - Command Queuing Capabilities at address offset 0x404, read-only */
   uint32_t cqrs01;
   /** CQRS02 - Command Queuing Configuration at address offset 0x408, read-write */
   uint32_t cqrs02;
   /** CQRS03 - Command Queuing Control at address offset 0x40C, read-write */
   uint32_t cqrs03;
   /** CQRS04 - Command Queuing Interrupt Status at address offset 0x410, read-write */
   uint32_t cqrs04;
   /** CQRS05 - Command Queuing Interrupt Status Enable at address offset 0x414, read-write */
   uint32_t cqrs05;
   /** CQRS06 - Command Queuing Interrupt Signal Enable at address offset 0x418, read-write */
   uint32_t cqrs06;
   /** CQRS07 - Interrupt Coalescing at address offset 0x41C, read-write */
   uint32_t cqrs07;
   /** CQRS08 - Command Queuing Task Descriptor List Base Address at address offset 0x420, read-write */
   uint32_t cqrs08;
   /** CQRS09 - Command Queuing Task Descriptor List Base Address Upper 32 Bits at address offset 0x424, read-write */
   uint32_t cqrs09;
   /** CQRS10 - Command Queuing Task Doorbell at address offset 0x428, read-write */
   uint32_t cqrs10;
   /** CQRS11 - Task Complete Notification at address offset 0x42C, read-write */
   uint32_t cqrs11;
   /** CQRS12 - Device Queue Status at address offset 0x430, read-only */
   uint32_t cqrs12;
   /** CQRS13 - Device Pending Tasks at address offset 0x434, read-only */
   uint32_t cqrs13;
   /** CQRS14 - Task Clear at address offset 0x438, read-write */
   uint32_t cqrs14;
   /** Reserved space */
   uint8_t fill5[4];
   /** CQRS16 - Send Status Configuration 1 at address offset 0x440, read-write */
   uint32_t cqrs16;
   /** CQRS17 - Send Status Configuration 2 at address offset 0x444, read-write */
   uint32_t cqrs17;
   /** CQRS18 - Command Response for Direct-Command Task at address offset 0x448, read-only */
   uint32_t cqrs18;
   /** Reserved space */
   uint8_t fill6[4];
   /** CQRS20 - Response Mode Error Mask at address offset 0x450, read-write */
   uint32_t cqrs20;
   /** CQRS21 - Task Error Information at address offset 0x454, read-only */
   uint32_t cqrs21;
   /** CQRS22 - Command Response Index at address offset 0x458, read-only */
   uint32_t cqrs22;
   /** CQRS23 - Command Response Argument at address offset 0x45C, read-only */
   uint32_t cqrs23;
};

/** bit field defines for sdio_host_s#hrs00 */
#define SDIO_HOST_HRS00_SWR_OFFSET 0
#define SDIO_HOST_HRS00_SWR_SIZE 1
#define SDIO_HOST_HRS00_SAV_OFFSET 16
#define SDIO_HOST_HRS00_SAV_SIZE 8

/** bit field defines for sdio_host_s#hrs01 */
#define SDIO_HOST_HRS01_DP_OFFSET 0
#define SDIO_HOST_HRS01_DP_SIZE 24

/** bit field defines for sdio_host_s#hrs02 */
#define SDIO_HOST_HRS02_PBL_OFFSET 0
#define SDIO_HOST_HRS02_PBL_SIZE 4
#define SDIO_HOST_HRS02_OTN_OFFSET 16
#define SDIO_HOST_HRS02_OTN_SIZE 2

/** bit field defines for sdio_host_s#hrs03 */
#define SDIO_HOST_HRS03_AER_RD_OFFSET 0
#define SDIO_HOST_HRS03_AER_RD_SIZE 1
#define SDIO_HOST_HRS03_AER_RS_OFFSET 1
#define SDIO_HOST_HRS03_AER_RS_SIZE 1
#define SDIO_HOST_HRS03_AER_BD_OFFSET 2
#define SDIO_HOST_HRS03_AER_BD_SIZE 1
#define SDIO_HOST_HRS03_AER_BS_OFFSET 3
#define SDIO_HOST_HRS03_AER_BS_SIZE 1
#define SDIO_HOST_HRS03_AER_SENRD_OFFSET 8
#define SDIO_HOST_HRS03_AER_SENRD_SIZE 1
#define SDIO_HOST_HRS03_AER_SENRS_OFFSET 9
#define SDIO_HOST_HRS03_AER_SENRS_SIZE 1
#define SDIO_HOST_HRS03_AER_SENBD_OFFSET 10
#define SDIO_HOST_HRS03_AER_SENBD_SIZE 1
#define SDIO_HOST_HRS03_AER_SENBS_OFFSET 11
#define SDIO_HOST_HRS03_AER_SENBS_SIZE 1
#define SDIO_HOST_HRS03_AER_IERD_OFFSET 16
#define SDIO_HOST_HRS03_AER_IERD_SIZE 1
#define SDIO_HOST_HRS03_AER_IERS_OFFSET 17
#define SDIO_HOST_HRS03_AER_IERS_SIZE 1
#define SDIO_HOST_HRS03_AER_IEBD_OFFSET 18
#define SDIO_HOST_HRS03_AER_IEBD_SIZE 1
#define SDIO_HOST_HRS03_AER_IEBS_OFFSET 19
#define SDIO_HOST_HRS03_AER_IEBS_SIZE 1

/** bit field defines for sdio_host_s#hrs04 */
#define SDIO_HOST_HRS04_UIS_ADDR_OFFSET 0
#define SDIO_HOST_HRS04_UIS_ADDR_SIZE 6
#define SDIO_HOST_HRS04_UIS_WDATA_OFFSET 8
#define SDIO_HOST_HRS04_UIS_WDATA_SIZE 8
#define SDIO_HOST_HRS04_UIS_RDATA_OFFSET 16
#define SDIO_HOST_HRS04_UIS_RDATA_SIZE 5
#define SDIO_HOST_HRS04_UIS_WR_OFFSET 24
#define SDIO_HOST_HRS04_UIS_WR_SIZE 1
#define SDIO_HOST_HRS04_UIS_RD_OFFSET 25
#define SDIO_HOST_HRS04_UIS_RD_SIZE 1
#define SDIO_HOST_HRS04_UIS_ACK_OFFSET 26
#define SDIO_HOST_HRS04_UIS_ACK_SIZE 1

/** bit field defines for sdio_host_s#hrs05 */
#define SDIO_HOST_HRS05_BSTE_OFFSET 0
#define SDIO_HOST_HRS05_BSTE_SIZE 1
#define SDIO_HOST_HRS05_BSTD_OFFSET 1
#define SDIO_HOST_HRS05_BSTD_SIZE 1
#define SDIO_HOST_HRS05_BSTS_OFFSET 2
#define SDIO_HOST_HRS05_BSTS_SIZE 14

/** bit field defines for sdio_host_s#hrs06 */
#define SDIO_HOST_HRS06_EMM_OFFSET 0
#define SDIO_HOST_HRS06_EMM_SIZE 3
#define SDIO_HOST_HRS06_ETV_OFFSET 8
#define SDIO_HOST_HRS06_ETV_SIZE 6
#define SDIO_HOST_HRS06_ETR_OFFSET 15
#define SDIO_HOST_HRS06_ETR_SIZE 1

/** bit field defines for sdio_host_s#hrs32 */
#define SDIO_HOST_HRS32_DATA_OFFSET 0
#define SDIO_HOST_HRS32_DATA_SIZE 16
#define SDIO_HOST_HRS32_ADDR_OFFSET 16
#define SDIO_HOST_HRS32_ADDR_SIZE 15
#define SDIO_HOST_HRS32_LOAD_OFFSET 31
#define SDIO_HOST_HRS32_LOAD_SIZE 1

/** bit field defines for sdio_host_s#hrs33 */
#define SDIO_HOST_HRS33_STAT0_OFFSET 0
#define SDIO_HOST_HRS33_STAT0_SIZE 32

/** bit field defines for sdio_host_s#hrs34 */
#define SDIO_HOST_HRS34_STAT1_OFFSET 0
#define SDIO_HOST_HRS34_STAT1_SIZE 8

/** bit field defines for sdio_host_s#hrs35 */
#define SDIO_HOST_HRS35_TVAL_OFFSET 0
#define SDIO_HOST_HRS35_TVAL_SIZE 6
#define SDIO_HOST_HRS35_TFV_OFFSET 16
#define SDIO_HOST_HRS35_TFV_SIZE 6
#define SDIO_HOST_HRS35_TFR_OFFSET 31
#define SDIO_HOST_HRS35_TFR_SIZE 1

/** bit field defines for sdio_host_s#hrs36 */
#define SDIO_HOST_HRS36_BOOT_ACT_OFFSET 0
#define SDIO_HOST_HRS36_BOOT_ACT_SIZE 1
#define SDIO_HOST_HRS36_BOOT_EAT_OFFSET 1
#define SDIO_HOST_HRS36_BOOT_EAT_SIZE 1
#define SDIO_HOST_HRS36_BOOT_EAI_OFFSET 2
#define SDIO_HOST_HRS36_BOOT_EAI_SIZE 1
#define SDIO_HOST_HRS36_BOOT_EDT_OFFSET 3
#define SDIO_HOST_HRS36_BOOT_EDT_SIZE 1
#define SDIO_HOST_HRS36_BOOT_EDC_OFFSET 4
#define SDIO_HOST_HRS36_BOOT_EDC_SIZE 1
#define SDIO_HOST_HRS36_BOOT_EDE_OFFSET 5
#define SDIO_HOST_HRS36_BOOT_EDE_SIZE 1

/** bit field defines for sdio_host_s#hrs37 */
#define SDIO_HOST_HRS37_RGB_COEFF_IFM_OFFSET 0
#define SDIO_HOST_HRS37_RGB_COEFF_IFM_SIZE 6

/** bit field defines for sdio_host_s#hrs38 */
#define SDIO_HOST_HRS38_RGB_COEFF_OFFSET 0
#define SDIO_HOST_HRS38_RGB_COEFF_SIZE 6

/** bit field defines for sdio_host_s#hrs59 */
#define SDIO_HOST_HRS59_U2CRE_OFFSET 0
#define SDIO_HOST_HRS59_U2CRE_SIZE 1

/** bit field defines for sdio_host_s#crs63 */
#define SDIO_HOST_CRS63_ISES_OFFSET 0
#define SDIO_HOST_CRS63_ISES_SIZE 8
#define SDIO_HOST_CRS63_SVN_OFFSET 16
#define SDIO_HOST_CRS63_SVN_SIZE 8

/** bit field defines for sdio_host_s#srs00 */
#define SDIO_HOST_SRS00_SAAR_OFFSET 0
#define SDIO_HOST_SRS00_SAAR_SIZE 32

/** bit field defines for sdio_host_s#srs01 */
#define SDIO_HOST_SRS01_TBS_OFFSET 0
#define SDIO_HOST_SRS01_TBS_SIZE 12
#define SDIO_HOST_SRS01_SDMABB_OFFSET 12
#define SDIO_HOST_SRS01_SDMABB_SIZE 3
#define SDIO_HOST_SRS01_BCCT_OFFSET 16
#define SDIO_HOST_SRS01_BCCT_SIZE 16

/** bit field defines for sdio_host_s#srs02 */
#define SDIO_HOST_SRS02_ARG1_OFFSET 0
#define SDIO_HOST_SRS02_ARG1_SIZE 32

/** bit field defines for sdio_host_s#srs03 */
#define SDIO_HOST_SRS03_DMAE_OFFSET 0
#define SDIO_HOST_SRS03_DMAE_SIZE 1
#define SDIO_HOST_SRS03_BCE_OFFSET 1
#define SDIO_HOST_SRS03_BCE_SIZE 1
#define SDIO_HOST_SRS03_ACE_OFFSET 2
#define SDIO_HOST_SRS03_ACE_SIZE 2
#define SDIO_HOST_SRS03_DTDS_OFFSET 4
#define SDIO_HOST_SRS03_DTDS_SIZE 1
#define SDIO_HOST_SRS03_MSBS_OFFSET 5
#define SDIO_HOST_SRS03_MSBS_SIZE 1
#define SDIO_HOST_SRS03_RECT_OFFSET 6
#define SDIO_HOST_SRS03_RECT_SIZE 1
#define SDIO_HOST_SRS03_RECE_OFFSET 7
#define SDIO_HOST_SRS03_RECE_SIZE 1
#define SDIO_HOST_SRS03_RID_OFFSET 8
#define SDIO_HOST_SRS03_RID_SIZE 1
#define SDIO_HOST_SRS03_RTS_OFFSET 16
#define SDIO_HOST_SRS03_RTS_SIZE 2
#define SDIO_HOST_SRS03_CRCCE_OFFSET 19
#define SDIO_HOST_SRS03_CRCCE_SIZE 1
#define SDIO_HOST_SRS03_CICE_OFFSET 20
#define SDIO_HOST_SRS03_CICE_SIZE 1
#define SDIO_HOST_SRS03_DPS_OFFSET 21
#define SDIO_HOST_SRS03_DPS_SIZE 1
#define SDIO_HOST_SRS03_CT_OFFSET 22
#define SDIO_HOST_SRS03_CT_SIZE 2
#define SDIO_HOST_SRS03_CI_OFFSET 24
#define SDIO_HOST_SRS03_CI_SIZE 6

/** bit field defines for sdio_host_s#srs04 */
#define SDIO_HOST_SRS04_RESP1_OFFSET 0
#define SDIO_HOST_SRS04_RESP1_SIZE 32

/** bit field defines for sdio_host_s#srs05 */
#define SDIO_HOST_SRS05_RESP2_OFFSET 0
#define SDIO_HOST_SRS05_RESP2_SIZE 32

/** bit field defines for sdio_host_s#srs06 */
#define SDIO_HOST_SRS06_RESP3_OFFSET 0
#define SDIO_HOST_SRS06_RESP3_SIZE 32

/** bit field defines for sdio_host_s#srs07 */
#define SDIO_HOST_SRS07_RESP4_OFFSET 0
#define SDIO_HOST_SRS07_RESP4_SIZE 32

/** bit field defines for sdio_host_s#srs08 */
#define SDIO_HOST_SRS08_BDP_OFFSET 0
#define SDIO_HOST_SRS08_BDP_SIZE 32

/** bit field defines for sdio_host_s#srs09 */
#define SDIO_HOST_SRS09_CICMD_OFFSET 0
#define SDIO_HOST_SRS09_CICMD_SIZE 1
#define SDIO_HOST_SRS09_CIDAT_OFFSET 1
#define SDIO_HOST_SRS09_CIDAT_SIZE 1
#define SDIO_HOST_SRS09_DLA_OFFSET 2
#define SDIO_HOST_SRS09_DLA_SIZE 1
#define SDIO_HOST_SRS09_DATSL2_OFFSET 4
#define SDIO_HOST_SRS09_DATSL2_SIZE 4
#define SDIO_HOST_SRS09_WTA_OFFSET 8
#define SDIO_HOST_SRS09_WTA_SIZE 1
#define SDIO_HOST_SRS09_RTA_OFFSET 9
#define SDIO_HOST_SRS09_RTA_SIZE 1
#define SDIO_HOST_SRS09_BWE_OFFSET 10
#define SDIO_HOST_SRS09_BWE_SIZE 1
#define SDIO_HOST_SRS09_BRE_OFFSET 11
#define SDIO_HOST_SRS09_BRE_SIZE 1
#define SDIO_HOST_SRS09_CI_OFFSET 16
#define SDIO_HOST_SRS09_CI_SIZE 1
#define SDIO_HOST_SRS09_CSS_OFFSET 17
#define SDIO_HOST_SRS09_CSS_SIZE 1
#define SDIO_HOST_SRS09_CDSL_OFFSET 18
#define SDIO_HOST_SRS09_CDSL_SIZE 1
#define SDIO_HOST_SRS09_WPSL_OFFSET 19
#define SDIO_HOST_SRS09_WPSL_SIZE 1
#define SDIO_HOST_SRS09_DATSL1_OFFSET 20
#define SDIO_HOST_SRS09_DATSL1_SIZE 4
#define SDIO_HOST_SRS09_CMDSL_OFFSET 24
#define SDIO_HOST_SRS09_CMDSL_SIZE 1

/** bit field defines for sdio_host_s#srs10 */
#define SDIO_HOST_SRS10_LEDC_OFFSET 0
#define SDIO_HOST_SRS10_LEDC_SIZE 1
#define SDIO_HOST_SRS10_DTW_OFFSET 1
#define SDIO_HOST_SRS10_DTW_SIZE 1
#define SDIO_HOST_SRS10_HSE_OFFSET 2
#define SDIO_HOST_SRS10_HSE_SIZE 1
#define SDIO_HOST_SRS10_DMASEL_OFFSET 3
#define SDIO_HOST_SRS10_DMASEL_SIZE 2
#define SDIO_HOST_SRS10_EDTW_OFFSET 5
#define SDIO_HOST_SRS10_EDTW_SIZE 1
#define SDIO_HOST_SRS10_CDTL_OFFSET 6
#define SDIO_HOST_SRS10_CDTL_SIZE 1
#define SDIO_HOST_SRS10_CDSS_OFFSET 7
#define SDIO_HOST_SRS10_CDSS_SIZE 1
#define SDIO_HOST_SRS10_BP_OFFSET 8
#define SDIO_HOST_SRS10_BP_SIZE 1
#define SDIO_HOST_SRS10_BVS_OFFSET 9
#define SDIO_HOST_SRS10_BVS_SIZE 3
#define SDIO_HOST_SRS10_BP2_OFFSET 12
#define SDIO_HOST_SRS10_BP2_SIZE 1
#define SDIO_HOST_SRS10_BVS2_OFFSET 13
#define SDIO_HOST_SRS10_BVS2_SIZE 3
#define SDIO_HOST_SRS10_SBGR_OFFSET 16
#define SDIO_HOST_SRS10_SBGR_SIZE 1
#define SDIO_HOST_SRS10_CR_OFFSET 17
#define SDIO_HOST_SRS10_CR_SIZE 1
#define SDIO_HOST_SRS10_WOIQ_OFFSET 24
#define SDIO_HOST_SRS10_WOIQ_SIZE 1
#define SDIO_HOST_SRS10_WOIS_OFFSET 25
#define SDIO_HOST_SRS10_WOIS_SIZE 1
#define SDIO_HOST_SRS10_WORM_OFFSET 26
#define SDIO_HOST_SRS10_WORM_SIZE 1

/** bit field defines for sdio_host_s#srs11 */
#define SDIO_HOST_SRS11_ICE_OFFSET 0
#define SDIO_HOST_SRS11_ICE_SIZE 1
#define SDIO_HOST_SRS11_ICS_OFFSET 1
#define SDIO_HOST_SRS11_ICS_SIZE 1
#define SDIO_HOST_SRS11_SDCE_OFFSET 2
#define SDIO_HOST_SRS11_SDCE_SIZE 1
#define SDIO_HOST_SRS11_SDCFSH_OFFSET 6
#define SDIO_HOST_SRS11_SDCFSH_SIZE 2
#define SDIO_HOST_SRS11_SDCFSL_OFFSET 8
#define SDIO_HOST_SRS11_SDCFSL_SIZE 8
#define SDIO_HOST_SRS11_DTCV_OFFSET 16
#define SDIO_HOST_SRS11_DTCV_SIZE 4
#define SDIO_HOST_SRS11_SRFA_OFFSET 24
#define SDIO_HOST_SRS11_SRFA_SIZE 1
#define SDIO_HOST_SRS11_SRCMD_OFFSET 25
#define SDIO_HOST_SRS11_SRCMD_SIZE 1
#define SDIO_HOST_SRS11_SRDAT_OFFSET 26
#define SDIO_HOST_SRS11_SRDAT_SIZE 1

/** bit field defines for sdio_host_s#srs12 */
#define SDIO_HOST_SRS12_CC_OFFSET 0
#define SDIO_HOST_SRS12_CC_SIZE 1
#define SDIO_HOST_SRS12_TC_OFFSET 1
#define SDIO_HOST_SRS12_TC_SIZE 1
#define SDIO_HOST_SRS12_BGE_OFFSET 2
#define SDIO_HOST_SRS12_BGE_SIZE 1
#define SDIO_HOST_SRS12_DMAINT_OFFSET 3
#define SDIO_HOST_SRS12_DMAINT_SIZE 1
#define SDIO_HOST_SRS12_BWR_OFFSET 4
#define SDIO_HOST_SRS12_BWR_SIZE 1
#define SDIO_HOST_SRS12_BRR_OFFSET 5
#define SDIO_HOST_SRS12_BRR_SIZE 1
#define SDIO_HOST_SRS12_CIN_OFFSET 6
#define SDIO_HOST_SRS12_CIN_SIZE 1
#define SDIO_HOST_SRS12_CR_OFFSET 7
#define SDIO_HOST_SRS12_CR_SIZE 1
#define SDIO_HOST_SRS12_CINT_OFFSET 8
#define SDIO_HOST_SRS12_CINT_SIZE 1
#define SDIO_HOST_SRS12_CQINT_OFFSET 14
#define SDIO_HOST_SRS12_CQINT_SIZE 1
#define SDIO_HOST_SRS12_EINT_OFFSET 15
#define SDIO_HOST_SRS12_EINT_SIZE 1
#define SDIO_HOST_SRS12_ECT_OFFSET 16
#define SDIO_HOST_SRS12_ECT_SIZE 1
#define SDIO_HOST_SRS12_ECCRC_OFFSET 17
#define SDIO_HOST_SRS12_ECCRC_SIZE 1
#define SDIO_HOST_SRS12_ECEB_OFFSET 18
#define SDIO_HOST_SRS12_ECEB_SIZE 1
#define SDIO_HOST_SRS12_ECI_OFFSET 19
#define SDIO_HOST_SRS12_ECI_SIZE 1
#define SDIO_HOST_SRS12_EDT_OFFSET 20
#define SDIO_HOST_SRS12_EDT_SIZE 1
#define SDIO_HOST_SRS12_EDCRC_OFFSET 21
#define SDIO_HOST_SRS12_EDCRC_SIZE 1
#define SDIO_HOST_SRS12_EDEB_OFFSET 22
#define SDIO_HOST_SRS12_EDEB_SIZE 1
#define SDIO_HOST_SRS12_ECL_OFFSET 23
#define SDIO_HOST_SRS12_ECL_SIZE 1
#define SDIO_HOST_SRS12_EAC_OFFSET 24
#define SDIO_HOST_SRS12_EAC_SIZE 1
#define SDIO_HOST_SRS12_EADMA_OFFSET 25
#define SDIO_HOST_SRS12_EADMA_SIZE 1
#define SDIO_HOST_SRS12_ERSP_OFFSET 27
#define SDIO_HOST_SRS12_ERSP_SIZE 1

/** bit field defines for sdio_host_s#srs13 */
#define SDIO_HOST_SRS13_CC_SE_OFFSET 0
#define SDIO_HOST_SRS13_CC_SE_SIZE 1
#define SDIO_HOST_SRS13_TC_SE_OFFSET 1
#define SDIO_HOST_SRS13_TC_SE_SIZE 1
#define SDIO_HOST_SRS13_BGE_SE_OFFSET 2
#define SDIO_HOST_SRS13_BGE_SE_SIZE 1
#define SDIO_HOST_SRS13_DMAINT_SE_OFFSET 3
#define SDIO_HOST_SRS13_DMAINT_SE_SIZE 1
#define SDIO_HOST_SRS13_BWR_SE_OFFSET 4
#define SDIO_HOST_SRS13_BWR_SE_SIZE 1
#define SDIO_HOST_SRS13_BRR_SE_OFFSET 5
#define SDIO_HOST_SRS13_BRR_SE_SIZE 1
#define SDIO_HOST_SRS13_CIN_SE_OFFSET 6
#define SDIO_HOST_SRS13_CIN_SE_SIZE 1
#define SDIO_HOST_SRS13_CR_SE_OFFSET 7
#define SDIO_HOST_SRS13_CR_SE_SIZE 1
#define SDIO_HOST_SRS13_CINT_SE_OFFSET 8
#define SDIO_HOST_SRS13_CINT_SE_SIZE 1
#define SDIO_HOST_SRS13_CQINT_SE_OFFSET 14
#define SDIO_HOST_SRS13_CQINT_SE_SIZE 1
#define SDIO_HOST_SRS13_FIXED_TO_0_OFFSET 15
#define SDIO_HOST_SRS13_FIXED_TO_0_SIZE 1
#define SDIO_HOST_SRS13_ECT_SE_OFFSET 16
#define SDIO_HOST_SRS13_ECT_SE_SIZE 1
#define SDIO_HOST_SRS13_ECCRC_SE_OFFSET 17
#define SDIO_HOST_SRS13_ECCRC_SE_SIZE 1
#define SDIO_HOST_SRS13_ECEB_SE_OFFSET 18
#define SDIO_HOST_SRS13_ECEB_SE_SIZE 1
#define SDIO_HOST_SRS13_ECI_SE_OFFSET 19
#define SDIO_HOST_SRS13_ECI_SE_SIZE 1
#define SDIO_HOST_SRS13_EDT_SE_OFFSET 20
#define SDIO_HOST_SRS13_EDT_SE_SIZE 1
#define SDIO_HOST_SRS13_EDCRC_SE_OFFSET 21
#define SDIO_HOST_SRS13_EDCRC_SE_SIZE 1
#define SDIO_HOST_SRS13_EDEB_SE_OFFSET 22
#define SDIO_HOST_SRS13_EDEB_SE_SIZE 1
#define SDIO_HOST_SRS13_ECL_SE_OFFSET 23
#define SDIO_HOST_SRS13_ECL_SE_SIZE 1
#define SDIO_HOST_SRS13_EAC_SE_OFFSET 24
#define SDIO_HOST_SRS13_EAC_SE_SIZE 1
#define SDIO_HOST_SRS13_EADMA_SE_OFFSET 25
#define SDIO_HOST_SRS13_EADMA_SE_SIZE 1
#define SDIO_HOST_SRS13_ERSP_SE_OFFSET 27
#define SDIO_HOST_SRS13_ERSP_SE_SIZE 1

/** bit field defines for sdio_host_s#srs14 */
#define SDIO_HOST_SRS14_CC_IE_OFFSET 0
#define SDIO_HOST_SRS14_CC_IE_SIZE 1
#define SDIO_HOST_SRS14_TC_IE_OFFSET 1
#define SDIO_HOST_SRS14_TC_IE_SIZE 1
#define SDIO_HOST_SRS14_BGE_IE_OFFSET 2
#define SDIO_HOST_SRS14_BGE_IE_SIZE 1
#define SDIO_HOST_SRS14_DMAINT_IE_OFFSET 3
#define SDIO_HOST_SRS14_DMAINT_IE_SIZE 1
#define SDIO_HOST_SRS14_BWR_IE_OFFSET 4
#define SDIO_HOST_SRS14_BWR_IE_SIZE 1
#define SDIO_HOST_SRS14_BRR_IE_OFFSET 5
#define SDIO_HOST_SRS14_BRR_IE_SIZE 1
#define SDIO_HOST_SRS14_CIN_IE_OFFSET 6
#define SDIO_HOST_SRS14_CIN_IE_SIZE 1
#define SDIO_HOST_SRS14_CR_IE_OFFSET 7
#define SDIO_HOST_SRS14_CR_IE_SIZE 1
#define SDIO_HOST_SRS14_CINT_IE_OFFSET 8
#define SDIO_HOST_SRS14_CINT_IE_SIZE 1
#define SDIO_HOST_SRS14_CQINT_IE_OFFSET 14
#define SDIO_HOST_SRS14_CQINT_IE_SIZE 1
#define SDIO_HOST_SRS14_FIXED_TO_0_OFFSET 15
#define SDIO_HOST_SRS14_FIXED_TO_0_SIZE 1
#define SDIO_HOST_SRS14_ECT_IE_OFFSET 16
#define SDIO_HOST_SRS14_ECT_IE_SIZE 1
#define SDIO_HOST_SRS14_ECCRC_IE_OFFSET 17
#define SDIO_HOST_SRS14_ECCRC_IE_SIZE 1
#define SDIO_HOST_SRS14_ECEB_IE_OFFSET 18
#define SDIO_HOST_SRS14_ECEB_IE_SIZE 1
#define SDIO_HOST_SRS14_ECI_IE_OFFSET 19
#define SDIO_HOST_SRS14_ECI_IE_SIZE 1
#define SDIO_HOST_SRS14_EDT_IE_OFFSET 20
#define SDIO_HOST_SRS14_EDT_IE_SIZE 1
#define SDIO_HOST_SRS14_EDCRC_IE_OFFSET 21
#define SDIO_HOST_SRS14_EDCRC_IE_SIZE 1
#define SDIO_HOST_SRS14_EDEB_IE_OFFSET 22
#define SDIO_HOST_SRS14_EDEB_IE_SIZE 1
#define SDIO_HOST_SRS14_ECL_IE_OFFSET 23
#define SDIO_HOST_SRS14_ECL_IE_SIZE 1
#define SDIO_HOST_SRS14_EAC_IE_OFFSET 24
#define SDIO_HOST_SRS14_EAC_IE_SIZE 1
#define SDIO_HOST_SRS14_EADMA_IE_OFFSET 25
#define SDIO_HOST_SRS14_EADMA_IE_SIZE 1
#define SDIO_HOST_SRS14_ERSP_IE_OFFSET 27
#define SDIO_HOST_SRS14_ERSP_IE_SIZE 1

/** bit field defines for sdio_host_s#srs15 */
#define SDIO_HOST_SRS15_ACNE_OFFSET 0
#define SDIO_HOST_SRS15_ACNE_SIZE 1
#define SDIO_HOST_SRS15_ACTE_OFFSET 1
#define SDIO_HOST_SRS15_ACTE_SIZE 1
#define SDIO_HOST_SRS15_ACCE_OFFSET 2
#define SDIO_HOST_SRS15_ACCE_SIZE 1
#define SDIO_HOST_SRS15_ACEBE_OFFSET 3
#define SDIO_HOST_SRS15_ACEBE_SIZE 1
#define SDIO_HOST_SRS15_ACIE_OFFSET 4
#define SDIO_HOST_SRS15_ACIE_SIZE 1
#define SDIO_HOST_SRS15_ACRE_OFFSET 5
#define SDIO_HOST_SRS15_ACRE_SIZE 1
#define SDIO_HOST_SRS15_CNIACE_OFFSET 7
#define SDIO_HOST_SRS15_CNIACE_SIZE 1
#define SDIO_HOST_SRS15_UMS_OFFSET 16
#define SDIO_HOST_SRS15_UMS_SIZE 3
#define SDIO_HOST_SRS15_V18SE_OFFSET 19
#define SDIO_HOST_SRS15_V18SE_SIZE 1
#define SDIO_HOST_SRS15_DSS_OFFSET 20
#define SDIO_HOST_SRS15_DSS_SIZE 2
#define SDIO_HOST_SRS15_EXTNG_OFFSET 22
#define SDIO_HOST_SRS15_EXTNG_SIZE 1
#define SDIO_HOST_SRS15_SCS_OFFSET 23
#define SDIO_HOST_SRS15_SCS_SIZE 1
#define SDIO_HOST_SRS15_HV4E_OFFSET 28
#define SDIO_HOST_SRS15_HV4E_SIZE 1
#define SDIO_HOST_SRS15_A64B_OFFSET 29
#define SDIO_HOST_SRS15_A64B_SIZE 1
#define SDIO_HOST_SRS15_PVE_OFFSET 31
#define SDIO_HOST_SRS15_PVE_SIZE 1

/** bit field defines for sdio_host_s#srs16 */
#define SDIO_HOST_SRS16_TCF_OFFSET 0
#define SDIO_HOST_SRS16_TCF_SIZE 6
#define SDIO_HOST_SRS16_TCU_OFFSET 7
#define SDIO_HOST_SRS16_TCU_SIZE 1
#define SDIO_HOST_SRS16_BCSDCLK_OFFSET 8
#define SDIO_HOST_SRS16_BCSDCLK_SIZE 8
#define SDIO_HOST_SRS16_MBL_OFFSET 16
#define SDIO_HOST_SRS16_MBL_SIZE 2
#define SDIO_HOST_SRS16_EDS8_OFFSET 18
#define SDIO_HOST_SRS16_EDS8_SIZE 1
#define SDIO_HOST_SRS16_ADMA2S_OFFSET 19
#define SDIO_HOST_SRS16_ADMA2S_SIZE 1
#define SDIO_HOST_SRS16_ADMA1S_OFFSET 20
#define SDIO_HOST_SRS16_ADMA1S_SIZE 1
#define SDIO_HOST_SRS16_HSS_OFFSET 21
#define SDIO_HOST_SRS16_HSS_SIZE 1
#define SDIO_HOST_SRS16_DMAS_OFFSET 22
#define SDIO_HOST_SRS16_DMAS_SIZE 1
#define SDIO_HOST_SRS16_SRS_OFFSET 23
#define SDIO_HOST_SRS16_SRS_SIZE 1
#define SDIO_HOST_SRS16_VS33_OFFSET 24
#define SDIO_HOST_SRS16_VS33_SIZE 1
#define SDIO_HOST_SRS16_VS30_OFFSET 25
#define SDIO_HOST_SRS16_VS30_SIZE 1
#define SDIO_HOST_SRS16_VS18_OFFSET 26
#define SDIO_HOST_SRS16_VS18_SIZE 1
#define SDIO_HOST_SRS16_A64S_OFFSET 28
#define SDIO_HOST_SRS16_A64S_SIZE 1
#define SDIO_HOST_SRS16_AIS_OFFSET 29
#define SDIO_HOST_SRS16_AIS_SIZE 1
#define SDIO_HOST_SRS16_SLT_OFFSET 30
#define SDIO_HOST_SRS16_SLT_SIZE 2

/** bit field defines for sdio_host_s#srs17 */
#define SDIO_HOST_SRS17_SDR50_OFFSET 0
#define SDIO_HOST_SRS17_SDR50_SIZE 1
#define SDIO_HOST_SRS17_SDR104_OFFSET 1
#define SDIO_HOST_SRS17_SDR104_SIZE 1
#define SDIO_HOST_SRS17_DDR50_OFFSET 2
#define SDIO_HOST_SRS17_DDR50_SIZE 1
#define SDIO_HOST_SRS17_UHSII_OFFSET 3
#define SDIO_HOST_SRS17_UHSII_SIZE 1
#define SDIO_HOST_SRS17_DRVA_OFFSET 4
#define SDIO_HOST_SRS17_DRVA_SIZE 1
#define SDIO_HOST_SRS17_DRVC_OFFSET 5
#define SDIO_HOST_SRS17_DRVC_SIZE 1
#define SDIO_HOST_SRS17_DRVD_OFFSET 6
#define SDIO_HOST_SRS17_DRVD_SIZE 1
#define SDIO_HOST_SRS17_RTNGCNT_OFFSET 8
#define SDIO_HOST_SRS17_RTNGCNT_SIZE 4
#define SDIO_HOST_SRS17_UTSM50_OFFSET 13
#define SDIO_HOST_SRS17_UTSM50_SIZE 1
#define SDIO_HOST_SRS17_RTNGM_OFFSET 14
#define SDIO_HOST_SRS17_RTNGM_SIZE 2
#define SDIO_HOST_SRS17_CLKMPR_OFFSET 16
#define SDIO_HOST_SRS17_CLKMPR_SIZE 8
#define SDIO_HOST_SRS17_VDD2S_OFFSET 28
#define SDIO_HOST_SRS17_VDD2S_SIZE 1

/** bit field defines for sdio_host_s#srs18 */
#define SDIO_HOST_SRS18_MC33_OFFSET 0
#define SDIO_HOST_SRS18_MC33_SIZE 8
#define SDIO_HOST_SRS18_MC30_OFFSET 8
#define SDIO_HOST_SRS18_MC30_SIZE 8
#define SDIO_HOST_SRS18_MC18_OFFSET 16
#define SDIO_HOST_SRS18_MC18_SIZE 8

/** bit field defines for sdio_host_s#srs19 */
#define SDIO_HOST_SRS19_MC18V2_OFFSET 0
#define SDIO_HOST_SRS19_MC18V2_SIZE 8

/** bit field defines for sdio_host_s#srs20 */
#define SDIO_HOST_SRS20_ACNE_FE_OFFSET 0
#define SDIO_HOST_SRS20_ACNE_FE_SIZE 1
#define SDIO_HOST_SRS20_ACTE_FE_OFFSET 1
#define SDIO_HOST_SRS20_ACTE_FE_SIZE 1
#define SDIO_HOST_SRS20_ACCE_FE_OFFSET 2
#define SDIO_HOST_SRS20_ACCE_FE_SIZE 1
#define SDIO_HOST_SRS20_ACEBE_FE_OFFSET 3
#define SDIO_HOST_SRS20_ACEBE_FE_SIZE 1
#define SDIO_HOST_SRS20_ACIE_FE_OFFSET 4
#define SDIO_HOST_SRS20_ACIE_FE_SIZE 1
#define SDIO_HOST_SRS20_CNIACE_FE_OFFSET 7
#define SDIO_HOST_SRS20_CNIACE_FE_SIZE 1
#define SDIO_HOST_SRS20_ECT_FE_OFFSET 16
#define SDIO_HOST_SRS20_ECT_FE_SIZE 1
#define SDIO_HOST_SRS20_ECCRC_FE_OFFSET 17
#define SDIO_HOST_SRS20_ECCRC_FE_SIZE 1
#define SDIO_HOST_SRS20_ECEB_FE_OFFSET 18
#define SDIO_HOST_SRS20_ECEB_FE_SIZE 1
#define SDIO_HOST_SRS20_ECI_FE_OFFSET 19
#define SDIO_HOST_SRS20_ECI_FE_SIZE 1
#define SDIO_HOST_SRS20_EDT_FE_OFFSET 20
#define SDIO_HOST_SRS20_EDT_FE_SIZE 1
#define SDIO_HOST_SRS20_EDCRC_FE_OFFSET 21
#define SDIO_HOST_SRS20_EDCRC_FE_SIZE 1
#define SDIO_HOST_SRS20_EDEB_FE_OFFSET 22
#define SDIO_HOST_SRS20_EDEB_FE_SIZE 1
#define SDIO_HOST_SRS20_ECL_FE_OFFSET 23
#define SDIO_HOST_SRS20_ECL_FE_SIZE 1
#define SDIO_HOST_SRS20_EAC12_FE_OFFSET 24
#define SDIO_HOST_SRS20_EAC12_FE_SIZE 1
#define SDIO_HOST_SRS20_EADMA_FE_OFFSET 25
#define SDIO_HOST_SRS20_EADMA_FE_SIZE 1
#define SDIO_HOST_SRS20_ERESP_FE_OFFSET 27
#define SDIO_HOST_SRS20_ERESP_FE_SIZE 1

/** bit field defines for sdio_host_s#srs21 */
#define SDIO_HOST_SRS21_EADMAS_OFFSET 0
#define SDIO_HOST_SRS21_EADMAS_SIZE 2
#define SDIO_HOST_SRS21_EADMAL_OFFSET 2
#define SDIO_HOST_SRS21_EADMAL_SIZE 1

/** bit field defines for sdio_host_s#srs22 */
#define SDIO_HOST_SRS22_DMASA1_OFFSET 0
#define SDIO_HOST_SRS22_DMASA1_SIZE 32

/** bit field defines for sdio_host_s#srs23 */
#define SDIO_HOST_SRS23_DMASA2_OFFSET 0
#define SDIO_HOST_SRS23_DMASA2_SIZE 32

/** bit field defines for sdio_host_s#srs24 */
#define SDIO_HOST_SRS24_SDCFSPV_15_16_OFFSET 16
#define SDIO_HOST_SRS24_SDCFSPV_15_16_SIZE 10
#define SDIO_HOST_SRS24_DSSPV_31_30_OFFSET 30
#define SDIO_HOST_SRS24_DSSPV_31_30_SIZE 2

/** bit field defines for sdio_host_s#srs25 */
#define SDIO_HOST_SRS25_SDCFSPV_09_00_OFFSET 0
#define SDIO_HOST_SRS25_SDCFSPV_09_00_SIZE 10
#define SDIO_HOST_SRS25_DSSPV_15_14_OFFSET 14
#define SDIO_HOST_SRS25_DSSPV_15_14_SIZE 2
#define SDIO_HOST_SRS25_SDCFSPV_15_16_OFFSET 16
#define SDIO_HOST_SRS25_SDCFSPV_15_16_SIZE 10
#define SDIO_HOST_SRS25_DSSPV_31_30_OFFSET 30
#define SDIO_HOST_SRS25_DSSPV_31_30_SIZE 2

/** bit field defines for sdio_host_s#srs26 */
#define SDIO_HOST_SRS26_SDCFSPV_09_00_OFFSET 0
#define SDIO_HOST_SRS26_SDCFSPV_09_00_SIZE 10
#define SDIO_HOST_SRS26_DSSPV_15_14_OFFSET 14
#define SDIO_HOST_SRS26_DSSPV_15_14_SIZE 2
#define SDIO_HOST_SRS26_SDCFSPV_15_16_OFFSET 16
#define SDIO_HOST_SRS26_SDCFSPV_15_16_SIZE 10
#define SDIO_HOST_SRS26_DSSPV_31_30_OFFSET 30
#define SDIO_HOST_SRS26_DSSPV_31_30_SIZE 2

/** bit field defines for sdio_host_s#srs27 */
#define SDIO_HOST_SRS27_SDCFSPV_09_00_OFFSET 0
#define SDIO_HOST_SRS27_SDCFSPV_09_00_SIZE 10
#define SDIO_HOST_SRS27_DSSPV_15_14_OFFSET 14
#define SDIO_HOST_SRS27_DSSPV_15_14_SIZE 2
#define SDIO_HOST_SRS27_SDCFSPV_15_16_OFFSET 16
#define SDIO_HOST_SRS27_SDCFSPV_15_16_SIZE 10
#define SDIO_HOST_SRS27_DSSPV_31_30_OFFSET 30
#define SDIO_HOST_SRS27_DSSPV_31_30_SIZE 2

/** bit field defines for sdio_host_s#cqrs00 */
#define SDIO_HOST_CQRS00_CQVN3_OFFSET 0
#define SDIO_HOST_CQRS00_CQVN3_SIZE 4
#define SDIO_HOST_CQRS00_CQVN2_OFFSET 4
#define SDIO_HOST_CQRS00_CQVN2_SIZE 4
#define SDIO_HOST_CQRS00_CQVN1_OFFSET 8
#define SDIO_HOST_CQRS00_CQVN1_SIZE 4

/** bit field defines for sdio_host_s#cqrs01 */
#define SDIO_HOST_CQRS01_ITCFVAL_OFFSET 0
#define SDIO_HOST_CQRS01_ITCFVAL_SIZE 10
#define SDIO_HOST_CQRS01_ITCFMUL_OFFSET 12
#define SDIO_HOST_CQRS01_ITCFMUL_SIZE 4

/** bit field defines for sdio_host_s#cqrs02 */
#define SDIO_HOST_CQRS02_CQE_OFFSET 0
#define SDIO_HOST_CQRS02_CQE_SIZE 1
#define SDIO_HOST_CQRS02_CQTDS_OFFSET 8
#define SDIO_HOST_CQRS02_CQTDS_SIZE 1
#define SDIO_HOST_CQRS02_CQDCE_OFFSET 12
#define SDIO_HOST_CQRS02_CQDCE_SIZE 1

/** bit field defines for sdio_host_s#cqrs03 */
#define SDIO_HOST_CQRS03_CQHLT_OFFSET 0
#define SDIO_HOST_CQRS03_CQHLT_SIZE 1
#define SDIO_HOST_CQRS03_CQCAT_OFFSET 8
#define SDIO_HOST_CQRS03_CQCAT_SIZE 1

/** bit field defines for sdio_host_s#cqrs04 */
#define SDIO_HOST_CQRS04_CQHAC_OFFSET 0
#define SDIO_HOST_CQRS04_CQHAC_SIZE 1
#define SDIO_HOST_CQRS04_CQTCC_OFFSET 1
#define SDIO_HOST_CQRS04_CQTCC_SIZE 1
#define SDIO_HOST_CQRS04_CQREDI_OFFSET 2
#define SDIO_HOST_CQRS04_CQREDI_SIZE 1
#define SDIO_HOST_CQRS04_CQTCL_OFFSET 3
#define SDIO_HOST_CQRS04_CQTCL_SIZE 1

/** bit field defines for sdio_host_s#cqrs05 */
#define SDIO_HOST_CQRS05_CQHACST_OFFSET 0
#define SDIO_HOST_CQRS05_CQHACST_SIZE 1
#define SDIO_HOST_CQRS05_CQTCCST_OFFSET 1
#define SDIO_HOST_CQRS05_CQTCCST_SIZE 1
#define SDIO_HOST_CQRS05_CQREDST_OFFSET 2
#define SDIO_HOST_CQRS05_CQREDST_SIZE 1
#define SDIO_HOST_CQRS05_CQTCLST_OFFSET 3
#define SDIO_HOST_CQRS05_CQTCLST_SIZE 1

/** bit field defines for sdio_host_s#cqrs06 */
#define SDIO_HOST_CQRS06_CQHACSI_OFFSET 0
#define SDIO_HOST_CQRS06_CQHACSI_SIZE 1
#define SDIO_HOST_CQRS06_CQTCCSI_OFFSET 1
#define SDIO_HOST_CQRS06_CQTCCSI_SIZE 1
#define SDIO_HOST_CQRS06_CQREDSI_OFFSET 2
#define SDIO_HOST_CQRS06_CQREDSI_SIZE 1
#define SDIO_HOST_CQRS06_CQTCLSI_OFFSET 3
#define SDIO_HOST_CQRS06_CQTCLSI_SIZE 1

/** bit field defines for sdio_host_s#cqrs07 */
#define SDIO_HOST_CQRS07_CQICTOVAL_OFFSET 0
#define SDIO_HOST_CQRS07_CQICTOVAL_SIZE 7
#define SDIO_HOST_CQRS07_CQICTOVALEN_OFFSET 7
#define SDIO_HOST_CQRS07_CQICTOVALEN_SIZE 1
#define SDIO_HOST_CQRS07_CQICCTH_OFFSET 8
#define SDIO_HOST_CQRS07_CQICCTH_SIZE 5
#define SDIO_HOST_CQRS07_CQICCTHWEN_OFFSET 15
#define SDIO_HOST_CQRS07_CQICCTHWEN_SIZE 1
#define SDIO_HOST_CQRS07_CQICCTR_OFFSET 16
#define SDIO_HOST_CQRS07_CQICCTR_SIZE 1
#define SDIO_HOST_CQRS07_CQICSB_OFFSET 20
#define SDIO_HOST_CQRS07_CQICSB_SIZE 1
#define SDIO_HOST_CQRS07_CQICED_OFFSET 31
#define SDIO_HOST_CQRS07_CQICED_SIZE 1

/** bit field defines for sdio_host_s#cqrs08 */
#define SDIO_HOST_CQRS08_CQTDLBA_OFFSET 0
#define SDIO_HOST_CQRS08_CQTDLBA_SIZE 32

/** bit field defines for sdio_host_s#cqrs09 */
#define SDIO_HOST_CQRS09_CQTDLBAU_OFFSET 0
#define SDIO_HOST_CQRS09_CQTDLBAU_SIZE 32

/** bit field defines for sdio_host_s#cqrs10 */
#define SDIO_HOST_CQRS10_CQTD_OFFSET 0
#define SDIO_HOST_CQRS10_CQTD_SIZE 32

/** bit field defines for sdio_host_s#cqrs11 */
#define SDIO_HOST_CQRS11_CQTCN_OFFSET 0
#define SDIO_HOST_CQRS11_CQTCN_SIZE 32

/** bit field defines for sdio_host_s#cqrs12 */
#define SDIO_HOST_CQRS12_CQDQS_OFFSET 0
#define SDIO_HOST_CQRS12_CQDQS_SIZE 32

/** bit field defines for sdio_host_s#cqrs13 */
#define SDIO_HOST_CQRS13_CQDPT_OFFSET 0
#define SDIO_HOST_CQRS13_CQDPT_SIZE 32

/** bit field defines for sdio_host_s#cqrs14 */
#define SDIO_HOST_CQRS14_CQTC_OFFSET 0
#define SDIO_HOST_CQRS14_CQTC_SIZE 32

/** bit field defines for sdio_host_s#cqrs16 */
#define SDIO_HOST_CQRS16_CQSSCIT_OFFSET 0
#define SDIO_HOST_CQRS16_CQSSCIT_SIZE 16
#define SDIO_HOST_CQRS16_CQSSCBC_OFFSET 16
#define SDIO_HOST_CQRS16_CQSSCBC_SIZE 4

/** bit field defines for sdio_host_s#cqrs17 */
#define SDIO_HOST_CQRS17_CQSQSR_OFFSET 0
#define SDIO_HOST_CQRS17_CQSQSR_SIZE 16

/** bit field defines for sdio_host_s#cqrs18 */
#define SDIO_HOST_CQRS18_CQDCLR_OFFSET 0
#define SDIO_HOST_CQRS18_CQDCLR_SIZE 32

/** bit field defines for sdio_host_s#cqrs20 */
#define SDIO_HOST_CQRS20_CQRMEM_OFFSET 0
#define SDIO_HOST_CQRS20_CQRMEM_SIZE 32

/** bit field defines for sdio_host_s#cqrs21 */
#define SDIO_HOST_CQRS21_CQRMECI_OFFSET 0
#define SDIO_HOST_CQRS21_CQRMECI_SIZE 6
#define SDIO_HOST_CQRS21_CQRMETID_OFFSET 8
#define SDIO_HOST_CQRS21_CQRMETID_SIZE 5
#define SDIO_HOST_CQRS21_CQRMEFV_OFFSET 15
#define SDIO_HOST_CQRS21_CQRMEFV_SIZE 1
#define SDIO_HOST_CQRS21_CQDTECI_OFFSET 16
#define SDIO_HOST_CQRS21_CQDTECI_SIZE 6
#define SDIO_HOST_CQRS21_CQDTETID_OFFSET 24
#define SDIO_HOST_CQRS21_CQDTETID_SIZE 5
#define SDIO_HOST_CQRS21_CQDTEFV_OFFSET 31
#define SDIO_HOST_CQRS21_CQDTEFV_SIZE 1

/** bit field defines for sdio_host_s#cqrs22 */
#define SDIO_HOST_CQRS22_CQLCRI_OFFSET 0
#define SDIO_HOST_CQRS22_CQLCRI_SIZE 6

/** bit field defines for sdio_host_s#cqrs23 */
#define SDIO_HOST_CQRS23_CQLCRA_OFFSET 0
#define SDIO_HOST_CQRS23_CQLCRA_SIZE 32

/* EOF sdio_host.h */
#endif
