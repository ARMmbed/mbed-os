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

#ifndef RPC_DEFS_H
#define RPC_DEFS_H

/*
 * This file defines set of helper macros used in implementation of the RPC
 * (Remote Procedure Call) mechanism to allow M4 core (caller) to call functions
 * that will execute on M0 core (callee), mainly used for shared hardware resource
 * management.
 * The RPC mechanism is build around PSoC6 IPC messaging interface.
 * When a function  has to be called remotely, its ID as well as arguments
 * are packed into the message buffer and send out to M0 core.
 * M0 core receives the message, unpacks arguments from the buffer and calls
 * the proper function depending on the ID provided in the message. Then it puts
 * the function result back into the buffer and releases the message back to
 * M4 core.
 * Additional assumptions:
 *  - message buffers reside in the M4 core address space,
 *  - M0 core has read/write access at least to the message buffers and to other
 *    data referenced when function arguments are pointers.
 *
 * The helper macros are used to automatically generate proper function wrappers for
 * required APIs.
 * All remotely callable functions have to be declared in the rpc_api.h header using
 * a set of macros (see description in the rpc_api.h for details).
 * Wrapper generation will process in a few phases, controlled by the value of
 * RPC_GEN macro. Each generation phase is invoked as follows:
 *
 * #define RPC_GEN  <phase>
 * #include "rpc_api.h"
 * #undef RPC_GEN
 *
 * Macros declaring RPC APIs in the rpc_api.h will be expanded to various C code, depending
 * on the generation phase. See description of the generation phases below.
 */


/* Macros, that simply return it's n-th argument */
#define _GET_10TH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define _GET_9TH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define _GET_8TH_ARG(_1, _2, _3, _4, _5, _6, _7, N, ...) N
#define _GET_7TH_ARG(_1, _2, _3, _4, _5, _6, N, ...) N
#define _GET_6TH_ARG(_1, _2, _3, _4, _5, N, ...) N
#define _GET_5TH_ARG(_1, _2, _3, _4, N, ...) N
#define _GET_4TH_ARG(_1, _2, _3, N, ...) N
#define _GET_3TH_ARG(_1, _2, N, ...) N
#define _GET_2ND_ARG(_1, N, ...) N
#define _GET_1ST_ARG(N, ...) N

/* Count how many args are in a variadic macro. We now use GCC/Clang's extension to
 * handle the case where ... expands to nothing. We must add a placeholder arg before
 * ##__VA_ARGS__ (its value is totally irrelevant, but it's necessary to preserve
 * the shifting offset we want). In addition, we must add 0 as a valid value to be in
 * the N position.
 * Good for 0 to 9 arguments.
 */
#define COUNT_VARARGS(...) _GET_10TH_ARG(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


/* Non-variadic macros generating (RPC) function call arguments (on a callee MCU).
 * Macro arguments are argument types for the function to be called,
 * actual argument values are taken from RPC message buffer.
 * Variants for functions using 0 - 5 arguments.
 */
#define _RPC_CALL_0() ()
#define _RPC_CALL_1(_t1) ((_t1)(message->args[0]))
#define _RPC_CALL_2(_t1, _t2) ((_t1)(message->args[0]), (_t2)(message->args[1]))
#define _RPC_CALL_3(_t1, _t2, _t3) ((_t1)(message->args[0]), (_t2)(message->args[1]), (_t3)(message->args[2]))
#define _RPC_CALL_4(_t1, _t2, _t3, _t4) ((_t1)(message->args[0]), (_t2)(message->args[1]), (_t3)(message->args[2]), (_t4)(message->args[3]))
#define _RPC_CALL_5(_t1, _t2, _t3, _t4, _t5) ((_t1)(message->args[0]), (_t2)(message->args[1]), (_t4)(message->args[3]), (_t5)(message->args[4]), (_t2)(message->args[1]))

/* Non-variadic macros generating (RPC) function argument declaration (on a caller MCU).
 * Macro arguments are argument types for the function to be called.
 * Expands to predefined argument names (arg1, arg2 and so on).
 * Variants for functions using 0 - 5 arguments.
 */
#define _RPC_DECL_ARGS_0()
#define _RPC_DECL_ARGS_1(_t1) _t1 arg1
#define _RPC_DECL_ARGS_2(_t1, _t2) _t1 arg1, _t2 arg2
#define _RPC_DECL_ARGS_3(_t1, _t2, _t3) _t1 arg1, _t2 arg2, _t3 arg3
#define _RPC_DECL_ARGS_4(_t1, _t2, _t3, _t4) _t1 arg1, _t2 arg2, _t3 arg3, _t4 arg4
#define _RPC_DECL_ARGS_5(_t1, _t2, _t3, _t4, _t5) _t1 arg1, _t2 arg2, _t3 arg3, _t4 arg4, _t5 arg5

/* Non-variadic macros generating (RPC) function arguments (on a caller MCU).
 * Macro arguments are argument types for the function to be called.
 * Expands to predefined argument names (arg1, arg2 and so on).
 * Variants for functions using 0 - 5 arguments.
 */
#define _RPC_ARGS_0()
#define _RPC_ARGS_1(_t1) , arg1
#define _RPC_ARGS_2(_t1, _t2) , arg1, arg2
#define _RPC_ARGS_3(_t1, _t2, _t3) , arg1, arg2, arg3
#define _RPC_ARGS_4(_t1, _t2, _t3, _t4) , arg1, arg2, arg3, arg4
#define _RPC_ARGS_5(_t1, _t2, _t3, _t4, _t5) , arg1, arg2, arg3, arg4, arg5


/* Variadic macro generating (RPC) function argument declaration (on a caller MCU).
 * Macro arguments are argument types for the function to be called;
 * expands to  the non-variadic variant for the proper number of arguments.
 */
#define RPC_DECL_ARGS(...) \
    _GET_6TH_ARG(__VA_ARGS__, _RPC_DECL_ARGS_5, _RPC_DECL_ARGS_4, _RPC_DECL_ARGS_3, _RPC_DECL_ARGS_2, _RPC_DECL_ARGS_1, _RPC_DECL_ARGS_0)(__VA_ARGS__)


/* Variadic macro generating (RPC) function arguments (on a caller MCU).
 * Macro arguments are argument types for the function to be called;
 * expands to  the non-variadic variant for the proper number of arguments.
 */
#define RPC_LIST_ARGS(...) \
    _GET_6TH_ARG(__VA_ARGS__, _RPC_ARGS_5, _RPC_ARGS_4, _RPC_ARGS_3, _RPC_ARGS_2, _RPC_ARGS_1, _RPC_ARGS_0)(__VA_ARGS__)

/* Variadic macro generating (RPC) function call arguments (on a callee MCU).
 * Macro arguments are argument types for the function to be called;
 * expands to the non-variadic variant for the proper number of arguments.
  */
#define RPC_CALL_ARGS(...) \
    _GET_6TH_ARG(__VA_ARGS__, _RPC_CALL_5, _RPC_CALL_4, _RPC_CALL_3, _RPC_CALL_2, _RPC_CALL_1, _RPC_CALL_0)(__VA_ARGS__)


#define _RPC_VOID(arg)
#define _RPC_NON_VOID(arg)  arg
#define NAME(_name_)        _name_
#define TYPE(_type_)        _RPC_NON_VOID, _type_
#define VOID                _RPC_VOID, void
#define ARGS(...)           __VA_ARGS__
#define RPC_FUNCTION(...)   RPC_FUNCTION_(__VA_ARGS__)

/*
 * Wrapper generation phases.
 * ========================================================================================
 * Generation of the RPC wrappers consists of a few phases, both on caller and callee MCUs.
 * Each phase is invoked through re-definition of RPC_GEN macro (holding current phase id)
 * and then inclusion of the rpc_api.h header, which also includes this file. The phases
 * (and their applicability) are in sequence:
 *
 * RPC_GEN_INTERFACE_IDS (caller, callee)
 *      This will generate a set of variable declarations with names RPC_ID_<api name>,
 *      where <api_name> is a name of the RPCed function. Variables will be initialized
 *      in the next phase with unique integer values to be use in RPC messages to identify
 *      function to be called.
 *
 * RPC_GEN_INTERFACE_IDS_INIT (caller, callee)
 *      This will generate initialization code for the ID variables (assigning unique identifiers).
 *      The assumption is that the generation is invoked inside a function containing
 *      a local variable caller rpc_counter and initialized to 0. Example:
 *
 *      void ipcrpc_init2(void)
 *      {
 *          uint32_t rpc_counter = 0;
 *      #define RPC_GEN     RPC_GEN_INTERFACE_IDS_INIT
 *      #include "rpc_api.h"
 *      #undef RPC_GEN
 *      }
 *
 * RPC_GEN_INTERFACE (caller only)
 *      This will generate a set of functions with the defined API that will internally call
 *      RPC pipe message sending variadic function ipcrpc_call(...).
 *
 * RPC_GEN_IMPLEMENTATION (callee only)
 *      This will generate a set of functions named RPC_<api_name> which internally will decode
 *      an RPC message and will then call appropriate API function.
 *
 * RPC_GEN_IMPL_INITIALIZATION (callee only)
 *      This will generate an initialization code on a target(calee) core that will register
 *      interface functions generated in the previous step as RPC message receivers.
 *      This phase has to be invoked within a function with the same assumptions as for
 *      RPC_GEN_INTERFACE_IDS_INIT
 */

#define RPC_GEN_INTERFACE_IDS       1
#define RPC_GEN_INTERFACE_IDS_INIT  2
#define RPC_GEN_INTERFACE           3
#define RPC_GEN_IMPLEMENTATION      4
#define RPC_GEN_INITIALIZATION      5


#endif // RPC_DEFS_H

#undef __RPC_DEFS_H_BODY_START__
#define __RPC_DEFS_H_BODY_START__

#if RPC_GEN == RPC_GEN_INTERFACE_IDS
/* Generating interface IDs. */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_(T, _type_, _name_, ...)    static uint32_t RPC_ID_##_name_ = 0;

#elif RPC_GEN == RPC_GEN_INTERFACE_IDS_INIT
/* Initialization of interface IDs. */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_(T, _type_, _name_, ...)\
    RPC_ID_##_name_ = rpc_counter++;

#elif RPC_GEN == RPC_GEN_INTERFACE
/* Generating interface functions on caller core. */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_(T, _type_, _name_, ... )\
_type_ _name_(RPC_DECL_ARGS(__VA_ARGS__)) {\
    T(return ) (_type_)ipcrpc_call(RPC_ID_##_name_, COUNT_VARARGS(__VA_ARGS__) RPC_LIST_ARGS(__VA_ARGS__));\
}

#elif RPC_GEN == RPC_GEN_IMPLEMENTATION
/* Generating implementation interface on a target (calee) core. */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_( T, _type_, _name_, ...)\
void RPC_##_name_(uint32_t *msg_ptr) {\
    IpcRpcMessage *message = (IpcRpcMessage *)msg_ptr;\
    T(message->result = ) (_type_) _name_ RPC_CALL_ARGS(__VA_ARGS__);\
}

#elif RPC_GEN == RPC_GEN_INITIALIZATION
/* This will generate an initialization code on target(calee) core. */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_( T, _type_, _name_, ...)\
    Cy_IPC_Pipe_RegisterCallback(CY_IPC_EP_RPCPIPE_ADDR, RPC_##_name_, rpc_counter++);

#endif // RPC_GEN

#undef __RPC_DEFS_H_BODY_END__
#define __RPC_DEFS_H_BODY_END__

