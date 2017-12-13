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
#include "tztrng_defs.h"
#include "tztrng_pal.h"

static CCError_t LLF_RND_TRNG_ReadEhrData(uint32_t *pSourceOut)
{
    CCError_t error = CC_OK;
    uint32_t i;

    error = LLF_RND_TRNG_REQUIRED_ROSCS_NOT_ALLOWED_ERROR;

    /* wait RNG interrupt: isr signals error bits */
    error = CC_HalWaitInterrupt();

    /* in case of AUTOCORR_ERR or RNG_WATCHDOG, keep the default error value. will try the next ROSC. */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), 0xFFFFFFFF);

    if (error == CC_OK) {
        for (i = 0; i < LLF_RND_HW_TRNG_EHR_WIDTH_IN_WORDS; i++)
        {
            /* load the current random data to the output buffer */
            *(pSourceOut++) = CC_HAL_READ_REGISTER(DX_EHR_DATA_0_REG_OFFSET + (i*sizeof(uint32_t)));
            TRNG_EHR_DUMP("EHR[%d][0x%x].\n", i, *(pSourceOut - 1));
        }
        CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG, RNG_ISR));
    }

    return error;
}

/****************************************************************************************/
/*****************************       Public Functions      ******************************/
/****************************************************************************************/
CCError_t LLF_RND_StartTrngHW(
        CCRndState_t  *rndState_ptr,
        CCRndParams_t *trngParams_ptr,
        CCBool_t           isRestart,
        uint32_t         *roscsToStart_ptr)
{
    /* LOCAL DECLARATIONS */
    CCError_t error = CC_OK;
    uint32_t tmpSamplCnt = 0;
    uint32_t roscNum = 0;

    /* Check pointers */
    if ((rndState_ptr == NULL) || (trngParams_ptr == NULL) ||
            (roscsToStart_ptr == NULL))
            return LLF_RND_TRNG_ILLEGAL_PTR_ERROR;

    /*--------------------------------------------------------------*/
    /* 1. If full restart, get semaphore and set initial ROSCs      */
    /*--------------------------------------------------------------*/
    if (isRestart) {
            /* set ROSC to 1 (fastest)  */
            *roscsToStart_ptr = 1UL;

            /* init rndState flags to zero */
            rndState_ptr->TrngProcesState = 0;
    }

    if (*roscsToStart_ptr == 0)
            return LLF_RND_TRNG_REQUIRED_ROSCS_NOT_ALLOWED_ERROR;

    /* FE mode  */
    /* Get fastest allowed ROSC */
    error = LLF_RND_GetFastestRosc(
            trngParams_ptr,
            roscsToStart_ptr /*in/out*/);
    if (error)
        return error;

    error = LLF_RND_GetRoscSampleCnt(*roscsToStart_ptr, trngParams_ptr);
    if (error)
        return error;

    roscNum = LLF_RND_TRNG_RoscMaskToNum(*roscsToStart_ptr);

    /*--------------------------------------------------------------*/
    /* 2. Restart the TRNG and set parameters                   */
    /*--------------------------------------------------------------*/
    /* RNG Block HW Specification (10 Programming Reference)        */

    /* do software reset */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_SW_RESET), 0x1);
    /* in order to verify that the reset has completed the sample count need to be verified */
    do {
        /* set sampling ratio (rng_clocks) between consecutive bits */
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, SAMPLE_CNT1), trngParams_ptr->SubSamplingRatio);

        /* read the sampling ratio  */
        tmpSamplCnt = CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG, SAMPLE_CNT1));

    } while (tmpSamplCnt != trngParams_ptr->SubSamplingRatio);


    /* disable the RND source for setting new parameters in HW */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RND_SOURCE_ENABLE), LLF_RND_HW_RND_SRC_DISABLE_VAL);

    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), 0xFFFFFFFF);

    /* set interrupt mask */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_IMR), LLF_RNG_INT_MASK_ON_FETRNG_MODE);

    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, TRNG_CONFIG), roscNum);

    /* Debug Control register: set to 0 - no bypasses */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, TRNG_DEBUG_CONTROL), LLF_RND_HW_DEBUG_CONTROL_VALUE_ON_FE_MODE);

    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RND_SOURCE_ENABLE), LLF_RND_HW_RND_SRC_ENABLE_VAL);

    /* set indication about current started ROSCs:  */
    /*new started*/
    rndState_ptr->TrngProcesState = (rndState_ptr->TrngProcesState & 0x00ffffff) | (*roscsToStart_ptr << 24);
    /*total started*/
    rndState_ptr->TrngProcesState |= (*roscsToStart_ptr << 8);

    return error;
}

CCError_t LLF_RND_GetTrngSource(
        CCRndState_t  *rndState_ptr,        /*in/out*/
        CCRndParams_t  *trngParams_ptr,     /*in/out*/
        CCBool_t            isContinued,    /*in*/
        uint32_t        **sourceOut_ptr_ptr,    /*out*/
        uint32_t         *sourceOutSize_ptr,    /*in/out*/
        uint32_t         *rndWorkBuff_ptr)      /*in*/
{
    /* The return error identifier */
    CCError_t error = CC_OK;
    int32_t  i;
    uint32_t roscToStart;
    uint32_t *ramAddr;
    uint32_t trngBuff[LLF_RND_HW_DMA_EHR_SAMPLES_NUM_ON_FE_MODE * LLF_RND_HW_TRNG_EHR_WIDTH_IN_WORDS] = { 0 }; /* 2 EHR required */

    CC_UNUSED_PARAM(isContinued);

    /* Set source RAM address with offset 8 bytes from sourceOut address in
      order to remain empty bytes for CC operations */
    *sourceOut_ptr_ptr = rndWorkBuff_ptr;
    ramAddr = *sourceOut_ptr_ptr;
    /* init to 0 for FE mode */
    *sourceOutSize_ptr = 0;

    /* Full restart TRNG */
    error = LLF_RND_StartTrngHW(
            rndState_ptr,
            trngParams_ptr,
            CC_TRUE,        /*isRestart*/
            &roscToStart);

    /*Note: in case of error the TRNG HW is still not started*/
    if (error) {
        TRNG_LOG_DEBUG("LLF_RND_StartTrngHW() error[%d]\n", error);
        goto End;
    }

    /*====================================================*/
    /*====================================================*/
    /*         Processing after previous start            */
    /*====================================================*/
    /*====================================================*/

    /*====================================================*/
    /* FE mode processing: start Roscs sequentially -     */
    /* from fast to slow Rosc                             */
    /*====================================================*/

    for (i = 0; i < LLF_RND_NUM_OF_ROSCS; ++i) {

        /* read the first EHR */
        error = LLF_RND_TRNG_ReadEhrData(trngBuff);
        if (error == CC_OK) {
            /* read the second EHR */
            error = LLF_RND_TRNG_ReadEhrData(trngBuff + LLF_RND_HW_TRNG_EHR_WIDTH_IN_WORDS);
            if (error == CC_OK) {
                break;
            }
        }

        if (error != CC_OK) {
            /* try next rosc */
            /* if no remain roscs to start, return error */
            if (roscToStart == 0x8) {
                error = LLF_RND_TRNG_GENERATION_NOT_COMPLETED_ERROR;
               break;
            }
            else {
                /* Call StartTrng, with next ROSC */
                roscToStart <<= 1;
                TRNG_LOG_DEBUG("process with next rosc[%d]\n", roscToStart);
                error = LLF_RND_StartTrngHW(
                        rndState_ptr,
                        trngParams_ptr,
                        CC_FALSE,   /*isRestart*/
                        &roscToStart);

                /* if no remain valid roscs, return error */
                if (error == LLF_RND_TRNG_REQUIRED_ROSCS_NOT_ALLOWED_ERROR && (trngParams_ptr->RoscsAllowed != 0)) {
                    error = LLF_RND_TRNG_GENERATION_NOT_COMPLETED_ERROR;
                }

                if (error != CC_OK) {
                    TRNG_LOG_DEBUG("LLF_RND_StartTrngHW() error[%d]\n", error);
                    goto End;
                }
            }
        }

        /* update total processed ROSCs */
        rndState_ptr->TrngProcesState |= ((rndState_ptr->TrngProcesState >> 8) & 0x00FF0000);
        /*clean started & not processed*/
        rndState_ptr->TrngProcesState &= 0x00FFFFFF;

    }

    /* TODO: donot need to allocate a local trngbuf here */
    if (error == CC_OK) {
        tztrng_memcpy((uint8_t *)ramAddr, (uint8_t *)trngBuff, LLF_RND_HW_DMA_EHR_SAMPLES_NUM_ON_FE_MODE * LLF_RND_HW_TRNG_EHR_WIDTH_IN_BYTES);
        *sourceOutSize_ptr = LLF_RND_HW_DMA_EHR_SAMPLES_NUM_ON_FE_MODE * LLF_RND_HW_TRNG_EHR_WIDTH_IN_BYTES;
    }

    /* end FE mode */
    /* ................. end of function ..................................... */
    /* ----------------------------------------------------------------------- */
End:

    /* turn the RNG off */
    LLF_RND_TurnOffTrng();

    return error;
} /* END of LLF_RND_GetTrngSource */

CCError_t LLF_RND_RunTrngStartupTest(
        CCRndState_t        *rndState_ptr,
        CCRndParams_t       *trngParams_ptr,
        uint32_t                *rndWorkBuff_ptr)
{
    CCError_t error = CC_OK;

    CC_UNUSED_PARAM(rndState_ptr);
    CC_UNUSED_PARAM(trngParams_ptr);
    CC_UNUSED_PARAM(rndWorkBuff_ptr);

    return error;
}

