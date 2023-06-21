#include <std/ports.h>
#include <debug/serial.h>
#include <std/string.h>
// @brief Init the Serial on COM Port 0x3F8
void serial_init()
{

   outb(COM_PORT + 1, 0x00);    // Disable all interrupts
   outb(COM_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(COM_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(COM_PORT + 1, 0x00);    //                  (hi byte)
   outb(COM_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(COM_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(COM_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(COM_PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(COM_PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(COM_PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(COM_PORT + 4, 0x0F);
}
int serial_received() {
   return inb(COM_PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inb(COM_PORT);
}
int is_transmit_empty() {
   return inb(COM_PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(COM_PORT,a);
}
void serial_string(char* str)
{
    for(int i=0; i < strlen(str); i++)
    {
        char out = str[i];
        write_serial(out);
    }
}