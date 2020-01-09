/**************************************************
 *
 * Part two of the system initialization code, contains C-level
 * initialization, thumb-2 only variant.
 *
 * $Revision: 59783 $
 *
 **************************************************/
/* Copyright 2008-2017, IAR Systems AB.
   This source code is the property of IAR Systems. The source code may only
   be used together with the IAR Embedded Workbench. Redistribution and use
   in source and binary forms, with or without modification, is permitted
   provided that the following conditions are met:
   - Redistributions of source code, in whole or in part, must retain the
     above copyright notice, this list of conditions and the disclaimer below.
   - IAR Systems name may not be used to endorse or promote products
     derived from this software without specific prior written permission.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

; --------------------------------------------------
; Module ?cmain, C-level initialization.
;


        SECTION SHT$$PREINIT_ARRAY:CONST:NOROOT(2)
        SECTION SHT$$INIT_ARRAY:CONST:NOROOT(2)

        SECTION .text:CODE:NOROOT(2)

        PUBLIC  __cmain
        ;; Keep ?main for legacy reasons, it is accessed in countless instances of cstartup.s around the world...
        PUBLIC  ?main
        EXTWEAK __iar_data_init3
        EXTWEAK __iar_argc_argv
        EXTERN  __low_level_init
        EXTERN  __call_ctors
        EXTERN  main
        EXTERN  exit
        EXTERN  __iar_dynamic_initialization
        EXTERN mbed_sdk_init
        EXTWEAK __mbed_init
        EXTERN mbed_main
        EXTERN SystemInit
        
        THUMB
__cmain:
?main:

; Initialize segments.
; __segment_init and __low_level_init are assumed to use the same
; instruction set and to be reachable by BL from the ICODE segment
; (it is safest to link them in segment ICODE).

          FUNCALL __cmain, __low_level_init
        bl      __low_level_init
        cmp     r0,#0
        beq     ?l1
          FUNCALL __cmain, __iar_data_init3
        bl      __iar_data_init3
        MOVS    r0,#0             ;  No parameters
        FUNCALL __cmain, mbed_sdk_init
        BL      mbed_sdk_init
        MOVS    r0,#0             ;  No parameters        
        FUNCALL __cmain, __mbed_init
        BL      __mbed_init
        MOVS    r0,#0             ;  No parameters
          FUNCALL __cmain, __iar_dynamic_initialization
        BL      __iar_dynamic_initialization   ; C++ dynamic initialization  

?l1:
        REQUIRE ?l3

        SECTION .text:CODE:NOROOT(2)

        PUBLIC  _main
        PUBLIC _call_main
        THUMB

__iar_init$$done:                 ; Copy initialization is done

?l3:
_call_main:
        MOVS    r0,#0             ;  No parameters  
          FUNCALL __cmain, __iar_argc_argv
        BL      __iar_argc_argv   ; Maybe setup command line

        MOVS    r0,#0             ;  No parameters  
          FUNCALL __cmain, mbed_main
        BL      mbed_main

          FUNCALL __cmain, main
        BL      main
_main:
          FUNCALL __cmain, exit
        BL      exit

        END
