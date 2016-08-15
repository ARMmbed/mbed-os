/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "m2mconnectionhandler.h"
#include "m2mconnectionsecurity.h"
#include "m2mtimer.h"
#include "m2msecurity.h"
#include <string.h>
#include "m2mconnectionsecurity_stub.h"

bool m2mconnectionsecurityimpl_stub::use_inc_int;
int m2mconnectionsecurityimpl_stub::inc_int_value;
int m2mconnectionsecurityimpl_stub::int_value;

void m2mconnectionsecurityimpl_stub::clear()
{
    int_value = -1;
    use_inc_int = false;
    inc_int_value = 0;
}

M2MConnectionSecurity::M2MConnectionSecurity(M2MConnectionSecurity::SecurityMode mode)
{

}

M2MConnectionSecurity::~M2MConnectionSecurity(){
}

void M2MConnectionSecurity::reset(){
}

int M2MConnectionSecurity::init(const M2MSecurity *security){
    if(m2mconnectionsecurityimpl_stub::use_inc_int){
        return m2mconnectionsecurityimpl_stub::inc_int_value++;
    }
    return m2mconnectionsecurityimpl_stub::int_value;
}

int M2MConnectionSecurity::connect(M2MConnectionHandler* connHandler){
    if(m2mconnectionsecurityimpl_stub::use_inc_int){
        return m2mconnectionsecurityimpl_stub::inc_int_value++;
    }
    return m2mconnectionsecurityimpl_stub::int_value;
}

int M2MConnectionSecurity::start_connecting_non_blocking(M2MConnectionHandler* connHandler)
{
    if(m2mconnectionsecurityimpl_stub::use_inc_int){
        return m2mconnectionsecurityimpl_stub::inc_int_value++;
    }
    return m2mconnectionsecurityimpl_stub::int_value;
}

int M2MConnectionSecurity::continue_connecting()
{
    if(m2mconnectionsecurityimpl_stub::use_inc_int){
        return m2mconnectionsecurityimpl_stub::inc_int_value++;
    }
    return m2mconnectionsecurityimpl_stub::int_value;
}

int M2MConnectionSecurity::send_message(unsigned char *message, int len){
    if(m2mconnectionsecurityimpl_stub::use_inc_int){
        return m2mconnectionsecurityimpl_stub::inc_int_value++;
    }
    return m2mconnectionsecurityimpl_stub::int_value;
}

int M2MConnectionSecurity::read(unsigned char* buffer, uint16_t len){
    if(m2mconnectionsecurityimpl_stub::use_inc_int){
        return m2mconnectionsecurityimpl_stub::inc_int_value++;
    }
    return m2mconnectionsecurityimpl_stub::int_value;
}
