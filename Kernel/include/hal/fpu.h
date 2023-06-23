#pragma once
#include <lib/types.h>

typedef enum FRESULT FRESULT;

/// @brief describes the result of an FPU action
enum FRESULT {
  FPU_NOT_PRESENT,
  FPU_EXCEPTION,
  FPU_NAN,
  FPU_INFINITY,
  FPU_NEG_INFINITY,
  FPU_REAL,
  FPU_SUCCESS
};

/// @brief probes for an FPU @param force force probing @return FPU_SUCCESS if found, FPU_NOT_PRESENT otherwise
FRESULT fpu_probe(bool force);