# UNITY: Unit Testing for C

This yotta module is a fork of the [UNITY test framework](http://www.throwtheswitch.org/unity/) with minor modifications to be able to use it with the [utest harness](https://github.com/ARMmbed/utest) for mbed OS.

Please note that this module only contains the unity test macros, and no additions such as the unity fixtures.
Furthermore the failure macros have been modified to integrate with `utest` failure handlers, as `setjmp` and `longjmp` are not supported by `utest`.

Specifically
- `UNITY_FAIL_AND_BAIL` calls `utest_unity_assert_failure()`, and
- `UNITY_IGNORE_AND_BAIL` calls `utest_unity_ignore_failure()`

which then invoke the appropriate action in `utest`.

**To use these macros you need to depend on both `unity` and `utest`!**

For the original documentation of UNITY, please [visit the project's homepage](http://www.throwtheswitch.org/unity/).

## Macros

UNITY provides a lot of test macros.

**Be aware of the macro argument order**: (EXPECTED VALUE, ACTUAL VALUE).  
So `TEST_ASSERT_EQUAL(4, value);` is the right way around.

Note, that you can provide a custom failure message by appending `_MESSAGE` to any macro and passing the message string as the _last argument_.

#### Generic:

- `TEST_FAIL()`
- `TEST_IGNORE()`
- `TEST_ONLY()`

#### Boolean:

- `TEST_ASSERT(condition)`
- `TEST_ASSERT_TRUE(condition)`
- `TEST_ASSERT_UNLESS(condition)`
- `TEST_ASSERT_FALSE(condition)`

#### Pointer:

- `TEST_ASSERT_NULL(pointer)`
- `TEST_ASSERT_NOT_NULL(pointer)`

#### Equality:

- `TEST_ASSERT_EQUAL(expected, actual)`
- `TEST_ASSERT_NOT_EQUAL(expected, actual)`
- `TEST_ASSERT_EQUAL_INT(expected, actual)`
- `TEST_ASSERT_EQUAL_INT8(expected, actual)`
- `TEST_ASSERT_EQUAL_INT16(expected, actual)`
- `TEST_ASSERT_EQUAL_INT32(expected, actual)`
- `TEST_ASSERT_EQUAL_INT64(expected, actual)`
- `TEST_ASSERT_EQUAL_UINT(expected, actual)`
- `TEST_ASSERT_EQUAL_UINT8(expected, actual)`
- `TEST_ASSERT_EQUAL_UINT16(expected, actual)`
- `TEST_ASSERT_EQUAL_UINT32(expected, actual)`
- `TEST_ASSERT_EQUAL_UINT64(expected, actual)`
- `TEST_ASSERT_EQUAL_HEX(expected, actual)`
- `TEST_ASSERT_EQUAL_HEX8(expected, actual)`
- `TEST_ASSERT_EQUAL_HEX16(expected, actual)`
- `TEST_ASSERT_EQUAL_HEX32(expected, actual)`
- `TEST_ASSERT_EQUAL_HEX64(expected, actual)`

#### Bit Masks:

- `TEST_ASSERT_BITS(mask, expected, actual)`
- `TEST_ASSERT_BITS_HIGH(mask, actual)`
- `TEST_ASSERT_BITS_LOW(mask, actual)`
- `TEST_ASSERT_BIT_HIGH(bit, actual)`
- `TEST_ASSERT_BIT_LOW(bit, actual)`

#### Deltas:

- `TEST_ASSERT_INT_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_INT8_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_INT16_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_INT32_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_INT64_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_UINT_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_UINT8_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_UINT16_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_UINT32_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_UINT64_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_HEX_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_HEX8_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_HEX16_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_HEX32_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_HEX64_WITHIN(delta, expected, actual)`

#### Memory:

- `TEST_ASSERT_EQUAL_PTR(expected, actual)`
- `TEST_ASSERT_EQUAL_STRING(expected, actual)`
- `TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len)`
- `TEST_ASSERT_EQUAL_MEMORY(expected, actual, len)`

#### Array:

- `TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_INT8_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_INT16_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_INT32_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_INT64_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_UINT_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_UINT16_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_UINT64_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_HEX_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_HEX16_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_HEX32_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_HEX64_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_PTR_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, actual, len, num_elements)`

#### Single Precision Floating Point:

- `TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_EQUAL_FLOAT(expected, actual)`
- `TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_FLOAT_IS_INF(actual)`
- `TEST_ASSERT_FLOAT_IS_NEG_INF(actual)`
- `TEST_ASSERT_FLOAT_IS_NAN(actual)`
- `TEST_ASSERT_FLOAT_IS_DETERMINATE(actual)`
- `TEST_ASSERT_FLOAT_IS_NOT_INF(actual)`
- `TEST_ASSERT_FLOAT_IS_NOT_NEG_INF(actual)`
- `TEST_ASSERT_FLOAT_IS_NOT_NAN(actual)`
- `TEST_ASSERT_FLOAT_IS_NOT_DETERMINATE(actual)`

#### Double Precision Floating Point:

- `TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual)`
- `TEST_ASSERT_EQUAL_DOUBLE(expected, actual)`
- `TEST_ASSERT_EQUAL_DOUBLE_ARRAY(expected, actual, num_elements)`
- `TEST_ASSERT_DOUBLE_IS_INF(actual)`
- `TEST_ASSERT_DOUBLE_IS_NEG_INF(actual)`
- `TEST_ASSERT_DOUBLE_IS_NAN(actual)`
- `TEST_ASSERT_DOUBLE_IS_DETERMINATE(actual)`
- `TEST_ASSERT_DOUBLE_IS_NOT_INF(actual)`
- `TEST_ASSERT_DOUBLE_IS_NOT_NEG_INF(actual)`
- `TEST_ASSERT_DOUBLE_IS_NOT_NAN(actual)`
- `TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(actual)`
