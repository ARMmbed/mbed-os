/**********************************************************************
 *
 * Filename:    flash_journal_crc.h.h
 *
 * Description: A header file describing the various CRC standards.
 *
 * Notes:
 *
 *
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#ifndef _flash_journal_crc_h
#define _flash_journal_crc_h

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t  flash_journal_crc32_t;

void flashJournalCrcReset(void);
flash_journal_crc32_t flashJournalCrcCummulative(unsigned char const message[], int nBytes);

#ifdef __cplusplus
}
#endif

#endif /* _flash_journal_crc_h */
