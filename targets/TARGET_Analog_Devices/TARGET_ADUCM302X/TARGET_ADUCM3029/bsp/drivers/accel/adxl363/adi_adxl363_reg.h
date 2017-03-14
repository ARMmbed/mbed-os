/*!
 ******************************************************************************
 * @file:    adi_adxl363_reg.h
 * @brief:   ADXL363 register definition  header file which contain the bit position  for vairous
             filed of the reigsters and address of the registers.   
 * @version: $Revision$
 * @date:    $Date$
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2014 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/


#ifndef _ADI_ADXL363_REG_H_
#define _ADI_ADXL363_REG_H_
/* Register address */
/*! \cond PRIVATE */
#define ADI_ADXL363_ADI_DEVID           0x00u   /* Analog Devices, Inc., accelerometer ID */
#define ADI_ADXL363_MEMS_DEVID          0x01u   /* Analog Devices MEMS device ID */
#define ADI_ADXL363_DEVID               0x02u   /* Device ID */
#define ADI_ADXL363_REVID               0x03u   /* product revision ID*/
#define ADI_ADXL363_XDATA               0x08u   /* X-axis acceleration data*/
#define ADI_ADXL363_YDATA               0x09u   /* Y-axis acceleration data*/
#define ADI_ADXL363_ZDATA               0x0Au   /* Z-axis acceleration data*/
#define ADI_ADXL363_STATUS              0x0Bu   /* Status register */
#define ADI_ADXL363_FIFO_ENTRIES_L      0x0Cu   /* Valid data samples in the FIFO-L */
#define ADI_ADXL363_FIFO_ENTRIES_H      0x0Du   /* Valid data samples in the FIFO-H */
#define ADI_ADXL363_DATAX_L             0x0Eu   /* X-axis acceleration data (lower part of 12 bit)*/
#define ADI_ADXL363_DATAX_H             0x0Fu   /* X-axis acceleration data (Higher part of 12 bit) */
#define ADI_ADXL363_DATAY_L             0x10u   /* Y-axis acceleration data (lower part of 12 bit)*/
#define ADI_ADXL363_DATAY_H             0x11u   /* Y-axis acceleration data (Higher part of 12 bit) */
#define ADI_ADXL363_DATAZ_L             0x12u   /* Z-axis acceleration data (lower part of 12 bit)*/
#define ADI_ADXL363_DATAZ_H             0x13u   /* Z-axis acceleration data (Higher part of 12 bit) */

#define ADI_ADXL363_TEMP_DATA_L         0x14u   /* Temperature sensor output data LSBs*/
#define ADI_ADXL363_TEMP_DATA_H         0x15u   /* Temperature sensor output data MSBs*/
#define ADI_ADXL363_DATAADC_L           0x16u   /* ADC-axis acceleration data (lower part of 12 bit)*/
#define ADI_ADXL363_DATAADC_H           0x17u   /* ADC-axis acceleration data (Higher part of 12 bit) */

#define ADI_ADXL363_SRESET              0x1Fu   /* Z axis offset */
#define ADI_ADXL363_THRESH_ACT_L        0x20u   /* Activity threshold LSBs*/
#define ADI_ADXL363_THRESH_ACT_H        0x21u   /* Activity threshold  MSBs*/
#define ADI_ADXL363_TIME_ACT            0x22u   /* Activity time */
#define ADI_ADXL363_THRESH_INACT_L      0x23u   /* Inactivity threshold LSBs */
#define ADI_ADXL363_THRESH_INACT_H      0x24u   /* Inactivity threshold MSBs*/
#define ADI_ADXL363_TIME_INACT_L        0x25u   /* Inctivity time LSBs*/
#define ADI_ADXL363_TIME_INACT_H        0x26u   /* Inctivity time MSBs*/
#define ADI_ADXL363_ACT_INACT_CTL       0x27u   /* Act/Inact control  */
#define ADI_ADXL363_FIFO_CTL            0x28u   /* FIFO control */
#define ADI_ADXL363_FIFO_SAMPLES        0x29u   /* FIFO samples */
#define ADI_ADXL363_INT1_MAP            0x2Au   /* Interrupt-1 mapping control */
#define ADI_ADXL363_INT2_MAP            0x2Bu   /* Interrupt-2 mapping control */
#define ADI_ADXL363_FILTER_CTL          0x2Cu   /* Filter control*/
#define ADI_ADXL363_POWER_CTL           0x2Du   /* Power control */
#define ADI_ADXL363_SELF_TEST           0x2Eu   /* Self test */



/* ==================================================== *
     Register fields position, masks and enumerations
 * ==================================================== */

/* ACT_INACT_CTL register */
#define BITP_ADXL363_ACT_EN             0x00u   /* Bit position for Activity Enable.*/
#define BITM_ADXL363_ACT_EN             0x01u   /* Bit mask for Activity Enable. */
#define BITP_ADXL363_ACT_REF            0x01u   /* Bit position for activity absolute/reference*/
#define BITM_ADXL363_ACT_REF            0x02u   /* Bit mask for activity absolute/reference*/
#define BITP_ADXL363_INACT_EN           0x02u   /* DC or AC coupled activity detection operation */
#define BITM_ADXL363_INACT_EN           0x04u   /* Bit mask for Activity ACDC */
#define BITP_ADXL363_INACT_REF          0x03u   /* Bit position for activity absolute/reference */
#define BITM_ADXL363_INACT_REF          0x08u   /* Bit mask for Inactivity absolute/reference */
#define BITP_ADXL363_LINK_LOOP          0x04u   /* Bit position for Link-Loop mode */
#define BITM_ADXL363_LINK_LOOP          0x30u   /* Bit mask for Link-Loop mode */

#define BITP_ADXL363_FIFO_MODE          0x00u   /* Bit position for FIFO mode */
#define BITM_ADXL363_FIFO_MODE          0x03u   /* Bit mask for for FIFO mode  */
#define BITP_ADXL363_TEMP_FIFO_EN       0x02u   /* Bit position for enabling storing temperature data in the FIFO */
#define BITM_ADXL363_TEMP_FIFO_EN       0x04u   /* Bit mask for enabling storing temperature data in the FIFO  */
#define BITP_ADXL363_FIFO_HALF          0x03u   /* Bit position for enabling storing temperature data in the FIFO */
#define BITM_ADXL363_FIFO_HALF          0x08u   /* Bit mask for enabling storing temperature data in the FIFO  */


#define BITP_ADXL363_ODR                0x00u   /* Bit position for Output Data Rate. */
#define BITM_ADXL363_ODR                0x07u   /* Bit mask for Output Data Rate. */
#define BITP_ADXL363_EXT_SAMLING_EN     0x03u   /* Bit position for External Sampling Trigger */
#define BITM_ADXL363_EXT_SAMLING_EN     0x08u   /* Bit mask for External Sampling Trigger */
#define BITP_ADXL363_HALF_BW_EN         0x04u   /* Bit position for Halved Bandwidth */
#define BITM_ADXL363_HALF_BW_EN         0x10u   /* Bit mask for Halved Bandwidth */
#define BITP_ADXL363_RNAGE              0x06u   /* Bit position for Measurement Range */
#define BITM_ADXL363_RANGE              0xC0u   /* Bit mask for Measurement Range */

#define BITP_ADXL363_MEASURE_EN         0x00u   /* Bit position for Measurement Mode */
#define BITM_ADXL363_MEASURE_EN         0x03u   /* Bit mask  for Measurement Mode*/
#define BITP_ADXL363_AUTO_SLEEP         0x02u   /* Bit position for Autosleep. */
#define BITM_ADXL363_AUTO_SLEEP         0x04u   /* Bit mask for Autosleep. */
#define BITP_ADXL363_WAKE_UP            0x03u   /* Bit position forWake-Up Mode. */
#define BITM_ADXL363_WAKE_UP            0x08u   /* Bit mask forWake-Up Mode. */
#define BITP_ADXL363_LOW_NOISE          0x04u   /* Bit position for Power vs. Noise level  */
#define BITM_ADXL363_LOW_NOISE          0x30u   /* Bit mask for Power vs. Noise level */
#define BITP_ADXL363_EXT_CLK_EN         0x06u   /* Bit position for External Clock */
#define BITM_ADXL363_EXT_CLK_EN         0x40u   /* Bit mask for External Clock */
#define BITP_ADXL363_ADC_EN             0x07u   /* Bit position for ADC Enable. */
#define BITM_ADXL363_ADC_EN             0x80u   /* Bit mask for ADC Enable. */

#define BITP_ADXL363_INT_DATA_READY     0x00u   /* Bit position for Data Ready Interrupt. */
#define BITM_ADXL363_INT_DATA_READY     0x01u   /* Bit mask  for for Data Ready Interrupt.*/
#define BITP_ADXL363_INT_FIFO_READY     0x01u   /* Bit position  for FIFO Ready Interrupt*/
#define BITM_ADXL363_INT_FIFO_READY     0x02u   /* Bit mask  for FIFO Ready Interrupt*/
#define BITP_ADXL363_INT_FIFO_WATERMART 0X02u   /* Bit position for FIFO Watermark Interrupt */
#define BITM_ADXL363_INT_FIFO_WATERMART 0X04u   /* Bit mask for FIFO Watermark Interrupt */
#define BITP_ADXL363_INT_FIFO_OVERRUN   0x03u   /* Bit position for FIFO Overrun Interrupt. */
#define BITM_ADXL363_INT_FIFO_OVERRUN   0x08u   /* Bit mask for for FIFO Overrun Interrupt. */
#define BITP_ADXL363_INT_ACTIVITY       0x04u   /* Bit position for Activity Interrupt. */
#define BITM_ADXL363_INT_ACTIVITY       0x10u   /* Bit mask for for Activity Interrupt. */
#define BITP_ADXL363_INT_INACTIVITY     0x05u   /* Bit position for Inactivity Interrupt. */
#define BITM_ADXL363_INT_INACTIVITY     0x20u   /* Bit mask for Inactivity Interrupt. */
#define BITP_ADXL363_INT_AWAKE          0x06u   /* Bit position for Awake Interrupt. */
#define BITM_ADXL363_INT_AWAKE          0x40u   /* Bit mask for Awake Interrupt. */
#define BITP_ADXL363_INT_LOW            0x07u   /* Bit position for Interrupt Active Low. */
#define BITM_ADXL363_INT_LOW            0x80u   /* Bit mask for Interrupt Active Low. */

#define BITP_ADXL363_SELFTEST_EN        0x07u   /* Bit position for Self Test. */
#define BITM_ADXL363_SELFTEST_EN        0x80u   /* Bit mask for Self Test. */

/*! \endcond */
#endif /* _ADI_ADXL363_REG_H_ */
