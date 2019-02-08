/***************************************************************************//**
* \file cyip_headers.h
*
* \brief
* Common header file to be included by all IP definition headers
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#ifndef _CYIP_HEADERS_H_
#define _CYIP_HEADERS_H_

#include <stdint.h>

#if ((defined(__GNUC__)        && (__ARM_ARCH == 6) && (__ARM_ARCH_6M__ == 1)) || \
     (defined(__ICCARM__)      && (__CORE__ == __ARM6M__)) || \
     (defined(__ARMCC_VERSION) && (__TARGET_ARCH_THUMB == 3)) || \
     (defined(__ghs__)         && defined(__CORE_CORTEXM0PLUS__)))
#include "core_cm0plus.h"
#else
#include "core_cm4.h"
#endif

#endif /* _CYIP_HEADERS_H_ */


/* [] END OF FILE */
