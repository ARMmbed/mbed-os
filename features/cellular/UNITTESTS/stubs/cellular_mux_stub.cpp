
#include "cellular_mux.h"
#include "cellular_mux_stub.h"

using namespace mbed;

ssize_t cellular_mux_stub::size_value = 0;
Mux::MuxReturnStatus cellular_mux_stub::status_value = Mux::MUX_STATUS_SUCCESS;


void Mux::module_init()
{

}

Mux::MuxReturnStatus Mux::mux_start(MuxEstablishStatus &status)
{
    return cellular_mux_stub::status_value;
}

Mux::MuxReturnStatus Mux::dlci_establish(uint8_t dlci_id, MuxEstablishStatus &status, FileHandle **obj)
{
    return cellular_mux_stub::status_value;
}

void Mux::serial_attach(FileHandle *serial)
{

}

void Mux::eventqueue_attach(events::EventQueue *event_queue)
{

}

//Private functions are needed because those are static
ssize_t Mux::user_data_tx(uint8_t dlci_id, const void* buffer, size_t size)
{
    return cellular_mux_stub::size_value;
}

ssize_t Mux::user_data_rx(void* buffer, size_t size)
{
    return cellular_mux_stub::size_value;
}

