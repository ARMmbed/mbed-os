/*************************************************************************//**
 *
 * \file    crc_api.c
 * \brief   ...
 *
 * \note    None
 *
 * \author  Daniel Cesarini <daniel.cesarini@tridonic.com>
 * \author  Martin Tomasini <martin.tomasini2@tridonic.com>
 * \version 0.1
 * \date    4 July 2016
 * \bug     None
 *
 * \copyright 2016, Tridonic GmbH & Co KG
 *                  Färbergasse 15 - 6851 Dornbirn, Austria
 *
 *****************************************************************************/

#include "crc_api.h"
#include "cmsis.h"

static uint32_t l_CRC;

/* CRC handler declaration */
CRC_HandleTypeDef   CRC_HandleStruct;

int Init()
{
    __HAL_RCC_CRC_CLK_ENABLE();
    CRC_HandleStruct.Instance = CRC;
    HAL_CRC_Init(&CRC_HandleStruct);
}

int Reset()
{
    /* Reset CRC Calculation Unit */
    __HAL_CRC_DR_RESET(&CRC_HandleStruct);
}

int CalcSingle(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
{
    uint32_t *pui32StartAddress = (uint32_t*)pui8StartAddress;
    uint32_t ui32DataSize32Bits = ui32DataSize / 4;

    for (uint32_t i = 0; i < ui32DataSize32Bits; i++)
    {
        pui32StartAddress[i] = __RBIT(pui32StartAddress[i]);
        pui32StartAddress[i] = __REV(pui32StartAddress[i]);
    }

    *pui32CRC = HAL_CRC_Calculate(&CRC_HandleStruct, (uint32_t*)pui8StartAddress, ui32DataSize32Bits);

    // Reverse back the data (otherwise it will leave the data changed)
    for (uint32_t i = 0; i < ui32DataSize32Bits; i++)
    {
        pui32StartAddress[i] = __REV(pui32StartAddress[i]);
        pui32StartAddress[i] = __RBIT(pui32StartAddress[i]);
    }

    // reverse CRC result before final XOR
    *pui32CRC = __RBIT(*pui32CRC);
    *pui32CRC ^= 0xffffffff;

    return 0;
}

int CalcAccumulate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
{
    uint32_t *pui32StartAddress = (uint32_t*)pui8StartAddress;
    uint32_t ui32DataSize32Bits = ui32DataSize / 4;
    uint32_t ui32TemporaryDataStorage;

    // Reverse input bits
    for (uint32_t i = 0; i < ui32DataSize32Bits; i++)
    {
        ui32TemporaryDataStorage = pui32StartAddress[i];
        ui32TemporaryDataStorage = __RBIT(ui32TemporaryDataStorage);
        *pui32CRC = HAL_CRC_Accumulate(&CRC_HandleStruct, &ui32TemporaryDataStorage, 1);
    }

    // reverse CRC result before final XOR
    *pui32CRC = __RBIT(*pui32CRC);
    *pui32CRC ^= 0xffffffff;

    return 0;
}

