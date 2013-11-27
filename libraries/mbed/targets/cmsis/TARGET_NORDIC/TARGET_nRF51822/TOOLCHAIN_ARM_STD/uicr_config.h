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



/* Template files (including this one) are application specific and therefore expected to 
   be copied into the application project folder prior to its use! */

#ifndef _UICR_CONFIG_H
#define _UICR_CONFIG_H

/*lint ++flb "Enter library region" */

#include <stdint.h>

/* 
 * Include this file in your project if you want to include in your compiled code files data 
 * for the User Information Configuration Registers (UICR) area; see nRF51 Series Reference 
 * Manual chapter User Information Configuration Registers. This file declares one variable
 * per register of the UICR area and informs the linker where to place them. To include 
 * the desired value in the desired address, uncomment the variable with the proper address
 * at the target area and update the assignment value.
 *
 * Please note that UICR values are stored in a reserved area of the flash and should only be
 * stored into when downloading a hex file. Do not use these defined variables to store data 
 * at run time.
 * 
 * Note as well that this file uses one non-standard attribute ("at"). It will only function 
 * with the ARMCC compiler toolset.
 *
 * Note that the hex file generated when this file is included will fail to download when using
 * the standard download algorithm provided by Nordic. See example project "uicr_config_example" 
 * in any of the board example folders for an example of the recommended download method as well
 * as the documentation that follows with the SDK. nrfjprog can be used as normal.
 *
 * Please note as well that if you are using a SoftDevice the UICR_CLENR0 address will
 * already be in use. Do not uncomment that line.
 */

// const uint32_t UICR_CLENR0    __attribute__((at(0x10001000))) __attribute__((used)) = 0xFFFFFFFF;   // WARNING: This address might be used by the SoftDevice. Use with care.
// const uint32_t UICR_RBPCONF   __attribute__((at(0x10001004))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_XTALFREQ  __attribute__((at(0x10001008))) __attribute__((used)) = 0xFFFFFFFF;

// const uint32_t UICR_ADDR_0x80 __attribute__((at(0x10001080))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0x84 __attribute__((at(0x10001084))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0x88 __attribute__((at(0x10001088))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0x8C __attribute__((at(0x1000108C))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0x90 __attribute__((at(0x10001090))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0x94 __attribute__((at(0x10001094))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0x98 __attribute__((at(0x10001098))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0x9C __attribute__((at(0x1000109C))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xA0 __attribute__((at(0x100010A0))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xA4 __attribute__((at(0x100010A4))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xA8 __attribute__((at(0x100010A8))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xAC __attribute__((at(0x100010AC))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xB0 __attribute__((at(0x100010B0))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xB4 __attribute__((at(0x100010B4))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xB8 __attribute__((at(0x100010B8))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xBC __attribute__((at(0x100010BC))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xC0 __attribute__((at(0x100010C0))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xC4 __attribute__((at(0x100010C4))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xC8 __attribute__((at(0x100010C8))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xCC __attribute__((at(0x100010CC))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xD0 __attribute__((at(0x100010D0))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xD4 __attribute__((at(0x100010D4))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xD8 __attribute__((at(0x100010D8))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xDC __attribute__((at(0x100010DC))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xE0 __attribute__((at(0x100010E0))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xE4 __attribute__((at(0x100010E4))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xE8 __attribute__((at(0x100010E8))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xEC __attribute__((at(0x100010EC))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xF0 __attribute__((at(0x100010F0))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xF4 __attribute__((at(0x100010F4))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xF8 __attribute__((at(0x100010F8))) __attribute__((used)) = 0xFFFFFFFF;
// const uint32_t UICR_ADDR_0xFC __attribute__((at(0x100010FC))) __attribute__((used)) = 0xFFFFFFFF;

/*lint --flb "Leave library region" */

#endif //_UICR_CONFIG_H
