/*************************************************************************//**
 *
 * \file    crc_api.c
 * \brief   ...
 *
 * \note    None
 *
 * \author  Daniel Cesarini <daniel.cesarini@tridonic.com>
 *
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
    CRC_HandleStruct.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;       // use default poly=0x04c11db7
    CRC_HandleStruct.Init.CRCLength               = CRC_POLYLENGTH_32B;              // calc 32bit crc
    CRC_HandleStruct.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;       // use default init value (0xffffffff)
    CRC_HandleStruct.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_BYTE;    // The input data are inverted
    CRC_HandleStruct.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE; // The output data are inverted
    CRC_HandleStruct.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;      // The input data are 8 bits length

    HAL_CRC_Init(&CRC_HandleStruct);
}

int Reset()
{
    /* Reset CRC Calculation Unit */
    // For now to do the reset we DeInit and Init again.
    HAL_CRC_DeInit(&CRC_HandleStruct);
    HAL_CRC_Init(&CRC_HandleStruct);
}

/*
 * Not implemented yet.
 */
int CalcSingle(uint8_t* ui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
{
   *pui32CRC = 2;
   return 0;
}

int CalcAccumulate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
{
    uint32_t uwCRCValue;
    uint32_t ui32DataSize32Bits = ui32DataSize / 4;
    uint32_t ui32TemporaryDataStorage;

    // Reverse input bits
    for (uint32_t i = 0; i < ui32DataSize32Bits; i++)
    {
        //printf("StartAddress: %x, i: %d \n", pui32StartAddress, i);
        ui32TemporaryDataStorage =
                (pui8StartAddress[i*4+0] << 24) +
                (pui8StartAddress[i*4+1] << 16) +
                (pui8StartAddress[i*4+2] <<  8) +
                (pui8StartAddress[i*4+3] <<  0);

        uwCRCValue = HAL_CRC_Accumulate(&CRC_HandleStruct, &ui32TemporaryDataStorage, 1);
    }

    // final XOR
    uwCRCValue ^= 0xffffffff;

    *pui32CRC = uwCRCValue;

    return 0;
}

