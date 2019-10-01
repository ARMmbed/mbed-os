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
 FILE:  qcbor_decode.c

 DESCRIPTION:  This file contains the implementation of QCBOR.

 EDIT HISTORY FOR FILE:

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

 when               who             what, where, why
 --------           ----            ---------------------------------------------------
 01/10/19           llundblade      Clever type and argument decoder is 250 bytes smaller
 11/9/18            llundblade      Error codes are now enums.
 11/2/18            llundblade      Simplify float decoding and align with preferred
                                    float encoding
 10/31/18           llundblade      Switch to one license that is almost BSD-3.
 10/28/18           llundblade      Reworked tag decoding
 10/15/18           llundblade      Indefinite length maps and arrays supported
 10/8/18            llundblade      Indefinite length strings supported
 02/04/17           llundbla        Work on CPUs that don's require pointer alignment
                                    by making use of changes in UsefulBuf
 03/01/17           llundbla        More data types; decoding improvements and fixes
 11/13/16           llundbla        Integrate most TZ changes back into github version.
 09/30/16           gkanike         Porting to TZ.
 03/15/16           llundbla        Initial Version.

 =====================================================================================*/

#include "qcbor.h"
#include "ieee754.h"


/*
 This casts away the const-ness of a pointer, usually so it can be
 freed or realloced.
 */
#define UNCONST_POINTER(ptr)    ((void *)(ptr))


/*
 Collection of functions to track the map/array nesting for decoding
 */

inline static int IsMapOrArray(uint8_t uDataType)
{
   return uDataType == QCBOR_TYPE_MAP || uDataType == QCBOR_TYPE_ARRAY;
}

inline static int DecodeNesting_IsNested(const QCBORDecodeNesting *pNesting)
{
   return pNesting->pCurrent != &(pNesting->pMapsAndArrays[0]);
}

inline static int DecodeNesting_IsIndefiniteLength(const QCBORDecodeNesting *pNesting)
{
   return pNesting->pCurrent->uCount == UINT16_MAX;
}

inline static uint8_t DecodeNesting_GetLevel(QCBORDecodeNesting *pNesting)
{
   return pNesting->pCurrent - &(pNesting->pMapsAndArrays[0]);
}

inline static int DecodeNesting_TypeIsMap(const QCBORDecodeNesting *pNesting)
{
   if(!DecodeNesting_IsNested(pNesting)) {
      return 0;
   }

   return CBOR_MAJOR_TYPE_MAP == pNesting->pCurrent->uMajorType;
}

// Process a break. This will either ascend the nesting or error out
inline static QCBORError DecodeNesting_BreakAscend(QCBORDecodeNesting *pNesting)
{
   // breaks must always occur when there is nesting
   if(!DecodeNesting_IsNested(pNesting)) {
      return QCBOR_ERR_BAD_BREAK;
   }

   // breaks can only occur when the map/array is indefinite length
   if(!DecodeNesting_IsIndefiniteLength(pNesting)) {
      return QCBOR_ERR_BAD_BREAK;
   }

   // if all OK, the break reduces the level of nesting
   pNesting->pCurrent--;

   return QCBOR_SUCCESS;
}

// Called on every single item except breaks including the opening of a map/array
inline static void DecodeNesting_DecrementCount(QCBORDecodeNesting *pNesting)
{
   if(!DecodeNesting_IsNested(pNesting)) {
      // at top level where there is no tracking
      return;
   }

   if(DecodeNesting_IsIndefiniteLength(pNesting)) {
      // There is no count for indefinite length arrays/maps
      return;
   }

   // Decrement the count of items in this array/map
   pNesting->pCurrent->uCount--;

   // Pop up nesting levels if the counts at the levels are zero
   while(DecodeNesting_IsNested(pNesting) && 0 == pNesting->pCurrent->uCount) {
      pNesting->pCurrent--;
      if(!DecodeNesting_IsIndefiniteLength(pNesting)) {
         pNesting->pCurrent->uCount--;
      }
   }
}

// Called on every map/array
inline static QCBORError DecodeNesting_Descend(QCBORDecodeNesting *pNesting, QCBORItem *pItem)
{
   QCBORError nReturn = QCBOR_SUCCESS;

   if(pItem->val.uCount == 0) {
      // Nothing to do for empty definite lenth arrays. They are just are
      // effectively the same as an item that is not a map or array
      goto Done;
      // Empty indefinite length maps and arrays are handled elsewhere
   }

   // Error out if arrays is too long to handle
   if(pItem->val.uCount != UINT16_MAX && pItem->val.uCount > QCBOR_MAX_ITEMS_IN_ARRAY) {
      nReturn = QCBOR_ERR_ARRAY_TOO_LONG;
      goto Done;
   }

   // Error out if nesting is too deep
   if(pNesting->pCurrent >= &(pNesting->pMapsAndArrays[QCBOR_MAX_ARRAY_NESTING])) {
      nReturn = QCBOR_ERR_ARRAY_NESTING_TOO_DEEP;
      goto Done;
   }

   // The actual descend
   pNesting->pCurrent++;

   // Record a few details for this nesting level
   pNesting->pCurrent->uMajorType = pItem->uDataType;
   pNesting->pCurrent->uCount     = pItem->val.uCount;

Done:
   return nReturn;;
}

inline static void DecodeNesting_Init(QCBORDecodeNesting *pNesting)
{
   pNesting->pCurrent = &(pNesting->pMapsAndArrays[0]);
}



/*
 This list of built-in tags. Only add tags here that are
 clearly established and useful. Once a tag is added here
 it can't be taken out as that would break backwards compatibility.
 There are only 48 slots available forever.
 */
static const uint16_t spBuiltInTagMap[] = {
   CBOR_TAG_DATE_STRING, // See TAG_MAPPER_FIRST_FOUR
   CBOR_TAG_DATE_EPOCH, // See TAG_MAPPER_FIRST_FOUR
   CBOR_TAG_POS_BIGNUM, // See TAG_MAPPER_FIRST_FOUR
   CBOR_TAG_NEG_BIGNUM, // See TAG_MAPPER_FIRST_FOUR
   CBOR_TAG_FRACTION,
   CBOR_TAG_BIGFLOAT,
   CBOR_TAG_COSE_ENCRYPTO,
   CBOR_TAG_COSE_MAC0,
   CBOR_TAG_COSE_SIGN1,
   CBOR_TAG_ENC_AS_B64URL,
   CBOR_TAG_ENC_AS_B64,
   CBOR_TAG_ENC_AS_B16,
   CBOR_TAG_CBOR,
   CBOR_TAG_URI,
   CBOR_TAG_B64URL,
   CBOR_TAG_B64,
   CBOR_TAG_REGEX,
   CBOR_TAG_MIME,
   CBOR_TAG_BIN_UUID,
   CBOR_TAG_CWT,
   CBOR_TAG_ENCRYPT,
   CBOR_TAG_MAC,
   CBOR_TAG_SIGN,
   CBOR_TAG_GEO_COORD,
   CBOR_TAG_CBOR_MAGIC
};

// This is used in a bit of cleverness in GetNext_TaggedItem() to
// keep code size down and switch for the internal processing of
// these types. This will break if the first four items in
// spBuiltInTagMap don't have values 0,1,2,3. That is the
// mapping is 0 to 0, 1 to 1, 2 to 2 and 3 to 3.
#define QCBOR_TAGFLAG_DATE_STRING    (0x01LL << CBOR_TAG_DATE_STRING)
#define QCBOR_TAGFLAG_DATE_EPOCH     (0x01LL << CBOR_TAG_DATE_EPOCH)
#define QCBOR_TAGFLAG_POS_BIGNUM     (0x01LL << CBOR_TAG_POS_BIGNUM)
#define QCBOR_TAGFLAG_NEG_BIGNUM     (0x01LL << CBOR_TAG_NEG_BIGNUM)

#define TAG_MAPPER_FIRST_FOUR (QCBOR_TAGFLAG_DATE_STRING |\
                               QCBOR_TAGFLAG_DATE_EPOCH  |\
                               QCBOR_TAGFLAG_POS_BIGNUM  |\
                               QCBOR_TAGFLAG_NEG_BIGNUM)

#define TAG_MAPPER_TOTAL_TAG_BITS 64 // Number of bits in a uint64_t
#define TAG_MAPPER_CUSTOM_TAGS_BASE_INDEX (TAG_MAPPER_TOTAL_TAG_BITS - QCBOR_MAX_CUSTOM_TAGS) // 48
#define TAG_MAPPER_MAX_SIZE_BUILT_IN_TAGS (TAG_MAPPER_TOTAL_TAG_BITS - QCBOR_MAX_CUSTOM_TAGS ) // 48

static inline int TagMapper_LookupBuiltIn(uint64_t uTag)
{
   if(sizeof(spBuiltInTagMap)/sizeof(uint16_t) > TAG_MAPPER_MAX_SIZE_BUILT_IN_TAGS) {
      // This is a cross-check to make sure the above array doesn't
      // accidentally get made too big.
      // In normal conditions the above test should optimize out
      // as all the values are known at compile time.
      return -1;
   }

   if(uTag > UINT16_MAX) {
      // This tag map works only on 16-bit tags
      return -1;
   }

   for(int nTagBitIndex = 0; nTagBitIndex < (int)(sizeof(spBuiltInTagMap)/sizeof(uint16_t)); nTagBitIndex++) {
      if(spBuiltInTagMap[nTagBitIndex] == uTag) {
         return nTagBitIndex;
      }
   }
   return -1; // Indicates no match
}

static inline int TagMapper_LookupCallerConfigured(const QCBORTagListIn *pCallerConfiguredTagMap, uint64_t uTag)
{
   for(int nTagBitIndex = 0; nTagBitIndex < pCallerConfiguredTagMap->uNumTags; nTagBitIndex++) {
      if(pCallerConfiguredTagMap->puTags[nTagBitIndex] == uTag) {
         return nTagBitIndex + TAG_MAPPER_CUSTOM_TAGS_BASE_INDEX;
      }
   }

   return -1; // Indicates no match
}

/*
  Find the tag bit index for a given tag value, or error out

 This and the above functions could probably be optimized and made
 clearer and neater.
 */
static QCBORError TagMapper_Lookup(const QCBORTagListIn *pCallerConfiguredTagMap, uint64_t uTag, uint8_t *puTagBitIndex)
{
   int nTagBitIndex = TagMapper_LookupBuiltIn(uTag);
   if(nTagBitIndex >= 0) {
      // Cast is safe because TagMapper_LookupBuiltIn never returns > 47
      *puTagBitIndex = (uint8_t)nTagBitIndex;
      return QCBOR_SUCCESS;
   }

   if(pCallerConfiguredTagMap) {
      if(pCallerConfiguredTagMap->uNumTags > QCBOR_MAX_CUSTOM_TAGS) {
         return QCBOR_ERR_TOO_MANY_TAGS;
      }
      nTagBitIndex = TagMapper_LookupCallerConfigured(pCallerConfiguredTagMap, uTag);
      if(nTagBitIndex >= 0) {
         // Cast is safe because TagMapper_LookupBuiltIn never returns > 63

         *puTagBitIndex = (uint8_t)nTagBitIndex;
         return QCBOR_SUCCESS;
      }
   }

   return QCBOR_ERR_BAD_OPT_TAG;
}




/*
 Public function, see header file
 */
void QCBORDecode_Init(QCBORDecodeContext *me, UsefulBufC EncodedCBOR, QCBORDecodeMode nDecodeMode)
{
   memset(me, 0, sizeof(QCBORDecodeContext));
   UsefulInputBuf_Init(&(me->InBuf), EncodedCBOR);
   // Don't bother with error check on decode mode. If a bad value is passed it will just act as
   // if the default normal mode of 0 was set.
   me->uDecodeMode = nDecodeMode;
   DecodeNesting_Init(&(me->nesting));
}


/*
 Public function, see header file
 */
void QCBORDecode_SetUpAllocator(QCBORDecodeContext *pCtx, const QCBORStringAllocator *pAllocator, bool bAllocAll)
{
   pCtx->pStringAllocator = (void *)pAllocator;
   pCtx->bStringAllocateAll = bAllocAll;
}

void QCBORDecode_SetCallerConfiguredTagList(QCBORDecodeContext *me, const QCBORTagListIn *pTagList)
{
   me->pCallerConfiguredTagList = pTagList;
}


/*
 This decodes the fundamental part of a CBOR data item, the type and number

 This is the Counterpart to InsertEncodedTypeAndNumber().

 This does the network->host byte order conversion. The conversion here
 also results in the conversion for floats in addition to that for
 lengths, tags and integer values.

 This returns:
   pnMajorType -- the major type for the item
   puNumber -- the "number" which is used a the value for integers, tags and floats and length for strings and arrays
   puAdditionalInfo -- Pass this along to know what kind of float or if length is indefinite

 */
inline static QCBORError DecodeTypeAndNumber(UsefulInputBuf *pUInBuf,
                                              int *pnMajorType,
                                              uint64_t *puArgument,
                                              uint8_t *puAdditionalInfo)
{
   QCBORError nReturn;

   // Get the initial byte that every CBOR data item has
   const uint8_t uInitialByte = UsefulInputBuf_GetByte(pUInBuf);

   // Break down the initial byte
   const uint8_t uTmpMajorType   = uInitialByte >> 5;
   const uint8_t uAdditionalInfo = uInitialByte & 0x1f;

   // Where the number or argument accumulates
   uint64_t uArgument;

   if(uAdditionalInfo >= LEN_IS_ONE_BYTE && uAdditionalInfo <= LEN_IS_EIGHT_BYTES) {
      // Need to get 1,2,4 or 8 additional argument bytes
      // Map LEN_IS_ONE_BYTE.. LEN_IS_EIGHT_BYTES to actual length
      static const uint8_t aIterate[] = {1,2,4,8};

      // Loop getting all the bytes in the argument
      uArgument = 0;
      for(int i = aIterate[uAdditionalInfo - LEN_IS_ONE_BYTE]; i; i--) {
         // This shift and add gives the endian conversion
         uArgument = (uArgument << 8) + UsefulInputBuf_GetByte(pUInBuf);
      }
   } else if(uAdditionalInfo >= ADDINFO_RESERVED1 && uAdditionalInfo <= ADDINFO_RESERVED3) {
      // The reserved and thus-far unused additional info values
      nReturn = QCBOR_ERR_UNSUPPORTED;
      goto Done;
   } else {
      // Less than 24, additional info is argument or 31, an indefinite length
      // No more bytes to get
      uArgument = uAdditionalInfo;
   }

   if(UsefulInputBuf_GetError(pUInBuf)) {
      nReturn = QCBOR_ERR_HIT_END;
      goto Done;
   }

   // All successful if we got here.
   nReturn           = QCBOR_SUCCESS;
   *pnMajorType      = uTmpMajorType;
   *puArgument       = uArgument;
   *puAdditionalInfo = uAdditionalInfo;

Done:
   return nReturn;
}

/*
 CBOR doesn't explicitly specify two's compliment for integers but all CPUs
 use it these days and the test vectors in the RFC are so. All integers in the CBOR
 structure are positive and the major type indicates positive or negative.
 CBOR can express positive integers up to 2^x - 1 where x is the number of bits
 and negative integers down to 2^x.  Note that negative numbers can be one
 more away from zero than positive.
 Stdint, as far as I can tell, uses two's compliment to represent
 negative integers.

 See http://www.unix.org/whitepapers/64bit.html for reasons int isn't
 used here in any way including in the interface
 */
inline static QCBORError DecodeInteger(int nMajorType, uint64_t uNumber, QCBORItem *pDecodedItem)
{
   // Stack usage: int/ptr 1 -- 8
   QCBORError nReturn = QCBOR_SUCCESS;

   if(nMajorType == CBOR_MAJOR_TYPE_POSITIVE_INT) {
      if (uNumber <= INT64_MAX) {
         pDecodedItem->val.int64 = (int64_t)uNumber;
         pDecodedItem->uDataType = QCBOR_TYPE_INT64;

      } else {
         pDecodedItem->val.uint64 = uNumber;
         pDecodedItem->uDataType  = QCBOR_TYPE_UINT64;

      }
   } else {
      if(uNumber <= INT64_MAX) {
         pDecodedItem->val.int64 = -uNumber-1;
         pDecodedItem->uDataType = QCBOR_TYPE_INT64;

      } else {
         // C can't represent a negative integer in this range
         // so it is an error.  todo -- test this condition
         nReturn = QCBOR_ERR_INT_OVERFLOW;
      }
   }

   return nReturn;
}

// Make sure #define value line up as DecodeSimple counts on this.
#if QCBOR_TYPE_FALSE != CBOR_SIMPLEV_FALSE
#error QCBOR_TYPE_FALSE macro value wrong
#endif

#if QCBOR_TYPE_TRUE != CBOR_SIMPLEV_TRUE
#error QCBOR_TYPE_TRUE macro value wrong
#endif

#if QCBOR_TYPE_NULL != CBOR_SIMPLEV_NULL
#error QCBOR_TYPE_NULL macro value wrong
#endif

#if QCBOR_TYPE_UNDEF != CBOR_SIMPLEV_UNDEF
#error QCBOR_TYPE_UNDEF macro value wrong
#endif

#if QCBOR_TYPE_BREAK != CBOR_SIMPLE_BREAK
#error QCBOR_TYPE_BREAK macro value wrong
#endif

#if QCBOR_TYPE_DOUBLE != DOUBLE_PREC_FLOAT
#error QCBOR_TYPE_DOUBLE macro value wrong
#endif

#if QCBOR_TYPE_FLOAT != SINGLE_PREC_FLOAT
#error QCBOR_TYPE_FLOAT macro value wrong
#endif

/*
 Decode true, false, floats, break...
 */

inline static QCBORError DecodeSimple(uint8_t uAdditionalInfo, uint64_t uNumber, QCBORItem *pDecodedItem)
{
   // Stack usage: 0
   QCBORError nReturn = QCBOR_SUCCESS;

   // uAdditionalInfo is 5 bits from the initial byte
   // compile time checks above make sure uAdditionalInfo values line up with uDataType values
   pDecodedItem->uDataType = uAdditionalInfo;

   switch(uAdditionalInfo) {
      case ADDINFO_RESERVED1:  // 28
      case ADDINFO_RESERVED2:  // 29
      case ADDINFO_RESERVED3:  // 30
         nReturn = QCBOR_ERR_UNSUPPORTED;
         break;

      case HALF_PREC_FLOAT:
         pDecodedItem->val.dfnum = IEEE754_HalfToDouble((uint16_t)uNumber);
         pDecodedItem->uDataType = QCBOR_TYPE_DOUBLE;
         break;
      case SINGLE_PREC_FLOAT:
         pDecodedItem->val.dfnum = (double)UsefulBufUtil_CopyUint32ToFloat((uint32_t)uNumber);
         pDecodedItem->uDataType = QCBOR_TYPE_DOUBLE;
         break;
      case DOUBLE_PREC_FLOAT:
         pDecodedItem->val.dfnum = UsefulBufUtil_CopyUint64ToDouble(uNumber);
         pDecodedItem->uDataType = QCBOR_TYPE_DOUBLE;
         break;

      case CBOR_SIMPLEV_FALSE: // 20
      case CBOR_SIMPLEV_TRUE:  // 21
      case CBOR_SIMPLEV_NULL:  // 22
      case CBOR_SIMPLEV_UNDEF: // 23
      case CBOR_SIMPLE_BREAK:  // 31
         break; // nothing to do

      case CBOR_SIMPLEV_ONEBYTE: // 24
         if(uNumber <= CBOR_SIMPLE_BREAK) {
            // This takes out f8 00 ... f8 1f which should be encoded as e0 … f7
            nReturn = QCBOR_ERR_INVALID_CBOR;
            goto Done;
         }
         /* FALLTHROUGH */
         // fall through intentionally

      default: // 0-19
         pDecodedItem->uDataType   = QCBOR_TYPE_UKNOWN_SIMPLE;
         // DecodeTypeAndNumber will make uNumber equal to uAdditionalInfo when uAdditionalInfo is < 24
         // This cast is safe because the 2, 4 and 8 byte lengths of uNumber are in the double/float cases above
         pDecodedItem->val.uSimple = (uint8_t)uNumber;
         break;
   }

Done:
   return nReturn;
}



/*
 Decode text and byte strings. Call the string allocator if asked to.
 */
inline static QCBORError DecodeBytes(const QCBORStringAllocator *pAlloc, int nMajorType, uint64_t uStrLen, UsefulInputBuf *pUInBuf, QCBORItem *pDecodedItem)
{
   // Stack usage: UsefulBuf 2, int/ptr 1  40
   QCBORError nReturn = QCBOR_SUCCESS;

   const UsefulBufC Bytes = UsefulInputBuf_GetUsefulBuf(pUInBuf, uStrLen);
   if(UsefulBuf_IsNULLC(Bytes)) {
      // Failed to get the bytes for this string item
      nReturn = QCBOR_ERR_HIT_END;
      goto Done;
   }

   if(pAlloc) {
      // We are asked to use string allocator to make a copy
      UsefulBuf NewMem = pAlloc->fAllocate(pAlloc->pAllocaterContext, NULL, uStrLen);
      if(UsefulBuf_IsNULL(NewMem)) {
         nReturn = QCBOR_ERR_STRING_ALLOCATE;
         goto Done;
      }
      pDecodedItem->val.string = UsefulBuf_Copy(NewMem, Bytes);
   } else {
      // Normal case with no string allocator
      pDecodedItem->val.string = Bytes;
   }
   pDecodedItem->uDataType  = (nMajorType == CBOR_MAJOR_TYPE_BYTE_STRING) ? QCBOR_TYPE_BYTE_STRING : QCBOR_TYPE_TEXT_STRING;

Done:
   return nReturn;
}


/*
 Mostly just assign the right data type for the date string.
 */
inline static QCBORError DecodeDateString(QCBORItem *pDecodedItem)
{
   // Stack Use: UsefulBuf 1 16
   if(pDecodedItem->uDataType != QCBOR_TYPE_TEXT_STRING) {
      return QCBOR_ERR_BAD_OPT_TAG;
   }

   const UsefulBufC Temp        = pDecodedItem->val.string;
   pDecodedItem->val.dateString = Temp;
   pDecodedItem->uDataType      = QCBOR_TYPE_DATE_STRING;
   return QCBOR_SUCCESS;
}


/*
 Mostly just assign the right data type for the bignum.
 */
inline static QCBORError DecodeBigNum(QCBORItem *pDecodedItem)
{
   // Stack Use: UsefulBuf 1  -- 16
   if(pDecodedItem->uDataType != QCBOR_TYPE_BYTE_STRING) {
      return QCBOR_ERR_BAD_OPT_TAG;
   }
   const UsefulBufC Temp    = pDecodedItem->val.string;
   pDecodedItem->val.bigNum = Temp;
   pDecodedItem->uDataType  = pDecodedItem->uTagBits & QCBOR_TAGFLAG_POS_BIGNUM ? QCBOR_TYPE_POSBIGNUM : QCBOR_TYPE_NEGBIGNUM;
   return QCBOR_SUCCESS;
}


/*
 The epoch formatted date. Turns lots of different forms of encoding date into uniform one
 */
static int DecodeDateEpoch(QCBORItem *pDecodedItem)
{
   // Stack usage: 1
   QCBORError nReturn = QCBOR_SUCCESS;

   pDecodedItem->val.epochDate.fSecondsFraction = 0;

   switch (pDecodedItem->uDataType) {

      case QCBOR_TYPE_INT64:
         pDecodedItem->val.epochDate.nSeconds = pDecodedItem->val.int64;
         break;

      case QCBOR_TYPE_UINT64:
         if(pDecodedItem->val.uint64 > INT64_MAX) {
            nReturn = QCBOR_ERR_DATE_OVERFLOW;
            goto Done;
         }
         pDecodedItem->val.epochDate.nSeconds = pDecodedItem->val.uint64;
         break;

      case QCBOR_TYPE_DOUBLE:
         {
            const double d = pDecodedItem->val.dfnum;
            if(d > INT64_MAX) {
               nReturn = QCBOR_ERR_DATE_OVERFLOW;
               goto Done;
            }
            pDecodedItem->val.epochDate.nSeconds = d; // Float to integer conversion happening here.
            pDecodedItem->val.epochDate.fSecondsFraction = d - pDecodedItem->val.epochDate.nSeconds;
         }
         break;

      default:
         nReturn = QCBOR_ERR_BAD_OPT_TAG;
         goto Done;
   }
   pDecodedItem->uDataType = QCBOR_TYPE_DATE_EPOCH;

Done:
   return nReturn;
}




// Make sure the constants align as this is assumed by the GetAnItem() implementation
#if QCBOR_TYPE_ARRAY != CBOR_MAJOR_TYPE_ARRAY
#error QCBOR_TYPE_ARRAY value not lined up with major type
#endif
#if QCBOR_TYPE_MAP != CBOR_MAJOR_TYPE_MAP
#error QCBOR_TYPE_MAP value not lined up with major type
#endif

/*
 This gets a single data item and decodes it including preceding optional tagging. This does not
 deal with arrays and maps and nesting except to decode the data item introducing them. Arrays and
 maps are handled at the next level up in GetNext().

 Errors detected here include: an array that is too long to decode, hit end of buffer unexpectedly,
    a few forms of invalid encoded CBOR
 */
static QCBORError GetNext_Item(UsefulInputBuf *pUInBuf, QCBORItem *pDecodedItem, const QCBORStringAllocator *pAlloc)
{
   // Stack usage: int/ptr 3 -- 24
   QCBORError nReturn;

   // Get the major type and the number. Number could be length of more bytes or the value depending on the major type
   // nAdditionalInfo is an encoding of the length of the uNumber and is needed to decode floats and doubles
   int      uMajorType;
   uint64_t uNumber;
   uint8_t  uAdditionalInfo;

   nReturn = DecodeTypeAndNumber(pUInBuf, &uMajorType, &uNumber, &uAdditionalInfo);

   // Error out here if we got into trouble on the type and number.
   // The code after this will not work if the type and number is not good.
   if(nReturn)
      goto Done;

   memset(pDecodedItem, 0, sizeof(QCBORItem));

   // At this point the major type and the value are valid. We've got the type and the number that
   // starts every CBOR data item.
   switch (uMajorType) {
      case CBOR_MAJOR_TYPE_POSITIVE_INT: // Major type 0
      case CBOR_MAJOR_TYPE_NEGATIVE_INT: // Major type 1
         nReturn = DecodeInteger(uMajorType, uNumber, pDecodedItem);
         break;

      case CBOR_MAJOR_TYPE_BYTE_STRING: // Major type 2
      case CBOR_MAJOR_TYPE_TEXT_STRING: // Major type 3
         if(uAdditionalInfo == LEN_IS_INDEFINITE) {
            pDecodedItem->uDataType  = (uMajorType == CBOR_MAJOR_TYPE_BYTE_STRING) ? QCBOR_TYPE_BYTE_STRING : QCBOR_TYPE_TEXT_STRING;
            pDecodedItem->val.string = (UsefulBufC){NULL, SIZE_MAX};
         } else {
            nReturn = DecodeBytes(pAlloc, uMajorType, uNumber, pUInBuf, pDecodedItem);
         }
         break;

      case CBOR_MAJOR_TYPE_ARRAY: // Major type 4
      case CBOR_MAJOR_TYPE_MAP:   // Major type 5
         // Record the number of items in the array or map
         if(uNumber > QCBOR_MAX_ITEMS_IN_ARRAY) {
            nReturn = QCBOR_ERR_ARRAY_TOO_LONG;
            goto Done;
         }
         if(uAdditionalInfo == LEN_IS_INDEFINITE) {
            pDecodedItem->val.uCount = UINT16_MAX; // Indicate indefinite length
         } else {
            pDecodedItem->val.uCount = (uint16_t)uNumber; // type conversion OK because of check above
         }
         pDecodedItem->uDataType  = uMajorType; // C preproc #if above makes sure constants align
         break;

      case CBOR_MAJOR_TYPE_OPTIONAL: // Major type 6, optional prepended tags
         pDecodedItem->val.uTagV = uNumber;
         pDecodedItem->uDataType = QCBOR_TYPE_OPTTAG;
         break;

      case CBOR_MAJOR_TYPE_SIMPLE: // Major type 7, float, double, true, false, null...
         nReturn = DecodeSimple(uAdditionalInfo, uNumber, pDecodedItem);
         break;

      default: // Should never happen because DecodeTypeAndNumber() should never return > 7
         nReturn = QCBOR_ERR_UNSUPPORTED;
         break;
   }

Done:
   return nReturn;
}



/*
 This layer deals with indefinite length strings. It pulls all the
 individual chunk items together into one QCBORItem using the
 string allocator.

 Code Reviewers: THIS FUNCTION DOES A LITTLE POINTER MATH
 */
static inline QCBORError GetNext_FullItem(QCBORDecodeContext *me, QCBORItem *pDecodedItem)
{
   // Stack usage; int/ptr 2 UsefulBuf 2 QCBORItem  -- 96
   QCBORError nReturn;
   QCBORStringAllocator *pAlloc = (QCBORStringAllocator *)me->pStringAllocator;
   UsefulBufC FullString = NULLUsefulBufC;

   nReturn = GetNext_Item(&(me->InBuf), pDecodedItem, me->bStringAllocateAll ? pAlloc: NULL);
   if(nReturn) {
      goto Done;
   }

   // To reduce code size by removing support for indefinite length strings, the
   // code in this function from here down can be eliminated. Run tests, except
   // indefinite length string tests, to be sure all is OK if this is removed.

   // Only do indefinite length processing on strings
   if(pDecodedItem->uDataType != QCBOR_TYPE_BYTE_STRING && pDecodedItem->uDataType != QCBOR_TYPE_TEXT_STRING) {
      goto Done; // no need to do any work here on non-string types
   }

   // Is this a string with an indefinite length?
   if(pDecodedItem->val.string.len != SIZE_MAX) {
      goto Done; // length is not indefinite, so no work to do here
   }

   // Can't do indefinite length strings without a string allocator
   if(pAlloc == NULL) {
      nReturn = QCBOR_ERR_NO_STRING_ALLOCATOR;
      goto Done;
   }

   // There is an indefinite length string to work on...
   // Track which type of string it is
   const uint8_t uStringType = pDecodedItem->uDataType;

   // Loop getting chunk of indefinite string
   for(;;) {
      // Get item for next chunk
      QCBORItem StringChunkItem;
      // NULL passed to never string alloc chunk of indefinite length strings
      nReturn = GetNext_Item(&(me->InBuf), &StringChunkItem, NULL);
      if(nReturn) {
         break;  // Error getting the next chunk
      }

      // See if it is a marker at end of indefinite length string
      if(StringChunkItem.uDataType == QCBOR_TYPE_BREAK) {
         // String is complete
         pDecodedItem->val.string = FullString;
         pDecodedItem->uDataAlloc = 1;
         break;
      }

      // Match data type of chunk to type at beginning.
      // Also catches error of other non-string types that don't belong.
      if(StringChunkItem.uDataType != uStringType) {
         nReturn = QCBOR_ERR_INDEFINITE_STRING_CHUNK;
         break;
      }

      // Alloc new buffer or expand previously allocated buffer so it can fit
      UsefulBuf NewMem = (*pAlloc->fAllocate)(pAlloc->pAllocaterContext,
                                              UNCONST_POINTER(FullString.ptr),
                                              FullString.len + StringChunkItem.val.string.len);
      if(UsefulBuf_IsNULL(NewMem)) {
         // Allocation of memory for the string failed
         nReturn = QCBOR_ERR_STRING_ALLOCATE;
         break;
      }

      // Copy new string chunk at the end of string so far.
      FullString = UsefulBuf_CopyOffset(NewMem, FullString.len, StringChunkItem.val.string);
   }

Done:
   if(pAlloc && nReturn && !UsefulBuf_IsNULLC(FullString)) {
      // Getting item failed, clean up the allocated memory
      (pAlloc->fFree)(pAlloc->pAllocaterContext, UNCONST_POINTER(FullString.ptr));
   }

   return nReturn;
}


/*
 Returns an error if there was something wrong with the optional item or it couldn't
 be handled.
 */
static QCBORError GetNext_TaggedItem(QCBORDecodeContext *me, QCBORItem *pDecodedItem, QCBORTagListOut *pTags)
{
   // Stack usage: int/ptr: 3 -- 24
   QCBORError nReturn;
   uint64_t  uTagBits = 0;
   if(pTags) {
      pTags->uNumUsed = 0;
   }

   for(;;) {
      nReturn = GetNext_FullItem(me, pDecodedItem);
      if(nReturn) {
         goto Done; // Error out of the loop
      }

      if(pDecodedItem->uDataType != QCBOR_TYPE_OPTTAG) {
         // Successful exit from loop; maybe got some tags, maybe not
         pDecodedItem->uTagBits = uTagBits;
         break;
      }

      uint8_t uTagBitIndex;
      // Tag was mapped, tag was not mapped, error with tag list
      switch(TagMapper_Lookup(me->pCallerConfiguredTagList, pDecodedItem->val.uTagV, &uTagBitIndex)) {

         case QCBOR_SUCCESS:
            // Successfully mapped the tag
            uTagBits |= 0x01ULL << uTagBitIndex;
            break;

         case QCBOR_ERR_BAD_OPT_TAG:
            // Tag is not recognized. Do nothing
            break;

         default:
            // Error Condition
            goto Done;
      }

      if(pTags) {
         // Caller wants all tags recorded in the provided buffer
         if(pTags->uNumUsed >= pTags->uNumAllocated) {
            nReturn = QCBOR_ERR_TOO_MANY_TAGS;
            goto Done;
         }
         pTags->puTags[pTags->uNumUsed] = pDecodedItem->val.uTagV;
         pTags->uNumUsed++;
      }
   }

   switch(pDecodedItem->uTagBits & TAG_MAPPER_FIRST_FOUR) {
      case 0:
         // No tags at all or none we know about. Nothing to do.
         // This is part of the pass-through path of this function
         // that will mostly be taken when decoding any item.
         break;

      case QCBOR_TAGFLAG_DATE_STRING:
         nReturn = DecodeDateString(pDecodedItem);
         break;

      case QCBOR_TAGFLAG_DATE_EPOCH:
         nReturn = DecodeDateEpoch(pDecodedItem);
         break;

      case QCBOR_TAGFLAG_POS_BIGNUM:
      case QCBOR_TAGFLAG_NEG_BIGNUM:
         nReturn = DecodeBigNum(pDecodedItem);
         break;

      default:
         // Encountering some mixed up CBOR like something that
         // is tagged as both a string and integer date.
         nReturn = QCBOR_ERR_BAD_OPT_TAG;
   }

Done:
   return nReturn;
}


/*
 This layer takes care of map entries. It combines the label and data items into one QCBORItem.
 */
static inline QCBORError GetNext_MapEntry(QCBORDecodeContext *me, QCBORItem *pDecodedItem, QCBORTagListOut *pTags)
{
   // Stack use: int/ptr 1, QCBORItem  -- 56
   QCBORError nReturn = GetNext_TaggedItem(me, pDecodedItem, pTags);
   if(nReturn)
      goto Done;

   if(pDecodedItem->uDataType == QCBOR_TYPE_BREAK) {
      // Break can't be a map entry
      goto Done;
   }

   if(me->uDecodeMode != QCBOR_DECODE_MODE_MAP_AS_ARRAY) {
      // In a map and caller wants maps decoded, not treated as arrays

      if(DecodeNesting_TypeIsMap(&(me->nesting))) {
         // If in a map and the right decoding mode, get the label

         // Get the next item which will be the real data; Item will be the label
         QCBORItem LabelItem = *pDecodedItem;
         nReturn = GetNext_TaggedItem(me, pDecodedItem, pTags);
         if(nReturn)
            goto Done;

         pDecodedItem->uLabelAlloc = LabelItem.uDataAlloc;

         if(LabelItem.uDataType == QCBOR_TYPE_TEXT_STRING) {
            // strings are always good labels
            pDecodedItem->label.string = LabelItem.val.string;
            pDecodedItem->uLabelType = QCBOR_TYPE_TEXT_STRING;
         } else if (QCBOR_DECODE_MODE_MAP_STRINGS_ONLY == me->uDecodeMode) {
            // It's not a string and we only want strings, probably for easy translation to JSON
            nReturn = QCBOR_ERR_MAP_LABEL_TYPE;
            goto Done;
         } else if(LabelItem.uDataType == QCBOR_TYPE_INT64) {
            pDecodedItem->label.int64 = LabelItem.val.int64;
            pDecodedItem->uLabelType = QCBOR_TYPE_INT64;
         } else if(LabelItem.uDataType == QCBOR_TYPE_UINT64) {
            pDecodedItem->label.uint64 = LabelItem.val.uint64;
            pDecodedItem->uLabelType = QCBOR_TYPE_UINT64;
         } else if(LabelItem.uDataType == QCBOR_TYPE_BYTE_STRING) {
            pDecodedItem->label.string = LabelItem.val.string;
            pDecodedItem->uLabelAlloc = LabelItem.uDataAlloc;
            pDecodedItem->uLabelType = QCBOR_TYPE_BYTE_STRING;
         } else {
            // label is not an int or a string. It is an arrray
            // or a float or such and this implementation doesn't handle that.
            // Also, tags on labels are ignored.
            nReturn = QCBOR_ERR_MAP_LABEL_TYPE;
            goto Done;
         }
      }
   } else {
      if(pDecodedItem->uDataType == QCBOR_TYPE_MAP) {
         // Decoding a map as an array
         pDecodedItem->uDataType = QCBOR_TYPE_MAP_AS_ARRAY;
         pDecodedItem->val.uCount *= 2;
      }
   }

Done:
   return nReturn;
}


/*
 Public function, see header qcbor.h file
 */
QCBORError QCBORDecode_GetNextWithTags(QCBORDecodeContext *me, QCBORItem *pDecodedItem, QCBORTagListOut *pTags)
{
   // Stack ptr/int: 2, QCBORItem : 64

   // The public entry point for fetching and parsing the next QCBORItem.
   // All the CBOR parsing work is here and in subordinate calls.
   QCBORError nReturn;

   nReturn = GetNext_MapEntry(me, pDecodedItem, pTags);
   if(nReturn) {
      goto Done;
   }

   // Break ending arrays/maps are always processed at the end of this function.
   // They should never show up here.
   if(pDecodedItem->uDataType == QCBOR_TYPE_BREAK) {
      nReturn = QCBOR_ERR_BAD_BREAK;
      goto Done;
   }

   // Record the nesting level for this data item before processing any of
   // decrementing and descending.
   pDecodedItem->uNestingLevel = DecodeNesting_GetLevel(&(me->nesting));

   // Process the item just received for descent or decrement, and
   // ascent if decrements are enough to close out a definite length array/map
   if(IsMapOrArray(pDecodedItem->uDataType)) {
      // If the new item is array or map, the nesting level descends
      nReturn = DecodeNesting_Descend(&(me->nesting), pDecodedItem);
      // Maps and arrays do count in as items in the map/array that encloses
      // them so a decrement needs to be done for them too, but that is done
      // only when all the items in them have been processed, not when they
      // are opened.
   } else {
      // Decrement the count of items in the enclosing map/array
      // If the count in the enclosing map/array goes to zero, that
      // triggers a decrement in the map/array above that and
      // an ascend in nesting level.
      DecodeNesting_DecrementCount(&(me->nesting));
   }
   if(nReturn) {
      goto Done;
   }

   // For indefinite length maps/arrays, looking at any and
   // all breaks that might terminate them. The equivalent
   // for definite length maps/arrays happens in
   // DecodeNesting_DecrementCount().
   if(DecodeNesting_IsNested(&(me->nesting)) && DecodeNesting_IsIndefiniteLength(&(me->nesting))) {
      while(UsefulInputBuf_BytesUnconsumed(&(me->InBuf))) {
         // Peek forward one item to see if it is a break.
         QCBORItem Peek;
         size_t uPeek = UsefulInputBuf_Tell(&(me->InBuf));
         nReturn = GetNext_Item(&(me->InBuf), &Peek, NULL);
         if(nReturn) {
            goto Done;
         }
         if(Peek.uDataType != QCBOR_TYPE_BREAK) {
            // It is not a break, rewind so it can be processed normally.
            UsefulInputBuf_Seek(&(me->InBuf), uPeek);
            break;
         }
         // It is a break. Ascend one nesting level.
         // The break is consumed.
         nReturn = DecodeNesting_BreakAscend(&(me->nesting));
         if(nReturn) {
            // break occured outside of an indefinite length array/map
            goto Done;
         }
      }
   }

   // Tell the caller what level is next. This tells them what maps/arrays
   // were closed out and makes it possible for them to reconstruct
   // the tree with just the information returned by GetNext
   pDecodedItem->uNextNestLevel = DecodeNesting_GetLevel(&(me->nesting));

Done:
   return nReturn;
}


QCBORError QCBORDecode_GetNext(QCBORDecodeContext *me, QCBORItem *pDecodedItem)
{
   return QCBORDecode_GetNextWithTags(me, pDecodedItem, NULL);
}


/*
 Decoding items is done in 5 layered functions, one calling the
 next one down. If a layer has no work to do for a particular item
 it returns quickly.

 - QCBORDecode_GetNext -- The top layer manages the beginnings and
 ends of maps and arrays. It tracks descending into and ascending
 out of maps/arrays. It processes all breaks that terminate
 maps and arrays.

 - GetNext_MapEntry -- This handles the combining of two
 items, the label and the data, that make up a map entry.
 It only does work on maps. It combines the label and data
 items into one labeled item.

 - GetNext_TaggedItem -- This handles the type 6 tagged items.
 It accumulates all the tags and combines them with the following
 non-tagged item. If the tagged item is something that is understood
 like a date, the decoding of that item is invoked.

 - GetNext_FullItem -- This assembles the sub items that make up
 an indefinte length string into one string item. It uses the
 string allocater to create contiguous space for the item. It
 processes all breaks that are part of indefinite length strings.

 - GetNext_Item -- This gets and decodes the most atomic
 item in CBOR, the thing with an initial byte containing
 the major type.

 Roughly this takes 300 bytes of stack for vars. Need to
 evaluate this more carefully and correctly.

 */


/*
 Public function, see header qcbor.h file
 */
int QCBORDecode_IsTagged(QCBORDecodeContext *me, const QCBORItem *pItem, uint64_t uTag)
{
   const QCBORTagListIn *pCallerConfiguredTagMap = me->pCallerConfiguredTagList;

   uint8_t uTagBitIndex;
   // Do not care about errors in pCallerConfiguredTagMap here. They are
   // caught during GetNext() before this is called.
   if(TagMapper_Lookup(pCallerConfiguredTagMap, uTag, &uTagBitIndex)) {
      return 0;
   }

   const uint64_t uTagBit = 0x01ULL << uTagBitIndex;
   return (uTagBit & pItem->uTagBits) != 0;
}


/*
 Public function, see header qcbor.h file
 */
QCBORError QCBORDecode_Finish(QCBORDecodeContext *me)
{
   int nReturn = QCBOR_SUCCESS;

   // Error out if all the maps/arrays are not closed out
   if(DecodeNesting_IsNested(&(me->nesting))) {
      nReturn = QCBOR_ERR_ARRAY_OR_MAP_STILL_OPEN;
      goto Done;
   }

   // Error out if not all the bytes are consumed
   if(UsefulInputBuf_BytesUnconsumed(&(me->InBuf))) {
      nReturn = QCBOR_ERR_EXTRA_BYTES;
   }

Done:
   // Call the destructor for the string allocator if there is one.
   // Always called, even if there are errors; always have to clean up
   if(me->pStringAllocator) {
      QCBORStringAllocator *pAllocator = (QCBORStringAllocator *)me->pStringAllocator;
      if(pAllocator->fDestructor) {
         (pAllocator->fDestructor)(pAllocator->pAllocaterContext);
      }
   }

   return nReturn;
}



/*

Decoder errors handled in this file

 - Hit end of input before it was expected while decoding type and number QCBOR_ERR_HIT_END

 - negative integer that is too large for C QCBOR_ERR_INT_OVERFLOW

 - Hit end of input while decoding a text or byte string QCBOR_ERR_HIT_END

 - Encountered conflicting tags -- e.g., an item is tagged both a date string and an epoch date QCBOR_ERR_UNSUPPORTED

 - Encontered an array or mapp that has too many items QCBOR_ERR_ARRAY_TOO_LONG

 - Encountered array/map nesting that is too deep QCBOR_ERR_ARRAY_NESTING_TOO_DEEP

 - An epoch date > INT64_MAX or < INT64_MIN was encountered QCBOR_ERR_DATE_OVERFLOW

 - The type of a map label is not a string or int QCBOR_ERR_MAP_LABEL_TYPE

 - Hit end with arrays or maps still open -- QCBOR_ERR_EXTRA_BYTES

 */




/*
 This is a very primitive memory allocator. It does not track individual
 allocations, only a high-water mark. A free or reallotcation must be of
 the last chunk allocated.

 All of this following code will get dead-stripped if QCBORDecode_SetMemPool()
 is not called.
 */

typedef struct {
   QCBORStringAllocator  StringAllocator;
   uint8_t              *pStart;  // First byte that can be allocated
   uint8_t              *pEnd;    // One past the last byte that can be allocated
   uint8_t              *pFree;   // Where the next free chunk is
} MemPool;


/*
 Internal function for an allocation

 Code Reviewers: THIS FUNCTION DOES POINTER MATH
 */
static UsefulBuf MemPool_Alloc(void *ctx, void *pMem, size_t uNewSize)
{
   MemPool *me      = (MemPool *)ctx;
   void    *pReturn = NULL;

   if(pMem) {
      // Realloc case
      // This check will work even if uNewSize is a super-large value like UINT64_MAX
      if((uNewSize <= (size_t)(me->pEnd - (uint8_t *)pMem)) && ((uint8_t *)pMem >= me->pStart)) {
         me->pFree = (uint8_t *)pMem + uNewSize;
         pReturn   = pMem;
      }
   } else {
      // New chunk case
      // This check will work even if uNewSize is a super large value like UINT64_MAX
      if(uNewSize <= (size_t)(me->pEnd - me->pFree)) {
         pReturn    = me->pFree;
         me->pFree += uNewSize;
      }
   }

   return (UsefulBuf){pReturn, uNewSize};
}

/*
 Internal function to free memory
 */
static void MemPool_Free(void *ctx, void *pOldMem)
{
   MemPool *me = (MemPool *)ctx;
   me->pFree   = pOldMem;
}

/*
 Public function, see header qcbor.h file
 */
QCBORError QCBORDecode_SetMemPool(QCBORDecodeContext *me, UsefulBuf Pool, bool bAllStrings)
{
   // The idea behind QCBOR_MIN_MEM_POOL_SIZE is
   // that the caller knows exactly what size to
   // allocate and that the tests can run conclusively
   // no matter what size MemPool is
   // even though it wastes some memory. MemPool
   // will vary depending on pointer size of the
   // the machine. QCBOR_MIN_MEM_POOL_SIZE is
   // set for pointers up to 64-bits. This
   // wastes about 50 bytes on a 32-bit machine.
   // This check makes sure things don't go
   // horribly wrong. It should optimize out
   // when there is no problem as the sizes are
   // known at compile time.
   if(sizeof(MemPool) > QCBOR_DECODE_MIN_MEM_POOL_SIZE) {
      return QCBOR_ERR_MEM_POOL_INTERNAL;
   }

   // The first bytes of the Pool passed in are used
   // as the context (vtable of sorts) for the memory pool
   // allocator.
   if(Pool.len < QCBOR_DECODE_MIN_MEM_POOL_SIZE) {
      return QCBOR_ERR_BUFFER_TOO_SMALL;
   }
   MemPool *pMP = (MemPool *)Pool.ptr;

   // Fill in the "vtable"
   pMP->StringAllocator.fAllocate   = MemPool_Alloc;
   pMP->StringAllocator.fFree       = MemPool_Free;
   pMP->StringAllocator.fDestructor = NULL;

   // Set up the pointers to the memory to be allocated
   pMP->pStart = (uint8_t *)Pool.ptr + QCBOR_DECODE_MIN_MEM_POOL_SIZE;
   pMP->pFree  = pMP->pStart;
   pMP->pEnd   = (uint8_t *)Pool.ptr + Pool.len;

   // More book keeping of context
   pMP->StringAllocator.pAllocaterContext = pMP;
   me->pStringAllocator   = pMP;

   // The flag indicating when to use the allocator
   me->bStringAllocateAll = bAllStrings;

   return QCBOR_SUCCESS;
}


/*
 Extra little hook to make MemPool testing work right
 without adding any code size or overhead to non-test
 uses. This will get dead-stripped for non-test use.

 This is not a public function.
 */
size_t MemPoolTestHook_GetPoolSize(void *ctx)
{
   MemPool *me = (MemPool *)ctx;

   return me->pEnd - me->pStart;
}

