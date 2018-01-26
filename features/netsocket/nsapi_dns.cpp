/* nsapi_dns.cpp
 * Original work Copyright (c) 2013 Henry Leinen (henry[dot]leinen [at] online [dot] de)
 * Modified work Copyright (c) 2015 ARM Limited
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
#include "nsapi_dns.h"
#include "netsocket/UDPSocket.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CLASS_IN 1

#define RR_A 1
#define RR_AAAA 28

// DNS options
#define DNS_BUFFER_SIZE 512
#define DNS_TIMEOUT 5000
#define DNS_SERVERS_SIZE 5

nsapi_addr_t dns_servers[DNS_SERVERS_SIZE] = {
    {NSAPI_IPv4, {8, 8, 8, 8}},                             // Google
    {NSAPI_IPv4, {209, 244, 0, 3}},                         // Level 3
    {NSAPI_IPv4, {84, 200, 69, 80}},                        // DNS.WATCH
    {NSAPI_IPv6, {0x20,0x01, 0x48,0x60, 0x48,0x60, 0,0,     // Google
                  0,0, 0,0, 0,0, 0x88,0x88}},
    {NSAPI_IPv6, {0x20,0x01, 0x16,0x08, 0,0x10, 0,0x25,     // DNS.WATCH
                  0,0, 0,0, 0x1c,0x04, 0xb1,0x2f}},
};

// DNS server configuration
extern "C" nsapi_error_t nsapi_dns_add_server(nsapi_addr_t addr)
{
    memmove(&dns_servers[1], &dns_servers[0],
            (DNS_SERVERS_SIZE-1)*sizeof(nsapi_addr_t));

    dns_servers[0] = addr;
    return NSAPI_ERROR_OK;
}


// DNS packet parsing
static void dns_append_byte(uint8_t **p, uint8_t byte)
{
    *(*p)++ = byte;
}

static void dns_append_word(uint8_t **p, uint16_t word)
{

    dns_append_byte(p, 0xff & (word >> 8));
    dns_append_byte(p, 0xff & (word >> 0));
}

static void dns_append_name(uint8_t **p, const char *name, uint8_t len)
{
    dns_append_byte(p, len);
    memcpy(*p, name, len);
    *p += len;
}

static uint8_t dns_scan_byte(const uint8_t **p)
{
    return *(*p)++;
}

static uint16_t dns_scan_word(const uint8_t **p)
{
    uint16_t a = dns_scan_byte(p);
    uint16_t b = dns_scan_byte(p);
    return (a << 8) | b;
}


static void dns_append_question(uint8_t **p, const char *host, nsapi_version_t version)
{
    // fill the header
    dns_append_word(p, 1);      // id      = 1
    dns_append_word(p, 0x0100); // flags   = recursion required
    dns_append_word(p, 1);      // qdcount = 1
    dns_append_word(p, 0);      // ancount = 0
    dns_append_word(p, 0);      // nscount = 0
    dns_append_word(p, 0);      // arcount = 0

    // fill out the question names
    while (host[0]) {
        size_t label_len = strcspn(host, ".");
        dns_append_name(p, host, label_len);
        host += label_len + (host[label_len] == '.');
    }

    dns_append_byte(p, 0);

    // fill out question footer
    if (version != NSAPI_IPv6) {
        dns_append_word(p, RR_A);       // qtype  = ipv4
    } else {
        dns_append_word(p, RR_AAAA);    // qtype  = ipv6
    }
    dns_append_word(p, CLASS_IN);
}

static int dns_scan_response(const uint8_t **p, nsapi_addr_t *addr, unsigned addr_count)
{
    // scan header
    uint16_t id    = dns_scan_word(p);
    uint16_t flags = dns_scan_word(p);
    bool    qr     = 0x1 & (flags >> 15);
    uint8_t opcode = 0xf & (flags >> 11);
    uint8_t rcode  = 0xf & (flags >>  0);

    uint16_t qdcount = dns_scan_word(p); // qdcount
    uint16_t ancount = dns_scan_word(p); // ancount
    dns_scan_word(p);                    // nscount
    dns_scan_word(p);                    // arcount

    // verify header is response to query
    if (!(id == 1 && qr && opcode == 0 && rcode == 0)) {
        return 0;
    }

    // skip questions
    for (int i = 0; i < qdcount; i++) {
        while (true) {
            uint8_t len = dns_scan_byte(p);
            if (len == 0) {
                break;
            }

            *p += len;
        }

        dns_scan_word(p); // qtype
        dns_scan_word(p); // qclass
    }

    // scan each response
    unsigned count = 0;

    for (int i = 0; i < ancount && count < addr_count; i++) {
        while (true) {
            uint8_t len = dns_scan_byte(p);
            if (len == 0) {
                break;
            } else if (len & 0xc0) { // this is link
                dns_scan_byte(p);
                break;
            }

            *p += len;
        }

        uint16_t rtype    = dns_scan_word(p); // rtype
        uint16_t rclass   = dns_scan_word(p); // rclass
        *p += 4;                              // ttl
        uint16_t rdlength = dns_scan_word(p); // rdlength

        if (rtype == RR_A && rclass == CLASS_IN && rdlength == NSAPI_IPv4_BYTES) {
            // accept A record
            addr->version = NSAPI_IPv4;
            for (int i = 0; i < NSAPI_IPv4_BYTES; i++) {
                addr->bytes[i] = dns_scan_byte(p);
            }

            addr += 1;
            count += 1;
        } else if (rtype == RR_AAAA && rclass == CLASS_IN && rdlength == NSAPI_IPv6_BYTES) {
            // accept AAAA record
            addr->version = NSAPI_IPv6;
            for (int i = 0; i < NSAPI_IPv6_BYTES; i++) {
                addr->bytes[i] = dns_scan_byte(p);
            }

            addr += 1;
            count += 1;
        } else {
            // skip unrecognized records
            *p += rdlength;
        }
    }

    return count;
}

// core query function
static nsapi_size_or_error_t nsapi_dns_query_multiple(NetworkStack *stack, const char *host,
        nsapi_addr_t *addr, unsigned addr_count, nsapi_version_t version)
{
    // check for valid host name
    int host_len = host ? strlen(host) : 0;
    if (host_len > 128 || host_len == 0) {
        return NSAPI_ERROR_PARAMETER;
    }

    // create a udp socket
    UDPSocket socket;
    int err = socket.open(stack);
    if (err) {
        return err;
    }

    socket.set_timeout(DNS_TIMEOUT);

    // create network packet
    uint8_t * const packet = (uint8_t *)malloc(DNS_BUFFER_SIZE);
    if (!packet) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    nsapi_size_or_error_t result = NSAPI_ERROR_DNS_FAILURE;

    // check against each dns server
    for (unsigned i = 0; i < DNS_SERVERS_SIZE; i++) {
        // send the question
        uint8_t *question = packet;
        dns_append_question(&question, host, version);

        err = socket.sendto(SocketAddress(dns_servers[i], 53), packet, question - packet);
        // send may fail for various reasons, including wrong address type - move on
        if (err < 0) {
            continue;
        }

        // recv the response
        err = socket.recvfrom(NULL, packet, DNS_BUFFER_SIZE);
        if (err == NSAPI_ERROR_WOULD_BLOCK) {
            continue;
        } else if (err < 0) {
            result = err;
            break;
        }

        const uint8_t *response = packet;
        int count = dns_scan_response(&response, addr, addr_count);
        if (count > 0) {
            result = count;
        }

        /* The DNS response is final, no need to check other servers */
        break;
    }

    // clean up packet
    free(packet);

    // clean up udp
    err = socket.close();
    if (err) {
        return err;
    }

    // return result
    return result;
}

// convenience functions for other forms of queries
extern "C" nsapi_size_or_error_t nsapi_dns_query_multiple(nsapi_stack_t *stack, const char *host,
        nsapi_addr_t *addr, nsapi_size_t addr_count, nsapi_version_t version)
{
    NetworkStack *nstack = nsapi_create_stack(stack);
    return nsapi_dns_query_multiple(nstack, host, addr, addr_count, version);
}

nsapi_size_or_error_t nsapi_dns_query_multiple(NetworkStack *stack, const char *host,
        SocketAddress *addresses, nsapi_size_t addr_count, nsapi_version_t version)
{
    nsapi_addr_t *addrs = new nsapi_addr_t[addr_count];
    nsapi_size_or_error_t result = nsapi_dns_query_multiple(stack, host, addrs, addr_count, version);

    if (result > 0) {
        for (int i = 0; i < result; i++) {
            addresses[i].set_addr(addrs[i]);
        }
    }

    delete[] addrs;
    return result;
}

extern "C" nsapi_error_t nsapi_dns_query(nsapi_stack_t *stack, const char *host,
        nsapi_addr_t *addr, nsapi_version_t version)
{
    NetworkStack *nstack = nsapi_create_stack(stack);
    nsapi_size_or_error_t result = nsapi_dns_query_multiple(nstack, host, addr, 1, version);
    return (nsapi_error_t)((result > 0) ? 0 : result);
}

nsapi_error_t nsapi_dns_query(NetworkStack *stack, const char *host,
        SocketAddress *address, nsapi_version_t version)
{
    nsapi_addr_t addr;
    nsapi_size_or_error_t result = nsapi_dns_query_multiple(stack, host, &addr, 1, version);
    address->set_addr(addr);
    return (nsapi_error_t)((result > 0) ? 0 : result);
}
