/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

/* This file contains declarations of all the functions executed on CM0+ core
 * that need to be callable from CM4 core via internal RPC mechanism.
 *
 * Functions are declared using RPC_FUNCTION( _type_, _name_, _args_) macro,
 * where it's arguments are also created using support macros:
 *   _type_ is a function type declared as either TYPE(<actual_type>) or VOID,
 *   _name_ is a function name declared using macro NAME(<function name>),
 *          function must use "C" type interfacing (ABI),
 *   _args_ function arguments declared using macro ARGS(<list of types>),
 *          where <list of types> is a list of type definitions (as in function
 *          prototype).
 *
 * Proper RPC interfacing between processors is automatically generated based
 * on the content of this file.
 */

/* Notice! This file explicitly avoids using re-include suppress schema. */

#include "psoc6_utils.h"
#include "rpc_defs.h"

#undef __RPC_API_H_BODY_START__
#define __RPC_API_H_BODY_START__

#if !PSOC6_DYNSRM_DISABLE

RPC_FUNCTION(TYPE( uint32_t ),
             NAME( cy_clk_allocate_divider ),
             ARGS( cy_en_divider_types_t /* div_type */))

RPC_FUNCTION(TYPE( uint32_t ),
             NAME( cy_clk_reserve_divider ),
             ARGS( cy_en_divider_types_t /* div_type */, uint32_t /* div_num */))

RPC_FUNCTION(VOID,
             NAME( cy_clk_free_divider ),
             ARGS( cy_en_divider_types_t /* div_type */, uint32_t /* div_num */))

RPC_FUNCTION(TYPE( int ),
             NAME( cy_reserve_scb ),
             ARGS( uint32_t /* scb_num */))

RPC_FUNCTION(VOID,
             NAME( cy_free_scb ),
             ARGS( uint32_t /* scb_num */))

RPC_FUNCTION(TYPE( int ),
             NAME( cy_reserve_tcpwm ),
             ARGS( uint32_t /* tcpwm_num */))

RPC_FUNCTION(VOID,
             NAME( cy_free_tcpwm ),
             ARGS( uint32_t /* tcpwm_num */))

RPC_FUNCTION(TYPE( int ),
             NAME( cy_reserve_io_pin ),
             ARGS( PinName /* pin */))

RPC_FUNCTION(VOID,
             NAME( cy_free_io_pin ),
             ARGS( PinName /* pin */))

RPC_FUNCTION(VOID,
             NAME( cy_get_bd_mac_address ),
             ARGS( uint8_t* /* buffer */ ))

#endif // !PSOC6_DYNSRM_DISABLE

#undef __RPC_API_H_BODY_END__
#define __RPC_API_H_BODY_END__
