#include "HTTPSClient.h"
#include "HTTPHeader.h"
#include <string>
#include <cstring>
#include "mbed.h"
#include <stdlib.h>
#include <stdio.h>

using std::memset;
using std::memcpy;    
using std::string;

const static int HTTPS_PORT = 443;
char buf[256];

HTTPSClient::HTTPSClient() :
        _is_connected(false),
        _ssl_ctx(),
        _ssl(),
        _host() {
}

HTTPSClient::~HTTPSClient() {
    close();
}

int HTTPSClient::connect(const char* host) {
    if (init_socket(SOCK_STREAM) < 0)
        return -1;
    
    if (set_address(host, HTTPS_PORT) != 0)
        return -1;
    
    if (lwip_connect(_sock_fd, (const struct sockaddr *) &_remoteHost, sizeof(_remoteHost)) < 0) {
        close();
        return -1;
    }

    if(ssl_ctx_new(&_ssl_ctx, SSL_SERVER_VERIFY_LATER|SSL_DISPLAY_BYTES|SSL_DISPLAY_STATES, SSL_DEFAULT_CLNT_SESS) != &_ssl_ctx)
        return -1;

    _ssl.ssl_ctx = &_ssl_ctx;
    
    if(ssl_client_new(&_ssl, _sock_fd, NULL, 0) == NULL)
    {
        close();
        return -1;
    }
    if(_ssl.hs_status != SSL_OK)
    {
        close();
        return -1;
    }
    
    _is_connected = true;
    _host = host;
    return 0;
}

bool HTTPSClient::is_connected(void) {
    return _is_connected;
}

int HTTPSClient::send(char* data, int length) {
    if ((_sock_fd < 0) || !_is_connected)
        return -1;
            
    return ssl_write(&_ssl, (uint8_t*)data, length);
}



HTTPHeader HTTPSClient::get(char *request)
{
    if((_sock_fd < 0) || !_is_connected)
        return HTTPHeader();
        
    sprintf(buf, "GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n", request, _host.c_str(), get_port());
    printf("buf=%s\n", buf);
    if(send(buf, strlen(buf)) != strlen(buf))
        return HTTPHeader();
    printf("Finished sending request\n");
    return read_header();
}


HTTPHeader HTTPSClient::read_header()
{
    _ssl.bm_read_index = 0;
    
    HTTPHeader hdr;
    read_line();
    
    int status;
    
    if(sscanf(buf, "HTTP/%*d.%*d %d %*s", &status) == -1)
        return hdr;
    
    if(status == 200)
        hdr._status = HTTP_OK;
    printf("status=%d\n", status);
    read_line();
    do
    {
        string name, value;
        int ret = sscanf(buf, "%s:%s", name, value); 
        printf("sscanf return=%d\n", ret);
        printf("name=%s\n", name);
        printf("value=%s\n", value);
        hdr._fields[name] = value;
        read_line();
        printf("strlen(buf)=%d\n", strlen(buf));
    }while(strlen(buf));
    
    return hdr;
}

void HTTPSClient::read_line()
{
    int index = 0;
    do
    {
        if(ssl_read(&_ssl, (uint8_t*)(&buf[index]), 1))
        {
            // error
        }
        
    }while(buf[index++] != '\n' && index < 255);
    buf[index-1] = '\0';
    printf("line=%s\n", buf);
}

// -1:error
// otherwise return nb of characters read. Cannot be > than len
int HTTPSClient::read(char *data, int len)
{
    return ssl_read(&_ssl, (uint8_t*)data, len);
}
/*
    0    : must close connection
    -1   : error
    else : get data

int HTTPSClient::receive(char* data, int length) {
    if ((_sock_fd < 0) || !_is_connected)
        return -1;
  
    if(read_record(&_ssl) < 0)
        return -1;
    return process_data(&_ssl, (uint8_t*)data, length);
}
*/
void HTTPSClient::close()
{
    if(!_is_connected)
        return;
    ssl_ctx_free(_ssl.ssl_ctx);
    Socket::close();
    _is_connected = false;
    _host.clear();
}
