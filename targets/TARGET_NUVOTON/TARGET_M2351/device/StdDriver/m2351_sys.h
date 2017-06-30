/**************************************************************************//**
 * @file     SYS.h
 * @version  V3
 * @brief    M2351 series System Manager (SYS) driver header file
 *
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __SYS_H__
#define __SYS_H__


#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup SYS_Driver SYS Driver
  @{
*/

/** @addtogroup SYS_EXPORTED_CONSTANTS SYS Exported Constants
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Module Reset Control Resister constant definitions.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PDMA0_RST   ((0x0<<24)|SYS_IPRST0_PDMA0RST_Pos)     /*!< PDMA0 reset is one of the SYS_ResetModule parameter */
#define EBI_RST     ((0x0<<24)|SYS_IPRST0_EBIRST_Pos)       /*!< EBI reset is one of the SYS_ResetModule parameter */
#define USBH_RST    ((0x0<<24)|SYS_IPRST0_USBHRST_Pos)      /*!< USBH reset is one of the SYS_ResetModule parameter */
#define SDH0_RST    ((0x0<<24)|SYS_IPRST0_SDH0RST_Pos)      /*!< SDH0 reset is one of the SYS_ResetModule parameter */
#define CRC_RST     ((0x0<<24)|SYS_IPRST0_CRCRST_Pos)       /*!< CRC reset is one of the SYS_ResetModule parameter */
#define CRPT_RST    ((0x0<<24)|SYS_IPRST0_CRPTRST_Pos)      /*!< CRPT reset is one of the SYS_ResetModule parameter */
#define PDMA1_RST   ((0x0<<24)|SYS_IPRST0_PDMA1RST_Pos)     /*!< PDMA1 reset is one of the SYS_ResetModule parameter */

#define GPIO_RST    ((0x4<<24)|SYS_IPRST1_GPIORST_Pos)      /*!< GPIO reset is one of the SYS_ResetModule parameter */
#define TMR0_RST    ((0x4<<24)|SYS_IPRST1_TMR0RST_Pos)      /*!< TMR0 reset is one of the SYS_ResetModule parameter */
#define TMR1_RST    ((0x4<<24)|SYS_IPRST1_TMR1RST_Pos)      /*!< TMR1 reset is one of the SYS_ResetModule parameter */
#define TMR2_RST    ((0x4<<24)|SYS_IPRST1_TMR2RST_Pos)      /*!< TMR2 reset is one of the SYS_ResetModule parameter */
#define TMR3_RST    ((0x4<<24)|SYS_IPRST1_TMR3RST_Pos)      /*!< TMR3 reset is one of the SYS_ResetModule parameter */
#define ACMP01_RST  ((0x4<<24)|SYS_IPRST1_ACMP01RST_Pos)    /*!< ACMP01 reset is one of the SYS_ResetModule parameter */
#define I2C0_RST    ((0x4<<24)|SYS_IPRST1_I2C0RST_Pos)      /*!< I2C0 reset is one of the SYS_ResetModule parameter */
#define I2C1_RST    ((0x4<<24)|SYS_IPRST1_I2C1RST_Pos)      /*!< I2C1 reset is one of the SYS_ResetModule parameter */
#define I2C2_RST    ((0x4<<24)|SYS_IPRST1_I2C2RST_Pos)      /*!< I2C2 reset is one of the SYS_ResetModule parameter */
#define SPI0_RST    ((0x4<<24)|SYS_IPRST1_SPI0RST_Pos)      /*!< SPI0 reset is one of the SYS_ResetModule parameter */
#define SPI1_RST    ((0x4<<24)|SYS_IPRST1_SPI1RST_Pos)      /*!< SPI1 reset is one of the SYS_ResetModule parameter */
#define SPI2_RST    ((0x4<<24)|SYS_IPRST1_SPI2RST_Pos)      /*!< SPI2 reset is one of the SYS_ResetModule parameter */
#define SPI3_RST    ((0x4<<24)|SYS_IPRST1_SPI3RST_Pos)      /*!< SPI3 reset is one of the SYS_ResetModule parameter */
#define UART0_RST   ((0x4<<24)|SYS_IPRST1_UART0RST_Pos)     /*!< UART0 reset is one of the SYS_ResetModule parameter */
#define UART1_RST   ((0x4<<24)|SYS_IPRST1_UART1RST_Pos)     /*!< UART1 reset is one of the SYS_ResetModule parameter */
#define UART2_RST   ((0x4<<24)|SYS_IPRST1_UART2RST_Pos)     /*!< UART2 reset is one of the SYS_ResetModule parameter */
#define UART3_RST   ((0x4<<24)|SYS_IPRST1_UART3RST_Pos)     /*!< UART3 reset is one of the SYS_ResetModule parameter */
#define UART4_RST   ((0x4<<24)|SYS_IPRST1_UART4RST_Pos)     /*!< UART4 reset is one of the SYS_ResetModule parameter */
#define UART5_RST   ((0x4<<24)|SYS_IPRST1_UART5RST_Pos)     /*!< UART5 reset is one of the SYS_ResetModule parameter */
#define DSRC_RST    ((0x4<<24)|SYS_IPRST1_DSRCRST_Pos)      /*!< DSRC reset is one of the SYS_ResetModule parameter */
#define CAN0_RST    ((0x4<<24)|SYS_IPRST1_CAN0RST_Pos)      /*!< CAN0 reset is one of the SYS_ResetModule parameter */
#define OTG_RST     ((0x4<<24)|SYS_IPRST1_OTGRST_Pos)       /*!< OTG reset is one of the SYS_ResetModule parameter */
#define USBD_RST    ((0x4<<24)|SYS_IPRST1_USBDRST_Pos)      /*!< USBD reset is one of the SYS_ResetModule parameter */
#define EADC_RST    ((0x4<<24)|SYS_IPRST1_EADCRST_Pos)      /*!< EADC reset is one of the SYS_ResetModule parameter */
#define I2S0_RST    ((0x4<<24)|SYS_IPRST1_I2S0RST_Pos)      /*!< I2S0 reset is one of the SYS_ResetModule parameter */
#define TRNG_RST    ((0x4<<24)|SYS_IPRST1_TRNGRST_Pos)      /*!< TRNG reset is one of the SYS_ResetModule parameter */

#define SC0_RST     ((0x8<<24)|SYS_IPRST2_SC0RST_Pos)       /*!< SC0 reset is one of the SYS_ResetModule parameter */
#define SC1_RST     ((0x8<<24)|SYS_IPRST2_SC1RST_Pos)       /*!< SC1 reset is one of the SYS_ResetModule parameter */
#define SC2_RST     ((0x8<<24)|SYS_IPRST2_SC2RST_Pos)       /*!< SC2 reset is one of the SYS_ResetModule parameter */
#define SPI4_RST    ((0x8<<24)|SYS_IPRST2_SPI4RST_Pos)      /*!< SPI4 reset is one of the SYS_ResetModule parameter */
#define SPI5_RST    ((0x8<<24)|SYS_IPRST2_SPI5RST_Pos)      /*!< SPI5 reset is one of the SYS_ResetModule parameter */
#define USCI0_RST   ((0x8<<24)|SYS_IPRST2_USCI0RST_Pos)     /*!< USCI0 reset is one of the SYS_ResetModule parameter */
#define USCI1_RST   ((0x8<<24)|SYS_IPRST2_USCI1RST_Pos)     /*!< USCI1 reset is one of the SYS_ResetModule parameter */
#define USCI2_RST   ((0x8<<24)|SYS_IPRST2_USCI2RST_Pos)     /*!< USCI2 reset is one of the SYS_ResetModule parameter */
#define DAC_RST     ((0x8<<24)|SYS_IPRST2_DACRST_Pos)       /*!< DAC reset is one of the SYS_ResetModule parameter */
#define PWM0_RST    ((0x8<<24)|SYS_IPRST2_PWM0RST_Pos)      /*!< PWM0 reset is one of the SYS_ResetModule parameter */
#define PWM1_RST    ((0x8<<24)|SYS_IPRST2_PWM1RST_Pos)      /*!< PWM1 reset is one of the SYS_ResetModule parameter */
#define BPWM0_RST   ((0x8<<24)|SYS_IPRST2_BPWM0RST_Pos)     /*!< BPWM0 reset is one of the SYS_ResetModule parameter */
#define BPWM1_RST   ((0x8<<24)|SYS_IPRST2_BPWM1RST_Pos)     /*!< BPWM1 reset is one of the SYS_ResetModule parameter */
#define QEI0_RST    ((0x8<<24)|SYS_IPRST2_QEI0RST_Pos)      /*!< QEI0 reset is one of the SYS_ResetModule parameter */
#define QEI1_RST    ((0x8<<24)|SYS_IPRST2_QEI1RST_Pos)      /*!< QEI1 reset is one of the SYS_ResetModule parameter */
#define ECAP0_RST   ((0x8<<24)|SYS_IPRST2_ECAP0RST_Pos)     /*!< ECAP0 reset is one of the SYS_ResetModule parameter */
#define ECAP1_RST   ((0x8<<24)|SYS_IPRST2_ECAP1RST_Pos)     /*!< ECAP1 reset is one of the SYS_ResetModule parameter */


/*---------------------------------------------------------------------------------------------------------*/
/*  Brown Out Detector Threshold Voltage Selection constant definitions.                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_BODCTL_BOD_RST_EN           (1UL<<SYS_BODCTL_BODRSTEN_Pos)    /*!< Brown-out Reset Enable */
#define SYS_BODCTL_BOD_INTERRUPT_EN     (0UL<<SYS_BODCTL_BODRSTEN_Pos)    /*!< Brown-out Interrupt Enable */
#define SYS_BODCTL_BODVL_3_0V           (7UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 3.0V */
#define SYS_BODCTL_BODVL_2_8V           (6UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.8V */
#define SYS_BODCTL_BODVL_2_6V           (5UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.6V */
#define SYS_BODCTL_BODVL_2_4V           (4UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.4V */
#define SYS_BODCTL_BODVL_2_2V           (3UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.2V */
#define SYS_BODCTL_BODVL_2_0V           (2UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.0V */
#define SYS_BODCTL_BODVL_1_8V           (1UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 1.8V */
#define SYS_BODCTL_BODVL_1_6V           (0UL<<SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 1.6V */


/*---------------------------------------------------------------------------------------------------------*/
/*  VREFCTL constant definitions. (Write-Protection Register)                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_VREFCTL_VREF_PIN        (0x0UL<<SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = Vref pin */
#define SYS_VREFCTL_VREF_1_6V       (0x3UL<<SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 1.6V */
#define SYS_VREFCTL_VREF_2_0V       (0x7UL<<SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 2.0V */
#define SYS_VREFCTL_VREF_2_5V       (0xBUL<<SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 2.5V */
#define SYS_VREFCTL_VREF_3_0V       (0xFUL<<SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 3.0V */
#define SYS_VREFCTL_VREF_AVDD       (0x10UL<<SYS_VREFCTL_VREFCTL_Pos)   /*!< Vref = AVDD */


/*---------------------------------------------------------------------------------------------------------*/
/*  USBPHY constant definitions. (Write-Protection Register)                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_USBPHY_USBROLE_STD_USBD   (0x0UL<<SYS_USBPHY_USBROLE_Pos)   /*!< Standard USB device */
#define SYS_USBPHY_USBROLE_STD_USBH   (0x1UL<<SYS_USBPHY_USBROLE_Pos)   /*!< Standard USB host */
#define SYS_USBPHY_USBROLE_ID_DEPH    (0x2UL<<SYS_USBPHY_USBROLE_Pos)   /*!< ID dependent device */
#define SYS_USBPHY_USBROLE_ON_THE_GO  (0x3UL << SYS_USBPHY_USBROLE_Pos)   /*!<  On-The-Go device */


/*---------------------------------------------------------------------------------------------------------*/
/*  Multi-Function constant definitions.                                                                   */
/*---------------------------------------------------------------------------------------------------------*/

/* How to use below #define?

Example: If user want to set PA.2 as UART0_TXD and PA.3 as UART0_RXD in initial function,
         user can issue following command to achieve it.

         SYS->GPA_MFPL = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA2MFP_Msk)) | SYS_GPA_MFPL_PA2MFP_UART0_TXD;
         SYS->GPA_MFPL = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA3MFP_Msk)) | SYS_GPA_MFPL_PA3MFP_UART0_RXD;
*/

//PA.0 MFP
#define SYS_GPA_MFPL_PA0MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for GPIO             */
#define SYS_GPA_MFPL_PA0MFP_UART1_nCTS      (0x1UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for UART1_nCTS       */
#define SYS_GPA_MFPL_PA0MFP_I2C2_SCL        (0x2UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for I2C2_SCL         */
#define SYS_GPA_MFPL_PA0MFP_UART1_TXD       (0x3UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for UART1_TXD        */
#define SYS_GPA_MFPL_PA0MFP_CAN0_RXD        (0x4UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for CAN0_RXD         */
#define SYS_GPA_MFPL_PA0MFP_SC0_CLK         (0x5UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for SC0_CLK          */
#define SYS_GPA_MFPL_PA0MFP_PWM1_CH5        (0x6UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for PWM1_CH5         */
#define SYS_GPA_MFPL_PA0MFP_EBI_AD0         (0x7UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for EBI_AD0          */
#define SYS_GPA_MFPL_PA0MFP_INT0            (0x8UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for INT0             */
#define SYS_GPA_MFPL_PA0MFP_SPI3_MISO       (0x9UL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for SPI3_MISO        */
#define SYS_GPA_MFPL_PA0MFP_SPI1_I2SMCLK    (0xAUL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for SPI1_I2SMCLK     */
#define SYS_GPA_MFPL_PA0MFP_USCI1_CTL0      (0xCUL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for USCI1_CTL0       */
#define SYS_GPA_MFPL_PA0MFP_USB_VBUS_EN     (0xDUL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for USB_VBUS_EN      */
#define SYS_GPA_MFPL_PA0MFP_SPI5_MISO       (0xFUL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for SPI5_MISO        */
#define SYS_GPA_MFPL_PA0MFP_ECAP0_IC0       (0xEUL<<SYS_GPA_MFPL_PA0MFP_Pos)    /*!< GPA_MFPL PA0 setting for ECAP0_IC0        */

//PA.1 MFP
#define SYS_GPA_MFPL_PA1MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for GPIO             */
#define SYS_GPA_MFPL_PA1MFP_UART1_nRTS      (0x1UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for UART1_nRTS       */
#define SYS_GPA_MFPL_PA1MFP_I2C2_SDA        (0x2UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for I2C2_SDA         */
#define SYS_GPA_MFPL_PA1MFP_UART1_RXD       (0x3UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for UART1_RXD        */
#define SYS_GPA_MFPL_PA1MFP_CAN0_TXD        (0x4UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for CAN0_TXD         */
#define SYS_GPA_MFPL_PA1MFP_SC0_DAT         (0x5UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for SC0_DAT          */
#define SYS_GPA_MFPL_PA1MFP_PWM1_CH4        (0x6UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for PWM1_CH4         */
#define SYS_GPA_MFPL_PA1MFP_EBI_AD1         (0x7UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for EBI_AD1          */
#define SYS_GPA_MFPL_PA1MFP_SPI3_CLK        (0x9UL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for SPI3_CLK         */
#define SYS_GPA_MFPL_PA1MFP_EADC0_ST        (0xAUL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for EADC0_ST         */
#define SYS_GPA_MFPL_PA1MFP_USCI1_DAT1      (0xCUL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for USCI1_DAT1       */
#define SYS_GPA_MFPL_PA1MFP_ECAP0_IC1       (0xEUL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for ECAP0_IC1        */
#define SYS_GPA_MFPL_PA1MFP_SPI5_MOSI       (0xFUL<<SYS_GPA_MFPL_PA1MFP_Pos)    /*!< GPA_MFPL PA1 setting for SPI5_MOSI        */

//PA.2 MFP
#define SYS_GPA_MFPL_PA2MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for GPIO             */
#define SYS_GPA_MFPL_PA2MFP_UART0_TXD       (0x2UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for UART0_TXD        */
#define SYS_GPA_MFPL_PA2MFP_UART0_nCTS      (0x3UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for UART0_nCTS       */
#define SYS_GPA_MFPL_PA2MFP_I2C0_SDA        (0x4UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for I2C0_SDA         */
#define SYS_GPA_MFPL_PA2MFP_SC0_RST         (0x5UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for SC0_RST          */
#define SYS_GPA_MFPL_PA2MFP_PWM1_CH3        (0x6UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for PWM1_CH3         */
#define SYS_GPA_MFPL_PA2MFP_EBI_AD2         (0x7UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for EBI_AD2          */
#define SYS_GPA_MFPL_PA2MFP_I2S0_MCLK       (0x8UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for I2S0_MCLK        */
#define SYS_GPA_MFPL_PA2MFP_SPI3_SS         (0x9UL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for SPI3_SS          */
#define SYS_GPA_MFPL_PA2MFP_USCI1_DAT0      (0xCUL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for USCI1_DAT0       */
#define SYS_GPA_MFPL_PA2MFP_BPWM1_CH3       (0xDUL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for BPWM1_CH3        */
#define SYS_GPA_MFPL_PA2MFP_ECAP0_IC2       (0xEUL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for ECAP0_IC2        */
#define SYS_GPA_MFPL_PA2MFP_SPI5_SS         (0xFUL<<SYS_GPA_MFPL_PA2MFP_Pos)    /*!< GPA_MFPL PA2 setting for SPI5_SS          */

//PA.3 MFP
#define SYS_GPA_MFPL_PA3MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for GPIO             */
#define SYS_GPA_MFPL_PA3MFP_UART0_RXD       (0x2UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for UART0_RXD        */
#define SYS_GPA_MFPL_PA3MFP_UART0_nRTS      (0x3UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for UART0_nRTS       */
#define SYS_GPA_MFPL_PA3MFP_I2C0_SCL        (0x4UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for I2C0_SCL         */
#define SYS_GPA_MFPL_PA3MFP_SC0_PWR         (0x5UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for SC0_PWR          */
#define SYS_GPA_MFPL_PA3MFP_PWM1_CH2        (0x6UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for PWM1_CH2         */
#define SYS_GPA_MFPL_PA3MFP_EBI_AD3         (0x7UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for EBI_AD3          */
#define SYS_GPA_MFPL_PA3MFP_SPI3_MOSI       (0x9UL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for SPI3_MOSI        */
#define SYS_GPA_MFPL_PA3MFP_USCI1_CLK       (0xCUL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for USCI1_CLK        */
#define SYS_GPA_MFPL_PA3MFP_BPWM1_CH2       (0xDUL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for BPWM1_CH2        */
#define SYS_GPA_MFPL_PA3MFP_SPI5_CLK        (0xFUL<<SYS_GPA_MFPL_PA3MFP_Pos)    /*!< GPA_MFPL PA3 setting for SPI5_CLK         */

//PA.4 MFP
#define SYS_GPA_MFPL_PA4MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA4MFP_Pos)    /*!< GPA_MFPL PA4 setting for GPIO             */
#define SYS_GPA_MFPL_PA4MFP_SPI1_SS         (0x2UL<<SYS_GPA_MFPL_PA4MFP_Pos)    /*!< GPA_MFPL PA4 setting for SPI1_SS          */
#define SYS_GPA_MFPL_PA4MFP_EBI_AD4         (0x7UL<<SYS_GPA_MFPL_PA4MFP_Pos)    /*!< GPA_MFPL PA4 setting for EBI_AD4          */
#define SYS_GPA_MFPL_PA4MFP_ECAP1_IC0       (0xEUL<<SYS_GPA_MFPL_PA4MFP_Pos)    /*!< GPA_MFPL PA4 setting for ECAP1_IC0        */

//PA.5 MFP
#define SYS_GPA_MFPL_PA5MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA5MFP_Pos)    /*!< GPA_MFPL PA5 setting for GPIO             */
#define SYS_GPA_MFPL_PA5MFP_SPI1_MOSI       (0x2UL<<SYS_GPA_MFPL_PA5MFP_Pos)    /*!< GPA_MFPL PA5 setting for SPI1_MOSI        */
#define SYS_GPA_MFPL_PA5MFP_TM2_EXT         (0x3UL<<SYS_GPA_MFPL_PA5MFP_Pos)    /*!< GPA_MFPL PA5 setting for TM2_EXT          */
#define SYS_GPA_MFPL_PA5MFP_EBI_AD5         (0x7UL<<SYS_GPA_MFPL_PA5MFP_Pos)    /*!< GPA_MFPL PA5 setting for EBI_AD5          */
#define SYS_GPA_MFPL_PA5MFP_ECAP1_IC1       (0xEUL<<SYS_GPA_MFPL_PA5MFP_Pos)    /*!< GPA_MFPL PA5 setting for ECAP1_IC1        */

//PA.6 MFP
#define SYS_GPA_MFPL_PA6MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA6MFP_Pos)    /*!< GPA_MFPL PA6 setting for GPIO             */
#define SYS_GPA_MFPL_PA6MFP_SPI1_MISO       (0x2UL<<SYS_GPA_MFPL_PA6MFP_Pos)    /*!< GPA_MFPL PA6 setting for SPI1_MISO        */
#define SYS_GPA_MFPL_PA6MFP_TM1_EXT         (0x3UL<<SYS_GPA_MFPL_PA6MFP_Pos)    /*!< GPA_MFPL PA6 setting for TM1_EXT          */
#define SYS_GPA_MFPL_PA6MFP_EBI_AD6         (0x7UL<<SYS_GPA_MFPL_PA6MFP_Pos)    /*!< GPA_MFPL PA6 setting for EBI_AD6          */
#define SYS_GPA_MFPL_PA6MFP_ECAP1_IC2       (0xEUL<<SYS_GPA_MFPL_PA6MFP_Pos)    /*!< GPA_MFPL PA6 setting for ECAP1_IC2        */

//PA.7 MFP
#define SYS_GPA_MFPL_PA7MFP_GPIO            (0x0UL<<SYS_GPA_MFPL_PA7MFP_Pos)    /*!< GPA_MFPL PA7 setting for GPIO             */
#define SYS_GPA_MFPL_PA7MFP_SPI1_CLK        (0x2UL<<SYS_GPA_MFPL_PA7MFP_Pos)    /*!< GPA_MFPL PA7 setting for SPI1_CLK         */
#define SYS_GPA_MFPL_PA7MFP_TM0_EXT         (0x3UL<<SYS_GPA_MFPL_PA7MFP_Pos)    /*!< GPA_MFPL PA7 setting for TM0_EXT          */
#define SYS_GPA_MFPL_PA7MFP_EBI_AD7         (0x7UL<<SYS_GPA_MFPL_PA7MFP_Pos)    /*!< GPA_MFPL PA7 setting for EBI_AD7          */
#define SYS_GPA_MFPL_PA7MFP_DSRC_WAKEUP     (0xCUL<<SYS_GPA_MFPL_PA7MFP_Pos)    /*!< GPA_MFPL PA7 setting for DSRC_WAKEUP      */

//PA.8 MFP
#define SYS_GPA_MFPH_PA8MFP_GPIO            (0x0UL<<SYS_GPA_MFPH_PA8MFP_Pos)    /*!< GPA_MFPH PA8 setting for GPIO             */
#define SYS_GPA_MFPH_PA8MFP_UART3_TXD       (0x3UL<<SYS_GPA_MFPH_PA8MFP_Pos)    /*!< GPA_MFPH PA8 setting for UART3_TXD        */
#define SYS_GPA_MFPH_PA8MFP_DSRC_TXON       (0xCUL<<SYS_GPA_MFPH_PA8MFP_Pos)    /*!< GPA_MFPH PA8 setting for DSRC_TXON        */

//PA.9 MFP
#define SYS_GPA_MFPH_PA9MFP_GPIO            (0x0UL<<SYS_GPA_MFPH_PA9MFP_Pos)    /*!< GPA_MFPH PA9 setting for GPIO             */
#define SYS_GPA_MFPH_PA9MFP_UART3_RXD       (0x3UL<<SYS_GPA_MFPH_PA9MFP_Pos)    /*!< GPA_MFPH PA9 setting for UART3_RXD        */
#define SYS_GPA_MFPH_PA9MFP_DSRC_RXON       (0xCUL<<SYS_GPA_MFPH_PA9MFP_Pos)    /*!< GPA_MFPH PA9 setting for DSRC_RXON        */

//PA.10 MFP
#define SYS_GPA_MFPH_PA10MFP_GPIO           (0x0UL<<SYS_GPA_MFPH_PA10MFP_Pos)   /*!< GPA_MFPH PA10 setting for GPIO            */
#define SYS_GPA_MFPH_PA10MFP_UART3_nCTS     (0x3UL<<SYS_GPA_MFPH_PA10MFP_Pos)   /*!< GPA_MFPH PA10 setting for UART3_nCTS      */

//PA.11 MFP
#define SYS_GPA_MFPH_PA11MFP_GPIO           (0x0UL<<SYS_GPA_MFPH_PA11MFP_Pos)   /*!< GPA_MFPH PA11 setting for GPIO            */
#define SYS_GPA_MFPH_PA11MFP_UART3_nRTS     (0x3UL<<SYS_GPA_MFPH_PA11MFP_Pos)   /*!< GPA_MFPH PA11 setting for UART3_nRTS      */

//PA.12 MFP
#define SYS_GPA_MFPH_PA12MFP_GPIO           (0x0UL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for GPIO            */
#define SYS_GPA_MFPH_PA12MFP_SPI1_I2SMCLK   (0x2UL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for SPI1_I2SMCLK    */
#define SYS_GPA_MFPH_PA12MFP_UART2_RXD      (0x3UL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for UART2_RXD       */
#define SYS_GPA_MFPH_PA12MFP_CAN0_TXD       (0x4UL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for CAN0_TXD        */
#define SYS_GPA_MFPH_PA12MFP_EBI_ADR8       (0x7UL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for EBI_ADR8        */
#define SYS_GPA_MFPH_PA12MFP_I2S0_DO        (0x8UL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for I2S0_DO         */
#define SYS_GPA_MFPH_PA12MFP_SC2_PWR        (0x9UL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for SC2_PWR         */
#define SYS_GPA_MFPH_PA12MFP_SPI3_I2SMCLK   (0xAUL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for SPI3_I2SMCLK    */
#define SYS_GPA_MFPH_PA12MFP_USB_VBUS_ST    (0xBUL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for USB_VBUS_ST     */
#define SYS_GPA_MFPH_PA12MFP_DSRC_WAKEUP    (0xCUL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for DSRC_WAKEUP     */
#define SYS_GPA_MFPH_PA12MFP_BPWM0_CH5      (0xDUL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for BPWM0_CH5       */
#define SYS_GPA_MFPH_PA12MFP_QEI1_INDEX     (0xEUL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for QEI1_INDEX      */
#define SYS_GPA_MFPH_PA12MFP_SPI2_I2SMCLK   (0xFUL<<SYS_GPA_MFPH_PA12MFP_Pos)   /*!< GPA_MFPH PA12 setting for SPI2_I2SMCLK    */

//PA.13 MFP
#define SYS_GPA_MFPH_PA13MFP_GPIO           (0x0UL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for GPIO            */
#define SYS_GPA_MFPH_PA13MFP_UART2_TXD      (0x3UL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for UART2_TXD       */
#define SYS_GPA_MFPH_PA13MFP_CAN0_RXD       (0x4UL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for CAN0_RXD        */
#define SYS_GPA_MFPH_PA13MFP_EBI_ADR7       (0x7UL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for EBI_ADR7        */
#define SYS_GPA_MFPH_PA13MFP_I2S0_DI        (0x8UL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for I2S0_DI         */
#define SYS_GPA_MFPH_PA13MFP_SC2_RST        (0x9UL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for SC2_RST         */
#define SYS_GPA_MFPH_PA13MFP_SPI1_CLK       (0xAUL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for SPI1_CLK        */
#define SYS_GPA_MFPH_PA13MFP_USB_VBUS_EN    (0xBUL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for USB_VBUS_EN     */
#define SYS_GPA_MFPH_PA13MFP_QEI1_A         (0xEUL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for QEI1_A          */
#define SYS_GPA_MFPH_PA13MFP_DSRC_TXON      (0xCUL<<SYS_GPA_MFPH_PA13MFP_Pos)   /*!< GPA_MFPH PA13 setting for DSRC_TXON       */

//PA.14 MFP
#define SYS_GPA_MFPH_PA14MFP_GPIO           (0x0UL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for GPIO            */
#define SYS_GPA_MFPH_PA14MFP_UART2_nCTS     (0x3UL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for UART2_nCTS      */
#define SYS_GPA_MFPH_PA14MFP_I2C0_SMBAL     (0x4UL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for I2C0_SMBAL      */
#define SYS_GPA_MFPH_PA14MFP_EBI_ADR6       (0x7UL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for EBI_ADR6        */
#define SYS_GPA_MFPH_PA14MFP_I2S0_BCLK      (0x8UL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for I2S0_BCLK       */
#define SYS_GPA_MFPH_PA14MFP_SC2_CLK        (0x9UL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for SC2_CLK         */
#define SYS_GPA_MFPH_PA14MFP_SPI1_MISO      (0xAUL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for SPI1_MISO       */
#define SYS_GPA_MFPH_PA14MFP_DSRC_RXON      (0xCUL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for DSRC_RXON       */
#define SYS_GPA_MFPH_PA14MFP_QEI1_B         (0xEUL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for QEI1_B          */
#define SYS_GPA_MFPH_PA14MFP_BPWM1_CH0      (0xDUL<<SYS_GPA_MFPH_PA14MFP_Pos)   /*!< GPA_MFPH PA14 setting for BPWM1_CH0       */

//PA.15 MFP
#define SYS_GPA_MFPH_PA15MFP_GPIO           (0x0UL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for GPIO            */
#define SYS_GPA_MFPH_PA15MFP_UART2_nRTS     (0x3UL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for UART2_nRTS      */
#define SYS_GPA_MFPH_PA15MFP_I2C0_SMBSUS    (0x4UL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for I2C0_SMBSUS     */
#define SYS_GPA_MFPH_PA15MFP_PWM1_CH0       (0x6UL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for PWM1_CH0        */
#define SYS_GPA_MFPH_PA15MFP_EBI_ADR5       (0x7UL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for EBI_ADR5        */
#define SYS_GPA_MFPH_PA15MFP_I2S0_LRCK      (0x8UL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for I2S0_LRCK       */
#define SYS_GPA_MFPH_PA15MFP_SC2_DAT        (0x9UL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for SC2_DAT         */
#define SYS_GPA_MFPH_PA15MFP_SPI1_MOSI      (0xAUL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for SPI1_MOSI       */
#define SYS_GPA_MFPH_PA15MFP_BPWM1_CH1      (0xDUL<<SYS_GPA_MFPH_PA15MFP_Pos)   /*!< GPA_MFPH PA15 setting for BPWM1_CH1       */

//PB.0 MFP
#define SYS_GPB_MFPL_PB0MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for GPIO             */
#define SYS_GPB_MFPL_PB0MFP_EADC0_CH0       (0x1UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for EADC0_CH0        */
#define SYS_GPB_MFPL_PB0MFP_SPI0_MOSI1      (0x2UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for SPI0_MOSI1       */
#define SYS_GPB_MFPL_PB0MFP_UART2_RXD       (0x3UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for UART2_RXD        */
#define SYS_GPB_MFPL_PB0MFP_TM2             (0x4UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for TM2              */
#define SYS_GPB_MFPL_PB0MFP_DAC0_OUT        (0x5UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for DAC0_OUT         */
#define SYS_GPB_MFPL_PB0MFP_PWM1_CH5        (0x6UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for PWM1_CH5         */
#define SYS_GPB_MFPL_PB0MFP_EBI_nWRL        (0x7UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for EBI_nWRL         */
#define SYS_GPB_MFPL_PB0MFP_INT1            (0x8UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for INT1             */
#define SYS_GPB_MFPL_PB0MFP_I2C2_SDA        (0x9UL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for I2C2_SDA         */
#define SYS_GPB_MFPL_PB0MFP_SC1_RST         (0xAUL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for SC1_RST          */
#define SYS_GPB_MFPL_PB0MFP_SC0_DAT         (0xBUL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for SC0_DAT          */
#define SYS_GPB_MFPL_PB0MFP_USCI1_CTL1      (0xCUL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for USCI1_CTL1       */
#define SYS_GPB_MFPL_PB0MFP_DSRC_WAKEUP     (0xDUL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for DSRC_WAKEUP      */
#define SYS_GPB_MFPL_PB0MFP_SPI1_SS         (0xEUL<<SYS_GPB_MFPL_PB0MFP_Pos)    /*!< GPB_MFPL PB0 setting for SPI1_SS          */

//PB.1 MFP
#define SYS_GPB_MFPL_PB1MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for GPIO             */
#define SYS_GPB_MFPL_PB1MFP_EADC0_CH1       (0x1UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for EADC0_CH1        */
#define SYS_GPB_MFPL_PB1MFP_SPI0_MISO1      (0x2UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for SPI0_MISO1       */
#define SYS_GPB_MFPL_PB1MFP_UART2_TXD       (0x3UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for UART2_TXD        */
#define SYS_GPB_MFPL_PB1MFP_TM3             (0x4UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for TM3              */
#define SYS_GPB_MFPL_PB1MFP_SC0_RST         (0x5UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for SC0_RST          */
#define SYS_GPB_MFPL_PB1MFP_PWM0_SYNC_OUT   (0x6UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for PWM0_SYNC_OUT    */
#define SYS_GPB_MFPL_PB1MFP_EBI_nWRH        (0x7UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for EBI_nWRH         */
#define SYS_GPB_MFPL_PB1MFP_DAC1_OUT        (0x8UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for DAC1_OUT         */
#define SYS_GPB_MFPL_PB1MFP_I2C2_SCL        (0x9UL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for I2C2_SCL         */
#define SYS_GPB_MFPL_PB1MFP_SC1_PWR         (0xAUL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for SC1_PWR          */
#define SYS_GPB_MFPL_PB1MFP_SC2_nCD         (0xBUL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for SC2_nCD          */
#define SYS_GPB_MFPL_PB1MFP_USCI1_DAT1      (0xCUL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for USCI1_DAT1       */
#define SYS_GPB_MFPL_PB1MFP_DSRC_TXON       (0xDUL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for DSRC_TXON        */
#define SYS_GPB_MFPL_PB1MFP_SPI1_MOSI       (0xEUL<<SYS_GPB_MFPL_PB1MFP_Pos)    /*!< GPB_MFPL PB1 setting for SPI1_MOSI        */

//PB.2 MFP
#define SYS_GPB_MFPL_PB2MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for GPIO             */
#define SYS_GPB_MFPL_PB2MFP_EADC0_CH2       (0x1UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for EADC0_CH2        */
#define SYS_GPB_MFPL_PB2MFP_SPI0_CLK        (0x2UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for SPI0_CLK         */
#define SYS_GPB_MFPL_PB2MFP_SPI1_CLK        (0x3UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for SPI1_CLK         */
#define SYS_GPB_MFPL_PB2MFP_UART1_RXD       (0x4UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for UART1_RXD        */
#define SYS_GPB_MFPL_PB2MFP_SC0_nCD         (0x5UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for SC0_nCD          */
#define SYS_GPB_MFPL_PB2MFP_PWM1_CH4        (0x6UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for PWM1_CH4         */
#define SYS_GPB_MFPL_PB2MFP_EBI_ADR16       (0x7UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for EBI_ADR16        */
#define SYS_GPB_MFPL_PB2MFP_I2C2_SMBAL      (0x8UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for I2C2_SMBAL       */
#define SYS_GPB_MFPL_PB2MFP_UART3_RXD       (0x9UL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for UART3_RXD        */
#define SYS_GPB_MFPL_PB2MFP_SC1_CLK         (0xAUL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for SC1_CLK          */
#define SYS_GPB_MFPL_PB2MFP_TM2_EXT         (0xBUL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for TM2_EXT          */
#define SYS_GPB_MFPL_PB2MFP_USCI1_DAT0      (0xCUL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for USCI1_DAT0       */
#define SYS_GPB_MFPL_PB2MFP_USB_VBUS_ST     (0xDUL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for USB_VBUS_ST      */
#define SYS_GPB_MFPL_PB2MFP_DSRC_RXON       (0xEUL<<SYS_GPB_MFPL_PB2MFP_Pos)    /*!< GPB_MFPL PB2 setting for DSRC_RXON        */

//PB.3 MFP
#define SYS_GPB_MFPL_PB3MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for GPIO             */
#define SYS_GPB_MFPL_PB3MFP_EADC0_CH3       (0x1UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for EADC0_CH3        */
#define SYS_GPB_MFPL_PB3MFP_SPI0_MISO0      (0x2UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for SPI0_MISO0       */
#define SYS_GPB_MFPL_PB3MFP_SPI1_MISO       (0x3UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for SPI1_MISO        */
#define SYS_GPB_MFPL_PB3MFP_UART1_TXD       (0x4UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for UART1_TXD        */
#define SYS_GPB_MFPL_PB3MFP_PWM0_BRAKE0     (0x5UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for PWM0_BRAKE0      */
#define SYS_GPB_MFPL_PB3MFP_PWM1_CH3        (0x6UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for PWM1_CH3         */
#define SYS_GPB_MFPL_PB3MFP_EBI_ADR17       (0x7UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for EBI_ADR17        */
#define SYS_GPB_MFPL_PB3MFP_I2C2_SMBSUS     (0x8UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for I2C2_SMBSUS      */
#define SYS_GPB_MFPL_PB3MFP_UART3_TXD       (0x9UL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for UART3_TXD        */
#define SYS_GPB_MFPL_PB3MFP_SC1_nCD         (0xAUL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for SC1_nCD          */
#define SYS_GPB_MFPL_PB3MFP_TM0_EXT         (0xBUL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for TM0_EXT          */
#define SYS_GPB_MFPL_PB3MFP_USCI1_DAT1      (0xCUL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for USCI1_DAT1       */
#define SYS_GPB_MFPL_PB3MFP_USB_VBUS_EN     (0xDUL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for USB_VBUS_EN      */
#define SYS_GPB_MFPL_PB3MFP_ACMP0_WLAT      (0xEUL<<SYS_GPB_MFPL_PB3MFP_Pos)    /*!< GPB_MFPL PB3 setting for ACMP0_WLAT       */

//PB.4 MFP
#define SYS_GPB_MFPL_PB4MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for GPIO             */
#define SYS_GPB_MFPL_PB4MFP_EADC0_CH4       (0x1UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for EADC0_CH4        */
#define SYS_GPB_MFPL_PB4MFP_SPI0_SS         (0x2UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for SPI0_SS          */
#define SYS_GPB_MFPL_PB4MFP_SPI1_SS         (0x3UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for SPI1_SS          */
#define SYS_GPB_MFPL_PB4MFP_UART1_nCTS      (0x4UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for UART1_nCTS       */
#define SYS_GPB_MFPL_PB4MFP_ACMP0_N         (0x5UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for ACMP0_N          */
#define SYS_GPB_MFPL_PB4MFP_PWM1_CH2        (0x6UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for PWM1_CH2         */
#define SYS_GPB_MFPL_PB4MFP_EBI_AD7         (0x7UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for EBI_AD7          */
#define SYS_GPB_MFPL_PB4MFP_UART4_TXD       (0x8UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for UART4_TXD        */
#define SYS_GPB_MFPL_PB4MFP_UART2_TXD       (0x9UL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for UART2_TXD        */
#define SYS_GPB_MFPL_PB4MFP_SC1_DAT         (0xAUL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for SC1_DAT          */
#define SYS_GPB_MFPL_PB4MFP_TM1_EXT         (0xBUL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for TM1_EXT          */
#define SYS_GPB_MFPL_PB4MFP_USCI1_CLK       (0xCUL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for USCI1_CLK        */
#define SYS_GPB_MFPL_PB4MFP_SPI4_CLK        (0xDUL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for SPI4_CLK         */
#define SYS_GPB_MFPL_PB4MFP_ECAP1_IC0       (0xEUL<<SYS_GPB_MFPL_PB4MFP_Pos)    /*!< GPB_MFPL PB4 setting for ECAP1_IC0        */

//PB.5 MFP
#define SYS_GPB_MFPL_PB5MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for GPIO             */
#define SYS_GPB_MFPL_PB5MFP_EADC0_CH13      (0x1UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for EADC0_CH13       */
#define SYS_GPB_MFPL_PB5MFP_SPI0_MOSI0      (0x2UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for SPI0_MOSI0       */
#define SYS_GPB_MFPL_PB5MFP_SPI1_MOSI       (0x3UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for SPI1_MOSI        */
#define SYS_GPB_MFPL_PB5MFP_UART5_TXD       (0x4UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for UART5_TXD        */
#define SYS_GPB_MFPL_PB5MFP_ACMP0_P2        (0x5UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for ACMP0_P2         */
#define SYS_GPB_MFPL_PB5MFP_PWM0_BRAKE1     (0x6UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for PWM0_BRAKE1      */
#define SYS_GPB_MFPL_PB5MFP_EBI_AD6         (0x7UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for EBI_AD6          */
#define SYS_GPB_MFPL_PB5MFP_UART2_RXD       (0x9UL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for UART2_RXD        */
#define SYS_GPB_MFPL_PB5MFP_SC1_RST         (0xBUL<<SYS_GPB_MFPL_PB5MFP_Pos)    /*!< GPB_MFPL PB5 setting for SC1_RST          */

//PB.6 MFP
#define SYS_GPB_MFPL_PB6MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for GPIO             */
#define SYS_GPB_MFPL_PB6MFP_EADC0_CH14      (0x1UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for EADC0_CH14       */
#define SYS_GPB_MFPL_PB6MFP_SPI0_MISO0      (0x2UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for SPI0_MISO0       */
#define SYS_GPB_MFPL_PB6MFP_SPI1_MISO       (0x3UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for SPI1_MISO        */
#define SYS_GPB_MFPL_PB6MFP_UART4_TXD       (0x4UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for UART4_TXD        */
#define SYS_GPB_MFPL_PB6MFP_ACMP0_P1        (0x5UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for ACMP0_P1         */
#define SYS_GPB_MFPL_PB6MFP_PWM1_CH0        (0x6UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for PWM1_CH0         */
#define SYS_GPB_MFPL_PB6MFP_EBI_AD5         (0x7UL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for EBI_AD5          */
#define SYS_GPB_MFPL_PB6MFP_SC1_CLK         (0xBUL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for SC1_CLK          */
#define SYS_GPB_MFPL_PB6MFP_USCI0_DAT0      (0xCUL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for USCI0_DAT0       */
#define SYS_GPB_MFPL_PB6MFP_BPWM0_CH0       (0xDUL<<SYS_GPB_MFPL_PB6MFP_Pos)    /*!< GPB_MFPL PB6 setting for BPWM0_CH0        */

//PB.7 MFP
#define SYS_GPB_MFPL_PB7MFP_GPIO            (0x0UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for GPIO             */
#define SYS_GPB_MFPL_PB7MFP_EADC0_CH15      (0x1UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for EADC0_CH15       */
#define SYS_GPB_MFPL_PB7MFP_SPI0_CLK        (0x2UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for SPI0_CLK         */
#define SYS_GPB_MFPL_PB7MFP_SPI1_CLK        (0x3UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for SPI1_CLK         */
#define SYS_GPB_MFPL_PB7MFP_UART4_RXD       (0x4UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for UART4_RXD        */
#define SYS_GPB_MFPL_PB7MFP_ACMP0_P0        (0x5UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for ACMP0_P0         */
#define SYS_GPB_MFPL_PB7MFP_PWM1_CH1        (0x6UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for PWM1_CH1         */
#define SYS_GPB_MFPL_PB7MFP_EBI_AD4         (0x7UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for EBI_AD4          */
#define SYS_GPB_MFPL_PB7MFP_SC0_PWR         (0x9UL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for SC0_PWR          */
#define SYS_GPB_MFPL_PB7MFP_EADC0_ST        (0xAUL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for EADC0_ST         */
#define SYS_GPB_MFPL_PB7MFP_SC1_DAT         (0xBUL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for SC1_DAT          */
#define SYS_GPB_MFPL_PB7MFP_BPWM0_CH1       (0xDUL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for BPWM0_CH1        */
#define SYS_GPB_MFPL_PB7MFP_USCI0_DAT1      (0xCUL<<SYS_GPB_MFPL_PB7MFP_Pos)    /*!< GPB_MFPL PB7 setting for USCI0_DAT1       */

//PB.8 MFP
#define SYS_GPB_MFPH_PB8MFP_GPIO            (0x0UL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for GPIO             */
#define SYS_GPB_MFPH_PB8MFP_EADC0_CH5       (0x1UL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for EADC0_CH5        */
#define SYS_GPB_MFPH_PB8MFP_SPI4_SS         (0x2UL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for SPI4_SS          */
#define SYS_GPB_MFPH_PB8MFP_UART1_nRTS      (0x4UL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for UART1_nRTS       */
#define SYS_GPB_MFPH_PB8MFP_PWM0_CH2        (0x6UL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for PWM0_CH2         */
#define SYS_GPB_MFPH_PB8MFP_EBI_AD8         (0x7UL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for EBI_AD8          */
#define SYS_GPB_MFPH_PB8MFP_UART4_RXD       (0x8UL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for UART4_RXD        */
#define SYS_GPB_MFPH_PB8MFP_ECAP1_IC1       (0xEUL<<SYS_GPB_MFPH_PB8MFP_Pos)    /*!< GPB_MFPH PB8 setting for ECAP1_IC1        */

//PB.9 MFP
#define SYS_GPB_MFPH_PB9MFP_GPIO            (0x0UL<<SYS_GPB_MFPH_PB9MFP_Pos)    /*!< GPB_MFPH PB9 setting for GPIO             */
#define SYS_GPB_MFPH_PB9MFP_EADC0_CH6       (0x1UL<<SYS_GPB_MFPH_PB9MFP_Pos)    /*!< GPB_MFPH PB9 setting for EADC0_CH6        */
#define SYS_GPB_MFPH_PB9MFP_SPI4_I2SMCLK    (0x2UL<<SYS_GPB_MFPH_PB9MFP_Pos)    /*!< GPB_MFPH PB9 setting for SPI4_I2SMCLK     */
#define SYS_GPB_MFPH_PB9MFP_UART5_nRTS      (0x4UL<<SYS_GPB_MFPH_PB9MFP_Pos)    /*!< GPB_MFPH PB9 setting for UART5_nRTS       */
#define SYS_GPB_MFPH_PB9MFP_EBI_AD9         (0x7UL<<SYS_GPB_MFPH_PB9MFP_Pos)    /*!< GPB_MFPH PB9 setting for EBI_AD9          */

//PB.10 MFP
#define SYS_GPB_MFPH_PB10MFP_GPIO           (0x0UL<<SYS_GPB_MFPH_PB10MFP_Pos)   /*!< GPB_MFPH PB10 setting for GPIO            */
#define SYS_GPB_MFPH_PB10MFP_EADC0_CH7      (0x1UL<<SYS_GPB_MFPH_PB10MFP_Pos)   /*!< GPB_MFPH PB10 setting for EADC0_CH7       */
#define SYS_GPB_MFPH_PB10MFP_SPI4_CLK       (0x2UL<<SYS_GPB_MFPH_PB10MFP_Pos)   /*!< GPB_MFPH PB10 setting for SPI4_CLK        */
#define SYS_GPB_MFPH_PB10MFP_UART5_nCTS     (0x4UL<<SYS_GPB_MFPH_PB10MFP_Pos)   /*!< GPB_MFPH PB10 setting for UART5_nCTS      */
#define SYS_GPB_MFPH_PB10MFP_EBI_AD10       (0x7UL<<SYS_GPB_MFPH_PB10MFP_Pos)   /*!< GPB_MFPH PB10 setting for EBI_AD10        */

//PB.11 MFP
#define SYS_GPB_MFPH_PB11MFP_GPIO           (0x0UL<<SYS_GPB_MFPH_PB11MFP_Pos)   /*!< GPB_MFPH PB11 setting for GPIO            */
#define SYS_GPB_MFPH_PB11MFP_EADC0_CH8      (0x1UL<<SYS_GPB_MFPH_PB11MFP_Pos)   /*!< GPB_MFPH PB11 setting for EADC0_CH8       */
#define SYS_GPB_MFPH_PB11MFP_SPI4_MISO      (0x2UL<<SYS_GPB_MFPH_PB11MFP_Pos)   /*!< GPB_MFPH PB11 setting for SPI4_MISO       */
#define SYS_GPB_MFPH_PB11MFP_UART5_TXD      (0x4UL<<SYS_GPB_MFPH_PB11MFP_Pos)   /*!< GPB_MFPH PB11 setting for UART5_TXD       */
#define SYS_GPB_MFPH_PB11MFP_EBI_ADR18      (0x7UL<<SYS_GPB_MFPH_PB11MFP_Pos)   /*!< GPB_MFPH PB11 setting for EBI_ADR18       */
#define SYS_GPB_MFPH_PB11MFP_UART4_nCTS     (0x8UL<<SYS_GPB_MFPH_PB11MFP_Pos)   /*!< GPB_MFPH PB11 setting for UART4_nCTS      */
#define SYS_GPB_MFPH_PB11MFP_ECAP1_IC2      (0xEUL<<SYS_GPB_MFPH_PB11MFP_Pos)   /*!< GPB_MFPH PB11 setting for ECAP1_IC2       */

//PB.12 MFP
#define SYS_GPB_MFPH_PB12MFP_GPIO           (0x0UL<<SYS_GPB_MFPH_PB12MFP_Pos)   /*!< GPB_MFPH PB12 setting for GPIO            */
#define SYS_GPB_MFPH_PB12MFP_EADC0_CH9      (0x1UL<<SYS_GPB_MFPH_PB12MFP_Pos)   /*!< GPB_MFPH PB12 setting for EADC0_CH9       */
#define SYS_GPB_MFPH_PB12MFP_SPI4_MOSI      (0x2UL<<SYS_GPB_MFPH_PB12MFP_Pos)   /*!< GPB_MFPH PB12 setting for SPI4_MOSI       */
#define SYS_GPB_MFPH_PB12MFP_UART5_RXD      (0x4UL<<SYS_GPB_MFPH_PB12MFP_Pos)   /*!< GPB_MFPH PB12 setting for UART5_RXD       */
#define SYS_GPB_MFPH_PB12MFP_EBI_ADR19      (0x7UL<<SYS_GPB_MFPH_PB12MFP_Pos)   /*!< GPB_MFPH PB12 setting for EBI_ADR19       */
#define SYS_GPB_MFPH_PB12MFP_UART4_nRTS     (0x8UL<<SYS_GPB_MFPH_PB12MFP_Pos)   /*!< GPB_MFPH PB12 setting for UART4_nRTS      */

//PB.13 MFP
#define SYS_GPB_MFPH_PB13MFP_GPIO           (0x0UL<<SYS_GPB_MFPH_PB13MFP_Pos)   /*!< GPB_MFPH PB13 setting for GPIO            */
#define SYS_GPB_MFPH_PB13MFP_EADC0_CH10     (0x1UL<<SYS_GPB_MFPH_PB13MFP_Pos)   /*!< GPB_MFPH PB13 setting for EADC0_CH10      */
#define SYS_GPB_MFPH_PB13MFP_EBI_ADR9       (0x7UL<<SYS_GPB_MFPH_PB13MFP_Pos)   /*!< GPB_MFPH PB13 setting for EBI_ADR9        */

//PB.14 MFP
#define SYS_GPB_MFPH_PB14MFP_GPIO           (0x0UL<<SYS_GPB_MFPH_PB14MFP_Pos)   /*!< GPB_MFPH PB14 setting for GPIO            */
#define SYS_GPB_MFPH_PB14MFP_EADC0_CH11     (0x1UL<<SYS_GPB_MFPH_PB14MFP_Pos)   /*!< GPB_MFPH PB14 setting for EADC0_CH11      */
#define SYS_GPB_MFPH_PB14MFP_EBI_ADR8       (0x7UL<<SYS_GPB_MFPH_PB14MFP_Pos)   /*!< GPB_MFPH PB14 setting for EBI_ADR8        */

//PB.15 MFP
#define SYS_GPB_MFPH_PB15MFP_GPIO           (0x0UL<<SYS_GPB_MFPH_PB15MFP_Pos)   /*!< GPB_MFPH PB15 setting for GPIO            */
#define SYS_GPB_MFPH_PB15MFP_EADC0_CH12     (0x1UL<<SYS_GPB_MFPH_PB15MFP_Pos)   /*!< GPB_MFPH PB15 setting for EADC0_CH12      */
#define SYS_GPB_MFPH_PB15MFP_SPI1_SS        (0x3UL<<SYS_GPB_MFPH_PB15MFP_Pos)   /*!< GPB_MFPH PB15 setting for SPI1_SS         */
#define SYS_GPB_MFPH_PB15MFP_UART5_RXD      (0x4UL<<SYS_GPB_MFPH_PB15MFP_Pos)   /*!< GPB_MFPH PB15 setting for UART5_RXD       */
#define SYS_GPB_MFPH_PB15MFP_ACMP0_P3       (0x5UL<<SYS_GPB_MFPH_PB15MFP_Pos)   /*!< GPB_MFPH PB15 setting for ACMP0_P3        */
#define SYS_GPB_MFPH_PB15MFP_EBI_nCS1       (0x7UL<<SYS_GPB_MFPH_PB15MFP_Pos)   /*!< GPB_MFPH PB15 setting for EBI_nCS1        */
#define SYS_GPB_MFPH_PB15MFP_SC1_PWR        (0xBUL<<SYS_GPB_MFPH_PB15MFP_Pos)   /*!< GPB_MFPH PB15 setting for SC1_PWR         */

//PC.0 MFP
#define SYS_GPC_MFPL_PC0MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for GPIO             */
#define SYS_GPC_MFPL_PC0MFP_SPI0_MISO1      (0x1UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for SPI0_MISO1       */
#define SYS_GPC_MFPL_PC0MFP_SPI2_CLK        (0x2UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for SPI2_CLK         */
#define SYS_GPC_MFPL_PC0MFP_UART2_nCTS      (0x3UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for UART2_nCTS       */
#define SYS_GPC_MFPL_PC0MFP_CAN0_TXD        (0x4UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for CAN0_TXD         */
#define SYS_GPC_MFPL_PC0MFP_TM3_EXT         (0x5UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for TM3_EXT          */
#define SYS_GPC_MFPL_PC0MFP_PWM0_CH0        (0x6UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for PWM0_CH0         */
#define SYS_GPC_MFPL_PC0MFP_EBI_AD8         (0x7UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for EBI_AD8          */
#define SYS_GPC_MFPL_PC0MFP_INT2            (0x8UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for INT2             */
#define SYS_GPC_MFPL_PC0MFP_UART3_TXD       (0x9UL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for UART3_TXD        */
#define SYS_GPC_MFPL_PC0MFP_I2S0_LRCK       (0xAUL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for I2S0_LRCK        */
#define SYS_GPC_MFPL_PC0MFP_SD0_CLK         (0xBUL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for SD0_CLK          */
#define SYS_GPC_MFPL_PC0MFP_USCI0_CLK       (0xCUL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for USCI0_CLK        */
#define SYS_GPC_MFPL_PC0MFP_ETM_TRACE_CLK   (0xDUL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for ETM_TRACE_CLK    */
#define SYS_GPC_MFPL_PC0MFP_DSRC_TXON       (0xFUL<<SYS_GPC_MFPL_PC0MFP_Pos)    /*!< GPC_MFPL PC0 setting for DSRC_TXON        */

//PC.1 MFP
#define SYS_GPC_MFPL_PC1MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for GPIO             */
#define SYS_GPC_MFPL_PC1MFP_CLKO            (0x1UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for CLKO             */
#define SYS_GPC_MFPL_PC1MFP_DAC0_ST         (0x2UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for DAC0_ST          */
#define SYS_GPC_MFPL_PC1MFP_UART2_nRTS      (0x3UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for UART2_nRTS       */
#define SYS_GPC_MFPL_PC1MFP_CAN0_RXD        (0x4UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for CAN0_RXD         */
#define SYS_GPC_MFPL_PC1MFP_SPI2_I2SMCLK    (0x5UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for SPI2_I2SMCLK     */
#define SYS_GPC_MFPL_PC1MFP_PWM0_CH1        (0x6UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for PWM0_CH1         */
#define SYS_GPC_MFPL_PC1MFP_EBI_AD9         (0x7UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for EBI_AD9          */
#define SYS_GPC_MFPL_PC1MFP_SPI0_MOSI1      (0x8UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for SPI0_MOSI1       */
#define SYS_GPC_MFPL_PC1MFP_UART3_RXD       (0x9UL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for UART3_RXD        */
#define SYS_GPC_MFPL_PC1MFP_I2S0_DO         (0xAUL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for I2S0_DO          */
#define SYS_GPC_MFPL_PC1MFP_SD0_DAT3        (0xBUL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for SD0_DAT3         */
#define SYS_GPC_MFPL_PC1MFP_USCI0_DAT0      (0xCUL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for USCI0_DAT0       */
#define SYS_GPC_MFPL_PC1MFP_ETM_TRACE_DATA0 (0xDUL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for ETM_TRACE_DATA0  */
#define SYS_GPC_MFPL_PC1MFP_DSRC_RXON       (0xFUL<<SYS_GPC_MFPL_PC1MFP_Pos)    /*!< GPC_MFPL PC1 setting for DSRC_RXON        */

//PC.2 MFP
#define SYS_GPC_MFPL_PC2MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for GPIO             */
#define SYS_GPC_MFPL_PC2MFP_SPI2_SS         (0x2UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for SPI2_SS          */
#define SYS_GPC_MFPL_PC2MFP_UART2_TXD       (0x3UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for UART2_TXD        */
#define SYS_GPC_MFPL_PC2MFP_TM3             (0x4UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for TM3              */
#define SYS_GPC_MFPL_PC2MFP_ACMP1_O         (0x5UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for ACMP1_O          */
#define SYS_GPC_MFPL_PC2MFP_PWM0_CH2        (0x6UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for PWM0_CH2         */
#define SYS_GPC_MFPL_PC2MFP_EBI_AD10        (0x7UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for EBI_AD10         */
#define SYS_GPC_MFPL_PC2MFP_SPI0_MISO0      (0x8UL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for SPI0_MISO0       */
#define SYS_GPC_MFPL_PC2MFP_I2S0_MCLK       (0xAUL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for I2S0_MCLK        */
#define SYS_GPC_MFPL_PC2MFP_SD0_DAT2        (0xBUL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for SD0_DAT2         */
#define SYS_GPC_MFPL_PC2MFP_USCI0_DAT1      (0xCUL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for USCI0_DAT1       */
#define SYS_GPC_MFPL_PC2MFP_ETM_TRACE_DATA1 (0xDUL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for ETM_TRACE_DATA1  */
#define SYS_GPC_MFPL_PC2MFP_SPI5_MISO       (0xFUL<<SYS_GPC_MFPL_PC2MFP_Pos)    /*!< GPC_MFPL PC2 setting for SPI5_MISO        */

//PC.3 MFP
#define SYS_GPC_MFPL_PC3MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for GPIO             */
#define SYS_GPC_MFPL_PC3MFP_SPI2_MOSI       (0x2UL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for SPI2_MOSI        */
#define SYS_GPC_MFPL_PC3MFP_UART2_RXD       (0x3UL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for UART2_RXD        */
#define SYS_GPC_MFPL_PC3MFP_PWM0_CH3        (0x6UL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for PWM0_CH3         */
#define SYS_GPC_MFPL_PC3MFP_EBI_AD11        (0x7UL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for EBI_AD11         */
#define SYS_GPC_MFPL_PC3MFP_SPI0_MOSI0      (0x8UL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for SPI0_MOSI0       */
#define SYS_GPC_MFPL_PC3MFP_I2S0_BCLK       (0xAUL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for I2S0_BCLK        */
#define SYS_GPC_MFPL_PC3MFP_SD0_DAT1        (0xBUL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for SD0_DAT1         */
#define SYS_GPC_MFPL_PC3MFP_USCI0_CTL0      (0xCUL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for USCI0_CTL0       */
#define SYS_GPC_MFPL_PC3MFP_ETM_TRACE_DATA2 (0xDUL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for ETM_TRACE_DATA2  */
#define SYS_GPC_MFPL_PC3MFP_SPI5_MOSI       (0xFUL<<SYS_GPC_MFPL_PC3MFP_Pos)    /*!< GPC_MFPL PC3 setting for SPI5_MOSI        */

//PC.4 MFP
#define SYS_GPC_MFPL_PC4MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for GPIO             */
#define SYS_GPC_MFPL_PC4MFP_SC0_PWR         (0x1UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for SC0_PWR          */
#define SYS_GPC_MFPL_PC4MFP_SPI2_MISO       (0x2UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for SPI2_MISO        */
#define SYS_GPC_MFPL_PC4MFP_I2C1_SCL        (0x3UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for I2C1_SCL         */
#define SYS_GPC_MFPL_PC4MFP_PWM0_CH4        (0x6UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for PWM0_CH4         */
#define SYS_GPC_MFPL_PC4MFP_EBI_AD12        (0x7UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for EBI_AD12         */
#define SYS_GPC_MFPL_PC4MFP_SPI0_CLK        (0x8UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for SPI0_CLK         */
#define SYS_GPC_MFPL_PC4MFP_UART4_nCTS      (0x9UL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for UART4_nCTS       */
#define SYS_GPC_MFPL_PC4MFP_I2S0_DI         (0xAUL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for I2S0_DI          */
#define SYS_GPC_MFPL_PC4MFP_SD0_DAT0        (0xBUL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for SD0_DAT0         */
#define SYS_GPC_MFPL_PC4MFP_USCI0_CTL1      (0xCUL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for USCI0_CTL1       */
#define SYS_GPC_MFPL_PC4MFP_ETM_TRACE_DATA3 (0xDUL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for ETM_TRACE_DATA3  */
#define SYS_GPC_MFPL_PC4MFP_SPI5_CLK        (0xFUL<<SYS_GPC_MFPL_PC4MFP_Pos)    /*!< GPC_MFPL PC4 setting for SPI5_CLK         */

//PC.5 MFP
#define SYS_GPC_MFPL_PC5MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for GPIO             */
#define SYS_GPC_MFPL_PC5MFP_SPI2_I2SMCLK    (0x2UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for SPI2_I2SMCLK     */
#define SYS_GPC_MFPL_PC5MFP_DAC1_ST         (0x3UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for DAC1_ST          */
#define SYS_GPC_MFPL_PC5MFP_BPWM0_CH1       (0x4UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for BPWM0_CH1        */
#define SYS_GPC_MFPL_PC5MFP_PWM0_CH1        (0x6UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for PWM0_CH1         */
#define SYS_GPC_MFPL_PC5MFP_EBI_AD13        (0x7UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for EBI_AD13         */
#define SYS_GPC_MFPL_PC5MFP_SPI0_SS         (0x8UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for SPI0_SS          */
#define SYS_GPC_MFPL_PC5MFP_UART4_TXD       (0x9UL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for UART4_TXD        */
#define SYS_GPC_MFPL_PC5MFP_DSRC_WAKEUP     (0xDUL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for DSRC_WAKEUP      */
#define SYS_GPC_MFPL_PC5MFP_SPI5_SS         (0xFUL<<SYS_GPC_MFPL_PC5MFP_Pos)    /*!< GPC_MFPL PC5 setting for SPI5_SS          */

//PC.6 MFP
#define SYS_GPC_MFPL_PC6MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for GPIO             */
#define SYS_GPC_MFPL_PC6MFP_I2C1_SMBAL      (0x3UL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for I2C1_SMBAL       */
#define SYS_GPC_MFPL_PC6MFP_SC0_DAT         (0x4UL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for SC0_DAT          */
#define SYS_GPC_MFPL_PC6MFP_ACMP1_O         (0x5UL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for ACMP1_O          */
#define SYS_GPC_MFPL_PC6MFP_PWM1_CH0        (0x6UL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for PWM1_CH0         */
#define SYS_GPC_MFPL_PC6MFP_EBI_AD14        (0x7UL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for EBI_AD14         */
#define SYS_GPC_MFPL_PC6MFP_UART0_RXD       (0x9UL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for UART0_RXD        */
#define SYS_GPC_MFPL_PC6MFP_UART4_RXD       (0xAUL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for UART4_RXD        */
#define SYS_GPC_MFPL_PC6MFP_SPI5_MOSI       (0xCUL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for SPI5_MOSI        */
#define SYS_GPC_MFPL_PC6MFP_ECAP0_IC1       (0xEUL<<SYS_GPC_MFPL_PC6MFP_Pos)    /*!< GPC_MFPL PC6 setting for ECAP0_IC1        */

//PC.7 MFP
#define SYS_GPC_MFPL_PC7MFP_GPIO            (0x0UL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for GPIO             */
#define SYS_GPC_MFPL_PC7MFP_I2C1_SMBSUS     (0x3UL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for I2C1_SMBSUS      */
#define SYS_GPC_MFPL_PC7MFP_SC0_CLK         (0x4UL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for SC0_CLK          */
#define SYS_GPC_MFPL_PC7MFP_PWM1_CH1        (0x6UL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for PWM1_CH1         */
#define SYS_GPC_MFPL_PC7MFP_EBI_AD15        (0x7UL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for EBI_AD15         */
#define SYS_GPC_MFPL_PC7MFP_UART0_TXD       (0x9UL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for UART0_TXD        */
#define SYS_GPC_MFPL_PC7MFP_UART4_nRTS      (0xAUL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for UART4_nRTS       */
#define SYS_GPC_MFPL_PC7MFP_SPI5_MISO       (0xCUL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for SPI5_MISO        */
#define SYS_GPC_MFPL_PC7MFP_ECAP0_IC0       (0xEUL<<SYS_GPC_MFPL_PC7MFP_Pos)    /*!< GPC_MFPL PC7 setting for ECAP0_IC0        */

//PC.8 MFP
#define SYS_GPC_MFPH_PC8MFP_GPIO            (0x0UL<<SYS_GPC_MFPH_PC8MFP_Pos)    /*!< GPC_MFPH PC8 setting for GPIO             */
#define SYS_GPC_MFPH_PC8MFP_SPI2_I2SMCLK    (0x4UL<<SYS_GPC_MFPH_PC8MFP_Pos)    /*!< GPC_MFPH PC8 setting for SPI2_I2SMCLK     */
#define SYS_GPC_MFPH_PC8MFP_EBI_nCS2        (0x7UL<<SYS_GPC_MFPH_PC8MFP_Pos)    /*!< GPC_MFPH PC8 setting for EBI_nCS2         */
#define SYS_GPC_MFPH_PC8MFP_BPWM0_CH0       (0xDUL<<SYS_GPC_MFPH_PC8MFP_Pos)    /*!< GPC_MFPH PC8 setting for BPWM0_CH0        */

//PC.9 MFP
#define SYS_GPC_MFPH_PC9MFP_GPIO            (0x0UL<<SYS_GPC_MFPH_PC9MFP_Pos)    /*!< GPC_MFPH PC9 setting for GPIO             */
#define SYS_GPC_MFPH_PC9MFP_SPI2_I2SMCLK    (0x2UL<<SYS_GPC_MFPH_PC9MFP_Pos)    /*!< GPC_MFPH PC9 setting for SPI2_I2SMCLK     */
#define SYS_GPC_MFPH_PC9MFP_PWM1_CH0        (0x6UL<<SYS_GPC_MFPH_PC9MFP_Pos)    /*!< GPC_MFPH PC9 setting for PWM1_CH0         */
#define SYS_GPC_MFPH_PC9MFP_EBI_ADR15       (0x7UL<<SYS_GPC_MFPH_PC9MFP_Pos)    /*!< GPC_MFPH PC9 setting for EBI_ADR15        */

//PC.10 MFP
#define SYS_GPC_MFPH_PC10MFP_GPIO           (0x0UL<<SYS_GPC_MFPH_PC10MFP_Pos)   /*!< GPC_MFPH PC10 setting for GPIO            */
#define SYS_GPC_MFPH_PC10MFP_SPI2_MOSI      (0x2UL<<SYS_GPC_MFPH_PC10MFP_Pos)   /*!< GPC_MFPH PC10 setting for SPI2_MOSI       */
#define SYS_GPC_MFPH_PC10MFP_PWM1_CH1       (0x6UL<<SYS_GPC_MFPH_PC10MFP_Pos)   /*!< GPC_MFPH PC10 setting for PWM1_CH1        */
#define SYS_GPC_MFPH_PC10MFP_EBI_ADR14      (0x7UL<<SYS_GPC_MFPH_PC10MFP_Pos)   /*!< GPC_MFPH PC10 setting for EBI_ADR14       */

//PC.11 MFP
#define SYS_GPC_MFPH_PC11MFP_GPIO           (0x0UL<<SYS_GPC_MFPH_PC11MFP_Pos)   /*!< GPC_MFPH PC11 setting for GPIO            */
#define SYS_GPC_MFPH_PC11MFP_SPI2_MISO      (0x2UL<<SYS_GPC_MFPH_PC11MFP_Pos)   /*!< GPC_MFPH PC11 setting for SPI2_MISO       */
#define SYS_GPC_MFPH_PC11MFP_PWM1_CH2       (0x6UL<<SYS_GPC_MFPH_PC11MFP_Pos)   /*!< GPC_MFPH PC11 setting for PWM1_CH2        */
#define SYS_GPC_MFPH_PC11MFP_EBI_ADR13      (0x7UL<<SYS_GPC_MFPH_PC11MFP_Pos)   /*!< GPC_MFPH PC11 setting for EBI_ADR13       */

//PC.12 MFP
#define SYS_GPC_MFPH_PC12MFP_GPIO           (0x0UL<<SYS_GPC_MFPH_PC12MFP_Pos)   /*!< GPC_MFPH PC12 setting for GPIO            */
#define SYS_GPC_MFPH_PC12MFP_SPI2_CLK       (0x2UL<<SYS_GPC_MFPH_PC12MFP_Pos)   /*!< GPC_MFPH PC12 setting for SPI2_CLK        */
#define SYS_GPC_MFPH_PC12MFP_PWM1_CH3       (0x6UL<<SYS_GPC_MFPH_PC12MFP_Pos)   /*!< GPC_MFPH PC12 setting for PWM1_CH3        */
#define SYS_GPC_MFPH_PC12MFP_EBI_ADR12      (0x7UL<<SYS_GPC_MFPH_PC12MFP_Pos)   /*!< GPC_MFPH PC12 setting for EBI_ADR12       */

//PC.13 MFP
#define SYS_GPC_MFPH_PC13MFP_GPIO           (0x0UL<<SYS_GPC_MFPH_PC13MFP_Pos)   /*!< GPC_MFPH PC13 setting for GPIO            */
#define SYS_GPC_MFPH_PC13MFP_SPI2_SS        (0x2UL<<SYS_GPC_MFPH_PC13MFP_Pos)   /*!< GPC_MFPH PC13 setting for SPI2_SS         */
#define SYS_GPC_MFPH_PC13MFP_PWM1_CH4       (0x6UL<<SYS_GPC_MFPH_PC13MFP_Pos)   /*!< GPC_MFPH PC13 setting for PWM1_CH4        */
#define SYS_GPC_MFPH_PC13MFP_EBI_ADR11      (0x7UL<<SYS_GPC_MFPH_PC13MFP_Pos)   /*!< GPC_MFPH PC13 setting for EBI_ADR11       */

//PC.14 MFP
#define SYS_GPC_MFPH_PC14MFP_GPIO           (0x0UL<<SYS_GPC_MFPH_PC14MFP_Pos)   /*!< GPC_MFPH PC14 setting for GPIO            */
#define SYS_GPC_MFPH_PC14MFP_PWM1_CH5       (0x6UL<<SYS_GPC_MFPH_PC14MFP_Pos)   /*!< GPC_MFPH PC14 setting for PWM1_CH5        */
#define SYS_GPC_MFPH_PC14MFP_EBI_ADR10      (0x7UL<<SYS_GPC_MFPH_PC14MFP_Pos)   /*!< GPC_MFPH PC14 setting for EBI_ADR10       */

//PC.15 MFP
#define SYS_GPC_MFPH_PC15MFP_GPIO           (0x0UL<<SYS_GPC_MFPH_PC15MFP_Pos)   /*!< GPC_MFPH PC15 setting for GPIO            */
#define SYS_GPC_MFPH_PC15MFP_PWM1_CH0       (0x6UL<<SYS_GPC_MFPH_PC15MFP_Pos)   /*!< GPC_MFPH PC15 setting for PWM1_CH0        */
#define SYS_GPC_MFPH_PC15MFP_EBI_ADR0       (0x7UL<<SYS_GPC_MFPH_PC15MFP_Pos)   /*!< GPC_MFPH PC15 setting for EBI_ADR0        */
#define SYS_GPC_MFPH_PC15MFP_SPI3_I2SMCLK   (0x9UL<<SYS_GPC_MFPH_PC15MFP_Pos)   /*!< GPC_MFPH PC15 setting for SPI3_I2SMCLK    */

//PD.0 MFP
#define SYS_GPD_MFPL_PD0MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for GPIO             */
#define SYS_GPD_MFPL_PD0MFP_SPI1_I2SMCLK    (0x2UL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for SPI1_I2SMCLK     */
#define SYS_GPD_MFPL_PD0MFP_UART0_RXD       (0x3UL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for UART0_RXD        */
#define SYS_GPD_MFPL_PD0MFP_UART5_RXD       (0x4UL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for UART5_RXD        */
#define SYS_GPD_MFPL_PD0MFP_ACMP1_N         (0x5UL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for ACMP1_N          */
#define SYS_GPD_MFPL_PD0MFP_EBI_ALE         (0x7UL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for EBI_ALE          */
#define SYS_GPD_MFPL_PD0MFP_INT3            (0x8UL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for INT3             */
#define SYS_GPD_MFPL_PD0MFP_SC0_PWR         (0xAUL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PDM setting for SC0_PWR          */
#define SYS_GPD_MFPL_PD0MFP_SC1_nCD         (0xBUL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for SC1_nCD          */
#define SYS_GPD_MFPL_PD0MFP_TM3             (0xCUL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for TM3              */
#define SYS_GPD_MFPL_PD0MFP_BPWM0_CH2       (0xDUL<<SYS_GPD_MFPL_PD0MFP_Pos)    /*!< GPD_MFPL PD0 setting for BPWM0_CH2        */

//PD.1 MFP
#define SYS_GPD_MFPL_PD1MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for GPIO             */
#define SYS_GPD_MFPL_PD1MFP_UART5_RXD       (0x1UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for UART5_RXD        */
#define SYS_GPD_MFPL_PD1MFP_PWM0_SYNC_IN    (0x2UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for PWM0_SYNC_IN     */
#define SYS_GPD_MFPL_PD1MFP_UART0_TXD       (0x3UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for UART0_TXD        */
#define SYS_GPD_MFPL_PD1MFP_SPI2_MISO       (0x4UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for SPI2_MISO        */
#define SYS_GPD_MFPL_PD1MFP_ACMP1_P2        (0x5UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for ACMP1_P2         */
#define SYS_GPD_MFPL_PD1MFP_TM0             (0x6UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for TM0              */
#define SYS_GPD_MFPL_PD1MFP_EBI_nRD         (0x7UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for EBI_nRD          */
#define SYS_GPD_MFPL_PD1MFP_QEI0_B          (0x8UL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for QEI0_B           */
#define SYS_GPD_MFPL_PD1MFP_SC0_RST         (0xAUL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for SC0_RST          */
#define SYS_GPD_MFPL_PD1MFP_SC2_RST         (0xBUL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for SC2_RST          */
#define SYS_GPD_MFPL_PD1MFP_USCI0_CTL0      (0xCUL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for USCI0_CTL0       */
#define SYS_GPD_MFPL_PD1MFP_BPWM0_CH3       (0xDUL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for BPWM0_CH3        */
#define SYS_GPD_MFPL_PD1MFP_SPI3_I2SMCLK    (0xFUL<<SYS_GPD_MFPL_PD1MFP_Pos)    /*!< GPD_MFPL PD1 setting for SPI3_I2SMCLK     */

//PD.2 MFP
#define SYS_GPD_MFPL_PD2MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for GPIO             */
#define SYS_GPD_MFPL_PD2MFP_EADC0_ST        (0x1UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for EADC0_ST         */
#define SYS_GPD_MFPL_PD2MFP_UART3_TXD       (0x2UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for UART3_TXD        */
#define SYS_GPD_MFPL_PD2MFP_TM0_EXT         (0x3UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for TM0_EXT          */
#define SYS_GPD_MFPL_PD2MFP_SPI2_CLK        (0x4UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for SPI2_CLK         */
#define SYS_GPD_MFPL_PD2MFP_ACMP1_P1        (0x5UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for ACMP1_P1         */
#define SYS_GPD_MFPL_PD2MFP_PWM0_BRAKE0     (0x6UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for PWM0_BRAKE0      */
#define SYS_GPD_MFPL_PD2MFP_EBI_nWR         (0x7UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for EBI_nWR          */
#define SYS_GPD_MFPL_PD2MFP_INT0            (0x8UL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for INT0             */
#define SYS_GPD_MFPL_PD2MFP_SC0_DAT         (0xAUL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for SC0_DAT          */
#define SYS_GPD_MFPL_PD2MFP_SC2_CLK         (0xBUL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for SC2_CLK          */
#define SYS_GPD_MFPL_PD2MFP_USCI0_CTL1      (0xCUL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for USCI0_CTL1       */
#define SYS_GPD_MFPL_PD2MFP_BPWM0_CH4       (0xDUL<<SYS_GPD_MFPL_PD2MFP_Pos)    /*!< GPD_MFPL PD2 setting for BPWM0_CH4        */

//PD.3 MFP
#define SYS_GPD_MFPL_PD3MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for GPIO             */
#define SYS_GPD_MFPL_PD3MFP_TM2             (0x1UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for TM2              */
#define SYS_GPD_MFPL_PD3MFP_UART3_RXD       (0x2UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for UART3_RXD        */
#define SYS_GPD_MFPL_PD3MFP_TM1_EXT         (0x3UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for TM1_EXT          */
#define SYS_GPD_MFPL_PD3MFP_SPI1_I2SMCLK    (0x4UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for SPI1_I2SMCLK     */
#define SYS_GPD_MFPL_PD3MFP_ACMP1_P0        (0x5UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for ACMP1_P0         */
#define SYS_GPD_MFPL_PD3MFP_PWM0_BRAKE1     (0x6UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for PWM0_BRAKE1      */
#define SYS_GPD_MFPL_PD3MFP_EBI_MCLK        (0x7UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for EBI_MCLK         */
#define SYS_GPD_MFPL_PD3MFP_INT1            (0x8UL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for INT1             */
#define SYS_GPD_MFPL_PD3MFP_SC0_CLK         (0xAUL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for SC0_CLK          */
#define SYS_GPD_MFPL_PD3MFP_SC2_DAT         (0xBUL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for SC2_DAT          */
#define SYS_GPD_MFPL_PD3MFP_USCI0_CLK       (0xCUL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for USCI0_CLK        */
#define SYS_GPD_MFPL_PD3MFP_BPWM0_CH5       (0xDUL<<SYS_GPD_MFPL_PD3MFP_Pos)    /*!< GPD_MFPL PD3 setting for BPWM0_CH5        */

//PD.4 MFP
#define SYS_GPD_MFPL_PD4MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD4MFP_Pos)    /*!< GPD_MFPL PD4 setting for GPIO             */
#define SYS_GPD_MFPL_PD4MFP_SPI1_CLK        (0x2UL<<SYS_GPD_MFPL_PD4MFP_Pos)    /*!< GPD_MFPL PD4 setting for SPI1_CLK         */
#define SYS_GPD_MFPL_PD4MFP_I2C0_SDA        (0x3UL<<SYS_GPD_MFPL_PD4MFP_Pos)    /*!< GPD_MFPL PD4 setting for I2C0_SDA         */
#define SYS_GPD_MFPL_PD4MFP_PWM0_BRAKE0     (0x5UL<<SYS_GPD_MFPL_PD4MFP_Pos)    /*!< GPD_MFPL PD4 setting for PWM0_BRAKE0      */
#define SYS_GPD_MFPL_PD4MFP_TM0             (0x6UL<<SYS_GPD_MFPL_PD4MFP_Pos)    /*!< GPD_MFPL PD4 setting for TM0              */
#define SYS_GPD_MFPL_PD4MFP_EBI_AD11        (0x7UL<<SYS_GPD_MFPL_PD4MFP_Pos)    /*!< GPD_MFPL PD4 setting for EBI_AD11         */

//PD.5 MFP
#define SYS_GPD_MFPL_PD5MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD5MFP_Pos)    /*!< GPD_MFPL PD5 setting for GPIO             */
#define SYS_GPD_MFPL_PD5MFP_CLKO            (0x1UL<<SYS_GPD_MFPL_PD5MFP_Pos)    /*!< GPD_MFPL PD5 setting for CLKO             */
#define SYS_GPD_MFPL_PD5MFP_SPI1_MISO       (0x2UL<<SYS_GPD_MFPL_PD5MFP_Pos)    /*!< GPD_MFPL PD5 setting for SPI1_MISO        */
#define SYS_GPD_MFPL_PD5MFP_I2C0_SCL        (0x3UL<<SYS_GPD_MFPL_PD5MFP_Pos)    /*!< GPD_MFPL PD5 setting for I2C0_SCL         */
#define SYS_GPD_MFPL_PD5MFP_PWM0_BRAKE1     (0x5UL<<SYS_GPD_MFPL_PD5MFP_Pos)    /*!< GPD_MFPL PD5 setting for PWM0_BRAKE1      */
#define SYS_GPD_MFPL_PD5MFP_TM1             (0x6UL<<SYS_GPD_MFPL_PD5MFP_Pos)    /*!< GPD_MFPL PD5 setting for TM1              */
#define SYS_GPD_MFPL_PD5MFP_EBI_AD12        (0x7UL<<SYS_GPD_MFPL_PD5MFP_Pos)    /*!< GPD_MFPL PD5 setting for EBI_AD12         */

//PD.6 MFP
#define SYS_GPD_MFPL_PD6MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for GPIO             */
#define SYS_GPD_MFPL_PD6MFP_CLKO            (0x1UL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for CLKO             */
#define SYS_GPD_MFPL_PD6MFP_SPI1_SS         (0x2UL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for SPI1_SS          */
#define SYS_GPD_MFPL_PD6MFP_UART0_RXD       (0x3UL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for UART0_RXD        */
#define SYS_GPD_MFPL_PD6MFP_ACMP0_O         (0x5UL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for ACMP0_O          */
#define SYS_GPD_MFPL_PD6MFP_PWM0_CH5        (0x6UL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for PWM0_CH5         */
#define SYS_GPD_MFPL_PD6MFP_EBI_nWR         (0x7UL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for EBI_nWR          */
#define SYS_GPD_MFPL_PD6MFP_SD0_CMD         (0xEUL<<SYS_GPD_MFPL_PD6MFP_Pos)    /*!< GPD_MFPL PD6 setting for SD0_CMD          */

//PD.7 MFP
#define SYS_GPD_MFPL_PD7MFP_GPIO            (0x0UL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for GPIO             */
#define SYS_GPD_MFPL_PD7MFP_PWM0_SYNC_IN    (0x3UL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for PWM0_SYNC_IN     */
#define SYS_GPD_MFPL_PD7MFP_TM1             (0x4UL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for TM1              */
#define SYS_GPD_MFPL_PD7MFP_ACMP0_O         (0x5UL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for ACMP0_O          */
#define SYS_GPD_MFPL_PD7MFP_PWM0_CH5        (0x6UL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for PWM0_CH5         */
#define SYS_GPD_MFPL_PD7MFP_EBI_nRD         (0x7UL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for EBI_nRD          */
#define SYS_GPD_MFPL_PD7MFP_SC2_PWR         (0xAUL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for SC2_PWR          */
#define SYS_GPD_MFPL_PD7MFP_SC0_nCD         (0xBUL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for SC0_nCD          */
#define SYS_GPD_MFPL_PD7MFP_PWM1_BRAKE1     (0xCUL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for PWM1_BRAKE1      */
#define SYS_GPD_MFPL_PD7MFP_ECAP1_IC1       (0xDUL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for ECAP1_IC1        */
#define SYS_GPD_MFPL_PD7MFP_SD0_CMD         (0xEUL<<SYS_GPD_MFPL_PD7MFP_Pos)    /*!< GPD_MFPL PD7 setting for SD0_CMD          */

//PD.8 MFP
#define SYS_GPD_MFPH_PD8MFP_GPIO            (0x0UL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for GPIO             */
#define SYS_GPD_MFPH_PD8MFP_I2C0_SDA        (0x3UL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for I2C0_SDA         */
#define SYS_GPD_MFPH_PD8MFP_SPI2_SS         (0x4UL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for SPI2_SS          */
#define SYS_GPD_MFPH_PD8MFP_ACMP1_WLAT      (0x5UL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for ACMP1_WLAT       */
#define SYS_GPD_MFPH_PD8MFP_EBI_nCS0        (0x7UL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for EBI_nCS0         */
#define SYS_GPD_MFPH_PD8MFP_QEI0_INDEX      (0x8UL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for QEI0_INDEX       */
#define SYS_GPD_MFPH_PD8MFP_SC2_nCD         (0xBUL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for SC2_nCD          */
#define SYS_GPD_MFPH_PD8MFP_USCI0_DAT0      (0xCUL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for USCI0_DAT0       */
#define SYS_GPD_MFPH_PD8MFP_BPWM0_CH1       (0xDUL<<SYS_GPD_MFPH_PD8MFP_Pos)    /*!< GPD_MFPH PD8 setting for BPWM0_CH1        */

//PD.9 MFP
#define SYS_GPD_MFPH_PD9MFP_GPIO            (0x0UL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for GPIO             */
#define SYS_GPD_MFPH_PD9MFP_I2C0_SCL        (0x3UL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for I2C0_SCL         */
#define SYS_GPD_MFPH_PD9MFP_SPI2_MOSI       (0x4UL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for SPI2_MOSI        */
#define SYS_GPD_MFPH_PD9MFP_ACMP1_P3        (0x5UL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for ACMP1_P3         */
#define SYS_GPD_MFPH_PD9MFP_EBI_ALE         (0x7UL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for EBI_ALE          */
#define SYS_GPD_MFPH_PD9MFP_QEI0_A          (0x8UL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for QEI0_A           */
#define SYS_GPD_MFPH_PD9MFP_SC2_PWR         (0xBUL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for SC2_PWR          */
#define SYS_GPD_MFPH_PD9MFP_USCI0_DAT1      (0xCUL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for USCI0_DAT1       */
#define SYS_GPD_MFPH_PD9MFP_BPWM0_CH2       (0xDUL<<SYS_GPD_MFPH_PD9MFP_Pos)    /*!< GPD_MFPH PD9 setting for BPWM0_CH2        */

//PD.10 MFP
#define SYS_GPD_MFPH_PD10MFP_GPIO           (0x0UL<<SYS_GPD_MFPH_PD10MFP_Pos)   /*!< GPD_MFPH PD10 setting for GPIO            */
#define SYS_GPD_MFPH_PD10MFP_SPI1_CLK       (0x1UL<<SYS_GPD_MFPH_PD10MFP_Pos)   /*!< GPD_MFPH PD10 setting for SPI1_CLK        */
#define SYS_GPD_MFPH_PD10MFP_TM2            (0x4UL<<SYS_GPD_MFPH_PD10MFP_Pos)   /*!< GPD_MFPH PD10 setting for TM2             */
#define SYS_GPD_MFPH_PD10MFP_EBI_ADR9       (0x7UL<<SYS_GPD_MFPH_PD10MFP_Pos)   /*!< GPD_MFPH PD10 setting for EBI_ADR9        */
#define SYS_GPD_MFPH_PD10MFP_USCI0_CTL1     (0xCUL<<SYS_GPD_MFPH_PD10MFP_Pos)   /*!< GPD_MFPH PD10 setting for USCI0_CTL1      */
#define SYS_GPD_MFPH_PD10MFP_SD0_DAT2       (0xEUL<<SYS_GPD_MFPH_PD10MFP_Pos)   /*!< GPD_MFPH PD10 setting for SD0_DAT2        */

//PD.11 MFP
#define SYS_GPD_MFPH_PD11MFP_GPIO           (0x0UL<<SYS_GPD_MFPH_PD11MFP_Pos)   /*!< GPD_MFPH PD11 setting for GPIO            */
#define SYS_GPD_MFPH_PD11MFP_SPI1_MOSI      (0x1UL<<SYS_GPD_MFPH_PD11MFP_Pos)   /*!< GPD_MFPH PD11 setting for SPI1_MOSI       */
#define SYS_GPD_MFPH_PD11MFP_TM3            (0x4UL<<SYS_GPD_MFPH_PD11MFP_Pos)   /*!< GPD_MFPH PD11 setting for TM3             */
#define SYS_GPD_MFPH_PD11MFP_EBI_ADR8       (0x7UL<<SYS_GPD_MFPH_PD11MFP_Pos)   /*!< GPD_MFPH PD11 setting for EBI_ADR8        */
#define SYS_GPD_MFPH_PD11MFP_USCI0_CTL0     (0xCUL<<SYS_GPD_MFPH_PD11MFP_Pos)   /*!< GPD_MFPH PD11 setting for USCI0_CTL0      */
#define SYS_GPD_MFPH_PD11MFP_SD0_DAT1       (0xEUL<<SYS_GPD_MFPH_PD11MFP_Pos)   /*!< GPD_MFPH PD11 setting for SD0_DAT1        */

//PD.12 MFP
#define SYS_GPD_MFPH_PD12MFP_GPIO           (0x0UL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for GPIO            */
#define SYS_GPD_MFPH_PD12MFP_SPI1_MISO      (0x1UL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for SPI1_MISO       */
#define SYS_GPD_MFPH_PD12MFP_SPI2_SS        (0x2UL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for SPI2_SS         */
#define SYS_GPD_MFPH_PD12MFP_UART3_TXD      (0x3UL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for UART3_TXD       */
#define SYS_GPD_MFPH_PD12MFP_PWM1_CH0       (0x6UL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for PWM1_CH0        */
#define SYS_GPD_MFPH_PD12MFP_EBI_ADR16      (0x7UL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for EBI_ADR16       */
#define SYS_GPD_MFPH_PD12MFP_SC2_nCD        (0xAUL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for SC2_nCD         */
#define SYS_GPD_MFPH_PD12MFP_USCI0_CLK      (0xCUL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for USCI0_CLK       */
#define SYS_GPD_MFPH_PD12MFP_SD0_DAT0       (0xEUL<<SYS_GPD_MFPH_PD12MFP_Pos)   /*!< GPD_MFPH PD12 setting for SD0_DAT0        */

//PD.13 MFP
#define SYS_GPD_MFPH_PD13MFP_GPIO           (0x0UL<<SYS_GPD_MFPH_PD13MFP_Pos)   /*!< GPD_MFPH PD13 setting for GPIO            */
#define SYS_GPD_MFPH_PD13MFP_SPI2_MOSI      (0x2UL<<SYS_GPD_MFPH_PD13MFP_Pos)   /*!< GPD_MFPH PD13 setting for SPI2_MOSI       */
#define SYS_GPD_MFPH_PD13MFP_UART3_RXD      (0x3UL<<SYS_GPD_MFPH_PD13MFP_Pos)   /*!< GPD_MFPH PD13 setting for UART3_RXD       */
#define SYS_GPD_MFPH_PD13MFP_PWM1_CH1       (0x6UL<<SYS_GPD_MFPH_PD13MFP_Pos)   /*!< GPD_MFPH PD13 setting for PWM1_CH1        */
#define SYS_GPD_MFPH_PD13MFP_EBI_ADR17      (0x7UL<<SYS_GPD_MFPH_PD13MFP_Pos)   /*!< GPD_MFPH PD13 setting for EBI_ADR17       */
#define SYS_GPD_MFPH_PD13MFP_SC2_RST        (0xAUL<<SYS_GPD_MFPH_PD13MFP_Pos)   /*!< GPD_MFPH PD13 setting for SC2_RST         */

//PD.14 MFP
#define SYS_GPD_MFPH_PD14MFP_GPIO           (0x0UL<<SYS_GPD_MFPH_PD14MFP_Pos)   /*!< GPD_MFPH PD14 setting for GPIO            */
#define SYS_GPD_MFPH_PD14MFP_SPI2_MISO      (0x2UL<<SYS_GPD_MFPH_PD14MFP_Pos)   /*!< GPD_MFPH PD14 setting for SPI2_MISO       */
#define SYS_GPD_MFPH_PD14MFP_UART3_nCTS     (0x3UL<<SYS_GPD_MFPH_PD14MFP_Pos)   /*!< GPD_MFPH PD14 setting for UART3_nCTS      */
#define SYS_GPD_MFPH_PD14MFP_PWM1_BRAKE0    (0x5UL<<SYS_GPD_MFPH_PD14MFP_Pos)   /*!< GPD_MFPH PD14 setting for PWM1_BRAKE0     */
#define SYS_GPD_MFPH_PD14MFP_PWM1_CH2       (0x6UL<<SYS_GPD_MFPH_PD14MFP_Pos)   /*!< GPD_MFPH PD14 setting for PWM1_CH2        */
#define SYS_GPD_MFPH_PD14MFP_EBI_ADR18      (0x7UL<<SYS_GPD_MFPH_PD14MFP_Pos)   /*!< GPD_MFPH PD14 setting for EBI_ADR18       */
#define SYS_GPD_MFPH_PD14MFP_SC2_DAT        (0xAUL<<SYS_GPD_MFPH_PD14MFP_Pos)   /*!< GPD_MFPH PD14 setting for SC2_DAT         */

//PD.15 MFP
#define SYS_GPD_MFPH_PD15MFP_GPIO           (0x0UL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for GPIO            */
#define SYS_GPD_MFPH_PD15MFP_SPI2_CLK       (0x2UL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for SPI2_CLK        */
#define SYS_GPD_MFPH_PD15MFP_UART3_nRTS     (0x3UL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for UART3_nRTS      */
#define SYS_GPD_MFPH_PD15MFP_PWM1_CH3       (0x6UL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for PWM1_CH3        */
#define SYS_GPD_MFPH_PD15MFP_EBI_ADR19      (0x7UL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for EBI_ADR19       */
#define SYS_GPD_MFPH_PD15MFP_SC2_CLK        (0xAUL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for SC2_CLK         */
#define SYS_GPD_MFPH_PD15MFP_QEI1_INDEX     (0xBUL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for QEI1_INDEX      */
#define SYS_GPD_MFPH_PD15MFP_QEI0_INDEX     (0xCUL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for QEI0_INDEX      */
#define SYS_GPD_MFPH_PD15MFP_ECAP0_IC2      (0xDUL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for ECAP0_IC2       */
#define SYS_GPD_MFPH_PD15MFP_SD0_nCD        (0xEUL<<SYS_GPD_MFPH_PD15MFP_Pos)   /*!< GPD_MFPH PD15 setting for SD0_nCD         */

//PE.0 MFP
#define SYS_GPE_MFPL_PE0MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for GPIO             */
#define SYS_GPE_MFPL_PE0MFP_SPI0_SS         (0x1UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for SPI0_SS          */
#define SYS_GPE_MFPL_PE0MFP_SPI2_CLK        (0x2UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for SPI2_CLK         */
#define SYS_GPE_MFPL_PE0MFP_I2C1_SDA        (0x3UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for I2C1_SDA         */
#define SYS_GPE_MFPL_PE0MFP_BPWM0_CH0       (0x4UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for BPWM0_CH0        */
#define SYS_GPE_MFPL_PE0MFP_SC0_nCD         (0x5UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for SC0_nCD          */
#define SYS_GPE_MFPL_PE0MFP_PWM0_CH0        (0x6UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for PWM0_CH0         */
#define SYS_GPE_MFPL_PE0MFP_EBI_nCS1        (0x7UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for EBI_nCS1         */
#define SYS_GPE_MFPL_PE0MFP_INT4            (0x8UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for INT4             */
#define SYS_GPE_MFPL_PE0MFP_UART4_RXD       (0x9UL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for UART4_RXD        */
#define SYS_GPE_MFPL_PE0MFP_I2S0_BCLK       (0xAUL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for I2S0_BCLK        */
#define SYS_GPE_MFPL_PE0MFP_PWM0_CH5        (0xBUL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for PWM0_CH5         */
#define SYS_GPE_MFPL_PE0MFP_SPI0_CLK        (0xCUL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for SPI0_CLK         */
#define SYS_GPE_MFPL_PE0MFP_TM2_EXT         (0xDUL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for TM2_EXT          */
#define SYS_GPE_MFPL_PE0MFP_ECAP0_IC2       (0xEUL<<SYS_GPE_MFPL_PE0MFP_Pos)    /*!< GPE_MFPL PE0 setting for ECAP0_IC2        */

//PE.1 MFP
#define SYS_GPE_MFPL_PE1MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE1MFP_Pos)    /*!< GPE_MFPL PE1 setting for GPIO             */
#define SYS_GPE_MFPL_PE1MFP_UART1_RXD       (0x1UL<<SYS_GPE_MFPL_PE1MFP_Pos)    /*!< GPE_MFPL PE1 setting for UART1_RXD        */
#define SYS_GPE_MFPL_PE1MFP_TM3_EXT         (0x3UL<<SYS_GPE_MFPL_PE1MFP_Pos)    /*!< GPE_MFPL PE1 setting for TM3_EXT          */
#define SYS_GPE_MFPL_PE1MFP_SC0_nCD         (0x5UL<<SYS_GPE_MFPL_PE1MFP_Pos)    /*!< GPE_MFPL PE1 setting for SC0_nCD          */
#define SYS_GPE_MFPL_PE1MFP_PWM0_CH1        (0x6UL<<SYS_GPE_MFPL_PE1MFP_Pos)    /*!< GPE_MFPL PE1 setting for PWM0_CH1         */
#define SYS_GPE_MFPL_PE1MFP_EBI_ADR7        (0x7UL<<SYS_GPE_MFPL_PE1MFP_Pos)    /*!< GPE_MFPL PE1 setting for EBI_ADR7         */
#define SYS_GPE_MFPL_PE1MFP_DSRC_TXON       (0xEUL<<SYS_GPE_MFPL_PE1MFP_Pos)    /*!< GPE_MFPL PE1 setting for DSRC_TXON        */

//PE.2 MFP
#define SYS_GPE_MFPL_PE2MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE2MFP_Pos)    /*!< GPE_MFPL PE2 setting for GPIO             */
#define SYS_GPE_MFPL_PE2MFP_PWM1_CH1        (0x6UL<<SYS_GPE_MFPL_PE2MFP_Pos)    /*!< GPE_MFPL PE2 setting for PWM1_CH1         */
#define SYS_GPE_MFPL_PE2MFP_EBI_ADR9        (0x7UL<<SYS_GPE_MFPL_PE2MFP_Pos)    /*!< GPE_MFPL PE2 setting for EBI_ADR9         */
#define SYS_GPE_MFPL_PE2MFP_SPI3_MISO       (0x9UL<<SYS_GPE_MFPL_PE2MFP_Pos)    /*!< GPE_MFPL PE2 setting for SPI3_MISO        */

//PE.3 MFP
#define SYS_GPE_MFPL_PE3MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE3MFP_Pos)    /*!< GPE_MFPL PE3 setting for GPIO             */
#define SYS_GPE_MFPL_PE3MFP_SPI1_MOSI       (0x2UL<<SYS_GPE_MFPL_PE3MFP_Pos)    /*!< GPE_MFPL PE3 setting for SPI1_MOSI        */
#define SYS_GPE_MFPL_PE3MFP_PWM0_CH3        (0x6UL<<SYS_GPE_MFPL_PE3MFP_Pos)    /*!< GPE_MFPL PE3 setting for PWM0_CH3         */
#define SYS_GPE_MFPL_PE3MFP_EBI_AD13        (0x7UL<<SYS_GPE_MFPL_PE3MFP_Pos)    /*!< GPE_MFPL PE3 setting for EBI_AD13         */
#define SYS_GPE_MFPL_PE3MFP_SD0_nCD         (0xEUL<<SYS_GPE_MFPL_PE3MFP_Pos)    /*!< GPE_MFPL PE3 setting for SD0_nCD          */

//PE.4 MFP
#define SYS_GPE_MFPL_PE4MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE4MFP_Pos)    /*!< GPE_MFPL PE4 setting for GPIO             */
#define SYS_GPE_MFPL_PE4MFP_I2C1_SCL        (0x3UL<<SYS_GPE_MFPL_PE4MFP_Pos)    /*!< GPE_MFPL PE4 setting for I2C1_SCL         */
#define SYS_GPE_MFPL_PE4MFP_SC0_PWR         (0x4UL<<SYS_GPE_MFPL_PE4MFP_Pos)    /*!< GPE_MFPL PE4 setting for SC0_PWR          */
#define SYS_GPE_MFPL_PE4MFP_PWM1_BRAKE0     (0x6UL<<SYS_GPE_MFPL_PE4MFP_Pos)    /*!< GPE_MFPL PE4 setting for PWM1_BRAKE0      */
#define SYS_GPE_MFPL_PE4MFP_EBI_nCS0        (0x7UL<<SYS_GPE_MFPL_PE4MFP_Pos)    /*!< GPE_MFPL PE4 setting for EBI_nCS0         */
#define SYS_GPE_MFPL_PE4MFP_INT0            (0x8UL<<SYS_GPE_MFPL_PE4MFP_Pos)    /*!< GPE_MFPL PE4 setting for INT0             */
#define SYS_GPE_MFPL_PE4MFP_SPI5_SS         (0xCUL<<SYS_GPE_MFPL_PE4MFP_Pos)    /*!< GPE_MFPL PE4 setting for SPI5_SS          */

//PE.5 MFP
#define SYS_GPE_MFPL_PE5MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE5MFP_Pos)    /*!< GPE_MFPL PE5 setting for GPIO             */
#define SYS_GPE_MFPL_PE5MFP_I2C1_SDA        (0x3UL<<SYS_GPE_MFPL_PE5MFP_Pos)    /*!< GPE_MFPL PE5 setting for I2C1_SDA         */
#define SYS_GPE_MFPL_PE5MFP_SC0_RST         (0x4UL<<SYS_GPE_MFPL_PE5MFP_Pos)    /*!< GPE_MFPL PE5 setting for SC0_RST          */
#define SYS_GPE_MFPL_PE5MFP_PWM1_BRAKE1     (0x6UL<<SYS_GPE_MFPL_PE5MFP_Pos)    /*!< GPE_MFPL PE5 setting for PWM1_BRAKE1      */
#define SYS_GPE_MFPL_PE5MFP_EBI_ALE         (0x7UL<<SYS_GPE_MFPL_PE5MFP_Pos)    /*!< GPE_MFPL PE5 setting for EBI_ALE          */
#define SYS_GPE_MFPL_PE5MFP_INT1            (0x8UL<<SYS_GPE_MFPL_PE5MFP_Pos)    /*!< GPE_MFPL PE5 setting for INT1             */
#define SYS_GPE_MFPL_PE5MFP_SPI5_CLK        (0xCUL<<SYS_GPE_MFPL_PE5MFP_Pos)    /*!< GPE_MFPL PE5 setting for SPI5_CLK         */

//PE.6 MFP
#define SYS_GPE_MFPL_PE6MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE6MFP_Pos)    /*!< GPE_MFPL PE6 setting for GPIO             */
#define SYS_GPE_MFPL_PE6MFP_TM3_EXT         (0x3UL<<SYS_GPE_MFPL_PE6MFP_Pos)    /*!< GPE_MFPL PE6 setting for TM3_EXT          */

//PE.7 MFP
#define SYS_GPE_MFPL_PE7MFP_GPIO            (0x0UL<<SYS_GPE_MFPL_PE7MFP_Pos)    /*!< GPE_MFPL PE7 setting for GPIO             */
#define SYS_GPE_MFPL_PE7MFP_EBI_MCLK        (0x7UL<<SYS_GPE_MFPL_PE7MFP_Pos)    /*!< GPE_MFPL PE7 setting for EBI_MCLK         */
#define SYS_GPE_MFPL_PE7MFP_PWM1_BRAKE1     (0xDUL<<SYS_GPE_MFPL_PE7MFP_Pos)    /*!< GPE_MFPL PE7 setting for PWM1_BRAKE1      */

//PE.8 MFP
#define SYS_GPE_MFPH_PE8MFP_GPIO            (0x0UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for GPIO             */
#define SYS_GPE_MFPH_PE8MFP_UART1_TXD       (0x1UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for UART1_TXD        */
#define SYS_GPE_MFPH_PE8MFP_SPI0_MISO1      (0x2UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for SPI0_MISO1       */
#define SYS_GPE_MFPH_PE8MFP_I2C1_SCL        (0x4UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for I2C1_SCL         */
#define SYS_GPE_MFPH_PE8MFP_SC0_PWR         (0x5UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for SC0_PWR          */
#define SYS_GPE_MFPH_PE8MFP_UART4_nRTS      (0x6UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for UART4_nRTS       */
#define SYS_GPE_MFPH_PE8MFP_EBI_ADR6        (0x7UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for EBI_ADR6         */
#define SYS_GPE_MFPH_PE8MFP_CLKO            (0x9UL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for CLKO             */
#define SYS_GPE_MFPH_PE8MFP_PWM0_BRAKE0     (0xAUL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for PWM0_BRAKE0      */
#define SYS_GPE_MFPH_PE8MFP_TM1             (0xBUL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for TM1              */
#define SYS_GPE_MFPH_PE8MFP_BPWM1_CH0       (0xDUL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for BPWM1_CH0        */
#define SYS_GPE_MFPH_PE8MFP_DSRC_RXON       (0xEUL<<SYS_GPE_MFPH_PE8MFP_Pos)    /*!< GPE_MFPH PE8 setting for DSRC_RXON        */

//PE.9 MFP
#define SYS_GPE_MFPH_PE9MFP_GPIO            (0x0UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for GPIO             */
#define SYS_GPE_MFPH_PE9MFP_UART1_RXD       (0x1UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for UART1_RXD        */
#define SYS_GPE_MFPH_PE9MFP_SPI0_MOSI1      (0x2UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for SPI0_MOSI1       */
#define SYS_GPE_MFPH_PE9MFP_I2C1_SDA        (0x4UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for I2C1_SDA         */
#define SYS_GPE_MFPH_PE9MFP_SC0_RST         (0x5UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for SC0_RST          */
#define SYS_GPE_MFPH_PE9MFP_UART4_nCTS      (0x6UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for UART4_nCTS       */
#define SYS_GPE_MFPH_PE9MFP_EBI_ADR5        (0x7UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for EBI_ADR5         */
#define SYS_GPE_MFPH_PE9MFP_I2S0_BCLK       (0x8UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for I2S0_BCLK        */
#define SYS_GPE_MFPH_PE9MFP_SPI1_I2SMCLK    (0x9UL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for SPI1_I2SMCLK     */
#define SYS_GPE_MFPH_PE9MFP_PWM1_BRAKE1     (0xAUL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for PWM1_BRAKE1      */
#define SYS_GPE_MFPH_PE9MFP_TM2             (0xBUL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for TM2              */
#define SYS_GPE_MFPH_PE9MFP_USCI1_CTL1      (0xCUL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for USCI1_CTL1       */
#define SYS_GPE_MFPH_PE9MFP_BPWM1_CH1       (0xDUL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for BPWM1_CH1        */
#define SYS_GPE_MFPH_PE9MFP_DSRC_WAKEUP     (0xEUL<<SYS_GPE_MFPH_PE9MFP_Pos)    /*!< GPE_MFPH PE9 setting for DSRC_WAKEUP      */

//PE.10 MFP
#define SYS_GPE_MFPH_PE10MFP_GPIO           (0x0UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for GPIO            */
#define SYS_GPE_MFPH_PE10MFP_SPI1_MISO      (0x1UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for SPI1_MISO       */
#define SYS_GPE_MFPH_PE10MFP_SPI0_MISO0     (0x2UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for SPI0_MISO0      */
#define SYS_GPE_MFPH_PE10MFP_UART1_nCTS     (0x3UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for UART1_nCTS      */
#define SYS_GPE_MFPH_PE10MFP_I2C0_SMBAL     (0x4UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for I2C0_SMBAL      */
#define SYS_GPE_MFPH_PE10MFP_SC0_DAT        (0x5UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for SC0_DAT         */
#define SYS_GPE_MFPH_PE10MFP_PWM1_CH0       (0x6UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for PWM1_CH0        */
#define SYS_GPE_MFPH_PE10MFP_EBI_ADR4       (0x7UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for EBI_ADR4        */
#define SYS_GPE_MFPH_PE10MFP_I2S0_MCLK      (0x8UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for I2S0_MCLK       */
#define SYS_GPE_MFPH_PE10MFP_UART3_TXD      (0x9UL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for UART3_TXD       */
#define SYS_GPE_MFPH_PE10MFP_I2C1_SCL       (0xBUL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for I2C1_SCL        */
#define SYS_GPE_MFPH_PE10MFP_USCI1_CTL0     (0xCUL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for USCI1_CTL0      */
#define SYS_GPE_MFPH_PE10MFP_BPWM1_CH2      (0xDUL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for BPWM1_CH2       */
#define SYS_GPE_MFPH_PE10MFP_SPI5_SS        (0xEUL<<SYS_GPE_MFPH_PE10MFP_Pos)   /*!< GPE_MFPH PE10 setting for SPI5_SS         */

//PE.11 MFP
#define SYS_GPE_MFPH_PE11MFP_GPIO           (0x0UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for GPIO            */
#define SYS_GPE_MFPH_PE11MFP_SPI1_MOSI      (0x1UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for SPI1_MOSI       */
#define SYS_GPE_MFPH_PE11MFP_SPI0_MOSI0     (0x2UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for SPI0_MOSI0      */
#define SYS_GPE_MFPH_PE11MFP_UART1_nRTS     (0x3UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for UART1_nRTS      */
#define SYS_GPE_MFPH_PE11MFP_I2C0_SMBSUS    (0x4UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for I2C0_SMBSUS     */
#define SYS_GPE_MFPH_PE11MFP_SC0_CLK        (0x5UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for SC0_CLK         */
#define SYS_GPE_MFPH_PE11MFP_PWM1_CH1       (0x6UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for PWM1_CH1        */
#define SYS_GPE_MFPH_PE11MFP_EBI_ADR3       (0x7UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for EBI_ADR3        */
#define SYS_GPE_MFPH_PE11MFP_I2S0_DI        (0x8UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for I2S0_DI         */
#define SYS_GPE_MFPH_PE11MFP_UART3_RXD      (0x9UL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for UART3_RXD       */
#define SYS_GPE_MFPH_PE11MFP_I2C1_SDA       (0xBUL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for I2C1_SDA        */
#define SYS_GPE_MFPH_PE11MFP_USCI1_DAT0     (0xCUL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for USCI1_DAT0      */
#define SYS_GPE_MFPH_PE11MFP_BPWM1_CH3      (0xDUL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for BPWM1_CH3       */
#define SYS_GPE_MFPH_PE11MFP_SPI5_CLK       (0xEUL<<SYS_GPE_MFPH_PE11MFP_Pos)   /*!< GPE_MFPH PE11 setting for SPI5_CLK        */

//PE.12 MFP
#define SYS_GPE_MFPH_PE12MFP_GPIO           (0x0UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for GPIO            */
#define SYS_GPE_MFPH_PE12MFP_SPI1_SS        (0x1UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for SPI1_SS         */
#define SYS_GPE_MFPH_PE12MFP_SPI0_SS        (0x2UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for SPI0_SS         */
#define SYS_GPE_MFPH_PE12MFP_UART1_TXD      (0x3UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for UART1_TXD       */
#define SYS_GPE_MFPH_PE12MFP_I2C0_SCL       (0x4UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for I2C0_SCL        */
#define SYS_GPE_MFPH_PE12MFP_SC0_PWR        (0x5UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for SC0_PWR         */
#define SYS_GPE_MFPH_PE12MFP_UART4_TXD      (0x6UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for UART4_TXD       */
#define SYS_GPE_MFPH_PE12MFP_EBI_ADR2       (0x7UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for EBI_ADR2        */
#define SYS_GPE_MFPH_PE12MFP_I2S0_DO        (0x8UL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for I2S0_DO         */
#define SYS_GPE_MFPH_PE12MFP_PWM0_CH4       (0xAUL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for PWM0_CH4        */
#define SYS_GPE_MFPH_PE12MFP_SD0_CMD        (0xBUL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for SD0_CMD         */
#define SYS_GPE_MFPH_PE12MFP_USCI1_DAT1     (0xCUL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for USCI1_DAT1      */
#define SYS_GPE_MFPH_PE12MFP_BPWM1_CH4      (0xDUL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for BPWM1_CH4       */
#define SYS_GPE_MFPH_PE12MFP_SPI5_MISO      (0xEUL<<SYS_GPE_MFPH_PE12MFP_Pos)   /*!< GPE_MFPH PE12 setting for SPI5_MISO       */

//PE.13 MFP
#define SYS_GPE_MFPH_PE13MFP_GPIO           (0x0UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for GPIO            */
#define SYS_GPE_MFPH_PE13MFP_SPI1_CLK       (0x1UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for SPI1_CLK        */
#define SYS_GPE_MFPH_PE13MFP_SPI0_CLK       (0x2UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for SPI0_CLK        */
#define SYS_GPE_MFPH_PE13MFP_UART1_RXD      (0x3UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for UART1_RXD       */
#define SYS_GPE_MFPH_PE13MFP_I2C0_SDA       (0x4UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for I2C0_SDA        */
#define SYS_GPE_MFPH_PE13MFP_SC0_RST        (0x5UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for SC0_RST         */
#define SYS_GPE_MFPH_PE13MFP_UART4_RXD      (0x6UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for UART4_RXD       */
#define SYS_GPE_MFPH_PE13MFP_EBI_ADR1       (0x7UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for EBI_ADR1        */
#define SYS_GPE_MFPH_PE13MFP_I2S0_LRCK      (0x8UL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for I2S0_LRCK       */
#define SYS_GPE_MFPH_PE13MFP_PWM0_CH5       (0xAUL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for PWM0_CH5        */
#define SYS_GPE_MFPH_PE13MFP_SD0_nCD        (0xBUL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for SD0_nCD         */
#define SYS_GPE_MFPH_PE13MFP_USCI1_CLK      (0xCUL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for USCI1_CLK       */
#define SYS_GPE_MFPH_PE13MFP_BPWM1_CH5      (0xDUL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for BPWM1_CH5       */
#define SYS_GPE_MFPH_PE13MFP_SPI5_MOSI      (0xEUL<<SYS_GPE_MFPH_PE13MFP_Pos)   /*!< GPE_MFPH PE13 setting for SPI5_MOSI       */

//PF.0 MFP
#define SYS_GPF_MFPL_PF0MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF0MFP_Pos)    /*!< GPF_MFPL PF0 setting for GPIO             */
#define SYS_GPF_MFPL_PF0MFP_X32_OUT         (0x1UL<<SYS_GPF_MFPL_PF0MFP_Pos)    /*!< GPF_MFPL PF0 setting for X32_OUT          */
#define SYS_GPF_MFPL_PF0MFP_SPI1_SS         (0x2UL<<SYS_GPF_MFPL_PF0MFP_Pos)    /*!< GPF_MFPL PF0 setting for SPI1_SS          */
#define SYS_GPF_MFPL_PF0MFP_TM3_EXT         (0x4UL<<SYS_GPF_MFPL_PF0MFP_Pos)    /*!< GPF_MFPL PF0 setting for TM3_EXT          */
#define SYS_GPF_MFPL_PF0MFP_INT5            (0x8UL<<SYS_GPF_MFPL_PF0MFP_Pos)    /*!< GPF_MFPL PF0 setting for INT5             */

//PF.1 MFP
#define SYS_GPF_MFPL_PF1MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF1MFP_Pos)    /*!< GPF_MFPL PF1 setting for GPIO             */
#define SYS_GPF_MFPL_PF1MFP_X32_IN          (0x1UL<<SYS_GPF_MFPL_PF1MFP_Pos)    /*!< GPF_MFPL PF1 setting for X32_IN           */
#define SYS_GPF_MFPL_PF1MFP_SPI1_MOSI       (0x2UL<<SYS_GPF_MFPL_PF1MFP_Pos)    /*!< GPF_MFPL PF1 setting for SPI1_MOSI        */
#define SYS_GPF_MFPL_PF1MFP_TM3             (0x4UL<<SYS_GPF_MFPL_PF1MFP_Pos)    /*!< GPF_MFPL PF1 setting for TM3              */
#define SYS_GPF_MFPL_PF1MFP_DSRC_TXON       (0x9UL<<SYS_GPF_MFPL_PF1MFP_Pos)    /*!< GPF_MFPL PF1 setting for DSRC_TXON        */

//PF.2 MFP
#define SYS_GPF_MFPL_PF2MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for GPIO             */
#define SYS_GPF_MFPL_PF2MFP_TAMPER0         (0x1UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for TAMPER0          */
#define SYS_GPF_MFPL_PF2MFP_DAC0_ST         (0x2UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for DAC0_ST          */
#define SYS_GPF_MFPL_PF2MFP_DAC1_ST         (0x3UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for DAC1_ST          */
#define SYS_GPF_MFPL_PF2MFP_TM2_EXT         (0x4UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for TM2_EXT          */
#define SYS_GPF_MFPL_PF2MFP_SPI1_CLK        (0x5UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for SPI1_CLK         */
#define SYS_GPF_MFPL_PF2MFP_PWM0_CH5        (0x6UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for PWM0_CH5         */
#define SYS_GPF_MFPL_PF2MFP_EBI_nCS2        (0x7UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for EBI_nCS2         */
#define SYS_GPF_MFPL_PF2MFP_INT5            (0x8UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for INT5             */
#define SYS_GPF_MFPL_PF2MFP_DSRC_RXON       (0x9UL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for DSRC_RXON        */
#define SYS_GPF_MFPL_PF2MFP_SC0_nCD         (0xAUL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for SC0_nCD          */
#define SYS_GPF_MFPL_PF2MFP_SC1_nCD         (0xBUL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for SC1_nCD          */
#define SYS_GPF_MFPL_PF2MFP_QEI0_A          (0xCUL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for QEI0_A           */
#define SYS_GPF_MFPL_PF2MFP_ECAP1_IC2       (0xDUL<<SYS_GPF_MFPL_PF2MFP_Pos)    /*!< GPF_MFPL PF2 setting for ECAP1_IC2        */

//PF.3 MFP
#define SYS_GPF_MFPL_PF3MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF3MFP_Pos)    /*!< GPF_MFPL PF3 setting for GPIO             */
#define SYS_GPF_MFPL_PF3MFP_XT1_OUT         (0x1UL<<SYS_GPF_MFPL_PF3MFP_Pos)    /*!< GPF_MFPL PF3 setting for XT1_OUT          */
#define SYS_GPF_MFPL_PF3MFP_I2C1_SCL        (0x3UL<<SYS_GPF_MFPL_PF3MFP_Pos)    /*!< GPF_MFPL PF3 setting for I2C1_SCL         */
#define SYS_GPF_MFPL_PF3MFP_PWM1_CH4        (0x6UL<<SYS_GPF_MFPL_PF3MFP_Pos)    /*!< GPF_MFPL PF3 setting for PWM1_CH4         */

//PF.4 MFP
#define SYS_GPF_MFPL_PF4MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF4MFP_Pos)    /*!< GPF_MFPL PF4 setting for GPIO             */
#define SYS_GPF_MFPL_PF4MFP_XT1_IN          (0x1UL<<SYS_GPF_MFPL_PF4MFP_Pos)    /*!< GPF_MFPL PF4 setting for XT1_IN           */
#define SYS_GPF_MFPL_PF4MFP_I2C1_SDA        (0x3UL<<SYS_GPF_MFPL_PF4MFP_Pos)    /*!< GPF_MFPL PF4 setting for I2C1_SDA         */
#define SYS_GPF_MFPL_PF4MFP_PWM1_CH5        (0x6UL<<SYS_GPF_MFPL_PF4MFP_Pos)    /*!< GPF_MFPL PF4 setting for PWM1_CH5         */

//PF.5 MFP
#define SYS_GPF_MFPL_PF5MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for GPIO             */
#define SYS_GPF_MFPL_PF5MFP_ICE_CLK         (0x1UL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for ICE_CLK          */
#define SYS_GPF_MFPL_PF5MFP_UART3_TXD       (0x2UL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for UART3_TXD        */
#define SYS_GPF_MFPL_PF5MFP_UART0_RXD       (0x3UL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for UART0_RXD        */
#define SYS_GPF_MFPL_PF5MFP_I2C0_SCL        (0x4UL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for I2C0_SCL         */
#define SYS_GPF_MFPL_PF5MFP_SC0_DAT         (0x5UL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for SC0_DAT          */
#define SYS_GPF_MFPL_PF5MFP_SD0_nCD         (0xBUL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for SD0_nCD          */
#define SYS_GPF_MFPL_PF5MFP_BPWM1_CH0       (0xDUL<<SYS_GPF_MFPL_PF5MFP_Pos)    /*!< GPF_MFPL PF5 setting for BPWM1_CH0        */

//PF.6 MFP
#define SYS_GPF_MFPL_PF6MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for GPIO             */
#define SYS_GPF_MFPL_PF6MFP_ICE_DAT         (0x1UL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for ICE_DAT          */
#define SYS_GPF_MFPL_PF6MFP_UART3_RXD       (0x2UL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for UART3_RXD        */
#define SYS_GPF_MFPL_PF6MFP_UART0_TXD       (0x3UL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for UART0_TXD        */
#define SYS_GPF_MFPL_PF6MFP_I2C0_SDA        (0x4UL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for I2C0_SDA         */
#define SYS_GPF_MFPL_PF6MFP_SC0_CLK         (0x5UL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for SC0_CLK          */
#define SYS_GPF_MFPL_PF6MFP_SD0_CMD         (0xBUL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for SD0_CMD          */
#define SYS_GPF_MFPL_PF6MFP_BPWM1_CH1       (0xDUL<<SYS_GPF_MFPL_PF6MFP_Pos)    /*!< GPF_MFPL PF6 setting for BPWM1_CH1        */

//PF.7 MFP
#define SYS_GPF_MFPL_PF7MFP_GPIO            (0x0UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for GPIO             */
#define SYS_GPF_MFPL_PF7MFP_TAMPER1         (0x1UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for TAMPER1          */
#define SYS_GPF_MFPL_PF7MFP_SPI2_I2SMCLK    (0x2UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for SPI2_I2SMCLK     */
#define SYS_GPF_MFPL_PF7MFP_UART3_TXD       (0x3UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for UART3_TXD        */
#define SYS_GPF_MFPL_PF7MFP_TM1_EXT         (0x4UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for TM1_EXT          */
#define SYS_GPF_MFPL_PF7MFP_SPI1_MISO       (0x5UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for SPI1_MISO        */
#define SYS_GPF_MFPL_PF7MFP_PWM1_CH0        (0x6UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for PWM1_CH0         */
#define SYS_GPF_MFPL_PF7MFP_EBI_ADR16       (0x7UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for EBI_ADR16        */
#define SYS_GPF_MFPL_PF7MFP_INT4            (0x8UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for INT4             */
#define SYS_GPF_MFPL_PF7MFP_EBI_ALE         (0x9UL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for EBI_ALE          */
#define SYS_GPF_MFPL_PF7MFP_SC2_nCD         (0xAUL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for SC2_nCD          */
#define SYS_GPF_MFPL_PF7MFP_USCI0_CLK       (0xBUL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for USCI0_CLK        */
#define SYS_GPF_MFPL_PF7MFP_QEI0_B          (0xCUL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for QEI0_B           */
#define SYS_GPF_MFPL_PF7MFP_ECAP1_IC1       (0xDUL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for ECAP1_IC1        */
#define SYS_GPF_MFPL_PF7MFP_SD0_DAT0        (0xEUL<<SYS_GPF_MFPL_PF7MFP_Pos)    /*!< GPF_MFPL PF7 setting for SD0_DAT0         */

//PF.8 MFP
#define SYS_GPF_MFPH_PF8MFP_GPIO            (0x0UL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for GPIO             */
#define SYS_GPF_MFPH_PF8MFP_TAMPER2         (0x1UL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for TAMPER2          */
#define SYS_GPF_MFPH_PF8MFP_SPI2_CLK        (0x2UL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for SPI2_CLK         */
#define SYS_GPF_MFPH_PF8MFP_TM0_EXT         (0x4UL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for TM0_EXT          */
#define SYS_GPF_MFPH_PF8MFP_PWM1_BRAKE1     (0x5UL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for PWM1_BRAKE1      */
#define SYS_GPF_MFPH_PF8MFP_INT3            (0x8UL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for INT3             */
#define SYS_GPF_MFPH_PF8MFP_QEI0_INDEX      (0xCUL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for QEI0_INDEX       */
#define SYS_GPF_MFPH_PF8MFP_ECAP1_IC0       (0xDUL<<SYS_GPF_MFPH_PF8MFP_Pos)    /*!< GPF_MFPH PF8 setting for ECAP1_IC0        */

//PF.9 MFP
#define SYS_GPF_MFPH_PF9MFP_GPIO            (0x0UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for GPIO             */
#define SYS_GPF_MFPH_PF9MFP_TAMPER3         (0x1UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for TAMPER3          */
#define SYS_GPF_MFPH_PF9MFP_SPI2_SS         (0x2UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for SPI2_SS          */
#define SYS_GPF_MFPH_PF9MFP_UART3_TXD       (0x3UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for UART3_TXD        */
#define SYS_GPF_MFPH_PF9MFP_TM2             (0x4UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for TM2              */
#define SYS_GPF_MFPH_PF9MFP_PWM1_BRAKE0     (0x5UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for PWM1_BRAKE0      */
#define SYS_GPF_MFPH_PF9MFP_PWM1_CH0        (0x6UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for PWM1_CH0         */
#define SYS_GPF_MFPH_PF9MFP_EBI_ADR16       (0x7UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for EBI_ADR16        */
#define SYS_GPF_MFPH_PF9MFP_INT2            (0x8UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for INT2             */
#define SYS_GPF_MFPH_PF9MFP_EADC0_ST        (0x9UL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for EADC0_ST         */
#define SYS_GPF_MFPH_PF9MFP_SC1_RST         (0xBUL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for SC1_RST          */
#define SYS_GPF_MFPH_PF9MFP_QEI1_INDEX      (0xCUL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for QEI1_INDEX       */
#define SYS_GPF_MFPH_PF9MFP_ECAP0_IC2       (0xDUL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for ECAP0_IC2        */
#define SYS_GPF_MFPH_PF9MFP_SD0_DAT0        (0xEUL<<SYS_GPF_MFPH_PF9MFP_Pos)    /*!< GPF_MFPH PF9 setting for SD0_DAT0         */

//PF.10 MFP
#define SYS_GPF_MFPH_PF10MFP_GPIO           (0x0UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for GPIO            */
#define SYS_GPF_MFPH_PF10MFP_TAMPER4        (0x1UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for TAMPER4         */
#define SYS_GPF_MFPH_PF10MFP_SPI2_MOSI      (0x2UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for SPI2_MOSI       */
#define SYS_GPF_MFPH_PF10MFP_UART3_RXD      (0x3UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for UART3_RXD       */
#define SYS_GPF_MFPH_PF10MFP_TM1            (0x4UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for TM1             */
#define SYS_GPF_MFPH_PF10MFP_PWM0_BRAKE0    (0x5UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for PWM0_BRAKE0     */
#define SYS_GPF_MFPH_PF10MFP_PWM1_CH1       (0x6UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for PWM1_CH1        */
#define SYS_GPF_MFPH_PF10MFP_EBI_ADR17      (0x7UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for EBI_ADR17       */
#define SYS_GPF_MFPH_PF10MFP_INT1           (0x8UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for INT1            */
#define SYS_GPF_MFPH_PF10MFP_EBI_nRD        (0x9UL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for EBI_nRD         */
#define SYS_GPF_MFPH_PF10MFP_I2C2_SCL       (0xAUL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for I2C2_SCL        */
#define SYS_GPF_MFPH_PF10MFP_SC1_PWR        (0xBUL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for SC1_PWR         */
#define SYS_GPF_MFPH_PF10MFP_QEI1_A         (0xCUL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for QEI1_A          */
#define SYS_GPF_MFPH_PF10MFP_ECAP0_IC1      (0xDUL<<SYS_GPF_MFPH_PF10MFP_Pos)   /*!< GPF_MFPH PF10 setting for ECAP0_IC1       */

//PF.11 MFP
#define SYS_GPF_MFPH_PF11MFP_GPIO           (0x0UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for GPIO            */
#define SYS_GPF_MFPH_PF11MFP_TAMPER5        (0x1UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for TAMPER5         */
#define SYS_GPF_MFPH_PF11MFP_SPI2_MISO      (0x2UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for SPI2_MISO       */
#define SYS_GPF_MFPH_PF11MFP_UART3_nCTS     (0x3UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for UART3_nCTS      */
#define SYS_GPF_MFPH_PF11MFP_TM0            (0x4UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for TM0             */
#define SYS_GPF_MFPH_PF11MFP_PWM0_BRAKE1    (0x5UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for PWM0_BRAKE1     */
#define SYS_GPF_MFPH_PF11MFP_PWM1_CH2       (0x6UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for PWM1_CH2        */
#define SYS_GPF_MFPH_PF11MFP_EBI_ADR18      (0x7UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for EBI_ADR18       */
#define SYS_GPF_MFPH_PF11MFP_INT0           (0x8UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for INT0            */
#define SYS_GPF_MFPH_PF11MFP_EBI_nWR        (0x9UL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for EBI_nWR         */
#define SYS_GPF_MFPH_PF11MFP_I2C2_SDA       (0xAUL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for I2C2_SDA        */
#define SYS_GPF_MFPH_PF11MFP_SC1_nCD        (0xBUL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for SC1_nCD         */
#define SYS_GPF_MFPH_PF11MFP_QEI1_B         (0xCUL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for QEI1_B          */
#define SYS_GPF_MFPH_PF11MFP_ECAP0_IC0      (0xDUL<<SYS_GPF_MFPH_PF11MFP_Pos)   /*!< GPF_MFPH PF11 setting for ECAP0_IC0       */

//PF.12 MFP
#define SYS_GPF_MFPH_PF12MFP_GPIO           (0x0UL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for GPIO            */
#define SYS_GPF_MFPH_PF12MFP_USB_OTG_ID     (0x1UL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for USB _OTG_ID     */
#define SYS_GPF_MFPH_PF12MFP_SPI1_MISO      (0x2UL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for SPI1_MISO       */
#define SYS_GPF_MFPH_PF12MFP_UART5_RXD      (0x3UL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for UART5_RXD       */
#define SYS_GPF_MFPH_PF12MFP_I2C0_SCL       (0x4UL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for I2C0_SCL        */
#define SYS_GPF_MFPH_PF12MFP_CAN0_TXD       (0x5UL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for CAN0_TXD        */
#define SYS_GPF_MFPH_PF12MFP_USCI1_CTL0     (0xCUL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for USCI1_CTL0      */
#define SYS_GPF_MFPH_PF12MFP_PWM1_BRAKE0    (0xDUL<<SYS_GPF_MFPH_PF12MFP_Pos)   /*!< GPF_MFPH PF12 setting for PWM1_BRAKE0     */

//PF.13 MFP
#define SYS_GPF_MFPH_PF13MFP_GPIO           (0x0UL<<SYS_GPF_MFPH_PF13MFP_Pos)   /*!< GPF_MFPH PF13 setting for GPIO            */
#define SYS_GPF_MFPH_PF13MFP_USB_D_P        (0x1UL<<SYS_GPF_MFPH_PF13MFP_Pos)   /*!< GPF_MFPH PF13 setting for USB_D_P        */
#define SYS_GPF_MFPH_PF13MFP_SPI1_MOSI      (0x2UL<<SYS_GPF_MFPH_PF13MFP_Pos)   /*!< GPF_MFPH PF13 setting for SPI1_MOSI       */
#define SYS_GPF_MFPH_PF13MFP_UART5_TXD      (0x3UL<<SYS_GPF_MFPH_PF13MFP_Pos)   /*!< GPF_MFPH PF13 setting for UART5_TXD       */
#define SYS_GPF_MFPH_PF13MFP_I2C0_SDA       (0x4UL<<SYS_GPF_MFPH_PF13MFP_Pos)   /*!< GPF_MFPH PF13 setting for I2C0_SDA        */
#define SYS_GPF_MFPH_PF13MFP_CAN0_RXD       (0x5UL<<SYS_GPF_MFPH_PF13MFP_Pos)   /*!< GPF_MFPH PF13 setting for CAN0_RXD        */
#define SYS_GPF_MFPH_PF13MFP_USCI1_DAT0     (0xCUL<<SYS_GPF_MFPH_PF13MFP_Pos)   /*!< GPF_MFPH PF13 setting for USCI1_DAT0      */

//PF.14 MFP
#define SYS_GPF_MFPH_PF14MFP_GPIO           (0x0UL<<SYS_GPF_MFPH_PF14MFP_Pos)   /*!< GPF_MFPH PF14 setting for GPIO            */
#define SYS_GPF_MFPH_PF14MFP_USB_D_N        (0x1UL<<SYS_GPF_MFPH_PF14MFP_Pos)   /*!< GPF_MFPH PF14 setting for USB_D_N         */
#define SYS_GPF_MFPH_PF14MFP_SPI1_SS        (0x2UL<<SYS_GPF_MFPH_PF14MFP_Pos)   /*!< GPF_MFPH PF14 setting for SPI1_SS         */
#define SYS_GPF_MFPH_PF14MFP_UART5_nCTS     (0x3UL<<SYS_GPF_MFPH_PF14MFP_Pos)   /*!< GPF_MFPH PF14 setting for UART5_nCTS      */
#define SYS_GPF_MFPH_PF14MFP_I2C1_SCL       (0x4UL<<SYS_GPF_MFPH_PF14MFP_Pos)   /*!< GPF_MFPH PF14 setting for I2C1_SCL        */
#define SYS_GPF_MFPH_PF14MFP_USCI1_DAT1     (0xCUL<<SYS_GPF_MFPH_PF14MFP_Pos)   /*!< GPF_MFPH PF14 setting for USCI1_DAT1      */

//PF.15 MFP
#define SYS_GPF_MFPH_PF15MFP_GPIO           (0x0UL<<SYS_GPF_MFPH_PF15MFP_Pos)   /*!< GPF_MFPH PF15 setting for GPIO            */
#define SYS_GPF_MFPH_PF15MFP_USB_VBUS       (0x1UL<<SYS_GPF_MFPH_PF15MFP_Pos)   /*!< GPF_MFPH PF15 setting for USB_VBUS        */
#define SYS_GPF_MFPH_PF15MFP_SPI1_CLK       (0x2UL<<SYS_GPF_MFPH_PF15MFP_Pos)   /*!< GPF_MFPH PF15 setting for SPI1_CLK        */
#define SYS_GPF_MFPH_PF15MFP_UART5_nRTS     (0x3UL<<SYS_GPF_MFPH_PF15MFP_Pos)   /*!< GPF_MFPH PF15 setting for UART5_nRTS      */
#define SYS_GPF_MFPH_PF15MFP_I2C1_SDA       (0x4UL<<SYS_GPF_MFPH_PF15MFP_Pos)   /*!< GPF_MFPH PF15 setting for I2C1_SDA        */
#define SYS_GPF_MFPH_PF15MFP_USCI1_CLK      (0xCUL<<SYS_GPF_MFPH_PF15MFP_Pos)   /*!< GPF_MFPH PF15 setting for USCI1_CLK       */

//PG.1 MFP
#define SYS_GPG_MFPL_PG1MFP_GPIO            (0x0UL<<SYS_GPG_MFPL_PG1MFP_Pos)    /*!< GPG_MFPL PG1 setting for GPIO             */
#define SYS_GPG_MFPL_PG1MFP_SPI0_MISO0      (0x2UL<<SYS_GPG_MFPL_PG1MFP_Pos)    /*!< GPG_MFPL PG1 setting for SPI0_MISO0       */
#define SYS_GPG_MFPL_PG1MFP_UART2_RXD       (0x3UL<<SYS_GPG_MFPL_PG1MFP_Pos)    /*!< GPG_MFPL PG1 setting for UART2_RXD        */
#define SYS_GPG_MFPL_PG1MFP_PWM1_CH2        (0x6UL<<SYS_GPG_MFPL_PG1MFP_Pos)    /*!< GPG_MFPL PG1 setting for PWM1_CH2         */
#define SYS_GPG_MFPL_PG1MFP_EBI_AD7         (0x7UL<<SYS_GPG_MFPL_PG1MFP_Pos)    /*!< GPG_MFPL PG1 setting for EBI_AD7          */
#define SYS_GPG_MFPL_PG1MFP_TM2             (0xBUL<<SYS_GPG_MFPL_PG1MFP_Pos)    /*!< GPG_MFPL PG1 setting for TM2              */

//PG.2 MFP
#define SYS_GPG_MFPL_PG2MFP_GPIO            (0x0UL<<SYS_GPG_MFPL_PG2MFP_Pos)    /*!< GPG_MFPL PG2 setting for GPIO             */
#define SYS_GPG_MFPL_PG2MFP_SPI0_MOSI0      (0x2UL<<SYS_GPG_MFPL_PG2MFP_Pos)    /*!< GPG_MFPL PG2 setting for SPI0_MOSI0       */
#define SYS_GPG_MFPL_PG2MFP_UART2_TXD       (0x3UL<<SYS_GPG_MFPL_PG2MFP_Pos)    /*!< GPG_MFPL PG2 setting for UART2_TXD        */
#define SYS_GPG_MFPL_PG2MFP_PWM1_CH3        (0x6UL<<SYS_GPG_MFPL_PG2MFP_Pos)    /*!< GPG_MFPL PG2 setting for PWM1_CH3         */
#define SYS_GPG_MFPL_PG2MFP_EBI_AD6         (0x7UL<<SYS_GPG_MFPL_PG2MFP_Pos)    /*!< GPG_MFPL PG2 setting for EBI_AD6          */
#define SYS_GPG_MFPL_PG2MFP_INT3            (0x8UL<<SYS_GPG_MFPL_PG2MFP_Pos)    /*!< GPG_MFPL PG2 setting for INT3             */
#define SYS_GPG_MFPL_PG2MFP_TM3_EXT         (0xBUL<<SYS_GPG_MFPL_PG2MFP_Pos)    /*!< GPG_MFPL PG2 setting for TM3_EXT          */

//PG.3 MFP
#define SYS_GPG_MFPL_PG3MFP_GPIO            (0x0UL<<SYS_GPG_MFPL_PG3MFP_Pos)    /*!< GPG_MFPL PG3 setting for GPIO             */
#define SYS_GPG_MFPL_PG3MFP_SPI0_CLK        (0x2UL<<SYS_GPG_MFPL_PG3MFP_Pos)    /*!< GPG_MFPL PG3 setting for SPI0_CLK         */
#define SYS_GPG_MFPL_PG3MFP_UART2_nCTS      (0x3UL<<SYS_GPG_MFPL_PG3MFP_Pos)    /*!< GPG_MFPL PG3 setting for UART2_nCTS       */
#define SYS_GPG_MFPL_PG3MFP_PWM1_CH4        (0x6UL<<SYS_GPG_MFPL_PG3MFP_Pos)    /*!< GPG_MFPL PG3 setting for PWM1_CH4         */
#define SYS_GPG_MFPL_PG3MFP_EBI_AD5         (0x7UL<<SYS_GPG_MFPL_PG3MFP_Pos)    /*!< GPG_MFPL PG3 setting for EBI_AD5          */
#define SYS_GPG_MFPL_PG3MFP_INT4            (0x8UL<<SYS_GPG_MFPL_PG3MFP_Pos)    /*!< GPG_MFPL PG3 setting for INT4             */
#define SYS_GPG_MFPL_PG3MFP_TM3             (0xBUL<<SYS_GPG_MFPL_PG3MFP_Pos)    /*!< GPG_MFPL PG3 setting for TM3              */

//PG.4 MFP
#define SYS_GPG_MFPL_PG4MFP_GPIO            (0x0UL<<SYS_GPG_MFPL_PG4MFP_Pos)    /*!< GPG_MFPL PG4 setting for GPIO             */
#define SYS_GPG_MFPL_PG4MFP_SPI0_SS         (0x2UL<<SYS_GPG_MFPL_PG4MFP_Pos)    /*!< GPG_MFPL PG4 setting for SPI0_SS          */
#define SYS_GPG_MFPL_PG4MFP_UART2_nRTS      (0x3UL<<SYS_GPG_MFPL_PG4MFP_Pos)    /*!< GPG_MFPL PG4 setting for UART2_nRTS       */
#define SYS_GPG_MFPL_PG4MFP_PWM1_CH5        (0x6UL<<SYS_GPG_MFPL_PG4MFP_Pos)    /*!< GPG_MFPL PG4 setting for PWM1_CH5         */
#define SYS_GPG_MFPL_PG4MFP_EBI_AD4         (0x7UL<<SYS_GPG_MFPL_PG4MFP_Pos)    /*!< GPG_MFPL PG4 setting for EBI_AD4          */
#define SYS_GPG_MFPL_PG4MFP_INT5            (0x8UL<<SYS_GPG_MFPL_PG4MFP_Pos)    /*!< GPG_MFPL PG4 setting for INT5             */

//PG.5 MFP
#define SYS_GPG_MFPL_PG5MFP_GPIO            (0x0UL<<SYS_GPG_MFPL_PG5MFP_Pos)    /*!< GPG_MFPL PG5 setting for GPIO             */

//PG.7 MFP
#define SYS_GPG_MFPL_PG7MFP_GPIO            (0x0UL<<SYS_GPG_MFPL_PG7MFP_Pos)    /*!< GPG_MFPL PG7 setting for GPIO             */
#define SYS_GPG_MFPL_PG7MFP_UART0_RXD       (0x3UL<<SYS_GPG_MFPL_PG7MFP_Pos)    /*!< GPG_MFPL PG7 setting for UART0_RXD        */
#define SYS_GPG_MFPL_PG7MFP_EBI_nRD         (0x7UL<<SYS_GPG_MFPL_PG7MFP_Pos)    /*!< GPG_MFPL PG7 setting for EBI_nRD          */
#define SYS_GPG_MFPL_PG7MFP_SC1_CLK         (0xBUL<<SYS_GPG_MFPL_PG7MFP_Pos)    /*!< GPG_MFPL PG7 setting for SC1_CLK          */
#define SYS_GPG_MFPL_PG7MFP_USCI0_DAT0      (0xCUL<<SYS_GPG_MFPL_PG7MFP_Pos)    /*!< GPG_MFPL PG7 setting for USCI0_DAT0       */
#define SYS_GPG_MFPL_PG7MFP_SD0_CLK         (0xEUL<<SYS_GPG_MFPL_PG7MFP_Pos)    /*!< GPG_MFPL PG7 setting for SD0_CLK          */

//PG.8 MFP
#define SYS_GPG_MFPH_PG8MFP_GPIO            (0x0UL<<SYS_GPG_MFPH_PG8MFP_Pos)    /*!< GPG_MFPH PG8 setting for GPIO             */
#define SYS_GPG_MFPH_PG8MFP_SPI1_SS         (0x1UL<<SYS_GPG_MFPH_PG8MFP_Pos)    /*!< GPG_MFPH PG8 setting for SPI1_SS          */
#define SYS_GPG_MFPH_PG8MFP_UART0_TXD       (0x3UL<<SYS_GPG_MFPH_PG8MFP_Pos)    /*!< GPG_MFPH PG8 setting for UART0_TXD        */
#define SYS_GPG_MFPH_PG8MFP_EBI_nWR         (0x7UL<<SYS_GPG_MFPH_PG8MFP_Pos)    /*!< GPG_MFPH PG8 setting for EBI_nWR          */
#define SYS_GPG_MFPH_PG8MFP_SC1_DAT         (0xBUL<<SYS_GPG_MFPH_PG8MFP_Pos)    /*!< GPG_MFPH PG8 setting for SC1_DAT          */
#define SYS_GPG_MFPH_PG8MFP_USCI0_DAT1      (0xCUL<<SYS_GPG_MFPH_PG8MFP_Pos)    /*!< GPG_MFPH PG8 setting for USCI0_DAT1       */
#define SYS_GPG_MFPH_PG8MFP_SD0_DAT3        (0xEUL<<SYS_GPG_MFPH_PG8MFP_Pos)    /*!< GPG_MFPH PG8 setting for SD0_DAT3         */

//PG.9 MFP
#define SYS_GPG_MFPH_PG9MFP_GPIO            (0x0UL<<SYS_GPG_MFPH_PG9MFP_Pos)    /*!< GPG_MFPH PG9 setting for GPIO             */
#define SYS_GPG_MFPH_PG9MFP_UART5_RXD       (0x6UL<<SYS_GPG_MFPH_PG9MFP_Pos)    /*!< GPG_MFPH PG9 setting for UART5_RXD        */
#define SYS_GPG_MFPH_PG9MFP_QEI0_A          (0xCUL<<SYS_GPG_MFPH_PG9MFP_Pos)    /*!< GPG_MFPH PG9 setting for QEI0_A           */

//PG.10 MFP
#define SYS_GPG_MFPH_PG10MFP_GPIO           (0x0UL<<SYS_GPG_MFPH_PG10MFP_Pos)   /*!< GPG_MFPH PG10 setting for GPIO            */
#define SYS_GPG_MFPH_PG10MFP_UART5_TXD      (0x6UL<<SYS_GPG_MFPH_PG10MFP_Pos)   /*!< GPG_MFPH PG10 setting for UART5_TXD       */
#define SYS_GPG_MFPH_PG10MFP_QEI0_B         (0xCUL<<SYS_GPG_MFPH_PG10MFP_Pos)   /*!< GPG_MFPH PG10 setting for QEI0_B          */

//PG.11 MFP
#define SYS_GPG_MFPH_PG11MFP_GPIO           (0x0UL<<SYS_GPG_MFPH_PG11MFP_Pos)   /*!< GPG_MFPH PG11 setting for GPIO            */
#define SYS_GPG_MFPH_PG11MFP_UART5_nCTS     (0x6UL<<SYS_GPG_MFPH_PG11MFP_Pos)   /*!< GPG_MFPH PG11 setting for UART5_nCTS      */
#define SYS_GPG_MFPH_PG11MFP_USB_VBUS_ST    (0xBUL<<SYS_GPG_MFPH_PG11MFP_Pos)   /*!< GPG_MFPH PG11 setting for USB_VBUS_ST     */
#define SYS_GPG_MFPH_PG11MFP_QEI0_INDEX     (0xCUL<<SYS_GPG_MFPH_PG11MFP_Pos)   /*!< GPG_MFPH PG11 setting for QEI0_INDEX      */

//PG.12 MFP
#define SYS_GPG_MFPH_PG12MFP_GPIO           (0x0UL<<SYS_GPG_MFPH_PG12MFP_Pos)   /*!< GPG_MFPH PG12 setting for GPIO            */
#define SYS_GPG_MFPH_PG12MFP_UART5_nRTS     (0x6UL<<SYS_GPG_MFPH_PG12MFP_Pos)   /*!< GPG_MFPH PG12 setting for UART5_nRTS      */
#define SYS_GPG_MFPH_PG12MFP_USB_VBUS_EN    (0xBUL<<SYS_GPG_MFPH_PG12MFP_Pos)   /*!< GPG_MFPH PG12 setting for USB_VBUS_EN     */



/*@}*/ /* end of group SYS_EXPORTED_CONSTANTS */

/** @addtogroup SYS_EXPORTED_FUNCTIONS SYS Exported Functions
  @{
*/


/**
  * @brief      Clear Brown-out detector interrupt flag
  * @param      None
  * @return     None
  * @details    This macro clear Brown-out detector interrupt flag.
  */
#define SYS_CLEAR_BOD_INT_FLAG()        (SYS->BODCTL |= SYS_BODCTL_BODIF_Msk)

/**
  * @brief      Set Brown-out detector function to normal mode
  * @param      None
  * @return     None
  * @details    This macro set Brown-out detector to normal mode.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_CLEAR_BOD_LPM()             (SYS->BODCTL &= ~SYS_BODCTL_BODLPM_Msk)

/**
  * @brief      Disable Brown-out detector function
  * @param      None
  * @return     None
  * @details    This macro disable Brown-out detector function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_DISABLE_BOD()               (SYS->BODCTL &= ~SYS_BODCTL_BODEN_Msk)

/**
  * @brief      Enable Brown-out detector function
  * @param      None
  * @return     None
  * @details    This macro enable Brown-out detector function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_ENABLE_BOD()                (SYS->BODCTL |= SYS_BODCTL_BODEN_Msk)

/**
  * @brief      Get Brown-out detector interrupt flag
  * @param      None
  * @retval     0   Brown-out detect interrupt flag is not set.
  * @retval     >=1 Brown-out detect interrupt flag is set.
  * @details    This macro get Brown-out detector interrupt flag.
  */
#define SYS_GET_BOD_INT_FLAG()          (SYS->BODCTL & SYS_BODCTL_BODIF_Msk)

/**
  * @brief      Get Brown-out detector status
  * @param      None
  * @retval     0   System voltage is higher than BOD threshold voltage setting or BOD function is disabled.
  * @retval     >=1 System voltage is lower than BOD threshold voltage setting.
  * @details    This macro get Brown-out detector output status.
  *             If the BOD function is disabled, this function always return 0.
  */
#define SYS_GET_BOD_OUTPUT()            (SYS->BODCTL & SYS_BODCTL_BODOUT_Msk)

/**
  * @brief      Enable Brown-out detector interrupt function
  * @param      None
  * @return     None
  * @details    This macro enable Brown-out detector interrupt function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_DISABLE_BOD_RST()           (SYS->BODCTL &= ~SYS_BODCTL_BODRSTEN_Msk)

/**
  * @brief      Enable Brown-out detector reset function
  * @param      None
  * @return     None
  * @details    This macro enable Brown-out detect reset function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_ENABLE_BOD_RST()            (SYS->BODCTL |= SYS_BODCTL_BODRSTEN_Msk)

/**
  * @brief      Set Brown-out detector function low power mode
  * @param      None
  * @return     None
  * @details    This macro set Brown-out detector to low power mode.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_SET_BOD_LPM()               (SYS->BODCTL |= SYS_BODCTL_BODLPM_Msk)

/**
  * @brief      Set Brown-out detector voltage level
  * @param[in]  u32Level is Brown-out voltage level. Including :
  *             - \ref SYS_BODCTL_BODVL_1_6V
  *             - \ref SYS_BODCTL_BODVL_1_8V
  *             - \ref SYS_BODCTL_BODVL_2_0V
  *             - \ref SYS_BODCTL_BODVL_2_2V
  *             - \ref SYS_BODCTL_BODVL_2_4V
  *             - \ref SYS_BODCTL_BODVL_2_6V
  *             - \ref SYS_BODCTL_BODVL_2_8V
  *             - \ref SYS_BODCTL_BODVL_3_0V
  * @return     None
  * @details    This macro set Brown-out detector voltage level.
  *             The write-protection function should be disabled before using this macro.
  */
#define SYS_SET_BOD_LEVEL(u32Level)     (SYS->BODCTL = (SYS->BODCTL & ~SYS_BODCTL_BODVL_Msk) | (u32Level))

/**
  * @brief      Get reset source is from Brown-out detector reset
  * @param      None
  * @retval     0   Previous reset source is not from Brown-out detector reset
  * @retval     >=1 Previous reset source is from Brown-out detector reset
  * @details    This macro get previous reset source is from Brown-out detect reset or not.
  */
#define SYS_IS_BOD_RST()                (SYS->RSTSTS & SYS_RSTSTS_BODRF_Msk)

/**
  * @brief      Get reset source is from CPU reset
  * @param      None
  * @retval     0   Previous reset source is not from CPU reset
  * @retval     >=1 Previous reset source is from CPU reset
  * @details    This macro get previous reset source is from CPU reset.
  */
#define SYS_IS_CPU_RST()                (SYS->RSTSTS & SYS_RSTSTS_CPURF_Msk)

/**
  * @brief      Get reset source is from LVR Reset
  * @param      None
  * @retval     0   Previous reset source is not from Low-Voltage-Reset
  * @retval     >=1 Previous reset source is from Low-Voltage-Reset
  * @details    This macro get previous reset source is from Low-Voltage-Reset.
  */
#define SYS_IS_LVR_RST()                (SYS->RSTSTS & SYS_RSTSTS_LVRF_Msk)

/**
  * @brief      Get reset source is from Power-on Reset
  * @param      None
  * @retval     0   Previous reset source is not from Power-on Reset
  * @retval     >=1 Previous reset source is from Power-on Reset
  * @details    This macro get previous reset source is from Power-on Reset.
  */
#define SYS_IS_POR_RST()                (SYS->RSTSTS & SYS_RSTSTS_PORF_Msk)

/**
  * @brief      Get reset source is from reset pin reset
  * @param      None
  * @retval     0   Previous reset source is not from reset pin reset
  * @retval     >=1 Previous reset source is from reset pin reset
  * @details    This macro get previous reset source is from reset pin reset.
  */
#define SYS_IS_RSTPIN_RST()             (SYS->RSTSTS & SYS_RSTSTS_PINRF_Msk)

/**
  * @brief      Get reset source is from system reset
  * @param      None
  * @retval     0   Previous reset source is not from system reset
  * @retval     >=1 Previous reset source is from system reset
  * @details    This macro get previous reset source is from system reset.
  */
#define SYS_IS_SYSTEM_RST()             (SYS->RSTSTS & SYS_RSTSTS_SYSRF_Msk)

/**
  * @brief      Get reset source is from window watch dog reset
  * @param      None
  * @retval     0   Previous reset source is not from window watch dog reset
  * @retval     >=1 Previous reset source is from window watch dog reset
  * @details    This macro get previous reset source is from window watch dog reset.
  */
#define SYS_IS_WDT_RST()                (SYS->RSTSTS & SYS_RSTSTS_WDTRF_Msk)

/**
  * @brief      Disable Low-Voltage-Reset function
  * @param      None
  * @return     None
  * @details    This macro disable Low-Voltage-Reset function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_DISABLE_LVR()               (SYS->BODCTL &= ~SYS_BODCTL_LVREN_Msk)

/**
  * @brief      Enable Low-Voltage-Reset function
  * @param      None
  * @return     None
  * @details    This macro enable Low-Voltage-Reset function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_ENABLE_LVR()                (SYS->BODCTL |= SYS_BODCTL_LVREN_Msk)

/**
  * @brief      Disable Power-on Reset function
  * @param      None
  * @return     None
  * @details    This macro disable Power-on Reset function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_DISABLE_POR()               (SYS->PORCTL = 0x5AA5)

/**
  * @brief      Enable Power-on Reset function
  * @param      None
  * @return     None
  * @details    This macro enable Power-on Reset function.
  *             The register write-protection function should be disabled before using this macro.
  */
#define SYS_ENABLE_POR()                (SYS->PORCTL = 0)

/**
  * @brief      Clear reset source flag
  * @param[in]  u32RstSrc is reset source. Including :
  *             - \ref SYS_RSTSTS_PORF_Msk
  *             - \ref SYS_RSTSTS_PINRF_Msk
  *             - \ref SYS_RSTSTS_WDTRF_Msk
  *             - \ref SYS_RSTSTS_LVRF_Msk
  *             - \ref SYS_RSTSTS_BODRF_Msk
  *             - \ref SYS_RSTSTS_MCURF_Msk
  *             - \ref SYS_RSTSTS_CPURF_Msk
  *             - \ref SYS_RSTSTS_CPULKRF_Msk
  * @return     None
  * @details    This macro clear reset source flag.
  */
#define SYS_CLEAR_RST_SOURCE(u32RstSrc) ((SYS->RSTSTS) = (u32RstSrc) )




/**
  * @brief      Disable register write-protection function
  * @param      None
  * @return     None
  * @details    This function disable register write-protection function.
  *             To unlock the protected register to allow write access.
  */
__STATIC_INLINE void SYS_UnlockReg(void)
{
    do
    {
        SYS->REGLCTL = 0x59;
        SYS->REGLCTL = 0x16;
        SYS->REGLCTL = 0x88;
    }
    while(SYS->REGLCTL == 0);
}

/**
  * @brief      Enable register write-protection function
  * @param      None
  * @return     None
  * @details    This function is used to enable register write-protection function.
  *             To lock the protected register to forbid write access.
  */
__STATIC_INLINE void SYS_LockReg(void)
{
    SYS->REGLCTL = 0;
}


void SYS_ClearResetSrc(uint32_t u32Src);
uint32_t SYS_GetBODStatus(void);
uint32_t SYS_GetResetSrc(void);
uint32_t SYS_IsRegLocked(void);
uint32_t SYS_ReadPDID(void);
void SYS_ResetChip(void);
void SYS_ResetCPU(void);
void SYS_ResetModule(uint32_t u32ModuleIndex);
void SYS_EnableBOD(int32_t i32Mode, uint32_t u32BODLevel);
void SYS_DisableBOD(void);


/*@}*/ /* end of group SYS_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group SYS_Driver */

/*@}*/ /* end of group Standard_Driver */


#ifdef __cplusplus
}
#endif

#endif //__SYS_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
