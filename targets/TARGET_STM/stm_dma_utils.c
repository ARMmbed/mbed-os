/* mbed Microcontroller Library
 * Copyright (c) 2016-2023 STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
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

#include "stm_dma_utils.h"

#include "mbed_error.h"

#include <stdbool.h>
#include <malloc.h>
#include <string.h>

// Array to store pointer to DMA handle for each DMA channel.
// Note: arrays are 0-indexed, so DMA1 Channel2 is at stmDMAHandles[0][1].
static DMA_HandleTypeDef * stmDMAHandles[NUM_DMA_CONTROLLERS][MAX_DMA_CHANNELS_PER_CONTROLLER];

DMA_Channel_TypeDef * stm_get_dma_channel(const DMALinkInfo *dmaLink)
{
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA1_Channel1
                case 1:
                    return DMA1_Channel1;
#endif
#ifdef DMA1_Channel2
                case 2:
                    return DMA1_Channel2;
#endif
#ifdef DMA1_Channel3
                case 3:
                    return DMA1_Channel3;
#endif
#ifdef DMA1_Channel4
                case 4:
                    return DMA1_Channel4;
#endif
#ifdef DMA1_Channel5
                case 5:
                    return DMA1_Channel5;
#endif
#ifdef DMA1_Channel6
                case 6:
                    return DMA1_Channel6;
#endif
#ifdef DMA1_Channel7
                case 7:
                    return DMA1_Channel7;
#endif
#ifdef DMA1_Stream0
                case 0:
                    return DMA1_Stream0;
#endif
#ifdef DMA1_Stream1
                case 1:
                    return DMA1_Stream1;
#endif
#ifdef DMA1_Stream2
                case 2:
                    return DMA1_Stream2;
#endif
#ifdef DMA1_Stream3
                case 3:
                    return DMA1_Stream3;
#endif
#ifdef DMA1_Stream4
                case 4:
                    return DMA1_Stream4;
#endif
#ifdef DMA1_Stream5
                case 5:
                    return DMA1_Stream5;
#endif
#ifdef DMA1_Stream6
                case 6:
                    return DMA1_Stream6;
#endif
#ifdef DMA1_Stream7
                case 7:
                    return DMA1_Stream7;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif

#ifdef DMA2
        case 2:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA2_Channel1
                case 1:
                    return DMA2_Channel1;
#endif
#ifdef DMA2_Channel2
                case 2:
                    return DMA2_Channel2;
#endif
#ifdef DMA2_Channel3
                case 3:
                    return DMA2_Channel3;
#endif
#ifdef DMA2_Channel4
                case 4:
                    return DMA2_Channel4;
#endif
#ifdef DMA2_Channel5
                case 5:
                    return DMA2_Channel5;
#endif
#ifdef DMA2_Channel6
                case 6:
                    return DMA2_Channel6;
#endif
#ifdef DMA2_Channel7
                case 7:
                    return DMA2_Channel7;
#endif
#ifdef DMA2_Stream0
                case 0:
                    return DMA2_Stream0;
#endif
#ifdef DMA2_Stream1
                case 1:
                    return DMA2_Stream1;
#endif
#ifdef DMA2_Stream2
                case 2:
                    return DMA2_Stream2;
#endif
#ifdef DMA2_Stream3
                case 3:
                    return DMA2_Stream3;
#endif
#ifdef DMA2_Stream4
                case 4:
                    return DMA2_Stream4;
#endif
#ifdef DMA2_Stream5
                case 5:
                    return DMA2_Stream5;
#endif
#ifdef DMA2_Stream6
                case 6:
                    return DMA2_Stream6;
#endif
#ifdef DMA2_Stream7
                case 7:
                    return DMA2_Stream7;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif
#ifdef GPDMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef GPDMA1_Channel0
                case 0:
                    return GPDMA1_Channel0;
#endif
#ifdef GPDMA1_Channel1
                case 1:
                    return GPDMA1_Channel1;
#endif
#ifdef GPDMA1_Channel2
                case 2:
                    return GPDMA1_Channel2;
#endif
#ifdef GPDMA1_Channel3
                case 3:
                    return GPDMA1_Channel3;
#endif
#ifdef GPDMA1_Channel4
                case 4:
                    return GPDMA1_Channel4;
#endif
#ifdef GPDMA1_Channel5
                case 5:
                    return GPDMA1_Channel5;
#endif
#ifdef GPDMA1_Channel6
                case 6:
                    return GPDMA1_Channel6;
#endif
#ifdef GPDMA1_Channel7
                case 7:
                    return GPDMA1_Channel7;
#endif
#ifdef GPDMA1_Channel8
                case 8:
                    return GPDMA1_Channel8;
#endif
#ifdef GPDMA1_Channel9
                case 9:
                    return GPDMA1_Channel9;
#endif
#ifdef GPDMA1_Channel10
                case 10:
                    return GPDMA1_Channel10;
#endif
#ifdef GPDMA1_Channel11
                case 11:
                    return GPDMA1_Channel11;
#endif
#ifdef GPDMA1_Channel12
                case 12:
                    return GPDMA1_Channel12;
#endif
#ifdef GPDMA1_Channel13
                case 13:
                    return GPDMA1_Channel13;
#endif
#ifdef GPDMA1_Channel14
                case 14:
                    return GPDMA1_Channel14;
#endif
#ifdef GPDMA1_Channel15
                case 15:
                    return GPDMA1_Channel15;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);

    }
}

IRQn_Type stm_get_dma_irqn(const DMALinkInfo *dmaLink)
{
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef DMA1_Channel1
                case 1:
                    return DMA1_Channel1_IRQn;
#endif

// STM32F0 has shared ISRs for Ch2-Ch3 and Ch4-Ch5, and NO ISRs for channels 6 and 7
#ifdef TARGET_MCU_STM32F0
                case 2:
                case 3:
                    return DMA1_Channel2_3_IRQn;
                case 4:
                case 5:
                    return DMA1_Channel4_5_IRQn;

// STM32G0 has shared ISRs for Ch2-Ch3 and Ch4-Ch7 (and also all DMA2 channels on devices with DMA2)
#elif defined(TARGET_MCU_STM32G0)
                case 2:
                case 3:
                    return DMA1_Channel2_3_IRQn;

                // IRQ name for the remaining DMA channels depends on whether DMA2 exists or not
                case 4:
                case 5:
                case 6:
                case 7:
#ifdef DMA2
                    return DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn;
#elif defined(DMA1_Channel7)
                    return DMA1_Ch4_7_DMAMUX1_OVR_IRQn;
#else
                    return DMA1_Ch4_5_DMAMUX1_OVR_IRQn;
#endif

// STM32L0 has shared ISRs for Ch2-Ch3 and Ch4-Ch7
#elif defined(TARGET_MCU_STM32L0)
                case 2:
                case 3:
                    return DMA1_Channel2_3_IRQn;

                case 4:
                case 5:
                case 6:
                case 7:
                    return DMA1_Channel4_5_6_7_IRQn;
#else
#ifdef DMA1_Channel2
                case 2:
                    return DMA1_Channel2_IRQn;
#endif
#ifdef DMA1_Channel3
                case 3:
                    return DMA1_Channel3_IRQn;
#endif
#ifdef DMA1_Channel4
                case 4:
                    return DMA1_Channel4_IRQn;
#endif
#ifdef DMA1_Channel5
                case 5:
                    return DMA1_Channel5_IRQn;
#endif
#ifdef DMA1_Channel6
                case 6:
                    return DMA1_Channel6_IRQn;
#endif
#ifdef DMA1_Channel7
                case 7:
                    return DMA1_Channel7_IRQn;
#endif
#endif

#ifdef DMA1_Stream0
                case 0:
                    return DMA1_Stream0_IRQn;
#endif
#ifdef DMA1_Stream1
                case 1:
                    return DMA1_Stream1_IRQn;
#endif
#ifdef DMA1_Stream2
                case 2:
                    return DMA1_Stream2_IRQn;
#endif
#ifdef DMA1_Stream3
                case 3:
                    return DMA1_Stream3_IRQn;
#endif
#ifdef DMA1_Stream4
                case 4:
                    return DMA1_Stream4_IRQn;
#endif
#ifdef DMA1_Stream5
                case 5:
                    return DMA1_Stream5_IRQn;
#endif
#ifdef DMA1_Stream6
                case 6:
                    return DMA1_Stream6_IRQn;
#endif
#ifdef DMA1_Stream7
                case 7:
                    return DMA1_Stream7_IRQn;
#endif

                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif

#ifdef DMA2
        case 2:
            switch(dmaLink->channelIdx)
            {
#ifdef TARGET_MCU_STM32G0
                // STM32G0 does its own thing and has all DMA2 channels under 1 IRQ
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    return DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn;
#else

#ifdef DMA2_Channel1
                case 1:
                    return DMA2_Channel1_IRQn;
#endif
#ifdef DMA2_Channel2
                case 2:
                    return DMA2_Channel2_IRQn;
#endif
#ifdef DMA2_Channel3
                case 3:
                    return DMA2_Channel3_IRQn;
#endif
#ifdef DMA2_Channel4
                case 4:
                    return DMA2_Channel4_IRQn;
#endif
#ifdef DMA2_Channel5
                case 5:
                    return DMA2_Channel5_IRQn;
#endif
#ifdef DMA2_Channel6
                case 6:
                    return DMA2_Channel6_IRQn;
#endif
#ifdef DMA2_Channel7
                case 7:
                    return DMA2_Channel7_IRQn;
#endif

#ifdef DMA2_Stream0
                    case 0:
                    return DMA2_Stream0_IRQn;
#endif
#ifdef DMA2_Stream1
                    case 1:
                    return DMA2_Stream1_IRQn;
#endif
#ifdef DMA2_Stream2
                    case 2:
                    return DMA2_Stream2_IRQn;
#endif
#ifdef DMA2_Stream3
                    case 3:
                    return DMA2_Stream3_IRQn;
#endif
#ifdef DMA2_Stream4
                    case 4:
                    return DMA2_Stream4_IRQn;
#endif
#ifdef DMA2_Stream5
                    case 5:
                    return DMA2_Stream5_IRQn;
#endif
#ifdef DMA2_Stream6
                    case 6:
                    return DMA2_Stream6_IRQn;
#endif
#ifdef DMA2_Stream7
                    case 7:
                    return DMA2_Stream7_IRQn;
#endif
#endif

                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif

#ifdef GPDMA1
        case 1:
            switch(dmaLink->channelIdx)
            {
#ifdef GPDMA1_Channel0
                case 0:
                    return GPDMA1_Channel0_IRQn;
#endif
#ifdef GPDMA1_Channel1
                case 1:
                    return GPDMA1_Channel1_IRQn;
#endif
#ifdef GPDMA1_Channel2
                case 2:
                    return GPDMA1_Channel2_IRQn;
#endif
#ifdef GPDMA1_Channel3
                case 3:
                    return GPDMA1_Channel3_IRQn;
#endif
#ifdef GPDMA1_Channel4
                case 4:
                    return GPDMA1_Channel4_IRQn;
#endif
#ifdef GPDMA1_Channel5
                case 5:
                    return GPDMA1_Channel5_IRQn;
#endif
#ifdef GPDMA1_Channel6
                case 6:
                    return GPDMA1_Channel6_IRQn;
#endif
#ifdef GPDMA1_Channel7
                case 7:
                    return GPDMA1_Channel7_IRQn;
#endif
#ifdef GPDMA1_Channel8
                case 8:
                    return GPDMA1_Channel8_IRQn;
#endif
#ifdef GPDMA1_Channel9
                case 9:
                    return GPDMA1_Channel9_IRQn;
#endif
#ifdef GPDMA1_Channel10
                case 10:
                    return GPDMA1_Channel10_IRQn;
#endif
#ifdef GPDMA1_Channel11
                case 11:
                    return GPDMA1_Channel11_IRQn;
#endif
#ifdef GPDMA1_Channel12
                case 12:
                    return GPDMA1_Channel12_IRQn;
#endif
#ifdef GPDMA1_Channel13
                case 13:
                    return GPDMA1_Channel13_IRQn;
#endif
#ifdef GPDMA1_Channel14
                case 14:
                    return GPDMA1_Channel14_IRQn;
#endif
#ifdef GPDMA1_Channel15
                case 15:
                    return GPDMA1_Channel15_IRQn;
#endif
                default:
                    mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA channel", dmaLink->channelIdx, MBED_FILENAME, __LINE__);
            }
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);

    }
}

DMA_HandleTypeDef *stm_init_dma_link(const DMALinkInfo *dmaLink, uint32_t direction, bool periphInc, bool memInc,
                                     uint8_t periphDataAlignment, uint8_t memDataAlignment){

#ifdef DMA_IP_VERSION_V2
    // Channels start from 1 in IP v2 only
    uint8_t channelIdx = dmaLink->channelIdx - 1;
#else
    uint8_t channelIdx = dmaLink->channelIdx;
#endif

    if(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx] != NULL)
    {
        // Channel already allocated (e.g. two SPI busses which use the same DMA request tried to be initialized)
        return NULL;
    }

     // Enable DMA mux clock for devices with it
#ifdef __HAL_RCC_DMAMUX1_CLK_ENABLE
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
#endif

    // Turn on clock for the DMA module
    switch(dmaLink->dmaIdx)
    {
#ifdef DMA1
        case 1:
            __HAL_RCC_DMA1_CLK_ENABLE();
            break;
#endif
#ifdef DMA2
        case 2:
            __HAL_RCC_DMA2_CLK_ENABLE();
            break;
#endif
#ifdef GPDMA1
        case 1:
            __HAL_RCC_GPDMA1_CLK_ENABLE();
            break;
#endif
#ifdef BDMA
        case 3:
            __HAL_RCC_BDMA_CLK_ENABLE();
            break;
#endif
        default:
            mbed_error(MBED_ERROR_ITEM_NOT_FOUND, "Invalid DMA controller", dmaLink->dmaIdx, MBED_FILENAME, __LINE__);
    }

    // Allocate DMA handle.
    // Yes it's a little gross that we have to allocate on the heap, but this structure uses quite a lot of memory,
    // so we don't want to allocate DMA handles until they're needed.
    DMA_HandleTypeDef * dmaHandle = malloc(sizeof(DMA_HandleTypeDef));
    memset(dmaHandle, 0, sizeof(DMA_HandleTypeDef));
    stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx] = dmaHandle;

    // Configure handle
    dmaHandle->Instance = stm_get_dma_channel(dmaLink);
#if STM_DEVICE_HAS_DMA_SOURCE_SELECTION

    // Most devices with IP v1 call this member "Channel" and most with IP v2 call it "Request".
    // But not STM32H7!
#if defined(DMA_IP_VERSION_V1) && !defined(TARGET_MCU_STM32H7)
    dmaHandle->Init.Channel = dmaLink->sourceNumber << DMA_SxCR_CHSEL_Pos;
#else
    dmaHandle->Init.Request = dmaLink->sourceNumber;
#endif

#endif
    dmaHandle->Init.Direction = direction;

    // IP v3 uses different fields for... basically everything in this struct
#ifdef DMA_IP_VERSION_V3
    if(direction == DMA_MEMORY_TO_PERIPH || direction == DMA_MEMORY_TO_MEMORY)
    {
        // Source is memory
        dmaHandle->Init.SrcInc = memInc ? DMA_SINC_INCREMENTED : DMA_SINC_FIXED;

        switch(memDataAlignment) {
            case 4:
                dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
                break;
            case 2:
                dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
                break;
            case 1:
            default:
                dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
                break;

        }
    }
    else {
        // Source is a peripheral
        dmaHandle->Init.SrcInc = periphInc ? DMA_SINC_INCREMENTED : DMA_SINC_FIXED;

        switch(periphDataAlignment) {
            case 4:
                dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
                break;
            case 2:
                dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
                break;
            case 1:
            default:
                dmaHandle->Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
                break;

        }
    }

    if(direction == DMA_PERIPH_TO_MEMORY || direction == DMA_MEMORY_TO_MEMORY)
    {
        // Destination is memory
        dmaHandle->Init.DestInc = memInc ? DMA_SINC_INCREMENTED : DMA_SINC_FIXED;

        switch(memDataAlignment) {
            case 4:
                dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
                break;
            case 2:
                dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
                break;
            case 1:
            default:
                dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
                break;

        }
    }
    else {
        // Destination is a peripheral
        dmaHandle->Init.DestInc = periphInc ? DMA_SINC_INCREMENTED : DMA_SINC_FIXED;

        switch(periphDataAlignment) {
            case 4:
                dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
                break;
            case 2:
                dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
                break;
            case 1:
            default:
                dmaHandle->Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
                break;

        }
    }

    dmaHandle->Init.SrcBurstLength = 1;
    dmaHandle->Init.DestBurstLength = 1;
    dmaHandle->Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    dmaHandle->Init.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
    dmaHandle->Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT0;
    dmaHandle->Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;

#else
    dmaHandle->Init.PeriphInc = periphInc ? DMA_PINC_ENABLE : DMA_PINC_DISABLE;
    dmaHandle->Init.MemInc = memInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;
    dmaHandle->Init.Priority = DMA_PRIORITY_MEDIUM;

    switch(periphDataAlignment) {
        case 4:
            dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
            break;
        case 2:
            dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
            break;
        case 1:
        default:
            dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            break;

    }

    switch(memDataAlignment) {
        case 4:
            dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
            break;
        case 2:
            dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
            break;
        case 1:
        default:
            dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            break;

    }

#endif

    dmaHandle->Init.Mode = DMA_NORMAL;

    HAL_DMA_Init(dmaHandle);

    // Set up interrupt
    IRQn_Type irqNum = stm_get_dma_irqn(dmaLink);
    NVIC_EnableIRQ(irqNum);
    NVIC_SetPriority(irqNum, 7);

    return dmaHandle;
}

void stm_free_dma_link(const DMALinkInfo *dmaLink)
{
    // Note: we can't disable the interrupt here, in case one ISR is shared by multiple DMA channels
    // and another channel is still using the interrupt.

#ifdef DMA_IP_VERSION_V2
    // Channels start from 1 in IP v2 only
    uint8_t channelIdx = dmaLink->channelIdx - 1;
#else
    uint8_t channelIdx = dmaLink->channelIdx;
#endif

    // Deinit hardware channel
    HAL_DMA_DeInit(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx]);

    // Free memory
    free(stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx]);
    stmDMAHandles[dmaLink->dmaIdx - 1][channelIdx] = NULL;
}

#ifdef DMA_IP_VERSION_V2

#ifdef DMA1_Channel1
void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][0]);
}
#endif

// STM32F0 has shared ISRs for Ch2-Ch3 and Ch4-Ch5
#ifdef TARGET_MCU_STM32F0

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
    }
    if(stmDMAHandles[0][2] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
    }
}

void DMA1_Channel4_5_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
    }
}

#elif defined(TARGET_MCU_STM32G0)

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
    }
    if(stmDMAHandles[0][2] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
    }
}

#ifdef DMA2
void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
    }
    if(stmDMAHandles[0][5] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5]);
    }
    if(stmDMAHandles[0][6] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6]);
    }
    if(stmDMAHandles[1][0] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][0]);
    }
    if(stmDMAHandles[1][1] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][1]);
    }
    if(stmDMAHandles[1][2] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][2]);
    }
    if(stmDMAHandles[1][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][3]);
    }
    if(stmDMAHandles[1][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[1][4]);
    }
}
#elif defined(DMA1_Channel7)
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
    }
    if(stmDMAHandles[0][5] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5]);
    }
    if(stmDMAHandles[0][6] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6]);
    }
}
#else
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
    }
}
#endif

#elif defined(TARGET_MCU_STM32L0)

void DMA1_Channel2_3_IRQHandler(void)
{
    if(stmDMAHandles[0][1] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
    }
    if(stmDMAHandles[0][2] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
    }
}

void DMA1_Channel4_5_6_7_IRQHandler(void)
{
    if(stmDMAHandles[0][3] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
    }
    if(stmDMAHandles[0][4] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
    }
    if(stmDMAHandles[0][5] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][5]);
    }
    if(stmDMAHandles[0][6] != NULL) {
        HAL_DMA_IRQHandler(stmDMAHandles[0][6]);
    }
}

#else

#ifdef DMA1_Channel2
void DMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
}
#endif

#ifdef DMA1_Channel3
void DMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
}
#endif

#ifdef DMA1_Channel4
void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
}
#endif

#ifdef DMA1_Channel5
void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
}
#endif

#ifdef DMA1_Channel6
void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][5]);
}
#endif

#ifdef DMA1_Channel7
void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][6]);
}
#endif
#endif

#ifdef DMA2_Channel1
void DMA2_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][0]);
}
#endif

#ifdef DMA2_Channel2
void DMA2_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][1]);
}
#endif

#ifdef DMA2_Channel3
void DMA2_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][2]);
}
#endif

#ifdef DMA2_Channel4
void DMA2_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][3]);
}
#endif

#ifdef DMA2_Channel5
void DMA2_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][4]);
}
#endif

#ifdef DMA2_Channel6
void DMA2_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][5]);
}
#endif

#ifdef DMA2_Channel7
void DMA2_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][6]);
}
#endif
#endif // DMA_IP_VERSION_V2

#ifdef DMA_IP_VERSION_V1
#ifdef DMA1_Stream0
void DMA1_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][0]);
}
#endif

#ifdef DMA1_Stream1
void DMA1_Stream1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
}
#endif

#ifdef DMA1_Stream2
void DMA1_Stream2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
}
#endif

#ifdef DMA1_Stream3
void DMA1_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
}
#endif

#ifdef DMA1_Stream4
void DMA1_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
}
#endif

#ifdef DMA1_Stream5
void DMA1_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][5]);
}
#endif

#ifdef DMA1_Stream6
void DMA1_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][6]);
}
#endif

#ifdef DMA1_Stream7
void DMA1_Stream7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][7]);
}
#endif

#ifdef DMA2_Stream0
void DMA2_Stream0_IRQHandler(void)
{
    // Note: Unlike both IP v2 and IP v3, IP v1 channels are 0-indexed.
    HAL_DMA_IRQHandler(stmDMAHandles[1][0]);
}
#endif

#ifdef DMA2_Stream1
void DMA2_Stream1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][1]);
}
#endif

#ifdef DMA2_Stream2
void DMA2_Stream2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][2]);
}
#endif

#ifdef DMA2_Stream3
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][3]);
}
#endif

#ifdef DMA2_Stream4
void DMA2_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][4]);
}
#endif

#ifdef DMA2_Stream5
void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][5]);
}
#endif

#ifdef DMA2_Stream6
void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][6]);
}
#endif

#ifdef DMA2_Stream7
void DMA2_Stream7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[1][7]);
}
#endif
#endif // DMA_IP_VERSION_V1

#ifdef DMA_IP_VERSION_V3
#ifdef GPDMA1_Channel0
void GPDMA1_Channel0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][0]);
}
#endif

#ifdef GPDMA1_Channel1
void GPDMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][1]);
}
#endif

#ifdef GPDMA1_Channel2
void GPDMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][2]);
}
#endif

#ifdef GPDMA1_Channel3
void GPDMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][3]);
}
#endif

#ifdef GPDMA1_Channel4
void GPDMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][4]);
}
#endif

#ifdef GPDMA1_Channel5
void GPDMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][5]);
}
#endif

#ifdef GPDMA1_Channel6
void GPDMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][6]);
}
#endif

#ifdef GPDMA1_Channel7
void GPDMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][7]);
}
#endif

#ifdef GPDMA1_Channel8
void GPDMA1_Channel8_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][8]);
}
#endif

#ifdef GPDMA1_Channel9
void GPDMA1_Channel9_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][9]);
}
#endif

#ifdef GPDMA1_Channel10
void GPDMA1_Channel10_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][10]);
}
#endif

#ifdef GPDMA1_Channel11
void GPDMA1_Channel11_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][11]);
}
#endif

#ifdef GPDMA1_Channel12
void GPDMA1_Channel12_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][12]);
}
#endif

#ifdef GPDMA1_Channel13
void GPDMA1_Channel13_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][13]);
}
#endif

#ifdef GPDMA1_Channel14
void GPDMA1_Channel14_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][14]);
}
#endif

#ifdef GPDMA1_Channel15
void GPDMA1_Channel15_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stmDMAHandles[0][15]);
}
#endif
#endif // DMA_IP_VERSION_V3