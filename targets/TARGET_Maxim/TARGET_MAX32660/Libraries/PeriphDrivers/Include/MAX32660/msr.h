/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*
********************************************************************************
*/

/**
* \file
* \brief    Magnetic Stripe Reader driver
* \details  This driver can be used to configure and operate the Magnetic Stripe
*           Reader. It reads and decodes magnetic stripe data that is encoded
*           according to the ISO/IEC standard 7811.
* \details  This file defines the driver API including data types and function
*           prototypes.
*/

#ifndef _MSR_H_
#define _MSR_H_

/***** Definitions *****/

/// Number of tracks on a card
#ifndef MSR_NUM_TRACKS
#define MSR_NUM_TRACKS      3
#endif

#define MSR_MAX_SAMPLES     1536

// Assuming nominal bit density of 210 bpi and 3.375 inch length
#define MSR_MAX_RAW_LEN_BITS      (709)
#define MSR_MAX_RAW_LEN_BYTES     ((MSR_MAX_RAW_LEN_BITS + 7) / 8)
#define MSR_MAX_RAW_LEN_HALFWORDS ((MSR_MAX_RAW_LEN_BITS + 15) / 16)

/// Maximum size in bytes of decoded track characters (5-bit min to 8-bit max)
#define MSR_MAX_DEC_LEN   (MSR_MAX_RAW_LEN_BITS / 5)

/// Swipe direction: the card was swiped in the forward direction
#define MSR_FORWARD     0
/// Swipe direction: the card was swiped in the reverse direction
#define MSR_REVERSE     1

/// Error codes
#define MSR_ERR_OK          0x00
#define MSR_ERR_BAD_LEN     0x01
#define MSR_ERR_START_SEN   0x02
#define MSR_ERR_END_SEN     0x04
#define MSR_ERR_OUTLIER     0x08
#define MSR_ERR_PARAM       0x10
#define MSR_ERR_LRC         0x40
#define MSR_ERR_PARITY      0x80

/// Structure to contain result of a track decode
typedef struct {
    uint8_t error_code;   /**< Error code value */
    uint8_t parity_errs;  /**< Number of characters with parity errors */
    uint8_t lrc;          /**< LRC check value. A value of '0' indicates a
                             successful LRC check. Any other value should be
                             considered a failure. */
    uint8_t direction;    /**< Swipe direction determined from decode */
    uint8_t len;          /**< Number or decoded characters. This does not include
                             the sentinels or the LRC. */
    uint16_t speed;
    uint8_t data[MSR_MAX_DEC_LEN];  /**< The decoded data */
} msr_decoded_track_t;

/// MSR sample fields
typedef union {
    struct {
        uint16_t time : 9;
        uint16_t amp  : 7;
    };
    uint16_t value;
} msr_sample_t;

/// Structure to contain raw MSR samples
typedef struct {
    uint16_t len;
    msr_sample_t data[MSR_MAX_SAMPLES];
} msr_samples_t;

/***** Function Prototypes *****/

/**
*   \brief    Initializes magnetic card reader hardware
*   \returns #E_NO_ERROR if everything is successful
*/
int msr_init (void);

/**
*   \brief    Initializes specified track
*   \param    track   track number (1 to 3)
*/
void msr_init_track (unsigned int track);

/**
*   \brief    Enables magnetic card reader
*   \pre      The reader should be initialized by calling msr_init() and then
*             waiting at least 100 us before calling this function.
*/
void msr_enable (void);

/**
*   \brief    Disables magnetic card reader
*/
void msr_disable (void);

/**
*   \brief    Task used to execute driver functionality.
*   \details  This function executes the internal driver functionality that
*             processes MSR events and reads stripe data. This function is used
*             when MSR interrupt servicing is disabled.
*   \returns  1 if all tracking reading is complete, 0 otherwise
*/
int msr_task (void);

/**
*   \brief    Decodes the specified track of data
*   \param    track           track number (1 to 3)
*   \param    decoded_track   track decode results
*   \returns  number of characters decoded
*   \note     This function has significant stack usage.
*/
unsigned int msr_track_decode (unsigned int track, msr_decoded_track_t * decoded_track);

/**
*   \brief    Registers an application callback function
*   \details  The callback function will be called after completion of the read
*             of all enabled card tracks
*   \details  Unregistering of the callback can be performed by calling this
*             function function with a NULL parameter.
*   \param    func  application callback function
*/
void msr_set_complete_callback (void (*func) (void));

/**
*   \brief    Retrieves the raw (undecoded) sample data for the specified track
*             of data
*   \param    track       track number (1 to 3)
*   \param    samples     pointer to where the sample data will be copied
*   \returns  number of samples retrieved
*/
unsigned int mcr_get_track_samples (unsigned int track, msr_samples_t * samples);

#endif /* _MSR_H_ */

