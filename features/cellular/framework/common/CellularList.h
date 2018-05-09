/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef CELLULAR_LIST_H_
#define CELLULAR_LIST_H_

#include <stddef.h>

namespace mbed {

/** Class CellularList
 *
 *  Templated linked list class for common usage.
 *
 */
template <class T> class CellularList
{
private:
    T *_head, *_tail;
public:
    CellularList()
    {
      _head=NULL;
      _tail=NULL;
    }

     ~CellularList()
    {
        T *temp = _head;
        while (temp) {
            _head = _head->next;
            delete temp;
            temp = _head;
        }
    }

    T* add_new()
    {
      T *temp=new T;
      if (!temp) {
          return NULL;
      }
      temp->next = NULL;
      if (_head == NULL) {
        _head = temp;
      } else {
        _tail->next=temp;
      }
      _tail = temp;

      return _tail;
    }

    void delete_last()
    {
        T* previous = NULL;
        T *current=_head;

        if (!current) {
            return;
        }

        while (current->next != NULL) {
            previous=current;
            current=current->next;
        }

        if (previous) {
            _tail=previous;
            previous->next=NULL;
        } else {
            _head = NULL;
            _tail = NULL;
        }

        delete current;
    }

    void delete_all()
    {
        T *temp = _head;
        while (temp) {
            _head = _head->next;
            delete temp;
            temp = _head;
        }
        _tail=NULL;
    }


    T *get_head()
    {
        return _head;
    }
};

} // namespace mbed

#endif // CELLULAR_LIST_H_
