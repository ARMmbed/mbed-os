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
 FILE:  qcbor_encode.c

 DESCRIPTION:  This file contains the implementation of QCBOR.

 EDIT HISTORY FOR FILE:

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

 when               who             what, where, why
 --------           ----            ---------------------------------------------------
 12/30/18           llundblade      Small efficient clever encode of type & argument.
 11/29/18           llundblade      Rework to simpler handling of tags and labels.
 11/9/18            llundblade      Error codes are now enums.
 11/1/18            llundblade      Floating support.
 10/31/18           llundblade      Switch to one license that is almost BSD-3.
 09/28/18           llundblade      Added bstr wrapping feature for COSE implementation.
 02/05/18           llundbla        Works on CPUs which require integer alignment.
                                    Requires new version of UsefulBuf.
 07/05/17           llundbla        Add bstr wrapping of maps/arrays for COSE
 03/01/17           llundbla        More data types
 11/13/16           llundbla        Integrate most TZ changes back into github version.
 09/30/16           gkanike         Porting to TZ.
 03/15/16           llundbla        Initial Version.

 =====================================================================================*/

#include "qcbor.h"
#include "ieee754.h"


/*...... This is a ruler that is 80 characters long...........................*/


/*
 CBOR's two nesting types, arrays and maps, are tracked here. There is a
 limit of QCBOR_MAX_ARRAY_NESTING to the number of arrays and maps
 that can be nested in one encoding so the encoding context stays
 small enough to fit on the stack.

 When an array / map is opened, pCurrentNesting points to the element
 in pArrays that records the type, start position and accumluates a
 count of the number of items added. When closed the start position is
 used to go back and fill in the type and number of items in the array
 / map.

 Encoded output be just items like ints and strings that are
 not part of any array / map. That is, the first thing encoded
 does not have to be an array or a map.
 */
inline static void Nesting_Init(QCBORTrackNesting *pNesting)
{
   // assumes pNesting has been zeroed
   pNesting->pCurrentNesting = &pNesting->pArrays[0];
   // Implied CBOR array at the top nesting level. This is never returned,
   // but makes the item count work correctly.
   pNesting->pCurrentNesting->uMajorType = CBOR_MAJOR_TYPE_ARRAY;
}

inline static QCBORError Nesting_Increase(QCBORTrackNesting *pNesting,
                                          uint8_t uMajorType,
                                          uint32_t uPos)
{
   QCBORError nReturn = QCBOR_SUCCESS;

   if(pNesting->pCurrentNesting == &pNesting->pArrays[QCBOR_MAX_ARRAY_NESTING]) {
      // trying to open one too many
      nReturn = QCBOR_ERR_ARRAY_NESTING_TOO_DEEP;
   } else {
      pNesting->pCurrentNesting++;
      pNesting->pCurrentNesting->uCount     = 0;
      pNesting->pCurrentNesting->uStart     = uPos;
      pNesting->pCurrentNesting->uMajorType = uMajorType;
   }
   return nReturn;
}

inline static void Nesting_Decrease(QCBORTrackNesting *pNesting)
{
   pNesting->pCurrentNesting--;
}

inline static QCBORError Nesting_Increment(QCBORTrackNesting *pNesting)
{
   if(1 >= QCBOR_MAX_ITEMS_IN_ARRAY - pNesting->pCurrentNesting->uCount) {
      return QCBOR_ERR_ARRAY_TOO_LONG;
   }

   pNesting->pCurrentNesting->uCount += 1;

   return QCBOR_SUCCESS;
}

inline static uint16_t Nesting_GetCount(QCBORTrackNesting *pNesting)
{
   // The nesting count recorded is always the actual number of individiual
   // data items in the array or map. For arrays CBOR uses the actual item
   // count. For maps, CBOR uses the number of pairs.  This function returns
   // the number needed for the CBOR encoding, so it divides the number of
   // items by two for maps to get the number of pairs.  This implementation
   // takes advantage of the map major type being one larger the array major
   // type, hence uDivisor is either 1 or 2.
   const uint16_t uDivisor = pNesting->pCurrentNesting->uMajorType - CBOR_MAJOR_TYPE_ARRAY+1;

   return pNesting->pCurrentNesting->uCount / uDivisor;
}

inline static uint32_t Nesting_GetStartPos(QCBORTrackNesting *pNesting)
{
   return pNesting->pCurrentNesting->uStart;
}

inline static uint8_t Nesting_GetMajorType(QCBORTrackNesting *pNesting)
{
   return pNesting->pCurrentNesting->uMajorType;
}

inline static int Nesting_IsInNest(QCBORTrackNesting *pNesting)
{
   return pNesting->pCurrentNesting == &pNesting->pArrays[0] ? 0 : 1;
}




/*
 Error tracking plan -- Errors are tracked internally and not returned
 until Finish is called. The CBOR errors are in me->uError.
 UsefulOutBuf also tracks whether the buffer is full or not in its
 context.  Once either of these errors is set they are never
 cleared. Only QCBOREncode_Init() resets them. Or said another way, they must
 never be cleared or we'll tell the caller all is good when it is not.

 Only one error code is reported by QCBOREncode_Finish() even if there are
 multiple errors. The last one set wins. The caller might have to fix
 one error to reveal the next one they have to fix.  This is OK.

 The buffer full error tracked by UsefulBuf is only pulled out of
 UsefulBuf in Finish() so it is the one that usually wins.  UsefulBuf
 will never go off the end of the buffer even if it is called again
 and again when full.

 It is really tempting to not check for overflow on the count in the
 number of items in an array. It would save a lot of code, it is
 extremely unlikely that any one will every put 65,000 items in an
 array, and the only bad thing that would happen is the CBOR would be
 bogus.

 Since this does not parse any input, you could in theory remove all
 error checks in this code if you knew the caller called it
 correctly. Maybe someday CDDL or some such language will be able to
 generate the code to call this and the calling code would always be
 correct. This could also automatically size some of the data
 structures like array/map nesting resulting in some stack memory
 savings.

 Errors returned here fall into two categories:

 Sizes
   QCBOR_ERR_BUFFER_TOO_LARGE -- Encoded output exceeded UINT32_MAX
   QCBOR_ERR_BUFFER_TOO_SMALL -- output buffer too small

   QCBOR_ERR_ARRAY_NESTING_TOO_DEEP -- Array/map nesting > QCBOR_MAX_ARRAY_NESTING1
   QCBOR_ERR_ARRAY_TOO_LONG -- Too many things added to an array/map

 Nesting constructed incorrectly
   QCBOR_ERR_TOO_MANY_CLOSES -- more close calls than opens
   QCBOR_ERR_CLOSE_MISMATCH -- Type of close does not match open
   QCBOR_ERR_ARRAY_OR_MAP_STILL_OPEN -- Finish called without enough closes
 */




/*
 Public function for initialization. See header qcbor.h
 */
void QCBOREncode_Init(QCBOREncodeContext *me, UsefulBuf Storage)
{
   memset(me, 0, sizeof(QCBOREncodeContext));
   UsefulOutBuf_Init(&(me->OutBuf), Storage);
   Nesting_Init(&(me->nesting));
}




/*
 All CBOR data items have a type and an "argument". The argument is
 either the value of the item for integer types, the length of the
 content for string, byte, array and map types, a tag for major type
 6, and has several uses for major type 7.

 This function encodes the type and the argument. There are several
 encodings for the argument depending on how large it is and how it is
 used.

 Every encoding of the type and argument has at least one byte, the
 "initial byte".

 The top three bits of the initial byte are the major type for the
 CBOR data item.  The eight major types defined by the standard are
 defined as CBOR_MAJOR_TYPE_xxxx in qcbor.h.

 The remaining five bits, known as "additional information", and
 possibly more bytes encode the argument. If the argument is less than
 24, then it is encoded entirely in the five bits. This is neat
 because it allows you to encode an entire CBOR data item in 1 byte
 for many values and types (integers 0-23, true, false, and tags).

 If the argument is larger than 24, then it is encoded in 1,2,4 or 8
 additional bytes, with the number of these bytes indicated by the
 values of the 5 bits 24, 25, 25 and 27.

 It is possible to encode a particular argument in many ways with this
 representation.  This implementation always uses the smallest
 possible representation. This conforms with CBOR preferred encoding.

 This function inserts them into the output buffer at the specified
 position. AppendEncodedTypeAndNumber() appends to the end.

 This function takes care of converting to network byte order.

 This function is also used to insert floats and doubles. Before this
 function is called the float or double must be copied into a
 uint64_t. That is how they are passed in. They are then converted to
 network byte order correctly. The uMinLen param makes sure that even

 if all the digits of a half, float or double are 0 it is still
 correctly encoded in 2, 4 or 8 bytes.
 */

static void InsertEncodedTypeAndNumber(QCBOREncodeContext *me,
                                       uint8_t uMajorType,
                                       int nMinLen,
                                       uint64_t uNumber,
                                       size_t uPos)
{
   /*
    This code does endian conversion without hton or knowing the
    endianness of the machine using masks and shifts. This avoids the
    dependency on hton and the mess of figuring out how to find the
    machine's endianness.

    This is a good efficient implementation on little-endian machines.
    A faster and small implementation is possible on big-endian
    machines because CBOR/network byte order is big endian. However
    big endian machines are uncommon.

    On x86, it is about 200 bytes instead of 500 bytes for the more
    formal unoptimized code.

    This also does the CBOR preferred shortest encoding for integers
    and is called to do endian conversion for floats.

    It works backwards from the LSB to the MSB as needed.

    Code Reviewers: THIS FUNCTION DOES POINTER MATH
    */
   // Holds up to 9 bytes of type and argument
   // plus one extra so pointer always points to
   // valid bytes.
   uint8_t bytes[sizeof(uint64_t)+2];
   // Point to the last bytes and work backwards
   uint8_t *pByte = &bytes[sizeof(bytes)-1];
   // This is the 5 bits in the initial byte that is not the major type
   uint8_t uAdditionalInfo;

   if(uNumber < CBOR_TWENTY_FOUR && nMinLen == 0) {
      // Simple case where argument is < 24
      uAdditionalInfo = uNumber;
   } else  {
      /*
       Encode argument in 1,2,4 or 8 bytes. Outer loop
       runs once for 1 byte and 4 times for 8 bytes.
       Inner loop runs 1, 2 or 4 times depending on
       outer loop counter. This works backwards taking
       8 bits off the argument being encoded at a time
       until all bits from uNumber have been encoded
       and the minimum encoding size is reached.
       Minimum encoding size is for floating point
       numbers with zero bytes.
       */
      static const uint8_t aIterate[] = {1,1,2,4};
      uint8_t i;
      for(i = 0; uNumber || nMinLen > 0; i++) {
         const uint8_t uIterations = aIterate[i];
         for(int j = 0; j < uIterations; j++) {
            *--pByte = uNumber & 0xff;
            uNumber = uNumber >> 8;
         }
         nMinLen -= uIterations;
      }
      // Additional info is the encoding of the
      // number of additional bytes to encode
      // argument.
      uAdditionalInfo = LEN_IS_ONE_BYTE-1 + i;
   }
   *--pByte = (uMajorType << 5) + uAdditionalInfo;

   UsefulOutBuf_InsertData(&(me->OutBuf), pByte, &bytes[sizeof(bytes)-1] - pByte, uPos);
}


/*
 Append the type and number info to the end of the buffer.

 See InsertEncodedTypeAndNumber() function above for details
*/
inline static void AppendEncodedTypeAndNumber(QCBOREncodeContext *me,
                                              uint8_t uMajorType,
                                              uint64_t uNumber)
{
   // An append is an insert at the end.
   InsertEncodedTypeAndNumber(me,
                              uMajorType,
                              0,
                              uNumber,
                              UsefulOutBuf_GetEndPosition(&(me->OutBuf)));
}




/*
 Public functions for closing arrays and maps. See header qcbor.h
 */
void QCBOREncode_AddUInt64(QCBOREncodeContext *me, uint64_t uValue)
{
   if(me->uError == QCBOR_SUCCESS) {
      AppendEncodedTypeAndNumber(me, CBOR_MAJOR_TYPE_POSITIVE_INT, uValue);
      me->uError = Nesting_Increment(&(me->nesting));
   }
}


/*
 Public functions for closing arrays and maps. See header qcbor.h
 */
void QCBOREncode_AddInt64(QCBOREncodeContext *me, int64_t nNum)
{
   if(me->uError == QCBOR_SUCCESS) {
      uint8_t      uMajorType;
      uint64_t     uValue;

      if(nNum < 0) {
         // In CBOR -1 encodes as 0x00 with major type negative int.
         uValue = (uint64_t)(-nNum - 1);
         uMajorType = CBOR_MAJOR_TYPE_NEGATIVE_INT;
      } else {
         uValue = (uint64_t)nNum;
         uMajorType = CBOR_MAJOR_TYPE_POSITIVE_INT;
      }

      AppendEncodedTypeAndNumber(me, uMajorType, uValue);
      me->uError = Nesting_Increment(&(me->nesting));
   }
}


/*
 Semi-private function. It is exposed to user of the interface,
 but they will usually call one of the inline wrappers rather than this.

 See header qcbor.h

 Does the work of adding some bytes to the CBOR output. Works for a
 byte and text strings, which are the same in in CBOR though they have
 different major types.  This is also used to insert raw
 pre-encoded CBOR.
 */
void QCBOREncode_AddBuffer(QCBOREncodeContext *me, uint8_t uMajorType, UsefulBufC Bytes)
{
   if(me->uError == QCBOR_SUCCESS) {
      // If it is not Raw CBOR, add the type and the length
      if(uMajorType != CBOR_MAJOR_NONE_TYPE_RAW) {
         AppendEncodedTypeAndNumber(me, uMajorType, Bytes.len);
      }

      // Actually add the bytes
      UsefulOutBuf_AppendUsefulBuf(&(me->OutBuf), Bytes);

      // Update the array counting if there is any nesting at all
      me->uError = Nesting_Increment(&(me->nesting));
   }
}


/*
 Public functions for closing arrays and maps. See header qcbor.h
 */
void QCBOREncode_AddTag(QCBOREncodeContext *me, uint64_t uTag)
{
   AppendEncodedTypeAndNumber(me, CBOR_MAJOR_TYPE_OPTIONAL, uTag);
}


/*
 Semi-private function. It is exposed to user of the interface,
 but they will usually call one of the inline wrappers rather than this.

 See header qcbor.h
 */
void QCBOREncode_AddType7(QCBOREncodeContext *me, size_t uSize, uint64_t uNum)
{
   if(me->uError == QCBOR_SUCCESS) {
      // This function call takes care of endian swapping for the float / double
      InsertEncodedTypeAndNumber(me,
                                 // The major type for floats and doubles
                                 CBOR_MAJOR_TYPE_SIMPLE,
                                 // size makes sure floats with zeros encode correctly
                                 (int)uSize,
                                 // Bytes of the floating point number as a uint
                                 uNum,
                                 // end position because this is append
                                 UsefulOutBuf_GetEndPosition(&(me->OutBuf)));

      me->uError = Nesting_Increment(&(me->nesting));
   }
}


/*
 Public functions for closing arrays and maps. See header qcbor.h
 */
void QCBOREncode_AddDouble(QCBOREncodeContext *me, double dNum)
{
   const IEEE754_union uNum = IEEE754_DoubleToSmallest(dNum);

   QCBOREncode_AddType7(me, uNum.uSize, uNum.uValue);
}


/*
 Semi-public function. It is exposed to user of the interface,
 but they will usually call one of the inline wrappers rather than this.

 See header qcbor.h
*/
void QCBOREncode_OpenMapOrArray(QCBOREncodeContext *me, uint8_t uMajorType)
{
   // Add one item to the nesting level we are in for the new map or array
   me->uError = Nesting_Increment(&(me->nesting));
   if(me->uError == QCBOR_SUCCESS) {
      // The offset where the length of an array or map will get written
      // is stored in a uint32_t, not a size_t to keep stack usage smaller. This
      // checks to be sure there is no wrap around when recording the offset.
      // Note that on 64-bit machines CBOR larger than 4GB can be encoded as long as no
      // array / map offsets occur past the 4GB mark, but the public interface
      // says that the maximum is 4GB to keep the discussion simpler.
      size_t uEndPosition = UsefulOutBuf_GetEndPosition(&(me->OutBuf));

      // QCBOR_MAX_ARRAY_OFFSET is slightly less than UINT32_MAX so this
      // code can run on a 32-bit machine and tests can pass on a 32-bit
      // machine. If it was exactly UINT32_MAX, then this code would
      // not compile or run on a 32-bit machine and an #ifdef or some
      // machine size detection would be needed reducing portability.
      if(uEndPosition >= QCBOR_MAX_ARRAY_OFFSET) {
         me->uError = QCBOR_ERR_BUFFER_TOO_LARGE;

      } else {
         // Increase nesting level because this is a map or array.
         // Cast from size_t to uin32_t is safe because of check above
         me->uError = Nesting_Increase(&(me->nesting), uMajorType, (uint32_t)uEndPosition);
      }
   }
}


/*
 Public functions for closing arrays and maps. See header qcbor.h
 */
void QCBOREncode_CloseMapOrArray(QCBOREncodeContext *me,
                                 uint8_t uMajorType,
                                 UsefulBufC *pWrappedCBOR)
{
   if(me->uError == QCBOR_SUCCESS) {
      if(!Nesting_IsInNest(&(me->nesting))) {
         me->uError = QCBOR_ERR_TOO_MANY_CLOSES;
      } else if(Nesting_GetMajorType(&(me->nesting)) != uMajorType) {
         me->uError = QCBOR_ERR_CLOSE_MISMATCH;
      } else {
         // When the array, map or bstr wrap was started, nothing was done
         // except note the position of the start of it. This code goes back
         // and inserts the actual CBOR array, map or bstr and its length.
         // That means all the data that is in the array, map or wrapped
         // needs to be slid to the right. This is done by UsefulOutBuf's
         // insert function that is called from inside
         // InsertEncodedTypeAndNumber()
         const size_t uInsertPosition         = Nesting_GetStartPos(&(me->nesting));
         const size_t uEndPosition            = UsefulOutBuf_GetEndPosition(&(me->OutBuf));
         // This can't go negative because the UsefulOutBuf always only grows
         // and never shrinks. UsefulOutBut itself also has defenses such that
         // it won't write were it should not even if given hostile input lengths
         const size_t uLenOfEncodedMapOrArray = uEndPosition - uInsertPosition;

         // Length is number of bytes for a bstr and number of items a for map & array
         const size_t uLength = uMajorType == CBOR_MAJOR_TYPE_BYTE_STRING ?
                                    uLenOfEncodedMapOrArray : Nesting_GetCount(&(me->nesting));

         // Actually insert
         InsertEncodedTypeAndNumber(me,
                                    uMajorType,       // major type bstr, array or map
                                    0,                // no minimum length for encoding
                                    uLength,          // either len of bstr or num map / array items
                                    uInsertPosition); // position in out buffer

         // Return pointer and length to the enclosed encoded CBOR. The intended
         // use is for it to be hashed (e.g., SHA-256) in a COSE implementation.
         // This must be used right away, as the pointer and length go invalid
         // on any subsequent calls to this function because of the
         // InsertEncodedTypeAndNumber() call that slides data to the right.
         if(pWrappedCBOR) {
            const UsefulBufC PartialResult = UsefulOutBuf_OutUBuf(&(me->OutBuf));
            const size_t uBstrLen = UsefulOutBuf_GetEndPosition(&(me->OutBuf)) - uEndPosition;
            *pWrappedCBOR = UsefulBuf_Tail(PartialResult, uInsertPosition+uBstrLen);
         }
         Nesting_Decrease(&(me->nesting));
      }
   }
}




/*
 Public functions to finish and get the encoded result. See header qcbor.h
 */
QCBORError QCBOREncode_Finish(QCBOREncodeContext *me, UsefulBufC *pEncodedCBOR)
{
   QCBORError uReturn = me->uError;

   if(uReturn != QCBOR_SUCCESS) {
      goto Done;
   }

   if (Nesting_IsInNest(&(me->nesting))) {
      uReturn = QCBOR_ERR_ARRAY_OR_MAP_STILL_OPEN;
      goto Done;
   }

   if(UsefulOutBuf_GetError(&(me->OutBuf))) {
      // Stuff didn't fit in the buffer.
      // This check catches this condition for all the appends and inserts
      // so checks aren't needed when the appends and inserts are performed.
      // And of course UsefulBuf will never overrun the input buffer given
      // to it. No complex analysis of the error handling in this file is
      // needed to know that is true. Just read the UsefulBuf code.
      uReturn = QCBOR_ERR_BUFFER_TOO_SMALL;
      goto Done;
   }

   *pEncodedCBOR = UsefulOutBuf_OutUBuf(&(me->OutBuf));

Done:
   return uReturn;
}


/*
 Public functions to finish and get the encoded result. See header qcbor.h
 */
QCBORError QCBOREncode_FinishGetSize(QCBOREncodeContext *me, size_t *puEncodedLen)
{
   UsefulBufC Enc;

   QCBORError nReturn = QCBOREncode_Finish(me, &Enc);

   if(nReturn == QCBOR_SUCCESS) {
      *puEncodedLen = Enc.len;
   }

   return nReturn;
}




/*
 Notes on the code

 CBOR Major Type     Public Function
 0                   QCBOREncode_AddUInt64
 0, 1                QCBOREncode_AddUInt64, QCBOREncode_AddInt64
 2, 3                QCBOREncode_AddBuffer, Also QCBOREncode_OpenMapOrArray
 4, 5                QCBOREncode_OpenMapOrArray
 6                   QCBOREncode_AddTag
 7                   QCBOREncode_AddDouble, QCBOREncode_AddType7

 Object code sizes on X86 with LLVM compiler and -Os (Dec 30, 2018)

 _QCBOREncode_Init   69
 _QCBOREncode_AddUInt64   76
 _QCBOREncode_AddInt64   87
 _QCBOREncode_AddBuffer   113
 _QCBOREncode_AddTag 27
 _QCBOREncode_AddType7   87
 _QCBOREncode_AddDouble 36
 _QCBOREncode_OpenMapOrArray   103
 _QCBOREncode_CloseMapOrArray   181
 _InsertEncodedTypeAndNumber   190
 _QCBOREncode_Finish   72
 _QCBOREncode_FinishGetSize  70

 Total is about 1.1KB

 _QCBOREncode_CloseMapOrArray is larger because it has a lot
 of nesting tracking to do and much of Nesting_ inlines
 into it. It probably can't be reduced much.

 If the error returned by Nesting_Increment() can be ignored
 because the limit is so high and the consequence of exceeding
 is proved to be inconsequential, then a lot of if(me->uError)
 instance can be removed, saving some code.

 */

