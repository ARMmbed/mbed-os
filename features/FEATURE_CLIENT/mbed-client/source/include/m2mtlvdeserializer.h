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
#include "mbed-client/m2mvector.h"
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"

/**
 * @brief M2MTLVDeserializer
 * TLV Deserialiser get the object instances and resources as binary data and 
 * builds the <code>lwm2m</code> representation from it. See OMA-LWM2M 
 * specification, chapter 6.1 for the resource model and chapter 6.3.3 for
 * the OMA-TLV specification.
 */
class M2MTLVDeserializer {

public :

    typedef enum {
        None,
        NotFound,
        NotAllowed,
        NotValid
    } Error;

    typedef enum {
        Put,
        Post
    } Operation;


    /**
    * Constructor.
    */
    M2MTLVDeserializer();

    /**
    * Destructor.
    */
    virtual ~M2MTLVDeserializer();

    /**
     * This method checks whether the given binary encodes an object instance
     * or something else. It returns <code>true</code> if bits 7-6 of the first
     * byte is "00".
     * @param tlv Binary to be checked as LWM2M object instance
     * @return <code>true</code> or <code>false</code>.
     */
    bool is_object_instance(uint8_t *tlv);

    
    /**
     * This method checks whether the given binary encodes a resource or 
     * something else. It returns <code>true</code> if bits 7-6 of the first
     * byte is "11".
     * @param tlv Binary to be checked as LWM2M resource.
     * @return <code>true</code> or <code>false</code>.
     */
    bool is_resource(uint8_t *tlv);

    /**
     * This method checks whether the given binary encodes a multiple resource
     * or something else. It returns <code>true</code> if bits 7-6 of the first
     * byte is "10".
     * @param tlv Binary to be checked as LWM2M multiple resource.
     * @return <code>true</code> or <code>false</code>.
     */
    bool is_multiple_resource(uint8_t *tlv);
    
    /**
     * This method checks whether the given binary encodes a resource instance
     * or something else. It returns <code>true</code> if bits 7-6 of the first
     * byte is "01".
     * @param tlv Binary to be checked as LWM2M resource instance.
     * @return <code>true</code> or <code>false</code>.
     */
    bool is_resource_instance(uint8_t *tlv);

    /**
     * Deserialises the given binary that must encode object instances. Binary
     * array can be checked before invoking this method with 
     */
    M2MTLVDeserializer::Error deserialise_object_instances(uint8_t* tlv,
                                                           uint32_t tlv_size,
                                                           M2MObject &object,
                                                           M2MTLVDeserializer::Operation operation);

    /**
     * Deserialises the given binary that must encode resources. Binary array 
     * can be checked before invoking this method.
     */
    M2MTLVDeserializer::Error deserialize_resources(uint8_t *tlv,
                                                    uint32_t tlv_size,
                                                    M2MObjectInstance &object_instance,
                                                    M2MTLVDeserializer::Operation operation);

    /**
     * Deserialises the given binary that must encode resource instances. Binary array
     * can be checked before invoking this method.
     */
    M2MTLVDeserializer::Error deserialize_resource_instances(uint8_t *tlv,
                                                             uint32_t tlv_size,
                                                             M2MResource &resource,
                                                             M2MTLVDeserializer::Operation operation);
    /**
     * This method return object instance id or resource id.
     * @param tlv Binary to be checked
     * @return Object instance id or resource id.
     */
    uint16_t instance_id(uint8_t *tlv);
    
private:

    M2MTLVDeserializer::Error deserialize_object_instances(uint8_t *tlv,
                                                           uint32_t tlv_size,
                                                           uint32_t offset,
                                                           M2MObject &object,
                                                           M2MTLVDeserializer::Operation operation,
                                                           bool update_value);
    
    M2MTLVDeserializer::Error deserialize_resources(uint8_t *tlv,
                                                    uint32_t tlv_size,
                                                    uint32_t offset,
                                                    M2MObjectInstance &object_instance,
                                                    M2MTLVDeserializer::Operation operation,
                                                    bool update_value);

    M2MTLVDeserializer::Error deserialize_resource_instances(uint8_t *tlv,
                                                             uint32_t tlv_size,
                                                             uint32_t offset,
                                                             M2MResource &resource,
                                                             M2MObjectInstance &object_instance,
                                                             M2MTLVDeserializer::Operation operation,
                                                             bool update_value);

    M2MTLVDeserializer::Error deserialize_resource_instances(uint8_t *tlv,
                                                             uint32_t tlv_size,
                                                             uint32_t offset,
                                                             M2MResource &resource,
                                                             M2MTLVDeserializer::Operation operation,
                                                             bool update_value);

    bool is_object_instance(uint8_t *tlv, uint32_t offset);
    
    bool is_resource(uint8_t *tlv, uint32_t offset);
    
    bool is_multiple_resource(uint8_t *tlv, uint32_t offset);
    
    bool is_resource_instance(uint8_t *tlv, uint32_t offset);
};

class TypeIdLength {

public:

    static TypeIdLength* createTypeIdLength(uint8_t *tlv, uint32_t offset);

    TypeIdLength* deserialize();

    void deserialiseID (uint32_t idLength);

    void deserialiseLength (uint32_t lengthType);

    uint8_t             *_tlv;
    uint32_t            _offset;
    uint32_t            _type;
    uint16_t            _id;
    uint32_t            _length;

    friend class Test_M2MTLVDeserializer;
};
