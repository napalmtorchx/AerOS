#include <hal/devmgr.h>
#include <kernel.h>

static device_t* _devices[DEVICE_COUNT];
static uint32_t  _count;
static uint32_t  _duid;

void devmgr_init()
{
    _duid = DUID_MIN;

    debug_log("%s Initializing devices...\n", DEBUG_INFO);
    enable_sse();

    debug_log("%s Initializing video devices...\n", DEBUG_INFO);
    vga_init();
    vbe_init();
    kernel_init_graphics();

    debug_log("%s Detecting CPU features...\n", DEBUG_INFO);
    fpu_init();
    get_cpu_name();

    debug_log("%s Probing PCI devices...\n", DEBUG_INFO);
    if(is_qemu()) { debug_log("%s System is running in QEMU\n", DEBUG_INFO); } else { debug_log("%s System is running on real hardware\n", DEBUG_INFO); }
    pci_init();
    
    debug_log("%s Initializing required system devices...\n", DEBUG_INFO);
    pit_init();
    rtc_init();
    ps2_init();

    debug_log("%s Initialized device manager\n", DEBUG_OK);
}

duid_t devmgr_register(device_t* dev)
{
    if (dev == NULL) { debug_error("devmgr_register(%p) - Null device pointer"); return 0; }

    for (size_t i = 0; i < DEVICE_COUNT; i++)
    {
        if (_devices[i] == NULL)
        {
            dev->uid = _duid++;
            _devices[i] = dev;            
            debug_log("%s Registered device - UID:%p Start:%p Stop:%p Name:%s\n", DEBUG_INFO, dev->uid, dev->fn_start, dev->fn_stop, dev->name);
            return dev->uid;
        }
    }

    debug_error("devmgr_register(%p) - Failed to register device");
    return 0;
}

void devmgr_unregister(duid_t dev)
{
    if (!devmgr_validate(dev, true)) { debug_error("devmgr_unregister(%p) - Invalid device uid"); return; }

    for (size_t i = 0; i < DEVICE_COUNT; i++)
    {
        if (_devices[i] != NULL && _devices[i]->uid == dev)
        {
            if (_devices[i]->running && _devices[i]->fn_stop != NULL) { _devices[i]->fn_stop(_devices[i]); }
            debug_log("%s Unregistered device - UID:%p Start:%p Stop:%p Name:%s\n", DEBUG_INFO, _devices[i]->uid, _devices[i]->fn_start, _devices[i]->fn_stop, _devices[i]->name);
            _devices[i] = NULL;
            return;
        }
    }
    debug_error("devmgr_unregister(%p) - Failed to unregister device %p", dev);
}

KRESULT devmgr_start(duid_t dev, void* arg)
{
    device_t* device = devmgr_from_uid(dev);
    if (device == NULL) { debug_error("devmgr_start(%p, %p) - Failed to start device", dev, arg); return false; }
    if (device->fn_start == NULL) { return false; }
    
    device->running = true;
    KRESULT res = device->fn_start(device, arg);
    device->running = (res == KRESULT_SUCCESS);

    if (res == KRESULT_SUCCESS) { debug_log("%s Started device - UID:%p Name:%s\n", DEBUG_INFO, device->uid, device->name); return res; }
    debug_error("devmgr_start(%p, %p) - Failed to start device, error code %2x\n", dev, arg, res);
    return res;
}

KRESULT devmgr_stop(duid_t dev)
{
    device_t* device = devmgr_from_uid(dev);
    if (device == NULL) { debug_error("devmgr_stop(%p) - Failed to stop device", dev); return false; }
    int res = device->fn_stop != NULL ? device->fn_stop(device) : false;
    if (res != false) 
    {
        device->running = false; 
        debug_log("%s Stopped device - UID:%p Name:%s\n", DEBUG_INFO, device->uid, device->name); 
        return res;
    }
    debug_error("devmgr_stop(%p) - Failed to start device, error code %2x\n", dev, res);
    return res;
}

device_t* devmgr_from_uid(duid_t dev)
{
    if (!devmgr_validate(dev, true)) { return NULL; }

    for (size_t i = 0; i < DEVICE_COUNT; i++)
    {
        if (_devices[i] != NULL && _devices[i]->uid == dev) { return _devices[i]; }
    }
    return NULL;
}

device_t* devmgr_from_name(const char* name)
{
    device_t* dev = devmgr_try_from_name(name);
    if (dev == NULL) { debug_error("devmgr_from_name(%s) - No device found with specified name", name); return NULL; }
    return dev;
}

device_t* devmgr_try_from_name(const char* name)
{
    if (strlen(name) == 0) { return NULL; }

    for (size_t i = 0; i < DEVICE_COUNT; i++)
    {
        if (_devices[i] != NULL && !strcmp(_devices[i]->name, name)) { return _devices[i]; }
    }
    return NULL;
}

bool devmgr_validate(duid_t duid, bool registered)
{
    if (duid < DUID_MIN || duid >= DUID_MAX) { return false; }
    if (!registered) { return true; }
    
    for (size_t i = 0; i < DEVICE_COUNT; i++)
    {
        if (_devices[i] != NULL && _devices[i]->uid == duid) { return true; }
    }
    return false;
}

uint32_t devmgr_count() { return _count; }