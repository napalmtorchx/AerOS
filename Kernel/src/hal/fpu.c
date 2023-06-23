#include <hal/fpu.h>
#include <kernel.h>

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
  else return FPU_NOT_PRESENT;
}
FRESULT fpu_probe(bool force) {
  if (force) return fpu_force_probe();
  else if (cpu_fpu_present()) return FPU_SUCCESS;
  else return FPU_NOT_PRESENT;
}