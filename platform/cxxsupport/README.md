## C++ support ##

Mbed OS supports a number of toolchains, and the files here provide support
to smooth over C++ library differences.

The current baseline version is C++14, so we hope for full C++14 library
support.

Omissions are:
* areas like chrono and threads, which depend on OS support, where
  retargeting is not complete.
* atomics and shared pointers, as atomic implementations for ARMv6-M
  are not provided by all toolchains, and the ARMv7-M implementations include
  DMB instructions we do not want/need for non-SMP.

User code should normally be able to include C++14 headers and get
most expected functionality. (But bear in mind that many C++ library
features like streams and STL containers are quite heavy and may
not be appropriate for small embedded use).

However, ARM C 5 has only C++11 language support (at least a large subset), and
no C++11/14 library. For the headers that are totally new in C++11,
they are provided here in TOOLCHAIN_ARMC5 under their standard C++11 name.
(Eg `<array>`, `<cinttypes>`, `<initializer_list>`, `<type_traits>`).
But for headers that already exist in C++03, extensions are required.

So, to support ARM C 5, use `#include <mstd_utility>`, rather than
`#include <utility>` if you need C++11 or later features from that header.

Each `mstd_` file includes the toolchain's corresponding header file,
which will provide its facilities in `namespace std`. Any missing
C++11/C++14 facilities for ARM C 5 are also provided in `namespace std`.

Then APIs from `namespace std` are added to `namespace mstd`, with adjustment
if necessary, and APIs being omitted if not suitable for embedded use.
For example:

   * `std::size_t` (`<cstddef>`) - toolchain's `std::size_t`
   * `mstd::size_t` (`<mstd_cstddef>`) - alias for `std::size_t`
   * `std::swap` (`<utility>`) - toolchain's `std::swap` (not move-capable for ARM C 5)
   * `mstd::swap` (`<mstd_utility>`) - alias for `std::swap` or move-capable ARM C 5 replacement.
   * `std::atomic` (`<atomic>`) - toolchain's `std::atomic` (not implemented for IAR ARMv6)
   * `mstd::atomic` (`<mstd_atomic>`) - custom `mstd::atomic` for all toolchains
   * `std::void_t` (`<type_traits>`) - toolchain's `std::void_t` if available (it's C++17 so likely not)
   * `mstd::void_t` (`<mstd_type_traits>`) - alias for `std::void_t` if available, else local implementation
   * `std::thread` (`<thread>`) - toolchain's `std::thread` - not available or ported
   * `mstd::thread` - doesn't exist - `mstd` APIs only exist if available on all toolchains

Using `std::xxx` will generally work, but may suffer from toolchain variance. `mstd::xxx` should always be better - it will either be an alias to `std::xxx`, or work better for Mbed OS.

In portable code, when compiling for non-Mbed OS, the directive `namespace mstd == std` can be used
to cover the difference:

```C++
// my_code.c
#if TARGET_LIKE_MBED
#include <mstd_atomic>
#else
#include <atomic>
namespace mstd = std;
#endif

mstd::atomic my_atomic;
```

