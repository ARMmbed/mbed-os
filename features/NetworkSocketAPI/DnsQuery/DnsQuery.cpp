/* Copyright (c) 2013 Henry Leinen (henry[dot]leinen [at] online [dot] de)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "DnsQuery.h"
#include <stdio.h>
#include <string.h>


#define DNS_COUNT (sizeof DNS_IPS / sizeof DNS_IPS[0])
const char *DNS_IPS[] = {
    "8.8.8.8",
    "209.244.0.3",
    "84.200.69.80",
    "8.26.56.26",
    "208.67.222.222"
};

static bool isIP(const char *host)
{
    int i;

    for (i = 0; host[i]; i++) {
        if (!(host[i] >= '0' && host[i] <= '9') && host[i] != '.') {
            return false;
        }
    }

    // Ending with '.' garuntees host
    if (i > 0 && host[i-1] == '.') {
        return false;
    }

    return true;
}


static bool parseRR(uint8_t *resp, int &c, char *adr)
{
    int n = 0;
    while((n=resp[c++]) != 0) {
        if ((n & 0xc0) != 0) {
            //  This is a link
            c++;
            break;
        } else {
            //  skip this segment, not interested in string domain names
            c+= n;
        }
    }

    int TYPE = (((int)resp[c])<<8) + resp[c+1];
    int CLASS = (((int)resp[c+2])<<8) + resp[c+3];
    int RDLENGTH = (((int)resp[c+8])<<8) + resp[c+9];

    c+= 10;
    if ((CLASS == 1) && (TYPE == 1)) {
        sprintf(adr,"%d.%d.%d.%d", resp[c], resp[c+1], resp[c+2], resp[c+3]);
        c+= RDLENGTH;
        return true;
    }
    c+= RDLENGTH;

    return false;
}


static bool resolve(unsigned char *resp, char *ipaddress)
{

    int ID = (((int)resp[0]) <<8) + resp[1];
    int QR = resp[2] >>7;
    int Opcode = (resp[2]>>3) & 0x0F;
    int RCODE = (resp[3] & 0x0F);
    int ANCOUNT = (((int)resp[6])<<8)+ resp[7];

    if ((ID != 1) || (QR != 1) || (Opcode != 0) || (RCODE != 0)) {
        return false;
    }

    int c = 12;
    int d;
    //  Skip domain question
    while(  (d=resp[c++]) != 0) {
        c+=d;
    }
    c+= 4; //   skip QTYPE and QCLASS

    //  Here comes the resource record
    for (int ans = 0 ; ans < ANCOUNT; ans++) {
        if (parseRR(resp, c, ipaddress)) {
            return true;
        }
    }

    return false;
}

static int32_t query(UDPSocket *socket, const SocketAddress &addr, const char *hostname, char *ipaddress)
{
    int len = 0;
    if (hostname == NULL) {
        return false;
    }
    len = strlen(hostname);
    if ((len > 128) || (len == 0)) {
        return false;
    }

    int packetlen = /* size of HEADER structure */ 12 + /* size of QUESTION Structure */5 + len + 1;
    uint8_t *packet = new uint8_t[packetlen]; /* this is the UDP packet to send to the DNS */
    if (packet == NULL) {
        return false;
    }

    //  Fill the header
    memset(packet, 0, packetlen);
    packet[1] = 1;      //  ID = 1
    packet[5] = 1;      // QDCOUNT = 1 (contains one question)
    packet[2] = 1;      // recursion requested

    int c = 13;         //  point to NAME element in question section or request
    int cnt = 12;       //  points to the counter of
    packet[cnt] = 0;
    for (int i = 0 ; i < len ; i++) {
        if (hostname[i] != '.') {
            //  Copy the character and increment the character counter
            packet[cnt]++;
            packet[c++] = hostname[i];
        } else {
            //  Finished with this part, so go to the next
            cnt = c++;
            packet[cnt] = 0;
        }
    }

    //  Terminate this domain name with a zero entry
    packet[c++] = 0;

    //  Set QTYPE
    packet[c++] = 0;
    packet[c++] = 1;
    //  Set QCLASS
    packet[c++] = 0;
    packet[c++] = 1;


    if (socket->sendto(addr, packet, packetlen) < 0) {
        delete packet;
        return false;
    }
    delete packet;

    packet = new uint8_t [1024];

    //  Receive the answer from DNS
    int response_length = 0;
    response_length = socket->recvfrom(NULL, packet, 1024);

    if (response_length > 0 ) {
        if (!resolve(packet, ipaddress)) {
            delete packet;
            return NSAPI_ERROR_DNS_FAILURE;
        }
        
        //  cleanup and return
        delete packet;
        return 0;
    }
    
    delete packet;
    return NSAPI_ERROR_DNS_FAILURE;
}

int32_t dnsQuery(NetworkStack *iface, const char *host, char *ip)
{
    if (isIP(host)) {
        strcpy(ip, host);
        return 0;
    }

    UDPSocket sock(iface);

    for (unsigned i = 0; i < DNS_COUNT; i++) {
        return query(&sock, SocketAddress(DNS_IPS[0], 53), host, ip);
    }

    return NSAPI_ERROR_DNS_FAILURE;
}

int32_t dnsQuery(UDPSocket *socket, const char *host, char *ip)
{
    if (isIP(host)) {
        strcpy(ip, host);
        return 0;
    }

    for (unsigned i = 0; i < DNS_COUNT; i++) {
        return query(socket, SocketAddress(DNS_IPS[0], 53), host, ip);
    }

    return NSAPI_ERROR_DNS_FAILURE;
}
