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

#include <string.h>
#include "mbed-client/m2mconnectionhandler.h"
#include "mbed-client/m2mconnectionsecurity.h"
#include "mbed-client/m2mtimer.h"
#include "mbed-client/m2msecurity.h"
#include "mbed-client-mbedtls/m2mconnectionsecuritypimpl.h"

M2MConnectionSecurity::M2MConnectionSecurity(SecurityMode mode)
{
    _private_impl = new M2MConnectionSecurityPimpl(mode);
}

M2MConnectionSecurity::~M2MConnectionSecurity(){
    delete _private_impl;
}

void M2MConnectionSecurity::reset(){
    _private_impl->reset();
}

int M2MConnectionSecurity::init(const M2MSecurity *security){
    return _private_impl->init(security);
}

int M2MConnectionSecurity::start_connecting_non_blocking(M2MConnectionHandler* connHandler)
{
    return _private_impl->start_connecting_non_blocking(connHandler);
}

int M2MConnectionSecurity::continue_connecting()
{
    return _private_impl->continue_connecting();
}

int M2MConnectionSecurity::connect(M2MConnectionHandler* connHandler){
    return _private_impl->connect(connHandler);
}

int M2MConnectionSecurity::send_message(unsigned char *message, int len){
    return _private_impl->send_message(message, len);
}

int M2MConnectionSecurity::read(unsigned char* buffer, uint16_t len){
    return _private_impl->read(buffer, len);
}

void M2MConnectionSecurity::set_random_number_callback(random_number_cb callback)
{
    _private_impl->set_random_number_callback(callback);
}

void M2MConnectionSecurity::set_entropy_callback(entropy_cb callback)
{
    _private_impl->set_entropy_callback(callback);
}
