#include "ControlPlane_netif.h"
#include "ATHandler.h"
#include "AT_CellularBase.h"

namespace mbed {

class AT_ControlPlane_netif: public ControlPlane_netif, public AT_CellularBase {
public:
    AT_ControlPlane_netif(ATHandler &at, int cid);
    virtual ~AT_ControlPlane_netif();

// ControlPlane_netif
    // +CSODCP: 3GPP 27007 10.1.43
    virtual nsapi_size_or_error_t send(const void *cpdata, nsapi_size_t cpdata_length);
    // +CRTDCP: 3GPP 27007 10.1.44
    virtual nsapi_size_or_error_t recv(void *cpdata, nsapi_size_t cpdata_length);
    virtual void attach(void (*callback)(void *), void *data);
    virtual void data_received();

protected:
    // Id of the PDP context that enables the control plane data connection
    int _cid;

private:
    void (*_cb)(void *);
    void *_data;
    char _recv_buffer[MAX_CP_DATA_RECV_LEN];
    size_t _recv_len;
    void urc_cp_recv();
};

} //mbed namespace
