/**********************************************************************
 *
 * Filename:    flash_journal_crc.c
 *
 * Description: Slow and fast implementations of the CRC standards.
 *
 * Notes:       The parameters for each supported CRC standard are
 *              defined in the header file crc.h.  The implementations
 *              here should stand up to further additions to that list.
 *
 *
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#include "flash-journal-strategy-sequential/flash_journal_crc.h"

#define FALSE   0
#define TRUE    !FALSE

#define CRC_NAME            "CRC-32"
#define POLYNOMIAL          0x04C11DB7
#define INITIAL_REMAINDER   0xFFFFFFFF
#define FINAL_XOR_VALUE     0xFFFFFFFF
#define REFLECT_DATA        TRUE
#define REFLECT_REMAINDER   TRUE
#define CHECK_VALUE         0xCBF43926


/*
 * Derive parameters from the standard-specific parameters in crc.h.
 */
#define WIDTH    (8 * sizeof(flash_journal_crc32_t))
/* U postfix required to suppress the following warning with TOOLCHAIN_ARM:
 * #61-D: integer operation result is out of range
 */
#define TOPBIT   0x80000000U

#if (REFLECT_DATA == TRUE)
#undef  REFLECT_DATA
#define REFLECT_DATA(X)         ((unsigned char) reflect((X), 8))
#else
#undef  REFLECT_DATA
#define REFLECT_DATA(X)         (X)
#endif

#if (REFLECT_REMAINDER == TRUE)
#undef  REFLECT_REMAINDER
#define REFLECT_REMAINDER(X)    ((flash_journal_crc32_t) reflect((X), WIDTH))
#else
#undef  REFLECT_REMAINDER
#define REFLECT_REMAINDER(X)    (X)
#endif


/*********************************************************************
 *
 * Function:    reflect()
 *
 * Description: Reorder the bits of a binary sequence, by reflecting
 *              them about the middle position.
 *
 * Notes:       No checking is done that nBits <= 32.
 *
 * Returns:     The reflection of the original data.
 *
 *********************************************************************/
static uint32_t
reflect(uint32_t data, unsigned char nBits)
{
    uint32_t  reflection = 0x00000000;
    unsigned char  bit;

    /*
     * Reflect the data about the center bit.
     */
    for (bit = 0; bit < nBits; ++bit)
    {
        /*
         * If the LSB bit is set, set the reflection of it.
         */
        if (data & 0x01)
        {
            reflection |= (1 << ((nBits - 1) - bit));
        }

        data = (data >> 1);
    }

    return (reflection);

}   /* reflect() */


static flash_journal_crc32_t  crcTable[256];
static flash_journal_crc32_t  crcEngineRemainder = INITIAL_REMAINDER;

/*********************************************************************
 *
 * Function:    flashJournalCrcInit()
 *
 * Description: Populate the partial CRC lookup table.
 *
 * Notes:       This function must be rerun any time the CRC standard
 *              is changed.  If desired, it can be run "offline" and
 *              the table results stored in an embedded system's ROM.
 *
 * Returns:     None defined.
 *
 *********************************************************************/
void
flashJournalCrcInit(void)
{
    flash_journal_crc32_t            remainder;
    int            dividend;
    unsigned char  bit;


    /*
     * Compute the remainder of each possible dividend.
     */
    for (dividend = 0; dividend < 256; ++dividend)
    {
        /*
         * Start with the dividend followed by zeros.
         */
        remainder = dividend << (WIDTH - 8);

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
         * Store the result into the table.
         */
        crcTable[dividend] = remainder;
    }

}   /* flashJournalCrcInit() */

/*********************************************************************
 *
 * Function:    flashJournalCrcReset()
 *
 * Description: Resets internal state before calling crcCummulative().
 *
 * Notes: See the notes to crcCummulative().
 *
 * Returns:     None defined.
 *
 *********************************************************************/
void
flashJournalCrcReset(void)
{
    static unsigned initCalled = 0;
    if (!initCalled) {
        flashJournalCrcInit();
        initCalled = 1;
    }

    crcEngineRemainder = INITIAL_REMAINDER;

}   /* flashJournalCrcReset() */

/*********************************************************************
 *
 * Function:    crcCummulative()
 *
 * Description: Compute the CRC of a group of messages.
 *
 * Notes:
 * This function is intended to be used in the following way:
 *  - crcReset() is called first to reset internal state before the first
 *    fragment of a new message is processed with crcCummulative().
 *  - crcCummulative() called successfully appending additional message
 *    fragments to those previously supplied (in order), and returning
 *    the current crc for the message payload so far.
 *
 * Returns:     The CRC of the message.
 *
 *********************************************************************/
flash_journal_crc32_t
flashJournalCrcCummulative(unsigned char const message[], int nBytes)
{
    unsigned char  data;
    int            byte;


    /*
     * Divide the message by the polynomial, a byte at a time.
     */
    for (byte = 0; byte < nBytes; ++byte)
    {
        data = REFLECT_DATA(message[byte]) ^ (crcEngineRemainder >> (WIDTH - 8));
        crcEngineRemainder = crcTable[data] ^ (crcEngineRemainder << 8);
    }

    /*
     * The final remainder is the CRC.
     */
    return (REFLECT_REMAINDER(crcEngineRemainder) ^ FINAL_XOR_VALUE);

}   /* crcCummulative() */

