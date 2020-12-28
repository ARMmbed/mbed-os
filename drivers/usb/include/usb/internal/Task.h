/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#ifndef MBED_TASK_H
#define MBED_TASK_H

#include "events/EventQueue.h"
#include "usb/internal/TaskBase.h"
#include "platform/mbed_assert.h"
#include "platform/Callback.h"

namespace events {
/** \addtogroup drivers-internal-api-usb
 * @{
 */


template<typename F, typename A1 = void, typename A2 = void, typename A3 = void, typename A4 = void, typename A5 = void>
struct AllArgs;

template<typename B0>
struct AllArgs<B0> {
    typedef AllArgs<B0> Self;
    B0 b0;

    AllArgs(B0 b0 = B0()): b0(b0) {}

    template <typename T, typename _>
    struct Operations {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            s->b0();
            s->~Self();
        }
    };

    typedef Operations<B0, void> ops;
};

template<typename B0, typename B1>
struct AllArgs<B0, B1> {
    typedef AllArgs<B0, B1> Self;
    B0 b0;
    B1 b1;

    AllArgs(B0 b0 = B0(), B1 b1 = B1()): b0(b0), b1(b1) {}

    template <typename T, typename _>
    struct Operations {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            s->b0(s->b1);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T *, R(U::*)()> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))();
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)() const> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))();
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)() volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))();
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)() const volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))();
            s->~Self();
        }
    };

    typedef Operations<B0, B1> ops;
};

template<typename B0, typename B1, typename B2>
struct AllArgs<B0, B1, B2> {
    typedef AllArgs<B0, B1, B2> Self;
    B0 b0;
    B1 b1;
    B2 b2;


    AllArgs(B0 b0 = B0(), B1 b1 = B1(), B2 b2 = B2()): b0(b0), b1(b1), b2(b2) {}

    template <typename T, typename _>
    struct Operations {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            s->b0(s->b1, s->b2);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T *, R(U::*)(B2)> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2) const> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2) volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2) const volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2);
            s->~Self();
        }
    };

    typedef Operations<B0, B1> ops;
};

template<typename B0, typename B1, typename B2, typename B3>
struct AllArgs<B0, B1, B2, B3> {
    typedef AllArgs<B0, B1, B2, B3> Self;
    B0 b0;
    B1 b1;
    B2 b2;
    B3 b3;


    AllArgs(B0 b0 = B0(), B1 b1 = B1(), B2 b2 = B2(), B3 b3 = B3()): b0(b0), b1(b1), b2(b2), b3(b3) {}

    template <typename T, typename _>
    struct Operations {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            s->b0(s->b1, s->b2, s->b3);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T *, R(U::*)(B2, B3)> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3) const> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3) volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3) const volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3);
            s->~Self();
        }
    };

    typedef Operations<B0, B1> ops;
};

template<typename B0, typename B1, typename B2, typename B3, typename B4>
struct AllArgs<B0, B1, B2, B3, B4> {
    typedef AllArgs<B0, B1, B2, B3, B4> Self;
    B0 b0;
    B1 b1;
    B2 b2;
    B3 b3;
    B4 b4;


    AllArgs(B0 b0 = B0(), B1 b1 = B1(), B2 b2 = B2(), B3 b3 = B3(), B4 b4 = B4()): b0(b0), b1(b1), b2(b2), b3(b3), b4(b4) {}

    template <typename T, typename _>
    struct Operations {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            s->b0(s->b1, s->b2, s->b3, s->b4);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T *, R(U::*)(B2, B3, B4)> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3, B4) const> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3, B4) volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3, B4) const volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4);
            s->~Self();
        }
    };

    typedef Operations<B0, B1> ops;
};

template<typename B0, typename B1, typename B2, typename B3, typename B4, typename B5>
struct AllArgs {
    typedef AllArgs<B0, B1, B2, B3, B4, B5> Self;
    B0 b0;
    B1 b1;
    B2 b2;
    B3 b3;
    B4 b4;
    B5 b5;


    AllArgs(B0 b0 = B0(), B1 b1 = B1(), B2 b2 = B2(), B3 b3 = B3(), B4 b4 = B4(), B5 b5 = B5()): b0(b0), b1(b1), b2(b2), b3(b3), b4(b4), b5(b5) {}

    template <typename T, typename _>
    struct Operations {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            s->b0(s->b1, s->b2, s->b3, s->b4, s->b5);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T *, R(U::*)(B2, B3, B4, B5)> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4, s->b5);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3, B4, B5) const> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4, s->b5);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3, B4, B5) volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4, s->b5);
            s->~Self();
        }
    };

    template <typename T, typename R, typename U>
    struct Operations<T, R(U::*)(B2, B3, B4, B5) const volatile> {
        static void copy(void *_dest, void *_src)
        {
            new (_dest) Self(*(Self *)_src);
        }

        static void call(void *data)
        {
            Self *s = static_cast<Self *>(data);
            ((s->b0)->*(s->b1))(s->b2, s->b3, s->b4, s->b5);
            s->~Self();
        }
    };

    typedef Operations<B0, B1> ops;
};


template <typename F>
class Task;

template <typename R>
class Task<R()>: public TaskBase {
public:

    Task(TaskQueue *q = NULL, mbed::Callback<R()> cb = mbed::Callback<R()>())
        : TaskBase(q), _args(cb)
    {
    }

    Task &operator=(mbed::Callback<R()> cb)
    {
        _args.b0 = cb;
        return *this;
    }

    void call()
    {
        post();
    }

protected:

    virtual uint32_t size()
    {
        return sizeof(_args);
    }

    virtual run_callback_t start(void *data, uint32_t max_size)
    {
        All::ops::copy(data, (void *)&_args);
        return &All::ops::call;
    }

private:
    typedef AllArgs<mbed::Callback<R()> > All;
    All _args;
};

template <typename R, typename A0>
class Task<R(A0)>: public TaskBase {
public:

    Task(TaskQueue *q = NULL, mbed::Callback<R(A0)> cb = mbed::Callback<R(A0)>())
        : TaskBase(q), _args(cb)
    {
    }

    Task &operator=(mbed::Callback<R(A0)> cb)
    {
        _args.b0 = cb;
        return *this;
    }

    void call(A0 a0)
    {
        _args.b1 = a0;
        post();
    }

protected:

    virtual uint32_t size()
    {
        return sizeof(_args);
    }

    virtual run_callback_t start(void *data, uint32_t max_size)
    {
        All::ops::copy(data, (void *)&_args);
        return &All::ops::call;
    }

private:
    typedef AllArgs<mbed::Callback<R(A0)>, A0> All;
    All _args;
};

/** Task
 *
 *  Representation of a postable task
 */
template <typename R, typename A0, typename A1>
class Task<R(A0, A1)>: public TaskBase {
public:

    /**
     * Construct a new task
     *
     * @param q TaskQueue to post to
     * @param cb Callback to run
     */
    Task(TaskQueue *q = NULL, mbed::Callback<R(A0, A1)> cb = mbed::Callback<R(A0, A1)>())
        : TaskBase(q), _args(cb)
    {
    }

    /**
     * Set the callback of this task
     *
     * @param cb Callback to run
     */
    Task &operator=(mbed::Callback<R(A0, A1)> cb)
    {
        _args.b0 = cb;
        return *this;
    }

    /**
     * Post this task for execution
     *
     * The number of arguments to call should match
     * the type of the callback. For example Task<void(int, int)>
     * expects two integers as arguments to call, while Task<void()>
     * expects no arguments.
     *
     * @param a0 First callback parameter
     * @param a1 Second callback parameter
     */
    void call(A0 a0, A1 a1)
    {
        _args.b1 = a0;
        _args.b2 = a1;
        post();
    }

protected:

    virtual uint32_t size()
    {
        return sizeof(_args);
    }

    virtual run_callback_t start(void *data, uint32_t max_size)
    {
        All::ops::copy(data, (void *)&_args);
        return &All::ops::call;
    }

private:
    typedef AllArgs<mbed::Callback<R(A0, A1)>, A0, A1> All;
    All _args;
};

/** @}*/

}

#endif
