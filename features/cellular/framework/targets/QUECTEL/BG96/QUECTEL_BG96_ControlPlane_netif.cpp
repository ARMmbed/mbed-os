#include "QUECTEL_BG96_ControlPlane_netif.h"

namespace mbed {

QUECTEL_BG96_ControlPlane_netif::QUECTEL_BG96_ControlPlane_netif(ATHandler &at, int cid) : AT_ControlPlane_netif(at, cid)
{}

nsapi_size_or_error_t QUECTEL_BG96_ControlPlane_netif::send(const void *data, nsapi_size_t size)
{
    _at.lock();

    _at.cmd_start("AT+QCFGEXT=\"nipds\",0,");
    _at.write_string((char *)data);
    _at.write_int(size);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    nsapi_error_t err = _at.get_last_error();

    _at.unlock();

    if (err == NSAPI_ERROR_OK) {
        return size;
    }

    return err;
}

nsapi_size_or_error_t QUECTEL_BG96_ControlPlane_netif::recv(void *buffer, nsapi_size_t size)
{
    _at.lock();

    _at.cmd_start("AT+QCFGEXT=\"nipdr\",0");
    _at.cmd_stop();
    _at.resp_start("+QCFGEXT: ");
    // skip 3 params: "nipdr",<total_receive_length>,<have_read_length>
    _at.skip_param(3);
    // get to <unread_length>
    int unread_length = _at.read_int();
    _at.resp_stop();

    if (!unread_length || unread_length == -1) {
        _at.unlock();
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    _at.cmd_start("AT+QCFGEXT=\"nipdr\",");
    _at.write_int(unread_length);
    _at.cmd_stop();

    _at.resp_start("+QCFGEXT:");
    // skip "nipdr"
    _at.skip_param();
    int read_length = _at.read_int();
    _at.read_string((char *)buffer, read_length);
    _at.resp_stop();
    nsapi_error_t err = _at.get_last_error();
    _at.unlock();

    if (err == NSAPI_ERROR_OK && read_length) {
        return read_length;
    }

    return NSAPI_ERROR_WOULD_BLOCK;
}

} // mbed namespace
