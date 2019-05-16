/*==============================================================================
 Copyright (c) 2016-2018, The Linux Foundation.
 Copyright (c) 2018-2019, Laurence Lundblade.
 All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors, nor the name "Laurence Lundblade" may be used to
      endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================*/


/*===================================================================================
 FILE:  qcbor.h

 DESCRIPTION:  This is the full public API and data structures for QCBOR

 EDIT HISTORY FOR FILE:

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

 when               who             what, where, why
 --------           ----            ---------------------------------------------------
 12/18/18           llundblade      Move decode malloc optional code to separate repository
 12/13/18           llundblade      Documentatation improvements
 11/29/18           llundblade      Rework to simpler handling of tags and labels.
 11/9/18            llundblade      Error codes are now enums.
 11/1/18            llundblade      Floating support.
 10/31/18           llundblade      Switch to one license that is almost BSD-3.
 10/15/18           llundblade      Indefinite length maps and arrays supported
 10/8/18            llundblade      Indefinite length strings supported
 09/28/18           llundblade      Added bstr wrapping feature for COSE implementation.
 07/05/17           llundbla        Add bstr wrapping of maps/arrays for COSE.
 03/01/17           llundbla        More data types; decoding improvements and fixes.
 11/13/16           llundbla        Integrate most TZ changes back into github version.
 09/30/16           gkanike         Porting to TZ.
 03/15/16           llundbla        Initial Version.

 =====================================================================================*/

#ifndef __QCBOR__qcbor__
#define __QCBOR__qcbor__

/*...... This is a ruler that is 80 characters long...........................*/

/* ===========================================================================
   BEGINNING OF PRIVATE PART OF THIS FILE

   Caller of QCBOR should not reference any of the details below up until
   the start of the public part.
   =========================================================================== */

/*
 Standard integer types are used in the interface to be precise about
 sizes to be better at preventing underflow/overflow errors.
 */
#include <stdint.h>
#include <stdbool.h>
#include "UsefulBuf.h"


/*
 The maxium nesting of arrays and maps when encoding or decoding.
 (Further down in the file there is a definition that refers to this
 that is public. This is done this way so there can be a nice
 separation of public and private parts in this file.
*/
#define QCBOR_MAX_ARRAY_NESTING1 15 // Do not increase this over 255


/* The largest offset to the start of an array or map. It is slightly
 less than UINT32_MAX so the error condition can be tests on 32-bit machines.
 UINT32_MAX comes from uStart in QCBORTrackNesting being a uin32_t.

 This will cause trouble on a machine where size_t is less than 32-bits.
 */
#define QCBOR_MAX_ARRAY_OFFSET  (UINT32_MAX - 100)

/*
 PRIVATE DATA STRUCTURE

 Holds the data for tracking array and map nesting during encoding. Pairs up with
 the Nesting_xxx functions to make an "object" to handle nesting encoding.

 uStart is a uint32_t instead of a size_t to keep the size of this
 struct down so it can be on the stack without any concern.  It would be about
 double if size_t was used instead.

 Size approximation (varies with CPU/compiler):
    64-bit machine: (15 + 1) * (4 + 2 + 1 + 1 pad) + 8 = 136 bytes
    32-bit machine: (15 + 1) * (4 + 2 + 1 + 1 pad) + 4 = 132 bytes
*/
typedef struct __QCBORTrackNesting {
   // PRIVATE DATA STRUCTURE
   struct {
      // See function OpenArrayInternal() for detailed comments on how this works
      uint32_t  uStart;     // uStart is the byte position where the array starts
      uint16_t  uCount;     // Number of items in the arrary or map; counts items in a map, not pairs of items
      uint8_t   uMajorType; // Indicates if item is a map or an array
   } pArrays[QCBOR_MAX_ARRAY_NESTING1+1], // stored state for the nesting levels
   *pCurrentNesting; // the current nesting level
} QCBORTrackNesting;


/*
 PRIVATE DATA STRUCTURE

 Context / data object for encoding some CBOR. Used by all encode functions to
 form a public "object" that does the job of encdoing.

 Size approximation (varies with CPU/compiler):
   64-bit machine: 27 + 1 (+ 4 padding) + 136 = 32 + 136 = 168 bytes
   32-bit machine: 15 + 1 + 132 = 148 bytes
*/
struct _QCBOREncodeContext {
   // PRIVATE DATA STRUCTURE
   UsefulOutBuf      OutBuf;  // Pointer to output buffer, its length and position in it
   uint8_t           uError;  // Error state
   QCBORTrackNesting nesting; // Keep track of array and map nesting
};


/*
 PRIVATE DATA STRUCTURE

 Holds the data for array and map nesting for decoding work. This structure
 and the DecodeNesting_xxx functions form an "object" that does the work
 for arrays and maps.

 Size approximation (varies with CPU/compiler):
   64-bit machine: 4 * 16 + 8 = 72
   32-bit machine: 4 * 16 + 4 = 68
 */
typedef struct __QCBORDecodeNesting  {
  // PRIVATE DATA STRUCTURE
   struct {
      uint16_t uCount;
      uint8_t  uMajorType;
   } pMapsAndArrays[QCBOR_MAX_ARRAY_NESTING1+1],
   *pCurrent;
} QCBORDecodeNesting;


/*
 PRIVATE DATA STRUCTURE

 The decode context. This data structure plus the public QCBORDecode_xxx
 functions form an "object" that does CBOR decoding.

 Size approximation (varies with CPU/compiler):
   64-bit machine: 32 + 1 + 1 + 6 bytes padding + 72 + 16 = 128 bytes
   32-bit machine: 16 + 1 + 1 + 2 bytes padding + 68 + 8  = 68 bytes
 */
struct _QCBORDecodeContext {
   // PRIVATE DATA STRUCTURE
   UsefulInputBuf InBuf;

   uint8_t        uDecodeMode;
   uint8_t        bStringAllocateAll;

   QCBORDecodeNesting nesting;

   // This is NULL or points to a QCBORStringAllocator. It is void
   // here because _QCBORDecodeContext is defined early in the
   // private part of this file and QCBORStringAllocat is defined
   // later in the public part of this file.
   void *pStringAllocator;

   // This is NULL or points to QCBORTagList.
   // It is type void for the same reason as above.
   const void *pCallerConfiguredTagList;
};

// Used internally in the impementation here
// Must not conflict with any of the official CBOR types
#define CBOR_MAJOR_NONE_TYPE_RAW  9
#define CBOR_MAJOR_NONE_TAG_LABEL_REORDER 10


/* ===========================================================================
   END OF PRIVATE PART OF THIS FILE

   BEGINNING OF PUBLIC PART OF THIS FILE
   =========================================================================== */



/* ===========================================================================
   BEGINNING OF CONSTANTS THAT COME FROM THE CBOR STANDARD, RFC 7049

   It is not necessary to use these directly when encoding or decoding
   CBOR with this implementation.
   =========================================================================== */

/* Standard CBOR Major type for positive integers of various lengths */
#define CBOR_MAJOR_TYPE_POSITIVE_INT 0

/* Standard CBOR Major type for negative integer of various lengths */
#define CBOR_MAJOR_TYPE_NEGATIVE_INT 1

/* Standard CBOR Major type for an array of arbitrary 8-bit bytes. */
#define CBOR_MAJOR_TYPE_BYTE_STRING  2

/* Standard CBOR Major type for a UTF-8 string. Note this is true 8-bit UTF8
 with no encoding and no NULL termination */
#define CBOR_MAJOR_TYPE_TEXT_STRING  3

/* Standard CBOR Major type for an ordered array of other CBOR data items */
#define CBOR_MAJOR_TYPE_ARRAY        4

/* Standard CBOR Major type for CBOR MAP. Maps an array of pairs. The
 first item in the pair is the "label" (key, name or identfier) and the second
 item is the value.  */
#define CBOR_MAJOR_TYPE_MAP          5

/* Standard CBOR optional tagging. This tags things like dates and URLs */
#define CBOR_MAJOR_TYPE_OPTIONAL     6

/* Standard CBOR extra simple types like floats and the values true and false */
#define CBOR_MAJOR_TYPE_SIMPLE       7


/*
 These are special values for the AdditionalInfo bits that are part of the first byte.
 Mostly they encode the length of the data item.
 */
#define LEN_IS_ONE_BYTE    24
#define LEN_IS_TWO_BYTES   25
#define LEN_IS_FOUR_BYTES  26
#define LEN_IS_EIGHT_BYTES 27
#define ADDINFO_RESERVED1  28
#define ADDINFO_RESERVED2  29
#define ADDINFO_RESERVED3  30
#define LEN_IS_INDEFINITE  31


/*
 24 is a special number for CBOR. Integers and lengths
 less than it are encoded in the same byte as the major type
 */
#define CBOR_TWENTY_FOUR   24


/*
 Tags that are used with CBOR_MAJOR_TYPE_OPTIONAL. These are
 the ones defined in the CBOR spec.
 */
/** See QCBOREncode_AddDateString() below */
#define CBOR_TAG_DATE_STRING    0
/** See QCBOREncode_AddDateEpoch_2() */
#define CBOR_TAG_DATE_EPOCH     1
#define CBOR_TAG_POS_BIGNUM     2
#define CBOR_TAG_NEG_BIGNUM     3
#define CBOR_TAG_FRACTION       4
#define CBOR_TAG_BIGFLOAT       5

#define CBOR_TAG_COSE_ENCRYPTO 16
#define CBOR_TAG_COSE_MAC0     17
#define CBOR_TAG_COSE_SIGN1    18

/* The data in byte string should be converted in base 64 URL when encoding in JSON or similar text-based representations */
#define CBOR_TAG_ENC_AS_B64URL 21
/* The data in byte string should be encoded in base 64 when encoding in JSON */
#define CBOR_TAG_ENC_AS_B64    22
/* The data in byte string should be encoded in base 16 when encoding in JSON */
#define CBOR_TAG_ENC_AS_B16    23
#define CBOR_TAG_CBOR          24
/** The data in the string is a URIs, as defined in RFC3986 */
#define CBOR_TAG_URI           32
/** The data in the string is a base 64'd URL */
#define CBOR_TAG_B64URL        33
/** The data in the string is base 64'd */
#define CBOR_TAG_B64           34
/** regular expressions in Perl Compatible Regular Expressions (PCRE) / JavaScript syntax ECMA262. */
#define CBOR_TAG_REGEX         35
/** MIME messages (including all headers), as defined in RFC2045 */
#define CBOR_TAG_MIME          36
/** Binary UUID */
#define CBOR_TAG_BIN_UUID      37

#define CBOR_TAG_CWT           61

#define CBOR_TAG_ENCRYPT       96
#define CBOR_TAG_MAC           97
#define CBOR_TAG_SIGN          98

#define CBOR_TAG_GEO_COORD    103


/** The data is CBOR data */
#define CBOR_TAG_CBOR_MAGIC 55799
#define CBOR_TAG_NONE  UINT64_MAX


/*
 Values for the 5 bits for items of major type 7
 */
#define CBOR_SIMPLEV_FALSE   20
#define CBOR_SIMPLEV_TRUE    21
#define CBOR_SIMPLEV_NULL    22
#define CBOR_SIMPLEV_UNDEF   23
#define CBOR_SIMPLEV_ONEBYTE 24
#define HALF_PREC_FLOAT      25
#define SINGLE_PREC_FLOAT    26
#define DOUBLE_PREC_FLOAT    27
#define CBOR_SIMPLE_BREAK    31



/* ===========================================================================

 END OF CONSTANTS THAT COME FROM THE CBOR STANDARD, RFC 7049

 BEGINNING OF PUBLIC INTERFACE FOR QCBOR ENCODER / DECODER

 =========================================================================== */

/**

 @file qcbor.h

 Q C B O R   E n c o d e / D e c o d e

 This implements CBOR -- Concise Binary Object Representation as defined
 in RFC 7049. More info is at http://cbor.io.  This is a near-complete
 implementation of the specification. Limitations are listed further down.

 CBOR is intentionally designed to be translatable to JSON, but not
 all CBOR can convert to JSON. See RFC 7049 for more info on how to
 construct CBOR that is the most JSON friendly.

 The memory model for encoding and decoding is that encoded CBOR
 must be in a contiguous buffer in memory.  During encoding the
 caller must supply an output buffer and if the encoding would go
 off the end of the buffer an error is returned.  During decoding
 the caller supplies the encoded CBOR in a contiguous buffer
 and the decoder returns pointers and lengths into that buffer
 for strings.

 This implementation does not require malloc. All data structures
 passed in/out of the APIs can fit on the stack.

 Decoding of indefinite length strings is a special case that requires
 a "string allocator" to allocate memory into which the segments of
 the string are coalesced. Without this, decoding will error out if
 an indefinite length string is encountered (indefinite length maps
 and arrays do not require the string allocator). A simple string
 allocator called MemPool is built-in and will work if supplied with
 a block of memory to allocate. The string allocator can optionally
 use malloc() or some other custom scheme.

 Here are some terms and definitions:

 - "Item", "Data Item": An integer or string or such. The basic "thing" that
 CBOR is about. An array is an item itself that contains some items.

 - "Array": An ordered sequence of items, the same as JSON.

 - "Map": A collection of label/value pairs. Each pair is a data
 item. A JSON "object" is the same as a CBOR "map".

 - "Label": The data item in a pair in a map that names or identifies the
 pair, not the value. This implementation refers to it as a "label".
 JSON refers to it as the "name". The CBOR RFC refers to it this as a "key".
 This implementation chooses label instead because key is too easily confused
 with a cryptographic key. The COSE standard, which uses CBOR, has also
 chosen to use the term "label" rather than "key" for this same reason.

 - "Key": See "Label" above.

 - "Tag": Optional info that can be added before each data item. This is always
 CBOR major type 6.

 - "Initial Byte": The first byte of an encoded item. Encoding and decoding of
 this byte is taken care of by the implementation.

 - "Additional Info": In addition to the major type, all data items have some
 other info. This is usually the length of the data, but can be several
 other things. Encoding and decoding of this is taken care of by the
 implementation.

 CBOR has two mechanisms for tagging and labeling the data
 values like integers and strings. For example, an integer that
 represents someone's birthday in epoch seconds since Jan 1, 1970
 could be encoded like this:

 - First it is CBOR_MAJOR_TYPE_POSITIVE_INT, the primitive positive
 integer.
 - Next it has a "tag" CBOR_TAG_DATE_EPOCH indicating the integer
 represents a date in the form of the number of seconds since
 Jan 1, 1970.
 - Last it has a string "label" like "BirthDate" indicating
 the meaning of the data.

 The encoded binary looks like this:
   a1                      # Map of 1 item
      69                   # Indicates text string of 9 bytes
        426972746844617465 # The text "BirthDate"
     c1                    # Tags next int as epoch date
        1a                 # Indicates 4 byte integer
            580d4172       # unsigned integer date 1477263730

 Implementors using this API will primarily work with labels. Generally
 tags are only needed for making up new data types. This implementation
 covers most of the data types defined in the RFC using tags. It also,
 allows for the creation of news tags if necessary.

 This implementation explicitly supports labels that are text strings
 and integers. Text strings translate nicely into JSON objects and
 are very readable.  Integer labels are much less readable, but
 can be very compact. If they are in the range of -23 to
 23 they take up only one byte.

 CBOR allows a label to be any type of data including an array or
 a map. It is possible to use this API to construct and
 parse such labels, but it is not explicitly supported.

 A common encoding usage mode is to invoke the encoding twice. First
 with no output buffer to compute the length of the needed output
 buffer. Then the correct sized output buffer is allocated. Last the
 encoder is invoked again, this time with the output buffer.

 The double invocation is not required if the max output buffer size
 can be predicted. This is usually possible for simple CBOR structures.
 If the double invocation is implemented, it can be
 in a loop or function as in the example code so that the code doesn't
 have to actually be written twice, saving code size.

 If a buffer too small to hold the encoded output is given, the error
 QCBOR_ERR_BUFFER_TOO_SMALL will be returned. Data will never be
 written off the end of the output buffer no matter which functions
 here are called or what parameters are passed to them.

 The error handling is simple. The only possible errors are trying to
 encode structures that are too large or too complex. There are no
 internal malloc calls so there will be no failures for out of memory.
 Only the final call, QCBOREncode_Finish(), returns an error code.
 Once an error happens, the encoder goes into an error state and calls
 to it will do nothing so the encoding can just go on. An error
 check is not needed after every data item is added.

 Encoding generally proceeds by calling QCBOREncode_Init(), calling
 lots of "Add" functions and calling QCBOREncode_Finish(). There
 are many "Add" functions for various data types. The input
 buffers need only to be valid during the "Add" calls. The
 data is copied into the output buf during the "Add" call.

 There are three `Add` functions for each data type. The first
 / main one for the type is for adding the data item to an array.
 The second one's name ends in `ToMap`, is used for adding
 data items to maps and takes a string
 argument that is its label in the map. The third one ends in
 `ToMapN`, is also used for adding data items to maps, and
 takes an integer argument that is its label in the map.

 The simplest aggregate type is an array, which is a simple ordered
 set of items without labels the same as JSON arrays. Call
 QCBOREncode_OpenArray() to open a new array, then "Add" to
 put items in the array and then QCBOREncode_CloseArray(). Nesting
 to a limit is allowed.  All opens must be matched by closes or an
 encoding error will be returned.

 The other aggregate type is a map which does use labels. The
 `Add` functions that end in `ToMap` and `ToMapN` are convenient
 ways to add labeled data items to a map. You can also call
 any type of `Add` function once to add a label of any time and
 then call any type of `Add` again to add its value.

 Note that when you nest arrays or maps in a map, the nested
 array or map has a label.

 Usually it is not necessary to add tags explicitly as most
 tagged types have functions here, but they can be added by
 calling QCBOREncode_AddTag().  There is an IANA registry for new tags that are
 for broad use and standardization as per RFC 7049. It is also
 allowed for protocols to make up new tags in the range above 256.
 Note that even arrays and maps can be tagged.

 Summary Limits of this implementation:
 - The entire encoded CBOR must fit into contiguous memory.
 - Max size of encoded / decoded CBOR data is UINT32_MAX (4GB).
 - Max array / map nesting level when encoding / decoding is
   QCBOR_MAX_ARRAY_NESTING (this is typically 15).
 - Max items in an array or map when encoding / decoding is
   QCBOR_MAX_ITEMS_IN_ARRAY (typically 65,536).
 - Does not support encoding indefinite lengths (decoding is supported).
 - Does not directly support some tagged types: decimal fractions, big floats
 - Does not directly support labels in maps other than text strings and ints.
 - Does not directly support int labels greater than INT64_MAX
 - Epoch dates limited to INT64_MAX (+/- 292 billion years)
 - Tags on labels are ignored during decoding

 This implementation is intended to run on 32 and 64-bit CPUs. Minor
 modifications are needed for it to work on 16-bit CPUs.

 The public interface uses size_t for all lengths. Internally the
 implementation uses 32-bit lengths by design to use less memory and
 fit structures on the stack. This limits the encoded
 CBOR it can work with to size UINT32_MAX (4GB) which should be
 enough.

 This implementation assumes two's compliment integer
 machines. Stdint.h also requires this. It of course would be easy to
 fix this implementation for another integer representation, but all
 modern machines seem to be two's compliment.

 */


/**
 The maximum number of items in a single array or map when encoding of decoding.
*/
// -1 is because the value UINT16_MAX is used to track indefinite length arraysUINT16_MAX
#define QCBOR_MAX_ITEMS_IN_ARRAY (UINT16_MAX-1)

/**
 The maximum nesting of arrays and maps when encoding or decoding. The
 error QCBOR_ERR_ARRAY_NESTING_TOO_DEEP will be returned on encoding
 of decoding if it is exceeded
*/
#define QCBOR_MAX_ARRAY_NESTING  QCBOR_MAX_ARRAY_NESTING1

/**
 The maximum number of tags that can be in QCBORTagListIn and passed to
 QCBORDecode_SetCallerConfiguredTagList()
 */
#define QCBOR_MAX_CUSTOM_TAGS    16


typedef enum {
   /** The encode or decode completely correctly. */
   QCBOR_SUCCESS = 0,

   /** The buffer provided for the encoded output when doing encoding was
       too small and the encoded output will not fit. Also, when the buffer
       given to QCBORDecode_SetMemPool() is too small. */
   QCBOR_ERR_BUFFER_TOO_SMALL,

   /** During encoding or decoding, the array or map nesting was deeper than
    this implementation can handle. Note that in the interest of code size
    and memory use, this implementation has a hard limit on array nesting. The
    limit is defined as the constant QCBOR_MAX_ARRAY_NESTING. */
   QCBOR_ERR_ARRAY_NESTING_TOO_DEEP,

   /** During decoding or encoding, the array or map had too many items in it.
       This limit QCBOR_MAX_ITEMS_IN_ARRAY, typically 65,535. */
   QCBOR_ERR_ARRAY_TOO_LONG,

   /** During encoding, more arrays or maps were closed than opened. This is a
       coding error on the part of the caller of the encoder. */
   QCBOR_ERR_TOO_MANY_CLOSES,

   /** During decoding, some CBOR construct was encountered that this decoder
        doesn't support, primarily this is the reserved additional info values,
        28 through 30. */
   QCBOR_ERR_UNSUPPORTED,

   /** During decoding, hit the end of the given data to decode. For example,
       a byte string of 100 bytes was expected, but the end of the input was
       hit before finding those 100 bytes.  Corrupted CBOR input will often
       result in this error. */
   QCBOR_ERR_HIT_END,

   /** During encoding, the length of the encoded CBOR exceeded UINT32_MAX.
     */
   QCBOR_ERR_BUFFER_TOO_LARGE,

   /** During decoding, an integer smaller than INT64_MIN was received (CBOR
       can represent integers smaller than INT64_MIN, but C cannot). */
   QCBOR_ERR_INT_OVERFLOW,

   /** During decoding, the label for a map entry is bad. What causes this
       error depends on the decoding mode. */
   QCBOR_ERR_MAP_LABEL_TYPE,

   /** During encoding or decoding, the number of array or map opens was not
       matched by the number of closes. */
   QCBOR_ERR_ARRAY_OR_MAP_STILL_OPEN,

   /** During encoding, the simple value is not between CBOR_SIMPLEV_FALSE
       and CBOR_SIMPLEV_UNDEF. */
   QCBOR_ERR_BAD_SIMPLE,

   /** During decoding, a date greater than +- 292 billion years from Jan 1
       1970 encountered during parsing. */
   QCBOR_ERR_DATE_OVERFLOW,

   /** During decoding, the CBOR is not valid, primarily a simple type is encoded in
      a prohibited way. */
   QCBOR_ERR_INVALID_CBOR,

   /** Optional tagging that doesn't make sense (an int is tagged as a
       date string) or can't be handled. */
   QCBOR_ERR_BAD_OPT_TAG,

   /** Returned by QCBORDecode_Finish() if all the inputs bytes have not
       been consumed. */
   QCBOR_ERR_EXTRA_BYTES,

   /** During encoding, QCBOREncode_Close() call with a different type than
       is currently open. */
   QCBOR_ERR_CLOSE_MISMATCH,

   /** Unable to decode an indefinite length string because no string
       allocator was configured. */
   QCBOR_ERR_NO_STRING_ALLOCATOR,

   /** One of the chunks in an indefinite length string is not of the type of
       the string. */
   QCBOR_ERR_INDEFINITE_STRING_CHUNK,

   /** Error allocating space for a string, usually for an indefinite length
       string. */
   QCBOR_ERR_STRING_ALLOCATE,

   /** During decoding, a break occurred outside an indefinite length item. */
   QCBOR_ERR_BAD_BREAK,

   /** During decoding, too many tags in the caller-configured tag list, or not
       enough space in QCBORTagListOut. */
   QCBOR_ERR_TOO_MANY_TAGS,

   /** Returned by QCBORDecode_SetMemPool() when xx is too small. This should
       never happen on a machine with 64-bit or smaller pointers. Fixing
       it is probably by increasing QCBOR_DECODE_MIN_MEM_POOL_SIZE. */
   QCBOR_ERR_MEM_POOL_INTERNAL

} QCBORError;


typedef enum {
   /** See QCBORDecode_Init() */
   QCBOR_DECODE_MODE_NORMAL = 0,
   /** See QCBORDecode_Init() */
   QCBOR_DECODE_MODE_MAP_STRINGS_ONLY = 1,
   /** See QCBORDecode_Init() */
   QCBOR_DECODE_MODE_MAP_AS_ARRAY = 2
} QCBORDecodeMode;





/* Do not renumber these. Code depends on some of these values. */
/** The type is unknown, unset or invalid */
#define QCBOR_TYPE_NONE           0
/** Type for an integer that decoded either between INT64_MIN and INT32_MIN or INT32_MAX and INT64_MAX; val.int64 */
#define QCBOR_TYPE_INT64          2
/** Type for an integer that decoded to a more than INT64_MAX and UINT64_MAX; val.uint64 */
#define QCBOR_TYPE_UINT64         3
/** Type for an array. The number of items in the array is in val.uCount. */
#define QCBOR_TYPE_ARRAY          4
/** Type for a map; number of items in map is in val.uCount */
#define QCBOR_TYPE_MAP            5
/** Type for a buffer full of bytes. Data is in val.string. */
#define QCBOR_TYPE_BYTE_STRING    6
/** Type for a UTF-8 string. It is not NULL terminated. Data is in val.string.  */
#define QCBOR_TYPE_TEXT_STRING    7
/** Type for a positive big number. Data is in val.bignum, a pointer and a length. */
#define QCBOR_TYPE_POSBIGNUM     9
/** Type for a negative big number. Data is in val.bignum, a pointer and a length. */
#define QCBOR_TYPE_NEGBIGNUM     10
/** Type for RFC 3339 date string, possibly with time zone. Data is in val.dateString */
#define QCBOR_TYPE_DATE_STRING   11
/** Type for integer seconds since Jan 1970 + floating point fraction. Data is in val.epochDate */
#define QCBOR_TYPE_DATE_EPOCH    12
/** A simple type that this CBOR implementation doesn't know about; Type is in val.uSimple. */
#define QCBOR_TYPE_UKNOWN_SIMPLE 13
/** Type for the simple value false; nothing more; nothing in val union. */
#define QCBOR_TYPE_FALSE         20
/** Type for the simple value true; nothing more; nothing in val union. */
#define QCBOR_TYPE_TRUE          21
/** Type for the simple value null; nothing more; nothing in val union. */
#define QCBOR_TYPE_NULL          22
/** Type for the simple value undef; nothing more; nothing in val union. */
#define QCBOR_TYPE_UNDEF         23
/** Type for a floating point number. Data is in val.float. */
#define QCBOR_TYPE_FLOAT         26
/** Type for a double floating point number. Data is in val.double. */
#define QCBOR_TYPE_DOUBLE        27
/** For QCBOR_DECODE_MODE_MAP_AS_ARRAY decode mode, a map that is being traversed as an array. See QCBORDecode_Init() */
#define QCBOR_TYPE_MAP_AS_ARRAY  32

#define QCBOR_TYPE_BREAK         31 // Used internally; never returned

#define QCBOR_TYPE_OPTTAG       254 // Used internally; never returned



/*
 Approx Size of this:
   8 + 8 + 1 + 1 + 1 + (1 padding) + (4 padding on 64-bit machine) = 24 for first part (20 on a 32-bit machine)
   16 bytes for the val union
   16 bytes for label union
   total = 56 bytes (52 bytes on 32-bit machine)
 */

/**
 QCBORItem holds the type, value and other info for a decoded item returned by GetNextItem().
 */
typedef struct _QCBORItem {
   uint8_t  uDataType;     /** Tells what element of the val union to use. One of QCBOR_TYPE_XXXX */
   uint8_t  uNestingLevel; /** How deep the nesting from arrays and maps are. 0 is the top level with no arrays or maps entered */
   uint8_t  uLabelType;    /** Tells what element of the label union to use */
   uint8_t  uDataAlloc;    /** 1 if allocated with string allocator, 0 if not. See QCBORDecode_MakeMallocStringAllocator() */
   uint8_t  uLabelAlloc;   /** Like uDataAlloc, but for label */
   uint8_t  uNextNestLevel; /** If not equal to uNestingLevel, this item closed out at least one map/array */

   union {
      int64_t     int64;      /** The value for uDataType QCBOR_TYPE_INT64 */
      uint64_t    uint64;     /** The value for uDataType QCBOR_TYPE_UINT64 */

      UsefulBufC  string;     /** The value for uDataType QCBOR_TYPE_BYTE_STRING and QCBOR_TYPE_TEXT_STRING */
      uint16_t    uCount;     /** The "value" for uDataType QCBOR_TYPE_ARRAY or QCBOR_TYPE_MAP -- the number of items in the array or map
                                  UINT16_MAX when decoding indefinite lengths maps and arrays. */
      double      dfnum;      /** The value for uDataType QCBOR_TYPE_DOUBLE */
      struct {
         int64_t  nSeconds;
         double   fSecondsFraction;
      } epochDate;            /** The value for uDataType QCBOR_TYPE_DATE_EPOCH */
      UsefulBufC  dateString; /** The value for uDataType QCBOR_TYPE_DATE_STRING */
      UsefulBufC  bigNum;     /** The value for uDataType QCBOR_TYPE_BIGNUM */
      uint8_t     uSimple;    /** The integer value for unknown simple types */
      uint64_t    uTagV;

   } val;  /** The union holding the item's value. Select union member based on uDataType */

   union {
      UsefulBufC  string;  /** The label for uLabelType QCBOR_TYPE_BYTE_STRING and QCBOR_TYPE_TEXT_STRING */
      int64_t     int64;   /** The label for uLabelType for QCBOR_TYPE_INT64 */
      uint64_t    uint64;  /** The label for uLabelType for QCBOR_TYPE_UINT64 */
   } label; /** Union holding the different label types selected based on uLabelType */

   uint64_t uTagBits; /** Bit indicating which tags (major type 6) on this item.  */

} QCBORItem;


/**
 This is a set of functions and pointer context (in object-oriented parlance,
 an "object") used to allocate memory for coalescing the segments of an indefinite
 length string into one.

 The fAllocate function works as an initial allocator and a reallocator to
 expand the string for each new segment. When it is an initial allocator
 pOldMem is NULL.

 The fFree function is called to clean up an individual allocation when an error occurs.

 The fDesctructor function is called when QCBORDecode_Finish is called.

 Any memory allocated with this will be marked by setting uDataAlloc
 or uLabelAlloc in the QCBORItem structure so the caller knows they
 have to free it.

 fAllocate is only ever called to increase the single most recent
 allocation made, making implementation of a memory pool very simple.

 fFree is also only called on the single most recent allocation.
 */
typedef struct {
   void       *pAllocaterContext;
   UsefulBuf (*fAllocate)(void *pAllocaterContext, void *pOldMem, size_t uNewSize);
   void      (*fFree)(void *pAllocaterContext, void *pMem);
   void      (*fDestructor)(void *pAllocaterContext);
} QCBORStringAllocator;


/**
 This only matters if you use a string allocator
 and and set it up with QCBORDecode_SetMemPool(). It is
 the size of the overhead needed needed by
 QCBORDecode_SetMemPool().  If you write your own
 string allocator or use the separately available malloc
 based string allocator, this size will not apply
 */
#define QCBOR_DECODE_MIN_MEM_POOL_SIZE 72


/**
 This is used to tell the decoder about tags that it should
 record in uTagBits in QCBORItem beyond the built-in
 tags. puTags points to an
 array of uint64_t integers that are the tags. uNumTags
 is the number of integers in the array. The maximum
 size is QCBOR_MAX_CUSTOM_TAGS.  See QCBORDecode_IsTagged()
 and QCBORDecode_SetCallerAddedTagMap().
 */
typedef struct {
   uint8_t uNumTags;
   const uint64_t *puTags;
} QCBORTagListIn;


/**
 This is for QCBORDecode_GetNextWithTags() to be able to return the
 full list of tags on an item. It not needed for most CBOR protocol
 implementations. Its primary use is for pretty-printing CBOR or
 protocol conversion to another format.

 On input, puTags points to a buffer to be filled in
 and uNumAllocated is the number of uint64_t values
 in the buffer.

 On output the buffer contains the tags for the item.
 uNumUsed tells how many there are.
 */
typedef struct {
   uint8_t uNumUsed;
   uint8_t uNumAllocated;
   uint64_t *puTags;
} QCBORTagListOut;


/**
 QCBOREncodeContext is the data type that holds context for all the
 encoding functions. It is less than 200 bytes, so it can go on
 the stack. The contents are opaque, and the caller should not access
 any internal items.  A context may be re used serially as long as
 it is re initialized.
 */
typedef struct _QCBOREncodeContext QCBOREncodeContext;


/**
 Initialize the the encoder to prepare to encode some CBOR.

 @param[in,out]  pCtx    The encoder context to initialize.
 @param[in]      Storage The buffer into which this encoded result will be placed.

 Call this once at the start of an encoding of a CBOR structure. Then
 call the various QCBOREncode_AddXXX() functions to add the data
 items. Then call QCBOREncode_Finish().

 The maximum output buffer is UINT32_MAX (4GB). This is not a practical
 limit in any way and reduces the memory needed by the implementation.
 The error QCBOR_ERR_BUFFER_TOO_LARGE will be returned by QCBOR_Finish()
 if a larger buffer length is passed in.

 If this is called with pBuf as NULL and uBufLen a large value like
 UINT32_MAX, all the QCBOREncode_AddXXXX() functions and
 QCBORE_Encode_Finish() can still be called. No data will be encoded,
 but the length of what would be encoded will be calculated. The
 length of the encoded structure will be handed back in the call to
 QCBOREncode_Finish(). You can then allocate a buffer of that size
 and call all the encoding again, this time to fill in the buffer.

 A QCBORContext can be reused over and over as long as
 QCBOREncode_Init() is called.
 */
void QCBOREncode_Init(QCBOREncodeContext *pCtx, UsefulBuf Storage);


/**
 @brief  Add a signed 64-bit integer to the encoded output.

 @param[in] pCtx      The encoding context to add the integer to.
 @param[in] nNum      The integer to add.

 The integer will be encoded and added to the CBOR output.

 This function figures out the size and the sign and encodes in the
 correct minimal CBOR. Specifically, it will select CBOR major type 0 or 1
 based on sign and will encode to 1, 2, 4 or 8 bytes depending on the
 value of the integer. Values less than 24 effectively encode to one
 byte because they are encoded in with the CBOR major type.  This is
 a neat and efficient characteristic of CBOR that can be taken
 advantage of when designing CBOR-based protocols. If integers like
 tags can be kept between -23 and 23 they will be encoded in one byte
 including the major type.

 If you pass a smaller int, say an int16_t or a small value, say 100,
 the encoding will still be CBOR's most compact that can represent the
 value.  For example, CBOR always encodes the value 0 as one byte,
 0x00. The representation as 0x00 includes identification of the type
 as an integer too as the major type for an integer is 0. See RFC 7049
 Appendix A for more examples of CBOR encoding. This compact encoding
 is also canonical CBOR as per section 3.9 in RFC 7049.

 There are no functions to add int16_t or int32_t because they are
 not necessary because this always encodes to the smallest number
 of bytes based on the value (If this code is running on a 32-bit
 machine having a way to add 32-bit integers would reduce code size some).

 If the encoding context is in an error state, this will do
 nothing. If an error occurs when adding this integer, the internal
 error flag will be set, and the error will be returned when
 QCBOREncode_Finish() is called.

 See also QCBOREncode_AddUInt64().
 */
void QCBOREncode_AddInt64(QCBOREncodeContext *pCtx, int64_t nNum);

static void QCBOREncode_AddInt64ToMap(QCBOREncodeContext *pCtx, const char *szLabel, int64_t uNum);

static void QCBOREncode_AddInt64ToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, int64_t uNum);


/**
 @brief  Add an unsigned 64-bit integer to the encoded output.

 @param[in] pCtx      The encoding context to add the integer to.
 @param[in] uNum      The integer to add.

 The integer will be encoded and added to the CBOR output.

 The only reason so use this function is for integers larger than
 INT64_MAX and smaller than UINT64_MAX. Otherwise QCBOREncode_AddInt64()
 will work fine.

 Error handling is the same as for QCBOREncode_AddInt64().
 */
void QCBOREncode_AddUInt64(QCBOREncodeContext *pCtx, uint64_t uNum);

static void QCBOREncode_AddUInt64ToMap(QCBOREncodeContext *pCtx, const char *szLabel, uint64_t uNum);

static void QCBOREncode_AddUInt64ToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, uint64_t uNum);


/**

 @brief  Add a UTF-8 text string to the encoded output

 @param[in] pCtx     The context to initialize.
 @param[in] Text    Pointer and length of text to add.

 The text passed in must be unencoded UTF-8 according to RFC
 3629. There is no NULL termination. The text is added as CBOR
 major type 3.

 If called with nBytesLen equal to 0, an empty string will be
 added. When nBytesLen is 0, pBytes may be NULL.

 Note that the restriction of the buffer length to an uint32_t is
 entirely intentional as this encoder is not capable of encoding
 lengths greater. This limit to 4GB for a text string should not be a
 problem.

 Error handling is the same as QCBOREncode_AddInt64().
 */
static void QCBOREncode_AddText(QCBOREncodeContext *pCtx, UsefulBufC Text);

static void QCBOREncode_AddTextToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Text);

static void QCBOREncode_AddTextToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Text);


/**
 @brief  Add a UTF-8 text string to the encoded output

 @param[in] pCtx      The context to initialize.
 @param[in] szString  Null-terminated text to add.

 This works the same as QCBOREncode_AddText().
 */
static void QCBOREncode_AddSZString(QCBOREncodeContext *pCtx, const char *szString);

static void QCBOREncode_AddSZStringToMap(QCBOREncodeContext *pCtx, const char *szLabel, const char *szString);

static void QCBOREncode_AddSZStringToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, const char *szString);


/**
 @brief  Add a floating-point number to the encoded output

 @param[in] pCtx      The encoding context to add the float to.
 @param[in] dNum      The double precision number to add.

 This outputs a floating-point number with CBOR major type 7.

 This will selectively encode the double-precision floating point
 number as either double-precision, single-precision or
 half-precision. It will always encode infinity, NaN and 0 has half
 precision. If no precision will be lost in the conversion to
 half-precision then it will be converted and encoded. If not and no
 precision will be lost in conversion to single-precision, then it
 will be converted and encoded. If not, then no conversion is
 performed, and it encoded as a double.

 Half-precision floating point numbers take up 2 bytes, half that of
 single-precision, one quarter of double-precision

 This automatically reduces the size of encoded messages a lot, maybe
 even by four if most of values are 0, infinity or NaN.

 On decode, these will always be returned as a double.

 Error handling is the same as QCBOREncode_AddInt64().
 */
void QCBOREncode_AddDouble(QCBOREncodeContext *pCtx, double dNum);

static void QCBOREncode_AddDoubleToMap(QCBOREncodeContext *pCtx, const char *szLabel, double dNum);

static void QCBOREncode_AddDoubleToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, double dNum);


/**
 @brief[in] Add an optional tag

 @param[in] pCtx  The encoding context to add the integer to.
 @param[in] uTag  The tag to add

 This outputs a CBOR major type 6 optional tag.

 The tag is applied to the next data item added to the encoded
 output. That data item that is to be tagged can be of any major
 CBOR type. Any number of tags can be added to a data item by calling
 this multiple times before the data item is added.

 For many of the common standard tags a function to encode
 data using it already exists and this is not needed. For example,
 QCBOREncode_AddDateEpoch() already exists to output
 integers representing dates with the right tag.
*/
void QCBOREncode_AddTag(QCBOREncodeContext *pCtx,uint64_t uTag);


/**
 @brief  Add an epoch-based date

 @param[in] pCtx     The encoding context to add the simple value to.
 @param[in] date     Number of seconds since 1970-01-01T00:00Z in UTC time.

 As per RFC 7049 this is similar to UNIX/Linux/POSIX dates. This is
 the most compact way to specify a date and time in CBOR. Note that this
 is always UTC and does not include the time zone.  Use
 QCBOREncode_AddDateString() if you want to include the time zone.

 The integer encoding rules apply here so the date will be encoded in a
 minimal number of 1, 2 4 or 8 bytes. Until about the year 2106 these
 dates should encode in 6 bytes -- one byte for the tag, one byte for the type
 and 4 bytes for the integer.

 If you care about leap-seconds and that level of accuracy, make sure the
 system you are running this code on does it correctly. This code just takes
 the value passed in.

 This implementation cannot encode fractional seconds using float or double
 even though that is allowed by CBOR, but you can encode them if you
 want to by calling QCBOREncode_AddDouble()
 with the right parameters.

 Error handling is the same as QCBOREncode_AddInt64().
 */
static void QCBOREncode_AddDateEpoch(QCBOREncodeContext *pCtx, int64_t date);

static void QCBOREncode_AddDateEpochToMap(QCBOREncodeContext *pCtx, const char *szLabel, int64_t date);

static  void QCBOREncode_AddDateEpochToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, int64_t date);


/**
 @brief Add a byte string to the encoded output.

 @param[in] pCtx      The context to initialize.
 @param[in] Bytes     Pointer and length of the input data.

 Simply adds the bytes to the encoded output as CBOR major type 2.

 If called with Bytes.len equal to 0, an empty string will be
 added. When Bytes.len is 0, Bytes.ptr may be NULL.

 Error handling is the same as QCBOREncode_AddInt64().
 */
static void QCBOREncode_AddBytes(QCBOREncodeContext *pCtx, UsefulBufC Bytes);

static void QCBOREncode_AddBytesToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes);

static void QCBOREncode_AddBytesToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes);



/**
 @brief Add a binary UUID to the encoded output.

 @param[in] pCtx      The context to initialize.
 @param[in] Bytes     Pointer and length of the binary UUID.

 A binary UUID as defined in RFC 4122 is added to the ouput.

 It is output as CBOR major type 2, a binary string, with
 optional tag 36 indicating the binary string is a UUID.
 */
static void QCBOREncode_AddBinaryUUID(QCBOREncodeContext *pCtx, UsefulBufC Bytes);

static void QCBOREncode_AddBinaryUUIDToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes);

static void QCBOREncode_AddBinaryUUIDToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes);


/**
 @brief Add a positive big number to the encoded output.

 @param[in] pCtx      The context to initialize.
 @param[in] Bytes     Pointer and length of the big number.

 Big numbers are integers larger than 64-bits. Their format
 is described in RFC 7049.

 It is output as CBOR major type 2, a binary string, with
 optional tag 2 indicating the binary string is a positive big
 number.

 Often big numbers are used to represent cryptographic keys,
 however, COSE which defines representations for keys chose not
 to use this particular type.
 */
static void QCBOREncode_AddPositiveBignum(QCBOREncodeContext *pCtx, UsefulBufC Bytes);

static void QCBOREncode_AddPositiveBignumToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes);

static void QCBOREncode_AddPositiveBignumToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes);


/**
 @brief Add a negative big number to the encoded output.

 @param[in] pCtx      The context to initialize.
 @param[in] Bytes     Pointer and length of the big number.

 Big numbers are integers larger than 64-bits. Their format
 is described in RFC 7049.

 It is output as CBOR major type 2, a binary string, with
 optional tag 2 indicating the binary string is a negative big
 number.

 Often big numbers are used to represent cryptographic keys,
 however, COSE which defines representations for keys chose not
 to use this particular type.
 */
static void QCBOREncode_AddNegativeBignum(QCBOREncodeContext *pCtx, UsefulBufC Bytes);

static void QCBOREncode_AddNegativeBignumToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes);

static void QCBOREncode_AddNegativeBignumToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes);


/**
 @brief Add a text URI to the encoded output.

 @param[in] pCtx    The context to initialize.
 @param[in] URI     Pointer and length of the URI.

 The format of URI is RFC 3986.

 It is output as CBOR major type 3, a text string, with
 optional tag 32 indicating the text string is a URI.
 */
static void QCBOREncode_AddURI(QCBOREncodeContext *pCtx, UsefulBufC URI);

static void QCBOREncode_AddURIToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC URI);

static void QCBOREncode_AddURIToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC URI);


/**
 @brief Add base 64-encoded text to encoded output.

 @param[in] pCtx     The context to initialize.
 @param[in] B64Text  Pointer and length of the base-64 encoded text.

 The text content is base 64 encoded data per RFC 4648.

 It is output as CBOR major type 3, a text string, with
 optional tag 34 indicating the text string is a URI.
 */
static void QCBOREncode_AddB64Text(QCBOREncodeContext *pCtx, UsefulBufC B64Text);

static void QCBOREncode_AddB64TextToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC B64Text);

static void QCBOREncode_AddB64TextToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC B64Text);


/**
 @brief Add base 64URL -encoded URL to encoded output.

 @param[in] pCtx    The context to initialize.
 @param[in] B64Text  Pointer and length of the base-64 encoded text.

 The text content is base 64 URL format encoded text as per RFC 4648.

 It is output as CBOR major type 3, a text string, with
 optional tag 33 indicating the text string is a URI.
 */
static void QCBOREncode_AddB64URLText(QCBOREncodeContext *pCtx, UsefulBufC B64Text);

static void QCBOREncode_AddB64URLTextToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC B64Text);

static void QCBOREncode_AddB64URLTextToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC B64Text);


/**
 @brief Add Perl Compatible Regular Expression

 @param[in] pCtx    The context to initialize.
 @param[in] Regex   Pointer and length of the regular expression.

 The text content is Perl Compatible Regular
 Expressions (PCRE) / JavaScript syntax [ECMA262].

 It is output as CBOR major type 3, a text string, with
 optional tag 35 indicating the text string is a regular expression.
 */
static void QCBOREncode_AddRegex(QCBOREncodeContext *pCtx, UsefulBufC Regex);

static void QCBOREncode_AddRegexToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Regex);

static void QCBOREncode_AddRegexToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Regex);


/**
 @brief MIME encoded text to the encoded output.

 @param[in] pCtx      The context to initialize.
 @param[in] MIMEData  Pointer and length of the regular expression.

 The text content is in MIME format per RFC 2045 including the headers.

 It is output as CBOR major type 3, a text string, with
 optional tag 36 indicating the text string is MIME data.
 */
static void QCBOREncode_AddMIMEData(QCBOREncodeContext *pCtx, UsefulBufC MIMEData);

static void QCBOREncode_AddMIMEDataToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC MIMEData);

static void QCBOREncode_AddMIMEDataToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC MIMEData);


/**
 @brief  Add an RFC 3339 date string

 @param[in] pCtx      The encoding context to add the simple value to.
 @param[in] szDate    Null-terminated string with date to add

 The string szDate should be in the form of RFC 3339 as defined by section
 3.3 in RFC 4287. This is as described in section 2.4.1 in RFC 7049.

 Note that this function doesn't validate the format of the date string
 at all. If you add an incorrect format date string, the generated
 CBOR will be incorrect and the receiver may not be able to handle it.

 Error handling is the same as QCBOREncode_AddInt64().
 */
static void QCBOREncode_AddDateString(QCBOREncodeContext *pCtx, const char *szDate);

static void QCBOREncode_AddDateStringToMap(QCBOREncodeContext *pCtx, const char *szLabel, const char *szDate);

static void QCBOREncode_AddDateStringToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, const char *szDate);


/**
 @brief  Add a standard boolean.

 @param[in] pCtx   The encoding context to add the simple value to.
 @param[in] b      true or false from stdbool. Anything will result in an error.

 Adds a boolean value as CBOR major type 7.

 Error handling is the same as QCBOREncode_AddInt64().
 */
static void QCBOREncode_AddBool(QCBOREncodeContext *pCtx, bool b);

static void QCBOREncode_AddBoolToMap(QCBOREncodeContext *pCtx, const char *szLabel, bool b);

static void QCBOREncode_AddBoolToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, bool b);



/**
 @brief  Add a NULL to the encoded output.

 @param[in] pCtx   The encoding context to add the simple value to.

 Adds the NULL value as CBOR major type 7.

 This NULL doesn't have any special meaning in CBOR such as a terminating
 value for a string or an empty value.

 Error handling is the same as QCBOREncode_AddInt64().
 */
static void QCBOREncode_AddNULL(QCBOREncodeContext *pCtx);

static void QCBOREncode_AddNULLToMap(QCBOREncodeContext *pCtx, const char *szLabel);

static void QCBOREncode_AddNULLToMapN(QCBOREncodeContext *pCtx, int64_t nLabel);


/**
 @brief  Add an "undef" to the encoded output.

 @param[in] pCtx   The encoding context to add the simple value to.

 Adds the undef value as CBOR major type 7.

 Note that this value will not translate to JSON.

 This Undef doesn't have any special meaning in CBOR such as a terminating
 value for a string or an empty value.

 Error handling is the same as QCBOREncode_AddInt64().
 */
static void QCBOREncode_AddUndef(QCBOREncodeContext *pCtx);

static void QCBOREncode_AddUndefToMap(QCBOREncodeContext *pCtx, const char *szLabel);

static void QCBOREncode_AddUndefToMapN(QCBOREncodeContext *pCtx, int64_t nLabel);


/**
 @brief  Indicates that the next items added are in an array.

 @param[in] pCtx The encoding context to open the array in.

 Arrays are the basic CBOR aggregate or structure type. Call this
 function to start or open an array. Then call the various AddXXX
 functions to add the items that go into the array. Then call
 QCBOREncode_CloseArray() when all items have been added. The data
 items in the array can be of any type and can be of mixed types.

 Nesting of arrays and maps is allowed and supported just by calling
 QCBOREncode_OpenArray() again before calling CloseArray.  While CBOR
 has no limit on nesting, this implementation does in order to keep it
 smaller and simpler.  The limit is QCBOR_MAX_ARRAY_NESTING. This is
 the max number of times this can be called without calling
 QCBOREncode_CloseArray(). QCBOREncode_Finish() will return
 QCBOR_ERR_ARRAY_NESTING_TOO_DEEP when it is called as this function
 just sets an error state and returns no value when this occurs.

 If you try to add more than QCBOR_MAX_ITEMS_IN_ARRAY items to a
 single array or map, QCBOR_ERR_ARRAY_TOO_LONG will be returned when
 QCBOREncode_Finish() is called.

 An array itself must have a label if it is being added to a map.
 Note that array elements do not have labels (but map elements do).

 An array itself may be tagged.
 */
static void QCBOREncode_OpenArray(QCBOREncodeContext *pCtx);

static void QCBOREncode_OpenArrayInMap(QCBOREncodeContext *pCtx, const char *szLabel);

static void QCBOREncode_OpenArrayInMapN(QCBOREncodeContext *pCtx,  int64_t nLabel);


/**
 @brief Close an open array.

 @param[in] pCtx The context to add to.

 The closes an array opened by QCBOREncode_OpenArray(). It reduces
 nesting level by one. All arrays (and maps) must be closed before
 calling QCBOREncode_Finish().

 When an error occurs as a result of this call, the encoder records
 the error and enters the error state. The error will be returned when
 QCBOREncode_Finish() is called.

 If this has been called more times than QCBOREncode_OpenArray(), then
 QCBOR_ERR_TOO_MANY_CLOSES will be returned when QCBOREncode_Finish()
 is called.

 If this is called and it is not an array that is currently open,
 QCBOR_ERR_CLOSE_MISMATCH will be returned when QCBOREncode_Finish()
 is called.
 */
static void QCBOREncode_CloseArray(QCBOREncodeContext *pCtx);


/**
 @brief  Indicates that the next items added are in a map.

 @param[in] pCtx The context to add to.

 See QCBOREncode_OpenArray() for more information, particularly error
 handling.

 CBOR maps are an aggregate type where each item in the map consists
 of a label and a value. They are similar to JSON objects.

 The value can be any CBOR type including another map.

 The label can also be any CBOR type, but in practice they are
 typically, integers as this gives the most compact output. They might
 also be text strings which gives readability and translation to JSON.

 Every QCBOREncode_AddXXX() call has once version that is "InMap" for
 adding items to maps with string labels and on that is "InMapN" that
 is for adding with integer labels.

 RFC 7049 uses the term "key" instead of "label".

 If you wish to use map labels that are neither integer labels or
 text strings, then just call the QCBOREncode_AddXXX() function
 explicitly to add the label. Then call it again to add the value.

 See the RFC7049 for a lot more information on creating maps.
 */
static void QCBOREncode_OpenMap(QCBOREncodeContext *pCtx);

static void QCBOREncode_OpenMapInMap(QCBOREncodeContext *pCtx, const char *szLabel);

static void QCBOREncode_OpenMapInMapN(QCBOREncodeContext *pCtx, int64_t nLabel);



/**
 @brief Close an open map.

 @param[in] pCtx The context to add to.

 The closes a map opened by QCBOREncode_OpenMap(). It reduces nesting
 level by one.

 When an error occurs as a result of this call, the encoder records
 the error and enters the error state. The error will be returned when
 QCBOREncode_Finish() is called.

 If this has been called more times than QCBOREncode_OpenMap(),
 then QCBOR_ERR_TOO_MANY_CLOSES will be returned when
 QCBOREncode_Finish() is called.

 If this is called and it is not a map that is currently
 open, QCBOR_ERR_CLOSE_MISMATCH will be returned when QCBOREncode_Finish()
 is called.
 */
static void QCBOREncode_CloseMap(QCBOREncodeContext *pCtx);


/**
 @brief Indicate start of encoded CBOR to be wrapped in a bstr.

 @param[in] pCtx The context to add to.

 All added encoded items between this call and a call to
 QCBOREncode_CloseBstrWrap() will be wrapped in a bstr. They will
 appear in the final output as a byte string.  That byte string will
 contain encoded CBOR.

 The typical use case is for encoded CBOR that is to be
 cryptographically hashed, as part of a COSE (RFC 8152)
 implementation. This avoids having to encode the items first in one
 buffer (e.g., the COSE payload) and then add that buffer as a bstr to
 another encoding (e.g. the COSE to-be-signed bytes, the
 Sig_structure) potentially saving a lot of memory.

 When constructing cryptographically signed CBOR objects, maps or
 arrays, they typically are encoded normally and then wrapped as a
 byte string. The COSE standard for example does this. The wrapping is
 simply treating the encoded CBOR map as a byte string.

 The stated purpose of this wrapping is to prevent code relaying the
 signed data but not verifying it from tampering with the signed data
 thus making the signature unverifiable. It is also quite beneficial
 for the signature verification code. Standard CBOR parsers usually do
 not give access to partially parsed CBOR as would be need to check
 the signature of some CBOR. With this wrapping, standard CBOR parsers
 can be used to get to all the data needed for a signature
 verification.
 */
static void QCBOREncode_BstrWrap(QCBOREncodeContext *pCtx);

static void QCBOREncode_BstrWrapInMap(QCBOREncodeContext *pCtx, const char *szLabel);

static void QCBOREncode_BstrWrapInMapN(QCBOREncodeContext *pCtx, int64_t nLabel);


/**
 @brief Close a wrapping bstr.

 @param[in] pCtx The context to add to.
 @param[out] pWrappedCBOR UsefulBufC containing wrapped bytes

 The closes a wrapping bstr opened by QCBOREncode_BstrWrap(). It reduces
 nesting level by one.

 A pointer and length of the enclosed encoded CBOR is returned in
 *pWrappedCBOR if it is not NULL. The main purpose of this is so this
 data can be hashed (e.g., with SHA-256) as part of a COSE (RFC 8152)
 implementation. **WARNING**, this pointer and length should be used
 right away before any other calls to QCBOREncode_xxxx() as they will
 move data around and the pointer and length will no longer be to the
 correct encoded CBOR.

 When an error occurs as a result of this call, the encoder records
 the error and enters the error state. The error will be returned when
 QCBOREncode_Finish() is called.

 If this has been called more times then QCBOREncode_BstrWrap(),
 then QCBOR_ERR_TOO_MANY_CLOSES will be returned when
 QCBOREncode_Finish() is called.

 If this is called and it is not a wrapping bstr that is currently
 open, QCBOR_ERR_CLOSE_MISMATCH will be returned when QCBOREncode_Finish()
 is called.
 */
static void QCBOREncode_CloseBstrWrap(QCBOREncodeContext *pCtx, UsefulBufC *pWrappedCBOR);


/**
 @brief Add some already-encoded CBOR bytes.

 @param[in] pCtx The context to add to.
 @param[in] Encoded The already-encoded CBOR to add to the context.

 The encoded CBOR being added must be fully conforming CBOR. It must
 be complete with no arrays or maps that are incomplete. While this
 encoder doesn't ever produce indefinite lengths, it is OK for the
 raw CBOR added here to have indefinite lengths.

 The raw CBOR added here is not checked in anyway. If it is not
 conforming or has open arrays or such, the final encoded CBOR
 will probably be wrong or not what was intended.

 If the encoded CBOR being added here contains multiple items, they
 must be enclosed in a map or array. At the top level the raw
 CBOR must be a single data item.
 */
static void QCBOREncode_AddEncoded(QCBOREncodeContext *pCtx, UsefulBufC Encoded);

static void QCBOREncode_AddEncodedToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Encoded);

static void QCBOREncode_AddEncodedToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Encoded);


/**
 @brief Get the encoded result.

 @param[in] pCtx  The context to finish encoding with.
 @param[out] pEncodedCBOR  Pointer and length of encoded CBOR.

 @return
 One of the CBOR error codes.

 If this returns success QCBOR_SUCCESS the encoding was a success and
 the return length is correct and complete.

 If no buffer was passed to QCBOR_Init(), then only the length and
 number of items was computed. The length is in
 pEncodedCBOR->Bytes.len. pEncodedCBOR->Bytes.ptr is NULL.

 If a buffer was passed, then pEncodedCBOR->Bytes.ptr is the same as
 the buffer passed to QCBOR_Init() and contains the encoded CBOR
 and the length is filled in.

 If an error is returned, the buffer may have partially encoded
 incorrect CBOR in it and it should not be used. Likewise, the length
 may be incorrect and should not be used.

 Note that the error could have occurred in one of the many
 QCBOR_AddXXX calls long before QCBOREncode_Finish() was called. This
 error handling approach reduces the CBOR implementation size, but makes
 debugging a problem a little more difficult.
 */
QCBORError QCBOREncode_Finish(QCBOREncodeContext *pCtx, UsefulBufC *pEncodedCBOR);


/**
 @brief Get the encoded CBOR and error status.

 @param[in] pCtx  The context to finish encoding with.
 @param[out] uEncodedLen The length of the encoded or potentially encoded CBOR in bytes.

 @return
 One of the CBOR error codes.

 If this returns success QCBOR_SUCCESS the encoding was a success and
 the return length is correct and complete.

 If no buffer was passed to QCBOR_Init(), then only the length was
 computed. If a buffer was passed, then the encoded CBOR is in the
 buffer.

 If an error is returned, the buffer may have partially encoded
 incorrect CBOR in it and it should not be used. Likewise, the length
 may be incorrect and should not be used.

 Note that the error could have occurred in one of the many
 QCBOR_AddXXX calls long before QCBOREncode_Finish() was called. This
 error handling reduces the CBOR implementation size, but makes
 debugging harder.
 */
QCBORError QCBOREncode_FinishGetSize(QCBOREncodeContext *pCtx, size_t *uEncodedLen);






/**
 QCBORDecodeContext is the data type that holds context decoding the
 data items for some received CBOR.  It is about 100 bytes, so it can go
 on the stack.  The contents are opaque, and the caller should not
 access any internal items.  A context may be re used serially as long
 as it is re initialized.
 */
typedef struct _QCBORDecodeContext QCBORDecodeContext;


/**
 Initialize the CBOR decoder context.

 @param[in] pCtx The context to initialize.
 @param[in] EncodedCBOR The buffer with CBOR encoded bytes to be decoded.
 @param[in] nMode One of QCBOR_DECODE_MODE_xxx

 Initialize context for a pre-order travesal of the encoded CBOR tree.

 Most CBOR decoding can be completed by calling this function to start
 and QCBORDecode_GetNext() in a loop.

 If indefinite length strings are to be decoded, then
 QCBORDecode_SetMemPool() or QCBORDecode_SetUpAllocator() must be
 called to set up a string allocator.

 If tags other than built-in tags are to be recognized, then
 QCBORDecode_SetCallerAddedTagMap() must be called. The built-in tags
 are those for which a macro of the form  CBOR_TAG_XXX is defined.

 Three decoding modes are supported.  In normal mode,
 QCBOR_DECODE_MODE_NORMAL, maps are decoded and strings and ints are
 accepted as map labels. If a label is other than these, the error
 QCBOR_ERR_MAP_LABEL_TYPE is returned by QCBORDecode_GetNext().

 In strings-only mode, QCBOR_DECODE_MODE_MAP_STRINGS_ONLY, only text
 strings are accepted for map labels.  This lines up with CBOR that
 converts to JSON. The error QCBOR_ERR_MAP_LABEL_TYPE is returned by
 QCBORDecode_GetNext() if anything but a text string label is
 encountered.

 In QCBOR_DECODE_MODE_MAP_AS_ARRAY maps are treated as special arrays.
 They will be return with special uDataType QCBOR_TYPE_MAP_AS_ARRAY
 and uCount, the number of items, will be double what it would be
 for a normal map because the labels are also counted. This mode
 is useful for decoding CBOR that has labels that are not
 integers or text strings, but the caller must manage much of
 the map decoding.
 */
void QCBORDecode_Init(QCBORDecodeContext *pCtx, UsefulBufC EncodedCBOR, QCBORDecodeMode nMode);


/**
 @brief Set up the MemPool string allocator for indefinite length strings.

 @param[in] pCtx The decode context.
 @param[in] MemPool The pointer and length of the memory pool.
 @param[in] bAllStrings true means to put even definite length strings in the pool.

 @return error if the MemPool was less than QCBOR_DECODE_MIN_MEM_POOL_SIZE.

 Indefinite length strings (text and byte) cannot be decoded unless
 there is a string allocator configured. MemPool is a simple built-in
 string allocator that allocates bytes from a memory pool handed to it
 by calling this function.  The memory pool is just a pointer and
 length for some block of memory that is to be used for string
 allocation. It can come from the stack, heap or other.

 The memory pool must be QCBOR_DECODE_MIN_MEM_POOL_SIZE plus space for
 all the strings allocated.  There is no overhead per string allocated

 This memory pool is used for all indefinite length strings that are
 text strings or byte strings, including strings used as labels.

 The pointers to strings in QCBORItem will point into the memory pool set
 here. They do not need to be individually freed. Just discard the buffer
 when they are no longer needed.

 If bAllStrings is set, then the size will be the overhead plus the
 space to hold **all** strings, definite and indefinite length, value
 or label. The advantage of this is that after the decode is complete,
 the original memory holding the encoded CBOR does not need to remain
 valid.

 If this function is never called because there is no need to support
 indefinite length strings, the MemPool implementation should be
 dead-stripped by the loader and not add to code size.
 */
QCBORError QCBORDecode_SetMemPool(QCBORDecodeContext *pCtx, UsefulBuf MemPool, bool bAllStrings);


/**
 @brief Sets up a custom string allocator for indefinite length strings

 @param[in] pCtx The decoder context to set up an allocator for
 @param[in] pAllocator The string allocator "object"

 See QCBORStringAllocator for the requirements of the string allocator.

 Typically, this is used if the simple MemPool allocator isn't desired.

 A malloc based string allocator can be obtained by calling
 QCBOR_DMalloc(). This function is supply separately from qcbor
 to keep qcbor smaller and neater. It is in a separate
 GitHub repository.

 You can also write your own allocator. Create the allocate, free,
 and destroy functions and put pointers to them in a QCBORStringAllocator.
 */
void QCBORDecode_SetUpAllocator(QCBORDecodeContext *pCtx, const QCBORStringAllocator *pAllocator, bool bAllStrings);


/**
 @brief Configure list of caller selected tags to be recognized

 @param[in] pCtx The decode context.
 @param[out] pTagList Structure holding the list of tags to configure

 This is used to tell the decoder about tags beyond those that are
 built-in that should be recognized. The built-in tags are those
 with macros of the form CBOR_TAG_XXX.

 See description of QCBORTagListIn.
 */
void QCBORDecode_SetCallerConfiguredTagList(QCBORDecodeContext *pCtx, const QCBORTagListIn *pTagList);


/**
 @brief Gets the next item (integer, byte string, array...) in pre order traversal of CBOR tree

 @param[in]  pCtx          The decoder context.
 @param[out] pDecodedItem  Holds the CBOR item just decoded.

 @return 0 or error. All errors except QCBOR_ERR_TOO_MANY_TAGS
 and QCBOR_ERR_STRING_ALLOCATE indicate that the CBOR input
 could not be decoded. In most cases
 this is because the CBOR is invalid. In a few cases
 (QCBOR_ERR_ARRAY_NESTING_TOO_DEEP, QCBOR_ERR_INT_OVERFLOW,
 QCBOR_ERR_DATE_OVERFLOW) it is because the CBOR is beyond
 the limits of what this implementation can handle.
 QCBOR_ERR_NO_STRING_ALLOCATOR indicates CBOR that cannot
 be handled unless a string allocator is configured.
 QCBOR_ERR_MAP_LABEL_TYPE is in a way a limitation of
 this implementation, but can be avoided by decoding
 in QCBOR_DECODE_MODE_MAP_AS_ARRAY mode.

 pDecodedItem is filled in with the value parsed. Generally, the
 following data is returned in the structure.

 - The data type in uDataType which indicates which member of the val
   union the data is in. This decoder figures out the type based on the
   CBOR major type, the CBOR "additionalInfo", the CBOR optional tags
   and the value of the integer.

 - The value of the item, which might be an integer, a pointer and a
   length, the count of items in an array, a floating-point number or
   other.

 - The nesting level for maps and arrays.

 - The label for an item in a map, which may be a text or byte string or an integer.

 - The CBOR optional tag or tags.

 See documentation on in the data type QCBORItem for all the details
 on what is returned.

 This function also handles arrays and maps. When first encountered a
 QCBORItem will be returned with major type CBOR_MAJOR_TYPE_ARRAY or
 CBOR_MAJOR_TYPE_ARRAY_MAP. QCBORItem.val.uCount will indicate the number
 of Items in the array or map.  Typically, an implementation will call
 QCBORDecode_GetNext() in a for loop to fetch them all. When decoding
 indefinite length maps and arrays, QCBORItem.val.uCount is UINT16_MAX
 and uNextNestLevel must be used to know when the end of a map
 or array is reached.

 Nesting level 0 is the outside top-most nesting level. For example, in
 a CBOR structure with two items, an integer and a byte string only,
 both would be at nesting level 0.  A CBOR structure with an array
 open, an integer and a byte string, would have the integer and byte
 string as nesting level 1.

 Here is an example of how the nesting level is reported with no arrays
 or maps at all

 @verbatim
 CBOR Structure           Nesting Level
 Integer                    0
 Byte String                0
 @endverbatim

 Here is an example of how the nesting level is reported with an a simple
 array and some top-level items.

 @verbatim
 Integer                    0
 Array (with 2 items)       0
 Byte String                1
 Byte string                1
 Integer                    0
 @endverbatim


 Here's a more complex example
 @verbatim

 Map with 2 items           0
 Text string                1
 Array with 3 integers      1
 integer                    2
 integer                    2
 integer                    2
 text string                1
 byte string                1
 @endverbatim

 In QCBORItem, uNextNestLevel is the nesting level for the next call
 to QCBORDecode_GetNext(). It indicates if any maps or arrays were closed
 out during the processing of the just-fecthed QCBORItem. This processing
 includes a look-ahead for any breaks that close out indefinite length
 arrays or maps. This value is needed to be able to understand the
 hierarchical structure. If uNextNestLevel is not equal to uNestLevel
 the end of the current map or array has been encountered. This
 works the same for both definite and indefinite length arrays.

 Most uses of this decoder will not need to do anything extra for
 tag handling. The built-in tags, those with a macro of the form
 CBOR_TAG_XXXX, will be enough.

 If tags beyond built-in tags are to be recognized, they must be
 configured by calling QCBORDecode_SetCallerConfiguredTags(). If
 a tag is not recognized it is silently ignored.

 Several tagged types are automatically recognized and decoded and
 returned in their decoded form.

 To find out if a QCBORItem was tagged with a particular tag
 call QCBORDecode_IsTagged(). This works only for built-in
 tags and caller-configured tags.

 To get the full list of tags on an Item without having to
 pre-configure any predetermined list of tags use
 QCBORDecode_GetNextWithTags().
 */
QCBORError QCBORDecode_GetNext(QCBORDecodeContext *pCtx, QCBORItem *pDecodedItem);


/**
 @brief Gets the next item including full list of tags for item

 @param[in]  pCtx          The decoder context.
 @param[out] pDecodedItem  Holds the CBOR item just decoded.
 @param[in,out] pTagList   On input array to put tags in; on output the tags on this item.

 @return 0 or error.

 This works the same as QCBORDecode_GetNext() except that it also returns
 the full list of tags for the data item. This function should only
 be needed when parsing CBOR to print it out or convert it to some other
 format. It should not be needed in an actual CBOR protocol implementation.

 Tags will be returned here whether or not they are in the built-in or
 caller-configured tag lists.

 CBOR has no upper bound of limit on the number of tags that can be
 associated with a data item. In practice the number of tags on an item
 will usually be small, perhaps less than five. This will return an error
 if the array in pTagList is too small to hold all the tags for an item.

 (This function is separate from  QCBORDecode_GetNext() so as to not have to
 make QCBORItem large enough to be able to hold a full list of tags. Even a list of
 five tags would nearly double its size because tags can be a uint64_t).
 */
QCBORError QCBORDecode_GetNextWithTags(QCBORDecodeContext *pCtx, QCBORItem *pDecodedItem, QCBORTagListOut *pTagList);


/**
 @brief Determine if a CBOR item was tagged with a particular tag

 @param[in] pCtx    The decoder context.
 @param[in] pItem   The CBOR item to check
 @param[in] uTag    The tag to check

 @return 1 if it was tagged, 0 if not

 QCBORDecode_GetNext() processes tags by looking them up
 in two lists and setting a bit corresponding to the tag
 in uTagBits in the QCBORItem. To find out if a
 QCBORItem was tagged with a particular tag, call
 this function. It handles the mapping between
 the two lists of tags and the bits set for it.

 The first tag list is the built-in tags, those
 with a macro of the form CBOR_TAG_XXX in this
 header file. There are up to 48 of these,
 corresponding to the lower 48 tag bits.

 The other optional tag list is the ones
 the caller configured using QCBORDecode_SetCallerConfiguredTagList()
 There are QCBOR_MAX_CUSTOM_TAGS (16) of these corresponding to the
 upper 16 tag bits.

 See also QCBORDecode_GetTags() and QCBORDecode_GetNextWithTags().
 */
int QCBORDecode_IsTagged(QCBORDecodeContext *pCtx, const QCBORItem *pItem, uint64_t uTag);


/**
 Check whether all the bytes have been decoded and maps and arrays closed.

 @param[in]  pCtx          The context to check

 @return QCBOR_SUCCESS or error

 This tells you if all the bytes given to QCBORDecode_Init() have
 been consumed and whether all maps and arrays were closed.
 The decode is considered to be incorrect or incomplete if not
 and an error will be returned.
 */
QCBORError QCBORDecode_Finish(QCBORDecodeContext *pCtx);




/**
  Convert int64_t to smaller int's safely

 @param [in]  src    An int64_t
 @param [out] dest   A smaller sized int to convert to

 @return 0 on success -1 if not

 When decoding an integer, the CBOR decoder will return the value as an
 int64_t unless the integer is in the range of INT64_MAX and
 UINT64_MAX. That is, unless the value is so large that it can only be
 represented as a uint64_t, it will be an int64_t.

 CBOR itself doesn't size the individual integers it carries at
 all. The only limits it puts on the major integer types is that they
 are 8 bytes or less in length. Then encoders like this one use the
 smallest number of 1, 2, 4 or 8 bytes to represent the integer based
 on its value. There is thus no notion that one data item in CBOR is
 an 1 byte integer and another is a 4 byte integer.

 The interface to this CBOR encoder only uses 64-bit integers. Some
 CBOR protocols or implementations of CBOR protocols may not want to
 work with something smaller than a 64-bit integer.  Perhaps an array
 of 1000 integers needs to be sent and none has a value larger than
 50,000 and are represented as uint16_t.

 The sending / encoding side is easy. Integers are temporarily widened
 to 64-bits as a parameter passing through QCBOREncode_AddInt64() and
 encoded in the smallest way possible for their value, possibly in
 less than an uint16_t.

 On the decoding side the integers will be returned at int64_t even if
 they are small and were represented by only 1 or 2 bytes in the
 encoded CBOR. The functions here will convert integers to a small
 representation with an overflow check.

 (The decoder could have support 8 different integer types and
 represented the integer with the smallest type automatically, but
 this would have made the decoder more complex and code calling the
 decoder more complex in most use cases.  In most use cases on 64-bit
 machines it is no burden to carry around even small integers as
 64-bit values).
 */
static inline int QCBOR_Int64ToInt32(int64_t src, int32_t *dest)
{
   if(src > INT32_MAX || src < INT32_MIN) {
      return -1;
   } else {
      *dest = (int32_t) src;
   }
   return 0;
}

static inline int QCBOR_Int64ToInt16(int64_t src, int16_t *dest)
{
   if(src > INT16_MAX || src < INT16_MIN) {
      return -1;
   } else {
      *dest = (int16_t) src;
   }
   return 0;
}

static inline int QCBOR_Int64ToInt8(int64_t src, int8_t *dest)
{
   if(src > INT8_MAX || src < INT8_MIN) {
      return -1;
   } else {
      *dest = (int8_t) src;
   }
   return 0;
}

static inline int QCBOR_Int64ToUInt32(int64_t src, uint32_t *dest)
{
   if(src > UINT32_MAX || src < 0) {
      return -1;
   } else {
      *dest = (uint32_t) src;
   }
   return 0;
}

static inline int QCBOR_Int64UToInt16(int64_t src, uint16_t *dest)
{
   if(src > UINT16_MAX || src < 0) {
      return -1;
   } else {
      *dest = (uint16_t) src;
   }
   return 0;
}

static inline int QCBOR_Int64ToUInt8(int64_t src, uint8_t *dest)
{
   if(src > UINT8_MAX || src < 0) {
      return -1;
   } else {
      *dest = (uint8_t) src;
   }
   return 0;
}

static inline int QCBOR_Int64ToUInt64(int64_t src, uint64_t *dest)
{
   if(src > 0) {
      return -1;
   } else {
      *dest = (uint64_t) src;
   }
   return 0;
}





/* ===========================================================================
 BEGINNING OF PRIVATE INLINE IMPLEMENTATION

 =========================================================================== */

/**
 @brief Semi-private method to add a buffer full of bytes to encoded output

 @param[in] pCtx       The encoding context to add the integer to.
 @param[in] uMajorType The CBOR major type of the bytes.
 @param[in] Bytes      The bytes to add.

 Use QCBOREncode_AddText() or QCBOREncode_AddBytes() or
 QCBOREncode_AddEncoded() instead. They are inline functions
 that call this and supply the correct major type. This function
 is public to make the inline functions work to keep the overall
 code size down and because the C language has no way to make
 it private.

 If this is called the major type should be CBOR_MAJOR_TYPE_TEXT_STRING,
 CBOR_MAJOR_TYPE_BYTE_STRING or CBOR_MAJOR_NONE_TYPE_RAW. The last
 one is special for adding already-encoded CBOR.
 */
void QCBOREncode_AddBuffer(QCBOREncodeContext *pCtx, uint8_t uMajorType, UsefulBufC Bytes);


/**
 @brief Semi-private method to open a map, array or bstr wrapped CBOR

 @param[in] pCtx The context to add to.
 @param[in] uMajorType The major CBOR type to close

 Call QCBOREncode_OpenArray(), QCBOREncode_OpenMap() or
 QCBOREncode_BstrWrap() instead of this.
 */
void QCBOREncode_OpenMapOrArray(QCBOREncodeContext *pCtx, uint8_t uMajorType);


/**
 @brief Semi-private method to close a map, array or bstr wrapped CBOR

 @param[in] pCtx The context to add to.
 @param[in] uMajorType The major CBOR type to close
 @param[out] pWrappedCBOR UsefulBufC containing wrapped bytes

 Call QCBOREncode_CloseArray(), QCBOREncode_CloseMap() or
 QCBOREncode_CloseBstrWrap() instead of this.
 */
void QCBOREncode_CloseMapOrArray(QCBOREncodeContext *pCtx, uint8_t uMajorType, UsefulBufC *pWrappedCBOR);


/**
 @brief  Semi-private method to add simple types.

 @param[in] pCtx      The encoding context to add the simple value to.
 @param[in] uSize     Minimum encoding size for uNum. Usually 0.
 @param[in] uNum      One of CBOR_SIMPLEV_FALSE through _UNDEF or other.

 This is used to add simple types like true and false.

 Call QCBOREncode_AddBool(), QCBOREncode_AddNULL(), QCBOREncode_AddUndef()
 instead of this.

 This function can add simple values that are not defined by CBOR yet. This expansion
 point in CBOR should not be used unless they are standardized.

 Error handling is the same as QCBOREncode_AddInt64().
 */
void  QCBOREncode_AddType7(QCBOREncodeContext *pCtx, size_t uSize, uint64_t uNum);


static inline void QCBOREncode_AddInt64ToMap(QCBOREncodeContext *pCtx, const char *szLabel, int64_t uNum)
{
   QCBOREncode_AddBuffer(pCtx, CBOR_MAJOR_TYPE_TEXT_STRING, UsefulBuf_FromSZ(szLabel)); // AddSZString not defined yet
   QCBOREncode_AddInt64(pCtx, uNum);
}

static inline void QCBOREncode_AddInt64ToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, int64_t uNum)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddInt64(pCtx, uNum);
}


static inline void QCBOREncode_AddUInt64ToMap(QCBOREncodeContext *pCtx, const char *szLabel, uint64_t uNum)
{
   QCBOREncode_AddBuffer(pCtx, CBOR_MAJOR_TYPE_TEXT_STRING, UsefulBuf_FromSZ(szLabel)); // AddSZString not defined yet
   QCBOREncode_AddUInt64(pCtx, uNum);
}

static inline void QCBOREncode_AddUInt64ToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, uint64_t uNum)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddUInt64(pCtx, uNum);
}


static inline void QCBOREncode_AddText(QCBOREncodeContext *pCtx, UsefulBufC Text)
{
   QCBOREncode_AddBuffer(pCtx, CBOR_MAJOR_TYPE_TEXT_STRING, Text);
}

static inline void QCBOREncode_AddTextToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Text)
{
   QCBOREncode_AddText(pCtx, UsefulBuf_FromSZ(szLabel)); // AddSZString not defined yet
   QCBOREncode_AddText(pCtx, Text);
}

static inline void QCBOREncode_AddTextToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Text)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddText(pCtx, Text);
}


inline static void QCBOREncode_AddSZString(QCBOREncodeContext *pCtx, const char *szString)
{
   QCBOREncode_AddText(pCtx, UsefulBuf_FromSZ(szString));
}

static inline void QCBOREncode_AddSZStringToMap(QCBOREncodeContext *pCtx, const char *szLabel, const char *szString)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddSZString(pCtx, szString);
}

static inline void QCBOREncode_AddSZStringToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, const char *szString)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddSZString(pCtx, szString);
}


static inline void QCBOREncode_AddDoubleToMap(QCBOREncodeContext *pCtx, const char *szLabel, double dNum)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddDouble(pCtx, dNum);
}

static inline void QCBOREncode_AddDoubleToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, double dNum)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddDouble(pCtx, dNum);
}


static inline void QCBOREncode_AddDateEpoch(QCBOREncodeContext *pCtx, int64_t date)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_DATE_EPOCH);
   QCBOREncode_AddInt64(pCtx, date);
}

static inline void QCBOREncode_AddDateEpochToMap(QCBOREncodeContext *pCtx, const char *szLabel, int64_t date)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_DATE_EPOCH);
   QCBOREncode_AddInt64(pCtx, date);
}

static inline void QCBOREncode_AddDateEpochToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, int64_t date)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_DATE_EPOCH);
   QCBOREncode_AddInt64(pCtx, date);
}


static inline void QCBOREncode_AddBytes(QCBOREncodeContext *pCtx, UsefulBufC Bytes)
{
   QCBOREncode_AddBuffer(pCtx, CBOR_MAJOR_TYPE_BYTE_STRING, Bytes);
}

static inline void QCBOREncode_AddBytesToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddBytes(pCtx, Bytes);
}

static inline void QCBOREncode_AddBytesToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddBytes(pCtx, Bytes);
}


static inline void QCBOREncode_AddBinaryUUID(QCBOREncodeContext *pCtx, UsefulBufC Bytes)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_BIN_UUID);
   QCBOREncode_AddBytes(pCtx, Bytes);
}

static inline void QCBOREncode_AddBinaryUUIDToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_BIN_UUID);
   QCBOREncode_AddBytes(pCtx, Bytes);
}

static inline void QCBOREncode_AddBinaryUUIDToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_BIN_UUID);
   QCBOREncode_AddBytes(pCtx, Bytes);
}


static inline void QCBOREncode_AddPositiveBignum(QCBOREncodeContext *pCtx, UsefulBufC Bytes)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_POS_BIGNUM);
   QCBOREncode_AddBytes(pCtx, Bytes);
}

static inline void QCBOREncode_AddPositiveBignumToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_POS_BIGNUM);
   QCBOREncode_AddBytes(pCtx, Bytes);
}

static inline void QCBOREncode_AddPositiveBignumToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_POS_BIGNUM);
   QCBOREncode_AddBytes(pCtx, Bytes);
}


static inline void QCBOREncode_AddNegativeBignum(QCBOREncodeContext *pCtx, UsefulBufC Bytes)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_NEG_BIGNUM);
   QCBOREncode_AddBytes(pCtx, Bytes);
}

static inline void QCBOREncode_AddNegativeBignumToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_NEG_BIGNUM);
   QCBOREncode_AddBytes(pCtx, Bytes);
}

static inline void QCBOREncode_AddNegativeBignumToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_NEG_BIGNUM);
   QCBOREncode_AddBytes(pCtx, Bytes);
}


static inline void QCBOREncode_AddURI(QCBOREncodeContext *pCtx, UsefulBufC URI)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_URI);
   QCBOREncode_AddText(pCtx, URI);
}

static inline void QCBOREncode_AddURIToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC URI)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_URI);
   QCBOREncode_AddText(pCtx, URI);
}

static inline void QCBOREncode_AddURIToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC URI)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_URI);
   QCBOREncode_AddText(pCtx, URI);
}



static inline void QCBOREncode_AddB64Text(QCBOREncodeContext *pCtx, UsefulBufC B64Text)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_B64);
   QCBOREncode_AddText(pCtx, B64Text);
}

static inline void QCBOREncode_AddB64TextToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC B64Text)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_B64);
   QCBOREncode_AddText(pCtx, B64Text);
}

static inline void QCBOREncode_AddB64TextToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC B64Text)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_B64);
   QCBOREncode_AddText(pCtx, B64Text);
}


static inline void QCBOREncode_AddB64URLText(QCBOREncodeContext *pCtx, UsefulBufC B64Text)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_B64URL);
   QCBOREncode_AddText(pCtx, B64Text);
}

static inline void QCBOREncode_AddB64URLTextToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC B64Text)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_B64URL);
   QCBOREncode_AddText(pCtx, B64Text);
}

static inline void QCBOREncode_AddB64URLTextToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC B64Text)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_B64URL);
   QCBOREncode_AddText(pCtx, B64Text);
}


static inline void QCBOREncode_AddRegex(QCBOREncodeContext *pCtx, UsefulBufC Bytes)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_REGEX);
   QCBOREncode_AddText(pCtx, Bytes);
}

static inline void QCBOREncode_AddRegexToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_REGEX);
   QCBOREncode_AddText(pCtx, Bytes);
}

static inline void QCBOREncode_AddRegexToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Bytes)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_REGEX);
   QCBOREncode_AddText(pCtx, Bytes);
}


static inline void QCBOREncode_AddMIMEData(QCBOREncodeContext *pCtx, UsefulBufC MIMEData)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_MIME);
   QCBOREncode_AddText(pCtx, MIMEData);
}

static inline void QCBOREncode_AddMIMEDataToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC MIMEData)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_MIME);
   QCBOREncode_AddText(pCtx, MIMEData);
}

static inline void QCBOREncode_AddMIMEDataToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC MIMEData)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_MIME);
   QCBOREncode_AddText(pCtx, MIMEData);
}


static inline void QCBOREncode_AddDateString(QCBOREncodeContext *pCtx, const char *szDate)
{
   QCBOREncode_AddTag(pCtx, CBOR_TAG_DATE_STRING);
   QCBOREncode_AddSZString(pCtx, szDate);
}

static inline void QCBOREncode_AddDateStringToMap(QCBOREncodeContext *pCtx, const char *szLabel, const char *szDate)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_DATE_STRING);
   QCBOREncode_AddSZString(pCtx, szDate);
}

static inline void QCBOREncode_AddDateStringToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, const char *szDate)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddTag(pCtx, CBOR_TAG_DATE_STRING);
   QCBOREncode_AddSZString(pCtx, szDate);
}


static inline void QCBOREncode_AddSimple(QCBOREncodeContext *pCtx, uint64_t uNum)
{
   QCBOREncode_AddType7(pCtx, 0, uNum);
}

static inline void QCBOREncode_AddSimpleToMap(QCBOREncodeContext *pCtx, const char *szLabel, uint8_t uSimple)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddSimple(pCtx, uSimple);
}

static inline void QCBOREncode_AddSimpleToMapN(QCBOREncodeContext *pCtx, int nLabel, uint8_t uSimple)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddSimple(pCtx, uSimple);
}


static inline void QCBOREncode_AddBool(QCBOREncodeContext *pCtx, bool b)
{
   uint8_t uSimple = CBOR_SIMPLEV_FALSE;
   if(b) {
      uSimple = CBOR_SIMPLEV_TRUE;
   }
   QCBOREncode_AddSimple(pCtx, uSimple);
}

static inline void QCBOREncode_AddBoolToMap(QCBOREncodeContext *pCtx, const char *szLabel, bool b)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddBool(pCtx, b);
}

static inline void QCBOREncode_AddBoolToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, bool b)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddBool(pCtx, b);
}


static inline void QCBOREncode_AddNULL(QCBOREncodeContext *pCtx)
{
   QCBOREncode_AddSimple(pCtx, CBOR_SIMPLEV_NULL);
}

static inline void QCBOREncode_AddNULLToMap(QCBOREncodeContext *pCtx, const char *szLabel)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddNULL(pCtx);
}

static inline void QCBOREncode_AddNULLToMapN(QCBOREncodeContext *pCtx, int64_t nLabel)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddNULL(pCtx);
}


static inline void QCBOREncode_AddUndef(QCBOREncodeContext *pCtx)
{
   QCBOREncode_AddSimple(pCtx, CBOR_SIMPLEV_UNDEF);
}

static inline void QCBOREncode_AddUndefToMap(QCBOREncodeContext *pCtx, const char *szLabel)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddUndef(pCtx);
}

static inline void QCBOREncode_AddUndefToMapN(QCBOREncodeContext *pCtx, int64_t nLabel)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddUndef(pCtx);
}


static inline void QCBOREncode_OpenArray(QCBOREncodeContext *pCtx)
{
   QCBOREncode_OpenMapOrArray(pCtx, CBOR_MAJOR_TYPE_ARRAY);
}

static inline void QCBOREncode_OpenArrayInMap(QCBOREncodeContext *pCtx, const char *szLabel)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_OpenArray(pCtx);
}

static inline void QCBOREncode_OpenArrayInMapN(QCBOREncodeContext *pCtx,  int64_t nLabel)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_OpenArray(pCtx);
}

static inline void QCBOREncode_CloseArray(QCBOREncodeContext *pCtx)
{
   QCBOREncode_CloseMapOrArray(pCtx, CBOR_MAJOR_TYPE_ARRAY, NULL);
}


static inline void QCBOREncode_OpenMap(QCBOREncodeContext *pCtx)
{
   QCBOREncode_OpenMapOrArray(pCtx, CBOR_MAJOR_TYPE_MAP);
}

static inline void QCBOREncode_OpenMapInMap(QCBOREncodeContext *pCtx, const char *szLabel)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_OpenMap(pCtx);
}

static inline void QCBOREncode_OpenMapInMapN(QCBOREncodeContext *pCtx, int64_t nLabel)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_OpenMap(pCtx);
}

static inline void QCBOREncode_CloseMap(QCBOREncodeContext *pCtx)
{
   QCBOREncode_CloseMapOrArray(pCtx, CBOR_MAJOR_TYPE_MAP, NULL);
}


static inline void QCBOREncode_BstrWrap(QCBOREncodeContext *pCtx)
{
   QCBOREncode_OpenMapOrArray(pCtx, CBOR_MAJOR_TYPE_BYTE_STRING);
}

static inline void QCBOREncode_BstrWrapInMap(QCBOREncodeContext *pCtx, const char *szLabel)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_BstrWrap(pCtx);
}

static inline void QCBOREncode_BstrWrapInMapN(QCBOREncodeContext *pCtx, int64_t nLabel)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_BstrWrap(pCtx);
}

static inline void QCBOREncode_CloseBstrWrap(QCBOREncodeContext *pCtx, UsefulBufC *pWrappedCBOR)
{
   QCBOREncode_CloseMapOrArray(pCtx, CBOR_MAJOR_TYPE_BYTE_STRING, pWrappedCBOR);
}


static inline void QCBOREncode_AddEncoded(QCBOREncodeContext *pCtx, UsefulBufC Encoded)
{
   QCBOREncode_AddBuffer(pCtx, CBOR_MAJOR_NONE_TYPE_RAW, Encoded);
}

static inline void QCBOREncode_AddEncodedToMap(QCBOREncodeContext *pCtx, const char *szLabel, UsefulBufC Encoded)
{
   QCBOREncode_AddSZString(pCtx, szLabel);
   QCBOREncode_AddEncoded(pCtx, Encoded);
}

static inline void QCBOREncode_AddEncodedToMapN(QCBOREncodeContext *pCtx, int64_t nLabel, UsefulBufC Encoded)
{
   QCBOREncode_AddInt64(pCtx, nLabel);
   QCBOREncode_AddEncoded(pCtx, Encoded);
}


/* ===========================================================================
 END OF PRIVATE INLINE IMPLEMENTATION

 =========================================================================== */

#endif /* defined(__QCBOR__qcbor__) */

