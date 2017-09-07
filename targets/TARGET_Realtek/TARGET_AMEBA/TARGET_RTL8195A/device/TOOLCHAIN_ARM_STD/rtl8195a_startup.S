;
; Copyright (c) 2017 Realtek Semiconductor Corp.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

            PRESERVE8
            THUMB

            AREA    |i.PLAT_Start|, CODE, READONLY

PLAT_Start  PROC

            EXPORT  PLAT_Start
    IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|
    IMPORT  PLAT_Init
            LDR     SP, =|Image$$ARM_LIB_STACK$$ZI$$Limit|
            LDR     R0, =PLAT_Init
            BX      R0
            ENDP
            ALIGN
            END
