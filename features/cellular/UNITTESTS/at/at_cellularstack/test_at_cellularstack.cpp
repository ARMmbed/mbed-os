/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularstack.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularStack.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;

uint32_t mbed::cellular_log_time = 0;

class MyStack : public AT_CellularStack {
public:
    MyStack(ATHandler &atr, int cid, nsapi_ip_stack_t typ) : AT_CellularStack(atr, cid, typ){}

    virtual int get_max_socket_count(){return 0;}

    virtual int get_max_packet_size(){return 0;}

    virtual bool is_protocol_supported(nsapi_protocol_t protocol){return false;}

    virtual nsapi_error_t socket_close_impl(int sock_id){return NSAPI_ERROR_OK;}

    virtual nsapi_error_t create_socket_impl(CellularSocket *socket){return NSAPI_ERROR_OK;}

    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
            const void *data, nsapi_size_t size){return NSAPI_ERROR_OK;}

    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
            void *buffer, nsapi_size_t size) {return NSAPI_ERROR_OK;}
};

Test_AT_CellularStack::Test_AT_CellularStack()
{

}

Test_AT_CellularStack::~Test_AT_CellularStack()
{
}

void Test_AT_CellularStack::test_AT_CellularStack_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    MyStack *st = new MyStack(at, 0, IPV4_STACK);

    delete st;
}

void Test_AT_CellularStack::test_AT_CellularStack_get_ip_address()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_open()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_close()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_bind()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_listen()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_connect()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_accept()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_send()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_recv()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_sendto()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_recvfrom()
{

}

void Test_AT_CellularStack::test_AT_CellularStack_socket_attach()
{

}
