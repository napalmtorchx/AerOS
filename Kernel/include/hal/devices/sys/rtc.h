#pragma once
#include <lib/types.h>
#include <lib/kresult.h>
#include <lib/time.h>
#include <hal/device.h>
#include <hal/ints/idt.h>

#define RTC_CMD         0x70
#define RTC_DATA        0x71
#define RTC_MILLIS_TIME 0.9765625f
#define RTC_FREQ        1024

typedef struct
{
    device_t base;
    time_t   tm;
    uint32_t ticks;
    uint32_t tps, timer;
    float    seconds;
    uint32_t millis, millis_t;
} rtc_device_t;

void rtc_init(void);
KRESULT rtc_start(rtc_device_t* dev, void* unused);
KRESULT  rtc_stop(rtc_device_t* dev);
void rtc_flush(void);
void rtc_update(void);
void rtc_callback(irq_context_t* context);
uint8_t rtc_rdreg(uint8_t reg);
bool rtc_updating(void);