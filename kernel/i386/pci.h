#ifndef _PCI_H
#define _PCI_H

#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define PCI_MULTIFUNCTION (1 << 7)

#define PCI_MASS_STORAGE_CTRL 0x1
#define PCI_NETWORK_CTRL 0x2
#define PCI_DISPLAY_CTRL 0x3
#define PCI_MULTIMEDIA_CONTROLLER 0x4
#define PCI_MEMORY_CONTROLLER 0x5
#define PCI_BRIDGE 0x6

#define PCI_SCSI_BUS_CTRL 0x0
#define PCI_IDE_CTRL 0x1
#define PCI_FLOPPY_DISK_CTRL 0x2
#define PCI_IPI_BUS_CTRL 0x3
#define PCI_RAID_CTRL 0x4
#define PCI_ATA_CTRL 0x5
#define PCI_SATA_CTRL 0x6
#define PCI_SERIAL_SCSI_CTRL 0x7
#define PCI_NON_VOLATILE_MEM_CTRL 0x8


typedef struct {
    uint16_t vendorId;
    uint16_t deviceId;
    uint16_t command;
    uint16_t status;
    uint8_t revId;
    uint8_t progIf;
    uint8_t subClass;
    uint8_t class;
    uint8_t cacheLineSize;
    uint8_t latencyTimer;
    uint8_t headerType;
    uint8_t bist;
} __attribute__((packed)) pci_header_common_t;

typedef struct {
    pci_header_common_t commonHeader;
    uint32_t BAR[6];
    uint32_t cardbusCISPointer;
    uint16_t subsystemVendorId;
    uint16_t subsystemId;
    uint32_t expansionROMBaseAddress;
    uint8_t capabilitiesPointer;
    uint8_t reserved1;
    uint16_t reserved2;
    uint32_t reserved3;
    uint8_t interruptLine;
    uint8_t interruptPin;
    uint8_t minGrant;
    uint8_t maxLatency;
} __attribute__((packed)) pci_header_device_t;

typedef struct {
    pci_header_common_t commonHeader;
    uint32_t BAR[2];
    uint8_t primaryBusNumber;
    uint8_t secondaryBusNumber;
    uint8_t subordinateBusNumber;
    uint8_t SecondaryLatencyTimer;
    uint8_t ioBase;
    uint8_t ioLimit;
    uint16_t secondaryStatus;
    uint16_t memoryBase;
    uint16_t memoryLimit;
    uint16_t prefetchableMemoryBase;
    uint16_t prefetchableMemoryLimit;
    uint32_t prefetchableMemoryBaseUpper;
    uint32_t prefetchableMemoryLimitUpper;
    uint16_t ioBaseUpper;
    uint16_t ioLimitUpper;
    uint8_t capabilityPointer;
    uint8_t reserved1;
    uint16_t reserved2;
    uint32_t expansionROMBaseAddress;
    uint8_t interruptLine;
    uint8_t interruptPin;
    uint16_t bridgeControl;
} __attribute__((packed)) pci_header_pci_bridge_t;

typedef struct {
    pci_header_common_t commonHeader;
    uint32_t cardbusSocketBaseAddress;
    uint8_t offsetOfCapabilitesList;
    uint8_t reserved;
    uint16_t secondaryStatus;
    uint8_t pciBusNumber;
    uint8_t cardbusBusNumber;
    uint8_t subordinateBusNumber;
    uint8_t cardbusLatencyTimer;
    uint32_t memoryBaseAddress0;
    uint32_t memoryLimit0;
    uint32_t memoryBaseAddress1;
    uint32_t memoryLimit1;
    uint32_t ioBaseAddress0;
    uint32_t ioLimit0;
    uint32_t ioBaseAddress1;
    uint32_t ioLimit1;
    uint8_t interruptLine;
    uint8_t interruptPin;
    uint16_t bridgeControl;
    uint16_t subsystemDeviceId;
    uint16_t subsystemVendorId;
    uint32_t legacyBaseAddress;
} __attribute__((packed)) pci_header_cardbus_bridge_t;

void listPCI();

#endif