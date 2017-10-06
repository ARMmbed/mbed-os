/*******************************************************************************
 *Copyright (c) 2013-2016 Realtek Semiconductor Corp, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 * 
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); 
 * you may not use this file except in compliance with the License.
 * 
 * You may obtain a copy of the License at https://www.mbed.com/licenses/PBL-1.0
 * 
 * See the License for the specific language governing permissions and limitations under the License.
 *******************************************************************************
 */


#ifndef _HAL_SPIFLASH__
#define _HAL_SPIFLASH__
//======================================================
// Header files

#define SPIC_CALIBRATION_IN_NVM         1   // if store the SPIC calibration data in the NVM
#ifndef CONFIG_IMAGE_SEPARATE       // Store SPIC Calibration only for seprated image
#undef SPIC_CALIBRATION_IN_NVM
#define SPIC_CALIBRATION_IN_NVM         0
#endif

//======================================================
// Definition
#define HAL_SPI_WRITE32(addr, value32)  HAL_WRITE32(SPI_FLASH_CTRL_BASE, addr, value32)
#define HAL_SPI_WRITE16(addr, value16)  HAL_WRITE16(SPI_FLASH_CTRL_BASE, addr, value16)
#define HAL_SPI_WRITE8(addr, value8)    HAL_WRITE8(SPI_FLASH_CTRL_BASE, addr, value8)
#define HAL_SPI_READ32(addr)            HAL_READ32(SPI_FLASH_CTRL_BASE, addr)
#define HAL_SPI_READ16(addr)            HAL_READ16(SPI_FLASH_CTRL_BASE, addr)
#define HAL_SPI_READ8(addr)             HAL_READ8(SPI_FLASH_CTRL_BASE, addr)

typedef struct _SPIC_PARA_MODE_ {
    u8 Valid:1;         // valid
    u8 CpuClk:3;        // CPU clock
    u8 BitMode:2;       // Bit mode
	u8 Reserved:2;		// reserved
} SPIC_PARA_MODE, *PSPIC_PARA_MODE;

typedef struct _SPIC_INIT_PARA_ {
    u8  BaudRate;
    u8  RdDummyCyle;
    u8  DelayLine;
    union {
        u8  Rsvd;
        u8  Valid;
        SPIC_PARA_MODE Mode;
    };
#if defined(E_CUT_ROM_DOMAIN) || (!defined(CONFIG_RELEASE_BUILD_LIBRARIES))
    u8 id[3];
    u8 flashtype;
#endif
}SPIC_INIT_PARA, *PSPIC_INIT_PARA;


enum _SPIC_BIT_MODE_ {
    SpicOneBitMode = 0,
    SpicDualBitMode = 1,
    SpicQuadBitMode = 2,
};

//======================================================
// Flash type used 
#define FLASH_OTHERS 0
#define FLASH_MXIC 1
#define FLASH_MXIC_4IO 2
#define FLASH_WINBOND 3
#define FLASH_MICRON 4
#define FLASH_EON 5

//#define FLASH_MXIC_MX25L4006E   0
//#define FLASH_MXIC_MX25L8073E   0
//#define FLASH_MICRON_N25Q512A 1
// The below parts are based on the flash characteristics
//====== Flash Command Definition  ======
//#if FLASH_MICRON_N25Q512A

/*Common command*/
#define FLASH_CMD_WREN      0x06            //write enable
#define FLASH_CMD_WRDI      0x04            //write disable
#define FLASH_CMD_WRSR      0x01            //write status register
#define FLASH_CMD_RDID      0x9F            //read idenfication
#define FLASH_CMD_RDSR      0x05            //read status register
#define FLASH_CMD_RDSFDP    0x5A            //Read SFDP
#define FLASH_CMD_READ      0x03            //read data
#define FLASH_CMD_FREAD     0x0B            //fast read data
#define FLASH_CMD_PP        0x02            //Page Program
#define FLASH_CMD_DREAD 0x3B            //Double Output Mode command 1-1-2
#define FLASH_CMD_2READ 0xBB            // 2 x I/O read  command 1-2-2
#define FLASH_CMD_QREAD 0x6B            // 1I / 4O read command 1-1-4
#define FLASH_CMD_4READ 0xEB            // 4 x I/O read  command 1-4-4
#define FLASH_CMD_DPP   0xA2            // 1-1-2
#define FLASH_CMD_2PP   0xD2            //  1-2-2
#define FLASH_CMD_QPP   0x32            //  1-1-4
#define FLASH_CMD_4PP   0x38            //quad page program 1-4-4
#define FLASH_CMD_SE        0x20            //Sector Erase
#define FLASH_CMD_BE    0xD8            //Block Erase(or 0x52)
#define FLASH_CMD_CE    0xC7            //Chip Erase(or 0xC7)
#define FLASH_CMD_DP    0xB9            //Deep Power Down
#define FLASH_CMD_RDP   0xAB            //Release from Deep Power-Down

/*Micron Special command*/
#define FLASH_CMD_DE    0xC4
#define FLASH_CMD_4PP2   0x12
#define FLASH_CMD_RFSR 0x70
#define FLASH_CMD_CFSR 0x50
#define FLASH_CMD_RNCR 0xB5
#define FLASH_CMD_WNCR 0xB1
#define FLASH_CMD_RVCR 0x85
#define FLASH_CMD_WVCR 0x81
#define FLASH_CMD_REVCR 0x65
#define FLASH_CMD_WEVCR 0x61
#define FLASH_CMD_REAR 0xC8
#define FLASH_CMD_WEAR 0xC5
#define FLASH_CMD_ENQUAD 0x35
#define FLASH_CMD_EXQUAD 0xF5

/*MXIC Special command*/
#define FLASH_CMD_RDCR      0x15        //read configurate register
#define FLASH_CMD_REMS2     0xEF            // read ID for 2x I/O mode
#define FLASH_CMD_REMS4     0xDF            // read ID for 4x I/O mode
#define FLASH_CMD_ENSO      0xB1            // enter secured OTP
#define FLASH_CMD_EXSO      0xC1            // exit secured OTP
#define FLASH_CMD_RDSCUR    0x2B            // read security register
#define FLASH_CMD_WRSCUR    0x2F            // write security register    

//#endif
#if 0
#if FLASH_MXIC_MX25L4006E
    #define FLASH_CMD_WREN      0x06            //write enable
    #define FLASH_CMD_WRDI      0x04            //write disable
    #define FLASH_CMD_WRSR      0x01            //write status register
    #define FLASH_CMD_RDID      0x9F            //read idenfication
    #define FLASH_CMD_RDSR      0x05            //read status register
    #define FLASH_CMD_READ      0x03            //read data
    #define FLASH_CMD_FREAD     0x0B            //fast read data
    #define FLASH_CMD_RDSFDP    0x5A            //Read SFDP
    #define FLASH_CMD_RES       0xAB            //Read Electronic ID
    #define FLASH_CMD_REMS      0x90            //Read Electronic Manufacturer & Device ID
    #define FLASH_CMD_DREAD     0x3B            //Double Output Mode command
    #define FLASH_CMD_SE        0x20            //Sector Erase
    #define FLASH_CMD_BE        0xD8            //Block Erase(or 0x52)
    #define FLASH_CMD_CE        0x60            //Chip Erase(or 0xC7)
    #define FLASH_CMD_PP        0x02            //Page Program
    #define FLASH_CMD_DP        0xB9            //Deep Power Down
    #define FLASH_CMD_RDP       0xAB            //Release from Deep Power-Down
    #define FLASH_CMD_RDCR      0x15        //read configurate register
    #define FLASH_CMD_2READ     0xBB            // 2 x I/O read  command
    #define FLASH_CMD_4READ     0xEB            // 4 x I/O read  command
    #define FLASH_CMD_QREAD     0x6B            // 1I / 4O read command
    #define FLASH_CMD_4PP       0x38            //quad page program
    #define FLASH_CMD_FF        0xFF            //Release Read Enhanced
    #define FLASH_CMD_REMS2     0xEF            // read ID for 2x I/O mode
    #define FLASH_CMD_REMS4     0xDF            // read ID for 4x I/O mode
    #define FLASH_CMD_ENSO      0xB1            // enter secured OTP
    #define FLASH_CMD_EXSO      0xC1            // exit secured OTP
    #define FLASH_CMD_RDSCUR    0x2B            // read security register
    #define FLASH_CMD_WRSCUR    0x2F            // write security register    
#elif  FLASH_MXIC_MX25L8073E
    #define FLASH_CMD_WREN      0x06            //write enable
    #define FLASH_CMD_WRDI      0x04            //write disable
    #define FLASH_CMD_WRSR      0x01            //write status register
    #define FLASH_CMD_RDID      0x9F            //read idenfication
    #define FLASH_CMD_RDSR      0x05            //read status register
    #define FLASH_CMD_READ      0x03            //read data
    #define FLASH_CMD_FREAD     0x0B            //fast read data
    #define FLASH_CMD_RDSFDP    0x5A            //Read SFDP
    #define FLASH_CMD_RES       0xAB            //Read Electronic ID
    #define FLASH_CMD_REMS      0x90            //Read Electronic Manufacturer & Device ID
    #define FLASH_CMD_DREAD     0x3B            //Double Output Mode command
    #define FLASH_CMD_SE        0x20            //Sector Erase
    #define FLASH_CMD_BE        0x52            //Block Erase
    #define FLASH_CMD_CE        0x60            //Chip Erase(or 0xC7)
    #define FLASH_CMD_PP        0x02            //Page Program
    #define FLASH_CMD_DP        0xB9            //Deep Power Down
    #define FLASH_CMD_RDP       0xAB            //Release from Deep Power-Down
    #define FLASH_CMD_2READ     0xBB            // 2 x I/O read  command
    #define FLASH_CMD_4READ     0xEB            // 4 x I/O read  command
    #define FLASH_CMD_QREAD     0x6B            // 1I / 4O read command
    #define FLASH_CMD_4PP       0x38            //quad page program
    #define FLASH_CMD_FF        0xFF            //Release Read Enhanced
    #define FLASH_CMD_REMS2     0xEF            // read ID for 2x I/O mode
    #define FLASH_CMD_REMS4     0xDF            // read ID for 4x I/O mode
    #define FLASH_CMD_ENSO      0xB1            // enter secured OTP
    #define FLASH_CMD_EXSO      0xC1            // exit secured OTP
    #define FLASH_CMD_RDSCUR    0x2B            // read security register
    #define FLASH_CMD_WRSCUR    0x2F            // write security register
#else
    #define FLASH_CMD_WREN      0x06            //write enable
    #define FLASH_CMD_WRDI      0x04            //write disable
    #define FLASH_CMD_WRSR      0x01            //write status register
    #define FLASH_CMD_RDID      0x9F            //read idenfication
    #define FLASH_CMD_RDSR      0x05            //read status register
    #define FLASH_CMD_READ      0x03            //read data
    #define FLASH_CMD_FREAD     0x0B            //fast read data
    #define FLASH_CMD_RDSFDP    0x5A            //Read SFDP
    #define FLASH_CMD_RES       0xAB            //Read Electronic ID
    #define FLASH_CMD_REMS      0x90            //Read Electronic Manufacturer & Device ID
    #define FLASH_CMD_DREAD     0x3B            //Double Output Mode command
    #define FLASH_CMD_SE        0x20            //Sector Erase
    #define FLASH_CMD_BE        0x52            //Block Erase
    #define FLASH_CMD_CE        0x60            //Chip Erase(or 0xC7)
    #define FLASH_CMD_PP        0x02            //Page Program
    #define FLASH_CMD_DP        0xB9            //Deep Power Down
    #define FLASH_CMD_RDP       0xAB            //Release from Deep Power-Down
    #define FLASH_CMD_2READ     0xBB            // 2 x I/O read  command
    #define FLASH_CMD_4READ     0xEB            // 4 x I/O read  command
    #define FLASH_CMD_QREAD     0x6B            // 1I / 4O read command
    #define FLASH_CMD_4PP       0x38            //quad page program
    #define FLASH_CMD_FF        0xFF            //Release Read Enhanced
    #define FLASH_CMD_REMS2     0xEF            // read ID for 2x I/O mode
    #define FLASH_CMD_REMS4     0xDF            // read ID for 4x I/O mode
    #define FLASH_CMD_ENSO      0xB1            // enter secured OTP
    #define FLASH_CMD_EXSO      0xC1            // exit secured OTP
    #define FLASH_CMD_RDSCUR    0x2B            // read security register
    #define FLASH_CMD_WRSCUR    0x2F            // write security register
#endif //#if FLASH_MXIC_MX25L4006E
#endif
// ============================

// =====  Flash Parameter Definition  =====
//#if FLASH_MICRON_N25Q512A
#if 0
#define FLASH_RD_2IO_EN         1
#define FLASH_RD_2O_EN          1
#define FLASH_RD_4IO_EN         1
#define FLASH_RD_4O_EN          1
#define FLASH_WR_2IO_EN         1
#define FLASH_WR_2O_EN          1
#define FLASH_WR_4IO_EN         1
#define FLASH_WR_4O_EN          1                             
#endif
#define FLASH_DM_CYCLE_2O  0x08 // 1-1-2
#define FLASH_DM_CYCLE_2IO 0x04 // 1-2-2
#define FLASH_DM_CYCLE_4O  0x08 // 1-1-4
#define FLASH_DM_CYCLE_4IO 0x08 // 1-4-4
#define FLASH_VLD_DUAL_CMDS (BIT_WR_BLOCKING | BIT_RD_DUAL_I)// 1-1-2
#define FLASH_VLD_QUAD_CMDS (BIT_WR_BLOCKING | BIT_RD_QUAD_IO)// 1-4-4
#define FLASH_VLD_QUAD_CMDS2 (BIT_WR_BLOCKING | BIT_RD_QUAD_O)// 1-1-4


//#endif
#if 0
#if FLASH_MXIC_MX25L4006E
#define FLASH_RD_2IO_EN         1
#define FLASH_RD_2O_EN          0
#define FLASH_RD_4IO_EN         1
#define FLASH_RD_4O_EN          0
#define FLASH_WR_2IO_EN        1
#define FLASH_WR_2O_EN          0
#define FLASH_WR_4IO_EN         1
#define FLASH_WR_4O_EN          0    
#define FLASH_DM_CYCLE_2O  0x04 // 1-1-2
#define FLASH_DM_CYCLE_2IO 0x08 // 1-2-2
#define FLASH_DM_CYCLE_4O  0x04 // 1-1-4
#define FLASH_DM_CYCLE_4IO 0x08 // 1-4-4
#define FLASH_VLD_DUAL_CMDS (BIT_WR_BLOCKING | BIT_WR_DUAL_II | BIT_RD_DUAL_IO)
#define FLASH_VLD_QUAD_CMDS (BIT_WR_BLOCKING | BIT_WR_QUAD_II | BIT_RD_QUAD_IO)
    
#elif  FLASH_MXIC_MX25L8073E //This flash model is just for prototype, if you want to use it, 
                             //the code MUST be rechecked according to the flash spec.
    #define FLASH_RD_2IO_EN         1
    #define FLASH_RD_2O_EN          0
    #define FLASH_RD_4IO_EN         1
    #define FLASH_RD_4O_EN          0
    #define FLASH_WR_2IO_EN         1
    #define FLASH_WR_2O_EN          0
    #define FLASH_WR_4IO_EN         1
    #define FLASH_WR_4O_EN          0                             
    
    #define FLASH_DM_CYCLE_2O  0x08
    #define FLASH_DM_CYCLE_2IO 0x04
    #define FLASH_DM_CYCLE_4O  0x08
    #define FLASH_DM_CYCLE_4IO 0x04
    
    #define FLASH_VLD_DUAL_CMDS (BIT_WR_BLOCKING | BIT_RD_DUAL_IO)
    #define FLASH_VLD_QUAD_CMDS (BIT_WR_BLOCKING | BIT_WR_QUAD_II | BIT_RD_QUAD_IO)
#else
    #define FLASH_RD_2IO_EN         1
    #define FLASH_RD_2O_EN          0
    #define FLASH_RD_4IO_EN         1
    #define FLASH_RD_4O_EN          0
    #define FLASH_WR_2IO_EN         1
    #define FLASH_WR_2O_EN          0
    #define FLASH_WR_4IO_EN         1
    #define FLASH_WR_4O_EN          0
    
    #define FLASH_DM_CYCLE_2O  0x08
    #define FLASH_DM_CYCLE_2IO 0x04
    #define FLASH_DM_CYCLE_4O  0x08
    #define FLASH_DM_CYCLE_4IO 0x04
    
    #define FLASH_VLD_DUAL_CMDS (BIT_WR_BLOCKING | BIT_RD_DUAL_IO)  
    #define FLASH_VLD_QUAD_CMDS (BIT_WR_BLOCKING | BIT_WR_QUAD_II | BIT_RD_QUAD_IO)
#endif
#endif
#if 0
//======================================================
// Function prototype
BOOLEAN SpicFlashInitRtl8195A(u8 SpicBitMode);

_LONG_CALL_
extern VOID SpicLoadInitParaFromClockRtl8195A(u8 CpuClkMode, u8 BaudRate, PSPIC_INIT_PARA pSpicInitPara); 

// spi-flash controller initialization
_LONG_CALL_
extern VOID SpicInitRtl8195A(u8 InitBaudRate, u8 SpicBitMode);

// wait sr[0] = 0, wait transmission done
_LONG_CALL_
extern VOID SpicWaitBusyDoneRtl8195A(VOID);

// wait spi-flash status register[0] = 0
//_LONG_CALL_
//extern VOID SpicWaitWipDoneRtl8195A(SPIC_INIT_PARA SpicInitPara);
#endif

//======================================================
// ROM Function prototype
_LONG_CALL_ VOID SpiFlashAppV02(IN  VOID *Data);
_LONG_CALL_ROM_ VOID SpicInitRtl8195AV02(IN  u8 InitBaudRate,IN  u8 SpicBitMode);

_LONG_CALL_ROM_ VOID SpicEraseFlashRtl8195AV02(VOID);

_LONG_CALL_ROM_ VOID SpicLoadInitParaFromClockRtl8195AV02(IN  u8 CpuClkMode,IN  u8 BaudRate,IN  PSPIC_INIT_PARA pSpicInitPara);


VOID SpicBlockEraseFlashRtl8195A(IN u32 Address);
VOID SpicSectorEraseFlashRtl8195A(IN u32 Address);
VOID SpicDieEraseFlashRtl8195A(IN u32 Address);
VOID SpicWriteProtectFlashRtl8195A(IN u32 Protect);
VOID SpicWaitWipDoneRefinedRtl8195A(IN  SPIC_INIT_PARA SpicInitPara);
VOID SpicWaitOperationDoneRtl8195A(IN SPIC_INIT_PARA SpicInitPara);
VOID SpicRxCmdRefinedRtl8195A(IN  u8 cmd,IN  SPIC_INIT_PARA SpicInitPara);
u8 SpicGetFlashStatusRefinedRtl8195A(IN  SPIC_INIT_PARA SpicInitPara);
VOID SpicInitRefinedRtl8195A(IN  u8 InitBaudRate,IN  u8 SpicBitMode);
u32 SpicWaitWipRtl8195A(VOID);
u32 SpicOneBitCalibrationRtl8195A(IN u8 SysCpuClk);
VOID SpicDisableRtl8195A(VOID);
VOID SpicDeepPowerDownFlashRtl8195A(VOID);
VOID SpicUserProgramRtl8195A(IN u8 * data, IN SPIC_INIT_PARA SpicInitPara, IN u32 addr, IN u32 * LengthInfo);
VOID SpicUserReadRtl8195A(IN u32 Length, IN u32 addr, IN u8 * data, IN u8 BitMode);
VOID SpicUserReadFourByteRtl8195A(IN u32 Length, IN u32 addr, IN u32 * data, IN u8 BitMode);
VOID SpicReadIDRtl8195A(VOID);
VOID SpicSetFlashStatusRefinedRtl8195A(IN u32 data, IN SPIC_INIT_PARA SpicInitPara);
VOID SpicSetExtendAddrRtl8195A(IN u32 data, IN SPIC_INIT_PARA SpicInitPara);
u8 SpicGetExtendAddrRtl8195A(IN SPIC_INIT_PARA SpicInitPara);
#if SPIC_CALIBRATION_IN_NVM
VOID SpicNVMCalLoad(u8 BitMode, u8 CpuClk);
VOID SpicNVMCalLoadAll(void);
VOID SpicNVMCalStore(u8 BitMode, u8 CpuClk);
#endif  // #if SPIC_CALIBRATION_IN_NVM

#endif //_HAL_SPIFLASH__
