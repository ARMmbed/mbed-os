/**************************************************************************//**
 * @file efm32gg11b_perpriv_register.h
 * @brief EFM32GG11B_PERPRIV_REGISTER register and bit field definitions
 * @version 5.3.2
 ******************************************************************************
 * # License
 * <b>Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com</b>
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

#if defined(__ICCARM__)
#pragma system_include       /* Treat file as system include file. */
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang system_header  /* Treat file as system include file. */
#endif

/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @defgroup EFM32GG11B_PERPRIV_REGISTER Peripheral Privilege Register
 * @{
 *****************************************************************************/
/** PERPRIV_REGISTER Register Declaration */
typedef struct {
  /* Note! Use of double __IOM (volatile) qualifier to ensure that both */
  /* pointer and referenced memory are declared volatile. */
  __IOM int TIMER0         : 1;
  __IOM int TIMER1         : 1;
  __IOM int TIMER2         : 1;
  __IOM int TIMER3         : 1;
  __IOM int TIMER4         : 1;
  __IOM int TIMER5         : 1;
  __IOM int TIMER6         : 1;
  __IOM int WTIMER0        : 1;
  __IOM int WTIMER1        : 1;
  __IOM int WTIMER2        : 1;
  __IOM int WTIMER3        : 1;
  __IOM int USART0         : 1;
  __IOM int USART1         : 1;
  __IOM int USART2         : 1;
  __IOM int USART3         : 1;
  __IOM int USART4         : 1;
  __IOM int USART5         : 1;
  __IOM int UART0          : 1;
  __IOM int UART1          : 1;
  __IOM int CAN0           : 1;
  __IOM int CAN1           : 1;
  __IOM int RESERVED0      : 1; /**< Reserved for future use **/
  __IOM int RESERVED1      : 1; /**< Reserved for future use **/
  __IOM int TRNG0          : 1;
  __IOM int MSC            : 1;
  __IOM int EBI            : 1;
  __IOM int ETH            : 1;
  __IOM int LDMA           : 1;
  __IOM int FPUEH          : 1;
  __IOM int GPCRC          : 1;
  __IOM int QSPI0          : 1;
  __IOM int USB            : 1;
  __IOM int SMU            : 1;

  __IOM int ACMP0          : 1;
  __IOM int ACMP1          : 1;
  __IOM int ACMP2          : 1;
  __IOM int ACMP3          : 1;
  __IOM int I2C0           : 1;
  __IOM int I2C1           : 1;
  __IOM int I2C2           : 1;
  __IOM int ADC0           : 1;
  __IOM int ADC1           : 1;
  __IOM int CRYOTIMER      : 1;
  __IOM int VDAC0          : 1;
  __IOM int IDAC0          : 1;
  __IOM int CSEN           : 1;
  __IOM int RESERVED2      : 1; /**< Reserved for future use **/
  __IOM int APB_RSYNC_COMB : 1;
  __IOM int GPIO           : 1;
  __IOM int PRS            : 1;
  __IOM int EMU            : 1;
  __IOM int RMU            : 1;
  __IOM int CMU            : 1;

  __IOM int PCNT0          : 1;
  __IOM int PCNT1          : 1;
  __IOM int PCNT2          : 1;
  __IOM int LEUART0        : 1;
  __IOM int LEUART1        : 1;
  __IOM int LETIMER0       : 1;
  __IOM int LETIMER1       : 1;
  __IOM int WDOG0          : 1;
  __IOM int WDOG1          : 1;
  __IOM int LESENSE        : 1;
  __IOM int LCD            : 1;
  __IOM int RTC            : 1;
  __IOM int RTCC           : 1;
} PERPRIV_REGISTER_TypeDef;      /**< @} */

/** @} End of group Parts */
