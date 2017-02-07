/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 * This file contains miscellaneous functionality used by the FAT filesystem interface code.
 */

#include "FATMisc.h"
#include <errno.h>
#include "platform/retarget.h"


/* @brief   Set errno based on the error code returned from underlying filesystem
 *
 * @param   res     result returned from underlying filesystem
 *
 * @return  No return value
 */
void fat_filesystem_set_errno(FRESULT res)
{
    switch(res) {
        case FR_OK:                     /* (0) Succeeded */
            errno = 0;                  /* no error */
            break;
        case FR_DISK_ERR:               /* (1) A hard error occurred in the low level disk I/O layer */
        case FR_NOT_READY:              /* (3) The physical drive cannot work */
            errno = EIO;                /* I/O error */
            break;
        case FR_NO_FILE:                /* (4) Could not find the file */
        case FR_NO_PATH:                /* (5) Could not find the path */
        case FR_INVALID_NAME:           /* (6) The path name format is invalid */
        case FR_INVALID_DRIVE:          /* (11) The logical drive number is invalid */
        case FR_NO_FILESYSTEM:          /* (13) There is no valid FAT volume */
            errno = ENOENT;             /* No such file or directory */
            break;
        case FR_DENIED:                 /* (7) Access denied due to prohibited access or directory full */
            errno = EACCES;             /* Permission denied */
            break;
        case FR_EXIST:                  /* (8) Access denied due to prohibited access */
            errno = EEXIST;             /* File exists */
            break;
        case FR_WRITE_PROTECTED:        /* (10) The physical drive is write protected */
        case FR_LOCKED:                 /* (16) The operation is rejected according to the file sharing policy */
            errno = EACCES;             /* Permission denied */
            break;
        case FR_INVALID_OBJECT:         /* (9) The file/directory object is invalid */
            errno = EFAULT;             /* Bad address */
            break;
        case FR_NOT_ENABLED:            /* (12) The volume has no work area */
            errno = ENXIO;              /* No such device or address */
            break;
        case FR_NOT_ENOUGH_CORE:        /* (17) LFN working buffer could not be allocated */
            errno = ENOMEM;             /* Not enough space */
            break;
        case FR_TOO_MANY_OPEN_FILES:    /* (18) Number of open files > _FS_LOCK */
            errno = ENFILE;             /* Too many open files in system */
            break;
        case FR_INVALID_PARAMETER:      /* (19) Given parameter is invalid */
            errno = ENOEXEC;            /* Exec format error */
            break;
        case FR_INT_ERR:                /* (2) Assertion failed */
        case FR_MKFS_ABORTED:           /* (14) The f_mkfs() aborted due to any parameter error */
        case FR_TIMEOUT:                /* (15) Could not get a grant to access the volume within defined period */
        default:
            errno = EBADF;              /* Bad file number */
            break;
    }
    return;
}

