#pragma once
#include <lib/types.h>

typedef enum FRESULT FRESULT;
typedef enum fpu_precision_ctrl fpu_precision_ctrl_t;
typedef enum fpu_round_ctrl fpu_round_ctrl_t;
typedef enum fpu_infinity_ctrl fpu_infinity_ctrl_t;

typedef struct fpu_control_word fpu_control_word_t;
typedef struct fpu_masks fpu_masks_t;

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
/// @brief describes the precision control bit
enum fpu_precision_ctrl {
  FPU_PRECISION_REAL4,
  FPU_PRECISION_INVALID,
  FPU_PRECISION_REAL8,
  FPU_PRECISION_REAL10, 
};
/// @brief describes the round control bit
enum fpu_round_ctrl {
  FPU_ROUND_NEAREST,
  FPU_ROUND_DOWN,
  FPU_ROUND_UP,
  FPU_TRUNCATE
};
/// @brief describes whether +inf and -inf should be treated separately or not
enum fpu_infinity_ctrl {
  FPU_UNSIGNED_INF,
  FPU_SIGNED_INF
};

/// @brief describes the exception masks for FPU
struct attr_pack fpu_masks {
  bool invalid_operation : 1;
  bool denormalized_operand : 1;
  bool divide_by_zero : 1;
  bool overflow : 1;
  bool underflow : 1;
  bool precision : 1;

  uint8_t reserved : 1;

  bool interrupt_enable : 1;
};
/// @brief describes the fpu control word register
struct attr_pack fpu_control_word {
  fpu_masks_t exception_masks;

  fpu_precision_ctrl_t precision_control : 2;
  fpu_round_ctrl_t round_control : 2;
  fpu_infinity_ctrl_t infinity_control : 1;
};

/// @brief reads the control register of the FPU @return the read FPU control register data
fpu_control_word_t fpu_read_ctrl();
/// @brief writes the given data to the FPU register @param data the data to write to the FPU control register
void fpu_write_ctrl(const fpu_control_word_t data);

/// @brief probes for an FPU @param force force probing @return FPU_SUCCESS if found, FPU_NOT_PRESENT otherwise
FRESULT fpu_probe(bool force);
/// @brief initializes the FPU @return FPU_SUCCESS on success, FPU_NOT_PRESENT if probe failed, FPU_EXCEPTION on any error
FRESULT fpu_init();
/// @brief checks if fpu is emulated @return whether the fpu is emulated or not
bool fpu_is_emulated();