#include "ControlPlane_netif.h"
#include "ATHandler.h"
#include "AT_CellularBase.h"

namespace mbed {

class AT_ControlPlane_netif: public ControlPlane_netif, public AT_CellularBase {
public:
    AT_ControlPlane_netif(ATHandler &at, int cid);
    virtual ~AT_ControlPlane_netif();

protected:

// ControlPlane_netif

    /* Sends data using +CSODCP specified in
     *  3GPP 27007 10.1.43: Sending of originating data via the control plane
     */
    virtual nsapi_size_or_error_t send(const void *cpdata, nsapi_size_t cpdata_length);


    /* Receives data using +CRTDCP specified in
     *  3GPP 27007 10.1.44: Reporting of terminating data via the control plane
     */
    virtual nsapi_size_or_error_t recv(void *cpdata, nsapi_size_t cpdata_length);

    virtual void data_received();
    virtual void attach(void (*callback)(void *), void *data);

    // Id of the PDP context that enables the control plane data connection
    int _cid;

private:
    void (*_cb)(void *);
    void *_data;
    char _recv_buffer[MAX_CP_DATA_RECV_LEN];
    size_t _recv_len;
    // Called on receiving URC: +CRTDCP
    void urc_cp_recv();
};

} //mbed namespace
