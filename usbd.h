#ifndef _USBD_H_
#define _USBD_H_

typedef struct SceUsbdDeviceInfo {
    unsigned int port;
    unsigned int device_num;
    unsigned int unk3; // handled? 0, 1, 2
} SceUsbdDeviceInfo;

typedef struct {
        uint8_t bLength; // Length of this descriptor.
        uint8_t bDescriptorType; // DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
        uint16_t bcdUSB; // USB Spec Release Number (BCD).
        uint8_t bDeviceClass; // Class code (assigned by the USB-IF). 0xFF-Vendor specific.
        uint8_t bDeviceSubClass; // Subclass code (assigned by the USB-IF).
        uint8_t bDeviceProtocol; // Protocol code (assigned by the USB-IF). 0xFF-Vendor specific.
        uint8_t bMaxPacketSize0; // Maximum packet size for endpoint 0.
        uint16_t idVendor; // Vendor ID (assigned by the USB-IF).
        uint16_t idProduct; // Product ID (assigned by the manufacturer).
        uint16_t bcdDevice; // Device release number (BCD).
        uint8_t iManufacturer; // Index of String Descriptor describing the manufacturer.
        uint8_t iProduct; // Index of String Descriptor describing the product.
        uint8_t iSerialNumber; // Index of String Descriptor with the device's serial number.
        uint8_t bNumConfigurations; // Number of possible configurations.
} __attribute__((packed)) USB_DEVICE_DESCRIPTOR;

/* Configuration descriptor structure */
typedef struct {
        uint8_t bLength; // Length of this descriptor.
        uint8_t bDescriptorType; // CONFIGURATION descriptor type (USB_DESCRIPTOR_CONFIGURATION).
        uint16_t wTotalLength; // Total length of all descriptors for this configuration.
        uint8_t bNumInterfaces; // Number of interfaces in this configuration.
        uint8_t bConfigurationValue; // Value of this configuration (1 based).
        uint8_t iConfiguration; // Index of String Descriptor describing the configuration.
        uint8_t bmAttributes; // Configuration characteristics.
        uint8_t bMaxPower; // Maximum power consumed by this configuration.
} __attribute__((packed)) USB_CONFIGURATION_DESCRIPTOR;

/* Interface descriptor structure */
typedef struct {
        uint8_t bLength; // Length of this descriptor.
        uint8_t bDescriptorType; // INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
        uint8_t bInterfaceNumber; // Number of this interface (0 based).
        uint8_t bAlternateSetting; // Value of this alternate interface setting.
        uint8_t bNumEndpoints; // Number of endpoints in this interface.
        uint8_t bInterfaceClass; // Class code (assigned by the USB-IF).  0xFF-Vendor specific.
        uint8_t bInterfaceSubClass; // Subclass code (assigned by the USB-IF).
        uint8_t bInterfaceProtocol; // Protocol code (assigned by the USB-IF).  0xFF-Vendor specific.
        uint8_t iInterface; // Index of String Descriptor describing the interface.
} __attribute__((packed)) USB_INTERFACE_DESCRIPTOR;

/* Endpoint descriptor structure */
typedef struct {
        uint8_t bLength; // Length of this descriptor.
        uint8_t bDescriptorType; // ENDPOINT descriptor type (USB_DESCRIPTOR_ENDPOINT).
        uint8_t bEndpointAddress; // Endpoint address. Bit 7 indicates direction (0=OUT, 1=IN).
        uint8_t bmAttributes; // Endpoint transfer type.
        uint16_t wMaxPacketSize; // Maximum packet size.
        uint8_t bInterval; // Polling interval in frames.
} __attribute__((packed)) USB_ENDPOINT_DESCRIPTOR;

#define USB_DESCRIPTOR_DEVICE                   0x01    // bDescriptorType for a Device Descriptor.
#define USB_DESCRIPTOR_CONFIGURATION            0x02    // bDescriptorType for a Configuration Descriptor.
#define USB_DESCRIPTOR_STRING                   0x03    // bDescriptorType for a String Descriptor.
#define USB_DESCRIPTOR_INTERFACE                0x04    // bDescriptorType for an Interface Descriptor.
#define USB_DESCRIPTOR_ENDPOINT                 0x05    // bDescriptorType for an Endpoint Descriptor.

#define USB_DESCRIPTOR_DEVICE_QUALIFIER         0x06    // bDescriptorType for a Device Qualifier.
#define USB_DESCRIPTOR_OTHER_SPEED              0x07    // bDescriptorType for a Other Speed Configuration.
#define USB_DESCRIPTOR_INTERFACE_POWER          0x08    // bDescriptorType for Interface Power.
#define USB_DESCRIPTOR_OTG                      0x09    // bDescriptorType for an OTG Descriptor.

int sceUsbdInit(SceUID *uid);
int sceUsbdEnd(SceUID uid);

int sceUsbdGetDeviceList(SceUID uid, unsigned int num, SceUsbdDeviceInfo *info); // 8 devices max
// returns all descriptors (device, config, interface, etc.)
int sceUsbdGetDescriptor(SceUID uid, unsigned int device_id, unsigned char *descriptor, unsigned int size);
int sceUsbdGetDescriptorSize(SceUID uid, unsigned int device_id);
// 0, 1, 2 = Low, Full, High
int sceUsbdGetDeviceSpeed(SceUID uid, unsigned int device_id, unsigned int *speed);

#endif /* _USBD_H_ */
