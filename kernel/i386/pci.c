#include <stdio.h>
#include <string.h>

#include "pci.h"
#include "io.h"
#include "../heap.h"

static const char const *pciClasses[] = {
    "Unclassified",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device Controller",
    "Docking Station",
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent Controller",
    "Satellite Communication Controller",
    "Encryption Controller",
    "Signal Processing Controller",
    "Processing Accelerator",
    "Non Essential Instrumentation",
};
static uint32_t pci_address(uint32_t bus, uint32_t slot, uint32_t func, uint32_t offset)
{
    return (bus << 16) | (slot << 11) | (func << 8) | (offset & 0XFC) | 0x80000000; 
}

static uint32_t pciConfigReadl(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t adr = pci_address(bus, slot, func, offset);
    outl(PCI_CONFIG_ADDRESS, adr);
    return inl(PCI_CONFIG_DATA);
}

static uint16_t pciConfigReadw(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    return 0xFFFF & (pciConfigReadl(bus, slot, func, offset) >> (offset & 2 * 8));
}

static uint16_t pciConfigReadb(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    return 0xFF & (pciConfigReadl(bus, slot, func, offset) >> (offset & 3 * 8));
}

static pci_header_common_t *readCommonHeader(uint8_t bus, uint8_t device, uint8_t func)
{
    int i;
    uint32_t *hdr = kmalloc(sizeof(pci_header_common_t));
    for(i = 0; i < sizeof(pci_header_common_t) / 4; i++)
    {
        hdr[i] = pciConfigReadl(bus, device, func, i*4);
    }
    return (pci_header_common_t *)hdr;
}

static void checkDevice(uint8_t device, uint8_t bus);

static void checkBus(uint8_t bus) {
    uint8_t device;
    for(device = 0; device < 32; device++) {
        checkDevice(bus, device);
    }
}

static void checkFunction(uint8_t bus, uint8_t device, uint8_t func, pci_header_common_t *h)
{
    char *subclassName = kmalloc(256);
    sprintf(subclassName, "%d", h->subClass);

    char *className = "Unknown/Invalid";
    if(h->class < 0x14)
        className = pciClasses[h->class];
    
    if(h->class == 0x40)
        className = "Co-Processor";

    if(h->class == PCI_MASS_STORAGE_CTRL) {
        switch(h->subClass) {
            case PCI_SCSI_BUS_CTRL:
                strcpy(subclassName, "SCSI Bus Controller");
            break;
            case PCI_IDE_CTRL:
                strcpy(subclassName, "IDE Controller");
            break;
            case PCI_FLOPPY_DISK_CTRL:
                strcpy(subclassName, "Floppy Disk Controller");
            break;
            case PCI_IPI_BUS_CTRL:
                strcpy(subclassName, "IPI Bus Controller");
            break;
            case PCI_RAID_CTRL:
                strcpy(subclassName, "RAID Controller");
            break;
            case PCI_ATA_CTRL:
                strcpy(subclassName, "ATA Controller");
            break;
            case PCI_SATA_CTRL:
                strcpy(subclassName, "SATA Controller");
            break;
            case PCI_SERIAL_SCSI_CTRL:
                strcpy(subclassName, "Serial SCSI Bus Controller");
            break;
            case PCI_NON_VOLATILE_MEM_CTRL:
                strcpy(subclassName, "NVM Controller");
            break;
            default:
                strcpy(subclassName, "Unknown Mass Storage Subclass");
        }

    }
    printf("%d.%d: Class: %s SubClass %s (Interface: %X) (%X-%X)\n", bus, device, className, subclassName, h->progIf, h->vendorId, h->deviceId);
    if(h->class == 0x6 && h->subClass == 0x4) {
        uint8_t secondaryBus = pciConfigReadb(bus, device, func, 0x20);
        checkBus(secondaryBus);
    }

    kfree(subclassName);
}

static void checkDevice(uint8_t bus, uint8_t device)
{
    uint8_t func = 0;
    pci_header_common_t *h = readCommonHeader(bus, device, func);
    
    if(h->deviceId == 0xFFFF) {
        kfree(h);
        return;
    }

    checkFunction(bus, device, func, h);

    if(h->headerType & PCI_MULTIFUNCTION) {

        for(func = 1; func < 8; func++)
        {
            pci_header_common_t *h2 = readCommonHeader(bus, device, func);

            if(h2->deviceId != 0xFFFF)
                checkFunction(bus, device, func, h2);

            kfree(h2);
        }
    }

    kfree(h);
}

void listPCI() {
    checkBus(0);
}