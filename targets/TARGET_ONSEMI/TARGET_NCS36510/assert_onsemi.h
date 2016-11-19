/**
 ******************************************************************************
 * @file assert.h
 * @brief Defines an assertion for debugging purposes.
 * @internal
 * @author ON Semiconductor
 * $Rev: 3823 $
 * $Date: 2015-10-23 16:21:37 +0530 (Fri, 23 Oct 2015) $
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
 * While debugging, the ASSERT macro can be used to verify the expected behaviour
 * of the source code. If the condition that is passed as a parameter to the ASSERT
 * macro evaluates to False, execution stops.
 *
 * The user has the possibility to hook into the assertion through the assertCallback
 * callback function. Note though that the callback function must not use any
 * functionality of the RTOS, or rely on interrupts being called. Once the function
 * returns, it's done.
 *
 * @ingroup debug
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG

/** Executes when an assertion condition evaluates to false.
 * @param filename The name of the current file (normally the __FILE__ macro).
 * @param line The current line number (normally the __LINE__ macro).
 */
void fOnAssert(const char *filename, unsigned int line);

/** Can be assigned to hook into the assertion. */
extern void (*assertCallback)(const char *filename, unsigned int line);

#define ASSERT(test)        ((test) ? (void)0 : fOnAssert(__FILE__, __LINE__))

#define VERIFY(test)        ASSERT(test)

#else

#define ASSERT(test)        ((test) ? (void)0 : 1)

#define VERIFY(test)        ((void)(test))

#endif // DEBUG

#ifdef __cplusplus
}
#endif

#endif /* ASSERT_H_ */
