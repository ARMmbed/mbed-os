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

static CCError_t startTrngHW(
        CCRndState_t  *rndState_ptr,
        CCRndParams_t *trngParams_ptr,
        CCBool_t           isRestart,
        uint32_t          *roscsToStart_ptr,
        CCBool_t           isStartup);
extern CCError_t runContinuousTesting(uint32_t* pData, uint32_t sizeInBytes);

/************************************************************************************/
/**
 * The function checks that parameters, loaded in the TRNG HW
 * are match to parameters, required by trngParams_ptr structures.
 *
 * @author reuvenl (6/25/2012)
 *
 * @param trngParams_ptr
 *
 * @return CCError_t
 */
static CCError_t LLF_RND_TRNG_CheckHwParams(CCRndParams_t *trngParams_ptr)
{
    uint32_t temp;
    CCBool_t isTrue = CC_TRUE;

    /* check Debug control - masked TRNG tests according to mode */
    temp = CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG, TRNG_DEBUG_CONTROL));
    isTrue &= (temp == LLF_RND_HW_DEBUG_CONTROL_VALUE_ON_TRNG90B_MODE);
    /* check samplesCount */
    temp = CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG,SAMPLE_CNT1));
    isTrue &= (temp == trngParams_ptr->SubSamplingRatio);

    /* if any parameters are not match return an Error */
    if (isTrue == CC_FALSE)
        return LLF_RND_TRNG_PREVIOUS_PARAMS_NOT_MATCH_ERROR;
    else
        return CC_OK;
}

static int32_t readEhrData(uint32_t *sample,
                            CCRndState_t  *rndState_ptr,
                            CCRndParams_t *trngParams_ptr,
                            uint32_t *roscsToStart_ptr)
{
    uint32_t i;
    CCError_t err = CC_OK;

    err = startTrngHW(rndState_ptr, trngParams_ptr, CC_FALSE, roscsToStart_ptr, CC_FALSE);
    if (err) {
        TRNG_LOG_DEBUG("startTrngHW() failed..\n");
        return err;
    }

    /* Read 1 EHR of random bits */
    if (CC_HalWaitInterrupt()) {
        TRNG_LOG_DEBUG("CC_HalWaitInterrupt() failed..\n");
        LLF_RND_TurnOffTrng();
        return (-1);
    }

    /* Read EHR into tmp buffer */
    for (i = 0; i < TRNG_EHR_SIZE; i++) {
        uint32_t ehr = CC_HAL_READ_REGISTER(DX_EHR_DATA_0_REG_OFFSET + (i * sizeof(uint32_t)));
        TRNG_EHR_DUMP("EHR[%i][0x%x]\n", i, ehrp);
        sample[i] = ehr;
    }

    return CC_OK;
}

static CCError_t readMultipleEHR(uint32_t inSize, uint8_t *ramAddr,
                                    CCRndState_t  *rndState_ptr,
                                    CCRndParams_t *trngParams_ptr,
                                    uint32_t *roscsToStart_ptr)
{
    uint32_t noise_samples[TRNG_EHR_SIZE];
    CCError_t err = CC_OK;
    uint32_t read_size = TRNG_EHR_SIZE * sizeof(uint32_t);

    while (inSize) {

        LLF_RND_TurnOffTrng();
        err = readEhrData(noise_samples, rndState_ptr, trngParams_ptr, roscsToStart_ptr);
        if (err != CC_OK)
            return err;

        /* Copy the needed amount of bytes to the result buffer */
        tztrng_memcpy(ramAddr, (uint8_t *)noise_samples, read_size);

        inSize -= read_size;
        ramAddr += read_size;
    }

    return CC_OK;
}

static CCError_t startTrngHW(
        CCRndState_t  *rndState_ptr,
        CCRndParams_t *trngParams_ptr,
        CCBool_t           isRestart,
        uint32_t          *roscsToStart_ptr,
        CCBool_t           isStartup)
{
    /* LOCAL DECLARATIONS */

    CCError_t error = CC_OK;
    uint32_t  tmpSamplCnt = 0;
    uint32_t roscNum = 0;

    CC_UNUSED_PARAM(isStartup);

    /* FUNCTION LOGIC */

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

    /* TRNG90B mode  */
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
    /* 2. Restart the TRNG and set  parameters              */
    /*--------------------------------------------------------------*/

    /* in order to verify that the reset has completed the sample count need to be verified */
    do {
        /* set sampling ratio (rng_clocks) between consequtive bits */
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, SAMPLE_CNT1), trngParams_ptr->SubSamplingRatio);

        /* read the sampling ratio  */
        tmpSamplCnt = CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG, SAMPLE_CNT1));

    } while (tmpSamplCnt != trngParams_ptr->SubSamplingRatio);

    /* disable the RND source for setting new parameters in HW */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RND_SOURCE_ENABLE), LLF_RND_HW_RND_SRC_DISABLE_VAL);

    /* set TRNG_CONFIG to choose SOP_SEL = 1 - i.e. EHR output */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, TRNG_CONFIG), roscNum);

    /* Debug Control register: set to 0 - no bypasses */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, TRNG_DEBUG_CONTROL), LLF_RND_HW_DEBUG_CONTROL_VALUE_ON_TRNG90B_MODE);

    /* set interrupt mask */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_IMR), LLF_RNG_INT_MASK_ON_TRNG90B_MODE);

    /* clear RNG interrupts !!TBD */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), 0xFFFFFFFF);

    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RND_SOURCE_ENABLE), LLF_RND_HW_RND_SRC_ENABLE_VAL);

    /* set indication about current started ROSCs:  */
    /*new started*/
    rndState_ptr->TrngProcesState = (rndState_ptr->TrngProcesState & 0x00ffffff) | (*roscsToStart_ptr << 24);
    /*total started*/
    rndState_ptr->TrngProcesState |= (*roscsToStart_ptr << 8);

    /* end  of function */
    return error;
}

static CCError_t getTrngSource(
                CCRndState_t  *rndState_ptr,      /*in/out*/
                CCRndParams_t  *trngParams_ptr,   /*in/out*/
                CCBool_t            isContinued,    /*in*/
                uint32_t        **sourceOut_ptr_ptr,   /*out*/
                uint32_t         *sourceOutSize_ptr,/*in/out*/
                uint32_t         *rndWorkBuff_ptr,     /*in*/
                CCBool_t          isStartup)           /*in*/
{
    /* LOCAL DECLARATIONS */

    /* The return error identifier */
    CCError_t error = 0, descrError = 0;

    int32_t  i;
    uint32_t roscToStart;
    uint32_t *ramAddr;
    const uint32_t trng90bRequiredBytes = (isStartup == CC_FALSE ? CC_CONFIG_TRNG90B_AMOUNT_OF_BYTES : CC_CONFIG_TRNG90B_AMOUNT_OF_BYTES_STARTUP);

    /* FUNCTION LOGIC */

    /* ............... local initializations .............................. */
    /* -------------------------------------------------------------------- */

    /* initializing the Error to O.K */
    error = CC_OK;

    /* Set source RAM address with offset 8 bytes from sourceOut address in
      order to remain empty bytes for CC operations */
    *sourceOut_ptr_ptr = rndWorkBuff_ptr;
    ramAddr = *sourceOut_ptr_ptr;

    /* init to 0 for FE mode */
    *sourceOutSize_ptr = 0;

    /* If not continued mode, set TRNG parameters and restart TRNG  */
    /*--------------------------------------------------------------*/
    if (isContinued == CC_FALSE) {

        /* Full restart TRNG */
        error = startTrngHW(
               rndState_ptr,
               trngParams_ptr,
               CC_TRUE/*isRestart*/,
               &roscToStart,
               isStartup);

        /*Note: in case of error the TRNG HW is still not started*/
        if (error)
            goto End;
    }
    /* On continued mode check HW TRNG */
    else {
        /* check TRNG parameters */
        error = LLF_RND_TRNG_CheckHwParams(trngParams_ptr);
        if (error != CC_OK)
            goto End;

        /* previously started ROSCs */
        roscToStart = (rndState_ptr->TrngProcesState & 0xff000000) >> 24;
    }

    /*====================================================*/
    /*====================================================*/
    /*         Processing after previous start            */
    /*====================================================*/
    /*====================================================*/

    /*====================================================*/
    /* TRNG90b mode processing: start Roscs sequentionally - *
    * from fast to slow Rosc                  */
    /*====================================================*/

    for (i = 0; i < LLF_RND_NUM_OF_ROSCS; ++i) {

        *sourceOutSize_ptr = trng90bRequiredBytes;

        descrError = readMultipleEHR(*sourceOutSize_ptr, (uint8_t *)ramAddr,
                                        rndState_ptr,
                                        trngParams_ptr,
                                        &roscToStart);

        if (descrError == CC_OK) {
            error = runContinuousTesting(ramAddr, *sourceOutSize_ptr);
            if (error == CC_OK) {
                break;
            }
            *sourceOutSize_ptr = 0;
        }
        else {
            /* Note: Priority to SW error */
            error = descrError;
        }

        /* update total processed ROSCs */
        rndState_ptr->TrngProcesState |= ((rndState_ptr->TrngProcesState >> 8) & 0x00FF0000);
        /*clean started & not processed*/
        rndState_ptr->TrngProcesState &= 0x00FFFFFF;

        /* case of erors or watchdog exceed  - try next rosc */
        /*  if no remain roscs to start, return error */
        if (roscToStart == 0x8) {
            error = LLF_RND_TRNG_GENERATION_NOT_COMPLETED_ERROR;
            break;
        } else {
            /* Call StartTrng, with next ROSC */
            roscToStart <<= 1;
            TRNG_LOG_DEBUG("process with next rosc[%d]\n",(int) roscToStart);
            error = startTrngHW(
               rndState_ptr,
               trngParams_ptr,
               CC_FALSE/*isRestart*/,
               &roscToStart,
               isStartup);

            if (error != CC_OK)
                goto End;
        }
    }

    /* ................. end of function ..................................... */
    /* ----------------------------------------------------------------------- */
    End:

    /* turn the RNG off    */
    LLF_RND_TurnOffTrng();

    return error;
} /* END of getTrngSource */

/****************************************************************************************/
/*****************************       Public Functions      ******************************/
/****************************************************************************************/

CCError_t LLF_RND_StartTrngHW(
        CCRndState_t  *rndState_ptr,
        CCRndParams_t *trngParams_ptr,
        CCBool_t           isRestart,
        uint32_t         *roscsToStart_ptr)
{
        CCError_t error = CC_OK;

        error = startTrngHW(rndState_ptr, trngParams_ptr, isRestart, roscsToStart_ptr, CC_FALSE/*isStartup*/);

        return error;
}


CCError_t LLF_RND_GetTrngSource(
                 CCRndState_t   *rndState_ptr,      /*in/out*/
                 CCRndParams_t  *trngParams_ptr,    /*in/out*/
                 CCBool_t       isContinued,        /*in*/
                 uint32_t       **sourceOut_ptr_ptr,/*out*/
                 uint32_t       *sourceOutSize_ptr, /*in/out*/
                 uint32_t       *rndWorkBuff_ptr)   /*in*/
{
    CCError_t error = CC_OK;

    error = getTrngSource(rndState_ptr, trngParams_ptr, isContinued,
                  sourceOut_ptr_ptr, sourceOutSize_ptr, rndWorkBuff_ptr, CC_FALSE/*isStartup*/);

    return error;
}

CCError_t LLF_RND_RunTrngStartupTest(
        CCRndState_t        *rndState_ptr,
        CCRndParams_t       *trngParams_ptr,
        uint32_t                *rndWorkBuff_ptr)
{
    CCError_t error = CC_OK;
    uint32_t *pSourceOut;
    uint32_t sourceOutSize;

    error = getTrngSource(rndState_ptr, trngParams_ptr, CC_FALSE/*isContinued*/,
                &pSourceOut, &sourceOutSize, rndWorkBuff_ptr, CC_TRUE/* isStartup*/);

    return error;
}

