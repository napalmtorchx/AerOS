#include <hal/devices/sys/pci.h>
#include <kernel.h>

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
char* data;
void load_pci_file()
{
    FILE* pci_json = fopen("A:/pci.json", "r");
    data = (char*)malloc(pci_json->sz);
    fread(data, pci_json->sz, 1, pci_json);
    fclose(pci_json);
}
//this is dirty i know, don't judge me
//at this point i have no other option since we are lacking a proper json parser
//so, will, bite it! xD
char* search_for_device(uint16_t dev_id,uint16_t ven_id,char* dev_id_m,char* ven_id_m,char* out_buff)
{
    // read file


    // attempt to match vendor id
    char* ven_id_str = ven_id_m;
    sprintf(ven_id_str, "%x", ven_id);
    char* ven_id_match = strstr(data, ven_id_str);
    free(ven_id_str);

    

    if (ven_id_match == NULL)
    {
        strcat(out_buff, "Unknown");
        return out_buff;
    }
    else
    {
        char* dev_id_str = dev_id_m;
        sprintf(dev_id_str, "%x", dev_id);
        char* dev_id_match = strstr(ven_id_match, dev_id_str);
        free(dev_id_str);

        if (dev_id_match == NULL)
        {
            strcat(out_buff, "Unknown");
            return out_buff;
        }
        else
        {
            char* name = strstr(dev_id_match, "name");
            char* name_end = strstr(name, ",");
            char* name_str = (char*)malloc(name_end - name);
            memcpy(name_str, name + 7, name_end - name - 7);
            name_str[name_end - name - 7] = '\0';

            // remove char
            char* new_name_str = remove_char(name_str, '"');
            strcpy(out_buff, new_name_str);
            free(new_name_str);
            free(name_str);

            // check vendor id
            switch(ven_id)
            {
                case 0x8086:
                    strcat(out_buff," (Intel)");
                    break;
                case 0x1022:
                    strcat(out_buff," (AMD)");
                    break;
                case 0x10DE:
                    strcat(out_buff," (Nvidia)");
                    break;
                case 0x1234:
                    strcat(out_buff," (QEMU)");
                    break;
                case 0x1AF4:
                    strcat(out_buff," (VirtIO)");
                    break;
                case 0x1B36:
                    strcat(out_buff," (Red Hat)");
                    break;
                case 0x1B21:
                    strcat(out_buff," (ASMedia)");
                    break;
                case 0x1D6B:
                    strcat(out_buff," (Linux Foundation)");
                    break;
                case 0x1AB8:
                    strcat(out_buff," (Google)");
                    break;
                case 0x1AE0:
                    strcat(out_buff," (Google)");
                    break;
                case 0x1B73:
                    strcat(out_buff," (Fresco Logic)");
                    break;
                case 0x1B6F:
                    strcat(out_buff," (VMware)");
                    break;
                case 0x1B96:
                    strcat(out_buff," (Red Hat)");
                    break;
                case 0x1B97:
                    strcat(out_buff," (Red Hat)");
                    break;
                case 0x1B98:
                    strcat(out_buff," (Red Hat)");
                    break;
                case 0x1B99:    
                    strcat(out_buff," (Red Hat)");
                    break;
                case 0x1B9A:
                    strcat(out_buff," (Red Hat)");
                    break;
                case 0x15AD:
                    strcat(out_buff," (VMware)");
                    break;
                default:
                    break;
            }

            return out_buff;
        }
    }
   
}
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
                    char* dev_id_mem = malloc(7);
                    char* ven_id_mem = malloc(7);
                    char* out_buff = (char*)malloc(1024);
                    memset(dev_id_mem,0,7);
                    memset(ven_id_mem,0,7);
                    char* dev_name = search_for_device(device_id,vendor_id,dev_id_mem,ven_id_mem,out_buff);
                    debug_log("%s Located PCI device at %2x:%2x%2x\n", DEBUG_INFO, bus, slot, func);
                    debug_log("         Name  :%s\n", dev_name);
                    debug_log("         ID    :%4x:%4x\n", vendor_id, device_id);
                    debug_log("         Class :%2x\n", class_code);
                    free(dev_name);
                    free(dev_id_mem);
                    free(ven_id_mem);
                    free(out_buff);
                }
            }
        }
    }
    free(data);
}