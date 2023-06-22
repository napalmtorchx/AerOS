#pragma once
#include <lib/types.h>

typedef enum
{
    KRESULT_SUCCESS,
    KRESULT_FAILURE,
    KRESULT_OUTOFBOUNDS_INDEX,
    KRESULT_OUTOFBOUNDS_ADDRESS,
    KRESULT_INVALID_PTR,
    KRESULT_INVALID_ARG,
    KRESULT_NULL_PTR,
    KRESULT_NULL_ARG,
    KRESULT_NODEVICE,
} KRESULT;

#define FAILURE(result) ((result) != KRESULT_SUCCESS)
