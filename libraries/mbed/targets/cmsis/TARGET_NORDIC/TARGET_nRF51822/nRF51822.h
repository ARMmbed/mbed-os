/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#ifndef NRF_H
#define NRF_H

//#ifndef _WIN32

#include "compiler_abstraction.h"

/* Family selection for main includes. NRF51 must be selected. */
//#ifdef NRF51
    #include "nrf51.h"
    #include "nrf51_bitfields.h"
 //   #include "nrf51_deprecated.h"
//#else
//    #error "Device family must be defined. See nrf.h."
//#endif /* NRF51 */

//#endif /* _WIN32 */

#endif /* NRF_H */

