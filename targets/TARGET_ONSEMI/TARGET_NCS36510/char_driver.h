/**
 ******************************************************************************
 * @file char_driver.h
 * @brief Defines a character driver data type.
 * @internal
 * @author ON Semiconductor
 * $Rev: 2607 $
 * $Date: 2013-12-06 18:02:43 +0530 (Fri, 06 Dec 2013) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @details
 * The character driver is intended for devices that allow read and write
 * operations with "streams" of data, such as UART devices, SPI or I2c, etc.
 *
 * The character driver derives from the generic driver template (see driver.h).
 * It does so by including an element of the generic driver_t type.
 *
 * The driver defines blocking and non-blocking read and write operations. It is
 * up to the driver implementation to decide which of these to actually implement.
 *
 * @ingroup char_drivers
 */

#ifndef CHAR_DRIVER_H_
#define CHAR_DRIVER_H_

#include "driver.h"

#define DRV_NO_ERROR       (True)
#define DRV_ERROR          (False)

/** A character driver structure. */
typedef struct char_driver {
    /** The parent generic driver. */
    driver_t driver;

    /** Blocking read into a buffer.
     * @param device The device to read from.
     * @param buf The buffer to read into.
     * @param len The number of bytes to read.
     */
    uint8_t (*read_b)(device_pt device, uint8_t *const buf, uint32_t len);

    /** Non-blocking read into a buffer.
     * @param device The device to read from.
     * @param buf The buffer to read into.
     * @param len The maximum number of bytes to read; typically the size of the buffer.
     * @return The number of bytes actually read.
     */
    uint32_t (*read_nb)(device_pt device, uint8_t *const buf, uint32_t len);

    /** Blocking write from a buffer.
     * @param device The device to write to.
     * @param buf The buffer to read from.
     * @param len The number of bytes to write; typically the size of the buffer.
     * @return success or error message
     */
    uint8_t (*write_b)(device_pt device, const uint8_t *buf, uint32_t len);

    /** Non-blocking write from a buffer.
     * @param device The device to write to.
     * @param buf The buffer to read from.
     * @param len The number of bytes to write; typically the size of the buffer.
     * @return success or error message
     */
    uint8_t (*write_nb)(device_pt device, const uint8_t *buf, uint32_t len);
} char_driver_t, *char_driver_pt;

#endif /* CHAR_DRIVER_H_ */
