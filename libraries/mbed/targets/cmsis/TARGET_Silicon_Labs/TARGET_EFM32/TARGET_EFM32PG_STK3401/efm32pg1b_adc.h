/**************************************************************************//**
 * @file efm32pg1b_adc.h
 * @brief EFM32PG1B_ADC register and bit field definitions
 * @version 4.1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com</b>
 ******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.@n
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.@n
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Laboratories, Inc.
 * has no obligation to support this Software. Silicon Laboratories, Inc. is
 * providing the Software "AS IS", with no express or implied warranties of any
 * kind, including, but not limited to, any implied warranties of
 * merchantability or fitness for any particular purpose or warranties against
 * infringement of any proprietary rights of a third party.
 *
 * Silicon Laboratories, Inc. will not be liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this Software.
 *
 *****************************************************************************/
/**************************************************************************//**
 * @defgroup EFM32PG1B_ADC
 * @{
 * @brief EFM32PG1B_ADC Register Declaration
 *****************************************************************************/
typedef struct
{
  __IO uint32_t CTRL;            /**< Control Register  */
  uint32_t      RESERVED0[1];    /**< Reserved for future use **/
  __IO uint32_t CMD;             /**< Command Register  */
  __I uint32_t  STATUS;          /**< Status Register  */
  __IO uint32_t SINGLECTRL;      /**< Single Sample Control Register  */
  __IO uint32_t SINGLECTRLX;     /**< Single Sample Control Register continued  */
  __IO uint32_t SCANCTRL;        /**< Scan Control Register  */
  __IO uint32_t SCANCTRLX;       /**< Scan Sample Control Register continued  */
  __IO uint32_t SCANMASK;        /**< Scan Sequence Input Mask Register  */
  __IO uint32_t SCANCHCONF;      /**< Input Channel Configuration register for Scan mode  */
  __IO uint32_t SCANNSEL;        /**< Negative Channel select register for Scan  */
  __IO uint32_t CMPTHR;          /**< Compare Threshold Register  */
  __IO uint32_t BIASPROG;        /**< Bias Programming Register for various analog blocks used in ADC operation  */
  __IO uint32_t CAL;             /**< Calibration Register  */
  __I uint32_t  IF;              /**< Interrupt Flag Register  */
  __IO uint32_t IFS;             /**< Interrupt Flag Set Register  */
  __IO uint32_t IFC;             /**< Interrupt Flag Clear Register  */
  __IO uint32_t IEN;             /**< Interrupt Enable Register  */
  __I uint32_t  SINGLEDATA;      /**< Single Conversion Result Data  */
  __I uint32_t  SCANDATA;        /**< Scan Conversion Result Data  */
  __I uint32_t  SINGLEDATAP;     /**< Single Conversion Result Data Peek Register  */
  __I uint32_t  SCANDATAP;       /**< Scan Sequence Result Data Peek Register  */
  uint32_t      RESERVED1[4];    /**< Reserved for future use **/
  __I uint32_t  SCANDATAX;       /**< Scan Sequence Result Data + Data Source Register  */
  __I uint32_t  SCANDATAXP;      /**< Scan Sequence Result Data + Data Source Peek Register  */

  uint32_t      RESERVED2[3];    /**< Reserved for future use **/
  __I uint32_t  BUSREQ;          /**< BUS Request Status Register  */
  __I uint32_t  BUSCONFLICT;     /**< BUS Request Status Register  */
  __I uint32_t  SINGLEFIFOCOUNT; /**< Single FIFO Count Register  */
  __I uint32_t  SCANFIFOCOUNT;   /**< Scan FIFO Count Register  */
  __IO uint32_t SINGLEFIFOCLEAR; /**< Single FIFO Count Register  */
  __IO uint32_t SCANFIFOCLEAR;   /**< Scan FIFO Count Register  */
} ADC_TypeDef;                   /** @} */

/**************************************************************************//**
 * @defgroup EFM32PG1B_ADC_BitFields
 * @{
 *****************************************************************************/

/* Bit fields for ADC CTRL */
#define _ADC_CTRL_RESETVALUE                            0x001F0000UL                              /**< Default value for ADC_CTRL */
#define _ADC_CTRL_MASK                                  0x2F7F7FDFUL                              /**< Mask for ADC_CTRL */
#define _ADC_CTRL_WARMUPMODE_SHIFT                      0                                         /**< Shift value for ADC_WARMUPMODE */
#define _ADC_CTRL_WARMUPMODE_MASK                       0x3UL                                     /**< Bit mask for ADC_WARMUPMODE */
#define _ADC_CTRL_WARMUPMODE_DEFAULT                    0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define _ADC_CTRL_WARMUPMODE_NORMAL                     0x00000000UL                              /**< Mode NORMAL for ADC_CTRL */
#define _ADC_CTRL_WARMUPMODE_KEEPINSTANDBY              0x00000001UL                              /**< Mode KEEPINSTANDBY for ADC_CTRL */
#define _ADC_CTRL_WARMUPMODE_KEEPINSLOWACC              0x00000002UL                              /**< Mode KEEPINSLOWACC for ADC_CTRL */
#define _ADC_CTRL_WARMUPMODE_KEEPADCWARM                0x00000003UL                              /**< Mode KEEPADCWARM for ADC_CTRL */
#define ADC_CTRL_WARMUPMODE_DEFAULT                     (_ADC_CTRL_WARMUPMODE_DEFAULT << 0)       /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_WARMUPMODE_NORMAL                      (_ADC_CTRL_WARMUPMODE_NORMAL << 0)        /**< Shifted mode NORMAL for ADC_CTRL */
#define ADC_CTRL_WARMUPMODE_KEEPINSTANDBY               (_ADC_CTRL_WARMUPMODE_KEEPINSTANDBY << 0) /**< Shifted mode KEEPINSTANDBY for ADC_CTRL */
#define ADC_CTRL_WARMUPMODE_KEEPINSLOWACC               (_ADC_CTRL_WARMUPMODE_KEEPINSLOWACC << 0) /**< Shifted mode KEEPINSLOWACC for ADC_CTRL */
#define ADC_CTRL_WARMUPMODE_KEEPADCWARM                 (_ADC_CTRL_WARMUPMODE_KEEPADCWARM << 0)   /**< Shifted mode KEEPADCWARM for ADC_CTRL */
#define ADC_CTRL_SINGLEDMAWU                            (0x1UL << 2)                              /**< SINGLEFIFO DMA Wakeup */
#define _ADC_CTRL_SINGLEDMAWU_SHIFT                     2                                         /**< Shift value for ADC_SINGLEDMAWU */
#define _ADC_CTRL_SINGLEDMAWU_MASK                      0x4UL                                     /**< Bit mask for ADC_SINGLEDMAWU */
#define _ADC_CTRL_SINGLEDMAWU_DEFAULT                   0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_SINGLEDMAWU_DEFAULT                    (_ADC_CTRL_SINGLEDMAWU_DEFAULT << 2)      /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_SCANDMAWU                              (0x1UL << 3)                              /**< SCANFIFO DMA Wakeup */
#define _ADC_CTRL_SCANDMAWU_SHIFT                       3                                         /**< Shift value for ADC_SCANDMAWU */
#define _ADC_CTRL_SCANDMAWU_MASK                        0x8UL                                     /**< Bit mask for ADC_SCANDMAWU */
#define _ADC_CTRL_SCANDMAWU_DEFAULT                     0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_SCANDMAWU_DEFAULT                      (_ADC_CTRL_SCANDMAWU_DEFAULT << 3)        /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_TAILGATE                               (0x1UL << 4)                              /**< Conversion Tailgating */
#define _ADC_CTRL_TAILGATE_SHIFT                        4                                         /**< Shift value for ADC_TAILGATE */
#define _ADC_CTRL_TAILGATE_MASK                         0x10UL                                    /**< Bit mask for ADC_TAILGATE */
#define _ADC_CTRL_TAILGATE_DEFAULT                      0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_TAILGATE_DEFAULT                       (_ADC_CTRL_TAILGATE_DEFAULT << 4)         /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_ASYNCCLKEN                             (0x1UL << 6)                              /**< Selects ASYNC CLK enable mode when ADCCLKMODE=1 */
#define _ADC_CTRL_ASYNCCLKEN_SHIFT                      6                                         /**< Shift value for ADC_ASYNCCLKEN */
#define _ADC_CTRL_ASYNCCLKEN_MASK                       0x40UL                                    /**< Bit mask for ADC_ASYNCCLKEN */
#define _ADC_CTRL_ASYNCCLKEN_DEFAULT                    0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define _ADC_CTRL_ASYNCCLKEN_ASNEEDED                   0x00000000UL                              /**< Mode ASNEEDED for ADC_CTRL */
#define _ADC_CTRL_ASYNCCLKEN_ALWAYSON                   0x00000001UL                              /**< Mode ALWAYSON for ADC_CTRL */
#define ADC_CTRL_ASYNCCLKEN_DEFAULT                     (_ADC_CTRL_ASYNCCLKEN_DEFAULT << 6)       /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_ASYNCCLKEN_ASNEEDED                    (_ADC_CTRL_ASYNCCLKEN_ASNEEDED << 6)      /**< Shifted mode ASNEEDED for ADC_CTRL */
#define ADC_CTRL_ASYNCCLKEN_ALWAYSON                    (_ADC_CTRL_ASYNCCLKEN_ALWAYSON << 6)      /**< Shifted mode ALWAYSON for ADC_CTRL */
#define ADC_CTRL_ADCCLKMODE                             (0x1UL << 7)                              /**< ADC Clock Mode */
#define _ADC_CTRL_ADCCLKMODE_SHIFT                      7                                         /**< Shift value for ADC_ADCCLKMODE */
#define _ADC_CTRL_ADCCLKMODE_MASK                       0x80UL                                    /**< Bit mask for ADC_ADCCLKMODE */
#define _ADC_CTRL_ADCCLKMODE_DEFAULT                    0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define _ADC_CTRL_ADCCLKMODE_SYNC                       0x00000000UL                              /**< Mode SYNC for ADC_CTRL */
#define _ADC_CTRL_ADCCLKMODE_ASYNC                      0x00000001UL                              /**< Mode ASYNC for ADC_CTRL */
#define ADC_CTRL_ADCCLKMODE_DEFAULT                     (_ADC_CTRL_ADCCLKMODE_DEFAULT << 7)       /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_ADCCLKMODE_SYNC                        (_ADC_CTRL_ADCCLKMODE_SYNC << 7)          /**< Shifted mode SYNC for ADC_CTRL */
#define ADC_CTRL_ADCCLKMODE_ASYNC                       (_ADC_CTRL_ADCCLKMODE_ASYNC << 7)         /**< Shifted mode ASYNC for ADC_CTRL */
#define _ADC_CTRL_PRESC_SHIFT                           8                                         /**< Shift value for ADC_PRESC */
#define _ADC_CTRL_PRESC_MASK                            0x7F00UL                                  /**< Bit mask for ADC_PRESC */
#define _ADC_CTRL_PRESC_DEFAULT                         0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define _ADC_CTRL_PRESC_NODIVISION                      0x00000000UL                              /**< Mode NODIVISION for ADC_CTRL */
#define ADC_CTRL_PRESC_DEFAULT                          (_ADC_CTRL_PRESC_DEFAULT << 8)            /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_PRESC_NODIVISION                       (_ADC_CTRL_PRESC_NODIVISION << 8)         /**< Shifted mode NODIVISION for ADC_CTRL */
#define _ADC_CTRL_TIMEBASE_SHIFT                        16                                        /**< Shift value for ADC_TIMEBASE */
#define _ADC_CTRL_TIMEBASE_MASK                         0x7F0000UL                                /**< Bit mask for ADC_TIMEBASE */
#define _ADC_CTRL_TIMEBASE_DEFAULT                      0x0000001FUL                              /**< Mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_TIMEBASE_DEFAULT                       (_ADC_CTRL_TIMEBASE_DEFAULT << 16)        /**< Shifted mode DEFAULT for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_SHIFT                         24                                        /**< Shift value for ADC_OVSRSEL */
#define _ADC_CTRL_OVSRSEL_MASK                          0xF000000UL                               /**< Bit mask for ADC_OVSRSEL */
#define _ADC_CTRL_OVSRSEL_DEFAULT                       0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X2                            0x00000000UL                              /**< Mode X2 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X4                            0x00000001UL                              /**< Mode X4 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X8                            0x00000002UL                              /**< Mode X8 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X16                           0x00000003UL                              /**< Mode X16 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X32                           0x00000004UL                              /**< Mode X32 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X64                           0x00000005UL                              /**< Mode X64 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X128                          0x00000006UL                              /**< Mode X128 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X256                          0x00000007UL                              /**< Mode X256 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X512                          0x00000008UL                              /**< Mode X512 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X1024                         0x00000009UL                              /**< Mode X1024 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X2048                         0x0000000AUL                              /**< Mode X2048 for ADC_CTRL */
#define _ADC_CTRL_OVSRSEL_X4096                         0x0000000BUL                              /**< Mode X4096 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_DEFAULT                        (_ADC_CTRL_OVSRSEL_DEFAULT << 24)         /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X2                             (_ADC_CTRL_OVSRSEL_X2 << 24)              /**< Shifted mode X2 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X4                             (_ADC_CTRL_OVSRSEL_X4 << 24)              /**< Shifted mode X4 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X8                             (_ADC_CTRL_OVSRSEL_X8 << 24)              /**< Shifted mode X8 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X16                            (_ADC_CTRL_OVSRSEL_X16 << 24)             /**< Shifted mode X16 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X32                            (_ADC_CTRL_OVSRSEL_X32 << 24)             /**< Shifted mode X32 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X64                            (_ADC_CTRL_OVSRSEL_X64 << 24)             /**< Shifted mode X64 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X128                           (_ADC_CTRL_OVSRSEL_X128 << 24)            /**< Shifted mode X128 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X256                           (_ADC_CTRL_OVSRSEL_X256 << 24)            /**< Shifted mode X256 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X512                           (_ADC_CTRL_OVSRSEL_X512 << 24)            /**< Shifted mode X512 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X1024                          (_ADC_CTRL_OVSRSEL_X1024 << 24)           /**< Shifted mode X1024 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X2048                          (_ADC_CTRL_OVSRSEL_X2048 << 24)           /**< Shifted mode X2048 for ADC_CTRL */
#define ADC_CTRL_OVSRSEL_X4096                          (_ADC_CTRL_OVSRSEL_X4096 << 24)           /**< Shifted mode X4096 for ADC_CTRL */
#define ADC_CTRL_CHCONMODE                              (0x1UL << 29)                             /**< Channel Connect */
#define _ADC_CTRL_CHCONMODE_SHIFT                       29                                        /**< Shift value for ADC_CHCONMODE */
#define _ADC_CTRL_CHCONMODE_MASK                        0x20000000UL                              /**< Bit mask for ADC_CHCONMODE */
#define _ADC_CTRL_CHCONMODE_DEFAULT                     0x00000000UL                              /**< Mode DEFAULT for ADC_CTRL */
#define _ADC_CTRL_CHCONMODE_MAXSETTLE                   0x00000000UL                              /**< Mode MAXSETTLE for ADC_CTRL */
#define _ADC_CTRL_CHCONMODE_MAXRESP                     0x00000001UL                              /**< Mode MAXRESP for ADC_CTRL */
#define ADC_CTRL_CHCONMODE_DEFAULT                      (_ADC_CTRL_CHCONMODE_DEFAULT << 29)       /**< Shifted mode DEFAULT for ADC_CTRL */
#define ADC_CTRL_CHCONMODE_MAXSETTLE                    (_ADC_CTRL_CHCONMODE_MAXSETTLE << 29)     /**< Shifted mode MAXSETTLE for ADC_CTRL */
#define ADC_CTRL_CHCONMODE_MAXRESP                      (_ADC_CTRL_CHCONMODE_MAXRESP << 29)       /**< Shifted mode MAXRESP for ADC_CTRL */

/* Bit fields for ADC CMD */
#define _ADC_CMD_RESETVALUE                             0x00000000UL                        /**< Default value for ADC_CMD */
#define _ADC_CMD_MASK                                   0x0000000FUL                        /**< Mask for ADC_CMD */
#define ADC_CMD_SINGLESTART                             (0x1UL << 0)                        /**< Single Conversion Start */
#define _ADC_CMD_SINGLESTART_SHIFT                      0                                   /**< Shift value for ADC_SINGLESTART */
#define _ADC_CMD_SINGLESTART_MASK                       0x1UL                               /**< Bit mask for ADC_SINGLESTART */
#define _ADC_CMD_SINGLESTART_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_CMD */
#define ADC_CMD_SINGLESTART_DEFAULT                     (_ADC_CMD_SINGLESTART_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_CMD */
#define ADC_CMD_SINGLESTOP                              (0x1UL << 1)                        /**< Single Conversion Stop */
#define _ADC_CMD_SINGLESTOP_SHIFT                       1                                   /**< Shift value for ADC_SINGLESTOP */
#define _ADC_CMD_SINGLESTOP_MASK                        0x2UL                               /**< Bit mask for ADC_SINGLESTOP */
#define _ADC_CMD_SINGLESTOP_DEFAULT                     0x00000000UL                        /**< Mode DEFAULT for ADC_CMD */
#define ADC_CMD_SINGLESTOP_DEFAULT                      (_ADC_CMD_SINGLESTOP_DEFAULT << 1)  /**< Shifted mode DEFAULT for ADC_CMD */
#define ADC_CMD_SCANSTART                               (0x1UL << 2)                        /**< Scan Sequence Start */
#define _ADC_CMD_SCANSTART_SHIFT                        2                                   /**< Shift value for ADC_SCANSTART */
#define _ADC_CMD_SCANSTART_MASK                         0x4UL                               /**< Bit mask for ADC_SCANSTART */
#define _ADC_CMD_SCANSTART_DEFAULT                      0x00000000UL                        /**< Mode DEFAULT for ADC_CMD */
#define ADC_CMD_SCANSTART_DEFAULT                       (_ADC_CMD_SCANSTART_DEFAULT << 2)   /**< Shifted mode DEFAULT for ADC_CMD */
#define ADC_CMD_SCANSTOP                                (0x1UL << 3)                        /**< Scan Sequence Stop */
#define _ADC_CMD_SCANSTOP_SHIFT                         3                                   /**< Shift value for ADC_SCANSTOP */
#define _ADC_CMD_SCANSTOP_MASK                          0x8UL                               /**< Bit mask for ADC_SCANSTOP */
#define _ADC_CMD_SCANSTOP_DEFAULT                       0x00000000UL                        /**< Mode DEFAULT for ADC_CMD */
#define ADC_CMD_SCANSTOP_DEFAULT                        (_ADC_CMD_SCANSTOP_DEFAULT << 3)    /**< Shifted mode DEFAULT for ADC_CMD */

/* Bit fields for ADC STATUS */
#define _ADC_STATUS_RESETVALUE                          0x00000000UL                             /**< Default value for ADC_STATUS */
#define _ADC_STATUS_MASK                                0x00031F03UL                             /**< Mask for ADC_STATUS */
#define ADC_STATUS_SINGLEACT                            (0x1UL << 0)                             /**< Single Conversion Active */
#define _ADC_STATUS_SINGLEACT_SHIFT                     0                                        /**< Shift value for ADC_SINGLEACT */
#define _ADC_STATUS_SINGLEACT_MASK                      0x1UL                                    /**< Bit mask for ADC_SINGLEACT */
#define _ADC_STATUS_SINGLEACT_DEFAULT                   0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SINGLEACT_DEFAULT                    (_ADC_STATUS_SINGLEACT_DEFAULT << 0)     /**< Shifted mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SCANACT                              (0x1UL << 1)                             /**< Scan Conversion Active */
#define _ADC_STATUS_SCANACT_SHIFT                       1                                        /**< Shift value for ADC_SCANACT */
#define _ADC_STATUS_SCANACT_MASK                        0x2UL                                    /**< Bit mask for ADC_SCANACT */
#define _ADC_STATUS_SCANACT_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SCANACT_DEFAULT                      (_ADC_STATUS_SCANACT_DEFAULT << 1)       /**< Shifted mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SINGLEREFWARM                        (0x1UL << 8)                             /**< Single Reference Warmed Up */
#define _ADC_STATUS_SINGLEREFWARM_SHIFT                 8                                        /**< Shift value for ADC_SINGLEREFWARM */
#define _ADC_STATUS_SINGLEREFWARM_MASK                  0x100UL                                  /**< Bit mask for ADC_SINGLEREFWARM */
#define _ADC_STATUS_SINGLEREFWARM_DEFAULT               0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SINGLEREFWARM_DEFAULT                (_ADC_STATUS_SINGLEREFWARM_DEFAULT << 8) /**< Shifted mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SCANREFWARM                          (0x1UL << 9)                             /**< Scan Reference Warmed Up */
#define _ADC_STATUS_SCANREFWARM_SHIFT                   9                                        /**< Shift value for ADC_SCANREFWARM */
#define _ADC_STATUS_SCANREFWARM_MASK                    0x200UL                                  /**< Bit mask for ADC_SCANREFWARM */
#define _ADC_STATUS_SCANREFWARM_DEFAULT                 0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SCANREFWARM_DEFAULT                  (_ADC_STATUS_SCANREFWARM_DEFAULT << 9)   /**< Shifted mode DEFAULT for ADC_STATUS */
#define _ADC_STATUS_PROGERR_SHIFT                       10                                       /**< Shift value for ADC_PROGERR */
#define _ADC_STATUS_PROGERR_MASK                        0xC00UL                                  /**< Bit mask for ADC_PROGERR */
#define _ADC_STATUS_PROGERR_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define _ADC_STATUS_PROGERR_BUSCONF                     0x00000001UL                             /**< Mode BUSCONF for ADC_STATUS */
#define _ADC_STATUS_PROGERR_NEGSELCONF                  0x00000002UL                             /**< Mode NEGSELCONF for ADC_STATUS */
#define ADC_STATUS_PROGERR_DEFAULT                      (_ADC_STATUS_PROGERR_DEFAULT << 10)      /**< Shifted mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_PROGERR_BUSCONF                      (_ADC_STATUS_PROGERR_BUSCONF << 10)      /**< Shifted mode BUSCONF for ADC_STATUS */
#define ADC_STATUS_PROGERR_NEGSELCONF                   (_ADC_STATUS_PROGERR_NEGSELCONF << 10)   /**< Shifted mode NEGSELCONF for ADC_STATUS */
#define ADC_STATUS_WARM                                 (0x1UL << 12)                            /**< ADC Warmed Up */
#define _ADC_STATUS_WARM_SHIFT                          12                                       /**< Shift value for ADC_WARM */
#define _ADC_STATUS_WARM_MASK                           0x1000UL                                 /**< Bit mask for ADC_WARM */
#define _ADC_STATUS_WARM_DEFAULT                        0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_WARM_DEFAULT                         (_ADC_STATUS_WARM_DEFAULT << 12)         /**< Shifted mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SINGLEDV                             (0x1UL << 16)                            /**< Single Sample Data Valid */
#define _ADC_STATUS_SINGLEDV_SHIFT                      16                                       /**< Shift value for ADC_SINGLEDV */
#define _ADC_STATUS_SINGLEDV_MASK                       0x10000UL                                /**< Bit mask for ADC_SINGLEDV */
#define _ADC_STATUS_SINGLEDV_DEFAULT                    0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SINGLEDV_DEFAULT                     (_ADC_STATUS_SINGLEDV_DEFAULT << 16)     /**< Shifted mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SCANDV                               (0x1UL << 17)                            /**< Scan Data Valid */
#define _ADC_STATUS_SCANDV_SHIFT                        17                                       /**< Shift value for ADC_SCANDV */
#define _ADC_STATUS_SCANDV_MASK                         0x20000UL                                /**< Bit mask for ADC_SCANDV */
#define _ADC_STATUS_SCANDV_DEFAULT                      0x00000000UL                             /**< Mode DEFAULT for ADC_STATUS */
#define ADC_STATUS_SCANDV_DEFAULT                       (_ADC_STATUS_SCANDV_DEFAULT << 17)       /**< Shifted mode DEFAULT for ADC_STATUS */

/* Bit fields for ADC SINGLECTRL */
#define _ADC_SINGLECTRL_RESETVALUE                      0x00FFFF00UL                             /**< Default value for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_MASK                            0xAFFFFFFFUL                             /**< Mask for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REP                              (0x1UL << 0)                             /**< Single Sample Repetitive Mode */
#define _ADC_SINGLECTRL_REP_SHIFT                       0                                        /**< Shift value for ADC_REP */
#define _ADC_SINGLECTRL_REP_MASK                        0x1UL                                    /**< Bit mask for ADC_REP */
#define _ADC_SINGLECTRL_REP_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REP_DEFAULT                      (_ADC_SINGLECTRL_REP_DEFAULT << 0)       /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_DIFF                             (0x1UL << 1)                             /**< Single Sample Differential Mode */
#define _ADC_SINGLECTRL_DIFF_SHIFT                      1                                        /**< Shift value for ADC_DIFF */
#define _ADC_SINGLECTRL_DIFF_MASK                       0x2UL                                    /**< Bit mask for ADC_DIFF */
#define _ADC_SINGLECTRL_DIFF_DEFAULT                    0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_DIFF_DEFAULT                     (_ADC_SINGLECTRL_DIFF_DEFAULT << 1)      /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_ADJ                              (0x1UL << 2)                             /**< Single Sample Result Adjustment */
#define _ADC_SINGLECTRL_ADJ_SHIFT                       2                                        /**< Shift value for ADC_ADJ */
#define _ADC_SINGLECTRL_ADJ_MASK                        0x4UL                                    /**< Bit mask for ADC_ADJ */
#define _ADC_SINGLECTRL_ADJ_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_ADJ_RIGHT                       0x00000000UL                             /**< Mode RIGHT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_ADJ_LEFT                        0x00000001UL                             /**< Mode LEFT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_ADJ_DEFAULT                      (_ADC_SINGLECTRL_ADJ_DEFAULT << 2)       /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_ADJ_RIGHT                        (_ADC_SINGLECTRL_ADJ_RIGHT << 2)         /**< Shifted mode RIGHT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_ADJ_LEFT                         (_ADC_SINGLECTRL_ADJ_LEFT << 2)          /**< Shifted mode LEFT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_RES_SHIFT                       3                                        /**< Shift value for ADC_RES */
#define _ADC_SINGLECTRL_RES_MASK                        0x18UL                                   /**< Bit mask for ADC_RES */
#define _ADC_SINGLECTRL_RES_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_RES_12BIT                       0x00000000UL                             /**< Mode 12BIT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_RES_8BIT                        0x00000001UL                             /**< Mode 8BIT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_RES_6BIT                        0x00000002UL                             /**< Mode 6BIT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_RES_OVS                         0x00000003UL                             /**< Mode OVS for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_RES_DEFAULT                      (_ADC_SINGLECTRL_RES_DEFAULT << 3)       /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_RES_12BIT                        (_ADC_SINGLECTRL_RES_12BIT << 3)         /**< Shifted mode 12BIT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_RES_8BIT                         (_ADC_SINGLECTRL_RES_8BIT << 3)          /**< Shifted mode 8BIT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_RES_6BIT                         (_ADC_SINGLECTRL_RES_6BIT << 3)          /**< Shifted mode 6BIT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_RES_OVS                          (_ADC_SINGLECTRL_RES_OVS << 3)           /**< Shifted mode OVS for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_SHIFT                       5                                        /**< Shift value for ADC_REF */
#define _ADC_SINGLECTRL_REF_MASK                        0xE0UL                                   /**< Bit mask for ADC_REF */
#define _ADC_SINGLECTRL_REF_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_1V25                        0x00000000UL                             /**< Mode 1V25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_2V5                         0x00000001UL                             /**< Mode 2V5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_VDD                         0x00000002UL                             /**< Mode VDD for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_5VDIFF                      0x00000003UL                             /**< Mode 5VDIFF for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_EXTSINGLE                   0x00000004UL                             /**< Mode EXTSINGLE for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_2XEXTDIFF                   0x00000005UL                             /**< Mode 2XEXTDIFF for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_2XVDD                       0x00000006UL                             /**< Mode 2XVDD for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_REF_CONF                        0x00000007UL                             /**< Mode CONF for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_DEFAULT                      (_ADC_SINGLECTRL_REF_DEFAULT << 5)       /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_1V25                         (_ADC_SINGLECTRL_REF_1V25 << 5)          /**< Shifted mode 1V25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_2V5                          (_ADC_SINGLECTRL_REF_2V5 << 5)           /**< Shifted mode 2V5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_VDD                          (_ADC_SINGLECTRL_REF_VDD << 5)           /**< Shifted mode VDD for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_5VDIFF                       (_ADC_SINGLECTRL_REF_5VDIFF << 5)        /**< Shifted mode 5VDIFF for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_EXTSINGLE                    (_ADC_SINGLECTRL_REF_EXTSINGLE << 5)     /**< Shifted mode EXTSINGLE for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_2XEXTDIFF                    (_ADC_SINGLECTRL_REF_2XEXTDIFF << 5)     /**< Shifted mode 2XEXTDIFF for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_2XVDD                        (_ADC_SINGLECTRL_REF_2XVDD << 5)         /**< Shifted mode 2XVDD for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_REF_CONF                         (_ADC_SINGLECTRL_REF_CONF << 5)          /**< Shifted mode CONF for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_SHIFT                    8                                        /**< Shift value for ADC_POSSEL */
#define _ADC_SINGLECTRL_POSSEL_MASK                     0xFF00UL                                 /**< Bit mask for ADC_POSSEL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH0                 0x00000000UL                             /**< Mode BUS0XCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH1                 0x00000001UL                             /**< Mode BUS0XCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH2                 0x00000002UL                             /**< Mode BUS0XCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH3                 0x00000003UL                             /**< Mode BUS0XCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH4                 0x00000004UL                             /**< Mode BUS0XCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH5                 0x00000005UL                             /**< Mode BUS0XCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH6                 0x00000006UL                             /**< Mode BUS0XCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH7                 0x00000007UL                             /**< Mode BUS0XCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH8                 0x00000008UL                             /**< Mode BUS0XCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH9                 0x00000009UL                             /**< Mode BUS0XCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH10                0x0000000AUL                             /**< Mode BUS0XCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH11                0x0000000BUL                             /**< Mode BUS0XCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH12                0x0000000CUL                             /**< Mode BUS0XCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH13                0x0000000DUL                             /**< Mode BUS0XCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH14                0x0000000EUL                             /**< Mode BUS0XCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0XCH15                0x0000000FUL                             /**< Mode BUS0XCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH0                 0x00000010UL                             /**< Mode BUS0YCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH1                 0x00000011UL                             /**< Mode BUS0YCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH2                 0x00000012UL                             /**< Mode BUS0YCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH3                 0x00000013UL                             /**< Mode BUS0YCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH4                 0x00000014UL                             /**< Mode BUS0YCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH5                 0x00000015UL                             /**< Mode BUS0YCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH6                 0x00000016UL                             /**< Mode BUS0YCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH7                 0x00000017UL                             /**< Mode BUS0YCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH8                 0x00000018UL                             /**< Mode BUS0YCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH9                 0x00000019UL                             /**< Mode BUS0YCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH10                0x0000001AUL                             /**< Mode BUS0YCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH11                0x0000001BUL                             /**< Mode BUS0YCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH12                0x0000001CUL                             /**< Mode BUS0YCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH13                0x0000001DUL                             /**< Mode BUS0YCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH14                0x0000001EUL                             /**< Mode BUS0YCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS0YCH15                0x0000001FUL                             /**< Mode BUS0YCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH0                 0x00000020UL                             /**< Mode BUS1XCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH1                 0x00000021UL                             /**< Mode BUS1YCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH2                 0x00000022UL                             /**< Mode BUS1XCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH3                 0x00000023UL                             /**< Mode BUS1YCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH4                 0x00000024UL                             /**< Mode BUS1XCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH5                 0x00000025UL                             /**< Mode BUS1YCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH6                 0x00000026UL                             /**< Mode BUS1XCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH7                 0x00000027UL                             /**< Mode BUS1YCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH8                 0x00000028UL                             /**< Mode BUS1XCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH9                 0x00000029UL                             /**< Mode BUS1YCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH10                0x0000002AUL                             /**< Mode BUS1XCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH11                0x0000002BUL                             /**< Mode BUS1YCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH12                0x0000002CUL                             /**< Mode BUS1XCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH13                0x0000002DUL                             /**< Mode BUS1YCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH14                0x0000002EUL                             /**< Mode BUS1XCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH15                0x0000002FUL                             /**< Mode BUS1YCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH16                0x00000030UL                             /**< Mode BUS1XCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH17                0x00000031UL                             /**< Mode BUS1YCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH18                0x00000032UL                             /**< Mode BUS1XCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH19                0x00000033UL                             /**< Mode BUS1YCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH20                0x00000034UL                             /**< Mode BUS1XCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH21                0x00000035UL                             /**< Mode BUS1YCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH22                0x00000036UL                             /**< Mode BUS1XCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH23                0x00000037UL                             /**< Mode BUS1YCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH24                0x00000038UL                             /**< Mode BUS1XCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH25                0x00000039UL                             /**< Mode BUS1YCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH26                0x0000003AUL                             /**< Mode BUS1XCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH27                0x0000003BUL                             /**< Mode BUS1YCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH28                0x0000003CUL                             /**< Mode BUS1XCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH29                0x0000003DUL                             /**< Mode BUS1YCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1XCH30                0x0000003EUL                             /**< Mode BUS1XCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS1YCH31                0x0000003FUL                             /**< Mode BUS1YCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH0                 0x00000040UL                             /**< Mode BUS2YCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH1                 0x00000041UL                             /**< Mode BUS2XCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH2                 0x00000042UL                             /**< Mode BUS2YCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH3                 0x00000043UL                             /**< Mode BUS2XCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH4                 0x00000044UL                             /**< Mode BUS2YCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH5                 0x00000045UL                             /**< Mode BUS2XCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH6                 0x00000046UL                             /**< Mode BUS2YCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH7                 0x00000047UL                             /**< Mode BUS2XCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH8                 0x00000048UL                             /**< Mode BUS2YCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH9                 0x00000049UL                             /**< Mode BUS2XCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH10                0x0000004AUL                             /**< Mode BUS2YCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH11                0x0000004BUL                             /**< Mode BUS2XCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH12                0x0000004CUL                             /**< Mode BUS2YCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH13                0x0000004DUL                             /**< Mode BUS2XCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH14                0x0000004EUL                             /**< Mode BUS2YCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH15                0x0000004FUL                             /**< Mode BUS2XCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH16                0x00000050UL                             /**< Mode BUS2YCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH17                0x00000051UL                             /**< Mode BUS2XCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH18                0x00000052UL                             /**< Mode BUS2YCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH19                0x00000053UL                             /**< Mode BUS2XCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH20                0x00000054UL                             /**< Mode BUS2YCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH21                0x00000055UL                             /**< Mode BUS2XCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH22                0x00000056UL                             /**< Mode BUS2YCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH23                0x00000057UL                             /**< Mode BUS2XCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH24                0x00000058UL                             /**< Mode BUS2YCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH25                0x00000059UL                             /**< Mode BUS2XCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH26                0x0000005AUL                             /**< Mode BUS2YCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH27                0x0000005BUL                             /**< Mode BUS2XCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH28                0x0000005CUL                             /**< Mode BUS2YCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH29                0x0000005DUL                             /**< Mode BUS2XCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2YCH30                0x0000005EUL                             /**< Mode BUS2YCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS2XCH31                0x0000005FUL                             /**< Mode BUS2XCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH0                 0x00000060UL                             /**< Mode BUS3XCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH1                 0x00000061UL                             /**< Mode BUS3YCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH2                 0x00000062UL                             /**< Mode BUS3XCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH3                 0x00000063UL                             /**< Mode BUS3YCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH4                 0x00000064UL                             /**< Mode BUS3XCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH5                 0x00000065UL                             /**< Mode BUS3YCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH6                 0x00000066UL                             /**< Mode BUS3XCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH7                 0x00000067UL                             /**< Mode BUS3YCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH8                 0x00000068UL                             /**< Mode BUS3XCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH9                 0x00000069UL                             /**< Mode BUS3YCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH10                0x0000006AUL                             /**< Mode BUS3XCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH11                0x0000006BUL                             /**< Mode BUS3YCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH12                0x0000006CUL                             /**< Mode BUS3XCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH13                0x0000006DUL                             /**< Mode BUS3YCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH14                0x0000006EUL                             /**< Mode BUS3XCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH15                0x0000006FUL                             /**< Mode BUS3YCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH16                0x00000070UL                             /**< Mode BUS3XCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH17                0x00000071UL                             /**< Mode BUS3YCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH18                0x00000072UL                             /**< Mode BUS3XCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH19                0x00000073UL                             /**< Mode BUS3YCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH20                0x00000074UL                             /**< Mode BUS3XCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH21                0x00000075UL                             /**< Mode BUS3YCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH22                0x00000076UL                             /**< Mode BUS3XCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH23                0x00000077UL                             /**< Mode BUS3YCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH24                0x00000078UL                             /**< Mode BUS3XCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH25                0x00000079UL                             /**< Mode BUS3YCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH26                0x0000007AUL                             /**< Mode BUS3XCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH27                0x0000007BUL                             /**< Mode BUS3YCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH28                0x0000007CUL                             /**< Mode BUS3XCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH29                0x0000007DUL                             /**< Mode BUS3YCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3XCH30                0x0000007EUL                             /**< Mode BUS3XCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS3YCH31                0x0000007FUL                             /**< Mode BUS3YCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH0                 0x00000080UL                             /**< Mode BUS4YCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH1                 0x00000081UL                             /**< Mode BUS4XCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH2                 0x00000082UL                             /**< Mode BUS4YCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH3                 0x00000083UL                             /**< Mode BUS4XCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH4                 0x00000084UL                             /**< Mode BUS4YCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH5                 0x00000085UL                             /**< Mode BUS4XCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH6                 0x00000086UL                             /**< Mode BUS4YCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH7                 0x00000087UL                             /**< Mode BUS4XCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH8                 0x00000088UL                             /**< Mode BUS4YCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH9                 0x00000089UL                             /**< Mode BUS4XCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH10                0x0000008AUL                             /**< Mode BUS4YCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH11                0x0000008BUL                             /**< Mode BUS4XCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH12                0x0000008CUL                             /**< Mode BUS4YCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH13                0x0000008DUL                             /**< Mode BUS4XCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH14                0x0000008EUL                             /**< Mode BUS4YCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH15                0x0000008FUL                             /**< Mode BUS4XCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH16                0x00000090UL                             /**< Mode BUS4YCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH17                0x00000091UL                             /**< Mode BUS4XCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH18                0x00000092UL                             /**< Mode BUS4YCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH19                0x00000093UL                             /**< Mode BUS4XCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH20                0x00000094UL                             /**< Mode BUS4YCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH21                0x00000095UL                             /**< Mode BUS4XCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH22                0x00000096UL                             /**< Mode BUS4YCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH23                0x00000097UL                             /**< Mode BUS4XCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH24                0x00000098UL                             /**< Mode BUS4YCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH25                0x00000099UL                             /**< Mode BUS4XCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH26                0x0000009AUL                             /**< Mode BUS4YCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH27                0x0000009BUL                             /**< Mode BUS4XCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH28                0x0000009CUL                             /**< Mode BUS4YCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH29                0x0000009DUL                             /**< Mode BUS4XCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4YCH30                0x0000009EUL                             /**< Mode BUS4YCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BUS4XCH31                0x0000009FUL                             /**< Mode BUS4XCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_AVDD                     0x000000E0UL                             /**< Mode AVDD for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_BU                       0x000000E1UL                             /**< Mode BU for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_AREG                     0x000000E2UL                             /**< Mode AREG for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_VREGOUTPA                0x000000E3UL                             /**< Mode VREGOUTPA for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_PDBU                     0x000000E4UL                             /**< Mode PDBU for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_IO0                      0x000000E5UL                             /**< Mode IO0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_IO1                      0x000000E6UL                             /**< Mode IO1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_VSP                      0x000000E7UL                             /**< Mode VSP for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_SP0                      0x000000F2UL                             /**< Mode SP0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_TEMP                     0x000000F3UL                             /**< Mode TEMP for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_DAC0OUT0                 0x000000F4UL                             /**< Mode DAC0OUT0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_TESTP                    0x000000F5UL                             /**< Mode TESTP for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_SP1                      0x000000F6UL                             /**< Mode SP1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_SP2                      0x000000F7UL                             /**< Mode SP2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_DAC0OUT1                 0x000000F8UL                             /**< Mode DAC0OUT1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_SUBLSB                   0x000000F9UL                             /**< Mode SUBLSB for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_DEFAULT                  0x000000FFUL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_POSSEL_VSS                      0x000000FFUL                             /**< Mode VSS for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH0                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH0 << 8)   /**< Shifted mode BUS0XCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH1                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH1 << 8)   /**< Shifted mode BUS0XCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH2                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH2 << 8)   /**< Shifted mode BUS0XCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH3                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH3 << 8)   /**< Shifted mode BUS0XCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH4                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH4 << 8)   /**< Shifted mode BUS0XCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH5                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH5 << 8)   /**< Shifted mode BUS0XCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH6                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH6 << 8)   /**< Shifted mode BUS0XCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH7                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH7 << 8)   /**< Shifted mode BUS0XCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH8                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH8 << 8)   /**< Shifted mode BUS0XCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH9                  (_ADC_SINGLECTRL_POSSEL_BUS0XCH9 << 8)   /**< Shifted mode BUS0XCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH10                 (_ADC_SINGLECTRL_POSSEL_BUS0XCH10 << 8)  /**< Shifted mode BUS0XCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH11                 (_ADC_SINGLECTRL_POSSEL_BUS0XCH11 << 8)  /**< Shifted mode BUS0XCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH12                 (_ADC_SINGLECTRL_POSSEL_BUS0XCH12 << 8)  /**< Shifted mode BUS0XCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH13                 (_ADC_SINGLECTRL_POSSEL_BUS0XCH13 << 8)  /**< Shifted mode BUS0XCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH14                 (_ADC_SINGLECTRL_POSSEL_BUS0XCH14 << 8)  /**< Shifted mode BUS0XCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0XCH15                 (_ADC_SINGLECTRL_POSSEL_BUS0XCH15 << 8)  /**< Shifted mode BUS0XCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH0                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH0 << 8)   /**< Shifted mode BUS0YCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH1                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH1 << 8)   /**< Shifted mode BUS0YCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH2                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH2 << 8)   /**< Shifted mode BUS0YCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH3                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH3 << 8)   /**< Shifted mode BUS0YCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH4                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH4 << 8)   /**< Shifted mode BUS0YCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH5                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH5 << 8)   /**< Shifted mode BUS0YCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH6                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH6 << 8)   /**< Shifted mode BUS0YCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH7                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH7 << 8)   /**< Shifted mode BUS0YCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH8                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH8 << 8)   /**< Shifted mode BUS0YCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH9                  (_ADC_SINGLECTRL_POSSEL_BUS0YCH9 << 8)   /**< Shifted mode BUS0YCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH10                 (_ADC_SINGLECTRL_POSSEL_BUS0YCH10 << 8)  /**< Shifted mode BUS0YCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH11                 (_ADC_SINGLECTRL_POSSEL_BUS0YCH11 << 8)  /**< Shifted mode BUS0YCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH12                 (_ADC_SINGLECTRL_POSSEL_BUS0YCH12 << 8)  /**< Shifted mode BUS0YCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH13                 (_ADC_SINGLECTRL_POSSEL_BUS0YCH13 << 8)  /**< Shifted mode BUS0YCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH14                 (_ADC_SINGLECTRL_POSSEL_BUS0YCH14 << 8)  /**< Shifted mode BUS0YCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS0YCH15                 (_ADC_SINGLECTRL_POSSEL_BUS0YCH15 << 8)  /**< Shifted mode BUS0YCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH0                  (_ADC_SINGLECTRL_POSSEL_BUS1XCH0 << 8)   /**< Shifted mode BUS1XCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH1                  (_ADC_SINGLECTRL_POSSEL_BUS1YCH1 << 8)   /**< Shifted mode BUS1YCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH2                  (_ADC_SINGLECTRL_POSSEL_BUS1XCH2 << 8)   /**< Shifted mode BUS1XCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH3                  (_ADC_SINGLECTRL_POSSEL_BUS1YCH3 << 8)   /**< Shifted mode BUS1YCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH4                  (_ADC_SINGLECTRL_POSSEL_BUS1XCH4 << 8)   /**< Shifted mode BUS1XCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH5                  (_ADC_SINGLECTRL_POSSEL_BUS1YCH5 << 8)   /**< Shifted mode BUS1YCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH6                  (_ADC_SINGLECTRL_POSSEL_BUS1XCH6 << 8)   /**< Shifted mode BUS1XCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH7                  (_ADC_SINGLECTRL_POSSEL_BUS1YCH7 << 8)   /**< Shifted mode BUS1YCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH8                  (_ADC_SINGLECTRL_POSSEL_BUS1XCH8 << 8)   /**< Shifted mode BUS1XCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH9                  (_ADC_SINGLECTRL_POSSEL_BUS1YCH9 << 8)   /**< Shifted mode BUS1YCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH10                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH10 << 8)  /**< Shifted mode BUS1XCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH11                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH11 << 8)  /**< Shifted mode BUS1YCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH12                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH12 << 8)  /**< Shifted mode BUS1XCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH13                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH13 << 8)  /**< Shifted mode BUS1YCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH14                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH14 << 8)  /**< Shifted mode BUS1XCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH15                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH15 << 8)  /**< Shifted mode BUS1YCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH16                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH16 << 8)  /**< Shifted mode BUS1XCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH17                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH17 << 8)  /**< Shifted mode BUS1YCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH18                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH18 << 8)  /**< Shifted mode BUS1XCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH19                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH19 << 8)  /**< Shifted mode BUS1YCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH20                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH20 << 8)  /**< Shifted mode BUS1XCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH21                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH21 << 8)  /**< Shifted mode BUS1YCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH22                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH22 << 8)  /**< Shifted mode BUS1XCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH23                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH23 << 8)  /**< Shifted mode BUS1YCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH24                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH24 << 8)  /**< Shifted mode BUS1XCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH25                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH25 << 8)  /**< Shifted mode BUS1YCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH26                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH26 << 8)  /**< Shifted mode BUS1XCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH27                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH27 << 8)  /**< Shifted mode BUS1YCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH28                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH28 << 8)  /**< Shifted mode BUS1XCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH29                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH29 << 8)  /**< Shifted mode BUS1YCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1XCH30                 (_ADC_SINGLECTRL_POSSEL_BUS1XCH30 << 8)  /**< Shifted mode BUS1XCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS1YCH31                 (_ADC_SINGLECTRL_POSSEL_BUS1YCH31 << 8)  /**< Shifted mode BUS1YCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH0                  (_ADC_SINGLECTRL_POSSEL_BUS2YCH0 << 8)   /**< Shifted mode BUS2YCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH1                  (_ADC_SINGLECTRL_POSSEL_BUS2XCH1 << 8)   /**< Shifted mode BUS2XCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH2                  (_ADC_SINGLECTRL_POSSEL_BUS2YCH2 << 8)   /**< Shifted mode BUS2YCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH3                  (_ADC_SINGLECTRL_POSSEL_BUS2XCH3 << 8)   /**< Shifted mode BUS2XCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH4                  (_ADC_SINGLECTRL_POSSEL_BUS2YCH4 << 8)   /**< Shifted mode BUS2YCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH5                  (_ADC_SINGLECTRL_POSSEL_BUS2XCH5 << 8)   /**< Shifted mode BUS2XCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH6                  (_ADC_SINGLECTRL_POSSEL_BUS2YCH6 << 8)   /**< Shifted mode BUS2YCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH7                  (_ADC_SINGLECTRL_POSSEL_BUS2XCH7 << 8)   /**< Shifted mode BUS2XCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH8                  (_ADC_SINGLECTRL_POSSEL_BUS2YCH8 << 8)   /**< Shifted mode BUS2YCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH9                  (_ADC_SINGLECTRL_POSSEL_BUS2XCH9 << 8)   /**< Shifted mode BUS2XCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH10                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH10 << 8)  /**< Shifted mode BUS2YCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH11                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH11 << 8)  /**< Shifted mode BUS2XCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH12                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH12 << 8)  /**< Shifted mode BUS2YCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH13                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH13 << 8)  /**< Shifted mode BUS2XCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH14                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH14 << 8)  /**< Shifted mode BUS2YCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH15                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH15 << 8)  /**< Shifted mode BUS2XCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH16                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH16 << 8)  /**< Shifted mode BUS2YCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH17                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH17 << 8)  /**< Shifted mode BUS2XCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH18                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH18 << 8)  /**< Shifted mode BUS2YCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH19                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH19 << 8)  /**< Shifted mode BUS2XCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH20                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH20 << 8)  /**< Shifted mode BUS2YCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH21                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH21 << 8)  /**< Shifted mode BUS2XCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH22                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH22 << 8)  /**< Shifted mode BUS2YCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH23                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH23 << 8)  /**< Shifted mode BUS2XCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH24                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH24 << 8)  /**< Shifted mode BUS2YCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH25                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH25 << 8)  /**< Shifted mode BUS2XCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH26                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH26 << 8)  /**< Shifted mode BUS2YCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH27                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH27 << 8)  /**< Shifted mode BUS2XCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH28                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH28 << 8)  /**< Shifted mode BUS2YCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH29                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH29 << 8)  /**< Shifted mode BUS2XCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2YCH30                 (_ADC_SINGLECTRL_POSSEL_BUS2YCH30 << 8)  /**< Shifted mode BUS2YCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS2XCH31                 (_ADC_SINGLECTRL_POSSEL_BUS2XCH31 << 8)  /**< Shifted mode BUS2XCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH0                  (_ADC_SINGLECTRL_POSSEL_BUS3XCH0 << 8)   /**< Shifted mode BUS3XCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH1                  (_ADC_SINGLECTRL_POSSEL_BUS3YCH1 << 8)   /**< Shifted mode BUS3YCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH2                  (_ADC_SINGLECTRL_POSSEL_BUS3XCH2 << 8)   /**< Shifted mode BUS3XCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH3                  (_ADC_SINGLECTRL_POSSEL_BUS3YCH3 << 8)   /**< Shifted mode BUS3YCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH4                  (_ADC_SINGLECTRL_POSSEL_BUS3XCH4 << 8)   /**< Shifted mode BUS3XCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH5                  (_ADC_SINGLECTRL_POSSEL_BUS3YCH5 << 8)   /**< Shifted mode BUS3YCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH6                  (_ADC_SINGLECTRL_POSSEL_BUS3XCH6 << 8)   /**< Shifted mode BUS3XCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH7                  (_ADC_SINGLECTRL_POSSEL_BUS3YCH7 << 8)   /**< Shifted mode BUS3YCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH8                  (_ADC_SINGLECTRL_POSSEL_BUS3XCH8 << 8)   /**< Shifted mode BUS3XCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH9                  (_ADC_SINGLECTRL_POSSEL_BUS3YCH9 << 8)   /**< Shifted mode BUS3YCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH10                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH10 << 8)  /**< Shifted mode BUS3XCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH11                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH11 << 8)  /**< Shifted mode BUS3YCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH12                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH12 << 8)  /**< Shifted mode BUS3XCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH13                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH13 << 8)  /**< Shifted mode BUS3YCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH14                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH14 << 8)  /**< Shifted mode BUS3XCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH15                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH15 << 8)  /**< Shifted mode BUS3YCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH16                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH16 << 8)  /**< Shifted mode BUS3XCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH17                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH17 << 8)  /**< Shifted mode BUS3YCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH18                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH18 << 8)  /**< Shifted mode BUS3XCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH19                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH19 << 8)  /**< Shifted mode BUS3YCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH20                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH20 << 8)  /**< Shifted mode BUS3XCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH21                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH21 << 8)  /**< Shifted mode BUS3YCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH22                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH22 << 8)  /**< Shifted mode BUS3XCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH23                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH23 << 8)  /**< Shifted mode BUS3YCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH24                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH24 << 8)  /**< Shifted mode BUS3XCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH25                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH25 << 8)  /**< Shifted mode BUS3YCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH26                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH26 << 8)  /**< Shifted mode BUS3XCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH27                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH27 << 8)  /**< Shifted mode BUS3YCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH28                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH28 << 8)  /**< Shifted mode BUS3XCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH29                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH29 << 8)  /**< Shifted mode BUS3YCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3XCH30                 (_ADC_SINGLECTRL_POSSEL_BUS3XCH30 << 8)  /**< Shifted mode BUS3XCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS3YCH31                 (_ADC_SINGLECTRL_POSSEL_BUS3YCH31 << 8)  /**< Shifted mode BUS3YCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH0                  (_ADC_SINGLECTRL_POSSEL_BUS4YCH0 << 8)   /**< Shifted mode BUS4YCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH1                  (_ADC_SINGLECTRL_POSSEL_BUS4XCH1 << 8)   /**< Shifted mode BUS4XCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH2                  (_ADC_SINGLECTRL_POSSEL_BUS4YCH2 << 8)   /**< Shifted mode BUS4YCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH3                  (_ADC_SINGLECTRL_POSSEL_BUS4XCH3 << 8)   /**< Shifted mode BUS4XCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH4                  (_ADC_SINGLECTRL_POSSEL_BUS4YCH4 << 8)   /**< Shifted mode BUS4YCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH5                  (_ADC_SINGLECTRL_POSSEL_BUS4XCH5 << 8)   /**< Shifted mode BUS4XCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH6                  (_ADC_SINGLECTRL_POSSEL_BUS4YCH6 << 8)   /**< Shifted mode BUS4YCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH7                  (_ADC_SINGLECTRL_POSSEL_BUS4XCH7 << 8)   /**< Shifted mode BUS4XCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH8                  (_ADC_SINGLECTRL_POSSEL_BUS4YCH8 << 8)   /**< Shifted mode BUS4YCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH9                  (_ADC_SINGLECTRL_POSSEL_BUS4XCH9 << 8)   /**< Shifted mode BUS4XCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH10                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH10 << 8)  /**< Shifted mode BUS4YCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH11                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH11 << 8)  /**< Shifted mode BUS4XCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH12                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH12 << 8)  /**< Shifted mode BUS4YCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH13                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH13 << 8)  /**< Shifted mode BUS4XCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH14                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH14 << 8)  /**< Shifted mode BUS4YCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH15                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH15 << 8)  /**< Shifted mode BUS4XCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH16                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH16 << 8)  /**< Shifted mode BUS4YCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH17                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH17 << 8)  /**< Shifted mode BUS4XCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH18                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH18 << 8)  /**< Shifted mode BUS4YCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH19                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH19 << 8)  /**< Shifted mode BUS4XCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH20                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH20 << 8)  /**< Shifted mode BUS4YCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH21                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH21 << 8)  /**< Shifted mode BUS4XCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH22                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH22 << 8)  /**< Shifted mode BUS4YCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH23                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH23 << 8)  /**< Shifted mode BUS4XCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH24                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH24 << 8)  /**< Shifted mode BUS4YCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH25                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH25 << 8)  /**< Shifted mode BUS4XCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH26                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH26 << 8)  /**< Shifted mode BUS4YCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH27                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH27 << 8)  /**< Shifted mode BUS4XCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH28                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH28 << 8)  /**< Shifted mode BUS4YCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH29                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH29 << 8)  /**< Shifted mode BUS4XCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4YCH30                 (_ADC_SINGLECTRL_POSSEL_BUS4YCH30 << 8)  /**< Shifted mode BUS4YCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BUS4XCH31                 (_ADC_SINGLECTRL_POSSEL_BUS4XCH31 << 8)  /**< Shifted mode BUS4XCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_AVDD                      (_ADC_SINGLECTRL_POSSEL_AVDD << 8)       /**< Shifted mode AVDD for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_BU                        (_ADC_SINGLECTRL_POSSEL_BU << 8)         /**< Shifted mode BU for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_AREG                      (_ADC_SINGLECTRL_POSSEL_AREG << 8)       /**< Shifted mode AREG for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_VREGOUTPA                 (_ADC_SINGLECTRL_POSSEL_VREGOUTPA << 8)  /**< Shifted mode VREGOUTPA for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_PDBU                      (_ADC_SINGLECTRL_POSSEL_PDBU << 8)       /**< Shifted mode PDBU for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_IO0                       (_ADC_SINGLECTRL_POSSEL_IO0 << 8)        /**< Shifted mode IO0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_IO1                       (_ADC_SINGLECTRL_POSSEL_IO1 << 8)        /**< Shifted mode IO1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_VSP                       (_ADC_SINGLECTRL_POSSEL_VSP << 8)        /**< Shifted mode VSP for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_SP0                       (_ADC_SINGLECTRL_POSSEL_SP0 << 8)        /**< Shifted mode SP0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_TEMP                      (_ADC_SINGLECTRL_POSSEL_TEMP << 8)       /**< Shifted mode TEMP for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_DAC0OUT0                  (_ADC_SINGLECTRL_POSSEL_DAC0OUT0 << 8)   /**< Shifted mode DAC0OUT0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_TESTP                     (_ADC_SINGLECTRL_POSSEL_TESTP << 8)      /**< Shifted mode TESTP for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_SP1                       (_ADC_SINGLECTRL_POSSEL_SP1 << 8)        /**< Shifted mode SP1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_SP2                       (_ADC_SINGLECTRL_POSSEL_SP2 << 8)        /**< Shifted mode SP2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_DAC0OUT1                  (_ADC_SINGLECTRL_POSSEL_DAC0OUT1 << 8)   /**< Shifted mode DAC0OUT1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_SUBLSB                    (_ADC_SINGLECTRL_POSSEL_SUBLSB << 8)     /**< Shifted mode SUBLSB for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_DEFAULT                   (_ADC_SINGLECTRL_POSSEL_DEFAULT << 8)    /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_POSSEL_VSS                       (_ADC_SINGLECTRL_POSSEL_VSS << 8)        /**< Shifted mode VSS for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_SHIFT                    16                                       /**< Shift value for ADC_NEGSEL */
#define _ADC_SINGLECTRL_NEGSEL_MASK                     0xFF0000UL                               /**< Bit mask for ADC_NEGSEL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH0                 0x00000000UL                             /**< Mode BUS0XCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH1                 0x00000001UL                             /**< Mode BUS0XCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH2                 0x00000002UL                             /**< Mode BUS0XCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH3                 0x00000003UL                             /**< Mode BUS0XCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH4                 0x00000004UL                             /**< Mode BUS0XCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH5                 0x00000005UL                             /**< Mode BUS0XCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH6                 0x00000006UL                             /**< Mode BUS0XCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH7                 0x00000007UL                             /**< Mode BUS0XCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH8                 0x00000008UL                             /**< Mode BUS0XCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH9                 0x00000009UL                             /**< Mode BUS0XCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH10                0x0000000AUL                             /**< Mode BUS0XCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH11                0x0000000BUL                             /**< Mode BUS0XCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH12                0x0000000CUL                             /**< Mode BUS0XCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH13                0x0000000DUL                             /**< Mode BUS0XCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH14                0x0000000EUL                             /**< Mode BUS0XCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0XCH15                0x0000000FUL                             /**< Mode BUS0XCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH0                 0x00000010UL                             /**< Mode BUS0YCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH1                 0x00000011UL                             /**< Mode BUS0YCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH2                 0x00000012UL                             /**< Mode BUS0YCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH3                 0x00000013UL                             /**< Mode BUS0YCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH4                 0x00000014UL                             /**< Mode BUS0YCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH5                 0x00000015UL                             /**< Mode BUS0YCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH6                 0x00000016UL                             /**< Mode BUS0YCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH7                 0x00000017UL                             /**< Mode BUS0YCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH8                 0x00000018UL                             /**< Mode BUS0YCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH9                 0x00000019UL                             /**< Mode BUS0YCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH10                0x0000001AUL                             /**< Mode BUS0YCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH11                0x0000001BUL                             /**< Mode BUS0YCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH12                0x0000001CUL                             /**< Mode BUS0YCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH13                0x0000001DUL                             /**< Mode BUS0YCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH14                0x0000001EUL                             /**< Mode BUS0YCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS0YCH15                0x0000001FUL                             /**< Mode BUS0YCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH0                 0x00000020UL                             /**< Mode BUS1XCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH1                 0x00000021UL                             /**< Mode BUS1YCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH2                 0x00000022UL                             /**< Mode BUS1XCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH3                 0x00000023UL                             /**< Mode BUS1YCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH4                 0x00000024UL                             /**< Mode BUS1XCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH5                 0x00000025UL                             /**< Mode BUS1YCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH6                 0x00000026UL                             /**< Mode BUS1XCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH7                 0x00000027UL                             /**< Mode BUS1YCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH8                 0x00000028UL                             /**< Mode BUS1XCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH9                 0x00000029UL                             /**< Mode BUS1YCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH10                0x0000002AUL                             /**< Mode BUS1XCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH11                0x0000002BUL                             /**< Mode BUS1YCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH12                0x0000002CUL                             /**< Mode BUS1XCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH13                0x0000002DUL                             /**< Mode BUS1YCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH14                0x0000002EUL                             /**< Mode BUS1XCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH15                0x0000002FUL                             /**< Mode BUS1YCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH16                0x00000030UL                             /**< Mode BUS1XCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH17                0x00000031UL                             /**< Mode BUS1YCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH18                0x00000032UL                             /**< Mode BUS1XCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH19                0x00000033UL                             /**< Mode BUS1YCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH20                0x00000034UL                             /**< Mode BUS1XCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH21                0x00000035UL                             /**< Mode BUS1YCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH22                0x00000036UL                             /**< Mode BUS1XCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH23                0x00000037UL                             /**< Mode BUS1YCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH24                0x00000038UL                             /**< Mode BUS1XCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH25                0x00000039UL                             /**< Mode BUS1YCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH26                0x0000003AUL                             /**< Mode BUS1XCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH27                0x0000003BUL                             /**< Mode BUS1YCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH28                0x0000003CUL                             /**< Mode BUS1XCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH29                0x0000003DUL                             /**< Mode BUS1YCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1XCH30                0x0000003EUL                             /**< Mode BUS1XCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS1YCH31                0x0000003FUL                             /**< Mode BUS1YCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH0                 0x00000040UL                             /**< Mode BUS2YCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH1                 0x00000041UL                             /**< Mode BUS2XCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH2                 0x00000042UL                             /**< Mode BUS2YCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH3                 0x00000043UL                             /**< Mode BUS2XCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH4                 0x00000044UL                             /**< Mode BUS2YCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH5                 0x00000045UL                             /**< Mode BUS2XCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH6                 0x00000046UL                             /**< Mode BUS2YCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH7                 0x00000047UL                             /**< Mode BUS2XCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH8                 0x00000048UL                             /**< Mode BUS2YCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH9                 0x00000049UL                             /**< Mode BUS2XCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH10                0x0000004AUL                             /**< Mode BUS2YCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH11                0x0000004BUL                             /**< Mode BUS2XCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH12                0x0000004CUL                             /**< Mode BUS2YCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH13                0x0000004DUL                             /**< Mode BUS2XCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH14                0x0000004EUL                             /**< Mode BUS2YCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH15                0x0000004FUL                             /**< Mode BUS2XCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH16                0x00000050UL                             /**< Mode BUS2YCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH17                0x00000051UL                             /**< Mode BUS2XCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH18                0x00000052UL                             /**< Mode BUS2YCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH19                0x00000053UL                             /**< Mode BUS2XCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH20                0x00000054UL                             /**< Mode BUS2YCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH21                0x00000055UL                             /**< Mode BUS2XCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH22                0x00000056UL                             /**< Mode BUS2YCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH23                0x00000057UL                             /**< Mode BUS2XCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH24                0x00000058UL                             /**< Mode BUS2YCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH25                0x00000059UL                             /**< Mode BUS2XCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH26                0x0000005AUL                             /**< Mode BUS2YCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH27                0x0000005BUL                             /**< Mode BUS2XCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH28                0x0000005CUL                             /**< Mode BUS2YCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH29                0x0000005DUL                             /**< Mode BUS2XCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2YCH30                0x0000005EUL                             /**< Mode BUS2YCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS2XCH31                0x0000005FUL                             /**< Mode BUS2XCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH0                 0x00000060UL                             /**< Mode BUS3XCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH1                 0x00000061UL                             /**< Mode BUS3YCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH2                 0x00000062UL                             /**< Mode BUS3XCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH3                 0x00000063UL                             /**< Mode BUS3YCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH4                 0x00000064UL                             /**< Mode BUS3XCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH5                 0x00000065UL                             /**< Mode BUS3YCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH6                 0x00000066UL                             /**< Mode BUS3XCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH7                 0x00000067UL                             /**< Mode BUS3YCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH8                 0x00000068UL                             /**< Mode BUS3XCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH9                 0x00000069UL                             /**< Mode BUS3YCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH10                0x0000006AUL                             /**< Mode BUS3XCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH11                0x0000006BUL                             /**< Mode BUS3YCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH12                0x0000006CUL                             /**< Mode BUS3XCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH13                0x0000006DUL                             /**< Mode BUS3YCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH14                0x0000006EUL                             /**< Mode BUS3XCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH15                0x0000006FUL                             /**< Mode BUS3YCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH16                0x00000070UL                             /**< Mode BUS3XCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH17                0x00000071UL                             /**< Mode BUS3YCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH18                0x00000072UL                             /**< Mode BUS3XCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH19                0x00000073UL                             /**< Mode BUS3YCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH20                0x00000074UL                             /**< Mode BUS3XCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH21                0x00000075UL                             /**< Mode BUS3YCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH22                0x00000076UL                             /**< Mode BUS3XCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH23                0x00000077UL                             /**< Mode BUS3YCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH24                0x00000078UL                             /**< Mode BUS3XCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH25                0x00000079UL                             /**< Mode BUS3YCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH26                0x0000007AUL                             /**< Mode BUS3XCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH27                0x0000007BUL                             /**< Mode BUS3YCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH28                0x0000007CUL                             /**< Mode BUS3XCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH29                0x0000007DUL                             /**< Mode BUS3YCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3XCH30                0x0000007EUL                             /**< Mode BUS3XCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS3YCH31                0x0000007FUL                             /**< Mode BUS3YCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH0                 0x00000080UL                             /**< Mode BUS4YCH0 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH1                 0x00000081UL                             /**< Mode BUS4XCH1 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH2                 0x00000082UL                             /**< Mode BUS4YCH2 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH3                 0x00000083UL                             /**< Mode BUS4XCH3 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH4                 0x00000084UL                             /**< Mode BUS4YCH4 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH5                 0x00000085UL                             /**< Mode BUS4XCH5 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH6                 0x00000086UL                             /**< Mode BUS4YCH6 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH7                 0x00000087UL                             /**< Mode BUS4XCH7 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH8                 0x00000088UL                             /**< Mode BUS4YCH8 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH9                 0x00000089UL                             /**< Mode BUS4XCH9 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH10                0x0000008AUL                             /**< Mode BUS4YCH10 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH11                0x0000008BUL                             /**< Mode BUS4XCH11 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH12                0x0000008CUL                             /**< Mode BUS4YCH12 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH13                0x0000008DUL                             /**< Mode BUS4XCH13 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH14                0x0000008EUL                             /**< Mode BUS4YCH14 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH15                0x0000008FUL                             /**< Mode BUS4XCH15 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH16                0x00000090UL                             /**< Mode BUS4YCH16 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH17                0x00000091UL                             /**< Mode BUS4XCH17 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH18                0x00000092UL                             /**< Mode BUS4YCH18 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH19                0x00000093UL                             /**< Mode BUS4XCH19 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH20                0x00000094UL                             /**< Mode BUS4YCH20 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH21                0x00000095UL                             /**< Mode BUS4XCH21 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH22                0x00000096UL                             /**< Mode BUS4YCH22 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH23                0x00000097UL                             /**< Mode BUS4XCH23 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH24                0x00000098UL                             /**< Mode BUS4YCH24 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH25                0x00000099UL                             /**< Mode BUS4XCH25 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH26                0x0000009AUL                             /**< Mode BUS4YCH26 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH27                0x0000009BUL                             /**< Mode BUS4XCH27 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH28                0x0000009CUL                             /**< Mode BUS4YCH28 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH29                0x0000009DUL                             /**< Mode BUS4XCH29 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4YCH30                0x0000009EUL                             /**< Mode BUS4YCH30 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_BUS4XCH31                0x0000009FUL                             /**< Mode BUS4XCH31 for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_TESTN                    0x000000F5UL                             /**< Mode TESTN for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_DEFAULT                  0x000000FFUL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_NEGSEL_VSS                      0x000000FFUL                             /**< Mode VSS for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH0                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH0 << 16)  /**< Shifted mode BUS0XCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH1                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH1 << 16)  /**< Shifted mode BUS0XCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH2                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH2 << 16)  /**< Shifted mode BUS0XCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH3                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH3 << 16)  /**< Shifted mode BUS0XCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH4                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH4 << 16)  /**< Shifted mode BUS0XCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH5                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH5 << 16)  /**< Shifted mode BUS0XCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH6                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH6 << 16)  /**< Shifted mode BUS0XCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH7                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH7 << 16)  /**< Shifted mode BUS0XCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH8                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH8 << 16)  /**< Shifted mode BUS0XCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH9                  (_ADC_SINGLECTRL_NEGSEL_BUS0XCH9 << 16)  /**< Shifted mode BUS0XCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH10                 (_ADC_SINGLECTRL_NEGSEL_BUS0XCH10 << 16) /**< Shifted mode BUS0XCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH11                 (_ADC_SINGLECTRL_NEGSEL_BUS0XCH11 << 16) /**< Shifted mode BUS0XCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH12                 (_ADC_SINGLECTRL_NEGSEL_BUS0XCH12 << 16) /**< Shifted mode BUS0XCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH13                 (_ADC_SINGLECTRL_NEGSEL_BUS0XCH13 << 16) /**< Shifted mode BUS0XCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH14                 (_ADC_SINGLECTRL_NEGSEL_BUS0XCH14 << 16) /**< Shifted mode BUS0XCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0XCH15                 (_ADC_SINGLECTRL_NEGSEL_BUS0XCH15 << 16) /**< Shifted mode BUS0XCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH0                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH0 << 16)  /**< Shifted mode BUS0YCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH1                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH1 << 16)  /**< Shifted mode BUS0YCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH2                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH2 << 16)  /**< Shifted mode BUS0YCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH3                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH3 << 16)  /**< Shifted mode BUS0YCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH4                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH4 << 16)  /**< Shifted mode BUS0YCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH5                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH5 << 16)  /**< Shifted mode BUS0YCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH6                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH6 << 16)  /**< Shifted mode BUS0YCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH7                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH7 << 16)  /**< Shifted mode BUS0YCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH8                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH8 << 16)  /**< Shifted mode BUS0YCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH9                  (_ADC_SINGLECTRL_NEGSEL_BUS0YCH9 << 16)  /**< Shifted mode BUS0YCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH10                 (_ADC_SINGLECTRL_NEGSEL_BUS0YCH10 << 16) /**< Shifted mode BUS0YCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH11                 (_ADC_SINGLECTRL_NEGSEL_BUS0YCH11 << 16) /**< Shifted mode BUS0YCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH12                 (_ADC_SINGLECTRL_NEGSEL_BUS0YCH12 << 16) /**< Shifted mode BUS0YCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH13                 (_ADC_SINGLECTRL_NEGSEL_BUS0YCH13 << 16) /**< Shifted mode BUS0YCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH14                 (_ADC_SINGLECTRL_NEGSEL_BUS0YCH14 << 16) /**< Shifted mode BUS0YCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS0YCH15                 (_ADC_SINGLECTRL_NEGSEL_BUS0YCH15 << 16) /**< Shifted mode BUS0YCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH0                  (_ADC_SINGLECTRL_NEGSEL_BUS1XCH0 << 16)  /**< Shifted mode BUS1XCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH1                  (_ADC_SINGLECTRL_NEGSEL_BUS1YCH1 << 16)  /**< Shifted mode BUS1YCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH2                  (_ADC_SINGLECTRL_NEGSEL_BUS1XCH2 << 16)  /**< Shifted mode BUS1XCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH3                  (_ADC_SINGLECTRL_NEGSEL_BUS1YCH3 << 16)  /**< Shifted mode BUS1YCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH4                  (_ADC_SINGLECTRL_NEGSEL_BUS1XCH4 << 16)  /**< Shifted mode BUS1XCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH5                  (_ADC_SINGLECTRL_NEGSEL_BUS1YCH5 << 16)  /**< Shifted mode BUS1YCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH6                  (_ADC_SINGLECTRL_NEGSEL_BUS1XCH6 << 16)  /**< Shifted mode BUS1XCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH7                  (_ADC_SINGLECTRL_NEGSEL_BUS1YCH7 << 16)  /**< Shifted mode BUS1YCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH8                  (_ADC_SINGLECTRL_NEGSEL_BUS1XCH8 << 16)  /**< Shifted mode BUS1XCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH9                  (_ADC_SINGLECTRL_NEGSEL_BUS1YCH9 << 16)  /**< Shifted mode BUS1YCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH10                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH10 << 16) /**< Shifted mode BUS1XCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH11                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH11 << 16) /**< Shifted mode BUS1YCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH12                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH12 << 16) /**< Shifted mode BUS1XCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH13                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH13 << 16) /**< Shifted mode BUS1YCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH14                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH14 << 16) /**< Shifted mode BUS1XCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH15                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH15 << 16) /**< Shifted mode BUS1YCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH16                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH16 << 16) /**< Shifted mode BUS1XCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH17                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH17 << 16) /**< Shifted mode BUS1YCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH18                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH18 << 16) /**< Shifted mode BUS1XCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH19                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH19 << 16) /**< Shifted mode BUS1YCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH20                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH20 << 16) /**< Shifted mode BUS1XCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH21                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH21 << 16) /**< Shifted mode BUS1YCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH22                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH22 << 16) /**< Shifted mode BUS1XCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH23                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH23 << 16) /**< Shifted mode BUS1YCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH24                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH24 << 16) /**< Shifted mode BUS1XCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH25                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH25 << 16) /**< Shifted mode BUS1YCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH26                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH26 << 16) /**< Shifted mode BUS1XCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH27                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH27 << 16) /**< Shifted mode BUS1YCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH28                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH28 << 16) /**< Shifted mode BUS1XCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH29                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH29 << 16) /**< Shifted mode BUS1YCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1XCH30                 (_ADC_SINGLECTRL_NEGSEL_BUS1XCH30 << 16) /**< Shifted mode BUS1XCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS1YCH31                 (_ADC_SINGLECTRL_NEGSEL_BUS1YCH31 << 16) /**< Shifted mode BUS1YCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH0                  (_ADC_SINGLECTRL_NEGSEL_BUS2YCH0 << 16)  /**< Shifted mode BUS2YCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH1                  (_ADC_SINGLECTRL_NEGSEL_BUS2XCH1 << 16)  /**< Shifted mode BUS2XCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH2                  (_ADC_SINGLECTRL_NEGSEL_BUS2YCH2 << 16)  /**< Shifted mode BUS2YCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH3                  (_ADC_SINGLECTRL_NEGSEL_BUS2XCH3 << 16)  /**< Shifted mode BUS2XCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH4                  (_ADC_SINGLECTRL_NEGSEL_BUS2YCH4 << 16)  /**< Shifted mode BUS2YCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH5                  (_ADC_SINGLECTRL_NEGSEL_BUS2XCH5 << 16)  /**< Shifted mode BUS2XCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH6                  (_ADC_SINGLECTRL_NEGSEL_BUS2YCH6 << 16)  /**< Shifted mode BUS2YCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH7                  (_ADC_SINGLECTRL_NEGSEL_BUS2XCH7 << 16)  /**< Shifted mode BUS2XCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH8                  (_ADC_SINGLECTRL_NEGSEL_BUS2YCH8 << 16)  /**< Shifted mode BUS2YCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH9                  (_ADC_SINGLECTRL_NEGSEL_BUS2XCH9 << 16)  /**< Shifted mode BUS2XCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH10                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH10 << 16) /**< Shifted mode BUS2YCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH11                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH11 << 16) /**< Shifted mode BUS2XCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH12                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH12 << 16) /**< Shifted mode BUS2YCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH13                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH13 << 16) /**< Shifted mode BUS2XCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH14                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH14 << 16) /**< Shifted mode BUS2YCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH15                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH15 << 16) /**< Shifted mode BUS2XCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH16                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH16 << 16) /**< Shifted mode BUS2YCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH17                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH17 << 16) /**< Shifted mode BUS2XCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH18                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH18 << 16) /**< Shifted mode BUS2YCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH19                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH19 << 16) /**< Shifted mode BUS2XCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH20                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH20 << 16) /**< Shifted mode BUS2YCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH21                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH21 << 16) /**< Shifted mode BUS2XCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH22                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH22 << 16) /**< Shifted mode BUS2YCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH23                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH23 << 16) /**< Shifted mode BUS2XCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH24                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH24 << 16) /**< Shifted mode BUS2YCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH25                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH25 << 16) /**< Shifted mode BUS2XCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH26                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH26 << 16) /**< Shifted mode BUS2YCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH27                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH27 << 16) /**< Shifted mode BUS2XCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH28                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH28 << 16) /**< Shifted mode BUS2YCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH29                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH29 << 16) /**< Shifted mode BUS2XCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2YCH30                 (_ADC_SINGLECTRL_NEGSEL_BUS2YCH30 << 16) /**< Shifted mode BUS2YCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS2XCH31                 (_ADC_SINGLECTRL_NEGSEL_BUS2XCH31 << 16) /**< Shifted mode BUS2XCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH0                  (_ADC_SINGLECTRL_NEGSEL_BUS3XCH0 << 16)  /**< Shifted mode BUS3XCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH1                  (_ADC_SINGLECTRL_NEGSEL_BUS3YCH1 << 16)  /**< Shifted mode BUS3YCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH2                  (_ADC_SINGLECTRL_NEGSEL_BUS3XCH2 << 16)  /**< Shifted mode BUS3XCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH3                  (_ADC_SINGLECTRL_NEGSEL_BUS3YCH3 << 16)  /**< Shifted mode BUS3YCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH4                  (_ADC_SINGLECTRL_NEGSEL_BUS3XCH4 << 16)  /**< Shifted mode BUS3XCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH5                  (_ADC_SINGLECTRL_NEGSEL_BUS3YCH5 << 16)  /**< Shifted mode BUS3YCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH6                  (_ADC_SINGLECTRL_NEGSEL_BUS3XCH6 << 16)  /**< Shifted mode BUS3XCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH7                  (_ADC_SINGLECTRL_NEGSEL_BUS3YCH7 << 16)  /**< Shifted mode BUS3YCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH8                  (_ADC_SINGLECTRL_NEGSEL_BUS3XCH8 << 16)  /**< Shifted mode BUS3XCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH9                  (_ADC_SINGLECTRL_NEGSEL_BUS3YCH9 << 16)  /**< Shifted mode BUS3YCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH10                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH10 << 16) /**< Shifted mode BUS3XCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH11                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH11 << 16) /**< Shifted mode BUS3YCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH12                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH12 << 16) /**< Shifted mode BUS3XCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH13                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH13 << 16) /**< Shifted mode BUS3YCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH14                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH14 << 16) /**< Shifted mode BUS3XCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH15                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH15 << 16) /**< Shifted mode BUS3YCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH16                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH16 << 16) /**< Shifted mode BUS3XCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH17                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH17 << 16) /**< Shifted mode BUS3YCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH18                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH18 << 16) /**< Shifted mode BUS3XCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH19                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH19 << 16) /**< Shifted mode BUS3YCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH20                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH20 << 16) /**< Shifted mode BUS3XCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH21                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH21 << 16) /**< Shifted mode BUS3YCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH22                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH22 << 16) /**< Shifted mode BUS3XCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH23                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH23 << 16) /**< Shifted mode BUS3YCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH24                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH24 << 16) /**< Shifted mode BUS3XCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH25                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH25 << 16) /**< Shifted mode BUS3YCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH26                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH26 << 16) /**< Shifted mode BUS3XCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH27                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH27 << 16) /**< Shifted mode BUS3YCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH28                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH28 << 16) /**< Shifted mode BUS3XCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH29                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH29 << 16) /**< Shifted mode BUS3YCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3XCH30                 (_ADC_SINGLECTRL_NEGSEL_BUS3XCH30 << 16) /**< Shifted mode BUS3XCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS3YCH31                 (_ADC_SINGLECTRL_NEGSEL_BUS3YCH31 << 16) /**< Shifted mode BUS3YCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH0                  (_ADC_SINGLECTRL_NEGSEL_BUS4YCH0 << 16)  /**< Shifted mode BUS4YCH0 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH1                  (_ADC_SINGLECTRL_NEGSEL_BUS4XCH1 << 16)  /**< Shifted mode BUS4XCH1 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH2                  (_ADC_SINGLECTRL_NEGSEL_BUS4YCH2 << 16)  /**< Shifted mode BUS4YCH2 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH3                  (_ADC_SINGLECTRL_NEGSEL_BUS4XCH3 << 16)  /**< Shifted mode BUS4XCH3 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH4                  (_ADC_SINGLECTRL_NEGSEL_BUS4YCH4 << 16)  /**< Shifted mode BUS4YCH4 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH5                  (_ADC_SINGLECTRL_NEGSEL_BUS4XCH5 << 16)  /**< Shifted mode BUS4XCH5 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH6                  (_ADC_SINGLECTRL_NEGSEL_BUS4YCH6 << 16)  /**< Shifted mode BUS4YCH6 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH7                  (_ADC_SINGLECTRL_NEGSEL_BUS4XCH7 << 16)  /**< Shifted mode BUS4XCH7 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH8                  (_ADC_SINGLECTRL_NEGSEL_BUS4YCH8 << 16)  /**< Shifted mode BUS4YCH8 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH9                  (_ADC_SINGLECTRL_NEGSEL_BUS4XCH9 << 16)  /**< Shifted mode BUS4XCH9 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH10                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH10 << 16) /**< Shifted mode BUS4YCH10 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH11                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH11 << 16) /**< Shifted mode BUS4XCH11 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH12                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH12 << 16) /**< Shifted mode BUS4YCH12 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH13                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH13 << 16) /**< Shifted mode BUS4XCH13 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH14                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH14 << 16) /**< Shifted mode BUS4YCH14 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH15                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH15 << 16) /**< Shifted mode BUS4XCH15 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH16                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH16 << 16) /**< Shifted mode BUS4YCH16 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH17                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH17 << 16) /**< Shifted mode BUS4XCH17 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH18                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH18 << 16) /**< Shifted mode BUS4YCH18 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH19                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH19 << 16) /**< Shifted mode BUS4XCH19 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH20                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH20 << 16) /**< Shifted mode BUS4YCH20 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH21                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH21 << 16) /**< Shifted mode BUS4XCH21 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH22                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH22 << 16) /**< Shifted mode BUS4YCH22 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH23                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH23 << 16) /**< Shifted mode BUS4XCH23 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH24                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH24 << 16) /**< Shifted mode BUS4YCH24 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH25                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH25 << 16) /**< Shifted mode BUS4XCH25 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH26                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH26 << 16) /**< Shifted mode BUS4YCH26 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH27                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH27 << 16) /**< Shifted mode BUS4XCH27 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH28                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH28 << 16) /**< Shifted mode BUS4YCH28 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH29                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH29 << 16) /**< Shifted mode BUS4XCH29 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4YCH30                 (_ADC_SINGLECTRL_NEGSEL_BUS4YCH30 << 16) /**< Shifted mode BUS4YCH30 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_BUS4XCH31                 (_ADC_SINGLECTRL_NEGSEL_BUS4XCH31 << 16) /**< Shifted mode BUS4XCH31 for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_TESTN                     (_ADC_SINGLECTRL_NEGSEL_TESTN << 16)     /**< Shifted mode TESTN for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_DEFAULT                   (_ADC_SINGLECTRL_NEGSEL_DEFAULT << 16)   /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_NEGSEL_VSS                       (_ADC_SINGLECTRL_NEGSEL_VSS << 16)       /**< Shifted mode VSS for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_SHIFT                        24                                       /**< Shift value for ADC_AT */
#define _ADC_SINGLECTRL_AT_MASK                         0xF000000UL                              /**< Bit mask for ADC_AT */
#define _ADC_SINGLECTRL_AT_DEFAULT                      0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_1CYCLE                       0x00000000UL                             /**< Mode 1CYCLE for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_2CYCLES                      0x00000001UL                             /**< Mode 2CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_3CYCLES                      0x00000002UL                             /**< Mode 3CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_4CYCLES                      0x00000003UL                             /**< Mode 4CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_8CYCLES                      0x00000004UL                             /**< Mode 8CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_16CYCLES                     0x00000005UL                             /**< Mode 16CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_32CYCLES                     0x00000006UL                             /**< Mode 32CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_64CYCLES                     0x00000007UL                             /**< Mode 64CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_128CYCLES                    0x00000008UL                             /**< Mode 128CYCLES for ADC_SINGLECTRL */
#define _ADC_SINGLECTRL_AT_256CYCLES                    0x00000009UL                             /**< Mode 256CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_DEFAULT                       (_ADC_SINGLECTRL_AT_DEFAULT << 24)       /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_1CYCLE                        (_ADC_SINGLECTRL_AT_1CYCLE << 24)        /**< Shifted mode 1CYCLE for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_2CYCLES                       (_ADC_SINGLECTRL_AT_2CYCLES << 24)       /**< Shifted mode 2CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_3CYCLES                       (_ADC_SINGLECTRL_AT_3CYCLES << 24)       /**< Shifted mode 3CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_4CYCLES                       (_ADC_SINGLECTRL_AT_4CYCLES << 24)       /**< Shifted mode 4CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_8CYCLES                       (_ADC_SINGLECTRL_AT_8CYCLES << 24)       /**< Shifted mode 8CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_16CYCLES                      (_ADC_SINGLECTRL_AT_16CYCLES << 24)      /**< Shifted mode 16CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_32CYCLES                      (_ADC_SINGLECTRL_AT_32CYCLES << 24)      /**< Shifted mode 32CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_64CYCLES                      (_ADC_SINGLECTRL_AT_64CYCLES << 24)      /**< Shifted mode 64CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_128CYCLES                     (_ADC_SINGLECTRL_AT_128CYCLES << 24)     /**< Shifted mode 128CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_AT_256CYCLES                     (_ADC_SINGLECTRL_AT_256CYCLES << 24)     /**< Shifted mode 256CYCLES for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_PRSEN                            (0x1UL << 29)                            /**< Single Sample PRS Trigger Enable */
#define _ADC_SINGLECTRL_PRSEN_SHIFT                     29                                       /**< Shift value for ADC_PRSEN */
#define _ADC_SINGLECTRL_PRSEN_MASK                      0x20000000UL                             /**< Bit mask for ADC_PRSEN */
#define _ADC_SINGLECTRL_PRSEN_DEFAULT                   0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_PRSEN_DEFAULT                    (_ADC_SINGLECTRL_PRSEN_DEFAULT << 29)    /**< Shifted mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_CMPEN                            (0x1UL << 31)                            /**< Compare Logic Enable for Single Sample */
#define _ADC_SINGLECTRL_CMPEN_SHIFT                     31                                       /**< Shift value for ADC_CMPEN */
#define _ADC_SINGLECTRL_CMPEN_MASK                      0x80000000UL                             /**< Bit mask for ADC_CMPEN */
#define _ADC_SINGLECTRL_CMPEN_DEFAULT                   0x00000000UL                             /**< Mode DEFAULT for ADC_SINGLECTRL */
#define ADC_SINGLECTRL_CMPEN_DEFAULT                    (_ADC_SINGLECTRL_CMPEN_DEFAULT << 31)    /**< Shifted mode DEFAULT for ADC_SINGLECTRL */

/* Bit fields for ADC SINGLECTRLX */
#define _ADC_SINGLECTRLX_RESETVALUE                     0x00000000UL                                      /**< Default value for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_MASK                           0x0F1F7FFFUL                                      /**< Mask for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_SHIFT                  0                                                 /**< Shift value for ADC_VREFSEL */
#define _ADC_SINGLECTRLX_VREFSEL_MASK                   0x7UL                                             /**< Bit mask for ADC_VREFSEL */
#define _ADC_SINGLECTRLX_VREFSEL_DEFAULT                0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VBGR                   0x00000000UL                                      /**< Mode VBGR for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VDDXWATT               0x00000001UL                                      /**< Mode VDDXWATT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VREFPWATT              0x00000002UL                                      /**< Mode VREFPWATT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VREFP                  0x00000003UL                                      /**< Mode VREFP for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VENTROPY               0x00000004UL                                      /**< Mode VENTROPY for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VREFPNWATT             0x00000005UL                                      /**< Mode VREFPNWATT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VREFPN                 0x00000006UL                                      /**< Mode VREFPN for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFSEL_VBGRLOW                0x00000007UL                                      /**< Mode VBGRLOW for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_DEFAULT                 (_ADC_SINGLECTRLX_VREFSEL_DEFAULT << 0)           /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VBGR                    (_ADC_SINGLECTRLX_VREFSEL_VBGR << 0)              /**< Shifted mode VBGR for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VDDXWATT                (_ADC_SINGLECTRLX_VREFSEL_VDDXWATT << 0)          /**< Shifted mode VDDXWATT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VREFPWATT               (_ADC_SINGLECTRLX_VREFSEL_VREFPWATT << 0)         /**< Shifted mode VREFPWATT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VREFP                   (_ADC_SINGLECTRLX_VREFSEL_VREFP << 0)             /**< Shifted mode VREFP for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VENTROPY                (_ADC_SINGLECTRLX_VREFSEL_VENTROPY << 0)          /**< Shifted mode VENTROPY for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VREFPNWATT              (_ADC_SINGLECTRLX_VREFSEL_VREFPNWATT << 0)        /**< Shifted mode VREFPNWATT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VREFPN                  (_ADC_SINGLECTRLX_VREFSEL_VREFPN << 0)            /**< Shifted mode VREFPN for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFSEL_VBGRLOW                 (_ADC_SINGLECTRLX_VREFSEL_VBGRLOW << 0)           /**< Shifted mode VBGRLOW for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFATTFIX                      (0x1UL << 3)                                      /**< Enable 1/3 scaling on VREF */
#define _ADC_SINGLECTRLX_VREFATTFIX_SHIFT               3                                                 /**< Shift value for ADC_VREFATTFIX */
#define _ADC_SINGLECTRLX_VREFATTFIX_MASK                0x8UL                                             /**< Bit mask for ADC_VREFATTFIX */
#define _ADC_SINGLECTRLX_VREFATTFIX_DEFAULT             0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFATTFIX_DEFAULT              (_ADC_SINGLECTRLX_VREFATTFIX_DEFAULT << 3)        /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VREFATT_SHIFT                  4                                                 /**< Shift value for ADC_VREFATT */
#define _ADC_SINGLECTRLX_VREFATT_MASK                   0xF0UL                                            /**< Bit mask for ADC_VREFATT */
#define _ADC_SINGLECTRLX_VREFATT_DEFAULT                0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VREFATT_DEFAULT                 (_ADC_SINGLECTRLX_VREFATT_DEFAULT << 4)           /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_VINATT_SHIFT                   8                                                 /**< Shift value for ADC_VINATT */
#define _ADC_SINGLECTRLX_VINATT_MASK                    0xF00UL                                           /**< Bit mask for ADC_VINATT */
#define _ADC_SINGLECTRLX_VINATT_DEFAULT                 0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_VINATT_DEFAULT                  (_ADC_SINGLECTRLX_VINATT_DEFAULT << 8)            /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_DVL_SHIFT                      12                                                /**< Shift value for ADC_DVL */
#define _ADC_SINGLECTRLX_DVL_MASK                       0x3000UL                                          /**< Bit mask for ADC_DVL */
#define _ADC_SINGLECTRLX_DVL_DEFAULT                    0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_DVL_DEFAULT                     (_ADC_SINGLECTRLX_DVL_DEFAULT << 12)              /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_FIFOOFACT                       (0x1UL << 14)                                     /**< Single Sample FIFO Overflow Action */
#define _ADC_SINGLECTRLX_FIFOOFACT_SHIFT                14                                                /**< Shift value for ADC_FIFOOFACT */
#define _ADC_SINGLECTRLX_FIFOOFACT_MASK                 0x4000UL                                          /**< Bit mask for ADC_FIFOOFACT */
#define _ADC_SINGLECTRLX_FIFOOFACT_DEFAULT              0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_FIFOOFACT_DISCARD              0x00000000UL                                      /**< Mode DISCARD for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE            0x00000001UL                                      /**< Mode OVERWRITE for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_FIFOOFACT_DEFAULT               (_ADC_SINGLECTRLX_FIFOOFACT_DEFAULT << 14)        /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_FIFOOFACT_DISCARD               (_ADC_SINGLECTRLX_FIFOOFACT_DISCARD << 14)        /**< Shifted mode DISCARD for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE             (_ADC_SINGLECTRLX_FIFOOFACT_OVERWRITE << 14)      /**< Shifted mode OVERWRITE for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSMODE                         (0x1UL << 16)                                     /**< Single Sample PRS Trigger Mode */
#define _ADC_SINGLECTRLX_PRSMODE_SHIFT                  16                                                /**< Shift value for ADC_PRSMODE */
#define _ADC_SINGLECTRLX_PRSMODE_MASK                   0x10000UL                                         /**< Bit mask for ADC_PRSMODE */
#define _ADC_SINGLECTRLX_PRSMODE_DEFAULT                0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSMODE_PULSED                 0x00000000UL                                      /**< Mode PULSED for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSMODE_TIMED                  0x00000001UL                                      /**< Mode TIMED for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSMODE_DEFAULT                 (_ADC_SINGLECTRLX_PRSMODE_DEFAULT << 16)          /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSMODE_PULSED                  (_ADC_SINGLECTRLX_PRSMODE_PULSED << 16)           /**< Shifted mode PULSED for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSMODE_TIMED                   (_ADC_SINGLECTRLX_PRSMODE_TIMED << 16)            /**< Shifted mode TIMED for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_SHIFT                   17                                                /**< Shift value for ADC_PRSSEL */
#define _ADC_SINGLECTRLX_PRSSEL_MASK                    0x1E0000UL                                        /**< Bit mask for ADC_PRSSEL */
#define _ADC_SINGLECTRLX_PRSSEL_DEFAULT                 0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH0                  0x00000000UL                                      /**< Mode PRSCH0 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH1                  0x00000001UL                                      /**< Mode PRSCH1 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH2                  0x00000002UL                                      /**< Mode PRSCH2 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH3                  0x00000003UL                                      /**< Mode PRSCH3 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH4                  0x00000004UL                                      /**< Mode PRSCH4 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH5                  0x00000005UL                                      /**< Mode PRSCH5 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH6                  0x00000006UL                                      /**< Mode PRSCH6 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH7                  0x00000007UL                                      /**< Mode PRSCH7 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH8                  0x00000008UL                                      /**< Mode PRSCH8 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH9                  0x00000009UL                                      /**< Mode PRSCH9 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH10                 0x0000000AUL                                      /**< Mode PRSCH10 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_PRSSEL_PRSCH11                 0x0000000BUL                                      /**< Mode PRSCH11 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_DEFAULT                  (_ADC_SINGLECTRLX_PRSSEL_DEFAULT << 17)           /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH0                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH0 << 17)            /**< Shifted mode PRSCH0 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH1                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH1 << 17)            /**< Shifted mode PRSCH1 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH2                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH2 << 17)            /**< Shifted mode PRSCH2 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH3                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH3 << 17)            /**< Shifted mode PRSCH3 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH4                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH4 << 17)            /**< Shifted mode PRSCH4 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH5                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH5 << 17)            /**< Shifted mode PRSCH5 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH6                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH6 << 17)            /**< Shifted mode PRSCH6 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH7                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH7 << 17)            /**< Shifted mode PRSCH7 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH8                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH8 << 17)            /**< Shifted mode PRSCH8 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH9                   (_ADC_SINGLECTRLX_PRSSEL_PRSCH9 << 17)            /**< Shifted mode PRSCH9 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH10                  (_ADC_SINGLECTRLX_PRSSEL_PRSCH10 << 17)           /**< Shifted mode PRSCH10 for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_PRSSEL_PRSCH11                  (_ADC_SINGLECTRLX_PRSSEL_PRSCH11 << 17)           /**< Shifted mode PRSCH11 for ADC_SINGLECTRLX */
#define _ADC_SINGLECTRLX_CONVSTARTDELAY_SHIFT           24                                                /**< Shift value for ADC_CONVSTARTDELAY */
#define _ADC_SINGLECTRLX_CONVSTARTDELAY_MASK            0x7000000UL                                       /**< Bit mask for ADC_CONVSTARTDELAY */
#define _ADC_SINGLECTRLX_CONVSTARTDELAY_DEFAULT         0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_CONVSTARTDELAY_DEFAULT          (_ADC_SINGLECTRLX_CONVSTARTDELAY_DEFAULT << 24)   /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_CONVSTARTDELAYEN                (0x1UL << 27)                                     /**< Enable delaying next conversion start */
#define _ADC_SINGLECTRLX_CONVSTARTDELAYEN_SHIFT         27                                                /**< Shift value for ADC_CONVSTARTDELAYEN */
#define _ADC_SINGLECTRLX_CONVSTARTDELAYEN_MASK          0x8000000UL                                       /**< Bit mask for ADC_CONVSTARTDELAYEN */
#define _ADC_SINGLECTRLX_CONVSTARTDELAYEN_DEFAULT       0x00000000UL                                      /**< Mode DEFAULT for ADC_SINGLECTRLX */
#define ADC_SINGLECTRLX_CONVSTARTDELAYEN_DEFAULT        (_ADC_SINGLECTRLX_CONVSTARTDELAYEN_DEFAULT << 27) /**< Shifted mode DEFAULT for ADC_SINGLECTRLX */

/* Bit fields for ADC SCANCTRL */
#define _ADC_SCANCTRL_RESETVALUE                        0x00000000UL                        /**< Default value for ADC_SCANCTRL */
#define _ADC_SCANCTRL_MASK                              0xAF0000FFUL                        /**< Mask for ADC_SCANCTRL */
#define ADC_SCANCTRL_REP                                (0x1UL << 0)                        /**< Scan Sequence Repetitive Mode */
#define _ADC_SCANCTRL_REP_SHIFT                         0                                   /**< Shift value for ADC_REP */
#define _ADC_SCANCTRL_REP_MASK                          0x1UL                               /**< Bit mask for ADC_REP */
#define _ADC_SCANCTRL_REP_DEFAULT                       0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_REP_DEFAULT                        (_ADC_SCANCTRL_REP_DEFAULT << 0)    /**< Shifted mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_DIFF                               (0x1UL << 1)                        /**< Scan Sequence Differential Mode */
#define _ADC_SCANCTRL_DIFF_SHIFT                        1                                   /**< Shift value for ADC_DIFF */
#define _ADC_SCANCTRL_DIFF_MASK                         0x2UL                               /**< Bit mask for ADC_DIFF */
#define _ADC_SCANCTRL_DIFF_DEFAULT                      0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_DIFF_DEFAULT                       (_ADC_SCANCTRL_DIFF_DEFAULT << 1)   /**< Shifted mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_ADJ                                (0x1UL << 2)                        /**< Scan Sequence Result Adjustment */
#define _ADC_SCANCTRL_ADJ_SHIFT                         2                                   /**< Shift value for ADC_ADJ */
#define _ADC_SCANCTRL_ADJ_MASK                          0x4UL                               /**< Bit mask for ADC_ADJ */
#define _ADC_SCANCTRL_ADJ_DEFAULT                       0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_ADJ_RIGHT                         0x00000000UL                        /**< Mode RIGHT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_ADJ_LEFT                          0x00000001UL                        /**< Mode LEFT for ADC_SCANCTRL */
#define ADC_SCANCTRL_ADJ_DEFAULT                        (_ADC_SCANCTRL_ADJ_DEFAULT << 2)    /**< Shifted mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_ADJ_RIGHT                          (_ADC_SCANCTRL_ADJ_RIGHT << 2)      /**< Shifted mode RIGHT for ADC_SCANCTRL */
#define ADC_SCANCTRL_ADJ_LEFT                           (_ADC_SCANCTRL_ADJ_LEFT << 2)       /**< Shifted mode LEFT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_RES_SHIFT                         3                                   /**< Shift value for ADC_RES */
#define _ADC_SCANCTRL_RES_MASK                          0x18UL                              /**< Bit mask for ADC_RES */
#define _ADC_SCANCTRL_RES_DEFAULT                       0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_RES_12BIT                         0x00000000UL                        /**< Mode 12BIT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_RES_8BIT                          0x00000001UL                        /**< Mode 8BIT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_RES_6BIT                          0x00000002UL                        /**< Mode 6BIT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_RES_OVS                           0x00000003UL                        /**< Mode OVS for ADC_SCANCTRL */
#define ADC_SCANCTRL_RES_DEFAULT                        (_ADC_SCANCTRL_RES_DEFAULT << 3)    /**< Shifted mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_RES_12BIT                          (_ADC_SCANCTRL_RES_12BIT << 3)      /**< Shifted mode 12BIT for ADC_SCANCTRL */
#define ADC_SCANCTRL_RES_8BIT                           (_ADC_SCANCTRL_RES_8BIT << 3)       /**< Shifted mode 8BIT for ADC_SCANCTRL */
#define ADC_SCANCTRL_RES_6BIT                           (_ADC_SCANCTRL_RES_6BIT << 3)       /**< Shifted mode 6BIT for ADC_SCANCTRL */
#define ADC_SCANCTRL_RES_OVS                            (_ADC_SCANCTRL_RES_OVS << 3)        /**< Shifted mode OVS for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_SHIFT                         5                                   /**< Shift value for ADC_REF */
#define _ADC_SCANCTRL_REF_MASK                          0xE0UL                              /**< Bit mask for ADC_REF */
#define _ADC_SCANCTRL_REF_DEFAULT                       0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_1V25                          0x00000000UL                        /**< Mode 1V25 for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_2V5                           0x00000001UL                        /**< Mode 2V5 for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_VDD                           0x00000002UL                        /**< Mode VDD for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_5VDIFF                        0x00000003UL                        /**< Mode 5VDIFF for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_EXTSINGLE                     0x00000004UL                        /**< Mode EXTSINGLE for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_2XEXTDIFF                     0x00000005UL                        /**< Mode 2XEXTDIFF for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_2XVDD                         0x00000006UL                        /**< Mode 2XVDD for ADC_SCANCTRL */
#define _ADC_SCANCTRL_REF_CONF                          0x00000007UL                        /**< Mode CONF for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_DEFAULT                        (_ADC_SCANCTRL_REF_DEFAULT << 5)    /**< Shifted mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_1V25                           (_ADC_SCANCTRL_REF_1V25 << 5)       /**< Shifted mode 1V25 for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_2V5                            (_ADC_SCANCTRL_REF_2V5 << 5)        /**< Shifted mode 2V5 for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_VDD                            (_ADC_SCANCTRL_REF_VDD << 5)        /**< Shifted mode VDD for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_5VDIFF                         (_ADC_SCANCTRL_REF_5VDIFF << 5)     /**< Shifted mode 5VDIFF for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_EXTSINGLE                      (_ADC_SCANCTRL_REF_EXTSINGLE << 5)  /**< Shifted mode EXTSINGLE for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_2XEXTDIFF                      (_ADC_SCANCTRL_REF_2XEXTDIFF << 5)  /**< Shifted mode 2XEXTDIFF for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_2XVDD                          (_ADC_SCANCTRL_REF_2XVDD << 5)      /**< Shifted mode 2XVDD for ADC_SCANCTRL */
#define ADC_SCANCTRL_REF_CONF                           (_ADC_SCANCTRL_REF_CONF << 5)       /**< Shifted mode CONF for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_SHIFT                          24                                  /**< Shift value for ADC_AT */
#define _ADC_SCANCTRL_AT_MASK                           0xF000000UL                         /**< Bit mask for ADC_AT */
#define _ADC_SCANCTRL_AT_DEFAULT                        0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_1CYCLE                         0x00000000UL                        /**< Mode 1CYCLE for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_2CYCLES                        0x00000001UL                        /**< Mode 2CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_3CYCLES                        0x00000002UL                        /**< Mode 3CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_4CYCLES                        0x00000003UL                        /**< Mode 4CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_8CYCLES                        0x00000004UL                        /**< Mode 8CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_16CYCLES                       0x00000005UL                        /**< Mode 16CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_32CYCLES                       0x00000006UL                        /**< Mode 32CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_64CYCLES                       0x00000007UL                        /**< Mode 64CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_128CYCLES                      0x00000008UL                        /**< Mode 128CYCLES for ADC_SCANCTRL */
#define _ADC_SCANCTRL_AT_256CYCLES                      0x00000009UL                        /**< Mode 256CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_DEFAULT                         (_ADC_SCANCTRL_AT_DEFAULT << 24)    /**< Shifted mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_1CYCLE                          (_ADC_SCANCTRL_AT_1CYCLE << 24)     /**< Shifted mode 1CYCLE for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_2CYCLES                         (_ADC_SCANCTRL_AT_2CYCLES << 24)    /**< Shifted mode 2CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_3CYCLES                         (_ADC_SCANCTRL_AT_3CYCLES << 24)    /**< Shifted mode 3CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_4CYCLES                         (_ADC_SCANCTRL_AT_4CYCLES << 24)    /**< Shifted mode 4CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_8CYCLES                         (_ADC_SCANCTRL_AT_8CYCLES << 24)    /**< Shifted mode 8CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_16CYCLES                        (_ADC_SCANCTRL_AT_16CYCLES << 24)   /**< Shifted mode 16CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_32CYCLES                        (_ADC_SCANCTRL_AT_32CYCLES << 24)   /**< Shifted mode 32CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_64CYCLES                        (_ADC_SCANCTRL_AT_64CYCLES << 24)   /**< Shifted mode 64CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_128CYCLES                       (_ADC_SCANCTRL_AT_128CYCLES << 24)  /**< Shifted mode 128CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_AT_256CYCLES                       (_ADC_SCANCTRL_AT_256CYCLES << 24)  /**< Shifted mode 256CYCLES for ADC_SCANCTRL */
#define ADC_SCANCTRL_PRSEN                              (0x1UL << 29)                       /**< Scan Sequence PRS Trigger Enable */
#define _ADC_SCANCTRL_PRSEN_SHIFT                       29                                  /**< Shift value for ADC_PRSEN */
#define _ADC_SCANCTRL_PRSEN_MASK                        0x20000000UL                        /**< Bit mask for ADC_PRSEN */
#define _ADC_SCANCTRL_PRSEN_DEFAULT                     0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_PRSEN_DEFAULT                      (_ADC_SCANCTRL_PRSEN_DEFAULT << 29) /**< Shifted mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_CMPEN                              (0x1UL << 31)                       /**< Compare Logic Enable for Scan Sample */
#define _ADC_SCANCTRL_CMPEN_SHIFT                       31                                  /**< Shift value for ADC_CMPEN */
#define _ADC_SCANCTRL_CMPEN_MASK                        0x80000000UL                        /**< Bit mask for ADC_CMPEN */
#define _ADC_SCANCTRL_CMPEN_DEFAULT                     0x00000000UL                        /**< Mode DEFAULT for ADC_SCANCTRL */
#define ADC_SCANCTRL_CMPEN_DEFAULT                      (_ADC_SCANCTRL_CMPEN_DEFAULT << 31) /**< Shifted mode DEFAULT for ADC_SCANCTRL */

/* Bit fields for ADC SCANCTRLX */
#define _ADC_SCANCTRLX_RESETVALUE                       0x00000000UL                                    /**< Default value for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_MASK                             0x0F1F7FFFUL                                    /**< Mask for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFSEL_SHIFT                    0                                               /**< Shift value for ADC_VREFSEL */
#define _ADC_SCANCTRLX_VREFSEL_MASK                     0x7UL                                           /**< Bit mask for ADC_VREFSEL */
#define _ADC_SCANCTRLX_VREFSEL_DEFAULT                  0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFSEL_VBGR                     0x00000000UL                                    /**< Mode VBGR for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFSEL_VDDXWATT                 0x00000001UL                                    /**< Mode VDDXWATT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFSEL_VREFPWATT                0x00000002UL                                    /**< Mode VREFPWATT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFSEL_VREFP                    0x00000003UL                                    /**< Mode VREFP for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFSEL_VREFPNWATT               0x00000005UL                                    /**< Mode VREFPNWATT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFSEL_VREFPN                   0x00000006UL                                    /**< Mode VREFPN for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFSEL_DEFAULT                   (_ADC_SCANCTRLX_VREFSEL_DEFAULT << 0)           /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFSEL_VBGR                      (_ADC_SCANCTRLX_VREFSEL_VBGR << 0)              /**< Shifted mode VBGR for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFSEL_VDDXWATT                  (_ADC_SCANCTRLX_VREFSEL_VDDXWATT << 0)          /**< Shifted mode VDDXWATT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFSEL_VREFPWATT                 (_ADC_SCANCTRLX_VREFSEL_VREFPWATT << 0)         /**< Shifted mode VREFPWATT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFSEL_VREFP                     (_ADC_SCANCTRLX_VREFSEL_VREFP << 0)             /**< Shifted mode VREFP for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFSEL_VREFPNWATT                (_ADC_SCANCTRLX_VREFSEL_VREFPNWATT << 0)        /**< Shifted mode VREFPNWATT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFSEL_VREFPN                    (_ADC_SCANCTRLX_VREFSEL_VREFPN << 0)            /**< Shifted mode VREFPN for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFATTFIX                        (0x1UL << 3)                                    /**< Enable fixed 1/3 scaling on VREF */
#define _ADC_SCANCTRLX_VREFATTFIX_SHIFT                 3                                               /**< Shift value for ADC_VREFATTFIX */
#define _ADC_SCANCTRLX_VREFATTFIX_MASK                  0x8UL                                           /**< Bit mask for ADC_VREFATTFIX */
#define _ADC_SCANCTRLX_VREFATTFIX_DEFAULT               0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFATTFIX_DEFAULT                (_ADC_SCANCTRLX_VREFATTFIX_DEFAULT << 3)        /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VREFATT_SHIFT                    4                                               /**< Shift value for ADC_VREFATT */
#define _ADC_SCANCTRLX_VREFATT_MASK                     0xF0UL                                          /**< Bit mask for ADC_VREFATT */
#define _ADC_SCANCTRLX_VREFATT_DEFAULT                  0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VREFATT_DEFAULT                   (_ADC_SCANCTRLX_VREFATT_DEFAULT << 4)           /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_VINATT_SHIFT                     8                                               /**< Shift value for ADC_VINATT */
#define _ADC_SCANCTRLX_VINATT_MASK                      0xF00UL                                         /**< Bit mask for ADC_VINATT */
#define _ADC_SCANCTRLX_VINATT_DEFAULT                   0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_VINATT_DEFAULT                    (_ADC_SCANCTRLX_VINATT_DEFAULT << 8)            /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_DVL_SHIFT                        12                                              /**< Shift value for ADC_DVL */
#define _ADC_SCANCTRLX_DVL_MASK                         0x3000UL                                        /**< Bit mask for ADC_DVL */
#define _ADC_SCANCTRLX_DVL_DEFAULT                      0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_DVL_DEFAULT                       (_ADC_SCANCTRLX_DVL_DEFAULT << 12)              /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_FIFOOFACT                         (0x1UL << 14)                                   /**< Scan Sample FIFO Overflow Action */
#define _ADC_SCANCTRLX_FIFOOFACT_SHIFT                  14                                              /**< Shift value for ADC_FIFOOFACT */
#define _ADC_SCANCTRLX_FIFOOFACT_MASK                   0x4000UL                                        /**< Bit mask for ADC_FIFOOFACT */
#define _ADC_SCANCTRLX_FIFOOFACT_DEFAULT                0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_FIFOOFACT_DISCARD                0x00000000UL                                    /**< Mode DISCARD for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_FIFOOFACT_OVERWRITE              0x00000001UL                                    /**< Mode OVERWRITE for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_FIFOOFACT_DEFAULT                 (_ADC_SCANCTRLX_FIFOOFACT_DEFAULT << 14)        /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_FIFOOFACT_DISCARD                 (_ADC_SCANCTRLX_FIFOOFACT_DISCARD << 14)        /**< Shifted mode DISCARD for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_FIFOOFACT_OVERWRITE               (_ADC_SCANCTRLX_FIFOOFACT_OVERWRITE << 14)      /**< Shifted mode OVERWRITE for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSMODE                           (0x1UL << 16)                                   /**< Scan Sample PRS Trigger Mode */
#define _ADC_SCANCTRLX_PRSMODE_SHIFT                    16                                              /**< Shift value for ADC_PRSMODE */
#define _ADC_SCANCTRLX_PRSMODE_MASK                     0x10000UL                                       /**< Bit mask for ADC_PRSMODE */
#define _ADC_SCANCTRLX_PRSMODE_DEFAULT                  0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSMODE_PULSED                   0x00000000UL                                    /**< Mode PULSED for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSMODE_TIMED                    0x00000001UL                                    /**< Mode TIMED for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSMODE_DEFAULT                   (_ADC_SCANCTRLX_PRSMODE_DEFAULT << 16)          /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSMODE_PULSED                    (_ADC_SCANCTRLX_PRSMODE_PULSED << 16)           /**< Shifted mode PULSED for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSMODE_TIMED                     (_ADC_SCANCTRLX_PRSMODE_TIMED << 16)            /**< Shifted mode TIMED for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_SHIFT                     17                                              /**< Shift value for ADC_PRSSEL */
#define _ADC_SCANCTRLX_PRSSEL_MASK                      0x1E0000UL                                      /**< Bit mask for ADC_PRSSEL */
#define _ADC_SCANCTRLX_PRSSEL_DEFAULT                   0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH0                    0x00000000UL                                    /**< Mode PRSCH0 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH1                    0x00000001UL                                    /**< Mode PRSCH1 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH2                    0x00000002UL                                    /**< Mode PRSCH2 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH3                    0x00000003UL                                    /**< Mode PRSCH3 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH4                    0x00000004UL                                    /**< Mode PRSCH4 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH5                    0x00000005UL                                    /**< Mode PRSCH5 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH6                    0x00000006UL                                    /**< Mode PRSCH6 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH7                    0x00000007UL                                    /**< Mode PRSCH7 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH8                    0x00000008UL                                    /**< Mode PRSCH8 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH9                    0x00000009UL                                    /**< Mode PRSCH9 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH10                   0x0000000AUL                                    /**< Mode PRSCH10 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_PRSSEL_PRSCH11                   0x0000000BUL                                    /**< Mode PRSCH11 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_DEFAULT                    (_ADC_SCANCTRLX_PRSSEL_DEFAULT << 17)           /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH0                     (_ADC_SCANCTRLX_PRSSEL_PRSCH0 << 17)            /**< Shifted mode PRSCH0 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH1                     (_ADC_SCANCTRLX_PRSSEL_PRSCH1 << 17)            /**< Shifted mode PRSCH1 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH2                     (_ADC_SCANCTRLX_PRSSEL_PRSCH2 << 17)            /**< Shifted mode PRSCH2 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH3                     (_ADC_SCANCTRLX_PRSSEL_PRSCH3 << 17)            /**< Shifted mode PRSCH3 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH4                     (_ADC_SCANCTRLX_PRSSEL_PRSCH4 << 17)            /**< Shifted mode PRSCH4 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH5                     (_ADC_SCANCTRLX_PRSSEL_PRSCH5 << 17)            /**< Shifted mode PRSCH5 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH6                     (_ADC_SCANCTRLX_PRSSEL_PRSCH6 << 17)            /**< Shifted mode PRSCH6 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH7                     (_ADC_SCANCTRLX_PRSSEL_PRSCH7 << 17)            /**< Shifted mode PRSCH7 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH8                     (_ADC_SCANCTRLX_PRSSEL_PRSCH8 << 17)            /**< Shifted mode PRSCH8 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH9                     (_ADC_SCANCTRLX_PRSSEL_PRSCH9 << 17)            /**< Shifted mode PRSCH9 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH10                    (_ADC_SCANCTRLX_PRSSEL_PRSCH10 << 17)           /**< Shifted mode PRSCH10 for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_PRSSEL_PRSCH11                    (_ADC_SCANCTRLX_PRSSEL_PRSCH11 << 17)           /**< Shifted mode PRSCH11 for ADC_SCANCTRLX */
#define _ADC_SCANCTRLX_REPDELAY_SHIFT                   24                                              /**< Shift value for ADC_REPDELAY */
#define _ADC_SCANCTRLX_REPDELAY_MASK                    0x7000000UL                                     /**< Bit mask for ADC_REPDELAY */
#define _ADC_SCANCTRLX_REPDELAY_DEFAULT                 0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_REPDELAY_DEFAULT                  (_ADC_SCANCTRLX_REPDELAY_DEFAULT << 24)         /**< Shifted mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_CONVSTARTDELAYEN                  (0x1UL << 27)                                   /**< Enable delaying next conversion start */
#define _ADC_SCANCTRLX_CONVSTARTDELAYEN_SHIFT           27                                              /**< Shift value for ADC_CONVSTARTDELAYEN */
#define _ADC_SCANCTRLX_CONVSTARTDELAYEN_MASK            0x8000000UL                                     /**< Bit mask for ADC_CONVSTARTDELAYEN */
#define _ADC_SCANCTRLX_CONVSTARTDELAYEN_DEFAULT         0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANCTRLX */
#define ADC_SCANCTRLX_CONVSTARTDELAYEN_DEFAULT          (_ADC_SCANCTRLX_CONVSTARTDELAYEN_DEFAULT << 27) /**< Shifted mode DEFAULT for ADC_SCANCTRLX */

/* Bit fields for ADC SCANMASK */
#define _ADC_SCANMASK_RESETVALUE                        0x00000000UL                               /**< Default value for ADC_SCANMASK */
#define _ADC_SCANMASK_MASK                              0xFFFFFFFFUL                               /**< Mask for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_SHIFT                    0                                          /**< Shift value for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_MASK                     0xFFFFFFFFUL                               /**< Bit mask for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_DEFAULT                  0x00000000UL                               /**< Mode DEFAULT for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH0CH0NSEL               0x00000001UL                               /**< Mode CH0CH0NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH0                      0x00000001UL                               /**< Mode CH0 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH1CH2                   0x00000002UL                               /**< Mode CH1CH2 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH1                      0x00000002UL                               /**< Mode CH1 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH2                      0x00000004UL                               /**< Mode CH2 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH2CH2NSEL               0x00000004UL                               /**< Mode CH2CH2NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH3                      0x00000008UL                               /**< Mode CH3 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH3CH4                   0x00000008UL                               /**< Mode CH3CH4 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH4                      0x00000010UL                               /**< Mode CH4 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH4CH4NSEL               0x00000010UL                               /**< Mode CH4CH4NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH5CH6                   0x00000020UL                               /**< Mode CH5CH6 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH5                      0x00000020UL                               /**< Mode CH5 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH6                      0x00000040UL                               /**< Mode CH6 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH6CH6NSEL               0x00000040UL                               /**< Mode CH6CH6NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH7                      0x00000080UL                               /**< Mode CH7 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH7CH0                   0x00000080UL                               /**< Mode CH7CH0 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH8                      0x00000100UL                               /**< Mode CH8 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH8CH9                   0x00000100UL                               /**< Mode CH8CH9 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH9                      0x00000200UL                               /**< Mode CH9 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH9CH9NSEL               0x00000200UL                               /**< Mode CH9CH9NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH10                     0x00000400UL                               /**< Mode CH10 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH10CH11                 0x00000400UL                               /**< Mode CH10CH11 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH11                     0x00000800UL                               /**< Mode CH11 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH11CH11NSEL             0x00000800UL                               /**< Mode CH11CH11NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH12CH13                 0x00001000UL                               /**< Mode CH12CH13 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH12                     0x00001000UL                               /**< Mode CH12 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH13                     0x00002000UL                               /**< Mode CH13 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH13CH13NSEL             0x00002000UL                               /**< Mode CH13CH13NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH14CH15                 0x00004000UL                               /**< Mode CH14CH15 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH14                     0x00004000UL                               /**< Mode CH14 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH15                     0x00008000UL                               /**< Mode CH15 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH15CH15NSEL             0x00008000UL                               /**< Mode CH15CH15NSEL for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH16                     0x00010000UL                               /**< Mode CH16 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH16CH17                 0x00010000UL                               /**< Mode CH16CH17 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH17CH18                 0x00020000UL                               /**< Mode CH17CH18 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH17                     0x00020000UL                               /**< Mode CH17 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH18                     0x00040000UL                               /**< Mode CH18 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH18CH19                 0x00040000UL                               /**< Mode CH18CH19 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH19CH20                 0x00080000UL                               /**< Mode CH19CH20 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH19                     0x00080000UL                               /**< Mode CH19 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH20                     0x00100000UL                               /**< Mode CH20 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH20CH21                 0x00100000UL                               /**< Mode CH20CH21 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH21                     0x00200000UL                               /**< Mode CH21 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH21CH22                 0x00200000UL                               /**< Mode CH21CH22 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH22                     0x00400000UL                               /**< Mode CH22 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH22CH23                 0x00400000UL                               /**< Mode CH22CH23 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH23                     0x00800000UL                               /**< Mode CH23 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH23CH16                 0x00800000UL                               /**< Mode CH23CH16 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH24                     0x01000000UL                               /**< Mode CH24 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH24CH25                 0x01000000UL                               /**< Mode CH24CH25 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH25                     0x02000000UL                               /**< Mode CH25 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH25CH26                 0x02000000UL                               /**< Mode CH25CH26 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH26                     0x04000000UL                               /**< Mode CH26 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH26CH27                 0x04000000UL                               /**< Mode CH26CH27 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH27                     0x08000000UL                               /**< Mode CH27 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH27CH28                 0x08000000UL                               /**< Mode CH27CH28 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH28                     0x10000000UL                               /**< Mode CH28 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH28CH29                 0x10000000UL                               /**< Mode CH28CH29 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH29CH30                 0x20000000UL                               /**< Mode CH29CH30 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH29                     0x20000000UL                               /**< Mode CH29 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH30                     0x40000000UL                               /**< Mode CH30 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH30CH31                 0x40000000UL                               /**< Mode CH30CH31 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH31CH24                 0x80000000UL                               /**< Mode CH31CH24 for ADC_SCANMASK */
#define _ADC_SCANMASK_SCANMASK_CH31                     0x80000000UL                               /**< Mode CH31 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_DEFAULT                   (_ADC_SCANMASK_SCANMASK_DEFAULT << 0)      /**< Shifted mode DEFAULT for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH0CH0NSEL                (_ADC_SCANMASK_SCANMASK_CH0CH0NSEL << 0)   /**< Shifted mode CH0CH0NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH0                       (_ADC_SCANMASK_SCANMASK_CH0 << 0)          /**< Shifted mode CH0 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH1CH2                    (_ADC_SCANMASK_SCANMASK_CH1CH2 << 0)       /**< Shifted mode CH1CH2 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH1                       (_ADC_SCANMASK_SCANMASK_CH1 << 0)          /**< Shifted mode CH1 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH2                       (_ADC_SCANMASK_SCANMASK_CH2 << 0)          /**< Shifted mode CH2 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH2CH2NSEL                (_ADC_SCANMASK_SCANMASK_CH2CH2NSEL << 0)   /**< Shifted mode CH2CH2NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH3                       (_ADC_SCANMASK_SCANMASK_CH3 << 0)          /**< Shifted mode CH3 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH3CH4                    (_ADC_SCANMASK_SCANMASK_CH3CH4 << 0)       /**< Shifted mode CH3CH4 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH4                       (_ADC_SCANMASK_SCANMASK_CH4 << 0)          /**< Shifted mode CH4 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH4CH4NSEL                (_ADC_SCANMASK_SCANMASK_CH4CH4NSEL << 0)   /**< Shifted mode CH4CH4NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH5CH6                    (_ADC_SCANMASK_SCANMASK_CH5CH6 << 0)       /**< Shifted mode CH5CH6 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH5                       (_ADC_SCANMASK_SCANMASK_CH5 << 0)          /**< Shifted mode CH5 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH6                       (_ADC_SCANMASK_SCANMASK_CH6 << 0)          /**< Shifted mode CH6 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH6CH6NSEL                (_ADC_SCANMASK_SCANMASK_CH6CH6NSEL << 0)   /**< Shifted mode CH6CH6NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH7                       (_ADC_SCANMASK_SCANMASK_CH7 << 0)          /**< Shifted mode CH7 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH7CH0                    (_ADC_SCANMASK_SCANMASK_CH7CH0 << 0)       /**< Shifted mode CH7CH0 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH8                       (_ADC_SCANMASK_SCANMASK_CH8 << 0)          /**< Shifted mode CH8 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH8CH9                    (_ADC_SCANMASK_SCANMASK_CH8CH9 << 0)       /**< Shifted mode CH8CH9 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH9                       (_ADC_SCANMASK_SCANMASK_CH9 << 0)          /**< Shifted mode CH9 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH9CH9NSEL                (_ADC_SCANMASK_SCANMASK_CH9CH9NSEL << 0)   /**< Shifted mode CH9CH9NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH10                      (_ADC_SCANMASK_SCANMASK_CH10 << 0)         /**< Shifted mode CH10 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH10CH11                  (_ADC_SCANMASK_SCANMASK_CH10CH11 << 0)     /**< Shifted mode CH10CH11 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH11                      (_ADC_SCANMASK_SCANMASK_CH11 << 0)         /**< Shifted mode CH11 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH11CH11NSEL              (_ADC_SCANMASK_SCANMASK_CH11CH11NSEL << 0) /**< Shifted mode CH11CH11NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH12CH13                  (_ADC_SCANMASK_SCANMASK_CH12CH13 << 0)     /**< Shifted mode CH12CH13 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH12                      (_ADC_SCANMASK_SCANMASK_CH12 << 0)         /**< Shifted mode CH12 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH13                      (_ADC_SCANMASK_SCANMASK_CH13 << 0)         /**< Shifted mode CH13 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH13CH13NSEL              (_ADC_SCANMASK_SCANMASK_CH13CH13NSEL << 0) /**< Shifted mode CH13CH13NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH14CH15                  (_ADC_SCANMASK_SCANMASK_CH14CH15 << 0)     /**< Shifted mode CH14CH15 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH14                      (_ADC_SCANMASK_SCANMASK_CH14 << 0)         /**< Shifted mode CH14 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH15                      (_ADC_SCANMASK_SCANMASK_CH15 << 0)         /**< Shifted mode CH15 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH15CH15NSEL              (_ADC_SCANMASK_SCANMASK_CH15CH15NSEL << 0) /**< Shifted mode CH15CH15NSEL for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH16                      (_ADC_SCANMASK_SCANMASK_CH16 << 0)         /**< Shifted mode CH16 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH16CH17                  (_ADC_SCANMASK_SCANMASK_CH16CH17 << 0)     /**< Shifted mode CH16CH17 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH17CH18                  (_ADC_SCANMASK_SCANMASK_CH17CH18 << 0)     /**< Shifted mode CH17CH18 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH17                      (_ADC_SCANMASK_SCANMASK_CH17 << 0)         /**< Shifted mode CH17 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH18                      (_ADC_SCANMASK_SCANMASK_CH18 << 0)         /**< Shifted mode CH18 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH18CH19                  (_ADC_SCANMASK_SCANMASK_CH18CH19 << 0)     /**< Shifted mode CH18CH19 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH19CH20                  (_ADC_SCANMASK_SCANMASK_CH19CH20 << 0)     /**< Shifted mode CH19CH20 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH19                      (_ADC_SCANMASK_SCANMASK_CH19 << 0)         /**< Shifted mode CH19 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH20                      (_ADC_SCANMASK_SCANMASK_CH20 << 0)         /**< Shifted mode CH20 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH20CH21                  (_ADC_SCANMASK_SCANMASK_CH20CH21 << 0)     /**< Shifted mode CH20CH21 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH21                      (_ADC_SCANMASK_SCANMASK_CH21 << 0)         /**< Shifted mode CH21 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH21CH22                  (_ADC_SCANMASK_SCANMASK_CH21CH22 << 0)     /**< Shifted mode CH21CH22 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH22                      (_ADC_SCANMASK_SCANMASK_CH22 << 0)         /**< Shifted mode CH22 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH22CH23                  (_ADC_SCANMASK_SCANMASK_CH22CH23 << 0)     /**< Shifted mode CH22CH23 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH23                      (_ADC_SCANMASK_SCANMASK_CH23 << 0)         /**< Shifted mode CH23 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH23CH16                  (_ADC_SCANMASK_SCANMASK_CH23CH16 << 0)     /**< Shifted mode CH23CH16 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH24                      (_ADC_SCANMASK_SCANMASK_CH24 << 0)         /**< Shifted mode CH24 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH24CH25                  (_ADC_SCANMASK_SCANMASK_CH24CH25 << 0)     /**< Shifted mode CH24CH25 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH25                      (_ADC_SCANMASK_SCANMASK_CH25 << 0)         /**< Shifted mode CH25 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH25CH26                  (_ADC_SCANMASK_SCANMASK_CH25CH26 << 0)     /**< Shifted mode CH25CH26 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH26                      (_ADC_SCANMASK_SCANMASK_CH26 << 0)         /**< Shifted mode CH26 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH26CH27                  (_ADC_SCANMASK_SCANMASK_CH26CH27 << 0)     /**< Shifted mode CH26CH27 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH27                      (_ADC_SCANMASK_SCANMASK_CH27 << 0)         /**< Shifted mode CH27 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH27CH28                  (_ADC_SCANMASK_SCANMASK_CH27CH28 << 0)     /**< Shifted mode CH27CH28 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH28                      (_ADC_SCANMASK_SCANMASK_CH28 << 0)         /**< Shifted mode CH28 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH28CH29                  (_ADC_SCANMASK_SCANMASK_CH28CH29 << 0)     /**< Shifted mode CH28CH29 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH29CH30                  (_ADC_SCANMASK_SCANMASK_CH29CH30 << 0)     /**< Shifted mode CH29CH30 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH29                      (_ADC_SCANMASK_SCANMASK_CH29 << 0)         /**< Shifted mode CH29 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH30                      (_ADC_SCANMASK_SCANMASK_CH30 << 0)         /**< Shifted mode CH30 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH30CH31                  (_ADC_SCANMASK_SCANMASK_CH30CH31 << 0)     /**< Shifted mode CH30CH31 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH31CH24                  (_ADC_SCANMASK_SCANMASK_CH31CH24 << 0)     /**< Shifted mode CH31CH24 for ADC_SCANMASK */
#define ADC_SCANMASK_SCANMASK_CH31                      (_ADC_SCANMASK_SCANMASK_CH31 << 0)         /**< Shifted mode CH31 for ADC_SCANMASK */

/* Bit fields for ADC SCANCHCONF */
#define _ADC_SCANCHCONF_RESETVALUE                      0x00000000UL                                     /**< Default value for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_MASK                            0x1F1F1F1FUL                                     /**< Mask for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_SHIFT                 0                                                /**< Shift value for ADC_CH0TO7SEL */
#define _ADC_SCANCHCONF_CH0TO7SEL_MASK                  0x1FUL                                           /**< Bit mask for ADC_CH0TO7SEL */
#define _ADC_SCANCHCONF_CH0TO7SEL_DEFAULT               0x00000000UL                                     /**< Mode DEFAULT for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS0CH0TO7            0x00000000UL                                     /**< Mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS0CH8TO15           0x00000001UL                                     /**< Mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS1CH0TO7            0x00000004UL                                     /**< Mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS1CH8TO15           0x00000005UL                                     /**< Mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS1CH16TO23          0x00000006UL                                     /**< Mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS1CH24TO31          0x00000007UL                                     /**< Mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS2CH0TO7            0x00000008UL                                     /**< Mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS2CH8TO15           0x00000009UL                                     /**< Mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS2CH16TO23          0x0000000AUL                                     /**< Mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS2CH24TO31          0x0000000BUL                                     /**< Mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS3CH0TO7            0x0000000CUL                                     /**< Mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS3CH8TO15           0x0000000DUL                                     /**< Mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS3CH16TO23          0x0000000EUL                                     /**< Mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS3CH24TO31          0x0000000FUL                                     /**< Mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS4CH0TO7            0x00000010UL                                     /**< Mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS4CH8TO15           0x00000011UL                                     /**< Mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS4CH16TO23          0x00000012UL                                     /**< Mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH0TO7SEL_BUS4CH24TO31          0x00000013UL                                     /**< Mode BUS4CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_DEFAULT                (_ADC_SCANCHCONF_CH0TO7SEL_DEFAULT << 0)         /**< Shifted mode DEFAULT for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS0CH0TO7             (_ADC_SCANCHCONF_CH0TO7SEL_BUS0CH0TO7 << 0)      /**< Shifted mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS0CH8TO15            (_ADC_SCANCHCONF_CH0TO7SEL_BUS0CH8TO15 << 0)     /**< Shifted mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS1CH0TO7             (_ADC_SCANCHCONF_CH0TO7SEL_BUS1CH0TO7 << 0)      /**< Shifted mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS1CH8TO15            (_ADC_SCANCHCONF_CH0TO7SEL_BUS1CH8TO15 << 0)     /**< Shifted mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS1CH16TO23           (_ADC_SCANCHCONF_CH0TO7SEL_BUS1CH16TO23 << 0)    /**< Shifted mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS1CH24TO31           (_ADC_SCANCHCONF_CH0TO7SEL_BUS1CH24TO31 << 0)    /**< Shifted mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS2CH0TO7             (_ADC_SCANCHCONF_CH0TO7SEL_BUS2CH0TO7 << 0)      /**< Shifted mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS2CH8TO15            (_ADC_SCANCHCONF_CH0TO7SEL_BUS2CH8TO15 << 0)     /**< Shifted mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS2CH16TO23           (_ADC_SCANCHCONF_CH0TO7SEL_BUS2CH16TO23 << 0)    /**< Shifted mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS2CH24TO31           (_ADC_SCANCHCONF_CH0TO7SEL_BUS2CH24TO31 << 0)    /**< Shifted mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS3CH0TO7             (_ADC_SCANCHCONF_CH0TO7SEL_BUS3CH0TO7 << 0)      /**< Shifted mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS3CH8TO15            (_ADC_SCANCHCONF_CH0TO7SEL_BUS3CH8TO15 << 0)     /**< Shifted mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS3CH16TO23           (_ADC_SCANCHCONF_CH0TO7SEL_BUS3CH16TO23 << 0)    /**< Shifted mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS3CH24TO31           (_ADC_SCANCHCONF_CH0TO7SEL_BUS3CH24TO31 << 0)    /**< Shifted mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS4CH0TO7             (_ADC_SCANCHCONF_CH0TO7SEL_BUS4CH0TO7 << 0)      /**< Shifted mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS4CH8TO15            (_ADC_SCANCHCONF_CH0TO7SEL_BUS4CH8TO15 << 0)     /**< Shifted mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS4CH16TO23           (_ADC_SCANCHCONF_CH0TO7SEL_BUS4CH16TO23 << 0)    /**< Shifted mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH0TO7SEL_BUS4CH24TO31           (_ADC_SCANCHCONF_CH0TO7SEL_BUS4CH24TO31 << 0)    /**< Shifted mode BUS4CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_SHIFT                8                                                /**< Shift value for ADC_CH8TO15SEL */
#define _ADC_SCANCHCONF_CH8TO15SEL_MASK                 0x1F00UL                                         /**< Bit mask for ADC_CH8TO15SEL */
#define _ADC_SCANCHCONF_CH8TO15SEL_DEFAULT              0x00000000UL                                     /**< Mode DEFAULT for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS0CH0TO7           0x00000000UL                                     /**< Mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS0CH8TO15          0x00000001UL                                     /**< Mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS1CH0TO7           0x00000004UL                                     /**< Mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS1CH8TO15          0x00000005UL                                     /**< Mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS1CH16TO23         0x00000006UL                                     /**< Mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS1CH24TO31         0x00000007UL                                     /**< Mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS2CH0TO7           0x00000008UL                                     /**< Mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS2CH8TO15          0x00000009UL                                     /**< Mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS2CH16TO23         0x0000000AUL                                     /**< Mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS2CH24TO31         0x0000000BUL                                     /**< Mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS3CH0TO7           0x0000000CUL                                     /**< Mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS3CH8TO15          0x0000000DUL                                     /**< Mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS3CH16TO23         0x0000000EUL                                     /**< Mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS3CH24TO31         0x0000000FUL                                     /**< Mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS4CH0TO7           0x00000010UL                                     /**< Mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS4CH8TO15          0x00000011UL                                     /**< Mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS4CH16TO23         0x00000012UL                                     /**< Mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH8TO15SEL_BUS4CH24TO31         0x00000013UL                                     /**< Mode BUS4CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_DEFAULT               (_ADC_SCANCHCONF_CH8TO15SEL_DEFAULT << 8)        /**< Shifted mode DEFAULT for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS0CH0TO7            (_ADC_SCANCHCONF_CH8TO15SEL_BUS0CH0TO7 << 8)     /**< Shifted mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS0CH8TO15           (_ADC_SCANCHCONF_CH8TO15SEL_BUS0CH8TO15 << 8)    /**< Shifted mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS1CH0TO7            (_ADC_SCANCHCONF_CH8TO15SEL_BUS1CH0TO7 << 8)     /**< Shifted mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS1CH8TO15           (_ADC_SCANCHCONF_CH8TO15SEL_BUS1CH8TO15 << 8)    /**< Shifted mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS1CH16TO23          (_ADC_SCANCHCONF_CH8TO15SEL_BUS1CH16TO23 << 8)   /**< Shifted mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS1CH24TO31          (_ADC_SCANCHCONF_CH8TO15SEL_BUS1CH24TO31 << 8)   /**< Shifted mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS2CH0TO7            (_ADC_SCANCHCONF_CH8TO15SEL_BUS2CH0TO7 << 8)     /**< Shifted mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS2CH8TO15           (_ADC_SCANCHCONF_CH8TO15SEL_BUS2CH8TO15 << 8)    /**< Shifted mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS2CH16TO23          (_ADC_SCANCHCONF_CH8TO15SEL_BUS2CH16TO23 << 8)   /**< Shifted mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS2CH24TO31          (_ADC_SCANCHCONF_CH8TO15SEL_BUS2CH24TO31 << 8)   /**< Shifted mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS3CH0TO7            (_ADC_SCANCHCONF_CH8TO15SEL_BUS3CH0TO7 << 8)     /**< Shifted mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS3CH8TO15           (_ADC_SCANCHCONF_CH8TO15SEL_BUS3CH8TO15 << 8)    /**< Shifted mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS3CH16TO23          (_ADC_SCANCHCONF_CH8TO15SEL_BUS3CH16TO23 << 8)   /**< Shifted mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS3CH24TO31          (_ADC_SCANCHCONF_CH8TO15SEL_BUS3CH24TO31 << 8)   /**< Shifted mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS4CH0TO7            (_ADC_SCANCHCONF_CH8TO15SEL_BUS4CH0TO7 << 8)     /**< Shifted mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS4CH8TO15           (_ADC_SCANCHCONF_CH8TO15SEL_BUS4CH8TO15 << 8)    /**< Shifted mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS4CH16TO23          (_ADC_SCANCHCONF_CH8TO15SEL_BUS4CH16TO23 << 8)   /**< Shifted mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH8TO15SEL_BUS4CH24TO31          (_ADC_SCANCHCONF_CH8TO15SEL_BUS4CH24TO31 << 8)   /**< Shifted mode BUS4CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_SHIFT               16                                               /**< Shift value for ADC_CH16TO23SEL */
#define _ADC_SCANCHCONF_CH16TO23SEL_MASK                0x1F0000UL                                       /**< Bit mask for ADC_CH16TO23SEL */
#define _ADC_SCANCHCONF_CH16TO23SEL_DEFAULT             0x00000000UL                                     /**< Mode DEFAULT for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS0CH0TO7          0x00000000UL                                     /**< Mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS0CH8TO15         0x00000001UL                                     /**< Mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS1CH0TO7          0x00000004UL                                     /**< Mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS1CH8TO15         0x00000005UL                                     /**< Mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS1CH16TO23        0x00000006UL                                     /**< Mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS1CH24TO31        0x00000007UL                                     /**< Mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS2CH0TO7          0x00000008UL                                     /**< Mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS2CH8TO15         0x00000009UL                                     /**< Mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS2CH16TO23        0x0000000AUL                                     /**< Mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS2CH24TO31        0x0000000BUL                                     /**< Mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS3CH0TO7          0x0000000CUL                                     /**< Mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS3CH8TO15         0x0000000DUL                                     /**< Mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS3CH16TO23        0x0000000EUL                                     /**< Mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS3CH24TO31        0x0000000FUL                                     /**< Mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS4CH0TO7          0x00000010UL                                     /**< Mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS4CH8TO15         0x00000011UL                                     /**< Mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS4CH16TO23        0x00000012UL                                     /**< Mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH16TO23SEL_BUS4CH24TO31        0x00000013UL                                     /**< Mode BUS4CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_DEFAULT              (_ADC_SCANCHCONF_CH16TO23SEL_DEFAULT << 16)      /**< Shifted mode DEFAULT for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS0CH0TO7           (_ADC_SCANCHCONF_CH16TO23SEL_BUS0CH0TO7 << 16)   /**< Shifted mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS0CH8TO15          (_ADC_SCANCHCONF_CH16TO23SEL_BUS0CH8TO15 << 16)  /**< Shifted mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS1CH0TO7           (_ADC_SCANCHCONF_CH16TO23SEL_BUS1CH0TO7 << 16)   /**< Shifted mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS1CH8TO15          (_ADC_SCANCHCONF_CH16TO23SEL_BUS1CH8TO15 << 16)  /**< Shifted mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS1CH16TO23         (_ADC_SCANCHCONF_CH16TO23SEL_BUS1CH16TO23 << 16) /**< Shifted mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS1CH24TO31         (_ADC_SCANCHCONF_CH16TO23SEL_BUS1CH24TO31 << 16) /**< Shifted mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS2CH0TO7           (_ADC_SCANCHCONF_CH16TO23SEL_BUS2CH0TO7 << 16)   /**< Shifted mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS2CH8TO15          (_ADC_SCANCHCONF_CH16TO23SEL_BUS2CH8TO15 << 16)  /**< Shifted mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS2CH16TO23         (_ADC_SCANCHCONF_CH16TO23SEL_BUS2CH16TO23 << 16) /**< Shifted mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS2CH24TO31         (_ADC_SCANCHCONF_CH16TO23SEL_BUS2CH24TO31 << 16) /**< Shifted mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS3CH0TO7           (_ADC_SCANCHCONF_CH16TO23SEL_BUS3CH0TO7 << 16)   /**< Shifted mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS3CH8TO15          (_ADC_SCANCHCONF_CH16TO23SEL_BUS3CH8TO15 << 16)  /**< Shifted mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS3CH16TO23         (_ADC_SCANCHCONF_CH16TO23SEL_BUS3CH16TO23 << 16) /**< Shifted mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS3CH24TO31         (_ADC_SCANCHCONF_CH16TO23SEL_BUS3CH24TO31 << 16) /**< Shifted mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS4CH0TO7           (_ADC_SCANCHCONF_CH16TO23SEL_BUS4CH0TO7 << 16)   /**< Shifted mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS4CH8TO15          (_ADC_SCANCHCONF_CH16TO23SEL_BUS4CH8TO15 << 16)  /**< Shifted mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS4CH16TO23         (_ADC_SCANCHCONF_CH16TO23SEL_BUS4CH16TO23 << 16) /**< Shifted mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH16TO23SEL_BUS4CH24TO31         (_ADC_SCANCHCONF_CH16TO23SEL_BUS4CH24TO31 << 16) /**< Shifted mode BUS4CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_SHIFT               24                                               /**< Shift value for ADC_CH24TO31SEL */
#define _ADC_SCANCHCONF_CH24TO31SEL_MASK                0x1F000000UL                                     /**< Bit mask for ADC_CH24TO31SEL */
#define _ADC_SCANCHCONF_CH24TO31SEL_DEFAULT             0x00000000UL                                     /**< Mode DEFAULT for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS0CH0TO7          0x00000000UL                                     /**< Mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS0CH8TO15         0x00000001UL                                     /**< Mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS1CH0TO7          0x00000004UL                                     /**< Mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS1CH8TO15         0x00000005UL                                     /**< Mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS1CH16TO23        0x00000006UL                                     /**< Mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS1CH24TO31        0x00000007UL                                     /**< Mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS2CH0TO7          0x00000008UL                                     /**< Mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS2CH8TO15         0x00000009UL                                     /**< Mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS2CH16TO23        0x0000000AUL                                     /**< Mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS2CH24TO31        0x0000000BUL                                     /**< Mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS3CH0TO7          0x0000000CUL                                     /**< Mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS3CH8TO15         0x0000000DUL                                     /**< Mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS3CH16TO23        0x0000000EUL                                     /**< Mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS3CH24TO31        0x0000000FUL                                     /**< Mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS4CH0TO7          0x00000010UL                                     /**< Mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS4CH8TO15         0x00000011UL                                     /**< Mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS4CH16TO23        0x00000012UL                                     /**< Mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define _ADC_SCANCHCONF_CH24TO31SEL_BUS4CH24TO31        0x00000013UL                                     /**< Mode BUS4CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_DEFAULT              (_ADC_SCANCHCONF_CH24TO31SEL_DEFAULT << 24)      /**< Shifted mode DEFAULT for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS0CH0TO7           (_ADC_SCANCHCONF_CH24TO31SEL_BUS0CH0TO7 << 24)   /**< Shifted mode BUS0CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS0CH8TO15          (_ADC_SCANCHCONF_CH24TO31SEL_BUS0CH8TO15 << 24)  /**< Shifted mode BUS0CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS1CH0TO7           (_ADC_SCANCHCONF_CH24TO31SEL_BUS1CH0TO7 << 24)   /**< Shifted mode BUS1CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS1CH8TO15          (_ADC_SCANCHCONF_CH24TO31SEL_BUS1CH8TO15 << 24)  /**< Shifted mode BUS1CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS1CH16TO23         (_ADC_SCANCHCONF_CH24TO31SEL_BUS1CH16TO23 << 24) /**< Shifted mode BUS1CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS1CH24TO31         (_ADC_SCANCHCONF_CH24TO31SEL_BUS1CH24TO31 << 24) /**< Shifted mode BUS1CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS2CH0TO7           (_ADC_SCANCHCONF_CH24TO31SEL_BUS2CH0TO7 << 24)   /**< Shifted mode BUS2CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS2CH8TO15          (_ADC_SCANCHCONF_CH24TO31SEL_BUS2CH8TO15 << 24)  /**< Shifted mode BUS2CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS2CH16TO23         (_ADC_SCANCHCONF_CH24TO31SEL_BUS2CH16TO23 << 24) /**< Shifted mode BUS2CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS2CH24TO31         (_ADC_SCANCHCONF_CH24TO31SEL_BUS2CH24TO31 << 24) /**< Shifted mode BUS2CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS3CH0TO7           (_ADC_SCANCHCONF_CH24TO31SEL_BUS3CH0TO7 << 24)   /**< Shifted mode BUS3CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS3CH8TO15          (_ADC_SCANCHCONF_CH24TO31SEL_BUS3CH8TO15 << 24)  /**< Shifted mode BUS3CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS3CH16TO23         (_ADC_SCANCHCONF_CH24TO31SEL_BUS3CH16TO23 << 24) /**< Shifted mode BUS3CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS3CH24TO31         (_ADC_SCANCHCONF_CH24TO31SEL_BUS3CH24TO31 << 24) /**< Shifted mode BUS3CH24TO31 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS4CH0TO7           (_ADC_SCANCHCONF_CH24TO31SEL_BUS4CH0TO7 << 24)   /**< Shifted mode BUS4CH0TO7 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS4CH8TO15          (_ADC_SCANCHCONF_CH24TO31SEL_BUS4CH8TO15 << 24)  /**< Shifted mode BUS4CH8TO15 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS4CH16TO23         (_ADC_SCANCHCONF_CH24TO31SEL_BUS4CH16TO23 << 24) /**< Shifted mode BUS4CH16TO23 for ADC_SCANCHCONF */
#define ADC_SCANCHCONF_CH24TO31SEL_BUS4CH24TO31         (_ADC_SCANCHCONF_CH24TO31SEL_BUS4CH24TO31 << 24) /**< Shifted mode BUS4CH24TO31 for ADC_SCANCHCONF */

/* Bit fields for ADC SCANNSEL */
#define _ADC_SCANNSEL_RESETVALUE                        0x000039E4UL                           /**< Default value for ADC_SCANNSEL */
#define _ADC_SCANNSEL_MASK                              0x0000FFFFUL                           /**< Mask for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH0NSEL_SHIFT                     0                                      /**< Shift value for ADC_CH0NSEL */
#define _ADC_SCANNSEL_CH0NSEL_MASK                      0x3UL                                  /**< Bit mask for ADC_CH0NSEL */
#define _ADC_SCANNSEL_CH0NSEL_DEFAULT                   0x00000000UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH0NSEL_CH1                       0x00000000UL                           /**< Mode CH1 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH0NSEL_CH3                       0x00000001UL                           /**< Mode CH3 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH0NSEL_CH5                       0x00000002UL                           /**< Mode CH5 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH0NSEL_CH7                       0x00000003UL                           /**< Mode CH7 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH0NSEL_DEFAULT                    (_ADC_SCANNSEL_CH0NSEL_DEFAULT << 0)   /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH0NSEL_CH1                        (_ADC_SCANNSEL_CH0NSEL_CH1 << 0)       /**< Shifted mode CH1 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH0NSEL_CH3                        (_ADC_SCANNSEL_CH0NSEL_CH3 << 0)       /**< Shifted mode CH3 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH0NSEL_CH5                        (_ADC_SCANNSEL_CH0NSEL_CH5 << 0)       /**< Shifted mode CH5 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH0NSEL_CH7                        (_ADC_SCANNSEL_CH0NSEL_CH7 << 0)       /**< Shifted mode CH7 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH2NSEL_SHIFT                     2                                      /**< Shift value for ADC_CH2NSEL */
#define _ADC_SCANNSEL_CH2NSEL_MASK                      0xCUL                                  /**< Bit mask for ADC_CH2NSEL */
#define _ADC_SCANNSEL_CH2NSEL_CH1                       0x00000000UL                           /**< Mode CH1 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH2NSEL_DEFAULT                   0x00000001UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH2NSEL_CH3                       0x00000001UL                           /**< Mode CH3 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH2NSEL_CH5                       0x00000002UL                           /**< Mode CH5 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH2NSEL_CH7                       0x00000003UL                           /**< Mode CH7 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH2NSEL_CH1                        (_ADC_SCANNSEL_CH2NSEL_CH1 << 2)       /**< Shifted mode CH1 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH2NSEL_DEFAULT                    (_ADC_SCANNSEL_CH2NSEL_DEFAULT << 2)   /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH2NSEL_CH3                        (_ADC_SCANNSEL_CH2NSEL_CH3 << 2)       /**< Shifted mode CH3 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH2NSEL_CH5                        (_ADC_SCANNSEL_CH2NSEL_CH5 << 2)       /**< Shifted mode CH5 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH2NSEL_CH7                        (_ADC_SCANNSEL_CH2NSEL_CH7 << 2)       /**< Shifted mode CH7 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH4NSEL_SHIFT                     4                                      /**< Shift value for ADC_CH4NSEL */
#define _ADC_SCANNSEL_CH4NSEL_MASK                      0x30UL                                 /**< Bit mask for ADC_CH4NSEL */
#define _ADC_SCANNSEL_CH4NSEL_CH1                       0x00000000UL                           /**< Mode CH1 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH4NSEL_CH3                       0x00000001UL                           /**< Mode CH3 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH4NSEL_DEFAULT                   0x00000002UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH4NSEL_CH5                       0x00000002UL                           /**< Mode CH5 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH4NSEL_CH7                       0x00000003UL                           /**< Mode CH7 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH4NSEL_CH1                        (_ADC_SCANNSEL_CH4NSEL_CH1 << 4)       /**< Shifted mode CH1 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH4NSEL_CH3                        (_ADC_SCANNSEL_CH4NSEL_CH3 << 4)       /**< Shifted mode CH3 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH4NSEL_DEFAULT                    (_ADC_SCANNSEL_CH4NSEL_DEFAULT << 4)   /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH4NSEL_CH5                        (_ADC_SCANNSEL_CH4NSEL_CH5 << 4)       /**< Shifted mode CH5 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH4NSEL_CH7                        (_ADC_SCANNSEL_CH4NSEL_CH7 << 4)       /**< Shifted mode CH7 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH6NSEL_SHIFT                     6                                      /**< Shift value for ADC_CH6NSEL */
#define _ADC_SCANNSEL_CH6NSEL_MASK                      0xC0UL                                 /**< Bit mask for ADC_CH6NSEL */
#define _ADC_SCANNSEL_CH6NSEL_CH1                       0x00000000UL                           /**< Mode CH1 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH6NSEL_CH3                       0x00000001UL                           /**< Mode CH3 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH6NSEL_CH5                       0x00000002UL                           /**< Mode CH5 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH6NSEL_DEFAULT                   0x00000003UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH6NSEL_CH7                       0x00000003UL                           /**< Mode CH7 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH6NSEL_CH1                        (_ADC_SCANNSEL_CH6NSEL_CH1 << 6)       /**< Shifted mode CH1 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH6NSEL_CH3                        (_ADC_SCANNSEL_CH6NSEL_CH3 << 6)       /**< Shifted mode CH3 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH6NSEL_CH5                        (_ADC_SCANNSEL_CH6NSEL_CH5 << 6)       /**< Shifted mode CH5 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH6NSEL_DEFAULT                    (_ADC_SCANNSEL_CH6NSEL_DEFAULT << 6)   /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH6NSEL_CH7                        (_ADC_SCANNSEL_CH6NSEL_CH7 << 6)       /**< Shifted mode CH7 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH9NSEL_SHIFT                     8                                      /**< Shift value for ADC_CH9NSEL */
#define _ADC_SCANNSEL_CH9NSEL_MASK                      0x300UL                                /**< Bit mask for ADC_CH9NSEL */
#define _ADC_SCANNSEL_CH9NSEL_CH8                       0x00000000UL                           /**< Mode CH8 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH9NSEL_DEFAULT                   0x00000001UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH9NSEL_CH10                      0x00000001UL                           /**< Mode CH10 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH9NSEL_CH12                      0x00000002UL                           /**< Mode CH12 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH9NSEL_CH14                      0x00000003UL                           /**< Mode CH14 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH9NSEL_CH8                        (_ADC_SCANNSEL_CH9NSEL_CH8 << 8)       /**< Shifted mode CH8 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH9NSEL_DEFAULT                    (_ADC_SCANNSEL_CH9NSEL_DEFAULT << 8)   /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH9NSEL_CH10                       (_ADC_SCANNSEL_CH9NSEL_CH10 << 8)      /**< Shifted mode CH10 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH9NSEL_CH12                       (_ADC_SCANNSEL_CH9NSEL_CH12 << 8)      /**< Shifted mode CH12 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH9NSEL_CH14                       (_ADC_SCANNSEL_CH9NSEL_CH14 << 8)      /**< Shifted mode CH14 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH11NSEL_SHIFT                    10                                     /**< Shift value for ADC_CH11NSEL */
#define _ADC_SCANNSEL_CH11NSEL_MASK                     0xC00UL                                /**< Bit mask for ADC_CH11NSEL */
#define _ADC_SCANNSEL_CH11NSEL_CH8                      0x00000000UL                           /**< Mode CH8 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH11NSEL_CH10                     0x00000001UL                           /**< Mode CH10 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH11NSEL_DEFAULT                  0x00000002UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH11NSEL_CH12                     0x00000002UL                           /**< Mode CH12 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH11NSEL_CH14                     0x00000003UL                           /**< Mode CH14 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH11NSEL_CH8                       (_ADC_SCANNSEL_CH11NSEL_CH8 << 10)     /**< Shifted mode CH8 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH11NSEL_CH10                      (_ADC_SCANNSEL_CH11NSEL_CH10 << 10)    /**< Shifted mode CH10 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH11NSEL_DEFAULT                   (_ADC_SCANNSEL_CH11NSEL_DEFAULT << 10) /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH11NSEL_CH12                      (_ADC_SCANNSEL_CH11NSEL_CH12 << 10)    /**< Shifted mode CH12 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH11NSEL_CH14                      (_ADC_SCANNSEL_CH11NSEL_CH14 << 10)    /**< Shifted mode CH14 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH13NSEL_SHIFT                    12                                     /**< Shift value for ADC_CH13NSEL */
#define _ADC_SCANNSEL_CH13NSEL_MASK                     0x3000UL                               /**< Bit mask for ADC_CH13NSEL */
#define _ADC_SCANNSEL_CH13NSEL_CH8                      0x00000000UL                           /**< Mode CH8 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH13NSEL_CH10                     0x00000001UL                           /**< Mode CH10 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH13NSEL_CH12                     0x00000002UL                           /**< Mode CH12 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH13NSEL_DEFAULT                  0x00000003UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH13NSEL_CH14                     0x00000003UL                           /**< Mode CH14 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH13NSEL_CH8                       (_ADC_SCANNSEL_CH13NSEL_CH8 << 12)     /**< Shifted mode CH8 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH13NSEL_CH10                      (_ADC_SCANNSEL_CH13NSEL_CH10 << 12)    /**< Shifted mode CH10 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH13NSEL_CH12                      (_ADC_SCANNSEL_CH13NSEL_CH12 << 12)    /**< Shifted mode CH12 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH13NSEL_DEFAULT                   (_ADC_SCANNSEL_CH13NSEL_DEFAULT << 12) /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH13NSEL_CH14                      (_ADC_SCANNSEL_CH13NSEL_CH14 << 12)    /**< Shifted mode CH14 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH15NSEL_SHIFT                    14                                     /**< Shift value for ADC_CH15NSEL */
#define _ADC_SCANNSEL_CH15NSEL_MASK                     0xC000UL                               /**< Bit mask for ADC_CH15NSEL */
#define _ADC_SCANNSEL_CH15NSEL_DEFAULT                  0x00000000UL                           /**< Mode DEFAULT for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH15NSEL_CH8                      0x00000000UL                           /**< Mode CH8 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH15NSEL_CH10                     0x00000001UL                           /**< Mode CH10 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH15NSEL_CH12                     0x00000002UL                           /**< Mode CH12 for ADC_SCANNSEL */
#define _ADC_SCANNSEL_CH15NSEL_CH14                     0x00000003UL                           /**< Mode CH14 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH15NSEL_DEFAULT                   (_ADC_SCANNSEL_CH15NSEL_DEFAULT << 14) /**< Shifted mode DEFAULT for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH15NSEL_CH8                       (_ADC_SCANNSEL_CH15NSEL_CH8 << 14)     /**< Shifted mode CH8 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH15NSEL_CH10                      (_ADC_SCANNSEL_CH15NSEL_CH10 << 14)    /**< Shifted mode CH10 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH15NSEL_CH12                      (_ADC_SCANNSEL_CH15NSEL_CH12 << 14)    /**< Shifted mode CH12 for ADC_SCANNSEL */
#define ADC_SCANNSEL_CH15NSEL_CH14                      (_ADC_SCANNSEL_CH15NSEL_CH14 << 14)    /**< Shifted mode CH14 for ADC_SCANNSEL */

/* Bit fields for ADC CMPTHR */
#define _ADC_CMPTHR_RESETVALUE                          0x00000000UL                     /**< Default value for ADC_CMPTHR */
#define _ADC_CMPTHR_MASK                                0xFFFFFFFFUL                     /**< Mask for ADC_CMPTHR */
#define _ADC_CMPTHR_ADLT_SHIFT                          0                                /**< Shift value for ADC_ADLT */
#define _ADC_CMPTHR_ADLT_MASK                           0xFFFFUL                         /**< Bit mask for ADC_ADLT */
#define _ADC_CMPTHR_ADLT_DEFAULT                        0x00000000UL                     /**< Mode DEFAULT for ADC_CMPTHR */
#define ADC_CMPTHR_ADLT_DEFAULT                         (_ADC_CMPTHR_ADLT_DEFAULT << 0)  /**< Shifted mode DEFAULT for ADC_CMPTHR */
#define _ADC_CMPTHR_ADGT_SHIFT                          16                               /**< Shift value for ADC_ADGT */
#define _ADC_CMPTHR_ADGT_MASK                           0xFFFF0000UL                     /**< Bit mask for ADC_ADGT */
#define _ADC_CMPTHR_ADGT_DEFAULT                        0x00000000UL                     /**< Mode DEFAULT for ADC_CMPTHR */
#define ADC_CMPTHR_ADGT_DEFAULT                         (_ADC_CMPTHR_ADGT_DEFAULT << 16) /**< Shifted mode DEFAULT for ADC_CMPTHR */

/* Bit fields for ADC BIASPROG */
#define _ADC_BIASPROG_RESETVALUE                        0x00000000UL                             /**< Default value for ADC_BIASPROG */
#define _ADC_BIASPROG_MASK                              0x0001113FUL                             /**< Mask for ADC_BIASPROG */
#define _ADC_BIASPROG_ADCBIASPROG_SHIFT                 0                                        /**< Shift value for ADC_ADCBIASPROG */
#define _ADC_BIASPROG_ADCBIASPROG_MASK                  0xFUL                                    /**< Bit mask for ADC_ADCBIASPROG */
#define _ADC_BIASPROG_ADCBIASPROG_DEFAULT               0x00000000UL                             /**< Mode DEFAULT for ADC_BIASPROG */
#define ADC_BIASPROG_ADCBIASPROG_DEFAULT                (_ADC_BIASPROG_ADCBIASPROG_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_BIASPROG */
#define _ADC_BIASPROG_CONVSPEED_SHIFT                   4                                        /**< Shift value for ADC_CONVSPEED */
#define _ADC_BIASPROG_CONVSPEED_MASK                    0x30UL                                   /**< Bit mask for ADC_CONVSPEED */
#define _ADC_BIASPROG_CONVSPEED_DEFAULT                 0x00000000UL                             /**< Mode DEFAULT for ADC_BIASPROG */
#define _ADC_BIASPROG_CONVSPEED_X2                      0x00000000UL                             /**< Mode X2 for ADC_BIASPROG */
#define _ADC_BIASPROG_CONVSPEED_X1                      0x00000001UL                             /**< Mode X1 for ADC_BIASPROG */
#define _ADC_BIASPROG_CONVSPEED_X1DIV2                  0x00000002UL                             /**< Mode X1DIV2 for ADC_BIASPROG */
#define _ADC_BIASPROG_CONVSPEED_X1DIV3                  0x00000003UL                             /**< Mode X1DIV3 for ADC_BIASPROG */
#define ADC_BIASPROG_CONVSPEED_DEFAULT                  (_ADC_BIASPROG_CONVSPEED_DEFAULT << 4)   /**< Shifted mode DEFAULT for ADC_BIASPROG */
#define ADC_BIASPROG_CONVSPEED_X2                       (_ADC_BIASPROG_CONVSPEED_X2 << 4)        /**< Shifted mode X2 for ADC_BIASPROG */
#define ADC_BIASPROG_CONVSPEED_X1                       (_ADC_BIASPROG_CONVSPEED_X1 << 4)        /**< Shifted mode X1 for ADC_BIASPROG */
#define ADC_BIASPROG_CONVSPEED_X1DIV2                   (_ADC_BIASPROG_CONVSPEED_X1DIV2 << 4)    /**< Shifted mode X1DIV2 for ADC_BIASPROG */
#define ADC_BIASPROG_CONVSPEED_X1DIV3                   (_ADC_BIASPROG_CONVSPEED_X1DIV3 << 4)    /**< Shifted mode X1DIV3 for ADC_BIASPROG */
#define ADC_BIASPROG_VREGSHUNT                          (0x1UL << 8)                             /**< Shunt mode for digital regulator */
#define _ADC_BIASPROG_VREGSHUNT_SHIFT                   8                                        /**< Shift value for ADC_VREGSHUNT */
#define _ADC_BIASPROG_VREGSHUNT_MASK                    0x100UL                                  /**< Bit mask for ADC_VREGSHUNT */
#define _ADC_BIASPROG_VREGSHUNT_DEFAULT                 0x00000000UL                             /**< Mode DEFAULT for ADC_BIASPROG */
#define ADC_BIASPROG_VREGSHUNT_DEFAULT                  (_ADC_BIASPROG_VREGSHUNT_DEFAULT << 8)   /**< Shifted mode DEFAULT for ADC_BIASPROG */
#define ADC_BIASPROG_VFAULTCLR                          (0x1UL << 12)                            /**< Set Vfault_clr flag */
#define _ADC_BIASPROG_VFAULTCLR_SHIFT                   12                                       /**< Shift value for ADC_VFAULTCLR */
#define _ADC_BIASPROG_VFAULTCLR_MASK                    0x1000UL                                 /**< Bit mask for ADC_VFAULTCLR */
#define _ADC_BIASPROG_VFAULTCLR_DEFAULT                 0x00000000UL                             /**< Mode DEFAULT for ADC_BIASPROG */
#define ADC_BIASPROG_VFAULTCLR_DEFAULT                  (_ADC_BIASPROG_VFAULTCLR_DEFAULT << 12)  /**< Shifted mode DEFAULT for ADC_BIASPROG */
#define ADC_BIASPROG_GPBIASACC                          (0x1UL << 16)                            /**< Accuracy setting for the gp_bias block during ADC operation */
#define _ADC_BIASPROG_GPBIASACC_SHIFT                   16                                       /**< Shift value for ADC_GPBIASACC */
#define _ADC_BIASPROG_GPBIASACC_MASK                    0x10000UL                                /**< Bit mask for ADC_GPBIASACC */
#define _ADC_BIASPROG_GPBIASACC_DEFAULT                 0x00000000UL                             /**< Mode DEFAULT for ADC_BIASPROG */
#define _ADC_BIASPROG_GPBIASACC_HIGHACC                 0x00000000UL                             /**< Mode HIGHACC for ADC_BIASPROG */
#define _ADC_BIASPROG_GPBIASACC_LOWACC                  0x00000001UL                             /**< Mode LOWACC for ADC_BIASPROG */
#define ADC_BIASPROG_GPBIASACC_DEFAULT                  (_ADC_BIASPROG_GPBIASACC_DEFAULT << 16)  /**< Shifted mode DEFAULT for ADC_BIASPROG */
#define ADC_BIASPROG_GPBIASACC_HIGHACC                  (_ADC_BIASPROG_GPBIASACC_HIGHACC << 16)  /**< Shifted mode HIGHACC for ADC_BIASPROG */
#define ADC_BIASPROG_GPBIASACC_LOWACC                   (_ADC_BIASPROG_GPBIASACC_LOWACC << 16)   /**< Shifted mode LOWACC for ADC_BIASPROG */

/* Bit fields for ADC CAL */
#define _ADC_CAL_RESETVALUE                             0x40784078UL                            /**< Default value for ADC_CAL */
#define _ADC_CAL_MASK                                   0xFFFFFFFFUL                            /**< Mask for ADC_CAL */
#define _ADC_CAL_SINGLEOFFSET_SHIFT                     0                                       /**< Shift value for ADC_SINGLEOFFSET */
#define _ADC_CAL_SINGLEOFFSET_MASK                      0xFUL                                   /**< Bit mask for ADC_SINGLEOFFSET */
#define _ADC_CAL_SINGLEOFFSET_DEFAULT                   0x00000008UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_SINGLEOFFSET_DEFAULT                    (_ADC_CAL_SINGLEOFFSET_DEFAULT << 0)    /**< Shifted mode DEFAULT for ADC_CAL */
#define _ADC_CAL_SINGLEOFFSETINV_SHIFT                  4                                       /**< Shift value for ADC_SINGLEOFFSETINV */
#define _ADC_CAL_SINGLEOFFSETINV_MASK                   0xF0UL                                  /**< Bit mask for ADC_SINGLEOFFSETINV */
#define _ADC_CAL_SINGLEOFFSETINV_DEFAULT                0x00000007UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_SINGLEOFFSETINV_DEFAULT                 (_ADC_CAL_SINGLEOFFSETINV_DEFAULT << 4) /**< Shifted mode DEFAULT for ADC_CAL */
#define _ADC_CAL_SINGLEGAIN_SHIFT                       8                                       /**< Shift value for ADC_SINGLEGAIN */
#define _ADC_CAL_SINGLEGAIN_MASK                        0x7F00UL                                /**< Bit mask for ADC_SINGLEGAIN */
#define _ADC_CAL_SINGLEGAIN_DEFAULT                     0x00000040UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_SINGLEGAIN_DEFAULT                      (_ADC_CAL_SINGLEGAIN_DEFAULT << 8)      /**< Shifted mode DEFAULT for ADC_CAL */
#define ADC_CAL_OFFSETINVMODE                           (0x1UL << 15)                           /**< Negative single-ended offset calibration is enabled */
#define _ADC_CAL_OFFSETINVMODE_SHIFT                    15                                      /**< Shift value for ADC_OFFSETINVMODE */
#define _ADC_CAL_OFFSETINVMODE_MASK                     0x8000UL                                /**< Bit mask for ADC_OFFSETINVMODE */
#define _ADC_CAL_OFFSETINVMODE_DEFAULT                  0x00000000UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_OFFSETINVMODE_DEFAULT                   (_ADC_CAL_OFFSETINVMODE_DEFAULT << 15)  /**< Shifted mode DEFAULT for ADC_CAL */
#define _ADC_CAL_SCANOFFSET_SHIFT                       16                                      /**< Shift value for ADC_SCANOFFSET */
#define _ADC_CAL_SCANOFFSET_MASK                        0xF0000UL                               /**< Bit mask for ADC_SCANOFFSET */
#define _ADC_CAL_SCANOFFSET_DEFAULT                     0x00000008UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_SCANOFFSET_DEFAULT                      (_ADC_CAL_SCANOFFSET_DEFAULT << 16)     /**< Shifted mode DEFAULT for ADC_CAL */
#define _ADC_CAL_SCANOFFSETINV_SHIFT                    20                                      /**< Shift value for ADC_SCANOFFSETINV */
#define _ADC_CAL_SCANOFFSETINV_MASK                     0xF00000UL                              /**< Bit mask for ADC_SCANOFFSETINV */
#define _ADC_CAL_SCANOFFSETINV_DEFAULT                  0x00000007UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_SCANOFFSETINV_DEFAULT                   (_ADC_CAL_SCANOFFSETINV_DEFAULT << 20)  /**< Shifted mode DEFAULT for ADC_CAL */
#define _ADC_CAL_SCANGAIN_SHIFT                         24                                      /**< Shift value for ADC_SCANGAIN */
#define _ADC_CAL_SCANGAIN_MASK                          0x7F000000UL                            /**< Bit mask for ADC_SCANGAIN */
#define _ADC_CAL_SCANGAIN_DEFAULT                       0x00000040UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_SCANGAIN_DEFAULT                        (_ADC_CAL_SCANGAIN_DEFAULT << 24)       /**< Shifted mode DEFAULT for ADC_CAL */
#define ADC_CAL_CALEN                                   (0x1UL << 31)                           /**< Calibration mode is enabled */
#define _ADC_CAL_CALEN_SHIFT                            31                                      /**< Shift value for ADC_CALEN */
#define _ADC_CAL_CALEN_MASK                             0x80000000UL                            /**< Bit mask for ADC_CALEN */
#define _ADC_CAL_CALEN_DEFAULT                          0x00000000UL                            /**< Mode DEFAULT for ADC_CAL */
#define ADC_CAL_CALEN_DEFAULT                           (_ADC_CAL_CALEN_DEFAULT << 31)          /**< Shifted mode DEFAULT for ADC_CAL */

/* Bit fields for ADC IF */
#define _ADC_IF_RESETVALUE                              0x00000000UL                      /**< Default value for ADC_IF */
#define _ADC_IF_MASK                                    0x03030F03UL                      /**< Mask for ADC_IF */
#define ADC_IF_SINGLE                                   (0x1UL << 0)                      /**< Single Conversion Complete Interrupt Flag */
#define _ADC_IF_SINGLE_SHIFT                            0                                 /**< Shift value for ADC_SINGLE */
#define _ADC_IF_SINGLE_MASK                             0x1UL                             /**< Bit mask for ADC_SINGLE */
#define _ADC_IF_SINGLE_DEFAULT                          0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SINGLE_DEFAULT                           (_ADC_IF_SINGLE_DEFAULT << 0)     /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_SCAN                                     (0x1UL << 1)                      /**< Scan Conversion Complete Interrupt Flag */
#define _ADC_IF_SCAN_SHIFT                              1                                 /**< Shift value for ADC_SCAN */
#define _ADC_IF_SCAN_MASK                               0x2UL                             /**< Bit mask for ADC_SCAN */
#define _ADC_IF_SCAN_DEFAULT                            0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SCAN_DEFAULT                             (_ADC_IF_SCAN_DEFAULT << 1)       /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_SINGLEOF                                 (0x1UL << 8)                      /**< Single Result Overflow Interrupt Flag */
#define _ADC_IF_SINGLEOF_SHIFT                          8                                 /**< Shift value for ADC_SINGLEOF */
#define _ADC_IF_SINGLEOF_MASK                           0x100UL                           /**< Bit mask for ADC_SINGLEOF */
#define _ADC_IF_SINGLEOF_DEFAULT                        0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SINGLEOF_DEFAULT                         (_ADC_IF_SINGLEOF_DEFAULT << 8)   /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_SCANOF                                   (0x1UL << 9)                      /**< Scan Result Overflow Interrupt Flag */
#define _ADC_IF_SCANOF_SHIFT                            9                                 /**< Shift value for ADC_SCANOF */
#define _ADC_IF_SCANOF_MASK                             0x200UL                           /**< Bit mask for ADC_SCANOF */
#define _ADC_IF_SCANOF_DEFAULT                          0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SCANOF_DEFAULT                           (_ADC_IF_SCANOF_DEFAULT << 9)     /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_SINGLEUF                                 (0x1UL << 10)                     /**< Single Result Underflow Interrupt Flag */
#define _ADC_IF_SINGLEUF_SHIFT                          10                                /**< Shift value for ADC_SINGLEUF */
#define _ADC_IF_SINGLEUF_MASK                           0x400UL                           /**< Bit mask for ADC_SINGLEUF */
#define _ADC_IF_SINGLEUF_DEFAULT                        0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SINGLEUF_DEFAULT                         (_ADC_IF_SINGLEUF_DEFAULT << 10)  /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_SCANUF                                   (0x1UL << 11)                     /**< Scan Result Underflow Interrupt Flag */
#define _ADC_IF_SCANUF_SHIFT                            11                                /**< Shift value for ADC_SCANUF */
#define _ADC_IF_SCANUF_MASK                             0x800UL                           /**< Bit mask for ADC_SCANUF */
#define _ADC_IF_SCANUF_DEFAULT                          0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SCANUF_DEFAULT                           (_ADC_IF_SCANUF_DEFAULT << 11)    /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_SINGLECMP                                (0x1UL << 16)                     /**< Single Result Compare Match Interrupt Flag */
#define _ADC_IF_SINGLECMP_SHIFT                         16                                /**< Shift value for ADC_SINGLECMP */
#define _ADC_IF_SINGLECMP_MASK                          0x10000UL                         /**< Bit mask for ADC_SINGLECMP */
#define _ADC_IF_SINGLECMP_DEFAULT                       0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SINGLECMP_DEFAULT                        (_ADC_IF_SINGLECMP_DEFAULT << 16) /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_SCANCMP                                  (0x1UL << 17)                     /**< Scan Result Compare Match Interrupt Flag */
#define _ADC_IF_SCANCMP_SHIFT                           17                                /**< Shift value for ADC_SCANCMP */
#define _ADC_IF_SCANCMP_MASK                            0x20000UL                         /**< Bit mask for ADC_SCANCMP */
#define _ADC_IF_SCANCMP_DEFAULT                         0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_SCANCMP_DEFAULT                          (_ADC_IF_SCANCMP_DEFAULT << 17)   /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_VREFOV                                   (0x1UL << 24)                     /**< VREF OverVoltage Interrupt Flag */
#define _ADC_IF_VREFOV_SHIFT                            24                                /**< Shift value for ADC_VREFOV */
#define _ADC_IF_VREFOV_MASK                             0x1000000UL                       /**< Bit mask for ADC_VREFOV */
#define _ADC_IF_VREFOV_DEFAULT                          0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_VREFOV_DEFAULT                           (_ADC_IF_VREFOV_DEFAULT << 24)    /**< Shifted mode DEFAULT for ADC_IF */
#define ADC_IF_PROGERR                                  (0x1UL << 25)                     /**< Programming Error Interrupt Flag */
#define _ADC_IF_PROGERR_SHIFT                           25                                /**< Shift value for ADC_PROGERR */
#define _ADC_IF_PROGERR_MASK                            0x2000000UL                       /**< Bit mask for ADC_PROGERR */
#define _ADC_IF_PROGERR_DEFAULT                         0x00000000UL                      /**< Mode DEFAULT for ADC_IF */
#define ADC_IF_PROGERR_DEFAULT                          (_ADC_IF_PROGERR_DEFAULT << 25)   /**< Shifted mode DEFAULT for ADC_IF */

/* Bit fields for ADC IFS */
#define _ADC_IFS_RESETVALUE                             0x00000000UL                       /**< Default value for ADC_IFS */
#define _ADC_IFS_MASK                                   0x03030F00UL                       /**< Mask for ADC_IFS */
#define ADC_IFS_SINGLEOF                                (0x1UL << 8)                       /**< Set SINGLEOF Interrupt Flag */
#define _ADC_IFS_SINGLEOF_SHIFT                         8                                  /**< Shift value for ADC_SINGLEOF */
#define _ADC_IFS_SINGLEOF_MASK                          0x100UL                            /**< Bit mask for ADC_SINGLEOF */
#define _ADC_IFS_SINGLEOF_DEFAULT                       0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_SINGLEOF_DEFAULT                        (_ADC_IFS_SINGLEOF_DEFAULT << 8)   /**< Shifted mode DEFAULT for ADC_IFS */
#define ADC_IFS_SCANOF                                  (0x1UL << 9)                       /**< Set SCANOF Interrupt Flag */
#define _ADC_IFS_SCANOF_SHIFT                           9                                  /**< Shift value for ADC_SCANOF */
#define _ADC_IFS_SCANOF_MASK                            0x200UL                            /**< Bit mask for ADC_SCANOF */
#define _ADC_IFS_SCANOF_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_SCANOF_DEFAULT                          (_ADC_IFS_SCANOF_DEFAULT << 9)     /**< Shifted mode DEFAULT for ADC_IFS */
#define ADC_IFS_SINGLEUF                                (0x1UL << 10)                      /**< Set SINGLEUF Interrupt Flag */
#define _ADC_IFS_SINGLEUF_SHIFT                         10                                 /**< Shift value for ADC_SINGLEUF */
#define _ADC_IFS_SINGLEUF_MASK                          0x400UL                            /**< Bit mask for ADC_SINGLEUF */
#define _ADC_IFS_SINGLEUF_DEFAULT                       0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_SINGLEUF_DEFAULT                        (_ADC_IFS_SINGLEUF_DEFAULT << 10)  /**< Shifted mode DEFAULT for ADC_IFS */
#define ADC_IFS_SCANUF                                  (0x1UL << 11)                      /**< Set SCANUF Interrupt Flag */
#define _ADC_IFS_SCANUF_SHIFT                           11                                 /**< Shift value for ADC_SCANUF */
#define _ADC_IFS_SCANUF_MASK                            0x800UL                            /**< Bit mask for ADC_SCANUF */
#define _ADC_IFS_SCANUF_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_SCANUF_DEFAULT                          (_ADC_IFS_SCANUF_DEFAULT << 11)    /**< Shifted mode DEFAULT for ADC_IFS */
#define ADC_IFS_SINGLECMP                               (0x1UL << 16)                      /**< Set SINGLECMP Interrupt Flag */
#define _ADC_IFS_SINGLECMP_SHIFT                        16                                 /**< Shift value for ADC_SINGLECMP */
#define _ADC_IFS_SINGLECMP_MASK                         0x10000UL                          /**< Bit mask for ADC_SINGLECMP */
#define _ADC_IFS_SINGLECMP_DEFAULT                      0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_SINGLECMP_DEFAULT                       (_ADC_IFS_SINGLECMP_DEFAULT << 16) /**< Shifted mode DEFAULT for ADC_IFS */
#define ADC_IFS_SCANCMP                                 (0x1UL << 17)                      /**< Set SCANCMP Interrupt Flag */
#define _ADC_IFS_SCANCMP_SHIFT                          17                                 /**< Shift value for ADC_SCANCMP */
#define _ADC_IFS_SCANCMP_MASK                           0x20000UL                          /**< Bit mask for ADC_SCANCMP */
#define _ADC_IFS_SCANCMP_DEFAULT                        0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_SCANCMP_DEFAULT                         (_ADC_IFS_SCANCMP_DEFAULT << 17)   /**< Shifted mode DEFAULT for ADC_IFS */
#define ADC_IFS_VREFOV                                  (0x1UL << 24)                      /**< Set VREFOV Interrupt Flag */
#define _ADC_IFS_VREFOV_SHIFT                           24                                 /**< Shift value for ADC_VREFOV */
#define _ADC_IFS_VREFOV_MASK                            0x1000000UL                        /**< Bit mask for ADC_VREFOV */
#define _ADC_IFS_VREFOV_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_VREFOV_DEFAULT                          (_ADC_IFS_VREFOV_DEFAULT << 24)    /**< Shifted mode DEFAULT for ADC_IFS */
#define ADC_IFS_PROGERR                                 (0x1UL << 25)                      /**< Set PROGERR Interrupt Flag */
#define _ADC_IFS_PROGERR_SHIFT                          25                                 /**< Shift value for ADC_PROGERR */
#define _ADC_IFS_PROGERR_MASK                           0x2000000UL                        /**< Bit mask for ADC_PROGERR */
#define _ADC_IFS_PROGERR_DEFAULT                        0x00000000UL                       /**< Mode DEFAULT for ADC_IFS */
#define ADC_IFS_PROGERR_DEFAULT                         (_ADC_IFS_PROGERR_DEFAULT << 25)   /**< Shifted mode DEFAULT for ADC_IFS */

/* Bit fields for ADC IFC */
#define _ADC_IFC_RESETVALUE                             0x00000000UL                       /**< Default value for ADC_IFC */
#define _ADC_IFC_MASK                                   0x03030F00UL                       /**< Mask for ADC_IFC */
#define ADC_IFC_SINGLEOF                                (0x1UL << 8)                       /**< Clear SINGLEOF Interrupt Flag */
#define _ADC_IFC_SINGLEOF_SHIFT                         8                                  /**< Shift value for ADC_SINGLEOF */
#define _ADC_IFC_SINGLEOF_MASK                          0x100UL                            /**< Bit mask for ADC_SINGLEOF */
#define _ADC_IFC_SINGLEOF_DEFAULT                       0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_SINGLEOF_DEFAULT                        (_ADC_IFC_SINGLEOF_DEFAULT << 8)   /**< Shifted mode DEFAULT for ADC_IFC */
#define ADC_IFC_SCANOF                                  (0x1UL << 9)                       /**< Clear SCANOF Interrupt Flag */
#define _ADC_IFC_SCANOF_SHIFT                           9                                  /**< Shift value for ADC_SCANOF */
#define _ADC_IFC_SCANOF_MASK                            0x200UL                            /**< Bit mask for ADC_SCANOF */
#define _ADC_IFC_SCANOF_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_SCANOF_DEFAULT                          (_ADC_IFC_SCANOF_DEFAULT << 9)     /**< Shifted mode DEFAULT for ADC_IFC */
#define ADC_IFC_SINGLEUF                                (0x1UL << 10)                      /**< Clear SINGLEUF Interrupt Flag */
#define _ADC_IFC_SINGLEUF_SHIFT                         10                                 /**< Shift value for ADC_SINGLEUF */
#define _ADC_IFC_SINGLEUF_MASK                          0x400UL                            /**< Bit mask for ADC_SINGLEUF */
#define _ADC_IFC_SINGLEUF_DEFAULT                       0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_SINGLEUF_DEFAULT                        (_ADC_IFC_SINGLEUF_DEFAULT << 10)  /**< Shifted mode DEFAULT for ADC_IFC */
#define ADC_IFC_SCANUF                                  (0x1UL << 11)                      /**< Clear SCANUF Interrupt Flag */
#define _ADC_IFC_SCANUF_SHIFT                           11                                 /**< Shift value for ADC_SCANUF */
#define _ADC_IFC_SCANUF_MASK                            0x800UL                            /**< Bit mask for ADC_SCANUF */
#define _ADC_IFC_SCANUF_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_SCANUF_DEFAULT                          (_ADC_IFC_SCANUF_DEFAULT << 11)    /**< Shifted mode DEFAULT for ADC_IFC */
#define ADC_IFC_SINGLECMP                               (0x1UL << 16)                      /**< Clear SINGLECMP Interrupt Flag */
#define _ADC_IFC_SINGLECMP_SHIFT                        16                                 /**< Shift value for ADC_SINGLECMP */
#define _ADC_IFC_SINGLECMP_MASK                         0x10000UL                          /**< Bit mask for ADC_SINGLECMP */
#define _ADC_IFC_SINGLECMP_DEFAULT                      0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_SINGLECMP_DEFAULT                       (_ADC_IFC_SINGLECMP_DEFAULT << 16) /**< Shifted mode DEFAULT for ADC_IFC */
#define ADC_IFC_SCANCMP                                 (0x1UL << 17)                      /**< Clear SCANCMP Interrupt Flag */
#define _ADC_IFC_SCANCMP_SHIFT                          17                                 /**< Shift value for ADC_SCANCMP */
#define _ADC_IFC_SCANCMP_MASK                           0x20000UL                          /**< Bit mask for ADC_SCANCMP */
#define _ADC_IFC_SCANCMP_DEFAULT                        0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_SCANCMP_DEFAULT                         (_ADC_IFC_SCANCMP_DEFAULT << 17)   /**< Shifted mode DEFAULT for ADC_IFC */
#define ADC_IFC_VREFOV                                  (0x1UL << 24)                      /**< Clear VREFOV Interrupt Flag */
#define _ADC_IFC_VREFOV_SHIFT                           24                                 /**< Shift value for ADC_VREFOV */
#define _ADC_IFC_VREFOV_MASK                            0x1000000UL                        /**< Bit mask for ADC_VREFOV */
#define _ADC_IFC_VREFOV_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_VREFOV_DEFAULT                          (_ADC_IFC_VREFOV_DEFAULT << 24)    /**< Shifted mode DEFAULT for ADC_IFC */
#define ADC_IFC_PROGERR                                 (0x1UL << 25)                      /**< Clear PROGERR Interrupt Flag */
#define _ADC_IFC_PROGERR_SHIFT                          25                                 /**< Shift value for ADC_PROGERR */
#define _ADC_IFC_PROGERR_MASK                           0x2000000UL                        /**< Bit mask for ADC_PROGERR */
#define _ADC_IFC_PROGERR_DEFAULT                        0x00000000UL                       /**< Mode DEFAULT for ADC_IFC */
#define ADC_IFC_PROGERR_DEFAULT                         (_ADC_IFC_PROGERR_DEFAULT << 25)   /**< Shifted mode DEFAULT for ADC_IFC */

/* Bit fields for ADC IEN */
#define _ADC_IEN_RESETVALUE                             0x00000000UL                       /**< Default value for ADC_IEN */
#define _ADC_IEN_MASK                                   0x03030F03UL                       /**< Mask for ADC_IEN */
#define ADC_IEN_SINGLE                                  (0x1UL << 0)                       /**< SINGLE Interrupt Enable */
#define _ADC_IEN_SINGLE_SHIFT                           0                                  /**< Shift value for ADC_SINGLE */
#define _ADC_IEN_SINGLE_MASK                            0x1UL                              /**< Bit mask for ADC_SINGLE */
#define _ADC_IEN_SINGLE_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SINGLE_DEFAULT                          (_ADC_IEN_SINGLE_DEFAULT << 0)     /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCAN                                    (0x1UL << 1)                       /**< SCAN Interrupt Enable */
#define _ADC_IEN_SCAN_SHIFT                             1                                  /**< Shift value for ADC_SCAN */
#define _ADC_IEN_SCAN_MASK                              0x2UL                              /**< Bit mask for ADC_SCAN */
#define _ADC_IEN_SCAN_DEFAULT                           0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCAN_DEFAULT                            (_ADC_IEN_SCAN_DEFAULT << 1)       /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_SINGLEOF                                (0x1UL << 8)                       /**< SINGLEOF Interrupt Enable */
#define _ADC_IEN_SINGLEOF_SHIFT                         8                                  /**< Shift value for ADC_SINGLEOF */
#define _ADC_IEN_SINGLEOF_MASK                          0x100UL                            /**< Bit mask for ADC_SINGLEOF */
#define _ADC_IEN_SINGLEOF_DEFAULT                       0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SINGLEOF_DEFAULT                        (_ADC_IEN_SINGLEOF_DEFAULT << 8)   /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCANOF                                  (0x1UL << 9)                       /**< SCANOF Interrupt Enable */
#define _ADC_IEN_SCANOF_SHIFT                           9                                  /**< Shift value for ADC_SCANOF */
#define _ADC_IEN_SCANOF_MASK                            0x200UL                            /**< Bit mask for ADC_SCANOF */
#define _ADC_IEN_SCANOF_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCANOF_DEFAULT                          (_ADC_IEN_SCANOF_DEFAULT << 9)     /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_SINGLEUF                                (0x1UL << 10)                      /**< SINGLEUF Interrupt Enable */
#define _ADC_IEN_SINGLEUF_SHIFT                         10                                 /**< Shift value for ADC_SINGLEUF */
#define _ADC_IEN_SINGLEUF_MASK                          0x400UL                            /**< Bit mask for ADC_SINGLEUF */
#define _ADC_IEN_SINGLEUF_DEFAULT                       0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SINGLEUF_DEFAULT                        (_ADC_IEN_SINGLEUF_DEFAULT << 10)  /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCANUF                                  (0x1UL << 11)                      /**< SCANUF Interrupt Enable */
#define _ADC_IEN_SCANUF_SHIFT                           11                                 /**< Shift value for ADC_SCANUF */
#define _ADC_IEN_SCANUF_MASK                            0x800UL                            /**< Bit mask for ADC_SCANUF */
#define _ADC_IEN_SCANUF_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCANUF_DEFAULT                          (_ADC_IEN_SCANUF_DEFAULT << 11)    /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_SINGLECMP                               (0x1UL << 16)                      /**< SINGLECMP Interrupt Enable */
#define _ADC_IEN_SINGLECMP_SHIFT                        16                                 /**< Shift value for ADC_SINGLECMP */
#define _ADC_IEN_SINGLECMP_MASK                         0x10000UL                          /**< Bit mask for ADC_SINGLECMP */
#define _ADC_IEN_SINGLECMP_DEFAULT                      0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SINGLECMP_DEFAULT                       (_ADC_IEN_SINGLECMP_DEFAULT << 16) /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCANCMP                                 (0x1UL << 17)                      /**< SCANCMP Interrupt Enable */
#define _ADC_IEN_SCANCMP_SHIFT                          17                                 /**< Shift value for ADC_SCANCMP */
#define _ADC_IEN_SCANCMP_MASK                           0x20000UL                          /**< Bit mask for ADC_SCANCMP */
#define _ADC_IEN_SCANCMP_DEFAULT                        0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_SCANCMP_DEFAULT                         (_ADC_IEN_SCANCMP_DEFAULT << 17)   /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_VREFOV                                  (0x1UL << 24)                      /**< VREFOV Interrupt Enable */
#define _ADC_IEN_VREFOV_SHIFT                           24                                 /**< Shift value for ADC_VREFOV */
#define _ADC_IEN_VREFOV_MASK                            0x1000000UL                        /**< Bit mask for ADC_VREFOV */
#define _ADC_IEN_VREFOV_DEFAULT                         0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_VREFOV_DEFAULT                          (_ADC_IEN_VREFOV_DEFAULT << 24)    /**< Shifted mode DEFAULT for ADC_IEN */
#define ADC_IEN_PROGERR                                 (0x1UL << 25)                      /**< PROGERR Interrupt Enable */
#define _ADC_IEN_PROGERR_SHIFT                          25                                 /**< Shift value for ADC_PROGERR */
#define _ADC_IEN_PROGERR_MASK                           0x2000000UL                        /**< Bit mask for ADC_PROGERR */
#define _ADC_IEN_PROGERR_DEFAULT                        0x00000000UL                       /**< Mode DEFAULT for ADC_IEN */
#define ADC_IEN_PROGERR_DEFAULT                         (_ADC_IEN_PROGERR_DEFAULT << 25)   /**< Shifted mode DEFAULT for ADC_IEN */

/* Bit fields for ADC SINGLEDATA */
#define _ADC_SINGLEDATA_RESETVALUE                      0x00000000UL                        /**< Default value for ADC_SINGLEDATA */
#define _ADC_SINGLEDATA_MASK                            0xFFFFFFFFUL                        /**< Mask for ADC_SINGLEDATA */
#define _ADC_SINGLEDATA_DATA_SHIFT                      0                                   /**< Shift value for ADC_DATA */
#define _ADC_SINGLEDATA_DATA_MASK                       0xFFFFFFFFUL                        /**< Bit mask for ADC_DATA */
#define _ADC_SINGLEDATA_DATA_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_SINGLEDATA */
#define ADC_SINGLEDATA_DATA_DEFAULT                     (_ADC_SINGLEDATA_DATA_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SINGLEDATA */

/* Bit fields for ADC SCANDATA */
#define _ADC_SCANDATA_RESETVALUE                        0x00000000UL                      /**< Default value for ADC_SCANDATA */
#define _ADC_SCANDATA_MASK                              0xFFFFFFFFUL                      /**< Mask for ADC_SCANDATA */
#define _ADC_SCANDATA_DATA_SHIFT                        0                                 /**< Shift value for ADC_DATA */
#define _ADC_SCANDATA_DATA_MASK                         0xFFFFFFFFUL                      /**< Bit mask for ADC_DATA */
#define _ADC_SCANDATA_DATA_DEFAULT                      0x00000000UL                      /**< Mode DEFAULT for ADC_SCANDATA */
#define ADC_SCANDATA_DATA_DEFAULT                       (_ADC_SCANDATA_DATA_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SCANDATA */

/* Bit fields for ADC SINGLEDATAP */
#define _ADC_SINGLEDATAP_RESETVALUE                     0x00000000UL                          /**< Default value for ADC_SINGLEDATAP */
#define _ADC_SINGLEDATAP_MASK                           0xFFFFFFFFUL                          /**< Mask for ADC_SINGLEDATAP */
#define _ADC_SINGLEDATAP_DATAP_SHIFT                    0                                     /**< Shift value for ADC_DATAP */
#define _ADC_SINGLEDATAP_DATAP_MASK                     0xFFFFFFFFUL                          /**< Bit mask for ADC_DATAP */
#define _ADC_SINGLEDATAP_DATAP_DEFAULT                  0x00000000UL                          /**< Mode DEFAULT for ADC_SINGLEDATAP */
#define ADC_SINGLEDATAP_DATAP_DEFAULT                   (_ADC_SINGLEDATAP_DATAP_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SINGLEDATAP */

/* Bit fields for ADC SCANDATAP */
#define _ADC_SCANDATAP_RESETVALUE                       0x00000000UL                        /**< Default value for ADC_SCANDATAP */
#define _ADC_SCANDATAP_MASK                             0xFFFFFFFFUL                        /**< Mask for ADC_SCANDATAP */
#define _ADC_SCANDATAP_DATAP_SHIFT                      0                                   /**< Shift value for ADC_DATAP */
#define _ADC_SCANDATAP_DATAP_MASK                       0xFFFFFFFFUL                        /**< Bit mask for ADC_DATAP */
#define _ADC_SCANDATAP_DATAP_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_SCANDATAP */
#define ADC_SCANDATAP_DATAP_DEFAULT                     (_ADC_SCANDATAP_DATAP_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SCANDATAP */

/* Bit fields for ADC SCANDATAX */
#define _ADC_SCANDATAX_RESETVALUE                       0x00000000UL                               /**< Default value for ADC_SCANDATAX */
#define _ADC_SCANDATAX_MASK                             0x001FFFFFUL                               /**< Mask for ADC_SCANDATAX */
#define _ADC_SCANDATAX_DATA_SHIFT                       0                                          /**< Shift value for ADC_DATA */
#define _ADC_SCANDATAX_DATA_MASK                        0xFFFFUL                                   /**< Bit mask for ADC_DATA */
#define _ADC_SCANDATAX_DATA_DEFAULT                     0x00000000UL                               /**< Mode DEFAULT for ADC_SCANDATAX */
#define ADC_SCANDATAX_DATA_DEFAULT                      (_ADC_SCANDATAX_DATA_DEFAULT << 0)         /**< Shifted mode DEFAULT for ADC_SCANDATAX */
#define _ADC_SCANDATAX_SCANDATASRC_SHIFT                16                                         /**< Shift value for ADC_SCANDATASRC */
#define _ADC_SCANDATAX_SCANDATASRC_MASK                 0x1F0000UL                                 /**< Bit mask for ADC_SCANDATASRC */
#define _ADC_SCANDATAX_SCANDATASRC_DEFAULT              0x00000000UL                               /**< Mode DEFAULT for ADC_SCANDATAX */
#define ADC_SCANDATAX_SCANDATASRC_DEFAULT               (_ADC_SCANDATAX_SCANDATASRC_DEFAULT << 16) /**< Shifted mode DEFAULT for ADC_SCANDATAX */

/* Bit fields for ADC SCANDATAXP */
#define _ADC_SCANDATAXP_RESETVALUE                      0x00000000UL                                    /**< Default value for ADC_SCANDATAXP */
#define _ADC_SCANDATAXP_MASK                            0x001FFFFFUL                                    /**< Mask for ADC_SCANDATAXP */
#define _ADC_SCANDATAXP_DATAP_SHIFT                     0                                               /**< Shift value for ADC_DATAP */
#define _ADC_SCANDATAXP_DATAP_MASK                      0xFFFFUL                                        /**< Bit mask for ADC_DATAP */
#define _ADC_SCANDATAXP_DATAP_DEFAULT                   0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANDATAXP */
#define ADC_SCANDATAXP_DATAP_DEFAULT                    (_ADC_SCANDATAXP_DATAP_DEFAULT << 0)            /**< Shifted mode DEFAULT for ADC_SCANDATAXP */
#define _ADC_SCANDATAXP_SCANDATASRCPEEK_SHIFT           16                                              /**< Shift value for ADC_SCANDATASRCPEEK */
#define _ADC_SCANDATAXP_SCANDATASRCPEEK_MASK            0x1F0000UL                                      /**< Bit mask for ADC_SCANDATASRCPEEK */
#define _ADC_SCANDATAXP_SCANDATASRCPEEK_DEFAULT         0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANDATAXP */
#define ADC_SCANDATAXP_SCANDATASRCPEEK_DEFAULT          (_ADC_SCANDATAXP_SCANDATASRCPEEK_DEFAULT << 16) /**< Shifted mode DEFAULT for ADC_SCANDATAXP */

/* Bit fields for ADC BUSREQ */
#define _ADC_BUSREQ_RESETVALUE                          0x00000000UL                        /**< Default value for ADC_BUSREQ */
#define _ADC_BUSREQ_MASK                                0x000003FFUL                        /**< Mask for ADC_BUSREQ */
#define ADC_BUSREQ_BUS0XREQ                             (0x1UL << 0)                        /**< 1 if the bus connected to BUS0X is requested */
#define _ADC_BUSREQ_BUS0XREQ_SHIFT                      0                                   /**< Shift value for ADC_BUS0XREQ */
#define _ADC_BUSREQ_BUS0XREQ_MASK                       0x1UL                               /**< Bit mask for ADC_BUS0XREQ */
#define _ADC_BUSREQ_BUS0XREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS0XREQ_DEFAULT                     (_ADC_BUSREQ_BUS0XREQ_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS0YREQ                             (0x1UL << 1)                        /**< 1 if the bus connected to BUS0Y is requested */
#define _ADC_BUSREQ_BUS0YREQ_SHIFT                      1                                   /**< Shift value for ADC_BUS0YREQ */
#define _ADC_BUSREQ_BUS0YREQ_MASK                       0x2UL                               /**< Bit mask for ADC_BUS0YREQ */
#define _ADC_BUSREQ_BUS0YREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS0YREQ_DEFAULT                     (_ADC_BUSREQ_BUS0YREQ_DEFAULT << 1) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS1XREQ                             (0x1UL << 2)                        /**< 1 if the bus connected to BUS2X is requested */
#define _ADC_BUSREQ_BUS1XREQ_SHIFT                      2                                   /**< Shift value for ADC_BUS1XREQ */
#define _ADC_BUSREQ_BUS1XREQ_MASK                       0x4UL                               /**< Bit mask for ADC_BUS1XREQ */
#define _ADC_BUSREQ_BUS1XREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS1XREQ_DEFAULT                     (_ADC_BUSREQ_BUS1XREQ_DEFAULT << 2) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS1YREQ                             (0x1UL << 3)                        /**< 1 if the bus connected to BUS1X is requested */
#define _ADC_BUSREQ_BUS1YREQ_SHIFT                      3                                   /**< Shift value for ADC_BUS1YREQ */
#define _ADC_BUSREQ_BUS1YREQ_MASK                       0x8UL                               /**< Bit mask for ADC_BUS1YREQ */
#define _ADC_BUSREQ_BUS1YREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS1YREQ_DEFAULT                     (_ADC_BUSREQ_BUS1YREQ_DEFAULT << 3) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS2XREQ                             (0x1UL << 4)                        /**< 1 if the bus connected to BUS2X is requested */
#define _ADC_BUSREQ_BUS2XREQ_SHIFT                      4                                   /**< Shift value for ADC_BUS2XREQ */
#define _ADC_BUSREQ_BUS2XREQ_MASK                       0x10UL                              /**< Bit mask for ADC_BUS2XREQ */
#define _ADC_BUSREQ_BUS2XREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS2XREQ_DEFAULT                     (_ADC_BUSREQ_BUS2XREQ_DEFAULT << 4) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS2YREQ                             (0x1UL << 5)                        /**< 1 if the bus connected to BUS2Y is requested */
#define _ADC_BUSREQ_BUS2YREQ_SHIFT                      5                                   /**< Shift value for ADC_BUS2YREQ */
#define _ADC_BUSREQ_BUS2YREQ_MASK                       0x20UL                              /**< Bit mask for ADC_BUS2YREQ */
#define _ADC_BUSREQ_BUS2YREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS2YREQ_DEFAULT                     (_ADC_BUSREQ_BUS2YREQ_DEFAULT << 5) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS3XREQ                             (0x1UL << 6)                        /**< 1 if the bus connected to BUS3X is requested */
#define _ADC_BUSREQ_BUS3XREQ_SHIFT                      6                                   /**< Shift value for ADC_BUS3XREQ */
#define _ADC_BUSREQ_BUS3XREQ_MASK                       0x40UL                              /**< Bit mask for ADC_BUS3XREQ */
#define _ADC_BUSREQ_BUS3XREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS3XREQ_DEFAULT                     (_ADC_BUSREQ_BUS3XREQ_DEFAULT << 6) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS3YREQ                             (0x1UL << 7)                        /**< 1 if the bus connected to BUS3Y is requested */
#define _ADC_BUSREQ_BUS3YREQ_SHIFT                      7                                   /**< Shift value for ADC_BUS3YREQ */
#define _ADC_BUSREQ_BUS3YREQ_MASK                       0x80UL                              /**< Bit mask for ADC_BUS3YREQ */
#define _ADC_BUSREQ_BUS3YREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS3YREQ_DEFAULT                     (_ADC_BUSREQ_BUS3YREQ_DEFAULT << 7) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS4XREQ                             (0x1UL << 8)                        /**< 1 if the bus connected to BUS4X is requested */
#define _ADC_BUSREQ_BUS4XREQ_SHIFT                      8                                   /**< Shift value for ADC_BUS4XREQ */
#define _ADC_BUSREQ_BUS4XREQ_MASK                       0x100UL                             /**< Bit mask for ADC_BUS4XREQ */
#define _ADC_BUSREQ_BUS4XREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS4XREQ_DEFAULT                     (_ADC_BUSREQ_BUS4XREQ_DEFAULT << 8) /**< Shifted mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS4YREQ                             (0x1UL << 9)                        /**< 1 if the bus connected to BUS4Y is requested */
#define _ADC_BUSREQ_BUS4YREQ_SHIFT                      9                                   /**< Shift value for ADC_BUS4YREQ */
#define _ADC_BUSREQ_BUS4YREQ_MASK                       0x200UL                             /**< Bit mask for ADC_BUS4YREQ */
#define _ADC_BUSREQ_BUS4YREQ_DEFAULT                    0x00000000UL                        /**< Mode DEFAULT for ADC_BUSREQ */
#define ADC_BUSREQ_BUS4YREQ_DEFAULT                     (_ADC_BUSREQ_BUS4YREQ_DEFAULT << 9) /**< Shifted mode DEFAULT for ADC_BUSREQ */

/* Bit fields for ADC BUSCONFLICT */
#define _ADC_BUSCONFLICT_RESETVALUE                     0x00000000UL                                  /**< Default value for ADC_BUSCONFLICT */
#define _ADC_BUSCONFLICT_MASK                           0x000003FFUL                                  /**< Mask for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS0XCONFLICT                   (0x1UL << 0)                                  /**< 1 if the bus connected to BUS0X is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS0XCONFLICT_SHIFT            0                                             /**< Shift value for ADC_BUS0XCONFLICT */
#define _ADC_BUSCONFLICT_BUS0XCONFLICT_MASK             0x1UL                                         /**< Bit mask for ADC_BUS0XCONFLICT */
#define _ADC_BUSCONFLICT_BUS0XCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS0XCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS0XCONFLICT_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS0YCONFLICT                   (0x1UL << 1)                                  /**< 1 if the bus connected to BUS0Y is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS0YCONFLICT_SHIFT            1                                             /**< Shift value for ADC_BUS0YCONFLICT */
#define _ADC_BUSCONFLICT_BUS0YCONFLICT_MASK             0x2UL                                         /**< Bit mask for ADC_BUS0YCONFLICT */
#define _ADC_BUSCONFLICT_BUS0YCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS0YCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS0YCONFLICT_DEFAULT << 1) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS1XCONFLICT                   (0x1UL << 2)                                  /**< 1 if the bus connected to BUS1X is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS1XCONFLICT_SHIFT            2                                             /**< Shift value for ADC_BUS1XCONFLICT */
#define _ADC_BUSCONFLICT_BUS1XCONFLICT_MASK             0x4UL                                         /**< Bit mask for ADC_BUS1XCONFLICT */
#define _ADC_BUSCONFLICT_BUS1XCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS1XCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS1XCONFLICT_DEFAULT << 2) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS1YCONFLICT                   (0x1UL << 3)                                  /**< 1 if the bus connected to BUS1X is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS1YCONFLICT_SHIFT            3                                             /**< Shift value for ADC_BUS1YCONFLICT */
#define _ADC_BUSCONFLICT_BUS1YCONFLICT_MASK             0x8UL                                         /**< Bit mask for ADC_BUS1YCONFLICT */
#define _ADC_BUSCONFLICT_BUS1YCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS1YCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS1YCONFLICT_DEFAULT << 3) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS2XCONFLICT                   (0x1UL << 4)                                  /**< 1 if the bus connected to BUS2X is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS2XCONFLICT_SHIFT            4                                             /**< Shift value for ADC_BUS2XCONFLICT */
#define _ADC_BUSCONFLICT_BUS2XCONFLICT_MASK             0x10UL                                        /**< Bit mask for ADC_BUS2XCONFLICT */
#define _ADC_BUSCONFLICT_BUS2XCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS2XCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS2XCONFLICT_DEFAULT << 4) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS2YCONFLICT                   (0x1UL << 5)                                  /**< 1 if the bus connected to BUS2Y is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS2YCONFLICT_SHIFT            5                                             /**< Shift value for ADC_BUS2YCONFLICT */
#define _ADC_BUSCONFLICT_BUS2YCONFLICT_MASK             0x20UL                                        /**< Bit mask for ADC_BUS2YCONFLICT */
#define _ADC_BUSCONFLICT_BUS2YCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS2YCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS2YCONFLICT_DEFAULT << 5) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS3XCONFLICT                   (0x1UL << 6)                                  /**< 1 if the bus connected to BUS3X is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS3XCONFLICT_SHIFT            6                                             /**< Shift value for ADC_BUS3XCONFLICT */
#define _ADC_BUSCONFLICT_BUS3XCONFLICT_MASK             0x40UL                                        /**< Bit mask for ADC_BUS3XCONFLICT */
#define _ADC_BUSCONFLICT_BUS3XCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS3XCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS3XCONFLICT_DEFAULT << 6) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS3YCONFLICT                   (0x1UL << 7)                                  /**< 1 if the bus connected to BUS3Y is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS3YCONFLICT_SHIFT            7                                             /**< Shift value for ADC_BUS3YCONFLICT */
#define _ADC_BUSCONFLICT_BUS3YCONFLICT_MASK             0x80UL                                        /**< Bit mask for ADC_BUS3YCONFLICT */
#define _ADC_BUSCONFLICT_BUS3YCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS3YCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS3YCONFLICT_DEFAULT << 7) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS4XCONFLICT                   (0x1UL << 8)                                  /**< 1 if the bus connected to BUS4X is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS4XCONFLICT_SHIFT            8                                             /**< Shift value for ADC_BUS4XCONFLICT */
#define _ADC_BUSCONFLICT_BUS4XCONFLICT_MASK             0x100UL                                       /**< Bit mask for ADC_BUS4XCONFLICT */
#define _ADC_BUSCONFLICT_BUS4XCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS4XCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS4XCONFLICT_DEFAULT << 8) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS4YCONFLICT                   (0x1UL << 9)                                  /**< 1 if the bus connected to BUS4Y is in conflict with another peripheral */
#define _ADC_BUSCONFLICT_BUS4YCONFLICT_SHIFT            9                                             /**< Shift value for ADC_BUS4YCONFLICT */
#define _ADC_BUSCONFLICT_BUS4YCONFLICT_MASK             0x200UL                                       /**< Bit mask for ADC_BUS4YCONFLICT */
#define _ADC_BUSCONFLICT_BUS4YCONFLICT_DEFAULT          0x00000000UL                                  /**< Mode DEFAULT for ADC_BUSCONFLICT */
#define ADC_BUSCONFLICT_BUS4YCONFLICT_DEFAULT           (_ADC_BUSCONFLICT_BUS4YCONFLICT_DEFAULT << 9) /**< Shifted mode DEFAULT for ADC_BUSCONFLICT */

/* Bit fields for ADC SINGLEFIFOCOUNT */
#define _ADC_SINGLEFIFOCOUNT_RESETVALUE                 0x00000000UL                                 /**< Default value for ADC_SINGLEFIFOCOUNT */
#define _ADC_SINGLEFIFOCOUNT_MASK                       0x00000007UL                                 /**< Mask for ADC_SINGLEFIFOCOUNT */
#define _ADC_SINGLEFIFOCOUNT_SINGLEDC_SHIFT             0                                            /**< Shift value for ADC_SINGLEDC */
#define _ADC_SINGLEFIFOCOUNT_SINGLEDC_MASK              0x7UL                                        /**< Bit mask for ADC_SINGLEDC */
#define _ADC_SINGLEFIFOCOUNT_SINGLEDC_DEFAULT           0x00000000UL                                 /**< Mode DEFAULT for ADC_SINGLEFIFOCOUNT */
#define ADC_SINGLEFIFOCOUNT_SINGLEDC_DEFAULT            (_ADC_SINGLEFIFOCOUNT_SINGLEDC_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SINGLEFIFOCOUNT */

/* Bit fields for ADC SCANFIFOCOUNT */
#define _ADC_SCANFIFOCOUNT_RESETVALUE                   0x00000000UL                             /**< Default value for ADC_SCANFIFOCOUNT */
#define _ADC_SCANFIFOCOUNT_MASK                         0x00000007UL                             /**< Mask for ADC_SCANFIFOCOUNT */
#define _ADC_SCANFIFOCOUNT_SCANDC_SHIFT                 0                                        /**< Shift value for ADC_SCANDC */
#define _ADC_SCANFIFOCOUNT_SCANDC_MASK                  0x7UL                                    /**< Bit mask for ADC_SCANDC */
#define _ADC_SCANFIFOCOUNT_SCANDC_DEFAULT               0x00000000UL                             /**< Mode DEFAULT for ADC_SCANFIFOCOUNT */
#define ADC_SCANFIFOCOUNT_SCANDC_DEFAULT                (_ADC_SCANFIFOCOUNT_SCANDC_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SCANFIFOCOUNT */

/* Bit fields for ADC SINGLEFIFOCLEAR */
#define _ADC_SINGLEFIFOCLEAR_RESETVALUE                 0x00000000UL                                        /**< Default value for ADC_SINGLEFIFOCLEAR */
#define _ADC_SINGLEFIFOCLEAR_MASK                       0x00000001UL                                        /**< Mask for ADC_SINGLEFIFOCLEAR */
#define ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR             (0x1UL << 0)                                        /**< Clear Single FIFO content */
#define _ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR_SHIFT      0                                                   /**< Shift value for ADC_SINGLEFIFOCLEAR */
#define _ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR_MASK       0x1UL                                               /**< Bit mask for ADC_SINGLEFIFOCLEAR */
#define _ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR_DEFAULT    0x00000000UL                                        /**< Mode DEFAULT for ADC_SINGLEFIFOCLEAR */
#define ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR_DEFAULT     (_ADC_SINGLEFIFOCLEAR_SINGLEFIFOCLEAR_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SINGLEFIFOCLEAR */

/* Bit fields for ADC SCANFIFOCLEAR */
#define _ADC_SCANFIFOCLEAR_RESETVALUE                   0x00000000UL                                    /**< Default value for ADC_SCANFIFOCLEAR */
#define _ADC_SCANFIFOCLEAR_MASK                         0x00000001UL                                    /**< Mask for ADC_SCANFIFOCLEAR */
#define ADC_SCANFIFOCLEAR_SCANFIFOCLEAR                 (0x1UL << 0)                                    /**< Clear Scan FIFO content */
#define _ADC_SCANFIFOCLEAR_SCANFIFOCLEAR_SHIFT          0                                               /**< Shift value for ADC_SCANFIFOCLEAR */
#define _ADC_SCANFIFOCLEAR_SCANFIFOCLEAR_MASK           0x1UL                                           /**< Bit mask for ADC_SCANFIFOCLEAR */
#define _ADC_SCANFIFOCLEAR_SCANFIFOCLEAR_DEFAULT        0x00000000UL                                    /**< Mode DEFAULT for ADC_SCANFIFOCLEAR */
#define ADC_SCANFIFOCLEAR_SCANFIFOCLEAR_DEFAULT         (_ADC_SCANFIFOCLEAR_SCANFIFOCLEAR_DEFAULT << 0) /**< Shifted mode DEFAULT for ADC_SCANFIFOCLEAR */

/** @} End of group EFM32PG1B_ADC */


