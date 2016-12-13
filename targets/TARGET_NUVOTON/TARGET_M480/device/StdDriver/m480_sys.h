/**************************************************************************//**
 * @file     SYS.h
 * @version  V3.0
 * @brief    M480 Series SYS Driver Header File
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __SYS_H__
#define __SYS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup M480_Device_Driver Device Driver
  @{
*/

/** @addtogroup M480_SYS_Driver SYS Driver
  @{
*/

/** @addtogroup M480_SYS_EXPORTED_CONSTANTS SYS Exported Constants
  @{
*/


/*---------------------------------------------------------------------------------------------------------*/
/*  Module Reset Control Resister constant definitions.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PDMA_RST    ((0x0<<24) | SYS_IPRST0_PDMARST_Pos)   /*!< Reset PDMA */
#define EBI_RST     ((0x0<<24) | SYS_IPRST0_EBIRST_Pos)    /*!< Reset EBI */
#define EMAC_RST    ((0x0<<24) | SYS_IPRST0_EMACRST_Pos)   /*!< Reset EMAC */
#define SDH0_RST    ((0x0<<24) | SYS_IPRST0_SDH0RST_Pos)   /*!< Reset SDH0 */
#define CRC_RST     ((0x0<<24) | SYS_IPRST0_CRCRST_Pos)    /*!< Reset CRC */
#define UDC20_RST   ((0x0<<24) | SYS_IPRST0_UDC20RST_Pos)  /*!< Reset UDC20 */
#define CRYPTO_RST  ((0x0<<24) | SYS_IPRST0_CRYPTORST_Pos) /*!< Reset CRYPTO */
#define SPIM_RST    ((0x0<<24) | SYS_IPRST0_SPIMRST_Pos)   /*!< Reset SPIM */
#define UHC20_RST   ((0x0<<24) | SYS_IPRST0_UHC20RST_Pos)  /*!< Reset UHC20 */
#define SDH1_RST    ((0x0<<24) | SYS_IPRST0_SDH1RST_Pos)   /*!< Reset SDH1 */
#define SWDC_RST    ((0x0<<24) | SYS_IPRST0_SWDCRST_Pos)   /*!< Reset SWDC */
#define ETMC_RST    ((0x0<<24) | SYS_IPRST0_ETMCRST_Pos)   /*!< Reset ETMC */

#define GPIO_RST    ((0x4<<24) | SYS_IPRST1_GPIORST_Pos)   /*!< Reset GPIO */
#define TMR0_RST    ((0x4<<24) | SYS_IPRST1_TMR0RST_Pos)   /*!< Reset TMR0 */
#define TMR1_RST    ((0x4<<24) | SYS_IPRST1_TMR1RST_Pos)   /*!< Reset TMR1 */
#define TMR2_RST    ((0x4<<24) | SYS_IPRST1_TMR2RST_Pos)   /*!< Reset TMR2 */
#define TMR3_RST    ((0x4<<24) | SYS_IPRST1_TMR3RST_Pos)   /*!< Reset TMR3 */
#define ACMP01_RST  ((0x4<<24) | SYS_IPRST1_ACMP01RST_Pos) /*!< Reset ACMP01 */
#define I2C0_RST    ((0x4<<24) | SYS_IPRST1_I2C0RST_Pos)   /*!< Reset I2C0 */
#define I2C1_RST    ((0x4<<24) | SYS_IPRST1_I2C1RST_Pos)   /*!< Reset I2C1 */
#define I2C2_RST    ((0x4<<24) | SYS_IPRST1_I2C2RST_Pos)   /*!< Reset I2C2 */
#define SPI0_RST    ((0x4<<24) | SYS_IPRST1_SPI0RST_Pos)   /*!< Reset SPI0 */
#define SPI1_RST    ((0x4<<24) | SYS_IPRST1_SPI1RST_Pos)   /*!< Reset SPI1 */
#define SPI2_RST    ((0x4<<24) | SYS_IPRST1_SPI2RST_Pos)   /*!< Reset SPI2 */
#define SPI3_RST    ((0x4<<24) | SYS_IPRST1_SPI3RST_Pos)   /*!< Reset SPI3 */
#define UART0_RST   ((0x4<<24) | SYS_IPRST1_UART0RST_Pos)  /*!< Reset UART0 */
#define UART1_RST   ((0x4<<24) | SYS_IPRST1_UART1RST_Pos)  /*!< Reset UART1 */
#define UART2_RST   ((0x4<<24) | SYS_IPRST1_UART2RST_Pos)  /*!< Reset UART2 */
#define UART3_RST   ((0x4<<24) | SYS_IPRST1_UART3RST_Pos)  /*!< Reset UART3 */
#define UART4_RST   ((0x4<<24) | SYS_IPRST1_UART4RST_Pos)  /*!< Reset UART4 */
#define UART5_RST   ((0x4<<24) | SYS_IPRST1_UART5RST_Pos)  /*!< Reset UART5 */
#define CAN0_RST    ((0x4<<24) | SYS_IPRST1_CAN0RST_Pos)   /*!< Reset CAN0 */
#define CAN1_RST    ((0x4<<24) | SYS_IPRST1_CAN1RST_Pos)   /*!< Reset CAN1 */
#define USBD_RST    ((0x4<<24) | SYS_IPRST1_USBDRST_Pos)   /*!< Reset USBD */
#define EADC_RST    ((0x4<<24) | SYS_IPRST1_EADCRST_Pos)   /*!< Reset EADC */
#define I2S0_RST    ((0x4<<24) | SYS_IPRST1_I2S0RST_Pos)   /*!< Reset I2S0 */

#define SC0_RST     ((0x8<<24) | SYS_IPRST2_SC0RST_Pos)    /*!< Reset SC0 */
#define SC1_RST     ((0x8<<24) | SYS_IPRST2_SC1RST_Pos)    /*!< Reset SC1 */
#define SC2_RST     ((0x8<<24) | SYS_IPRST2_SC2RST_Pos)    /*!< Reset SC2 */
#define SPI4_RST    ((0x8<<24) | SYS_IPRST2_SPI4RST_Pos)   /*!< Reset SPI4 */
#define USCI0_RST   ((0x8<<24) | SYS_IPRST2_USCI0RST_Pos)  /*!< Reset USCI0 */
#define USCI1_RST   ((0x8<<24) | SYS_IPRST2_USCI1RST_Pos)  /*!< Reset USCI1 */
#define DAC_RST     ((0x8<<24) | SYS_IPRST2_DACRST_Pos)    /*!< Reset DAC */
#define PWM0_RST    ((0x8<<24) | SYS_IPRST2_PWM0RST_Pos)   /*!< Reset PWM0 */
#define PWM1_RST    ((0x8<<24) | SYS_IPRST2_PWM1RST_Pos)   /*!< Reset PWM1 */
#define BPWM0_RST   ((0x8<<24) | SYS_IPRST2_BPWM0RST_Pos)  /*!< Reset BPWM0 */
#define BPWM1_RST   ((0x8<<24) | SYS_IPRST2_BPWM1RST_Pos)  /*!< Reset BPWM1 */
#define QEI0_RST    ((0x8<<24) | SYS_IPRST2_QEI0RST_Pos)   /*!< Reset QEI0 */
#define QEI1_RST    ((0x8<<24) | SYS_IPRST2_QEI1RST_Pos)   /*!< Reset QEI1 */
#define ECAP0_RST   ((0x8<<24) | SYS_IPRST2_ECAP0RST_Pos)  /*!< Reset ECAP0 */
#define ECAP1_RST   ((0x8<<24) | SYS_IPRST2_ECAP1RST_Pos)  /*!< Reset ECAP1 */
#define OPA_RST     ((0x8<<24) | SYS_IPRST2_OPARST_Pos)    /*!< Reset OPA */

/*---------------------------------------------------------------------------------------------------------*/
/*  Brown Out Detector Threshold Voltage Selection constant definitions.                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_BODCTL_BOD_RST_EN           (1UL << SYS_BODCTL_BODRSTEN_Pos)    /*!< Brown-out Reset Enable */
#define SYS_BODCTL_BOD_INTERRUPT_EN     (0UL << SYS_BODCTL_BODRSTEN_Pos)    /*!< Brown-out Interrupt Enable */
#define SYS_BODCTL_BODVL_3_0V           (7UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 3.0V */
#define SYS_BODCTL_BODVL_2_8V           (6UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.8V */
#define SYS_BODCTL_BODVL_2_6V           (5UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.6V */
#define SYS_BODCTL_BODVL_2_4V           (4UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.4V */
#define SYS_BODCTL_BODVL_2_2V           (3UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.2V */
#define SYS_BODCTL_BODVL_2_0V           (2UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 2.0V */
#define SYS_BODCTL_BODVL_1_8V           (1UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 1.8V */
#define SYS_BODCTL_BODVL_1_6V           (0UL << SYS_BODCTL_BODVL_Pos)       /*!< Setting Brown Out Detector Threshold Voltage as 1.6V */


/*---------------------------------------------------------------------------------------------------------*/
/*  VREFCTL constant definitions. (Write-Protection Register)                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_VREFCTL_VREF_PIN        (0x0UL << SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = Vref pin */
#define SYS_VREFCTL_VREF_1_6V       (0x3UL << SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 1.6V */
#define SYS_VREFCTL_VREF_2_0V       (0x7UL << SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 2.0V */
#define SYS_VREFCTL_VREF_2_5V       (0xBUL << SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 2.5V */
#define SYS_VREFCTL_VREF_3_0V       (0xFUL << SYS_VREFCTL_VREFCTL_Pos)    /*!< Vref = 3.0V */
#define SYS_VREFCTL_VREF_AVDD       (0x10UL << SYS_VREFCTL_VREFCTL_Pos)   /*!< Vref = AVDD */


/*---------------------------------------------------------------------------------------------------------*/
/*  USBPHY constant definitions. (Write-Protection Register)                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define SYS_USBPHY_USBROLE_STD_USBD   (0x0UL << SYS_USBPHY_USBROLE_Pos)   /*!<  Standard USB device */
#define SYS_USBPHY_USBROLE_STD_USBH   (0x1UL << SYS_USBPHY_USBROLE_Pos)   /*!<  Standard USB host */
#define SYS_USBPHY_USBROLE_ID_DEPH    (0x2UL << SYS_USBPHY_USBROLE_Pos)   /*!<  ID dependent device */
#define SYS_USBPHY_USBROLE_ON_THE_GO  (0x3UL << SYS_USBPHY_USBROLE_Pos)   /*!<  On-The-Go device */


/*---------------------------------------------------------------------------------------------------------*/
/*  Multi-Function constant definitions.                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
/* How to use below #define?
Example 1: If user want to set PA.0 as SC0_CLK in initial function,
           user can issue following command to achieve it.

           SYS->GPA_MFPL  = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA0MFP_Msk) ) | SYS_GPA_MFPL_PA0_MFP_SC0_CLK  ;

*/
/********************* Bit definition of GPA_MFPL register **********************/
#define SYS_GPA_MFPL_PA0MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA0MFP_UART1_nCTS        (0x01UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< Clear to Send input pin for UART1. */
#define SYS_GPA_MFPL_PA0MFP_I2C2_SCL          (0x02UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< I2C2 clock pin. */
#define SYS_GPA_MFPL_PA0MFP_UART1_TXD         (0x03UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< Data transmitter output pin for UART1. */
#define SYS_GPA_MFPL_PA0MFP_CAN0_RXD          (0x04UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< CAN0 bus receiver input. */
#define SYS_GPA_MFPL_PA0MFP_SC0_CLK           (0x05UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< SmartCard0 clock pin. */
#define SYS_GPA_MFPL_PA0MFP_PWM1_CH5          (0x06UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< PWM1 channel5 output/capture input. */
#define SYS_GPA_MFPL_PA0MFP_EBI_AD0           (0x07UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< EBI address/data bus bit0. */
#define SYS_GPA_MFPL_PA0MFP_INT0              (0x08UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< External interrupt0 input pin. */
#define SYS_GPA_MFPL_PA0MFP_SPI3_MISO         (0x09UL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< 1st SPI3 MISO (Master In, Slave Out) pin. */
#define SYS_GPA_MFPL_PA0MFP_SPI1_I2SMCLK      (0x0AUL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< SPI1 I2S master clock output pin. */
#define SYS_GPA_MFPL_PA0MFP_USB_VBUS_EN       (0x0BUL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPA_MFPL_PA0MFP_USCI1_CTL0        (0x0CUL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< USCI1 control0 pin. */
#define SYS_GPA_MFPL_PA0MFP_BPWM1_CH5         (0x0DUL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< BPWM1 channel5 output/capture input. */
#define SYS_GPA_MFPL_PA0MFP_ECAP0_IC0         (0x0EUL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< Input 0 of enhanced capture unit 0. */
#define SYS_GPA_MFPL_PA0MFP_SPIM_MISO         (0x0FUL << SYS_GPA_MFPL_PA0MFP_Pos) /*!< 1st SPI* MISO (Master In, Slave Out) pin. */
#define SYS_GPA_MFPL_PA1MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA1MFP_UART1_nRTS        (0x01UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< Request to Send output pin for UART1. */
#define SYS_GPA_MFPL_PA1MFP_I2C2_SDA          (0x02UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< I2C2 data input/output pin. */
#define SYS_GPA_MFPL_PA1MFP_UART1_RXD         (0x03UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< Data receiver input pin for UART1. */
#define SYS_GPA_MFPL_PA1MFP_CAN0_TXD          (0x04UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< CAN0 bus transmitter output. */
#define SYS_GPA_MFPL_PA1MFP_SC0_DAT           (0x05UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< SmartCard0 data pin. */
#define SYS_GPA_MFPL_PA1MFP_PWM1_CH4          (0x06UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< PWM1 channel4 output/capture input. */
#define SYS_GPA_MFPL_PA1MFP_EBI_AD1           (0x07UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPA_MFPL_PA1MFP_SPI0_SS           (0x08UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< 1st SPI0 slave select pin. */
#define SYS_GPA_MFPL_PA1MFP_SPI3_CLK          (0x09UL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< SPI3 serial clock pin. */
#define SYS_GPA_MFPL_PA1MFP_EADC0_ST          (0x0AUL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< EADC external trigger input. */
#define SYS_GPA_MFPL_PA1MFP_USB_VBUS_ST       (0x0BUL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPA_MFPL_PA1MFP_USCI1_DAT1        (0x0CUL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< USCI1 data1 pin. */
#define SYS_GPA_MFPL_PA1MFP_BPWM1_CH4         (0x0DUL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< BPWM1 channel4 output/capture input. */
#define SYS_GPA_MFPL_PA1MFP_ECAP0_IC1         (0x0EUL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< Input 1 of enhanced capture unit 0. */
#define SYS_GPA_MFPL_PA1MFP_SPIM_MOSI         (0x0FUL << SYS_GPA_MFPL_PA1MFP_Pos) /*!< 1st SPI* MOSI (Master Out, Slave In) pin. */
#define SYS_GPA_MFPL_PA2MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA2MFP_HSUSB_VBUS_EN     (0x01UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPA_MFPL_PA2MFP_UART0_TXD         (0x02UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< Data transmitter output pin for UART0. */
#define SYS_GPA_MFPL_PA2MFP_UART0_nCTS        (0x03UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< Clear to Send input pin for UART0. */
#define SYS_GPA_MFPL_PA2MFP_I2C0_SDA          (0x04UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< I2C0 data input/output pin. */
#define SYS_GPA_MFPL_PA2MFP_SC0_RST           (0x05UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< SmartCard0 reset pin. */
#define SYS_GPA_MFPL_PA2MFP_PWM1_CH3          (0x06UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< PWM1 channel3 output/capture input. */
#define SYS_GPA_MFPL_PA2MFP_EBI_AD2           (0x07UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< EBI address/data bus bit2. */
#define SYS_GPA_MFPL_PA2MFP_I2S0_MCLK         (0x08UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< I2S0 master clock output pin. */
#define SYS_GPA_MFPL_PA2MFP_SPI3_SS           (0x09UL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< 1st SPI3 slave select pin. */
#define SYS_GPA_MFPL_PA2MFP_USCI1_DAT0        (0x0CUL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< USCI1 data0 pin. */
#define SYS_GPA_MFPL_PA2MFP_BPWM1_CH3         (0x0DUL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< BPWM1 channel3 output/capture input. */
#define SYS_GPA_MFPL_PA2MFP_ECAP0_IC2         (0x0EUL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< Input 0 of enhanced capture unit 2. */
#define SYS_GPA_MFPL_PA2MFP_SPIM_SS           (0x0FUL << SYS_GPA_MFPL_PA2MFP_Pos) /*!< 1st SPI* slave select pin. */
#define SYS_GPA_MFPL_PA3MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA3MFP_HSUSB_VBUS_ST     (0x01UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPA_MFPL_PA3MFP_UART0_RXD         (0x02UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPA_MFPL_PA3MFP_UART0_nRTS        (0x03UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< Request to Send output pin for UART0. */
#define SYS_GPA_MFPL_PA3MFP_I2C0_SCL          (0x04UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< I2C0 clock pin. */
#define SYS_GPA_MFPL_PA3MFP_SC0_PWR           (0x05UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< SmartCard0 power pin. */
#define SYS_GPA_MFPL_PA3MFP_PWM1_CH2          (0x06UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< PWM1 channel2 output/capture input. */
#define SYS_GPA_MFPL_PA3MFP_EBI_AD3           (0x07UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< EBI address/data bus bit3. */
#define SYS_GPA_MFPL_PA3MFP_SPI3_MOSI         (0x09UL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< 1st SPI3 MOSI (Master Out, Slave In) pin. */
#define SYS_GPA_MFPL_PA3MFP_USCI1_CLK         (0x0CUL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< USCI1 clock pin. */
#define SYS_GPA_MFPL_PA3MFP_BPWM1_CH2         (0x0DUL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< BPWM1 channel2 output/capture input. */
#define SYS_GPA_MFPL_PA3MFP_SPIM_CLK          (0x0FUL << SYS_GPA_MFPL_PA3MFP_Pos) /*!< SPI* serial clock pin. */
#define SYS_GPA_MFPL_PA4MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA4MFP_SPI1_SS           (0x02UL << SYS_GPA_MFPL_PA4MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPA_MFPL_PA4MFP_EBI_AD4           (0x07UL << SYS_GPA_MFPL_PA4MFP_Pos) /*!< EBI address/data bus bit4. */
#define SYS_GPA_MFPL_PA4MFP_ECAP1_IC0         (0x0EUL << SYS_GPA_MFPL_PA4MFP_Pos) /*!< Input 0 of enhanced capture unit 1. */
#define SYS_GPA_MFPL_PA5MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA5MFP_SPI1_MOSI         (0x02UL << SYS_GPA_MFPL_PA5MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPA_MFPL_PA5MFP_TM2_EXT           (0x03UL << SYS_GPA_MFPL_PA5MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPA_MFPL_PA5MFP_EBI_AD5           (0x07UL << SYS_GPA_MFPL_PA5MFP_Pos) /*!< EBI address/data bus bit5. */
#define SYS_GPA_MFPL_PA5MFP_ECAP1_IC1         (0x0EUL << SYS_GPA_MFPL_PA5MFP_Pos) /*!< Input 1 of enhanced capture unit 1. */
#define SYS_GPA_MFPL_PA6MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA6MFP_SPI1_MISO         (0x02UL << SYS_GPA_MFPL_PA6MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPA_MFPL_PA6MFP_TM1_EXT           (0x03UL << SYS_GPA_MFPL_PA6MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPA_MFPL_PA6MFP_EBI_AD6           (0x07UL << SYS_GPA_MFPL_PA6MFP_Pos) /*!< EBI address/data bus bit6. */
#define SYS_GPA_MFPL_PA6MFP_ECAP1_IC2         (0x0EUL << SYS_GPA_MFPL_PA6MFP_Pos) /*!< Input 1 of enhanced capture unit 2. */
#define SYS_GPA_MFPL_PA7MFP_GPIO              (0x00UL << SYS_GPA_MFPL_PA7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPL_PA7MFP_SPI1_CLK          (0x02UL << SYS_GPA_MFPL_PA7MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPA_MFPL_PA7MFP_TM0_EXT           (0x03UL << SYS_GPA_MFPL_PA7MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPA_MFPL_PA7MFP_EBI_AD7           (0x07UL << SYS_GPA_MFPL_PA7MFP_Pos) /*!< EBI address/data bus bit7. */
#define SYS_GPA_MFPL_PA7MFP_SD1_CLK           (0x0BUL << SYS_GPA_MFPL_PA7MFP_Pos) /*!< SD/SDIO mode #1??clock. */
/********************* Bit definition of GPA_MFPH register **********************/
#define SYS_GPA_MFPH_PA8MFP_GPIO              (0x00UL << SYS_GPA_MFPH_PA8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA8MFP_UART3_TXD         (0x03UL << SYS_GPA_MFPH_PA8MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPA_MFPH_PA8MFP_SD1_CLK           (0x0BUL << SYS_GPA_MFPH_PA8MFP_Pos) /*!< SD/SDIO mode #1??clock. */
#define SYS_GPA_MFPH_PA8MFP_SPIM_D2           (0x0FUL << SYS_GPA_MFPH_PA8MFP_Pos) /*!< SPIM data 2 pin for Quad Mode I/O. */
#define SYS_GPA_MFPH_PA9MFP_GPIO              (0x00UL << SYS_GPA_MFPH_PA9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA9MFP_UART3_RXD         (0x03UL << SYS_GPA_MFPH_PA9MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPA_MFPH_PA9MFP_SD1_DAT3          (0x0BUL << SYS_GPA_MFPH_PA9MFP_Pos) /*!< SD/SDIO mode #1  data line bit 3. */
#define SYS_GPA_MFPH_PA9MFP_SPIM_D3           (0x0FUL << SYS_GPA_MFPH_PA9MFP_Pos) /*!< SPIM data 3 pin for Quad Mode I/O. */
#define SYS_GPA_MFPH_PA10MFP_GPIO             (0x00UL << SYS_GPA_MFPH_PA10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA10MFP_SPIM_SS          (0x02UL << SYS_GPA_MFPH_PA10MFP_Pos) /*!< 1st SPI* slave select pin. */
#define SYS_GPA_MFPH_PA10MFP_UART3_nCTS       (0x03UL << SYS_GPA_MFPH_PA10MFP_Pos) /*!< Clear to Send input pin for UART3. */
#define SYS_GPA_MFPH_PA10MFP_SD1_DAT2         (0x0BUL << SYS_GPA_MFPH_PA10MFP_Pos) /*!< SD/SDIO mode #1  data line bit 2. */
#define SYS_GPA_MFPH_PA11MFP_GPIO             (0x00UL << SYS_GPA_MFPH_PA11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA11MFP_SPIM_CLK         (0x02UL << SYS_GPA_MFPH_PA11MFP_Pos) /*!< SPI* serial clock pin. */
#define SYS_GPA_MFPH_PA11MFP_UART3_nRTS       (0x03UL << SYS_GPA_MFPH_PA11MFP_Pos) /*!< Request to Send output pin for UART3. */
#define SYS_GPA_MFPH_PA11MFP_SD1_DAT1         (0x0BUL << SYS_GPA_MFPH_PA11MFP_Pos) /*!< SD/SDIO mode #1  data line bit 1. */
#define SYS_GPA_MFPH_PA12MFP_GPIO             (0x00UL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA12MFP_SPI1_I2SMCLK     (0x02UL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< SPI1 I2S master clock output pin. */
#define SYS_GPA_MFPH_PA12MFP_UART2_RXD        (0x03UL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< Data receiver input pin for UART2. */
#define SYS_GPA_MFPH_PA12MFP_CAN0_TXD         (0x04UL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< CAN0 bus transmitter output. */
#define SYS_GPA_MFPH_PA12MFP_EBI_ADR8         (0x07UL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPA_MFPH_PA12MFP_I2S0_DO          (0x08UL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< I2S0 data output. */
#define SYS_GPA_MFPH_PA12MFP_SC2_PWR          (0x09UL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< SmartCard2 power pin. */
#define SYS_GPA_MFPH_PA12MFP_USB_VBUS_ST      (0x0BUL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPA_MFPH_PA12MFP_SPIM_D2          (0x0CUL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< SPIM data 2 pin for Quad Mode I/O. */
#define SYS_GPA_MFPH_PA12MFP_BPWM0_CH5        (0x0DUL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< BPWM0 channel5 output/capture input. */
#define SYS_GPA_MFPH_PA12MFP_QEI1_INDEX       (0x0EUL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< Quadrature encoder index input of QEI Unit 1. */
#define SYS_GPA_MFPH_PA12MFP_SPI2_I2SMCLK     (0x0FUL << SYS_GPA_MFPH_PA12MFP_Pos) /*!< SPI2 I2S master clock output pin. */
#define SYS_GPA_MFPH_PA13MFP_GPIO             (0x00UL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA13MFP_UART2_TXD        (0x03UL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< Data transmitter output pin for UART2. */
#define SYS_GPA_MFPH_PA13MFP_CAN0_RXD         (0x04UL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< CAN0 bus receiver input. */
#define SYS_GPA_MFPH_PA13MFP_EBI_ADR7         (0x07UL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPA_MFPH_PA13MFP_I2S0_DI          (0x08UL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< I2S0 data input. */
#define SYS_GPA_MFPH_PA13MFP_SC2_RST          (0x09UL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< SmartCard2 reset pin. */
#define SYS_GPA_MFPH_PA13MFP_SPI1_CLK         (0x0AUL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPA_MFPH_PA13MFP_USB_VBUS_EN      (0x0BUL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPA_MFPH_PA13MFP_SPIM_D3          (0x0CUL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< SPIM data 3 pin for Quad Mode I/O. */
#define SYS_GPA_MFPH_PA13MFP_QEI1_A           (0x0EUL << SYS_GPA_MFPH_PA13MFP_Pos) /*!< Quadrature encoder phase A input of QEI Unit 1. */
#define SYS_GPA_MFPH_PA14MFP_GPIO             (0x00UL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA14MFP_UART2_nCTS       (0x03UL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< Clear to Send input pin for UART2. */
#define SYS_GPA_MFPH_PA14MFP_I2C0_SMBAL       (0x04UL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< I2C0 SMBus SMBALTER# pin  */
#define SYS_GPA_MFPH_PA14MFP_EBI_ADR6         (0x07UL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPA_MFPH_PA14MFP_I2S0_BCLK        (0x08UL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< I2S0 bit clock pin. */
#define SYS_GPA_MFPH_PA14MFP_SC2_CLK          (0x09UL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< SmartCard2 clock pin. */
#define SYS_GPA_MFPH_PA14MFP_SPI1_MISO        (0x0AUL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPA_MFPH_PA14MFP_HSUSB_VBUS_ST    (0x0BUL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPA_MFPH_PA14MFP_BPWM1_CH0        (0x0DUL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< BPWM1 channel0 output/capture input. */
#define SYS_GPA_MFPH_PA14MFP_QEI1_B           (0x0EUL << SYS_GPA_MFPH_PA14MFP_Pos) /*!< Quadrature encoder phase B input of QEI Unit 1. */
#define SYS_GPA_MFPH_PA15MFP_GPIO             (0x00UL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPA_MFPH_PA15MFP_UART2_nRTS       (0x03UL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< Request to Send output pin for UART2. */
#define SYS_GPA_MFPH_PA15MFP_I2C0_SMBSUS      (0x04UL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< I2C0 SMBus SMBSUS# pin (PMBus CONTROL pin) */
#define SYS_GPA_MFPH_PA15MFP_PWM1_CH0         (0x06UL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPA_MFPH_PA15MFP_EBI_ADR5         (0x07UL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPA_MFPH_PA15MFP_I2S0_LRCK        (0x08UL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< I2S0 left right channel clock. */
#define SYS_GPA_MFPH_PA15MFP_SC2_DAT          (0x09UL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< SmartCard2 data pin. */
#define SYS_GPA_MFPH_PA15MFP_SPI1_MOSI        (0x0AUL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPA_MFPH_PA15MFP_HSUSB_VBUS_EN    (0x0BUL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPA_MFPH_PA15MFP_BPWM1_CH1        (0x0DUL << SYS_GPA_MFPH_PA15MFP_Pos) /*!< BPWM1 channel1 output/capture input. */
/********************* Bit definition of GPB_MFPL register **********************/
#define SYS_GPB_MFPL_PB0MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB0MFP_EADC0_CH0         (0x01UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< EADC0 channel0 analog input. */
#define SYS_GPB_MFPL_PB0MFP_SPI0_MOSI1        (0x02UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< 2nd SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPB_MFPL_PB0MFP_UART2_RXD         (0x03UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< Data receiver input pin for UART2. */
#define SYS_GPB_MFPL_PB0MFP_TM2               (0x04UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPB_MFPL_PB0MFP_DAC0_OUT          (0x05UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< DAC0 channel analog output. */
#define SYS_GPB_MFPL_PB0MFP_PWM1_CH5          (0x06UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< PWM1 channel5 output/capture input. */
#define SYS_GPB_MFPL_PB0MFP_EBI_nWRL          (0x07UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< EBI wirte enable output pin. */
#define SYS_GPB_MFPL_PB0MFP_INT1              (0x08UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< External interrupt1 input pin. */
#define SYS_GPB_MFPL_PB0MFP_I2C2_SDA          (0x09UL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< I2C2 data input/output pin. */
#define SYS_GPB_MFPL_PB0MFP_SC1_RST           (0x0AUL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< SmartCard1 reset pin. */
#define SYS_GPB_MFPL_PB0MFP_SC0_DAT           (0x0BUL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< SmartCard0 data pin. */
#define SYS_GPB_MFPL_PB0MFP_USCI1_CTL1        (0x0CUL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< USCI1 control1 pin. */
#define SYS_GPB_MFPL_PB0MFP_SPIM_D2           (0x0DUL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< SPIM data 2 pin for Quad Mode I/O. */
#define SYS_GPB_MFPL_PB0MFP_SPI1_SS           (0x0EUL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPB_MFPL_PB0MFP_ETM_TRACE_CLK     (0x0FUL << SYS_GPB_MFPL_PB0MFP_Pos) /*!< ETM Rx clock input pin. */
#define SYS_GPB_MFPL_PB1MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB1MFP_EADC0_CH1         (0x01UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< EADC0 channel1 analog input. */
#define SYS_GPB_MFPL_PB1MFP_SPI0_MISO1        (0x02UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< 2nd SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPB_MFPL_PB1MFP_UART2_TXD         (0x03UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< Data transmitter output pin for UART2. */
#define SYS_GPB_MFPL_PB1MFP_TM3               (0x04UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPB_MFPL_PB1MFP_SC0_RST           (0x05UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< SmartCard0 reset pin. */
#define SYS_GPB_MFPL_PB1MFP_PWM0_SYNC_OUT     (0x06UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< PWM0 counter synchronous trigger output pin. */
#define SYS_GPB_MFPL_PB1MFP_EBI_nWRH          (0x07UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< EBI wirte enable output pin. */
#define SYS_GPB_MFPL_PB1MFP_DAC1_OUT          (0x08UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< DAC1 channel analog output. */
#define SYS_GPB_MFPL_PB1MFP_I2C2_SCL          (0x09UL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< I2C2 clock pin. */
#define SYS_GPB_MFPL_PB1MFP_SC1_PWR           (0x0AUL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< SmartCard1 power pin. */
#define SYS_GPB_MFPL_PB1MFP_SC2_nCD           (0x0BUL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< SmartCard2 card detect pin. */
#define SYS_GPB_MFPL_PB1MFP_USCI1_DAT1        (0x0CUL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< USCI1 data1 pin. */
#define SYS_GPB_MFPL_PB1MFP_SPIM_D3           (0x0DUL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< SPIM data 3 pin for Quad Mode I/O. */
#define SYS_GPB_MFPL_PB1MFP_SPI1_MOSI         (0x0EUL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPB_MFPL_PB1MFP_ETM_TRACE_DATA0   (0x0FUL << SYS_GPB_MFPL_PB1MFP_Pos) /*!< ETM Rx input bus bit0. */
#define SYS_GPB_MFPL_PB2MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB2MFP_EADC0_CH2         (0x01UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< EADC0 channel2 analog input. */
#define SYS_GPB_MFPL_PB2MFP_SPI0_CLK          (0x02UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< SPI0 serial clock pin. */
#define SYS_GPB_MFPL_PB2MFP_SPI1_CLK          (0x03UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPB_MFPL_PB2MFP_UART1_RXD         (0x04UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< Data receiver input pin for UART1. */
#define SYS_GPB_MFPL_PB2MFP_SC0_nCD           (0x05UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< SmartCard0 card detect pin. */
#define SYS_GPB_MFPL_PB2MFP_PWM1_CH4          (0x06UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< PWM1 channel4 output/capture input. */
#define SYS_GPB_MFPL_PB2MFP_EBI_ADR16         (0x07UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPB_MFPL_PB2MFP_I2C2_SMBAL        (0x08UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< I2C2 SMBus SMBALTER# pin  */
#define SYS_GPB_MFPL_PB2MFP_UART3_RXD         (0x09UL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPB_MFPL_PB2MFP_SC1_CLK           (0x0AUL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< SmartCard1 clock pin. */
#define SYS_GPB_MFPL_PB2MFP_TM2_EXT           (0x0BUL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPB_MFPL_PB2MFP_USCI1_DAT0        (0x0CUL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< USCI1 data0 pin. */
#define SYS_GPB_MFPL_PB2MFP_USB_VBUS_ST       (0x0DUL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPB_MFPL_PB2MFP_ETM_TRACE_DATA1   (0x0FUL << SYS_GPB_MFPL_PB2MFP_Pos) /*!< ETM Rx input bus bit1. */
#define SYS_GPB_MFPL_PB3MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB3MFP_EADC0_CH3         (0x01UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< EADC0 channel3 analog input. */
#define SYS_GPB_MFPL_PB3MFP_SPI0_MISO0        (0x02UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< 1st SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPB_MFPL_PB3MFP_SPI1_MISO         (0x03UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPB_MFPL_PB3MFP_UART1_TXD         (0x04UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< Data transmitter output pin for UART1. */
#define SYS_GPB_MFPL_PB3MFP_PWM0_BRAKE0       (0x05UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< Brake input pin 0 of PWM0. */
#define SYS_GPB_MFPL_PB3MFP_PWM1_CH3          (0x06UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< PWM1 channel3 output/capture input. */
#define SYS_GPB_MFPL_PB3MFP_EBI_ADR17         (0x07UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPB_MFPL_PB3MFP_I2C2_SMBSUS       (0x08UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< I2C2 SMBus SMBSUS# pin (PMBus CONTROL pin) */
#define SYS_GPB_MFPL_PB3MFP_UART3_TXD         (0x09UL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPB_MFPL_PB3MFP_SC1_nCD           (0x0AUL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< SmartCard1 card detect pin. */
#define SYS_GPB_MFPL_PB3MFP_TM0_EXT           (0x0BUL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPB_MFPL_PB3MFP_USCI1_DAT1        (0x0CUL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< USCI1 data1 pin. */
#define SYS_GPB_MFPL_PB3MFP_USB_VBUS_EN       (0x0DUL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPB_MFPL_PB3MFP_ACMP0_WLAT        (0x0EUL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< Analog comparator0 window latch input pin. */
#define SYS_GPB_MFPL_PB3MFP_ETM_TRACE_DATA2   (0x0FUL << SYS_GPB_MFPL_PB3MFP_Pos) /*!< ETM Rx input bus bit2. */
#define SYS_GPB_MFPL_PB4MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB4MFP_EADC0_CH4         (0x01UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< EADC0 channel4 analog input. */
#define SYS_GPB_MFPL_PB4MFP_SPI0_SS           (0x02UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< 1st SPI0 slave select pin. */
#define SYS_GPB_MFPL_PB4MFP_SPI1_SS           (0x03UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPB_MFPL_PB4MFP_UART1_nCTS        (0x04UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< Clear to Send input pin for UART1. */
#define SYS_GPB_MFPL_PB4MFP_ACMP0_N           (0x05UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< Analog comparator0 negative input pin. */
#define SYS_GPB_MFPL_PB4MFP_PWM1_CH2          (0x06UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< PWM1 channel2 output/capture input. */
#define SYS_GPB_MFPL_PB4MFP_EBI_AD7           (0x07UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< EBI address/data bus bit7. */
#define SYS_GPB_MFPL_PB4MFP_UART4_TXD         (0x08UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< Data transmitter output pin for UART4. */
#define SYS_GPB_MFPL_PB4MFP_UART2_TXD         (0x09UL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< Data transmitter output pin for UART2. */
#define SYS_GPB_MFPL_PB4MFP_SC1_DAT           (0x0AUL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< SmartCard1 data pin. */
#define SYS_GPB_MFPL_PB4MFP_TM1_EXT           (0x0BUL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPB_MFPL_PB4MFP_USCI1_CLK         (0x0CUL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< USCI1 clock pin. */
#define SYS_GPB_MFPL_PB4MFP_SPI4_CLK          (0x0DUL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< SPI4 serial clock pin. */
#define SYS_GPB_MFPL_PB4MFP_ECAP1_IC0         (0x0EUL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< Input 0 of enhanced capture unit 1. */
#define SYS_GPB_MFPL_PB4MFP_ETM_TRACE_DATA3   (0x0FUL << SYS_GPB_MFPL_PB4MFP_Pos) /*!< ETM Rx input bus bit3. */
#define SYS_GPB_MFPL_PB5MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB5MFP_EADC0_CH13        (0x01UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< EADC0 channel1 analog input. */
#define SYS_GPB_MFPL_PB5MFP_SPI0_MOSI0        (0x02UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< 1st SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPB_MFPL_PB5MFP_SPI1_MOSI         (0x03UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPB_MFPL_PB5MFP_UART5_TXD         (0x04UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< Data transmitter output pin for UART5. */
#define SYS_GPB_MFPL_PB5MFP_ACMP0_P2          (0x05UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< Analog comparator0 positive input pin. */
#define SYS_GPB_MFPL_PB5MFP_PWM0_BRAKE1       (0x06UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< Brake input pin 1 of PWM0. */
#define SYS_GPB_MFPL_PB5MFP_EBI_AD6           (0x07UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< EBI address/data bus bit6. */
#define SYS_GPB_MFPL_PB5MFP_OPA0_O            (0x08UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< Operational amplifier output pin. */
#define SYS_GPB_MFPL_PB5MFP_UART2_RXD         (0x09UL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< Data receiver input pin for UART2. */
#define SYS_GPB_MFPL_PB5MFP_SC1_RST           (0x0BUL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< SmartCard1 reset pin. */
#define SYS_GPB_MFPL_PB5MFP_SD1_DAT3          (0x0EUL << SYS_GPB_MFPL_PB5MFP_Pos) /*!< SD/SDIO mode #1  data line bit 3. */
#define SYS_GPB_MFPL_PB6MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB6MFP_EADC0_CH14        (0x01UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< EADC0 channel1 analog input. */
#define SYS_GPB_MFPL_PB6MFP_SPI0_MISO0        (0x02UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< 1st SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPB_MFPL_PB6MFP_SPI1_MISO         (0x03UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPB_MFPL_PB6MFP_UART4_TXD         (0x04UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< Data transmitter output pin for UART4. */
#define SYS_GPB_MFPL_PB6MFP_ACMP0_P1          (0x05UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< Analog comparator0 positive input pin. */
#define SYS_GPB_MFPL_PB6MFP_PWM1_CH0          (0x06UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPB_MFPL_PB6MFP_EBI_AD5           (0x07UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< EBI address/data bus bit5. */
#define SYS_GPB_MFPL_PB6MFP_OPA0_N            (0x08UL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< Operational amplifier negative input pin. */
#define SYS_GPB_MFPL_PB6MFP_SC1_CLK           (0x0BUL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< SmartCard1 clock pin. */
#define SYS_GPB_MFPL_PB6MFP_USCI0_DAT0        (0x0CUL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< USCI0 data0 pin. */
#define SYS_GPB_MFPL_PB6MFP_BPWM0_CH0         (0x0DUL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< BPWM0 channel0 output/capture input. */
#define SYS_GPB_MFPL_PB6MFP_SD1_DAT2          (0x0EUL << SYS_GPB_MFPL_PB6MFP_Pos) /*!< SD/SDIO mode #1  data line bit 2. */
#define SYS_GPB_MFPL_PB7MFP_GPIO              (0x00UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPL_PB7MFP_EADC0_CH15        (0x01UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< EADC0 channel1 analog input. */
#define SYS_GPB_MFPL_PB7MFP_SPI0_CLK          (0x02UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< SPI0 serial clock pin. */
#define SYS_GPB_MFPL_PB7MFP_SPI1_CLK          (0x03UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPB_MFPL_PB7MFP_UART4_RXD         (0x04UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< Data receiver input pin for UART4. */
#define SYS_GPB_MFPL_PB7MFP_ACMP0_P0          (0x05UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< Analog comparator0 positive input pin. */
#define SYS_GPB_MFPL_PB7MFP_PWM1_CH1          (0x06UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPB_MFPL_PB7MFP_EBI_AD4           (0x07UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< EBI address/data bus bit4. */
#define SYS_GPB_MFPL_PB7MFP_OPA0_P            (0x08UL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< Operational amplifier positive input pin. */
#define SYS_GPB_MFPL_PB7MFP_EADC0_ST          (0x0AUL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< EADC external trigger input. */
#define SYS_GPB_MFPL_PB7MFP_SC1_DAT           (0x0BUL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< SmartCard1 data pin. */
#define SYS_GPB_MFPL_PB7MFP_USCI0_DAT1        (0x0CUL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< USCI0 data1 pin. */
#define SYS_GPB_MFPL_PB7MFP_BPWM0_CH1         (0x0DUL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< BPWM0 channel1 output/capture input. */
#define SYS_GPB_MFPL_PB7MFP_SD1_DAT1          (0x0EUL << SYS_GPB_MFPL_PB7MFP_Pos) /*!< SD/SDIO mode #1  data line bit 1. */
/********************* Bit definition of GPB_MFPH register **********************/
#define SYS_GPB_MFPH_PB8MFP_GPIO              (0x00UL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB8MFP_EADC0_CH5         (0x01UL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< EADC0 channel5 analog input. */
#define SYS_GPB_MFPH_PB8MFP_SPI4_SS           (0x02UL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< 1st SPI4 slave select pin. */
#define SYS_GPB_MFPH_PB8MFP_UART1_nRTS        (0x04UL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< Request to Send output pin for UART1. */
#define SYS_GPB_MFPH_PB8MFP_PWM0_CH2          (0x06UL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< PWM0 channel2 output/capture input. */
#define SYS_GPB_MFPH_PB8MFP_EBI_AD8           (0x07UL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< EBI address/data bus bit8. */
#define SYS_GPB_MFPH_PB8MFP_UART4_RXD         (0x08UL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< Data receiver input pin for UART4. */
#define SYS_GPB_MFPH_PB8MFP_ECAP1_IC1         (0x0EUL << SYS_GPB_MFPH_PB8MFP_Pos) /*!< Input 1 of enhanced capture unit 1. */
#define SYS_GPB_MFPH_PB9MFP_GPIO              (0x00UL << SYS_GPB_MFPH_PB9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB9MFP_EADC0_CH6         (0x01UL << SYS_GPB_MFPH_PB9MFP_Pos) /*!< EADC0 channel6 analog input. */
#define SYS_GPB_MFPH_PB9MFP_SPI4_I2SMCLK      (0x02UL << SYS_GPB_MFPH_PB9MFP_Pos) /*!< SPI4 I2S master clock output pin. */
#define SYS_GPB_MFPH_PB9MFP_UART5_nRTS        (0x04UL << SYS_GPB_MFPH_PB9MFP_Pos) /*!< Request to Send output pin for UART5. */
#define SYS_GPB_MFPH_PB9MFP_EBI_AD9           (0x07UL << SYS_GPB_MFPH_PB9MFP_Pos) /*!< EBI address/data bus bit9. */
#define SYS_GPB_MFPH_PB10MFP_GPIO             (0x00UL << SYS_GPB_MFPH_PB10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB10MFP_EADC0_CH7        (0x01UL << SYS_GPB_MFPH_PB10MFP_Pos) /*!< EADC0 channel7 analog input. */
#define SYS_GPB_MFPH_PB10MFP_SPI4_CLK         (0x02UL << SYS_GPB_MFPH_PB10MFP_Pos) /*!< SPI4 serial clock pin. */
#define SYS_GPB_MFPH_PB10MFP_UART5_nCTS       (0x04UL << SYS_GPB_MFPH_PB10MFP_Pos) /*!< Clear to Send input pin for UART5. */
#define SYS_GPB_MFPH_PB10MFP_EBI_AD10         (0x07UL << SYS_GPB_MFPH_PB10MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPB_MFPH_PB11MFP_GPIO             (0x00UL << SYS_GPB_MFPH_PB11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB11MFP_EADC0_CH8        (0x01UL << SYS_GPB_MFPH_PB11MFP_Pos) /*!< EADC0 channel8 analog input. */
#define SYS_GPB_MFPH_PB11MFP_SPI4_MISO        (0x02UL << SYS_GPB_MFPH_PB11MFP_Pos) /*!< 1st SPI4 MISO (Master In, Slave Out) pin. */
#define SYS_GPB_MFPH_PB11MFP_UART5_TXD        (0x04UL << SYS_GPB_MFPH_PB11MFP_Pos) /*!< Data transmitter output pin for UART5. */
#define SYS_GPB_MFPH_PB11MFP_EBI_ADR18        (0x07UL << SYS_GPB_MFPH_PB11MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPB_MFPH_PB11MFP_UART4_nCTS       (0x08UL << SYS_GPB_MFPH_PB11MFP_Pos) /*!< Clear to Send input pin for UART4. */
#define SYS_GPB_MFPH_PB11MFP_ECAP1_IC2        (0x0EUL << SYS_GPB_MFPH_PB11MFP_Pos) /*!< Input 1 of enhanced capture unit 2. */
#define SYS_GPB_MFPH_PB12MFP_GPIO             (0x00UL << SYS_GPB_MFPH_PB12MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB12MFP_EADC0_CH9        (0x01UL << SYS_GPB_MFPH_PB12MFP_Pos) /*!< EADC0 channel9 analog input. */
#define SYS_GPB_MFPH_PB12MFP_SPI4_MOSI        (0x02UL << SYS_GPB_MFPH_PB12MFP_Pos) /*!< 1st SPI4 MOSI (Master Out, Slave In) pin. */
#define SYS_GPB_MFPH_PB12MFP_UART5_RXD        (0x04UL << SYS_GPB_MFPH_PB12MFP_Pos) /*!< Data receiver input pin for UART5. */
#define SYS_GPB_MFPH_PB12MFP_EBI_ADR19        (0x07UL << SYS_GPB_MFPH_PB12MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPB_MFPH_PB12MFP_UART4_nRTS       (0x08UL << SYS_GPB_MFPH_PB12MFP_Pos) /*!< Request to Send output pin for UART4. */
#define SYS_GPB_MFPH_PB13MFP_GPIO             (0x00UL << SYS_GPB_MFPH_PB13MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB13MFP_EADC0_CH10       (0x01UL << SYS_GPB_MFPH_PB13MFP_Pos) /*!< EADC0 channel1 analog input. */
#define SYS_GPB_MFPH_PB13MFP_EBI_ADR9         (0x07UL << SYS_GPB_MFPH_PB13MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPB_MFPH_PB14MFP_GPIO             (0x00UL << SYS_GPB_MFPH_PB14MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB14MFP_EADC0_CH11       (0x01UL << SYS_GPB_MFPH_PB14MFP_Pos) /*!< EADC0 channel1 analog input. */
#define SYS_GPB_MFPH_PB14MFP_EBI_ADR8         (0x07UL << SYS_GPB_MFPH_PB14MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPB_MFPH_PB15MFP_GPIO             (0x00UL << SYS_GPB_MFPH_PB15MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPB_MFPH_PB15MFP_EADC0_CH12       (0x01UL << SYS_GPB_MFPH_PB15MFP_Pos) /*!< EADC0 channel1 analog input. */
#define SYS_GPB_MFPH_PB15MFP_SPI1_SS          (0x03UL << SYS_GPB_MFPH_PB15MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPB_MFPH_PB15MFP_UART5_RXD        (0x04UL << SYS_GPB_MFPH_PB15MFP_Pos) /*!< Data receiver input pin for UART5. */
#define SYS_GPB_MFPH_PB15MFP_ACMP0_P3         (0x05UL << SYS_GPB_MFPH_PB15MFP_Pos) /*!< Analog comparator0 positive input pin. */
#define SYS_GPB_MFPH_PB15MFP_EBI_nCS1         (0x07UL << SYS_GPB_MFPH_PB15MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPB_MFPH_PB15MFP_SC1_PWR          (0x0BUL << SYS_GPB_MFPH_PB15MFP_Pos) /*!< SmartCard1 power pin. */
/********************* Bit definition of GPC_MFPL register **********************/
#define SYS_GPC_MFPL_PC0MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC0MFP_SPI0_MISO1        (0x01UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< 2nd SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPC_MFPL_PC0MFP_SPI2_CLK          (0x02UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< SPI2 serial clock pin. */
#define SYS_GPC_MFPL_PC0MFP_UART2_nCTS        (0x03UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< Clear to Send input pin for UART2. */
#define SYS_GPC_MFPL_PC0MFP_CAN0_TXD          (0x04UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< CAN0 bus transmitter output. */
#define SYS_GPC_MFPL_PC0MFP_TM3_EXT           (0x05UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPC_MFPL_PC0MFP_PWM0_CH0          (0x06UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< PWM0 channel0 output/capture input. */
#define SYS_GPC_MFPL_PC0MFP_EBI_AD8           (0x07UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< EBI address/data bus bit8. */
#define SYS_GPC_MFPL_PC0MFP_INT2              (0x08UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< External interrupt2 input pin. */
#define SYS_GPC_MFPL_PC0MFP_UART3_TXD         (0x09UL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPC_MFPL_PC0MFP_I2S0_LRCK         (0x0AUL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< I2S0 left right channel clock. */
#define SYS_GPC_MFPL_PC0MFP_SD0_CLK           (0x0BUL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< SD/SDIO mode #0??clock. */
#define SYS_GPC_MFPL_PC0MFP_USCI0_CLK         (0x0CUL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< USCI0 clock pin. */
#define SYS_GPC_MFPL_PC0MFP_ETM_TRACE_CLK     (0x0DUL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< ETM Rx clock input pin. */
#define SYS_GPC_MFPL_PC0MFP_EMAC_REFCLK       (0x0EUL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< EMAC mode clock input. */
#define SYS_GPC_MFPL_PC0MFP_SPIM_MISO         (0x0FUL << SYS_GPC_MFPL_PC0MFP_Pos) /*!< 1st SPI* MISO (Master In, Slave Out) pin. */
#define SYS_GPC_MFPL_PC1MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC1MFP_CLKO              (0x01UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< Clock Output pin. */
#define SYS_GPC_MFPL_PC1MFP_DAC0_ST           (0x02UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< DAC0 external trigger input. */
#define SYS_GPC_MFPL_PC1MFP_UART2_nRTS        (0x03UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< Request to Send output pin for UART2. */
#define SYS_GPC_MFPL_PC1MFP_CAN0_RXD          (0x04UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< CAN0 bus receiver input. */
#define SYS_GPC_MFPL_PC1MFP_SPI2_I2SMCLK      (0x05UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< SPI2 I2S master clock output pin. */
#define SYS_GPC_MFPL_PC1MFP_PWM0_CH1          (0x06UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< PWM0 channel1 output/capture input. */
#define SYS_GPC_MFPL_PC1MFP_EBI_AD9           (0x07UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< EBI address/data bus bit9. */
#define SYS_GPC_MFPL_PC1MFP_SPI0_MOSI1        (0x08UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< 2nd SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPC_MFPL_PC1MFP_UART3_RXD         (0x09UL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPC_MFPL_PC1MFP_I2S0_DO           (0x0AUL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< I2S0 data output. */
#define SYS_GPC_MFPL_PC1MFP_SD0_DAT3          (0x0BUL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< SD/SDIO mode #0  data line bit 3. */
#define SYS_GPC_MFPL_PC1MFP_USCI0_DAT0        (0x0CUL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< USCI0 data0 pin. */
#define SYS_GPC_MFPL_PC1MFP_ETM_TRACE_DATA0   (0x0DUL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< ETM Rx input bus bit0. */
#define SYS_GPC_MFPL_PC1MFP_EMAC_MII_RXD0     (0x0EUL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< MII/RMII Receive Data bus bit 0. */
#define SYS_GPC_MFPL_PC1MFP_SPIM_MOSI         (0x0FUL << SYS_GPC_MFPL_PC1MFP_Pos) /*!< 1st SPI* MOSI (Master Out, Slave In) pin. */
#define SYS_GPC_MFPL_PC2MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC2MFP_CAN1_TXD          (0x01UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< CAN1 bus transmitter output. */
#define SYS_GPC_MFPL_PC2MFP_SPI2_SS           (0x02UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< 1st SPI2 slave select pin. */
#define SYS_GPC_MFPL_PC2MFP_UART2_TXD         (0x03UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< Data transmitter output pin for UART2. */
#define SYS_GPC_MFPL_PC2MFP_TM3               (0x04UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPC_MFPL_PC2MFP_ACMP1_O           (0x05UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< Analog comparator1 output. */
#define SYS_GPC_MFPL_PC2MFP_PWM0_CH2          (0x06UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< PWM0 channel2 output/capture input. */
#define SYS_GPC_MFPL_PC2MFP_EBI_AD10          (0x07UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPC_MFPL_PC2MFP_SPI0_MISO0        (0x08UL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< 1st SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPC_MFPL_PC2MFP_I2S0_MCLK         (0x0AUL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< I2S0 master clock output pin. */
#define SYS_GPC_MFPL_PC2MFP_SD0_DAT2          (0x0BUL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< SD/SDIO mode #0  data line bit 2. */
#define SYS_GPC_MFPL_PC2MFP_USCI0_DAT1        (0x0CUL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< USCI0 data1 pin. */
#define SYS_GPC_MFPL_PC2MFP_ETM_TRACE_DATA1   (0x0DUL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< ETM Rx input bus bit1. */
#define SYS_GPC_MFPL_PC2MFP_EMAC_MII_RXD1     (0x0EUL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< MII/RMII Receive Data bus bit 1. */
#define SYS_GPC_MFPL_PC2MFP_SPIM_CLK          (0x0FUL << SYS_GPC_MFPL_PC2MFP_Pos) /*!< SPI* serial clock pin. */
#define SYS_GPC_MFPL_PC3MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC3MFP_CAN1_RXD          (0x01UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< CAN1 bus receiver input. */
#define SYS_GPC_MFPL_PC3MFP_SPI2_MOSI         (0x02UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< 1st SPI2 MOSI (Master Out, Slave In) pin. */
#define SYS_GPC_MFPL_PC3MFP_UART2_RXD         (0x03UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< Data receiver input pin for UART2. */
#define SYS_GPC_MFPL_PC3MFP_HSUSB_VBUS_ST     (0x04UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPC_MFPL_PC3MFP_PWM0_CH3          (0x06UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< PWM0 channel3 output/capture input. */
#define SYS_GPC_MFPL_PC3MFP_EBI_AD11          (0x07UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPC_MFPL_PC3MFP_SPI0_MOSI0        (0x08UL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< 1st SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPC_MFPL_PC3MFP_I2S0_BCLK         (0x0AUL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< I2S0 bit clock pin. */
#define SYS_GPC_MFPL_PC3MFP_SD0_DAT1          (0x0BUL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< SD/SDIO mode #0  data line bit 1. */
#define SYS_GPC_MFPL_PC3MFP_USCI0_CTL0        (0x0CUL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< USCI0 control0 pin. */
#define SYS_GPC_MFPL_PC3MFP_ETM_TRACE_DATA2   (0x0DUL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< ETM Rx input bus bit2. */
#define SYS_GPC_MFPL_PC3MFP_EMAC_MII_RXDV     (0x0EUL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< MII Receive Data Valid / RMII CRS_DV input. */
#define SYS_GPC_MFPL_PC3MFP_SPIM_SS           (0x0FUL << SYS_GPC_MFPL_PC3MFP_Pos) /*!< 1st SPI* slave select pin. */
#define SYS_GPC_MFPL_PC4MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC4MFP_SC0_PWR           (0x01UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< SmartCard0 power pin. */
#define SYS_GPC_MFPL_PC4MFP_SPI2_MISO         (0x02UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< 1st SPI2 MISO (Master In, Slave Out) pin. */
#define SYS_GPC_MFPL_PC4MFP_I2C1_SCL          (0x03UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< I2C1 clock pin. */
#define SYS_GPC_MFPL_PC4MFP_HSUSB_VBUS_EN     (0x04UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPC_MFPL_PC4MFP_USCI1_CLK         (0x05UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< USCI1 clock pin. */
#define SYS_GPC_MFPL_PC4MFP_PWM0_CH4          (0x06UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< PWM0 channel4 output/capture input. */
#define SYS_GPC_MFPL_PC4MFP_EBI_AD12          (0x07UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPC_MFPL_PC4MFP_SPI0_CLK          (0x08UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< SPI0 serial clock pin. */
#define SYS_GPC_MFPL_PC4MFP_UART4_nCTS        (0x09UL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< Clear to Send input pin for UART4. */
#define SYS_GPC_MFPL_PC4MFP_I2S0_DI           (0x0AUL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< I2S0 data input. */
#define SYS_GPC_MFPL_PC4MFP_SD0_DAT0          (0x0BUL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< SD/SDIO mode #0  data line bit 0. */
#define SYS_GPC_MFPL_PC4MFP_USCI0_CTL1        (0x0CUL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< USCI0 control1 pin. */
#define SYS_GPC_MFPL_PC4MFP_ETM_TRACE_DATA3   (0x0DUL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< ETM Rx input bus bit3. */
#define SYS_GPC_MFPL_PC4MFP_EMAC_MII_RXERR    (0x0EUL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< MII/RMII Receive Data error. */
#define SYS_GPC_MFPL_PC4MFP_SPIM_D2           (0x0FUL << SYS_GPC_MFPL_PC4MFP_Pos) /*!< SPIM data 2 pin for Quad Mode I/O. */
#define SYS_GPC_MFPL_PC5MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC5MFP_SPI2_I2SMCLK      (0x02UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< SPI2 I2S master clock output pin. */
#define SYS_GPC_MFPL_PC5MFP_DAC1_ST           (0x03UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< DAC1 external trigger input. */
#define SYS_GPC_MFPL_PC5MFP_BPWM0_CH1         (0x04UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< BPWM0 channel1 output/capture input. */
#define SYS_GPC_MFPL_PC5MFP_PWM0_CH1          (0x06UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< PWM0 channel1 output/capture input. */
#define SYS_GPC_MFPL_PC5MFP_EBI_AD13          (0x07UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPC_MFPL_PC5MFP_SPI0_SS           (0x08UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< 1st SPI0 slave select pin. */
#define SYS_GPC_MFPL_PC5MFP_UART4_TXD         (0x09UL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< Data transmitter output pin for UART4. */
#define SYS_GPC_MFPL_PC5MFP_SPIM_D3           (0x0FUL << SYS_GPC_MFPL_PC5MFP_Pos) /*!< SPIM data 3 pin for Quad Mode I/O. */
#define SYS_GPC_MFPL_PC6MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC6MFP_I2C1_SMBAL        (0x03UL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< I2C1 SMBus SMBALTER# pin  */
#define SYS_GPC_MFPL_PC6MFP_SC0_DAT           (0x04UL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< SmartCard0 data pin. */
#define SYS_GPC_MFPL_PC6MFP_ACMP1_O           (0x05UL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< Analog comparator1 output. */
#define SYS_GPC_MFPL_PC6MFP_PWM1_CH0          (0x06UL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPC_MFPL_PC6MFP_EBI_AD14          (0x07UL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPC_MFPL_PC6MFP_UART0_RXD         (0x09UL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPC_MFPL_PC6MFP_UART4_RXD         (0x0AUL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< Data receiver input pin for UART4. */
#define SYS_GPC_MFPL_PC6MFP_SD1_nCD           (0x0BUL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< SD/SDIO mode #1 ??card detect */
#define SYS_GPC_MFPL_PC6MFP_ECAP0_IC1         (0x0EUL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< Input 1 of enhanced capture unit 0. */
#define SYS_GPC_MFPL_PC6MFP_SPIM_SS           (0x0FUL << SYS_GPC_MFPL_PC6MFP_Pos) /*!< 1st SPI* slave select pin. */
#define SYS_GPC_MFPL_PC7MFP_GPIO              (0x00UL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPL_PC7MFP_I2C1_SMBSUS       (0x03UL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< I2C1 SMBus SMBSUS# pin (PMBus CONTROL pin) */
#define SYS_GPC_MFPL_PC7MFP_SC0_CLK           (0x04UL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< SmartCard0 clock pin. */
#define SYS_GPC_MFPL_PC7MFP_PWM1_CH1          (0x06UL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPC_MFPL_PC7MFP_EBI_AD15          (0x07UL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPC_MFPL_PC7MFP_UART0_TXD         (0x09UL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< Data transmitter output pin for UART0. */
#define SYS_GPC_MFPL_PC7MFP_UART4_nRTS        (0x0AUL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< Request to Send output pin for UART4. */
#define SYS_GPC_MFPL_PC7MFP_SD1_CMD           (0x0BUL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< SD/SDIO mode #1 ??command/response. */
#define SYS_GPC_MFPL_PC7MFP_ECAP0_IC0         (0x0EUL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< Input 0 of enhanced capture unit 0. */
#define SYS_GPC_MFPL_PC7MFP_SPIM_CLK          (0x0FUL << SYS_GPC_MFPL_PC7MFP_Pos) /*!< SPI* serial clock pin. */
/********************* Bit definition of GPC_MFPH reg ister **********************/
#define SYS_GPC_MFPH_PC8MFP_GPIO              (0x00UL << SYS_GPC_MFPH_PC8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC8MFP_SPI2_I2SMCLK      (0x04UL << SYS_GPC_MFPH_PC8MFP_Pos) /*!< SPI2 I2S master clock output pin. */
#define SYS_GPC_MFPH_PC8MFP_EBI_nCS2          (0x07UL << SYS_GPC_MFPH_PC8MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPC_MFPH_PC8MFP_BPWM0_CH0         (0x0DUL << SYS_GPC_MFPH_PC8MFP_Pos) /*!< BPWM0 channel0 output/capture input. */
#define SYS_GPC_MFPH_PC9MFP_GPIO              (0x00UL << SYS_GPC_MFPH_PC9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC9MFP_SPI2_I2SMCLK      (0x02UL << SYS_GPC_MFPH_PC9MFP_Pos) /*!< SPI2 I2S master clock output pin. */
#define SYS_GPC_MFPH_PC9MFP_PWM1_CH0          (0x06UL << SYS_GPC_MFPH_PC9MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPC_MFPH_PC9MFP_EBI_ADR15         (0x07UL << SYS_GPC_MFPH_PC9MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPC_MFPH_PC9MFP_EMAC_MII_MDC      (0x0EUL << SYS_GPC_MFPH_PC9MFP_Pos) /*!< MII/RMII Management Data Clock. */
#define SYS_GPC_MFPH_PC10MFP_GPIO             (0x00UL << SYS_GPC_MFPH_PC10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC10MFP_SPI2_MOSI        (0x02UL << SYS_GPC_MFPH_PC10MFP_Pos) /*!< 1st SPI2 MOSI (Master Out, Slave In) pin. */
#define SYS_GPC_MFPH_PC10MFP_PWM1_CH1         (0x06UL << SYS_GPC_MFPH_PC10MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPC_MFPH_PC10MFP_EBI_ADR14        (0x07UL << SYS_GPC_MFPH_PC10MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPC_MFPH_PC10MFP_EMAC_MII_MDIO    (0x0EUL << SYS_GPC_MFPH_PC10MFP_Pos) /*!< MII/RMII Management Data I/O. */
#define SYS_GPC_MFPH_PC11MFP_GPIO             (0x00UL << SYS_GPC_MFPH_PC11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC11MFP_SPI2_MISO        (0x02UL << SYS_GPC_MFPH_PC11MFP_Pos) /*!< 1st SPI2 MISO (Master In, Slave Out) pin. */
#define SYS_GPC_MFPH_PC11MFP_PWM1_CH2         (0x06UL << SYS_GPC_MFPH_PC11MFP_Pos) /*!< PWM1 channel2 output/capture input. */
#define SYS_GPC_MFPH_PC11MFP_EBI_ADR13        (0x07UL << SYS_GPC_MFPH_PC11MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPC_MFPH_PC11MFP_EMAC_MII_TXD0    (0x0EUL << SYS_GPC_MFPH_PC11MFP_Pos) /*!< MII/RMII Transmit Data bus bit 0. */
#define SYS_GPC_MFPH_PC12MFP_GPIO             (0x00UL << SYS_GPC_MFPH_PC12MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC12MFP_SPI2_CLK         (0x02UL << SYS_GPC_MFPH_PC12MFP_Pos) /*!< SPI2 serial clock pin. */
#define SYS_GPC_MFPH_PC12MFP_PWM1_CH3         (0x06UL << SYS_GPC_MFPH_PC12MFP_Pos) /*!< PWM1 channel3 output/capture input. */
#define SYS_GPC_MFPH_PC12MFP_EBI_ADR12        (0x07UL << SYS_GPC_MFPH_PC12MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPC_MFPH_PC12MFP_EMAC_MII_TXD1    (0x0EUL << SYS_GPC_MFPH_PC12MFP_Pos) /*!< MII/RMII Transmit Data bus bit 1. */
#define SYS_GPC_MFPH_PC13MFP_GPIO             (0x00UL << SYS_GPC_MFPH_PC13MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC13MFP_SPI2_SS          (0x02UL << SYS_GPC_MFPH_PC13MFP_Pos) /*!< 1st SPI2 slave select pin. */
#define SYS_GPC_MFPH_PC13MFP_PWM1_CH4         (0x06UL << SYS_GPC_MFPH_PC13MFP_Pos) /*!< PWM1 channel4 output/capture input. */
#define SYS_GPC_MFPH_PC13MFP_EBI_ADR11        (0x07UL << SYS_GPC_MFPH_PC13MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPC_MFPH_PC13MFP_EMAC_MII_TXEN    (0x0EUL << SYS_GPC_MFPH_PC13MFP_Pos) /*!< MII/RMII? Transmit Enable. */
#define SYS_GPC_MFPH_PC14MFP_GPIO             (0x00UL << SYS_GPC_MFPH_PC14MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC14MFP_PWM1_CH5         (0x06UL << SYS_GPC_MFPH_PC14MFP_Pos) /*!< PWM1 channel5 output/capture input. */
#define SYS_GPC_MFPH_PC14MFP_EBI_ADR10        (0x07UL << SYS_GPC_MFPH_PC14MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPC_MFPH_PC14MFP_EMAC_PPS         (0x0EUL << SYS_GPC_MFPH_PC14MFP_Pos) /*!< EMAC Pulse Per Second output */
#define SYS_GPC_MFPH_PC15MFP_GPIO             (0x00UL << SYS_GPC_MFPH_PC15MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPC_MFPH_PC15MFP_PWM1_CH0         (0x06UL << SYS_GPC_MFPH_PC15MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPC_MFPH_PC15MFP_EBI_ADR0         (0x07UL << SYS_GPC_MFPH_PC15MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPC_MFPH_PC15MFP_SPI3_I2SMCLK     (0x09UL << SYS_GPC_MFPH_PC15MFP_Pos) /*!< SPI3 I2S master clock output pin. */
#define SYS_GPC_MFPH_PC15MFP_HSUSB_VBUS_ST    (0x0BUL << SYS_GPC_MFPH_PC15MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
/********************* Bit definition of GPD_MFPL register **********************/
#define SYS_GPD_MFPL_PD0MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD0MFP_SPI1_I2SMCLK      (0x02UL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< SPI1 I2S master clock output pin. */
#define SYS_GPD_MFPL_PD0MFP_UART0_RXD         (0x03UL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPD_MFPL_PD0MFP_UART5_RXD         (0x04UL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< Data receiver input pin for UART5. */
#define SYS_GPD_MFPL_PD0MFP_ACMP1_N           (0x05UL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< Analog comparator1 negative input pin. */
#define SYS_GPD_MFPL_PD0MFP_EBI_ALE           (0x07UL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< EBI address latch enable output pin. */
#define SYS_GPD_MFPL_PD0MFP_INT3              (0x08UL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< External interrupt3 input pin. */
#define SYS_GPD_MFPL_PD0MFP_SC0_PWR           (0x0AUL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< SmartCard0 power pin. */
#define SYS_GPD_MFPL_PD0MFP_SC1_nCD           (0x0BUL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< SmartCard1 card detect pin. */
#define SYS_GPD_MFPL_PD0MFP_TM3               (0x0CUL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPD_MFPL_PD0MFP_BPWM0_CH2         (0x0DUL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< BPWM0 channel2 output/capture input. */
#define SYS_GPD_MFPL_PD0MFP_SD1_DAT0          (0x0EUL << SYS_GPD_MFPL_PD0MFP_Pos) /*!< SD/SDIO mode #1  data line bit 0. */
#define SYS_GPD_MFPL_PD1MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD1MFP_PWM0_SYNC_IN      (0x02UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< PWM0 counter synchronous trigger input pin. */
#define SYS_GPD_MFPL_PD1MFP_UART0_TXD         (0x03UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< Data transmitter output pin for UART0. */
#define SYS_GPD_MFPL_PD1MFP_SPI2_MISO         (0x04UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< 1st SPI2 MISO (Master In, Slave Out) pin. */
#define SYS_GPD_MFPL_PD1MFP_ACMP1_P2          (0x05UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< Analog comparator1 positive input pin. */
#define SYS_GPD_MFPL_PD1MFP_TM0               (0x06UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPD_MFPL_PD1MFP_EBI_nRD           (0x07UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< EBI read enable output pin. */
#define SYS_GPD_MFPL_PD1MFP_QEI0_B            (0x08UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< Quadrature encoder phase B input of QEI Unit 0. */
#define SYS_GPD_MFPL_PD1MFP_OPA1_O            (0x09UL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< Operational amplifier output pin. */
#define SYS_GPD_MFPL_PD1MFP_SC0_RST           (0x0AUL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< SmartCard0 reset pin. */
#define SYS_GPD_MFPL_PD1MFP_SC2_RST           (0x0BUL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< SmartCard2 reset pin. */
#define SYS_GPD_MFPL_PD1MFP_USCI0_CTL0        (0x0CUL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< USCI0 control0 pin. */
#define SYS_GPD_MFPL_PD1MFP_BPWM0_CH3         (0x0DUL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< BPWM0 channel3 output/capture input. */
#define SYS_GPD_MFPL_PD1MFP_SD1_CLK           (0x0EUL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< SD/SDIO mode #1??clock. */
#define SYS_GPD_MFPL_PD1MFP_SPI3_I2SMCLK      (0x0FUL << SYS_GPD_MFPL_PD1MFP_Pos) /*!< SPI3 I2S master clock output pin. */
#define SYS_GPD_MFPL_PD2MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD2MFP_EADC0_ST          (0x01UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< EADC external trigger input. */
#define SYS_GPD_MFPL_PD2MFP_UART3_TXD         (0x02UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPD_MFPL_PD2MFP_TM0_EXT           (0x03UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPD_MFPL_PD2MFP_SPI2_CLK          (0x04UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< SPI2 serial clock pin. */
#define SYS_GPD_MFPL_PD2MFP_ACMP1_P1          (0x05UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< Analog comparator1 positive input pin. */
#define SYS_GPD_MFPL_PD2MFP_PWM0_BRAKE0       (0x06UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< Brake input pin 0 of PWM0. */
#define SYS_GPD_MFPL_PD2MFP_EBI_nWR           (0x07UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< EBI wirte enable output pin. */
#define SYS_GPD_MFPL_PD2MFP_INT0              (0x08UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< External interrupt0 input pin. */
#define SYS_GPD_MFPL_PD2MFP_OPA1_N            (0x09UL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< Operational amplifier negative input pin. */
#define SYS_GPD_MFPL_PD2MFP_SC0_DAT           (0x0AUL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< SmartCard0 data pin. */
#define SYS_GPD_MFPL_PD2MFP_SC2_CLK           (0x0BUL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< SmartCard2 clock pin. */
#define SYS_GPD_MFPL_PD2MFP_USCI0_CTL1        (0x0CUL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< USCI0 control1 pin. */
#define SYS_GPD_MFPL_PD2MFP_BPWM0_CH4         (0x0DUL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< BPWM0 channel4 output/capture input. */
#define SYS_GPD_MFPL_PD2MFP_SD1_nCD           (0x0EUL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< SD/SDIO mode #1 ??card detect */
#define SYS_GPD_MFPL_PD2MFP_SWDH_CLK          (0x0FUL << SYS_GPD_MFPL_PD2MFP_Pos) /*!< SWD Host interface clock output pin. */
#define SYS_GPD_MFPL_PD3MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD3MFP_TM2               (0x01UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPD_MFPL_PD3MFP_UART3_RXD         (0x02UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPD_MFPL_PD3MFP_TM1_EXT           (0x03UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPD_MFPL_PD3MFP_SPI1_I2SMCLK      (0x04UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< SPI1 I2S master clock output pin. */
#define SYS_GPD_MFPL_PD3MFP_ACMP1_P0          (0x05UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< Analog comparator1 positive input pin. */
#define SYS_GPD_MFPL_PD3MFP_PWM0_BRAKE1       (0x06UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< Brake input pin 1 of PWM0. */
#define SYS_GPD_MFPL_PD3MFP_EBI_MCLK          (0x07UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< EBI external clock output pin. */
#define SYS_GPD_MFPL_PD3MFP_INT1              (0x08UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< External interrupt1 input pin. */
#define SYS_GPD_MFPL_PD3MFP_OPA1_P            (0x09UL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< Operational amplifier positive input pin. */
#define SYS_GPD_MFPL_PD3MFP_SC0_CLK           (0x0AUL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< SmartCard0 clock pin. */
#define SYS_GPD_MFPL_PD3MFP_SC2_DAT           (0x0BUL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< SmartCard2 data pin. */
#define SYS_GPD_MFPL_PD3MFP_USCI0_CLK         (0x0CUL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< USCI0 clock pin. */
#define SYS_GPD_MFPL_PD3MFP_BPWM0_CH5         (0x0DUL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< BPWM0 channel5 output/capture input. */
#define SYS_GPD_MFPL_PD3MFP_SD1_CMD           (0x0EUL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< SD/SDIO mode #1 ??command/response. */
#define SYS_GPD_MFPL_PD3MFP_SWDH_DAT          (0x0FUL << SYS_GPD_MFPL_PD3MFP_Pos) /*!< SWD Host interface input/output bus bit. */
#define SYS_GPD_MFPL_PD4MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD4MFP_SPI1_CLK          (0x02UL << SYS_GPD_MFPL_PD4MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPD_MFPL_PD4MFP_I2C0_SDA          (0x03UL << SYS_GPD_MFPL_PD4MFP_Pos) /*!< I2C0 data input/output pin. */
#define SYS_GPD_MFPL_PD4MFP_PWM0_BRAKE0       (0x05UL << SYS_GPD_MFPL_PD4MFP_Pos) /*!< Brake input pin 0 of PWM0. */
#define SYS_GPD_MFPL_PD4MFP_TM0               (0x06UL << SYS_GPD_MFPL_PD4MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPD_MFPL_PD4MFP_EBI_AD11          (0x07UL << SYS_GPD_MFPL_PD4MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPD_MFPL_PD5MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD5MFP_CLKO              (0x01UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< Clock Output pin. */
#define SYS_GPD_MFPL_PD5MFP_SPI1_MISO         (0x02UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPD_MFPL_PD5MFP_I2C0_SCL          (0x03UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< I2C0 clock pin. */
#define SYS_GPD_MFPL_PD5MFP_PWM0_BRAKE1       (0x05UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< Brake input pin 1 of PWM0. */
#define SYS_GPD_MFPL_PD5MFP_TM1               (0x06UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPD_MFPL_PD5MFP_EBI_AD12          (0x07UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPD_MFPL_PD5MFP_OPA2_O            (0x09UL << SYS_GPD_MFPL_PD5MFP_Pos) /*!< Operational amplifier output pin. */
#define SYS_GPD_MFPL_PD6MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD6MFP_CLKO              (0x01UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< Clock Output pin. */
#define SYS_GPD_MFPL_PD6MFP_SPI1_SS           (0x02UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPD_MFPL_PD6MFP_UART0_RXD         (0x03UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPD_MFPL_PD6MFP_I2C1_SDA          (0x04UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< I2C1 data input/output pin. */
#define SYS_GPD_MFPL_PD6MFP_ACMP0_O           (0x05UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< Analog comparator0 output. */
#define SYS_GPD_MFPL_PD6MFP_PWM0_CH5          (0x06UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< PWM0 channel5 output/capture input. */
#define SYS_GPD_MFPL_PD6MFP_EBI_nWR           (0x07UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< EBI wirte enable output pin. */
#define SYS_GPD_MFPL_PD6MFP_OPA2_P            (0x09UL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< Operational amplifier positive input pin. */
#define SYS_GPD_MFPL_PD6MFP_SD0_CMD           (0x0EUL << SYS_GPD_MFPL_PD6MFP_Pos) /*!< SD/SDIO mode #0 ??command/response. */
#define SYS_GPD_MFPL_PD7MFP_GPIO              (0x00UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPL_PD7MFP_CAN1_RXD          (0x01UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< CAN1 bus receiver input. */
#define SYS_GPD_MFPL_PD7MFP_PWM0_SYNC_IN      (0x03UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< PWM0 counter synchronous trigger input pin. */
#define SYS_GPD_MFPL_PD7MFP_TM1               (0x04UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPD_MFPL_PD7MFP_ACMP0_O           (0x05UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< Analog comparator0 output. */
#define SYS_GPD_MFPL_PD7MFP_PWM0_CH5          (0x06UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< PWM0 channel5 output/capture input. */
#define SYS_GPD_MFPL_PD7MFP_EBI_nRD           (0x07UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< EBI read enable output pin. */
#define SYS_GPD_MFPL_PD7MFP_UART0_TXD         (0x08UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< Data transmitter output pin for UART0. */
#define SYS_GPD_MFPL_PD7MFP_I2C0_SDA          (0x09UL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< I2C0 data input/output pin. */
#define SYS_GPD_MFPL_PD7MFP_SC2_PWR           (0x0AUL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< SmartCard2 power pin. */
#define SYS_GPD_MFPL_PD7MFP_SC0_nCD           (0x0BUL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< SmartCard0 card detect pin. */
#define SYS_GPD_MFPL_PD7MFP_PWM1_BRAKE1       (0x0CUL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< Brake input pin 1 of PWM1. */
#define SYS_GPD_MFPL_PD7MFP_ECAP1_IC1         (0x0DUL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< Input 1 of enhanced capture unit 1. */
#define SYS_GPD_MFPL_PD7MFP_SD0_CMD           (0x0EUL << SYS_GPD_MFPL_PD7MFP_Pos) /*!< SD/SDIO mode #0 ??command/response. */
/********************* Bit definition of GPD_MFPH register **********************/
#define SYS_GPD_MFPH_PD8MFP_GPIO              (0x00UL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD8MFP_I2C0_SDA          (0x03UL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< I2C0 data input/output pin. */
#define SYS_GPD_MFPH_PD8MFP_SPI2_SS           (0x04UL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< 1st SPI2 slave select pin. */
#define SYS_GPD_MFPH_PD8MFP_ACMP1_WLAT        (0x05UL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< Analog comparator1 window latch input pin. */
#define SYS_GPD_MFPH_PD8MFP_EBI_nCS0          (0x07UL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPD_MFPH_PD8MFP_QEI0_INDEX        (0x08UL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< Quadrature encoder index input of QEI Unit 0. */
#define SYS_GPD_MFPH_PD8MFP_SC2_nCD           (0x0BUL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< SmartCard2 card detect pin. */
#define SYS_GPD_MFPH_PD8MFP_USCI0_DAT0        (0x0CUL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< USCI0 data0 pin. */
#define SYS_GPD_MFPH_PD8MFP_BPWM0_CH1         (0x0DUL << SYS_GPD_MFPH_PD8MFP_Pos) /*!< BPWM0 channel1 output/capture input. */
#define SYS_GPD_MFPH_PD9MFP_GPIO              (0x00UL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD9MFP_I2C0_SCL          (0x03UL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< I2C0 clock pin. */
#define SYS_GPD_MFPH_PD9MFP_SPI2_MOSI         (0x04UL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< 1st SPI2 MOSI (Master Out, Slave In) pin. */
#define SYS_GPD_MFPH_PD9MFP_ACMP1_P3          (0x05UL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< Analog comparator1 positive input pin. */
#define SYS_GPD_MFPH_PD9MFP_EBI_ALE           (0x07UL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< EBI address latch enable output pin. */
#define SYS_GPD_MFPH_PD9MFP_QEI0_A            (0x08UL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< Quadrature encoder phase A input of QEI Unit 0. */
#define SYS_GPD_MFPH_PD9MFP_SC2_PWR           (0x0BUL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< SmartCard2 power pin. */
#define SYS_GPD_MFPH_PD9MFP_USCI0_DAT1        (0x0CUL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< USCI0 data1 pin. */
#define SYS_GPD_MFPH_PD9MFP_BPWM0_CH2         (0x0DUL << SYS_GPD_MFPH_PD9MFP_Pos) /*!< BPWM0 channel2 output/capture input. */
#define SYS_GPD_MFPH_PD10MFP_GPIO             (0x00UL << SYS_GPD_MFPH_PD10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD10MFP_SPI1_CLK         (0x01UL << SYS_GPD_MFPH_PD10MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPD_MFPH_PD10MFP_TM2              (0x04UL << SYS_GPD_MFPH_PD10MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPD_MFPH_PD10MFP_EBI_ADR9         (0x07UL << SYS_GPD_MFPH_PD10MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPD_MFPH_PD10MFP_USCI0_CTL1       (0x0CUL << SYS_GPD_MFPH_PD10MFP_Pos) /*!< USCI0 control1 pin. */
#define SYS_GPD_MFPH_PD10MFP_SD0_DAT2         (0x0EUL << SYS_GPD_MFPH_PD10MFP_Pos) /*!< SD/SDIO mode #0  data line bit 2. */
#define SYS_GPD_MFPH_PD11MFP_GPIO             (0x00UL << SYS_GPD_MFPH_PD11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD11MFP_SPI1_MOSI        (0x01UL << SYS_GPD_MFPH_PD11MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPD_MFPH_PD11MFP_TM3              (0x04UL << SYS_GPD_MFPH_PD11MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPD_MFPH_PD11MFP_EBI_ADR8         (0x07UL << SYS_GPD_MFPH_PD11MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPD_MFPH_PD11MFP_USCI0_CTL0       (0x0CUL << SYS_GPD_MFPH_PD11MFP_Pos) /*!< USCI0 control0 pin. */
#define SYS_GPD_MFPH_PD11MFP_SD0_DAT1         (0x0EUL << SYS_GPD_MFPH_PD11MFP_Pos) /*!< SD/SDIO mode #0  data line bit 1. */
#define SYS_GPD_MFPH_PD12MFP_GPIO             (0x00UL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD12MFP_SPI1_MISO        (0x01UL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPD_MFPH_PD12MFP_SPI2_SS          (0x02UL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< 1st SPI2 slave select pin. */
#define SYS_GPD_MFPH_PD12MFP_UART3_TXD        (0x03UL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPD_MFPH_PD12MFP_PWM1_CH0         (0x06UL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPD_MFPH_PD12MFP_EBI_ADR16        (0x07UL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPD_MFPH_PD12MFP_SC2_nCD          (0x0AUL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< SmartCard2 card detect pin. */
#define SYS_GPD_MFPH_PD12MFP_USCI0_CLK        (0x0CUL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< USCI0 clock pin. */
#define SYS_GPD_MFPH_PD12MFP_SD0_DAT0         (0x0EUL << SYS_GPD_MFPH_PD12MFP_Pos) /*!< SD/SDIO mode #0  data line bit 0. */
#define SYS_GPD_MFPH_PD13MFP_GPIO             (0x00UL << SYS_GPD_MFPH_PD13MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD13MFP_SPI2_MOSI        (0x02UL << SYS_GPD_MFPH_PD13MFP_Pos) /*!< 1st SPI2 MOSI (Master Out, Slave In) pin. */
#define SYS_GPD_MFPH_PD13MFP_UART3_RXD        (0x03UL << SYS_GPD_MFPH_PD13MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPD_MFPH_PD13MFP_PWM1_CH1         (0x06UL << SYS_GPD_MFPH_PD13MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPD_MFPH_PD13MFP_EBI_ADR17        (0x07UL << SYS_GPD_MFPH_PD13MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPD_MFPH_PD13MFP_SC2_RST          (0x0AUL << SYS_GPD_MFPH_PD13MFP_Pos) /*!< SmartCard2 reset pin. */
#define SYS_GPD_MFPH_PD14MFP_GPIO             (0x00UL << SYS_GPD_MFPH_PD14MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD14MFP_SPI2_MISO        (0x02UL << SYS_GPD_MFPH_PD14MFP_Pos) /*!< 1st SPI2 MISO (Master In, Slave Out) pin. */
#define SYS_GPD_MFPH_PD14MFP_UART3_nCTS       (0x03UL << SYS_GPD_MFPH_PD14MFP_Pos) /*!< Clear to Send input pin for UART3. */
#define SYS_GPD_MFPH_PD14MFP_PWM1_BRAKE0      (0x05UL << SYS_GPD_MFPH_PD14MFP_Pos) /*!< Brake input pin 0 of PWM1. */
#define SYS_GPD_MFPH_PD14MFP_PWM1_CH2         (0x06UL << SYS_GPD_MFPH_PD14MFP_Pos) /*!< PWM1 channel2 output/capture input. */
#define SYS_GPD_MFPH_PD14MFP_EBI_ADR18        (0x07UL << SYS_GPD_MFPH_PD14MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPD_MFPH_PD14MFP_SC2_DAT          (0x0AUL << SYS_GPD_MFPH_PD14MFP_Pos) /*!< SmartCard2 data pin. */
#define SYS_GPD_MFPH_PD15MFP_GPIO             (0x00UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPD_MFPH_PD15MFP_CAN1_TXD         (0x01UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< CAN1 bus transmitter output. */
#define SYS_GPD_MFPH_PD15MFP_SPI2_CLK         (0x02UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< SPI2 serial clock pin. */
#define SYS_GPD_MFPH_PD15MFP_UART3_nRTS       (0x03UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< Request to Send output pin for UART3. */
#define SYS_GPD_MFPH_PD15MFP_PWM1_CH3         (0x06UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< PWM1 channel3 output/capture input. */
#define SYS_GPD_MFPH_PD15MFP_EBI_ADR19        (0x07UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPD_MFPH_PD15MFP_UART0_RXD        (0x08UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPD_MFPH_PD15MFP_I2C0_SCL         (0x09UL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< I2C0 clock pin. */
#define SYS_GPD_MFPH_PD15MFP_SC2_CLK          (0x0AUL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< SmartCard2 clock pin. */
#define SYS_GPD_MFPH_PD15MFP_QEI1_INDEX       (0x0BUL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< Quadrature encoder index input of QEI Unit 1. */
#define SYS_GPD_MFPH_PD15MFP_QEI0_INDEX       (0x0CUL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< Quadrature encoder index input of QEI Unit 0. */
#define SYS_GPD_MFPH_PD15MFP_ECAP0_IC2        (0x0DUL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< Input 0 of enhanced capture unit 2. */
#define SYS_GPD_MFPH_PD15MFP_SD0_nCD          (0x0EUL << SYS_GPD_MFPH_PD15MFP_Pos) /*!< SD/SDIO mode #0 ??card detect */
/********************* Bit definition of GPE_MFPL register **********************/
#define SYS_GPE_MFPL_PE0MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE0MFP_SPI0_SS           (0x01UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< 1st SPI0 slave select pin. */
#define SYS_GPE_MFPL_PE0MFP_SPI2_CLK          (0x02UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< SPI2 serial clock pin. */
#define SYS_GPE_MFPL_PE0MFP_I2C1_SDA          (0x03UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< I2C1 data input/output pin. */
#define SYS_GPE_MFPL_PE0MFP_BPWM0_CH0         (0x04UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< BPWM0 channel0 output/capture input. */
#define SYS_GPE_MFPL_PE0MFP_SC0_nCD           (0x05UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< SmartCard0 card detect pin. */
#define SYS_GPE_MFPL_PE0MFP_PWM0_CH0          (0x06UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< PWM0 channel0 output/capture input. */
#define SYS_GPE_MFPL_PE0MFP_EBI_nCS1          (0x07UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPE_MFPL_PE0MFP_INT4              (0x08UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< External interrupt4 input pin. */
#define SYS_GPE_MFPL_PE0MFP_UART4_RXD         (0x09UL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< Data receiver input pin for UART4. */
#define SYS_GPE_MFPL_PE0MFP_I2S0_BCLK         (0x0AUL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< I2S0 bit clock pin. */
#define SYS_GPE_MFPL_PE0MFP_PWM0_CH5          (0x0BUL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< PWM0 channel5 output/capture input. */
#define SYS_GPE_MFPL_PE0MFP_SPI0_CLK          (0x0CUL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< SPI0 serial clock pin. */
#define SYS_GPE_MFPL_PE0MFP_TM2_EXT           (0x0DUL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPE_MFPL_PE0MFP_ECAP0_IC2         (0x0EUL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< Input 0 of enhanced capture unit 2. */
#define SYS_GPE_MFPL_PE0MFP_SPIM_D3           (0x0FUL << SYS_GPE_MFPL_PE0MFP_Pos) /*!< SPIM data 3 pin for Quad Mode I/O. */
#define SYS_GPE_MFPL_PE1MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE1MFP_UART1_RXD         (0x01UL << SYS_GPE_MFPL_PE1MFP_Pos) /*!< Data receiver input pin for UART1. */
#define SYS_GPE_MFPL_PE1MFP_TM3_EXT           (0x03UL << SYS_GPE_MFPL_PE1MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPE_MFPL_PE1MFP_SC0_nCD           (0x05UL << SYS_GPE_MFPL_PE1MFP_Pos) /*!< SmartCard0 card detect pin. */
#define SYS_GPE_MFPL_PE1MFP_PWM0_CH1          (0x06UL << SYS_GPE_MFPL_PE1MFP_Pos) /*!< PWM0 channel1 output/capture input. */
#define SYS_GPE_MFPL_PE1MFP_EBI_ADR7          (0x07UL << SYS_GPE_MFPL_PE1MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPL_PE1MFP_SD1_nCD           (0x0BUL << SYS_GPE_MFPL_PE1MFP_Pos) /*!< SD/SDIO mode #1 ??card detect */
#define SYS_GPE_MFPL_PE2MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE2MFP_PWM1_CH1          (0x06UL << SYS_GPE_MFPL_PE2MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPE_MFPL_PE2MFP_EBI_ADR9          (0x07UL << SYS_GPE_MFPL_PE2MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPL_PE2MFP_SPI3_MISO         (0x09UL << SYS_GPE_MFPL_PE2MFP_Pos) /*!< 1st SPI3 MISO (Master In, Slave Out) pin. */
#define SYS_GPE_MFPL_PE2MFP_HSUSB_VBUS_EN     (0x0BUL << SYS_GPE_MFPL_PE2MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPE_MFPL_PE3MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE3MFP_SPI1_MOSI         (0x02UL << SYS_GPE_MFPL_PE3MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPE_MFPL_PE3MFP_I2C1_SCL          (0x04UL << SYS_GPE_MFPL_PE3MFP_Pos) /*!< I2C1 clock pin. */
#define SYS_GPE_MFPL_PE3MFP_PWM0_CH3          (0x06UL << SYS_GPE_MFPL_PE3MFP_Pos) /*!< PWM0 channel3 output/capture input. */
#define SYS_GPE_MFPL_PE3MFP_EBI_AD13          (0x07UL << SYS_GPE_MFPL_PE3MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPE_MFPL_PE3MFP_OPA2_N            (0x09UL << SYS_GPE_MFPL_PE3MFP_Pos) /*!< Operational amplifier negative input pin. */
#define SYS_GPE_MFPL_PE3MFP_SD0_nCD           (0x0EUL << SYS_GPE_MFPL_PE3MFP_Pos) /*!< SD/SDIO mode #0 ??card detect */
#define SYS_GPE_MFPL_PE4MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE4MFP_I2C1_SCL          (0x03UL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< I2C1 clock pin. */
#define SYS_GPE_MFPL_PE4MFP_SC0_PWR           (0x04UL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< SmartCard0 power pin. */
#define SYS_GPE_MFPL_PE4MFP_SD1_nCD           (0x05UL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< SD/SDIO mode #1 ??card detect */
#define SYS_GPE_MFPL_PE4MFP_PWM1_BRAKE0       (0x06UL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< Brake input pin 0 of PWM1. */
#define SYS_GPE_MFPL_PE4MFP_EBI_nCS0          (0x07UL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPE_MFPL_PE4MFP_INT0              (0x08UL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< External interrupt0 input pin. */
#define SYS_GPE_MFPL_PE4MFP_SPIM_MOSI         (0x0FUL << SYS_GPE_MFPL_PE4MFP_Pos) /*!< 1st SPI* MOSI (Master Out, Slave In) pin. */
#define SYS_GPE_MFPL_PE5MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE5MFP_I2C1_SDA          (0x03UL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< I2C1 data input/output pin. */
#define SYS_GPE_MFPL_PE5MFP_SC0_RST           (0x04UL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< SmartCard0 reset pin. */
#define SYS_GPE_MFPL_PE5MFP_SD1_CMD           (0x05UL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< SD/SDIO mode #1 ??command/response. */
#define SYS_GPE_MFPL_PE5MFP_PWM1_BRAKE1       (0x06UL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< Brake input pin 1 of PWM1. */
#define SYS_GPE_MFPL_PE5MFP_EBI_ALE           (0x07UL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< EBI address latch enable output pin. */
#define SYS_GPE_MFPL_PE5MFP_INT1              (0x08UL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< External interrupt1 input pin. */
#define SYS_GPE_MFPL_PE5MFP_SPIM_MISO         (0x0FUL << SYS_GPE_MFPL_PE5MFP_Pos) /*!< 1st SPI* MISO (Master In, Slave Out) pin. */
#define SYS_GPE_MFPL_PE6MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE6MFP_SPIM_D2           (0x02UL << SYS_GPE_MFPL_PE6MFP_Pos) /*!< SPIM data 2 pin for Quad Mode I/O. */
#define SYS_GPE_MFPL_PE6MFP_TM3_EXT           (0x03UL << SYS_GPE_MFPL_PE6MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPE_MFPL_PE6MFP_SD1_DAT0          (0x0BUL << SYS_GPE_MFPL_PE6MFP_Pos) /*!< SD/SDIO mode #1  data line bit 0. */
#define SYS_GPE_MFPL_PE7MFP_GPIO              (0x00UL << SYS_GPE_MFPL_PE7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPL_PE7MFP_EBI_MCLK          (0x07UL << SYS_GPE_MFPL_PE7MFP_Pos) /*!< EBI external clock output pin. */
#define SYS_GPE_MFPL_PE7MFP_PWM1_BRAKE1       (0x0DUL << SYS_GPE_MFPL_PE7MFP_Pos) /*!< Brake input pin 1 of PWM1. */
/********************* Bit definition of GPE_MFPH register **********************/
#define SYS_GPE_MFPH_PE8MFP_GPIO              (0x00UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPH_PE8MFP_UART1_TXD         (0x01UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< Data transmitter output pin for UART1. */
#define SYS_GPE_MFPH_PE8MFP_SPI0_MISO1        (0x02UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< 2nd SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPE_MFPH_PE8MFP_I2C1_SCL          (0x04UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< I2C1 clock pin. */
#define SYS_GPE_MFPH_PE8MFP_SC0_PWR           (0x05UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< SmartCard0 power pin. */
#define SYS_GPE_MFPH_PE8MFP_UART4_nRTS        (0x06UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< Request to Send output pin for UART4. */
#define SYS_GPE_MFPH_PE8MFP_EBI_ADR6          (0x07UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPH_PE8MFP_CLKO              (0x09UL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< Clock Output pin. */
#define SYS_GPE_MFPH_PE8MFP_PWM0_BRAKE0       (0x0AUL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< Brake input pin 0 of PWM0. */
#define SYS_GPE_MFPH_PE8MFP_TM1               (0x0BUL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPE_MFPH_PE8MFP_BPWM1_CH0         (0x0DUL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< BPWM1 channel0 output/capture input. */
#define SYS_GPE_MFPH_PE8MFP_SPIM_D2           (0x0FUL << SYS_GPE_MFPH_PE8MFP_Pos) /*!< SPIM data 2 pin for Quad Mode I/O. */
#define SYS_GPE_MFPH_PE9MFP_GPIO              (0x00UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPH_PE9MFP_UART1_RXD         (0x01UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< Data receiver input pin for UART1. */
#define SYS_GPE_MFPH_PE9MFP_SPI0_MOSI1        (0x02UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< 2nd SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPE_MFPH_PE9MFP_I2C1_SDA          (0x04UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< I2C1 data input/output pin. */
#define SYS_GPE_MFPH_PE9MFP_SC0_RST           (0x05UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< SmartCard0 reset pin. */
#define SYS_GPE_MFPH_PE9MFP_UART4_nCTS        (0x06UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< Clear to Send input pin for UART4. */
#define SYS_GPE_MFPH_PE9MFP_EBI_ADR5          (0x07UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPH_PE9MFP_I2S0_BCLK         (0x08UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< I2S0 bit clock pin. */
#define SYS_GPE_MFPH_PE9MFP_SPI1_I2SMCLK      (0x09UL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< SPI1 I2S master clock output pin. */
#define SYS_GPE_MFPH_PE9MFP_PWM1_BRAKE1       (0x0AUL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< Brake input pin 1 of PWM1. */
#define SYS_GPE_MFPH_PE9MFP_TM2               (0x0BUL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPE_MFPH_PE9MFP_USCI1_CTL1        (0x0CUL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< USCI1 control1 pin. */
#define SYS_GPE_MFPH_PE9MFP_BPWM1_CH1         (0x0DUL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< BPWM1 channel1 output/capture input. */
#define SYS_GPE_MFPH_PE9MFP_SPIM_D3           (0x0FUL << SYS_GPE_MFPH_PE9MFP_Pos) /*!< SPIM data 3 pin for Quad Mode I/O. */
#define SYS_GPE_MFPH_PE10MFP_GPIO             (0x00UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPH_PE10MFP_SPI1_MISO        (0x01UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPE_MFPH_PE10MFP_SPI0_MISO0       (0x02UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< 1st SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPE_MFPH_PE10MFP_UART1_nCTS       (0x03UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< Clear to Send input pin for UART1. */
#define SYS_GPE_MFPH_PE10MFP_I2C0_SMBAL       (0x04UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< I2C0 SMBus SMBALTER# pin  */
#define SYS_GPE_MFPH_PE10MFP_SC0_DAT          (0x05UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< SmartCard0 data pin. */
#define SYS_GPE_MFPH_PE10MFP_PWM1_CH0         (0x06UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPE_MFPH_PE10MFP_EBI_ADR4         (0x07UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPH_PE10MFP_I2S0_MCLK        (0x08UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< I2S0 master clock output pin. */
#define SYS_GPE_MFPH_PE10MFP_UART3_TXD        (0x09UL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPE_MFPH_PE10MFP_I2C1_SCL         (0x0BUL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< I2C1 clock pin. */
#define SYS_GPE_MFPH_PE10MFP_USCI1_CTL0       (0x0CUL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< USCI1 control0 pin. */
#define SYS_GPE_MFPH_PE10MFP_BPWM1_CH2        (0x0DUL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< BPWM1 channel2 output/capture input. */
#define SYS_GPE_MFPH_PE10MFP_SPIM_MISO        (0x0FUL << SYS_GPE_MFPH_PE10MFP_Pos) /*!< 1st SPI* MISO (Master In, Slave Out) pin. */
#define SYS_GPE_MFPH_PE11MFP_GPIO             (0x00UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPH_PE11MFP_SPI1_MOSI        (0x01UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPE_MFPH_PE11MFP_SPI0_MOSI0       (0x02UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< 1st SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPE_MFPH_PE11MFP_UART1_nRTS       (0x03UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< Request to Send output pin for UART1. */
#define SYS_GPE_MFPH_PE11MFP_I2C0_SMBSUS      (0x04UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< I2C0 SMBus SMBSUS# pin (PMBus CONTROL pin) */
#define SYS_GPE_MFPH_PE11MFP_SC0_CLK          (0x05UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< SmartCard0 clock pin. */
#define SYS_GPE_MFPH_PE11MFP_PWM1_CH1         (0x06UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPE_MFPH_PE11MFP_EBI_ADR3         (0x07UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPH_PE11MFP_I2S0_DI          (0x08UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< I2S0 data input. */
#define SYS_GPE_MFPH_PE11MFP_UART3_RXD        (0x09UL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPE_MFPH_PE11MFP_I2C1_SDA         (0x0BUL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< I2C1 data input/output pin. */
#define SYS_GPE_MFPH_PE11MFP_USCI1_DAT0       (0x0CUL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< USCI1 data0 pin. */
#define SYS_GPE_MFPH_PE11MFP_BPWM1_CH3        (0x0DUL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< BPWM1 channel3 output/capture input. */
#define SYS_GPE_MFPH_PE11MFP_SPIM_MOSI        (0x0FUL << SYS_GPE_MFPH_PE11MFP_Pos) /*!< 1st SPI* MOSI (Master Out, Slave In) pin. */
#define SYS_GPE_MFPH_PE12MFP_GPIO             (0x00UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPH_PE12MFP_SPI1_SS          (0x01UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPE_MFPH_PE12MFP_SPI0_SS          (0x02UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< 1st SPI0 slave select pin. */
#define SYS_GPE_MFPH_PE12MFP_UART1_TXD        (0x03UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< Data transmitter output pin for UART1. */
#define SYS_GPE_MFPH_PE12MFP_I2C0_SCL         (0x04UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< I2C0 clock pin. */
#define SYS_GPE_MFPH_PE12MFP_SC0_PWR          (0x05UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< SmartCard0 power pin. */
#define SYS_GPE_MFPH_PE12MFP_UART4_TXD        (0x06UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< Data transmitter output pin for UART4. */
#define SYS_GPE_MFPH_PE12MFP_EBI_ADR2         (0x07UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPH_PE12MFP_I2S0_DO          (0x08UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< I2S0 data output. */
#define SYS_GPE_MFPH_PE12MFP_I2C1_SMBAL       (0x09UL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< I2C1 SMBus SMBALTER# pin  */
#define SYS_GPE_MFPH_PE12MFP_PWM0_CH4         (0x0AUL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< PWM0 channel4 output/capture input. */
#define SYS_GPE_MFPH_PE12MFP_SD0_CMD          (0x0BUL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< SD/SDIO mode #0 ??command/response. */
#define SYS_GPE_MFPH_PE12MFP_USCI1_DAT1       (0x0CUL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< USCI1 data1 pin. */
#define SYS_GPE_MFPH_PE12MFP_BPWM1_CH4        (0x0DUL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< BPWM1 channel4 output/capture input. */
#define SYS_GPE_MFPH_PE12MFP_SPIM_SS          (0x0FUL << SYS_GPE_MFPH_PE12MFP_Pos) /*!< 1st SPI* slave select pin. */
#define SYS_GPE_MFPH_PE13MFP_GPIO             (0x00UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPE_MFPH_PE13MFP_SPI1_CLK         (0x01UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPE_MFPH_PE13MFP_SPI0_CLK         (0x02UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< SPI0 serial clock pin. */
#define SYS_GPE_MFPH_PE13MFP_UART1_RXD        (0x03UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< Data receiver input pin for UART1. */
#define SYS_GPE_MFPH_PE13MFP_I2C0_SDA         (0x04UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< I2C0 data input/output pin. */
#define SYS_GPE_MFPH_PE13MFP_SC0_RST          (0x05UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< SmartCard0 reset pin. */
#define SYS_GPE_MFPH_PE13MFP_UART4_RXD        (0x06UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< Data receiver input pin for UART4. */
#define SYS_GPE_MFPH_PE13MFP_EBI_ADR1         (0x07UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPE_MFPH_PE13MFP_I2S0_LRCK        (0x08UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< I2S0 left right channel clock. */
#define SYS_GPE_MFPH_PE13MFP_I2C1_SMBSUS      (0x09UL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< I2C1 SMBus SMBSUS# pin (PMBus CONTROL pin) */
#define SYS_GPE_MFPH_PE13MFP_PWM0_CH5         (0x0AUL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< PWM0 channel5 output/capture input. */
#define SYS_GPE_MFPH_PE13MFP_SD0_nCD          (0x0BUL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< SD/SDIO mode #0 ??card detect */
#define SYS_GPE_MFPH_PE13MFP_USCI1_CLK        (0x0CUL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< USCI1 clock pin. */
#define SYS_GPE_MFPH_PE13MFP_BPWM1_CH5        (0x0DUL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< BPWM1 channel5 output/capture input. */
#define SYS_GPE_MFPH_PE13MFP_SPIM_CLK         (0x0FUL << SYS_GPE_MFPH_PE13MFP_Pos) /*!< SPI* serial clock pin. */
/********************* Bit definition of GPF_MFPL register **********************/
#define SYS_GPF_MFPL_PF0MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF0MFP_X32_OUT           (0x01UL << SYS_GPF_MFPL_PF0MFP_Pos) /*!< External 32.768 kHz (low speed) crystal output pin. */
#define SYS_GPF_MFPL_PF0MFP_SPI1_SS           (0x02UL << SYS_GPF_MFPL_PF0MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPF_MFPL_PF0MFP_TM3_EXT           (0x04UL << SYS_GPF_MFPL_PF0MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPF_MFPL_PF0MFP_INT5              (0x08UL << SYS_GPF_MFPL_PF0MFP_Pos) /*!< External interrupt5 input pin. */
#define SYS_GPF_MFPL_PF1MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF1MFP_X32_IN            (0x01UL << SYS_GPF_MFPL_PF1MFP_Pos) /*!< External 32.768 kHz (low speed) crystal input pin. */
#define SYS_GPF_MFPL_PF1MFP_SPI1_MOSI         (0x02UL << SYS_GPF_MFPL_PF1MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPF_MFPL_PF1MFP_TM3               (0x04UL << SYS_GPF_MFPL_PF1MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPF_MFPL_PF2MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF2MFP_TAMPER0           (0x01UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< TAMPER detector loop pin0. */
#define SYS_GPF_MFPL_PF2MFP_DAC0_ST           (0x02UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< DAC0 external trigger input. */
#define SYS_GPF_MFPL_PF2MFP_DAC1_ST           (0x03UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< DAC1 external trigger input. */
#define SYS_GPF_MFPL_PF2MFP_TM2_EXT           (0x04UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPF_MFPL_PF2MFP_SPI1_CLK          (0x05UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPF_MFPL_PF2MFP_PWM0_CH5          (0x06UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< PWM0 channel5 output/capture input. */
#define SYS_GPF_MFPL_PF2MFP_EBI_nCS2          (0x07UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPF_MFPL_PF2MFP_INT5              (0x08UL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< External interrupt5 input pin. */
#define SYS_GPF_MFPL_PF2MFP_SC0_nCD           (0x0AUL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< SmartCard0 card detect pin. */
#define SYS_GPF_MFPL_PF2MFP_SC1_nCD           (0x0BUL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< SmartCard1 card detect pin. */
#define SYS_GPF_MFPL_PF2MFP_QEI0_A            (0x0CUL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< Quadrature encoder phase A input of QEI Unit 0. */
#define SYS_GPF_MFPL_PF2MFP_ECAP1_IC2         (0x0DUL << SYS_GPF_MFPL_PF2MFP_Pos) /*!< Input 1 of enhanced capture unit 2. */
#define SYS_GPF_MFPL_PF3MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF3MFP_XT1_OUT           (0x01UL << SYS_GPF_MFPL_PF3MFP_Pos) /*!< External 4~24 MHz (high speed) crystal output pin. */
#define SYS_GPF_MFPL_PF3MFP_I2C1_SCL          (0x03UL << SYS_GPF_MFPL_PF3MFP_Pos) /*!< I2C1 clock pin. */
#define SYS_GPF_MFPL_PF3MFP_PWM1_CH4          (0x06UL << SYS_GPF_MFPL_PF3MFP_Pos) /*!< PWM1 channel4 output/capture input. */
#define SYS_GPF_MFPL_PF4MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF4MFP_XT1_IN            (0x01UL << SYS_GPF_MFPL_PF4MFP_Pos) /*!< External 4~24 MHz (high speed) crystal input pin. */
#define SYS_GPF_MFPL_PF4MFP_I2C1_SDA          (0x03UL << SYS_GPF_MFPL_PF4MFP_Pos) /*!< I2C1 data input/output pin. */
#define SYS_GPF_MFPL_PF4MFP_PWM1_CH5          (0x06UL << SYS_GPF_MFPL_PF4MFP_Pos) /*!< PWM1 channel5 output/capture input. */
#define SYS_GPF_MFPL_PF5MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF5MFP_ICE_CLK           (0x01UL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< Serial wired debugger clock pin. */
#define SYS_GPF_MFPL_PF5MFP_UART3_TXD         (0x02UL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPF_MFPL_PF5MFP_UART0_RXD         (0x03UL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPF_MFPL_PF5MFP_I2C0_SCL          (0x04UL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< I2C0 clock pin. */
#define SYS_GPF_MFPL_PF5MFP_SC0_DAT           (0x05UL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< SmartCard0 data pin. */
#define SYS_GPF_MFPL_PF5MFP_SD0_nCD           (0x0BUL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< SD/SDIO mode #0 ??card detect */
#define SYS_GPF_MFPL_PF5MFP_BPWM1_CH0         (0x0DUL << SYS_GPF_MFPL_PF5MFP_Pos) /*!< BPWM1 channel0 output/capture input. */
#define SYS_GPF_MFPL_PF6MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF6MFP_ICE_DAT           (0x01UL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< Serial wired debugger data pin. */
#define SYS_GPF_MFPL_PF6MFP_UART3_RXD         (0x02UL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPF_MFPL_PF6MFP_UART0_TXD         (0x03UL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< Data transmitter output pin for UART0. */
#define SYS_GPF_MFPL_PF6MFP_I2C0_SDA          (0x04UL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< I2C0 data input/output pin. */
#define SYS_GPF_MFPL_PF6MFP_SC0_CLK           (0x05UL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< SmartCard0 clock pin. */
#define SYS_GPF_MFPL_PF6MFP_SD0_CMD           (0x0BUL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< SD/SDIO mode #0 ??command/response. */
#define SYS_GPF_MFPL_PF6MFP_BPWM1_CH1         (0x0DUL << SYS_GPF_MFPL_PF6MFP_Pos) /*!< BPWM1 channel1 output/capture input. */
#define SYS_GPF_MFPL_PF7MFP_GPIO              (0x00UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPL_PF7MFP_TAMPER1           (0x01UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< TAMPER detector loop pin1. */
#define SYS_GPF_MFPL_PF7MFP_SPI2_I2SMCLK      (0x02UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< SPI2 I2S master clock output pin. */
#define SYS_GPF_MFPL_PF7MFP_UART3_TXD         (0x03UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPF_MFPL_PF7MFP_TM1_EXT           (0x04UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPF_MFPL_PF7MFP_SPI1_MISO         (0x05UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPF_MFPL_PF7MFP_PWM1_CH0          (0x06UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPF_MFPL_PF7MFP_EBI_ADR16         (0x07UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPF_MFPL_PF7MFP_INT4              (0x08UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< External interrupt4 input pin. */
#define SYS_GPF_MFPL_PF7MFP_EBI_ALE           (0x09UL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< EBI address latch enable output pin. */
#define SYS_GPF_MFPL_PF7MFP_SC2_nCD           (0x0AUL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< SmartCard2 card detect pin. */
#define SYS_GPF_MFPL_PF7MFP_USCI0_CLK         (0x0BUL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< USCI0 clock pin. */
#define SYS_GPF_MFPL_PF7MFP_QEI0_B            (0x0CUL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< Quadrature encoder phase B input of QEI Unit 0. */
#define SYS_GPF_MFPL_PF7MFP_ECAP1_IC1         (0x0DUL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< Input 1 of enhanced capture unit 1. */
#define SYS_GPF_MFPL_PF7MFP_SD0_DAT0          (0x0EUL << SYS_GPF_MFPL_PF7MFP_Pos) /*!< SD/SDIO mode #0  data line bit 0. */
/********************* Bit definition of GPF_MFPH register **********************/
#define SYS_GPF_MFPH_PF8MFP_GPIO              (0x00UL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF8MFP_TAMPER2           (0x01UL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< TAMPER detector loop pin2. */
#define SYS_GPF_MFPH_PF8MFP_SPI2_CLK          (0x02UL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< SPI2 serial clock pin. */
#define SYS_GPF_MFPH_PF8MFP_TM0_EXT           (0x04UL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPF_MFPH_PF8MFP_PWM1_BRAKE1       (0x05UL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< Brake input pin 1 of PWM1. */
#define SYS_GPF_MFPH_PF8MFP_INT3              (0x08UL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< External interrupt3 input pin. */
#define SYS_GPF_MFPH_PF8MFP_QEI0_INDEX        (0x0CUL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< Quadrature encoder index input of QEI Unit 0. */
#define SYS_GPF_MFPH_PF8MFP_ECAP1_IC0         (0x0DUL << SYS_GPF_MFPH_PF8MFP_Pos) /*!< Input 0 of enhanced capture unit 1. */
#define SYS_GPF_MFPH_PF9MFP_GPIO              (0x00UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF9MFP_TAMPER3           (0x01UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< TAMPER detector loop pin3. */
#define SYS_GPF_MFPH_PF9MFP_SPI2_SS           (0x02UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< 1st SPI2 slave select pin. */
#define SYS_GPF_MFPH_PF9MFP_UART3_TXD         (0x03UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPF_MFPH_PF9MFP_TM2               (0x04UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPF_MFPH_PF9MFP_PWM1_BRAKE0       (0x05UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< Brake input pin 0 of PWM1. */
#define SYS_GPF_MFPH_PF9MFP_PWM1_CH0          (0x06UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPF_MFPH_PF9MFP_EBI_ADR16         (0x07UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPF_MFPH_PF9MFP_INT2              (0x08UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< External interrupt2 input pin. */
#define SYS_GPF_MFPH_PF9MFP_EADC0_ST          (0x09UL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< EADC external trigger input. */
#define SYS_GPF_MFPH_PF9MFP_SC1_RST           (0x0BUL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< SmartCard1 reset pin. */
#define SYS_GPF_MFPH_PF9MFP_QEI1_INDEX        (0x0CUL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< Quadrature encoder index input of QEI Unit 1. */
#define SYS_GPF_MFPH_PF9MFP_ECAP0_IC2         (0x0DUL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< Input 0 of enhanced capture unit 2. */
#define SYS_GPF_MFPH_PF9MFP_SD0_DAT0          (0x0EUL << SYS_GPF_MFPH_PF9MFP_Pos) /*!< SD/SDIO mode #0  data line bit 0. */
#define SYS_GPF_MFPH_PF10MFP_GPIO             (0x00UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF10MFP_TAMPER4          (0x01UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< TAMPER detector loop pin4. */
#define SYS_GPF_MFPH_PF10MFP_SPI2_MOSI        (0x02UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< 1st SPI2 MOSI (Master Out, Slave In) pin. */
#define SYS_GPF_MFPH_PF10MFP_UART3_RXD        (0x03UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPF_MFPH_PF10MFP_TM1              (0x04UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPF_MFPH_PF10MFP_PWM0_BRAKE0      (0x05UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< Brake input pin 0 of PWM0. */
#define SYS_GPF_MFPH_PF10MFP_PWM1_CH1         (0x06UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPF_MFPH_PF10MFP_EBI_ADR17        (0x07UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPF_MFPH_PF10MFP_INT1             (0x08UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< External interrupt1 input pin. */
#define SYS_GPF_MFPH_PF10MFP_EBI_nRD          (0x09UL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< EBI read enable output pin. */
#define SYS_GPF_MFPH_PF10MFP_I2C2_SCL         (0x0AUL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< I2C2 clock pin. */
#define SYS_GPF_MFPH_PF10MFP_SC1_PWR          (0x0BUL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< SmartCard1 power pin. */
#define SYS_GPF_MFPH_PF10MFP_QEI1_A           (0x0CUL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< Quadrature encoder phase A input of QEI Unit 1. */
#define SYS_GPF_MFPH_PF10MFP_ECAP0_IC1        (0x0DUL << SYS_GPF_MFPH_PF10MFP_Pos) /*!< Input 1 of enhanced capture unit 0. */
#define SYS_GPF_MFPH_PF11MFP_GPIO             (0x00UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF11MFP_TAMPER5          (0x01UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< TAMPER detector loop pin5. */
#define SYS_GPF_MFPH_PF11MFP_SPI2_MISO        (0x02UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< 1st SPI2 MISO (Master In, Slave Out) pin. */
#define SYS_GPF_MFPH_PF11MFP_UART3_nCTS       (0x03UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< Clear to Send input pin for UART3. */
#define SYS_GPF_MFPH_PF11MFP_TM0              (0x04UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPF_MFPH_PF11MFP_PWM0_BRAKE1      (0x05UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< Brake input pin 1 of PWM0. */
#define SYS_GPF_MFPH_PF11MFP_PWM1_CH2         (0x06UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< PWM1 channel2 output/capture input. */
#define SYS_GPF_MFPH_PF11MFP_EBI_ADR18        (0x07UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< EBI address/data bus bit*. */
#define SYS_GPF_MFPH_PF11MFP_INT0             (0x08UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< External interrupt0 input pin. */
#define SYS_GPF_MFPH_PF11MFP_EBI_nWR          (0x09UL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< EBI wirte enable output pin. */
#define SYS_GPF_MFPH_PF11MFP_I2C2_SDA         (0x0AUL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< I2C2 data input/output pin. */
#define SYS_GPF_MFPH_PF11MFP_SC1_nCD          (0x0BUL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< SmartCard1 card detect pin. */
#define SYS_GPF_MFPH_PF11MFP_QEI1_B           (0x0CUL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< Quadrature encoder phase B input of QEI Unit 1. */
#define SYS_GPF_MFPH_PF11MFP_ECAP0_IC0        (0x0DUL << SYS_GPF_MFPH_PF11MFP_Pos) /*!< Input 0 of enhanced capture unit 0. */
#define SYS_GPF_MFPH_PF12MFP_GPIO             (0x00UL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF12MFP_USB_OTG_ID       (0x01UL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< USB Full speed identification. */
#define SYS_GPF_MFPH_PF12MFP_SPI1_MISO        (0x02UL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< 1st SPI1 MISO (Master In, Slave Out) pin. */
#define SYS_GPF_MFPH_PF12MFP_UART5_RXD        (0x03UL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< Data receiver input pin for UART5. */
#define SYS_GPF_MFPH_PF12MFP_I2C0_SCL         (0x04UL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< I2C0 clock pin. */
#define SYS_GPF_MFPH_PF12MFP_CAN0_TXD         (0x05UL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< CAN0 bus transmitter output. */
#define SYS_GPF_MFPH_PF12MFP_USCI1_CTL0       (0x0CUL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< USCI1 control0 pin. */
#define SYS_GPF_MFPH_PF12MFP_PWM1_BRAKE0      (0x0DUL << SYS_GPF_MFPH_PF12MFP_Pos) /*!< Brake input pin 0 of PWM1. */
#define SYS_GPF_MFPH_PF13MFP_GPIO             (0x00UL << SYS_GPF_MFPH_PF13MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF13MFP_USB_D_P          (0x01UL << SYS_GPF_MFPH_PF13MFP_Pos) /*!< USB Full speed differential signal D+. */
#define SYS_GPF_MFPH_PF13MFP_SPI1_MOSI        (0x02UL << SYS_GPF_MFPH_PF13MFP_Pos) /*!< 1st SPI1 MOSI (Master Out, Slave In) pin. */
#define SYS_GPF_MFPH_PF13MFP_UART5_TXD        (0x03UL << SYS_GPF_MFPH_PF13MFP_Pos) /*!< Data transmitter output pin for UART5. */
#define SYS_GPF_MFPH_PF13MFP_I2C0_SDA         (0x04UL << SYS_GPF_MFPH_PF13MFP_Pos) /*!< I2C0 data input/output pin. */
#define SYS_GPF_MFPH_PF13MFP_CAN0_RXD         (0x05UL << SYS_GPF_MFPH_PF13MFP_Pos) /*!< CAN0 bus receiver input. */
#define SYS_GPF_MFPH_PF13MFP_USCI1_DAT0       (0x0CUL << SYS_GPF_MFPH_PF13MFP_Pos) /*!< USCI1 data0 pin. */
#define SYS_GPF_MFPH_PF14MFP_GPIO             (0x00UL << SYS_GPF_MFPH_PF14MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF14MFP_USB_D_N          (0x01UL << SYS_GPF_MFPH_PF14MFP_Pos) /*!< USB Full speed differential signal D-. */
#define SYS_GPF_MFPH_PF14MFP_SPI1_SS          (0x02UL << SYS_GPF_MFPH_PF14MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPF_MFPH_PF14MFP_UART5_nCTS       (0x03UL << SYS_GPF_MFPH_PF14MFP_Pos) /*!< Clear to Send input pin for UART5. */
#define SYS_GPF_MFPH_PF14MFP_I2C1_SCL         (0x04UL << SYS_GPF_MFPH_PF14MFP_Pos) /*!< I2C1 clock pin. */
#define SYS_GPF_MFPH_PF14MFP_CAN1_TXD         (0x05UL << SYS_GPF_MFPH_PF14MFP_Pos) /*!< CAN1 bus transmitter output. */
#define SYS_GPF_MFPH_PF14MFP_USCI1_DAT1       (0x0CUL << SYS_GPF_MFPH_PF14MFP_Pos) /*!< USCI1 data1 pin. */
#define SYS_GPF_MFPH_PF15MFP_GPIO             (0x00UL << SYS_GPF_MFPH_PF15MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPF_MFPH_PF15MFP_USB_VBUS         (0x01UL << SYS_GPF_MFPH_PF15MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPF_MFPH_PF15MFP_SPI1_CLK         (0x02UL << SYS_GPF_MFPH_PF15MFP_Pos) /*!< SPI1 serial clock pin. */
#define SYS_GPF_MFPH_PF15MFP_UART5_nRTS       (0x03UL << SYS_GPF_MFPH_PF15MFP_Pos) /*!< Request to Send output pin for UART5. */
#define SYS_GPF_MFPH_PF15MFP_I2C1_SDA         (0x04UL << SYS_GPF_MFPH_PF15MFP_Pos) /*!< I2C1 data input/output pin. */
#define SYS_GPF_MFPH_PF15MFP_CAN1_RXD         (0x05UL << SYS_GPF_MFPH_PF15MFP_Pos) /*!< CAN1 bus receiver input. */
#define SYS_GPF_MFPH_PF15MFP_USCI1_CLK        (0x0CUL << SYS_GPF_MFPH_PF15MFP_Pos) /*!< USCI1 clock pin. */
/********************* Bit definition of GPG_MFPL register **********************/
#define SYS_GPG_MFPL_PG0MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG0MFP_SPI0_MOSI1        (0x02UL << SYS_GPG_MFPL_PG0MFP_Pos) /*!< 2nd SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPG_MFPL_PG0MFP_EBI_ALE           (0x07UL << SYS_GPG_MFPL_PG0MFP_Pos) /*!< EBI address latch enable output pin. */
#define SYS_GPG_MFPL_PG0MFP_INT6              (0x08UL << SYS_GPG_MFPL_PG0MFP_Pos) /*!< External interrupt6 input pin. */
#define SYS_GPG_MFPL_PG0MFP_TM2_EXT           (0x0BUL << SYS_GPG_MFPL_PG0MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPG_MFPL_PG1MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG1MFP_SPI0_MISO0        (0x02UL << SYS_GPG_MFPL_PG1MFP_Pos) /*!< 1st SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPG_MFPL_PG1MFP_UART2_RXD         (0x03UL << SYS_GPG_MFPL_PG1MFP_Pos) /*!< Data receiver input pin for UART2. */
#define SYS_GPG_MFPL_PG1MFP_PWM1_CH2          (0x06UL << SYS_GPG_MFPL_PG1MFP_Pos) /*!< PWM1 channel2 output/capture input. */
#define SYS_GPG_MFPL_PG1MFP_EBI_AD7           (0x07UL << SYS_GPG_MFPL_PG1MFP_Pos) /*!< EBI address/data bus bit7. */
#define SYS_GPG_MFPL_PG1MFP_TM2               (0x0BUL << SYS_GPG_MFPL_PG1MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPG_MFPL_PG2MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG2MFP_SPI0_MOSI0        (0x02UL << SYS_GPG_MFPL_PG2MFP_Pos) /*!< 1st SPI0 MOSI (Master Out, Slave In) pin. */
#define SYS_GPG_MFPL_PG2MFP_UART2_TXD         (0x03UL << SYS_GPG_MFPL_PG2MFP_Pos) /*!< Data transmitter output pin for UART2. */
#define SYS_GPG_MFPL_PG2MFP_PWM1_CH3          (0x06UL << SYS_GPG_MFPL_PG2MFP_Pos) /*!< PWM1 channel3 output/capture input. */
#define SYS_GPG_MFPL_PG2MFP_EBI_AD6           (0x07UL << SYS_GPG_MFPL_PG2MFP_Pos) /*!< EBI address/data bus bit6. */
#define SYS_GPG_MFPL_PG2MFP_INT3              (0x08UL << SYS_GPG_MFPL_PG2MFP_Pos) /*!< External interrupt3 input pin. */
#define SYS_GPG_MFPL_PG2MFP_TM3_EXT           (0x0BUL << SYS_GPG_MFPL_PG2MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPG_MFPL_PG3MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG3MFP_SPI0_CLK          (0x02UL << SYS_GPG_MFPL_PG3MFP_Pos) /*!< SPI0 serial clock pin. */
#define SYS_GPG_MFPL_PG3MFP_UART2_nCTS        (0x03UL << SYS_GPG_MFPL_PG3MFP_Pos) /*!< Clear to Send input pin for UART2. */
#define SYS_GPG_MFPL_PG3MFP_PWM1_CH4          (0x06UL << SYS_GPG_MFPL_PG3MFP_Pos) /*!< PWM1 channel4 output/capture input. */
#define SYS_GPG_MFPL_PG3MFP_EBI_AD5           (0x07UL << SYS_GPG_MFPL_PG3MFP_Pos) /*!< EBI address/data bus bit5. */
#define SYS_GPG_MFPL_PG3MFP_INT4              (0x08UL << SYS_GPG_MFPL_PG3MFP_Pos) /*!< External interrupt4 input pin. */
#define SYS_GPG_MFPL_PG3MFP_TM3               (0x0BUL << SYS_GPG_MFPL_PG3MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPG_MFPL_PG4MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG4MFP_SPI0_SS           (0x02UL << SYS_GPG_MFPL_PG4MFP_Pos) /*!< 1st SPI0 slave select pin. */
#define SYS_GPG_MFPL_PG4MFP_UART2_nRTS        (0x03UL << SYS_GPG_MFPL_PG4MFP_Pos) /*!< Request to Send output pin for UART2. */
#define SYS_GPG_MFPL_PG4MFP_PWM1_CH5          (0x06UL << SYS_GPG_MFPL_PG4MFP_Pos) /*!< PWM1 channel5 output/capture input. */
#define SYS_GPG_MFPL_PG4MFP_EBI_AD4           (0x07UL << SYS_GPG_MFPL_PG4MFP_Pos) /*!< EBI address/data bus bit4. */
#define SYS_GPG_MFPL_PG4MFP_INT5              (0x08UL << SYS_GPG_MFPL_PG4MFP_Pos) /*!< External interrupt5 input pin. */
#define SYS_GPG_MFPL_PG5MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG5MFP_SPI4_CLK          (0x02UL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< SPI4 serial clock pin. */
#define SYS_GPG_MFPL_PG5MFP_SC0_PWR           (0x05UL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< SmartCard0 power pin. */
#define SYS_GPG_MFPL_PG5MFP_PWM1_CH1          (0x06UL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPG_MFPL_PG5MFP_EBI_AD0           (0x07UL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< EBI address/data bus bit0. */
#define SYS_GPG_MFPL_PG5MFP_SC2_nCD           (0x09UL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< SmartCard2 card detect pin. */
#define SYS_GPG_MFPL_PG5MFP_USCI1_CTL1        (0x0CUL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< USCI1 control1 pin. */
#define SYS_GPG_MFPL_PG5MFP_BPWM0_CH4         (0x0DUL << SYS_GPG_MFPL_PG5MFP_Pos) /*!< BPWM0 channel4 output/capture input. */
#define SYS_GPG_MFPL_PG6MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG6MFP_SPI4_SS           (0x02UL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< 1st SPI4 slave select pin. */
#define SYS_GPG_MFPL_PG6MFP_UART4_nRTS        (0x03UL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< Request to Send output pin for UART4. */
#define SYS_GPG_MFPL_PG6MFP_I2C2_SCL          (0x04UL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< I2C2 clock pin. */
#define SYS_GPG_MFPL_PG6MFP_SC0_nCD           (0x05UL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< SmartCard0 card detect pin. */
#define SYS_GPG_MFPL_PG6MFP_PWM1_CH5          (0x06UL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< PWM1 channel5 output/capture input. */
#define SYS_GPG_MFPL_PG6MFP_EBI_AD1           (0x07UL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPG_MFPL_PG6MFP_TM0_EXT           (0x0BUL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPG_MFPL_PG6MFP_USCI1_CLK         (0x0CUL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< USCI1 clock pin. */
#define SYS_GPG_MFPL_PG6MFP_BPWM0_CH3         (0x0DUL << SYS_GPG_MFPL_PG6MFP_Pos) /*!< BPWM0 channel3 output/capture input. */
#define SYS_GPG_MFPL_PG7MFP_GPIO              (0x00UL << SYS_GPG_MFPL_PG7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPL_PG7MFP_UART0_RXD         (0x03UL << SYS_GPG_MFPL_PG7MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPG_MFPL_PG7MFP_EBI_nRD           (0x07UL << SYS_GPG_MFPL_PG7MFP_Pos) /*!< EBI read enable output pin. */
#define SYS_GPG_MFPL_PG7MFP_SC1_CLK           (0x0BUL << SYS_GPG_MFPL_PG7MFP_Pos) /*!< SmartCard1 clock pin. */
#define SYS_GPG_MFPL_PG7MFP_USCI0_DAT0        (0x0CUL << SYS_GPG_MFPL_PG7MFP_Pos) /*!< USCI0 data0 pin. */
#define SYS_GPG_MFPL_PG7MFP_SD0_CLK           (0x0EUL << SYS_GPG_MFPL_PG7MFP_Pos) /*!< SD/SDIO mode #0??clock. */
/********************* Bit definition of GPG_MFPH register **********************/
#define SYS_GPG_MFPH_PG8MFP_GPIO              (0x00UL << SYS_GPG_MFPH_PG8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPH_PG8MFP_SPI1_SS           (0x01UL << SYS_GPG_MFPH_PG8MFP_Pos) /*!< 1st SPI1 slave select pin. */
#define SYS_GPG_MFPH_PG8MFP_UART0_TXD         (0x03UL << SYS_GPG_MFPH_PG8MFP_Pos) /*!< Data transmitter output pin for UART0. */
#define SYS_GPG_MFPH_PG8MFP_EBI_nWR           (0x07UL << SYS_GPG_MFPH_PG8MFP_Pos) /*!< EBI wirte enable output pin. */
#define SYS_GPG_MFPH_PG8MFP_SC1_DAT           (0x0BUL << SYS_GPG_MFPH_PG8MFP_Pos) /*!< SmartCard1 data pin. */
#define SYS_GPG_MFPH_PG8MFP_USCI0_DAT1        (0x0CUL << SYS_GPG_MFPH_PG8MFP_Pos) /*!< USCI0 data1 pin. */
#define SYS_GPG_MFPH_PG8MFP_SD0_DAT3          (0x0EUL << SYS_GPG_MFPH_PG8MFP_Pos) /*!< SD/SDIO mode #0  data line bit 3. */
#define SYS_GPG_MFPH_PG9MFP_GPIO              (0x00UL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPH_PG9MFP_SPI4_MOSI         (0x02UL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< 1st SPI4 MOSI (Master Out, Slave In) pin. */
#define SYS_GPG_MFPH_PG9MFP_UART4_nCTS        (0x03UL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< Clear to Send input pin for UART4. */
#define SYS_GPG_MFPH_PG9MFP_I2C2_SDA          (0x04UL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< I2C2 data input/output pin. */
#define SYS_GPG_MFPH_PG9MFP_SC0_CLK           (0x05UL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< SmartCard0 clock pin. */
#define SYS_GPG_MFPH_PG9MFP_PWM1_CH4          (0x06UL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< PWM1 channel4 output/capture input. */
#define SYS_GPG_MFPH_PG9MFP_EBI_AD2           (0x07UL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< EBI address/data bus bit2. */
#define SYS_GPG_MFPH_PG9MFP_TM3_EXT           (0x0BUL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< Timer3 event counter input / toggle output */
#define SYS_GPG_MFPH_PG9MFP_USCI1_DAT1        (0x0CUL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< USCI1 data1 pin. */
#define SYS_GPG_MFPH_PG9MFP_BPWM0_CH2         (0x0DUL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< BPWM0 channel2 output/capture input. */
#define SYS_GPG_MFPH_PG9MFP_ECAP1_IC0         (0x0EUL << SYS_GPG_MFPH_PG9MFP_Pos) /*!< Input 0 of enhanced capture unit 1. */
#define SYS_GPG_MFPH_PG10MFP_GPIO             (0x00UL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPH_PG10MFP_SPI4_MISO        (0x02UL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< 1st SPI4 MISO (Master In, Slave Out) pin. */
#define SYS_GPG_MFPH_PG10MFP_UART4_TXD        (0x03UL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< Data transmitter output pin for UART4. */
#define SYS_GPG_MFPH_PG10MFP_I2C2_SMBSUS      (0x04UL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< I2C2 SMBus SMBSUS# pin (PMBus CONTROL pin) */
#define SYS_GPG_MFPH_PG10MFP_SC0_DAT          (0x05UL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< SmartCard0 data pin. */
#define SYS_GPG_MFPH_PG10MFP_PWM1_CH3         (0x06UL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< PWM1 channel3 output/capture input. */
#define SYS_GPG_MFPH_PG10MFP_EBI_AD3          (0x07UL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< EBI address/data bus bit3. */
#define SYS_GPG_MFPH_PG10MFP_TM2_EXT          (0x0BUL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< Timer2 event counter input / toggle output */
#define SYS_GPG_MFPH_PG10MFP_USCI1_DAT0       (0x0CUL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< USCI1 data0 pin. */
#define SYS_GPG_MFPH_PG10MFP_BPWM0_CH1        (0x0DUL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< BPWM0 channel1 output/capture input. */
#define SYS_GPG_MFPH_PG10MFP_ECAP1_IC1        (0x0EUL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< Input 1 of enhanced capture unit 1. */
#define SYS_GPG_MFPH_PG10MFP_SPIM_D3          (0x0FUL << SYS_GPG_MFPH_PG10MFP_Pos) /*!< SPIM data 3 pin for Quad Mode I/O. */
#define SYS_GPG_MFPH_PG11MFP_GPIO             (0x00UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPG_MFPH_PG11MFP_I2C2_SDA         (0x01UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< I2C2 data input/output pin. */
#define SYS_GPG_MFPH_PG11MFP_SPI4_I2SMCLK     (0x02UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< SPI4 I2S master clock output pin. */
#define SYS_GPG_MFPH_PG11MFP_UART4_RXD        (0x03UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< Data receiver input pin for UART4. */
#define SYS_GPG_MFPH_PG11MFP_I2C2_SMBAL       (0x04UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< I2C2 SMBus SMBALTER# pin  */
#define SYS_GPG_MFPH_PG11MFP_SC0_RST          (0x05UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< SmartCard0 reset pin. */
#define SYS_GPG_MFPH_PG11MFP_PWM1_CH2         (0x06UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< PWM1 channel2 output/capture input. */
#define SYS_GPG_MFPH_PG11MFP_EBI_nCS2         (0x07UL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPG_MFPH_PG11MFP_TM1_EXT          (0x0BUL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPG_MFPH_PG11MFP_USCI1_CTL0       (0x0CUL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< USCI1 control0 pin. */
#define SYS_GPG_MFPH_PG11MFP_BPWM0_CH0        (0x0DUL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< BPWM0 channel0 output/capture input. */
#define SYS_GPG_MFPH_PG11MFP_ECAP1_IC2        (0x0EUL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< Input 1 of enhanced capture unit 2. */
#define SYS_GPG_MFPH_PG11MFP_SPIM_D2          (0x0FUL << SYS_GPG_MFPH_PG11MFP_Pos) /*!< SPIM data 2 pin for Quad Mode I/O. */
/********************* Bit definition of GPH_MFPL register **********************/
#define SYS_GPH_MFPL_PH0MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH0MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH0MFP_SPI3_MISO         (0x02UL << SYS_GPH_MFPL_PH0MFP_Pos) /*!< 1st SPI3 MISO (Master In, Slave Out) pin. */
#define SYS_GPH_MFPL_PH0MFP_UART0_RXD         (0x03UL << SYS_GPH_MFPL_PH0MFP_Pos) /*!< Data receiver input pin for UART0. */
#define SYS_GPH_MFPL_PH0MFP_PWM0_CH0          (0x06UL << SYS_GPH_MFPL_PH0MFP_Pos) /*!< PWM0 channel0 output/capture input. */
#define SYS_GPH_MFPL_PH0MFP_EBI_AD15          (0x07UL << SYS_GPH_MFPL_PH0MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPH_MFPL_PH0MFP_INT7              (0x08UL << SYS_GPH_MFPL_PH0MFP_Pos) /*!< External interrupt7 input pin. */
#define SYS_GPH_MFPL_PH1MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH1MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH1MFP_SPI3_MOSI         (0x02UL << SYS_GPH_MFPL_PH1MFP_Pos) /*!< 1st SPI3 MOSI (Master Out, Slave In) pin. */
#define SYS_GPH_MFPL_PH1MFP_UART0_TXD         (0x03UL << SYS_GPH_MFPL_PH1MFP_Pos) /*!< Data transmitter output pin for UART0. */
#define SYS_GPH_MFPL_PH1MFP_PWM0_CH1          (0x06UL << SYS_GPH_MFPL_PH1MFP_Pos) /*!< PWM0 channel1 output/capture input. */
#define SYS_GPH_MFPL_PH1MFP_EBI_AD14          (0x07UL << SYS_GPH_MFPL_PH1MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPH_MFPL_PH2MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH2MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH2MFP_SPI3_SS           (0x02UL << SYS_GPH_MFPL_PH2MFP_Pos) /*!< 1st SPI3 slave select pin. */
#define SYS_GPH_MFPL_PH2MFP_UART0_nCTS        (0x03UL << SYS_GPH_MFPL_PH2MFP_Pos) /*!< Clear to Send input pin for UART0. */
#define SYS_GPH_MFPL_PH2MFP_PWM0_CH2          (0x06UL << SYS_GPH_MFPL_PH2MFP_Pos) /*!< PWM0 channel2 output/capture input. */
#define SYS_GPH_MFPL_PH2MFP_EBI_AD13          (0x07UL << SYS_GPH_MFPL_PH2MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPH_MFPL_PH3MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH3MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH3MFP_SPI3_CLK          (0x02UL << SYS_GPH_MFPL_PH3MFP_Pos) /*!< SPI3 serial clock pin. */
#define SYS_GPH_MFPL_PH3MFP_UART0_nRTS        (0x03UL << SYS_GPH_MFPL_PH3MFP_Pos) /*!< Request to Send output pin for UART0. */
#define SYS_GPH_MFPL_PH3MFP_PWM0_CH4          (0x06UL << SYS_GPH_MFPL_PH3MFP_Pos) /*!< PWM0 channel4 output/capture input. */
#define SYS_GPH_MFPL_PH3MFP_EBI_AD12          (0x07UL << SYS_GPH_MFPL_PH3MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPH_MFPL_PH4MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH4MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH4MFP_UART1_RXD         (0x03UL << SYS_GPH_MFPL_PH4MFP_Pos) /*!< Data receiver input pin for UART1. */
#define SYS_GPH_MFPL_PH4MFP_PWM1_BRAKE0       (0x06UL << SYS_GPH_MFPL_PH4MFP_Pos) /*!< Brake input pin 0 of PWM1. */
#define SYS_GPH_MFPL_PH4MFP_EBI_AD11          (0x07UL << SYS_GPH_MFPL_PH4MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPH_MFPL_PH4MFP_TM0_EXT           (0x0BUL << SYS_GPH_MFPL_PH4MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPH_MFPL_PH5MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH5MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH5MFP_UART1_TXD         (0x03UL << SYS_GPH_MFPL_PH5MFP_Pos) /*!< Data transmitter output pin for UART1. */
#define SYS_GPH_MFPL_PH5MFP_PWM1_BRAKE1       (0x06UL << SYS_GPH_MFPL_PH5MFP_Pos) /*!< Brake input pin 1 of PWM1. */
#define SYS_GPH_MFPL_PH5MFP_EBI_AD10          (0x07UL << SYS_GPH_MFPL_PH5MFP_Pos) /*!< EBI address/data bus bit1. */
#define SYS_GPH_MFPL_PH5MFP_TM0               (0x0BUL << SYS_GPH_MFPL_PH5MFP_Pos) /*!< Timer0 event counter input / toggle output */
#define SYS_GPH_MFPL_PH6MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH6MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH6MFP_UART1_nCTS        (0x03UL << SYS_GPH_MFPL_PH6MFP_Pos) /*!< Clear to Send input pin for UART1. */
#define SYS_GPH_MFPL_PH6MFP_PWM1_CH0          (0x06UL << SYS_GPH_MFPL_PH6MFP_Pos) /*!< PWM1 channel0 output/capture input. */
#define SYS_GPH_MFPL_PH6MFP_EBI_AD9           (0x07UL << SYS_GPH_MFPL_PH6MFP_Pos) /*!< EBI address/data bus bit9. */
#define SYS_GPH_MFPL_PH6MFP_TM1_EXT           (0x0BUL << SYS_GPH_MFPL_PH6MFP_Pos) /*!< Timer1 event counter input / toggle output */
#define SYS_GPH_MFPL_PH7MFP_GPIO              (0x00UL << SYS_GPH_MFPL_PH7MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPL_PH7MFP_SPI0_MISO1        (0x02UL << SYS_GPH_MFPL_PH7MFP_Pos) /*!< 2nd SPI0 MISO (Master In, Slave Out) pin. */
#define SYS_GPH_MFPL_PH7MFP_UART1_nRTS        (0x03UL << SYS_GPH_MFPL_PH7MFP_Pos) /*!< Request to Send output pin for UART1. */
#define SYS_GPH_MFPL_PH7MFP_PWM1_CH1          (0x06UL << SYS_GPH_MFPL_PH7MFP_Pos) /*!< PWM1 channel1 output/capture input. */
#define SYS_GPH_MFPL_PH7MFP_EBI_AD8           (0x07UL << SYS_GPH_MFPL_PH7MFP_Pos) /*!< EBI address/data bus bit8. */
#define SYS_GPH_MFPL_PH7MFP_TM1               (0x0BUL << SYS_GPH_MFPL_PH7MFP_Pos) /*!< Timer1 event counter input / toggle output */
/********************* Bit definition of GPH_MFPH register **********************/
#define SYS_GPH_MFPH_PH8MFP_GPIO              (0x00UL << SYS_GPH_MFPH_PH8MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH8MFP_UART5_RXD         (0x06UL << SYS_GPH_MFPH_PH8MFP_Pos) /*!< Data receiver input pin for UART5. */
#define SYS_GPH_MFPH_PH8MFP_HSUSB_VBUS_ST     (0x0BUL << SYS_GPH_MFPH_PH8MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPH_MFPH_PH8MFP_QEI0_A            (0x0CUL << SYS_GPH_MFPH_PH8MFP_Pos) /*!< Quadrature encoder phase A input of QEI Unit 0. */
#define SYS_GPH_MFPH_PH9MFP_GPIO              (0x00UL << SYS_GPH_MFPH_PH9MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH9MFP_UART5_TXD         (0x06UL << SYS_GPH_MFPH_PH9MFP_Pos) /*!< Data transmitter output pin for UART5. */
#define SYS_GPH_MFPH_PH9MFP_HSUSB_VBUS_EN     (0x0BUL << SYS_GPH_MFPH_PH9MFP_Pos) /*!< Power supply from USB High speed host or HUB. */
#define SYS_GPH_MFPH_PH9MFP_QEI0_B            (0x0CUL << SYS_GPH_MFPH_PH9MFP_Pos) /*!< Quadrature encoder phase B input of QEI Unit 0. */
#define SYS_GPH_MFPH_PH10MFP_GPIO             (0x00UL << SYS_GPH_MFPH_PH10MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH10MFP_UART5_nCTS       (0x06UL << SYS_GPH_MFPH_PH10MFP_Pos) /*!< Clear to Send input pin for UART5. */
#define SYS_GPH_MFPH_PH10MFP_USB_VBUS_ST      (0x0BUL << SYS_GPH_MFPH_PH10MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPH_MFPH_PH10MFP_QEI0_INDEX       (0x0CUL << SYS_GPH_MFPH_PH10MFP_Pos) /*!< Quadrature encoder index input of QEI Unit 0. */
#define SYS_GPH_MFPH_PH11MFP_GPIO             (0x00UL << SYS_GPH_MFPH_PH11MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH11MFP_UART5_nRTS       (0x06UL << SYS_GPH_MFPH_PH11MFP_Pos) /*!< Request to Send output pin for UART5. */
#define SYS_GPH_MFPH_PH11MFP_USB_VBUS_EN      (0x0BUL << SYS_GPH_MFPH_PH11MFP_Pos) /*!< Power supply from USB Full speed host or HUB. */
#define SYS_GPH_MFPH_PH12MFP_GPIO             (0x00UL << SYS_GPH_MFPH_PH12MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH12MFP_UART3_RXD        (0x04UL << SYS_GPH_MFPH_PH12MFP_Pos) /*!< Data receiver input pin for UART3. */
#define SYS_GPH_MFPH_PH12MFP_PWM1_CH3         (0x06UL << SYS_GPH_MFPH_PH12MFP_Pos) /*!< PWM1 channel3 output/capture input. */
#define SYS_GPH_MFPH_PH12MFP_EBI_ALE          (0x07UL << SYS_GPH_MFPH_PH12MFP_Pos) /*!< EBI address latch enable output pin. */
#define SYS_GPH_MFPH_PH12MFP_SPI2_MISO        (0x0AUL << SYS_GPH_MFPH_PH12MFP_Pos) /*!< 1st SPI2 MISO (Master In, Slave Out) pin. */
#define SYS_GPH_MFPH_PH12MFP_BPWM1_CH2        (0x0DUL << SYS_GPH_MFPH_PH12MFP_Pos) /*!< BPWM1 channel2 output/capture input. */
#define SYS_GPH_MFPH_PH13MFP_GPIO             (0x00UL << SYS_GPH_MFPH_PH13MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH13MFP_UART3_TXD        (0x04UL << SYS_GPH_MFPH_PH13MFP_Pos) /*!< Data transmitter output pin for UART3. */
#define SYS_GPH_MFPH_PH13MFP_EBI_nCS2         (0x07UL << SYS_GPH_MFPH_PH13MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPH_MFPH_PH13MFP_SPI2_MOSI        (0x0AUL << SYS_GPH_MFPH_PH13MFP_Pos) /*!< 1st SPI2 MOSI (Master Out, Slave In) pin. */
#define SYS_GPH_MFPH_PH13MFP_BPWM1_CH3        (0x0DUL << SYS_GPH_MFPH_PH13MFP_Pos) /*!< BPWM1 channel3 output/capture input. */
#define SYS_GPH_MFPH_PH14MFP_GPIO             (0x00UL << SYS_GPH_MFPH_PH14MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH14MFP_UART3_nCTS       (0x04UL << SYS_GPH_MFPH_PH14MFP_Pos) /*!< Clear to Send input pin for UART3. */
#define SYS_GPH_MFPH_PH14MFP_EBI_nCS1         (0x07UL << SYS_GPH_MFPH_PH14MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPH_MFPH_PH14MFP_SPI2_SS          (0x0AUL << SYS_GPH_MFPH_PH14MFP_Pos) /*!< 1st SPI2 slave select pin. */
#define SYS_GPH_MFPH_PH14MFP_BPWM1_CH4        (0x0DUL << SYS_GPH_MFPH_PH14MFP_Pos) /*!< BPWM1 channel4 output/capture input. */
#define SYS_GPH_MFPH_PH15MFP_GPIO             (0x00UL << SYS_GPH_MFPH_PH15MFP_Pos) /*!< General purpose digital I/O pin. */
#define SYS_GPH_MFPH_PH15MFP_UART3_nRTS       (0x04UL << SYS_GPH_MFPH_PH15MFP_Pos) /*!< Request to Send output pin for UART3. */
#define SYS_GPH_MFPH_PH15MFP_EBI_nCS0         (0x07UL << SYS_GPH_MFPH_PH15MFP_Pos) /*!< EBI chip select enable output pin. */
#define SYS_GPH_MFPH_PH15MFP_SPI2_CLK         (0x0AUL << SYS_GPH_MFPH_PH15MFP_Pos) /*!< SPI2 serial clock pin. */
#define SYS_GPH_MFPH_PH15MFP_BPWM1_CH5        (0x0DUL << SYS_GPH_MFPH_PH15MFP_Pos) /*!< BPWM1 channel5 output/capture input. */

/*@}*/ /* end of group M480_SYS_EXPORTED_CONSTANTS */


/** @addtogroup M480_SYS_EXPORTED_FUNCTIONS SYS Exported Functions
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
  *             - \ref SYS_BODCTL_BODVL_3_0V
  *             - \ref SYS_BODCTL_BODVL_2_8V
  *             - \ref SYS_BODCTL_BODVL_2_6V
  *             - \ref SYS_BODCTL_BODVL_2_4V
  *             - \ref SYS_BODCTL_BODVL_2_2V
  *             - \ref SYS_BODCTL_BODVL_2_0V
  *             - \ref SYS_BODCTL_BODVL_1_8V
  *             - \ref SYS_BODCTL_BODVL_1_6V
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
  *             - \ref SYS_RSTSTS_SYSRF_Msk
  *             - \ref SYS_RSTSTS_CPURF_Msk
  *             - \ref SYS_RSTSTS_CPULKRF_Msk
  * @return     None
  * @details    This macro clear reset source flag.
  */
#define SYS_CLEAR_RST_SOURCE(u32RstSrc) ((SYS->RSTSTS) = (u32RstSrc) )


/*---------------------------------------------------------------------------------------------------------*/
/* static inline functions                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/


/**
  * @brief      Disable register write-protection function
  * @param      None
  * @return     None
  * @details    This function disable register write-protection function.
  *             To unlock the protected register to allow write access.
  */
__STATIC_INLINE void SYS_UnlockReg(void)
{
    do {
        SYS->REGLCTL = 0x59;
        SYS->REGLCTL = 0x16;
        SYS->REGLCTL = 0x88;
    } while(SYS->REGLCTL == 0);
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


/*@}*/ /* end of group M480_SYS_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_SYS_Driver */

/*@}*/ /* end of group M480_Device_Driver */


#ifdef __cplusplus
}
#endif

#endif  //__SYS_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
