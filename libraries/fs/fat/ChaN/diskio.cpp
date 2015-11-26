/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "mbed_debug.h"
#include "FATFileSystem.h"

using namespace mbed;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv        /* Physical drive nmuber to identify the drive */
)
{
    debug_if(FFS_DBG, "disk_status on pdrv [%d]\n", pdrv);
    return (DSTATUS)FATFileSystem::_ffs[pdrv]->disk_status();
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv        /* Physical drive nmuber to identify the drive */
)
{
    debug_if(FFS_DBG, "disk_initialize on pdrv [%d]\n", pdrv);
    return (DSTATUS)FATFileSystem::_ffs[pdrv]->disk_initialize();
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,       /* Physical drive nmuber to identify the drive */
    BYTE* buff,      /* Data buffer to store read data */
    DWORD sector,    /* Sector address in LBA */
    UINT count       /* Number of sectors to read */
)
{
    debug_if(FFS_DBG, "disk_read(sector %d, count %d) on pdrv [%d]\n", sector, count, pdrv);
    if (FATFileSystem::_ffs[pdrv]->disk_read((uint8_t*)buff, sector, count))
        return RES_PARERR;
    else
        return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
    BYTE pdrv,           /* Physical drive nmuber to identify the drive */
    const BYTE* buff,    /* Data to be written */
    DWORD sector,        /* Sector address in LBA */
    UINT count           /* Number of sectors to write */
)
{
    debug_if(FFS_DBG, "disk_write(sector %d, count %d) on pdrv [%d]\n", sector, count, pdrv);
    if (FATFileSystem::_ffs[pdrv]->disk_write((uint8_t*)buff, sector, count))
        return RES_PARERR;
    else
        return RES_OK;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,         /* Control code */
    void* buff        /* Buffer to send/receive control data */
)
{
    debug_if(FFS_DBG, "disk_ioctl(%d)\n", cmd);
    switch(cmd) {
        case CTRL_SYNC:
            if(FATFileSystem::_ffs[pdrv] == NULL) {
                return RES_NOTRDY;
            } else if(FATFileSystem::_ffs[pdrv]->disk_sync()) {
                return RES_ERROR;
            }
            return RES_OK;
        case GET_SECTOR_COUNT:
            if(FATFileSystem::_ffs[pdrv] == NULL) {
                return RES_NOTRDY;
            } else {
                DWORD res = FATFileSystem::_ffs[pdrv]->disk_sectors();
                if(res > 0) {
                    *((DWORD*)buff) = res; // minimum allowed
                    return RES_OK;
                } else {
                    return RES_ERROR;
                }
            }
        case GET_BLOCK_SIZE:
            *((DWORD*)buff) = 1; // default when not known
            return RES_OK;

    }
    return RES_PARERR;
}
#endif
