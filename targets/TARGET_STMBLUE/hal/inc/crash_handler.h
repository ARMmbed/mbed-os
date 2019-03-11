/*
 * Copyright (c) 2019 STMicroelectronics. All Rights Reserved.
 *
 * The information contained herein is property of STMicroelectronics.
 * Terms and conditions of usage are described in detail in SOFTWARE
 * LICENSE AGREEMENT:
 *
 * (SLA0068 - PRODUCTION LIMITED LICENSE AGREEMENT FOR ST MATERIALS)
 *
 * You may obtain a copy of the License here:
 * LICENSE-SLA0068.txt and at STMicroelectronics website.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 */

#ifndef __CRASH_HANDLER_H_
#define __CRASH_HANDLER_H_

#include <stdint.h>
#include <string.h>

/**
 * @brief Number of word for the Exception RAM Locations 
 */
#define NMB_OF_EXCEP_RAM_WORD 10

/**
 * @brief Base address to store the Crash Information
 */
#define CRASH_RAM_SIZE          40

/**
 * @brief Signature Information: CRASH_SIGNATURE_BASE
 */
#define CRASH_SIGNATURE_BASE     0xBCEC0000
/**
 * @brief Signature Information: ASSERT_SIGNATURE
 */
#define ASSERT_SIGNATURE     (CRASH_SIGNATURE_BASE + 0)
/**
 * @brief Signature Information: NMI_SIGNATURE
 */
#define NMI_SIGNATURE        (CRASH_SIGNATURE_BASE + 1)
/**
 * @brief Signature Information: HARD_FAULT_SIGNATURE
 */
#define HARD_FAULT_SIGNATURE (CRASH_SIGNATURE_BASE + 2)
/**
 * @brief Signature Information: WATCHDOG_SIGNATURE
 */
#define WATCHDOG_SIGNATURE   (CRASH_SIGNATURE_BASE +3)

/**
 * @brief Typedef for the overall crash information (stack pointer, programm counter, registers, ...)
 */
typedef PACKED(struct) crash_infoS {
  uint32_t signature;
  uint32_t SP;
  uint32_t R0;
  uint32_t R1;
  uint32_t R2;
  uint32_t R3;
  uint32_t R12;
  uint32_t LR;
  uint32_t PC;
  uint32_t xPSR;
} crash_info_t;


extern crash_info_t crash_info_ram;
  
/**
 * @brief Macro to store in RAM the register information where an assert is verified.
 *
 * All the information stored are words (32 bit):
 * - Assert Signature
 * - SP
 * - R0
 * - R1
 * - R2
 * - R3
 * - R12
 * - LR
 * - PC
 * - xPSR
 */
#define ASSERT_HANDLER(expression) {                                              \
  volatile uint32_t * crash_info = (volatile uint32_t *)&crash_info_ram;  \
  register uint32_t reg_content;                                                  \
  if (!(expression)) {                                                            \
   /* Init to zero the crash_info RAM locations */                                \
   for (reg_content=0; reg_content<NMB_OF_EXCEP_RAM_WORD; reg_content++) {        \
     *crash_info = 0;                                                             \
     crash_info++;                                                                \
   }                                                                              \
   crash_info = (volatile uint32_t *)&crash_info_ram;                     \
   /* Store Crash Signature */                                                    \
   *crash_info = ASSERT_SIGNATURE;                                                \
   crash_info+=2;                                                                 \
   /* Store R0 register */                                                        \
   __ASM volatile ("MOV %0, R0" : "=r" (reg_content) );                           \
   *crash_info = reg_content;                                                     \
   crash_info--;                                                                  \
   /* Store SP register */                                                        \
   __ASM volatile ("MOV %0, SP" : "=r" (reg_content) );                           \
   *crash_info = reg_content;                                                     \
   crash_info+=2;                                                                 \
   /* Store R1 register */                                                        \
   __ASM volatile ("MOV %0, R1" : "=r" (reg_content) );                           \
   *crash_info = reg_content;                                                     \
   crash_info++;                                                                  \
   /* Store R2 register */                                                        \
   __ASM volatile ("MOV %0, R2" : "=r" (reg_content) );                           \
   *crash_info = reg_content;                                                     \
   crash_info++;                                                                  \
   /* Store R3 register */                                                        \
   __ASM volatile ("MOV %0, R3" : "=r" (reg_content) );                           \
   *crash_info = reg_content;                                                     \
   crash_info++;                                                                  \
   /* Store R12 register */                                                       \
   __ASM volatile ("MOV %0, R12" : "=r" (reg_content) );                          \
   *crash_info = reg_content;                                                     \
   crash_info++;                                                                  \
   /* Store LR register */                                                        \
   __ASM volatile ("MOV %0, LR" : "=r" (reg_content) );                           \
   *crash_info = reg_content;                                                     \
   crash_info++;                                                                  \
   /* Store PC register */                                                        \
   __ASM volatile ("MOV %0, PC" : "=r" (reg_content) );                           \
   *crash_info = reg_content;                                                     \
   crash_info++;                                                                  \
   /* Store xPSR register */                                                      \
   __ASM volatile ("MRS %0, PSR" : "=r" (reg_content) );                          \
   *crash_info = reg_content;                                                     \
   crash_info++;                                                                  \
  }                                                                               \
}                                                                                 \

#endif /* __CRASH_HANDLER_H_ */
