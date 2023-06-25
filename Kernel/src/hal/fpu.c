#include <hal/fpu.h>
#include <kernel.h>

static bool _fpu_loaded = false;

fpu_control_word_t fpu_read_ctrl() {
  // read the ctrl register data 
  fpu_control_word_t data;
  inline_asm ("fstcw %0; fwait" : "=m" (data));
  
  return data;
}
void fpu_write_ctrl(const fpu_control_word_t data) {
  // write to the ctrl register
  inline_asm ("fldcw %0; fwait" : : "m" (data));
}

fpu_state_word_t fpu_read_state() {
  // read the state register data
  fpu_state_word_t data;
  inline_asm ("fstsw %0; fwait" : "=m" (data));

  return data;
}

FRESULT fpu_force_probe() {
  uint16_t fpu_probe_word = 0xFFFF;

  // zero the EM and TS flags
  cpu_cr0_t cr0_data = cpu_read_cr0();
  cr0_data.task_switched = false;
  cr0_data.fpu_emulation = false;
  cpu_write_cr0(cr0_data);

  // store the status word from FPU
  inline_asm ("fninit");                              // fninit loads fpu defaults
  inline_asm ("fnstsw %0" : "=m" (fpu_probe_word));   // fnstsw stores the status word of the FPU at the given address

  // return probe result
  if (fpu_probe_word == 0) return FPU_SUCCESS;
  else FPU_NOT_PRESENT;
}
FRESULT fpu_probe(bool force) {
  if (force) return fpu_force_probe();
  else if (cpu_fpu_present()) return FPU_SUCCESS;
  else return FPU_NOT_PRESENT;
}

void fpu_setup_emulation() {
  // set emulation flag
  cpu_cr0_t cr0_data = cpu_read_cr0();
  cr0_data.fpu_emulation = true;
  cpu_write_cr0(cr0_data);

  _fpu_loaded = true; // probably better to have a flag for emulated FPU?
}
void fpu_setup_hardware() {
  // clear emulation flag on cr0 and set mp flag
  cpu_cr0_t cr0_data = cpu_read_cr0();
  cr0_data.fpu_emulation = false;
  cr0_data.monitor_coproc = true;   // exempts FWAIT from respecting task switching
  cpu_write_cr0(cr0_data);

  // setup fpu ctrl reg
  fpu_control_word_t fpu_ctrl = fpu_read_ctrl();

  // setup control bits
  fpu_ctrl.infinity_control = FPU_SIGNED_INF;
  fpu_ctrl.precision_control = FPU_PRECISION_REAL10;
  fpu_ctrl.round_control = FPU_ROUND_NEAREST;

  // setup masks
  fpu_ctrl.exception_masks.interrupt_enable = 0;    // tell the FPU that some masks will be enabled
  fpu_ctrl.exception_masks.divide_by_zero = 0;      // mask the divide by zero exception

  fpu_write_ctrl(fpu_ctrl);

  _fpu_loaded = true;
}

FRESULT fpu_init() {
  if (!fpu_probe(true)) {
    fpu_setup_emulation();
    debug_log(DEBUG_WARN " FPU emulation is required!\n");
    return FPU_NOT_PRESENT;
  } else {
    fpu_setup_hardware();
    debug_log(DEBUG_OK " FPU initialization was successful\n");
    return FPU_SUCCESS;
  }
}

FRESULT fpu_load_value(void *val, fpu_precision_ctrl_t precision) {
  if (!_fpu_loaded) return FPU_NOT_PRESENT;

  // load the value according to the given precision
  switch (precision) {
    case FPU_PRECISION_REAL4: inline_asm("fnclex; fwait; flds %0; fwait" : : "m" (*(float *)val)); break;
    case FPU_PRECISION_REAL8: inline_asm("fnclex; fwait; fldl %0; fwait" : : "m" (*(double *)val)); break;
    case FPU_PRECISION_REAL10: inline_asm("fnclex; fwait; fldt %0; fwait" : : "m" (*(double long *)val)); break;
    default: return FPU_NAN;
  }

  // check the state word for exceptions
  fpu_state_word_t state = fpu_read_state();
  if (state.exceptions.stack_fault || state.exceptions.invalid_operation || state.exceptions.denormalized_operand) {
    return FPU_EXCEPTION;
  }

  return FPU_SUCCESS;
}

FRESULT fpu_load_float(float value) {
  FRESULT result = fpu_load_value(&value, FPU_PRECISION_REAL4);
  return result;
}
FRESULT fpu_load_double(double value) {
  FRESULT result = fpu_load_value(&value, FPU_PRECISION_REAL8);
  return result;
}
FRESULT fpu_load_long_double(long double value) {
  FRESULT result = fpu_load_value(&value, FPU_PRECISION_REAL10);
  return result;
}

float fpu_get_float() {
  // clear exceptions and store the value
  float data;
  inline_asm("fnclex; fwait; fsts %0; fwait;" : "=m" (data));

  return data;
}
double fpu_get_double() {
  // clear exceptions and store the value
  double data;
  inline_asm("fnclex; fwait; fstl %0; fwait;" : "=m" (data));

  return data;
}
long double fpu_get_long_double() {
  // clear exceptions and store the value
  long double data;
  inline_asm ("fnclex; fwait; fstpt %0; fwait;" : "=m" (data));
  // refer to http://www.website.masmforum.com/tutorials/fptute/fpuchap4.htm#fstp to understand this line
  inline_asm ("fnclex; fwait; fldt %0; fwait" : : "m" (data));

  return data;
}

float fpu_retrive_float() {
  // retrive value
  float data;
  inline_asm ("fnclex; fwait; fstps %0; fwait;" : "=m" (data));

  return data;
}
double fpu_retrive_double() {
  // retrive value
  double data;
  inline_asm ("fnclex; fwait; fstpl %0; fwait;" : "=m" (data));

  return data;
}
long double fpu_retrive_long_double() {
  // retrive value
  long double data;
  inline_asm ("fnclex; fwait; fstpt %0; fwait;" : "=m" (data));

  return data;
}

void fpu_load_zero() { inline_asm("fldz; fwait"); }
void fpu_load_one() { inline_asm("fld1; fwait"); }
void fpu_load_pi() { inline_asm("fldpi; fwait"); }
void fpu_load_log2e() { inline_asm("fldl2e; fwait"); }
void fpu_load_log2t() { inline_asm("fldl2t; fwait"); }
void fpu_load_log10two() { inline_asm("fldlg2; fwait"); }
void fpu_load_logn2() { inline_asm("fldln2; fwait"); }

FRESULT fpu_load_integer(int data) {
  if (!_fpu_loaded) return FPU_NOT_PRESENT;

  // load value
  inline_asm("fnclex; fwait; fildl %0; fwait" : : "m" (data));
  return FPU_SUCCESS; 
}

FRESULT fpu_load_byte(int8_t data) { return fpu_load_integer((int)data); }
FRESULT fpu_load_word(int16_t data) { return fpu_load_integer((int)data); }
FRESULT fpu_load_dword(int32_t data) { return fpu_load_integer((int)data); }
FRESULT fpu_load_qword(uint64_t data) {
  if (!_fpu_loaded) return FPU_NOT_PRESENT;

  // load value
  inline_asm("fnclex; fwait; fildq %0; fwait" : : "m" (data));
  return FPU_SUCCESS; 
}

int fpu_get_integer() {
  int result;

  // store the value from TOP register
  inline_asm("fnclex; fwait; fistl %0; fwait" : "=m" (result));
  return result;
}

int8_t fpu_get_byte() {
  int result = fpu_get_integer();

  if (result > INT8_MAX) return INT8_MAX;
  return (int8_t)result;
}
int16_t fpu_get_word() {
  int result = fpu_get_integer();

  if (result > INT16_MAX) return INT16_MAX;
  return (int16_t)result;
}
int32_t fpu_get_dword() {
  int result = fpu_get_integer();

  if (result > INT32_MAX) return INT32_MAX;
  return (int32_t)result;
}
int64_t fpu_get_qword() {
  int64_t result;

  // store the value from TOP register
  inline_asm("fnclex; fwait; fistpq %0; fwait" : "=m" (result));
  // refer to http://www.website.masmforum.com/tutorials/fptute/fpuchap5.htm#fist to understand this line
  inline_asm("fnclex; fwait; fildq %0; fwait" : "=m" (result));

  return result;
}

int fpu_retrive_integer() {
  int result;

  // store the value from TOP register
  inline_asm("fnclex; fwait; fistpl %0; fwait" : "=m" (result));
  return result;
}

int8_t fpu_retrive_byte() {
  int result = fpu_retrive_integer();

  if (result > INT8_MAX) return INT8_MAX;
  return (int8_t)result;
}
int16_t fpu_retrive_word() {
  int result = fpu_retrive_integer();

  if (result > INT16_MAX) return INT16_MAX;
  return (int16_t)result;
}
int32_t fpu_retrive_dword() {
  int result = fpu_retrive_integer();

  if (result > INT32_MAX) return INT32_MAX;
  return (int32_t)result;

}
int64_t fpu_retrive_qword() {
  int64_t result;

  // store the value from TOP register
  inline_asm("fnclex; fwait; fistpq %0; fwait" : "=m" (result));

  return result;
}

FRESULT fpu_round() {
  // round number at st(0)
  inline_asm("frndint");

  // check for exceptions in fpu state
  fpu_state_word_t state = fpu_read_state();
  if (state.exceptions.stack_fault ||
      state.exceptions.invalid_operation ||
      state.exceptions.denormalized_operand ||
      state.exceptions.precision) return FPU_EXCEPTION;

  return FPU_SUCCESS;
}
FRESULT fpu_sqrt() {
  // calculate the square root
  inline_asm("fsqrt");

  // check for exceptions in fpu state
  fpu_state_word_t state = fpu_read_state();
  if (state.exceptions.stack_fault ||
      state.exceptions.invalid_operation ||
      state.exceptions.denormalized_operand ||
      state.exceptions.precision) return FPU_EXCEPTION;

  return FPU_SUCCESS;
}

void fpu_set_round_up() {
  // set the round up flag
  fpu_control_word_t control_w = fpu_read_ctrl();
  control_w.round_control = FPU_ROUND_UP;

  fpu_write_ctrl(control_w);
}
void fpu_set_round_down() {
  // set the round down flag
  fpu_control_word_t control_w = fpu_read_ctrl();
  control_w.round_control = FPU_ROUND_DOWN;

  fpu_write_ctrl(control_w);
}
void fpu_set_round_nearest() {
  // set the round-to-nearest flag
  fpu_control_word_t control_w = fpu_read_ctrl();
  control_w.round_control = FPU_ROUND_NEAREST;

  fpu_write_ctrl(control_w);
}
void fpu_set_truncate() {
  // set the round up flag
  fpu_control_word_t control_w = fpu_read_ctrl();
  control_w.round_control = FPU_TRUNCATE;

  fpu_write_ctrl(control_w);
}