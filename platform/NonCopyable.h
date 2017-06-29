/* Copyright (c) 2017 ARM Limited
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

#ifndef MBED_NONCOPYABLE_H_
#define MBED_NONCOPYABLE_H_

namespace mbed { 

/**
 * Inheriting from this class autogeneration of copy construction and copy 
 * assignement operations. 
 * 
 * Classes which are not value type should inherit privately from this class 
 * to avoid generation of invalid copy constructor or copy assignement operator
 * which can lead to unoticeable programming errors. 
 * 
 * As an example consider the following signature: 
 * 
 * @code
 * class Resource; 
 *
 * class Foo { 
 * public:     
 *   Foo() : _resource(new Resource()) { }
 *   ~Foo() { delete _resource; } 
 * private:
 *   Resource* _resource;
 * }
 * 
 * Foo get_foo();
 * 
 * Foo foo = get_foo();
 * @endcode 
 * 
 * There is a bug in this function, it returns a temporary value which will be 
 * byte copied into foo then destroyed. Unfortunately, internaly the Foo class 
 * manage a pointer to a Resource object. This pointer will be released when the 
 * temporary is destroyed and foo will manage a pointer to an already released 
 * Resource.
 * 
 * Two issues has to be fixed in the example above: 
 *   - Function signature has to be changed to reflect the fact that Foo 
 *     instances cannot be copied. In that case accessor should return a 
 *     reference to give access to objects already existing and managed. 
 *     Generator on the other hand should return a pointer to the created object.
 * 
 * @code 
 * // return a reference to an already managed Foo instance
 * Foo& get_foo(); 
 * Foo& foo = get_foo();
 * 
 * // create a new Foo instance
 * Foo* make_foo();
 * Foo* m = make_foo();
 * @endcode
 * 
 *   - Copy constructor and copy assignement operator has to be made private 
 *     in the Foo class. It prevents unwanted copy of Foo objects. This can be 
 *     done by declaring copy constructor and copy assignement in the private 
 *     section of the Foo class.
 *     
 * @code 
 * class Foo { 
 * public:     
 *   Foo() : _resource(new Resource()) { }
 *   ~Foo() { delete _resource; } 
 * private:
 *   // disallow copy operations 
 *   Foo(const Foo&);
 *   Foo& operator=(const Foo&);
 *   // data members 
 *   Resource* _resource;
 * }
 * @endcode
 * 
 * Another solution is to inherit privately from the NonCopyable class. 
 * It reduces the boiler plate needed to avoid copy operations but more 
 * importantly it clarifies the programer intent and the object semantic.
 *
 * class Foo : private NonCopyable<Foo> { 
 * public:     
 *   Foo() : _resource(new Resource()) { }
 *   ~Foo() { delete _resource; } 
 * private:
 *   Resource* _resource;
 * }
 * 
 * @tparam T The type that should be made non copyable. It prevent cases where 
 * the empty base optimization cannot be applied and therefore ensure that the 
 * cost of this semantic sugar is null. 
 * 
 * As an example, the empty base optimization is prohibited if one of the empty 
 * base class is also a base type of the first non static data member: 
 * 
 * @code 
 * struct A { };
 * struct B : A { 
 *    int foo;
 * };
 * // thanks to empty base optimization, sizeof(B) == sizeof(int)
 * 
 * struct C : A { 
 *   B b;
 * };
 * 
 * // empty base optimization cannot be applied here because A from C and A from
 * // B shall have a different address. In that case, with the alignement 
 * // sizeof(C) == 2* sizeof(int)
 * @endcode
 * 
 * The solution to that problem is to templatize the empty class to makes it 
 * unique to the type it is applied to: 
 * 
 * @code 
 * template<typename T>
 * struct A<T> { };
 * struct B : A<B> { 
 *    int foo;
 * };
 * struct C : A<C> { 
 *   B b;
 * };
 * 
 * // empty base optimization can be applied B and C does not refer to the same 
 * // kind of A. sizeof(C) == sizeof(B) == sizeof(int).
 * @endcode
 */
template<typename T>
class NonCopyable { 
protected:
    /** 
     * Disalow construction of NonCopyable objects from outside of its hierarchy.
     */
    NonCopyable() { }
    /** 
     * Disalow destruction of NonCopyable objects from outside of its hierarchy.
     */
    ~NonCopyable() { }

private: 
    /**
     * Declare copy constructor as private, any attempt to copy construct 
     * a NonCopyable will fail at compile time.
     */
    NonCopyable(const NonCopyable&);

    /**
     * Declare copy assignement operator as private, any attempt to copy assign 
     * a NonCopyable will fail at compile time.
     */
    NonCopyable& operator=(const NonCopyable&);
};

} // namespace mbed 

#endif /* MBED_NONCOPYABLE_H_ */
