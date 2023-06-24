#include <lib/stdio.h>
#include <kernel.h>
#include <hal/devices/sys/pci.h>

uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return (tmp);
}

void pci_write_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    outl(0xCFC, data);
}

uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    tmp = (uint32_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFFFFFF);
    return (tmp);
}

void pci_write_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    outl(0xCFC, data);
}

uint8_t pci_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint8_t tmp = 0;
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    tmp = (uint8_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFF);
    return (tmp);
}

void pci_write_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t data)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    outl(0xCFC, data);
}

char* remove_char(char* input,char c)
{
    char* str = (char*)malloc(strlen(input));
    int i = 0;
    int j = 0;
    while (input[i] != '\0')
    {
        if (input[i] != c)
        {
            str[j] = input[i];
            j++;
        }
        i++;
    }
    str[j] = '\0';
    return str;
}

//this is dirty i know, don't judge me
//at this point i have no other option since we are lacking a proper json parser
//so, will, bite it! xD
char* search_for_device(uint16_t dev_id,uint16_t ven_id)
{
    FILE* pci_json = fopen("A:/pci.json", "r");
    char* data = (char*)malloc(pci_json->sz);
    fread(data, pci_json->sz, 1, pci_json);
    fclose(pci_json);
    char* ven_id_str = (char*)malloc(ven_id);
    sprintf(ven_id_str, "%x", ven_id);
    char* ven_id_match = strstr(data, ven_id_str);
    if (ven_id_match == NULL)
    {
        return "Unknown";
    }
    else
    {
        char* dev_id_str = (char*)malloc(dev_id);
        sprintf(dev_id_str, "%x", dev_id);
        char* dev_id_match = strstr(ven_id_match, dev_id_str);
        if (dev_id_match == NULL)
        {
            return "Unknown";
        }
        else
        {
            char* name = strstr(dev_id_match, "name");
            char* name_end = strstr(name, ",");
            char* name_str = (char*)malloc(name_end - name);
            memcpy(name_str, name + 7, name_end - name - 7);
            name_str[name_end - name - 7] = '\0';
            name_str = remove_char(name_str, '"');
            switch(ven_id)
            {
                case 0x8086:
                    name_str = strcat(name_str," (Intel)");
                    break;
                case 0x1022:
                    name_str = strcat(name_str," (AMD)");
                    break;
                case 0x10DE:
                    name_str = strcat(name_str," (Nvidia)");
                    break;
                case 0x1234:
                    name_str = strcat(name_str," (QEMU)");
                    break;
                case 0x1AF4:

                    name_str = strcat(name_str," (VirtIO)");
                    break;
                case 0x1B36:
                    name_str = strcat(name_str," (Red Hat)");
                    break;
                case 0x1B21:
                    name_str = strcat(name_str," (ASMedia)");
                    break;
                case 0x1D6B:
                    name_str = strcat(name_str," (Linux Foundation)");
                    break;
                case 0x1AB8:
                    name_str = strcat(name_str," (Google)");
                    break;
                case 0x1AE0:
                    name_str = strcat(name_str," (Google)");
                    break;
                case 0x1B73:
                    name_str = strcat(name_str," (Fresco Logic)");
                    break;
                case 0x1B6F:
                    name_str = strcat(name_str," (VMware)");
                    break;
                case 0x1B96:
                    name_str = strcat(name_str," (Red Hat)");
                    break;
                case 0x1B97:
                    name_str = strcat(name_str," (Red Hat)");
                    break;
                case 0x1B98:
                    name_str = strcat(name_str," (Red Hat)");
                    break;
                case 0x1B99:    
                    name_str = strcat(name_str," (Red Hat)");
                    break;
                case 0x1B9A:
                    name_str = strcat(name_str," (Red Hat)");
                    break;
                //check vmware
                case 0x15AD:
                    name_str = strcat(name_str," (VMware)");
                    break;
                    default:
                        break;
            }
            return name_str;
        }
    }
   
}
//pci device struct
typedef struct
{
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t cardbus_cis_pointer;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    uint32_t expansion_rom_base_address;
    uint8_t capabilities_pointer;
    uint8_t reserved[7];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
} pci_device_t;
//pci_list_devices
void pci_list_devices()
{
    //query all PCI devices
    for (uint8_t bus = 0; bus < 255; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++)
        {
            for (uint8_t func = 0; func < 8; func++)
            {
                uint16_t vendor_id = pci_read_word(bus, slot, func, 0);
                if (vendor_id != 0xFFFF)
                {
                    uint16_t device_id = pci_read_word(bus, slot, func, 2);
                    uint16_t command = pci_read_word(bus, slot, func, 4);
                    uint16_t status = pci_read_word(bus, slot, func, 6);
                    uint8_t revision_id = pci_read_byte(bus, slot, func, 8);
                    uint8_t prog_if = pci_read_byte(bus, slot, func, 9);
                    uint8_t subclass = pci_read_byte(bus, slot, func, 10);
                    uint8_t class_code = pci_read_byte(bus, slot, func, 11);
                    uint8_t cache_line_size = pci_read_byte(bus, slot, func, 12);
                    uint8_t latency_timer = pci_read_byte(bus, slot, func, 13);
                    uint8_t header_type = pci_read_byte(bus, slot, func, 14);
                    uint8_t bist = pci_read_byte(bus, slot, func, 15);
                    uint32_t bar0 = pci_read_dword(bus, slot, func, 16);
                    uint32_t bar1 = pci_read_dword(bus, slot, func, 20);
                    uint32_t bar2 = pci_read_dword(bus, slot, func, 24);
                    uint32_t bar3 = pci_read_dword(bus, slot, func, 28);
                    uint32_t bar4 = pci_read_dword(bus, slot, func, 32);
                    uint32_t bar5 = pci_read_dword(bus, slot, func, 36);
                    uint32_t cardbus_cis_pointer = pci_read_dword(bus, slot, func, 40);
                    uint16_t subsystem_vendor_id = pci_read_word(bus, slot, func, 44);
                    uint16_t subsystem_id = pci_read_word(bus, slot, func, 46);
                    uint32_t expansion_rom_base_address = pci_read_dword(bus, slot, func, 48);
                    uint8_t capabilities_pointer = pci_read_byte(bus, slot, func, 52);
                    uint8_t interrupt_line = pci_read_byte(bus, slot, func, 60);
                    uint8_t interrupt_pin = pci_read_byte(bus, slot, func, 61);
                    uint8_t min_grant = pci_read_byte(bus, slot, func, 62);
                    //printf("PCI device found at bus %d, slot %d, function %d\n", bus, slot, func);
                    //debug log vendor id and device id
                    //printf("%s Vendor ID: %x-%s\n",DEBUG_INFO, vendor_id,get_pci_vendor(vendor_id));
                    printf("%s Vendor: %x - Device: %x\n",DEBUG_INFO, vendor_id,device_id);
                    printf("%s\n",search_for_device(device_id,vendor_id));
                }
            }
        }
    }
}