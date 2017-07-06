/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#include "objects.h"
#include "PinNames.h"
#include "pinmap.h"

#include "rtl8195a.h"
#include "flash_ext.h"

extern u32 ConfigDebugInfo;
extern SPIC_INIT_PARA SpicInitParaAllClk[3][CPU_CLK_TYPE_NO];

static int flash_inited = 0;
static flash_t flashobj;
static void flash_ext_init(void)
{
    if (!SpicFlashInitRtl8195A(SpicOneBitMode)){
        HAL_WRITE32(SYSTEM_CTRL_BASE, REG_SYS_DSTBY_INFO3, HAL_READ32(SYSTEM_CTRL_BASE, REG_SYS_DSTBY_INFO3)|0xf);
    } else {
        flash_inited = 1;
    }
    flashobj.SpicInitPara.flashtype = SpicInitParaAllClk[0][0].flashtype;
}

void __flash_ext_turnon(void)
{
    SPI_FLASH_PIN_FCTRL(ON);
    SpicWaitBusyDoneRtl8195A();

    if (flash_inited == 0) {
        flash_ext_init();
    }
}

void __flash_ext_turnoff(void)
{
    SpicDisableRtl8195A();
}

void flash_ext_write_protect(flash_t *obj, uint32_t protect)
{
    __flash_ext_turnon();
    SpicWriteProtectFlashRtl8195A(protect);
    __flash_ext_turnoff();
}

void __flash_ext_erase_sector(flash_t *obj, uint32_t address)
{
    SpicSectorEraseFlashRtl8195A(SPI_FLASH_BASE+address);
}

void flash_ext_erase_sector(flash_t *obj, uint32_t address)
{
    __flash_ext_turnon();
    __flash_ext_erase_sector(obj, address);
    __flash_ext_turnoff();
}

void flash_ext_erase_block(flash_t *obj, uint32_t address)
{
    __flash_ext_turnon();
    SpicBlockEraseFlashRtl8195A(SPI_FLASH_BASE+address);
    __flash_ext_turnoff();
}

int flash_ext_read_word(flash_t *obj, uint32_t address, uint32_t *data)
{
    __flash_ext_turnon();

    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    *data = HAL_READ32(SPI_FLASH_BASE, address);
    __flash_ext_turnoff();
    return 0;
}

/**
  * @brief  Write a word to specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be programmed.
  * @param  data: Specified the data to be programmed.
  * @retval   status: Success:1 or Failure: Others.
  */
int flash_ext_write_word(flash_t *obj, uint32_t address, uint32_t data)
{
    u8 flashtype = 0;
    __flash_ext_turnon();

    flashtype = flashobj.SpicInitPara.flashtype;
    HAL_WRITE32(SPI_FLASH_BASE, address, data);
    SpicWaitBusyDoneRtl8195A();

    if(flashtype == FLASH_MICRON){
        SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
    } else {
        SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    }

    __flash_ext_turnoff();
    return 0;
}

/**
  * @brief  Read a stream of data from specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  len: Specifies the length of the data to read.
  * @param  data: Specified the address to save the readback data.
  * @retval   status: Success:1 or Failure: Others.
  */
int flash_ext_stream_read(flash_t *obj, uint32_t addr, uint32_t len, uint8_t *data)
{
    uint32_t i, offset, word;
    uint8_t *ptr, *pbuf;

    __flash_ext_turnon();

    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

    offset = addr & 0x03;
    addr = addr & ~0x03;
    pbuf = data;
    if (offset != 0) {
        word = HAL_READ32(SPI_FLASH_BASE, addr);
        ptr = (uint8_t *)&word + offset;
        offset = 4 - offset;
        for (i = 0; i < offset && len > 0; i++, len--) {
            *pbuf = *ptr;
            pbuf++;
            ptr++;
        }
    addr += 4;
    }

    ptr = (uint8_t *)&word;
    if ((uint32_t)pbuf & 0x03) {
        while (len >= 4) {
            word = HAL_READ32(SPI_FLASH_BASE, addr);
            for (i = 0; i < 4; i++) {
                *pbuf = *(ptr+i);
                pbuf++;
            }
            addr += 4;
            len -= 4;
        }
    } else {
        while (len >= 4) {
            *((uint32_t *)pbuf) = HAL_READ32(SPI_FLASH_BASE, addr);
            pbuf += 4;
            addr += 4;
            len -= 4;
        }
    }

    if (len > 0) {
        word = HAL_READ32(SPI_FLASH_BASE, addr);
        for (i = 0; i < len; i++) {
            *pbuf = *(ptr+i);
            pbuf++;
        }
    }

    __flash_ext_turnoff();
    return 0;
}

/**
  * @brief  Write a stream of data to specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  len: Specifies the length of the data to write.
  * @param  data: Specified the pointer of the data to be written.
  * @retval   status: Success:1 or Failure: Others.
  */
int __flash_ext_stream_write(flash_t *obj, uint32_t addr, uint32_t len, const uint8_t *data)
{
    uint32_t i, offset, word;
    const uint8_t*pbuf;
    uint8_t *ptr;
    u8 flashtype = 0;

    offset = addr & 0x03;
    addr = addr & ~0x03;
    pbuf = data;
    flashtype = flashobj.SpicInitPara.flashtype;

    if (offset != 0) {
        word = HAL_READ32(SPI_FLASH_BASE, addr);
        ptr = (uint8_t *)&word + offset;
        offset = 4 - offset;
        for (i = 0; i < offset && len > 0; i++, len--) {
            *ptr = *pbuf;
            pbuf++;
            ptr++;
        }
        HAL_WRITE32(SPI_FLASH_BASE, addr, word);
        SpicWaitBusyDoneRtl8195A();

        if(flashtype == FLASH_MICRON){
            SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
        } else {
            SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
        }
        addr += 4;
    }

    if ((uint32_t)pbuf & 0x03) {
        while (len >= 4) {
            word = (uint32_t)(*pbuf) | ((uint32_t)(*(pbuf+1)) << 8)|
                ((uint32_t)(*(pbuf+2)) << 16) | ((uint32_t)(*(pbuf+3)) << 24);
            HAL_WRITE32(SPI_FLASH_BASE, addr, word);
            SpicWaitBusyDoneRtl8195A();

            if(flashtype == FLASH_MICRON){
                SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
            } else {
                SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
            }

            pbuf += 4;
            addr += 4;
            len -= 4;
        }
    } else {
        while (len >= 4) {
            HAL_WRITE32(SPI_FLASH_BASE, addr, (uint32_t)*((uint32_t *)pbuf));
            SpicWaitBusyDoneRtl8195A();

            if(flashtype == FLASH_MICRON){
                SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
            } else {
                SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
            }

            pbuf += 4;
            addr += 4;
            len -= 4;
        }
    }

    if (len > 0) {
        word = HAL_READ32(SPI_FLASH_BASE, addr);
        ptr = (uint8_t*)&word;
        for (i = 0; i < len; i++) {
            *(ptr+i) = *pbuf;
            pbuf++;
        }

        HAL_WRITE32(SPI_FLASH_BASE, addr, word);
        SpicWaitBusyDoneRtl8195A();

        if(flashtype == FLASH_MICRON){
            SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
        } else {
            SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
        }
    }

    return 0;
}

int flash_ext_stream_write(flash_t *obj, uint32_t addr, uint32_t len, const uint8_t *data)
{
    int32_t status;

    __flash_ext_turnon();
    status = __flash_ext_stream_write(obj, addr, len, data);
    __flash_ext_turnoff();

    return status;
}

int flash_stream_read(flash_t *obj, uint32_t addr, uint32_t len, uint8_t *data)
{
    return flash_ext_stream_read(obj, addr, len, data);
}

int flash_stream_write(flash_t *obj, uint32_t addr, uint32_t len, const uint8_t *data)
{
    return flash_ext_stream_write(obj, addr, len, data);
}

/*
Function Description:
This function performans the same functionality as the function flash_stream_write.
It enhances write performance by reducing overheads.
Users can use either of functions depending on their needs.

* @brief  Write a stream of data to specified address
* @param  obj: Specifies the parameter of flash object.
* @param  address: Specifies the address to be read.
* @param  length: Specifies the length of the data to write.
* @param  data: Specified the pointer of the data to be written.
* @retval   status: Success:1 or Failure: Others.

*/

int flash_ext_burst_write(flash_t *obj, uint32_t address ,uint32_t length, uint8_t *data)
{
    u32 OccuSize;
    u32 ProgramSize;
    u32 PageSize;
    u8 flashtype = 0;

    PageSize = 256;

    __flash_ext_turnon();

    flashtype = flashobj.SpicInitPara.flashtype;
    OccuSize = address & 0xFF;
    if((length >= PageSize) ||((length + OccuSize) >= PageSize)){
        ProgramSize = PageSize - OccuSize;
    } else {
        ProgramSize = length;
    }

    flashobj.Length = length;
    while(length > 0){
        if(OccuSize){
            SpicUserProgramRtl8195A(data, flashobj.SpicInitPara, address, &(flashobj.Length));
                // Wait spic busy done
            SpicWaitBusyDoneRtl8195A();
            // Wait flash busy done (wip=0)
            if(flashtype == FLASH_MICRON){
                SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
            } else {
                SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
            }

            address += ProgramSize;
            data += ProgramSize;
            length -= ProgramSize;
            OccuSize = 0;
        } else{
            while((flashobj.Length) >= PageSize){
                SpicUserProgramRtl8195A(data, flashobj.SpicInitPara, address, &(flashobj.Length));
                    // Wait spic busy done
                SpicWaitBusyDoneRtl8195A();
                // Wait flash busy done (wip=0)
                if(flashtype == FLASH_MICRON){
                    SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
                } else {
                    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
                }

                address += PageSize;
                data += PageSize;
                length -= PageSize;
            }
            flashobj.Length = length;
            if((flashobj.Length)  > 0){
                SpicUserProgramRtl8195A(data, flashobj.SpicInitPara, address, &(flashobj.Length));
                    // Wait spic busy done
                SpicWaitBusyDoneRtl8195A();
                // Wait flash busy done (wip=0)
                if(flashtype == FLASH_MICRON){
                    SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
                } else {
                    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
                }

                break;
            }
        }
        flashobj.Length = length;
    }
    __flash_ext_turnoff();
    return 0;

}
/**
  * @brief  Read a stream of data from specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  len: Specifies the length of the data to read.
  * @param  data: Specified the address to save the readback data.
  * @retval   status: Success:1 or Failure: Others.
  */

int flash_ext_burst_read(flash_t *obj, uint32_t address, uint32_t length, uint8_t *data)
{
    __flash_ext_turnon();
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    SpicUserReadRtl8195A(length, address, data, SpicOneBitMode);
    __flash_ext_turnoff();
    return 0;
}

int flash_ext_get_status(flash_t *obj)
{
    uint8_t status = 0;

    __flash_ext_turnon();
    status = SpicGetFlashStatusRefinedRtl8195A(flashobj.SpicInitPara);
    __flash_ext_turnoff();
    return status;
}

/*
Function Description:
Please refer to the datatsheet of flash for more details of the content of status register.
The block protected area and the corresponding control bits are provided in the flash datasheet.

* @brief  Set Status register to enable desired operation
* @param  obj: Specifies the parameter of flash object.
* @param  data: Specifies which bit users like to set
   ex: if users want to set the third bit, data = 0x8.

*/
int flash_ext_set_status(flash_t *obj, uint32_t data)
{
    __flash_ext_turnon();
    SpicSetFlashStatusRefinedRtl8195A(data, flashobj.SpicInitPara);
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    __flash_ext_turnoff();
    return 0;
}

/*
Function Description:
This function aims to reset the status register, please make sure the operation is appropriate.
*/
void flash_ext_reset_status(flash_t *obj)
{
    __flash_ext_turnon();
    SpicSetFlashStatusRefinedRtl8195A(0, flashobj.SpicInitPara);
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    __flash_ext_turnoff();
}

int flash_ext_get_extend_addr(flash_t *obj)
{
    uint8_t addr = 0;

    __flash_ext_turnon();
    addr = SpicGetExtendAddrRtl8195A(flashobj.SpicInitPara);
    __flash_ext_turnoff();
    return addr;
}
/*
Function Description:
This function is only for Micron 512Mbit flash to access beyond 128Mbit by switching between four 128 Mbit area.
Please refer to flash datasheet for more information about memory mapping.
*/

int flash_ext_set_extend_addr(flash_t *obj, uint32_t data)
{
    __flash_ext_turnon();
    SpicSetExtendAddrRtl8195A(data, flashobj.SpicInitPara);
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    __flash_ext_turnoff();
    return 0;
}


