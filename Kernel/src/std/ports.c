#include <std/ports.h>
uint32_t ind(uint16_t address)
{
  uint32_t data;
  __asm__ volatile (
    "inl %%dx, %%eax"
    : "=a" (data)
    : "d"  (address)
  );

  return data;
}
void outd(uint16_t address, uint32_t data)
{
  __asm__ volatile (
    "outl %%eax, %%dx"
    :: "a" (data), "d" (address)
  );
}

uint16_t inw(uint16_t address)
{
  uint16_t data;
  __asm__ volatile (
    "inw %%dx, %%ax"
    : "=a" (data)
    : "d"  (address)
  );

  return data;
}
void outw(uint16_t address, uint16_t data)
{
  __asm__ volatile (
    "outw %%ax, %%dx"
    :: "a" (data), "d" (address)
  );
}

uint8_t inb(uint16_t address)
{
  uint8_t data;
  __asm__ volatile (
    "inb %%dx, %%al"
    : "=a" (data)
    : "d"  (address)
  );
  
  return data;
}
void outb(uint16_t address, uint8_t data)
{
  __asm__ volatile (
    "outb %%al, %%dx"
    :: "a"(data), "d"(address)
  );
}

int   port_read(port_t port, bool *flag)
{
  if (!port.flags.read) return *flag = false;

  *flag = true;
  switch (port.unit)
  {
    case PORT_DWORD:
      return ind(port.address);
    case PORT_WORD:
      return inw(port.address);
    case PORT_BYTE:
      return inb(port.address);
    case SLOW_PORT_BYTE:
    default:
      return *flag = false;
  }
}
bool port_write(port_t port, int  data)
{
  if (!port.flags.write) return false;
  switch (port.unit)
  {
    case PORT_DWORD:
      outd(port.address, (uint32_t)data);
      break;
    case PORT_WORD:
      outw(port.address, (uint16_t)data);
      break;
    case PORT_BYTE:
      outb(port.address, (uint8_t)data);
      break;
    case SLOW_PORT_BYTE:
    default:
      return false;
  }
  return true;
}
  port_t port_init(
  uint16_t address,
  port_flags_t flags,
  port_unit_t unit_type
)
{
  return (port_t) {
    .address = address,
    .flags = flags,
    .unit = unit_type
  };
}