/**
 ******************************************************************************
 * @file error.h
 * @brief Defines an error data type, which can be used for error codes or return values.
 * @internal
 * @author ON Semiconductor
 * $Rev: 2074 $
 * $Date: 2013-07-10 18:06:15 +0530 (Wed, 10 Jul 2013) $
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
 * @ingroup util
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <stdint.h>

typedef uint8_t error;
#define NO_ERROR		(0xFF)

#endif /* ERROR_H_ */
