/*
 * Copyright (c) 2017 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

    .syntax unified
    .thumb

    .global  __StackTop
    .global  PLAT_Init

    /* entry point of application image */
    .section  .text.PLAT_Start
    .weak  PLAT_Start
    .type  PLAT_Start, %function
PLAT_Start:
    ldr   sp, =__StackTop
    ldr   r0, =PLAT_Init
    bx    r0
    .size  PLAT_Start, .-PLAT_Start
    .end
