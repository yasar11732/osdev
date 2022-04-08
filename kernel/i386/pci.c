#include <stdio.h>
#include <string.h>

#include "pci.h"
#include "io.h"
#include "../heap.h"
#include "../dynamic_array.h"

static d_array_t *devices = NULL;

typedef struct
{
    uint32_t bus;
    uint32_t slot;
    uint32_t function;
    pci_header_common_t header;
} pci_device_info_t;

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

static pci_header_common_t *readCommonHeader(pci_header_common_t *dest, uint8_t bus, uint8_t device, uint8_t func)
{
    int i;
    uint32_t *_dest = (uint32_t *)dest;
    for (i = 0; i < sizeof(pci_header_common_t) / 4; i++) {
        _dest[i] = pciConfigReadl(bus, device, func, i * 4);
    }
    return dest;
}

static inline pci_header_common_t *read_device_header(uint32_t bus, uint32_t device, uint32_t func)
{
    pci_device_info_t *i = d_array_get_first_free(devices);
    i->bus = bus;
    i->slot = device;
    i->function = func;
    return readCommonHeader(&i->header, bus, device, func);
}

static void checkDevice(uint8_t device, uint8_t bus);

static void checkBus(uint8_t bus)
{
    uint8_t device;
    for (device = 0; device < 32; device++) {
        checkDevice(bus, device);
    }
}

static void checkFunction(uint8_t bus, uint8_t device, uint8_t func, pci_header_common_t *h)
{
    if (h->class == 0x6 && h->subClass == 0x4) {
        uint8_t secondaryBus = pciConfigReadb(bus, device, func, 0x20);
        checkBus(secondaryBus);
    }
}

static void checkDevice(uint8_t bus, uint8_t device)
{

    uint8_t func = 0;
    pci_header_common_t *h = read_device_header(bus, device, func);

    if (h->vendorId == 0xFFFF) {
        d_array_pop(devices);
        return;
    }

    checkFunction(bus, device, func, h);

    if (h->headerType & PCI_MULTIFUNCTION) {

        for (func = 1; func < 8; func++) {
            pci_header_common_t *h2 = read_device_header(bus, device, func);

            if (h2->vendorId != 0xFFFF) {
                checkFunction(bus, device, func, h2);
            } else {
                d_array_pop(devices);
            }
        }
    }
}

void scanPCI()
{
    if (devices == NULL) {
        devices = d_array_create(sizeof(pci_device_info_t), 16);

        pci_header_common_t *h = read_device_header(0, 0, 0);

        d_array_reset(devices);

        if ((h->headerType & 0x80) == 0) {
            checkBus(0);
        } else {
            uint8_t function;
            for (function = 0; function < 8; function++) {
                pci_header_common_t *h2 = read_device_header(0, 0, function);
                if (h2->vendorId == 0xFFFF) {
                    d_array_pop(devices);
                    break;
                } else {
                    checkBus(function);
                }
            }
        }
    }
}

void listPCI()
{
    if (devices == NULL)
        scanPCI();

    uint32_t numDevices = d_array_get_item_count(devices);
    pci_device_info_t *dinfo = d_array_get_first_item(devices);

    while (numDevices--) {
        printf("%d.%d.%d - Vendor: %X, DeviceId: %X, Class %X, SubClass %X, Interface %X\n",
               dinfo[numDevices].bus,
               dinfo[numDevices].slot,
               dinfo[numDevices].function,
               dinfo[numDevices].header.vendorId,
               dinfo[numDevices].header.deviceId,
               dinfo[numDevices].header.class,
               dinfo[numDevices].header.subClass,
               dinfo[numDevices].header.progIf);
    }
}

pci_header_common_t *get_device_by_class(uint8_t class, uint8_t subclass, uint8_t i)
{
    if (devices == NULL)
        scanPCI();

    uint32_t numDevices = d_array_get_item_count(devices);
    pci_device_info_t *dinfo = d_array_get_first_item(devices);

    while (numDevices--) {
        if (dinfo[numDevices].header.class == class
            && dinfo[numDevices].header.subClass == subclass) {

            if (!i) {
                return &dinfo[numDevices].header;
            } else {
                i--;
            }
        }
    }

    return NULL;
}