/*
 * Copyright (c) 2015 ARM Limited. All Rights Reserved.
 */
#include "socket_api.h"
#include "socket_api_stub.h"

socket_api_stub_data_t socket_api_stub;

int8_t socket_raw_open(void (*passed_fptr)(void *))
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *))
{
    socket_api_stub.recv_cb = passed_fptr;
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}

int8_t socket_free(int8_t socket)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_listen(int8_t socket)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_connect(int8_t socket, ns_address_t *address, uint8_t randomly_take_src_number)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_bind(int8_t socket, const ns_address_t *address)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_close(int8_t socket, ns_address_t *address)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_send(int8_t socket, uint8_t *buffer, uint16_t length)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int16_t socket_read(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length)
{
    if( address ){
        memset(&address->address, 0, 16);
        address->identifier = 0;
    }
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_sendto(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_read_session_address(int8_t socket, ns_address_t *address)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
int8_t socket_getsockopt(int8_t socket, uint8_t level, uint8_t opt_name, void *opt_value, uint16_t *opt_len)
{
    if( socket_api_stub.counter >= 0){
        return socket_api_stub.values[socket_api_stub.counter--];
    }

    return socket_api_stub.int8_value;
}
