/***************************************************************************//**
 * @file pti.h
 * @brief This header file contains information for working with the packet
 * trace APIs.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __RADIO_PTI_H
#define __RADIO_PTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "em_gpio.h"

/********************************  TYPEDEFS   *********************************/

/** Channel type enumeration. */
typedef enum RADIO_PTIMode
{
  /** SPI mode. */
  RADIO_PTI_MODE_SPI = 0U,
  /** UART mode. */
  RADIO_PTI_MODE_UART = 1U,
  /** 9bit UART mode. */
  RADIO_PTI_MODE_UART_ONEWIRE = 2U,
  /** Turn PTI off entirely */
  RADIO_PTI_MODE_DISABLED = 3U,
} RADIO_PTIMode_t;

/** 
 * @struct RADIO_PTIInit_t
 * @brief Configuration structure for the packet trace interface (PTI)
 */
typedef struct RADIO_PTIInit {
  /** Packet Trace mode (UART or SPI) */
  RADIO_PTIMode_t mode;

  /** Output baudrate for PTI in Hz */
  uint32_t baud;

  /** Data output (DOUT) location for pin/port */
  uint8_t doutLoc;
  /** Data output (DOUT) GPIO port */
  GPIO_Port_TypeDef doutPort;
  /** Data output (DOUT) GPIO pin */
  uint8_t doutPin;

  /** Data clock (DCLK) location for pin/port. Only used in SPI mode */
  uint8_t dclkLoc;
  /** Data clock (DCLK) GPIO port. Only used in SPI mode */
  GPIO_Port_TypeDef dclkPort;
  /** Data clock (DCLK) GPIO pin. Only used in SPI mode */
  uint8_t dclkPin;

  /** Data frame (DFRAME) location for pin/port. Only used for  */
  uint8_t dframeLoc;
  /** Data frame (DFRAME) GPIO port */
  GPIO_Port_TypeDef dframePort;
  /** Data frame (DFRAME) GPIO pin */
  uint8_t dframePin;
} RADIO_PTIInit_t;

/*************************  FUNCTION PROTOTYPES   *****************************/
void RADIO_PTI_Init(RADIO_PTIInit_t *pitInit);
void RADIO_PTI_Enable(void);
void RADIO_PTI_Disable(void);

#ifdef __cplusplus
}
#endif

#endif //__RADIO_PTI_H
