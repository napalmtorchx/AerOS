#include <hal/cpu.h>
#include <kernel.h>

cpu_cr0_t cpu_read_cr0() 
{
  cpu_cr0_t data;

  // read the data from register cr0
  inline_asm ("mov %%cr0, %%eax" : "=a" (data) :);
  return data;
}
void cpu_write_cr0(const cpu_cr0_t data) {
  // write the data to the register
  inline_asm ("mov %%eax, %%cr0" : : "a" (data));
}

bool cpu_fpu_present() {
  const cpu_cr0_t data = cpu_read_cr0();
   
  // validate FPU presence 
  return data.extension_type == CPU_X87_PRESENT && !data.fpu_emulation;
}