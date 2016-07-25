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
#include "mbed-client-mbedtls/m2mconnectionsecuritypimpl.h"
#include "m2mtimer.h"
#include "m2msecurity.h"
#include <string.h>
#include "m2mconnectionsecuritypimpl_stub.h"

int m2mconnectionsecuritypimpl_stub::int_value;

void m2mconnectionsecuritypimpl_stub::clear()
{
    int_value = -1;
}

M2MConnectionSecurityPimpl::M2MConnectionSecurityPimpl(M2MConnectionSecurity::SecurityMode mode)
{

}

M2MConnectionSecurityPimpl::~M2MConnectionSecurityPimpl(){
}

void M2MConnectionSecurityPimpl::reset(){
}

int M2MConnectionSecurityPimpl::init(const M2MSecurity *security){
    return m2mconnectionsecuritypimpl_stub::int_value;
}

int M2MConnectionSecurityPimpl::connect(M2MConnectionHandler* connHandler){
    return m2mconnectionsecuritypimpl_stub::int_value;
}

int M2MConnectionSecurityPimpl::start_connecting_non_blocking(M2MConnectionHandler* connHandler)
{
    return m2mconnectionsecuritypimpl_stub::int_value;
}

int M2MConnectionSecurityPimpl::continue_connecting()
{
    return m2mconnectionsecuritypimpl_stub::int_value;
}

int M2MConnectionSecurityPimpl::send_message(unsigned char *message, int len){
    return m2mconnectionsecuritypimpl_stub::int_value;
}

int M2MConnectionSecurityPimpl::read(unsigned char* buffer, uint16_t len){
    return m2mconnectionsecuritypimpl_stub::int_value;
}

void M2MConnectionSecurityPimpl::timer_expired(M2MTimerObserver::Type type){
}

void M2MConnectionSecurityPimpl::set_random_number_callback(random_number_cb){
}

void M2MConnectionSecurityPimpl::set_entropy_callback(entropy_cb){
}
