//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <wmi.h>
#include <atheros_wifi_api.h>

#include "dset_api.h"

HOST_DSET_HANDLE dset_handles[MAX_HOST_DSET_SIZE];


//TODO: fixme. dummy fn, implemented in app
PREWEAK_CODE int32_t POSTWEAK_CODE remote_dset_op(DSET_OP op, HOST_DSET_HANDLE *pDsetHandle)
{
    return A_ERROR;
}


HOST_DSET_HANDLE *dset_find_handle(uint32_t dset_id)
{
    uint16_t i;
    HOST_DSET_HANDLE *pDsetHandle = dset_handles;

    for (i = 0; i < MAX_HOST_DSET_SIZE; i++)
    {
        if (pDsetHandle->dset_id == dset_id)
            return pDsetHandle;
    }
    return NULL;
}

void dset_close_handle(HOST_DSET_HANDLE *pDsetHandle)
{
    pDsetHandle->dset_id = INVALID_DSET_ID;
    if (pDsetHandle->data_ptr)
        A_FREE(pDsetHandle->data_ptr, MALLOC_ID_TEMPORARY);
}

HOST_DSET_HANDLE *dset_insert_handle(uint32_t dset_id, uint32_t flags, dset_callback_fn_t cb, void *cb_args)
{
    uint16_t i;
    HOST_DSET_HANDLE *pDsetHandle = dset_handles;

    for (i = 0; i < MAX_HOST_DSET_SIZE; i++, pDsetHandle++)
    {
        if (pDsetHandle->dset_id == INVALID_DSET_ID)
        {
            pDsetHandle->dset_id = dset_id;
            pDsetHandle->flags = flags;
            pDsetHandle->cb = cb;
            pDsetHandle->cb_args = cb_args;
            pDsetHandle->data_ptr = NULL;
            return pDsetHandle;
        };
    }
    return NULL;
}

int32_t qcom_dset_create(HOST_DSET_HANDLE **pDsetHandle,
                          uint32_t dset_id,
                          uint32_t media_id,
                          uint32_t length,
                          uint32_t flags,
                          dset_callback_fn_t create_cb,
                          void *callback_arg)
{
    int32_t status;

    *pDsetHandle = dset_find_handle(dset_id);
    if (*pDsetHandle != NULL)
        return A_ERROR;

    *pDsetHandle = dset_insert_handle(dset_id, flags, create_cb, callback_arg);
    if (*pDsetHandle == NULL)
        return A_ERROR;

    (*pDsetHandle)->length = length;
    (*pDsetHandle)->media_id = media_id;

    status = remote_dset_op(DSET_OP_CREATE, *pDsetHandle);

    return status;
}

int32_t qcom_dset_open(
    HOST_DSET_HANDLE **ppDsetHandle, uint32_t dset_id, uint32_t flags, dset_callback_fn_t open_cb, void *callback_arg)
{
    int32_t status;

    *ppDsetHandle = dset_find_handle(dset_id);
    if (*ppDsetHandle != NULL)
        return A_OK;

    *ppDsetHandle = dset_insert_handle(dset_id, flags, open_cb, callback_arg);
    if (*ppDsetHandle == NULL)
        return A_ERROR;

    status = remote_dset_op(DSET_OP_OPEN, *ppDsetHandle);

    return status;
}

uint32_t qcom_dset_write(HOST_DSET_HANDLE *pDsetHandle,
                         uint8_t *buffer,
                         uint32_t length,
                         uint32_t offset,
                         uint32_t flags,
                         dset_callback_fn_t write_cb,
                         void *callback_arg)
{
#define DSET_WRITE_HEAD_SIZE (sizeof(WMIX_DSET_OP_WRITE_PARAM_CMD) + +sizeof(WMIX_DSET_CMD_HDR) - 1)

    uint32_t status;

    pDsetHandle->offset = offset;
    pDsetHandle->length = length;
    pDsetHandle->flags = flags;
    pDsetHandle->cb = write_cb;
    pDsetHandle->cb_args = callback_arg;

    if (length + DSET_WRITE_HEAD_SIZE > MAX_DSET_BUFF_SIZE)
        return 0;

    if (pDsetHandle->data_ptr == NULL)
        pDsetHandle->data_ptr = A_MALLOC(MAX_DSET_BUFF_SIZE, MALLOC_ID_TEMPORARY);

    memcpy(pDsetHandle->data_ptr + DSET_WRITE_HEAD_SIZE, buffer, length);

    status = remote_dset_op(DSET_OP_WRITE, pDsetHandle);
    return status;
}

uint32_t qcom_dset_read(HOST_DSET_HANDLE *pDsetHandle,
                        uint8_t *buffer,
                        uint32_t length,
                        uint32_t offset,
                        dset_callback_fn_t read_cb,
                        void *callback_arg)
{
    uint32_t status;

    pDsetHandle->offset = offset;
    pDsetHandle->length = length;

    pDsetHandle->cb = read_cb;
    pDsetHandle->cb_args = callback_arg;

    if (length + sizeof(WMIX_DSET_OP_WRITE_PARAM_CMD) > MAX_DSET_BUFF_SIZE)
        return 0;

    if (pDsetHandle->data_ptr == NULL)
        pDsetHandle->data_ptr = A_MALLOC(MAX_DSET_BUFF_SIZE, MALLOC_ID_TEMPORARY);

    status = remote_dset_op(DSET_OP_READ, pDsetHandle);
    memcpy(buffer, pDsetHandle->data_ptr, length);
    return status;
}

uint32_t qcom_dset_commit(HOST_DSET_HANDLE *pDsetHandle, dset_callback_fn_t commit_cb, void *callback_arg)
{
    uint32_t status;

    pDsetHandle->cb = commit_cb;
    pDsetHandle->cb_args = callback_arg;
    status = remote_dset_op(DSET_OP_COMMIT, pDsetHandle);

    dset_close_handle(pDsetHandle);

    return status;
}

uint32_t qcom_dset_close(HOST_DSET_HANDLE *pDsetHandle, dset_callback_fn_t close_cb, void *callback_arg)
{
    uint32_t status;

    pDsetHandle->cb = close_cb;
    pDsetHandle->cb_args = callback_arg;

    status = remote_dset_op(DSET_OP_CLOSE, pDsetHandle);

    dset_close_handle(pDsetHandle);

    return status;
}

uint32_t qcom_dset_size(HOST_DSET_HANDLE *pDsetHandle, dset_callback_fn_t size_cb, void *callback_arg)
{
    uint32_t status;

    pDsetHandle->cb = size_cb;
    pDsetHandle->cb_args = callback_arg;

    status = remote_dset_op(DSET_OP_SIZE, pDsetHandle);
    return status;
}

uint32_t qcom_dset_delete(uint32_t dset_id, uint32_t flags, dset_callback_fn_t delete_cb, void *callback_arg)
{
    uint32_t status;
    HOST_DSET_HANDLE *pDsetHandle;

    pDsetHandle = dset_find_handle(dset_id);
    if (pDsetHandle == NULL)
    {
        pDsetHandle = dset_insert_handle(dset_id, flags, delete_cb, callback_arg);
        if (pDsetHandle == NULL)
            return false;
    }
    else
    {
        pDsetHandle->cb = delete_cb;
        pDsetHandle->cb_args = callback_arg;
        pDsetHandle->dset_id = dset_id;
        pDsetHandle->flags = flags;
    }

    status = remote_dset_op(DSET_OP_DELETE, pDsetHandle);
    return status;
}
