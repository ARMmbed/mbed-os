micro-ecc
==========

A small and fast ECDH and ECDSA implementation for 8-bit, 32-bit, and 64-bit processors.

The old version of micro-ecc can be found in the "old" branch.

Features
--------

 * Resistant to known side-channel attacks.
 * Written in C, with optional GCC inline assembly for AVR, ARM and Thumb platforms.
 * Supports 8, 32, and 64-bit architectures.
 * Small code size.
 * No dynamic memory allocation.
 * Support for 4 standard curves: secp160r1, secp192r1, secp256r1, and secp256k1.
 * BSD 2-clause license.

Usage Notes
-----------
### Point Representation ###
Compressed points are represented in the standard format as defined in http://www.secg.org/collateral/sec1_final.pdf; uncompressed points are represented in standard format, but without the `0x04` prefix. `uECC_make_key()`, `uECC_shared_secret()`, `uECC_sign()`, and `uECC_verify()` only handle uncompressed points; you can use `uECC_compress()` and `uECC_decompress()` to convert between compressed and uncompressed point representations.

Private keys are represented in the standard format.

### Using the Code ###

I recommend just copying (or symlink) uECC.h, uECC.c, and the appropriate asm\_&lt;arch&gt;\_.inc (if any) into your project. Then just `#include "uECC.h"` to use the micro-ecc functions.

For use with Arduino, you can just create a symlink to the `uECC` directory in your Arduino `libraries` directory. You can then use uECC just like any other Arduino library (uECC should show up in the **Sketch**=>**Import Library** submenu).

See uECC.h for documentation for each function.

### Compilation Notes ###

 * Should compile with any C/C++ compiler that supports stdint.h (this includes Visual Studio 2013).
 * If you want to change the defaults for `uECC_CURVE` and `uECC_ASM`, you must change them in your Makefile or similar so that uECC.c is compiled with the desired values (ie, compile uECC.c with `-DuECC_CURVE=uECC_secp256r1` or whatever).
 * When compiling for a Thumb-1 platform with inline assembly enabled (ie, `uECC_ASM` is defined to `uECC_asm_small` or `uECC_asm_fast`), you must use the `-fomit-frame-pointer` GCC option (this is enabled by default when compiling with `-O1` or higher).
 * When compiling for an ARM/Thumb-2 platform with fast inline assembly enabled (ie, `uECC_ASM` is defined to `uECC_asm_fast`), you must use the `-fomit-frame-pointer` GCC option (this is enabled by default when compiling with `-O1` or higher).
 * When compiling for AVR with inline assembly enabled, you must have optimizations enabled (compile with `-O1` or higher).
 * When building for Windows, you will need to link in the `advapi32.lib` system library.

ARM Performance
---------------

All tests were built using gcc 4.8.2 with `-O3`, and were run on a Raspberry Pi B+. `uECC_ASM` was defined to `uECC_asm_fast` and `ECC_SQUARE_FUNC` was defined to `1` in all cases. All times are in milliseconds.

<table>
	<tr>
		<th></th>
		<th>secp160r1</th>
		<th>secp192r1</th>
		<th>secp256r1</th>
		<th>secp256k1</th>
	</tr>
	<tr>
		<td><em>ECDH:</em></td>
		<td>2.3</td>
		<td>2.7</td>
		<td>7.9</td>
		<td>6.5</td>
	</tr>
	<tr>
		<td><em>ECDSA sign:</em></td>
		<td>2.8</td>
		<td>3.1</td>
		<td>8.6</td>
		<td>7.2</td>
	</tr>
	<tr>
		<td><em>ECDSA verify:</em></td>
		<td>2.7</td>
		<td>3.2</td>
		<td>9.2</td>
		<td>7.0</td>
	</tr>
</table>

AVR Performance
---------------

All tests were built using avr-gcc 4.8.1 with `-Os`, and were run on a 16 MHz ATmega256RFR2. Code size refers to the space used by micro-ecc code and data.

#### ECDH (fast) ####

In these tests, `uECC_ASM` was defined to `uECC_asm_fast` and `ECC_SQUARE_FUNC` was defined to `1` in all cases.

<table>
	<tr>
		<th></th>
		<th>secp160r1</th>
		<th>secp192r1</th>
		<th>secp256r1</th>
		<th>secp256k1</th>
	</tr>
	<tr>
		<td><em>ECDH time (ms):</em></td>
		<td>470</td>
		<td>810</td>
		<td>2220</td>
		<td>1615</td>
	</tr>
	<tr>
		<td><em>Code size (bytes):</em></td>
		<td>10768</td>
		<td>13112</td>
		<td>20886</td>
		<td>21126</td>
	</tr>
</table>

#### ECDH (small) ####

In these tests, `uECC_ASM` was defined to `uECC_asm_small` and `ECC_SQUARE_FUNC` was defined to `0` in all cases.

<table>
	<tr>
		<th></th>
		<th>secp160r1</th>
		<th>secp192r1</th>
		<th>secp256r1</th>
		<th>secp256k1</th>
	</tr>
	<tr>
		<td><em>ECDH time (ms):</em></td>
		<td>1250</td>
		<td>1810</td>
		<td>4790</td>
		<td>4700</td>
	</tr>
	<tr>
		<td><em>Code size (bytes):</em></td>
		<td>3244</td>
		<td>3400</td>
		<td>5274</td>
		<td>3426</td>
	</tr>
</table>

#### ECDSA (fast) ####

In these tests, `uECC_ASM` was defined to `uECC_asm_fast` and `ECC_SQUARE_FUNC` was defined to `1` in all cases.

<table>
	<tr>
		<th></th>
		<th>secp160r1</th>
		<th>secp192r1</th>
		<th>secp256r1</th>
		<th>secp256k1</th>
	</tr>
	<tr>
		<td><em>ECDSA sign time (ms):</em></td>
		<td>555</td>
		<td>902</td>
		<td>2386</td>
		<td>1773</td>
	</tr>
	<tr>
		<td><em>ECDSA verify time (ms):</em></td>
		<td>590</td>
		<td>990</td>
		<td>2650</td>
		<td>1800</td>
	</tr>
	<tr>
		<td><em>Code size (bytes):</em></td>
		<td>13246</td>
		<td>14798</td>
		<td>22594</td>
		<td>22826</td>
	</tr>
</table>

#### ECDSA (small) ####

In these tests, `uECC_ASM` was defined to `uECC_asm_small` and `ECC_SQUARE_FUNC` was defined to `0` in all cases.

<table>
	<tr>
		<th></th>
		<th>secp160r1</th>
		<th>secp192r1</th>
		<th>secp256r1</th>
		<th>secp256k1</th>
	</tr>
	<tr>
		<td><em>ECDSA sign time (ms):</em></td>
		<td>1359</td>
		<td>1931</td>
		<td>4998</td>
		<td>4904</td>
	</tr>
	<tr>
		<td><em>ECDSA verify time (ms):</em></td>
		<td>1515</td>
		<td>2160</td>
		<td>5700</td>
		<td>5220</td>
	</tr>
	<tr>
		<td><em>Code size (bytes):</em></td>
		<td>5690</td>
		<td>5054</td>
		<td>6980</td>
		<td>5080</td>
	</tr>
</table>
