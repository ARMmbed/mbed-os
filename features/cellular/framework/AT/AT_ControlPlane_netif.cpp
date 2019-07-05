/*AT_ControlPlane_netif.cpp*/
#include "AT_ControlPlane_netif.h"

namespace mbed {

AT_ControlPlane_netif::AT_ControlPlane_netif(ATHandler &at, int cid) : AT_CellularBase(at),
    _cid(cid), _cb(NULL), _data(NULL), _recv_len(0)
{
    _at.set_urc_handler("+CRTDCP:", mbed::Callback<void()>(this, &AT_ControlPlane_netif::urc_cp_recv));
}

AT_ControlPlane_netif::~AT_ControlPlane_netif()
{}

void AT_ControlPlane_netif::urc_cp_recv()
{
    //+CRTDCP: <cid>,<cpdata_length>,<cpdata>
    _at.lock();
    int cid = _at.read_int();
    int cpdata_length = _at.read_int();
    int read_len = _at.read_string(_recv_buffer, sizeof(_recv_buffer));

    _at.unlock();

    // cid not expected to be different because: one context - one file handle
    // so this file handle cannot get urc from different context
    if (read_len > 0 && read_len == cpdata_length && cid == _cid) {
        _recv_len = read_len;
        data_received();
    }
}

nsapi_size_or_error_t AT_ControlPlane_netif::send(const void *cpdata, nsapi_size_t cpdata_length)
{
    //CSODCP
    _at.lock();

    nsapi_size_or_error_t err = _at.at_cmd_discard("+CSODCP", "=", "%d%d%b", _cid, cpdata_length, cpdata, cpdata_length);

    return (err == NSAPI_ERROR_OK) ? cpdata_length : err;
}

nsapi_size_or_error_t AT_ControlPlane_netif::recv(void *cpdata, nsapi_size_t cpdata_length)
{
    // If no data received through CRTDCP URC
    if (!_recv_len) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    // If too small buffer for data
    if (_recv_len > cpdata_length) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    memcpy(cpdata, _recv_buffer, _recv_len);
    size_t recv = _recv_len;
    _recv_len = 0;
    return recv;
}

void AT_ControlPlane_netif::attach(void (*callback)(void *), void *data)
{
    _cb = callback;
    _data = data;
}

void AT_ControlPlane_netif::data_received()
{
    // call socket event
    if (!_cb) {
        return;
    }
    _cb(_data);
}

} //mbed namespace
