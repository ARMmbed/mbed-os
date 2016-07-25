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
#ifndef M2M_STRING_H
#define M2M_STRING_H

#include <stddef.h> // size_t
#include <stdexcept>
#include <stdint.h>

class Test_M2MString;

namespace m2m
{

  /** \brief Simple C++ string class, used as replacement for
   std::string.
   */
  class String
  {
    char* p;           ///< The data
    size_t allocated_;  ///< The allocated memory size (including trailing NULL)
    size_t size_;       ///< The currently used memory size (excluding trailing NULL)

  public:
    typedef size_t size_type;
    static const size_type npos;

    String();
    ~String();
    String(const String&);
    String(const char*);

    String& operator=(const char*);
    String& operator=(const String&);

    String& operator+=(const String&);
    String& operator+=(const char*);
    String& operator+=(char);
    void push_back(char);

    bool operator==(const char*) const;
    bool operator==(const String&) const;

    void clear();       // set string to empty string (memory remains reserved)

    size_type size()   const   { return size_; }   ///< size without terminating NULL
    size_type length() const   { return size_; }   ///< as size()

    size_type capacity() const { return allocated_-1; }

    bool empty() const    { return size_ == 0; }

    const char* c_str() const { return p; } ///< raw data

    /** Reserve internal string memory so that n characters can be put into the
        string (plus 1 for the NULL char). If there is already enough memory,
        nothing happens, if not, the memory will be realloated to exactly this
        amount.
        */
    void reserve( size_type n);

    /** Resize string. If n is less than the current size, the string will be truncated.
        If n is larger, the memory will be reallocated to exactly this amount, and
        the additional characters will be NULL characters.
        */
    void resize( size_type n);

    /** Resize string. If n is less than the current size, the string will be truncated.
        If n is larger, the memory will be reallocated to exactly this amount, and
        the additional characters will be c characters.
        */
    void resize( size_type n, char c);

    /// swap contents
    void swap( String& );

    String substr(const size_type pos, size_type length) const;

    // unchecked access:
    char& operator[](const size_type i)       { return p[i]; }
    char operator[](const size_type i) const { return p[i]; }
    // checked access:
    char at(const size_type i) const;

    /// erase len characters at position pos
    String& erase(size_type pos, size_type len);
    /// Append n characters of a string
    String& append(const char* str, size_type n);

    // Append n characters of a non-zero-terminated string
    // (in contrast with other append(), which performs strlen() for the given string).
    String& append_raw(const char*, size_type);

    // convert int to ascii and append it to end of string
    void append_int(int);

    int compare( size_type pos, size_type len, const String& str ) const;
    int compare( size_type pos, size_type len, const char*   str ) const;

    int find_last_of(char c) const;

    static uint8_t* convert_integer_to_array(int64_t value, uint8_t &size, uint8_t *array = NULL, uint32_t array_size = 0);
    static int64_t convert_array_to_integer(uint8_t *value, uint32_t size);

  private:
    // reallocate the internal memory
    void new_realloc( size_type n);
    char* strdup(const char* other);

    friend class ::Test_M2MString;

  };
  // class

  bool operator<(const String&, const String&);

  void reverse(char s[], uint32_t length);

  uint32_t itoa_c (int64_t n, char s[]);
} // namespace


#endif // M2M_STRING_H
