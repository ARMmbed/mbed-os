/**
 ******************************************************************************
 * @file spi.h
 * @brief Inlcude file of a SPI master driver
 * @internal
 * @author ON Semiconductor
 * @version $Rev:  $
 * @date $Date: 2016-02-05 $
 ******************************************************************************
 * @copyright (c) 2012 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup spi_h
 *
 * @details
 *
 */
#ifndef SPI_H_
#define SPI_H_

#include "device.h"
#include "spi_api.h"

#if DEVICE_SPI

#ifdef __cplusplus
extern "C" {
#endif

/* Miscellaneous I/O and control operations codes */
#define SPI_IPC7207_IOCTL_GET_SLAVE_SELECT	(0x1) /**< <b>Ioctl request code</b>: Reading slaveSelect register */
#define SPI_IPC7207_IOCTL_SET_SLAVE_SELECT	(0x2) /**< <b>Ioctl request code</b>: Setting slaveSelect register */
#define SPI_IPC7207_IOCTL_FLUSH				(0x3) /**< <b>Ioctl request code</b>: Flushin FIFOs and serial shift registers */

extern void fSpiInit(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel);
extern void fSpiClose(spi_t *obj);
extern int fSpiWriteB(spi_t *obj, uint32_t const buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEVICE_SPI */

#endif /* SPI_H_ */
