#include "ThreadInterface.h"
#include "include/thread_tasklet.h"
#include "callback_handler.h"
#include "mesh_system.h"
#include "randLIB.h"

#include "ns_trace.h"
#define TRACE_GROUP "nsth"

nsapi_error_t ThreadInterface::initialize(NanostackRfPhy *phy)
{
    return MeshInterfaceNanostack::initialize(phy);
}

int ThreadInterface::connect()
{
    nanostack_lock();

    if (register_phy() < 0) {
        nanostack_unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    // After the RF is up, we can seed the random from it.
    randLIB_seed_random();

    mesh_error_t status = init();
    if (status != MESH_ERROR_NONE) {
        nanostack_unlock();
        return map_mesh_error(status);
    }

    status = mesh_connect();
    if (status != MESH_ERROR_NONE) {
        nanostack_unlock();
        return map_mesh_error(status);
    }

    // Release mutex before blocking
    nanostack_unlock();

    int32_t count = connect_semaphore.wait(30000);

    if (count <= 0) {
        return NSAPI_ERROR_DHCP_FAILURE; // sort of...
    }
    return 0;
}

int ThreadInterface::disconnect()
{
    nanostack_lock();

    mesh_error_t status = mesh_disconnect();

    nanostack_unlock();

    return map_mesh_error(status);
}

mesh_error_t ThreadInterface::init()
{
    if (eui64 == NULL) {
        return MESH_ERROR_PARAM;
    }
    thread_tasklet_init();
    __mesh_handler_set_callback(this);
    thread_tasklet_device_config_set(eui64, NULL);
    _network_interface_id = thread_tasklet_network_init(_device_id);

    if (_network_interface_id == -2) {
        return MESH_ERROR_PARAM;
    } else if (_network_interface_id == -3) {
        return MESH_ERROR_MEMORY;
    } else if (_network_interface_id < 0) {
        return MESH_ERROR_UNKNOWN;
    }
    return MESH_ERROR_NONE;
}

bool ThreadInterface::getOwnIpAddress(char *address, int8_t len)
{
    tr_debug("getOwnIpAddress()");
    if (thread_tasklet_get_ip_address(address, len) == 0) {
        return true;
    }
    return false;
}

mesh_error_t ThreadInterface::mesh_connect()
{
    int8_t status;
    tr_debug("connect()");

    status = thread_tasklet_connect(&__mesh_handler_c_callback, _network_interface_id);

    if (status >= 0) {
        return MESH_ERROR_NONE;
    } else if (status == -1) {
        return MESH_ERROR_PARAM;
    } else if (status == -2) {
        return MESH_ERROR_MEMORY;
    } else if (status == -3) {
        return MESH_ERROR_STATE;
    } else {
        return MESH_ERROR_UNKNOWN;
    }
}


mesh_error_t ThreadInterface::mesh_disconnect()
{
    int8_t status;

    status = thread_tasklet_disconnect(true);

    if (status >= 0) {
        return MESH_ERROR_NONE;
    }

    return MESH_ERROR_UNKNOWN;
}
