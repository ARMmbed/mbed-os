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
 * This file contains symbols used by the filesystem implementation that are not present
 * in the armcc errno.h, or sys/stat.h, for example.
 *
 */

#ifndef FILESYSTEM_FAT_ERRNO_ARMCC_H
#define FILESYSTEM_FAT_ERRNO_ARMCC_H

//#ifdef TOOLCHAIN_ARM_STD
#if defined TOOLCHAIN_ARM_STD || defined TOOLCHAIN_IAR
/* The following are errno.h definitions not currently present in the ARMCC
 * errno.h. Note, ARMCC errno.h defines some symbols values differing from
 * GCC_ARM/IAR/standard POSIX definitions.Guard against this and future
 * changes by changing the symbol definition for filesystem use. */
#ifdef ENOENT
#undef ENOENT
#endif
#define ENOENT      2       /* No such file or directory. */

#ifdef EIO
#undef EIO
#endif
#define EIO         5       /* I/O error */

#ifdef ENXIO
#undef ENXIO
#endif
#define ENXIO       6       /* No such device or address */

#ifdef EBADF
#undef EBADF
#endif
#define EBADF       9       /* Bad file number */

#ifdef ENOMEM
#undef ENOMEM
#endif
#define ENOMEM      12      /* Not enough space */

#ifdef EACCES
#undef EACCES
#endif
#define EACCES      13      /* Permission denied */

#ifdef EFAULT
#undef EFAULT
#endif
#define EFAULT      14      /* Bad address */

#ifdef EEXIST
#undef EEXIST
#endif
#define EEXIST      17      /* File exists */

#ifdef ENFILE
#undef ENFILE
#endif
#define ENFILE      23      /* Too many open files in system */

#ifdef EMFILE
#undef EMFILE
#endif
#define EMFILE      24      /* File descriptor value too large */

/* Missing stat.h defines */
/* The following are sys/stat.h definitions not currently present in the ARMCC
 * errno.h. Note, ARMCC errno.h defines some symbols values differing from
 * GCC_ARM/IAR/standard POSIX definitions.Guard against this and future
 * changes by changing the symbol definition for filesystem use. */
#define     _IFDIR  0040000 /* directory */
#define     _IFREG  0100000 /* regular */

#define S_IFDIR     _IFDIR
#define S_IFREG     _IFREG

#define S_IRWXU     (S_IRUSR | S_IWUSR | S_IXUSR)
#define     S_IRUSR 0000400 /* read permission, owner */
#define     S_IWUSR 0000200 /* write permission, owner */
#define     S_IXUSR 0000100/* execute/search permission, owner */
#define S_IRWXG     (S_IRGRP | S_IWGRP | S_IXGRP)
#define     S_IRGRP 0000040 /* read permission, group */
#define     S_IWGRP 0000020 /* write permission, grougroup */
#define     S_IXGRP 0000010/* execute/search permission, group */
#define S_IRWXO     (S_IROTH | S_IWOTH | S_IXOTH)
#define     S_IROTH 0000004 /* read permission, other */
#define     S_IWOTH 0000002 /* write permission, other */
#define     S_IXOTH 0000001/* execute/search permission, other */

#endif /* TOOLCHAIN_ARM_STD */

#endif /* FILESYSTEM_FAT_ERRNO_ARMCC_H */
