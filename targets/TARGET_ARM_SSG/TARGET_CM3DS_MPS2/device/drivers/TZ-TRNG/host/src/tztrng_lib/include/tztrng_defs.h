/******************************************************************************
* Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
* SPDX-License-Identifier: Apache-2.0                                         *
*                                                                             *
* Licensed under the Apache License, Version 2.0 (the "License");             *
* you may not use this file except in compliance with the License.            *
*                                                                             *
* You may obtain a copy of the License at                                     *
* http://www.apache.org/licenses/LICENSE-2.0                                  *
*                                                                             *
* Unless required by applicable law or agreed to in writing, software         *
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
*                                                                             *
* See the License for the specific language governing permissions and         *
* limitations under the License.                                              *
******************************************************************************/
#ifndef _TZTRNG_DEFS_H_
#define _TZTRNG_DEFS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dx_reg_base_host.h"
#include "dx_rng.h"

#if (CC_CONFIG_TRNG_MODE == 1)
#include "config_trng90b.h"
#elif (CC_CONFIG_TRNG_MODE == 0)
#include "config_fetrng.h"
#else
#error "invalid VECT definition in Makefile"
#endif

#define CC_OK   0
#define CC_UNUSED_PARAM(prm)  ((void)prm)
#define CC_MAX_UINT32_VAL 	(0xFFFFFFFF)
#define LLF_RND_NUM_OF_ROSCS        4
#define TRNG_EHR_SIZE 6

/* TRNG Error definitions */
#define TRNG_MAX_CRNGT_ERRORS       (12)
#define TRNG_MAX_VN_ERRORS          (50)

extern unsigned long gCcRegBase;

typedef uint32_t CCError_t;
typedef uint32_t CCBool_t;

typedef enum {
    CC_FALSE = 0,
    CC_TRUE = 1
} CCBool;

/*
Requirements:
- Required entropy = 384 bits

Default values for Zynq FPGA:
- entropy per bit = 0.5
*/

/* amount of bytes for the required entropy bits = ROUND_UP(ROUND_UP(((required entropy bits)/(entropy per bit)), 1024), (EHR width in bytes)) / 8
   (multiple of the window size 1024 bits and multiple of the EHR width 192 bits) */
#define CC_CONFIG_TRNG90B_AMOUNT_OF_BYTES                      144  /* ROUND_UP(ROUND_UP((384/0.5), 1024), 192) / 8 = 144 */

/*** NIST SP 800-90B (2nd Draft) 4.4.1 ***/
/* C = ROUND_UP(1+(-log(W)/H)), W = 2^(-40), H=(entropy per bit) */
#define CC_CONFIG_TRNG90B_REPETITION_COUNTER_CUTOFF            81  /* ROUND_UP(1+(40/0.5)) = 81 */

/*** NIST SP 800-90B (2nd Draft) 4.4.2 ***/
/* C =CRITBINOM(W, power(2,(-H)),1-a), W = 1024, a = 2^(-40), H=(entropy per bit) */
#define CC_CONFIG_TRNG90B_ADAPTIVE_PROPORTION_CUTOFF           823      /* =CRITBINOM(1024, power(2,(-0.5)),1-2^(-40)) */

/*** For Startup Tests ***/
/* amount of bytes for the startup test = 528 (at least 4096 bits (NIST SP 800-90B (2nd Draft) 4.3.12) = 22 EHRs = 4224 bits) */
#define CC_CONFIG_TRNG90B_AMOUNT_OF_BYTES_STARTUP              528
/* Definitions of temp buffer for DMA */

#define CC_RND_WORK_BUFFER_SIZE_WORDS CC_CONFIG_TRNG90B_AMOUNT_OF_BYTES_STARTUP/4 /* the maximum possible usage of stack memory in words */

/* The CRYS Random Generator Parameters  structure CCRndParams_t -
   structure containing the user given Parameters */
typedef struct  CCRndParams_t
{
	/* allowed ring oscillator lengths: bits 0,1,2,3  */
	uint32_t  RoscsAllowed;

	/* sampling interval: count of ring oscillator cycles between
	   consecutive bits sampling */
	uint32_t  SubSamplingRatio;
	uint32_t  SubSamplingRatio1;
	uint32_t  SubSamplingRatio2;
	uint32_t  SubSamplingRatio3;
	uint32_t  SubSamplingRatio4;

} CCRndParams_t;

/*! RND state structure. Includes internal data that needs to be saved between boots by the user.*/
typedef  struct
{
	/* State flag (see definition of StateFlag above), containing bit-fields, defining:
	- b'0: instantiation steps:   0 - not done, 1 - done;
	- 2b'9,8: working or testing mode: 0 - working, 1 - KAT DRBG test, 2 -
	  KAT TRNG test;
	b'16: flag defining is Previous random valid or not:
	        0 - not valid, 1 - valid */
	/*! State flag used internally in the code.*/
	uint32_t StateFlag;

	/* Trng processing flag - indicates which ROSC lengths are:
	-  allowed (bits 0-3);
	-  total started (bits 8-11);
	-  processed (bits 16-19);
	-  started, but not processed (bits24-27)              */
	/*! TRNG process state used internally in the code */
	uint32_t TrngProcesState;

} CCRndState_t;

/* Register Offset macro */
#define CC_REG_OFFSET(unit_name, reg_name)               \
	(DX_ ## reg_name ## _REG_OFFSET)

/*!
 * Read TRNG memory-mapped-IO register.
 *
 * \param regOffset The offset of the ARM TRNG register to read
 * \return uint32_t Return the value of the given register
 */
#define CC_HAL_READ_REGISTER(regOffset) 				\
		(*((volatile uint32_t *)(gCcRegBase + (regOffset))))

/*!
 * Write TRNG memory-mapped-IO register.
 * \note This macro must be modified to make the operation synchronous, i.e. the write operation must complete,
 *       and the new value must be written to the register before the macro returns. The mechanisms required to
 *       achieve this are architecture-dependent (e.g., the memory barrier in ARM architecture).
 *
 * \param regOffset The offset of the ARM TRNG register to write
 * \param val The value to write
 */
#define CC_HAL_WRITE_REGISTER(regOffset, val) \
		(*((volatile uint32_t *)(gCcRegBase + (regOffset))) = (val))

#define LLF_RND_HW_TRNG_EHR_WIDTH_IN_WORDS  6
#define LLF_RND_HW_TRNG_EHR_WIDTH_IN_BYTES  (LLF_RND_HW_TRNG_EHR_WIDTH_IN_WORDS * sizeof(uint32_t))
#define LLF_RND_HW_DMA_EHR_SAMPLES_NUM_ON_TRNG90B_MODE  22UL
#define LLF_RND_HW_DEBUG_CONTROL_VALUE_ON_FE_MODE  0x00000000
/* ring oscillator offsets and numbers */
#define LLF_RND_HW_TRNG_ROSC0_BIT       0x1
#define LLF_RND_HW_TRNG_ROSC1_BIT       0x2
#define LLF_RND_HW_TRNG_ROSC2_BIT       0x4
#define LLF_RND_HW_TRNG_ROSC3_BIT       0x8
#define LLF_RND_HW_TRNG_ROSC0_NUM       0x0
#define LLF_RND_HW_TRNG_ROSC1_NUM       0x1
#define LLF_RND_HW_TRNG_ROSC2_NUM       0x2
#define LLF_RND_HW_TRNG_ROSC3_NUM       0x3

#define LLF_RND_HW_RND_SRC_ENABLE_VAL      	1UL
#define LLF_RND_HW_RND_SRC_DISABLE_VAL     	0UL
#define LLF_RND_HW_TRNG_WITH_DMA_CONFIG_VAL    0x4
#define LLF_RNG_INT_MASK_ON_TRNG90B_MODE  0xFFFFFFC5
#define LLF_RNG_INT_MASK_ON_FETRNG_MODE  0xFFFFFFEC

#define LLF_RND_HW_DMA_EHR_SAMPLES_NUM_ON_FE_MODE  2UL /*for both AES128 and AES256*/

/* Logging macros */
#define TRNG_LOG(format, ...) \
    fprintf(stderr, "tztrng:%s: " format , __func__, ##__VA_ARGS__)

#ifdef DEBUG
#define TRNG_LOG_DEBUG(format, ...) TRNG_LOG(format, ##__VA_ARGS__)
#else
#define TRNG_LOG_DEBUG(format, ...) do {} while (0)
#endif

#ifdef TZTRNG_EHR_DUMP
#define TRNG_EHR_DUMP(format, ...) \
    fprintf(stderr, format, ##__VA_ARGS__)
#else
#define TRNG_EHR_DUMP(format, ...) do {} while (0)
#endif

#define CC_ERROR_BASE          0x00F00000UL
#define CC_ERROR_LAYER_RANGE   0x00010000UL
#define CC_ERROR_MODULE_RANGE  0x00000100UL
#define CC_LAYER_ERROR_IDX     0x00UL
#define LLF_LAYER_ERROR_IDX      0x01UL
#define GENERIC_ERROR_IDX        0x05UL
#define RND_ERROR_IDX            0x0CUL
#define CC_RND_MODULE_ERROR_BASE   (CC_ERROR_BASE + \
                                     (CC_ERROR_LAYER_RANGE * CC_LAYER_ERROR_IDX) + \
                                     (CC_ERROR_MODULE_RANGE * RND_ERROR_IDX ) )

/*! Low level RND module error base address -  0x00F10C00. */
#define LLF_RND_MODULE_ERROR_BASE    (CC_ERROR_BASE + \
                                     (CC_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CC_ERROR_MODULE_RANGE * RND_ERROR_IDX ) )
#define LLF_RND_HW_DEBUG_CONTROL_VALUE_ON_TRNG90B_MODE  0x0000000A	/* bypass Von-Neumann balancer and autocorrelation test */
#define LLF_RND_HW_VERSION_NOT_CORRECT_ERROR    	    (LLF_RND_MODULE_ERROR_BASE + 0x0UL)
#define LLF_RND_CPRNG_TEST_FAIL_ERROR			        (LLF_RND_MODULE_ERROR_BASE + 0x1UL)
#define LLF_RND_CRNGT_TEST_FAIL_ERROR			        (LLF_RND_MODULE_ERROR_BASE + 0x2UL)
#define LLF_RND_STATE_PTR_INVALID_ERROR			        (LLF_RND_MODULE_ERROR_BASE + 0x3UL)
#define LLF_RND_AES_256_NOT_SUPPORTED_ERROR             (LLF_RND_MODULE_ERROR_BASE + 0x4UL)

#define LLF_RND_TRNG_TIME_LIMIT_EXCEEDED_ERROR          (LLF_RND_MODULE_ERROR_BASE + 0x20UL)
#define LLF_RND_TRNG_ENTR_ESTIM_SIZE_EXCEED_ERROR       (LLF_RND_MODULE_ERROR_BASE + 0x21UL)

#define LLF_RND_TRNG_PREVIOUS_PARAMS_NOT_MATCH_ERROR    (LLF_RND_MODULE_ERROR_BASE + 0x30UL)
#define LLF_RND_TRNG_REQUIRED_ROSCS_NOT_ALLOWED_ERROR   (LLF_RND_MODULE_ERROR_BASE + 0x31UL)
#define LLF_RND_TRNG_GENERATION_NOT_COMPLETED_ERROR     (LLF_RND_MODULE_ERROR_BASE + 0x32UL)
#define LLF_RND_TRNG_LOW_ENTROPY_ERROR	   	            (LLF_RND_MODULE_ERROR_BASE + 0x33UL)
#define LLF_RND_TRNG_NULL_ENTROPY_ERROR	   	            (LLF_RND_MODULE_ERROR_BASE + 0x34UL)
#define LLF_RND_TRNG_ILLEGAL_PTR_ERROR   	            (LLF_RND_MODULE_ERROR_BASE + 0x35UL)
#define LLF_RND_TRNG_REPETITION_COUNTER_ERROR           (LLF_RND_MODULE_ERROR_BASE + 0x36UL)
#define LLF_RND_TRNG_ADAPTION_PROPORTION_ERROR          (LLF_RND_MODULE_ERROR_BASE + 0x37UL)

#define CC_RND_CPRNG_TEST_FAIL_ERROR		       	    (CC_RND_MODULE_ERROR_BASE + 0x2UL)
#define CC_RND_TRNG_KAT_NOT_SUPPORTED_ERROR             (CC_RND_MODULE_ERROR_BASE + 0x34UL)

void LLF_RND_TurnOffTrng(void);
CCError_t LLF_RND_GetFastestRosc( CCRndParams_t *trngParams_ptr, uint32_t *rosc_ptr/*in/out*/);
CCError_t LLF_RND_GetRoscSampleCnt( uint32_t rosc, CCRndParams_t *pTrngParams);
uint32_t LLF_RND_TRNG_RoscMaskToNum(uint32_t mask);

#define CC_RND_NOT_INSTANTIATED             	0UL
#define CC_RND_INSTANTIATED                	    1UL
#define CC_RND_INSTANTRESEED_AUTOCORR_ERRORS 	2UL
#define CC_RND_INSTANTRESEED_TIME_EXCEED     	4UL
#define CC_RND_INSTANTRESEED_LESS_ENTROPY    	8UL

CCError_t LLF_RND_RunTrngStartupTest(
                 CCRndState_t   *rndState_ptr,      /*in/out*/
                 CCRndParams_t  *trngParams_ptr,    /*out*/
                 uint32_t       *rndWorkBuff_ptr);  /*in*/

CCError_t LLF_RND_GetTrngSource(
				 CCRndState_t   *rndState_ptr,	    /*in/out*/
				 CCRndParams_t  *trngParams_ptr,    /*in/out*/
				 CCBool_t       isContinued,	    /*in*/
				 uint32_t       **sourceOut_ptr_ptr,/*out*/
				 uint32_t       *sourceOutSize_ptr, /*in/out*/
				 uint32_t       *rndWorkBuff_ptr);   /*in*/

#endif

