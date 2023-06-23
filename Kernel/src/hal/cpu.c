#include <hal/cpu.h>
#include <kernel.h>

const cpu_cr0_t cpu_read_cr0() {
  cpu_cr0_t data;

  // read the data from register cr0
  inline_asm ("mov %%cr0, %%eax" : "=a" (data) :);
  return data;
}
bool cpu_fpu_present() {
  const cpu_cr0_t data = cpu_read_cr0();
   
  // validate FPU presence 
  return data.extension_type == CPU_X87_PRESENT && !data.fpu_emulation;
}