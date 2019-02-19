#  t_cose

## Introduction

t_cose is a partial implementation of the COSE standard (RFC 8152).
COSE is quite a large standard so a full implementation of all of it
is a large undertaking.  This implementation is starting out as just
enough for attestation and maybe CWT (Cbor Web Token) which is very
similar.

As it may grow to support more of COSE over time, it is structured
with that in mind.

It is also trying to be portable, the most interesting part of which
is interfacing with the libraries for performing cryptography and
access cryptographic keys.

## Source files
The following files are more or less the public interface.
* t_cose_common.h
* t_cose_sign1.h
* t_cose_sign1_verify.h

The rest of the files are internal source files that callers should
not depend on.

t_cose_defines.h contains contants from RFC 8152 that are
part of the COSE standard. It should never have anything else
in it.

t_cose_util is some utilities and code common to
signing and verification. Both signing and verifcation
depend on it.

t_cose_crypto is the crypto porting layer. Generally
the .h file should not need to change for a new
platform. The .c file will be changed lots for
each new platform.

## Dependency and Portability
t_cose is attempting to be very portable and
have a minmum number of #ifdefs. It is
designed to run 64-bit and 32-bit machines.

It uses a minimum number of standard C
libraries, mostly just <stdint.h>, <string.h> and <stddef.h>.

It uses QCBOR for CBOR encoding and decoding.
QCBOR is very portable.

### Cryptography and Keys
There is a cryptographic adaption layer that
provides the following:
* Impedence match of parameters passed to/from the crypto
* Management of algorithm identifiers
* Management of error codes
* Management of key identifiers

It is framed for the needs of t_cose, and is
not trying to be any kind of general crypto API.

#### Question
Q: How many crypto APIs does it take to screw in a light bulb?
A: One more than it takes to screw in a light bulb.

## Copyright and License

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
