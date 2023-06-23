#include <hal/fpu.h>
#include <kernel.h>

fpu_control_word_t fpu_read_ctrl() {
  // read the ctrl register data 
  fpu_control_word_t data;
  inline_asm ("fstcw %0" : "=m" (data));
  
  return data;
}
void fpu_write_ctrl(const fpu_control_word_t data) {
  // write to the ctrl register
  inline_asm ("fldcw %0" : : "m" (data));
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