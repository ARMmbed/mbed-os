//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <common_api.h>
#include <hcd_api.h>
#include <spi_hcd_if.h>
#include <targaddrs.h>
#include "hw20_mbox_host_reg.h"
#include "atheros_wifi_api.h"

/* prototypes */
static A_STATUS Driver_SetAddressWindowRegister(void *pCxt, uint32_t RegisterAddr, uint32_t Address);

/* Compile the 4BYTE version of the window register setup routine,
 * This mitigates host interconnect issues with non-4byte aligned bus requests, some
 * interconnects use bus adapters that impose strict limitations.
 * Since diag window access is not intended for performance critical operations, the 4byte mode should
 * be satisfactory even though it generates 4X the bus activity. */

#ifdef USE_4BYTE_REGISTER_ACCESS

/* set the window address register (using 4-byte register access ). */
/*****************************************************************************/
/*  Driver_SetAddressWindowRegister - Utility function to set the window
 *	 register. This is used for diagnostic reads and writes.
 *      void *pCxt - the driver context.
 *		uint32_t RegisterAddr - The window register address.
 *		uint32_t Address - the target address.
 *****************************************************************************/
static A_STATUS Driver_SetAddressWindowRegister(void *pCxt, uint32_t RegisterAddr, uint32_t Address)
{
    A_STATUS status;
    uint8_t addrValue[4];
    int32_t i;
    A_NETBUF_DECLARE req;
    void *pReq = (void *)&req;

    /* write bytes 1,2,3 of the register to set the upper address bytes, the LSB is written
     * last to initiate the access cycle */
    Address = A_CPU2LE32(Address);

    for (i = 1; i <= 3; i++)
    {
        /* fill the buffer with the address byte value we want to hit 4 times*/
        addrValue[0] = ((uint8_t *)&Address)[i];
        addrValue[1] = addrValue[0];
        addrValue[2] = addrValue[0];
        addrValue[3] = addrValue[0];

        A_NETBUF_CONFIGURE(pReq, (void *)addrValue, 0, 4, 4);
        ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, RegisterAddr + i, false, 4);

        if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
        {
            break;
        }
    }

    if (status != A_OK)
    {
        return status;
    }

    A_NETBUF_CONFIGURE(pReq, (void *)&Address, 0, 4, 4);
    ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, RegisterAddr, true, 4);
    status = Hcd_DoPioExternalAccess(pCxt, pReq);

    return status;
}

#else

/*****************************************************************************/
/*  Driver_SetAddressWindowRegister - Utility function to set the window
 *	 register. This is used for diagnostic reads and writes.
 *      void *pCxt - the driver context.
 *		uint32_t RegisterAddr - The window register address.
 *		uint32_t Address - the target address.
 *****************************************************************************/
static A_STATUS Driver_SetAddressWindowRegister(void *pCxt, uint32_t RegisterAddr, uint32_t Address)
{
    A_STATUS status;
    A_NETBUF_DECLARE req;
    void *pReq = (void *)&req;

    Address = A_CPU2LE32(Address);

    do
    {
        A_NETBUF_CONFIGURE(pReq, (((uint8_t *)(&Address)) + 1), (sizeof(uint32_t) - 1));
        ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, RegisterAddr + 1, true, (sizeof(uint32_t) - 1));

        if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
        {
            break;
        }

        A_NETBUF_CONFIGURE(pReq, ((uint8_t *)(&Address)), sizeof(uint8_t));
        ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, RegisterAddr, true, sizeof(uint8_t));

        if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
        {
            break;
        }
    } while (0);

    return status;
}

#endif

/*****************************************************************************/
/*  Driver_ReadRegDiag - Reads four bytes of data from the specified chip
 *	 device address.
 *      void *pCxt - the driver context.
 *		uint32_t address - the device chip address to start the read.
 *		uint8_t *data - the start of data destination buffer.
 *****************************************************************************/
A_STATUS
Driver_ReadRegDiag(void *pCxt, uint32_t *address, uint32_t *data)
{
    A_STATUS status;
    A_NETBUF_DECLARE req;
    void *pReq = (void *)&req;

    A_NETBUF_CONFIGURE(pReq, data, 0, sizeof(uint32_t), sizeof(uint32_t));

    do
    {
        /* set window register to start read cycle */
        if (A_OK != (status = Driver_SetAddressWindowRegister(pCxt, WINDOW_READ_ADDR_ADDRESS, *address)))
        {
            break;
        }

        ATH_SET_PIO_EXTERNAL_READ_OPERATION(pReq, WINDOW_DATA_ADDRESS, true, sizeof(uint32_t));

        if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
        {
            break;
        }
    } while (0);

    return status;
}

/*****************************************************************************/
/*  Driver_WriteRegDiag - Writes four bytes of data to the specified chip
 *	 device address.
 *      void *pCxt - the driver context.
 *		uint32_t address - the device chip address to start the write.
 *		uint8_t *data - the start of data source buffer.
 *****************************************************************************/
A_STATUS
Driver_WriteRegDiag(void *pCxt, uint32_t *address, uint32_t *data)
{
    A_STATUS status;

    A_NETBUF_DECLARE req;
    void *pReq = (void *)&req;

    A_NETBUF_CONFIGURE(pReq, data, 0, sizeof(uint32_t), sizeof(uint32_t));

    ATH_SET_PIO_EXTERNAL_WRITE_OPERATION(pReq, WINDOW_DATA_ADDRESS, true, sizeof(uint32_t));

    do
    {
        if (A_OK != (status = Hcd_DoPioExternalAccess(pCxt, pReq)))
        {
            break;
        }
        /* set window register, which starts the write cycle */
        if (A_OK != (status = Driver_SetAddressWindowRegister(pCxt, WINDOW_WRITE_ADDR_ADDRESS, *address)))
        {
            break;
        }
    } while (0);

    return status;
}

/*****************************************************************************/
/*  Driver_ReadDataDiag - Reads a buffer of data starting from address. Length
 * 	 of data is specified by length. Data is read from a contiguous chip
 *	 address memory/register space.
 *      void *pCxt - the driver context.
 *		uint32_t address - the device chip address to start the read.
 *		uint8_t *data - the start of data destination buffer.
 *		uint32_t length - the length of data in bytes.
 *****************************************************************************/
A_STATUS
Driver_ReadDataDiag(void *pCxt, uint32_t address, uint8_t *data, uint32_t length)
{
    uint32_t count;
    A_STATUS status = A_OK;

    for (count = 0; count < length; count += 4, address += 4)
    {
        if ((status = Driver_ReadRegDiag(pCxt, &address, (uint32_t *)&data[count])) != A_OK)
        {
            break;
        }
    }

    return status;
}

/*****************************************************************************/
/*  Driver_WriteDataDiag - Writes a buffer of data starting at address. Length
 * 	 of data is specified by length. Data is written to a contiguous chip
 *	 address memory/register space.
 *      void *pCxt - the driver context.
 *		uint32_t address - the device chip address to start the write.
 *		uint8_t *data - the start of data source buffer.
 *		uint32_t length - the length of data in bytes.
 *****************************************************************************/
A_STATUS
Driver_WriteDataDiag(void *pCxt, uint32_t address, uint8_t *data, uint32_t length)
{
    uint32_t count;
    A_STATUS status = A_OK;

    for (count = 0; count < length; count += 4, address += 4)
    {
        if ((status = Driver_WriteRegDiag(pCxt, &address, (uint32_t *)&data[count])) != A_OK)
        {
            break;
        }
    }

    return status;
}

#define REG_DUMP_COUNT_AR4100 60
#define REG_DUMP_COUNT_AR4002 60
#define REGISTER_DUMP_LEN_MAX 60

/*****************************************************************************/
/*  Driver_DumpAssertInfo - Collects assert information from chip and writes
 *	 it to stdout.
 *      void *pCxt - the driver context.
 *		uint32_t *pData - buffer to store UINT32 results
 *		uint16_t *pLength - IN/OUT param to store length of buffer for IN and
 *		 length of contents for OUT.
 *****************************************************************************/
A_STATUS
Driver_DumpAssertInfo(void *pCxt, uint32_t *pData, uint16_t *pLength)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint32_t address;
    uint32_t regDumpArea = 0;
    A_STATUS status = A_ERROR;
    uint32_t regDumpCount = 0;

    do
    {
        if (*pLength < REGISTER_DUMP_LEN_MAX)
        {
            break;
        }
        /* clear it */
        *pLength = 0;
        /* the reg dump pointer is copied to the host interest area */
        address = HOST_INTEREST_ITEM_ADDRESS(hi_failure_state);
        address = TARG_VTOP(address);

        if (pDCxt->targetType == TARGET_TYPE_AR4100 || pDCxt->targetType == TARGET_TYPE_AR400X)
        {
            regDumpCount = REG_DUMP_COUNT_AR4100;
        }
        else
        {
            A_ASSERT(0); /* should never happen */
#if DRIVER_CONFIG_DISABLE_ASSERT
            break;
#endif
        }

        /* read RAM location through diagnostic window */
        if (A_OK != (status = Driver_ReadRegDiag(pCxt, &address, &regDumpArea)))
        {
            A_ASSERT(0); /* should never happen */
#if DRIVER_CONFIG_DISABLE_ASSERT
            break;
#endif
        }

        regDumpArea = A_LE2CPU32(regDumpArea);

        if (regDumpArea == 0)
        {
            /* no reg dump */
            break;
        }

        regDumpArea = TARG_VTOP(regDumpArea);

        /* fetch register dump data */
        if (A_OK !=
            (status = Driver_ReadDataDiag(pCxt, regDumpArea, (uint8_t *)&pData[0], regDumpCount * (sizeof(uint32_t)))))
        {
            A_ASSERT(0); /* should never happen */
#if DRIVER_CONFIG_DISABLE_ASSERT
            break;
#endif
        }

        *pLength = regDumpCount;
    } while (0);

    return status;
}
