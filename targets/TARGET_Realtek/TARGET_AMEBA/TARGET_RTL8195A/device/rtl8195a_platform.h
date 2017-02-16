/*
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#if 0
#ifndef MBED_RTL8195A_PLATFORM_H
#define MBED_RTL8195A_PLATFORM_H

#define CHIP_ID_8711AM              0xFF
#define CHIP_ID_8195AM              0xFE
#define CHIP_ID_8711AF              0xFD
#define CHIP_ID_8710AF              0xFC
#define CHIP_ID_8711AN              0xFB
#define CHIP_ID_8710AM              0xFA

#define FW_VERSION                  (0x0100)
#define FW_SUBVERSION               (0x0001)
#define FW_CHIP_ID                  (0x8195)
#define FW_CHIP_VER                 (0x01)
#define FW_BUS_TYPE                 (0x01)   // the iNIC firmware type: USB/SDIO
#define FW_INFO_RSV1                (0x00)   // the firmware information reserved
#define FW_INFO_RSV2                (0x00)   // the firmware information reserved
#define FW_INFO_RSV3                (0x00)   // the firmware information reserved
#define FW_INFO_RSV4                (0x00)   // the firmware information reserved

///// ** wlan ** /////
#define FLASH_RESERVED_DATA_BASE        0x8000  // reserve 32K for Image1
#define FLASH_SYSTEM_DATA_ADDR          0x9000  // reserve 32K+4K for Image1 + Reserved data
// Flash Map for Calibration data
#define FLASH_CAL_DATA_BASE             0xA000
#define FLASH_CAL_DATA_ADDR(_offset)    (FLASH_CAL_DATA_BASE + _offset)
#define FLASH_CAL_DATA_SIZE             0x1000
#define FLASH_SECTOR_SIZE               0x1000
// SPIC Calibration Data
#define FLASH_SPIC_PARA_OFFSET          0x80
#define FLASH_SPIC_PARA_BASE            (FLASH_SYSTEM_DATA_ADDR+FLASH_SPIC_PARA_OFFSET)
// SDRC Calibration Data
#define FLASH_SDRC_PARA_OFFSET          0x180
#define FLASH_SDRC_PARA_BASE            (FLASH_SYSTEM_DATA_ADDR+FLASH_SDRC_PARA_OFFSET)
// ADC Calibration Data
#define FLASH_ADC_PARA_OFFSET           0x200
#define FLASH_ADC_PARA_BASE             (FLASH_SYSTEM_DATA_ADDR+FLASH_ADC_PARA_OFFSET)
/**********/

#define ROMVERSION                  0x03
#define ROMINFORMATION              ROMVERSION

//#define SYSTEM_CLK                  PLATFORM_CLK
//#define TIMER_CLK                   (32*1000)
//#define PLATFORM_CLK                (200000000UL/6*5) // 166MHz

#define PERIPHERAL_IRQ_ALL_LEVEL    0
#define PERIPHERAL_IRQ_STATUS       0x04
#define PERIPHERAL_IRQ_MODE         0x08
#define PERIPHERAL_IRQ_EN           0x0C
#define LP_PERI_EXT_IRQ_STATUS      0x24
#define LP_PERI_EXT_IRQ_MODE        0x28
#define LP_PERI_EXT_IRQ_EN          0x2C

//3 Peripheral IP Base Address
#define DRAM_BASE                   0x30000000
#define CTRL_BASE                   0x40000000
//#define SYSTEM_CTRL_BASE            (CTRL_BASE)
#define PERI_BASE                   0x40000000
//#define PERI_ON_BASE                (PERI_BASE)
#define GPIO_BASE                   0x40001000
#define TIMER_BASE                  0x40002000
#define NFC_BASE                    0x40002400
#define VENDOR_BASE                 0x40002800
#define UART3_BASE                  0x40003000
#define LUART_BASE                  (UART3_BASE)
#define I2C2_BASE                   0x40003400
#define I2C3_BASE                   0x40003800
#define SDRC_BASE                   0x40005000
#define SPIC_BASE                   0x40006000
//#define SPI_FLASH_CTRL_BASE         (SPIC_BASE)
#define ADC_BASE                    0x40010000
#define DAC_BASE                    0x40011000
#define UART0_BASE                  0x40040000
#define UART1_BASE                  0x40040400
#define UART2_BASE                  0x40040800
#define RUART_BASE                  (UART0_BASE)
#define SPI0_BASE                   0x40042000
#define SPI1_BASE                   0x40042400
#define SPI2_BASE                   0x40042800
#define SPI_BASE                    (SPI0_BASE)
#define SSI0_BASE                   0x40042000
#define SSI1_BASE                   0x40042400
#define SSI2_BASE                   0x40042800
#define SSI_BASE                    (SSI0_BASE)
#define I2C0_BASE                   0x40044000
#define I2C1_BASE                   0x40044400
#define MII_BASE                    0x40050000
#define SDIOD_BASE                  0x40050000
#define SDIOH_BASE                  0x40058000
#define GDMA0_BASE                  0x40060000
#define GDMA1_BASE                  0x40061000
#define GDMA_BASE                   (GDMA0_BASE)
#define I2S0_BASE                   0x40062000
#define I2S1_BASE                   0x40063000
#define PCM0_BASE                   0x40064000
#define PCM1_BASE                   0x40065000
#define CRYPTO_BASE                 0x40070000
#define WIFI_BASE                   0x40080000
#define USB_OTG_BASE                0x400C0000
#define SPI_FLASH_BASE              0x98000000

#define GDMA_OFFSET                 0x1000
#define I2S1_OFFSET                 0x1000
#define PCM1_OFFSET                 0x1000
#define SSI_OFFSET                  0x400
#define RUART_OFFSET                0x400

#define GDMA_SIZE                   (GDMA_OFFSET)
#define SSI_SIZE                    (SSI_OFFSET)
#define RUART_SIZE                  (RUART_OFFSET)

#define CPU_CLK_TYPE_NO             6
#if 0
enum _BOOT_TYPE_ {
    BOOT_FROM_FLASH = 0,
    BOOT_FROM_SDIO  = 1,
    BOOT_FROM_USB   = 2,
    BOOT_FROM_RSVD  = 3,
};

enum _EFUSE_CPU_CLK_ {
    CLK_200M   = 0,
    CLK_100M   = 1,
    CLK_50M    = 2,
    CLK_25M    = 3,
    CLK_12_5M  = 4,
    CLK_4M     = 5,
};
#endif

// Clock Control Register 0 (REG_PESOC_CLK_CTRL)
#define BIT_CKE_OCP                BIT(0)
#define BIT_CKE_PLFM               BIT(2)
#define BIT_ACTCK_TRACE            BIT(4)
#define BIT_SLPCK_TRACE            BIT(5)
#define BIT_SLPCK_VENDOR           BIT(7)
#define BIT_ACTCK_VENDOR           BIT(6)
#define BIT_ACTCK_FLASH            BIT(8)
#define BIT_SLPCK_FLASH            BIT(9)
#define BIT_ACTCK_SDRC             BIT(10)
#define BIT_SLPCK_SDRC             BIT(11)
#define BIT_ACTCK_UART3            BIT(12)
#define BIT_ACTCK_LUART            BIT(12)
#define BIT_SLPCK_UART3            BIT(13)
#define BIT_SLPCK_LUART            BIT(13)
#define BIT_ACTCK_TIMER            BIT(14)
#define BIT_SLPCK_TIMER            BIT(15)
#define BIT_ACTCK_GDMA0            BIT(16)
#define BIT_SLPCK_GDMA0            BIT(17)
#define BIT_ACTCK_GDMA1            BIT(18)
#define BIT_SLPCK_GDMA1            BIT(19)
#define BIT_ACTCK_GPIO             BIT(24)
#define BIT_SLPCK_GPIO             BIT(25)
#define BIT_ACTCK_BTCMD            BIT(28)
#define BIT_SLPCK_BTCMD            BIT(29)

// Clock Control Register 1 (REG_PESOC_PERI_CLK_CTRL0)
#define BIT_ACTCK_UART0            BIT(0)
#define BIT_ACTCK_RUART0           BIT(0)
#define BIT_SLPCK_UART0            BIT(1)
#define BIT_SLPCK_RUART0           BIT(1)
#define BIT_ACTCK_UART1            BIT(2)
#define BIT_ACTCK_RUART1           BIT(2)
#define BIT_SLPCK_UART1            BIT(3)
#define BIT_SLPCK_RUART1           BIT(3)
#define BIT_ACTCK_UART2            BIT(4)
#define BIT_ACTCK_RUART2           BIT(4)
#define BIT_SLPCK_UART2            BIT(5)
#define BIT_SLPCK_RUART2           BIT(5)
#define BIT_ACTCK_SPI0             BIT(16)
#define BIT_SLPCK_SPI0             BIT(17)
#define BIT_ACTCK_SPI1             BIT(18)
#define BIT_SLPCK_SPI1             BIT(19)
#define BIT_ACTCK_SPI2             BIT(20)
#define BIT_SLPCK_SPI2             BIT(21)

// Clock Control Register 2 (REG_PESOC_PERI_CLK_CTRL1)
#define BIT_ACTCK_I2C0             BIT(0)
#define BIT_SLPCK_I2C0             BIT(1)
#define BIT_ACTCK_I2C1             BIT(2)
#define BIT_SLPCK_I2C1             BIT(3)
#define BIT_ACTCK_I2C2             BIT(4)
#define BIT_SLPCK_I2C2             BIT(5)
#define BIT_ACTCK_I2C3             BIT(6)
#define BIT_SLPCK_I2C3             BIT(7)
#define BIT_ACTCK_I2S              BIT(16)
#define BIT_SLPCK_I2S              BIT(17)
#define BIT_ACTCK_PCM              BIT(20)
#define BIT_SLPCK_PCM              BIT(21)
#define BIT_ACTCK_ADC              BIT(24)
#define BIT_SLPCK_ADC              BIT(25)
#define BIT_ACTCK_DAC              BIT(28)
#define BIT_SLPCK_DAC              BIT(29)

// Clock Control Register 3 (REG_PESOC_CLK_CTRL3)

// HCI_CLK_CTRL0
#define BIT_ACTCK_SDIOD            BIT(0)
#define BIT_SLPCK_SDIOD            BIT(1)
#define BIT_ACTCK_SDIOH            BIT(2)
#define BIT_SLPCK_SDIOH            BIT(3)
#define BIT_ACTCK_HCI_OTG          BIT(4)
#define BIT_SLPCK_HCI_OTG          BIT(5)
#define BIT_ACTCK_MII_MPHY         BIT(24)
#define BIT_SLPCK_MII_MPHY         BIT(25)

// COM_CLK_CTRL1
#define BIT_ACTCK_WLAN             BIT(0)
#define BIT_SLPCK_WLAN             BIT(1)
#define BIT_ACTCK_SEC_ENG          BIT(4)
#define BIT_SLPCK_SEC_ENG          BIT(5)
#define BIT_ACTCK_NFC              BIT(16)
#define BIT_SLPCK_NFC              BIT(17)
#define BIT_CAL_NFC                BIT(18)

//2 REG_PESOC_CLK_SEL
#define BIT_CSEL_SPI1              BIT(18)
#define BIT_CSEL_SCLK_SHF          16
#define BIT_CSEL_SCLK_MSK          0x3
#define BIT_CSEL_SCLK(x)           (((x) & BIT_CSEL_SCLK_MSK) << BIT_CSEL_SCLK_SHF)

#define BIT_CSEL_SDR_SHF           10
#define BIT_CSEL_SDR_MSK           0x3
#define BIT_CSEL_SDR(x)            (((x) & BIT_CSEL_SDR_MSK) << BIT_CSEL_SDR_SHF)

#define BIT_CSEL_FLASH_SHF         8
#define BIT_CSEL_FLASH_MSK         0x3
#define BIT_CSEL_FLASH(x)          (((x) & BIT_CSEL_FLASH_MSK) << BIT_CSEL_FLASH_SHF)

#define BIT_CSEL_TRACE_SHF         4
#define BIT_CSEL_TRACE_MSK         0x3
#define BIT_CSEL_TRACE(x)          (((x) & BIT_CSEL_TRACE_MSK) << BIT_CSEL_TRACE_SHF)

// REG_SYS_ANACK_CAL_CTRL
#define BIT_SYS_ANACK_CAL_CMD      BIT(15)
#define BIT_SYS_ANACK_CAL_SEL      BIT(14)
#define BIT_SYS_ANACK_CAL_RPT_SHF  0
#define BIT_SYS_ANACK_CAL_RPT_MSK  0x3fff
#define BIT_SYS_ANACK_CAL_RPT(x)   (((x) & BIT_SYS_ANACK_CAL_RPT_MSK) << BIT_SYS_ANACK_CAL_RPT_SHF)

// REG_OSC32K_CTRL
#define BIT_32K_BIAS_CURRENT_SHF   16
#define BIT_32K_BIAS_CURRENT_MSK   0xffff
#define BIT_32K_BIAS_CURRENT(x)    (((x) & BIT_32K_BIAS_CURRENT_MSK) << BIT_32K_BIAS_CURRENT_SHF)

#define BIT_32K_RESISTOR_COM_SHF   2
#define BIT_32K_RESISTOR_COM_MSK   0x3
#define BIT_32K_RESISTOR_COM(x)    (((x) & BIT_32K_RESISTOR_COM_MSK) << BIT_32K_RESISTOR_COM_SHF)

#define BIT_32K_DBG_SEL            BIT(1)
#define BIT_32K_POW_CKGEN_EN       BIT(0)

// Function Control Register 0 (REG_SOC_FUNC_EN)
#define BIT_FCTRL_ENABLE           BIT(0)
#define BIT_FCTRL_OCP              BIT(1)
#define BIT_FCTRL_LXBUS            BIT(2)
#define BIT_FCTRL_FLASH            BIT(4)
#define BIT_FCTRL_MEM_CTRL         BIT(6)
#define BIT_FCTRL_CPU              BIT(8)
#define BIT_FCTRL_UART3            BIT(12)
#define BIT_FCTRL_LUART            BIT(12)
#define BIT_FCTRL_GDMA0            BIT(13)
#define BIT_FCTRL_GDMA1            BIT(14)
#define BIT_FCTRL_GTIMER           BIT(16)
#define BIT_FCTRL_SEC_ENG          BIT(20)

// Function Control Register 1 (REG_SOC_HCI_COM_FUNC_EN)
#define BIT_FCTRL_HCI_SDIOD_ON           BIT(0)
#define BIT_FCTRL_HCI_SDIOD_OFF          BIT(1)
#define BIT_FCTRL_HCI_SDIOH_ON           BIT(2)
#define BIT_FCTRL_HCI_SDIOD_ON_RST_MUX   BIT(3)
#define BIT_FCTRL_HCI_OTG                BIT(4)
#define BIT_FCTRL_HCI_OTG_RST_MUX        BIT(5)
#define BIT_FCTRL_HCI_MII                BIT(12)
#define BIT_FCTRL_HCI_SM_SEL             BIT(13)
#define BIT_FCTRL_HCI_WL_MACON           BIT(16)

// Function Control Register 1 (REG_SOC_PERI_FUNC0_EN)
#define BIT_FCTRL_UART0            BIT(0)
#define BIT_FCTRL_UART1            BIT(1)
#define BIT_FCTRL_UART2            BIT(2)
#define BIT_FCTRL_SPI0             BIT(8)
#define BIT_FCTRL_SPI1             BIT(9)
#define BIT_FCTRL_SPI2             BIT(10)
#define BIT_FCTRL_I2C0             BIT(16)
#define BIT_FCTRL_I2C1             BIT(17)
#define BIT_FCTRL_I2C2             BIT(18)
#define BIT_FCTRL_I2C3             BIT(19)
#define BIT_FCTRL_I2S0             BIT(24)
#define BIT_FCTRL_I2S1             BIT(25)
#define BIT_FCTRL_PCM0             BIT(28)
#define BIT_FCTRL_PCM1             BIT(29)

// Function Control Register 1 (REG_SOC_PERI_FUNC1_EN)
#define BIT_FCTRL_ADC0             BIT(0)
#define BIT_FCTRL_DAC0             BIT(4)
#define BIT_FCTRL_DAC1             BIT(5)
#define BIT_FCTRL_GPIO             BIT(8)

// Function Control BD (REG_SOC_PERI_BD_FUNC0_EN)
#define BIT_FCTRL_BD_UART0         BIT(0)
#define BIT_FCTRL_BD_UART1         BIT(1)
#define BIT_FCTRL_BD_UART2         BIT(2)

#define PERI_PWR_CTRL              0x0200  // REG_PEON_PWR_CTRL
#define PERI_PON_ISO_CTRL          0x0204  // REG_PON_ISO_CTRL
#define PERI_FCTRL_CTRL0           0x0210  // REG_SOC_FUNC_EN
#define PERI_FCTRL_HCI_COM         0x0214  // REG_SOC_HCI_COM_FUNC_EN
#define PERI_FCTRL_CTRL1           0x0218  // REG_SOC_PERI_FUNC0_EN
#define PERI_FCTRL_CTRL2           0x021C  // REG_SOC_PERI_FUNC1_EN
#define PERI_FCTRL_BD              0x0220  // REG_SOC_PERI_BD_FUNC0_EN
#define PERI_CCTRL_CTRL0           0x0230  // REG_PESOC_CLK_CTRL
#define PERI_CCTRL_CTRL1           0x0234  // REG_PESOC_PERI_CLK_CTRL0
#define PERI_CCTRL_CTRL2           0x0238  // REG_PESOC_PERI_CLK_CTRL1
#define PERI_CCTRL_CTRL3           0x023C  // REG_PESOC_CLK_CTRL3
#define PERI_CCTRL_HCI             0x0240  // REG_PESOC_HCI_CLK_CTRL0
#define PERI_CCTRL_COM             0x0244  // REG_PESOC_HCI_CLK_CTRL1
#define PERI_CCTRL_HW_ENG          0x0248  // REG_PESOC_HW_ENG_CLK_CTRL

#define PERI_CSEL_CTRL0            0x0250  // REG_PESOC_CLK_SEL
#define SYS_ANACK_CAL_CTRL         0x026C
#define OSC32K_CTRL                0x0270
#define OSC32K_CTRL0               0x0274
#define OSC32K_CTRL1               0x0278
#define THERMAL_METER_CTRL         0x027C
#define PERI_MCTRL_UART            0x0280
#define PERI_MCTRL_SPI             0x0284
#define PERI_MCTRL_I2C             0x0288  // REG_I2C_MUX_CTRL
#define PERI_MCTRL_I2S             0x028C  // REG_I2S_MUX_CTRL
#define PERI_MCTRL_HCI             0x02A0
#define PERI_MCTRL_WL              0x02A4
#define PERI_MCTRL_BT              0x02A8
#define PERI_MCTRL_PWM             0x02AC
#define PERI_FCTRL_CPU             0x02C0
#define HCI_CTRL_STATUS_0          0x02E0
#define HCI_CTRL_STATUS_1          0x02E4
#define PESOC_MEM_CTRL             0x0300
#define REG_PESOC_MEM_CTRL         (PESOC_MEM_CTRL)
#define PESOC_SOC_CTRL             0x0304  // REG_PESOC_SOC_CTRL
#define PESOC_PERI_CTRL            0x0308
#define GPIO_SHTDN_CTRL            0x0320
#define GPIO_DRIVING_CTRL          0x0324
#define GPIO_PULL_CTRL0            0x0330
#define GPIO_PULL_CTRL1            0x0334
#define GPIO_PULL_CTRL2            0x0338
#define GPIO_PULL_CTRL3            0x033C
#define GPIO_PULL_CTRL4            0x0340
#define GPIO_PULL_CTRL5            0x0344
#define GPIO_PULL_CTRL6            0x0348
#define PERI_PWM0_CTRL             0x0360
#define PERI_PWM1_CTRL             0x0364
#define PERI_PWM2_CTRL             0x0368
#define PERI_PWM3_CTRL             0x036C
#define PERI_TIM_EVT_CTRL          0x0370
#define PERI_EGTIM_CTRL            0x0374
#define PEON_CFG                   0x03F0
#define PEON_STATUS                0x03F4

#define CPU_OPT_WIDTH              0x1F
#define BIT_SYS_PWR_SOC            BIT(2)
#define BIT_SYS_PWR_RET_MEM        BIT(1)
#define BIT_SYS_PWR_PEON           BIT(0)
#define BIT_SYS_ISO_SYSPLL         BIT(7)
#define BIT_SYS_ISO_SOC            BIT(2)
#define BIT_SYS_ISO_RET_MEM        BIT(1)
#define BIT_SYS_ISO_PEON           BIT(0)
#define BIT_SYS_AMACRO             BIT(31)
#define BIT_SYS_PWRON_TRAP_SHTDN_N BIT(30)
#define BIT_SYS_FEN_SIC_MST        BIT(25)
#define BIT_SYS_FEN_SIC            BIT(24)
#define BIT_SOC_SYSPEON            BIT(4)
#define BIT_SYS_FEN_EELDR          BIT(0)
#define BIT_SOC_OCP_IOBUS_CK       BIT(2)
#define BIT_SYS_CK_EELDR           BIT(1)
#define BIT_SYS_CK_SYSREG          BIT(0)

#define BIT_PESOC_OCP_CPU_CK_SEL_SHF 4
#define BIT_PESOC_OCP_CPU_CK_SEL_MSK 0x7
#define BIT_PESOC_OCP_CPU_CK_SEL(x) (((x) & BIT_PESOC_OCP_CPU_CK_SEL_MSK) << BIT_PESOC_OCP_CPU_CK_SEL_SHF)

#define BIT_PESOC_EELDR_CK_SEL BIT(0)
#define BIT_SYS_EEROM_SWR_PAR_05_00_SHF 24
#define BIT_SYS_EEROM_SWR_PAR_05_00_MSK 0x3f
#define BIT_SYS_EEROM_SWR_PAR_05_00(x) (((x) & BIT_SYS_EEROM_SWR_PAR_05_00_MSK) << BIT_SYS_EEROM_SWR_PAR_05_00_SHF)

#define BIT_SYS_EEROM_LDO_PAR_07_04_SHF 20
#define BIT_SYS_EEROM_LDO_PAR_07_04_MSK 0xf
#define BIT_SYS_EEROM_LDO_PAR_07_04(x) (((x) & BIT_SYS_EEROM_LDO_PAR_07_04_MSK) << BIT_SYS_EEROM_LDO_PAR_07_04_SHF)

#define BIT_SYS_CHIPPDN_EN BIT(17)
#define BIT_SYS_EEROM_B12V_EN BIT(16)

#define BIT_SYS_EEROM_VID1_SHF 8
#define BIT_SYS_EEROM_VID1_MSK 0xff
#define BIT_SYS_EEROM_VID1(x) (((x) & BIT_SYS_EEROM_VID1_MSK) << BIT_SYS_EEROM_VID1_SHF)

#define BIT_SYS_EEROM_VID0_SHF 0
#define BIT_SYS_EEROM_VID0_MSK 0xff
#define BIT_SYS_EEROM_VID0(x) (((x) & BIT_SYS_EEROM_VID0_MSK) << BIT_SYS_EEROM_VID0_SHF)

//2 REG_SYS_EFUSE_SYSCFG1

#define BIT_SYS_PDSPL_STL_SHF 24
#define BIT_SYS_PDSPL_STL_MSK 0x3
#define BIT_SYS_PDSPL_STL(x) (((x) & BIT_SYS_PDSPL_STL_MSK) << BIT_SYS_PDSPL_STL_SHF)

#define BIT_SYS_PDSOC_STL_SHF 22
#define BIT_SYS_PDSOC_STL_MSK 0x3
#define BIT_SYS_PDSOC_STL(x) (((x) & BIT_SYS_PDSOC_STL_MSK) << BIT_SYS_PDSOC_STL_SHF)

#define BIT_SYS_PDPON_STL_SHF 20
#define BIT_SYS_PDPON_STL_MSK 0x3
#define BIT_SYS_PDPON_STL(x) (((x) & BIT_SYS_PDPON_STL_MSK) << BIT_SYS_PDPON_STL_SHF)

#define BIT_SYS_SWREG_XRT_SHF 18
#define BIT_SYS_SWREG_XRT_MSK 0x3
#define BIT_SYS_SWREG_XRT(x) (((x) & BIT_SYS_SWREG_XRT_MSK) << BIT_SYS_SWREG_XRT_SHF)

#define BIT_SYS_SWSLC_STL_SHF 16
#define BIT_SYS_SWSLC_STL_MSK 0x3
#define BIT_SYS_SWSLC_STL(x) (((x) & BIT_SYS_SWSLC_STL_MSK) << BIT_SYS_SWSLC_STL_SHF)

#define BIT_SYS_EEROM_SWR_PAR_46_45_SHF 14
#define BIT_SYS_EEROM_SWR_PAR_46_45_MSK 0x3
#define BIT_SYS_EEROM_SWR_PAR_46_45(x) (((x) & BIT_SYS_EEROM_SWR_PAR_46_45_MSK) << BIT_SYS_EEROM_SWR_PAR_46_45_SHF)

#define BIT_SYS_EEROM_SWR_PAR_40_39_SHF 12
#define BIT_SYS_EEROM_SWR_PAR_40_39_MSK 0x3
#define BIT_SYS_EEROM_SWR_PAR_40_39(x) (((x) & BIT_SYS_EEROM_SWR_PAR_40_39_MSK) << BIT_SYS_EEROM_SWR_PAR_40_39_SHF)

#define BIT_SYS_EEROM_SWR_PAR_33_26_SHF 4
#define BIT_SYS_EEROM_SWR_PAR_33_26_MSK 0xff
#define BIT_SYS_EEROM_SWR_PAR_33_26(x) (((x) & BIT_SYS_EEROM_SWR_PAR_33_26_MSK) << BIT_SYS_EEROM_SWR_PAR_33_26_SHF)

#define BIT_SYS_EEROM_SWSLD_VOL_SHF 0
#define BIT_SYS_EEROM_SWSLD_VOL_MSK 0x7
#define BIT_SYS_EEROM_SWSLD_VOL(x) (((x) & BIT_SYS_EEROM_SWSLD_VOL_MSK) << BIT_SYS_EEROM_SWSLD_VOL_SHF)

//2 REG_SYS_EFUSE_SYSCFG2

#define BIT_SYS_EERROM_ANAPAR_SPLL_24_15_SHF 21
#define BIT_SYS_EERROM_ANAPAR_SPLL_24_15_MSK 0x3ff
#define BIT_SYS_EERROM_ANAPAR_SPLL_24_15(x) (((x) & BIT_SYS_EERROM_ANAPAR_SPLL_24_15_MSK) << BIT_SYS_EERROM_ANAPAR_SPLL_24_15_SHF)

#define BIT_SYS_EEROM_ANAPAR_SPLL_05_02_SHF 16
#define BIT_SYS_EEROM_ANAPAR_SPLL_05_02_MSK 0xf
#define BIT_SYS_EEROM_ANAPAR_SPLL_05_02(x) (((x) & BIT_SYS_EEROM_ANAPAR_SPLL_05_02_MSK) << BIT_SYS_EEROM_ANAPAR_SPLL_05_02_SHF)

#define BIT_SYS_EEROM_XTAL_STEL_SEL_SHF 12
#define BIT_SYS_EEROM_XTAL_STEL_SEL_MSK 0x3
#define BIT_SYS_EEROM_XTAL_STEL_SEL(x) (((x) & BIT_SYS_EEROM_XTAL_STEL_SEL_MSK) << BIT_SYS_EEROM_XTAL_STEL_SEL_SHF)

#define BIT_SYS_EEROM_XTAL_FREQ_SEL_SHF 8
#define BIT_SYS_EEROM_XTAL_FREQ_SEL_MSK 0xf
#define BIT_SYS_EEROM_XTAL_FREQ_SEL(x) (((x) & BIT_SYS_EEROM_XTAL_FREQ_SEL_MSK) << BIT_SYS_EEROM_XTAL_FREQ_SEL_SHF)

//2 REG_SYS_EFUSE_SYSCFG3
#define BIT_SYS_DBG_PINGP_SHF 28
#define BIT_SYS_DBG_PINGP_MSK 0xf
#define BIT_SYS_DBG_PINGP(x) (((x) & BIT_SYS_DBG_PINGP_MSK) << BIT_SYS_DBG_PINGP_SHF)

#define BIT_SYS_DBG_SEL_SHF 16
#define BIT_SYS_DBG_SEL_MSK 0xfff
#define BIT_SYS_DBG_SEL(x) (((x) & BIT_SYS_DBG_SEL_MSK) << BIT_SYS_DBG_SEL_SHF)

#define BIT_SYS_DBGBY3_LOC_SEL_SHF 14
#define BIT_SYS_DBGBY3_LOC_SEL_MSK 0x3
#define BIT_SYS_DBGBY3_LOC_SEL(x) (((x) & BIT_SYS_DBGBY3_LOC_SEL_MSK) << BIT_SYS_DBGBY3_LOC_SEL_SHF)

#define BIT_SYS_DBGBY2_LOC_SEL_SHF 12
#define BIT_SYS_DBGBY2_LOC_SEL_MSK 0x3
#define BIT_SYS_DBGBY2_LOC_SEL(x) (((x) & BIT_SYS_DBGBY2_LOC_SEL_MSK) << BIT_SYS_DBGBY2_LOC_SEL_SHF)

#define BIT_SYS_DBGBY1_LOC_SEL_SHF 10
#define BIT_SYS_DBGBY1_LOC_SEL_MSK 0x3
#define BIT_SYS_DBGBY1_LOC_SEL(x) (((x) & BIT_SYS_DBGBY1_LOC_SEL_MSK) << BIT_SYS_DBGBY1_LOC_SEL_SHF)

#define BIT_SYS_DBGBY0_LOC_SEL_SHF 8
#define BIT_SYS_DBGBY0_LOC_SEL_MSK 0x3
#define BIT_SYS_DBGBY0_LOC_SEL(x) (((x) & BIT_SYS_DBGBY0_LOC_SEL_MSK) << BIT_SYS_DBGBY0_LOC_SEL_SHF)

#define BIT_SYS_EEROM_ANAPAR_SPLL_49 BIT(3)

#define BIT_SYS_EEROM_ANAPAR_SPLL_27_25_SHF 0
#define BIT_SYS_EEROM_ANAPAR_SPLL_27_25_MSK 0x7
#define BIT_SYS_EEROM_ANAPAR_SPLL_27_25(x) (((x) & BIT_SYS_EEROM_ANAPAR_SPLL_27_25_MSK) << BIT_SYS_EEROM_ANAPAR_SPLL_27_25_SHF)

#define BIT_SYS_GPIOA_E2_SHF      1
#define BIT_SYS_GPIOA_E2_MSK      0x7
#define BIT_SYS_GPIOA_E2(x)       (((x) & BIT_SYS_GPIOA_E2_MSK) << BIT_SYS_GPIOA_E2_SHF)

#define BIT_SYS_GPIOA_H3L1        BIT(0)

#define BIT_SYS_SPIC_INIT_BAUD_RATE_SEL_SHF 26
#define BIT_SYS_SPIC_INIT_BAUD_RATE_SEL_MSK 0x3
#define BIT_SYS_SPIC_INIT_BAUD_RATE_SEL(x) (((x) & BIT_SYS_SPIC_INIT_BAUD_RATE_SEL_MSK) << BIT_SYS_SPIC_INIT_BAUD_RATE_SEL_SHF)

#define BIT_SYS_CPU_CLK_SEL_SHF   24
#define BIT_SYS_CPU_CLK_SEL_MSK   0x3
#define BIT_SYS_CPU_CLK_SEL(x)    (((x) & BIT_SYS_CPU_CLK_SEL_MSK) << BIT_SYS_CPU_CLK_SEL_SHF)

//2 REG_SYS_EFUSE_SYSCFG7
#define BIT_SYS_MEM_RMV_SIGN      BIT(31)
#define BIT_SYS_MEM_RMV_1PRF1     BIT(29)
#define BIT_SYS_MEM_RMV_1PRF0     BIT(28)
#define BIT_SYS_MEM_RMV_1PSR      BIT(27)
#define BIT_SYS_MEM_RMV_1PHSR     BIT(26)
#define BIT_SYS_MEM_RMV_ROM       BIT(25)

#define BIT_SYS_MEM_RME_CPU_SHF   22
#define BIT_SYS_MEM_RME_CPU_MSK   0x7
#define BIT_SYS_MEM_RME_CPU(x)    (((x) & BIT_SYS_MEM_RME_CPU_MSK) << BIT_SYS_MEM_RME_CPU_SHF)

#define BIT_SYS_MEM_RME_WLAN_SHF 19
#define BIT_SYS_MEM_RME_WLAN_MSK 0x7
#define BIT_SYS_MEM_RME_WLAN(x) (((x) & BIT_SYS_MEM_RME_WLAN_MSK) << BIT_SYS_MEM_RME_WLAN_SHF)

#define BIT_SYS_MEM_RME_USB BIT(18)
#define BIT_SYS_MEM_RME_SDIO BIT(17)

//2 REG_SYS_REGU_CTRL0
#define BIT_SYS_REGU_LDO25M_ADJ_SHF 20
#define BIT_SYS_REGU_LDO25M_ADJ_MSK 0xf
#define BIT_SYS_REGU_LDO25M_ADJ(x) (((x) & BIT_SYS_REGU_LDO25M_ADJ_MSK) << BIT_SYS_REGU_LDO25M_ADJ_SHF)

#define BIT_SYS_REGU_ANACK_4M_EN BIT(19)
#define BIT_SYS_REGU_ANACK_4M_SEL BIT(18)
#define BIT_SYS_REGU_PC_EF_EN BIT(17)
#define BIT_SYS_REGU_LDOH12_SLP_EN BIT(16)

#define BIT_SYS_REGU_LDOH12_ADJ_SHF 12
#define BIT_SYS_REGU_LDOH12_ADJ_MSK 0xf
#define BIT_SYS_REGU_LDOH12_ADJ(x) (((x) & BIT_SYS_REGU_LDOH12_ADJ_MSK) << BIT_SYS_REGU_LDOH12_ADJ_SHF)

#define BIT_SYS_REGU_LDO25E_ADJ_SHF 8
#define BIT_SYS_REGU_LDO25E_ADJ_MSK 0xf
#define BIT_SYS_REGU_LDO25E_ADJ(x) (((x) & BIT_SYS_REGU_LDO25E_ADJ_MSK) << BIT_SYS_REGU_LDO25E_ADJ_SHF)

#define BIT_SYS_REGU_DSLEPM_EN BIT(7)
#define BIT_SYS_REGU_PC_33V_EN BIT(3)
#define BIT_SYS_REGU_PC_EF25_EN BIT(2)
#define BIT_SYS_REGU_LDO25M_EN BIT(1)
#define BIT_SYS_REGU_LDO25E_EN BIT(0)

#define BIT_SYS_SWR12_COMP_R2_SHF 30
#define BIT_SYS_SWR12_COMP_R2_MSK 0x3
#define BIT_SYS_SWR12_COMP_R2(x) (((x) & BIT_SYS_SWR12_COMP_R2_MSK) << BIT_SYS_SWR12_COMP_R2_SHF)

#define BIT_SYS_SWR12_COMP_R1_SHF 28
#define BIT_SYS_SWR12_COMP_R1_MSK 0x3
#define BIT_SYS_SWR12_COMP_R1(x) (((x) & BIT_SYS_SWR12_COMP_R1_MSK) << BIT_SYS_SWR12_COMP_R1_SHF)

#define BIT_SYS_SWR12_COMP_C3_SHF 26
#define BIT_SYS_SWR12_COMP_C3_MSK 0x3
#define BIT_SYS_SWR12_COMP_C3(x) (((x) & BIT_SYS_SWR12_COMP_C3_MSK) << BIT_SYS_SWR12_COMP_C3_SHF)

#define BIT_SYS_SWR12_COMP_C2_SHF 24
#define BIT_SYS_SWR12_COMP_C2_MSK 0x3
#define BIT_SYS_SWR12_COMP_C2(x) (((x) & BIT_SYS_SWR12_COMP_C2_MSK) << BIT_SYS_SWR12_COMP_C2_SHF)

#define BIT_SYS_SWR12_COMP_C1_SHF 22
#define BIT_SYS_SWR12_COMP_C1_MSK 0x3
#define BIT_SYS_SWR12_COMP_C1(x) (((x) & BIT_SYS_SWR12_COMP_C1_MSK) << BIT_SYS_SWR12_COMP_C1_SHF)

#define BIT_SYS_SWR12_COMP_TYPE_L BIT(21)
#define BIT_SYS_SWR12_FPWM_MD BIT(20)

#define BIT_SYS_SPSLDO_VOL_SHF 17
#define BIT_SYS_SPSLDO_VOL_MSK 0x7
#define BIT_SYS_SPSLDO_VOL(x) (((x) & BIT_SYS_SPSLDO_VOL_MSK) << BIT_SYS_SPSLDO_VOL_SHF)

#define BIT_SYS_SWR12_IN_SHF 14
#define BIT_SYS_SWR12_IN_MSK 0x7
#define BIT_SYS_SWR12_IN(x) (((x) & BIT_SYS_SWR12_IN_MSK) << BIT_SYS_SWR12_IN_SHF)

#define BIT_SYS_SWR12_STD_SHF 12
#define BIT_SYS_SWR12_STD_MSK 0x3
#define BIT_SYS_SWR12_STD(x) (((x) & BIT_SYS_SWR12_STD_MSK) << BIT_SYS_SWR12_STD_SHF)

#define BIT_SYS_SWR12_VOL_SHF 8
#define BIT_SYS_SWR12_VOL_MSK 0xf
#define BIT_SYS_SWR12_VOL(x) (((x) & BIT_SYS_SWR12_VOL_MSK) << BIT_SYS_SWR12_VOL_SHF)

#define BIT_SYS_SWR_LDO     BIT(0)
#define BIT_SYS_SWR         BIT(1)

//2 REG_SYS_SWR_CTRL1
#define BIT_SYS_SW12_PFM_SEL      BIT(25)
#define BIT_SYS_SW12_AUTO_ZCD_L   BIT(24)
#define BIT_SYS_SW12_AUTO_MODE    BIT(23)
#define BIT_SYS_SW12_LDOF_L       BIT(22)
#define BIT_SYS_SW12_OCPS_L       BIT(21)

#define BIT_SYS_SW12_TBOX_SHF 17
#define BIT_SYS_SW12_TBOX_MSK 0x3
#define BIT_SYS_SW12_TBOX(x) (((x) & BIT_SYS_SW12_TBOX_MSK) << BIT_SYS_SW12_TBOX_SHF)

#define BIT_SYS_SW12_NONOVRLAP_DLY_SHF 15
#define BIT_SYS_SW12_NONOVRLAP_DLY_MSK 0x3
#define BIT_SYS_SW12_NONOVRLAP_DLY(x) (((x) & BIT_SYS_SW12_NONOVRLAP_DLY_MSK) << BIT_SYS_SW12_NONOVRLAP_DLY_SHF)

#define BIT_SYS_SW12_CLAMP_DUTY BIT(14)
#define BIT_SYS_SWR12_BYPASS_SSR BIT(13)
#define BIT_SYS_SWR12_ZCDOUT_EN BIT(12)
#define BIT_SYS_SWR12_POW_ZCD BIT(11)
#define BIT_SYS_SW12_AREN BIT(10)

#define BIT_SYS_SWR12_OCP_CUR_SHF 7
#define BIT_SYS_SWR12_OCP_CUR_MSK 0x7
#define BIT_SYS_SWR12_OCP_CUR(x) (((x) & BIT_SYS_SWR12_OCP_CUR_MSK) << BIT_SYS_SWR12_OCP_CUR_SHF)

#define BIT_SYS_SWR12_OCP_EN BIT(6)

#define BIT_SYS_SWR12_SAWTOOTH_CF_L_SHF 4
#define BIT_SYS_SWR12_SAWTOOTH_CF_L_MSK 0x3
#define BIT_SYS_SWR12_SAWTOOTH_CF_L(x) (((x) & BIT_SYS_SWR12_SAWTOOTH_CF_L_MSK) << BIT_SYS_SWR12_SAWTOOTH_CF_L_SHF)

#define BIT_SYS_SWR12_SAWTOOTH_CFC_L_SHF 2
#define BIT_SYS_SWR12_SAWTOOTH_CFC_L_MSK 0x3
#define BIT_SYS_SWR12_SAWTOOTH_CFC_L(x) (((x) & BIT_SYS_SWR12_SAWTOOTH_CFC_L_MSK) << BIT_SYS_SWR12_SAWTOOTH_CFC_L_SHF)

#define BIT_SYS_SWR12_COMP_R3_SHF 0
#define BIT_SYS_SWR12_COMP_R3_MSK 0x3
#define BIT_SYS_SWR12_COMP_R3(x) (((x) & BIT_SYS_SWR12_COMP_R3_MSK) << BIT_SYS_SWR12_COMP_R3_SHF)

//2 REG_SYS_XTAL_CTRL0
#define BIT_SYS_XTAL_BGMB         BIT(0)
#define BIT_SYS_XTAL              BIT(1)

#define BIT_SYS_XTAL_GMP_SHF      8
#define BIT_SYS_XTAL_GMP_MSK      0x1f
#define BIT_SYS_XTAL_GMP(x)       (((x) & BIT_SYS_XTAL_GMP_MSK) << BIT_SYS_XTAL_GMP_SHF)

#define BIT_SYS_XTAL_GMN_SHF      13
#define BIT_SYS_XTAL_GMN_MSK      0x1f
#define BIT_SYS_XTAL_GMN(x)       (((x) & BIT_SYS_XTAL_GMN_MSK) << BIT_SYS_XTAL_GMN_SHF)

#define BIT_SYS_XTAL_SC_XI_SHF    18
#define BIT_SYS_XTAL_SC_XI_MSK    0x3f
#define BIT_SYS_XTAL_SC_XI(x)     (((x) & BIT_SYS_XTAL_SC_XI_MSK) << BIT_SYS_XTAL_SC_XI_SHF)

#define BIT_SYS_XTAL_SC_XO_SHF    24
#define BIT_SYS_XTAL_SC_XO_MSK    0x3f
#define BIT_SYS_XTAL_SC_XO(x)     (((x) & BIT_SYS_XTAL_SC_XO_MSK) << BIT_SYS_XTAL_SC_XO_SHF)

#define BIT_SYS_XTAL_XQSEL_RF     BIT(30)
#define BIT_SYS_XTAL_XQSEL        BIT(31)

//2 REG_SYS_XTAL_CTRL1
#define BIT_SYS_XTAL_DRV_AGPIO_SHF   19
#define BIT_SYS_XTAL_DRV_AGPIO_MSK   0x3
#define BIT_SYS_XTAL_DRV_AGPIO(x)    ((x & BIT_SYS_XTAL_DRV_AGPIO_MSK) << BIT_SYS_XTAL_DRV_AGPIO_SHF)
#define BIT_SYS_XTAL_AGPIO_SEL       BIT(21)
#define BIT_SYS_XTAL_DELAY_WLAFE     BIT(22)
#define BIT_SYS_XTAL_DELAY_USB       BIT(23)
#define BIT_SYS_XTAL_DELAY_SYSPLL    BIT(24)

#define BIT_SYS_XTAL_COUNTER_MUX_SHF 25
#define BIT_SYS_XTAL_COUNTER_MUX_MSK 0x3
#define BIT_SYS_XTAL_COUNTER_MUX(x)  ((x & BIT_SYS_XTAL_COUNTER_MUX_MSK) << BIT_SYS_XTAL_COUNTER_MUX_SHF)

#define BIT_SYS_XTAL_AGPIO_SHF       16
#define BIT_SYS_XTAL_AGPIO_MSK       0x7
#define BIT_SYS_XTAL_AGPIO(x)        ((x & BIT_SYS_XTAL_AGPIO_MSK) << BIT_SYS_XTAL_AGPIO_SHF)

#define BIT_SYS_XTAL_DRV_SYSPLL_SHF  14
#define BIT_SYS_XTAL_DRV_SYSPLL_MSK  0x3
#define BIT_SYS_XTAL_DRV_SYSPLL(x)   ((x & BIT_SYS_XTAL_DRV_SYSPLL_MSK) << BIT_SYS_XTAL_DRV_SYSPLL_SHF)

#define BIT_SYS_XTAL_GATE_SYSPLL BIT(13)

#define BIT_SYS_XTAL_DRV_USB_SHF 11
#define BIT_SYS_XTAL_DRV_USB_MSK 0x3
#define BIT_SYS_XTAL_DRV_USB(x) (((x) & BIT_SYS_XTAL_DRV_USB_MSK) << BIT_SYS_XTAL_DRV_USB_SHF)

#define BIT_SYS_XTAL_GATE_USB BIT(10)

#define BIT_SYS_XTAL_DRV_WLAFE_SHF 8
#define BIT_SYS_XTAL_DRV_WLAFE_MSK 0x3
#define BIT_SYS_XTAL_DRV_WLAFE(x) (((x) & BIT_SYS_XTAL_DRV_WLAFE_MSK) << BIT_SYS_XTAL_DRV_WLAFE_SHF)

#define BIT_SYS_XTAL_GATE_WLAFE BIT(7)

#define BIT_SYS_XTAL_DRV_RF2_SHF 5
#define BIT_SYS_XTAL_DRV_RF2_MSK 0x3
#define BIT_SYS_XTAL_DRV_RF2(x) (((x) & BIT_SYS_XTAL_DRV_RF2_MSK) << BIT_SYS_XTAL_DRV_RF2_SHF)

#define BIT_SYS_XTAL_GATE_RF2 BIT(4)

#define BIT_SYS_XTAL_DRV_RF1_SHF 3
#define BIT_SYS_XTAL_DRV_RF1_MSK 0x3
#define BIT_SYS_XTAL_DRV_RF1(x) (((x) & BIT_SYS_XTAL_DRV_RF1_MSK) << BIT_SYS_XTAL_DRV_RF1_SHF)

#define BIT_SYS_XTAL_GATE_RF1 BIT(1)

#define BIT_SYS_XTAL_LDO_SHF 0
#define BIT_SYS_XTAL_LDO_MSK 0x3
#define BIT_SYS_XTAL_LDO(x) (((x) & BIT_SYS_XTAL_LDO_MSK) << BIT_SYS_XTAL_LDO_SHF)

#define BIT_SYS_SYSPLL_LPF_R3_SHF 29
#define BIT_SYS_SYSPLL_LPF_R3_MSK 0x7
#define BIT_SYS_SYSPLL_LPF_R3(x) (((x) & BIT_SYS_SYSPLL_LPF_R3_MSK) << BIT_SYS_SYSPLL_LPF_R3_SHF)

#define BIT_SYS_SYSPLL_LPF_CS_SHF 27
#define BIT_SYS_SYSPLL_LPF_CS_MSK 0x3
#define BIT_SYS_SYSPLL_LPF_CS(x) (((x) & BIT_SYS_SYSPLL_LPF_CS_MSK) << BIT_SYS_SYSPLL_LPF_CS_SHF)

#define BIT_SYS_SYSPLL_LPF_CP_SHF 25
#define BIT_SYS_SYSPLL_LPF_CP_MSK 0x3
#define BIT_SYS_SYSPLL_LPF_CP(x) (((x) & BIT_SYS_SYSPLL_LPF_CP_MSK) << BIT_SYS_SYSPLL_LPF_CP_SHF)

#define BIT_SYS_SYSPLL_LPF_C3_SHF 23
#define BIT_SYS_SYSPLL_LPF_C3_MSK 0x3
#define BIT_SYS_SYSPLL_LPF_C3(x) (((x) & BIT_SYS_SYSPLL_LPF_C3_MSK) << BIT_SYS_SYSPLL_LPF_C3_SHF)

#define BIT_SYS_SYSPLL_WDOG_ENB BIT(22)
#define BIT_SYS_SYSPLL_CKTST_EN BIT(21)

#define BIT_SYS_SYSPLL_MONCK_SEL_SHF 18
#define BIT_SYS_SYSPLL_MONCK_SEL_MSK 0x7
#define BIT_SYS_SYSPLL_MONCK_SEL(x) (((x) & BIT_SYS_SYSPLL_MONCK_SEL_MSK) << BIT_SYS_SYSPLL_MONCK_SEL_SHF)

#define BIT_SYS_SYSPLL_CP_IOFFSET_SHF 13
#define BIT_SYS_SYSPLL_CP_IOFFSET_MSK 0x1f
#define BIT_SYS_SYSPLL_CP_IOFFSET(x) (((x) & BIT_SYS_SYSPLL_CP_IOFFSET_MSK) << BIT_SYS_SYSPLL_CP_IOFFSET_SHF)

#define BIT_SYS_SYSPLL_CP_IDOUBLE BIT(12)

#define BIT_SYS_SYSPLL_CP_BIAS_SHF 9
#define BIT_SYS_SYSPLL_CP_BIAS_MSK 0x7
#define BIT_SYS_SYSPLL_CP_BIAS(x) (((x) & BIT_SYS_SYSPLL_CP_BIAS_MSK) << BIT_SYS_SYSPLL_CP_BIAS_SHF)

#define BIT_SYS_SYSPLL_FREF_EDGE BIT(8)
#define BIT_SYS_SYSPLL_EN BIT(1)
#define BIT_SYS_SYSPLL_LVPC_EN BIT(0)

//2 REG_SYS_SYSPLL_CTRL1
#define BIT_SYS_SYSPLL_CK500K_SEL BIT(15)
#define BIT_SYS_SYSPLL_CK200M_EN BIT(14)
#define BIT_SYS_SYSPLL_CKSDR_EN BIT(13)

#define BIT_SYS_SYSPLL_CKSDR_DIV_SHF 11
#define BIT_SYS_SYSPLL_CKSDR_DIV_MSK 0x3
#define BIT_SYS_SYSPLL_CKSDR_DIV(x) (((x) & BIT_SYS_SYSPLL_CKSDR_DIV_MSK) << BIT_SYS_SYSPLL_CKSDR_DIV_SHF)

#define BIT_SYS_SYSPLL_CK24P576_EN BIT(9)
#define BIT_SYS_SYSPLL_CK22P5792_EN BIT(8)
#define BIT_SYS_SYSPLL_CK_PS_EN BIT(6)

#define BIT_SYS_SYSPLL_CK_PS_SEL_SHF 3
#define BIT_SYS_SYSPLL_CK_PS_SEL_MSK 0x7
#define BIT_SYS_SYSPLL_CK_PS_SEL(x) (((x) & BIT_SYS_SYSPLL_CK_PS_SEL_MSK) << BIT_SYS_SYSPLL_CK_PS_SEL_SHF)

#define BIT_SYS_SYSPLL_LPF_RS_SHF 0
#define BIT_SYS_SYSPLL_LPF_RS_MSK 0x7
#define BIT_SYS_SYSPLL_LPF_RS(x) (((x) & BIT_SYS_SYSPLL_LPF_RS_MSK) << BIT_SYS_SYSPLL_LPF_RS_SHF)

//2 REG_SYS_SYSPLL_CTRL2
#define BIT_XTAL_DRV_RF_LATCH_SHF 0
#define BIT_XTAL_DRV_RF_LATCH_MSK 0xffffffffL
#define BIT_XTAL_DRV_RF_LATCH(x) (((x) & BIT_XTAL_DRV_RF_LATCH_MSK) << BIT_XTAL_DRV_RF_LATCH_SHF)

#define BIT_PESOC_CPU_OCP_CK_SEL_SHF 0
#define BIT_PESOC_CPU_OCP_CK_SEL_MSK 0x7
#define BIT_PESOC_CPU_OCP_CK_SEL(x) (((x) & BIT_PESOC_CPU_OCP_CK_SEL_MSK) << BIT_PESOC_CPU_OCP_CK_SEL_SHF)

//2 REG_SYS_ANA_TIM_CTRL
#define BIT_SYS_ANACK_TU_TIME_SHF 16
#define BIT_SYS_ANACK_TU_TIME_MSK 0x3f
#define BIT_SYS_ANACK_TU_TIME(x) (((x) & BIT_SYS_ANACK_TU_TIME_MSK) << BIT_SYS_ANACK_TU_TIME_SHF)

#define BIT_SYS_DSBYCNT_EN BIT(15)

#define BIT_SYS_DSTDY_TIM_SCAL_SHF 8
#define BIT_SYS_DSTDY_TIM_SCAL_MSK 0xf
#define BIT_SYS_DSTDY_TIM_SCAL(x) (((x) & BIT_SYS_DSTDY_TIM_SCAL_MSK) << BIT_SYS_DSTDY_TIM_SCAL_SHF)

#define BIT_SYS_DSTBY_TIM_PERIOD_SHF 0
#define BIT_SYS_DSTBY_TIM_PERIOD_MSK 0xff
#define BIT_SYS_DSTBY_TIM_PERIOD(x) (((x) & BIT_SYS_DSTBY_TIM_PERIOD_MSK) << BIT_SYS_DSTBY_TIM_PERIOD_SHF)

#define BIT_SYS_REGU_ASIF_EN_SHF 24
#define BIT_SYS_REGU_ASIF_EN_MSK 0xff
#define BIT_SYS_REGU_ASIF_EN(x) (((x) & BIT_SYS_REGU_ASIF_EN_MSK) << BIT_SYS_REGU_ASIF_EN_SHF)

#define BIT_SYS_REGU_ASIF_THP_DA_SHF 20
#define BIT_SYS_REGU_ASIF_THP_DA_MSK 0x3
#define BIT_SYS_REGU_ASIF_THP_DA(x) (((x) & BIT_SYS_REGU_ASIF_THP_DA_MSK) << BIT_SYS_REGU_ASIF_THP_DA_SHF)

#define BIT_SYS_REGU_ASIF_TPD_CK_SHF 18
#define BIT_SYS_REGU_ASIF_TPD_CK_MSK 0x3
#define BIT_SYS_REGU_ASIF_TPD_CK(x) (((x) & BIT_SYS_REGU_ASIF_TPD_CK_MSK) << BIT_SYS_REGU_ASIF_TPD_CK_SHF)

#define BIT_SYS_REGU_ASIF_TSP_DA_SHF 16
#define BIT_SYS_REGU_ASIF_TSP_DA_MSK 0x3
#define BIT_SYS_REGU_ASIF_TSP_DA(x) (((x) & BIT_SYS_REGU_ASIF_TSP_DA_MSK) << BIT_SYS_REGU_ASIF_TSP_DA_SHF)

#define BIT_SYS_REGU_ASIF_POLL BIT(15)
#define BIT_SYS_REGU_ASIF_MODE BIT(14)
#define BIT_SYS_REGU_ASIF_WE BIT(12)

#define BIT_SYS_REGU_ASIF_AD_SHF 8
#define BIT_SYS_REGU_ASIF_AD_MSK 0xf
#define BIT_SYS_REGU_ASIF_AD(x) (((x) & BIT_SYS_REGU_ASIF_AD_MSK) << BIT_SYS_REGU_ASIF_AD_SHF)

#define BIT_SYS_REGU_ASIF_WD_SHF 0
#define BIT_SYS_REGU_ASIF_WD_MSK 0xff
#define BIT_SYS_REGU_ASIF_WD(x) (((x) & BIT_SYS_REGU_ASIF_WD_MSK) << BIT_SYS_REGU_ASIF_WD_SHF)

//2 REG_SYS_DSLP_TIM_CAL_CTRL
#define BIT_SYS_DSLP_TIM_EN BIT(24)

#define BIT_SYS_DSLP_TIM_PERIOD_SHF 0
#define BIT_SYS_DSLP_TIM_PERIOD_MSK 0x7fffff
#define BIT_SYS_DSLP_TIM_PERIOD(x) (((x) & BIT_SYS_DSLP_TIM_PERIOD_MSK) << BIT_SYS_DSLP_TIM_PERIOD_SHF)

//2 REG_SYS_DEBUG_CTRL
#define BIT_MCTRL_DBG            BIT(0)

//2 REG_SYS_PINMUX_CTRL
#define BIT_MCTRL_SIC            BIT(0)
#define BIT_MCTRL_EEPROM         BIT(4)

#define BIT_MCTRL_I2S0_PIN       BIT(0)
#define BIT_MCTRL_I2S0_MCK       BIT(1)
#define BIT_MCTRL_I2S1_PIN       BIT(4)
#define BIT_MCTRL_I2S1_MCK       BIT(5)
#define BIT_MCTRL_PCM0           BIT(16)
#define BIT_MCTRL_PCM1           BIT(20)

//2 REG_HCI_PINMUX_CTRL
#define BIT_MCTRL_HCI_SDIOD      BIT(0)
#define BIT_MCTRL_HCI_SDIOH      BIT(1)
#define BIT_MCTRL_HCI_MII        BIT(24)

//2 REG_WL_PINMUX_CTRL
#define BIT_MCTRL_WL_ANT0        BIT(8)
#define BIT_MCTRL_WL_ANT1        BIT(9)
#define BIT_MCTRL_WL_BTCOEX      BIT(12)
#define BIT_MCTRL_WL_BTCMD       BIT(13)
#define BIT_MCTRL_NFC            BIT(16)

#define BIT_MCTRL_WL_LED         BIT(0)
#define BIT_MCTRL_WL_LED_SEL_SHF 1
#define BIT_MCTRL_WL_LED_SEL_MSK 0x3
#define BIT_MCTRL_WL_LED_SEL(x)  (((x) & BIT_WL_LED_SEL_MSK) << BIT_WL_LED_SEL_SHF)

//2 REG_CPU_PERIPHERAL_CTRL
#define BIT_CPU_SPIC             BIT(0)
#define BIT_CPU_SPIC_SHF         1
#define BIT_CPU_SPIC_MSK         0x3
#define BIT_CPU_SPIC_SEL(x)      (((x) & BIT_CPU_SPIC_SEL_MSK) << BIT_CPU_SPI_SEL_SHF)

#define BIT_CPU_SDRC             BIT(4)
#define BIT_CPU_TRACE            BIT(17)

#define BIT_CPU_LUART_SEL_SHF    22
#define BIT_CPU_LUART_SEL_MSK    0x3
#define BIT_CPU_LUART_SEL(x)     (((x) & BIT_LUART_SEL_MSK) << BIT_LUART_SEL_SHF)
#define BIT_CPU_LUART            BIT(20)
#define BIT_CPU_LUART_IR         BIT(21)

//2 REG_SYS_GPIO_DSTBY_WAKE_CTRL0
#define BIT_SYS_GPIOE3_WEVENT_STS      BIT(27)
#define BIT_SYS_GPIOD5_WEVENT_STS      BIT(26)
#define BIT_SYS_GPIOC7_WEVENT_STS      BIT(25)
#define BIT_SYS_GPIOA5_WEVENT_STS      BIT(24)
#define BIT_SYS_GPIO_GPE3_PULL_CTRL    BIT(19)
#define BIT_SYS_GPIO_GPD5_PULL_CTRL    BIT(18)
#define BIT_SYS_GPIO_GPC7_PULL_CTRL    BIT(17)
#define BIT_SYS_GPIO_GPA5_PULL_CTRL    BIT(16)
#define BIT_SYS_GPIOE3_WINT_MODE       BIT(11)
#define BIT_SYS_GPIOD5_WINT_MODE       BIT(10)
#define BIT_SYS_GPIOC7_WINT_MODE       BIT(9)
#define BIT_SYS_GPIOA5_WINT_MODE       BIT(8)
#define BIT_SYS_GPIOE3                 BIT(3)
#define BIT_SYS_GPIOD5                 BIT(2)
#define BIT_SYS_GPIOC7                 BIT(1)
#define BIT_SYS_GPIOA5                 BIT(0)

//2 REG_SYS_GPIO_DSTBY_WAKE_CTRL1
#define BIT_SYS_GPIOA5_SHTDN_N         BIT(16)
#define BIT_SYS_GPIOC7_SHTDN_N         BIT(17)
#define BIT_SYS_GPIOD5_SHTDN_N         BIT(18)
#define BIT_SYS_GPIOE3_SHTDN_N         BIT(19)

#define BIT_SYS_WINT_DEBOUNCE_TIM_SCAL_SHF 8
#define BIT_SYS_WINT_DEBOUNCE_TIM_SCAL_MSK 0x3
#define BIT_SYS_WINT_DEBOUNCE_TIM_SCAL(x) (((x) & BIT_SYS_WINT_DEBOUNCE_TIM_SCAL_MSK) << BIT_SYS_WINT_DEBOUNCE_TIM_SCAL_SHF)

#define BIT_SYS_GPIOA5_WINT_DEBOUNCE_EN BIT(0)
#define BIT_SYS_GPIOC7_WINT_DEBOUNCE_EN BIT(1)
#define BIT_SYS_GPIOD5_WINT_DEBOUNCE_EN BIT(2)
#define BIT_SYS_GPIOE3_WINT_DEBOUNCE_EN BIT(3)

#define BIT_SYS_DBG_VALUE_SHF       0
#define BIT_SYS_DBG_VALUE_MSK       0xffffffffL
#define BIT_SYS_DBG_VALUE(x)        (((x) & BIT_SYS_DBG_VALUE_MSK) << BIT_SYS_DBG_VALUE_SHF)

#define BIT_SYS_EFUSE_UNLOCK_SHF    24
#define BIT_SYS_EFUSE_UNLOCK_MSK    0xff
#define BIT_SYS_EFUSE_UNLOCK(x)     (((x) & BIT_EFUSE_UNLOCK_MSK) << BIT_EFUSE_UNLOCK_SHF)
#define BIT_SYS_EFUSE_LDALL         BIT(16)
#define BIT_SYS_EEPROM_VPDIDX_SHF   8
#define BIT_SYS_EEPROM_VPDIDX_MSK   0xff
#define BIT_SYS_EEPROM_VPDIDX(x)    (((x) & BIT_SYS_EEPROM_VPDIDX_MSK) << BIT_SYS_EEPROM_VPDIDX_SHF)

#define BIT_SYS_EEPROM_MD_SHF 6
#define BIT_SYS_EEPROM_MD_MSK 0x3
#define BIT_SYS_EEPROM_MD(x) (((x) & BIT_SYS_EEPROM_MD_MSK) << BIT_SYS_EEPROM_MD_SHF)

#define BIT_SYS_AUTOLOAD_SUS BIT(5)
#define BIT_SYS_EEPROM_SEL BIT(4)
#define BIT_SYS_EEPROM_EECS BIT(3)
#define BIT_SYS_EEPROM_EESK BIT(2)
#define BIT_SYS_EEPROM_EEDI BIT(1)
#define BIT_SYS_EEPROM_EEDO BIT(0)

//2 REG_SYS_EEPROM_CTRL1
#define BIT_SYS_EEPROM_VPD_SHF 0
#define BIT_SYS_EEPROM_VPD_MSK 0xffffffffL
#define BIT_SYS_EEPROM_VPD(x) (((x) & BIT_SYS_EEPROM_VPD_MSK) << BIT_SYS_EEPROM_VPD_SHF)

//2 REG_SYS_EFUSE_CTRL
#define BIT_SYS_EF_RWFLAG BIT(31)

#define BIT_SYS_EF_PGPD_SHF 28
#define BIT_SYS_EF_PGPD_MSK 0x7
#define BIT_SYS_EF_PGPD(x) (((x) & BIT_SYS_EF_PGPD_MSK) << BIT_SYS_EF_PGPD_SHF)

#define BIT_SYS_EF_RDT_SHF 24
#define BIT_SYS_EF_RDT_MSK 0xf
#define BIT_SYS_EF_RDT(x) (((x) & BIT_SYS_EF_RDT_MSK) << BIT_SYS_EF_RDT_SHF)

#define BIT_SYS_EF_PGTS_SHF 20
#define BIT_SYS_EF_PGTS_MSK 0xf
#define BIT_SYS_EF_PGTS(x) (((x) & BIT_SYS_EF_PGTS_MSK) << BIT_SYS_EF_PGTS_SHF)

#define BIT_SYS_EF_PDWN BIT(19)
#define BIT_SYS_EF_ALDEN BIT(18)

#define BIT_SYS_EF_ADDR_SHF 8
#define BIT_SYS_EF_ADDR_MSK 0x3ff
#define BIT_SYS_EF_ADDR(x) (((x) & BIT_SYS_EF_ADDR_MSK) << BIT_SYS_EF_ADDR_SHF)

#define BIT_SYS_EF_DATA_SHF 0
#define BIT_SYS_EF_DATA_MSK 0xff
#define BIT_SYS_EF_DATA(x) (((x) & BIT_SYS_EF_DATA_MSK) << BIT_SYS_EF_DATA_SHF)

//2 REG_SYS_EFUSE_TEST
#define BIT_SYS_EF_CRES_SEL BIT(26)

#define BIT_SYS_EF_SCAN_START_SHF 16
#define BIT_SYS_EF_SCAN_START_MSK 0x1ff
#define BIT_SYS_EF_SCAN_START(x) (((x) & BIT_SYS_EF_SCAN_START_MSK) << BIT_SYS_EF_SCAN_START_SHF)

#define BIT_SYS_EF_SCAN_END_SHF 12
#define BIT_SYS_EF_SCAN_END_MSK 0xf
#define BIT_SYS_EF_SCAN_END(x) (((x) & BIT_SYS_EF_SCAN_END_MSK) << BIT_SYS_EF_SCAN_END_SHF)

#define BIT_SYS_EF_FORCE_PGMEN BIT(11)

#define BIT_SYS_EF_CELL_SEL_SHF 8
#define BIT_SYS_EF_CELL_SEL_MSK 0x3
#define BIT_SYS_EF_CELL_SEL(x) (((x) & BIT_SYS_EF_CELL_SEL_MSK) << BIT_SYS_EF_CELL_SEL_SHF)

#define BIT_SYS_EF_TRPT BIT(7)

#define BIT_SYS_EF_SCAN_TTHD_SHF 0
#define BIT_SYS_EF_SCAN_TTHD_MSK 0x7f
#define BIT_SYS_EF_SCAN_TTHD(x) (((x) & BIT_SYS_EF_SCAN_TTHD_MSK) << BIT_SYS_EF_SCAN_TTHD_SHF)

// REG_SYS_SLP_WAKE_EVENT_MSK0
#define BIT_SYS_WEVT_SYSTIM_MSK       BIT(0)
#define BIT_SYS_WEVT_GTIM_MSK         BIT(1)
#define BIT_SYS_WEVT_OVER_CURRENT_MSK BIT(2)
#define BIT_SYS_WEVT_CHIP_EN_MSK      BIT(3)
#define BIT_SYS_WEVT_GPIO_MSK         BIT(4)
#define BIT_SYS_WEVT_WLAN_MSK         BIT(8)
#define BIT_SYS_WEVT_NFC_MSK          BIT(9)
#define BIT_SYS_WEVT_SDIO_MSK         BIT(14)
#define BIT_SYS_WEVT_USB_MSK          BIT(16)
#define BIT_SYS_WEVT_UART_MSK         BIT(20)
#define BIT_SYS_WEVT_SPI_MSK          BIT(22)
#define BIT_SYS_WEVT_I2C_MSK          BIT(24)
#define BIT_SYS_WEVT_ADC_MSK          BIT(26)
#define BIT_SYS_WEVT_A33_MSK          BIT(28)
#define BIT_SYS_WEVT_GPIO_DSTBY_MSK   BIT(29)

// REG_SYS_SLP_WAKE_EVENT_MSK1

// REG_SYS_SLP_WAKE_EVENT_STATUS0
#define BIT_SYS_WEVT_SYSTIM_STS       BIT(0)
#define BIT_SYS_WEVT_GTIM_STS         BIT(1)
#define BIT_SYS_WEVT_OVER_CURRENT_STS BIT(2)
#define BIT_SYS_WEVT_CHIP_EN_STS      BIT(3)
#define BIT_SYS_WEVT_GPIO_STS         BIT(4)
#define BIT_SYS_WEVT_WLAN_STS         BIT(8)
#define BIT_SYS_WEVT_NFC_STS          BIT(9)
#define BIT_SYS_WEVT_SDIO_STS         BIT(14)
#define BIT_SYS_WEVT_USB_STS          BIT(16)
#define BIT_SYS_WEVT_UART_STS         BIT(20)
#define BIT_SYS_WEVT_SPI_STS          BIT(22)
#define BIT_SYS_WEVT_I2C_STS          BIT(24)
#define BIT_SYS_WEVT_ADC_STS          BIT(26)
#define BIT_SYS_WEVT_A33_STS          BIT(28)
#define BIT_SYS_WEVT_GPIO_DSTBY_STS   BIT(29)

#define BIT_SYS_WKPERI_IMR0_SHF       1
#define BIT_SYS_WKPERI_IMR0_MSK       0x7fffffffL
#define BIT_SYS_WKPERI_IMR0(x)        (((x) & BIT_SYS_WKPERI_IMR0_MSK) << BIT_SYS_WKPERI_IMR0_SHF)

#define BIT_SYS_SNFEVT_ADC_MSK        BIT(0)

#define BIT_SYS_WKPERI_ISR0_SHF       1
#define BIT_SYS_WKPERI_ISR0_MSK       0x7fffffffL
#define BIT_SYS_WKPERI_ISR0(x)        (((x) & BIT_SYS_WKPERI_ISR0_MSK) << BIT_SYS_WKPERI_ISR0_SHF)

#define BIT_SYS_SNFEVT_ADC_STS        BIT(0)

// REG_SYS_PWRMGT_CTRL
#define BIT_SYS_REGU_DSLP             BIT(7)

// REG_NOT_VALID
#define BIT_SYS_PM_CMD_DSLP           BIT(0)
#define BIT_SYS_PM_CMD_DSTBY          BIT(1)
#define BIT_SYS_PM_CMD_SLP            BIT(2)

//2 REG_SYS_PWRMGT_OPTION
#define BIT_SYS_PMOPT_DSTBY_LPLDO_SEL   BIT(0)
#define BIT_SYS_PMOPT_DSTBY_EN_SWR      BIT(1)
#define BIT_SYS_PMOPT_DSTBY_EN_PWM      BIT(2)
#define BIT_SYS_PMOPT_DSTBY_EN_SOC      BIT(3)
#define BIT_SYS_PMOPT_DSTBY_XTAL_EN     BIT(4)
#define BIT_SYS_PMOPT_DSTBY_SYSPLL_EN   BIT(5)
#define BIT_SYS_PMOPT_DSTBY_SYSCLK_SEL  BIT(6)
#define BIT_SYS_PMOPT_SLP_LPLDO_SEL     BIT(8)
#define BIT_SYS_PMOPT_SLP_EN_SWR        BIT(9)
#define BIT_SYS_PMOPT_SLP_EN_PWM        BIT(10)
#define BIT_SYS_PMOPT_SLP_EN_SOC        BIT(11)
#define BIT_SYS_PMOPT_SNZ_EN_SWR        BIT(17)
#define BIT_SYS_PMOPT_SNZ_EN_PWM        BIT(18)
#define BIT_SYS_PMOPT_SNZ_EN_SOC        BIT(19)
#define BIT_SYS_PMOPT_SNZ_XTAL_EN       BIT(20)
#define BIT_SYS_PMOPT_SNZ_SYSPLL_EN     BIT(21)
#define BIT_SYS_PMOPT_SNZ_SYSCLK_SEL    BIT(22)
#define BIT_SYS_PMOPT_NORM_LPLDO_SEL    BIT(24)
#define BIT_SYS_PMOPT_NORM_EN_SWR       BIT(25)
#define BIT_SYS_PMOPT_NORM_EN_PWM       BIT(26)
#define BIT_SYS_PMOPT_NORM_EN_SOC       BIT(27)
#define BIT_SYS_PMOPT_NORM_XTAL_EN      BIT(28)
#define BIT_SYS_PMOPT_NORM_SYSPLL_EN    BIT(29)
#define BIT_SYS_PMOPT_NORM_SYSCLK_SEL   BIT(30)
#define BIT_SYS_PMOPT_SNZ_LPLDO_SEL     BIT(16)
#define BIT_SYS_PMOPT_SLP_XTAL_EN       BIT(12)
#define BIT_SYS_PMOPT_SLP_SYSPLL_EN     BIT(13)
#define BIT_SYS_PMOPT_SLP_SYSCLK_SEL    BIT(14)

// REG_SYS_PWRMGT_OPTION_EXT
#define BIT_SYS_PMOPT_SLP_SWR_ADJ       BIT(0)
#define BIT_SYS_PMOPT_SLP_ANACK_EN      BIT(1)
#define BIT_SYS_PMOPT_SLP_ANACK_SEL     BIT(2)

// REG_SYS_DSLP_WEVENT
#define BIT_SYS_DSLP_GPIO BIT(2)
#define BIT_SYS_DSLP_NFC BIT(1)
#define BIT_SYS_DSLP_WTIMER33 BIT(0)

#define BIT_SYS_ISO33_NFC         BIT(0)
#define BIT_SYS_BD_PKG_SEL        BIT(31)

#define BIT_VENDOR_ID_SHF         8
#define BIT_VENDOR_ID_MSK         0xf
#define BIT_VENDOR_ID(x)          (((x) & BIT_VENDOR_ID_MSK) << BIT_VENDOR_ID_SHF)

#define BIT_CHIP_VER_SHF          4
#define BIT_CHIP_VER_MSK          0xf
#define BIT_CHIP_VER(x)           (((x) & BIT_CHIP_VER_MSK) << BIT_CHIP_VER_SHF)

#define BIT_RF_RL_ID_SHF          0
#define BIT_RF_RL_ID_MSK          0xf
#define BIT_RF_RL_ID(x)           (((x) & BIT_RF_RL_ID_MSK) << BIT_RF_RL_ID_SHF)

#define BIT_SYS_TRP_ICFG_SHF      28
#define BIT_SYS_TRP_ICFG_MSK      0xf
#define BIT_SYS_TRP_ICFG(x)       (((x) & BIT_SYS_TRP_ICFG_MSK) << BIT_SYS_TRP_ICFG_SHF)

#define BIT_SYS_ALDN_STS          BIT(0)
#define BIT_SYS_XCLK_VLD          BIT(8)
#define BIT_SYS_PLL_CLK_RDY       BIT(9)
#define BIT_V15_VLD               BIT(16)
#define BIT_SYS_TRP_SPSLDO_SEL    BIT(26)
#define BIT_SYS_TRP_BOOT_SEL      BIT(27)

// REG_PERI_PWM0_CTRL
#define BIT_PWM0_PERIOD_SHF       0
#define BIT_PWM0_PERIOD_MSK       0x3ff
#define BIT_PWM0_PERIOD(x)        ((x & BIT_PWM0_PERIOD_MSK) << BIT_PWM0_PERIOD_SHF)
#define BIT_PWM0_DUTY_SHF         12
#define BIT_PWM0_DUTY_MSK         0x3ff
#define BIT_PWM0_DUTY(x)          ((x & BIT_PWM0_DUTY_MSK) << BIT_PWM0_DUTY_SHF)
#define BIT_PWM0_GT_SEL_SHF       24
#define BIT_PWM0_GT_SEL_MSK       0xf
#define BIT_PWM0_GT_SEL(x)        ((x & BIT_PWM0_GT_SEL_MSK) << BIT_PWM0_GT_SEL_SHF)
#define BIT_FCTRL_PWM0            BIT(31)

// REG_PERI_PWM1_CTRL
#define BIT_PWM1_PERIOD_SHF       0
#define BIT_PWM1_PERIOD_MSK       0x3ff
#define BIT_PWM1_PERIOD(x)        ((x & BIT_PWM1_PERIOD_MSK) << BIT_PWM1_PERIOD_SHF)
#define BIT_PWM1_DUTY_SHF         12
#define BIT_PWM1_DUTY_MSK         0x3ff
#define BIT_PWM1_DUTY(x)          ((x & BIT_PWM1_DUTY_MSK) << BIT_PWM1_DUTY_SHF)
#define BIT_PWM1_GT_SEL_SHF       24
#define BIT_PWM1_GT_SEL_MSK       0xf
#define BIT_PWM1_GT_SEL(x)        ((x & BIT_PWM1_GT_SEL_MSK) << BIT_PWM1_GT_SEL_SHF)
#define BIT_FCTRL_PWM1            BIT(31)

// REG_PERI_PWM2_CTRL
#define BIT_PWM2_PERIOD_SHF       0
#define BIT_PWM2_PERIOD_MSK       0x3ff
#define BIT_PWM2_PERIOD(x)        ((x & BIT_PWM2_PERIOD_MSK) << BIT_PWM2_PERIOD_SHF)
#define BIT_PWM2_DUTY_SHF         12
#define BIT_PWM2_DUTY_MSK         0x3ff
#define BIT_PWM2_DUTY(x)          ((x & BIT_PWM2_DUTY_MSK) << BIT_PWM2_DUTY_SHF)
#define BIT_PWM2_GT_SEL_SHF       24
#define BIT_PWM2_GT_SEL_MSK       0xf
#define BIT_PWM2_GT_SEL(x)        ((x & BIT_PWM2_GT_SEL_MSK) << BIT_PWM2_GT_SEL_SHF)
#define BIT_FCTRL_PWM2            BIT(31)

// REG_PERI_PWM3_CTRL
#define BIT_PWM3_PERIOD_SHF       0
#define BIT_PWM3_PERIOD_MSK       0x3ff
#define BIT_PWM3_PERIOD(x)        ((x & BIT_PWM3_PERIOD_MSK) << BIT_PWM3_PERIOD_SHF)
#define BIT_PWM3_DUTY_SHF         12
#define BIT_PWM3_DUTY_MSK         0x3ff
#define BIT_PWM3_DUTY(x)          ((x & BIT_PWM3_DUTY_MSK) << BIT_PWM3_DUTY_SHF)
#define BIT_PWM3_GT_SEL_SHF       24
#define BIT_PWM3_GT_SEL_MSK       0xf
#define BIT_PWM3_GT_SEL(x)        ((x & BIT_PWM3_GT_SEL_MSK) << BIT_PWM3_GT_SEL_SHF)
#define BIT_FCTRL_PWM3            BIT(31)

// REG_PERI_TIM_EVT_CTRL
#define BIT_FCTRL_GT_EVT0         BIT(7)
#define BIT_GT_EVT0_PULSE_DUR_SHF 0
#define BIT_GT_EVT0_PULSE_DUR_MSK 0xf
#define BIT_GT_EVT0_PULSE_DUR(x) ((x & BIT_GT_EVT0_PULSE_DUR_MSK) << BIT_GT_EVT0_PULSE_DUR_SHF)
#define BIT_GT_EVT0_SRC_SEL_SHF   4
#define BIT_GT_EVT0_SRC_SEL_MSK   0x7
#define BIT_GT_EVT0_SRC_SEL(x)    ((x & BIT_GT_EVT0_SRC_SEL_MSK) << BIT_GT_EVT0_SRC_SEL_SHF)

#define BIT_FCTRL_GT_EVT1         BIT(15)
#define BIT_GT_EVT1_PULSE_DUR_SHF 8
#define BIT_GT_EVT1_PULSE_DUR_MSK 0xf
#define BIT_GT_EVT1_PULSE_DUR(x) ((x & BIT_GT_EVT1_PULSE_DUR_MSK) << BIT_GT_EVT1_PULSE_DUR_SHF)
#define BIT_GT_EVT1_SRC_SEL_SHF   12
#define BIT_GT_EVT1_SRC_SEL_MSK   0x7
#define BIT_GT_EVT1_SRC_SEL(x)    ((x & BIT_GT_EVT1_SRC_SEL_MSK) << BIT_GT_EVT1_SRC_SEL_SHF)

#define BIT_FCTRL_GT_EVT2         BIT(23)
#define BIT_GT_EVT2_PULSE_DUR_SHF 16
#define BIT_GT_EVT2_PULSE_DUR_MSK 0xf
#define BIT_GT_EVT2_PULSE_DUR(x)  ((x & BIT_GT_EVT2_PULSE_DUR_MSK) << BIT_GT_EVT2_PULSE_DUR_SHF)
#define BIT_GT_EVT2_SRC_SEL_SHF   20
#define BIT_GT_EVT2_SRC_SEL_MSK   0x7
#define BIT_GT_EVT2_SRC_SEL(x)    ((x & BIT_GT_EVT2_SRC_SEL_MSK) << BIT_GT_EVT2_SRC_SEL_SHF)

#define BIT_FCTRL_GT_EVT3         BIT(31)
#define BIT_GT_EVT3_PULSE_DUR_SHF 24
#define BIT_GT_EVT3_PULSE_DUR_MSK 0xf
#define BIT_GT_EVT3_PULSE_DUR(x)  ((x & BIT_GT_EVT3_PULSE_DUR_MSK) << BIT_GT_EVT3_PULSE_DUR_SHF)
#define BIT_GT_EVT3_SRC_SEL_SHF   28
#define BIT_GT_EVT3_SRC_SEL_MSK   0x7
#define BIT_GT_EVT3_SRC_SEL(x)    ((x & BIT_GT_EVT3_SRC_SEL_MSK) << BIT_GT_EVT3_SRC_SEL_SHF)

// REG_PERI_EGTIM_CTRL
#define BIT_FCTRL_EGTIM           BIT(0)
#define BIT_EGTIM_REF_SIG_SEL_SHF 4
#define BIT_EGTIM_REF_SIG_SEL_MSK 0x3
#define BIT_EGTIM_REF_SIG_SEL(x)  ((x & BIT_EGTIM_REF_SIG_SEL_MSK) << BIT_EGTIM_REF_SIG_SEL_SHF)

#define BIT_EGTIM_PIN_GROUP0_OPT_SEL_SHF 8
#define BIT_EGTIM_PIN_GROUP0_OPT_SEL_MSK 0x3
#define BIT_EGTIM_PIN_GROUP0_OPT_SEL(x) ((x & BIT_EGTIM_PIN_GROUP0_OPT_SEL_MSK) << BIT_EGTIM_PIN_GROUP0_OPT_SEL_SHF)

#define BIT_EGTIM_PIN_GROUP1_OPT_SEL_SHF 10
#define BIT_EGTIM_PIN_GROUP1_OPT_SEL_MSK 0x3
#define BIT_EGTIM_PIN_GROUP1_OPT_SEL(x) ((x & BIT_EGTIM_PIN_GROUP1_OPT_SEL_MSK) << BIT_EGTIM_PIN_GROUP1_OPT_SEL_SHF)

#define BIT_EGTIM_PIN_GROUP2_OPT_SEL_SHF 12
#define BIT_EGTIM_PIN_GROUP2_OPT_SEL_MSK 0x3
#define BIT_EGTIM_PIN_GROUP2_OPT_SEL(x) ((x & BIT_EGTIM_PIN_GROUP2_OPT_SEL_MSK) << BIT_EGTIM_PIN_GROUP2_OPT_SEL_SHF)

//================= Register Address Definition =====================//
#define SYS_PWR_CTRL              0x0000
#define SYS_ISO_CTRL              0x0002
#define SYS_FUNC_EN               0x0008
#define SYS_CLK_CTRL0             0x0010
#define SYS_CLK_CTRL1             0x0014
#define REG_SYS_CLK_CTRL1         (SYS_CLK_CTRL1)
#define SYS_EFUSE_CFG0            0x0020
#define SYS_EFUSE_CFG1            0x0024
#define SYS_EFUSE_CFG2            0x0028
#define SYS_EFUSE_CFG3            0x002C
#define SYS_EFUSE_CFG4            0x0030
#define SYS_EFUSE_CFG5            0x0034
#define SYS_EFUSE_CFG6            0x0038
#define REG_SYS_EFUSE_SYSCFG6     (SYS_EFUSE_CFG6)
#define SYS_EFUSE_CFG7            0x003C
#define SYS_REGU_CTRL0            0x0040  // REG_SYS_REGU_CTRL0
#define SYS_SWR_CTRL0             0x0048
#define SYS_SWR_CTRL1             0x004C
#define SYS_XTAL_CTRL0            0x0060
#define SYS_XTAL_CTRL1            0x0064
#define SYS_SYSPLL_CTRL0          0x0070
#define SYS_SYSPLL_CTRL1          0x0074
#define SYS_SYSPLL_CTRL2          0x0078
#define SYS_ANA_TIM_CTRL          0x0090
#define SYS_DSLP_TIM_CTRL         0x0094
#define SYS_DSLP_TIM_CAL_CTRL     0x0098
#define SYS_DEBUG_CTRL            0x00A0
#define SYS_PINMUX_CTRL           0x00A4
#define SYS_GPIO_DSTBY_WAKE_CTRL0 0x00A8
#define SYS_GPIO_DSTBY_WAKE_CTRL1 0x00AC
#define SYS_DEBUG_REG             0x00BC
#define SYS_EEPROM_CTRL0          0x00E0
#define REG_SYS_EEPROM_CTRL0      (SYS_EEPROM_CTRL0)
#define SYS_EEPROM_CTRL1          0x00E4
#define SYS_EFUSE_CTRL            0x00E8
#define REG_SYS_EFUSE_CTRL        (SYS_EFUSE_CTRL)
#define SYS_EFUSE_TEST            0x00EC
#define REG_SYS_EFUSE_TEST        (SYS_EFUSE_TEST)
#define SYS_DSTBY_INFO0           0x00F0
#define SYS_DSTBY_INFO1           0x00F4
#define SYS_DSTBY_INFO2           0x00F8
#define SYS_DSTBY_INFO3           0x00FC
#define REG_SYS_DSTBY_INFO3       (SYS_DSTBY_INFO3)
#define SYS_SLP_WAKE_EVENT_MSK0   0x0100
#define SYS_SLP_WAKE_EVENT_MSK1   0x0104
#define SYS_SLP_WAKE_EVENT_STATUS0 0x0108
#define SYS_SLP_WAKE_EVENT_STATUS1 0x010C
#define SYS_SNF_WAKE_EVENT_MSK0   0x0110
#define SYS_SNF_WAKE_EVENT_STATUS 0x0114
#define SYS_PWRMGT_CTRL           0x0118
#define SYS_PWRMGT_OPTION         0x0120
#define SYS_PWRMGT_OPTION_EXT     0x0124
#define SYS_DSLP_WEVENT           0x0130
#define SYS_PERI_MONITOR          0x0134
#define SYS_SYSTEM_CFG0           0x01F0
#define REG_SYS_SYSTEM_CFG0       (SYS_SYSTEM_CFG0)
#define SYS_SYSTEM_CFG1           0x01F4
#define REG_SYS_SYSTEM_CFG1       (SYS_SYSTEM_CFG1)
#define SYS_SYSTEM_CFG2           0x01F8

#define __LXBUS_Enable()          __RTK_PERI_SETBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_LXBUS)
#define __LXBUS_Disable()         __RTK_PERI_CLRBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_LXBUS)
#define __FLASH_Enable()          __RTK_PERI_SETBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_FLASH)
#define __FLASH_Disable()                                 \
  do {                                                    \
    __RTK_READ32(SPI_FLASH_BASE, 0);                      \
    __RTK_PERI_CLRBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_FLASH); \
  } while (0)

#define __GTIMER_Enable()         __RTK_PERI_SETBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_GTIMER)
#define __GTIMER_Disable()                                 \
  do {                                                     \
    __RTK_READ32(TIMER_BASE, 0);                           \
    __RTK_PERI_CLRBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_GTIMER); \
  } while (0)

#define __CRYPTO_Enable()         __RTK_PERI_SETBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_GTIMER)
#define __CRYPTO_Disable()                                  \
  do {                                                      \
    __RTK_READ32(CRYPTO_BASE, 0);                           \
    __RTK_PERI_SETBIT(PERI_FCTRL_CTRL0, BIT_FCTRL_SEC_ENG); \
  } while (0)

#endif
#endif

// Clock Control Register 3 (REG_PESOC_CLK_CTRL3)

// HCI_CLK_CTRL0
#define BIT_ACTCK_SDIOD            BIT(0)
#define BIT_SLPCK_SDIOD            BIT(1)
#define BIT_ACTCK_SDIOH            BIT(2)
#define BIT_SLPCK_SDIOH            BIT(3)
#define BIT_ACTCK_HCI_OTG          BIT(4)
#define BIT_SLPCK_HCI_OTG          BIT(5)
#define BIT_ACTCK_MII_MPHY         BIT(24)
#define BIT_SLPCK_MII_MPHY         BIT(25)


