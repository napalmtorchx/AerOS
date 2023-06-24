#pragma once
#include <lib/types.h>
#include <hal/ports.h>
#include <debug/debug.h>

// Structure for managing PCI device header
typedef struct
{
    uint8_t  bus;                           // PCI bus number
    uint8_t  slot;                          // PCI slot number
    uint8_t  func;                          // PCI function number
    uint16_t vendor_id;                     // Vendor ID
    uint16_t device_id;                     // Device ID
    uint16_t command;                       // Command register
    uint16_t status;                        // Status register
    uint8_t  revision_id;                   // Revision ID
    uint8_t  prog_if;                       // Programming Interface
    uint8_t  subclass;                      // Subclass code
    uint8_t  class_code;                    // Class code
    uint8_t  cache_line_size;               // Cache line size
    uint8_t  latency_timer;                 // Latency timer
    uint8_t  header_type;                   // Header type
    uint8_t  bist;                          // Built-in self-test
    uint32_t bar0;                          // Base Address Register 0
    uint32_t bar1;                          // Base Address Register 1
    uint32_t bar2;                          // Base Address Register 2
    uint32_t bar3;                          // Base Address Register 3
    uint32_t bar4;                          // Base Address Register 4
    uint32_t bar5;                          // Base Address Register 5
    uint32_t cardbus_cis_pointer;           // CardBus CIS Pointer
    uint16_t subsystem_vendor_id;           // Subsystem Vendor ID
    uint16_t subsystem_id;                  // Subsystem ID
    uint32_t expansion_rom_base_address;    // Expansion ROM Base Address
    uint8_t  capabilities_pointer;          // Capabilities Pointer
    uint8_t  reserved[7];                   // Reserved bytes
    uint8_t  interrupt_line;                // Interrupt Line
    uint8_t  interrupt_pin;                 // Interrupt Pin
    uint8_t  min_grant;                     // Minimum Grant
    uint8_t  max_latency;                   // Maximum Latency
} pci_device_t;

/// @brief Read a 16-bit word from the PCI configuration space. @param bus The PCI bus number. @param slot The PCI slot number. @param func The PCI function number. @param offset The offset within the PCI configuration space. @return The 16-bit word read from the PCI configuration space.
uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

/// @brief Write a 16-bit word to the PCI configuration space. @param bus The PCI bus number. @param slot The PCI slot number. @param func The PCI function number. @param offset The offset within the PCI configuration space. @param data The 16-bit word to be written.
void pci_write_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data);

///@brief Read a 32-bit double word from the PCI configuration space. @param bus The PCI bus number. @param slot The PCI slot number. @param func The PCI function number. @param offset The offset within the PCI configuration space. @return The 32-bit double word read from the PCI configuration space.
uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

/// @brief Write a 32-bit double word to the PCI configuration space. @param bus The PCI bus number. @param slot The PCI slot number. @param func The PCI function number. @param offset The offset within the PCI configuration space. @param data The 32-bit double word to be written.
void pci_write_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data);

/// @brief Read an 8-bit byte from the PCI configuration space. @param bus The PCI bus number. @param slot The PCI slot number. @param func The PCI function number. @param offset The offset within the PCI configuration space. @return The 8-bit byte read from the PCI configuration space.
uint8_t pci_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

/// @brief Write an 8-bit byte to the PCI configuration space. @param bus The PCI bus number.@param slot The PCI slot number. @param func The PCI function number.@param offset The offset within the PCI configuration space. @param data The 8-bit byte to be written.
void pci_write_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t data);