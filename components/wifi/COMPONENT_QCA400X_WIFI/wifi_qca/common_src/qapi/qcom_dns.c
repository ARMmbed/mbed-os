/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
***************************************************************************
*
*  DNS Resolver implementation.
*
***************************************************************************/

#include "wifi_common.h"
#include "qcom_api.h"
#include "atheros_wifi.h"

#define WIFI_DNS_HEADER_FLAGS_QR    (0x8000U)
#define WIFI_DNS_HEADER_FLAGS_AA    (0x0400U)
#define WIFI_DNS_HEADER_FLAGS_TC    (0x0200U)
#define WIFI_DNS_HEADER_FLAGS_RD    (0x0100U)
#define WIFI_DNS_HEADER_FLAGS_RA    (0x0080U)
#define WIFI_DNS_TYPE_A             (0x0001U)
#define WIFI_DNS_HEADER_CLASS_IN    (0x01U)

#define WIFI_MAX_DNS_NAME_LENGTH    (253)

/* Taken from FNET */
typedef PREPACK struct
{
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} POSTPACK wifi_dns_header_t;

/* Taken from FNET */
typedef PREPACK struct
{
    uint8_t   zero_length;
    uint16_t  qtype;
    uint16_t  qclass;

} POSTPACK wifi_dns_q_tail_t;

/* Taken from FNET */
typedef PREPACK struct
{
    PREPACK union
    {
        uint8_t   name_ptr_c[2];
        uint16_t  name_ptr;
    } POSTPACK name;
    uint16_t   type;
    uint16_t   rr_class;
    uint32_t   ttl;
    uint16_t   rdlength;
    uint32_t   rdata;

} POSTPACK wifi_dns_rr_header_t;

static int32_t wifi_dns_assemble_request(char *buffer, char *hostname, uint32_t buff_len)
{
    uint32_t total_length = 0, label_length = 0;
    wifi_dns_q_tail_t   *q_tail;
    /* Separate strings by '\0'*/
    uint32_t hostname_len = strlen(hostname);

    /* Buffer too small to handle 'hostname' */
    if ((buff_len < 0) || (buff_len <= hostname_len + sizeof(wifi_dns_q_tail_t)))
        return -1;

    strcpy(&buffer[1], hostname);
    buffer[0] = '\0';

    /* Force '\0' delimiter */
    for (int32_t i = 0; i < hostname_len + 1; i++)
    {
        if ('.' == buffer[i])
            buffer[i] = '\0';
    }

    /* Replace '\0' with string length */
    total_length = 0;
    while ((label_length = strlen(&buffer[total_length + 1])) > 0)
    {
        buffer[total_length] = label_length;
        total_length += label_length + 1;
    }

    q_tail = (wifi_dns_q_tail_t *)&buffer[total_length];

    /* QTYPE */
    q_tail->qtype = A_CPU2BE16(WIFI_DNS_TYPE_A);

    /* QCLASS */
    q_tail->qclass = A_CPU2BE16(WIFI_DNS_HEADER_CLASS_IN);

    return (total_length + sizeof(wifi_dns_q_tail_t));
}


static uint16_t wifi_id_cnt = 1;


A_STATUS qcom_dns_resolver(IP_ADDR_T dns_ip, char * hostname, IP_ADDR_T *host_ip, uint32_t timeout_ms)
{
    int32_t dns_sock = -1;
    SOCKADDR_T dns_addr = {0};
    char * tx_buffer = NULL;
    uint32_t tx_len = 0;
    char * rx_buffer = NULL;
    wifi_dns_rr_header_t *dns_resp = NULL;
    wifi_dns_header_t *dns_hdr = NULL;
    int32_t result = 0;
    uint16_t dns_addr_len = sizeof(dns_addr);
    A_STATUS status = A_ERROR;
    int32_t tmp_len = 0;

    const uint32_t rx_buffer_max = 512;
    /* The total DNS query message is the size of the header + a null character before the host name + the maximum host name
     * length + the size of the tail */
    const uint32_t tx_buffer_max = sizeof(wifi_dns_header_t) + 1 + WIFI_MAX_DNS_NAME_LENGTH + sizeof(wifi_dns_q_tail_t);

    do {
        /* Try to allocate buffer for request, must be fill with '\0' */
        tx_buffer = (char *)CUSTOM_ALLOC(tx_buffer_max);
        memset(tx_buffer, 0, tx_buffer_max);
        if (NULL == tx_buffer)
            break;

        /* Set DNS address */
        /* qcom code is expecting the port, address, and family in host order */
        dns_addr.sin_port = 53;
        dns_addr.sin_addr.s_addr = dns_ip.s_addr;
        dns_addr.sin_family = ATH_AF_INET;

        /* Create UDP socket */
        dns_sock = qcom_socket(ATH_AF_INET, SOCK_DGRAM_TYPE, 0);
        if (-1 == dns_sock)
            break;

        /* Prepare request header */
        dns_hdr = (wifi_dns_header_t*)&tx_buffer[0];
        dns_hdr->id = wifi_id_cnt++;
        dns_hdr->flags = A_CPU2BE16(WIFI_DNS_HEADER_FLAGS_RD);
        dns_hdr->qdcount = A_CPU2BE16(1U);

        /* Prepare request body */
        tx_len = sizeof(wifi_dns_header_t);
        if (0 > (tmp_len = wifi_dns_assemble_request(&tx_buffer[ tx_len ], hostname, tx_buffer_max - tx_len)))
            break;
        tx_len += tmp_len;

        /* Send data over UDP */
        result = qcom_sendto(dns_sock, tx_buffer, tx_len, 0, (void *)(&dns_addr), sizeof(dns_addr));
        if (-1 == result)
            break;

        /* Free TX resources*/
        CUSTOM_FREE(tx_buffer);
        tx_buffer = NULL;

        /* Obtain WiFi context */
        QCA_CONTEXT_STRUCT *qcaCtx = Custom_Api_GetDriverCxt(0);
        if (NULL == qcaCtx)
            break;

        /* Block on receive max 'timeout_ms' */
        result = t_select(qcaCtx, dns_sock, timeout_ms);
        if (A_OK != result)
            break;

        /* Receive data from DNS server */
        result = qcom_recvfrom(dns_sock, &rx_buffer, rx_buffer_max, 0, (struct sockaddr *)&dns_addr, &dns_addr_len);
        if (-1 == result)
          break;

        /* Typecast header */
        dns_hdr = (wifi_dns_header_t*)rx_buffer;

        /* Try to find response (should be always at addr '&rx_buffer[ tx_len ]') */
        for (tx_len = sizeof(wifi_dns_header_t); tx_len < result; tx_len++)
        {
            /* However, more stable solution is to find DNS_NAME_COMPRESSED_MASK (0xC) in stream (according to FNET) */
            if (0xC0 == (uint8_t)rx_buffer[tx_len])
            {
                /* Typecast response */
                dns_resp = (wifi_dns_rr_header_t*)&rx_buffer[ tx_len ];
                /* Expected IPv4 response*/
                if (
                    (dns_resp->rr_class == A_CPU2BE16(WIFI_DNS_HEADER_CLASS_IN)) &&
                    (dns_resp->type == A_CPU2BE16(WIFI_DNS_TYPE_A))
                 )
                {
                    /* Get IPv4 address */
                    /* Uncomment in case of wrong byte order */
                    // host_ip->s_addr = A_CPU2BE32(dns_resp->rdata);
                    host_ip->s_addr = dns_resp->rdata;
                    status = A_OK;
                    break;
                }
            }
        }
    } while(0);

    /* Cleanup */
    /* Free TX buffer */
    if (NULL != tx_buffer)
    {
        CUSTOM_FREE(tx_buffer);
        tx_buffer = NULL;
    }
    /* Free RxBuffer */
    if (NULL != rx_buffer)
    {
        zero_copy_free(rx_buffer);
        rx_buffer = NULL;
    }
    /* Close socket */
    if (-1 != dns_sock)
    {
        qcom_socket_close(dns_sock);
        dns_sock = -1;
    }

    return status;
}



