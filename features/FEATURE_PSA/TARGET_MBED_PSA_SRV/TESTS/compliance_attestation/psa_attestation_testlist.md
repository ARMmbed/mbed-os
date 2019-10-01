# PSA Initial Attestation Testcase checklist

| Test      | Return value                         | API                                                               | Test Algorithm                                                                                                                                                  | Test Cases                                                                                                                                                                                             |
|-----------|--------------------------------------|-------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| test_a001 | PSA_ATTEST_ERR_SUCCESS               | psa_initial_attest_get_token()<br />psa_initial_attest_get_token_size() | 1. Provide correct inputs to API with described challenge sizes  <br />2. Expect API to return this define as return value each time  <br />3. Verify the token | 1. Challenge_size = 32 <br />2. Challenge_size = 48 <br />3. Challenge_size = 64                                                                                                         |
|           | PSA_ATTEST_ERR_INVALID_INPUT         | psa_initial_attest_get_token()<br />psa_initial_attest_get_token_size() | 1. Provide described challenge sizes to the API along with other valid parameters <br />2. Expect API to return this define as return value each time           | 1. Challenge_size is zero <br />2. Invalid challenge size between 0 and 32 <br />3. Invalid challenge size between 32 and 64  <br />4. Challenge_size is greater than MAX_CHALLENGE_SIZE |
|           | PSA_ATTEST_ERR_TOKEN_BUFFER_OVERFLOW | psa_initial_attest_get_token()                                    | 1. Provide described taken size to the API along with other valid parameters <br />2. Expect API to return this define as return value each time                | Pass the token_size which less than actual/required token size                                                                                                                                        |
|           | PSA_ATTEST_ERR_INIT_FAILED           | psa_initial_attest_get_token()<br />psa_initial_attest_get_token_size() | Can't simulate. Test can't generate stimulus where attestation initialisation fails                                                                             |                                                                                                                                                                                                       |
|           | PSA_ATTEST_ERR_CLAIM_UNAVAILABLE     | psa_initial_attest_get_token()                                    | Can't simulate. Test can't generate stimulus where claim can unavailable                                                                                        |                                                                                                                                                                                                       |
|           | PSA_ATTEST_ERR_GENERAL               | psa_initial_attest_get_token()<br />psa_initial_attest_get_token_size() | Can't simulate. Test can't generate stimulus where unexpected error happened during API operation                                                               |                                                                                                                                                                                                       |

## Note

1. In verifying the token, only the data type of claims and presence of the mandatory claims are checked and the values of the claims are not checked.
2. Checks related to token signature validation will be part of future release

# License
Arm PSA test suite is distributed under Apache v2.0 License.

--------------

*Copyright (c) 2019, Arm Limited and Contributors. All rights reserved.*
