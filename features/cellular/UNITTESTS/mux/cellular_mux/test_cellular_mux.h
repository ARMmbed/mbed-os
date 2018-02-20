/*
 * Copyright (c) 2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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
#ifndef TEST_cellular_mux_H
#define TEST_cellular_mux_H

class Test_cellular_mux
{
public:
    Test_cellular_mux();

    virtual ~Test_cellular_mux();

    void test_cellular_mux_module_init();

    void test_cellular_mux_mux_start();

    void test_cellular_mux_dlci_establish();

    void test_cellular_mux_serial_attach();

    void test_cellular_mux_eventqueue_attach();
};

#endif // TEST_cellular_mux_H

