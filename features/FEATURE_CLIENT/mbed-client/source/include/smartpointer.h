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
#ifndef SMART_POINTER_H
#define SMART_POINTER_H

class ReferenceCount
{
private:

    int _count; // Reference count

public:

void add_ref()
{
    // Increment the reference count
    _count++;
}

int release()
{
    // Decrement the reference count and
    // return the reference count.
    return --_count;
}
};

template < typename T > class SmartPointer
{
private:

    T                   *_data;         // Generic pointer to be stored
    ReferenceCount      *_reference;    // Reference count


public:

SmartPointer()
: _data(0), _reference(0)
{
    // Create a new reference
    _reference = new ReferenceCount();
    // Increment the reference count
    _reference->add_ref();
}

SmartPointer(T* value)
: _data(value), _reference(0)
{
    // Create a new reference
    _reference = new ReferenceCount();
    // Increment the reference count
    _reference->add_ref();
}

SmartPointer(const SmartPointer<T>& smart_pointer)
: _data(smart_pointer._data), reference(smart_pointer._reference)
{
    // Copy constructor
    // Copy the data and reference pointer
    // and increment the reference count
    _reference->add_ref();
}

~SmartPointer()
{
    if(_reference->release() == 0) {
        delete _data;
        delete _reference;
    }
}

T& operator* ()
{
    return *_data;
}

T* operator-> ()
{
    return _data;
}

SmartPointer<T>& operator = (const SmartPointer<T>& smart_pointer)
{
    // Assignment operator
    if (this != &SmartPointer) { // Avoid self assignment
        // Decrement the old reference count
        // if reference become zero delete the old data
        if(_reference->release() == 0) {
           delete _data;
           delete _reference;
        }

        // Copy the data and reference pointer
        // and increment the reference count
        _data = SmartPointer._data;
        _reference = SmartPointer._reference;
        _reference->add_ref();
    }
    return *this;
}

};

#endif // SMART_POINTER_H
