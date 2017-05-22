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
#include "hal_spi_flash.h"
#include "hal_platform.h"
#include "rtl8195a_spi_flash.h"
#include "hal_api.h"
#include "flash_api.h"

extern u32 ConfigDebugInfo;
extern SPIC_INIT_PARA SpicInitParaAllClk[3][CPU_CLK_TYPE_NO];

_LONG_CALL_
extern VOID SpicWaitBusyDoneRtl8195A(VOID);

static int isinit = 0;
static flash_t flashobj;
    
static void flash_init(flash_t * obj);
static void flash_turnon();
/**
  * global data structure
  */   
//flash_t            flash;

/**
  * @brief  Control the flash chip write protect enable/disable
  * @param  protect: 1/0: protect/unprotect
  * @retval none
  */   
void flash_write_protect(flash_t *obj, uint32_t protect)
{
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);
    SpicWriteProtectFlashRtl8195A(protect);
    SpicDisableRtl8195A();
}

/**
  * @brief  Init Flash
  * @param  obj: address of the flash object
  * @retval   none
  */
void flash_init(flash_t *obj)
{
    //SPIC_INIT_PARA spic_init_para;

    // Init SPI Flash Controller
//    DBG_8195A("Initial Spi Flash Controller\n");
    //SPI_FLASH_PIN_FCTRL(ON);

    if (!SpicFlashInitRtl8195A(SpicOneBitMode)){

        DBG_8195A("SPI Init Fail!!!!!!\n");
        HAL_WRITE32(SYSTEM_CTRL_BASE, REG_SYS_DSTBY_INFO3, HAL_READ32(SYSTEM_CTRL_BASE, REG_SYS_DSTBY_INFO3)|0xf);
    } else {
        isinit = 1;
    }
    flashobj.SpicInitPara.flashtype = SpicInitParaAllClk[0][0].flashtype;
    
}
void flash_turnon()
{
    SPI_FLASH_PIN_FCTRL(ON);  
    SpicWaitBusyDoneRtl8195A();
}

/**
  * @brief  Erase flash sector
  * @param  address: Specifies the starting address to be erased.
  * @retval   none
  */
void flash_erase_sector(flash_t *obj, uint32_t address)
{
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    SpicSectorEraseFlashRtl8195A(SPI_FLASH_BASE + address);
    SpicDisableRtl8195A();
}

void flash_erase_block(flash_t *obj, uint32_t address)
{
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    SpicBlockEraseFlashRtl8195A(SPI_FLASH_BASE + address);
    SpicDisableRtl8195A();
}


/**
  * @brief  Read a word from specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  data: Specified the address to save the readback data.
  * @retval   status: Success:1 or Failure: Others.
  */
int  flash_read_word(flash_t *obj, uint32_t address, uint32_t * data)
{
    
    flash_turnon();
    if(isinit == 0)
        flash_init(&flashobj);
    // Wait flash busy done (wip=0)
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

    * data = HAL_READ32(SPI_FLASH_BASE, address);
    SpicDisableRtl8195A();

    return 1;
}

/**
  * @brief  Write a word to specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be programmed.
  * @param  data: Specified the data to be programmed.
  * @retval   status: Success:1 or Failure: Others.
  */
int  flash_write_word(flash_t *obj, uint32_t address, uint32_t data)
{
    u8 flashtype = 0;
    
    flash_turnon();
    if(isinit == 0)
        flash_init(&flashobj);


    flashtype = flashobj.SpicInitPara.flashtype;

    //Write word
    HAL_WRITE32(SPI_FLASH_BASE, address, data);

    // Wait spic busy done
    SpicWaitBusyDoneRtl8195A();
    
    // Wait flash busy done (wip=0)
    if(flashtype == FLASH_MICRON){
        SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
    } else
        SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    
    SpicDisableRtl8195A();
    return 1;
}


/**
  * @brief  Read a stream of data from specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  len: Specifies the length of the data to read.
  * @param  data: Specified the address to save the readback data.
  * @retval   status: Success:1 or Failure: Others.
  */
int  flash_stream_read(flash_t *obj, uint32_t address, uint32_t len, uint8_t * data)
{
    u32 offset_to_align;
    u32 i;
    u32 read_word;
    uint8_t *ptr;
    uint8_t *pbuf;
    
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);


    // Wait flash busy done (wip=0)
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

    offset_to_align = address & 0x03;
    pbuf = data;
    if (offset_to_align != 0) {
        // the start address is not 4-bytes aligned
        read_word = HAL_READ32(SPI_FLASH_BASE, (address - offset_to_align));
        ptr = (uint8_t*)&read_word + offset_to_align;
        offset_to_align = 4 - offset_to_align;
        for (i=0;i<offset_to_align;i++) {
            *pbuf = *(ptr+i);
            pbuf++;
            len--;
            if (len == 0) {
                break;
            }
        }
    }

    address = (((address-1) >> 2) + 1) << 2;    // address = next 4-bytes aligned

    ptr = (uint8_t*)&read_word;
    if ((u32)pbuf & 0x03) {
        while (len >= 4) {
            read_word = HAL_READ32(SPI_FLASH_BASE, address);
            for (i=0;i<4;i++) {
                *pbuf = *(ptr+i);
                pbuf++;
            }
            address += 4;
            len -= 4;
        }
    } else {
        while (len >= 4) {
            *((u32 *)pbuf) = HAL_READ32(SPI_FLASH_BASE, address);
            pbuf += 4;
            address += 4;
            len -= 4;
        }
    }

    if (len > 0) {
        read_word = HAL_READ32(SPI_FLASH_BASE, address);
        for (i=0;i<len;i++) {
            *pbuf = *(ptr+i);
            pbuf++;
        }        
    }

    SpicDisableRtl8195A();
    return 1;
}

/**
  * @brief  Write a stream of data to specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  len: Specifies the length of the data to write.
  * @param  data: Specified the pointer of the data to be written.
  * @retval   status: Success:1 or Failure: Others.
  */
int  flash_stream_write(flash_t *obj, uint32_t address, uint32_t len, uint8_t * data)
{
    u32 offset_to_align;
    u32 align_addr;
    u32 i;
    u32 write_word;
    uint8_t *ptr;
    uint8_t *pbuf;
    u8 flashtype = 0; 
    
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);
  
    flashtype = flashobj.SpicInitPara.flashtype;
    offset_to_align = address & 0x03;
    pbuf = data;
    if (offset_to_align != 0) {
        // the start address is not 4-bytes aligned
        align_addr = (address - offset_to_align);
        write_word = HAL_READ32(SPI_FLASH_BASE, align_addr);        
        ptr = (uint8_t*)&write_word + offset_to_align;
        offset_to_align = 4 - offset_to_align;
        for (i=0;i<offset_to_align;i++) {
            *(ptr+i) = *pbuf;
            pbuf++;
            len--;
            if (len == 0) {
                break;
            }
        }
        //Write word
        HAL_WRITE32(SPI_FLASH_BASE, align_addr, write_word);
        // Wait spic busy done
        SpicWaitBusyDoneRtl8195A();
        // Wait flash busy done (wip=0)
        if(flashtype == FLASH_MICRON){
            SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
        } else
            SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
        

    }

    address = (((address-1) >> 2) + 1) << 2;    // address = next 4-bytes aligned

    if ((u32)pbuf & 0x03) {
        while (len >= 4) {
            write_word = (u32)(*pbuf) | ((u32)(*(pbuf+1)) << 8) | ((u32)(*(pbuf+2)) << 16) | ((u32)(*(pbuf+3)) << 24);
            //Write word
            HAL_WRITE32(SPI_FLASH_BASE, address, write_word);
            // Wait spic busy done
            SpicWaitBusyDoneRtl8195A();
            // Wait flash busy done (wip=0)
            if(flashtype == FLASH_MICRON){
                SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
            } else
                SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

            pbuf += 4;
            address += 4;
            len -= 4;
        }
    } else {
        while (len >= 4) {
            //Write word
            HAL_WRITE32(SPI_FLASH_BASE, address, (u32)*((u32 *)pbuf));
            // Wait spic busy done
            SpicWaitBusyDoneRtl8195A();
            // Wait flash busy done (wip=0)
            if(flashtype == FLASH_MICRON){
                SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
            } else
                SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

            pbuf += 4;
            address += 4;
            len -= 4;
        }
    }

    if (len > 0) {
        write_word = HAL_READ32(SPI_FLASH_BASE, address);
        ptr = (uint8_t*)&write_word;
        for (i=0;i<len;i++) {
            *(ptr+i) = *pbuf;
            pbuf++;
        }
        //Write word
        HAL_WRITE32(SPI_FLASH_BASE, address, write_word);
        // Wait spic busy done
        SpicWaitBusyDoneRtl8195A();
        // Wait flash busy done (wip=0)
        if(flashtype == FLASH_MICRON){
            SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
        } else
            SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

    }

    SpicDisableRtl8195A();
    return 1;
}


/*
Function Description:
This function performans the same functionality as the function flash_stream_write.
It enhances write performance by reducing overheads.
Users can use either of functions depending on their needs.

* @brief  Write a stream of data to specified address
* @param  obj: Specifies the parameter of flash object.
* @param  address: Specifies the address to be read.
* @param  Length: Specifies the length of the data to write.
* @param  data: Specified the pointer of the data to be written.
* @retval   status: Success:1 or Failure: Others.

*/

int flash_burst_write(flash_t *obj, uint32_t address ,uint32_t Length, uint8_t * data)
{

    u32 OccuSize;
    u32 ProgramSize;
    u32 PageSize;
    u8 flashtype = 0;

    PageSize = 256;

    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    flashtype = flashobj.SpicInitPara.flashtype;
    
    OccuSize = address & 0xFF;
    if((Length >= PageSize) ||((Length + OccuSize) >= PageSize)){
        ProgramSize = PageSize - OccuSize;
    } else {
        ProgramSize = Length;
    }

    flashobj.Length = Length;
    while(Length > 0){
        if(OccuSize){
            SpicUserProgramRtl8195A(data, flashobj.SpicInitPara, address, &(flashobj.Length));
                // Wait spic busy done
            SpicWaitBusyDoneRtl8195A();
            // Wait flash busy done (wip=0)
            if(flashtype == FLASH_MICRON){
                SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
            } else
                SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

            address += ProgramSize;
            data+= ProgramSize;   
            Length -= ProgramSize;
            OccuSize = 0;
        } else{
            while((flashobj.Length) >= PageSize){
                SpicUserProgramRtl8195A(data, flashobj.SpicInitPara, address, &(flashobj.Length));
                    // Wait spic busy done
                SpicWaitBusyDoneRtl8195A();
                // Wait flash busy done (wip=0)
                if(flashtype == FLASH_MICRON){
                    SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
                } else
                    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

                address += PageSize;
                data+=PageSize;
                Length -= PageSize;
            }
            flashobj.Length = Length;
            if((flashobj.Length)  > 0){
                SpicUserProgramRtl8195A(data, flashobj.SpicInitPara, address, &(flashobj.Length));
                    // Wait spic busy done
                SpicWaitBusyDoneRtl8195A();
                // Wait flash busy done (wip=0)
                if(flashtype == FLASH_MICRON){
                    SpicWaitOperationDoneRtl8195A(flashobj.SpicInitPara);
                } else
                    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);

                break;
            }
        }
        flashobj.Length = Length;
    }
    
    SpicDisableRtl8195A();
    return 1;
}
/**
  * @brief  Read a stream of data from specified address
  * @param  obj: Specifies the parameter of flash object.
  * @param  address: Specifies the address to be read.
  * @param  len: Specifies the length of the data to read.
  * @param  data: Specified the address to save the readback data.
  * @retval   status: Success:1 or Failure: Others.
  */

int  flash_burst_read(flash_t *obj, uint32_t address, uint32_t Length, uint8_t * data)
{
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    // Wait flash busy done (wip=0)
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    SpicUserReadRtl8195A(Length, address, data, SpicOneBitMode);
    SpicDisableRtl8195A();
    return 1;
}

int flash_get_status(flash_t *obj)
{
    u8 Status = 0;

    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    Status = SpicGetFlashStatusRefinedRtl8195A(flashobj.SpicInitPara);

    SpicDisableRtl8195A();
    return Status;
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
int flash_set_status(flash_t *obj, uint32_t data)
{
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    SpicSetFlashStatusRefinedRtl8195A(data, flashobj.SpicInitPara);
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    DBG_8195A("Status Register After Setting= %x\n", flash_get_status(&flashobj));
    SpicDisableRtl8195A();
    return 1;
}

/*
Function Description:
This function aims to reset the status register, please make sure the operation is appropriate.
*/
void flash_reset_status(flash_t *obj)
{
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    SpicSetFlashStatusRefinedRtl8195A(0, flashobj.SpicInitPara);    
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    SpicDisableRtl8195A();
}
/*
Function Description:
This function is only for Micron 512Mbit flash to access beyond 128Mbit by switching between four 128 Mbit area.
Please refer to flash datasheet for more information about memory mapping.
*/

int flash_set_extend_addr(flash_t *obj, uint32_t data)
{
    flash_turnon();

    if(isinit == 0)
        flash_init(&flashobj);

    SpicSetExtendAddrRtl8195A(data, flashobj.SpicInitPara);
    SpicWaitWipDoneRefinedRtl8195A(flashobj.SpicInitPara);
    DBG_8195A("Extended Address Register After Setting= %x\n", flash_get_extend_addr(&flashobj));
    SpicDisableRtl8195A();
    return 1;
}

int flash_get_extend_addr(flash_t *obj)
{
    u8 Status = 0;
    
    flash_turnon();    
    if(isinit == 0)
        flash_init(&flashobj);
    Status = SpicGetExtendAddrRtl8195A(flashobj.SpicInitPara);
    
    SpicDisableRtl8195A();
    return Status;
}


