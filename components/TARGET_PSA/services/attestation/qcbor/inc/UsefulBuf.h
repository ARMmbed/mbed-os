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
 FILE:  UsefulBuf.h

 DESCRIPTION:  General purpose input and output buffers

 EDIT HISTORY FOR FILE:

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

 when               who             what, where, why
 --------           ----            ---------------------------------------------------
 12/17/2018         llundblade      Remove const from UsefulBuf and UsefulBufC .len
 12/13/2018         llundblade      Documentation improvements
 09/18/2018         llundblade      Cleaner distinction between UsefulBuf and UsefulBufC
 02/02/18           llundbla        Full support for integers in and out; fix pointer
                                    alignment bug. Incompatible change: integers in/out
                                    are now in network byte order.
 08/12/17           llundbla        Added UsefulOutBuf_AtStart and UsefulBuf_Find
 06/27/17           llundbla        Fix UsefulBuf_Compare() bug. Only affected comparison
                                    for < or > for unequal length buffers.  Added
                                    UsefulBuf_Set() function.
 05/30/17           llundbla        Functions for NULL UsefulBufs and const / unconst
 11/13/16           llundbla        Initial Version.


 =====================================================================================*/

#ifndef _UsefulBuf_h
#define _UsefulBuf_h


#include <stdint.h> // for uint8_t, uint16_t....
#include <string.h> // for strlen, memcpy, memmove, memset
#include <stddef.h> // for size_t

/**
 @file UsefulBuf.h

 The goal of this code is to make buffer and pointer manipulation
 easier and safer when working with binary data.

 You use the UsefulBuf, UsefulOutBuf and UsefulInputBuf
 structures to represent buffers rather than ad hoc pointers and lengths.

 With these it will often be possible to write code that does little or no
 direct pointer manipulation for copying and formatting data. For example
 the QCBOR encoder was rewritten using these and has no direct pointer
 manipulation.

 While it is true that object code using these functions will be a little
 larger and slower than a white-knuckle clever use of pointers might be, but
 not by that much or enough to have an affect for most use cases. For
 security-oriented code this is highly worthwhile. Clarity, simplicity,
 reviewability and are more important.

 There are some extra sanity and double checks in this code to help catch
 coding errors and simple memory corruption. They are helpful, but not a
 substitute for proper code review, input validation and such.

 This code consists of a lot of inline functions and a few that are not.
 It should not generate very much object code, especially with the
 optimizer turned up to -Os or -O3. The idea is that the inline
 functions are easier to review and understand and the optimizer does
 the work of making the code small.
 */


/*...... This is a ruler that is 80 characters long...........................*/

/**
 UsefulBufC and UsefulBuf are simple data structures to hold a pointer and
 length for a binary data.  In C99 this data structure can be passed on the
 stack making a lot of code cleaner than carrying around a pointer and
 length as two parameters.

 This is also conducive to secure code practice as the lengths are
 always carried with the pointer and the convention for handling a
 pointer and a length is clear.

 While it might be possible to write buffer and pointer code more
 efficiently in some use cases, the thought is that unless there is an
 extreme need for performance (e.g., you are building a gigabit-per-second
 IP router), it is probably better to have cleaner code you can be most
 certain about the security of.

 The non-const UsefulBuf is usually used to refer a buffer to be filled in.
 The length is the size of the buffer.

 The const UsefulBufC is usually used to refer to some data that has been
 filled in. The length is amount of valid data pointed to.

 A common use is to pass a UsefulBuf to a function, the function fills it
 in, the function returns a UsefulBufC. The pointer is the same in both.

 A UsefulBuf is NULL, it has no value, when the ptr in it is NULL.

 There are utility functions for the following:
  - Checking for UsefulBufs that are NULL, empty or both
  - Copying, copying with offset, copying head or tail
  - Comparing and finding substrings
  - Initializating
  - Create initialized const UsefulBufC from compiler literals
  - Create initialized const UsefulBufC from NULL-terminated string
  - Make an empty UsefulBuf on the stack

 See also UsefulOutBuf. It is a richer structure that has both the size of
 the valid data and the size of the buffer.

 UsefulBuf is only 16 or 8 bytes on a 64- or 32-bit machine so it can go
 on the stack and be a function parameter or return value.

 UsefulBuf is kind of like the Useful Pot Pooh gave Eeyore on his birthday.
 Eeyore's balloon fits beautifully, "it goes in and out like anything".

*/
typedef struct useful_buf_c {
    const void *ptr;
    size_t      len;
} UsefulBufC;


/**
 The non-const UsefulBuf typically used for some allocated memory
 that is to be filled in. The len is the amount of memory,
 not the length of the valid data in the buffer.
 */
typedef struct useful_buf {
   void  *ptr;
   size_t len;
} UsefulBuf;


/**
 A "NULL" UsefulBufC is one that has no value in the same way a NULL pointer has no value.
 A UsefulBuf is NULL when the ptr field is NULL. It doesn't matter what len is.
 See UsefulBuf_IsEmpty() for the distinction between NULL and empty.
 */
#define NULLUsefulBufC  ((UsefulBufC) {NULL, 0})

/** A NULL UsefulBuf is one that has no memory associated the say way
 NULL points to nothing. It does not matter what len is.
 */
#define NULLUsefulBuf   ((UsefulBuf) {NULL, 0})


/**
 @brief Check if a UsefulBuf is NULL or not

 @param[in] UB The UsefulBuf to check

 @return 1 if it is NULL, 0 if not.
 */
static inline int UsefulBuf_IsNULL(UsefulBuf UB) {
   return !UB.ptr;
}


/**
 @brief Check if a UsefulBufC is NULL or not

 @param[in] UB The UsefulBufC to check

 @return 1 if it is NULL, 0 if not.
 */
static inline int UsefulBuf_IsNULLC(UsefulBufC UB) {
   return !UB.ptr;
}


/**
 @brief Check if a UsefulBuf is empty or not

 @param[in] UB The UsefulBuf to check

 @return 1 if it is empty, 0 if not.

 An "Empty" UsefulBuf is one that has a value and can be considered to be set,
 but that value is of zero length.  It is empty when len is zero. It
 doesn't matter what the ptr is.

 A lot of uses will not need to clearly distinguish a NULL UsefulBuf
 from an empty one and can have the ptr NULL and the len 0.  However
 if a use of UsefulBuf needs to make a distinction then ptr should
 not be NULL when the UsefulBuf is considered empty, but not NULL.

 */
static inline int UsefulBuf_IsEmpty(UsefulBuf UB) {
   return !UB.len;
}


/**
 @brief Check if a UsefulBufC is empty or not

 @param[in] UB The UsefulBufC to check

 @return 1 if it is empty, 0 if not.
 */
static inline int UsefulBuf_IsEmptyC(UsefulBufC UB) {
   return !UB.len;
}


/**
 @brief Check if a UsefulBuf is NULL or empty

 @param[in] UB The UsefulBuf to check

 @return 1 if it is either NULL or empty, 0 if not.
 */
static inline int UsefulBuf_IsNULLOrEmpty(UsefulBuf UB) {
   return UsefulBuf_IsEmpty(UB) || UsefulBuf_IsNULL(UB);
}


/**
 @brief Check if a UsefulBufC is NULL or empty

 @param[in] UB The UsefulBufC to check

 @return 1 if it is either NULL or empty, 0 if not.
 */
static inline int UsefulBuf_IsNULLOrEmptyC(UsefulBufC UB) {
   return UsefulBuf_IsEmptyC(UB) || UsefulBuf_IsNULLC(UB);
}


/**
 @brief Convert a non const UsefulBuf to a const UsefulBufC

 @param[in] UB The UsefulBuf to convert

 Returns: a UsefulBufC struct
 */

static inline UsefulBufC UsefulBuf_Const(const UsefulBuf UB)
{
   return (UsefulBufC){UB.ptr, UB.len};
}


/**
 @brief Convert a const UsefulBufC to a non-const UsefulBuf

 @param[in] UBC The UsefulBuf to convert

 Returns: a non const UsefulBuf struct
 */
static inline UsefulBuf UsefulBuf_Unconst(const UsefulBufC UBC)
{
   return (UsefulBuf){(void *)UBC.ptr, UBC.len};
}


/**
 Convert a literal string to a UsefulBufC.

 szString must be a literal string that you can take sizeof.
 This is better for literal strings than UsefulBuf_FromSZ()
 because it generates less code. It will not work on
 non-literal strings.

 The terminating \0 (NULL) is NOT included in the length!

 */
#define UsefulBuf_FROM_SZ_LITERAL(szString) \
    ((UsefulBufC) {(szString), sizeof(szString)-1})


/**
 Convert a literal byte array to a UsefulBufC.

 pBytes must be a literal string that you can take sizeof.
 It will not work on  non-literal arrays.

 */
#define UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pBytes) \
    ((UsefulBufC) {(pBytes), sizeof(pBytes)})


/**
 Make an automatic variable with name of type UsefulBuf and point it to a stack
 variable of the give size
 */
#define  UsefulBuf_MAKE_STACK_UB(name, size) \
    uint8_t    __pBuf##name[(size)];\
    UsefulBuf  name = {__pBuf##name , sizeof( __pBuf##name )}


/**
 Make a byte array in to a UsefulBuf
 */
#define UsefulBuf_FROM_BYTE_ARRAY(pBytes) \
    ((UsefulBuf) {(pBytes), sizeof(pBytes)})

/**
 @brief Convert a NULL terminated string to a UsefulBufC.

 @param[in] szString The string to convert

 @return a UsefulBufC struct

 UsefulBufC.ptr points to the string so it's lifetime
 must be maintained.

 The terminating \0 (NULL) is NOT included in the length!

 */
static inline UsefulBufC UsefulBuf_FromSZ(const char *szString){
    return ((UsefulBufC) {szString, strlen(szString)});
}


/**
 @brief Copy one UsefulBuf into another at an offset

 @param[in] Dest Destiation buffer to copy into
 @param[in] uOffset The byte offset in Dest at which to copy to
 @param[in] Src The bytes to copy

 @return Pointer and length of the copy

 This fails and returns NULLUsefulBufC Src.len + uOffset > Dest.len.

 Like memcpy, there is no check for NULL. If NULL is passed
 this will crash.

 There is an assumption that there is valid data in Dest up to
 uOffset as the resulting UsefulBufC returned starts
 at the beginning of Dest and goes to Src.len + uOffset.

 */
UsefulBufC UsefulBuf_CopyOffset(UsefulBuf Dest, size_t uOffset, const UsefulBufC Src);


/**
 @brief Copy one UsefulBuf into another

 @param[in] Dest The destination buffer to copy into
 @param[out] Src  The source to copy from

 @return filled in UsefulBufC on success, NULLUsefulBufC on failure

 This fails if Src.len is greater than Dest.len.

 Note that like memcpy, the pointers are not checked and
 this will crash, rather than return NULLUsefulBufC if
 they are NULL or invalid.

 Results are undefined if Dest and Src overlap.

 */
static inline UsefulBufC UsefulBuf_Copy(UsefulBuf Dest, const UsefulBufC Src) {
   return UsefulBuf_CopyOffset(Dest, 0, Src);
}


/**
 @brief Set all bytes in a UsefulBuf to a value, for example 0

 @param[in] pDest The destination buffer to copy into
 @param[in] value The value to set the bytes to

 Note that like memset, the pointer in pDest is not checked and
 this will crash if NULL or invalid.

 */
static inline UsefulBufC UsefulBuf_Set(UsefulBuf pDest, uint8_t value)
{
   memset(pDest.ptr, value, pDest.len);
   return (UsefulBufC){pDest.ptr, pDest.len};
}


/**
 @brief Copy a pointer into a UsefulBuf

 @param[in,out] Dest The destination buffer to copy into
 @param[in] ptr  The source to copy from
 @param[in] len  Length of the source; amoutn to copy

 @return 0 on success, 1 on failure

 This fails and returns NULLUsefulBufC if len is greater than
 pDest->len.

 Note that like memcpy, the pointers are not checked and
 this will crash, rather than return 1 if they are NULL
 or invalid.

 */
inline static UsefulBufC UsefulBuf_CopyPtr(UsefulBuf Dest, const void *ptr, size_t len)
{
   return UsefulBuf_Copy(Dest, (UsefulBufC){ptr, len});
}


/**
  @brief Returns a truncation of a UsefulBufC

  @param[in] UB The buffer to get the head of
  @param[in] uAmount The number of bytes in the head

  @return A UsefulBufC that is the head of UB

 */
static inline UsefulBufC UsefulBuf_Head(UsefulBufC UB, size_t uAmount)
{
   if(uAmount > UB.len) {
      return NULLUsefulBufC;
   }
   return (UsefulBufC){UB.ptr, uAmount};
}


/**
 @brief  Returns bytes from the end of a UsefulBufC

 @param[in] UB       The buffer to get the tail of
 @param[in] uAmount  The offset from the start where the tail is to begin

 @return A UsefulBufC that is the tail of UB or NULLUsefulBufC if
         uAmount is greater than the length of the UsefulBufC

 If the input UsefulBufC is NULL, but the len is not, then the
 length of the tail will be calculated and returned along
 with a NULL ptr.
 */
static inline UsefulBufC UsefulBuf_Tail(UsefulBufC UB, size_t uAmount)
{
   UsefulBufC ReturnValue;

   if(uAmount > UB.len) {
      ReturnValue = NULLUsefulBufC;
   } else if(UB.ptr == NULL) {
      ReturnValue = (UsefulBufC){NULL, UB.len - uAmount};
   } else {
      ReturnValue = (UsefulBufC){(uint8_t *)UB.ptr + uAmount, UB.len - uAmount};
   }

   return ReturnValue;
}


/**
 @brief Compare two UsefulBufCs

 @param[in] UB1 The destination buffer to copy into
 @param[in] UB2  The source to copy from

 @return 0 if equal...

 Returns a negative value if UB1 if is less than UB2. UB1 is
 less than UB2 if it is shorter or the first byte that is not
 the same is less.

 Returns 0 if the UsefulBufs are the same.

 Returns a positive value if UB2 is less than UB1.

 All that is of significance is that the result is positive,
 negative or 0. (This doesn't return the difference between
 the first non-matching byte like memcmp).

 */
int UsefulBuf_Compare(const UsefulBufC UB1, const UsefulBufC UB2);


/**
 @brief Find one UsefulBuf in another

 @param[in] BytesToSearch  UsefulBuf to search through
 @param[in] BytesToFind    UsefulBuf with bytes to be found

 @return position of found bytes or SIZE_MAX if not found.

 */
size_t UsefulBuf_FindBytes(UsefulBufC BytesToSearch, UsefulBufC BytesToFind);




#if 0 // NOT_DEPRECATED
/** Deprecated macro; use UsefulBuf_FROM_SZ_LITERAL instead */
#define SZLiteralToUsefulBufC(szString) \
    ((UsefulBufC) {(szString), sizeof(szString)-1})

/** Deprecated macro; use UsefulBuf_MAKE_STACK_UB instead */
#define  MakeUsefulBufOnStack(name, size) \
    uint8_t    __pBuf##name[(size)];\
    UsefulBuf  name = {__pBuf##name , sizeof( __pBuf##name )}

/** Deprecated macro; use UsefulBuf_FROM_BYTE_ARRAY_LITERAL instead */
#define ByteArrayLiteralToUsefulBufC(pBytes) \
    ((UsefulBufC) {(pBytes), sizeof(pBytes)})

/** Deprecated function; use UsefulBuf_Unconst() instead */
static inline UsefulBuf UsefulBufC_Unconst(const UsefulBufC UBC)
{
    return (UsefulBuf){(void *)UBC.ptr, UBC.len};
}
#endif



/*
 Convenient functions to avoid type punning, compiler warnings and such
 The optimizer reduces them to a simple assignment
 This is a crusty corner of C. It shouldn't be this hard.
 */
static inline uint32_t UsefulBufUtil_CopyFloatToUint32(float f)
{
    uint32_t u32;
    memcpy(&u32, &f, sizeof(uint32_t));
    return u32;
}

static inline uint64_t UsefulBufUtil_CopyDoubleToUint64(double d)
{
    uint64_t u64;
    memcpy(&u64, &d, sizeof(uint64_t));
    return u64;
}

static inline double UsefulBufUtil_CopyUint64ToDouble(uint64_t u64)
{
    double d;
    memcpy(&d, &u64, sizeof(uint64_t));
    return d;
}

static inline float UsefulBufUtil_CopyUint32ToFloat(uint32_t u32)
{
    float f;
    memcpy(&f, &u32, sizeof(uint32_t));
    return f;
}





/**
 UsefulOutBuf is a structure and functions (an object) that are good
 for serializing data into a buffer such as is often done with network
 protocols or data written to files.

 The main idea is that all the pointer manipulation for adding data is
 done by UsefulOutBuf functions so the caller doesn't have to do any.
 All the pointer manipulation is centralized here.  This code will
 have been reviewed and written carefully so it spares the caller of
 much of this work and results in much safer code with much less work.

 The functions to add data to the output buffer always check the
 length and will never write off the end of the output buffer. If an
 attempt to add data that will not fit is made, an internal error flag
 will be set and further attempts to add data will not do anything.

 Basically, if you initialized with the correct buffer, there is no
 way to ever write off the end of that buffer when calling the Add
 and Insert functions here.

 The functions to add data do not return an error. The working model
 is that the caller just makes all the calls to add data without any
 error checking on each one. The error is instead checked after all the
 data is added when the result is to be used.  This makes the caller's
 code cleaner.

 There is a utility function to get the error status anytime along the
 way if the caller wants. There are functions to see how much room is
 left and see if some data will fit too, but their use is generally
 not necessary.

 The general call flow is like this:

    - Initialize the UsefulOutBuf with the buffer that is to have the
      data added.  The caller allocates the buffer.  It can be heap
      or stack or shared memory (or other).

    - Make calls to add data to the output buffer. Insert and append
      are both supported. The append and insert calls will never write
      off the end of the buffer.

    - When all data is added, check the error status to make sure
      everything fit.

    - Get the resulting serialized data either as a UsefulBuf (a
      pointer and length) or have it copied to another buffer.

 UsefulOutBuf can be initialized with just a buffer length by passing
 NULL as the pointer to the output buffer. This is useful if you want
 to go through the whole serialization process to either see if it
 will fit into a given buffer or compute the size of the buffer
 needed. Pass a very large buffer size when calling Init, if you want
 just to compute the size.

 Some inexpensive simple sanity checks are performed before every data
 addition to guard against use of an uninitialized or corrupted
 UsefulOutBuf.

 This has been used to create a CBOR encoder. The CBOR encoder has
 almost no pointer manipulation in it, is much easier to read, and
 easier to review.

 A UsefulOutBuf is 27 bytes or 15 bytes on 64- or 32-bit machines so it
 can go on the stack or be a C99 function parameter.
 */

typedef struct useful_out_buf {
   UsefulBuf  UB; // Memory that is being output to
   size_t     data_len;  // length of the data
   uint16_t   magic; // Used to detect corruption and lack of initialization
   uint8_t    err;
} UsefulOutBuf;


/**
 @brief Initialize and supply the actual output buffer

 @param[out] me The UsefulOutBuf to initialize
 @param[in] Storage  Buffer to output into

 Intializes the UsefulOutBuf with storage. Sets the current position
 to the beginning of the buffer clears the error.

 This must be called before the UsefulOutBuf is used.
 */
void UsefulOutBuf_Init(UsefulOutBuf *me, UsefulBuf Storage);




/** Convenience marco to make a UsefulOutBuf on the stack and
   initialize it with stack buffer
 */
#define  UsefulOutBuf_MakeOnStack(name, size) \
   uint8_t       __pBuf##name[(size)];\
   UsefulOutBuf  name;\
   UsefulOutBuf_Init(&(name), (UsefulBuf){__pBuf##name, (size)});



/**
 @brief Reset a UsefulOutBuf for re use

 @param[in] me Pointer to the UsefulOutBuf

 This sets the amount of data in the output buffer to none and
 clears the error state.

 The output buffer is still the same one and size as from the
 UsefulOutBuf_Init() call.

 It doesn't zero the data, just resets to 0 bytes of valid data.
 */
static inline void UsefulOutBuf_Reset(UsefulOutBuf *me)
{
   me->data_len = 0;
   me->err    = 0;
}


/**
 @brief Returns position of end of data in the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf

 @return position of end of data

 On a freshly initialized UsefulOutBuf with no data added, this will
 return 0. After ten bytes have been added, it will return 10 and so
 on.

 Generally callers will not need this function for most uses of
 UsefulOutBuf.

 */
static inline size_t UsefulOutBuf_GetEndPosition(UsefulOutBuf *me)
{
   return me->data_len;
}


/**
 @brief Returns whether any data has been added to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf

 @return 1 if output position is at start

 */
static inline int UsefulOutBuf_AtStart(UsefulOutBuf *me)
{
   return 0 == me->data_len;
}


/**
 @brief Inserts bytes into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] NewData UsefulBuf with the bytes to insert
 @param[in] uPos Index in output buffer at which to insert

 NewData is the pointer and length for the bytes to be added to the
 output buffer. There must be room in the output buffer for all of
 NewData or an error will occur.

 The insertion point must be between 0 and the current valid data. If
 not an error will occur. Appending data to the output buffer is
 achieved by inserting at the end of the valid data. This can be
 retrieved by calling UsefulOutBuf_GetEndPosition().

 When insertion is performed, the bytes between the insertion point and
 the end of data previously added to the output buffer is slid to the
 right to make room for the new data.

 Overlapping buffers are OK. NewData can point to data in the output
 buffer.

 If an error occurs an error state is set in the UsefulOutBuf. No
 error is returned.  All subsequent attempts to add data will do
 nothing.

 Call UsefulOutBuf_GetError() to find out if there is an error. This
 is usually not needed until all additions of data are complete.

 */
void UsefulOutBuf_InsertUsefulBuf(UsefulOutBuf *me, UsefulBufC NewData, size_t uPos);


/**
 @brief Insert a data buffer into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBul
 @param[in] pBytes Pointer to the bytes to insert
 @param[in] uLen Length of the bytes to insert
 @param[in] uPos Index in output buffer at which to insert

 See UsefulOutBuf_InsertUsefulBuf() for details. This is the same with
 the difference being a pointer and length is passed in rather than an
 UsefulBuf.

 */
static inline void UsefulOutBuf_InsertData(UsefulOutBuf *me, const void *pBytes, size_t uLen, size_t uPos)
{
   UsefulBufC Data = {pBytes, uLen};
   UsefulOutBuf_InsertUsefulBuf(me, Data, uPos);
}


/**
 @brief Insert a NULL-terminated string into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] szString string to append

 */
static inline void UsefulOutBuf_InsertString(UsefulOutBuf *me, const char *szString, size_t uPos)
{
   UsefulOutBuf_InsertUsefulBuf(me, (UsefulBufC){szString, strlen(szString)}, uPos);
}


/**
 @brief Insert a byte into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBul
 @param[in] byte Bytes to insert
 @param[in] uPos Index in output buffer at which to insert

 See UsefulOutBuf_InsertUsefulBuf() for details. This is the same with
 the difference being a single byte is to be inserted.
 */
static inline void UsefulOutBuf_InsertByte(UsefulOutBuf *me, uint8_t byte, size_t uPos)
{
   UsefulOutBuf_InsertData(me, &byte, 1, uPos);
}


/**
 @brief Insert a 16-bit integer into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBul
 @param[in] uInteger16 Integer to insert
 @param[in] uPos Index in output buffer at which to insert

 See UsefulOutBuf_InsertUsefulBuf() for details. This is the same with
 the difference being a single byte is to be inserted.

 The integer will be inserted in network byte order (big endian)
 */
static inline void UsefulOutBuf_InsertUint16(UsefulOutBuf *me, uint16_t uInteger16, size_t uPos)
{
   // Converts native integer format to network byte order (big endian)
   uint8_t tmp[2];
   tmp[0] = (uInteger16 & 0xff00) >> 8;
   tmp[1] = (uInteger16 & 0xff);
   UsefulOutBuf_InsertData(me, tmp, 2, uPos);
}


/**
 @brief Insert a 32-bit integer into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBul
 @param[in] uInteger32 Integer to insert
 @param[in] uPos Index in output buffer at which to insert

 See UsefulOutBuf_InsertUsefulBuf() for details. This is the same with
 the difference being a single byte is to be inserted.

 The integer will be inserted in network byte order (big endian)
 */
static inline void UsefulOutBuf_InsertUint32(UsefulOutBuf *me, uint32_t uInteger32, size_t uPos)
{
   // Converts native integer format to network byte order (big endian)
   uint8_t tmp[4];
   tmp[0] = (uInteger32 & 0xff000000) >> 24;
   tmp[1] = (uInteger32 & 0xff0000) >> 16;
   tmp[2] = (uInteger32 & 0xff00) >> 8;
   tmp[3] = (uInteger32 & 0xff);
   UsefulOutBuf_InsertData(me, tmp, 4, uPos);
}


/**
 @brief Insert a 64-bit integer into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBul
 @param[in] uInteger64 Integer to insert
 @param[in] uPos Index in output buffer at which to insert

 See UsefulOutBuf_InsertUsefulBuf() for details. This is the same with
 the difference being a single byte is to be inserted.

 The integer will be inserted in network byte order (big endian)
 */
static inline void UsefulOutBuf_InsertUint64(UsefulOutBuf *me, uint64_t uInteger64, size_t uPos)
{
   // Converts native integer format to network byte order (big endian)
   uint8_t tmp[8];
   tmp[0] = (uInteger64 & 0xff00000000000000) >> 56;
   tmp[1] = (uInteger64 & 0xff000000000000) >> 48;
   tmp[2] = (uInteger64 & 0xff0000000000) >> 40;
   tmp[3] = (uInteger64 & 0xff00000000) >> 32;
   tmp[4] = (uInteger64 & 0xff000000) >> 24;
   tmp[5] = (uInteger64 & 0xff0000) >> 16;
   tmp[6] = (uInteger64 & 0xff00) >> 8;
   tmp[7] = (uInteger64 & 0xff);
   UsefulOutBuf_InsertData(me, tmp, 8, uPos);
}


/**
 @brief Insert a float into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBul
 @param[in] f Integer to insert
 @param[in] uPos Index in output buffer at which to insert

 See UsefulOutBuf_InsertUsefulBuf() for details. This is the same with
 the difference being a single byte is to be inserted.

 The float will be inserted in network byte order (big endian)
 */
static inline void UsefulOutBuf_InsertFloat(UsefulOutBuf *me, float f, size_t uPos)
{
   UsefulOutBuf_InsertUint32(me, UsefulBufUtil_CopyFloatToUint32(f), uPos);
}


/**
 @brief Insert a double into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBul
 @param[in] d Integer to insert
 @param[in] uPos Index in output buffer at which to insert

 See UsefulOutBuf_InsertUsefulBuf() for details. This is the same with
 the difference being a single byte is to be inserted.

 The double will be inserted in network byte order (big endian)
 */
static inline void UsefulOutBuf_InsertDouble(UsefulOutBuf *me, double d, size_t uPos)
{
   UsefulOutBuf_InsertUint64(me, UsefulBufUtil_CopyDoubleToUint64(d), uPos);
}



/**
 Append a UsefulBuf into the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] NewData UsefulBuf with the bytes to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.

*/
static inline void UsefulOutBuf_AppendUsefulBuf(UsefulOutBuf *me, UsefulBufC NewData)
{
   // An append is just a insert at the end
   UsefulOutBuf_InsertUsefulBuf(me, NewData, UsefulOutBuf_GetEndPosition(me));
}


/**
 Append bytes to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] pBytes Pointer to bytes to append
 @param[in] uLen Index in output buffer at which to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.
 */

static inline void UsefulOutBuf_AppendData(UsefulOutBuf *me, const void *pBytes, size_t uLen)
{
   UsefulBufC Data = {pBytes, uLen};
   UsefulOutBuf_AppendUsefulBuf(me, Data);
}


/**
 Append a NULL-terminated string to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] szString string to append

 */
static inline void UsefulOutBuf_AppendString(UsefulOutBuf *me, const char *szString)
{
   UsefulOutBuf_AppendUsefulBuf(me, (UsefulBufC){szString, strlen(szString)});
}


/**
 @brief Append a byte to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] byte Bytes to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.
 */
static inline void UsefulOutBuf_AppendByte(UsefulOutBuf *me, uint8_t byte)
{
   UsefulOutBuf_AppendData(me, &byte, 1);
}

/**
 @brief Append an integer to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] uInteger16 Integer to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.

 The integer will be appended in network byte order (big endian).
 */
static inline void UsefulOutBuf_AppendUint16(UsefulOutBuf *me, uint16_t uInteger16){
   UsefulOutBuf_InsertUint16(me, uInteger16, UsefulOutBuf_GetEndPosition(me));
}

/**
 @brief Append an integer to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] uInteger32 Integer to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.

 The integer will be appended in network byte order (big endian).
 */
static inline void UsefulOutBuf_AppendUint32(UsefulOutBuf *me, uint32_t uInteger32){
   UsefulOutBuf_InsertUint32(me, uInteger32, UsefulOutBuf_GetEndPosition(me));
}

/**
 @brief Append an integer to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] uInteger64 Integer to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.

 The integer will be appended in network byte order (big endian).
 */
static inline void UsefulOutBuf_AppendUint64(UsefulOutBuf *me, uint64_t uInteger64){
   UsefulOutBuf_InsertUint64(me, uInteger64, UsefulOutBuf_GetEndPosition(me));
}


/**
 @brief Append a float to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] f Float to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.

 The float will be appended in network byte order (big endian).
 */
static inline void UsefulOutBuf_AppendFloat(UsefulOutBuf *me, float f){
   UsefulOutBuf_InsertFloat(me, f, UsefulOutBuf_GetEndPosition(me));
}

/**
 @brief Append a float to the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] d Double to append

 See UsefulOutBuf_InsertUsefulBuf() for details. This does the same
 with the insertion point at the end of the valid data.

 The double will be appended in network byte order (big endian).
 */
static inline void UsefulOutBuf_AppendDouble(UsefulOutBuf *me, double d){
   UsefulOutBuf_InsertDouble(me, d, UsefulOutBuf_GetEndPosition(me));
}

/**
 @brief Returns the current error status

 @param[in] me Pointer to the UsefulOutBuf

 @return 0 if all OK, 1 on error

 This is the error status since the call to either
 UsefulOutBuf_Reset() of UsefulOutBuf_Init().  Once it goes into error
 state it will stay until one of those functions is called.

 Possible error conditions are:
   - bytes to be inserted will not fit
   - insertion point is out of buffer or past valid data
   - current position is off end of buffer (probably corruption or uninitialized)
   - detect corruption / uninitialized by bad magic number
 */

static inline int UsefulOutBuf_GetError(UsefulOutBuf *me)
{
   return me->err;
}


/**
 @brief Returns number of bytes unused used in the output buffer

 @param[in] me Pointer to the UsefulOutBuf

 @return Number of unused bytes or zero

 Because of the error handling strategy and checks in UsefulOutBuf_InsertUsefulBuf()
 it is usually not necessary to use this.
 */

static inline size_t UsefulOutBuf_RoomLeft(UsefulOutBuf *me)
{
   return me->UB.len - me->data_len;
}


/**
 @brief Returns true / false if some number of bytes will fit in the UsefulOutBuf

 @param[in] me Pointer to the UsefulOutBuf
 @param[in] uLen Number of bytes for which to check

 @return 1 or 0 if nLen bytes would fit

 Because of the error handling strategy and checks in UsefulOutBuf_InsertUsefulBuf()
 it is usually not necessary to use this.
 */

static inline int UsefulOutBuf_WillItFit(UsefulOutBuf *me, size_t uLen)
{
   return uLen <= UsefulOutBuf_RoomLeft(me);
}


/**
   @brief Returns the resulting valid data in a UsefulOutBuf

   @param[in] me Pointer to the UsefulOutBuf.

   @return The valid data in UsefulOutBuf.

   The storage for the returned data is Storage parameter passed
   to UsefulOutBuf_Init(). See also UsefulOutBuf_CopyOut().

   This can be called anytime and many times to get intermediate
   results. It doesn't change the data or reset the current position
   so you can keep adding data.
 */

UsefulBufC UsefulOutBuf_OutUBuf(UsefulOutBuf *me);


/**
 @brief Copies the valid data out into a supplied buffer

 @param[in] me Pointer to the UsefulOutBuf
 @param[out] Dest The destination buffer to copy into

 @return Pointer and length of copied data.

 This is the same as UsefulOutBuf_OutUBuf() except it copies the data.
*/

UsefulBufC UsefulOutBuf_CopyOut(UsefulOutBuf *me, UsefulBuf Dest);













/**
 UsefulInputBuf is the counterpart to UsefulOutBuf and is for parsing
 data read or received.  Initialize it with the data
 from the network and its length. Then  use the functions
 here to get the various data types out of it. It maintains a position
 for getting the next item. This means you don't have to track a
 pointer as you get each object. UsefulInputBuf does that for you and
 makes sure it never goes off the end of the buffer.  The QCBOR
 implementation parser makes use of this for all its pointer math and
 length checking.

 UsefulInputBuf also maintains an internal error state so you do not have
 to. Once data has been requested off the end of the buffer, it goes
 into an error state. You can keep calling functions to get more data
 but they will either return 0 or NULL. As long as you don't
 dereference the NULL, you can wait until all data items have been
 fetched before checking for the error and this can simplify your
 code.

 The integer and float parsing expects network byte order (big endian).
 Network byte order is what is used by TCP/IP, CBOR and most internet
 protocols.

 Lots of inlining is used to keep code size down. The code optimizer,
 particularly with the -Os, also reduces code size a lot. The only
 non-inline code is UsefulInputBuf_GetBytes() which is less than 100
 bytes so use of UsefulInputBuf doesn't add much code for all the messy
 hard-to-get right issues with parsing in C that is solves.

 The parse context size is:
   64-bit machine: 16 + 8 + 2 + 1 (5 bytes padding to align) = 32 bytes
   32-bit machine: 8 + 4 + 2 + 1 (1 byte padding to align) = 16 bytes

 */

#define UIB_MAGIC (0xB00F)

typedef struct useful_input_buf {
   // Private data structure
   UsefulBufC UB;     // Data being parsed
   size_t     cursor; // Current offset in data being parse
   uint16_t   magic;  // Check for corrupted or uninitialized UsefulInputBuf
   uint8_t    err;    // Set request goes off end or magic number is bad
} UsefulInputBuf;



/**
 @brief Initialize the UsefulInputBuf structure before use.

 @param[in] me Pointer to the UsefulInputBuf instance.
 @param[in] UB Pointer to the data to parse.

 */
static inline void UsefulInputBuf_Init(UsefulInputBuf *me, UsefulBufC UB)
{
   me->cursor = 0;
   me->err    = 0;
   me->magic  = UIB_MAGIC;
   me->UB     = UB;
}


/**
 @brief Returns current position in input buffer

 @param[in] me Pointer to the UsefulInputBuf.

 @return Integer position of the cursor

 The position that the next bytes will be returned from.

 */
static inline size_t UsefulInputBuf_Tell(UsefulInputBuf *me)
{
   return me->cursor;
}


/**
 @brief Sets current position in input buffer

 @param[in] me Pointer to the UsefulInputBuf.
 @param[in] uPos  Position to set to

 If the position is off the end of the input buffer, the error state
 is entered and all functions will do nothing.

 Seeking to a valid position in the buffer will not reset the error
 state. Only re initialization will do that.

 */
static inline void UsefulInputBuf_Seek(UsefulInputBuf *me, size_t uPos)
{
   if(uPos > me->UB.len) {
      me->err = 1;
   } else {
      me->cursor = uPos;
   }
}


/**
 @brief Returns the number of bytes from the cursor to the end of the buffer,
 the uncomsummed bytes.

 @param[in] me Pointer to the UsefulInputBuf.

 @return number of bytes unconsumed or 0 on error.

 This is a critical function for input length validation. This does
 some pointer / offset math.

 Returns 0 if the cursor it invalid or corruption of the structure is
 detected.

 Code Reviewers: THIS FUNCTION DOES POINTER MATH
 */
static inline size_t UsefulInputBuf_BytesUnconsumed(UsefulInputBuf *me)
{
   // Magic number is messed up. Either the structure got overwritten
   // or was never initialized.
   if(me->magic != UIB_MAGIC) {
      return 0;
   }

   // The cursor is off the end of the input buffer given
   // Presuming there are no bugs in this code, this should never happen.
   // If it so, the struct was corrupted. The check is retained as
   // as a defense in case there is a bug in this code or the struct is corrupted.
   if(me->cursor > me->UB.len) {
      return 0;
   }

   // subtraction can't go neative because of check above
   return me->UB.len - me->cursor;
}


/**
 @brief Check if there are any unconsumed bytes

 @param[in] me Pointer to the UsefulInputBuf.

 @return 1 if len bytes are available after the cursor, and 0 if not

 */
static inline int UsefulInputBuf_BytesAvailable(UsefulInputBuf *me, size_t uLen)
{
   return UsefulInputBuf_BytesUnconsumed(me) >= uLen ? 1 : 0;
}


/**
 @brief Get pointer to bytes out of the input buffer

 @param[in] me Pointer to the UsefulInputBuf.
 @param[in] uNum  Number of bytes to get

 @return Pointer to bytes.

 This consumes n bytes from the input buffer. It returns a pointer to
 the start of the n bytes.

 If there are not n bytes in the input buffer, NULL will be returned
 and an error will be set.

 It advances the current position by n bytes.
 */
const void * UsefulInputBuf_GetBytes(UsefulInputBuf *me, size_t uNum);


/**
 @brief Get UsefulBuf out of the input buffer

 @param[in] me Pointer to the UsefulInputBuf.
 @param[in] uNum  Number of bytes to get

 @return UsefulBufC with ptr and length for bytes consumed.

 This consumes n bytes from the input buffer and returns the pointer
 and len to them as a UsefulBufC. The len returned will always be n.

 If there are not n bytes in the input buffer, UsefulBufC.ptr will be
 NULL and UsefulBufC.len will be 0. An error will be set.

 It advances the current position by n bytes.
 */
static inline UsefulBufC UsefulInputBuf_GetUsefulBuf(UsefulInputBuf *me, size_t uNum)
{
   const void *pResult = UsefulInputBuf_GetBytes(me, uNum);
   if(!pResult) {
      return NULLUsefulBufC;
   } else {
      return (UsefulBufC){pResult, uNum};
   }
}


/**
 @brief Get a byte out of the input buffer.

 @param[in] me Pointer to the UsefulInputBuf.

 @return The byte

 This consumes 1 byte from the input buffer. It returns the byte.

 If there is not 1 byte in the buffer, 0 will be returned for the byte
 and an error set internally.  You must check the error at some point
 to know whether the 0 was the real value or just returned in error,
 but you may not have to do that right away.  Check the error state
 with UsefulInputBuf_GetError().  You can also know you are in the
 error state if UsefulInputBuf_GetBytes() returns NULL or the ptr from
 UsefulInputBuf_GetUsefulBuf() is NULL.

 It advances the current position by 1 byte.
 */
static inline uint8_t UsefulInputBuf_GetByte(UsefulInputBuf *me)
{
   const void *pResult = UsefulInputBuf_GetBytes(me, sizeof(uint8_t));

   return pResult ? *(uint8_t *)pResult : 0;
}


/**
 @brief Get a uint16_t out of the input buffer

 @param[in] me Pointer to the UsefulInputBuf.

 @return The uint16_t

 See UsefulInputBuf_GetByte(). This works the same, except it returns
 a uint16_t and two bytes are consumed.

 The input bytes must be in network order (big endian).
 */
static inline uint16_t UsefulInputBuf_GetUint16(UsefulInputBuf *me)
{
   const uint8_t *pResult = (const uint8_t *)UsefulInputBuf_GetBytes(me, sizeof(uint16_t));

   if(!pResult) {
      return 0;
   }

   return  ((uint16_t)pResult[0] << 8) + (uint16_t)pResult[1];
}


/**
 @brief Get a uint32_t out of the input buffer

 @param[in] me Pointer to the UsefulInputBuf.

 @return The uint32_t

 See UsefulInputBuf_GetByte(). This works the same, except it returns
 a uint32_t and four bytes are consumed.

 The input bytes must be in network order (big endian).
 */
static inline uint32_t UsefulInputBuf_GetUint32(UsefulInputBuf *me)
{
   const uint8_t *pResult = (const uint8_t *)UsefulInputBuf_GetBytes(me, sizeof(uint32_t));

   if(!pResult) {
      return 0;
   }

   return ((uint32_t)pResult[0]<<24) +
          ((uint32_t)pResult[1]<<16) +
          ((uint32_t)pResult[2]<<8) +
           (uint32_t)pResult[3];
}


/**
 @brief Get a uint64_t out of the input buffer

 @param[in] me Pointer to the UsefulInputBuf.

 @return The uint64_t

 See UsefulInputBuf_GetByte(). This works the same, except it returns
 a uint64_t and eight bytes are consumed.

 The input bytes must be in network order (big endian).
 */
static inline uint64_t UsefulInputBuf_GetUint64(UsefulInputBuf *me)
{
   const uint8_t *pResult = (const uint8_t *)UsefulInputBuf_GetBytes(me, sizeof(uint64_t));

   if(!pResult) {
      return 0;
   }

   return   ((uint64_t)pResult[0]<<56) +
            ((uint64_t)pResult[1]<<48) +
            ((uint64_t)pResult[2]<<40) +
            ((uint64_t)pResult[3]<<32) +
            ((uint64_t)pResult[4]<<24) +
            ((uint64_t)pResult[5]<<16) +
            ((uint64_t)pResult[6]<<8)  +
            (uint64_t)pResult[7];
}


/**
 @brief Get a float out of the input buffer

 @param[in] me Pointer to the UsefulInputBuf.

 @return The float

 See UsefulInputBuf_GetByte(). This works the same, except it returns
 a float and four bytes are consumed.

 The input bytes must be in network order (big endian).
 */
static inline float UsefulInputBuf_GetFloat(UsefulInputBuf *me)
{
   uint32_t uResult = UsefulInputBuf_GetUint32(me);

   return uResult ? UsefulBufUtil_CopyUint32ToFloat(uResult) : 0;
}

/**
 @brief Get a double out of the input buffer

 @param[in] me Pointer to the UsefulInputBuf.

 @return The double

 See UsefulInputBuf_GetByte(). This works the same, except it returns
 a double and eight bytes are consumed.

 The input bytes must be in network order (big endian).
 */
static inline double UsefulInputBuf_GetDouble(UsefulInputBuf *me)
{
   uint64_t uResult = UsefulInputBuf_GetUint64(me);

   return uResult ? UsefulBufUtil_CopyUint64ToDouble(uResult) : 0;
}


/**
 @brief Get the error status

 @param[in] me Pointer to the UsefulInputBuf.

 @return The error.

 Zero is success, non-zero is error. Once in the error state, the only
 way to clear it is to call Init again.

 You may be able to only check the error state at the end after all
 the Get()'s have been done, but if what you get later depends on what
 you get sooner you cannot. For example if you get a length or count
 of following items you will have to check the error.

 */
static inline int UsefulInputBuf_GetError(UsefulInputBuf *me)
{
   return me->err;
}


#endif  // _UsefulBuf_h


