#include <hal/devices/test_device.h>
#include <kernel.h>

static test_device_t _dev;

void testdev_init(void)
{
    _dev = (test_device_t)
    {
        .base  = (device_t){ "test_device", NULL_DUID, testdev_start, testdev_stop, false },
        .blah  = 69,
        .blah2 = 420,
    };
    devmgr_register(&_dev);
    devmgr_start(_dev.base.uid, NULL);
}

KRESULT testdev_start(test_device_t* dev, void* arg)
{
    return KRESULT_SUCCESS;
}

KRESULT testdev_stop(test_device_t* dev)
{
    return KRESULT_SUCCESS;
}
