/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if DEVICE_SAI

#include <string.h>
#include "stm_sai_api.h"
#include "PeripheralPins.h"

static sai_result_t stm_sai_init(sai_t *obj, sai_init_t *init);
static bool stm_sai_xfer(sai_t *obj, uint32_t *sample);
static void stm_sai_free(sai_t *obj);

const stm_sai_api_t stm_sai_vtable = {
    .init = stm_sai_init,
    .xfer = stm_sai_xfer,
    .free = stm_sai_free
};

static sai_result_t stm_sai_init(sai_t *obj, sai_init_t *init) {
    uint32_t tmpreg = 0;
    if ((memcmp(&(init->format), &sai_mode_i2s16w32, sizeof(sai_format_t)) != 0) &&
        (memcmp(&(init->format), &sai_mode_i2s16, sizeof(sai_format_t)) != 0)) {
        // we only support 1 format so far
        return SAI_RESULT_CONFIG_UNSUPPORTED;
    }

    // validate pins & fetch base peripheral address
    int32_t mclk = pinmap_peripheral(init->mclk, PinMap_SAI_MCLK);
    int32_t sd = pinmap_peripheral(init->sd, PinMap_SAI_SD);
    int32_t bclk = pinmap_peripheral(init->bclk, PinMap_SAI_BCLK);
    int32_t wclk = pinmap_peripheral(init->wclk, PinMap_SAI_WCLK);
    MBED_ASSERT((sd != NC) && (bclk != NC) && (wclk != NC));

    int32_t base = pinmap_merge(pinmap_merge(pinmap_merge(sd, bclk), wclk), mclk);
    MBED_ASSERT(base != NC);
    MBED_ASSERT((obj != NULL) && (obj->base != NULL) && (obj->base->type == SAI_PERIPH_TYPE_SAI));

    /* SAI GPIO Configuration --------------------------------------------------*/
    if (init->mclk != (PinName)NC) {
        pinmap_pinout(init->mclk, PinMap_SAI_MCLK);
    }
    pinmap_pinout(init->sd, PinMap_SAI_SD);
    pinmap_pinout(init->bclk, PinMap_SAI_BCLK);
    pinmap_pinout(init->wclk, PinMap_SAI_WCLK);

    /* SAI Configuration -------------------------------------------------------*/
    {
        /* Enable PLLSAI Clock */
        __HAL_RCC_PLLSAI_ENABLE();
        /* Get tick */
        uint32_t tickstart = HAL_GetTick();
        /* Wait till PLLSAI is ready */
        while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET) {
            if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE) {
                /* return in case of Timeout detected */
                return SAI_RESULT_GENERIC_FAILURE;
            }
        }
    }

    /*
    Configure SAI_Block_x
    LSBFirst : Disabled
    DataSize : 16 */
    uint32_t vcoinput = 0;

    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI_PLLSAI;
    PeriphClkInit.PLLSAI.PLLSAIN = 256; /*!< This parameter must be a number between Min_Data = 50 and Max_Data = 432. */ // VCO max 432MHz
    PeriphClkInit.PLLSAI.PLLSAIQ = 5; /*!< This parameter must be a number between Min_Data = 2 and Max_Data = 15. */ // output max 192 MHz
    PeriphClkInit.PLLSAI.PLLSAIR = 4; /*!< This parameter must be a number between Min_Data = 2 and Max_Data = 7. */
    PeriphClkInit.PLLSAIDivQ = 25; /*!< This parameter must be a number between Min_Data = 1 and Max_Data = 32 */

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        return SAI_RESULT_GENERIC_FAILURE;
    }


    /* SAI Block clock source selection */
    if(obj->base->u.sai_block == SAI1_Block_A) {
        __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(RCC_SAIBCLKSOURCE_PLLSAI);
    } else {
        __HAL_RCC_SAI_BLOCKBCLKSOURCE_CONFIG(RCC_SAIACLKSOURCE_PLLSAI);
    }
    __HAL_RCC_SAI1_CLK_ENABLE();

    /* VCO Input Clock value calculation */
    if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI) {
        /* In Case the PLL Source is HSI (Internal Clock) */
        vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
    } else {
        /* In Case the PLL Source is HSE (External Clock) */
        vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));
    }
    /* Configure the PLLI2S division factor */
    /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
    /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
    tmpreg = (RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> 24U;
    uint32_t sai_clock_source = (vcoinput * ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> 6U))/(tmpreg);

    /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
    tmpreg = (((RCC->DCKCFGR & RCC_DCKCFGR_PLLSAIDIVQ) >> 8U) + 1U);
    sai_clock_source = sai_clock_source/(tmpreg);

    /*
        sample rate * frame length
     */
    uint32_t data_size = init->format.data_length; // possible values : 8, 10, 16, 20, 24, 32
    uint32_t slot_size = init->format.word_length; // 0, 16, 32. 0 means = datasize
    uint32_t slot_count = 2;

    /* compute hirez div times 2^n to keep some precision yet having fast results
       (division by a power of two is a simple bit shift) */

    uint32_t fix_point = 4; // xxxxxxxxx.yyyy
    uint32_t fix_factor = 1<<fix_point; // 0x10
    uint32_t fix_mask = fix_factor - 1; // 0x0F

    uint32_t real_slot_size = (slot_size==0?data_size:slot_size);
    uint32_t bitclock = (init->sample_rate * real_slot_size * slot_count);

    uint32_t required_mclk = 256 * init->sample_rate;

    uint32_t mckdiv_hirez = (sai_clock_source*fix_factor) / required_mclk;
    uint32_t mckdiv = mckdiv_hirez >> fix_point;

    /* Round result to the nearest integer */
    if ((mckdiv & (fix_mask)) > (fix_factor/2)) {
        mckdiv+= 1U;
    }

    if (mckdiv == 0) {
        // target clock can not be reached
        return SAI_RESULT_GENERIC_FAILURE;
    }
    if (mckdiv >= 32) {
        // out of MCKDIV range, reduce sai_clock_source frenquency
        return SAI_RESULT_GENERIC_FAILURE;
    }

    /* SAI_Init ----------------------------------------------------------------*/
    tmpreg = obj->base->u.sai_block->CR1;
    tmpreg &= ~(SAI_xCR1_MODE     | SAI_xCR1_PRTCFG     | SAI_xCR1_DS       |
                SAI_xCR1_LSBFIRST | SAI_xCR1_CKSTR      | SAI_xCR1_SYNCEN   |
                SAI_xCR1_MONO     | SAI_xCR1_OUTDRIV    | SAI_xCR1_DMAEN    |
                SAI_xCR1_NODIV    | SAI_xCR1_MCKDIV);
    uint32_t mode = obj->is_receiver ? 3 : 0; // slave_rx or master_tx

    uint32_t ds = ((data_size/8) << 1) + ((32-__CLZ(data_size%8)) != 0); // 31-clz(x) <=> ln(x)/ln(2)
    tmpreg |= (uint32_t)(
        (mode << SAI_xCR1_MODE_Pos) | (0 << SAI_xCR1_PRTCFG_Pos)    | (ds << SAI_xCR1_DS_Pos)   |
        (0 << SAI_xCR1_LSBFIRST_Pos)| (1 << SAI_xCR1_CKSTR_Pos)     | (0 << SAI_xCR1_SYNCEN_Pos)|
        (0 << SAI_xCR1_MONO_Pos)    | (0 << SAI_xCR1_OUTDRIV_Pos)   | (0 << SAI_xCR1_DMAEN_Pos) |
        (0 << SAI_xCR1_NODIV_Pos)   | (uint32_t)((mckdiv/2) << 20)
    );
    obj->base->u.sai_block->CR1 = tmpreg;

    tmpreg = obj->base->u.sai_block->CR2;
    tmpreg &= ~(SAI_xCR2_FTH | SAI_xCR2_FFLUSH | SAI_xCR2_COMP | SAI_xCR2_CPL);
    tmpreg |= (uint32_t)(SAI_FIFOTHRESHOLD_1QF);
    obj->base->u.sai_block->CR2 = tmpreg;
    /* SAI_Init ----------------------------------------------------------------*/

    /* SAI_FrameInit -----------------------------------------------------------*/
    tmpreg = obj->base->u.sai_block->FRCR;
    tmpreg &= ~(SAI_xFRCR_FRL | SAI_xFRCR_FSALL | SAI_xFRCR_FSDEF | SAI_xFRCR_FSPOL | SAI_xFRCR_FSOFF);
    tmpreg |= (uint32_t)(
        (((real_slot_size * slot_count) - 1) << SAI_xFRCR_FRL_Pos)  |
        ((real_slot_size - 1) << SAI_xFRCR_FSALL_Pos)|
        (0 << SAI_xFRCR_FSDEF_Pos)      |
        (1 << SAI_xFRCR_FSPOL_Pos)      |
        SAI_xFRCR_FSOFF
    );
    obj->base->u.sai_block->FRCR = tmpreg;
    /* SAI_FrameInit -----------------------------------------------------------*/

    /* SAI_SlotInit ------------------------------------------------------------*/
    tmpreg = obj->base->u.sai_block->SLOTR;
    tmpreg &= ~(SAI_xSLOTR_FBOFF | SAI_xSLOTR_SLOTSZ | SAI_xSLOTR_NBSLOT | SAI_xSLOTR_SLOTEN);
    tmpreg |=  0 | ((slot_size/16)<<SAI_xSLOTR_SLOTSZ_Pos) | ((slot_count - 1) << SAI_xSLOTR_NBSLOT_Pos) | SAI_xSLOTR_SLOTEN;
    obj->base->u.sai_block->SLOTR = tmpreg;
    /* SAI_SlotInit ------------------------------------------------------------*/

    /* FIFO flush */
    obj->base->u.sai_block->CR2 |= SAI_xCR2_FFLUSH;

    return SAI_RESULT_OK;
}

static bool stm_sai_xfer(sai_t *obj, uint32_t *psample) {
    bool ret = false;
    uint32_t sr = obj->base->u.sai_block->SR;
    uint32_t fifo_level = (sr & SAI_xSR_FLVL) >> SAI_xSR_FLVL_Pos;

    if ((obj->base->u.sai_block->CR1 & SAI_xCR1_SAIEN) != SAI_xCR1_SAIEN) {
        obj->base->u.sai_block->CR1 |= SAI_xCR1_SAIEN;
    }

    if (obj->is_receiver) {
        ret = fifo_level != 0; // 0 = EMPTY
        if (ret) {
            uint32_t sample = obj->base->u.sai_block->DR;
            if (psample != NULL) {
                *psample = sample;
            }
        }
    } else {
        ret = fifo_level != 5; // 5 = FULL
        if (ret && (psample != NULL)) {
            obj->base->u.sai_block->DR = *psample;
        }
    }
    return ret;
}

static void stm_sai_free(sai_t *obj) {
    obj->base->u.sai_block->CR1 &= ~SAI_xCR1_SAIEN;
    (void)*obj;
}

#endif // DEVICE_SAI
