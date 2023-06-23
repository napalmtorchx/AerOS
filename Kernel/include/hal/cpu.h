#pragma once
#include <lib/types.h>

typedef struct cpu_cr0 cpu_cr0_t;
typedef enum cpu_et_flag cpu_et_flag_t;
typedef uint16_t cpu_ne_flag_t;
typedef uint8_t  cpu_wp_flag_t;
typedef uint16_t cpu_am_flag_t;

/// @brief if PRESENT, FPU was found
enum cpu_et_flag {
  CPU_X87_NOT_PRESENT,
  CPU_X87_PRESENT
};

/// @brief defines the cr0 register of a 386 cpu
struct attr_pack cpu_cr0 {
  bool protected_mode : 1;
  bool monitor_coproc : 1;
  bool fpu_emulation  : 1;
  bool task_switched  : 1;
  cpu_et_flag_t extension_type : 1;
  cpu_ne_flag_t numeric_error  : 11;
  cpu_wp_flag_t write_protect : 2;
  cpu_am_flag_t alignment_mask : 11;
  bool not_write_through : 1;
  bool cache_disable : 1;
  bool paging : 1;
};

/// @brief reads the cr0 register into its type @return the read cr0 register
cpu_cr0_t cpu_read_cr0();
/// @brief writes to the cr0 the given data @param data the data to write into the cr0 
void cpu_write_cr0(const cpu_cr0_t data);

/// @brief checks the presence of the FPU @return true if present
bool cpu_fpu_present();