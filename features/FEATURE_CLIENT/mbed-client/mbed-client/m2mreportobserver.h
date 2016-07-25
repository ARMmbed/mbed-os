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
#ifndef M2MREPORTOBSERVER_H
#define M2MREPORTOBSERVER_H

#include <inttypes.h>
#include <mbed-client/m2mvector.h>

/**
 * \brief M2MReportObserver
 * An interface for inviting the base class
 * to send a report to the server.
 *
 */
class M2MReportObserver
{
  public:

    /**
     * \brief An observation callback to be sent to the
     * server due to a change in the observed parameter.
     * \param changed_instance_ids A list of changed object instance IDs.
     * \param send_object Indicates whether the whole object will be sent or not.
     */
    virtual void observation_to_be_sent(m2m::Vector<uint16_t> changed_instance_ids,
                                        bool send_object = false) = 0;

};

#endif // M2MREPORTOBSERVER_H
