/*!
 *****************************************************************************
 * @file:    adi_version.h
 * @brief:   Version macros for ADI ADuCMxxx Device Series
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2016 Analog Devices, Inc.

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

#ifndef __ADI_VERSION_H__
#define __ADI_VERSION_H__

/* use a 32-bit versioning scheme that supports numerical compares */
#define ADI_VERSION_MAJOR     1u     /* must be <= 255 */
#define ADI_VERSION_MINOR     0u     /* must be <= 255 */
#define ADI_VERSION_BUILD     0u     /* must be <= 255 */
#define ADI_VERSION_PATCH     0u     /* must be <= 255 */

#define ADI_CONSTRUCT_VERSION(a,b,c,d)  (((a) << 24u) | ((b) << 16u) | ((c) << 8u) | (d))

/* known versions */
#define ADI_VERSION_1_0_0_0   ADI_CONSTRUCT_VERSION(1u,0u,0u,0u)

/* test current version against known predefines (see SystemInit() example in system.c) */
#define ADI_VERSION_CURRENT   ADI_CONSTRUCT_VERSION(ADI_VERSION_MAJOR, ADI_VERSION_MINOR, ADI_VERSION_BUILD, ADI_VERSION_PATCH)

#endif /* __ADI_VERSION_H__ */
