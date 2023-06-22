#pragma once
#include <hal/device.h>

typedef struct
{
    device_t  base;
    uint32_t  blah, blah2;
} test_device_t;

void testdev_init();
KRESULT testdev_start(test_device_t* dev, void* arg);
KRESULT testdev_stop(test_device_t* dev);
