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
#ifndef M2M_OBSERVATION_HANDLER_H
#define M2M_OBSERVATION_HANDLER_H

//FORWARD DECLARATION
class M2MBase;
class M2MResourceInstance;

/**
 * \brief M2MObservationHandler
 * An interface for handling observation
 * callbacks from different objects.
 *
 */
class M2MObservationHandler
{
  public:

    /**
     * \brief The observation callback to be sent to the
     * server due to a change in a parameter under observation.
     * \param object The observed object whose information needs to be sent.
     * \param obs_number The observation number.
     * \param changed_instance_ids A list of changed object instance IDs.
     * \param send_object Indicates whether the whole object will be sent or not.
     */
    virtual void observation_to_be_sent(M2MBase *object,
                                        uint16_t obs_number,
                                        m2m::Vector<uint16_t> changed_instance_ids,
                                        bool send_object = false) = 0;

    /**
     * \brief A callback for deleting an NSDL resource.
     * \param resource_name The name of the observed object whose information
     * needs to be deleted.
     */
    virtual void resource_to_be_deleted(const String &resource_name) = 0;

    /**
     * \brief A callback indicating that the value of the resource object is updated by server.
     * \param base The object whose value is updated.
     * \param object_name The name of the updated resource, default is empty.
     */
    virtual void value_updated(M2MBase *base, const String &object_name = "") = 0;

    /**
     * \brief A callback for removing an object from the list.
     * \param object The M2MObject to be removed.
     */
    virtual void remove_object(M2MBase *object) = 0;

    /**
     * \brief A delayed response callback to be sent to the
     * server due to a changed response.
     * \param base The resource sending the response.
     */
    virtual void send_delayed_response(M2MBase *base) = 0;

};


#endif // M2M_OBSERVATION_HANDLER_H
