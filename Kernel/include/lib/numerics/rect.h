#pragma once
#include <lib/types.h>

typedef struct
{
    int x, y, w, h;
} rect_t;

#define RECT_EMPTY ((rect_t){ 0, 0, 0, 0 })