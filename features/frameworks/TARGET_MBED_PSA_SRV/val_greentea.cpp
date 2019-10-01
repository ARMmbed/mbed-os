#include "greentea-client/test_env.h"
#include "inttypes.h"
#include "val_greentea.h"

void pal_mbed_os_compliance_test_initialize(void);
void pal_mbed_os_compliance_test_destroy(void);

extern "C" {



/* globals */
test_status_buffer_t g_status_buffer;

void mbed_val_test_init(uint32_t test_num, char8_t *desc, uint32_t test_bitfield)
{
    /*global init*/
    g_status_buffer.state = 0;
    g_status_buffer.status = VAL_STATUS_INVALID;

    mbed_val_print(PRINT_ALWAYS, "\nTEST: %d | DESCRIPTION: ", test_num);
    mbed_val_print(PRINT_ALWAYS, desc, 0);
    GREENTEA_SETUP(100, "default_auto");
    mbed_val_set_status(RESULT_START(VAL_STATUS_SUCCESS));
    pal_mbed_os_compliance_test_initialize();
    return;
}

void mbed_val_test_exit(void)
{
    uint32_t status = mbed_val_get_status();
    pal_mbed_os_compliance_test_destroy();
    /* return if test skipped or failed */
    if (IS_TEST_FAIL(status) || IS_TEST_SKIP(status))
    {
        GREENTEA_TESTSUITE_RESULT(false);
    }
    else
    {
        GREENTEA_TESTSUITE_RESULT(true);
        mbed_val_set_status(RESULT_END(VAL_STATUS_SUCCESS));
    }
}

/**
@brief    - This function executes given list of tests from non-secure sequentially
            This covers non-secure to secure IPC API scenario
@param    - test_num   : Test_num
@param    - tests_list : list of tests to be executed
@param    - server_hs  : Initiate a server handshake
@return   - val_status_t
**/
val_status_t mbed_val_execute_non_secure_tests(uint32_t test_num, client_test_t *tests_list,
                                            bool_t server_hs)
{
    val_status_t status = VAL_STATUS_SUCCESS;
    int32_t test_status = VAL_STATUS_SUCCESS;
    psa_handle_t handle;
    uint32_t i = 1;
    test_info_t test_info;
    char testcase_name[100] = "";
    bool continue_test = true;

    test_info.test_num = test_num;

    mbed_val_print(PRINT_TEST, "[Info] Executing tests from non-secure\n", 0);
    while (tests_list[i] != NULL)
    {
        memset(testcase_name, 0, 100);
        sprintf(testcase_name, "Check%" PRIu32, i);
        GREENTEA_TESTCASE_START(testcase_name);
        if (server_hs == TRUE)
        {
            /* Handshake with server tests */
            test_info.block_num = i;
            status = mbed_val_execute_secure_test_func(&handle, test_info,
                                                    SERVER_TEST_DISPATCHER_SID);
            if (VAL_ERROR(status))
            {
                mbed_val_set_status(RESULT_FAIL(status));
                mbed_val_print(PRINT_ERROR, "[Check%d] START\n", i);
                return status;
            }
            else
            {
                mbed_val_print(PRINT_DEBUG, "[Check%d] START\n", i);
            }
        }

        /* Execute client tests */
        test_status = tests_list[i](CALLER_NONSECURE);

        if (server_hs == TRUE)
        {
            /* Retrive Server test status */
            status = mbed_val_get_secure_test_result(&handle);
        }

        if (test_status != VAL_STATUS_SUCCESS)
        {
            status = VAL_STATUS_ERROR;
        }

        if (IS_TEST_SKIP(status))
        {
            mbed_val_set_status(status);
            mbed_val_print(PRINT_DEBUG, "[Check%d] SKIPPED\n", i);
            GREENTEA_TESTCASE_FINISH(testcase_name, 1, 0);
            continue_test = false;
        }
        else if (VAL_ERROR(status))
        {
            mbed_val_set_status(RESULT_FAIL(status));
            if (server_hs == TRUE)
                mbed_val_print(PRINT_ERROR, "[Check%d] FAILED\n", i);
            GREENTEA_TESTCASE_FINISH(testcase_name, 0, 1);
            continue_test = false;
        }
        else
        {
            if (server_hs == TRUE)
                mbed_val_print(PRINT_DEBUG, "[Check%d] PASSED\n", i);
            GREENTEA_TESTCASE_FINISH(testcase_name, 1, 0);
            continue_test = true;
        }

        if (!continue_test)
        {
            return status;
        }

        i++;
    }
    return status;
}

/**
@brief    - Records the state and status of test
@return   - val_status_t
**/
val_status_t mbed_val_set_status(uint32_t status)
{
    g_status_buffer.state = ((status >> TEST_STATE_BIT) & TEST_STATE_MASK);
    g_status_buffer.status = (status & TEST_STATUS_MASK);

    return VAL_STATUS_SUCCESS;
}

/**
@brief    - Updates the state and status for a given test
@return   - test status
**/
uint32_t mbed_val_get_status(void)
{
    return ((g_status_buffer.state) << TEST_STATE_BIT) | (g_status_buffer.status);
}

/**
@brief    - This function is used to handshake between:
            - nonsecure client fn to server test fn
            - secure client fn and server test fn
            - nonsecure client fn to secure client test fn
@param    - handle     : handle returned while connecting given sid
@param    - test_info  : Test_num and block_num to be executed
@param    - sid        : RoT service to be connected. Partition dispatcher sid
@return   - val_status_t
**/
val_status_t mbed_val_execute_secure_test_func(psa_handle_t *handle, test_info_t test_info, uint32_t sid)
{
    uint32_t test_data;
    val_status_t status = VAL_STATUS_SUCCESS;
    psa_status_t status_of_call = PSA_SUCCESS;

    *handle = pal_ipc_connect(sid, 0);
    if (*handle < 0)
    {
        mbed_val_print(PRINT_ERROR, "Could not connect SID. Handle=%x\n", *handle);
        return VAL_STATUS_CONNECTION_FAILED;
    }

    test_data = ((uint32_t)(test_info.test_num) | ((uint32_t)(test_info.block_num) << BLOCK_NUM_POS) | ((uint32_t)(TEST_EXECUTE_FUNC) << ACTION_POS));
    psa_invec data[1] = {{&test_data, sizeof(test_data)}};

    status_of_call = pal_ipc_call(*handle, data, 1, NULL, 0);
    if (status_of_call != PSA_SUCCESS)
    {
        status = VAL_STATUS_CALL_FAILED;
        mbed_val_print(PRINT_ERROR, "Call to dispatch SF failed. Status=%x\n", status_of_call);
        pal_ipc_close(*handle);
    }

    return status;
}

/**
@brief    - Print module. This is client interface API of secure partition
            mbed_val_print_sf API for nspe world
@param    - verbosity: Print verbosity level
            - string   : Input string
            - data     : Value for format specifier
@return   - val_status_t
**/
val_status_t mbed_val_print(print_verbosity_t verbosity, const char *string, uint32_t data)
{
    if (data != 0) {
        printf(string, data);
    } else {
        printf(string);
    }

    return VAL_STATUS_SUCCESS;
}

/**
@brief    - This function is used to retrive the status of previously connected test function
            using mbed_val_execute_secure_test_func
@param    - handle     : handle of server function. Handle of Partition dispatcher sid
@return   - The status of test functions
**/
val_status_t mbed_val_get_secure_test_result(psa_handle_t *handle)
{
    uint32_t test_data;
    val_status_t status = VAL_STATUS_SUCCESS;
    psa_status_t status_of_call = PSA_SUCCESS;

    test_data = (TEST_RETURN_RESULT << ACTION_POS);

    psa_outvec resp = {&status, sizeof(status)};
    psa_invec data[1] = {{&test_data, sizeof(test_data)}};

    status_of_call = pal_ipc_call(*handle, data, 1, &resp, 1);
    if (status_of_call != PSA_SUCCESS)
    {
        status = VAL_STATUS_CALL_FAILED;
        mbed_val_print(PRINT_ERROR, "Call to dispatch SF failed. Status=%x\n", status_of_call);
    }

    pal_ipc_close(*handle);
    return status;
}

/**
 * @brief Connect to given sid
   @param  -sid : RoT service id
   @param  -minor_version : minor_version of RoT service
   @param  -handle - return connection handle
 * @return val_status_t
 */
val_status_t mbed_val_ipc_connect(uint32_t sid, uint32_t minor_version, psa_handle_t *handle )
{
    *handle = pal_ipc_connect(sid, minor_version);

    if (*handle < 0)
    {
        return VAL_STATUS_CONNECTION_FAILED;
    }

    return VAL_STATUS_SUCCESS;
}

/**
 * @brief Call a connected Root of Trust Service.@n
 * The caller must provide an array of ::psa_invec_t structures as the input payload.
 *
 * @param  handle   Handle for the connection.
 * @param  in_vec   Array of psa_invec structures.
 * @param  in_len   Number of psa_invec structures in in_vec.
 * @param out_vec  Array of psa_outvec structures for optional Root of Trust Service response.
 * @param  out_len  Number of psa_outvec structures in out_vec.
 * @return val_status_t
 */
val_status_t mbed_val_ipc_call(psa_handle_t handle, psa_invec *in_vec, size_t in_len,
                          psa_outvec *out_vec, size_t out_len)
{
    psa_status_t call_status = PSA_SUCCESS;

    call_status = pal_ipc_call(handle, in_vec, in_len, out_vec, out_len);

    if (call_status != PSA_SUCCESS)
    {
        return VAL_STATUS_CALL_FAILED;
    }

    return VAL_STATUS_SUCCESS;
}

/**
 * @brief Close a connection to a Root of Trust Service.
 * Sends the PSA_IPC_DISCONNECT message to the Root of Trust Service so it can clean up resources.
 *
 * @param handle Handle for the connection.
 * @return void
 */
void mbed_val_ipc_close(psa_handle_t handle)
{
    pal_ipc_close(handle);
}


/**
 * @brief reprogram the watchdog timer
 * always succeeds on mbed-greentead testing.
 *
 * @param timeout_type type of timeout.
 * @return val_status_t
 */
val_status_t mbed_val_wd_reprogram_timer(wd_timeout_type_t timeout_type)
{
    return VAL_STATUS_SUCCESS;
}


} // extern "C"


