# QCBOR

QCBOR encodes and decodes [RFC 7049](https://tools.ietf.org/html/rfc7049) CBOR.

## Characteristics

**Implemented in C with minimal dependency** – Only dependencies are
  C99, <stdint.h>, <stddef.h>, <stdbool.h> and <string.h> making it
  highly portable. There are no #ifdefs to be configured at all.

**Focused on C / native data representation** – Simpler code because
  there is no support for encoding/decoding to/from JSON, pretty
  printing, diagnostic notation... Only encoding from native C
  representations and decoding to native C representations is supported.

**Small simple memory model** – Malloc is not needed. The encode
  context is 136 bytes, decode context is 104 bytes and the
  description of decoded data item is 56 bytes. Stack use is light and
  there is no recursion. The caller supplies the memory to hold the
  encoded CBOR and encode/decode contexts so caller has full control
  of memory usage making it good for embedded implementations that
  have to run in small fixed memory.

**Supports nearly all of RFC 7049** – Only minor, corner-case parts of
  RFC 7049 are not directly supported (canonicalization, decimal
  fractions, big floats). Decoding indefinite length strings is supported,
  but requires a string allocator (see documentation). Encoding indefinite
  length strings is not supported, but is also not necessary or
  preferred.

**Extensible and general** – Provides a way to handle data types that
  are not directly supported.

**Secure coding style** – Uses a construct called UsefulBuf as a
  discipline for very safe coding the handling of binary data.

**Small code size** – When optimized for size using the compiler -Os
  option, x86 code is about 4KB (~1.1KB encode, ~2.5KB decode,
  ~0.4KB common). Other decoders may be smaller, but they may
  also do less for you, so overall size of the implementation may
  be larger. For example, QCBOR internally tracks error status
  so you don't have to check a return code on every operation.

**Clear documented public interface** – The public interface is
  separated from the implementation. It can be put to use without
  reading the source.

**Comprehensive test suite** – Easy to verify on a new platform
  or OS with the test suite. The test suite dependencies are also
  minimal, only additionally requiring <math.h> for floating point
  tests.

## Code Status

QCBOR was originally developed by Qualcomm. It was [open sourced
through CAF](https://source.codeaurora.org/quic/QCBOR/QCBOR/) with a
permissive Linux license, September 2018 (thanks Qualcomm!).

This code in [Laurence's
GitHub](https://github.com/laurencelundblade/QCBOR) has diverged from
the CAF source with some small simplifications and tidying up.

From Nov 3, 2018, the interface and code are fairly stable. Large
changes are not planned or expected, particularly in the
interface. The test coverage is pretty good.

## Building

There is a simple makefile for the UNIX style command line binary that
compiles everything to run the tests.

These seven files, the contents of the src and inc directories, make
up the entire implementation.

* inc
   * UsefulBuf.h
   * qcbor.h
* src
   * UsefulBuf.c
   * qcbor_encode.c
   * qcbor_decode.c
   * ieee754.h
   * ieee754.c

For most use cases you should just be able to add them to your
project. Hopefully the easy portability of this implementation makes
this work straight away, whatever your development environment is.

The files ieee754.c and ieee754.h are support for half-precision
floating point. The encoding side of the floating point functionality
is about 500 bytes. If it is never called because no floating point
numbers are ever encoded, all 500 bytes will be dead stripped and not
impact code size. The decoding side is about 150 bytes of object
code. It is never dead stripped because it directly referenced by the
core decoder, however it doesn't add very much to the size.

The test directory includes some tests that are nearly as portable as
the main implementation.  If your development environment doesn't
support UNIX style command line and make, you should be able to make a
simple project and add the test files to it.  Then just call
RunTests() to invoke them all.


## Changes from CAF Version
* Float support is restored
* Minimal length float encoding is added
* indefinite length arrays/maps are supported
* indefinite length strings are supported
* Tag decoding is changed; unlimited number of tags supported, any tag
value supported, tag utility function for easier tag checking
* Addition functions in UsefulBuf
* QCBOREncode_Init takes a UsefulBuf instead of a pointer and size
* QCBOREncode_Finish takes a UsefulBufC and EncodedCBOR is remove
* bstr wrapping of arrays/maps is replaced with OpenBstrwrap
* AddRaw renamed to AddEncoded and can now only add whole arrays or maps,
not partial maps and arrays (simplification; was a dangerous feature)
* Finish cannot be called repeatedly on a partial decode (some tests used
this, but it is not really a good thing to use in the first place)
* UsefulOutBuf_OutUBuf changed to work differently
* UsefulOutBuf_Init works differently
* The "_3" functions are replaced with a small number of simpler functions
* There is a new AddTag functon instead of the "_3" functions, making
the interface simpler and saving some code
* QCBOREncode_AddRawSimple_2 is removed (the macros that referenced
still exist and work the same)

## Credits
* Ganesh Kanike for porting to QSEE
* Mark Bapst for sponsorship and release as open source by Qualcomm
* Sachin Sharma for release through CAF
* Tamas Ban for porting to TF-M and 32-bit ARM

## Copyright and License

QCBOR is available under what is essentially the 3-Clause BSD License.

Files created inside Qualcomm and open-sourced through CAF (The Code
Aurora Forum) have a slightly modified 3-Clause BSD License. The
modification additionally disclaims NON-INFRINGEMENT.

Files created after release to CAF use the standard 3-Clause BSD
License with no modification. These files have the SPDX license
identifier, "SPDX-License-Identifier: BSD-3-Clause" in them.

### BSD-3-Clause license

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

### Copyright for this README

Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.



