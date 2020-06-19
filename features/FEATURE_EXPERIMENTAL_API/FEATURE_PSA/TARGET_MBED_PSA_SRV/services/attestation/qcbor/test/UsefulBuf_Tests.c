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

#include "UsefulBuf.h"


/* Basic exercise...

   Call all the main public functions.

   Binary compare the result to the expected.

   There is nothing adversarial in this test
 */
const char * UOBTest_NonAdversarial()
{
   const char *szReturn = NULL;

   UsefulBuf_MAKE_STACK_UB(outbuf,50);

   UsefulOutBuf UOB;

   UsefulOutBuf_Init(&UOB, outbuf);

   if(!UsefulOutBuf_AtStart(&UOB)) {
      szReturn = "Not at start";
      goto Done;
   }

   // Put 7 bytes at beginning of buf
   UsefulOutBuf_AppendData(&UOB, "bluster", 7);

   if(UsefulOutBuf_AtStart(&UOB)) {
      szReturn = "At start";
      goto Done;
   }

   // add a space to end
   UsefulOutBuf_AppendByte(&UOB, ' ');

   // Add 5 bytes to the end
   UsefulBufC UBC = {"hunny", 5};
   UsefulOutBuf_AppendUsefulBuf(&UOB, UBC);

   // Insert 9 bytes at the beginning, slide the previous stuff right
   UsefulOutBuf_InsertData(&UOB, "heffalump", 9, 0);
   UsefulOutBuf_InsertByte(&UOB, ' ', 9);

   // Put 9 bytes in at position 10 -- just after "heffalump "
   UsefulBufC UBC2 = {"unbounce ", 9};
   UsefulOutBuf_InsertUsefulBuf(&UOB, UBC2, 10);

   // Make it a null terminated string (because all the appends and inserts above not strcpy !)
   UsefulOutBuf_AppendByte(&UOB, '\0');


   UsefulBufC U = UsefulOutBuf_OutUBuf(&UOB);

   const char *expected = "heffalump unbounce bluster hunny";

   if(UsefulBuf_IsNULLC(U) || U.len-1 != strlen(expected) || strcmp(expected, U.ptr) || UsefulOutBuf_GetError(&UOB)) {
      szReturn = "OutUBuf";
   }

   UsefulBuf_MAKE_STACK_UB(buf, 50);
   UsefulBufC Out =  UsefulOutBuf_CopyOut(&UOB, buf);

   if(UsefulBuf_IsNULLC(Out) || Out.len-1 != strlen(expected) || strcmp(expected, Out.ptr)) {
      szReturn = "CopyOut";
   }

Done:
   return szReturn;
}


/*
  Append test utility.
    pUOB is the buffer to append too
    num is the amount to append
    expected is the expected return code, 0 or 1

 returns 0 if test passed

 */
static int AppendTest(UsefulOutBuf *pUOB, size_t num, int expected)
{
   //reset
   UsefulOutBuf_Reset(pUOB);

   // check status first
   if(UsefulOutBuf_GetError(pUOB))
      return 1;

   // append the bytes
   UsefulOutBuf_AppendData(pUOB, (const uint8_t *)"bluster", num);

   // check error status after
   if(UsefulOutBuf_GetError(pUOB) != expected)
      return 1;

   return 0;
}


/*
 Same as append, but takes a position param too
 */
static int InsertTest(UsefulOutBuf *pUOB,  size_t num, size_t pos, int expected)
{
   // reset
   UsefulOutBuf_Reset(pUOB);

   // check
   if(UsefulOutBuf_GetError(pUOB))
      return 1;

   UsefulOutBuf_InsertData(pUOB, (const uint8_t *)"bluster", num, pos);

   if(UsefulOutBuf_GetError(pUOB) != expected)
      return 1;

   return 0;
}


/*
 Boundary conditions to test
   - around 0
   - around the buffer size
   - around MAX size_t


 Test these for the buffer size and the cursor, the insert amount, the append amount and the insert position

 */

const char *UOBTest_BoundaryConditionsTest()
{
   UsefulBuf_MAKE_STACK_UB(outbuf,2);

   UsefulOutBuf UOB;

   UsefulOutBuf_Init(&UOB, outbuf);

   // append 0 byte to a 2 byte buffer --> success
   if(AppendTest(&UOB, 0, 0))
      return "Append 0 bytes failed";

   // append 1 byte to a 2 byte buffer --> success
   if(AppendTest(&UOB, 1, 0))
      return "Append of 1 byte failed";

   // append 2 byte to a 2 byte buffer --> success
   if(AppendTest(&UOB, 2, 0))
      return "Append to fill buffer failed";

   // append 3 bytes to a 2 byte buffer --> failure
   if(AppendTest(&UOB, 3, 1))
      return "Overflow of buffer not caught";

   // append max size_t to a 2 byte buffer --> failure
   if(AppendTest(&UOB, SIZE_MAX, 1))
      return "Append of SIZE_MAX error not caught";

   if(InsertTest(&UOB, 1, 0, 0))
      return "Insert 1 byte at start failed";

   if(InsertTest(&UOB, 2, 0, 0))
      return "Insert 2 bytes at start failed";

   if(InsertTest(&UOB, 3, 0, 1))
      return "Insert overflow not caught";

   if(InsertTest(&UOB, 1, 1, 1))
      return "Bad insertion point not caught";


   UsefulBuf_MAKE_STACK_UB(outBuf2,10);

   UsefulOutBuf_Init(&UOB, outBuf2);

   UsefulOutBuf_Reset(&UOB);
   // put data in the buffer
   UsefulOutBuf_AppendString(&UOB, "abc123");

   UsefulOutBuf_InsertString(&UOB, "xyz*&^", 0);

   if(!UsefulOutBuf_GetError(&UOB)) {
      return "insert with data should have failed";
   }


   UsefulOutBuf_Init(&UOB, (UsefulBuf){NULL, SIZE_MAX - 5});
   UsefulOutBuf_AppendData(&UOB, "123456789", SIZE_MAX -6);
   if(UsefulOutBuf_GetError(&UOB)) {
      return "insert in huge should have succeeded";
   }

   UsefulOutBuf_Init(&UOB, (UsefulBuf){NULL, SIZE_MAX - 5});
   UsefulOutBuf_AppendData(&UOB, "123456789", SIZE_MAX -5);
   if(UsefulOutBuf_GetError(&UOB)) {
      return "insert in huge should have succeeded";
   }

   UsefulOutBuf_Init(&UOB, (UsefulBuf){NULL, SIZE_MAX - 5});
   UsefulOutBuf_AppendData(&UOB, "123456789", SIZE_MAX - 4);
   if(!UsefulOutBuf_GetError(&UOB)) {
      return "lengths near max size";
   }

   return NULL;
}





// Test function to get size and magic number check

const char *TestBasicSanity()
{
   UsefulBuf_MAKE_STACK_UB(outbuf,10);

   UsefulOutBuf UOB;

   // First -- make sure that the room left function returns the right amount
   UsefulOutBuf_Init(&UOB, outbuf);

   if(UsefulOutBuf_RoomLeft(&UOB) != 10)
      return "room left failed";

   if(!UsefulOutBuf_WillItFit(&UOB, 9)) {
      return "it did not fit";
   }

   if(UsefulOutBuf_WillItFit(&UOB, 11)) {
      return "it should have not fit";
   }


   // Next -- make sure that the magic number checking is working right
   UOB.magic = 8888; // make magic bogus

   UsefulOutBuf_AppendData(&UOB, (const uint8_t *)"bluster", 7);

   if(!UsefulOutBuf_GetError(&UOB))
      return "magic corruption check failed";



   // Next make sure that the valid data length check is working right
   UsefulOutBuf_Init(&UOB, outbuf);

   UOB.data_len = UOB.UB.len+1; // make size bogus

   UsefulOutBuf_AppendData(&UOB, (const uint8_t *)"bluster", 7);
   if(!UsefulOutBuf_GetError(&UOB))
      return "valid data check failed";

   return NULL;
}



const char *UBMacroConversionsTest()
{
   char *szFoo = "foo";

   UsefulBufC Foo = UsefulBuf_FromSZ(szFoo);
   if(Foo.len != 3 || strncmp(Foo.ptr, szFoo, 3))
      return "SZToUsefulBufC failed";

   UsefulBufC Too = UsefulBuf_FROM_SZ_LITERAL("Toooo");
   if(Too.len != 5 || strncmp(Too.ptr, "Toooo", 5))
      return "UsefulBuf_FROM_SZ_LITERAL failed";

   uint8_t pB[] = {0x42, 0x6f, 0x6f};
   UsefulBufC Boo = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pB);
   if(Boo.len != 3 || strncmp(Boo.ptr, "Boo", 3))
     return "UsefulBuf_FROM_BYTE_ARRAY_LITERAL failed";

   char *sz = "not const"; // some data for the test
   UsefulBuf B = (UsefulBuf){sz, sizeof(sz)};
   UsefulBufC BC = UsefulBuf_Const(B);
   if(BC.len != sizeof(sz) || BC.ptr != sz)
      return "UsefulBufConst failed";

   return NULL;
}


const char *UBUtilTests()
{
   UsefulBuf UB = NULLUsefulBuf;

   if(!UsefulBuf_IsNULL(UB)){
      return "IsNull failed";
   }

   if(!UsefulBuf_IsEmpty(UB)){
      return "IsEmpty failed";
   }

   if(!UsefulBuf_IsNULLOrEmpty(UB)) {
      return "IsNULLOrEmpty failed";
   }

   const UsefulBufC UBC = UsefulBuf_Const(UB);

   if(!UsefulBuf_IsNULLC(UBC)){
      return "IsNull const failed";
   }

   if(!UsefulBuf_IsEmptyC(UBC)){
      return "IsEmptyC failed";
   }

   if(!UsefulBuf_IsNULLOrEmptyC(UBC)){
      return "IsNULLOrEmptyC failed";
   }

   const UsefulBuf UB2 = UsefulBuf_Unconst(UBC);
   if(!UsefulBuf_IsEmpty(UB2)) {
      return "Back to UB is Empty failed";
   }

   UB.ptr = "x"; // just some valid pointer

   if(UsefulBuf_IsNULL(UB)){
      return "IsNull failed";
   }

   if(!UsefulBuf_IsEmptyC(UBC)){
      return "IsEmpty failed";
   }

   // test the Unconst.
   if(UsefulBuf_Unconst(UBC).ptr != NULL) {
      return "Unconst failed";
   }

   // Set 100 bytes of '+'; validated a few tests later
   UsefulBuf_MAKE_STACK_UB(Temp, 100);
   const UsefulBufC TempC = UsefulBuf_Set(Temp, '+');

   // Try to copy into a buf that is too small and see failure
   UsefulBuf_MAKE_STACK_UB(Temp2, 99);
   if(!UsefulBuf_IsNULLC(UsefulBuf_Copy(Temp2, TempC))) {
      return "Copy should have failed";
   }

   if(UsefulBuf_IsNULLC(UsefulBuf_CopyPtr(Temp2, "xx", 2))) {
      return "CopyPtr failed";
   }

   UsefulBufC xxyy = UsefulBuf_CopyOffset(Temp2, 2, UsefulBuf_FROM_SZ_LITERAL("yy"));
   if(UsefulBuf_IsNULLC(xxyy)) {
      return "CopyOffset Failed";
   }

   if(UsefulBuf_Compare(UsefulBuf_Head(xxyy, 3), UsefulBuf_FROM_SZ_LITERAL("xxy"))) {
      return "head failed";
   }

   if(UsefulBuf_Compare(UsefulBuf_Tail(xxyy, 1), UsefulBuf_FROM_SZ_LITERAL("xyy"))) {
      return "tail failed";
   }

   if(!UsefulBuf_IsNULLC(UsefulBuf_Head(xxyy, 5))) {
      return "head should have failed";
   }

   if(!UsefulBuf_IsNULLC(UsefulBuf_Tail(xxyy, 5))) {
      return "tail should have failed";
   }

   if(!UsefulBuf_IsNULLC(UsefulBuf_Tail(NULLUsefulBufC, 0))) {
      return "tail of NULLUsefulBufC is not NULLUsefulBufC";
   }

   const UsefulBufC TailResult = UsefulBuf_Tail((UsefulBufC){NULL, 100}, 99);
   if(TailResult.ptr != NULL || TailResult.len != 1) {
      return "tail of NULL and length incorrect";
   }

   if(!UsefulBuf_IsNULLC(UsefulBuf_CopyOffset(Temp2, 100, UsefulBuf_FROM_SZ_LITERAL("yy")))) {
      return "Copy Offset should have failed";
   }

   // Try to copy into a NULL/empty buf and see failure
   const UsefulBuf UBNull = NULLUsefulBuf;
   if(!UsefulBuf_IsNULLC(UsefulBuf_Copy(UBNull, TempC))) {
      return "Copy to NULL should have failed";
   }


   // Try to set a NULL/empty buf; nothing should happen
   UsefulBuf_Set(UBNull, '+'); // This will crash on failure

   // Copy successfully to a buffer
   UsefulBuf_MAKE_STACK_UB(Temp3, 101);
   if(UsefulBuf_IsNULLC(UsefulBuf_Copy(Temp3, TempC))) {
      return "Copy should not have failed";
   }

   static const uint8_t pExpected[] = {
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
   };
   UsefulBufC Expected = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pExpected);
   // This validates comparison for equality and the UsefulBuf_Set
   if(UsefulBuf_Compare(Expected, TempC)) {
      return "Set / Copy / Compare failed";
   }

   // Compare two empties and expect success
   if(UsefulBuf_Compare(NULLUsefulBufC, NULLUsefulBufC)){
      return "Compare Empties failed";
   }

   // Compare with empty and expect the first to be larger
   if(UsefulBuf_Compare(Expected, NULLUsefulBufC) <= 0){
      return "Compare with empty failed";
   }


   static const uint8_t pExpectedBigger[] = {
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  ',',
   };
   const UsefulBufC ExpectedBigger = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pExpectedBigger);

   // Expect -1 when the first arg is smaller
   if(UsefulBuf_Compare(Expected, ExpectedBigger) >= 0){
      return "Compare with bigger";
   }


   static const uint8_t pExpectedSmaller[] = {
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '*',
   };
   const UsefulBufC ExpectedSmaller = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pExpectedSmaller);
   // Expect +1 when the first arg is larger
   if(UsefulBuf_Compare(Expected, ExpectedSmaller) <= 0){
      return "Compare with smaller";
   }


   static const uint8_t pExpectedLonger[] = {
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+', '+'
   };
   const UsefulBufC ExpectedLonger = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pExpectedLonger);

   // Expect -1 when the first arg is smaller
   if(UsefulBuf_Compare(Expected, ExpectedLonger) >= 0){
      return "Compare with longer";
   }


   static const uint8_t pExpectedShorter[] = {
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',  '+',
      '+',  '+',  '+',  '+', '+',  '+',  '+', '+',  '+',
   };
   const UsefulBufC ExpectedShorter = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pExpectedShorter);
   // Expect +1 with the first arg is larger
   if(UsefulBuf_Compare(Expected, ExpectedShorter) <= 0){
      return "Compare with shorter";
   }


   if(UsefulBuf_IsNULLC(UsefulBuf_Copy(Temp, NULLUsefulBufC))) {
      return "Copy null/empty failed";
   }

   // Look for +++++... in +++++... and find it at the beginning
   if(0 != UsefulBuf_FindBytes(ExpectedLonger, ExpectedShorter)){
      return "Failed to find";
   }

   // look for ++* in ....++* and find it at the end
   static const uint8_t pToFind[] = {'+', '+', '*'};
   const UsefulBufC ToBeFound = UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pToFind);

   if(97 != UsefulBuf_FindBytes(ExpectedSmaller, ToBeFound)){
      return "Failed to find 2";
   }

   // look for ++* in ....++, and find it near the end
   if(SIZE_MAX != UsefulBuf_FindBytes(ExpectedBigger, ToBeFound)){
      return "Failed to not find";
   }

   // Look for the whole buffer in itself and succeed.
   if(0 != UsefulBuf_FindBytes(ExpectedLonger, ExpectedLonger)){
      return "Failed to find 3";
   }

   return NULL;
}


const char *  UIBTest_IntegerFormat()
{
   UsefulOutBuf_MakeOnStack(UOB,100);

   const uint32_t u32 = 0x0A0B0C0D; // from https://en.wikipedia.org/wiki/Endianness
   const uint64_t u64 = 1984738472938472;
   const uint16_t u16 = 40000;
   const uint8_t  u8 = 9;
   const float    f  = (float)314.15;
   const double   d  = 2.1e10;


   UsefulOutBuf_AppendUint32(&UOB, u32); // Also tests UsefulOutBuf_InsertUint64 and UsefulOutBuf_GetEndPosition
   UsefulOutBuf_AppendUint64(&UOB, u64); // Also tests UsefulOutBuf_InsertUint32
   UsefulOutBuf_AppendUint16(&UOB, u16); // Also tests UsefulOutBuf_InsertUint16
   UsefulOutBuf_AppendByte(&UOB, u8);
   UsefulOutBuf_AppendFloat(&UOB, f); // Also tests UsefulOutBuf_InsertFloat
   UsefulOutBuf_AppendDouble(&UOB, d); // Also tests UsefulOutBuf_InsertDouble

   const UsefulBufC O = UsefulOutBuf_OutUBuf(&UOB);
   if(UsefulBuf_IsNULLC(O))
      return "Couldn't output integers";

   // from https://en.wikipedia.org/wiki/Endianness
   const uint8_t pExpectedNetworkOrder[4] = {0x0A, 0x0B, 0x0C, 0x0D};
   if(memcmp(O.ptr, pExpectedNetworkOrder, 4)) {
      return "not in network order";
   }

   UsefulInputBuf UIB;

   UsefulInputBuf_Init(&UIB, O);

   if(UsefulInputBuf_Tell(&UIB) != 0) {
      return "UsefulInputBuf_Tell failed";
   }

   if(UsefulInputBuf_GetUint32(&UIB) != u32) {
      return "u32 out then in failed";
   }
   if(UsefulInputBuf_GetUint64(&UIB) != u64) {
      return "u64 out then in failed";
   }
   if(UsefulInputBuf_GetUint16(&UIB) != u16) {
      return "u16 out then in failed";
   }
   if(UsefulInputBuf_GetByte(&UIB) != u8) {
      return "u8 out then in failed";
   }
   if(UsefulInputBuf_GetFloat(&UIB) != f) {
      return "float out then in failed";
   }
   if(UsefulInputBuf_GetDouble(&UIB) != d) {
      return "double out then in failed";
   }

   // Reset and go again for a few more tests
   UsefulInputBuf_Init(&UIB, O);

   const UsefulBufC Four = UsefulInputBuf_GetUsefulBuf(&UIB, 4);
   if(UsefulBuf_IsNULLC(Four)) {
      return "Four is NULL";
   }
   if(UsefulBuf_Compare(Four, UsefulBuf_FROM_BYTE_ARRAY_LITERAL(pExpectedNetworkOrder))) {
      return "Four compare failed";
   }

   if(UsefulInputBuf_BytesUnconsumed(&UIB) != 23){
      return "Wrong number of unconsumed bytes";
   }

   if(!UsefulInputBuf_BytesAvailable(&UIB, 23)){
      return "Wrong number of bytes available I";
   }

   if(UsefulInputBuf_BytesAvailable(&UIB, 24)){
      return "Wrong number of bytes available II";
   }

   UsefulInputBuf_Seek(&UIB, 0);

   if(UsefulInputBuf_GetError(&UIB)) {
      return "unexpected error after seek";
   }

   const uint8_t *pGetBytes = (const uint8_t *)UsefulInputBuf_GetBytes(&UIB, 4);
   if(pGetBytes == NULL) {
      return "GetBytes returns NULL";
   }

   if(memcmp(pGetBytes, pExpectedNetworkOrder, 4)) {
      return "Got wrong bytes";
   }

   UsefulInputBuf_Seek(&UIB, 28);

   if(!UsefulInputBuf_GetError(&UIB)) {
      return "expected error after seek";
   }

   return NULL;
}


const char *UBUTest_CopyUtil()
{
   if(UsefulBufUtil_CopyFloatToUint32(65536.0F) != 0x47800000) {
      return "CopyFloatToUint32 failed";
   }

   if(UsefulBufUtil_CopyDoubleToUint64(4e-40L) != 0X37C16C2800000000ULL) {
      return "CopyDoubleToUint64 failed";
   }

   if(UsefulBufUtil_CopyUint64ToDouble(0X37C16C2800000000ULL) != 4e-40L) {
      return "CopyUint64ToDouble failed";
   }

   if(UsefulBufUtil_CopyUint32ToFloat(0x47800000) != 65536.0F) {
      return "CopyUint32ToFloat failed";
   }

   return NULL;
}



