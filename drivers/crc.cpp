/*************************************************************************//**
 *
 * \file    crc.cpp
 * \brief   CRC32 CPP middle layer to the lower layers
 *
 * Access to the f0, f4 and sw implementation of CRC
 *
 * \note
 *
 * \author  Daniel Cesarini <daniel.cesarini@tridonic.com>
 * \author  Martin Tomasini <martin.tomasini2@tridonic.com>
 *
 * \version 0.1
 * \date    4 July 2016
 * \bug     None
 *
 * \copyright 2016, Tridonic GmbH & Co KG
 *                  Färbergasse 15 - 6851 Dornbirn, Austria
 *
 *****************************************************************************/

#include "crc.h"
#include "mbed.h"

/*** TEST logger-macros ***/
#if 0
  #define PRINT_LOG(...) printf("[crc.cpp -> INFO] "); printf(__VA_ARGS__);
#else
  #define PRINT_LOG(...) (void)0
#endif
/*** END TEST ***/

#define SWITCH_ENDIAN32(VARIABLE)\
        VARIABLE =  ((VARIABLE >> 24) & 0xff) | \
                    ((VARIABLE << 8) & 0xff0000) | \
                    ((VARIABLE >> 8) & 0xff00) | \
                    ((VARIABLE << 24) & 0xff000000)

#ifdef HW_CRC
extern "C"
{
    #include "crc_api.h"
}
#else
// Software implementation (CalcCrc)
extern "C" {
void CalcCrc(uint8_t* pui8Data, uint32_t ui32DataSize, uint32_t* pui32Result);
}
int Init(void);
int Reset(void);
int CalcSingle(uint8_t* ui32StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);
int CalcAccumulate(uint8_t* ui32StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);
#endif

/*
 * This function is taking the two parameters, xoring them, and putting the result in the first one.
 */
void myXor(uint8_t* pui8_inputOutputValue, uint32_t* pui32_inputValue)
{
    uint8_t* pui8_localInputValue = (uint8_t*) pui32_inputValue;

    for (int i = 0; i < 4; i++)
    {
        pui8_inputOutputValue[i] ^= pui8_localInputValue[i];
    }
}

namespace mbed {

    Crc::Crc(void)
    {
        PRINT_LOG("Crc::InstanceCounter: %u \n", Crc::InstanceCounter);
        if (!Initialized)
        {
            Init();
            Initialized = true;
        }
        Crc::InstanceCounter++;
        //Reset();
    }

    int Crc::_UseCrc(void)
    {
        return 0;
    }

    int Crc::_ReleaseCrc(void)
    {
        return 0;
    }

    void Crc::_Reset(void)
    {
        localCrc = 0x00000000;      // Initial value to XOR with the first word of the array
        Reset();
    }

    int Crc::_Calculate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
    {
        int result;
#ifdef HW_CRC
        uint32_t ui32LeftoverBytes;
        uint8_t aui8LastBytes[4];
        uint8_t ui8Padding;
        ui8Padding = 0x00;

        // Reset the CRC module
        Crc::_Reset();

        // Padding needed
        if (ui32DataSize%4)
        {
            // Calculate how many leftover bytes need to be copied to buffer
            ui32LeftoverBytes = (ui32DataSize%4);

            if ((ui32DataSize / 4) >= 1)
            {
                // Start the CRC calculation
                result = CalcAccumulate(pui8StartAddress, ui32DataSize-ui32LeftoverBytes, pui32CRC);
            }

            for(uint8_t i = 0; i < 4; i++)
            {
                if (ui32LeftoverBytes > 0)
                {
                    // Copy data from pui8Data
                    aui8LastBytes[i] = pui8StartAddress[ui32DataSize-(ui32DataSize%4)+i];
                    ui32LeftoverBytes--;
                }
                else
                {
                    // Insert padding
                    aui8LastBytes[i] = ui8Padding;
                }
            }
            result = CalcAccumulate(aui8LastBytes, 4, pui32CRC);
        }
        else
        {
            // Start the CRC calculation
            result = CalcAccumulate(pui8StartAddress, ui32DataSize, pui32CRC);
        }

#else
        result = CalcSingle(pui8StartAddress, ui32DataSize, pui32CRC);
#endif
        return result;
    }

    int Crc::_Accumulate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
    {
        int result;
        uint8_t aui8Bytes[4];
        uint8_t ui8Padding = 0x00;

        if (ui32DataSize < 4)
        {
            for(uint8_t i = 0; i < 4; i++)
            {
                if (i < ui32DataSize)
                {
                    // Copy data from pui8StartAddress
                    aui8Bytes[i] = pui8StartAddress[i];
                }
                else
                {
                    // Insert padding
                    aui8Bytes[i] = ui8Padding;
                }
            }
            myXor(aui8Bytes, &localCrc);
            result = Crc::_Calculate(aui8Bytes, 4, pui32CRC);
        }
        else
        {
            myXor(pui8StartAddress, &localCrc);
            result = Crc::_Calculate(pui8StartAddress, ui32DataSize, pui32CRC);
        }
        localCrc = *pui32CRC; // GB-DC: We suppose that this is implicitly performing an endianness inversion

        return result;
    }

    Crc::~Crc()
    {
        Crc::InstanceCounter--;
    }

    uint32_t Crc::InstanceCounter = 0;
    bool Crc::Initialized = false;

} // namespace mbed


#ifndef HW_CRC
int Init()
{
    return 0;
}

int Reset()
{
    return 0;
}

int CalcSingle(uint8_t* ui32StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
{
    // CalcCrc (SW) already is padding (with 0s) the last bytes in case they are not multiple of 4
    CalcCrc(ui32StartAddress, ui32DataSize, pui32CRC);
    return 0;
}

int CalcAccumulate(uint8_t* ui32StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
{
    *pui32CRC = 0x01020304;
    return -1;
}


/***************************************************
*  crcSw.c
*
*  Online CRC calculator for comparing results:
*  http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
*
*  Created on: 11.05.2016
*      Author: Daniel.Cesarini
*
*  Tridonic GmbH & Co KG
*  Färbergasse 15
*  6851 Dornbirn, Austria
*
***************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
#include "tri-stc-ilb/crc.h"

#define REAL_CRC_SW

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
#ifdef REAL_CRC_SW
/*******************************************************************************
 * Taken from:
 * http://opensource.apple.com//source/gdb/gdb-1824/src/gdb/rdi-share/crc.c
 * TODO:
 * import .h file:
 * http://opensource.apple.com//source/gdb/gdb-1824/src/gdb/rdi-share/crc.h
 *
 *******************************************************************************/
/*
 * Copyright (C) 1995 Advanced RISC Machines Limited. All rights reserved.
 *
 * This software may be freely used, copied, modified, and distributed
 * provided that the above copyright notice is preserved in all copies of the
 * software.
 */

/* -*-C-*-
 *
 * 1.1.1.1
 *     1999/04/16 01:34:27
 *
 *
 * crc.c - provides some "standard" CRC calculation routines.
 *
 */
//#include "crc.h"    /* describes this code */

/**********************************************************************/

/*
 * crc32                                                IEEE-802.3 32bit CRC
 * -----                                                --------------------
 */
//#define RUNTIME_CRC32_TABLE
#ifndef RUNTIME_CRC32_TABLE
/* This table was generated by the "crctable" program */
static const unsigned int crc32table[256] = {
                   /* 0x00 */ 0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
                   /* 0x04 */ 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
                   /* 0x08 */ 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
                   /* 0x0C */ 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
                   /* 0x10 */ 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
                   /* 0x14 */ 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
                   /* 0x18 */ 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
                   /* 0x1C */ 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
                   /* 0x20 */ 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
                   /* 0x24 */ 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
                   /* 0x28 */ 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
                   /* 0x2C */ 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
                   /* 0x30 */ 0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
                   /* 0x34 */ 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
                   /* 0x38 */ 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
                   /* 0x3C */ 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
                   /* 0x40 */ 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
                   /* 0x44 */ 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
                   /* 0x48 */ 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
                   /* 0x4C */ 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
                   /* 0x50 */ 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
                   /* 0x54 */ 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
                   /* 0x58 */ 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
                   /* 0x5C */ 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
                   /* 0x60 */ 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
                   /* 0x64 */ 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
                   /* 0x68 */ 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
                   /* 0x6C */ 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
                   /* 0x70 */ 0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
                   /* 0x74 */ 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
                   /* 0x78 */ 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
                   /* 0x7C */ 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
                   /* 0x80 */ 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
                   /* 0x84 */ 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
                   /* 0x88 */ 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
                   /* 0x8C */ 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
                   /* 0x90 */ 0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
                   /* 0x94 */ 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
                   /* 0x98 */ 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
                   /* 0x9C */ 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
                   /* 0xA0 */ 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
                   /* 0xA4 */ 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
                   /* 0xA8 */ 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
                   /* 0xAC */ 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
                   /* 0xB0 */ 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
                   /* 0xB4 */ 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
                   /* 0xB8 */ 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
                   /* 0xBC */ 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
                   /* 0xC0 */ 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
                   /* 0xC4 */ 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
                   /* 0xC8 */ 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
                   /* 0xCC */ 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
                   /* 0xD0 */ 0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
                   /* 0xD4 */ 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
                   /* 0xD8 */ 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
                   /* 0xDC */ 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
                   /* 0xE0 */ 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
                   /* 0xE4 */ 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
                   /* 0xE8 */ 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
                   /* 0xEC */ 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
                   /* 0xF0 */ 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
                   /* 0xF4 */ 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
                   /* 0xF8 */ 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
                   /* 0xFC */ 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};
#else

/*
 * Generate CRCtable at runtime
 * from http://www.faqs.org/rfcs/rfc1952.html
 * and https://tools.ietf.org/html/rfc1952
 */
/* Table of CRCs of all 8-bit messages. */
unsigned int crc32table[256];

/* Flag: has the table been computed? Initially false. */
int crc_table_computed = 0;

/* Make the table for a fast CRC. */
void make_crc_table(void)
{
    unsigned long c;

    int n, k;
    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1) {
                c = 0xedb88320L ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc32table[n] = c;
    }
    crc_table_computed = 1;
}
#endif

unsigned int crc32(unsigned char *address, unsigned int size, unsigned int crc)
{
  for (; (size > 0); size--)
    /* byte loop */
    crc = (((crc >> 8) & 0x00FFFFFF) ^
           crc32table[(crc ^ *address++) & 0x000000FF]);
  return(crc);
}

/**********************************************************************/
/* EOF crc.c */

void NonPaddingCalcCrc(uint8_t* pui8Data, uint32_t ui32DataSize, uint32_t* pui32Result)
{
    *pui32Result = crc32(pui8Data, ui32DataSize, 0xffffffff);
    *pui32Result ^= 0xffffffff;
}

/*
 * This CalcCrc is padding the data to be a multiple of 4 bytes.
 */
void CalcCrc(uint8_t* pui8Data, uint32_t ui32DataSize, uint32_t* pui32Result)
{
    uint32_t ui32PaddingBytes;
    uint8_t ui8Padding;
    ui8Padding = 0x00;
#ifdef RUNTIME_CRC32_TABLE
    if(crc_table_computed != 1)
    {
        make_crc_table();
    }
#endif

    // Is padding needed
    if (ui32DataSize%4)
    {
        // Calculate how many padding bytes are needed
        ui32PaddingBytes = (4 - (ui32DataSize%4));
    }
    else // Padding not needed
    {
        ui32PaddingBytes = 0;
    }

    *pui32Result = crc32(pui8Data, ui32DataSize, 0xffffffff);

    // while padding needed
    while (ui32PaddingBytes > 0)
    {
        *pui32Result = crc32(&ui8Padding, 1, *pui32Result);
        ui32PaddingBytes--;
    }

    *pui32Result ^= 0xffffffff;
}

#else // #ifdef REAL_CRC_SW -> Fake CRC
void CalcCrc(uint8_t* pui8Data, uint32_t ui32DataSize, uint32_t* pui32Result)
{
    *pui32Result = 0xaaaaaaaa;
}
#endif // #ifdef REAL_CRC_SW -> Fake CRC


#ifdef __cplusplus
}
#endif

#endif // #ifndef HW_CRC
