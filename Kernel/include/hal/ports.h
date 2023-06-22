#pragma once
#include <lib/types.h>

/**
 * @brief this enum type defines the kind of
 *        ports available to the system
*/
typedef enum {
  /**
   * @brief represents a 32bit port
  */
  PORT_DWORD,
  /**
   * @brief represents a 16bit port
  */
  PORT_WORD,
  /**
   * @brief represents an 8bit port
  */
  PORT_BYTE,
  /**
   * @brief represents an 8bit slow port
  */
  SLOW_PORT_BYTE
} port_unit_t;

/**
 * @brief represents the flags that can be
 *        attributed to a port
*/
typedef struct {
  bool read : 1;
  bool write : 1;
  uint8_t : 6;
} port_flags_t;

/**
 * @brief represents a port
*/
typedef struct {
  port_unit_t unit;
  port_flags_t flags;
  uint16_t address;
} port_t;

// legacy methods

/**
 * @brief read the input of a specific port
 *        at a specific address
 * @param address the address of the port to read
 * @attention 32 bits
 * @returns the value read from the port
*/
uint32_t ind(uint16_t address);
/**
 * @brief write to a specific port at a specific
 *        address
 * @param address the address of the port to write to
 * @param data the data to be written
 * @attention 32 bits
*/
void outd(uint16_t address, uint32_t data);

/**
 * @brief read the input of a specific port
 *        at a specific address
 * @param address the address of the port to read
 * @attention 16 bits
 * @returns the value read from the port
*/
uint16_t inw(uint16_t address);
/**
 * @brief write to a specific port at a specific
 *        address
 * @param address the address of the port to write to
 * @param data the data to be written
 * @attention 16 bits
*/
void outw(uint16_t address, uint16_t data);

/**
 * @brief read the input of a specific port
 *        at a specific address
 * @param address the address of the port to read
 * @attention 8 bits
 * @returns the value read from the port
*/
uint8_t inb(uint16_t address);
/**
 * @brief write to a specific port at a specific
 *        address
 * @param address the address of the port to write to
 * @param data the data to be written
 * @attention 8 bits
*/
void outb(uint16_t address, uint8_t data);

/**
 * @brief read from a AerOS port
 * @param port the port to read from
 * @param flag the referenced variable is set to 1 if
 *             the process was successful, 0 otherwise
 * @returns the value read from the port
*/
int port_read(port_t port, bool *flag);
/**
 * @brief write to a AerOS port
 * @param port the port to write to
 * @param data the data to be written
 * 
 * @returns 1 if the process was successful, 0 otherwise
*/
bool port_write(port_t port, int  data);
/**
 * @brief initialize a new AerOS port
 * @param address the 16-bit address to the port
 * @param flags the flags to be attributed to the port
*/
  port_t port_init(
  uint16_t address,
  port_flags_t flags,
  port_unit_t unit_type
);