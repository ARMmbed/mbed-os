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

// Count how many args are in a variadic macro. We now use GCC/Clang's extension to
// handle the case where ... expands to nothing. We must add a placeholder arg before
// ##__VA_ARGS__ (its value is totally irrelevant, but it's necessary to preserve
// the shifting offset we want). In addition, we must add 0 as a valid value to be in
// the N position.
#define COUNT_VARARGS(...) _GET_10TH_ARG(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define _GET_OVERRIDE(_1, _2, _3, _4, _5, NAME, ...) NAME

#define _RPC_CALL_0() ()
#define _RPC_CALL_1(_t1) ((_t1)(message->args[0]))
#define _RPC_CALL_2(_t1, _t2) ((_t1)(message->args[0]), (_t2)(message->args[1]))
#define _RPC_CALL_3(_t1, _t2, _t3) ((_t1)(message->args[0]), (_t2)(message->args[1]), (_t3)(message->args[2]))
#define _RPC_CALL_4(_t1, _t2, _t3, _t4) ((_t1)(message->args[0]), (_t2)(message->args[1]), (_t3)(message->args[2]), (_t4)(message->args[3]))
#define _RPC_CALL_5(_t1, _t2, _t3, _t4, _t5) ((_t1)(message->args[0]), (_t2)(message->args[1]), (_t4)(message->args[3]), (_t5)(message->args[4]), (_t2)(message->args[1]))

#define _RPC_DECL_ARGS_0()
#define _RPC_DECL_ARGS_1(_t1) _t1 arg1
#define _RPC_DECL_ARGS_2(_t1, _t2) _t1 arg1, _t2 arg2
#define _RPC_DECL_ARGS_3(_t1, _t2, _t3) _t1 arg1, _t2 arg2, _t3 arg3
#define _RPC_DECL_ARGS_4(_t1, _t2, _t3, _t4) _t1 arg1, _t2 arg2, _t3 arg3, _t4 arg4
#define _RPC_DECL_ARGS_5(_t1, _t2, _t3, _t4, _t5) _t1 arg1, _t2 arg2, _t3 arg3, _t4 arg4, _t5 arg5

#define _RPC_ARGS_0()
#define _RPC_ARGS_1(_t1) , arg1
#define _RPC_ARGS_2(_t1, _t2) , arg1, arg2
#define _RPC_ARGS_3(_t1, _t2, _t3) , arg1, arg2, arg3
#define _RPC_ARGS_4(_t1, _t2, _t3, _t4) , arg1, arg2, arg3, arg4
#define _RPC_ARGS_5(_t1, _t2, _t3, _t4, _t5) , arg1, arg2, arg3, arg4, arg5


#define RPC_DECL_ARGS(...) \
    _GET_OVERRIDE(__VA_ARGS__, _RPC_DECL_ARGS_5, _RPC_DECL_ARGS_4, _RPC_DECL_ARGS_3, _RPC_DECL_ARGS_2, _RPC_DECL_ARGS_1, _RPC_DECL_ARGS_0)(__VA_ARGS__)

#define RPC_LIST_ARGS(...) \
    _GET_OVERRIDE(__VA_ARGS__, _RPC_ARGS_5, _RPC_ARGS_4, _RPC_ARGS_3, _RPC_ARGS_2, _RPC_ARGS_1, _RPC_ARGS_0)(__VA_ARGS__)

#define RPC_CALL_ARGS(...) \
    _GET_OVERRIDE(__VA_ARGS__, _RPC_CALL_5, _RPC_CALL_4, _RPC_CALL_3, _RPC_CALL_2, _RPC_CALL_1, _RPC_CALL_0)(__VA_ARGS__)


#define _RPC_VOID(arg)
#define _RPC_NON_VOID(arg)  arg
#define NAME(_name_)        _name_
#define TYPE(_type_)        _RPC_NON_VOID, _type_
#define VOID                _RPC_VOID, void
#define ARGS(...)           __VA_ARGS__
#define RPC_FUNCTION(...)   RPC_FUNCTION_(__VA_ARGS__)

#define RPC_GEN_INTERFACE_IDS       1
#define RPC_GEN_INTERFACE_IDS_INIT  2
#define RPC_GEN_INTERFACE           3
#define RPC_GEN_IMPLEMENTATION      4
#define RPC_GEN_IMPL_INIT           5

#endif // RPC_DEFS_H

#undef __RPC_DEFS_H_BODY_START__
#define __RPC_DEFS_H_BODY_START__

#if RPC_GEN == RPC_GEN_INTERFACE_IDS
/** Generating interface IDs.
 * This will generate a set of variable named RPC_ID_<api name> with unique integer IDs.
 */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_(T, _type_, _name_, ...)    static uint32_t RPC_ID_##_name_ = 0;

#elif RPC_GEN == RPC_GEN_INTERFACE_IDS_INIT
/** Initialization of interface IDs.
 * This will generate a set of variable named RPC_ID_<api name> with unique integer IDs.
 */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_(T, _type_, _name_, ...)\
    RPC_ID_##_name_ = rpc_counter++;

#elif RPC_GEN == RPC_GEN_INTERFACE
/** Generating interface functions on caller core.
 * This will generate a set of functions with the defined API that will internally call RPC pipe.
 */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_(T, _type_, _name_, ... )\
_type_ _name_(RPC_DECL_ARGS(__VA_ARGS__)) {\
    T(return ) (_type_)ipcrpc_call(RPC_ID_##_name_, COUNT_VARARGS(__VA_ARGS__) RPC_LIST_ARGS(__VA_ARGS__));\
}

#elif RPC_GEN == RPC_GEN_IMPLEMENTATION
/** Generating implementation interface on a target (calee) core.
 * This will generate a set of functions named RPC_<api_name> which internally will decode
 * RPC message and will then call appropriate API function.
 */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_( T, _type_, _name_, ...)\
void RPC_##_name_(uint32_t *msg_ptr) {\
    IpcRpcMessage *message = (IpcRpcMessage *)msg_ptr;\
    T(message->result = ) (_type_) _name_ RPC_CALL_ARGS(__VA_ARGS__);\
}

#elif RPC_GEN == RPC_GEN_INITIALIZATION
/** This will generate an initialization code on target(calee) core that will register
 * interface functions generated in the previous step as RPC message receivers.
 */
#undef RPC_FUNCTION_
#define RPC_FUNCTION_( T, _type_, _name_, ...)\
    Cy_IPC_Pipe_RegisterCallback(CY_IPC_EP_RPCPIPE_ADDR, RPC_##_name_, rpc_counter++);

#endif // RPC_GEN

#undef __RPC_DEFS_H_BODY_END__
#define __RPC_DEFS_H_BODY_END__

