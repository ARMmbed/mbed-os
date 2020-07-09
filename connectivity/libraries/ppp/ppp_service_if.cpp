/*
 * Copyright (c) 2019 ARM Limited
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

#include <stdlib.h>
#include <stdio.h>
#include "mbed_interface.h"
#include "mbed_trace.h"
#include "netsocket/nsapi_types.h"
#include "mbed_shared_queues.h"
#include "mbed_poll.h"
#include "ip4string.h"
#include "ip6string.h"
#include "ppp_service.h"
#include "ppp_impl.h"

#if PPP_SUPPORT

#define TRACE_GROUP "ppp_serv_if"

typedef void (*ppp_sys_timeout_handler)(void *arg);

// Number of timers running simultaneous
#define PPP_TIMER_NUM            5
// Number of simultaneous memory buffers
#define PPP_PBUF_HANDLE_NUM      10

typedef struct {
    ppp_service *ppp_drv;
    ppp_sys_timeout_handler handler;
    void *arg;
    int equeue_id;
    uint8_t id;
} ppp_sys_timeout_t;

static ppp_sys_timeout_t *timeout[PPP_TIMER_NUM];
static SingletonPtr<PlatformMutex> ppp_service_if_mutex;
static uint8_t ppp_service_sys_timeout_id = 0;
static pbuf pbuf_handles[PPP_PBUF_HANDLE_NUM];

#if PPP_TRACE_ENABLE
#define OUTPUT_BUFFER           0
#define INPUT_BUFFER            1

#define BUFFER_SIZE             5000

typedef struct  {
    char buffer[BUFFER_SIZE];
    int buffer_index = 0;
    int line_len = 0;
} trace_buf_t;

static trace_buf_t output_trace_buffer;
static trace_buf_t input_trace_buffer;

#if PPP_TRACE_BUFFER_SIZE > 0
static char cont_trace_buffer[PPP_TRACE_BUFFER_SIZE];
static int cont_trace_buffer_index = 0;
#endif
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
#if !defined(ppp_htons)
/**
 * Convert an u16_t from host- to network byte order.
 *
 * @param n u16_t in host byte order
 * @return n in network byte order
 */
uint16_t ppp_htons(uint16_t n)
{
    return PP_HTONS(n);
}
#endif /* ppp_htons */

#if !defined(ppp_htonl)
/**
 * Convert an u32_t from host- to network byte order.
 *
 * @param n u32_t in host byte order
 * @return n in network byte order
 */
uint32_t ppp_htonl(uint32_t n)
{
    return PP_HTONL(n);
}
#endif /* ppp_htonl */

#endif /* BYTE_ORDER == LITTLE_ENDIAN */

extern "C" {

    void ppp_ip4_addr_set(nsapi_addr_t *addr, uint32_t *src)
    {
        memset(addr->bytes, 0, NSAPI_IP_BYTES);
        memcpy(addr->bytes, src, 4);
        addr->version = NSAPI_IPv4;
    }

    struct netif *ppp_netif_add(struct netif *netif, void *state, netif_init_fn init)
    {
#if PPP_IPV4_SUPPORT
        netif->output = NULL;
        netif->ipv4_up = false;
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
        netif->output_ip6 = NULL;
        netif->ipv6_up = false;
#endif
        netif->mtu = 0;
        netif->input = NULL;
        netif->state = state;

        /* call user specified initialization function for netif */
        if (init(netif) != ERR_OK) {
            return NULL;
        }

        return netif;
    }

    err_t ppp_ip_input(struct pbuf *p, struct netif *inp)
    {
        ppp_service *ppp_service_ptr = static_cast<ppp_service *>(inp->service_ptr);
        void *buffer = ppp_memory_buffer_convert_from(p);
        ppp_service_ptr->link_input(static_cast<net_stack_mem_buf_t *>(buffer));
        return ERR_OK;
    }

    void ppp_set_link_up(struct netif *netif)
    {
        ppp_service *ppp_service_ptr = static_cast<ppp_service *>(netif->service_ptr);
        ppp_service_ptr->link_state(true);
    }

    void ppp_set_link_down(struct netif *netif)
    {
        ppp_service *ppp_service_ptr = static_cast<ppp_service *>(netif->service_ptr);
        ppp_service_ptr->link_state(false);
    }

    err_t ppp_call_callback(void *service_ptr, ppp_service_cb callback, void *arg)
    {
        ppp_service *ppp_service_ptr = static_cast<ppp_service *>(service_ptr);

        int unique_id = ppp_service_ptr->event_queue_get()->call(mbed::callback(callback, arg));
        if (unique_id == 0) {
            return ERR_MEM;
        }

        return ERR_OK;
    }

    pbuf *ppp_memory_buffer_allocate(void *memory_manager, uint16_t size, ppp_buf_type_e type)
    {
        ppp_service_if_mutex->lock();

        int8_t free_index = -1;
        for (int8_t index = 0; index < PPP_PBUF_HANDLE_NUM; index++) {
            if (pbuf_handles[index].buffer == NULL) {
                free_index = index;
                break;
            }
        }

        if (free_index < 0) {
            ppp_service_if_mutex->unlock();
            return NULL;
        }

        NetStackMemoryManager *mem_mngr = static_cast<NetStackMemoryManager *>(memory_manager);

        void *buffer;
        if (type == PPP_BUF_HEAP) {
            buffer = mem_mngr->alloc_heap(size, 0);
        } else {
            buffer = mem_mngr->alloc_pool(size, 0);
        }

        if (!buffer) {
            ppp_service_if_mutex->unlock();
            return NULL;
        }

        // Must be continuous buffer from pool on alloc
        if (mem_mngr->get_next(buffer) != NULL) {
            mem_mngr->free(buffer);
            ppp_service_if_mutex->unlock();
            return NULL;
        }

        pbuf_handles[free_index].next = NULL;
        pbuf_handles[free_index].buffer = buffer;
        pbuf_handles[free_index].memory_manager = memory_manager;
        pbuf_handles[free_index].payload = static_cast<uint8_t *>(mem_mngr->get_ptr(buffer));
        pbuf_handles[free_index].len = size;
        pbuf_handles[free_index].tot_len = size;
        pbuf_handles[free_index].payload_start = pbuf_handles[free_index].payload;

        ppp_service_if_mutex->unlock();

        return &pbuf_handles[free_index];
    }

    void ppp_memory_buffer_free(pbuf *buffer)
    {
        ppp_service_if_mutex->lock();

        int8_t buffer_index = -1;
        for (int8_t index = 0; index < PPP_PBUF_HANDLE_NUM; index++) {
            if (&pbuf_handles[index] == buffer) {
                buffer_index = index;
                break;
            }
        }

        if (buffer_index < 0) {
            ppp_service_if_mutex->unlock();
            return;
        }

        if (pbuf_handles[buffer_index].buffer != NULL) {
            NetStackMemoryManager *mem_mngr = static_cast<NetStackMemoryManager *>(pbuf_handles[buffer_index].memory_manager);
            mem_mngr->free(pbuf_handles[buffer_index].buffer);
        }

        memset(&pbuf_handles[buffer_index], 0, sizeof(pbuf));

        ppp_service_if_mutex->unlock();
    }

    uint16_t ppp_memory_buffer_pool_alloc_unit_get(void *memory_manager)
    {
        NetStackMemoryManager *mem_mngr = static_cast<NetStackMemoryManager *>(memory_manager);
        return mem_mngr->get_pool_alloc_unit(0);
    }

    void ppp_memory_buffer_cat(void *memory_manager, pbuf *to_buf, pbuf *cat_buf)
    {
        for (struct pbuf *buf = to_buf; buf != NULL; buf = buf->next) {
            if (buf->next == NULL) {
                buf->next = cat_buf;
                break;
            }
        }
    }

    void ppp_memory_buffer_set_len(void *memory_manager, pbuf *buf, uint16_t len)
    {
        NetStackMemoryManager *mem_mngr = static_cast<NetStackMemoryManager *>(memory_manager);
        mem_mngr->set_len(buf->buffer, len);
    }

    struct pbuf *ppp_memory_buffer_convert_to(void *memory_manager, net_stack_mem_buf_t *mem_buf)
    {
        ppp_service_if_mutex->lock();

        int8_t free_index = -1;
        for (int8_t index = 0; index < PPP_PBUF_HANDLE_NUM; index++) {
            if (pbuf_handles[index].buffer == NULL) {
                free_index = index;
                break;
            }
        }

        if (free_index < 0) {
            ppp_service_if_mutex->unlock();
            return NULL;
        }

        NetStackMemoryManager *mem_mngr = static_cast<NetStackMemoryManager *>(memory_manager);

        pbuf_handles[free_index].buffer = mem_buf;
        pbuf_handles[free_index].memory_manager = memory_manager;
        pbuf_handles[free_index].payload = mem_mngr->get_ptr(mem_buf);
        pbuf_handles[free_index].len = mem_mngr->get_len(mem_buf);
        pbuf_handles[free_index].tot_len =  mem_mngr->get_total_len(mem_buf);
        pbuf_handles[free_index].payload_start = pbuf_handles[free_index].payload;

        ppp_service_if_mutex->unlock();

        return &pbuf_handles[free_index];
    }

    net_stack_mem_buf_t *ppp_memory_buffer_convert_from(struct pbuf *p)
    {
        NetStackMemoryManager *mem_mngr = static_cast<NetStackMemoryManager *>(p->memory_manager);

        net_stack_mem_buf_t *first_buffer = p->buffer;

        struct pbuf *buf_next;
        for (struct pbuf *buf = p; buf != NULL; buf = buf_next) {

            // Set actual memory buffer length
            mem_mngr->set_len(buf->buffer, buf->len);

            // Trim the headroom away from buffer if set
            if (buf->payload_start < buf->payload) {
                memmove(buf->payload_start, buf->payload, buf->len);
            }

            buf_next = buf->next;

            // If not first buffer cat to first buffer and free
            if (buf->buffer != first_buffer) {
                mem_mngr->cat(first_buffer, buf->buffer);
                buf->buffer = NULL;
                ppp_memory_buffer_free(buf);
            }
        }

        // Going to upper levels, do not deallocate the actual buffer
        p->buffer = NULL;
        ppp_memory_buffer_free(p);

        return first_buffer;
    }

    uint8_t ppp_memory_buffer_remove_header(pbuf *buffer, uint16_t header_len)
    {
        uint8_t *payload = static_cast<uint8_t *>(buffer->payload);
        payload += header_len;
        buffer->payload = payload;
        buffer->len -= header_len;
        return 0;
    }

    uint8_t ppp_memory_buffer_add_header(struct pbuf *buffer, uint16_t header_len)
    {
        uint32_t payload_headroom_len = static_cast<uint8_t *>(buffer->payload) - static_cast<uint8_t *>(buffer->payload_start);

        if (payload_headroom_len < header_len) {
            return 1; // failure
        }

        buffer->payload = static_cast<uint8_t *>(buffer->payload) - header_len;
        buffer->len += header_len;

        return 0;
    }

    uint32_t ppp_sys_now(void)
    {
        return osKernelGetTickCount();
    }

    uint32_t ppp_sys_jiffies(void)
    {
        return ppp_sys_now();
    }

    sys_prot_t ppp_sys_arch_protect(void *service_ptr)
    {
        ppp_service *ppp_service_ptr = static_cast<ppp_service *>(service_ptr);
        ppp_service_ptr->resource_lock();

        return (sys_prot_t) 1;
    }

    void ppp_sys_arch_unprotect(void *service_ptr, sys_prot_t p)
    {
        ppp_service *ppp_service_ptr = static_cast<ppp_service *>(service_ptr);
        ppp_service_ptr->resource_unlock();
    }

#if PPP_TRACE_ENABLE
    void ppp_trace_to_ascii_hex_dump(int buffer, int len, char *data)
    {
        char prefix[10];
        trace_buf_t *trace_buffer = NULL;
        if (buffer == OUTPUT_BUFFER) {
            strcpy(prefix, "O:");
            trace_buffer = &output_trace_buffer;
        } else if (buffer == INPUT_BUFFER) {
            strcpy(prefix, "I:");
            trace_buffer = &input_trace_buffer;
        }

        static char line[100];
        memset(line, 0, 100);
        int index = 0;

        bool add_prefix = false;
        if (trace_buffer->buffer_index == 0) {
            index += sprintf(&line[index], "\n %" PRIi32 " len: %i buffer index %i line len %i \n", ppp_sys_now(), len, trace_buffer->buffer_index, trace_buffer->line_len);
            add_prefix = true;
        }

        for (int i = 0; i < len; i++) {
            if ((trace_buffer->line_len % 16) == 0) {
                if (trace_buffer->line_len != 0) {
                    index += sprintf(&line[index], "\n");
                    trace_buffer->buffer_index += sprintf(&trace_buffer->buffer[trace_buffer->buffer_index], "%s", line);
                    index = 0;
                }

                bool add_ppp_flag = false;
                if (add_prefix) {
                    if (data[i] == 0xff || (data[i] == 0x57 && (data[i + 1] == 0x60 || data[i + 1] == 0x6a))) {
                        add_ppp_flag = true;
                    }
                }

                index += sprintf(&line[index], "%s %06x%s%s", prefix, trace_buffer->line_len, add_prefix ? " 00 00 88 81" : "", add_ppp_flag ? " 7e" : "");
                if (add_prefix) {
                    trace_buffer->line_len += 4;
                }
                if (add_ppp_flag) {
                    trace_buffer->line_len += 1;
                }
                add_prefix = false;
            }
            trace_buffer->line_len++;
            index += sprintf(&line[index], " %02x", data[i]);
        }

        trace_buffer->buffer_index += sprintf(&trace_buffer->buffer[trace_buffer->buffer_index], "%s", line);
    }

    void ppp_trace_to_ascii_hex_dump_print(int buffer)
    {
        trace_buf_t *trace_buffer = NULL;
        if (buffer == OUTPUT_BUFFER) {
            trace_buffer = &output_trace_buffer;
        } else if (buffer == INPUT_BUFFER) {
            trace_buffer = &input_trace_buffer;
        }

#if PPP_TRACE_BUFFER_SIZE == 0
        tr_info("%s", trace_buffer->buffer);
#endif

#if PPP_TRACE_BUFFER_SIZE > 0
        cont_trace_buffer_index += sprintf(&cont_trace_buffer[cont_trace_buffer_index], "%s", trace_buffer->buffer);
        memset(trace_buffer->buffer, 0, BUFFER_SIZE);

        if (cont_trace_buffer_index > (PPP_TRACE_BUFFER_SIZE - PPP_TRACE_BUFFER_SIZE / 5)) {
            cont_trace_buffer_index = 0;
        }
#endif

        trace_buffer->buffer_index = 0;
        trace_buffer->line_len = 0;
    }

#endif

    void ppp_sys_timeout_callback(void *cb_ptr)
    {
        ppp_service_if_mutex->lock();

        int id = reinterpret_cast<int>(cb_ptr);

        ppp_sys_timeout_handler handler = NULL;
        void *arg = NULL;

        for (int8_t i = 0; i < PPP_TIMER_NUM; i++) {
            if (timeout[i] && timeout[i]->id == id) {
                handler = timeout[i]->handler;
                arg = timeout[i]->arg;
                free(timeout[i]);
                timeout[i] = NULL;
                //tr_info("sys timer timeout, i: %i h/a: %p %p, id: %i", i, handler, arg, timeout[i]->equeue_id);
            }
        }

        ppp_service_if_mutex->unlock();

        if (handler) {
            handler(arg);
        } else {
            tr_error("Cancelled timeout");
        }
    }

    void ppp_sys_timeout(void *service_ptr, u32_t msecs, ppp_sys_timeout_handler handler, void *arg)
    {
        ppp_service *ppp_service_ptr = static_cast<ppp_service *>(service_ptr);

        ppp_service_if_mutex->lock();

        int8_t free_index = -1;

        for (int8_t i = 0; i < PPP_TIMER_NUM; i++) {
            if (timeout[i] == NULL && free_index < 0) {
                free_index = i;
            } else if (timeout[i] != NULL && timeout[i]->handler == handler && timeout[i]->arg == arg) {
                tr_error("Timeout already set");
                ppp_service_if_mutex->unlock();
                return;
            }
        }

        if (free_index < 0) {
            tr_error("No free timeouts");
            ppp_service_if_mutex->unlock();
        }

        timeout[free_index] = static_cast<ppp_sys_timeout_t *>(malloc(sizeof(ppp_sys_timeout_t)));
        if (!timeout[free_index]) {
            tr_error("No free memory for timeout");
            ppp_service_if_mutex->unlock();
            return;
        }

        void *cb_ptr = reinterpret_cast<void *>(ppp_service_sys_timeout_id);
        int unique_id = ppp_service_ptr->event_queue_get()->call_in(msecs, mbed::callback(ppp_sys_timeout_callback, cb_ptr));
        if (unique_id == 0) {
            tr_error("No free memory for timeout equeue");
            ppp_service_if_mutex->unlock();
            return;
        }

        timeout[free_index]->ppp_drv = ppp_service_ptr;
        timeout[free_index]->handler = handler;
        timeout[free_index]->arg = arg;
        timeout[free_index]->equeue_id = unique_id;
        timeout[free_index]->id = ppp_service_sys_timeout_id++;

        //tr_info("sys timer start, i: %i h/a: %p %p, t: %" PRIx32 " id: %i", free_index, handler, arg, msecs, unique_id);

        ppp_service_if_mutex->unlock();
    }

    void ppp_sys_untimeout(ppp_sys_timeout_handler handler, void *arg)
    {
        ppp_service_if_mutex->lock();

        for (int8_t i = 0; i < PPP_TIMER_NUM; i++) {
            if (timeout[i] != NULL && timeout[i]->handler == handler && timeout[i]->arg == arg) {
                int unique_id = timeout[i]->equeue_id;
                timeout[i]->ppp_drv->event_queue_get()->cancel(unique_id);
                //tr_info("sys timer cancel, i: %i h/a: %p %p, id: %i", i, timeout[i]->handler, timeout[i]->arg, unique_id);
                free(timeout[i]);
                timeout[i] = NULL;
            }
        }

        ppp_service_if_mutex->unlock();
    }

} // extern "C"

#if PPP_TRACE_ENABLE && PPP_TRACE_BUFFER_SIZE > 0
void ppp_trace_to_ascii_hex_dump_print_final(void)
{
    int start_i = 0;

    for (int i = 0; i < PPP_TRACE_BUFFER_SIZE; i++) {
        if (cont_trace_buffer[i] == '\n' || cont_trace_buffer[i] == '\0') {
            memcpy(output_trace_buffer.buffer, &cont_trace_buffer[start_i], i - start_i);
            output_trace_buffer.buffer[i - start_i] = 0;

            tr_info("%s", output_trace_buffer.buffer);

            if (cont_trace_buffer[i] == '\0') {
                break;
            }

            start_i = i + 1;
        }
    }
}
#endif

#endif
