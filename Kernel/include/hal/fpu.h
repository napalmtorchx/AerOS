#pragma once
#include <lib/types.h>

typedef enum FRESULT FRESULT;
typedef enum fpu_precision_ctrl fpu_precision_ctrl_t;
typedef enum fpu_round_ctrl fpu_round_ctrl_t;
typedef enum fpu_infinity_ctrl fpu_infinity_ctrl_t;
typedef enum fpu_tag fpu_tag_t;

typedef struct fpu_control_word fpu_control_word_t;
typedef struct fpu_masks fpu_masks_t;
typedef struct fpu_state_word fpu_state_word_t;
typedef struct fpu_exception_states fpu_exception_states_t; 
typedef struct fpu_tag_word fpu_tag_word_t;


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
/// @brief describes an fpu register tag
enum fpu_tag {
  FPU_TAG_VALID,
  FPU_TAG_ZERO,
  FPU_TAG_SPECIAL,
  FPU_TAG_EMPTY
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

  uint8_t reserved : 3;
};
/// @brief describes a set of exception states
struct attr_pack fpu_exception_states {
  bool invalid_operation : 1;
  bool denormalized_operand : 1;
  bool divide_by_zero : 1;
  bool overflow : 1;
  bool underflow : 1;
  bool precision : 1;
  bool stack_fault : 1;

  bool interrupt_request : 1;
};
/// @brief describes the fpu state after a given instruction
struct attr_pack fpu_state_word {
  fpu_exception_states_t exceptions;

  bool cf0 : 1; // conditional flag 0
  bool cf1 : 1; // conditional flag 1
  bool cf2 : 1; // conditional flag 2

  uint8_t top : 3; // index of register at ST(0)

  bool cf3 : 1; // conditional flag 3
  bool busy : 1; // 0 == idle, 1 == busy
};
/// @brief describes the fpu tag register content 
struct attr_pack fpu_tag_word {
  fpu_tag_t tag_0 : 2;
  fpu_tag_t tag_1 : 2;
  fpu_tag_t tag_2 : 2;
  fpu_tag_t tag_3 : 2;
  fpu_tag_t tag_4 : 2;
  fpu_tag_t tag_5 : 2;
  fpu_tag_t tag_6 : 2;
  fpu_tag_t tag_7 : 2;
};

/// @brief reads the control register of the FPU @return the read FPU control register data
fpu_control_word_t fpu_read_ctrl();
/// @brief writes the given data to the FPU register @param data the data to write to the FPU control register
void fpu_write_ctrl(const fpu_control_word_t data);

/// @brief read the FPU state @return the read FPU state register
fpu_state_word_t fpu_read_state();

/// @brief probes for an FPU @param force force probing @return FPU_SUCCESS if found, FPU_NOT_PRESENT otherwise
FRESULT fpu_probe(bool force);
/// @brief initializes the FPU @return FPU_SUCCESS on success, FPU_NOT_PRESENT if probe failed, FPU_EXCEPTION on any error
FRESULT fpu_init();
/// @brief checks if fpu is emulated @return whether the fpu is emulated or not
bool fpu_is_emulated();

// load functions

/// @brief load the specified value, respecting the given precision @param val the value to load @param precision the precision to respect @return FPU_SUCCESS if successful
FRESULT fpu_load_value(void *val, fpu_precision_ctrl_t precision);

/// @brief loads a float value into the TOP register @param value the value to load @return the result status
FRESULT fpu_load_float(float value);
/// @brief loads a double-precision float value into the TOP register @param value the value to load @return the result status
FRESULT fpu_load_double(double value);
/// @brief loads a long-double-precision float value into the TOP register @param value the value to load @return the result status
FRESULT fpu_load_long_double(long double value);

/// @brief loads constant value 0.0 into the TOP register
void fpu_load_zero();
/// @brief loads constant value 1.0 into the TOP register
void fpu_load_one();
/// @brief loads constant value PI into the TOP register
void fpu_load_pi();
/// @brief loads constant value log2(e) into the TOP register
void fpu_load_log2e(); 
/// @brief loads constant value log2(10) into the TOP register
void fpu_load_log2t(); 
/// @brief loads constant value log10(2) into the TOP register
void fpu_load_log10two();
/// @brief loads constant value ln(2) into the TOP register
void fpu_load_logn2();

/// @brief load an integer value @param data the integer to load @return FPU_SUCCESS
FRESULT fpu_load_integer(int data);
/// @brief loads a byte into the TOP register @param data the byte to load @return FPU_SUCCESS if successful
FRESULT fpu_load_byte(int8_t data);
/// @brief loads a byte into the TOP register @param data the word to load @return FPU_SUCCESS if successful
FRESULT fpu_load_word(int16_t data);
/// @brief loads a byte into the TOP register @param data the dword to load @return FPU_SUCCESS if successful
FRESULT fpu_load_dword(int32_t data);
/// @brief loads a byte into the TOP register @param data the qword to load @return FPU_SUCCESS if successful
FRESULT fpu_load_qword(uint64_t data);

// store functions

/// @brief gets the current value stred in the TOP register @return the stored value
float fpu_get_float();
/// @brief gets the current value stred in the TOP register @return the stored value
double fpu_get_double();
/// @brief gets the current value stred in the TOP register @return the stored value
long double fpu_get_long_double();

/// @brief retrives the value from the top register and clears it @return the retrived value
float fpu_retrive_float();
/// @brief retrives the value from the top register and clears it @return the retrived value
double fpu_retrive_double();
/// @brief retrives the value from the top register and clears it @return the retrived value
long double fpu_retrive_long_double();

/// @brief gets the current value in the TOP register, rounded to the nearest number @return the stored rounded value or MAX_INT if the stored value exceeds the max value 
int fpu_get_integer();
/// @brief gets the current value in the TOP register, rounded to the nearest number @return the stored rounded value or MAX_INT if the stored value exceeds the max value 
int8_t fpu_get_byte();
/// @brief gets the current value in the TOP register, rounded to the nearest number @return the stored rounded value or MAX_INT if the stored value exceeds the max value 
int16_t fpu_get_word();
/// @brief gets the current value in the TOP register, rounded to the nearest number @return the stored rounded value or MAX_INT if the stored value exceeds the max value 
int32_t fpu_get_dword();
/// @brief gets the current value in the TOP register, rounded to the nearest number @return the stored rounded value or MAX_INT if the stored value exceeds the max value 
int64_t fpu_get_qword();

/// @brief retrives the current value in the TOP register, rounded to the nearest number. Then cleares the register @return the retrived rounded value or MAX_INT if the stored value exceeds the max value 
int fpu_retrive_integer();
/// @brief retrives the current value in the TOP register, rounded to the nearest number. Then cleares the register @return the retrived rounded value or MAX_INT if the stored value exceeds the max value 
int8_t fpu_retrive_byte();
/// @brief retrives the current value in the TOP register, rounded to the nearest number. Then cleares the register @return the retrived rounded value or MAX_INT if the stored value exceeds the max value 
int16_t fpu_retrive_word();
/// @brief retrives the current value in the TOP register, rounded to the nearest number. Then cleares the register @return the retrived rounded value or MAX_INT if the stored value exceeds the max value 
int32_t fpu_retrive_dword();
/// @brief retrives the current value in the TOP register, rounded to the nearest number. Then cleares the register @return the retrived rounded value or MAX_INT if the stored value exceeds the max value 
int64_t fpu_retrive_qword();

/// @brief rounds accordingly to the FPU's round control bit @return FPU_success if successful
FRESULT fpu_round();

/// @brief sets the round up flag in the control 
void fpu_set_round_up();
