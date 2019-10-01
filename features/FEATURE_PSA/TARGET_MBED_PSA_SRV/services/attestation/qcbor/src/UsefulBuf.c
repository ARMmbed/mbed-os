/*==============================================================================
 Copyright (c) 2016-2018, The Linux Foundation.
 Copyright (c) 2018-2019, Laurence Lundblade.
 All rights reserved.
 SPDX-License-Identifier: BSD-3-Clause

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
 FILE:  UsefulBuf.c

 DESCRIPTION:  General purpose input and output buffers

 EDIT HISTORY FOR FILE:

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

 when               who             what, where, why
 --------           ----            ---------------------------------------------------
 09/07/17           llundbla        Fix critical bug in UsefulBuf_Find() -- a read off
                                    the end of memory when the bytes to find is longer
                                    than the bytes to search.
 06/27/17           llundbla        Fix UsefulBuf_Compare() bug. Only affected comparison
                                    for < or > for unequal length buffers.  Added
                                    UsefulBuf_Set() function.
 05/30/17           llundbla        Functions for NULL UsefulBufs and const / unconst
 11/13/16           llundbla        Initial Version.

 =====================================================================================*/

#include "UsefulBuf.h"

#define USEFUL_OUT_BUF_MAGIC  (0x0B0F) // used to catch use of uninitialized or corrupted UOBs


/*
 Public function -- see UsefulBuf.h
 */
UsefulBufC UsefulBuf_CopyOffset(UsefulBuf Dest, size_t uOffset, const UsefulBufC Src)
{
   // Do this with subtraction so it doesn't give erroneous result if uOffset + Src.len overflows
   if(uOffset > Dest.len || Src.len > Dest.len - uOffset) { // uOffset + Src.len > Dest.len
      return NULLUsefulBufC;
   }

   memcpy((uint8_t *)Dest.ptr + uOffset, Src.ptr, Src.len);

   return (UsefulBufC){Dest.ptr, Src.len + uOffset};
}


/*
   Public function -- see UsefulBuf.h
 */
int UsefulBuf_Compare(const UsefulBufC UB1, const UsefulBufC UB2)
{
   // use the comparisons rather than subtracting lengths to
   // return an int instead of a size_t
   if(UB1.len < UB2.len) {
      return -1;
   } else if (UB1.len > UB2.len) {
      return 1;
   } // else UB1.len == UB2.len

   return memcmp(UB1.ptr, UB2.ptr, UB1.len);
}



/*
 Public function -- see UsefulBuf.h
 */
size_t UsefulBuf_FindBytes(UsefulBufC BytesToSearch, UsefulBufC BytesToFind)
{
   if(BytesToSearch.len < BytesToFind.len) {
      return SIZE_MAX;
   }

   for(size_t uPos = 0; uPos <= BytesToSearch.len - BytesToFind.len; uPos++) {
      if(!UsefulBuf_Compare((UsefulBufC){((uint8_t *)BytesToSearch.ptr) + uPos, BytesToFind.len}, BytesToFind)) {
         return uPos;
      }
   }

   return SIZE_MAX;
}


/*
 Public function -- see UsefulBuf.h

 Code Reviewers: THIS FUNCTION DOES POINTER MATH
 */
void UsefulOutBuf_Init(UsefulOutBuf *me, UsefulBuf Storage)
{
    me->magic  = USEFUL_OUT_BUF_MAGIC;
    UsefulOutBuf_Reset(me);
    me->UB     = Storage;

#if 0
   // This check is off by default.

   // The following check fails on ThreadX

    // Sanity check on the pointer and size to be sure we are not
    // passed a buffer that goes off the end of the address space.
    // Given this test, we know that all unsigned lengths less than
    // me->size are valid and won't wrap in any pointer additions
    // based off of pStorage in the rest of this code.
    const uintptr_t ptrM = UINTPTR_MAX - Storage.len;
    if(Storage.ptr && (uintptr_t)Storage.ptr > ptrM) // Check #0
        me->err = 1;
#endif
}



/*
 Public function -- see UsefulBuf.h

 The core of UsefulOutBuf -- put some bytes in the buffer without writing off the end of it.

 Code Reviewers: THIS FUNCTION DOES POINTER MATH

 This function inserts the source buffer, NewData, into the destination buffer, me->UB.ptr.

 Destination is represented as:
   me->UB.ptr -- start of the buffer
   me->UB.len -- size of the buffer UB.ptr
   me->data_len -- length of value data in UB

 Source is data:
   NewData.ptr -- start of source buffer
   NewData.len -- length of source buffer

 Insertion point:
   uInsertionPos.

 Steps:

 0. Corruption checks on UsefulOutBuf

 1. Figure out if the new data will fit or not

 2. Is insertion position in the range of valid data?

 3. If insertion point is not at the end, slide data to the right of the insertion point to the right

 4. Put the new data in at the insertion position.

 */
void UsefulOutBuf_InsertUsefulBuf(UsefulOutBuf *me, UsefulBufC NewData, size_t uInsertionPos)
{
   if(me->err) {
      // Already in error state.
      return;
   }

   /* 0. Sanity check the UsefulOutBuf structure */
   // A "counter measure". If magic number is not the right number it
   // probably means me was not initialized or it was corrupted. Attackers
   // can defeat this, but it is a hurdle and does good with very
   // little code.
   if(me->magic != USEFUL_OUT_BUF_MAGIC) {
      me->err = 1;
      return;  // Magic number is wrong due to uninitalization or corrption
   }

   // Make sure valid data is less than buffer size. This would only occur
   // if there was corruption of me, but it is also part of the checks to
   // be sure there is no pointer arithmatic under/overflow.
   if(me->data_len > me->UB.len) {  // Check #1
      me->err = 1;
      return; // Offset of valid data is off the end of the UsefulOutBuf due to uninitialization or corruption
   }

   /* 1. Will it fit? */
   // WillItFit() is the same as: NewData.len <= (me->size - me->data_len)
   // Check #1 makes sure subtraction in RoomLeft will not wrap around
   if(! UsefulOutBuf_WillItFit(me, NewData.len)) { // Check #2
      // The new data will not fit into the the buffer.
      me->err = 1;
      return;
   }

   /* 2. Check the Insertion Position */
   // This, with Check #1, also confirms that uInsertionPos <= me->data_len
   if(uInsertionPos > me->data_len) { // Check #3
      // Off the end of the valid data in the buffer.
      me->err = 1;
      return;
   }

   /* 3. Slide existing data to the right */
   uint8_t *pSourceOfMove       = ((uint8_t *)me->UB.ptr) + uInsertionPos; // PtrMath #1
   size_t   uNumBytesToMove     = me->data_len - uInsertionPos; // PtrMath #2
   uint8_t *pDestinationOfMove  = pSourceOfMove + NewData.len; // PtrMath #3

   if(uNumBytesToMove && me->UB.ptr) {
      // To know memmove won't go off end of destination, see PtrMath #4
      memmove(pDestinationOfMove, pSourceOfMove, uNumBytesToMove);
   }

   /* 4. Put the new data in */
   uint8_t *pInsertionPoint = ((uint8_t *)me->UB.ptr) + uInsertionPos; // PtrMath #5
   if(me->UB.ptr) {
      // To know memmove won't go off end of destination, see PtrMath #6
      memmove(pInsertionPoint, NewData.ptr, NewData.len);
   }
   me->data_len += NewData.len ;
}


/*
 Rationale that describes why the above pointer math is safe

 PtrMath #1 will never wrap around over because
    Check #0 in UsefulOutBuf_Init makes sure me->UB.ptr + me->UB.len doesn't wrap
    Check #1 makes sure me->data_len is less than me->UB.len
    Check #3 makes sure uInsertionPos is less than me->data_len

 PtrMath #2 will never wrap around under because
    Check #3 makes sure uInsertionPos is less than me->data_len

 PtrMath #3 will never wrap around over because   todo
    PtrMath #1 is checked resulting in pSourceOfMove being between me->UB.ptr and a maximum valid ptr
    Check #2 that NewData.len will fit

 PtrMath #4 will never wrap under because
    Calculation for extent or memmove is uRoomInDestination  = me->UB.len - (uInsertionPos + NewData.len)
    Check #3 makes sure uInsertionPos is less than me->data_len
    Check #3 allows Check #2 to be refactored as NewData.Len > (me->size - uInsertionPos)
    This algebraically rearranges to me->size > uInsertionPos + NewData.len

 PtrMath #5 is exactly the same as PtrMath #1

 PtrMath #6 will never wrap under because
    Calculation for extent of memove is uRoomInDestination = me->UB.len - uInsertionPos;
    Check #1 makes sure me->data_len is less than me->size
    Check #3 makes sure uInsertionPos is less than me->data_len
 */


/*
 Public function -- see UsefulBuf.h
 */
UsefulBufC UsefulOutBuf_OutUBuf(UsefulOutBuf *me)
{
   if(me->err) {
      return NULLUsefulBufC;
   }

   if(me->magic != USEFUL_OUT_BUF_MAGIC) {
      me->err = 1;
      return NULLUsefulBufC;
   }

   return (UsefulBufC){me->UB.ptr,me->data_len};
}


/*
 Public function -- see UsefulBuf.h

 Copy out the data accumulated in to the output buffer.
 */
UsefulBufC UsefulOutBuf_CopyOut(UsefulOutBuf *me, UsefulBuf pDest)
{
   const UsefulBufC Tmp = UsefulOutBuf_OutUBuf(me);
   if(UsefulBuf_IsNULLC(Tmp)) {
      return NULLUsefulBufC;
   }
   return UsefulBuf_Copy(pDest, Tmp);
}




/*
 Public function -- see UsefulBuf.h

 The core of UsefulInputBuf -- consume some bytes without going off the end of the buffer.

 Code Reviewers: THIS FUNCTION DOES POINTER MATH
 */
const void * UsefulInputBuf_GetBytes(UsefulInputBuf *me, size_t uAmount)
{
   // Already in error state. Do nothing.
   if(me->err) {
      return NULL;
   }

   if(!UsefulInputBuf_BytesAvailable(me, uAmount)) {
      // The number of bytes asked for at current position are more than available
      me->err = 1;
      return NULL;
   }

   // This is going to succeed
   const void * const result = ((uint8_t *)me->UB.ptr) + me->cursor;
   me->cursor += uAmount; // this will not overflow because of check using UsefulInputBuf_BytesAvailable()
   return result;
}

