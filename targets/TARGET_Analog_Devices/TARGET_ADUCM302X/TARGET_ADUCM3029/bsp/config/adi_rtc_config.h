/*!
 *****************************************************************************
   @file:    adi_rtc_config.h
   @brief:   Configuration options for Real Time Clock  device  driver.
             This is specific to the RTC driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 33005 $
   @date:    $Date: 2015-12-12 10:43:13 -0500 (Sat, 12 Dec 2015) $
  -----------------------------------------------------------------------------

Copyright (c) 2012-2016 Analog Devices, Inc.

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

*****************************************************************************/

#ifndef ADI_RTC_CONFIG_H__
#define ADI_RTC_CONFIG_H__
#include <adi_global_config.h>

/** @addtogroup RTC_Driver_Config Static Configuration
 *  @ingroup RTC_Driver
 *  @{
 */

/*!
 * The safe write mode insures any pending writes that have not yet synchronized between the faster core clock
 * domain and the internal RTC 32kHz clock domain are reconciled before multiple writes to the same RTC register
 * are allowed
*/

#define ADI_RTC_CFG_ENABLE_SAFE_WRITE 1    


/** @addtogroup RTC_Driver_Config_RTC0 RTC0 Static Configuration
 *  @ingroup RTC_Driver_Config
 *  @{
 */

/*
===================================================================
 ------------------------RTC-0 CONFIGURATION MACRO-----------------
===================================================================
*/
/*! Enable the Alarm */
#define RTC0_CFG_ENABLE_ALARM                  0     

/*! Enable the Alarm interrupt*/
#define RTC0_CFG_ENABLE_ALARM_INTERRUPT        0     

/*! Enable the Trim */
#define RTC0_CFG_ENABLE_TRIM                    0     

/*! Enable the PENDERROR interrupt*/
#define RTC0_CFG_ENABLE_PENDERROR_INTERRUPT     0     

/*! Enable the write sync  interrupt*/
#define RTC0_CFG_ENABLE_WSYNC_INTERRUPT         0     

/*! Enable the pend write interrupt*/
#define RTC0_CFG_ENABLE_WRITEPEND_INTERRUPT     0     

/*! Initial the count Value*/
#define RTC0_CFG_COUNT_VALUE                    0     

/*! Initial the count Value-0*/
#define RTC0_CFG_COUNT_VALUE_0                  0     

/*! Initial the count Value-1*/
#define RTC0_CFG_COUNT_VALUE_1                  0     

/*! Alarm-0  Value*/
#define RTC0_CFG_ALARM_VALUE_0                  0     

/*! Alarm-1  Value*/
#define RTC0_CFG_ALARM_VALUE_1                  0     

/*! Trim interval*/
#define RTC0_CFG_TRIM_INTERVAL                  0     

/*! Trim interval with power of 2*/
#define RTC0_CFG_POW2_TRIM_INTERVAL             0     

/*! Trim operation to be performed for RTC0*/
#define RTC0_CFG_TRIM_OPERATION                 0     

/*! Trim  Value for RTC-0*/
#define RTC0_CFG_TRIM_VALUE                     0     

/*! GPIO Sample around Rising Edge of Sensor Strobe Channel 3. 
 * Enables sampling of Sensor Strobe GPIO inputs around rising edge of Sensor Strobe Channel 3 pulse.
 *
 *  0 No sampling of input around rising edge.
 *  1 Input sampled one clock cycle before rising edge of Sensor Strobe.
 * 10 Input sampled at rising edge of Sensor Strobe.
 * 11 Input sampled one clock cycle after rising edge of Sensor Strobe.
 */
#define RTC0_SS3_SMPONRE                          0

/*! GPIO Sample around Falling Edge of Sensor Strobe Channel 3. 
 *  Enables sampling of Sensor Strobe GPIO inputs around falling edge of Sensor Strobe Channel 3 pulse. 
 *
 *  0	No sampling of input around rising edge.		
 *  1	Input sampled one clock cycle before rising edge of Sensor Strobe.		
 * 10	Input sampled at rising edge of Sensor Strobe.
 * 11	Input sampled one clock cycle after rising edge of Sensor Strobe.		
 */
#define RTC0_SS3_SMPONFE                          0
/*! GPIO Sample around Falling Edge of Sensor Strobe Channel 2.  */
#define RTC0_SS2_SMPONFE                          0
/*! GPIO Sample around Rising Edge of Sensor Strobe Channel 1.   */ 
#define RTC0_SS1_SMPONRE                          0
/*! GPIO Sample around Falling Edge of Sensor Strobe Channel 1.  */
#define RTC0_SS1_SMPONFE                          0


/*! Sensor Strobe's GP Input Sampling Mux
 *  SS 2 GPIO Pin 1
 *
 * GPMUX0/1.SSxGPINySEL	3’b000	3’b001	3’b010	3’b011	3’b100	3’b101	3’b110	3’b111
 * RTCSSxGPIny           p0[12] p2[0]    p0[9]  p0[8]   p1[13]   p1[2]   p2[7]   p2[9]
 */
#define RTC0_SS2_GPIN1SEL                       0x4
/*! Sensor Strobe's GP Input Sampling Mux SS 2 GPIO Pin 0*/
#define RTC0_SS2_GPIN0SEL                       0x3
/*! Sensor Strobe's GP Input Sampling Mux SS 1 GPIO Pin 2*/
#define RTC0_SS1_GPIN2SEL                       0x2
/*! Sensor Strobe's GP Input Sampling Mux SS 1 GPIO Pin 1*/
#define RTC0_SS1_GPIN1SEL                       0x1
/*! Sensor Strobe's GP Input Sampling Mux SS 1 GPIO Pin 0*/
#define RTC0_SS1_GPIN0SEL                       0x0
/*! Sensor Strobe's GP Input Sampling Mux SS 3 GPIO Pin 2*/
#define RTC0_SS3_GPIN2SEL                       0x0
/*! Sensor Strobe's GP Input Sampling Mux SS 3 GPIO Pin 1*/
#define RTC0_SS3_GPIN1SEL                       0x7
/*! Sensor Strobe's GP Input Sampling Mux SS 3 GPIO Pin 0*/
#define RTC0_SS3_GPIN0SEL                       0x6
/*! Sensor Strobe's GP Input Sampling Mux SS 2 GPIO Pin 2*/
#define RTC0_SS2_GPIN2SEL                       0x5

/*! Differential output option for Sensor Strobe channel 3.
 *  Sensor Strobe channel3 is used as differential signal, actual RTC_SS3 out 
 *  for this channel is available in corresponding GPIO. 
 *  RTC_SS4 of Sensor Strobe channel 4 is used to provided inverted signal of RTC_SS3.
 */
#define RTC0_SS3_DIFFOUT                           0
/*! Differential output option for Sensor Strobe channel 1.
 *  Sensor Strobe channel 1 is used as differential signal, actual RTC_SS1 out 
 *  for this channel is available in corresponding GPIO. 
 *  RTC_SS1 of Sensor Strobe channel 2 is used to provided inverted signal of RTC_SS1.
 */
#define RTC0_SS1_DIFFOUT                           0



/*! @} */

/*
===================================================================
 ------------------------RTC-1 CONFIGURATION MACRO-----------------
===================================================================
*/

/** @addtogroup RTC_Driver_Config_RTC1 RTC1 Static Configuration
 *  @ingroup RTC_Driver_Config
 *  @{
 */


   
/*! Enable the Alarm */
#define RTC1_CFG_ENABLE_ALARM                       0     

/*! Enable the Alarm interrupt*/
#define RTC1_CFG_ENABLE_ALARM_INTERRUPT             0     

/*! Enable the Trim */
#define RTC1_CFG_ENABLE_TRIM                        0     

/*! Enable the mod-60 Alarm */
#define RTC1_CFG_ENABLE_MOD60_ALARM                 0     

/*! Enable the mod-60 Alarm  period*/
#define RTC1_CFG_ENABLE_MOD60_ALARM_PERIOD          0     

/*! Enable the Alarm interrupt*/
#define RTC1_CFG_ENABLE_MOD60_ALARM_INTERRUPT       0     

/*! Enable the ISOINT interrupt*/
#define RTC1_CFG_ENABLE_ISO_INTERRUPT               0     

/*! Enable the PENDERROR interrupt*/
#define RTC1_CFG_ENABLE_PENDERROR_INTERRUPT         0     

/*! Enable the write sync  interrupt*/
#define RTC1_CFG_ENABLE_WSYNC_INTERRUPT             0     

/*! Enable the pend write interrupt*/
#define RTC1_CFG_ENABLE_WRITEPEND_INTERRUPT         0     

/*! Enable the RTC count interrupt*/
#define RTC1_CFG_ENABLE_COUNT_INTERRUPT             0     

/*! Enable the prescaled modulo-1 interrupt*/
#define RTC1_CFG_ENABLE_MOD1_COUNT_INTERRUPT        0     

/*! Enable the Trim interrupt*/
#define RTC1_CFG_ENABLE_TRIM_INTERRUPT              0     

/*! Enable the Mod60 roll over  interrupt*/
#define RTC1_CFG_CNT_MOD60_ROLLLOVER_INTERRUPT      0     

/*! Prescale value for the RTC1*/
#define RTC1_CFG_PRESCALE                           0     

/*! Enable the counter roll over  interrupt*/
#define RTC1_CFG_CNT_ROLLLOVER_INTERRUPT            0     

/*! Initial the count Value-0*/
#define RTC1_CFG_COUNT_VALUE_0                      0     

/*! Initial the count Value-1*/
#define RTC1_CFG_COUNT_VALUE_1                      0     

/*! Alarm  Value-0*/
#define RTC1_CFG_ALARM_VALUE_0                      0     

/*! Alarm  Value-1*/
#define RTC1_CFG_ALARM_VALUE_1                      0     

/*! Alarm  Value-2*/
#define RTC1_CFG_ALARM_VALUE_2                      0     

/*! Trim interval*/
#define RTC1_CFG_TRIM_INTERVAL                      0     

/*! Trim interval with power of 2*/
#define RTC1_CFG_POW2_TRIM_INTERVAL                 0     

/*! Trim operation to be performed for RTC1*/
#define RTC1_CFG_TRIM_OPERATION                     0     

/*! Trim  Value for RTC-1*/
#define RTC1_CFG_TRIM_VALUE                         0     

/*! Enable the input capture channel-0*/
#define RTC1_CFG_IC0_ENABLE                         0     

/*! Enable the input capture channel-2*/
#define RTC1_CFG_IC2_ENABLE                         0     

/*! Enable the input capture channel-3*/
#define RTC1_CFG_IC3_ENABLE                         0     

/*! Enable the input capture channel-4*/
#define RTC1_CFG_IC4_ENABLE                         0     

/*! Enable the Sensor Strobe channel-1*/
#define RTC1_CFG_SS1_ENABLE                         0     
/*! Enable the Sensor Strobe channel-2*/
#define RTC1_CFG_SS2_ENABLE                         0     
/*! Enable the Sensor Strobe channel-3*/
#define RTC1_CFG_SS3_ENABLE                         0     
/*! Enable the Sensor Strobe channel-4*/
#define RTC1_CFG_SS4_ENABLE                         0     

/*! Enable the interrupt for input capture channel-0*/
#define RTC1_CFG_IC0_INT_ENABLE                     0     

/*! Enable the interrupt for input capture channel-2*/
#define RTC1_CFG_IC2_INT_ENABLE                     0     

/*! Enable the interrupt for input capture channel-3*/
#define RTC1_CFG_IC3_INT_ENABLE                     0     

/*! Enable the interrupt for input capture channel-4*/
#define RTC1_CFG_IC4_INT_ENABLE                     0     

/*! Enable the over write input capture channels*/
#define RTC1_CFG_IC_OVER_WRITE_ENABLE               0    

/*! Polarity for  input capture channel-0*/
#define RTC1_CFG_IC0_EDGE_POLARITY                  0     

/*! Polarity for  input capture channel-2*/
#define RTC1_CFG_IC2_EDGE_POLARITY                  0     

/*! Polarity for  input capture channel-3*/
#define RTC1_CFG_IC3_EDGE_POLARITY                  0     

/*! Polarity for  input capture channel-4*/
#define RTC1_CFG_IC4_EDGE_POLARITY                  0     

/*! Enable the interrupt for Sensor Strobe channel-1*/
#define RTC1_CFG_SS1_INT_ENABLE                     0     
/*! Enable the interrupt for Sensor Strobe channel-2*/
#define RTC1_CFG_SS2_INT_ENABLE                     0     
/*! Enable the interrupt for Sensor Strobe channel-3*/
#define RTC1_CFG_SS3_INT_ENABLE                     0     
/*! Enable the interrupt for Sensor Strobe channel-4*/
#define RTC1_CFG_SS4_INT_ENABLE                     0    

/*! Enable the masking for Sensor Strobe channel-1*/
#define RTC1_CFG_SS1_MASK_ENABLE                    0     
/*! Enable the masking for Sensor Strobe channel-2*/
#define RTC1_CFG_SS2_MASK_ENABLE                    0     
/*! Enable the masking for Sensor Strobe channel-3*/
#define RTC1_CFG_SS3_MASK_ENABLE                    0     
/*! Enable the masking for Sensor Strobe channel-4*/
#define RTC1_CFG_SS4_MASK_ENABLE                    0     

/*! Enable the auto-reloading for Sensor Strobe channel-0*/
#define RTC1_CFG_SS1_AUTO_RELOADING_ENABLE          0     

/*! Mask for Sensor Strobe channel-0 */
#define RTC1_CFG_SS1_MASK_VALUE                     0     


/*! Auto reload value  for Sensor Strobe channel-0 */
#define RTC1_CFG_SS1_AUTO_RELOAD_VALUE              32768/2


/*! Sensor Strobe GP Input Sampling Mux
 *  SS2 GPIO Pin 1
 *
 * GPMUX0/1.SSxGPINySEL	3’b000	3’b001	3’b010	3’b011	3’b100	3’b101	3’b110	3’b111
 * RTCSSxGPIny           p0[12] p2[0]    p0[9]  p0[8]   p1[13]   p1[2]   p2[7]   p2[9]
 */
#define RTC1_SS2_GPIN1SEL                       0x4
/*! Sensor Strobe's GP Input Sampling Mux SS 2 GPIO Pin 0*/
#define RTC1_SS2_GPIN0SEL                       0x3
/*! Sensor Strobe's GP Input Sampling Mux SS 1 GPIO Pin 2*/
#define RTC1_SS1_GPIN2SEL                       0x2
/*! Sensor Strobe's GP Input Sampling Mux SS 1 GPIO Pin 1*/
#define RTC1_SS1_GPIN1SEL                       0x1
/*! Sensor Strobe's GP Input Sampling Mux SS 1 GPIO Pin 0*/
#define RTC1_SS1_GPIN0SEL                       0x0
/*! Sensor Strobe's GP Input Sampling Mux SS 3 GPIO Pin 2*/
#define RTC1_SS3_GPIN2SEL                       0x0
/*! Sensor Strobe's GP Input Sampling Mux SS 3 GPIO Pin 1*/
#define RTC1_SS3_GPIN1SEL                       0x7
/*! Sensor Strobe's GP Input Sampling Mux SS 3 GPIO Pin 0*/
#define RTC1_SS3_GPIN0SEL                       0x6
/*! Sensor Strobe's GP Input Sampling Mux SS 2 GPIO Pin 2*/
#define RTC1_SS2_GPIN2SEL                       0x5

/*! Differential output option for Sensor Strobe channel 3.
 *  Sensor Strobe channel3 is used as differential signal, actual RTC_SS3 out 
 *  for this channel is available in corresponding GPIO. 
 *  RTC_SS4 of Sensor Strobe channel 4 is used to provided inverted signal of RTC_SS3.
 */
#define RTC1_SS3_DIFFOUT                           0
/*! Differential output option for Sensor Strobe channel 1.
 *  Sensor Strobe channel 1 is used as differential signal, actual RTC_SS1 out 
 *  for this channel is available in corresponding GPIO. 
 *  RTC_SS1 of Sensor Strobe channel 2 is used to provided inverted signal of RTC_SS1.
 */
#define RTC1_SS1_DIFFOUT                           0


/*! @} */

/*! @} */
#endif /* ADI_RTC_CONFIG_H__ */
