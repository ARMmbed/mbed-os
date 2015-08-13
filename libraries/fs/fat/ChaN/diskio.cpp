/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#include "ffconf.h"
#include "diskio.h"

#include "mbed_debug.h"
#include "FATFileSystem.h"

using namespace mbed;

DSTATUS disk_initialize (
    BYTE pdrv        /* Physical drive number (0..) */
)
{
    debug_if(FFS_DBG, "disk_initialize on pdrv [%d]\n", pdrv);
    return (DSTATUS)FATFileSystem::_ffs[pdrv]->disk_initialize();
}

DSTATUS disk_status (
    BYTE pdrv        /* Physical drive number (0..) */
)
{
    debug_if(FFS_DBG, "disk_status on pdrv [%d]\n", pdrv);
    return (DSTATUS)FATFileSystem::_ffs[pdrv]->disk_status();
}

DRESULT disk_read (
    BYTE pdrv,       /* Physical drive number (0..) */
    BYTE* buff,      /* Data buffer to store read data */
    DWORD sector,    /* Sector address (LBA) */
    UINT count       /* Number of sectors to read (1..) */
)
{
    debug_if(FFS_DBG, "disk_read(sector %d, count %d) on pdrv [%d]\n", sector, count, pdrv);
    if (FATFileSystem::_ffs[pdrv]->disk_read((uint8_t*)buff, sector, count))
        return RES_PARERR;
    else
        return RES_OK;
}

#if _READONLY == 0
DRESULT disk_write (
    BYTE pdrv,           /* Physical drive number (0..) */
    const BYTE* buff,    /* Data to be written */
    DWORD sector,        /* Sector address (LBA) */
    UINT count           /* Number of sectors to write (1..) */
)
{
    debug_if(FFS_DBG, "disk_write(sector %d, count %d) on pdrv [%d]\n", sector, count, pdrv);
    if (FATFileSystem::_ffs[pdrv]->disk_write((uint8_t*)buff, sector, count))
        return RES_PARERR;
    else
        return RES_OK;
}
#endif /* _READONLY */

DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive number (0..) */
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
