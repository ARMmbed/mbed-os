/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef US_TICKER_H
#define US_TICKER_H

/* TIMER0 is reserved for SoftDevice. We will use TIMER1 for us ticker
 * which counter size is 32 bits. */

#define US_TICKER_COUNTER_BITS        32u
#define US_TICKER_FREQ                1000000

#endif // US_TICKER_H
