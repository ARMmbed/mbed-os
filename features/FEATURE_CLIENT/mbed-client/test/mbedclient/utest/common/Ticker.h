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
#ifndef TICKER_H
#define TICKER_H

namespace mbed
{
typedef uint64_t timestamp_t;

    class Ticker{
    public :
        Ticker() {}
        ~Ticker(){}

        // Ticker
        template<typename T>
        void attach_us(T* tptr, void (T::*mptr)(void), timestamp_t){}

        void detach();
    };
}

#endif // TICKER_H
